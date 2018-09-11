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
 * \brief The KL26 interrupt user configure
 * \sa am_hwconf_arm_nvic.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-01-29  hbt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_kl26.h"
#include "am_arm_nvic.h"

/**
 * \addtogroup am_if_hwconf_arm_src_nvic
 * \copydoc am_hwconf_arm_nvic.c
 * @{
 */

/** \brief ISR information count */
#define __ISRINFO_COUNT    INUM_INTERNAL_COUNT

/** \brief Support interrupt count */
static struct am_arm_nvic_isr_info __nvic_isr_infor[__ISRINFO_COUNT];


/** \brief Interrupt map */
uint8_t __nvic_isr_map[INUM_INTERNAL_COUNT];


/** \brief Interrupt device information */
const am_arm_nvic_devinfo_t __g_nvic_devinfo =
{
    {
        INUM_INTERNAL_MIN,  /**< \brief Interrupt start number */
        INUM_INTERNAL_MAX   /**< \brief Interrupt end number   */
    },

	AM_ARM_NVIC_CORE_M0PLUS,

#ifdef AM_CORTEX_M4
    /* 代表主优先级有3位，子优先级有0位 */
    3,                      /**< \brief 优先级位数 */
    4,                      /**< \brief 设置组中断 */
#endif

#ifdef AM_CORTEX_M0
    /*仅有子优先级，且子优先级有2位 */
    2,                      /**< \brief 优先级位数 */
    0,                      /**< \brief 设置组中断 */
#endif

    INUM_INTERNAL_COUNT,    /**< \brief 总中断数量 */
    __nvic_isr_map,         /**< \brief ISR information map(size equal to input_cnt) */
    __ISRINFO_COUNT,        /**< \brief ISR information count */
    __nvic_isr_infor,       /**< \brief ISR information map memory(size equal to isrinfo_cnt) */

    NULL,                   /**< \brief 无需平台初始化 */
    NULL                    /**< \brief 无需平台去初始化 */
};

/** \brief Interrupt device */
static am_arm_nvic_dev_t __g_nvic_dev;

/**
 * \brief  中断实例初始化，初始化中断驱动
 */
int am_kl26_nvic_inst_init (void)
{
    return am_arm_nvic_init(&__g_nvic_dev, &__g_nvic_devinfo);
}

/**
 * \brief  中断实例解初始化
 */
void am_kl26_nvic_inst_deinit (void)
{
    am_arm_nvic_deinit();
}


/**
 * @}
 */

/* end of file */
