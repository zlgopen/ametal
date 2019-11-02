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
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过标准层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_11 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOA_12 引脚连接 I2C 主机的 SDA 引脚。
 *
 * - 实验现象:
 *      可用逻辑分析仪和示波器观察到i2c传输数据的波形；
 *      可调试查看数据缓存。
 *
 * \note
 *    当前 I2C 的 SCL 引脚使用的是 PIOA_11，SDA 引脚使用的是 PIOA_12，
 *    可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_hc32f19x_std_i2c_slave_int.c src_hc32f19x_std_i2c_slave_int
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f19x_std_i2c_slave_int
 * \copydoc demo_hc32f19x_std_i2c_slave_int.c
 */

/** [src_hc32f19x_std_i2c_slave_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_gpio.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "hw/amhw_hc32_rcc.h"
#include "demo_std_entries.h"
#include "am_hc32f19x_inst_init.h"

#define DEV_ADDR    0x50

/**
 * \brief 例程入口
 */
void demo_hc32f19x_core_std_i2c_slave_int_entry (void)
{
    am_kprintf("demo amf19x_core std i2c slave int!\r\n");

    demo_std_i2c_slave_entry(am_hc32_i2c1_slv_inst_init(), DEV_ADDR);
}
/** [src_hc32f19x_std_i2c_slave_int] */

/* end of file */
