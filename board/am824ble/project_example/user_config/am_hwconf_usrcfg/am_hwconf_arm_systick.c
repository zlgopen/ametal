/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ARM Systick 用户配置文件
 * \sa am_hwconf_arm_systick.c
 *
 * \internal
 * \par Modification History
 * - 1.00 15-08-01  win, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_arm_systick.h"
#include "am_lpc82x.h"
#include "hw/amhw_arm_systick.h"

/**
 * \addtogroup am_if_src_hwconf_arm_systick
 * \copydoc am_hwconf_arm_systick.c
 * @{
 */

/**
 * \brief Systick 设备信息
 *
 * \note 时钟源分为系统时钟 (#AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM) 和系统
 *       时钟的 1/2 (#AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM_HALF)
 */
am_local am_const am_arm_systick_devinfo_t __g_arm_systick_devinfo = {
    LPC82X_SYSTICK_BASE,                      /* 指向 Systick 寄存器块指针 */
    CLK_SYSTEM,                               /* Systick 时钟号，来源于主时钟 */
    AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM,    /* Systick 时钟选择系统时钟 */
    NULL,                                     /* 无需平台初始化函数 */
    NULL                                      /* 无需平台解初始化函数 */
};

/** \brief Systick 设备实例 */
am_local am_arm_systick_dev_t __g_arm_systick_dev;

/**
 * \brief Systick 实例初始化
 */
am_timer_handle_t am_arm_systick_inst_init (void)
{
    return am_arm_systick_init(&__g_arm_systick_dev, &__g_arm_systick_devinfo);
}

/**
 * \brief Systick 实例解初始化
 */
void am_arm_systick_inst_deinit (am_timer_handle_t handle)
{
    am_arm_systick_deinit(handle);
}

/**
 * @}
 */

/* end of file */
