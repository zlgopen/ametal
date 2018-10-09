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
 * \brief I2C 主机从 LM75 传感器读取温度值例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出温度值；
 *   2. 按下 RES 按键，过一会儿，串口输出温度值升高。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    2. 按键需要将 J14 跳线帽的 KEY 和 RES 短接才能使用；
 *    3. 需要短接 J11 和 J13 跳线帽，I2C1 才会与 LM75 连接。
 *
 * \par 源代码
 * \snippet demo_am824_std_i2c_lm75.c src_am824_std_i2c_lm75
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-10 win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_i2c_lm75
 * \copydoc demo_am824_std_i2c_lm75.c
 */

/** [src_am824_std_i2c_lm75] */
#include "ametal.h"
#include "am_temp.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_std_i2c_lm75_entry (void)
{
    am_temp_handle_t handle      = NULL;

    handle = am_temp_lm75_inst_init();
  
    demo_std_i2c_lm75_entry(handle);

}
/** [src_am824_std_i2c_lm75] */

/* end of file */
