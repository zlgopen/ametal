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
 * \brief QSPI 例程，通过 HW 层接口实现
 *
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_qspi.c src_hc32f460_hw_qspi
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_qspi
 * \copydoc demo_hc32f460_hw_qspi.c
 */

/** [src_hc32f460_hw_qspi] */

#include "ametal.h"
#include "am_hc32f460.h"
#include "am_board.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "demo_hc32f460_entries.h"
#include "am_hc32f460_inst_init.h"


/**
 * \brief 例程入口
 */
void demo_hc32f460_core_drv_qspi_entry (void)
{

    AM_DBG_INFO("demo hc32f460_core drv qspi !\r\n");

    demo_hc32f460_drv_qspi_entry((am_hc32f460_qspi_dev_t *)am_hc32f460_qspi_inst_init());
}

/** [src_hc32f460_hw_qspi] */

/* end of file */
