/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s lm75 driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_i2c.h"
#include "lm75.h"
#include "am_lpc82x_inst_init.h"

static am_i2c_device_t  g_lm75_dev; 

void lm75_init (void)
{
    am_i2c_handle_t i2c1_handle = am_lpc82x_i2c1_inst_init();

    am_i2c_mkdev(
        &g_lm75_dev,                        // 初始化与LM75B对应的从机实例初始化
        i2c1_handle,                        // 主机I2C实例句柄
        0x48,                               // 器件的7bit地址为0x48
        AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
}

// 返回值为读取的温度数值，高字节为整数部分，低字节为小数部分，实际温度应该除以256.0
int16_t  lm75_read (void)
{
    uint8_t  temp_value[2];                   // 用于存放读取的值
    int16_t  temp;                            // 16位有符号数，用于保存温度值

    am_i2c_read(&g_lm75_dev, 0x00, temp_value, 2) ;
    temp_value[1] &= 0xE0;           // 0xE0 = 1110 0000，小数部分仅高3位有效，低5位清零
    temp = temp_value[0] << 8 | temp_value[1];
    return temp;
}



/* end of file */
