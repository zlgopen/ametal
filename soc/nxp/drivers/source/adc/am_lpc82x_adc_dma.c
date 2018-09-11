/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.03 18-04-24  pea, update standard interface
 * - 1.02 15-12-14  hgo, fix inaccurate samplerate and always generate interrput
 * - 1.01 15-11-25  hgo, modified bug when count=0
 * - 1.00 15-07-15  bob, first implementation
 * \endinternal
 */
#include "am_lpc82x_adc_dma.h"
#include "am_int.h"
#include "am_clk.h"
#include "hw/amhw_lpc82x_inmux.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_adc.h"
/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __ADC_HW_DECL(p_hw_adc, p_drv)    \
        amhw_lpc82x_adc_t *p_hw_adc =            \
            ((am_lpc82x_adc_dma_dev_t *)p_drv)->p_devinfo->p_hw_adc

#define __ADC_VREF_GET(p_drv)             \
        (((am_lpc82x_adc_dma_dev_t *)p_drv)->p_devinfo->vref)


/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief ADC DMA传输完成中断函数 */
static void __adc_dma_int(void *p_arg, int state);

/** \brief ADC 使用DMA模式时启动配置 */
static int __adc_dma_work_startup(am_lpc82x_adc_dma_dev_t *p_dev,
                                  uint32_t                 desc_num);

/** \brief 启动ADC转换        */
static int __pfn_adc_start  (void                   *p_drv,
                             int                     chan,
                             am_adc_buf_desc_t      *p_desc,
                             uint32_t                desc_num,
                             uint32_t                count,
                             uint32_t                flags,
                             am_adc_seq_cb_t         pfn_callback,
                             void                   *p_arg);
 
/** \brief 停止转换           */
static int __pfn_adc_stop (void *p_drv, int chan);
                      
/** \brief 获取ADC的采样率    */
static int __pfn_rate_get (void       *p_drv,
                           int         chan,
                           uint32_t   *p_rate);

/** \brief 设置ADC的采样率，实际采样率可能存在差异 */
static int __pfn_rate_set (void     *p_drv,
                           int       chan,
                           uint32_t  rate);

/** \brief 获取ADC转换精度 */
static uint32_t __pfn_bits_get(void *p_drv, int chan);

/** \brief 获取ADC参考电压 */
static uint32_t __pfn_vref_get(void *p_drv, int chan);



/**
 * \brief ADC服务函数 
 */
static const struct am_adc_drv_funcs __g_adc_drvfuncs = {
    __pfn_adc_start,
    __pfn_adc_stop,
    __pfn_rate_get,
    __pfn_rate_set,
    __pfn_bits_get,
    __pfn_vref_get
};

/******************************************************************************/

/**
 * \brief ADC DMA
 */
static void __adc_dma_int(void *p_arg, int state)
{
    am_lpc82x_adc_dma_dev_t *p_dev    = (am_lpc82x_adc_dma_dev_t *)p_arg;
    amhw_lpc82x_adc_t       *p_hw_adc = NULL;

    uint32_t       buf_pos = 0;
    uint32_t       i;

    uint16_t      *p_buf = NULL;

    /* 错误中断 */
    if ( state & AM_LPC82X_DMA_STAT_INTERR) {

        __pfn_adc_stop (p_dev, p_dev->chan);

        if (NULL != p_dev->pfn_callback) {
            p_dev->pfn_callback(p_dev->p_arg, AM_ERROR);
        }
        return;
    }

    /* 判断采样率是否过高 */
    if ((state & AM_LPC82X_DMA_STAT_INTA) &&
        (state & AM_LPC82X_DMA_STAT_INTB)) {

        __pfn_adc_stop (p_dev, p_dev->chan);

        if (NULL != p_dev->p_desc[0].pfn_complete) {
            p_dev->p_desc[0].pfn_complete(p_dev->p_desc[0].p_arg, AM_ERROR);
        }

        if (NULL != p_dev->p_desc[1].pfn_complete) {
            p_dev->p_desc[1].pfn_complete(p_dev->p_desc[1].p_arg, AM_ERROR);
        }
        return;

    } else if (state & AM_LPC82X_DMA_STAT_INTA) {
        buf_pos = 0;
    } else if (state & AM_LPC82X_DMA_STAT_INTB) {
        buf_pos = 1;
    }

    /* 数据格式处理 */
    if (AM_ADC_DATA_ALIGN_RIGHT == p_dev->flags) {

        p_buf = (uint16_t *)(p_dev->p_desc[buf_pos].p_buf);

        for (i = 0; i < p_dev->p_desc[buf_pos].length; i++ ) {
            p_buf[i] = p_buf[i] >> 4;
        }
    }

    /* 描述符回调函数  */
    if (NULL != p_dev->p_desc[buf_pos].pfn_complete) {
        p_dev->p_desc[buf_pos].pfn_complete(p_dev->p_desc[buf_pos].p_arg, 
                                            AM_OK);
    }

    /* 序列完成调用系列完成中断 */
    if ((++p_dev->desc_index) == p_dev->desc_num) {

        p_dev->desc_index = 0;
        p_dev->seq_cnt++;

        if (p_dev->count != 0 && p_dev->seq_cnt >= p_dev->count) {

            /* 达到采样次数且不连续 */
            p_dev->seq_cnt = 0;
            __pfn_adc_stop (p_dev, p_dev->chan);

        } else if (1 == p_dev->desc_num) {

            p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

            {
                am_lpc82x_dma_controller_abort(p_dev->p_dma_ctr);   /* 停止DMA传输 */
                am_lpc82x_dma_controller_release(p_dev->p_dma_ctr); /* 释放DMA资源 */

                /* 停止当前传输，并禁能序列A */
                amhw_lpc82x_adc_seq_stop   (p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);
                amhw_lpc82x_adc_seq_disable(p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);

                /* 禁能序列A中断 */
                amhw_lpc82x_adc_int_disable(p_hw_adc,
                                            AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE);
            }

            {
                amhw_lpc82x_adc_int_enable(p_hw_adc,
                                           AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE);

                __adc_dma_work_startup(p_dev, p_dev->desc_num);
            }
        }

        if (NULL != p_dev->pfn_callback) {
            p_dev->pfn_callback(p_dev->p_arg, AM_OK);
        }
    }
}

/**
 * \brief ADC 使用DMA模式时启动配置
 */
static int __adc_dma_work_startup(am_lpc82x_adc_dma_dev_t *p_dev, uint32_t desc_num)
{
    uint32_t        dma_aflags = 0, dma_bflags = 0;
    uint32_t        seq_flags  = 0;
    
    am_lpc82x_dma_controller_t *p_dma_ctr = NULL;
    amhw_lpc82x_adc_t          *p_hw_adc  =  NULL;

    uint32_t trans_width_flag, trans_byte_one;

    p_hw_adc =  (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    p_dma_ctr = am_lpc82x_dma_controller_get(p_dev->p_devinfo->dma_chan,
                                             /* 通道优先级为1 */
                                             DMA_CHAN_OPT_PRIO_1             |
                                             /* 上升沿触发 */
                                             DMA_CHAN_OPT_HWTRIG_MODE_RISE   |
                                             /* 上升沿触发 */
                                             DMA_CHAN_OPT_HWTRIG_BURST_EN    |
                                             /* 突发传输大小为1 */
                                             DMA_CHAN_OPT_HWTRIG_BURST_1     |
                                             /* ADC0序列A中断触发DMA传输 */
                                             DMA_CHAN_OPT_HWTRIG_SRC_ADC0_SEQA_IRQ);

    if (p_dma_ctr == NULL) {
        p_dev->p_dma_ctr = NULL;
        return -AM_EPERM;
    }
    p_dev->p_dma_ctr = p_dma_ctr;

    /* 传输宽度配置 */
    trans_byte_one   = 2;
    trans_width_flag = AM_LPC82X_DMA_XFER_WIDTH_16BIT;

    /* DMA传输配置 */
    dma_aflags = AM_LPC82X_DMA_XFER_VALID           |  /* 当前通道描述符有效 */
                 AM_LPC82X_DMA_XFER_RELOAD          |  /* 传输完重载通道描述符 */
                 trans_width_flag                   |  /* 传输数据宽度 */
                 AM_LPC82X_DMA_XFER_SRCINC_NOINC    |  /* 源端数据地址不递增 */
                 AM_LPC82X_DMA_XFER_DSTINC_1X       |  /* 目标地址按1x宽度递增 */
                 AM_LPC82X_DMA_XFER_SETINTA;           /* 使能中断 A */

    /* 建立通道描述符A */
    am_lpc82x_dma_xfer_desc_build(&p_dev->dma_desc[0],                      /* A的重载描述符 */
                                  (uint32_t)(&p_hw_adc->dat[p_dev->chan]),  /* 源端数据地址  */
                                  (uint32_t)p_dev->p_desc[0].p_buf,         /* 缓冲区地址  */
                                  p_dev->p_desc[0].length * trans_byte_one, /* 传输字节数  */
                                  dma_aflags);                              /* 传输配置    */

    if (desc_num == 2) {

        /* DMA传输配置 */
        dma_bflags = AM_LPC82X_DMA_XFER_VALID           |  /* 当前通道描述符有效  */
                     AM_LPC82X_DMA_XFER_RELOAD          |  /* 传输完重载通道描述符 */
                     trans_width_flag                   |  /* 传输数据宽度 */
                     AM_LPC82X_DMA_XFER_SRCINC_NOINC    |  /* 源端数据地址不递增   */
                     AM_LPC82X_DMA_XFER_DSTINC_1X       |  /* 目标地址按1x宽度递增 */
                     AM_LPC82X_DMA_XFER_SETINTB;           /* 使能中断 B */

        /* 建立通道描述符B */
        am_lpc82x_dma_xfer_desc_build(&p_dev->dma_desc[1],                      /* B的重载描述符 */
                                      (uint32_t)(&p_hw_adc->dat[p_dev->chan]),  /* 源端数据地址  */
                                      (uint32_t)p_dev->p_desc[1].p_buf,         /* 缓冲区地址  */
                                      p_dev->p_desc[1].length * trans_byte_one, /* 传输字节数  */
                                      dma_bflags);                              /* 传输配置    */

        am_lpc82x_dma_xfer_desc_link(&p_dev->dma_desc[0], &p_dev->dma_desc[1]);
        am_lpc82x_dma_xfer_desc_link(&p_dev->dma_desc[1], &p_dev->dma_desc[0]);
    }

    /* 启动DMA传输，马上开始传输 */
    am_lpc82x_dma_xfer_desc_startup(p_dma_ctr,
                                    &p_dev->dma_desc[0],
                                    __adc_dma_int,
                                    (void *)p_dev);

    seq_flags = AMHW_LPC82X_ADC_SEQ_CTRL_MODE_BURST   |          /* 采用突发转换模式 */
                AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_POL_POS |          /* 使用正边沿触发方式 */
                AMHW_LPC82X_ADC_SEQ_CTRL_MODE_EOS     |          /* 序列转换完后触发DMA */
                AMHW_LPC82X_ADC_SEQ_CTRL_ENA          |          /* 序列A使能          */
                AMHW_LPC82X_ADC_SEQ_CTRL_ENABLE_CH(p_dev->chan); /* 使能ADC通道    */

    /* ADC序列A配置(ADC使用DMA传输，需先启动DMA传输，再启动ADC转换)  */
    amhw_lpc82x_adc_seq_config(p_hw_adc,
                               AMHW_LPC82X_ADC_SEQ_A,
                               seq_flags);

    return AM_OK;
}
/**
 * \brief 启动ADC转换
 */
static int __pfn_adc_start  (void                   *p_drv,
                             int                     chan,
                             am_adc_buf_desc_t      *p_desc,
                             uint32_t                desc_num,
                             uint32_t                count,
                             uint32_t                flags,
                             am_adc_seq_cb_t         pfn_callback,
                             void                   *p_arg)
{
    am_lpc82x_adc_dma_dev_t *p_dev = NULL;
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_drv || NULL == p_desc || 0 == desc_num) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc82x_adc_dma_dev_t *)p_drv;
    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    if (AM_TRUE != p_dev->is_idle) {
        return -AM_EBUSY;
    }

    p_dev->is_idle      = AM_FALSE;
    p_dev->p_desc       = p_desc;
    p_dev->chan         = chan;
    p_dev->desc_num     = desc_num;
    p_dev->count        = count;
    p_dev->flags        = flags;
    p_dev->pfn_callback = pfn_callback;
    p_dev->p_arg        = p_arg;
    p_dev->seq_cnt      = 0;
    p_dev->desc_index   = 0;
    p_dev->conv_cnt     = 0;

    /* 使能序列A中断 */
    amhw_lpc82x_adc_int_enable(p_hw_adc,
                               AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE);

   {
        /* DMA工作模式下，描述符数量最大支持2个 */
        if (NULL == p_drv || desc_num > 2) {
            return -AM_EINVAL;
        }
        /* DMA启动配置 */
        if (-AM_EPERM == __adc_dma_work_startup(p_dev, desc_num)) {
            return -AM_EINVAL;
        }
    }

    return AM_OK;
}
 
/**
 * \brief 停止转换
 */
static int __pfn_adc_stop (void *p_drv, int chan)
{
    am_lpc82x_adc_dma_dev_t *p_dev    = NULL;
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc82x_adc_dma_dev_t *)p_drv;
    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    {
        am_lpc82x_dma_controller_abort(p_dev->p_dma_ctr);   /* 停止DMA传输 */
        am_lpc82x_dma_controller_release(p_dev->p_dma_ctr); /* 释放DMA资源 */
    }
    /* 停止当前传输，并禁能序列A */
    amhw_lpc82x_adc_seq_stop   (p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);
    amhw_lpc82x_adc_seq_disable(p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);

    /* 禁能序列A中断 */
    amhw_lpc82x_adc_int_disable(p_hw_adc,
                               AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE);

    p_dev->is_idle = AM_TRUE;

    return AM_OK;
}

/**
 * \brief 获取ADC的采样率
 */
static int __pfn_rate_get (void       *p_drv,
                           int         chan,
                           uint32_t   *p_rate)
{
    uint8_t              adc_clkdiv;
    uint32_t             sys_clk;
    am_lpc82x_adc_dma_dev_t *p_dev    = NULL;
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_drv || NULL == p_rate) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc82x_adc_dma_dev_t *)p_drv;
    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    sys_clk  = am_clk_rate_get(p_dev->p_devinfo->clk_id);

    /* 得到时钟分频系数 */
    adc_clkdiv = amhw_lpc82x_adc_clkdiv_get(p_hw_adc);

    /* 得到分频值 */
    adc_clkdiv = (adc_clkdiv == 255) ? 255 : (adc_clkdiv + 1);

    /* LPC824中频率为采样率的25倍 */
    *p_rate = sys_clk / adc_clkdiv / 25;

    return AM_OK;
}

/**
 * \brief 设置ADC的采样率，实际采样率可能存在差异
 */
static int __pfn_rate_set (void     *p_drv,
                           int       chan,
                           uint32_t  rate)
{
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;
    am_lpc82x_adc_dma_dev_t *p_dev = NULL;
    uint32_t             adc_clkdiv;
    uint32_t             sys_clk;

    if (NULL == p_drv || 0 == rate || 1200000 < rate) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc82x_adc_dma_dev_t *)p_drv;
    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    sys_clk = am_clk_rate_get(p_dev->p_devinfo->clk_id);

    /* LPC824中频率为采样率的25倍 */
    adc_clkdiv = sys_clk / rate / 25;
    
    /* 采样率太低或太高时，设置到极限 */
    if (adc_clkdiv != 0) {
        adc_clkdiv = adc_clkdiv > 256 ? 255 : adc_clkdiv - 1; 
    }
    
    amhw_lpc82x_adc_clkdiv_set (p_hw_adc, adc_clkdiv);

    return AM_OK;

}

/**
 * \brief 获取ADC转换精度
 */
static uint32_t __pfn_bits_get (void *p_drv, int chan)
{
    return (uint32_t)12;
}

/**
 * \brief 获取ADC参考电压
 */
static uint32_t __pfn_vref_get (void *p_drv, int chan)
{
    if (NULL == p_drv) {
        return 0;   /* 资源已经释放，参考电压未知 */
    }

    return (uint32_t)__ADC_VREF_GET(p_drv);
}

/**
 * \brief ADC初始化
 */
am_adc_handle_t am_lpc82x_adc_dma_init (am_lpc82x_adc_dma_dev_t     *p_dev,
                              const am_lpc82x_adc_dma_devinfo_t *p_devinfo)
{

    if (NULL == p_devinfo || NULL == p_dev ) {
        return NULL;
    }
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }
    
    p_dev->p_devinfo         = p_devinfo;
    p_dev->adc_serve.p_funcs = &__g_adc_drvfuncs;
    p_dev->adc_serve.p_drv   = p_dev;

    p_dev->pfn_callback      = NULL;
    p_dev->p_desc            = NULL;
    p_dev->p_arg             = NULL;
    p_dev->desc_num          = 0;
    p_dev->flags             = 0;
    p_dev->count             = 0;
    p_dev->chan              = 0;
    p_dev->seq_cnt           = 0;
    p_dev->desc_index        = 0;
    p_dev->conv_cnt          = 0;
    p_dev->is_idle           = AM_TRUE;

    return (am_adc_handle_t)(&(p_dev->adc_serve));
}

/**
 * \brief ADC去初始化
 */
void am_lpc82x_adc_dma_deinit (am_adc_handle_t handle)
{
    am_lpc82x_adc_dma_dev_t *p_dev    = (am_lpc82x_adc_dma_dev_t *)handle;
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_dev) {
        return ;
    }

    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);
    
    /* 停止当前正在进行的转换，并禁止序列转换 */
    amhw_lpc82x_adc_seq_stop   (p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);
    amhw_lpc82x_adc_seq_disable(p_hw_adc,
                                AMHW_LPC82X_ADC_SEQ_A);

    /* 禁能序列A中断 */
    amhw_lpc82x_adc_int_disable(p_hw_adc,
                               AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE);

    {
        am_lpc82x_dma_controller_abort(p_dev->p_dma_ctr);   /* 停止DMA传输 */
        am_lpc82x_dma_controller_release(p_dev->p_dma_ctr); /* 释放DMA资源 */
    }

    p_dev->adc_serve.p_drv = NULL;
    
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}
/* end of file */
