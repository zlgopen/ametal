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
 * \brief ISO7816协议相关头文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-05-17  htf, first implementation.
 * \endinternal
 */

#ifndef __AM_ISO7816_H
#define __AM_ISO7816_H

#include "ametal.h"
#include "am_pwm.h"
#include "am_uart.h"
#include "am_gpio.h"
#include "am_timer.h"
#include "am_delay.h"
#include "am_softimer.h"

#define AM_ISO7816_TOUCH_CARD_TYPE_T0    0       /**< \brief T=0 接触式卡*/
#define AM_ISO7816_TOUCH_CARD_TYPE_T1    1       /**< \brief T=1 接触式卡*/

/**
 * \brief ISO7816-3 设备信息结构体
 */
typedef struct am_iso7816_devinfo {
    uint8_t          vicc_pin;          /**< \brief 接触式卡的电源引脚 */
    uint8_t          rst_pin;           /**< \brief 接触式卡的复位引脚 */
    uint8_t          pwm_pin;           /**< \brief 接触式卡的时钟信号引脚 */
    uint32_t         pwm_mode;          /**< \brief 时钟信号引脚PWM模式 */
    uint8_t          pwm_chan;          /**< \brief PWM通道号，用于接触式卡输入时钟频率*/
    uint8_t          io_pin;            /**< \brief 接触式卡的输入/输出引脚 */
    uint32_t         rx_mode;           /**< \brief 输入/输出引脚输入模式 */
    am_bool_t        is_high;           /**< \brief 接触式卡电源控制引脚高有效 */
    uint32_t         clk;               /**< \brief 接触式输出时钟选择 */
} am_iso7816_devinfo_t;

/**
 * \brief ISO7816-3 驱动函数结构体
 */
struct am_iso7816_drv_funcs {

    /** ISO7816-3 接触式IC卡冷复位 */
    int (*pfn_ic_cold_reset)(void        *p_drv,
                             uint8_t     *p_atr,
                             uint32_t    *p_nbytes,
                             am_bool_t    pps);

    /** ISO7816-3 接触式IC卡热复位 */
    int (*pfn_ic_warm_reset)(void     *p_drv,
                             uint8_t  *p_atr,
                             uint32_t *p_nbytes);

    /** \brief IOS7816-3 接触式IC卡传输 */
    int (*pfn_ic_transfer)(void       *p_drv,
                           uint8_t     type,
                           const void *p_tx_buf,
                           uint32_t    tx_nbytes,
                           void       *p_rx_buf,
                           uint32_t   *p_rx_nbytes);

    /** \brief IOS7816-3 接触式IC卡停活 */
    int (*pfn_ic_deactivate)(void *p_drv);
};

/**
 * \brief ISO7816-3 服务结构体
 */
typedef struct am_iso7816_serv {

    /** \brief ISO7816-3 驱动函数结构体指针 */
    struct am_iso7816_drv_funcs *p_funcs;

    /** \brief 用于驱动函数的第一个参数 */
    void                        *p_drv;
} am_iso7816_serv_t;

/** \brief 声明ISO7816-3类命令服务句柄 */
typedef am_iso7816_serv_t *am_iso7816_handle_t;

/**
 * \brief ISO7816-3 设备结构体
 */
typedef struct am_iso7816_dev{
    am_iso7816_serv_t           serv;               /**< \brief ISO7816服务结构体 */
    uint8_t                     status;             /**< \brief 当前设备状态 */

    /* 注意以下参数在接收复位信息之后会根据复位信息数据进行改变 */
    volatile uint32_t           etu;                /**< \brief 数据位保持标准时间 */
    volatile uint8_t            msb_first;          /**< \brief 正/反向约定 */
    volatile uint8_t            t_type;             /**< \brief 协议类型(T = 0 或 T = 1) */
    volatile uint8_t            fd;                 /**< \brief 时钟频率变换因子和比特率调整因子 */
    volatile uint8_t            extra_time;         /**< \brief 字节间额外保护时间 */
    volatile uint8_t            wi;                 /**< \brief 字符间时间间隔 960 * F / f * WI(T = 0) */
    volatile uint8_t            ifsc;               /**< \brief 最大接收信息长度(T = 1) */
    volatile uint8_t            b_cwi;              /**< \brief 字符等待时间分组等待时间(T = 1) */
    volatile uint8_t            tc3;                /**< \brief 帧尾校验模式 */
    volatile am_bool_t          auto_pps;           /**< \brief 是否自动处理协议 */
    volatile uint32_t           clkout;             /**< \brief 输出频率 */

    am_bool_t                   is_timeout;         /**< \brief 接收超时标志 */
    am_softimer_t               timeout_timer;      /**< \brief 接收超时软件定时器 */

    am_pwm_handle_t             pwm_handle;         /**< \brief 时钟输出句柄 */
    am_uart_handle_t            uart_handle;        /**< \brief 串口句柄(异步半双工模式) */
    const am_iso7816_devinfo_t *p_info;             /**< \brief 保存设备信息 */
} am_iso7816_dev_t;

/**
 * \brief ISO7816 接触式卡协议初始化
 *
 * \param[in] p_dev         : 接触式IC卡卡座结构体指针
 * \param[in] uart_handle   : 串口服务句柄
 * \param[in] p_info        : 设备信息结构体指针
 *
 * \retval ISO7816 句柄
 */
am_iso7816_handle_t am_iso7816_init (am_iso7816_dev_t              *p_dev,
                                     am_uart_handle_t               uart_handle,
                                     am_pwm_handle_t                pwm_handle,
                                     const am_iso7816_devinfo_t    *p_info);

/**
 * \brief  ISO7816 接触式卡冷复位
 *
 * \param[in] handle    : ISO7816  句柄
 * \param[in] fd        : 初始传输时钟频率变换因子和比特率调整因子  默认0x01
 * \param[in] p_atr     : 复位信息数据(无SW1 SW2)
 * \param[in] p_nbytes  : 复位信息数据长度
 *
 * \retval AM_OK : 操作成功
 */
am_static_inline
int am_iso7816_cold_reset (am_iso7816_handle_t  handle,
                           uint8_t             *p_atr,
                           uint32_t            *p_nbytes,
                           am_bool_t            pps)
{
    return handle->p_funcs->pfn_ic_cold_reset(handle->p_drv,
                                              p_atr,
                                              p_nbytes,
                                              pps);
}

/**
 * \brief  ISO7816 接触式卡热复位
 *
 * \param[in] handle    : ISO7816  句柄
 * \param[in] fd        : 初始传输时钟频率变换因子和比特率调整因子  默认0x01
 * \param[in] p_atr     : 复位信息数据(无SW1 SW2)
 * \param[in] p_nbytes  : 复位信息数据长度
 *
 * \retval AM_OK : 操作成功
 */
am_static_inline
int am_iso7816_warm_reset (am_iso7816_handle_t  handle,
                           uint8_t             *p_atr,
                           uint32_t            *p_nbytes)
{
    return handle->p_funcs->pfn_ic_warm_reset(handle->p_drv,
                                              p_atr,
                                              p_nbytes);
}

/**
 * \brief  ISO7816 接触式卡命令传输
 *
 * \param[in] handle      : ISO7816  句柄
 * \param[in] type        : ISO7816  协议卡类型  可选参数    AM_ISO7816_TOUCH_CARD_TYPE_T0
 *                                                 AM_ISO7816_TOUCH_CARD_TYPE_T1
 *                          目前只测试过T=0的接触式卡片
 * \param[in] p_tx_buf    : 待发送命令缓冲区
 * \param[in] tx_nbytes   : 待发送命令字节数
 * \param[in] p_rx_buf    : 接收数据缓冲区
 * \param[in] p_rx_nbytes : 接收到的数据长度
 *
 * \retval AM_OK : 操作成功
 */
am_static_inline
int am_iso7816_transfer (am_iso7816_handle_t    handle,
                         uint8_t                type,
                         uint8_t               *p_tx_buf,
                         uint32_t               tx_nbytes,
                         uint8_t               *p_rx_buf,
                         uint32_t              *p_rx_nbytes)
{
    return handle->p_funcs->pfn_ic_transfer(handle,
                                            type,
                                            p_tx_buf,
                                            tx_nbytes,
                                            p_rx_buf,
                                            p_rx_nbytes);
}

/**
 * \brief  ISO7816 接触式卡停活
 *
 * \param[in] handle : ISO7816  句柄
 *
 * \retval AM_OK : 操作成功
 */
am_static_inline
int am_iso7816_ic_deactivate (am_iso7816_handle_t handle)
{
    return handle->p_funcs->pfn_ic_deactivate(handle);
}

#endif  /* __AM_ISO7816_H */

/* end of file */

