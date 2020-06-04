/*******************************************************************************
*                                 AWORKS
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      aworks.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief Modbus Port interface from AWorks.
 *
 * \internal
 * \par modification history
 * - 1.00 2017-03-29  mkr, first implementation
 * - 1.01 2017-12-20  mkr, adding 'AM_MB_EVENT_SENDING' status
 *                         when sending data for asyn.
 * \endinternal
 */
#ifndef __AW_MB_PORT_H  /* { */
#define __AW_MB_PORT_H

/**
 * \addtogroup grp_am_if_mb_port
 * \copydoc am_mb_port.h
 * @{
 */

#include "am_mb_compiler.h"
#include "am_mb_err.h"
#include "am_mb_comm.h"
#include "am_timer.h"
#include "am_softimer.h"
#include "am_int.h"
#include "am_wait.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum am_mb_ev_type {
    AM_MB_EVENT_INIT,        /**< 事件初始化                */
    AM_MB_EVENT_READY,       /**< 初始化完成事件        */
    AM_MB_EVENT_RECEIVED,    /**< 接收到一帧数据事件     */
    AM_MB_EVENT_SENDING,     /**< 正在发送 */
    AM_MB_EVENT_SEND_OUT,    /**< 数据帧发送完毕事件 */
    AM_MB_EVENT_STOP,        /**< 停止事件 */
    AM_MB_EVENT_TIMEOUT      /**< 超时事件 */
} am_mb_event_type_t;

typedef struct am_mb_event {
    am_wait_t          ev_wait;  /**< \brief Wait Event */
    am_mb_event_type_t ev_type;  /**< \brief Event Type */
} am_mb_event_t;

/**
 * \brief     初始化事件接口
 *
 * @return  如果没有错误返回MB_ERR_NOERR
 *
 */
am_mb_err_t am_mb_event_init(am_mb_event_t *p_event, am_mb_event_type_t event);

/**
 * \brief 发送事件接口
 *
 * @param[in] p_event   : 事件句柄
 * @param[in] event_val : 发送的事件
 * @return 如果没有错误返回MB_ERR_NOERR
 */
am_mb_err_t am_mb_event_post(am_mb_event_t     *p_event,
                             am_mb_event_type_t event);

/**
 * \brief 查询事件接口.
 *              用于获取事件
 * @param[in]      p_event     : 事件句柄
 * @param[in][out] p_event_val : 获取事件类型
 * @param[in]      is_noblock  ： 是否阻塞
 *
 * @return 如果没有错误返回AW_MB_ERR_NOERR
 */
am_mb_err_t am_mb_event_get (am_mb_event_t      *p_event,
                             am_mb_event_type_t *p_event_val,
                             am_bool_t           is_noblock);

/**
 * \brief 查询事件接口(非阻塞)
 *            用于获取事件
 * @param[in]      p_event     : 事件句柄
 * @param[in][out] p_event_val : 获取事件类型
 *
 * @return 如果没有错误返回AW_MB_ERR_NOERR
 */
am_mb_err_t am_mb_event_noblock_get (am_mb_event_t      *p_event,
                                     am_mb_event_type_t *p_event_val);

/**
 * \brief 事件删除
 *
 * @param[in]  p_event : 事件句柄
 *
 * @return 如果没有错误返回AW_MB_ERR_NOERR
 */
am_mb_err_t am_mb_event_delete (am_mb_event_t *p_event);

/*****************************************************************************/
/** 串口接收/发送状态 */
enum am_mb_serial_rxtx_status {
    AM_SER_RXTX_DISABLE,   /**< \brief 禁止接收/发送 */
    AM_SER_RXTX_ENABLE,    /**< \brief 使能接收/发送  */
};

typedef struct am_mb_serial {
    const void         *p_hdl;    /**< \brief serial handle.之所以用void *是因为串口设备可以有环形缓冲区模式和一般的中断模式 */
    am_bool_t           rx_en;    /**< \brief rx enable. */
    am_bool_t           tx_en;    /**< \brief tx enable. */
    uint8_t             tx_char;  /**< \brief tx char. */
    uint8_t             rx_char;  /**< \brief rx char. */

    void (*tx_int_callback) (void *p_arg); /**< \brief tx interrupt callback.*/
    void (*rx_int_callback) (void *p_arg); /**< \brief rx interrupt callback.*/
    void  *p_arg;                          /**< \brief rx and tx int arg. */

} am_mb_serial_t;

/** \brief 串行链路设备信息 */
typedef struct am_mb_serial_info {
    am_uart_handle_t  uart;     /**< \brief 串口设备句柄 */
    uint32_t          baudrate; /**< \brief 波特率 */
    enum am_mb_parity parity;   /**< \brief 校验 */
    am_bool_t         rs485_en; /**< \brief RS485使能 */
} am_mb_serial_info_t;


/**
 * \brief 初始化串口.
 *           包括打开串口和配置串口等
 *
 * @param[in] p_serial  : Modbus串行设备
 * @param[in] p_hdl     : Ametal串行设备句柄值
 * @param[in] dataBits  : 数据位长度(7bits or 8bits)
 * @param[in] baud_rate : 波特率
 * @param[in] parity    ： 奇偶校验模式
 * @return 如果没有错误返回AW_MB_ERR_NOERR，否则返回AW_MB_ERR_EPORTERR
 */
am_mb_err_t am_mb_serial_port_init (am_mb_serial_t            *p_serial,
                                    uint8_t                    databits,
                                    const am_mb_serial_info_t *p_serial_info);

/**
 * \brief 设置串口的接收和发送功能状态.
 * 如果禁止接收功能，接收中断必须为不使能并且不能接接收数据。
 * @note 如果使能发送功能，必须在使用时调用aw_mb_send_int_callback并触发发送中断。
 *
 * @param[in] recvStatu 使能或禁止接收功能
 * @param[in] sndStatus 使能或禁止发送功能
 */
void am_mb_serial_port_set_rxtx_status (am_mb_serial_t  *p_serial,
                                        am_bool_t        recv_en,
                                        am_bool_t        send_en);

/**
 * \brief 关闭串口
 *
 * @param[in] p_serial : 串口句柄
 */
void am_mb_serial_port_close (am_mb_serial_t *p_serial, am_bool_t is_master);

/**
 * \brief 从串口读取一个字节数据
 * @note  该函数必须为非阻塞的，其在串口中断处理函数中通过aw_mb_recv_int_callback调用
 *
 * @param[in]  p_serial : 串口句柄
 * @param[out] p_data   : 读取数据缓冲
 */
void am_mb_serial_port_getchar (am_mb_serial_t *p_serial, uint8_t *p_data);

/**
 * \brief 发送一个字节数据到串口
 * @note  该函数必须为非阻塞的，其在串口中断处理函数中通过aw_mb_send_int_callback调用
 *
 * @param[in] p_serial : 串口句柄
 * @param[in] data     : 要发送的数据
 */
void am_mb_serial_port_putchar (am_mb_serial_t *p_serial, uint8_t data);
/*****************************************************************************/
typedef struct aw_mb_timer {

    am_softimer_t     sf_tmr;      /**< \brief software timer handler */
    am_timer_handle_t hw_tmr;      /**< \brief hardware timer handler */
    uint8_t           hwtmr_chan;  /**< \brief 硬件定时器通道号 */
    am_bool_t         is_hwtmr;    /**< \brief 是否使能硬件定时器 */

    /**
     * \brief 定时精度值，单位微秒数
     * - 使用硬件定时器时，表示每微秒所需要的定时器计数值
     * - 使用软件定时器时，软件定时器每一定时值所需的微秒数，1ms定时精度时为1000。
     */
    uint32_t          precisions;

    /**
     * \brief 欲设的定时值，微秒
     */
    uint32_t          expires;

    /** \brief 定时器中断回调函数 */
    void (*tmr_int_callback) (void *p_arg);
    void  *p_arg;   /**< \brief 定时器中断回调函数参数 */
} am_mb_timer_t;

/** \brief 定时器设备信息 */
typedef struct aw_mb_timer_info {
    am_timer_handle_t timer;    /**< \brief 定时器硬件句柄 */
    uint32_t          chan;     /**< \brief 定时器通道号 */
} aw_mb_timer_info_t;

/**
 * \brief 初始化定时器
 * \param[in] p_timer   : Modbus定时器设备
 * \param[in] hwtmr_hdl : 硬件定时器句柄值，为NULL时不使用硬件定时器，使用软件定时器
 * \param[in] time      : 定时时间，以微秒(us)计算
 * \return 如果没有错误返回MB_ERR_NOERR，否则返回MB_ERR_EPORTERR
 */
am_mb_err_t am_mb_timer_init (am_mb_timer_t            *p_timer,
                              const aw_mb_timer_info_t *p_timer_info);

/**
 * \brief 使能定时器
 *
 * @note 如果定时器已经使能，需重新计算
 */
void am_mb_timer_enable (am_mb_timer_t *p_timer);

/**
 * \brief 禁止定时器
 */
void am_mb_timer_disable (am_mb_timer_t *p_timer);

/**
 * \brief 关闭定时器，释放定时器
 */
void am_mb_timer_close (am_mb_timer_t *p_timer);

/**
 * \brief 重新设置定时器
 *
 * @param[in] p_timer   : 定时器句柄
 * @param[in] is_hw_tmr : 是否使用硬件定时器
 * @return 如果没有错误返回MB_ERR_NOERR，否则返回MB_ERR_EPORTERR
 */
am_mb_err_t am_mb_timer_reselect (am_mb_timer_t *p_timer, am_bool_t is_hw_tmr);

/**
 * \brief 设置定时器超时值
 *
 * \param[in] p_timer  定时器句柄
 * \param[in] us       微秒值
 *
 * \return N/A
 */
am_static_inline void am_mb_timer_timeout_set (am_mb_timer_t *p_timer,
                                               uint32_t       us)
{
    p_timer->expires = us;
}

/*****************************************************************************/
/**
 * \brief Modbus TCP初始化
 * @note  配置端口，开始监听
 * \param  tcpport TCP端口
 *
 * \retval AW_MB_ERR_NOERR      初始化成功
 */
am_mb_err_t am_mb_tcp_port_init(void *p_cookie, uint16_t tcpport);

/**
 * \brief 关闭TCP连接
 */
void am_mb_tcp_port_close (void *p_cookie);

/**
 * \brief TCP连接停止
 */
void am_mb_tcp_port_disable (void *p_cookie);


/**
 * \brief 获取TCP请求数据
 *
 * @note 读取一帧完整的Modbus TCP数据,
 *            在接收到数据后调用。获取数据后清零接收，设置初始接收偏移数
 *
 * \param **pp_tcpframe 请求数据帧缓冲
 * \param  *p_tcplen     帧数据长度
 *
 */
void am_mb_tcp_port_getrequest (void     *p_cookie,
                                uint8_t **pp_tcpframe,
                                uint16_t *p_tcplen);

/**
 * \brief TCP服务端响应请求
 * @note  响应请求，发送一帧响应数据回客户端
 *
 * \param *p_tcpframe 帧缓冲
 * \param  tcplen          帧数据长度
 *
 * \retval AW_MB_ERR_NOERR 数据帧发送成功
 */
am_mb_err_t am_mb_tcp_port_sndresponse (void          *p_cookie,
                                        const uint8_t *p_tcpframe,
                                        uint16_t       tcplen);

/**
 * \brief  轮询监听套接字和当前连接的Modbus TCP客户端的新事件。
 * \note   此功能检查新客户端的连接和已连接客户端发送的请求.
 *         如果一个新的客户端连接，并仍有剩下的客户端插槽（只有一个目前设备支持），
 *         那么连接被接受，并为新的客户端套接字的事件对象被激活(aw_mb_port_acceptclient())。
 *         客户端在发生FD_CLOSE后，客户端连接被释放的（aw_mb_port_releaseclient）。
 *
 * \retval AW_MB_ERR_EIO    内部IO错误
 * \retval AW_MB_ERR_NOERR  没有错误
 */
am_mb_err_t am_mb_port_tcp_poll (void *p_cookie);

/**
 * \brief 从站RTU模式定时器选择
 *
 * \param *p_cookie  : RTU模式句柄
 * \param  is_hw_tmr : 是否使用硬件定时器
 *
 * \retval AW_MB_ERR_NOERR 设置成功
 */
am_mb_err_t am_mbs_rtu_timer_select (void *p_cookie, am_bool_t is_hw_tmr);

/**
 * \brief 从站ASCII模式定时器选择
 *
 * \param *p_cookie  : ASCII模式句柄
 * \param  is_hw_tmr : 是否使用硬件定时器
 *
 * \retval AW_MB_ERR_NOERR 设置成功
 */
am_mb_err_t am_mbs_ascii_timer_select (void *p_cookie, am_bool_t is_hw_tmr);

/**
 * \brief 主站RTU模式定时器选择
 *
 * \param *p_cookie  : RTU模式句柄
 * \param  is_hw_tmr : 是否使用硬件定时器
 *
 * \retval AW_MB_ERR_NOERR 设置成功
 */
am_mb_err_t am_mb_m_rtu_timer_select (void *p_cookie, am_bool_t is_hw_tmr);

/**
 * \brief 主站ASCII模式定时器选择
 *
 * \param *p_cookie  : ASCII模式句柄
 * \param  is_hw_tmr : 是否使用硬件定时器
 *
 * \retval AW_MB_ERR_NOERR 设置成功
 */
am_mb_err_t am_mb_m_ascii_timer_select (void *p_cookie, am_bool_t is_hw_tmr);

#ifdef __cplusplus
}
#endif

/** @} grp_aw_mb_port_if */

#endif /* } __AW_MB_M_PORT_H */

/* end of file */

