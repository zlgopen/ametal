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
 * - 操作步骤:
 *   1. 将 J14 的 KEY 和 RES 短接在一起，使得按下 RES 键接通功率电阻；
 *   2. 短接跳线帽 J11 和 J13，使 I2C1 与 LM75 连接。
 *
 * - 实验现象:
 *   1. 串口输出温度值；
 *   2. 按下 RES 按键，过一会儿，串口输出温度值升高。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOB_3 引脚连接 PC 串口的 RXD；
 *
 * \par 源代码
 * \snippet demo_aml165_core_std_i2c_lm75.c src_aml165_core_std_i2c_lm75
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-10 win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_std_i2c_lm75
 * \copydoc demo_aml165_core_std_i2c_lm75.c
 */

/** [src_aml165_core_std_i2c_lm75] */

/*******************************************************************************
  includes
*******************************************************************************/
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_std_i2c_lm75_entry (void)
{
    am_kprintf("demo aml165_core std i2c lm75!\r\n");

    demo_std_i2c_lm75_entry(am_temp_lm75_inst_init());
}
/** [src_aml165_core_std_i2c_lm75] */

/* end of file */
