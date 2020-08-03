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
 * \brief 光谱传感器 AS7262 (UART模式) 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_AS7262_UART_H
#define __AM_SENSOR_AS7262_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_sensor_as7262.h"
#include "am_uart_rngbuf.h"

/* \brief  AS7262 错误标识 */
#define AM_AS7262_RECEIVE_INVALID       0x01    /* 接收内容无效 */
#define AM_AS7262_PARAMETER_INVALID     0x02    /* 返回参数失败 */

/**
 * \brief 传感器 AS7262 (UART模式) 设备信息结构体
 */
typedef struct am_sensor_as7262_uart_devinfo {

    const am_sensor_as7262_devinfo_t       *p_devinfo;

    am_uart_handle_t (*pfn_uart_handle_get) (void);         /**< \brief UART句柄获取函数 */

    void (*pfn_uart_deinit) (am_uart_handle_t uart_handle); /**< \brief UART解初始化函数 */

    uint32_t                                baudrate;       /**< \brief UART波特率 */

    uint8_t                                *p_uart_rxbuf;   /**< \brief 用于串口接收的缓冲区，建议大小在32以上     */

    uint8_t                                *p_uart_txbuf;   /**< \brief 用于串口发送的缓冲区，建议大小在32以上     */

    uint16_t                                rxbuf_size;     /**< \brief 用于串口接收的缓冲区大小 */

    uint16_t                                txbuf_size;     /**< \brief 用于串口发送的缓冲区大小 */

} am_sensor_as7262_uart_devinfo_t;

/**
 * \breif 传感器 AS7262 (UART模式) 设备结构体定义
 */
typedef struct am_sensor_as7262_uart_dev {
    am_sensor_as7262_dev_t                  isa;                    /**< \breif 传感器标准服务 */

    volatile int                            cmd_proc_state;         /**< \brief 命令处理的状态  */

    am_uart_handle_t                        uart_handle;            /**< \brief UART句柄*/

    struct am_rngbuf                        rx_rngbuf;              /**< \brief 接收数据环形缓冲区      */

    struct am_rngbuf                        tx_rngbuf;              /**< \brief 发送数据环形缓冲区      */

    const am_sensor_as7262_uart_devinfo_t  *sensor_uart_dev_info;   /**< \brief 设备信息 */

    am_wait_t                               ack_wait;               /**< \brief 用于接收命令应答的等待结构体       */

    am_wait_t                               rx_wait;                /**< \brief 用于接收数据的等待结构体   */

    volatile uint32_t                       timeout_ms;             /**< \brief 接收超时时间                */

    uint8_t                                *p_cmd_rxbuf;            /**< \brief 命令回应接收缓冲区  */

    volatile int                            cmd_rxbuf_len;          /**< \brief 命令回应接收缓冲区的长度  */

    volatile int                            cmd_rx_len;             /**< \brief 命令回应已经接收的长度  */

    am_softimer_t                           timer;                  /**< \brief 软件定时器用于判断一帧是否接收完成 */
} am_sensor_as7262_uart_dev_t;

/**
 * \brief 传感器 AS7262 (UART模式) 初始化
 *
 * \param[in] p_dev     : 指向传感器AS7262(UART模式)设备的指针
 * \param[in] p_devinfo : 指向传感器AS7262(UART模式)设备信息的指针
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_as7262_uart_init (
        am_sensor_as7262_uart_dev_t             *p_dev,
        const am_sensor_as7262_uart_devinfo_t   *p_devinfo);

/**
 * \brief 传感器 AS7262 (UART模式) 去初始化
 *
 * \param[in] handle : am_sensor_as7262_uart_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_as7262_uart_deinit (am_sensor_handle_t handle);


#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_AS7262_UART_H */

/* end of file */

