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
 * \brief MM32L373 ADC 用户配置文件
 * \sa am_hwconf_mm32l373_adc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-22  fra, first implementation
 * \endinternal
 */

#include "mm32l373_periph_map.h"
#include "am_mm32l373.h"
#include "mm32l373_inum.h"
#include "am_gpio.h"
#include "am_mm32_adc.h"
#include "amhw_mm32_adc.h"
#include "amhw_mm32l373_rcc.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l373_adc
 * \copydoc am_hwconf_mm32l373_adc.c
 * @{
 */



/** \brief ADC平台初始化 */
static void __mm32_plfm_adc1_init (void)
{
    am_gpio_pin_cfg(PIOA_0, PIOA_0_ADC1_IN0 | PIOA_0_AIN);     /* ADC1通道0 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_ADC1_IN1 | PIOA_1_AIN);     /* ADC1通道1 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_ADC1_IN2 | PIOA_2_AIN);     /* ADC1通道2 */
    am_gpio_pin_cfg(PIOA_3, PIOA_3_ADC1_IN3 | PIOA_3_AIN);     /* ADC1通道3 */
//    am_gpio_pin_cfg(PIOA_4, PIOA_4_ADC1_IN4 | PIOA_4_AIN);     /* ADC1通道4 */
//    am_gpio_pin_cfg(PIOA_5, PIOA_5_ADC1_IN5 | PIOA_5_AIN);     /* ADC1通道5 */
//    am_gpio_pin_cfg(PIOA_6, PIOA_6_ADC1_IN6 | PIOA_6_AIN);     /* ADC1通道6 ，温度传感器通道*/
//    am_gpio_pin_cfg(PIOA_7, PIOA_7_ADC1_IN7 | PIOA_7_AIN);     /* ADC1通道7 ，电压传感器通道*/

    am_clk_enable(CLK_ADC1);
}

/** \brief 解除ADC平台初始化 */
static void __mm32_plfm_adc1_deinit (void)
{
    am_clk_disable (CLK_ADC1);
}

/** \brief 设备信息 */
static const am_mm32_adc_devinfo_t __g_adc1_devinfo = {

    MM32L373_ADC1_BASE,                 /**< \brief ADC */
    INUM_ADC1_2,                      /**< \brief ADC的中断编号 */
    CLK_ADC1,                         /**< \brief ADC时钟号 */

    3300,                             /**< \brief 参考电压 */
    AMHW_MM32_ADC_DATA_VALID_12BIT, /**< \brief 转换精度 */

    __mm32_plfm_adc1_init,             /**< \brief ADC1的平台初始化 */
    __mm32_plfm_adc1_deinit,           /**< \brief ADC1的平台去初始化 */

};

/**< \brief 设备实例 */
static am_mm32_adc_dev_t  __g_adc1_dev;

/** \brief ADC实例初始化，获得ADC标准服务句柄 */
am_adc_handle_t am_mm32l373_adc1_inst_init (void)
{
    return am_mm32_adc_init(&__g_adc1_dev, &__g_adc1_devinfo);
}

/** \brief ADC实例解初始化 */
void am_mm32l373_adc1_inst_deinit (am_adc_handle_t handle)
{
    am_mm32_adc_deinit(handle);
}

/** \brief ADC2平台初始化 */
static void __mm32_plfm_adc2_init (void)
{
    am_gpio_pin_cfg(PIOB_0, PIOB_0_AIN);
    am_clk_enable(CLK_ADC2);
}

/** \brief 解除ADC平台初始化 */
static void __mm32_plfm_adc2_deinit (void)
{
    am_gpio_pin_cfg(PIOB_0, PIOB_0_INPUT_FLOAT);
    am_clk_disable (CLK_ADC2);
}

/** \brief ADC设备信息 */
static const am_mm32_adc_devinfo_t __g_adc2_devinfo = {

    MM32L373_ADC2_BASE,                 /**< \brief ADC */
    INUM_ADC1_2,                      /**< \brief ADC的中断编号 */
    CLK_ADC2,                         /**< \brief ADC时钟号 */

    3300,                             /**< \brief 参考电压 */
    AMHW_MM32_ADC_DATA_VALID_12BIT, /**< \brief 转换精度 */

    __mm32_plfm_adc2_init,             /**< \brief ADC2的平台初始化 */
    __mm32_plfm_adc2_deinit,           /**< \brief ADC2的平台去初始化 */

};

/**< \brief 设备实例 */
static am_mm32_adc_dev_t  __g_adc2_dev;

/** \brief ADC实例初始化，获得ADC标准服务句柄 */
am_adc_handle_t am_mm32l373_adc2_inst_init (void)
{
    return am_mm32_adc_init(&__g_adc2_dev, &__g_adc2_devinfo);
}

/** \brief ADC实例解初始化 */
void am_mm32l373_adc2_inst_deinit (am_adc_handle_t handle)
{
    am_mm32_adc_deinit(handle);
}

/**
 * @}
 */

/* end of file */
