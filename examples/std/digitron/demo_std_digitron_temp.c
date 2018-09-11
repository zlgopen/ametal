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
 * \brief 数码管实时显示温度例程
 *
 * 本应用需要使用到两位数码管（仅显示温度整数部分），指定的数码管显示器必须至少
 * 包含两位数码管
 *
 * - 操作步骤
 *   1. 调用入口函数，传入指定的数码管显示器编号和温度传感器句柄
 *
 * - 实验现象：
 *   1. 可以看到数码管显示当前温度
 *
 * \par 源代码
 * \snippet demo_std_digitron_temp.c src_std_digitron_temp
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_digitron_temp
 * \copydoc demo_std_digitron_temp.c
 */

/** [src_std_digitron_temp] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_digitron_disp.h"
#include "am_temp.h"

/**
 * \brief 例程入口 
 */
void demo_std_digitron_temp_entry (int32_t id, am_temp_handle_t temp_handle)
{
    int32_t temp;

    /* 初始化，设置8段ASCII解码 */
    am_digitron_disp_decode_set(id, am_digitron_seg8_ascii_decode);

    /* 读取温度，扩大了1000倍,由于只有两个数码管，只显示整数部分 */
    am_temp_read(temp_handle, &temp);

    am_digitron_disp_char_at(id, 1, '0' + (temp / 1000) % 10);       /* 个位数码管显示字符 */
    am_digitron_disp_char_at(id, 0, '0' + (temp / 1000) / 10 % 10);  /* 十位数码管显示字符 */

    while(1) {

        am_mdelay(500);

        /* 读取温度，扩大了 1000 倍,由于只有两个数码管，只显示整数部分 */
        am_temp_read(temp_handle, &temp);

        am_digitron_disp_char_at(id, 1, '0' + (temp / 1000) % 10);       /* 个位数码管显示字符 */
        am_digitron_disp_char_at(id, 0, '0' + (temp / 1000) / 10 % 10);  /* 十位数码管显示字符 */
    }
}
/** [src_std_digitron_temp] */

/* end of file */
