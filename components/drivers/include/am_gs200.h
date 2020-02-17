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
 * \brief GS200接口函数
 *
 * \internal
 * \par Modification History
 * - 1.00 19-12-16  fzb, first implementation.
 * \endinternal
 */


#ifndef __AM_GS200_H
#define __AM_GS200_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "am_softimer.h"
#include "am_uart_rngbuf.h"

/**
 * @addtogroup am_if_gs200
 * @copydoc am_gs200.h
 * @{
 */

#define  AM_GS200_COM_FRAME_LEN             (0x05)  /* 命令长度   */

/**
 * \name GS200通用控制指令(帧标识)
 * @{
 */
#define  AM_GS200_SET_BAUDRATE              (0x01)  /* 设置模块通讯接口波特率指令       */
#define  AM_GS200_SET_GESTURE_STATE         (0x02)  /* 设置模块手势功能使能与禁能指令   */
#define  AM_GS200_SET_WORKING_CONDITION     (0x03)  /* 设置工作状态指令                 */
#define  AM_GS200_SET_HOVERING_GESTURE_MODE (0x04)  /* 设置悬停手势(定时/单次)输出指令  */
#define  AM_GS200_SET_HOVERING_TIMING_TIME  (0x05)  /* 设置悬停手势定时输出时间间隔     */
#define  AM_GS200_SET_HOVERING_SINGLE_TIME  (0x06)  /* 设置悬停手势单次触发时间         */
/* GS200 串口控制指令 */
#define  AM_GS200_NREAD                     (0x07)  /* 获取接收缓冲区中已接收的数据个数 */
#define  AM_GS200_NWRITE                    (0x08)  /* 获取发送缓冲区中已写入的数据个数 */
#define  AM_GS200_FLUSH                     (0x09)  /* 清空发送缓冲区和接收缓冲区       */
#define  AM_GS200_WFLUSH                    (0x0A)  /* 清空发送缓冲区                   */
#define  AM_GS200_RFLUSH                    (0x0B)  /* 清空接收缓冲区                   */
#define  AM_GS200_TIMEOUT                   (0x0C)  /* 设置接收超时时间(单位：ms)       */

/* \brief  GS200 波特率设置选项 */
#define  AM_GS200_BAUDRATE_9600             (0x00)  // (默认)
#define  AM_GS200_BAUDRATE_4800             (0x01)
#define  AM_GS200_BAUDRATE_115200           (0x02)

//设置模块手势功能使能与禁能设置选项
#define  AM_GS200_SET_MOVEUP_DISABLE        (0x01)  /* 设置禁止GS200检测上移手势动作 */
#define  AM_GS200_SET_MOVEDOWN_DISABLE      (0x02)  /* 设置禁止GS200检测下移手势动作 */
#define  AM_GS200_SET_MOVELEFT_DISABLE      (0x03)  /* 设置禁止GS200检测左移手势动作 */
#define  AM_GS200_SET_MOVERIGHT_DISABLE     (0x04)  /* 设置禁止GS200检测右移手势动作 */
#define  AM_GS200_SET_HOVER_DISABLE         (0x05)  /* 设置禁止GS200检测悬停手势动作 */

#define  AM_GS200_SET_MOVEUP_ENABLE         (0x11)  /* 设置使能GS200检测上移手势动作 */
#define  AM_GS200_SET_MOVEDOWN_ENABLE       (0x12)  /* 设置使能GS200检测下移手势动作 */
#define  AM_GS200_SET_MOVELEFT_ENABLE       (0x13)  /* 设置使能GS200检测左移手势动作 */
#define  AM_GS200_SET_MOVERIGHT_ENABLE      (0x14)  /* 设置使能GS200检测右移手势动作 */
#define  AM_GS200_SET_HOVER_ENABLE          (0x15)  /* 设置使能GS200检测悬停手势动作 */

//设置工作模式指令
#define  AM_GS200_SET_SLEEP_MODE            (0x01)  /* 设置GS200立即进入睡眠模式 */
#define  AM_GS200_SET_WAKEUP                (0x02)  /* 立即唤醒GS200模块 */

//设置悬停手势输出使能
#define  AM_GS200_SET_OUTPUT_TIMING_ENABLE  (0x02)  /* 设置GS200悬停指令定时输出使能 */
#define  AM_GS200_SET_OUTPUT_SINGLE_ENABLE  (0x03)  /* 设置GS200悬停指令单次输出使能 */


/**
 * \brief GS200实例信息结构体定义
 */
typedef struct am_gs200_devinfo {

    /** \brief 模块初始化波特率        */
    uint32_t    baudrate;

    /** \brief 用于串口接收的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_rxbuf;

    /** \brief 用于串口发送的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_txbuf;

    /** \brief 用于命令发送缓冲区  */
    uint8_t     *p_cmd_txbuf;

    /** \brief 用于串口接收的缓冲区大小 */
    uint16_t    rxbuf_size;

    /** \brief 用于串口发送的缓冲区大小 */
    uint16_t    txbuf_size;

    /** \brief 用于命令发送缓冲区的大小  */
    uint16_t    cmd_txbuf_len;

} am_gs200_devinfo_t;

/**
 * \brief GS200设备结构体定义
 */
typedef struct am_gs200_dev {

    /** \brief 命令处理的状态  */
    volatile int                cmd_proc_state;

    /** \brief UART实例句柄                    */
    am_uart_handle_t            uart_handle;

    /** \brief 接收数据环形缓冲区      */
    struct am_rngbuf            rx_rngbuf;

    /** \brief 发送数据环形缓冲区      */
    struct am_rngbuf            tx_rngbuf;

    /** \brief 用于保存设备信息指针 */
    const am_gs200_devinfo_t *p_devinfo;

    /** \brief 用于接收命令应答的等待结构体       */
    am_wait_t                   ack_wait;

    /** \brief 用于接收数据的等待结构体   */
    am_wait_t                   rx_wait;

    /** \brief 接收超时时间                */
    volatile uint32_t           timeout_ms;

    /** \brief 命令回应接收缓冲区  */
    uint8_t                    *p_cmd_rxbuf;

    /** \brief 命令回应接收缓冲区的长度  */
    volatile int                cmd_rxbuf_len;

    /** \brief 命令回应已经接收的长度  */
    volatile int                cmd_rx_len;

    /** \brief 软件定时器用于判断一帧是否接收超时 */
    am_softimer_t               timer;
} am_gs200_dev_t;


/** \brief 定义GS200操作句柄  */
typedef struct am_gs200_dev *am_gs200_handle_t;

/**
 * \brief GS200 初始化函数
 *
 * 使用GS200前，应该首先调用该初始化函数，以获取操作GS200的handle。
 *
 * \param[in] p_dev      : 指向GS200设备的指针
 * \param[in] p_devinfo  : 指向GS200设备信息的指针
 * \param[in] uart_handle : UART标准服务操作句柄（使用该UART句柄与GS200通信）
 *
 * \return 操作GS200的handle，若初始化失败，则返回值为NULL。
 *
 * \note 设备指针p_dev指向的设备只需要定义，不需要在调用本函数前初始化。
 */
am_gs200_handle_t am_gs200_init (am_gs200_dev_t             *p_dev,
                                 const am_gs200_devinfo_t   *p_devinfo,
                                 am_uart_handle_t           uart_handle);

/**
 * \brief GS200 控制函数
 *
 * \param[in] handle    : GS200实例句柄
 * \param[in] cmd       : 控制指令，例如：#AM_GS200_SET_BAUDRATE
 * \param[in,out] p_arg : 命令对应的参数，例如： 115200
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_EIO     : 执行错误
 * \retval -AM_ENOTSUP : 指令不支持
 *
 * \note 处理
 */
int am_gs200_ioctl(am_gs200_handle_t    handle,
                   int                  cmd,
                   void                 *p_arg);

/**
 * \brief 发送数据
 *
 * \param[in] handle : GS200实例句柄
 * \param[in] p_buf  : 待发送的数据
 * \param[in] len    : 数据长度
 *
 * \return 非负数，实际发送的字节数，负数，发送失败
 */
int am_gs200_send (am_gs200_handle_t    handle,
                   const uint8_t        *p_buf,
                   int                  len);


/**
 * \brief 接收数据
 *
 * \param[in] handle : GS200实例句柄
 * \param[in] p_buf  : 接收数据的缓冲区
 * \param[out] len   : 接收数据长度
 *
 * \return 非负数，实际接收的字节数，负数，接收失败
 */
int am_gs200_recv (am_gs200_handle_t    handle,
                   uint8_t              *p_buf,
                   int                  len);

#ifdef __cplusplus
}
#endif

#endif


