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
 * \brief 程序清单4.49
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "hc595.h"
#include "am_delay.h"

int am_main(void)
{
    uint8_t data = 0x01;                      // 初始化bit0为1，表示点亮LED0

    hc595_init();
    while(1) {
        hc595_send_data(~data);               // data取反点亮LED
        am_mdelay(100);
        data <<= 1;
        if (data == 0) {                      // 8次循环结束，重新从0x01开始
            data = 0x01;
        }
    }
}



/* end of file */

