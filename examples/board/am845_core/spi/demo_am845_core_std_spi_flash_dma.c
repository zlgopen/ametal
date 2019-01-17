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
 * \brief SPI 读写 FLASH(MX25L3206E) 例程（DMA 方式），通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI0 与 SPI FLASH 进行物理连接。
 *
 * - 实验现象：
 *   1. 写入数据到 SPI FLASH 并读取出来进行校验，并将读取到的数据通过串口输出。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *    PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_spi_flash_dma.c src_am845_core_std_spi_flash_dma
 *
 * \internal
 * \par History
 * - 1.00 14-12-04  jon, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_spi_flash_dma
 * \copydoc demo_am845_core_std_spi_flash_dma.c
 */

/** [src_am845_core_std_spi_flash_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc84x.h"
#include "am_lpc84x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_std_spi_flash_dma_entry (void)
{

    AM_DBG_INFO("demo am845_core std spi flash dma!\r\n");

    demo_std_spi_flash_entry(am_lpc84x_spi0_dma_inst_init(), PIO0_10, 0, 256);
}
/** [src_am845_core_std_spi_flash_dma] */

/* end of file */
