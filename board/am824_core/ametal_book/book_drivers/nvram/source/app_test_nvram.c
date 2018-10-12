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
 * \brief book`s nvram driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-18  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_nvram.h"
#include "app_test_nvram.h"

int app_test_nvram (char *p_name, uint8_t unit)
{
    int      i;
    uint8_t  data[20];

    for (i = 0; i < 20; i++)                        //填充数据
        data[i] = i;
    am_nvram_set(p_name, unit, &data[0], 0, 20);    // 向"test"存储段中写入 20 个字节数据
     for (i = 0; i < 20; i++)                       // 清零数据
        data[i] = 0;
    am_nvram_get(p_name, unit, &data[0], 0, 20);    // 从"test"存储段中读取 20 个字节数据
    for (i = 0; i < 20; i++) {                      // 比较数据
        if (data[i] != i) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}
/* end of file */
