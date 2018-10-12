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
 * \brief ≥Ã–Ú«Âµ•7.28
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_temp.h"
#include "am_hwconf_lm75.h"
#include "am_delay.h"

int am_main (void)
{
    am_temp_handle_t handle = am_temp_lm75_inst_init();
    int32_t          temp;
    while (1) {
        am_temp_read(handle, &temp);
        am_kprintf("Current temperature : %d.%03d\r\n", temp / 1000, temp % 1000);
        am_mdelay(500);
    }
}


/* end of file */

