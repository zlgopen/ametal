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
 * \brief SPI 从机 DMA 方式例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. demo_hc32f460_core_std_spi_slaver_dma_entry()。
 *
 * - 实验现象：
 *   1. 将本从机与主机相连接。      slaver     master
 *                        NSS ------ NSS
 *                        SCK ------ SCK
 *                        MOSI ----- MOSI
 *                        MISO ----- MISO
 *   2. 主机发送数据，从机利用串口打印对应数据。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *    PIOE_5 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f460_std_spi_slave_dma.c src_hc32f460_std_spi_slave_dma
 *
 * \internal
 * \par History
 * - 1.00 20-04-16  cds, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_std_spi_slave_dma
 * \copydoc demo_hc32f460_std_spi_slave_dma.c
 */

/** [src_hc32f460_std_spi_slave_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "demo_std_entries.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_spi_slaver_dma_entry (void)
{
    AM_DBG_INFO("demo amhc32f460_core std spi slaver dma!\r\n");

    demo_std_spi_slave_entry(am_hc32f460_spi3_slv_dma_inst_init());
}
/** [src_hc32f460_std_spi_slave_dma] */

/* end of file */
