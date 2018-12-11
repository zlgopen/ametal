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
 * \brief KL26 TSI 用户配置文件。
 * \sa am_kl26_hwconfig_tsi.c
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-26  nwt, make some changes.
 * - 1.00 15-10-22  wxj, first implementation.
 * \endinternal
 */

#include "am_kl26.h"
#include "am_fsl_tsi.h"
#include "hw/amhw_fsl_tsi.h"
#include "kl26_periph_map.h"
#include "hw/amhw_kl26_sim.h"
#include "am_gpio.h"


/**
 * \addtogroup am_kl26_if_hwconfig_src_tsi
 * \copydoc am_kl26_hwconfig_tsi.c
 * @{
 */

/** \brief tsi平台初始化 */
static void __plfm_tsi_init (void)
{
    /* 使能tsi相关外设时钟 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_TSI);

    /* 配置PIOA_1引脚为TSI0_CH2 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_TSI0_CH2);
}

/** \brief tsi平台去初始化 */
static void __plfm_tsi_deinit (void)
{
    /* 禁能tsi相关外设时钟 */
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_TSI);
}

/** \brief tsi设备信息     */
static const am_fsl_tsi_devinfo_t __g_tsi_devinfo = {
    KL26_TSI,       /**< \brief 指向tsi端口寄存器块的指针 */
    AM_FALSE,               /**< \brief 不启用低功耗中断唤醒MCU功能 */
	INUM_TSI0,
    __plfm_tsi_init,     /**< \brief TSI平台初始化.  */
    __plfm_tsi_deinit    /**< \brief TSI平台去初始化.  */
};

/** \brief tsi设备实例   */
static am_fsl_tsi_dev_t __g_tsi_dev;

/**
 * \brief TSI实例初始化
 */
int am_kl26_tsi_inst_init (void)
{
    return am_fsl_tsi_init(&__g_tsi_dev, &__g_tsi_devinfo);
}

/**
 * \brief TSI实例解初始化
 */
void am_kl26_tsi_inst_deinit (void)
{
    am_fsl_tsi_deinit(&__g_tsi_dev);
}


/**
 * @}
 */

/* end of file */
