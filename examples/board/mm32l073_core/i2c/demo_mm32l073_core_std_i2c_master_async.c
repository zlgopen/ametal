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
 * \brief I2C 主机访问 EEPROM 例程，通过异步标准接口实现
 *
 * - 操作步骤：
 *   1. PIOB_6 引脚连接 EEPROM 的 SCL 引脚；
 *   2. PIOB_7 引脚连接 EEPROM 的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 主机写地址和数据到从机；
 *   2. 主机接收从机数据，并通过串口打印处理；
 *   3. 主机写入的数据和接收到的从机从机数据一致；
 *   4. LED0 以 200ms 时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIOB_1 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD。
 *    3. 当前 I2C 的 SCL 引脚使用的是 PIOB_6，SDA 引脚使用的是 PIOB_7，
 *       可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_std_i2c_master_async.c src_mm32l073_core_std_i2c_master_async
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_std_i2c_master_async
 * \copydoc demo_mm32l073_core_std_i2c_master_async.c
 */

/** [src_mm32l073_core_std_i2c_master_async] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mm32l073_inst_init.h"
#include "demo_std_entries.h"
#include "demo_mm32l073_core_entries.h"

#define EEPROM_ADDR 0x50 /**< \brief EEPROM 设备的地址 */
#define TEST_LEN    8    /**< \brief 操作 EEPROM 的页大小 */

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_std_i2c_master_async_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core std i2c master async!\r\n");

    demo_std_i2c_master_async_entry(am_mm32l073_i2c1_inst_init(),
                                    EEPROM_ADDR,
                                    TEST_LEN);
}
/** [src_mm32l073_core_std_i2c_master_async] */

/* end of file */
