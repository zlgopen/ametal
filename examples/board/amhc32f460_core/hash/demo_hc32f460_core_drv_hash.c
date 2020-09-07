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
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_drv_hash_entry (void)
{
    AM_DBG_INFO("demo amhc32f460_core drv hash!\r\n");
    demo_hc32f460_drv_hash_generate_entry (am_hc32f460_hash_inst_init());
}
/** [src_std_hash] */

/* end of file */
