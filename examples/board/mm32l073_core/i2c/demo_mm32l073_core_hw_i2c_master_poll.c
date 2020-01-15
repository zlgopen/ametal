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
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOB_6 引脚连接 EEPROM 的 SCL 引脚；
 *   2. PIOB_7 引脚连接 EEPROM 的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 向 EEPROM 设备写入 16 字节数据；
 *   2. 读取 EEPROM 中的数据通过串口打印出来；
 *   3. 若写入数据和读取的数据一致，校验通过，LED0 以 200ms 间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIOB_1 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD；
 *    3. 当前 I2C 的 SCL 引脚使用的是 PIOB_6，SDA 引脚使用的是 PIOB_7，
 *       可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_hw_i2c_master_poll.c src_mm32l073_core_hw_i2c_master_poll
 *
 * \internal
 * \par History
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_hw_i2c_master_poll
 * \copydoc demo_mm32l073_core_hw_i2c_master_poll.c
 */

/** [src_mm32l073_core_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_mm32l073.h"
#include "am_mm32l073_clk.h"
#include "hw/amhw_mm32_i2c.h"
#include "demo_mm32_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_hw_i2c_master_poll_entry (void)
{
    am_kprintf("demo mm32l073_core hw i2c master poll!\r\n");

    am_gpio_pin_cfg(PIOB_6, PIOB_6_I2C_SCL | PIOB_6_AF_OD | PIOB_6_SPEED_20MHz);
    am_gpio_pin_cfg(PIOB_7, PIOB_7_I2C_SDA | PIOB_7_AF_OD | PIOB_7_SPEED_20MHz);

    am_clk_enable(CLK_I2C1);
    am_mm32l073_clk_reset(CLK_I2C1);

    demo_mm32_hw_i2c_master_poll_entry(MM32L073_I2C, am_clk_rate_get (CLK_APB1));
}
/** [src_mm32l073_core_hw_i2c_master_poll] */

/* end of file */
