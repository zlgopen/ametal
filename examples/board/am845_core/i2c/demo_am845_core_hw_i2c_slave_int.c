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
 * \brief I2C 从机例程（中断方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_10 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIO0_11 引脚连接 I2C 主机的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 从机接收主机写地址和数据到从机，并通过串口打印出来；
 *   2. 从机发送来自主机的数据，并通过串口打印处理；
 *   3. LED0 以 200ms 时间间隔闪烁。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_i2c_slave_int.c src_am845_core_hw_i2c_slave_int
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_i2c_slave_int
 * \copydoc demo_am845_core_hw_i2c_slave_int.c
 */

/** [src_am845_core_hw_i2c_slave_int] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x.h"
#include "hw/amhw_lpc_i2c.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_i2c_slave_int_entry (void)
{

    AM_DBG_INFO("demo am845 I2C slave int mode.\r\n");

    am_gpio_pin_cfg(PIO0_10, PIO0_10_I2C0_SCL);
    am_gpio_pin_cfg(PIO0_11, PIO0_11_I2C0_SDA);

    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_I2C0);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_I2C0);
    amhw_lpc84x_clk_i2c0_clk_sel_set (AMHW_LPC84X_CLK_DEVICE_FRG0CLK);

    demo_lpc_hw_i2c_slave_int_entry(LPC84X_I2C0, INUM_I2C0);

}
/** [src_am845_core_hw_i2c_slave_int] */

/* end of file */
