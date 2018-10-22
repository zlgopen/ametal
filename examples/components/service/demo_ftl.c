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
 * \brief FTL（Flash Translation Layer）例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_ftl.c src_ftl
 *
 * \internal
 * \par Modification history
 * - 1.00  17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_ftl
 * \copydoc demo_ftl.c
 */

/** [src_ftl] */
#include "ametal.h"
#include "am_ftl.h"
#include "am_delay.h"
#include "am_vdebug.h"

#define __BUF_SIZE 16 /**< \brief 缓冲区大小 */

/**
 * \brief 例程入口
 */
void demo_ftl_entry (am_ftl_handle_t ftl_handle, int32_t test_lenth)
{
    uint8_t  buf[__BUF_SIZE] = {0};    /* 数据缓存 */
    uint16_t i;
    int      ret;

    if (__BUF_SIZE < test_lenth) {
        test_lenth = __BUF_SIZE;
    }

    /* 填充发送缓冲区 */
    for (i = 0;i < test_lenth; i++) {
        buf[i] = i;
    }

    /* 写数据 */
    ret = am_ftl_write(ftl_handle, 0, &buf[0]);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_ftl_write error(id: %d).\r\n", ret);
        return;
    }
    am_mdelay(5);

    /* 读数据 */
    ret = am_ftl_read(ftl_handle, 0, &buf[0]);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_ftl_read error(id: %d).\r\n", ret);
        return;
    }

    /* 校验写入和读取的数据是否一致 */
    for (i = 0; i < test_lenth; i++) {
        AM_DBG_INFO("Read FLASH the %2dth data is %2x\r\n", i ,buf[i]);

        /* 校验失败 */
        if(buf[i] != i) {
            AM_DBG_INFO("verify failed at index %d.\r\n", i);
            break;
        }
    }

    if (test_lenth == i) {
        AM_DBG_INFO("verify success!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_ftl] */

/* end of file */
