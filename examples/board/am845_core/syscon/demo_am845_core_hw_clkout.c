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
 * \brief 时钟频率输出例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印 "The CLKOUT  rate is  xxx Hz\r\n"，xxx 为当前频率；
 *   2. PIO0_27 引脚（默认配置 CLKOUT 引脚输出）输出当前时钟频率 2.5MHz；
 *   3. LED0 以 0.2s 的时间间隔闪烁。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_clkout.c src_am845_core_hw_clkout
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_clkout
 * \copydoc demo_am845_core_hw_clkout.c
 */

/** [src_am845_core_hw_clkout] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_clkout_entry (void)
{

    demo_lpc845_hw_clkout_entry();
}
/** [src_am845_core_hw_clkout] */

/* end of file */
