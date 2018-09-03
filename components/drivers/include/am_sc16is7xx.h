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
 * \brief SC16IS7XX 操作接口
 *
 * 使用本服务需要包含头文件 am_sc16is7xx.h
 *
 * \par 使用示例
 * \code
 * #include "am_sc16is7xx.h"
 * \endcode
 *
 * \internal
 * \par modification history
 * - 1.00 18-05-22  pea, first implementation
 * \endinternal
 */

#ifndef __AM_SC16IS7XX_H
#define __AM_SC16IS7XX_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_sc16is7xx
 * \copydoc am_sc16is7xx.h
 * @{
 */

#include "ametal.h"
#include "am_i2c.h"
#include "am_uart.h"
#include "am_wait.h"
#include "am_types.h"

#define  SC16IS7XX_CHAN_MAX    2     /**< \brief 最大串口通道数 */
#define  SC16IS7XX_FIFO_SIZE   64    /**< \brief FIFO 大小 */

/** \brief SC16IS7XX 回调信息 */
typedef struct am_sc16is7xx_cbinfo {
    void *p_arg[2];
} am_sc16is7xx_cbinfo_t;

/** \brief SC16IS7XX 设备信息 */
typedef struct am_sc16is7xx_devinfo {

    /** \brief 串口通道数量 */
    uint8_t  chan_num;

    /** \brief I2C 7 位从机地址 */
    uint8_t  i2c_addr;

    /** \brief 复位引脚 */
    int      rst_pin;

    /** \brief 中断引脚，配置为 -1 时串口只支持轮询模式 */
    int      irq_pin;

    /** \brief 系统时钟频率，单位为 HZ */
    uint32_t clk_freq;

    /** \brief 串口波特率 */
    uint32_t serial_rate[SC16IS7XX_CHAN_MAX];

    /** \brief 串口数据位，值为 5~8 */
    uint8_t  serial_data[SC16IS7XX_CHAN_MAX];

    /** \brief 串口校验位，0:无校验 1:奇校验 2:偶校验 */
    uint8_t  serial_parity[SC16IS7XX_CHAN_MAX];

    /** \brief 串口停止位，值为 1~2 */
    uint8_t  serial_stop[SC16IS7XX_CHAN_MAX];

    /** \brief RS485 方向控制函数, AM_TRUE:发送模式， AM_FALSE:接收模式 */
    void   (*pfn_rs485_dir)(uint8_t chan, am_bool_t is_txmode);

    /** \brief 平台初始化函数 */
    void   (*pfn_plfm_init)(void);

    /** \brief 平台去初始化函数 */
    void   (*pfn_plfm_deinit)(void);

} am_sc16is7xx_devinfo_t;

/** \brief SC16IS7XX 设备 */
typedef struct am_sc16is7xx_dev {

    /** \brief 标准 UART 服务 */
    am_uart_serv_t          uart_serv[SC16IS7XX_CHAN_MAX];

    /** \brief 指向用户注册的 txchar_get 函数 */
    am_uart_txchar_get_t    pfn_txchar_get[SC16IS7XX_CHAN_MAX];

    /** \brief 指向用户注册的 rxchar_put 函数 */
    am_uart_rxchar_put_t    pfn_rxchar_put[SC16IS7XX_CHAN_MAX];

    /** \brief 指向用户注册的错误回调函数 */
    am_uart_err_t           pfn_err[SC16IS7XX_CHAN_MAX];

    /** \brief txchar_get 函数参数 */
    void                   *p_txget_arg[SC16IS7XX_CHAN_MAX];

    /** \brief rxchar_put 函数参数 */
    void                   *p_rxput_arg[SC16IS7XX_CHAN_MAX];

    /** \brief 错误回调函数用户参数 */
    void                   *p_err_arg[SC16IS7XX_CHAN_MAX];

    /** \brief 串口波特率 */
    uint32_t                serial_rate[SC16IS7XX_CHAN_MAX];

    /** \brief 串口选项 */
    uint32_t                serial_opts[SC16IS7XX_CHAN_MAX];

    /** \brief 串口模式 */
    uint32_t                serial_mode[SC16IS7XX_CHAN_MAX];

    /** \brief 串口允许的模式 */
    uint32_t                serial_mode_avail;

    /** \brief 是否使能了 RS485 模式 */
    am_bool_t               is_rs485_en[SC16IS7XX_CHAN_MAX];

    /** \brief IIR 寄存器缓冲区 */
    uint8_t                 iir_reg[SC16IS7XX_CHAN_MAX];

    /** \brief IER 寄存器缓冲区 */
    uint8_t                 ier_reg[SC16IS7XX_CHAN_MAX];

    /** \brief LCR 寄存器缓冲区 */
    uint8_t                 lcr_reg[SC16IS7XX_CHAN_MAX];

    /** \brief LSR 寄存器缓冲区 */
    uint8_t                 lsr_reg[SC16IS7XX_CHAN_MAX];

    /** \brief RXLVL 寄存器缓冲区 */
    uint8_t                 rxlvl_reg[SC16IS7XX_CHAN_MAX];

    /** \brief TXLVL 寄存器缓冲区 */
    uint8_t                 txlvl_reg[SC16IS7XX_CHAN_MAX];

    /** \brief 接收缓冲区 */
    uint8_t                 rx_buf[SC16IS7XX_CHAN_MAX][SC16IS7XX_FIFO_SIZE];

    /** \brief 发送缓冲区 */
    uint8_t                 tx_buf[SC16IS7XX_CHAN_MAX][SC16IS7XX_FIFO_SIZE];

    /** \brief I2C 标准服务操作句柄 */
    am_i2c_handle_t         i2c_handle;

    /** \brief I2C 从设备 */
    am_i2c_device_t         i2c_dev[SC16IS7XX_CHAN_MAX];

    /** \brief I2C 消息 */
    am_i2c_message_t        i2c_msg[SC16IS7XX_CHAN_MAX];

    /** \brief I2C 传输 */
    am_i2c_transfer_t       i2c_trans[SC16IS7XX_CHAN_MAX][2];

    /** \brief 当前传输的子地址 */
    uint8_t                 subaddr[SC16IS7XX_CHAN_MAX];

    /** \brief I2C 错误计数 */
    uint32_t                i2c_err_cnt[SC16IS7XX_CHAN_MAX];

    /** \brief 当前 I2C 是否忙 */
    volatile am_bool_t      is_i2c_busy[SC16IS7XX_CHAN_MAX];

    /** \brief 当前状态机状态 */
    volatile uint8_t        state[SC16IS7XX_CHAN_MAX];

    /** \brief 无字符可取标志位 */
    volatile am_err_t       char_none_flag[SC16IS7XX_CHAN_MAX];

    /** \brief 启动中断发送标志位 */
    volatile am_err_t       int_en_flag[SC16IS7XX_CHAN_MAX];

    /** \brief 中断是否使能 */
    volatile am_bool_t      is_irq_en[SC16IS7XX_CHAN_MAX];

    /** \brief 状态机计数 */
    uint32_t                sm_cnt[SC16IS7XX_CHAN_MAX];

    /** \brief SC16IS7XX 串口信息 */
    am_sc16is7xx_cbinfo_t   uartinfo[SC16IS7XX_CHAN_MAX];

    /** \brief SC16IS7XX I2C 信息 */
    am_sc16is7xx_cbinfo_t   i2cinfo[SC16IS7XX_CHAN_MAX];

    /** \brief 设备信息 */
    am_sc16is7xx_devinfo_t *p_devinfo;

} am_sc16is7xx_dev_t;

/** \brief SC16IS7XX 标准服务操作句柄类型定义 */
typedef struct am_sc16is7xx_dev *am_sc16is7xx_handle_t;

/**
 * \brief 获取指定通道的 UART 标准服务操作句柄
 *
 * \param[in] handle SC16IS7XX 标准服务句柄
 * \param[in] chan   通道号
 *
 * \return UART 标准服务操作句柄，若为 NULL，表明获取失败
 */
am_uart_handle_t am_sc16is7xx_uart_handle_get (am_sc16is7xx_handle_t handle,
                                               uint8_t               chan);

/**
 * \brief 初始化 SC16IS7XX 驱动
 *
 * \param[in] p_dev      SC16IS7XX 设备结构体
 * \param[in] p_info     SC16IS7XX 实例信息
 * \param[in] i2c_handle 操作 SC16IS7XX 的 I2C 句柄
 *
 * \return SC16IS7XX 标准服务句柄，若为 NULL，表明初始化失败
 */
am_sc16is7xx_handle_t am_sc16is7xx_init (am_sc16is7xx_dev_t           *p_dev,
                                         const am_sc16is7xx_devinfo_t *p_devinfo,
                                         am_i2c_handle_t               i2c_handle);

/**
 * \brief 解初始化 SC16IS7XX 驱动
 *
 * \param[in] handle 通过 am_sc16is7xx_init 函数获取的 SC16IS7XX 句柄
 *
 * \retval  AM_OK     解初始化成功
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_sc16is7xx_deinit (am_sc16is7xx_handle_t handle);

/**
 * @} am_if_sc16is7xx
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SC16IS7XX_H */

/* end of file */
