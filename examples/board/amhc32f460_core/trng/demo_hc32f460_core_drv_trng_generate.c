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
 * \brief trng 例程，通过驱动层接口实现
 *
 * - 实验现象：
 *
 * \note
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_trng.c src_drv_trng
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-25  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_trng
 * \copydoc demo_hc32f460_drv_trng.c
 */

/** [src_hc32f460_drv_trng] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_trng.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_drv_trng_entry (void)
{
    AM_DBG_INFO("demo amhc32f460_core drv trng!\r\n");
    demo_hc32f460_drv_trng_generate_entry (am_hc32f460_trng_inst_init());
}
/** [src_drv_trng] */

/* end of file */
