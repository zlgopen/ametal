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
 * \brief BMP280 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_bmp280.c src_bmp280
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-11  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_bmp280
 * \copydoc demo_bmp280.c
 */

/** [src_bmp280] */
#include "ametal.h"
#include "am_delay.h"
#include "am_bmp280.h"
#include "am_hwconf_bmp280.h"
#include "am_vdebug.h"


/**
 * \brief 例程入口
 */
void demo_bmp280_entry (void)
{  
    am_bmp280_handle_t handle;
    int8_t   temp  = 0;
    uint32_t press = 0;
    
    handle = am_bmp280_inst_init();
    
    while (1) {
        
        am_bmp280_read_temp (handle, &temp);
        am_bmp280_read_press (handle, &press);
        
        AM_DBG_INFO("temp  = %d ℃  \r\n",temp); 
        AM_DBG_INFO("press = %d Pa  \r\n",press);
        
        am_mdelay(500);
    }
}
/** [src_bmp280] */

/* end of file */
