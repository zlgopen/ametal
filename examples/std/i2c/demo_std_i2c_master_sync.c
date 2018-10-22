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
 * \brief I2C 主机访问 EEPROM 设备例程，通过同步标准接口实现
 *
 * - 操作步骤：
 *   1. 连接 EEPROM 的 I2C 引脚到相应引脚。
 *
 * - 实验现象：
 *   1. 写数据到 EEPROM；
 *   2. 读出刚才写入的数据；
 *   3. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_std_i2c_master_sync.c src_std_i2c_master_sync
 *
 * \internal
 * \par Modification history
 * - 1.00  17-04-24  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_i2c_master_sync
 * \copydoc demo_std_i2c_master_sync.c
 */

/** [src_std_i2c_master_sync] */
#include "ametal.h"
#include "am_i2c.h"
#include "am_delay.h"
#include "am_vdebug.h"

#define __BUF_SIZE 8 /** \brief 缓冲区大小 */

/**
 * \brief 例程入口
 */
void demo_std_i2c_master_sync_entry (am_i2c_handle_t handle,
                                     uint32_t        eeprom_addr,
                                     uint32_t        test_len)
{
    uint8_t         i;
    uint8_t         wr_buf[__BUF_SIZE] = {0}; /* 写数据缓存定义 */
    uint8_t         rd_buf[__BUF_SIZE] = {0}; /* 读数据缓存定义 */
    am_err_t        ret;
    am_i2c_device_t i2cdev_eeprom;

    /* 构造 I2C 设备 */
    am_i2c_mkdev(&i2cdev_eeprom,
                  handle,
                  eeprom_addr,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 填充发送缓冲区 */
    for (i = 0; i < test_len; i++) {
        wr_buf[i] = (i + 6);
    }

    /* 写数据 */
    ret = am_i2c_write(&i2cdev_eeprom,
                        0x00,
                       &wr_buf[0],
                        test_len);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_i2c_write error(id: %d).\r\n", ret);
        return;
    }

    am_mdelay(100);

    /* 读数据 */
    ret = am_i2c_read(&i2cdev_eeprom,
                       0x00,
                      &rd_buf[0],
                       test_len);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_i2c_read error(id: %d).\r\n", ret);
        return;
    }

    /* 校验写入和读取的数据是否一致 */
    for (i = 0; i < test_len; i++) {
        AM_DBG_INFO("Read EEPROM the %2dth data is 0x%02x\r\n", i ,rd_buf[i]);

        /* 校验失败 */
        if(wr_buf[i] != rd_buf[i]) {
            AM_DBG_INFO("verify failed at index %d.\r\n", i);
            break;
        }
    }

    if (test_len == i) {
        AM_DBG_INFO("verify success!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_i2c_master_sync] */

/* end of file */
