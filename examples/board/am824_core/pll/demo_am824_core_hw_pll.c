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
 * \brief PLL 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 打印 PLL 期望频率、实际输出频率和 CLKOUT 引脚输出频率；
 *   2. CLKOUT 引脚(PIO0_27 引脚)输出 __CLKOUT_DIV 分频后的主时钟频率；
 *   3. LED0 以 0.5s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_hw_pll.c src_am824_hw_pll
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-20  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_pll
 * \copydoc demo_am824_hw_pll.c
 */

/** [src_am824_hw_pll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_nxp_entries.h"


/**
 * \brief 例程入口
 */
void demo_am824_core_hw_pll_entry (void)
{
    am_kprintf("demo am824 hw pll! \r\n");
  
    demo_lpc824_hw_pll_entry();
}
/** [src_am824_hw_pll] */

/* end of file */
