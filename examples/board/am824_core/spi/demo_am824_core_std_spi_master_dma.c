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
 * \brief SPI 回环例程（DMA 方式），通过标准接口实现
 *
 * - 操作步骤：
 *   1. 短接 SPI 的 MOSI(PIO0_12) 和 MISO(PIO0_13)；
 *   2. 按下 KEY，发起一次传输。
 *
 * - 实验现象：
 *   1. 主机发起一次传输，并校验发送的数据与接收的数据是否一致；
 *   2. 按下按键后，发起一次传输，并通过串口打印发送的数据与接收的数据。
 *
 * \note
 *    1. 使用按键需要将 J14 跳线帽的 KEY 和 PIO0_1 短接；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_std_spi_master_dma.c src_am_am824_std_spi_master_dma
 *
 * \internal
 * \par History
 * - 1.00 14-12-04  jon, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_spi_master_dma
 * \copydoc demo_am824_std_spi_master_dma.c
 */

/** [src_am_am824_std_spi_master_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"


/**
 * \brief 例程入口
 */
void demo_am824_core_std_spi_master_dma_entry (void)
{
    AM_DBG_INFO("demo am824_core std spi master dma!\r\n");
  
    demo_std_spi_master_entry(am_lpc82x_spi0_dma_inst_init(), PIO0_14);
}
/** [src_am_am824_std_spi_master_dma] */

/* end of file */
