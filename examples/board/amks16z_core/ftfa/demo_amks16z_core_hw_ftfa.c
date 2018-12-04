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
 * \brief FTFA例程,通过HW层的接口实现
 *
 * - 操作步骤：
 *      - 使用调试串口打印信息
 *      - 写入失败时：打印 “write error!”
 *      - 写入错误时：打印 “data not wrote correct!”
 *      - 成功写入时：打印写入的数据(32bit)和 “flash test finished”
 * - 实验现象：
 *
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_ftfa.c src_amks16z_core_hw_ftfa
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_ftfa
 * \copydoc demo_amks16z_core_hw_ftfa.c
 */


/** [src_amks16z_core_hw_ftfa] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "demo_amks16z_core_all_entries.h"

void demo_amks16z_core_hw_ftfa_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw ftfa!\r\n");

    deme_fsl_hw_ftfa_entry(KL26_FTFA);
}

/** [src_amks16z_core_hw_ftfa] */

/* end of file */
