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
 * \brief amf07x bootloader测试用的应用工程
 *
 * \internal
 * \par Modification history
 * - 1.00 20-8-20  xjy, first implementation
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_softimer.h"
#include "demo_amf07x_core_entries.h"

int am_main (void)
{
    AM_DBG_INFO("Start up successful!\r\n");

    /* UART  单区bootloader demo */
    demo_hc32f07x_core_single_bootloader_uart_entry();

    /* UART  双区bootloader demo */
//    demo_hc32f07x_core_double_bootloader_uart_entry();

    while (1) {

    }
}

/* end of file */
