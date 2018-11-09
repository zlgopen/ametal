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
 * \brief 程序清单7.40
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "app_key_code_led_show.h" 

int am_main (void)
{
    am_miniport_key_inst_init();   // MiniPort-ZLG72128实例初始化
    app_key_code_led_show();       // 使用LED0和LED1对按键编码进行二进制显示
    while (1) {
    }
}



/* end of file */

