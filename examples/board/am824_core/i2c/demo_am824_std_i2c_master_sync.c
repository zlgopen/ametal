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
 * \brief I2C 主机读写 EEPROM 例程，通过同步标准接口实现
 *
 * - 操作步骤：
 *   1. PIO0_16 引脚连接 EEPROM 的 SCL 引脚；
 *   2. PIO0_18 引脚连接 EEPROM 的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 主机写地址和数据到从机；
 *   2. 主机接收从机数据，并通过串口打印处理；
 *   3. 主机写入的数据和接收到的从机从机数据一致；
 *   4. LED0 以 200ms 时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_std_i2c_master_sync.c src_am824_std_i2c_master_sync
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-02  cyl, modified the implementation
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_i2c_master_sync
 * \copydoc demo_am824_std_i2c_master_sync.c
 */

/** [src_am824_std_i2c_master_sync] */
#include "ametal.h"
#include "am_i2c.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __EEPROM_ADDR  0x50   /**< \brief EEPROM 设备的地址 */
#define __TEST_LEN     8      /**< \brief 测试读写的字节数(不能大于页大小) */

/**
 * \brief 例程入口
 */
void demo_am824_std_i2c_master_sync_entry (void)
{
    am_i2c_handle_t handle             = NULL;

    AM_DBG_INFO("I2C master sync transfer(EEPROM) demo.\r\n");

    handle = am_lpc82x_i2c1_inst_init();
  
    demo_std_i2c_master_sync_entry(handle, __EEPROM_ADDR, __TEST_LEN);
    
}
/** [src_am824_std_i2c_master_sync] */

/* end of file */
