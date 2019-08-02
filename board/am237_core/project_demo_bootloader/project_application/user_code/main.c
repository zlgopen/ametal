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
 * \brief zlg237 bootloader测试用的应用工程
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

    /* 单区bootloader 的测试应用程序 demo */
    //demo_zlg237_core_single_application_entry();

    /* 双区bootloader 的测试应用程序 demo */
    demo_zlg237_core_double_application_entry();

    /* 基于上位机的 bootloader 的测试应用程序 demo */
    //demo_zlg237_core_kft_application_entry();

    while (1) {

    }
}

/* end of file */
