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
 * \brief MK100 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过0.1秒，MK100角度、磁旋钮位置、磁场强度阈值、方向传感器数据。红外传感器数据
 *
 * \par 源代码
 * \snippet demo_mk100.c src_mk100
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-20  mxy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_mk100
 * \copydoc demo_mk100.c
 */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mk100.h"
#include "am_delay.h"

#define  REPORT  0

/**
 * \brief 例程入口
 */
void demo_mk100_entry(am_mk100_handle_t handle)
{
    uint8_t upper_limit = 0, lower_limit = 0;
    int ret = 0;

#if REPORT
    uint8_t receive_cmd_buff[AM_MK100_FRAME_LEN] = {0};    
    
    /*设置定时上报数据*/
    ret = am_mk100_set_report_change_mode(handle);
    if (ret < 0) {
        AM_DBG_INFO("am_mk100_set_report_change_mode error!\r\n");
        am_mk100_display_error_recevice(handle);
    } else {
        AM_DBG_INFO("am_mk100_set_report_change_mode: %d\r\n", ret);
    }
#endif
    
    AM_FOREVER {

#if REPORT
        /*获取定时上报的数据，不需要延时*/
        am_mk100_get_report(handle, receive_cmd_buff, AM_MK100_FRAME_LEN);
        AM_DBG_INFO("recevice report: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\r\n",
                receive_cmd_buff[0], receive_cmd_buff[1], receive_cmd_buff[2], receive_cmd_buff[3], receive_cmd_buff[4]);
#endif
        ret = am_mk100_get_angle(handle, 0xFF);
        if (ret < 0) {
            AM_DBG_INFO("am_mk100_get_angle error!\r\n");
            am_mk100_display_error_recevice(handle);
        } else {
            AM_DBG_INFO("am_mk100_get_angle: %d\r\n", ret);
        }
        ret = am_mk100_get_knob(handle);
        if (ret < 0) {
            AM_DBG_INFO("am_mk100_get_knob error!\r\n");
            am_mk100_display_error_recevice(handle);
        } else {
            AM_DBG_INFO("am_mk100_get_knob: %d\r\n", ret);
        }
        ret = am_mk100_get_strength_threshold(handle, &upper_limit, &lower_limit);
        if (ret < 0) {
            AM_DBG_INFO("am_mk100_get_strength_threshold error!\r\n");
            am_mk100_display_error_recevice(handle);
        } else {
            AM_DBG_INFO("am_mk100_get_strength_threshold: upper_limit：%d lower_limit: %d\r\n", upper_limit, lower_limit);
        }
        ret = am_mk100_get_direction_data(handle, AM_MK100_DIRECTION_UP);
        if (ret < 0) {
            AM_DBG_INFO("am_mk100_get_direction_data error!\r\n");
            am_mk100_display_error_recevice(handle);
        } else {
            AM_DBG_INFO("am_mk100_get_direction_data: %d\r\n", ret);
        }
        ret = am_mk100_get_infrared_data(handle);
        if (ret < 0) {
            AM_DBG_INFO("am_mk100_get_infrared_data error!\r\n");
            am_mk100_display_error_recevice(handle);
        } else {
            AM_DBG_INFO("am_mk100_get_infrared_data: %d\r\n", ret);
        }

        am_mdelay(100);
    }
}
/* end of file */
