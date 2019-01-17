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
 * \brief I2C 从机模拟 TMP100 例程，通过标准接口实现
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
 * \snippet demo_am116_core_std_i2c_slave_subaddr.c src_am116_core_std_i2c_slave_subaddr
 *
 * \internal
 * \par Modification history
 * - 1.00  17-09-12  vir, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am116_core_std_i2c_slave_subaddr
 * \copydoc demo_am116_core_std_i2c_slave_subaddr.c
 */

/** [src_am116_core_std_i2c_slave_subaddr] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg116_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am116_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_am116_core_std_i2c_slave_subaddr_entry (void)
{
    AM_DBG_INFO("demo am116_core std i2c slave subaddr!\r\n");

    demo_std_i2c_slave_subaddr_entry(am_zlg116_i2c1_slv_inst_init());
}
/** [src_am116_core_std_i2c_slave_subaddr] */

/* end of file */
