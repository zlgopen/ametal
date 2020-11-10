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
 * \brief am42x bootloader测试用的应用工程
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-13  zp, first implementation
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
#include "demo_am42x_core_entries.h"
#include "am_zsl42x.h"
#include "am_zsl42x_flash.h" 
#include "am_int.h"
int am_main (void)
{

    AM_DBG_INFO("Start up successful!\r\n");

    /* 单区bootloader 的测试应用程序 demo */
    //demo_zsl42x_core_single_application_entry();

    /* uart 双区bootloader 的测试应用程序 demo */
    demo_zsl42x_core_double_application_entry();


    while (1) {

    }
}

/* end of file */
