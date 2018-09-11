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
 * \brief BMG160 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_bmg160.c src_bmg160
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-11  wk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_bmg160
 * \copydoc demo_bmg160.c
 */

/** [src_bmg160] */
#include "ametal.h"
#include "am_delay.h"
#include "am_bmg160.h"
#include "am_hwconf_bmg160.h"
#include "am_vdebug.h"

/**
 * \brief 例程入口
 */
void demo_bmg160_entry (void)
{  
	  int8_t temp = 0;
	  int16_t rate[3] = {0};
		
	  am_bmg160_handle_t handle;
	  handle = am_bmg160_inst_init();
		
    while (1) {
			    /** \brief 读取温度 */
			    am_bmg160_read_temp(handle, &temp);
			    AM_DBG_INFO("temperature = %d\r\n", temp);
			
          /* 读取X、Y、Z通道的角速率 */
		      am_bmg160_read_rate(handle, rate);
	        AM_DBG_INFO(" x_rate = %d\r\n y_rate = %d\r\n z_rate = %d\r\n",
			                     rate[0],rate[1],rate[2]);
			
          am_mdelay(1000);
    }
}
/** [src_bmg160] */

/* end of file */
