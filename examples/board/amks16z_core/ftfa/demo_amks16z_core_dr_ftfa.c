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
 * \brief FTFA例程,通过驱动层的接口实现
 *
 * - 操作步骤：
 *      - 串口接调试串口
 *
 * - 实验现象：
 *      - 擦除出错：串口打印 “erase error!”
 *      - 写入出错：串口打印 "program error!"
 *      - 写入成功：串口打印 55扇区的 (1024/4) 个32bit十六进制数据
 *
 * \par 源代码
 * \snippet demo_amks16z_core_dr_ftfa.c src_amks16z_core_dr_ftfa
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_dr_ftfa
 * \copydoc demo_amks16z_core_dr_ftfa.c
 */

/** [src_amks16z_core_dr_ftfa] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_fsl_ftfa.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

void demo_amks16z_core_dr_ftfa_entry (void)
{
    AM_DBG_INFO("demo amks16z_core dr ftfa!\r\n");

    demo_fsl_dr_ftfa_entry(KL26_FTFA);
}

/** [src_amks16z_core_dr_ftfa] */

/* end of file */
