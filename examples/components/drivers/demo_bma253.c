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
 * \brief BMA253 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_bma253.c src_bma253
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-07  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_bma253
 * \copydoc demo_bma253.c
 */

/** [src_bma253] */
#include "ametal.h"
#include "am_delay.h"
#include "am_bma253.h"
#include "am_hwconf_bma253.h"
#include "am_vdebug.h"


/**
 * \brief 例程入口
 */
void demo_bma253_entry (void)
{  
    int8_t temp = 0;
    int16_t accel[3];
    
    am_bma253_handle_t handle;

    handle = am_bma253_inst_init();    

    while (1) {
        
        /** \brief 读取温度 */
        am_bma253_read_temp(handle, &temp);
        AM_DBG_INFO("temperature = %d ℃\r\n", temp);
        
        /** \brief 读取三轴加速度 */
        am_bma253_read_accel (handle, accel);
        AM_DBG_INFO("accel_x = %d  accel_y = %d  accel_z = %d \r\n",
                    accel[0], accel[1], accel[2]);
        
        am_mdelay(500);
    }
}
/** [src_bma253] */

/* end of file */
