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
 * \brief 程序清单6.103
 *
 * \note 该历程需要用到第三方库，hash table
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-25  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_zlg600.h"
#include "am_hwconf_zlg600.h"
uint8_t g_key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int am_main (void)
{
    	am_zlg600_handle_t zlg600_handle = am_zlg600_uart_inst_init();
    
    	am_zlg600_ic_key_load (
		zlg600_handle,
		AM_ZLG600_IC_KEY_TYPE_A,		// 本次保存的是A类密钥
		0,								// 保存至区号为0的区域
		g_key,							// 待保存的密钥
		6);								// 密钥长度6字节
	while (1) {
	}
}

/* end of file */
