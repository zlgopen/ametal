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
 * \brief 程序清单4.14
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
    digitron_init();
    while(1) {
        digitron_disp_num(0, 1);             // com0显示1
        am_mdelay(5);                        // 延时5ms
        digitron_disp_num(1, 2);             // com1号显示2
        am_mdelay(5);                        // 延时5ms
    }
}



/* end of file */

