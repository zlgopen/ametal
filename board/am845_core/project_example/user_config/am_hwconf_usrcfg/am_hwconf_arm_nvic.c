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
 * \brief ARM NVIC 用户配置文件
 * \sa am_hwconf_arm_nvic.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-29  hbt, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_arm_nvic.h"

/**
 * \addtogroup am_if_src_hwconf_arm_nvic
 * \copydoc am_hwconf_arm_nvic.c
 * @{
 */

/** \brief 中断信息数量 */
#define __ISRINFO_COUNT    INUM_INTERNAL_COUNT

/** \brief 中断信息映射 */
am_local uint8_t __nvic_isr_map[INUM_INTERNAL_COUNT];

/** \brief 中断信息映射内存 */
am_local struct am_arm_nvic_isr_info __nvic_isr_infor[__ISRINFO_COUNT];

/** \brief 中断设备信息 */
am_local am_const am_arm_nvic_devinfo_t __g_arm_nvic_devinfo =
{
    {
        INUM_INTERNAL_MIN,   /* 起始中断号 */
        INUM_INTERNAL_MAX    /* 末尾中断号 */
    },

    AM_ARM_NVIC_CORE_M0PLUS, /* Cortex-M0+ 内核 */

    2,                       /* 仅有子优先级，且子优先级有 2 位 */
    0,                       /* 组中断 */

    INUM_INTERNAL_COUNT,     /* 总中断数量 */
    __nvic_isr_map,          /* 中断信息映射 */
    __ISRINFO_COUNT,         /* 中断信息数量 */
    __nvic_isr_infor,        /* 中断信息映射内存 */

    NULL,                    /* 无需平台初始化函数 */
    NULL                     /* 无需平台解初始化函数 */
};

/** \brief 中断设备实例 */
am_local am_arm_nvic_dev_t __g_arm_nvic_dev;

/**
 * \brief 中断实例初始化，初始化中断驱动
 */
int am_arm_nvic_inst_init (void)
{
    return am_arm_nvic_init(&__g_arm_nvic_dev, &__g_arm_nvic_devinfo);
}

/**
 * \brief 中断实例解初始化
 */
void am_arm_nvic_inst_deinit (void)
{
    am_arm_nvic_deinit();
}

/**
 * @}
 */

/* end of file */
