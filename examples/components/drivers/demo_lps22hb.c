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
 * \brief LPS22HB 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_lps22hb.c src_lps22hb
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-11  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lps22hb
 * \copydoc demo_lps22hb.c
 */

/** [src_lps22hb] */
#include "ametal.h"
#include "am_delay.h"
#include "am_lps22hb.h"
#include "am_hwconf_lps22hb.h"
#include "am_vdebug.h"


/**
 * \brief LPS22HB例程入口
 */
void demo_lps22hb_entry (void)
{  
    am_lps22hb_handle_t handle;
    int16_t  temp  = 0;
    uint32_t press = 0;
    
    handle = am_lps22hb_inst_init();    

    while (1) {
        am_lsp22hb_read_temp (handle, &temp);
        am_lsp22hb_read_press (handle, &press);
        AM_DBG_INFO("temp  = %d ℃  \r\n", temp);
        AM_DBG_INFO("press = %d hPa \r\n", press);
        am_mdelay(500);
    }
}
/** [src_lps22hb] */

/* end of file */
