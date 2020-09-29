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
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_11 引脚连接 EEPROM 的 SCL 引脚；
 *   2. PIOA_12 引脚连接 EEPROM 的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 向 EEPROM 设备写入 16 字节数据；
 *   2. 读取 EEPROM 中的数据通过串口打印出来；
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 当前 I2C 的 SCL 引脚使用的是 PIOA_11，SDA 引脚使用的是 PIOA_12，
 *       可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_zsl42x_core_hw_i2c_master_poll.c
 *          src_zsl42x_core_hw_i2c_master_poll
 *
 * \internal
 * \par History
 * - 1.00 19-12-16  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_core_hw_i2c_master_poll
 * \copydoc demo_zsl42x_core_hw_i2c_master_poll.c
 */

/** [src_zsl42x_core_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zsl42x.h"
#include "am_zsl42x_clk.h"
#include "hw/amhw_zsl42x_i2c.h"
#include "demo_zsl42x_entries.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_hw_i2c_master_poll_entry (void)
{
    am_kprintf("demo zsl42x_core hw i2c master poll!\r\n");

    am_gpio_pin_cfg(PIOA_11, PIOA_11_I2C1_SCL | PIOA_11_OUT_OD);
    am_gpio_pin_cfg(PIOA_12, PIOA_12_I2C1_SDA | PIOA_12_OUT_OD);

    am_clk_enable(CLK_I2C1);

    demo_zsl42x_hw_i2c_master_poll_entry(ZSL42x_I2C1, am_clk_rate_get (CLK_PCLK));
}
/** [src_zsl42x_core_hw_i2c_master_poll] */

/* end of file */
