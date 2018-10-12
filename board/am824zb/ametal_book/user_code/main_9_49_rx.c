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
 * \brief 程序清单 9.49 rx
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  ipk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zm516x.h"
#include "string.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_lpc82x_inst_init.h"

int am_main (void)
{
		uint8_t                  buf[20];
		uint32_t                 snd_tick = 0;
		am_zm516x_cfg_info_t     zm516x_cfg_info;
		am_zm516x_handle_t     	 zm516x_handle= am_zm516x_inst_init();
	
    //获取ZM516X模块的配置信息
		if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
			while (1);
		}

		//修改ZM516X模块的配置信息
  	zm516x_cfg_info.my_addr[0] = 0x20;
  	zm516x_cfg_info.my_addr[1] = 0x02;    //另一个模块设置为0x01
  	zm516x_cfg_info.dst_addr[0] = 0x20;
  	zm516x_cfg_info.dst_addr[1] = 0x01;   //另一个模块设置为0x02

		if (am_zm516x_cfg_info_set(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
			while (1);
		}

		am_zm516x_reset(zm516x_handle);
		am_mdelay(10);
		while (1) {

			//间隔1S发送一次数据
			if (snd_tick++ > 100) {
				snd_tick = 0;
				am_zm516x_send(zm516x_handle, "zm516x running\r\n", strlen("zm516x running\r\n"));
			}

			//am_zm516x_receive函数的读超时为10ms
			if (am_zm516x_receive(zm516x_handle, buf, sizeof(buf))> 0) {
				am_kprintf("%s", buf);
			}
		}
//		return 0;
	}
/* end of file */
