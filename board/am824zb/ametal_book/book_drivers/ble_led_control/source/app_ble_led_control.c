/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s app_ble_led_control driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_zlg9021.h"
#include "string.h"
#include "am_led.h"


int app_ble_led_control (am_zlg9021_handle_t zlg9021_handle)
{
    uint8_t  buf[4];
    // 复位ZLG9021
    am_zlg9021_ioctl(zlg9021_handle, AM_ZLG9021_RESET, NULL);
    // 设置超时时间为100ms
    am_zlg9021_ioctl(zlg9021_handle, AM_ZLG9021_TIMEOUT, (void*)100);
    while(1) {
        memset(buf, '\0', 4);                       	      // 清空buf中的内容，全部设置为 '\0'
        am_zlg9021_recv(zlg9021_handle, buf, 3);          	// 接收3个字节数据
        if (strcmp((char *)buf, "on") == 0) {               // 控制字符串 "on"
       		am_led_on(0);
            	am_zlg9021_send(zlg9021_handle, (uint8_t *)"ok!", 3);  // 回复"ok!"
        }
        if (strcmp((char *)buf, "off") == 0) {                  		 // 控制字符串 "off"
			  am_led_off(0);
         		am_zlg9021_send(zlg9021_handle, (uint8_t *)"ok!", 3);	   // 回复"ok!"
        	}
       	if (strcmp((char *)buf, "tog") == 0) {                  	   // 控制字符串 "tog"
            	am_led_toggle(0);
            	am_zlg9021_send(zlg9021_handle, (uint8_t *)"ok!", 3);	 // 回复"ok!"
        }
    }
}

/* end of file */
