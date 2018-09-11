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
 * \brief 掉电检测例程，通过 HW 层接口实现
 *
 * - 实验步骤：
 *   1. 改变供电电压，使之低于 2.85V。
 *
 * - 实验现象：
 *   1. 若外部引脚引起的复位，串口输出 "Reset by External Reset Pin"；
 *   2. 若掉电检测引起的复位，串口输出 "Reset by BOD"；
 *   3. 正常供电下，LED0 不亮，若供电电压不断降低，低于 2.85V 时，LED0 亮。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_am824_hw_bod.c src_am824_hw_bod
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-17  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_bod
 * \copydoc demo_am824_hw_bod.c
 */

/** [src_am824_hw_bod] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_hw_bod_entry (void)
{
    am_kprintf("demo am824_core hw bod!\r\n");
    demo_lpc824_hw_bod_entry();
}
/** [src_am824_hw_bod] */

/* end of file */
