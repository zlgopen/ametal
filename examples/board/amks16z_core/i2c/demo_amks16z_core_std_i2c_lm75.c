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
 * \brief I2C主机从LM75传感器读取温度值演示例程
 *
 * - 操作步骤:
 *       1. 将J11和J13用跳线帽连接；
 *       2. 将程序下载至板子。
 *
 * - 实验现象:
 *       从LM75读回来的温度值向串口打印。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_i2c_lm75.c src_amks16z_core_std_i2c_lm75
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-21 ari, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_std_i2c_lm75
 * \copydoc demo_amks16z_core_std_i2c_lm75.c
 */
 
/** [src_amks16z_core_std_i2c_lm75] */
/*******************************************************************************
  includes
*******************************************************************************/
#include "ametal.h"
#include "am_vdebug.h"
#include "am_i2c.h"
#include "am_kl26_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 测试函数入口
 */
void demo_amks16z_core_std_i2c_lm75_entry (void)
{
    am_temp_handle_t handle;

    AM_DBG_INFO("demo amks16z_core std i2c lm75!\r\n");

    handle = am_temp_lm75_inst_init();

    demo_std_i2c_lm75_entry(handle);

}



/** [src_amks16z_core_std_i2c_lm75] */

/* end of file */
