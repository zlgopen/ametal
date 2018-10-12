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
 * \brief 程序清单 ntc test
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-18  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "digitron1.h"
#include "ntc.h"
#include "am_delay.h"
#include "am_vdebug.h"

int am_main(void)
{
    int16_t temp;                                       // 保存温度值
    int i = 0;

    digitron1_hc595_init();
    ntc_init();
    while(1) {
        temp = ntc_temp_read();                         // 读取温度值
        am_kprintf("temp;%d \r\n",((temp >> 8)));
        if (temp < 0)
            temp = -1 * temp;                       // 温度为负时，也只显示温度数值
        digitron1_disp_num_set(0, (temp >> 8) / 10); // 显示温度整数部分的十位
        digitron1_disp_num_set(1, (temp >> 8) % 10); // 显示温度小数部分的十位
        for (i = 0; i < 100; i++) {
            // 循环100次，耗时500ms,使温度值以500ms的时间间隔更新
            digitron1_hc595_disp_scan();
            am_mdelay(5);
        }
    }
}



/* end of file */

