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
 * \brief 中断用户配置文件
 * \sa am_hwconf_arm_nvic.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-29  hbt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_mm32l073.h"
#include "am_arm_nvic.h"

/**
 * \addtogroup am_if_src_hwconf_arm_nvic
 * \copydoc am_hwconf_arm_nvic.c
 * @{
 */

/**
 * \brief 中断优先级位数及分组
 *        M0优先级位数为2位，全部用于设置抢占优先级
 */
#define __NVIC_PRIORITY_BITS       2            /**< \brief 芯片支持的优先级位数 */
#define __NVIC_PRIORITY_GROUP      0            /**< \brief NVIC中断优先级分组 */


/** 
 * \brief ISR 信息数量 
 *        该数量默认定义为MCU所支持的最大向量中断个数，
 *        用户可以根据应用中实际所需要用到的中断个数
 *        来修改该宏值，以达到减少不必要的内存浪费的目的。
 */
#define __ISRINFO_COUNT    INUM_INTERNAL_COUNT

/**
 * \brief 存储用户中断回调信息
 */
static struct am_arm_nvic_isr_info __nvic_isr_infor[__ISRINFO_COUNT];

/**
 * \brief 存储中断编号与其对应在__nvic_isr_infor中断回调信息中
 *        位置的映射，数组大小一般与MCU所支持的最大向量中断个数相等。
 */
static uint8_t __nvic_isr_map[INUM_INTERNAL_COUNT];
 

/** \brief 中断设备信息 */
static const am_arm_nvic_devinfo_t __g_nvic_devinfo =
{
    {
        INUM_INTERNAL_MIN, /**< \brief 中断起始号 */
        INUM_INTERNAL_MAX  /**< \brief 中断末尾号 */
    },                    

    AM_ARM_NVIC_CORE_M0,   /**< \brief 内核 */

	__NVIC_PRIORITY_BITS,  /**< \brief 优先级位数 */
	__NVIC_PRIORITY_GROUP, /**< \brief 设置组中断 */

    INUM_INTERNAL_COUNT,   /**< \brief 总中断数量 */
    __nvic_isr_map,        /**< \brief ISR 信息映射(大小与 input_cnt 一致) */
    __ISRINFO_COUNT,       /**< \brief ISR 信息数量 */
    __nvic_isr_infor,      /**< \brief ISR 信息映射内存(大小与 isrinfo_cnt 一致) */

    NULL,                  /**< \brief 无需平台初始化 */
    NULL                   /**< \brief 无需平台去初始化 */
};

/** \brief 中断设备实例 */
static am_arm_nvic_dev_t __g_nvic_dev;

/**
 * \brief 中断实例初始化，初始化中断驱动
 */
int am_mm32l073_nvic_inst_init (void)
{
    return am_arm_nvic_init(&__g_nvic_dev, &__g_nvic_devinfo);
}

/** 
 * \brief 中断实例解初始化
 */
void am_mm32l073_nvic_inst_deinit (void)
{
    am_arm_nvic_deinit();
}

/**
 * @}
 */

/* end of file */
