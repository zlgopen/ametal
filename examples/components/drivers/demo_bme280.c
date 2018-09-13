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
 * \brief BME280 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_bme280.c src_bme280
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-11  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_bme280
 * \copydoc demo_bme280.c
 */

/** [src_bme280] */
#include "ametal.h"
#include "am_delay.h"
#include "am_bme280.h"
#include "am_hwconf_bme280.h"
#include "am_vdebug.h"


/**
 * \brief 例程入口
 */
void demo_bme280_entry (void)
{  
    am_bme280_handle_t handle;
    int8_t   temp  = 0;
    uint32_t press = 0;
    uint8_t  hum   =  0;
    handle = am_bme280_inst_init();
    
    while (1) {

        am_bme280_read_temp (handle, &temp);
        am_bme280_read_press (handle, &press);
        am_bme280_read_hum (handle, &hum);

        AM_DBG_INFO("temp  = %d ℃  \r\n",temp); 
        AM_DBG_INFO("press = %d Pa  \r\n",press);
        AM_DBG_INFO("hum   = %d rH  \r\n",hum);
        
        am_mdelay(500);
    }
}
/** [src_bme280] */

/* end of file */
