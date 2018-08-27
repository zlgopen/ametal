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
 * \brief GPIO口 模拟I2C 驱动头文件
 *
 * \internal
 * \par modification history:
 * - 1.00 18-04-09  vir, first implementation.
 * \endinternal
 */

#ifndef __AM_I2C_GPIO_H
#define __AM_I2C_GPIO_H

#include "am_common.h"
#include "am_i2c.h"
#include "am_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup grp_am_if_i2c_gpio
 * \copydoc am_i2c_gpio.h
 * @{
 */
/**
 * \brief I2C_GPIO 设备信息参数结构体
 */
typedef struct am_i2c_gpio_devinfo {
    int      scl_pin;        /**< \brief SCL引脚 */
    int      sda_pin;        /**< \brief SDA引脚 */

    /** \brief 总线速度指数, 值越大总线速度越慢, 通常配合逻辑分析仪来确定实际的总线速度  */
    uint32_t speed_exp;
} am_i2c_gpio_devinfo_t;

/**
 * \brief I2C_GPIO 设备结构体
 */
typedef struct am_i2c_gpio_dev {
    /** \brief 标准I2C服务 */
    am_i2c_serv_t                           i2c_serv;

    /** \brief I2C控制器消息队列 */
    struct am_list_head                     msg_list;

    /** \brief 指向I2C传输结构体的指针,同一时间只能处理一个传输 */
    am_i2c_transfer_t                      *p_cur_trans;

    /** \brief 当前正在处理的消息 */
    am_i2c_message_t                       *p_cur_msg;

    /** \brief 忙标识 */
    volatile am_bool_t                      busy;

    /** \brief I2C状态 */
    volatile uint8_t                        state;

    /** \brief 用于数据接收/发送计数 */
    volatile uint32_t                       data_ptr;

    /** \brief I2C_GPIO 设备信息 */
    const am_i2c_gpio_devinfo_t             *p_devinfo;
} am_i2c_gpio_dev_t;

/**
 * \brief I2C_GPIO 初始化
 *
 * \param[in] p_dev     : 指向I2C_GPIO 设备结构体的指针
 * \param[in] p_devinfo : 指向I2C_GPIO 设备信息结构体的指针
 *
 * \return I2C标准服务操作句柄
 */
am_i2c_handle_t am_i2c_gpio_init (am_i2c_gpio_dev_t           *p_dev,
                                  const am_i2c_gpio_devinfo_t *p_devinfo);

/**
 * \brief 解除I2C_GPIO初始化
 *
 * \param[in] handle : 与从设备关联的I2C标准服务操作句柄
 *
 * \return 无
 */
void am_i2c_gpio_deinit (am_i2c_handle_t handle);


/** @} grp_am_if_i2c_gpio */

#ifdef __cplusplus
}
#endif

#endif /* __AM_I2C_GPIO_H */
