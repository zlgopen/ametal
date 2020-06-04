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
 * \brief user interface of modbus master protocol.
 *
 * \internal
 * \par modification history
 * - 1.00 2017-03-06  mkr, transplant from amorks
 * \endinternal
 */
#ifndef __AM_MB_MASTER_H  /* { */
#define __AM_MB_MASTER_H

/**
 * \addtogroup grp_am_if_mb_master
 * \copydoc am_mb_master.h
 * @{
 */

#include "am_mb_err.h"
#include "am_mb_comm.h"
#include "am_mb_m_netif.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 发送PDU数据处理接口
 * @note  用户注册自定义功能码时，需定义该类型接口注册。
 * @note  用户使用该接口将p_params数据转换为p_pdudata数据（不包含功能码）,并填入PDU
 *        数据长度(p_pdudata_len)，以此告知协议栈待发送PDU数据及发送长度。
 *
 * @param[in]           p_params : 用户自定义参数;
 *                                 该参数为am_mb_master_request接口的p_param参数.
 * @param[in][out]     p_pdudata : PDU发送数据缓冲区，用户将数据转换至缓冲区中。
 * @param[in][out] p_pdudata_len : PDU发送数据缓冲区长度，该参数传入时，告知用户
 *                                 PDU数据缓冲区的最大长度。
 * @return 若发送PDU数据处理失败，则返回相应错误类型；反之返回AM_MB_ERR_NOERR。
 */
typedef am_mb_err_t (*am_mb_snd_funcode_handler_t) (void    *p_params,
                                                    uint8_t *p_pdudata,
                                                    uint8_t *p_pdudata_len);

/**
 * \brief 接收PDU数据处理接口
 * @note  用户注册自定义功能码时，需定义该类型接口注册。
 * @note  用户使用该接口将PDU缓冲区数据转换为p_params.
 *
 * @param[in][out]  p_params : 用户自定义参数,
 *                             该参数为am_mb_master_request接口的p_param参数.
 * @param[in]      p_pdudata : PDU数据缓冲区(不包含功能码).
 * @param[in]    pdudata_len : PDU数据缓冲区长度.
 * @return 若PDU数据处理失败，则返回相应错误类型；反之返回AM_MB_ERR_NOERR。
 */
typedef am_mb_err_t (*am_mb_rcv_funcode_handler_t) (void    *p_params,
                                                    uint8_t *p_pdudata,
                                                    uint8_t  pdudata_len);

/** \brief  function code handler type */
typedef struct am_mb_m_funcode_handler {
    uint8_t                     funcode;     /** \brief function_code */
    am_mb_snd_funcode_handler_t snd_handler; /** \brief send handle interface */
    am_mb_rcv_funcode_handler_t rcv_handler; /** \brief recv handle interface */
} am_mb_m_funcode_handler_t;

/******************************************************************************/

/** \brief Modbus 主站服务  */
typedef struct am_mb_master_serv {
    /** \brief point to asii/rtu mode private data */
    void                     *p_cookie;

    uint8_t                   slave_addr; /**< \brief slave address */
    enum am_mb_mode           mode;       /**< \brief master mode   */
    enum am_mb_master_status  status;     /**< \brief master status */

    /** \brief 用户注册功能码缓冲 */
    am_mb_m_funcode_handler_t *p_funcode_hdl;

    /** \brief 可支持注册的功能码个数 */
    uint8_t                    funcode_num;

    am_mb_event_t            event;     /**< \brief Master event */

    const am_mb_m_netif_t     *p_netif;   /**< \brief net interface */

    uint32_t ack_timeout;  /**< \brief master wait ack timeout */
    uint32_t bd_delay;     /**< \brief rtu/ascii broadcast delay time */

} am_mb_master_serv_t;

/** \brief Modbus 主站句柄 */
typedef am_mb_master_serv_t *am_mb_master_handle;

/** \brief Modbus 主站设备 */
typedef struct am_mb_master_dev {
    /** \brief modbus master service */
    am_mb_master_serv_t mb_master_serv;

    union mode {
#if AM_MB_MASTER_RTU_ENABLE
        am_mb_m_rtu_t   mbm_rtu;     /**< \brief RTU mode private data */
#endif

#if AM_MB_MASTER_ASCII_ENABLE
        am_mb_m_ascii_t mbm_ascii;   /**< \brief ASCII mode private data */
#endif

#if AM_MB_MASTER_HANDLECP_ENABLE
        am_mb_m_tcp_t   mbm_tcp;     /**< \brief TCP mode private data */
#endif
    } common;

} am_mb_master_dev_t;

typedef struct am_mb_master_info {
    aw_mb_timer_info_t  timer;  /**< \brief 定时器设备信息 */
    am_mb_serial_info_t serial; /**< \brief 串行链路设备信息 */

    enum am_mb_mode     mode;   /**< \brief 主站工作模式 */
} am_mb_master_info_t;
/******************************************************************************/

/**
 * \brief 创建Modbus主站对象
 *
 * @note 在调用所有主站函数之前，必须先调用该函数.
 * @note TCP模式下，建议同一主站的所有操作在同一个线程下.用户可使用该接口建立多个连接.
 *
 * @brief p_m_dev  Modbus主站设备
 *
 * @return Modbus主站句柄，如果发生错误返回NULL
 */
am_mb_master_handle am_mb_master_create (am_mb_master_dev_t *p_dev);

/**
 * \brief 配置Modbus主站
 *
 * @note 该接口必须在主站停止或未配置下调用.
 *
 * @param[in] master           : Modbus主站句柄
 * @param[in] p_dev_handle     ： 外设句柄，如串行设备句柄
 * @param[in] p_params         : 配置参数，
 *                               对于AM_MB_RTU/AM_MB_ASCII模式，
 *                               使用am_mb_master_serial_params_t作为参数;
 *                               对于AM_MB_TCP模式,
 *                               使用am_mb_tcp_client_params作为参数.
 *                               若用户无需重新设置参数，该参数可传递NULL参数.
 * @retval: AM_MB_ERR_NOERR    : 主站配置成功.
 * @retval: AM_MB_ERR_EINVAL   : 参数错误.
 * @retval: AM_MB_ERR_EILLSTATE: 协议栈非法状态.
 * @retval: AM_MB_ERR_EIO      : 参数配置出错.
 */
am_mb_err_t am_mb_master_config (am_mb_master_handle        master,
                                 const am_mb_master_info_t *p_info);

/**
 * \brief 关闭、删除主站
 * @note  退出系统或者不需要再使用主站时调用
 *
 * @param[in] master : Modbus主站句柄
 *
 * @return 如果没有错误返回AM_MB_ERR_NOERR
 */
am_mb_err_t am_mb_master_delete (am_mb_master_handle master);

/**
 * \brief 启动Modbus主站
 *
 * @param[in] master           : Modbus主站句柄
 *
 * @retval: AM_MB_ERR_NOERR    : 主站启动成功
 * @retval: AM_MB_ERR_EINVAL   : 参数错误
 * @retval: AM_MB_ERR_EILLSTATE: 协议栈非法状态
 * @retval: AM_MB_ERR_EIO      : TCP模式下，连接建立失败.
 * @retval: AM_MB_ERR_ETIMEDOUT: TCP模式下，连接超时
 */
am_mb_err_t am_mb_master_start (am_mb_master_handle master);

/**
 * \brief 停止Modbus主站
 *
 * @note 调用该接口将立即停止传输；
 *
 * @param[in] master           : Modbus主站句柄
 *
 * @retval: AM_MB_ERR_NOERR    : 主站停止成功
 * @retval: AM_MB_ERR_EINVAL   : 参数错误
 * @retval: AM_MB_ERR_EILLSTATE: 协议栈非法状态
 * @retval: AM_MB_ERR_EIO      : IO错误
 */
am_mb_err_t am_mb_master_stop (am_mb_master_handle master);

/** \brief 主站读写参数 */
typedef struct am_mb_master_rw_param {
     uint16_t  rd_addr;     /**< \brief 读起始地址  */
     uint16_t  rd_num;      /**< \brief 读数目            */
     uint16_t *p_rd_buf;    /**< \brief 读缓存            */
     uint16_t  wr_addr;     /**< \brief 写起始地址  */
     uint16_t  wr_num;      /**< \brief 写数目            */
     uint16_t *p_wr_buf;    /**< \brief 写缓存            */
} am_mb_master_rw_param_t;

/**
 * \brief 主站请求
 * @note  该接口在主站创建，启动后可调用.
 * @note  该接口为同步阻塞接口.
 * @note  主站请求超时时间，通过接口 am_mb_master_ack_timeout_set 设置
 *
 * @note
 * @param[in] master       : Modbus主站句柄
 * @param[in] slave_addr   : 请求的目标从机地址
 * @param[in] funcode      : 功能码
 * @param[in][out] p_param : 读写参数;
 *                           若为协议栈支持功能码，
 *                           该参数为am_mb_master_rw_param_t类型;
 *                           若为用户自定义功能码，该参数由用户自定义。
 *
 * @return 如果没有错误返回AM_MB_ERR_NOERR；若请求失败，可查看具体错误类型。
 *
 */
am_mb_err_t am_mb_master_request (am_mb_master_handle master,
                                  uint8_t        slave_addr,
                                  uint8_t        funcode,
                                  void          *p_params);

/**
 * \brief 注册自定义功能码处理接口到指定主站.
 * @note  创建 主站后，可注册用户自定义功能码处理接口。
 * @note  该接口将功能码处理接口注册成功后，可使用am_mb_master_request接口来请求。
 * @note  用户可重新定义协议栈默认功能码处理接口。若用户多次注册同一功能码处理接口，
 *        则使用最后注册的功能码处理接口。
 * @note  可以注册的功能码处理函数个数在am_mb_config.h中
 *        AM_MB_MASTER_FUNCODE_HANDLER_MAX定义，超过该个数将无法再注册。
 *
 * @param[in] master      : 主站句柄
 * @param[in] funcode     : 功能码号
 * @param[in] snd_handler : 发送处理函数
 * @param[in] rcv_handler : 接收处理函数，若为写操作，用户不需处理接收PDU数据，
 *                          可将此接口置NULL。
 * @return 注册成功返回AM_MB_ERR_NOERR;失败返回相应错误类型.
 *
 * \code
 * #include "am_mb_master.h"
 *
 * #define MB_MASK_WRITE_REG_FUNCODE  22
 * struct wr_mask_param {
 *     uint16_t addr;
 *     uint16_t and_mask;
 *     uint16_t or_mask;
 * } my_mask_param;
 *
 * am_mb_err_t __mb_snd_mask_handler (void    *p_params,
 *                                    uint8_t *p_pdudata,
 *                                    uint8_t *pdudata_len)
 * {
 *     struct wr_mask_param *p_mask_param = (struct wr_mask_param *)p_params;
 *
 *     p_pdudata[0] = (uint8_t)(p_mask_param->addr >> 8);
 *     p_pdudata[1] = (uint8_t)(p_mask_param->addr);
 *     p_pdudata[2] = (uint8_t)(p_mask_param->and_mask >> 8);
 *     p_pdudata[3] = (uint8_t)(p_mask_param->and_mask);
 *     p_pdudata[4] = (uint8_t)(p_mask_param->or_mask >> 8);
 *     p_pdudata[5] = (uint8_t)(p_mask_param->or_mask);
 *
 *     *pdudata_len = 6;
 *
 *     return AM_MB_ERR_NOERR;
 * }
 *     //注册写寄存器掩码功能码处理函数
 *     am_mb_master_funcode_register(master,
 *                                   MB_MASK_WRITE_REG_FUNCODE,
 *                                   __mb_snd_mask_handler,
 *                                   NULL);
 *     //写寄存器掩码
 *     my_mask_param.addr     = 1000;
 *     my_mask_param.and_mask = 0x01;
 *     my_mask_param.or_mask  = 0;
 *     am_mb_master_request(master,
 *                          SLAVE_ADDR,
 *                          MB_MASK_WRITE_REG_FUNCODE,
 *                          &my_mask_param);
 * \endcode
 */
am_mb_err_t am_mb_master_funcode_register ( \
                                    am_mb_master_handle         master,
                                    uint8_t                     funcode,
                                    am_mb_snd_funcode_handler_t snd_handler,
                                    am_mb_rcv_funcode_handler_t rcv_handler);

/** \brief 主站配置类型定义  */
typedef int am_mbm_cfg_type_t;
#define AM_MBM_TYPE_USE_HW_TIMER      0x01    /**< \brief 使用硬件定时器 */
#define AM_MBM_TYPE_BOARDCAST_DELAY   0x02    /**< \brief 广播延时时间, p_arg为广播延时时间(ms)(uint32_t)   */
#define AM_MBM_TYPE_UID_AS_ADDR       0x03    /**< \brief TCP模式UID作为从站地址, p_arg为是否做为从站地址(bool_t)   */
#define AM_MBM_TYPE_TCP_CONTMOUT      0x04    /**< \brief TCP模式连接超时时间， p_arg为超时时间（ms）(uint32_t)   */
#define AM_MBM_TYPE_ACK_TIMEOUT_SET   0x05    /**< \brief 请求应答时间设置， p_arg为超时时间（ms）(uint32_t)   */
#define AM_MBM_TYPE_ACK_TIMEOUT_GET   0x06    /**< \brief 请求应答时间获取， p_arg为超时时间（ms）(uint32_t *)   */

/**
 * \brief 主站操作配置
 * @note  若无调用该接口，则设置参数按照am_mb_config.h中参数设置.
 * @note  该接口在主站启动前调用.
 *
 * @param[in] master : 主站句柄
 * @param[in] type   : 配置类型:
 *                     - AM_MBM_TYPE_USER_FUNCODE   : 配置自定义功能码数，默认不配置为功能码数
 *                                                   为#AM_MB_MASTER_FUNCODE_HANDLER_MAX
 *                     - AM_MBM_TYPE_BOARDCAST_DELAY: 广播延时时间，
 *                                                   默认不配置延时时间为#AM_MB_MASTER_DELAY_MS_CONVERT
 *                     - AM_MBM_TYPE_UID_AS_ADDR    : TCP模式UID作为从站地址设置, 默认不作为从站地址
 *                     - AM_MBM_TYPE_TCP_CONTMOUT   : TCP模式连接超时时间，默认超时时间为#AM_MB_TCP_CONNECT_TIMEOUT
 *                     - AM_MBM_TYPE_ACK_TIMEOUT_SET: 请求应答时间设置， 默认时间为#am_mb_master_handleIMEOUT_MS_RESPOND
 *                                                   与接口am_mb_master_ack_timeout_set功能一致
 *                     - AM_MBM_TYPE_ACK_TIMEOUT_GET: 请求应答时间获取，与接口am_mb_master_ack_timeout_get功能一致
 * @param[in] p_arg  : 配置类型参数
 * @return 配置成功返回AM_MB_ERR_NOERR;失败返回相应错误类型.
 */
am_mb_err_t am_mb_master_ioctl (am_mb_master_handle    master,
                                am_mbm_cfg_type_t      type,
                                void                  *p_arg);

/**
 * \brief 设置主机应答超时时间
 *
 * @param[in]      master      : 主站句柄
 * @param[in]      timeout     : 应答超时时间（毫秒）, 取值范围50 ~ 100000ms
 *
 * @retval AM_MB_ERR_EINVAL    : 参数错误
 */
am_mb_err_t am_mb_master_ack_timeout_set (am_mb_master_handle master,
                                          uint32_t       timeout);

/**
 * \brief 获取主机应答超时时间
 *
 * @param[in]      master      : 主站句柄
 *
 * @return 应答超时时间获取， 返回0为传入参数错误。
 */
uint32_t am_mb_master_ack_timeout_get (am_mb_master_handle master);

/**
 * \brief 当前请求是否为广播请求
 *
 * @param[in] p_master : 主站设备
 * @retval TRUE  : 请求为广播请求
 * @retval FALSE : 请求非广播请求
 */
am_static_inline am_bool_t am_mb_m_is_broadcast (am_mb_master_handle p_master)
{
    return p_master->slave_addr == AM_MB_ADDRESS_BROADCAST ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 获取功能码处理接口
 *
 * @param[in] p_master : 主站设备
 * @param[in] funcode  : 功能码
 * @return 返回相应功能码处理接口，若无该功能码处理接口，返回NULL.
 */
const am_mb_m_funcode_handler_t *am_mb_m_funcode_handler_get (
                                         am_mb_master_handle p_master,
                                         uint8_t             funcode);
/**
 * \brief 获取应答超时时间获取
 *
 * @param[in] p_master : 主站设备
 * @return    超时时间
 */
am_static_inline uint32_t am_mb_m_ack_timeout_get (am_mb_master_handle p_master)
{
    return p_master->ack_timeout;
}


/**
 * \brief 获取广播延迟获取
 *
 * @param[in] p_master : 主站设备
 * @return    超时时间
 */
am_static_inline uint32_t am_mb_master_broadcast_delay_get (
                                        am_mb_master_handle p_master)
{
    return p_master->bd_delay;
}

#if am_mb_master_handleCP_ENABLE
/**
 * \brief TCP主站创建钩子函数原型
 * @note  该钩子函数将在am_mb_master_start调用时执行.
 *
 * @param[in] p_arg  : 用户自定义参数
 * @param[in] socket : 连接socket
 *
 * @return 若处理正常返回AM_MB_ERR_NOERR.
 */
typedef am_mb_err_t (*am_mb_master_handlecp_hook_t) (void *p_arg, int socket);

/**
 * \brief TCP模式socket连接钩子函数注册
 *
 * @param[in] master   : 主站句柄
 * @param[in] tcp_hook : 钩子函数
 * @param[in] p_arg    : 钩子函数参数
 *
 * @retval AM_MB_ERR_NOERR     : 注册成功；
 * @retval AM_MB_ERR_EINVAL    : 参数错误；
 * @retval AM_MB_ERR_EILLSTATE : 协议栈非法状态
 */
am_mb_err_t am_mb_master_handlecp_hook_register (am_mb_master_handle          master,
                                            am_mb_master_handlecp_hook_t tcp_hook,
                                            void                   *p_arg);

/**
 * \brief TCP模式是否将地址作为UID
 *
 * @param[in] p_master : 主站设备
 * @return    超时时间
 */
am_static_inline bool_t am_mbm_is_uid_as_addr (am_mb_m_dev_t *p_master)
{
    return p_master->uid_as_addr;
}

/**
 * \brief 获取TCP模式超时时间获取
 *
 * @param[in] p_master : 主站设备
 * @return    超时时间
 */
am_static_inline uint32_t am_mb_tcp_contm_get (am_mb_m_dev_t *p_master)
{
    return p_master->tcp_contm;
}
#endif

/**
 * \brief 分配用户功能码
 *
 * @param[in] p_master : 主站设备
 * @param[in] num      : 用户功能码数
 * @return 若无错误返回AM_MB_ERR_NOERR
 */
am_mb_err_t am_mb_master_user_funcode_alloc (am_mb_master_handle        master,
                                             am_mb_m_funcode_handler_t *p_buf,
                                             uint8_t                    num);

/*****************************************************************************
 * Common Function's Code Interface(公共功能码请求接口)
 *****************************************************************************/
/**
 * \brief 读线圈功能
 * @note 功能码[0x01]
 *
 * @param[in] master        : 主站句柄
 * @param[in] slave_addr    : 目标从机地址
 * @param[in] start_addr    : 线圈起始地址
 * @param[in] quantity      : 线圈数, 取值范围1~2000
 * @param[in][out] p_rd_val : 读取线圈值
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_coils_read (am_mb_master_handle master,
                              uint8_t             slave_addr,
                              uint16_t            start_addr,
                              uint16_t            quantity,
                              uint8_t            *p_rd_val);

/**
 * \brief 读离散量功能
 * @note 功能码[0x02]
 *
 * @param[in] master        : 主站句柄
 * @param[in] slave_addr    : 目标从机地址
 * @param[in] start_addr    : 离散量输入起始地址
 * @param[in] quantity      : 离散量读取个数, 取值范围1~2000
 * @param[in][out] p_rd_val : 读取离散量值
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_discrete_inputs_read (am_mb_master_handle master,
                                        uint8_t             slave_addr,
                                        uint16_t            start_addr,
                                        uint16_t            quantity,
                                        uint8_t            *p_rd_val);

/**
 * \brief 读保持寄存器功能
 * @note 功能码[0x03]
 *
 * @param[in] master        : 主站句柄
 * @param[in] slave_addr    : 目标从机地址
 * @param[in] start_addr    : 起始地址
 * @param[in] quantity      : 读取保持寄存器个数，取值范围1~125
 * @param[in][out] p_rd_val : 读取保持寄存器值
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_holding_reg_read (am_mb_master_handle master,
                                    uint8_t             slave_addr,
                                    uint16_t            start_addr,
                                    uint16_t            quantity,
                                    uint16_t           *p_rd_val);

/**
 * \brief 读输入寄存器功能
 * @note 功能码[0x04]
 *
 * @param[in] master        : 主站句柄
 * @param[in] slave_addr    : 目标从机地址
 * @param[in] start_addr    : 起始地址
 * @param[in] quantity      : 读取输入寄存器个数，取值范围1~125
 * @param[in][out] p_rd_val : 读取输入寄存器值
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_input_reg_read (am_mb_master_handle master,
                                  uint8_t             slave_addr,
                                  uint16_t            start_addr,
                                  uint16_t            quantity,
                                  uint16_t           *p_rd_val);

/**
 * \brief 写单个线圈功能
 * @note 功能码[0x05]
 *
 * @param[in] master     : 主站句柄
 * @param[in] slave_addr : 目标从机地址
 * @param[in] coil_addr  : 写线圈地址
 * @param[in] coil_val   : 写线圈值，值为0x0000(COIL OFF)或 0xFF00(COIL ON).
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_single_coil_write (am_mb_master_handle master,
                                     uint8_t             slave_addr,
                                     uint16_t            coil_addr,
                                     uint16_t            coil_val);

/**
 * \brief 写单个寄存器功能
 * @note 功能码[0x06]
 *
 * @param[in] master     : 主站句柄
 * @param[in] slave_addr : 目标从机地址
 * @param[in] reg_addr   : 写寄存器地址
 * @param[in] reg_val    : 写入寄存器值
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_single_reg_write (am_mb_master_handle master,
                                    uint8_t             slave_addr,
                                    uint16_t            reg_addr,
                                    uint16_t            reg_val);

/**
 * \brief 写多个线圈功能
 * @note 功能码[0x0F]
 *
 * @param[in] master     : 主站句柄
 * @param[in] slave_addr : 目标从机地址
 * @param[in] start_addr : 写线圈起始地址
 * @param[in] quantity   : 写线圈个数（0x01~0x7b0）
 * @param[in] p_wr_buf   : 写线圈值(缓冲区)
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_multiple_coils_write (am_mb_master_handle master,
                                        uint8_t             slave_addr,
                                        uint16_t            start_addr,
                                        uint16_t            quantity,
                                        uint8_t            *p_wr_buf);

/**
 * \brief 写多个寄存器功能
 * @note 功能码[0x10]
 *
 * @param[in] master     : 主站句柄
 * @param[in] slave_addr : 目标从机地址
 * @param[in] start_addr : 写寄存器起始地址
 * @param[in] quantity   : 写寄存器个数
 * @param[in] p_wr_buf   : 写寄存器值(缓冲区)
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_multiple_regs_write (am_mb_master_handle master,
                                       uint8_t             slave_addr,
                                       uint16_t            start_addr,
                                       uint16_t            quantity,
                                       uint16_t           *p_wr_buf);

/**
 * \brief 读写多个寄存器功能
 * @note 功能码[0x17]
 *
 * @param[in]      master     : 主站句柄
 * @param[in]      slave_addr : 目标从机地址
 * @param[in][out] p_rw_param : 读写参数
 *
 * @return 执行成功返回AM_MB_ERR_NOERR;失败返回相应错误类型
 */
am_mb_err_t am_mb_multiple_regs_read_write ( \
                                        am_mb_master_handle      master,
                                        uint8_t                  slave_addr,
                                        am_mb_master_rw_param_t *p_rw_param);
/*****************************************************************************/


#ifdef __cplusplus
}
#endif

/** @} grp_am_if_mb_master */

#endif /* } __AM_MB_MASTER_H */

/* end of file */

