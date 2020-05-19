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
 * \brief I2C 从机例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. PIOB_6 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOB_7 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    1. 当前 I2C 的 SCL 引脚使用的是 PIOB_6，SDA 引脚使用的是 PIOB_7，
 *       可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_aml165_core_std_i2c_slave.c src_aml165_core_std_i2c_slave
 *
 * \internal
 * \par Modification history
 * - 1.00  17-09-06  vir, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_std_i2c_slave
 * \copydoc demo_aml165_core_std_i2c_slave.c
 */

/** [src_aml165_core_std_i2c_slave] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_aml165_core_entries.h"

#define __I2C_SLV_ADDR   0X50  /**< \brief 从设备的地址 */

/**
 * \brief 例程入口
 */
void demo_aml165_core_std_i2c_slave_entry (void)
{
    AM_DBG_INFO("demo aml165_core std i2c slave!\r\n");

    demo_std_i2c_slave_entry(am_zml165_i2c1_slv_inst_init(), __I2C_SLV_ADDR);
}
/** [src_aml165_core_std_i2c_slave] */

/* end of file */
