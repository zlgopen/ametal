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
 * \brief fm175xx对CPU卡进行测试，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *   2. 正确连接好天线；
 *   3. 将CPU卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 将卡片放置天线感应区后串口打印出卡片的ID；
 *   2. 如果放置的卡片是CPU卡，将打印ATS和COS指令的操作结果。
 *
 * \par 源代码
 * \snippet demo_am116_dr_fm175xx_cpu_card.c src_am116_dr_fm175xx_cpu_card
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-17  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_cpu_card
 * \copydoc demo_am116_dr_fm175xx_cpu_card.c
 */

/** [src_am116_dr_fm175xx_cpu_card] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 读CPU卡例程
 */
void demo_zlg237_core_dr_fm175xx_cpu_card (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_cpu_card(handle);
}

/** [src_am116_dr_fm175xx_cpu_card] */

/* end of file */
