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
 * \brief 以SPI为例，演示了SPI中Transfer的使用方法，通过标准接口实现
 *
 * - 操作步骤:
 *   1. 将SPI MOSI和MISO短接(PIOD2 == PIOD_3),虚拟从机设备连接
 *   2. 按下按键传输一次数据
 *
 * - 实验现象:
 * \note
 *   1. 主机通过 MOSI 发送数据，发出的数据从 MOSI 读回；
 *   2. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_spi_master_int.c src_amks16z_core_std_spi_master_int
 *
 * \internal
 * \par History
 * - 1.00 16-09-30  sdy, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_std_spi_master_int
 * \copydoc demo_amks16z_core_std_spi_master_int.c
 */
 
/** [src_amks16z_core_std_spi_master_int] */
#include "ametal.h"
#include "am_prj_config.h"
#include "am_spi.h"
#include "am_wait.h"
#include "am_gpio.h"
#include "am_kl26_inst_init.h"
#include "am_buzzer.h"
#include "am_board.h"
#include "demo_std_entries.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_spi_master_int_entry (void)
{
    int             cs_pin;
    am_spi_handle_t spi_handle = am_kl26_spi0_int_inst_init();

    cs_pin     = PIOD_0;
    demo_std_spi_master_entry(spi_handle, cs_pin);
}

/** [src_amks16z_core_std_spi_master_int] */

/* end of file */
