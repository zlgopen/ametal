/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/
/**
 * \file
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_11 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOA_12 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    当前 I2C 的 SCL 引脚使用的是 PIOA_11，SDA 引脚使用的是 PIOA_12，
 *    可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_hc32l19x_core_hw_i2c_slave_poll.c
 *          src_hc32l19x_core_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-16  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l19x_core_hw_i2c_slave_poll
 * \copydoc demo_hc32l19x_core_hw_i2c_slave_poll.c
 */

/** [src_hc32l19x_core_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "hw/amhw_hc32_i2c.h"
#include "demo_hc32_entries.h"
#include "demo_aml19x_core_entries.h"
#include "hc32_inum.h"

/**
 * \brief 例程入口
 */
void demo_hc32l19x_core_hw_i2c_slave_int_entry (void)
{
    am_kprintf("demo hc32l19x_core hw i2c slave poll!\r\n");

    am_gpio_pin_cfg(PIOA_11, PIOA_11_I2C1_SCL | PIOA_11_OUT_OD);
    am_gpio_pin_cfg(PIOA_12, PIOA_12_I2C1_SDA | PIOA_12_OUT_OD);

    am_clk_enable(CLK_I2C1);

    demo_hc32_hw_i2c_slave_int_entry(HC32_I2C1, INUM_I2C1);
}
/** [src_hc32l19x_core_hw_i2c_slave_poll] */

/* end of file */
