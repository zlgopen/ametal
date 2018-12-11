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
 * \brief KL26 TPM0用于捕获功能的用户配置文件
 * \sa am_kl26_hwconfig_tpm0_cap.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_fsl_tpm_cap.h"
#include "am_gpio.h"
#include "hw/amhw_kl26_sim.h"
#include "../../../../../soc/freescale/kl26/am_kl26.h"


/**
 * \addtogroup am_kl26_if_hwconfig_src_tpm0_cap
 * \copydoc am_kl26_hwconfig_tpm0_cap.c
 * @{
 */

/** \brief TPM0用于捕获功能的平台初始化 */
void __kl26_plfm_tpm0_cap_init (void)
{
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_TPM0);
    amhw_kl26_sim_tpm_src_set(KL26_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief  解除TPM0平台初始化 */
void __kl26_plfm_tpm0_cap_deinit (void)
{
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_TPM0);
}

/** \brief TPM0用于捕获功能的引脚配置信息列表 */
am_fsl_tpm_cap_ioinfo_t __g_tpm0_cap_ioinfo_list[] = {
    {PIOD_0, PIOD_0_TPM0_CH0, PIOD_0_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 0 */
    {PIOD_1, PIOD_1_TPM0_CH1, PIOD_1_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 1 */
    {PIOD_2, PIOD_2_TPM0_CH2, PIOD_2_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 2 */
    {PIOD_3, PIOD_3_TPM0_CH3, PIOD_3_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 3 */
    {PIOD_4, PIOD_4_TPM0_CH4, PIOD_4_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 4 */
    {PIOD_5, PIOD_5_TPM0_CH5, PIOD_5_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 5 */
};

/** \brief TPM0用于捕获功能的设备信息 */
const am_fsl_tpm_cap_devinfo_t  __g_tpm0_cap_devinfo = {
    KL26_TPM0,                      /**< \brief 指向TPM0寄存器块的指针 */
    INUM_TPM0,                      /**< \brief TPM0中断编号 */
    6,                              /**< \brief 6个捕获通道  */
    CLK_TPM0,                       /**< \brief 时钟号 */
    &__g_tpm0_cap_ioinfo_list[0],
    __kl26_plfm_tpm0_cap_init,      /**< \brief 平台初始化函数 */
    __kl26_plfm_tpm0_cap_deinit     /**< \brief 平台解初始化函数 */
};

/** \brief TPM0用于捕获功能的设备定义 */
am_fsl_tpm_cap_dev_t  __g_tpm0_cap_dev;

/** \brief tpm0 cap 实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_kl26_tpm0_cap_inst_init (void)
{
    return am_fsl_tpm_cap_init(&__g_tpm0_cap_dev,
                               &__g_tpm0_cap_devinfo);
}

/** \brief tpm0 cap 实例解初始化 */
void am_kl26_tpm0_cap_inst_deinit (am_cap_handle_t handle)
{
    am_fsl_tpm_cap_deinit(handle);
}

/**
 * @}
 */

/* end of file */
