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
 * \brief book`s view_zigbee1.c file for chapter9
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "view_zigbee1.h"
#include "am_delay.h"
#include "string.h"
#include "model_temp_monitor.h"
#include "am_vdebug.h"

static void __view_zigbee_update (observer_t *p_view, model_t *p_model)
{
    model_temp_monitor_t *p_model_temp    = (model_temp_monitor_t *)p_model;
    view_zigbee_t        *p_view_zigbee   = (view_zigbee_t *)p_view;
    char     buf[100];
    int      status;
    float    cur, min, max;

    model_temp_monitor_cur_get (p_model_temp, &cur);
    model_temp_monitor_max_get (p_model_temp, &max);
    model_temp_monitor_min_get (p_model_temp, &min);
    status = model_temp_monitor_status_get(p_model_temp);
    am_snprintf(
        buf,
        100,
        "cur = %d.%02d, max = %d.%02d, min = %d.%02d, status = %d\r\n",
        (int)cur, abs(((int)((cur) * 100))) % 100,
        (int)max, abs(((int)((max) * 100))) % 100,
        (int)min, abs(((int)((min) * 100))) % 100,
        status);
    am_zm516x_send(p_view_zigbee->zm516x_handle, buf, strlen(buf));
}

int view_zigbee_init1 (view_zigbee_t *p_view_zigbee, am_zm516x_handle_t  zm516x_handle)
{
    am_zm516x_cfg_info_t   zm516x_cfg_info;
    if ((p_view_zigbee == NULL) || (zm516x_handle == NULL)) {
        return -1;
    }
    view_init(&(p_view_zigbee->isa), __view_zigbee_update);
    p_view_zigbee->zm516x_handle = zm516x_handle;
    // 获取ZM516X模块的配置信息
    if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        return AM_ERROR;
    }
    // 修改ZM516X模块的配置信息，本地地址： 0x2001，目标地址：0x2002
    zm516x_cfg_info.my_addr[0]  = 0x20;
    zm516x_cfg_info.my_addr[1]  = 0x01;
    zm516x_cfg_info.dst_addr[0] = 0x20;
    zm516x_cfg_info.dst_addr[1] = 0x02;
    if (am_zm516x_cfg_info_set(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        return AM_ERROR;
    }
    // 使ZM516X模块复位，以使设置生效
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);
    return 0;
}

/* end of file */
