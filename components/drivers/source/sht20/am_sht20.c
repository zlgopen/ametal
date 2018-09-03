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
 * \brief  SHT20 温湿度传感器应用接口实现
 *
 * \internal
 * \par Modification History
 * - 1.00 2017-7-13  sky, first implementation.
 * \endinternal
 */

#include "am_sht20.h"

#define SHT20_DEVICE_ADDR           0x40        /* SHT20 I2C地址 */
#define SHT20_TRIG_HUMIDITY         0           /* 测量湿度 */
#define SHT20_TRIG_TEMPERATURE      1           /* 测量温度 */

/**
 * \brief SHT20传感器命令
 */
typedef enum am_sht20_command {
    TRIG_T_MEASUREMENT_HM    = 0xE3,     // command trig. temp meas. hold master
    TRIG_RH_MEASUREMENT_HM   = 0xE5,     // command trig. humidity meas. hold master
    TRIG_T_MEASUREMENT_POLL  = 0xF3,     // command trig. temp meas. no hold master
    TRIG_RH_MEASUREMENT_POLL = 0xF5,     // command trig. humidity meas. no hold master
    USER_REG_W               = 0xE6,     // command writing user register
    USER_REG_R               = 0xE7,     // command reading user register
    SOFT_RESET               = 0xFE      // command soft reset
} am_sht20_command_t;

/**
 * \brief SHT20传感器 CRC 多项式
 */
const uint16_t __g_polynomial = 0x131; //P(x)=x^8+x^5+x^4+1 = 100110001

/**
 * \brief SHT20传感器 CRC校验
 */
static int8_t __sht20_check_crc (uint8_t *p_data, uint8_t nb_bytes, uint8_t check_sum)
{
    uint8_t crc = 0;
    uint8_t byte_ctr;
    uint8_t bit = 0;

    //calculates 8-Bit checksum with given __g_polynomial
    for (byte_ctr = 0; byte_ctr < nb_bytes; ++byte_ctr) {
        crc ^= (p_data[byte_ctr]);
        for (bit = 8; bit > 0; --bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ __g_polynomial;
            } else {
                crc = (crc << 1);
            }
        }
    }

    if (crc != check_sum) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * \brief SHT20传感器计算相对湿度
 */
static float __sht20_calc_rh (uint16_t s_rh)
{
    float humidity_rh;  // variable for result
    s_rh &= ~0x0003;    // clear bits [1..0] (status bits)
    // RH= -6 + 125 * SRH/2^16
    humidity_rh = -6.0 + 125.0/65536 * (float)s_rh;
    return humidity_rh;
}

/**
 * \brief SHT20传感器计算温度[°C]
 */
static float __sht20_calc_temperature_c (uint16_t s_t)
{
    float temperature_c; // variable for result
    s_t &= ~0x0003;     // clear bits [1..0] (status bits)
    //T= -46.85 + 175.72 * ST/2^16
    temperature_c = -46.85 + 175.72/65536 * (float)s_t;
    return temperature_c;
}

/**
 * \brief SHT20传感器轮询式测量
 * \note type : 0 for humidity, 1 for temperature
 */
static int __sht20_measure_poll (am_sht20_handle_t handle, uint8_t type, uint16_t *p_measurand)
{
    uint8_t  cmd;
    uint8_t  temp_value[3] = {0, 0, 0};
    int      ret           = AM_OK;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    /* 写测量命令 */
    switch (type) {

    case SHT20_TRIG_HUMIDITY:       /* 湿度测量 */
        cmd = TRIG_RH_MEASUREMENT_POLL;
        ret = am_i2c_write(p_i2c_dev,
                           0,              //无子地址
                           &cmd,
                           1);
        break;

    case SHT20_TRIG_TEMPERATURE:    /* 温度测量 */
        cmd = TRIG_T_MEASUREMENT_POLL;
        ret = am_i2c_write(p_i2c_dev,
                           0,               //无子地址
                           &cmd,
                           1);
        break;

    default:
        break;
    }

    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }

    /* 等待测量完成 */
    am_mdelay(100);

    /* 读取测量数据 */
    ret = am_i2c_read(p_i2c_dev,
                      0,            //无子地址
                      temp_value,
                      3);

    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }

    /* 输出温度 */
    *p_measurand = temp_value[0] * 256 + temp_value[1];

    /* 验证校验和 */
    ret = __sht20_check_crc(temp_value, 2, temp_value[2]);

    return ret;
}

/**
 * \brief SHT20传感器读取温度
 */
int am_sht20_temperature_read (am_sht20_handle_t handle, float *p_temperature)
{
    int      ret = AM_OK;
    uint16_t s_t;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    ret = __sht20_measure_poll(handle, SHT20_TRIG_TEMPERATURE, &s_t);
    if (AM_OK != ret) {
        return ret;
    }

    *p_temperature = __sht20_calc_temperature_c(s_t);

    return ret;
}

/**
 * \brief SHT20传感器读取湿度
 */
int am_sht20_humidity_read (am_sht20_handle_t handle, float *p_humidity)
{
    int      ret = AM_OK;
    uint16_t s_rh;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    ret = __sht20_measure_poll(handle, SHT20_TRIG_HUMIDITY, &s_rh);
    if (AM_OK != ret) {
        return ret;
    }

    *p_humidity = __sht20_calc_rh(s_rh);

    return ret;
}

/**
 * \brief SHT20传感器软件复位
 */
int am_sht20_soft_reset (am_sht20_handle_t handle)
{
    int     ret;
    uint8_t soft_reset_cmd = SOFT_RESET;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 写复位指令 */
    ret = am_i2c_write(p_i2c_dev,
                       0,              //无子地址
                       &soft_reset_cmd,
                       1);

    /* 需要15ms达到空闲状态 */
    am_mdelay(20);

    return ret;
}

/**
 * \brief SHT20传感器初始化
 */
am_sht20_handle_t am_sht20_init (am_sht20_dev_t *p_dev, am_i2c_handle_t i2c_handle)
{
    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == i2c_handle) {
        return NULL;
    }

    /* 初始配置好sht20设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 SHT20_DEVICE_ADDR,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_NONE);   //无子地址

    return p_dev;
}

/**
 * \brief SHT20 设备解初始化
 */
void am_sht20_deinit (am_sht20_handle_t handle)
{

}

/* end of file */
