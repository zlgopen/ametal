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
 * \brief 
 *
 * \internal
 * \par Modification History
 * - 1.00 18-09-26  ipk, first implementation.
 * \endinternal
 */
 
#include "view_zigbee.h"
#include "am_delay.h"
#include "string.h"

static void __view_zigbee_update (observer_t *p_view, model_t *p_model)
{
	  view_zigbee_t 	*p_view_zigbee = (view_zigbee_t *)p_view;
	  am_bool_t   	   value;
	
	  if (model_bool_get((model_bool_t *)p_model, &value) == 0) {
			if (value) {
			    am_zm516x_send(p_view_zigbee->zm516x_handle, "1", strlen("1"));
			} else {
			    am_zm516x_send(p_view_zigbee->zm516x_handle, "0", strlen("0"));
			}
	  }
}

int view_zigbee_init (view_zigbee_t *p_view_zigbee, am_zm516x_handle_t  zm516x_handle)
{
    am_zm516x_cfg_info_t   zm516x_cfg_info;
    if ((p_view_zigbee == NULL) || (zm516x_handle == NULL)) {
        return -1;
    }
    p_view_zigbee->zm516x_handle = zm516x_handle;
		
    //获取ZM516X模块的配置信息
    if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        return AM_ERROR;
    }
    //修改ZM516模块的配置信息，本地地址：0x2001，目标地址:0x2002
    zm516x_cfg_info.my_addr[0]  	= 0x20;
    zm516x_cfg_info.my_addr[1]  	= 0x01;
    zm516x_cfg_info.dst_addr[0] 	= 0x20;
    zm516x_cfg_info.dst_addr[1] 	= 0x02;
		
    if (am_zm516x_cfg_info_set(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        return AM_ERROR;
    }
    //使ZM516X模块复位，以使设置生效
    am_zm516x_reset(zm516x_handle);
		
    am_mdelay(10);
    view_init(&(p_view_zigbee->isa), __view_zigbee_update);
    return 0;
}
/* end of file */
