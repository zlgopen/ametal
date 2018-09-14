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
 * \brief LIS3MDL例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_lis3mdl.c src_lis3mdl
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-12  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lis3mdl
 * \copydoc demo_lis3mdl.c
 */

/** [src_lis3mdl] */
#include "ametal.h"
#include "am_delay.h"
#include "am_lis3mdl.h"
#include "am_hwconf_lis3mdl.h"
#include "am_vdebug.h"


/**
 * \brief LIS3MDL例程入口
 */
void demo_lis3mdl_entry (void)
{  
    am_lis3mdl_handle_t handle;
    uint16_t data[3] = {0};
    int8_t   temp = 0;
    
    handle = am_lis3mdl_inst_init();    

    while (1) {

        am_lis3mdl_read (handle, data);
        am_lis3mdl_read_temp (handle, &temp);
        
        AM_DBG_INFO("LIS3MDL = %d    %d    %d \r\n", data[0], data[1], data[2]);
        AM_DBG_INFO("LIS3MDL_temp =  %d ℃     \r\n", temp);
        am_mdelay(500);
    }
}
/** [src_bh1730fvc] */

/* end of file */
