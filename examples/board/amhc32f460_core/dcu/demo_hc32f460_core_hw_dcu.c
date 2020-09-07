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
 * \brief DCU 数据计算例程，通过HW接口实现
 *
 * - 实验现象：
 *
 * \note
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_dcu.c src_hw_dcu
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-27  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_dcu
 * \copydoc demo_hc32f460_hw_dcu.c
 */

/** [src_hc32f460_hw_dcu] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "demo_hc32f460_entries.h"


/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_dcu_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core hw dcu!\r\n");

    am_clk_enable(CLK_DCU1);

    demo_hc32f460_hw_dcu_entry (HC32F460_DCU1);

}
/** [src_hw_dcu] */

/* end of file */
