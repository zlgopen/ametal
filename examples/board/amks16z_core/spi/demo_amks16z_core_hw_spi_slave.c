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
 * \brief SPI从机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI从机接口和主机接口对应连接起来;
 *   2. 将程序下载到demo板，重新上电或者复位。
 *
 * - 实验现象:
 *   1. 从机接收主机数据，并通过串口将接收到的数据打印出来；
 *   2. 从机发送数据到主机，并通过串口将发送的数据打印出来；
 *
 * - 注意：
 *   1. 需要使用其他设备充当主机。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_spi_slave.c src_amks16z_core_hw_spi_slave
 *
 * \internal
 * \par History
 * - 1.00 16-09-27  sdy, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_hw_spi_slave
 * \copydoc demo_amks16z_core_hw_spi_slave.c
 */
 
/** [src_amks16z_core_hw_spi_slave] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_kl26_inst_init.h"
#include "hw/amhw_fsl_spi.h"
#include "hw/amhw_kl26_sim.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_spi_slave_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw spi slave!\r\n");

    /* 必须使用片选,低电平有效 */
    am_gpio_pin_cfg(PIOC_4, PIOC_4_SPI0_PCS0);
    am_gpio_pin_cfg(PIOC_5, PIOC_5_SPI0_SCK );
    am_gpio_pin_cfg(PIOC_6, PIOC_6_SPI0_MOSI);
    am_gpio_pin_cfg(PIOC_7, PIOC_7_SPI0_MISO);

    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_SPI0);

    demo_fsl_hw_spi_slave_entry(KL26_SPI0);
}

/** [src_amks16z_core_hw_spi_slave] */

/* end of file */

