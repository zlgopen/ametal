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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief modbus netif header file
 *
 * \internal
 * \par modification history
 * - 1.00 2017-03-29  mkr, first implementation
 * \endinternal
 */

#ifndef __AM_MB_NETIF_H /* { */
#define __AM_MB_NETIF_H

/**
 * \addtogroup grp_am_if_mb_netif
 * \copydoc am_mb_netif.h
 * @{
 */

#include "am_mb_port.h"
#include "am_mb_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief pdu data buffer */
#if AM_MB_SER_PDU_RECV_BUF_ENABLE
#define __AM_MB_SER_ADU_BUF_MAX      2
#else
#define __AM_MB_SER_ADU_BUF_MAX      1
#endif

/** \brief receive pdu data deal with state */
enum am_mb_pdu_proc_state {
    AM_STATA_PDU_NO_PROCESS,
    AM_STATE_PDU_IN_PROCESS,
};
/******************************************************************************
 * RTU模式私有数据
 ******************************************************************************/
/** \brief state of rtu recv */
enum am_mb_rtu_recv_state {
    AM_RTU_STATE_RX_INIT,
    AM_RTU_STATE_RX_IDLE,
    AM_RTU_STATE_RX_RCV,
    AM_RTU_STATE_RX_ERROR,
};

/** \brief state of rtu send */
enum am_mb_rtu_send_state {
    AM_RTU_STATE_TX_IDLE,
    AM_RTU_STATE_TX_XMIT,
};

/** RTU私有数据  */
struct am_mb_rtu {
    enum am_mb_rtu_recv_state recv_state; /**< \brief recv state */
    enum am_mb_rtu_send_state send_state; /**< \brief send state */

#if AM_MB_SER_PDU_RECV_BUF_ENABLE > 0
    /** \brief recive PDU data state */
    enum am_mb_pdu_proc_state pdu_state;

    /** \brief PDU buffer in process data postion*/
    uint8_t  pdu_buf_proc_pos;
#endif

    uint8_t  slave_addr;   /**< \brief slave address */
    uint8_t  pdu_buf_pos;  /**< \brief recive PDU data buffer postion */
    uint16_t snd_data_len; /**< \brief send data length */
    uint8_t *snd_data;     /**< \brief send data buffer */

    /** \brief pdu buffer */
    uint8_t  pdu_buf[__AM_MB_SER_ADU_BUF_MAX][AM_MB_MAX_SER_ADU_LENGTH];
    /** \brief recv data count  */
    uint16_t rcv_data_pos[__AM_MB_SER_ADU_BUF_MAX];

    /** \brief 保存从站事件接口，上报上层  */
    am_mb_event_t   *p_event;

    /** \brief 串口信息  */
    am_mb_serial_t   serial;

    /** \brief 定时器信息  */
    am_mb_timer_t    timer;
};
/******************************************************************************
 * ASCII模式私有数据
 ******************************************************************************/
/** \brief state of ASCII receive */
enum am_mb_ascii_recv_state {
    AM_ASCII_STATE_RX_IDLE,      /**< \brief Receiver is in idle state. */
    AM_ASCII_STATE_RX_RCV_ADDR,  /**< \brief recevier the slave addr state */
    AM_ASCII_STATE_RX_RCV,       /**< \brief Frame is beeing received. */
    AM_ASCII_STATE_RX_WAIT_EOF,  /**< \brief Wait for End of Frame. */
    AM_ASCII_STATE_RX_ERR,       /**< \brief Error occur, aband this frame */
};

/** \brief state of ASCII send */
enum am_mb_ascii_send_state {
    AM_ASCII_STATE_TX_IDLE,      /**< \brief Transmitter is in idle state. */
    AM_ASCII_STATE_TX_START,     /**< \brief Starting transmission (':' sent). */
    AM_ASCII_STATE_TX_DATA,      /**< \brief Sending of data (Address, Data, LRC). */
    AM_ASCII_STATE_TX_END,       /**< \brief End of transmission. */
    AM_ASCII_STATE_TX_NOTIFY     /**< \brief Notify sender that the frame has been sent. */
};

/** \brief ASCII码高低字节标志  */
enum am_mb_byte_pos {
    AM_BYTE_HIGH_NIBBLE,   /**< \brief Character for high nibble of byte. */
    AM_BYTE_LOW_NIBBLE     /**< \brief Character for low nibble of byte. */
};

/** ASCII私有数据  */
struct am_mb_ascii {
    enum am_mb_ascii_recv_state recv_state;  /**< \brief recv state */
    enum am_mb_ascii_send_state send_state;  /**< \brief send state */

#if AM_MB_SER_PDU_RECV_BUF_ENABLE > 0
    /** \brief recive pdu data state */
    enum am_mb_pdu_proc_state pdu_state;

    /** \bref pdu buffer in process data postion*/
    uint8_t  pdu_buf_proc_pos;
#endif
    uint8_t  slave_addr;    /**< \brief slave address  */
    uint8_t  high_nibble;   /**< \brief =1 high nibble  =0 low nibble  */
    uint8_t  recv_byte;     /**< \brief the number of bytes recived */
    uint8_t  pdu_buf_pos;   /**< \brief recive pdu data buffer postion */
    uint16_t snd_data_len;  /**< \brief send data length */
    uint8_t *snd_data;      /**< \brief send data buffer */

    /** \brief recv data count  */
    uint16_t rev_data_pos[__AM_MB_SER_ADU_BUF_MAX];

    /** \brief pdu buffer */
    uint8_t  pdu_buffer[__AM_MB_SER_ADU_BUF_MAX][AM_MB_MAX_SER_ADU_LENGTH];

    /** \brief 保存从站事件接口，上报上层  */
    am_mb_event_t   *p_event;

    /** \brief 串口信息  */
    am_mb_serial_t   serial;

    /** \brief 定时器信息  */
    am_mb_timer_t    timer;
};

/******************************************************************************
 * TCP模式私有数据
 ******************************************************************************/
#if AM_MB_TCP_ENABLE

/**
 * \brief 从站TCP模式，接收连接钩子函数原型
 * @note  服务端接收连接调用该钩子函数.
 *
 * @param[in] p_arg  : 用户自定义参数
 * @param[in] p_info : 接收的连接信息
 *
 * @retval TRUE  : 允许接收该连接
 * @retval FALSE : 拒绝接收该连接
 */
typedef bool_t (*am_mb_tcp_accept_hook_t) (void                        *p_arg,
                                           struct am_mb_slave_tcp_info *p_info);

/**
 * \brief 从站TCP模式，连接断开钩子函数
 * @note  已接收的连接异常断开，
 *        或连接达到上限值（当设置为达到上限值，允许接收新连接时）主动断开时调用该钩子函数
 *
 * @param[in] p_arg  : 用户自定义参数
 * @param[in] socket : 断开连接的socket
 *
 */
typedef void (*am_mb_tcp_close_hook_t) (void *p_arg, int socket);

struct am_mb_tcp {
    uint8_t         tcp_buf[AM_MB_MAX_TCP_ADU_LENGTH];   /**< \brief TCP数据缓冲 */
    uint16_t        tcp_buf_pos;                         /**< \brief 缓冲区数据当前位置 */
    uint16_t        tcp_frame_bytesleft;                 /**< \brief 需要接收数据长度 */
    uint16_t        port;                                /**< \brief 端口号 */
    am_mbs_event_t *p_event;                             /**< \brief 保存从站事件接口，上报上层  */
    bool_t          is_accept_new;                       /**< \brief 连接达到设置值后是否允许允许接收新连接 */

    int             listen_socket;                       /**< \brief 侦听socket */
    int             cur_socket;                          /**< \brief 当前操作socket编号  */
    int             socket_num;                          /**< \brief 允许建立连接的socket数  */
    int             accept_num;                          /**< \brief 当前连接数  */
    int            *p_sockets;                           /**< \brief socket槽  */

    am_mb_tcp_accept_hook_t accept_hook;         /**< \brief TCP接收连接钩子函数  */
    am_mb_tcp_close_hook_t  close_hook;          /**< \brief TCP关闭连接钩子函数  */
    void                   *p_arg;               /**< \brief TCP连接钩子函数参数  */

};
#endif
/******************************************************************************
 * 从站对象
 ******************************************************************************/
/** slave status type */
enum am_mb_slave_status {
    AM_STATE_UNUSE,
    AM_STATE_INIT,
    AM_STATE_DISABLE,
    AM_STATE_ENABLE,
};

/** data-link common interface */
struct am_mb_netif {
    /** \brief 获取从站地址 */
    uint8_t     (*slave_addr_get)(void *p_cookie);
    /** \brief 设置从站地址 */
    am_mb_err_t (*slave_addr_set)(void *p_cookie, uint8_t addr);
    /** \brief 初始化Modbus从站 */
    am_mb_err_t (*init)(void           *p_cookie, const void *p_param,
                        am_mb_event_t  *p_ev);
    /** \brief 关闭Modbus从站 */
    void        (*close)(void *p_cookie);
    /** \brief 启动Modbus从站 */
    void        (*start)(void *p_cookie);
    /** \brief 停止Modbus从站 */
    void        (*stop) (void *p_cookie);
    /** \brief 从站接收数据 */
    am_mb_err_t (*recv) (void      *p_cookie,
                         uint8_t   *slave_addr,
                         uint8_t  **pp_pdu,
                         uint16_t  *p_len);
    /** \brief 从站发送数据 */
    am_mb_err_t (*send) (void *p_cookie, uint8_t *p_pud, uint16_t len);
};
/******************************************************************************/
#if AM_MB_TCP_ENABLE
/**
 * \brief 设置TCP连接数
 *
 * @param[in] p_cookie : TCP句柄
 * @param[in] tcp_num  : 建立连接数
 *
 * @return    p_err   错误号，如果没有错误，其值为am_MB_ERR_NOERR
 */
am_mb_err_t am_mb_netif_tcp_num_set (void *p_cookie, uint16_t tcp_num);

/**
 * \brief 获取已建立的连接数
 *
 * @param[in] p_cookie : Modbus TCP设备句柄
 * @return 已接收的连接数
 */
am_static_inline uint16_t am_mb_netif_tcp_num_get (void *p_cookie)
{
    return ((struct am_mb_tcp *)p_cookie)->accept_num;
}

/**
 * \brief 是否允许在连接满后关闭旧连接，接收新连接
 *
 * @param[in] p_cookie : Modbus TCP设备句柄
 * @param[in] is_allow : 是否允许.
 */
void am_mb_netif_allow_new_tcp (void *p_cookie, bool_t is_allow);

/**
 * \brief TCP钩子函数注册
 * @param[in]
 * @param[in] accept_hook : 接收连接钩子函数
 * @param[in] close_hook  : 断开连接钩子函数
 * @param[in] p_arg       : 钩子函数参数
 */
void am_mb_netif_tcp_hook_register (void                    *p_cookie,
                                    am_mb_tcp_accept_hook_t  accept_hook,
                                    am_mb_tcp_close_hook_t   close_hook,
                                    void                    *p_arg);
#endif

#ifdef __cplusplus
}
#endif

/* @} grp_am_if_mb_netif */

#endif /* } __AM_MB_NETIF_H */

/* end of file */
