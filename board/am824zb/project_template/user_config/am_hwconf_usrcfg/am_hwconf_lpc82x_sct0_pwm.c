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
 * \brief LPC82X SCT0 用于PWM输出的用户配置文件
 * \sa am_hwconf_lpc82x_sct0_pwm.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-19  hgo, add annotations.
 * - 1.00 15-07-25  oce, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_clk.h"
#include "am_lpc_sct_pwm.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_sct0_pwm
 * \copydoc am_hwconf_lpc82x_sct0_pwm.c
 * @{
 */

/** \brief SCT0用于PWM的输出引脚配置信息列表  PWM的输出个数为6个 */
static am_lpc_sct_pwm_ioinfo_t __g_sct0_pwm_ioinfo_list[] = {
    /** \brief   输出0 */
    {PIO0_6,  PIO_FUNC_SCT_OUT0,  PIO0_6_GPIO  | PIO0_6_GPIO_INPUT},
    /** \brief   输出1 */
    {PIO0_2,  PIO_FUNC_SCT_OUT1,  PIO0_2_GPIO  | PIO0_2_GPIO_INPUT},
    /** \brief   输出2 */
    {PIO0_10,  PIO_FUNC_SCT_OUT2,  PIO0_10_GPIO  | PIO0_10_GPIO_INPUT},
    /** \brief   输出3 */
    {PIO0_11,  PIO_FUNC_SCT_OUT3,  PIO0_11_GPIO  | PIO0_11_GPIO_INPUT},
    /** \brief   输出4 */
    {PIO0_13,  PIO_FUNC_SCT_OUT4,  PIO0_13_GPIO  | PIO0_13_GPIO_INPUT},
    /** \brief   输出5 */
    {PIO0_14,  PIO_FUNC_SCT_OUT5,  PIO0_14_GPIO  | PIO0_14_GPIO_INPUT},
};

/** \brief SCT0用于PWM输出 平台初始化 */
static void __lpc82x_sct0_pwm_plfm_init ()
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SCT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);
}

/** \brief 解除SCT0平台初始化 */
static void __lpc82x_sct0_pwm_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset (AMHW_LPC82X_RESET_SCT);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_SCT);
}

/** \brief SCT0用于PWM设备信息 */
static const am_lpc_sct_pwm_devinfo_t __g_sct0_pwm_devinfo = {
    LPC82X_SCT0_BASE,             /**< \brief SCT0寄存器块的基地址 */
    CLK_SCT,                          /**< \brief SCT0时钟编号 */
    6,                            /**< \brief 6个PWM输出通道 */
    &__g_sct0_pwm_ioinfo_list[0], /**< \brief 所有PWM引脚配置信息，用首地址参数传递 */

    __lpc82x_sct0_pwm_plfm_init,   /**< \brief 平台初始化函数 */
    __lpc82x_sct0_pwm_plfm_deinit, /**< \brief 平台解初始化函数 */
};

/** \brief SCT0用于PWM设备定义 */
static am_lpc_sct_pwm_dev_t  __g_sct0_pwm_dev;

/** \brief SCT0 PWM 实例初始化，获得PWM标准服务句柄 */
am_pwm_handle_t am_lpc82x_sct0_pwm_inst_init (void)
{
    return am_lpc_sct_pwm_init(&__g_sct0_pwm_dev, &__g_sct0_pwm_devinfo);
}

/** \brief SCT0 PWM 实例解初始化 */
void am_lpc82x_sct0_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_lpc_sct_pwm_deinit((am_lpc_sct_pwm_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
