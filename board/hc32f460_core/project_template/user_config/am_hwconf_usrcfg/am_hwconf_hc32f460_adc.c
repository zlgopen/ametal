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
 * \brief HC32F460 ADC 用户配置文件
 * \sa am_hwconf_hc32f460_adc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-21  cds, first implementation
 * \endinternal
 */

#include "am_hc32f460.h"
#include "hw/amhw_hc32f460_rcc.h"
#include "am_gpio.h"
#include "am_hc32f460_adc.h"
#include "hw/amhw_hc32f460_adc.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_adc
 * \copydoc am_hwconf_hc32f460_adc.c
 * @{
 */

/*******************************************************************************/
/** \brief ADC1平台初始化 */
static void __hc32f460_plfm_adc1_init (void)
{
    /* 使能ADC1时钟*/
    am_clk_enable(CLK_ADC1);
}

/** \brief 解除ADC平台初始化 */
static void __hc32f460_plfm_adc1_deinit (void)
{
    /* 失能ADC1时钟*/
    am_clk_disable (CLK_ADC1);
}

/**
 *  \brief  ADC的引脚配置信息列表
 *
 *  \note 每个成员代表每个ADC通道对应的通道类型及引脚配置
 *
 */
am_hc32f460_adc_ioinfo_t __g_adc1_ioinfo_list[] = {
    {0,  AMHW_HC32F460_ADC_CH_MUX_ADC1_IN0 ,       PIOA_0 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN0 },
    {1,  AMHW_HC32F460_ADC_CH_MUX_ADC1_IN1 ,       PIOA_1 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN1 },
    {2,  AMHW_HC32F460_ADC_CH_MUX_ADC1_IN2 ,       PIOA_2 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN2 },
    {3,  AMHW_HC32F460_ADC_CH_MUX_ADC1_IN3 ,       PIOA_3 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN3 },
    {4,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN4 ,      PIOA_4 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN4 },
    {5,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN5 ,      PIOA_5 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN5 },
    {6,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN6 ,      PIOA_6 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN6 },
    {7,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN7 ,      PIOA_7 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN7 },
    {8,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN8 ,      PIOB_0 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN8 },
    {9,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN9 ,      PIOB_1 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN9 },
    {10, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN10,      PIOC_0 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN10 },
    {11, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN11,      PIOC_1 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC12_IN11 },
    {12, AMHW_HC32F460_ADC_CH_MUX_ADC1_IN12,       PIOC_2 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN12 },
    {13, AMHW_HC32F460_ADC_CH_MUX_ADC1_IN13,       PIOC_3 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN13 },
    {14, AMHW_HC32F460_ADC_CH_MUX_ADC1_IN14,       PIOC_4 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN14 },
    {15, AMHW_HC32F460_ADC_CH_MUX_ADC1_IN15,       PIOC_5 , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) , GPIO_MODE(AMHW_HC32F460_GPIO_MODE_IPU), AMHW_HC32F460_ADC_CH_MUX_ADC1_IN15 },
    {16, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN_ANALOG,      0 ,                           0 ,                          0 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN_ANALOG},
};

/** \brief 设备信息 */
static const am_hc32f460_adc_devinfo_t __g_adc1_devinfo = {
    HC32F460_ADC1_BASE,                 /**< \brief ADC */
    INUM_ADC1,                          /**< \brief ADC的中断编号 */
    CLK_ADC1,                           /**< \brief ADC时钟号 */
    1,
    3300,                               /**< \brief 参考电压(mv)*/

    AMHW_HC32F460_ADC_RESOLUTION_12BIT, /**< \brief 转换精度，12位/10位/8位 */
    &__g_adc1_ioinfo_list[0],           /**< \brief 引脚信息列表 */
    __hc32f460_plfm_adc1_init,          /**< \brief ADC1的平台初始化 */
    __hc32f460_plfm_adc1_deinit,        /**< \brief ADC1的平台去初始化 */

};

/**< \brief 设备实例 */
am_hc32f460_adc_dev_t  __g_adc1_dev;

/** \brief ADC实例初始化，获得ADC标准服务句柄 */
am_adc_handle_t am_hc32f460_adc1_inst_init (void)
{
    return am_hc32f460_adc_init(&__g_adc1_dev, &__g_adc1_devinfo);
}

/** \brief ADC实例解初始化 */
void am_hc32f460_adc1_inst_deinit (am_adc_handle_t handle)
{
    am_hc32f460_adc_deinit(handle);
}




/** \brief ADC2平台初始化 */
static void __hc32f460_plfm_adc2_init (void)
{
    /* 使能ADC2时钟*/
    am_clk_enable(CLK_ADC2);
}

/** \brief 解除ADC平台初始化 */
static void __hc32f460_plfm_adc2_deinit (void)
{
    /* 失能ADC2时钟*/
    am_clk_disable (CLK_ADC2);
}

/**
 *  \brief  ADC的引脚配置信息列表
 *
 *  \note 每个成员代表每个ADC通道对应的通道类型及引脚配置
 *
 */
am_hc32f460_adc_ioinfo_t __g_adc2_ioinfo_list[] = {
    {0,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN4 ,     PIOA_4 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN4 },
    {1,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN5 ,     PIOA_5 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN5 },
    {2,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN6 ,     PIOA_6 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN6 },
    {3,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN7 ,     PIOA_7 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN7 },
    {4,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN8 ,     PIOB_0 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN8 },
    {5,  AMHW_HC32F460_ADC_CH_MUX_ADC12_IN9 ,     PIOB_1 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN9 },
    {6, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN10,      PIOC_0 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN10 },
    {7, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN11,      PIOC_1 , AMHW_HC32F460_GPIO_MODE_AIN , AMHW_HC32F460_GPIO_MODE_IPU, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN11 },
    {8, AMHW_HC32F460_ADC_CH_MUX_ADC12_IN_ANALOG,      0 ,                           0 ,                          0 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN_ANALOG},
};

/** \brief 设备信息 */
static const am_hc32f460_adc_devinfo_t __g_adc2_devinfo = {
    HC32F460_ADC2_BASE,                 /**< \brief ADC */
    INUM_ADC2,                          /**< \brief ADC的中断编号 */
    CLK_ADC2,                           /**< \brief ADC时钟号 */
    2,
    3300,                               /**< \brief 参考电压(mv)*/
    AMHW_HC32F460_ADC_RESOLUTION_12BIT, /**< \brief 转换精度，12位/10位/8位 */
    &__g_adc2_ioinfo_list[0],           /**< \brief 引脚信息列表 */
    __hc32f460_plfm_adc2_init,          /**< \brief ADC1的平台初始化 */
    __hc32f460_plfm_adc2_deinit,        /**< \brief ADC1的平台去初始化 */

};

/**< \brief 设备实例 */
am_hc32f460_adc_dev_t  __g_adc2_dev;

/** \brief ADC实例初始化，获得ADC标准服务句柄 */
am_adc_handle_t am_hc32f460_adc2_inst_init (void)
{
    return am_hc32f460_adc_init(&__g_adc2_dev, &__g_adc2_devinfo);
}

/** \brief ADC实例解初始化 */
void am_hc32f460_adc2_inst_deinit (am_adc_handle_t handle)
{
    am_hc32f460_adc_deinit(handle);
}

/**
 * @}
 */

/* end of file */
