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
 * \brief I2C 从机例程（轮询方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_10 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIO0_11 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    此例程可以用来模拟 EEPROM。
 *
 * \par 源代码
 * \snippet demo_am824_hw_i2c_slave_poll.c src_am824_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-10  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_i2c_slave_poll
 * \copydoc demo_am824_hw_i2c_slave_poll.c
 */

/** [src_am824_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x.h"
#include "hw/amhw_lpc_i2c.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_hw_i2c_slave_poll_entry (void)
{
    AM_DBG_INFO("demo am824 I2C slave poll.\r\n");

    am_gpio_pin_cfg(PIO0_10, PIO0_10_I2C0_SCL);
    am_gpio_pin_cfg(PIO0_11, PIO0_11_I2C0_SDA);

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_I2C0);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_I2C0);
  
    demo_lpc_hw_i2c_slave_poll_entry(LPC82X_I2C0);
}
/** [src_am824_hw_i2c_slave_poll] */

/* end of file */
