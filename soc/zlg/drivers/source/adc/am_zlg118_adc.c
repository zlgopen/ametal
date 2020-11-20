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
 * - 1.00 19-09-24  zp, first implementation
 * \endinternal
 */

#include "am_zlg118_adc.h"
#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "zlg118_pin.h"
#include "hw/amhw_zlg118_rcc.h"
#include "hw/amhw_zlg118_adc.h"
#include "zlg118_periph_map.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __ADC_HW_DECL(p_hw_adc, p_drv)   \
            amhw_zlg118_adc_t *p_hw_adc =   \
            ((am_zlg118_adc_dev_t *)p_drv)->p_devinfo->p_hw_adc

#define __ADC_VREF_GET(p_drv)  (((am_zlg118_adc_dev_t *)p_drv)->p_devinfo->vref)

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
void __zlg118_adc_irq_handle (void *p_arg)
{
    am_zlg118_adc_dev_t    *p_dev      = (am_zlg118_adc_dev_t *)p_arg;
    amhw_zlg118_adc_t      *p_hw_adc   =  NULL;

    /* 当前转换的序列描述符 */
    am_adc_buf_desc_t *p_desc  = &(p_dev->p_desc[p_dev->desc_index]);
    uint16_t          *p_buf16 = (uint16_t *)p_desc->p_buf;
    uint16_t           adc_dat = 0;

    if ( p_buf16 == NULL) {
        return ;
    }

    p_hw_adc =  (amhw_zlg118_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    /* 判断是否当前数据是有效的 */
    if (p_dev->conv_cnt < p_desc->length) {

        /* 以清除转换结束中断标志 */
        amhw_zlg118_adc_flag_clr(p_hw_adc,
                                 AMHW_ZLG118_ADC_FLAG_CONVERT_SINGLE);

        /* 读取数据 */
        adc_dat = amhw_zlg118_adc_data_get (p_hw_adc);

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

                    __fn_adc_stop (p_dev, p_dev->chan);  /* 关闭模块 */

                    return ; /* 返回 */
                }
            }
        } else {

            /* 单次转换启动 */
            amhw_zlg118_adc_single_convert_start(p_hw_adc);

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
    am_zlg118_adc_dev_t *p_dev = NULL;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_zlg118_adc_dev_t *)p_drv;

    /* 连接中断函数 */
    am_int_connect(p_dev->p_devinfo->inum,
                   __zlg118_adc_irq_handle,
                   (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief ADC 使用中断模式时启动配置
 */
static void __adc_int_work_startup (am_zlg118_adc_dev_t       *p_dev,
                                    amhw_zlg118_adc_chan_sel_t chan)
{
    amhw_zlg118_adc_t *p_hw_adc = (amhw_zlg118_adc_t *)
                                  (p_dev->p_devinfo->adc_reg_base);

    if(p_dev->p_devinfo->p_ioinfo != NULL) {

        /* ADC禁能*/
        amhw_zlg118_adc_disable(p_hw_adc);

        /* 链接ADC采样通道和引脚 */
        amhw_zlg118_adc_sgl_chan_sel(p_hw_adc,
                                     p_dev->p_devinfo->p_ioinfo[chan].chan_sel);

        /* 初始化引脚 */
        if(p_dev->p_devinfo->p_ioinfo[chan].chan_sel <
           AMHW_ZLG118_CHAN_DAC_OUT) {

            /* GPIO引脚模式设置 */
            am_gpio_pin_cfg(p_dev->p_devinfo->p_ioinfo[chan].gpio,
                            p_dev->p_devinfo->p_ioinfo[chan].func);
        } else {

            /* ADC输入信号放大器使能 */
            amhw_zlg118_adc_inamplify_enable(p_hw_adc);

            if(p_dev->p_devinfo->p_ioinfo[chan].chan_sel ==
               AMHW_ZLG118_CHAN_INSIDE_TEMP_SENSOR) {

                amhw_zlg118_bgr_temp_sensor_enable(AM_TRUE);
            }
        }

        /* 对齐方式设置 */
        amhw_zlg118_adc_align_way_sel(p_hw_adc,
                                      AMHW_ZLG118_ADC_DATA_ALIGN_RIGHT);

        /* 中断使能*/
        amhw_zlg118_adc_int_enable(p_hw_adc);

        /* 清标志位 */
        amhw_zlg118_adc_flag_clr(p_hw_adc, AMHW_ZLG118_ADC_FLAG_ALL);

        /* ADC使能*/
        amhw_zlg118_adc_enable(p_hw_adc);

        /* 单次转换启动 */
        amhw_zlg118_adc_single_convert_start(p_hw_adc);
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
    am_zlg118_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_zlg118_adc_dev_t *)p_drv;

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

    __adc_int_work_startup(p_dev,
                           (amhw_zlg118_adc_chan_sel_t)chan);/* 中断工作模式启动配置 */

    return AM_OK;
}

/**
 * \brief 停止转换
 */
static int __fn_adc_stop (void *p_drv, int chan)
{
    am_zlg118_adc_dev_t *p_dev    = (am_zlg118_adc_dev_t *)p_drv;
    amhw_zlg118_adc_t   *p_hw_adc = (amhw_zlg118_adc_t *)
                                    (p_dev->p_devinfo->adc_reg_base);

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }
    /* 单次转换停止 */
    amhw_zlg118_adc_single_convert_stop(p_hw_adc);

    /* ADC中断禁能 */
    amhw_zlg118_adc_int_disable(p_hw_adc);

    /* ADC禁能 */
    amhw_zlg118_adc_disable(p_hw_adc);

    if(p_dev->p_devinfo->p_ioinfo != NULL) {

        /* 初始化引脚 */
        if(p_dev->p_devinfo->p_ioinfo[chan].chan_sel <
           AMHW_ZLG118_CHAN_DAC_OUT) {

            /* GPIO引脚模式设置 */
            am_gpio_pin_cfg(p_dev->p_devinfo->p_ioinfo[chan].gpio,
                            p_dev->p_devinfo->p_ioinfo[chan].dfunc);
        } else {

            /* ADC输入信号放大器使能 */
            amhw_zlg118_adc_inamplify_disable(p_hw_adc);

            if(p_dev->p_devinfo->p_ioinfo[chan].chan_sel ==
               AMHW_ZLG118_CHAN_INSIDE_TEMP_SENSOR) {

                amhw_zlg118_bgr_temp_sensor_enable(AM_FALSE);
            }
        }
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
    am_zlg118_adc_dev_t *p_dev;
    amhw_zlg118_adc_t   *p_hw_adc;
    
    uint32_t sample;
    uint32_t adc_clkdiv = 1;
    uint32_t i;

    uint8_t  sample_time[4] = {20, 22, 24, 28};

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_zlg118_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_zlg118_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    i = amhw_zlg118_adc_sample_period_get(p_hw_adc);

    sample = sample_time[i];

    adc_clkdiv = (0x1ul << amhw_zlg118_adc_pclk_div_get(p_hw_adc));

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

    uint8_t  sample_time[4] = {20, 22, 24, 28};

    uint32_t   rate_high = 0;
    uint32_t   rate_low  = 0;
    uint32_t   rate_err  = 0;

    struct adc_err  {
      uint32_t err;
      uint8_t  sample_time;
      uint8_t  adc_div;
    };

    struct adc_err       adc_rate_err;
    am_zlg118_adc_dev_t *p_dev;
    amhw_zlg118_adc_t   *p_hw_adc;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_zlg118_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_zlg118_adc_t *)(p_dev->p_devinfo->adc_reg_base);
    
    adc_rate_err.err         = rate;
    adc_rate_err.sample_time = 0;
    adc_rate_err.adc_div     = 0;

    clk       = am_clk_rate_get(p_dev->p_devinfo->clk_num);

    rate_high = clk / (1 * 20);
    rate_low  = clk / (8 * 28);

    if (rate < rate_low || rate > rate_high) {
        return -AM_EINVAL;
    }

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {

            temp_rate = (clk / ((1 << i) * sample_time[j]));
            rate_err = temp_rate >= rate ? temp_rate - rate :
                                         rate - temp_rate;
            if (rate_err < adc_rate_err.err) {
                adc_rate_err.err         = rate_err;
                adc_rate_err.adc_div     = i;
                adc_rate_err.sample_time = j;
            }

            if (adc_rate_err.err == 0) {
              break;
            }
        }
    }

    /* 设置转换周期 */
    amhw_zlg118_adc_sample_period_sel(
        p_hw_adc, 
        (amhw_zlg118_adc_sample_period_t)adc_rate_err.sample_time);

    /* 设置ADC时钟分频 */
    amhw_zlg118_adc_pclk_div_sel(p_hw_adc, 
                                 (amhw_zlg118_adc_pclk_div_t)adc_rate_err.adc_div);

    return AM_OK;
}

/**
 * \brief 获取ADC转换精度
 */
static uint32_t __fn_bits_get (void *p_drv, int chan)
{
    /* 仅支持12位分辨率 */
    return 12;
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
am_adc_handle_t am_zlg118_adc_init (am_zlg118_adc_dev_t           *p_dev,
                                    const am_zlg118_adc_devinfo_t *p_devinfo)
{
    uint16_t bit = 0;

    amhw_zlg118_adc_t   *p_hw_adc = NULL;

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

    p_hw_adc = (amhw_zlg118_adc_t *)(p_dev->p_devinfo->adc_reg_base);

    if(p_devinfo->vref_choice == AMHW_ZLG118_ADC_REFVOL_OUTSIDE_PB1) {

        am_gpio_pin_cfg(PIOB_1, PIOB_1_GPIO | PIOB_1_AIN);

        /* 内部参考电压禁能 */
        amhw_zlg118_adc_inrefvol_disable(p_hw_adc);

        /* 设置参考电压源 */
        amhw_zlg118_adc_refvol_sel(p_hw_adc, p_devinfo->vref_choice);

    } else {

        /* 内部参考电压使能 */
        amhw_zlg118_adc_inrefvol_enable(p_hw_adc);

        /* 设置参考电压源 */
        amhw_zlg118_adc_refvol_sel(p_hw_adc, p_devinfo->vref_choice);
    }

    /* BGR模块使能 */
    amhw_zlg118_bgr_enable(AM_TRUE);

    /* ADC使能*/
    amhw_zlg118_adc_enable(p_hw_adc);

    /* 延时 20us，等待 ADC及 BGR模块启动完成 */
    am_udelay(20);

    /* ADC禁能 */
    amhw_zlg118_adc_disable(p_hw_adc);

    /* 使能单次转换模式 */
    amhw_zlg118_adc_mode_sel(p_hw_adc, AMHW_ZLG118_ADC_MODE_SINGLE);

    amhw_zlg118_adc_align_way_sel(p_hw_adc, AMHW_ZLG118_ADC_DATA_ALIGN_RIGHT);

    /* 有效位数获取 */
    bit = __fn_bits_get(p_dev, 0);

    /* 数据右移位数 */
    p_dev->right_bit = 12 - bit;

    /* 设置采样率 */
    __fn_adc_rate_set(p_dev, 0, 170000);

    if( p_dev->p_devinfo->temp_enable == 0) {
        amhw_zlg118_bgr_temp_sensor_enable(AM_FALSE);
    } else {
        amhw_zlg118_bgr_temp_sensor_enable(AM_TRUE);
    }

    /* ADC使能*/
    amhw_zlg118_adc_enable(p_hw_adc);

    /* 注册ADC相关中断 */
    __fn_adc_connect(p_dev);

    return (am_adc_handle_t)(&(p_dev->adc_serve));
}

/**
 * \brief ADC去初始化
 */
void am_zlg118_adc_deinit (am_adc_handle_t handle)
{
    am_zlg118_adc_dev_t *p_dev    = (am_zlg118_adc_dev_t *)handle;
    amhw_zlg118_adc_t   *p_hw_adc = NULL;

    if (NULL == p_dev) {
        return ;
    }

    p_hw_adc = (amhw_zlg118_adc_t *)(p_dev->p_devinfo->adc_reg_base);
    am_int_disable(p_dev->p_devinfo->inum);

    /* 关闭中断 */
    amhw_zlg118_adc_int_disable(p_hw_adc);

    /* ADC禁能 */
    amhw_zlg118_adc_disable(p_hw_adc);

    /* BGR模块禁能 */
    amhw_zlg118_bgr_enable(AM_FALSE);

    p_dev->adc_serve.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
