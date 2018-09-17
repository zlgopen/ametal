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
 * \brief MMC5883MA 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_mmc5883ma.c src_mmc5883ma
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-12  wk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mmc5883ma
 * \copydoc demo_mmc5883ma.c
 */

/** [src_mmc5883ma] */
#include "ametal.h"
#include "am_delay.h"
#include "am_mmc5883ma.h"
#include "am_hwconf_mmc5883ma.h"
#include "am_vdebug.h"

/**
 * \brief 例程入口
 */
 void demo_mmc5883ma_entry (void)
{  
	  int16_t magnetic[4] = {0};
		
	  am_mmc5883ma_handle_t handle;
	  handle = am_mmc5883ma_inst_init();
		
    while (1) {
      /** \brief 读取X、Y、Z通道的磁场 */
	  am_mmc5883ma_read_magnetic(handle, magnetic);
	  AM_DBG_INFO("x_magnetic = %d\r\ny_magnetic = %d\r\nz_magnetic = %d\r\n",
			         magnetic[0],       magnetic[1],       magnetic[2]);
	  AM_DBG_INFO("temperature = %d\r\n",magnetic[3]);
			
      am_mdelay(1000);
    }
}
/** [src_mmc5883ma] */

/* end of file */
