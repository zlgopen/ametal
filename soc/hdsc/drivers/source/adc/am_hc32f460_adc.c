/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief ADC驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-17  cds, first implementation
 * \endinternal
 */

#include "am_hc32f460_adc.h"
#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "hw/amhw_hc32f460_rcc.h"
#include "hw/amhw_hc32f460_adc.h"
#include "hc32f460_irq_handle.h"
#include "am_hc32f460.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __ADC_HW_DECL(p_hw_adc, p_drv)   \
            amhw_hc32f460_adc_t *p_hw_adc =   \
            ((am_hc32f460_adc_dev_t *)p_drv)->p_devinfo->p_hw_adc

#define __ADC_VREF_GET(p_drv)  (((am_hc32f460_adc_dev_t *)p_drv)->p_devinfo->vref)
#define __ADC_BITS_GET(p_drv)  (((am_hc32f460_adc_dev_t *)p_drv)->p_devinfo->bits_mode)

#define AMHW_HC32F460_ADC1_INT_VSSEL_BITS_MASK    (0xF)
#define AMHW_HC32F460_ADC2_INT_VSSEL_BITS_MASK    (0xF << 4)

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 启动ADC转换 */
static int __fn_adc_start (void                 *p_drv,
                           int                   chan,
                           am_adc_buf_desc_t    *p_desc,
                           uint32_t              desc_num,
                           uint32_t              count,
                           uint32_t              flags,
                           am_adc_seq_cb_t       pfn_callback,
                           void                  *p_arg);

/** \brief 停止转换 */
static int __fn_adc_stop (void *p_drv, int chan);

/** \brief 获取ADC的采样率 */
static int __fn_adc_rate_get (void          *p_drv,
                              int            chan,
                              uint32_t      *p_rate);

/** \brief 设置ADC的采样率，实际采样率可能存在差异 */
static int __fn_adc_rate_set (void         *p_drv,
                              int           chan,
                              uint32_t      rate);

/** \brief 获取ADC转换精度 */
static uint32_t __fn_bits_get (void *p_drv, int chan);

/** \brief 获取ADC参考电压 */
static uint32_t __fn_vref_get (void *p_drv, int chan);

/**
 * \brief ADC服务函数
 */
static const struct am_adc_drv_funcs __g_adc_drvfuncs = {
    __fn_adc_start,
    __fn_adc_stop,
    __fn_adc_rate_get,
    __fn_adc_rate_set,
    __fn_bits_get,
    __fn_vref_get
};

/******************************************************************************/

/**
 * \brief ADC数据转换完成中断
 */
void hc32f460_adc_irq_handle (void *p_arg)
{
    am_hc32f460_adc_dev_t    *p_dev      = (am_hc32f460_adc_dev_t *)p_arg;
    amhw_hc32f460_adc_t      *p_hw_adc   =  NULL;

    /* 当前转换的序列描述符 */
    am_adc_buf_desc_t *p_desc  = &(p_dev->p_desc[p_dev->desc_index]);
    uint16_t          *p_buf16 = (uint16_t *)p_desc->p_buf;
    uint16_t           adc_dat = 0;

    if ( p_buf16 == NULL) {
        return ;
    }

    p_hw_adc =  (amhw_hc32f460_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    /* 判断是否当前数据是有效的 */
    if (p_dev->conv_cnt < p_desc->length) {

        /* 以清除转换结束中断标志 */
        amhw_hc32f460_adc_int_flag_clr(p_hw_adc,
                                       AMHW_HC32F460_ADC_INT_EOCAF);

        /* 读取数据 */
        adc_dat = amhw_hc32f460_adc_ch_data_get (p_hw_adc,
                                                 p_dev->chan,
                                                 p_dev->p_devinfo->bits_mode,
                                                 AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

        /* 保存数据 */
        if (AM_ADC_DATA_ALIGN_LEFT & p_dev->flags) {
            p_buf16[p_dev->conv_cnt] = adc_dat << (16 - p_dev->left_bit);
        } else {
            p_buf16[p_dev->conv_cnt] = adc_dat;
        }

        p_dev->conv_cnt++;

        /* 判断当前序列描述符是否已经完成转换 */
        if (p_dev->conv_cnt >= p_desc->length) {

            p_dev->conv_cnt = 0;
            if (NULL != p_desc->pfn_complete) {
                p_desc->pfn_complete(p_desc->p_arg, AM_OK);
            }

            p_dev->desc_index++;

            /** \brief 判断整个序列描述符是否完成一轮转换 */
            if (p_dev->desc_index == p_dev->desc_num) {

                p_dev->desc_index = 0;
                if (NULL != p_dev->pfn_callback) {
                    p_dev->pfn_callback(p_dev->p_arg, AM_OK);
                }

                p_dev->seq_cnt++; /* 整个序列转换完成 */
                if (p_dev->count != 0 && p_dev->seq_cnt == p_dev->count) {
                    p_dev->seq_cnt = 0;

                    __fn_adc_stop (p_dev, p_dev->chan);  /* 关闭模块 */

                    return ;
                }
            }
        } else {

            /* 单次转换启动 */
            amhw_hc32f460_adc_convert_start(p_hw_adc);

        }

    } else {
        if (NULL != p_dev->pfn_callback) {
            p_dev->pfn_callback(p_dev->p_arg, AM_ERROR);
        }

        __fn_adc_stop (p_dev, p_dev->chan);
    }
}

/** \brief 指向ADC中断连接函数 */
static int __fn_adc_connect (void *p_drv)
{
    am_hc32f460_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_hc32f460_adc_dev_t *)p_drv;

    /* 连接中断函数 */
    if (p_dev->p_devinfo->dev_id == 1) {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum,
                                              AMHW_HC32F460_ADC1_INT_VSSEL_BITS_MASK);
    } else if (p_dev->p_devinfo->dev_id == 2){
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum,
                                              AMHW_HC32F460_ADC2_INT_VSSEL_BITS_MASK);
    }

    am_int_connect(p_dev->p_devinfo->inum,
                   IRQ142_Handler,
                   (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief ADC 使用中断模式时启动配置
 */
static void __adc_int_work_startup (am_hc32f460_adc_dev_t       *p_dev,
                                    uint8_t                      chan)
{
    amhw_hc32f460_adc_t *p_hw_adc = (amhw_hc32f460_adc_t *)
                                    (p_dev->p_devinfo->adc_reg_base);

    if (p_dev->p_devinfo->p_ioinfo != NULL) {

        /* 链接ADC采样通道和引脚 */
        amhw_hc32f460_adc_channel_mux_set(p_hw_adc,
                                          chan,
                                          p_dev->p_devinfo->p_ioinfo[chan].chan_sel);

        /* 设置转换通道 */
        amhw_hc32f460_adc_seq_a_channel_sel0(p_hw_adc, chan);

        /* GPIO引脚模式设置 */
        am_gpio_pin_cfg(p_dev->p_devinfo->p_ioinfo[chan].gpio,
                        p_dev->p_devinfo->p_ioinfo[chan].func);


        /* 设置数据对齐方式为右对齐 */
        amhw_hc32f460_adc_data_align_set(p_hw_adc,
                                         AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

        /* 中断使能*/
        amhw_hc32f460_adc_int_enable (p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);

        /* 清标志位 */
        amhw_hc32f460_adc_int_flag_clr(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);

        /* 单次转换启动 */
        amhw_hc32f460_adc_convert_start(p_hw_adc);
    }
}

/**
 * \brief 启动ADC转换
 */
static int __fn_adc_start (void                *p_drv,
                           int                  chan,
                           am_adc_buf_desc_t   *p_desc,
                           uint32_t             desc_num,
                           uint32_t             count,
                           uint32_t             flags,
                           am_adc_seq_cb_t      pfn_callback,
                           void                *p_arg)
{
    am_hc32f460_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_hc32f460_adc_dev_t *)p_drv;

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

    /* 设置采样率 */
    __fn_adc_rate_set(p_dev, chan, 170000);

    __adc_int_work_startup(p_dev, chan);/* 中断工作模式启动配置 */

    return AM_OK;
}

/**
 * \brief 停止转换
 */
static int __fn_adc_stop (void *p_drv, int chan)
{
    am_hc32f460_adc_dev_t *p_dev    = (am_hc32f460_adc_dev_t *)p_drv;
    amhw_hc32f460_adc_t   *p_hw_adc = (amhw_hc32f460_adc_t *)
                                      (p_dev->p_devinfo->adc_reg_base);

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }
    /* 单次转换停止 */
    amhw_hc32f460_adc_convert_stop(p_hw_adc);

    /* ADC中断禁能 */
    amhw_hc32f460_adc_all_int_disable(p_hw_adc);

    if(p_dev->p_devinfo->p_ioinfo != NULL) {

        /* 恢复ADC通道默认映射类型 */
        amhw_hc32f460_adc_channel_mux_set (p_hw_adc,
                                           chan,
                                           p_dev->p_devinfo->p_ioinfo[chan].d_chan_sel);
    }

    return AM_OK;
}

/**
 * \brief 获取ADC的采样率
 */
static int __fn_adc_rate_get (void          *p_drv,
                              int            chan,
                              uint32_t      *p_rate)
{
    am_hc32f460_adc_dev_t *p_dev;
    amhw_hc32f460_adc_t   *p_hw_adc;
    
    uint32_t i;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_hc32f460_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_hc32f460_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    i = amhw_hc32f460_adc_sampling_period_get(p_hw_adc, chan);

    *p_rate = am_clk_rate_get(p_dev->p_devinfo->clk_num) / i;

    return AM_OK;
}

/**
 * \brief 设置ADC的采样率，实际采样率可能存在差异
 */
static int __fn_adc_rate_set (void          *p_drv,
                              int            chan,
                              uint32_t       rate)
{
    uint32_t   clk;
    uint32_t   temp_rate = 0;
    uint8_t    i = 0;

    uint32_t   rate_high = 0;
    uint32_t   rate_low  = 0;
    uint32_t   rate_err  = 0;

    struct adc_err  {
      uint32_t err;
      uint8_t  sample_time;
    };

    struct adc_err       adc_rate_err;
    am_hc32f460_adc_dev_t *p_dev;
    amhw_hc32f460_adc_t   *p_hw_adc;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_hc32f460_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_hc32f460_adc_t *)(p_dev->p_devinfo->adc_reg_base);
    
    adc_rate_err.err         = rate;
    adc_rate_err.sample_time = 0;

    clk       = am_clk_rate_get(p_dev->p_devinfo->clk_num);

    rate_high = clk / (5);
    rate_low  = clk / (255);

    if (rate < rate_low || rate > rate_high) {
        return -AM_EINVAL;
    }

    for(i = 5; i < 256; i++) {

        temp_rate = (clk / i);
        rate_err = temp_rate >= rate ? temp_rate - rate : rate - temp_rate;
        if (rate_err < adc_rate_err.err) {
              adc_rate_err.err         = rate_err;
              adc_rate_err.sample_time = i;
         }

         if (adc_rate_err.err == 0) {
              break;
         }
    }

    /* 设置转换周期 */
    amhw_hc32f460_adc_sampling_period_set(p_hw_adc,
                                          p_dev->chan,
                                          adc_rate_err.sample_time);

    return AM_OK;
}

/**
 * \brief 获取ADC转换精度
 */
static uint32_t __fn_bits_get (void *p_drv, int chan)
{
    if (NULL == p_drv) {
        return 0;   /* 资源已经释放，精度未知 */
    }

    return (uint32_t)__ADC_BITS_GET(p_drv);
}

/**
 * \brief 获取ADC参考电压
 */
static uint32_t __fn_vref_get (void *p_drv, int chan)
{
    if (NULL == p_drv) {
        return 0;   /* 资源已经释放，参考电压未知 */
    }

    return (uint32_t)__ADC_VREF_GET(p_drv);
}

/**
 * \brief ADC初始化
 */
am_adc_handle_t am_hc32f460_adc_init (am_hc32f460_adc_dev_t           *p_dev,
                                     const am_hc32f460_adc_devinfo_t  *p_devinfo)
{
    uint16_t bit = 0;

    amhw_hc32f460_adc_t   *p_hw_adc = NULL;

    if (NULL == p_devinfo || NULL == p_dev ) {
        return NULL;
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

    /* 时钟使能 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_hw_adc = (amhw_hc32f460_adc_t *)(p_dev->p_devinfo->adc_reg_base);


    /* 停止ADC转换 */
    amhw_hc32f460_adc_convert_stop(p_hw_adc);

    /* 禁止数据寄存器自动清除 */
    amhw_hc32f460_adc_data_autoclr_disable(p_hw_adc);

    /* 转换模式为序列A单次转换 */
    amhw_hc32f460_adc_mode_set(p_hw_adc, AMHW_HC32F460_ADC_SEQ_A_ONCE);

    /* 设置数据对齐方式为右对齐 */
    amhw_hc32f460_adc_data_align_set(p_hw_adc,
                                     AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    /* 有效位数获取 */
    bit = __fn_bits_get(p_hw_adc, 0);

    /* 分辨率设置 */
    switch (bit) {

    case 12:
        amhw_hc32f460_adc_resolution_set(p_hw_adc, AMHW_HC32F460_ADC_RESOLUTION_12BIT);
        break;
    case 10:
        amhw_hc32f460_adc_resolution_set(p_hw_adc, AMHW_HC32F460_ADC_RESOLUTION_10BIT);
        break;
    case 8:
        amhw_hc32f460_adc_resolution_set(p_hw_adc, AMHW_HC32F460_ADC_RESOLUTION_8BIT);
        break;
    }

    /* 数据左对齐时左移位数 */
    p_dev->left_bit = 12 - bit;

    /* 注册ADC相关中断 */
    __fn_adc_connect(p_dev);

    return (am_adc_handle_t)(&(p_dev->adc_serve));
}

/**
 * \brief ADC去初始化
 */
void am_hc32f460_adc_deinit (am_adc_handle_t handle)
{
    am_hc32f460_adc_dev_t *p_dev    = (am_hc32f460_adc_dev_t *)handle;
    amhw_hc32f460_adc_t   *p_hw_adc = NULL;

    if (NULL == p_dev) {
        return ;
    }

    p_hw_adc = (amhw_hc32f460_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    /* 删除共享中断 */
    if (p_dev->p_devinfo->dev_id == 1) {
        amhw_hc32f460_intc_int_vssel_bits_clr(p_dev->p_devinfo->inum,
                                              AMHW_HC32F460_ADC1_INT_VSSEL_BITS_MASK);
    } else if (p_dev->p_devinfo->dev_id == 2){
        amhw_hc32f460_intc_int_vssel_bits_clr(p_dev->p_devinfo->inum,
                                              AMHW_HC32F460_ADC2_INT_VSSEL_BITS_MASK);
    }

    /* 如果该中断号的所有共享中断位都没有被使能，则删除中断连接，禁能中断 */
    if (amhw_hc32f460_intc_int_vssel_get(p_dev->p_devinfo->inum) == 0) {
        am_int_disconnect(p_dev->p_devinfo->inum,
                       IRQ142_Handler,
                       (void *)p_dev);
        am_int_disable(p_dev->p_devinfo->inum);
    }

    /* ADC中断禁能 */
    amhw_hc32f460_adc_all_int_disable(p_hw_adc);

    p_dev->adc_serve.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
