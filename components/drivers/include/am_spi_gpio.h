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
 * \brief GPIO口 模拟SPI 驱动头文件
 *
 * \internal
 * \par modification history:
 * - 1.00 18-04-10  vir, first implementation.
 * \endinternal
 */

#ifndef __AM_SPI_GPIO_H
#define __AM_SPI_GPIO_H

#include "am_common.h"
#include "am_spi.h"
#include "am_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_spi_gpio
 * \copydoc am_spi_gpio.h
 * @{
 */
/**
 * \brief SPI_GPIO 设备信息参数结构体
 */
typedef struct am_spi_gpio_devinfo {
    int      sck_pin;        /**< \brief SCk引脚 */
    int      mosi_pin;       /**< \brief MOSI引脚 (三线模式该引脚作为数据线) */
    int      miso_pin;       /**< \brief MISO引脚 */

    /**
     *  \brief 总线速度指数, 值越大总线速度越慢, 通常配合逻辑分析仪来确定实际的总线速度
     *         标准接口中配置SPI速度 无效
     */
    uint32_t speed_exp;
} am_spi_gpio_devinfo_t;

/**
 * \brief SPI_GPIO 设备结构体
 */
typedef struct am_spi_gpio_dev {
    /** \brief 标准SPI服务 */
    am_spi_serv_t                           spi_serv;

    /** \brief SPI控制器消息队列 */
    struct am_list_head                     msg_list;

    /** \brief 指向SPI传输结构体的指针,同一时间只能处理一个传输 */
    am_spi_transfer_t                      *p_cur_trans;

    /** \brief 当前正在处理的消息 */
    am_spi_message_t                       *p_cur_msg;

    /** \brief 当前传输的SPI设备 */
    am_spi_device_t                        *p_cur_spi_dev;

    /** \brief 忙标识 */
    volatile am_bool_t                      busy;

    /** \brief SPI状态 */
    volatile uint8_t                        state;

    /** \brief SCK 状态 */
    volatile uint8_t                        sck_state;

    /** \brief SPI_GPIO 设备信息 */
    const am_spi_gpio_devinfo_t            *p_devinfo;
} am_spi_gpio_dev_t;

/**
 * \brief SPI_GPIO 初始化
 *
 * \param[in] p_dev     : 指向SPI_GPIO 设备结构体的指针
 * \param[in] p_devinfo : 指向SPI_GPIO 设备信息结构体的指针
 *
 * \return SPI标准服务操作句柄
 */
am_spi_handle_t am_spi_gpio_init (am_spi_gpio_dev_t           *p_dev,
                                  const am_spi_gpio_devinfo_t *p_devinfo);

/**
 * \brief 解除SPI_GPIO初始化
 *
 * \param[in] handle : 与从设备关联的SPI标准服务操作句柄
 *
 * \return 无
 */
void am_spi_gpio_deinit (am_spi_handle_t handle);


/** @} am_if_spi_gpio */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SPI_GPIO_H */
