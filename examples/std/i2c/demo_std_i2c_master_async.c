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
 * \brief I2C 主机访问 EEPROM 设备例程，通过异步标准接口实现
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
 * \snippet demo_std_i2c_master_async.c src_std_i2c_master_async
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_i2c_master_async
 * \copydoc demo_std_i2c_master_async.c
 */

/** [src_std_i2c_master_async] */
#include "ametal.h"
#include "am_i2c.h"
#include "am_delay.h"
#include "am_vdebug.h"

#define __BUF_SIZE 8 /** \brief 缓冲区大小 */

/**
 * \brief I2C 传输完成回调函数
 */
void __i2c_msg_callback (void *p_arg)
{
    AM_DBG_INFO("I2C async transfer complete!\r\n");
}

/**
 * \brief 例程入口
 */
void demo_std_i2c_master_async_entry (am_i2c_handle_t handle,
                                      uint32_t        eeprom_addr,
                                      uint32_t        test_len)
{
    uint8_t           i;
    uint8_t           sub_addr;
    uint8_t           wr_buf[__BUF_SIZE] = {0}; /* 写数据缓存定义 */
    uint8_t           rd_buf[__BUF_SIZE] = {0}; /* 读数据缓存定义 */
    am_err_t          ret;
    am_i2c_message_t  msg;
    am_i2c_transfer_t trans[4];

    if (__BUF_SIZE < test_len) {
        test_len = __BUF_SIZE;
    }

    /**
     * 构造传输结构体
     * 第 1 个传输发送从机子地址
     * 第 2 个传输从 wr_buf 发送数据到 EEPROM
     * 第 3 个传输发送从机子地址
     * 第 4 个传输从 EEPROM 读取数据到 rd_buf
     */
    sub_addr = 0x00;
    am_i2c_mktrans(&trans[0],
                    eeprom_addr,
                    AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE | AM_I2C_M_WR,
                   &sub_addr,
                    1);

    /* 填充发送缓冲区 */
    for (i = 0;i < test_len; i++) {
        wr_buf[i] = (i + 6);
    }

    am_i2c_mktrans(&trans[1],
                    eeprom_addr,
                    AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE |
                    AM_I2C_M_WR | AM_I2C_M_NOSTART,
                   &wr_buf[0],
                    test_len);

    /* 构造消息 */
    am_i2c_mkmsg(&msg, &trans[0], 2, __i2c_msg_callback, NULL);

    /* 启动异步消息传输 */
    ret = am_i2c_msg_start(handle, &msg);
    if (ret != AM_OK) {
        AM_DBG_INFO("am_i2c_msg_start error(id: %d).\r\n", ret);
        return;
    }

    /* 延时一小段时间，让数据传输先完成再进行数据校验 */
    am_mdelay(100);

    am_i2c_mktrans(&trans[2],
                    eeprom_addr,
                    AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE | AM_I2C_M_WR,
                   &sub_addr,
                    1);

    am_i2c_mktrans(&trans[3],
                    eeprom_addr,
                    AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE | AM_I2C_M_RD,
                   &rd_buf[0],
                    test_len);

    /* 构造消息 */
    am_i2c_mkmsg(&msg, &trans[2], 2, __i2c_msg_callback, NULL);

    /* 启动异步消息传输 */
    ret = am_i2c_msg_start(handle, &msg);
    if (ret != AM_OK) {
        AM_DBG_INFO("am_i2c_msg_start error(id: %d).\r\n", ret);
        return;
    }

    /* 延时一小段时间，让数据传输先完成再进行数据校验 */
    am_mdelay(100);

    /* 校验写入和读取的数据是否一致 */
    for (i = 0;i < test_len; i++) {
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
/** [src_std_i2c_master_async] */

/* end of file */
