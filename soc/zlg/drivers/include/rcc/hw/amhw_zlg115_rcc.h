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
 * \brief RCC 操作接口
 *
 *  驱动直接使用了 ZLG116 外设寄存器基地址， 因此，该驱动仅适用于 ZLG116
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-11  sdy, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG115_RCC_H
#define __AMHW_ZLG115_RCC_H

#include "am_types.h"
#include "zml165_regbase.h"

/**
 * \brief 系统时钟源
 */
typedef enum {
    AMHW_ZLG115_SYSCLK_HSI_DIV6,  /**< \brief HSI 振荡器  6 分频作为系统时钟 */
    AMHW_ZLG115_SYSCLK_HSE,       /**< \brief HSE 作为系统时钟 */
    AMHW_ZLG115_SYSCLK_HSI,       /**< \brief PLL 输出作为系统时钟 */
    AMHW_ZLG115_SYSCLK_LSI,       /**< \brief LSI 输出作为系统时钟 */
}amhw_zlg115_sys_clk_src;


#endif

/* end of file */
