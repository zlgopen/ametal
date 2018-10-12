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
 * \brief 程序清单6.104
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

static void __zlg600_callback (void *p_arg) 
{
	// 设置回调函数时，p_arg的值设置为 detect_flag 的地址，因此p_arg实际上就是指向
	// detect_flag的指针，可以将其转换为uint8_t *类型的指针，进而修改detect_flag的值为1
	 *(uint8_t *)p_arg = 1;
}

int am_main(void)
{
    	uint8_t 			detect_flag 	= 0;
    	am_zlg600_handle_t 	zlg600_handle 	= am_zlg600_uart_inst_init();
    	am_zlg600_mifare_card_auto_detect_cb_set(zlg600_handle, __zlg600_callback, &detect_flag);  	
	    // p_arg的值设置为detect_flag的地址
	
      return 0;
}
/* end of file */
