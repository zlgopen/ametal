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
 * \brief STM32F103RBT6 模板工程
 *
 * - 实验现象:
 *   模板例程:LED0以1秒的频率闪烁; demo例程入口:详见demo入口函数文件介绍
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-07  nwt, first implementation
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

int am_main (void)
{
    AM_DBG_INFO("Start up successful!\r\n");

    /* demo例程入口 */

    while (1) {

        am_led_toggle(LED0);

        am_mdelay(1000);
    }
}

/* end of file */
