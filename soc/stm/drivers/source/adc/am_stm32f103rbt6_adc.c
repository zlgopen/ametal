/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.stm32f103rbt6.cn/
*******************************************************************************/

/**
 * \file
 * \brief ADC驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-12  ari, first implementation
 * \endinternal
 */

#include "am_stm32f103rbt6_adc.h"
#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "amhw_stm32f103rbt6_rcc.h"
#include "hw/amhw_stm32f103rbt6_adc.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __ADC_HW_DECL(p_hw_adc, p_drv)   \
            amhw_stm32f103rbt6_adc_t *p_hw_adc =   \
            ((am_stm32f103rbt6_adc_dev_t *)p_drv)->p_devinfo->p_hw_adc

#define __ADC_VREF_GET(p_drv)  (((am_stm32f103rbt6_adc_dev_t *)p_drv)->p_devinfo->vref)

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
void __stm32f103rbt6_adc_irq_handle (void *p_arg)
{
    am_stm32f103rbt6_adc_dev_t    *p_dev      = (am_stm32f103rbt6_adc_dev_t *)p_arg;
    amhw_stm32f103rbt6_adc_t      *p_hw_adc   =  NULL;

    /* 当前转换的序列描述符 */
    am_adc_buf_desc_t *p_desc  = &(p_dev->p_desc[p_dev->desc_index]);
    uint16_t          *p_buf16 = (uint16_t *)p_desc->p_buf;
    uint16_t           adc_dat = 0;

    if ( p_buf16 == NULL) {
        return ;
    }

    p_hw_adc =  (amhw_stm32f103rbt6_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    amhw_stm32f103rbt6_adc_swstart_enable(p_hw_adc);

    /* 判断是否当前数据是有效的 */
    if (p_dev->conv_cnt < p_desc->length) {

        /* 以清除转换结束中断标志 */
        amhw_stm32f103rbt6_adc_status_flag_clr(p_hw_adc,AMHW_STM32F103RBT6_ADC_END_FLAG);

        /* 读取数据 */
        adc_dat = amhw_stm32f103rbt6_adc_regular_data_get (p_hw_adc);

        /* 保存数据 */
        if (AM_ADC_DATA_ALIGN_LEFT & p_dev->flags) {
            p_buf16[p_dev->conv_cnt] = adc_dat << 4;
        } else {
            p_buf16[p_dev->conv_cnt] = adc_dat >> p_dev->right_bit;
        }

        p_dev->conv_cnt++;

        /* 判断当前序列描述符是否已经完成转换 */
        if (p_dev->conv_cnt >= p_desc->length) {

            p_dev->conv_cnt = 0;
            if (NULL != p_desc->pfn_complete) {
                p_desc->pfn_complete(p_desc->p_arg, AM_OK);
            }

            p_dev->desc_index++;

            /* 判断整个序列描述符是否完成一轮转换 */
            if (p_dev->desc_index == p_dev->desc_num) {

                p_dev->desc_index = 0;
                if (NULL != p_dev->pfn_callback) {
                    p_dev->pfn_callback(p_dev->p_arg, AM_OK);
                }

                p_dev->seq_cnt++; /* 整个序列转换完成 */
                if (p_dev->count != 0 && p_dev->seq_cnt == p_dev->count) {
                    p_dev->seq_cnt = 0;

                    amhw_stm32f103rbt6_adc_status_flag_clr(p_hw_adc,
                                                    AMHW_STM32F103RBT6_ADC_END_FLAG);
                    __fn_adc_stop (p_dev, p_dev->chan);  /* 关闭模块 */

                    return ; /* 返回 */
                }
            }
        }
    } else {
        if (NULL != p_dev->pfn_callback) {
            p_dev->pfn_callback(p_dev->p_arg, AM_ERROR);
        }
        if (amhw_stm32f103rbt6_adc_status_flag_check(
            p_hw_adc,AMHW_STM32F103RBT6_ADC_END_FLAG) == AM_TRUE) {
            amhw_stm32f103rbt6_adc_status_flag_clr(p_hw_adc,AMHW_STM32F103RBT6_ADC_END_FLAG);
        }
        __fn_adc_stop (p_dev, p_dev->chan);
    }
}

/** \brief 指向ADC中断连接函数 */
static int __fn_adc_connect (void *p_drv)
{
    am_stm32f103rbt6_adc_dev_t *p_dev = NULL;
    amhw_stm32f103rbt6_adc_t   *p_hw_adc;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_stm32f103rbt6_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_stm32f103rbt6_adc_t *)p_dev->p_devinfo->adc_reg_base;

    /* 转换结束中断使能*/
    amhw_stm32f103rbt6_adc_int_enable(p_hw_adc, AMHW_STM32F103RBT6_ADC_INT_END);

    /* 连接转换完成中断 */
    am_int_connect(p_dev->p_devinfo->inum,
                   __stm32f103rbt6_adc_irq_handle,
                   (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief ADC 使用中断模式时启动配置
 */
static void __adc_int_work_startup (am_stm32f103rbt6_adc_dev_t       *p_dev,
                                    amhw_stm32f103rbt6_adc_channel_t  chan)
{
    amhw_stm32f103rbt6_adc_t *p_hw_adc = (amhw_stm32f103rbt6_adc_t *)
                                  (p_dev->p_devinfo->adc_reg_base);

    /* ADC禁能*/
    amhw_stm32f103rbt6_adc_disable(p_hw_adc);

    /* 设置某一通道的采样时间*/
    amhw_stm32f103rbt6_adc_smpr_set(p_hw_adc,
                             AMHW_STM32F103RBT6_ADC_CHAN_ST239_5,
                             chan);

    /* 设置规则通道个数  1个 */
    amhw_stm32f103rbt6_adc_regular_channel_length_set(
        p_hw_adc,AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_1);

    /* 连接规则序列通道和ADC采样通道 */
    amhw_stm32f103rbt6_adc_regular_channel_order_set(
        p_hw_adc,
        AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_1st,
        chan);

    /* 转换结束中断使能*/
    amhw_stm32f103rbt6_adc_int_enable(p_hw_adc, AMHW_STM32F103RBT6_ADC_INT_END);

    /* ADC使能*/
    amhw_stm32f103rbt6_adc_enable(p_hw_adc);

    /* 启用复位校准*/
    amhw_stm32f103rbt6_adc_rstcal_enable(p_hw_adc);

    /* 等待复位校准结束*/
    while(amhw_stm32f103rbt6_adc_rstcal_check(p_hw_adc) == AM_FALSE);

    /* 启用AD校准*/
    amhw_stm32f103rbt6_adc_cal_enable(p_hw_adc);

    /* 等待AD校准结束*/
    while(amhw_stm32f103rbt6_adc_cal_check(p_hw_adc) == AM_FALSE);

    /* 启动转换 */
    amhw_stm32f103rbt6_adc_swstart_enable(p_hw_adc);

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
    am_stm32f103rbt6_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_stm32f103rbt6_adc_dev_t *)p_drv;

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

    am_int_enable(p_dev->p_devinfo->inum); /* 开启中断 */
    __adc_int_work_startup(p_dev,(amhw_stm32f103rbt6_adc_channel_t)chan); /* 中断工作模式启动配置 */

    return AM_OK;
}

/**
 * \brief 停止转换
 */
static int __fn_adc_stop (void *p_drv, int chan)
{
    am_stm32f103rbt6_adc_dev_t *p_dev    = (am_stm32f103rbt6_adc_dev_t *)p_drv;
    amhw_stm32f103rbt6_adc_t   *p_hw_adc = (amhw_stm32f103rbt6_adc_t *)
                                    (p_dev->p_devinfo->adc_reg_base);

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    amhw_stm32f103rbt6_adc_swstart_disable(p_hw_adc);
    amhw_stm32f103rbt6_adc_int_disable(p_hw_adc, AMHW_STM32F103RBT6_ADC_INT_END);
    amhw_stm32f103rbt6_adc_disable(p_hw_adc);

    am_int_disable(p_dev->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief 获取ADC的采样率
 */
static int __fn_adc_rate_get (void          *p_drv,
                              int            chan,
                              uint32_t      *p_rate)
{
    am_stm32f103rbt6_adc_dev_t *p_dev;
    amhw_stm32f103rbt6_adc_t   *p_hw_adc;
    
    uint32_t sample;
    uint32_t adc_clkdiv = 1;
    uint32_t i;

    uint8_t  sample_time[8] = {
      15, 21, 27, 42, 55, 69, 85, 253
    };

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_stm32f103rbt6_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_stm32f103rbt6_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    i = amhw_stm32f103rbt6_adc_smpr_get(p_hw_adc,
                                (amhw_stm32f103rbt6_adc_channel_t)p_dev->chan);
    sample = sample_time[i];

    /* ADC分频设置位于RCC驱动中 */
    adc_clkdiv = 2 * (amhw_stm32f103rbt6_rcc_adc_div_get() + 1);
    *p_rate = am_clk_rate_get(p_dev->p_devinfo->clk_num) /
              (adc_clkdiv * sample);

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
    uint8_t    i = 0, j  = 0;

    uint8_t  sample_time[8] = {
      15, 21, 27, 42, 55, 69, 85, 253
    };

    uint32_t   rate_high = 0;
    uint32_t   rate_low  = 0;
    uint32_t   rate_err  = 0;

    struct adc_err  {
      uint32_t err;
      uint8_t  sample_time;
      uint8_t  adc_div;
    };

    struct adc_err    adc_rate_err;
    am_stm32f103rbt6_adc_dev_t *p_dev;
    amhw_stm32f103rbt6_adc_t   *p_hw_adc;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_stm32f103rbt6_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_stm32f103rbt6_adc_t *)(p_dev->p_devinfo->adc_reg_base);
    
    adc_rate_err.err         = rate;
    adc_rate_err.sample_time = 0;
    adc_rate_err.adc_div     = 0;

    clk       = am_clk_rate_get(p_dev->p_devinfo->clk_num);
    rate_high = clk / 15;
    rate_low  = clk / (8 * 253);

    if (rate < rate_low || rate > rate_high) {
        return -AM_EINVAL;
    }

    for(i = 1; i < 9; i++) {
        if (rate >= (clk / (i * 253)) &&
            rate <= (clk / (i * 15))) {

            for(j = 0; j < 8; j++) {

                  temp_rate = (clk / (i * sample_time[j]));
                  rate_err = temp_rate >= rate ? temp_rate - rate :
                                                 rate - temp_rate;
                  if (rate_err < adc_rate_err.err) {
                      adc_rate_err.err = rate_err;
                      adc_rate_err.adc_div = i;
                      adc_rate_err.sample_time = j;
                  }

                  if (adc_rate_err.err == 0) {
                      break;
                  }
              }
        }
    }
    amhw_stm32f103rbt6_rcc_adc_div_set (adc_rate_err.adc_div / 2 - 1);
    amhw_stm32f103rbt6_adc_smpr_set (p_hw_adc,
          (amhw_stm32f103rbt6_adc_sample_time_t)adc_rate_err.sample_time,
          (amhw_stm32f103rbt6_adc_channel_t)chan);

    return AM_OK;
}

/**
 * \brief 获取ADC转换精度
 */
static uint32_t __fn_bits_get (void *p_drv, int chan)
{
    /* 仅支持12位分辨率 */
    return AMHW_STM32F103RBT6_ADC_DATA_VALID_12BIT;
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
am_adc_handle_t am_stm32f103rbt6_adc_init (am_stm32f103rbt6_adc_dev_t           *p_dev,
                                    const am_stm32f103rbt6_adc_devinfo_t *p_devinfo)
{
    uint16_t bit = 0;

    amhw_stm32f103rbt6_adc_t   *p_hw_adc = NULL;

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

    /* 引脚初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_hw_adc = (amhw_stm32f103rbt6_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    /* ADC禁能 */
    amhw_stm32f103rbt6_adc_disable(p_hw_adc);

    /* 关闭扫描模式、注入通道间断模式、规则通道间断模式*/
    amhw_stm32f103rbt6_adc_scan_mode_disable(p_hw_adc);
    amhw_stm32f103rbt6_adc_injected_disc_disable(p_hw_adc);
    amhw_stm32f103rbt6_adc_regular_disc_disable(p_hw_adc);

    /* 单次转换 */
    amhw_stm32f103rbt6_adc_cont_set(p_hw_adc, AMHW_STM32F103RBT6_ADC_CONVERSION_SINGLE);

    /* 设置成独立模式*/
    amhw_stm32f103rbt6_adc_dul_mode_set(p_hw_adc, AMHW_STM32F103RBT6_ADC_DUL_MODE_0);

    /* 启用规则通道外部触发，并设置成软件触发方式 */
    amhw_stm32f103rbt6_adc_extirig_enable(p_hw_adc);
    amhw_stm32f103rbt6_adc_extsel_set(p_hw_adc, AMHW_STM32F103RBT6_ADC12_REGULAR_SWSTART);

    /* 对齐方式--右对齐*/
    amhw_stm32f103rbt6_adc_data_alignment_set(p_hw_adc,AM_ADC_DATA_ALIGN_RIGHT);

    /* 有效位数获取 */
    bit = __fn_bits_get(p_dev, 0);

    /* 数据右移位数 */
    p_dev->right_bit = 12 - bit;

    /* 设置采样率 */
    __fn_adc_rate_set(p_dev, 0, 100000);

    if( p_dev->p_devinfo->temp_mode == 0) {
        amhw_stm32f103rbt6_adc_tsvrefe_disable(p_hw_adc);
    } else {
        amhw_stm32f103rbt6_adc_tsvrefe_enable(p_hw_adc);
    }

    /* ADC使能*/
    amhw_stm32f103rbt6_adc_enable(p_hw_adc);

    /* 启用复位校准*/
    amhw_stm32f103rbt6_adc_rstcal_enable(p_hw_adc);

    /* 等待复位校准结束*/
    while(amhw_stm32f103rbt6_adc_rstcal_check(p_hw_adc) == AM_FALSE);

    /* 启用AD校准*/
    amhw_stm32f103rbt6_adc_cal_enable(p_hw_adc);

    /* 等待AD校准结束*/
    while(amhw_stm32f103rbt6_adc_cal_check(p_hw_adc) == AM_FALSE);

    /* 注册ADC相关中断 */
    __fn_adc_connect(p_dev);

    return (am_adc_handle_t)(&(p_dev->adc_serve));
}

/**
 * \brief ADC去初始化
 */
void am_stm32f103rbt6_adc_deinit (am_adc_handle_t handle)
{
    am_stm32f103rbt6_adc_dev_t *p_dev    = (am_stm32f103rbt6_adc_dev_t *)handle;
    amhw_stm32f103rbt6_adc_t   *p_hw_adc = NULL;

    if (NULL == p_dev) {
        return ;
    }

    p_hw_adc = (amhw_stm32f103rbt6_adc_t *)(p_dev->p_devinfo->adc_reg_base);
    am_int_disable(p_dev->p_devinfo->inum);

    /* 关闭所有中断 */
    amhw_stm32f103rbt6_adc_int_disable(p_hw_adc, AMHW_STM32F103RBT6_ADC_INT_ALL);

    /* ADC禁能 */
    amhw_stm32f103rbt6_adc_disable(p_hw_adc);

    p_dev->adc_serve.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
