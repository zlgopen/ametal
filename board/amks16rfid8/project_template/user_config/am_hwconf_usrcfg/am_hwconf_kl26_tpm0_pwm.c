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
 * \brief KL26 TPM0 用于PWM输出 用户配置文件
 * \sa am_fsl_hwconfig_tpm0_pwm.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-18  sdy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_kl26.h"
#include "am_gpio.h"
#include "hw/amhw_kl26_sim.h"
#include "am_fsl_tpm_pwm.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_tpm0_pwm
 * \copydoc am_kl26_hwconfig_tpm0_pwm.c
 * @{
 */

/** \brief TPM0用于PWM输出 平台初始化 */
void __kl26_plfm_tpm0_pwm_init (void)
{
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_TPM0);
    amhw_kl26_sim_tpm_src_set(KL26_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief 解除TPM0平台初始化 */
void __kl26_plfm_tpm0_pwm_deinit (void)
{
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_TPM0);
}

/** \brief TPM0用于PWM功能的引脚配置信息列表 */
am_fsl_tpm_pwm_ioinfo_t __g_tpm0_pwm_ioinfo_list[] = {
    {PIOE_24, PIOE_24_TPM0_CH0, PIOE_24_GPIO | AM_GPIO_INPUT},  /**< \brief ??0 */
    {PIOE_25, PIOE_25_TPM0_CH1, PIOE_25_GPIO | AM_GPIO_INPUT},  /**< \brief ??1 */
    {PIOE_29, PIOE_29_TPM0_CH2, PIOE_29_GPIO | AM_GPIO_INPUT},  /**< \brief ??2 */
    {PIOE_30, PIOE_30_TPM0_CH3, PIOE_30_GPIO | AM_GPIO_INPUT},  /**< \brief ??3 */
    {PIOE_31, PIOE_31_TPM0_CH4, PIOE_31_GPIO | AM_GPIO_INPUT},  /**< \brief ??4 */
    //{PIOD_5, PIOD_5_TPM0_CH5, PIOD_5_GPIO | AM_GPIO_INPUT},  /**< \brief ??5 */
};

/** \brief TPM0用于PWM设备信息 */
const am_fsl_tpm_pwm_devinfo_t  __g_tpm0_pwm_devinfo = {
    KL26_TPM0,                  /**< \brief 指向TPM0寄存器块的指针 */
    6,                               /**< \brief 6个PWM输出通道 */
    CLK_TPM0,                        /**< \brief 时钟号 */
    &__g_tpm0_pwm_ioinfo_list[0],
    __kl26_plfm_tpm0_pwm_init,       /**< \brief 平台初始化函数 */
    __kl26_plfm_tpm0_pwm_deinit      /**< \brief 平台解初始化函数 */
};

/** \brief TPM0用于PWM设备定义 */
am_fsl_tpm_pwm_dev_t  __g_tpm0_pwm_dev;

/** \brief tpm0 Timer 实例初始化，获得Timer标准服务句柄 */
am_pwm_handle_t am_kl26_tpm0_pwm_inst_init (void)
{
    return am_fsl_tpm_pwm_init(&__g_tpm0_pwm_dev,
                               &__g_tpm0_pwm_devinfo);
}

/** \brief tpm0 pwm 实例解初始化 */
void am_kl26_tpm0_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_fsl_tpm_pwm_deinit(handle);
}

/**
 * @}
 */

/* end of file */
