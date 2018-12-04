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
 * \brief DMA内存到内存例程，由DMA驱动层实现
 *
 * - 实验现象：
 *   1. 传输成功则串口打印"transfer success"，否则打印"transfer failure"。
 *
 * \par 源代码
 * \snippet demo_kl26_dr_dma_m2m.c src_kl26_dr_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-18 mkr , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_dr_dma_m2m
 * \copydoc demo_amks16z_core_dr_dma_m2m.c
 */

/** [src_amks16z_core_dr_dma_m2m] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "demo_amks16z_core_all_entries.h"

extern void am_kl26_dma_inst_init (void);

/**
 * \brief DMA内存到内存
 * \return 无
 */
void demo_amks16z_core_dr_dma_m2m_entry (void)
{

    AM_DBG_INFO("demo amks16z_core hw dr dma m2m!\r\n");

    /* DMA初始化 */
    am_kl26_dma_inst_init();

    demo_kl26_dr_dma_m2m_entry();
}

/** [src_amks16z_core_dr_dma_m2m] */

/* end of file */

