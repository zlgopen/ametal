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
 * \brief HC32F460 示例工程
 *
 * - 实验现象:
 *   打印 "Start up successful!"
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-10  cds, first implementation
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_hc32f460.h"
#include "demo_hc32f460_core_entries.h"


int am_main (void)
{
//    demo_hc32f460_core_std_gpio_entry();
//    demo_hc32f460_core_std_gpio_trigger_entry();
//    demo_hc32f460_core_hw_gpio_entry();
//    demo_std_uart_ringbuf_entry(am_hc32f460_uart4_inst_init());
//    demo_std_uart_polling_entry(am_hc32f460_uart4_inst_init());
//    demo_hc32f460_core_hw_uart_int_entry();
    AM_DBG_INFO("Start up successful!\r\n");
    while (1) {

    }
}

/* end of file */
