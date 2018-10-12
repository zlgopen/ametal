/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s mx25xx driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-18  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_ftl.h"

int app_test_ftl (am_ftl_handle_t ftl_handle)
{
    int     i;
    static  uint8_t buf[256];

    for (i = 0; i < 256; i++)        buf[i] = i & 0xFF;   // 装载数据
    am_ftl_write(ftl_handle, 2, buf);
    for (i = 0; i < 256; i++)        buf[i] = 0x0;        // 将所有数据清零
    am_ftl_read(ftl_handle, 2, buf);
    for(i = 0; i < 256; i++) {
        if (buf[i] != (i & 0xFF)) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}



/* end of file */
