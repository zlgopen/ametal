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
 * \brief PCF8551例程，通过标准接口实现
 *
 * - 实验现象：
 *   LCD循环显示0-9
 *
 * \par 源代码
 * \snippet demo_std_pcf8551.c src_std_pcf8551
 *
 * \internal
 * \par Modification history
 * - 1.00 20-02-25  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_pcf8551
 * \copydoc demo_std_pcf8551.c
 */
 
#include "ametal.h"
#include "am_vdebug.h"
#include "am_pcf8551.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_pcf8551_entry (am_pcf8551_handle_t handle)
{
    /* LCD 0-9段码表*/
    uint8_t seg_code[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 
                           0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    int i; 

    /* 打开显示 */
    am_pcf8551_display_on(handle);

    while(1) {
        
        for(i = 0; i < 10; i++) {
            am_pcf8551_display(handle, 0x14, seg_code, 1);
            am_mdelay(1000);
        }
    }
}

/* end of file */
