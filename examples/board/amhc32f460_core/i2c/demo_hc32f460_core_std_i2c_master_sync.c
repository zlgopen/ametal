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
 * \brief I2C 主机访问 EEPROM 例程，通过同步标准接口实现
 *
 * - 操作步骤：
 *   1. PIOC_4 引脚连接 EEPROM 的 SCL 引脚；
 *   2. PIOC_5 引脚连接 EEPROM 的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 主机写地址和数据到从机；
 *   2. 主机接收从机数据，并通过串口打印处理；
 *   3. 主机写入的数据和接收到的从机从机数据一致；
 *   4. LED0 以 200ms 时间间隔闪烁。
 *
 * \note
 *
 * \par 源代码
 * \snippet demo_hc32f460_std_i2c_master_sync.c src_hc32f460_std_i2c_master_sync
 *
 * \internal
 * \par Modification history
 * - 1.00  17-04-24  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_std_i2c_master_sync
 * \copydoc demo_hc32f460_std_i2c_master_sync.c
 */

/** [src_hc32f460_std_i2c_master_sync] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_i2c.h"
#include "am_hc32f460_inst_init.h"
#include "am_hc32f460_i2c.h"
#include "demo_std_entries.h"
#include "demo_hc32f460_core_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

#define EEPROM_ADDR  0x50  /**< \brief EEPROM设备的地址 */
//#define EEPROM_ADDR  0x06  /**< \brief EEPROM设备的地址 */

#define TEST_LEN     8     /**< \brief 操作EEPROM的页大小，需要根据具体芯片设置 */

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_i2c_master_sync_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core std i2c master sync!\r\n");

    demo_std_i2c_master_sync_entry(am_hc32f460_i2c1_inst_init(),
                                   EEPROM_ADDR,
                                   TEST_LEN);
}
/** [src_hc32f460_std_i2c_master_sync] */

/* end of file */
