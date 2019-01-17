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
 * \brief LPC84X ADC (中断方式) 用户配置文件
 * \sa am_hwconf_lpc84x_adc0_int.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-14  hgo, add calibrate func
 * - 1.00 15-07-15  bob, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc84x_adc_int.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_adc.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "hw/amhw_lpc84x_adc.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_adc0_int
 * \copydoc am_hwconf_lpc84x_adc0_int.c
 * @{
 */

/**
 * \brief ADC0 平台初始化
 */
am_local void __lpc84x_adc0_int_plfm_init (void)
{
    amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_ADC0);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_ADC);
    amhw_lpc84x_clk_adc_sel_set (AMHW_LPC84X_CLK_ADC_FRO, 1);

    /* 配置通道，引脚配置为消极模式 INACTIVE */
    am_gpio_pin_cfg(PIO0_7,  PIO0_7_ADC_0   | PIO0_7_INACTIVE);
//    am_gpio_pin_cfg(PIO0_6,  PIO0_6_ADC_1   | PIO0_6_INACTIVE);
//    am_gpio_pin_cfg(PIO0_14, PIO0_14_ADC_2  | PIO0_14_INACTIVE);
//    am_gpio_pin_cfg(PIO0_23, PIO0_23_ADC_3  | PIO0_23_INACTIVE);
//    am_gpio_pin_cfg(PIO0_22, PIO0_22_ADC_4  | PIO0_22_INACTIVE);
//    am_gpio_pin_cfg(PIO0_21, PIO0_21_ADC_5  | PIO0_21_INACTIVE);
//    am_gpio_pin_cfg(PIO0_20, PIO0_20_ADC_6  | PIO0_20_INACTIVE);
    am_gpio_pin_cfg(PIO0_19, PIO0_19_ADC_7  | PIO0_19_INACTIVE);
//    am_gpio_pin_cfg(PIO0_18, PIO0_18_ADC_8  | PIO0_18_INACTIVE);
//    am_gpio_pin_cfg(PIO0_17, PIO0_17_ADC_9  | PIO0_17_INACTIVE);
//    am_gpio_pin_cfg(PIO0_13, PIO0_13_ADC_10 | PIO0_13_INACTIVE);
//    am_gpio_pin_cfg(PIO0_4,  PIO0_4_ADC_11  | PIO0_4_INACTIVE);

    /* ADC 自动矫正 */
    amhw_lpc84x_adc_calibrate(LPC84X_ADC0,
                              amhw_lpc84x_clk_system_clkrate_get());
}

/**
 * \brief ADC0 平台解初始化
 */
am_local void __lpc84x_adc0_int_plfm_deinit (void)
{
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_ADC);
    amhw_lpc84x_syscon_powerdown(AMHW_LPC84X_SYSCON_PD_ADC0);
}

/** \brief ADC0 (中断方式) 设备信息 */
am_local am_const am_lpc84x_adc_int_devinfo_t __g_lpc84x_adc0_int_devinfo = {
    LPC84X_ADC0_BASE,                 /* ADC0 寄存器块基地址 */
	CLK_ADC,                          /* ADC0 时钟号 */
    2500,                             /* ADC 参考电压，单位：mV */
    INUM_ADC0_SEQA,                   /* ADC0 序列 A 中断号 */
    INUM_ADC0_OVR,                    /* ADC0 overrun 中断号 */
    __lpc84x_adc0_int_plfm_init,      /* 平台初始化函数 */
    __lpc84x_adc0_int_plfm_deinit,    /* 平台解初始化函数 */
};

/** \brief ADC0 (中断方式) 设备实例 */
am_local am_lpc84x_adc_int_dev_t __g_lpc84x_adc0_int_dev;

/**
 * \brief ADC0 (中断方式) 实例初始化
 */
am_adc_handle_t am_lpc84x_adc0_int_inst_init (void)
{
    return am_lpc84x_adc_int_init(&__g_lpc84x_adc0_int_dev,
                                  &__g_lpc84x_adc0_int_devinfo);
}

/**
 * \brief ADC0 (中断方式) 实例解初始化
 */
void am_lpc84x_adc0_int_inst_deinit (am_adc_handle_t handle)
{
    am_lpc84x_adc_int_deinit(handle);
}

/**
 * @}
 */

/* end of file */
