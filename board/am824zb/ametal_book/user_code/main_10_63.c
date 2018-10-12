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
 * \brief 程序清单 10.63
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_digitron_disp.h"
#include "app_temp_monitor_main.h"
#include "am_lpc82x_inst_init.h"
#include "am_hwconf_lm75.h"
#include "am_hwconf_miniport_view_key.h"

int am_main (void)
{
		am_zm516x_handle_t zm516x_handle = am_zm516x_inst_init();
		am_temp_handle_t   temp_handle  = am_temp_lm75_inst_init();
		// 初始化，并设置8段ASCII解码
		am_miniport_view_key_inst_init();
		am_digitron_disp_decode_set(0, am_digitron_seg8_ascii_decode);
		return app_temp_monitor_main(zm516x_handle, temp_handle);
}

/* end of file */

