/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief PT100阻值温度转换
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-29  tee, first implementation.
 * \endinternal
 */

#include <stdio.h>
#include "am_pt100_to_temperature.h"
#include "am_pt100_temp_res_table.h"
 
/* PT100的电阻测量值计算温度值 */
float pt100_to_temperature (float pt100)
{
    const unsigned int table_size = sizeof(g_pt100_table) / sizeof(g_pt100_table[0]);
    
    unsigned int   pt100_x100 = 0;
    unsigned int   index      = 0;
    int            dif        = 0;
    float          dec        = 0;
 
    int low, high, mid;                          /* 用于二分法查找            */
 
    pt100_x100 = (unsigned int)(pt100 * 100);    /* 阻值扩大100倍后的整数部分 */

    /* 判断是否超出范围 */
    if (pt100_x100 < g_pt100_table[0]) {
        return -200.0f;
    } else if (pt100_x100 >= g_pt100_table[table_size - 1]) {
        return 850.0f;
    }
    
    dec = pt100 * 100 - pt100_x100;
 
    low  = 0;
    high = table_size - 1;
 
    while ((high - low) != 1) {                /* 直到温度范围缩小为1度  */

        mid = (low + high) / 2;

        if (pt100_x100 < g_pt100_table[mid]) {
            high = mid;
        } else if (pt100_x100 > g_pt100_table[mid]) {
            low  = mid;
        } else {
            index = mid;
            dif    = 0;
            break;
        }
    }
    
    if ((high - low) == 1) {
        
        index = low;
        dif   = pt100_x100 - g_pt100_table[low];
    }

    /* 表格分辨率为1度，计算落入区间的比例 */
    dec = 1.0f * (dif + dec) / (g_pt100_table[index + 1] - g_pt100_table[index]);

    /* 起始温度为 -200 */
    return index + dec - 200.0f;
}

/* end of file */
