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
 * \brief SPI 主机中断方式例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 的 MOSI 引脚和 MISO 引脚用线相连，模拟从机设备，回环测试；
 *   2. 按下按键传输一次数据。
 *
 * - 实验现象：
 *   1. 主机通过 MOSI 发送数据，发出的数据从 MOSI 读回；
 *   2. 串口打印出测试结果。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zlg118_std_spi_master_int.c src_zlg118_std_spi_master_int
 *
 * \internal
 * \par History
 * - 1.00 19-09-20  zp, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_std_spi_master_int
 * \copydoc demo_zlg118_std_spi_master_int.c
 */

/** [src_zlg118_std_spi_master_int] */
#include "ametal.h"
#include "zlg118_pin.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "am_zlg118_inst_init.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_std_spi_master_int_entry (void)
{
    AM_DBG_INFO("demo am118_core std spi master int!\r\n");

    demo_std_spi_master_entry(am_zlg118_spi1_int_inst_init(), PIOA_4);

}
/** [src_zlg118_std_spi_master_int] */

/* end of file */
