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
 * \brief HASH 例程，通过驱动层接口实现
 *
 * - 实验现象：
 *
 * \note
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_hash.c src_drv_hash
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-25  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_hash
 * \copydoc demo_hc32f460_drv_hash.c
 */

/** [src_hc32f460_drv_hash] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_hash.h"
#include "demo_hc32f460_entries.h"

static char data_src[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
01234567890123456789012345678901234567890123456789";
static uint8_t hash_digest[32];

/**
 * \brief 例程入口
 */
void demo_hc32f460_drv_hash_generate_entry (am_hash_handle_t handle)
{
    uint32_t i = 0;

    AM_DBG_INFO("demo hc32f460_core drv hash!\r\n");

    if (am_hc32f460_hash_compute(handle,
                                 (uint8_t *)data_src,
                                 sizeof(data_src),
                                 hash_digest) == AM_OK) {

        AM_DBG_INFO("compute success! the message digest is : \r\n");

        for (i = 0; i < sizeof(hash_digest); i++) {
            am_kprintf("%02x", hash_digest[i]);
        }

    } else {
        AM_DBG_INFO("compute failed!");
    }

}
/** [src_drv_hash] */

/* end of file */
