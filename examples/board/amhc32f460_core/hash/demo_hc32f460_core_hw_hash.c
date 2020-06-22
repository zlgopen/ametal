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
 * \brief HASH 例程，通过标准接口实现
 *
 * - 实验现象：
 *
 * \note
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_hash.c src_hw_hash
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-25  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_hash
 * \copydoc demo_hc32f460_hw_hash.c
 */

/** [src_hc32f460_hw_hash] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "demo_hc32f460_entries.h"

static uint8_t data_src[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
01234567890123456789012345678901234567890123456789";
static uint8_t hash_digest[32];

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_hash_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core hw hash!\r\n");

    am_clk_enable(CLK_HASH);

    demo_hc32f460_hw_hash_entry(HC32F460_HASH,
                                sizeof(data_src),
                                data_src,
                                hash_digest);

}
/** [src_std_hash] */

/* end of file */
