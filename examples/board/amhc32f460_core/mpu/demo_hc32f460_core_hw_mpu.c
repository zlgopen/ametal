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
 * \brief MPU 例程，通过 HW 层接口实现
 *
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_mpu.c src_hc32f460_hw_mpu
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_mpu
 * \copydoc demo_hc32f460_hw_mpu.c
 */

/** [src_hc32f460_hw_mpu] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_clk.h"
#include "demo_hc32f460_entries.h"
#include "demo_hc32f460_core_entries.h"
#include "am_hc32f460_dma.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_mpu_entry (void)
{
    AM_DBG_INFO("demo amhc32f460_core hw mpu !\r\n");

    demo_hc32f460_hw_mpu_entry(HC32F460_MPU);
}
/** [src_hc32f460_hw_mpu] */

/* end of file */
