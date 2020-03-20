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
 * \brief HC32 ADC 用户配置文件
 * \sa am_hwconf_hc32_adc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-24  zp, first implementation
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32.h"
#include "hc32_inum.h"
#include "hc32_periph_map.h"
#include "am_hc32_adc.h"
#include "hw/amhw_hc32_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_hc32_adc
 * \copydoc am_hwconf_hc32_adc.c
 * @{
 */

/** \brief ADC平台初始化 */
static void __hc32_plfm_adc_init (void)
{
    /* 使能ADC1时钟*/
    am_clk_enable(CLK_ADC_BGR);
}

/** \brief 解除ADC平台初始化 */
static void __hc32_plfm_adc_deinit (void)
{
    /* 失能ADC1时钟*/
    am_clk_disable (CLK_ADC_BGR);
}

/**
 *  \brief  ADC的引脚配置信息列表
 *
 *  \note1 每个成员代表每个ADC通道的引脚配置，例如：AMHW_HC32_CHAN_AIN0_PA0，代表TIM0捕获
 *         ADC通道0的引脚配置
 *
 *  \note2 使用AMHW_HC32_CHAN_INSIDE_TEMP_SENSOR时，要使能内部温度传感器
 *
 */
am_hc32_adc_ioinfo_t __g_adc_ioinfo_list[] = {
    {AMHW_HC32_CHAN_AIN0_PA0 ,  PIOA_0 , PIOA_0_GPIO  | PIOA_0_AIN , PIOA_0_GPIO  | PIOA_0_INPUT_PU },
    {AMHW_HC32_CHAN_AIN1_PA1 ,  PIOA_1 , PIOA_1_GPIO  | PIOA_1_AIN , PIOA_1_GPIO  | PIOA_1_INPUT_PU },
    {AMHW_HC32_CHAN_AIN2_PA2 ,  PIOA_2 , PIOA_2_GPIO  | PIOA_2_AIN , PIOA_2_GPIO  | PIOA_2_INPUT_PU },
    {AMHW_HC32_CHAN_AIN3_PA3 ,  PIOA_3 , PIOA_3_GPIO  | PIOA_3_AIN , PIOA_3_GPIO  | PIOA_3_INPUT_PU },
    {AMHW_HC32_CHAN_AIN4_PA4 ,  PIOA_4 , PIOA_4_GPIO  | PIOA_4_AIN , PIOA_4_GPIO  | PIOA_4_INPUT_PU },
    {AMHW_HC32_CHAN_AIN5_PA5 ,  PIOA_5 , PIOA_5_GPIO  | PIOA_5_AIN , PIOA_5_GPIO  | PIOA_5_INPUT_PU },
    {AMHW_HC32_CHAN_AIN6_PA6 ,  PIOA_6 , PIOA_6_GPIO  | PIOA_6_AIN , PIOA_6_GPIO  | PIOA_6_INPUT_PU },
    {AMHW_HC32_CHAN_AIN7_PA7 ,  PIOA_7 , PIOA_7_GPIO  | PIOA_7_AIN , PIOA_7_GPIO  | PIOA_7_INPUT_PU },
    {AMHW_HC32_CHAN_AIN8_PB0 ,  PIOB_0 , PIOB_0_GPIO  | PIOB_0_AIN , PIOB_0_GPIO  | PIOB_0_INPUT_PU },
    {AMHW_HC32_CHAN_AIN9_PB1 ,  PIOB_1 , PIOB_1_GPIO  | PIOB_1_AIN , PIOB_1_GPIO  | PIOB_1_INPUT_PU },
    {AMHW_HC32_CHAN_AIN10_PC0,  PIOC_0 , PIOC_0_GPIO  | PIOC_0_AIN , PIOC_0_GPIO  | PIOC_0_INPUT_PU },
    {AMHW_HC32_CHAN_AIN11_PC1,  PIOC_1 , PIOC_1_GPIO  | PIOC_1_AIN , PIOC_1_GPIO  | PIOC_1_INPUT_PU },
    {AMHW_HC32_CHAN_AIN12_PC2,  PIOC_2 , PIOC_2_GPIO  | PIOC_2_AIN , PIOC_2_GPIO  | PIOC_2_INPUT_PU },
    {AMHW_HC32_CHAN_AIN13_PC3,  PIOC_3 , PIOC_3_GPIO  | PIOC_3_AIN , PIOC_3_GPIO  | PIOC_3_INPUT_PU },
    {AMHW_HC32_CHAN_AIN14_PC4,  PIOC_4 , PIOC_4_GPIO  | PIOC_4_AIN , PIOC_4_GPIO  | PIOC_4_INPUT_PU },
    {AMHW_HC32_CHAN_AIN15_PC5,  PIOC_5 , PIOC_5_GPIO  | PIOC_5_AIN , PIOC_5_GPIO  | PIOC_5_INPUT_PU },
    {AMHW_HC32_CHAN_AIN16_PB2,  PIOB_2 , PIOB_2_GPIO  | PIOB_2_AIN , PIOB_2_GPIO  | PIOB_2_INPUT_PU },
    {AMHW_HC32_CHAN_AIN17_PB10, PIOB_10, PIOB_10_GPIO | PIOB_10_AIN, PIOB_10_GPIO | PIOB_10_INPUT_PU},
    {AMHW_HC32_CHAN_AIN18_PB11, PIOB_11, PIOB_11_GPIO | PIOB_11_AIN, PIOB_11_GPIO | PIOB_11_INPUT_PU},
    {AMHW_HC32_CHAN_AIN19_PB12, PIOB_12, PIOB_12_GPIO | PIOB_12_AIN, PIOB_12_GPIO | PIOB_12_INPUT_PU},
    {AMHW_HC32_CHAN_AIN20_PB13, PIOB_13, PIOB_13_GPIO | PIOB_13_AIN, PIOB_13_GPIO | PIOB_13_INPUT_PU},
    {AMHW_HC32_CHAN_AIN21_PB14, PIOB_14, PIOB_14_GPIO | PIOB_14_AIN, PIOB_14_GPIO | PIOB_14_INPUT_PU},
    {AMHW_HC32_CHAN_AIN22_PB15, PIOB_15, PIOB_15_GPIO | PIOB_15_AIN, PIOB_15_GPIO | PIOB_15_INPUT_PU},
    {AMHW_HC32_CHAN_AIN23_PC6,  PIOC_6 , PIOC_6_GPIO  | PIOC_6_AIN , PIOC_6_GPIO  | PIOC_6_INPUT_PU },
    {AWHW_HC32_CHAN_OPA1_OUT,   PIOC_7 , PIOC_7_GPIO  | PIOC_7_AIN , PIOC_7_GPIO  | PIOC_7_INPUT_PU },
    {AWHW_HC32_CHAN_OPA2_OUT,   PIOB_14, PIOB_14_GPIO | PIOB_14_AIN, PIOB_14_GPIO | PIOB_14_INPUT_PU},
    {AWHW_HC32_CHAN_OPA3_OUT,   PIOB_11, PIOB_11_GPIO | PIOB_11_AIN, PIOB_11_GPIO | PIOB_11_INPUT_PU},
    {AMHW_HC32_CHAN_VCC_DIV3  , 0                                  , 0                              },
    {AMHW_HC32_CHAN_INSIDE_TEMP_SENSOR, 0                          , 0                              },
    {AMHW_HC32_CHAN_INSIDE_REFVCC_12,   0                          , 0                              },
};

/** \brief 设备信息 */
static const am_hc32_adc_devinfo_t __g_adc_devinfo = {
    HC32_ADC_BASE,                      /**< \brief ADC */
    INUM_ADC_DAC,                         /**< \brief ADC的中断编号 */
    CLK_ADC_BGR,                          /**< \brief ADC时钟号 */
    AMHW_HC32_ADC_REFVOL_AVCC,          /**< \brief 参考电压选择 */
    3300,                                 /**< \brief 参考电压(mv)*/
    0,                                    /**< \brief ADC通道28内部温度传感器开启使能
                                           *          1：开启， 0：关闭
                                           */
    12,                                   /**< \brief 转换精度，hc32精度只能为12位 */
    &__g_adc_ioinfo_list[0],              /**< \brief 引脚信息列表 */
    __hc32_plfm_adc_init,               /**< \brief ADC1的平台初始化 */
    __hc32_plfm_adc_deinit,             /**< \brief ADC1的平台去初始化 */

};

/**< \brief 设备实例 */
static am_hc32_adc_dev_t  __g_adc_dev;

/** \brief ADC实例初始化，获得ADC标准服务句柄 */
am_adc_handle_t am_hc32_adc_inst_init (void)
{
    return am_hc32_adc_init(&__g_adc_dev, &__g_adc_devinfo);
}

/** \brief ADC实例解初始化 */
void am_hc32_adc_inst_deinit (am_adc_handle_t handle)
{
    am_hc32_adc_deinit(handle);
}

/**
 * @}
 */

/* end of file */
