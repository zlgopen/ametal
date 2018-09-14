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
 * \brief LSM6DSL 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_lsm6dsl.c src_lsm6dsl
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-12  wk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lsm6dsl
 * \copydoc demo_lsm6dsl.c
 */

/** [src_lsm6dsl] */
#include "ametal.h"
#include "am_delay.h"
#include "am_lsm6dsl.h"
#include "am_hwconf_lsm6dsl.h"
#include "am_vdebug.h"

/**
 * \brief 例程入口
 */
 void demo_lsm6dsl_entry (void)
{  
	  
	  int16_t temp     = 0;
	  int16_t accel[3] = {0};
		
	  am_lsm6dsl_handle_t handle;
	  handle = am_lsm6dsl_inst_init();
		
    while (1) {
			/** \brief 读取温度 */
			am_lsm6dsl_read_temp(handle, &temp);
			AM_DBG_INFO(" temp = %d\r\n",temp);
			
			/** \brief 读取X、Y、Z通道的加速度 */
		    am_lsm6dsl_read_accel(handle, accel);
	        AM_DBG_INFO(" x_accel = %d\r\n y_accel = %d\r\n z_accel = %d\r\n",
			                accel[0],      accel[1],          accel[2]);
            am_mdelay(1000);
    }
}
/** [src_lsm6dsl] */

/* end of file */

