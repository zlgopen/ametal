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
 * \brief HTS221 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_hts221.c src_hts221
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-11  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hts221
 * \copydoc demo_hts221.c
 */

/** [src_hts221] */
#include "ametal.h"
#include "am_delay.h"
#include "am_hts221.h"
#include "am_hwconf_hts221.h"
#include "am_vdebug.h"


/**
 * \brief HTS221例程入口
 */
void demo_hts221_entry (void)
{  
    am_hts221_handle_t handle;
    int16_t  temp  = 0;
    uint8_t  hum   = 0;
    
    handle = am_hts221_inst_init();    

    while (1) {
        am_hts221_read_temp (handle, &temp);
        am_hts221_read_hum (handle, &hum);
        AM_DBG_INFO("temp  = %d ℃  \r\n", temp);
        AM_DBG_INFO("hum   = %d rH \r\n", hum);
        am_mdelay(500);
    }
}
/** [src_hts221] */

/* end of file */
