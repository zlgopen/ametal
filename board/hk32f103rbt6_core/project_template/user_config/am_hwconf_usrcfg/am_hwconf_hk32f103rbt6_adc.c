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
 * \brief HK32F103RBT6 ADC 用户配置文件
 * \sa am_hwconf_hk32f103rbt6_adc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-22  fra, first implementation
 * \endinternal
 */

#include "am_hk32f103rbt6.h"
#include "amhw_hk32f103rbt6_rcc.h"
#include "hk32f103rbt6_inum.h"
#include "hk32f103rbt6_periph_map.h"
#include "am_gpio.h"
#include "am_hk32f103rbt6_adc.h"
#include "amhw_hk32f103rbt6_adc.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hk32f103rbt6_adc
 * \copydoc am_hwconf_hk32f103rbt6_adc.c
 * @{
 */



/** \brief ADC平台初始化 */
static void __hk32f103rbt6_plfm_adc1_init (void)
{
    /* ADC1和ADC2共用通道，实际上不可同时用作ADC12,除非特殊模式使能*/
    /* 此处用作ADC1采集通道*/
    am_gpio_pin_cfg(PIOA_0, PIOA_0_ADC12_IN0 | PIOA_0_AIN);     /* ADC12通道0 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_ADC12_IN1 | PIOA_1_AIN);     /* ADC12通道1 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_ADC12_IN2 | PIOA_2_AIN);     /* ADC12通道2 */
    am_gpio_pin_cfg(PIOA_3, PIOA_3_ADC12_IN3 | PIOA_3_AIN);     /* ADC12通道3 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_ADC12_IN4 | PIOA_4_AIN);     /* ADC12通道4 */
    am_gpio_pin_cfg(PIOA_5, PIOA_5_ADC12_IN5 | PIOA_5_AIN);     /* ADC12通道5 */
    am_gpio_pin_cfg(PIOA_6, PIOA_6_ADC12_IN6 | PIOA_6_AIN);     /* ADC12通道6 */
    am_gpio_pin_cfg(PIOA_7, PIOA_7_ADC12_IN7 | PIOA_7_AIN);     /* ADC12通道7 */

    /* 使能ADC1时钟*/
    am_clk_enable(CLK_ADC1);
}

/** \brief 解除ADC平台初始化 */
static void __hk32f103rbt6_plfm_adc1_deinit (void)
{
    /* 失能ADC1时钟*/
    am_clk_disable (CLK_ADC1);
}

/** \brief 设备信息 */
static const am_hk32f103rbt6_adc_devinfo_t __g_adc1_devinfo = {

    HK32F103RBT6_ADC1_BASE,                    /**< \brief ADC */
    INUM_ADC1_2,                         /**< \brief ADC的中断编号 */
    CLK_ADC1,                            /**< \brief ADC时钟号 */

    3300,                                /**< \brief 参考电压 */

    0,                                   /**< \brief ADC1通道16内部温度传感器开启使能
                                          *          1：开启， 0：关闭
                                          *          仅ADC1有效
                                          */

    AMHW_HK32F103RBT6_ADC_DATA_VALID_12BIT,    /**< \brief 转换精度，hk32f103rbt6精度只能为12位 */

    __hk32f103rbt6_plfm_adc1_init,             /**< \brief ADC1的平台初始化 */
    __hk32f103rbt6_plfm_adc1_deinit,           /**< \brief ADC1的平台去初始化 */

};

/**< \brief 设备实例 */
static am_hk32f103rbt6_adc_dev_t  __g_adc1_dev;

/** \brief ADC实例初始化，获得ADC标准服务句柄 */
am_adc_handle_t am_hk32f103rbt6_adc1_inst_init (void)
{
    return am_hk32f103rbt6_adc_init(&__g_adc1_dev, &__g_adc1_devinfo);
}

/** \brief ADC实例解初始化 */
void am_hk32f103rbt6_adc1_inst_deinit (am_adc_handle_t handle)
{
    am_hk32f103rbt6_adc_deinit(handle);
}

/** \brief ADC2平台初始化 */
static void __hk32f103rbt6_plfm_adc2_init (void)
{
    /* ADC1和ADC2共用通道，实际上不可同时用作ADC12,除非特殊模式使能*/
    /* 此处用作ADC2采集通道*/
    am_gpio_pin_cfg(PIOB_0, PIOB_0_ADC12_IN8  | PIOB_0_AIN);/* ADC12通道8 */
    am_gpio_pin_cfg(PIOB_1, PIOB_1_ADC12_IN9  | PIOB_1_AIN);/* ADC12通道9 */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_ADC12_IN10 | PIOC_0_AIN);/* ADC12通道10 */
    am_gpio_pin_cfg(PIOC_1, PIOC_1_ADC12_IN11 | PIOC_1_AIN);/* ADC12通道11 */
    am_gpio_pin_cfg(PIOC_2, PIOC_2_ADC12_IN12 | PIOC_2_AIN);/* ADC12通道12 */
    am_gpio_pin_cfg(PIOC_3, PIOC_3_ADC12_IN13 | PIOC_3_AIN);/* ADC12通道13 */
    am_gpio_pin_cfg(PIOC_4, PIOC_4_ADC12_IN14 | PIOC_4_AIN);/* ADC12通道14 */
    am_gpio_pin_cfg(PIOC_5, PIOC_5_ADC12_IN15 | PIOC_5_AIN);/* ADC12通道15 */

    /* 使能ADC2时钟*/
    am_clk_enable(CLK_ADC2);
}

/** \brief 解除ADC平台初始化 */
static void __hk32f103rbt6_plfm_adc2_deinit (void)
{
    /* 失能ADC2时钟*/
    am_clk_disable (CLK_ADC2);
}

/** \brief ADC设备信息 */
static const am_hk32f103rbt6_adc_devinfo_t __g_adc2_devinfo = {

    HK32F103RBT6_ADC2_BASE,                 /**< \brief ADC */
    INUM_ADC1_2,                      /**< \brief ADC的中断编号 */
    CLK_ADC2,                         /**< \brief ADC时钟号 */

    3300,                             /**< \brief 参考电压 */

    0,                                /**< \brief ADC1通道16内部温度传感器开启使能
                                       *          1：开启， 0：关闭
                                       *          仅ADC1有效
                                       */

    AMHW_HK32F103RBT6_ADC_DATA_VALID_12BIT, /**< \brief 转换精度，hk32f103rbt6精度只能为12位 */

    __hk32f103rbt6_plfm_adc2_init,          /**< \brief ADC2的平台初始化 */
    __hk32f103rbt6_plfm_adc2_deinit,        /**< \brief ADC2的平台去初始化 */

};

/**< \brief 设备实例 */
static am_hk32f103rbt6_adc_dev_t  __g_adc2_dev;

/** \brief ADC实例初始化，获得ADC标准服务句柄 */
am_adc_handle_t am_hk32f103rbt6_adc2_inst_init (void)
{
    return am_hk32f103rbt6_adc_init(&__g_adc2_dev, &__g_adc2_devinfo);
}

/** \brief ADC实例解初始化 */
void am_hk32f103rbt6_adc2_inst_deinit (am_adc_handle_t handle)
{
    am_hk32f103rbt6_adc_deinit(handle);
}

/**
 * @}
 */

/* end of file */
