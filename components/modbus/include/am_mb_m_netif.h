/*******************************************************************************
*                                 AMORKS
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      amorks.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief modbus master middle layer interface.
 *
 * \internal
 * \par modification history
 * - 1.00 2017-04-06  mkr, transplant from aworks
 * \endinternal
 */
#ifndef __AM_MB_M_NETIF_H  /* { */
#define __AM_MB_M_NETIF_H

#include "am_mb_port.h"
#include "am_mb_config.h"

/**
 * \addtogroup grp_am_if_mb_m_netif
 * \copydoc am_mb_m_netif.h
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
typedef enum
{
    AM_MB_M_RTU_STATE_RX_INIT,      /*!< Receiver is in initial state. */
    AM_MB_M_RTU_STATE_RX_IDLE,      /*!< Receiver is in idle state. */
    AM_MB_M_RTU_STATE_RX_RCV,       /*!< Frame is beeing received. */
    AM_MB_M_RTU_STATE_RX_ERROR,     /*!< If the frame is invalid. */
} am_mb_m_rtu_rcv_state_t;

typedef enum
{
    AM_MB_M_RTU_STATE_TX_IDLE,      /*!< Transmitter is in idle state. */
    AM_MB_M_RTU_STATE_TX_XMIT,      /*!< Transmitter is in transfer state. */
    AM_MB_M_RTU_STATE_TX_XFWR,      /*!< Transmitter is in transfer finish and
                                       wait receive state. */
} am_mb_m_rtu_snd_state_t;

/*
 * \brief RTU模式私有数据
 */
typedef struct am_mb_m_rtu {
    /** \brief RTU mode ADU Buffer  */
    uint8_t                 adu_buf[AM_MB_MAX_SER_ADU_LENGTH];
    uint16_t                snd_len;    /**< \brief send data length  */
    uint16_t                snd_pos;    /**< \brief send data pos  */
    uint16_t                rcv_pos;    /**< \brief receive data pos  */
    am_mb_m_rtu_rcv_state_t rcv_state;  /**< \brief receive state  */
    am_mb_m_rtu_snd_state_t snd_state;  /**< \brief send state  */
    am_mb_serial_t          serial_dev; /**< \brief serial information  */
    am_mb_timer_t           timer_dev;  /**< \brief timer information  */
    uint32_t                t35_time;   /**< \brief T3.5 time, microseconds  */

    /** \brief store the Master event, report to up level */
    am_mb_event_t *p_event;
} am_mb_m_rtu_t;
/*****************************************************************************/
/** state of ASCII receive */
typedef enum am_mb_m_ascii_rcv_state {
    AM_MB_M_ASCII_STATE_RX_IDLE,     /**< \brief Receiver is in idle state. */
    AM_MB_M_ASCII_STATE_RX_RCV_ADDR, /**< \brief recevier the slave addr state */
    AM_MB_M_ASCII_STATE_RX_RCV,      /**< \brief Frame is beeing received. */
    AM_MB_M_ASCII_STATE_RX_WAIT_EOF, /**< \brief Wait for End of Frame. */
    AM_MB_M_ASCII_STATE_RX_ERR,      /**< \brief Error occur, aband this frame */
} am_mb_m_ascii_rcv_state_t;

/** state of ASCII send */
typedef enum am_mb_m_ascii_snd_state {
    AM_MB_M_ASCII_STATE_TX_IDLE,  /**< \brief Transmitter is in idle state. */
    AM_MB_M_ASCII_STATE_TX_START, /**< \brief Starting transmission (':' sent). */
    AM_MB_M_ASCII_STATE_TX_DATA,  /**< \brief Sending of data (Address, Data, LRC). */
    AM_MB_M_ASCII_STATE_TX_END,   /**< \brief End of transmission. */
    AM_MB_M_ASCII_STATE_TX_XFWR   /**< \brief Transmitter is in transfer finish
                                              and wait receive state. */
} am_mb_m_ascii_snd_state_t;

/*
 * \brief ASCII模式私有数据
 */
typedef struct am_mb_m_ascii {
    uint8_t  adu_buf[AM_MB_MAX_SER_ADU_LENGTH];  /**< \brief ASCII Buffer  */
    uint8_t  high_nibble;   /**< \brief =1 high nibble  =0 low nibble  */
    uint8_t *p_snddata;     /**< \brief send data buffer  */
    uint8_t  sndlen;        /**< \brief send data length */
    uint8_t  rcv_byte;      /**< \brief the number of bytes received */
    uint8_t  rcv_pos;       /**< \brief receive pdu data buffer postion */

    /** \brief store the Master event, report to up level */
    am_mb_event_t *p_event;

    am_mb_m_ascii_rcv_state_t rcv_state;   /**< \brief receive state  */
    am_mb_m_ascii_snd_state_t snd_state;   /**< \brief send state  */
    am_mb_serial_t            serial_dev;  /**< \brief serial device */
    am_mb_timer_t             timer_dev;   /**< \brief timer device */
} am_mb_m_ascii_t;
/*****************************************************************************/
/*
 * \brief TCP客户端模式私有数据
 */
typedef struct am_mb_m_tcp {
    /** \brief TCP mode Buffer  */
    uint8_t              adu_buf[AM_MB_MAX_TCP_ADU_LENGTH];
//    am_mb_m_tcp_client_t client;        /**< \brief TCP client information  */
    uint16_t             cnt;           /**< \brief used TID  */
    am_bool_t            uid_as_addr;   /**< \brief UID as slave address  */

    /** \brief store the Master event, report to up level */
    am_mb_event_t     *p_event;
} am_mb_m_tcp_t;
/*****************************************************************************/
/**
 * \brief master status
 */
enum am_mb_master_status {
    AM_MB_MASTER_STATE_UNUSE,  /**< \brief Master create but no used state */
    AM_MB_MASTER_STATE_INIT,   /**< \brief Master initial state */
    AM_MB_MASTER_STATE_START,  /**< \brief Master startup state */
    AM_MB_MASTER_STATE_STOP,   /**< \brief Master stop state */
};

/** \brief data-link common interface */
typedef struct am_mb_m_netif {

    /** \brief master init */
    am_mb_err_t (*init)  (void            *p_cookie,
                          const void      *p_param,
                          am_mb_event_t *p_ev);

    /** \brief master close */
    am_mb_err_t (*close) (void *p_cookie);

    /** \brief master start */
    am_mb_err_t (*start) (void *p_cookie);

    /** \brief master stop */
    am_mb_err_t (*stop)  (void *p_cookie);

    /** \brief master recv slave data after recive a frame */
    am_mb_err_t (*m_recv)  (void     *p_cookie,
                            uint8_t  *p_slave_addr,
                            uint8_t **pp_pdu,
                            uint8_t  *p_pdulen);

    /** \brief master send a pdu */
    am_mb_err_t (*m_send)  (void    *p_cookie,
                            uint8_t  slave_addr,
                            uint8_t *p_pdu,
                            uint8_t  pdulen);

    /** \brief get a pdu buffer to send  */
    void (*pdu_get) (void *p_cookie, uint8_t **pp_pdu, uint8_t *p_pdulen);

} am_mb_m_netif_t;

/**
 * \brief 打开Modbus配置接口，配置相应模式参数,注册模式操作函数
 *
 * @param[in]     mode    Modbus通信模式
 * @param[in]     p_param 初始化参数，对于am_MB_RTU/am_MB_ASCII模式使用
 *                        struct am_mb_serial_param作为参数
 *                           对于am_MB_TCP模式，作为端口号参数使用
 * @return 错误号，如果没有错误，其值为AM_MB_ERR_NOERR
 */
am_mb_err_t am_mb_m_netif_open (void *p_cookie, void *p_param);

#ifdef __cplusplus
}
#endif

/** @} grp_am_if_mb_m_netif */

#endif /* } __AM_MB_M_NETIF_H */

/* end of file */
