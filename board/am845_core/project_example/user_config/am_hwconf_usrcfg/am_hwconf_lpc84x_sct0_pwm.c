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
 * \brief LPC84X SCT0 PWM 用户配置文件
 * \sa am_hwconf_lpc84x_sct0_pwm.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-19  hgo, add annotations.
 * - 1.00 15-07-25  oce, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc_sct_pwm.h"
#include "hw/amhw_lpc84x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_sct0_pwm
 * \copydoc am_hwconf_lpc84x_sct0_pwm.c
 * @{
 */

/** \brief SCT0 用于 PWM 的引脚配置信息列表，PWM 的输出个数为 6 个 */
am_local am_lpc_sct_pwm_ioinfo_t __g_sct0_pwm_ioinfo_list[] = {
    {PIO0_23, PIO_FUNC_SCT_OUT0, PIO0_23_GPIO | PIO0_23_GPIO_INPUT}, /* 通道 0 */
    {PIO0_24, PIO_FUNC_SCT_OUT1, PIO0_24_GPIO | PIO0_24_GPIO_INPUT}, /* 通道 1 */
    {PIO0_25, PIO_FUNC_SCT_OUT2, PIO0_25_GPIO | PIO0_25_GPIO_INPUT}, /* 通道 2 */
    {PIO0_26, PIO_FUNC_SCT_OUT3, PIO0_26_GPIO | PIO0_26_GPIO_INPUT}, /* 通道 3 */
    {PIO0_27, PIO_FUNC_SCT_OUT4, PIO0_27_GPIO | PIO0_27_GPIO_INPUT}, /* 通道 4 */
    {PIO0_15, PIO_FUNC_SCT_OUT5, PIO0_15_GPIO | PIO0_15_GPIO_INPUT}, /* 通道 5 */
};

/**
 * \brief SCT0 PWM 平台初始化
 */
am_local void __lpc84x_sct0_pwm_plfm_init ()
{

	amhw_lpc84x_sct_clk_sel_set( AMHW_LPC84X_CLK_SCT_MAIN_CLK, 2);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SCT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SCT);
}

/**
 * \brief SCT0 平台解初始化
 */
am_local void __lpc84x_sct0_pwm_plfm_deinit (void)
{
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SCT);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_SCT);
}

/** \brief SCT0 PWM 设备信息 */
am_local am_const am_lpc_sct_pwm_devinfo_t __g_lpc84x_pwm_devinfo = {
    LPC84X_SCT0_BASE,                 /* SCT0 寄存器块基地址 */
    CLK_SCT,                          /* SCT0 时钟号 */
    7,                                /* 6 个 PWM 输出通道 */
    &__g_sct0_pwm_ioinfo_list[0],     /* 所有 PWM 引脚配置信息，用首地址参数传递 */
    __lpc84x_sct0_pwm_plfm_init,      /* 平台初始化函数 */
    __lpc84x_sct0_pwm_plfm_deinit,    /* 平台解初始化函数 */
};

/** \brief SCT0 PWM 设备实例 */
am_local am_lpc_sct_pwm_dev_t __g_lpc84x_pwm_dev;

/**
 * \brief SCT0 PWM 实例初始化
 */
am_pwm_handle_t am_lpc84x_sct0_pwm_inst_init (void)
{
    return am_lpc_sct_pwm_init(&__g_lpc84x_pwm_dev, &__g_lpc84x_pwm_devinfo);
}

/**
 * \brief SCT0 PWM 实例解初始化
 */
void am_lpc84x_sct0_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_lpc_sct_pwm_deinit((am_lpc_sct_pwm_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
