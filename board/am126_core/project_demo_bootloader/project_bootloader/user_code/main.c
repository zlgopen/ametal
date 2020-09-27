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
 * \brief am116 bootloader测试用的应用工程
 *
 * \internal
 * \par Modification history
 * - 1.00 20-08-19  yrz, first implementation
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "demo_am116_core_entries.h"

int am_main (void)
{
    demo_am116_core_single_bootloader_uart_entry();

    while (1) {

    }
}

/* end of file */
