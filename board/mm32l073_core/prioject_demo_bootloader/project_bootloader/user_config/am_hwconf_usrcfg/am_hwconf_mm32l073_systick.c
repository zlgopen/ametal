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
 * \brief MM32L073 SYSTICK 用户配置文件
 * \sa am_hwconf_mm32l073_systick.c
 *
 * \internal
 * \par Modification History
 * - 1.00 17-12-04  pea, first implementation
 * \endinternal
 */

#include "am_arm_systick.h"
#include "am_mm32l073.h"
#include "hw/amhw_arm_systick.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l073_systick
 * \copydoc am_hwconf_mm32l073_systick.c
 * @{
 */

/**
 * \brief 设备信息
 * \note 时钟源分为系统时钟（#AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM）和系统时钟的 1/2
 *       （#AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM_HALF）
 */
static const am_arm_systick_devinfo_t __g_systick_devinfo = {
    MM32L073_SYSTICK_BASE,                   /**< \brief 指向 SYSTICK 寄存器块指针 */
    CLK_SYSCFG,                            /**< \brief SYSTICK 时钟 ID */
    AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM, /**< \brief SYSTICK 时钟选择系统时钟 */

    NULL,                                  /**< \brief 无需平台初始化 */
    NULL                                   /**< \brief 无需平台去初始化 */
};

/** \brief 设备实例 */
static am_arm_systick_dev_t __g_systick_dev;
static am_timer_handle_t    __g_systick_timer_handle = NULL;

/** \brief SYSTICK 实例初始化，获得 TIMER 标准服务句柄 */
am_timer_handle_t am_mm32l073_systick_inst_init (void)
{
    if (__g_systick_timer_handle == NULL) {
        __g_systick_timer_handle = am_arm_systick_init(&__g_systick_dev,
                                                       &__g_systick_devinfo);
    }

    return __g_systick_timer_handle;
}

/** \brief SYSTICK 实例解初始化 */
void am_mm32l073_systick_inst_deinit (am_timer_handle_t handle)
{
    if (__g_systick_timer_handle == handle) {
        am_arm_systick_deinit(handle);
        __g_systick_timer_handle = NULL;
    }

}

/**
 * @}
 */

/* end of file */
