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
 * \brief 程序清单4.15
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "digitron0.h"

int am_main (void)
{
    int i        = 0;
    int num      = 0;

    digitron_init();                       // 板级初始化
    while(1) {
        digitron_segcode_set (0xFF);       // 消影--熄灭全部数码管
        digitron_disp_num(0, num / 10);    // com0显示num的十位
        am_mdelay(5);                      // 延时5ms
        digitron_segcode_set (0xFF);       // 消影--熄灭全部数码管
        digitron_disp_num(1, num % 10);    // com1显示num的个位
        am_mdelay(5);
        i++;
        if (i == 100) {                    // 循环100次即运行时间达到1s
            i = 0;
            num = (num + 1) % 60;          // num数值从0 ~ 59循环
        }
    }
}


/* end of file */

