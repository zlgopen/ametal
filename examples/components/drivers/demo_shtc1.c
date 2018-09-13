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
 * \brief SHTC1例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_shtc1.c src_shtc1
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-12  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_shtc1
 * \copydoc demo_shtc1.c
 */

/** [src_shtc1] */
#include "ametal.h"
#include "am_delay.h"
#include "am_shtc1.h"
#include "am_hwconf_shtc1.h"
#include "am_vdebug.h"

/**
 * \brief SHTC1例程入口
 */
void demo_shtc1_entry (void)
{  
    am_shtc1_handle_t handle;
    uint8_t hum  = 0;
    int8_t  temp = 0;
    
    handle = am_shtc1_inst_init();    

    while (1) {

        am_shtc1_read_temp_hum (handle, &temp, &hum);
        
        AM_DBG_INFO("SHTC1_HUM   =  %d rH  \r\n", hum);
        AM_DBG_INFO("SHTC1_TEMP  =  %d ℃  \r\n", temp);
        am_mdelay(500);
    }
}
/** [src_shtc1] */

/* end of file */
