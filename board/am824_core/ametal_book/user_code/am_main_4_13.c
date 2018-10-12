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
 * \brief 程序清单4.13
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
    int i = 0;                     // 秒计数器清0

    digitron_init();
    while(1) {
        digitron_disp_num(0, i);   // com0显示i
        am_mdelay(1000);           // 延时1s
        i = (i + 1) % 10;          // 秒计数器+1计数，逢十进一
    }
}


/* end of file */

