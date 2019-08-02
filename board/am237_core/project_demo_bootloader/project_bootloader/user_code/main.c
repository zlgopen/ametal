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
 * \brief zlg237 bootloader 示例工程
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-027  yrh, first implementation
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
#include "demo_am237_core_entries.h"

int am_main (void)
{

    AM_DBG_INFO("Start up successful!\r\n");

    /* 单区bootloader demo */
    //demo_zlg237_core_single_bootloader_uart_entry();

    /* 双区bootloader demo */
    demo_zlg237_core_double_bootloader_uart_entry();

    /* 基于上位机的 bootloader demo */
    //demo_zlg237_core_bootloader_kft_entry();
    while (1) {

    }
}

/* end of file */
