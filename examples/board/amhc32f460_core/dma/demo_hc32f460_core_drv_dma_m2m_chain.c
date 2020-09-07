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
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. DMA 传输成功，打印 "transfer success";
 *   2. DMA 传输出错，打印 "transfer failure";
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *    PIOE_5 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_dma_m2m.c src_hc32f460_drv_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-09 cds , first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_dma_m2m
 * \copydoc demo_hc32f460_drv_dma_m2m.c
 */

/** [src_hc32f460_drv_dma_m2m] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "demo_hc32f460_entries.h"
#include "am_hc32f460_dma.h"

extern am_hc32f460_dma_dev_t __g_dma1_dev;
#define  DMA_M2M_CH       DMA_CHAN_1   /**< \brief 通道 */

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_drv_dma_m2m_chain_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core drv dma m2m chain !\r\n");

    demo_hc32f460_drv_dma_m2m_chain_entry(&__g_dma1_dev, DMA_M2M_CH);
}
/** [src_hc32f460_drv_dma_m2m] */

/* end of file */
