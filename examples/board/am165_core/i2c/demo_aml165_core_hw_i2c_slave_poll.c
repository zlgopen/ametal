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
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOB_6 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOB_7 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    当前 I2C 的 SCL 引脚使用的是 PIOB_6，SDA 引脚使用的是 PIOB_7，
 *    可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_aml165_core_hw_i2c_slave_poll.c src_aml165_core_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_hw_i2c_slave_poll
 * \copydoc demo_aml165_core_hw_i2c_slave_poll.c
 */

/** [src_aml165_core_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zml165.h"
#include "am_zml165_clk.h"
#include "hw/amhw_zlg_i2c.h"
#include "demo_zlg_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_hw_i2c_slave_poll_entry (void)
{
    am_kprintf("demo aml165_core hw i2c slave poll!\r\n");

    am_gpio_pin_cfg(PIOB_6, PIOB_6_I2C_SCL | PIOB_6_AF_OD | PIOB_6_SPEED_20MHz);
    am_gpio_pin_cfg(PIOB_7, PIOB_7_I2C_SDA | PIOB_7_AF_OD | PIOB_7_SPEED_20MHz);

    am_clk_enable(CLK_I2C1);
    am_zml165_clk_reset(CLK_I2C1);

    demo_zlg_hw_i2c_slave_poll_entry(ZML165_I2C);
}
/** [src_aml165_core_hw_i2c_slave_poll] */

/* end of file */
