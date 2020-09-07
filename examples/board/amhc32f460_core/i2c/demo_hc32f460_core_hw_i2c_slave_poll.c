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
 *   1. PIOC_4 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOC_5 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    当前 I2C 的 SCL 引脚使用的是 PIOC_4，SDA 引脚使用的是 PIOC_5，
 *    可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_hc32f460_core_hw_i2c_slave_poll.c
 *          src_hc32f460_core_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-16  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_core_hw_i2c_slave_poll
 * \copydoc demo_hc32f460_core_hw_i2c_slave_poll.c
 */

/** [src_hc32f460_core_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_clk.h"
#include "hw/amhw_hc32f460_i2c.h"
#include "demo_hc32f460_entries.h"
#include "demo_hc32f460_core_entries.h"
#include "hc32f460_regbase.h"

#define SLAVE_ADDR      0x06
#define SLAVE_SPEED     400000

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_i2c_slave_poll_entry (void)
{
    am_kprintf("demo hc32f460_core hw i2c slave poll!\r\n");

    am_gpio_pin_cfg(PIOC_4, GPIO_AFIO(AMHW_HC32F460_AFIO_I2C1_SCL));    
    am_gpio_pin_cfg(PIOC_5, GPIO_AFIO(AMHW_HC32F460_AFIO_I2C1_SDA));    

    demo_hc32f460_hw_i2c_slave_poll_entry((void *)HC32F460_I2C1_BASE, CLK_IIC1, SLAVE_SPEED, SLAVE_ADDR);
}
/** [src_hc32f460_core_hw_i2c_slave_poll] */

/* end of file */
