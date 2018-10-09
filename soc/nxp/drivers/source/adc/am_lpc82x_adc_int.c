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
#include "am_lpc82x_adc_int.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_inmux.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_adc.h"
/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __ADC_HW_DECL(p_hw_adc, p_drv)    \
        amhw_lpc82x_adc_t *p_hw_adc =            \
            ((am_lpc82x_adc_int_dev_t *)p_drv)->p_devinfo->p_hw_adc

#define __ADC_VREF_GET(p_drv)             \
        (((am_lpc82x_adc_int_dev_t *)p_drv)->p_devinfo->vref)


/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief ADC数据溢出中断函数 */
static void __adc_ovr_int(void *p_arg);



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
 * \brief ADC数据溢出中断
 */
static void __adc_ovr_int(void *p_arg)
{
    am_lpc82x_adc_int_dev_t *p_dev    = (am_lpc82x_adc_int_dev_t *)p_arg;
    amhw_lpc82x_adc_t       *p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    amhw_lpc82x_adc_chan_data_get(p_hw_adc, p_dev->chan);

    /* 从全局数据寄存器中读取数据，以清除中断标志 */
    amhw_lpc82x_adc_glo_data_get (p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);

    __pfn_adc_stop (p_dev, p_dev->chan);

    /* 溢出返回错误                 */
    if (NULL != p_dev->pfn_callback) {
        p_dev->pfn_callback(p_dev->p_arg, AM_ERROR);
    }
}

/**
 * \brief ADC数据转换完成中断
 */

void __adc_int(void *p_arg)
{
    am_lpc82x_adc_int_dev_t    *p_dev      = (am_lpc82x_adc_int_dev_t *)p_arg;
    amhw_lpc82x_adc_t      *p_hw_adc   =  NULL;

    /* 当前转换的序列描述符 */
    uint32_t           desc_index = p_dev->desc_index;
    am_adc_buf_desc_t *p_desc     = &(p_dev->p_desc[desc_index]);
    uint16_t           adc_dat    = 0;

    p_hw_adc =  (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    /* 判断是否当前数据是有效的 */
    if (p_dev->conv_cnt < p_desc->length) {
        /* 从全局数据寄存器中读取数据，以清除中断标志 */
        adc_dat= amhw_lpc82x_adc_glo_data_get (p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);
        /* 保存数据 */
        if (AM_ADC_DATA_ALIGN_RIGHT == p_dev->flags) {
            ((uint16_t *)p_desc->p_buf)[p_dev->conv_cnt] = (adc_dat &
                                                            0x0000FFF0) >> 4;
        } else {
            ((uint16_t *)p_desc->p_buf)[p_dev->conv_cnt] = (adc_dat &
                                                            0x0000FFF0);
        }

        p_dev->conv_cnt++;
        /*
         * 判断当前序列中一个描述符是否已经完成转换。注意，一个描述符的缓冲区具有很多个
         */
        if (p_dev->conv_cnt == p_desc->length) {

            p_dev->conv_cnt = 0;
            if (NULL != p_desc->pfn_complete) {
                p_desc->pfn_complete(p_desc->p_arg, AM_OK);
            }

            p_dev->desc_index++;
            /* 判断整个序列描述符是否完成一轮转换 */
            if (p_dev->desc_index == p_dev->desc_num) {

                p_dev->desc_index = 0;
                p_dev->seq_cnt++;

                if (p_dev->count != 0 && p_dev->seq_cnt >= p_dev->count) {

                    p_dev->seq_cnt = 0;
                    __pfn_adc_stop (p_dev, p_dev->chan);  /* 关闭模块 */
                }

                if (NULL != p_dev->pfn_callback) {
                    p_dev->pfn_callback(p_dev->p_arg, AM_OK);
                }
            }
        }
    } else {

        __pfn_adc_stop (p_dev, p_dev->chan);

        if (NULL != p_dev->pfn_callback) {
            p_dev->pfn_callback(p_dev->p_arg, AM_ERROR);
        }
    }
}



/** \brief 指向ADC中断连接函数 */
static int __pfn_adc_connect (void *p_drv)
{
    am_lpc82x_adc_int_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_lpc82x_adc_int_dev_t *)p_drv;

   {
         /* 连接转换完成中断 */
        am_int_connect(p_dev->p_devinfo->inum_seqa,
                        __adc_int,
                       (void *)p_dev);
        am_int_enable(p_dev->p_devinfo->inum_seqa);
    }
    
    /* 连接溢出中断 */
    am_int_connect(p_dev->p_devinfo->inum_ovr,
                   __adc_ovr_int,
                  (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum_ovr);

    return AM_OK;
}

/**
 * \brief ADC 使用中断模式时启动配置
 */

static void __adc_int_work_startup(am_lpc82x_adc_int_dev_t *p_dev)
{
    uint32_t           seq_flags = 0;
    amhw_lpc82x_adc_t *p_hw_adc  = NULL;
    
    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

    seq_flags = AMHW_LPC82X_ADC_SEQ_CTRL_MODE_BURST   |  /* 采用突发转换模式   */
                AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_POL_POS |  /* 使用正边沿触发方式 */
                AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_SW      |  /* 软件触发           */
                AMHW_LPC82X_ADC_SEQ_CTRL_MODE_EOC     |  /* 通道转换完后中断   */
                AMHW_LPC82X_ADC_SEQ_CTRL_ENA          |  /* 序列A使能          */
                                                         /* 使能ADC通道        */
                AMHW_LPC82X_ADC_SEQ_CTRL_ENABLE_CH(p_dev->chan);


    /* ADC序列A配置   */
    amhw_lpc82x_adc_seq_config(p_hw_adc, AMHW_LPC82X_ADC_SEQ_A, seq_flags);
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
    am_lpc82x_adc_int_dev_t *p_dev = NULL;

    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_lpc82x_adc_int_dev_t *)p_drv;
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

    {

        /* 使能序列A中断 */
        amhw_lpc82x_adc_int_enable(p_hw_adc,
                                   AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE);

        __adc_int_work_startup(p_dev);                /* 中断工作模式启动配置 */

    }

    return AM_OK;
}
 
/**
 * \brief 停止转换
 */
static int __pfn_adc_stop (void *p_drv, int chan)
{
    am_lpc82x_adc_int_dev_t *p_dev    = NULL;
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc82x_adc_int_dev_t *)p_drv;
    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);

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
    am_lpc82x_adc_int_dev_t *p_dev    = NULL;
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_drv || NULL == p_rate) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc82x_adc_int_dev_t *)p_drv;
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
    am_lpc82x_adc_int_dev_t *p_dev = NULL;
    uint32_t             adc_clkdiv;
    uint32_t             sys_clk;

    if (NULL == p_drv || 0 == rate || 1200000 < rate) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc82x_adc_int_dev_t *)p_drv;
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
am_adc_handle_t am_lpc82x_adc_int_init (am_lpc82x_adc_int_dev_t     *p_dev,
                              const am_lpc82x_adc_int_devinfo_t *p_devinfo)
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

    /* 注册ADC相关中断 */
    __pfn_adc_connect(p_dev);

    return (am_adc_handle_t)(&(p_dev->adc_serve));
}

/**
 * \brief ADC去初始化
 */
void am_lpc82x_adc_int_deinit (am_adc_handle_t handle)
{
    am_lpc82x_adc_int_dev_t *p_dev    = (am_lpc82x_adc_int_dev_t *)handle;
    amhw_lpc82x_adc_t   *p_hw_adc = NULL;

    if (NULL == p_dev) {
        return ;
    }

    p_hw_adc = (amhw_lpc82x_adc_t *)(p_dev->p_devinfo->adc_regbase);
    am_int_disable(p_dev->p_devinfo->inum_seqa);
    am_int_disable(p_dev->p_devinfo->inum_ovr);
    
    amhw_lpc82x_adc_int_enable (p_hw_adc,
                                AMHW_LPC82X_ADC_INTEN_ALL_DISABLE);
    
    /* 停止当前正在进行的转换，并禁止序列转换 */
    amhw_lpc82x_adc_seq_stop   (p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);
    amhw_lpc82x_adc_seq_disable(p_hw_adc,
                                AMHW_LPC82X_ADC_SEQ_A);

    p_dev->adc_serve.p_drv = NULL;
    
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}
/* end of file */
