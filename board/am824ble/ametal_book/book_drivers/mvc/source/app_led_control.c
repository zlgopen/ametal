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
 * - 1.00 18-09-13  ipk, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_zm516x.h"
#include "string.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_input.h"
#include "app_led_control.h"

static void __input_key_proc (void *p_arg, int key_code, int key_state)
{
	    //按键按下，发送"key_pressed"字符串
	    if ((key_code == KEY_F1) && (key_state == AM_INPUT_KEY_STATE_PRESSED)) {
	        am_zm516x_send(p_arg, "key_pressed", strlen("key_pressed"));
	    }
}

int app_led_control (am_zm516x_handle_t zm516x_handle, uint16_t my_addr, uint16_t dst_addr)
{
		uint8_t                	buf[20];
		am_zm516x_cfg_info_t    zm516x_cfg_info;
		am_input_key_handler_t  key_handler;
		
    // 获取ZM516X模块的信息
    if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        return AM_ERROR;
    }
		
    //修改ZM516X模块的信息
    zm516x_cfg_info.my_addr[0]  	= (my_addr >> 8) & 0xFF;
    zm516x_cfg_info.my_addr[1]  	= my_addr & 0xFF;
    zm516x_cfg_info.dst_addr[0] 	= (dst_addr >> 8) & 0xFF;
    zm516x_cfg_info.dst_addr[1] 	= dst_addr & 0xFF;
		
    if (am_zm516x_cfg_info_set(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        return AM_ERROR;
    }
    //使ZM516X模块复位，以使设置生效
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);
	  //注册按键处理函数
		am_input_key_handler_register(&key_handler, __input_key_proc, (void *)zm516x_handle);
	  while (1) {
        memset(buf, 0, sizeof(buf));
        am_zm516x_receive(zm516x_handle, buf, sizeof(buf)); 
        if (strcmp((const char *)buf, "key_pressed") == 0) {  // 若收到"key_pressed",则翻转LED0的状态
            am_led_toggle(0);
        }
        am_mdelay(10);
    }
}

/* end of file */
