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
 * \brief modbus从机协议栈用户接口头文件
 *
 * \internal
 * \par modification history
 * - 1.00 2017-03-29  mkr, transplant from aworks
 * \endinternal
 */

#ifndef __AM_MB_SLAVE_H /* { */
#define __AM_MB_SLAVE_H

/**
 * \addtogroup grp_am_if_mb_slave
 * \copydoc am_mb_slave.h
 * @{
 */

#include "am_mb_config.h"
#include "am_mb_err.h"
#include "am_mb_comm.h"
#include "am_mb_netif.h"
#include "am_mb_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 从机ID信息缓冲区 */
#define AM_MB_FUNC_OTHER_REP_SLAVEID_BUF  32

/** 功能码回调函数类型 */
enum am_mb_func_cb_type {
	AM_MB_FUNC_COILS_CALLBACK,                /**< \brief 读写线圈操作回调 */
	AM_MB_FUNC_DISCRETEINPUTS_CALLBACK,       /**< \brief 读离散量操作回调 */
	AM_MB_FUNC_HOLDREGISTERS_CALLBACK,        /**< \brief 读写保持寄存器回调 */
	AM_MB_FUNC_INPUTREGISTERS_CALLBACK,       /**< \brief 读输入寄存器回调 */
};

/** 功能码回调函数操作类型 */
enum am_mb_func_cb_op {
	AM_MB_FUNC_CALLBACK_READ,   /**< \brief 读操作 */
	AM_MB_FUNC_CALLBACK_WRITE,  /**< \brief 写操作 */
};

typedef struct am_mb_slave_serv am_mb_slave_serv_t;

/** Modbus从站句柄定义 */
typedef am_mb_slave_serv_t *am_mb_slave_handle;

/******************************************************************************/

/**
 * \file
 *
 * callback function
 *
 * Read the ON/OFF status of discrete outputs(DO)in the slave. The coils status
 * is packed as per bit of data field. Status is indicated as: 1=ON, 0=OFF.
 * The LSB of the first data byte contains the coil addressed in the query.
 * If the coil quantity is not a multiple of eight, the remaining bits in the
 * final data byte will be padded with zeros.
 *
 * For example. addr = 0x0013  num = 10,
 * status of coils 20 ~ 27 is ON-ON-OFF-OFF-ON-OFF-ON-OFF 28~29 ON-ON,
 * so the buf[0] = 01010011(0x53)  buf[1] = 00000011
 *
 * buf[0]:
 * 7  |6  |5  |4  |3  |2  |1  |0
 * ---|---|---|---|---|---|---|---
 * 27 |26 |25 |24 |23 |22 |21 |20
 * OFF|ON |OFF|ON |OFF|OFF|ON |ON
 * 0  |1  |0  |1  |0  |0  |1  |1
 *
 * buf[1]:
 * 7  |6  |5  |4  |3  |2  |1  |0
 * ---|---|---|---|---|---|---|----
 * -  |-  |-  |-  |-  |-  |29 |28
 * 0  |0  |0  |0  |0  |0  |ON |ON
 * 0  |0  |0  |0  |0  |0  |1  |1
 */

/**
 * \brief 读写线圈，读离散量，读写保存寄存器，读输入寄存器回调函数原型
 * @note  当使用本库提供的读写线圈，读离散量，读写保存寄存器，
 * 		     读输入寄存器等功能码处理函数时，需要注册
 * @code
 * am_mb_exception_t my_coils_callback(am_mb_slave_t slave,
 *                                     uint8_t   *buf,
 *                                     uint16_t   addr,
 *                                     uint16_t   num)
 * {
 *     //读线圈操作
 * }
 *
 * //在启动slave之前进行如下操作：
 * am_mb_slave_register_callback(slave,
 *                            	 am_MB_FUNC_COILS_CALLBACK,
 *                            	 am_MB_FUNC_CALLBACK_READ,
 *                            	 my_coils_callback);
 * @endcode
 *
 * @param[in]     slave 从机句柄
 * @param[in,out] buf   读写操作缓冲区
 * @param[in]     addr  读写操作开始地址
 * @param[in]     num   读写操作个数
 * @return 如果没有异常，返回AM_MB_EXP_NONE，否则返回对应的异常号
 * @see am_mb_slave_register_callback
 */
typedef am_mb_exception_t (*am_mb_slave_fn_code_callback_t)(am_mb_slave_handle  slave,
	                                                        uint8_t            *p_buf,
                                                            uint16_t            addr,
                                                            uint16_t            num);

/**
 * \brief 用户功能码处理函数原型.
 * @note  当从机收到一帧数据以后，将根据功能码调用对应的处理函数。
 * 		     处理函数由用户通过\a am_mb_slave_register_handler 来注册。
 *		     同时已经定义基本的读写线圈，读离散量，读写保存寄存器，读输入寄存器
 * 		     等处理函数
 *
 * @param[in] slave Modbus从站句柄
 * @param[in,out] p_pdubuf PDU数据(包含功能码)
 *         - [in]  请求PDU数据
 *         - [out] 应答PDU数据
 * @param[in,out] p_pdulen PDU数据长度
 *         - [in]  请求PDU数据长度
 *         - [out] 应答PDU数据长度
 * @return 如果没有异常，返回AM_MB_EXP_NONE，否则返回对应的异常号
 */
typedef am_mb_exception_t (*am_mb_fn_code_handler_t) (am_mb_slave_handle  slave,
                                                  	  uint8_t            *p_pdubuf,
                                                  	  uint16_t           *p_pdulen);

/** \brief function code handler type */
typedef struct am_funcode_handle {
    uint8_t                 funcode;
    am_mb_fn_code_handler_t handler;
} am_funcode_handle_t;

/******************************************************************************/

/** \brief Modbus 从站服务 */
typedef struct am_mb_slave_serv {
    void                     *p_cookie;  /**< \brief  */
    enum am_mb_slave_status   status;    /**< \brief 从站状态  */
    enum am_mb_mode           mode;      /**< \brief 当前工作模式 */
    const struct am_mb_netif *p_netif;   /**< \brief 模式接口 */
    /** \brief 用户注册功能码表 */
    am_funcode_handle_t      *p_funcode_hdl;
    /** \brief 可支持注册的功能码个数 */
    uint8_t                   funcode_hdl_max;

    am_mb_event_t             event;   /**< \brief 从站事件 */

    /** \brief 通用功能码回调  */
    am_mb_slave_fn_code_callback_t fn_read_coils_cb;
    am_mb_slave_fn_code_callback_t fn_write_coils_cb;
    am_mb_slave_fn_code_callback_t fn_read_discre_cb;
    am_mb_slave_fn_code_callback_t fn_read_input_cb;
    am_mb_slave_fn_code_callback_t fn_read_hold_cb;
    am_mb_slave_fn_code_callback_t fn_write_hold_cb;
#if 1
    /** \brief 用户ID信息，可不定义  */
    uint8_t  id[AM_MB_FUNC_OTHER_REP_SLAVEID_BUF];
    uint16_t id_len;
#endif
} am_mb_slave_serv_t;

/** \brief Modbus从站设备 */
typedef struct am_mb_slave_dev {
    /** \brief modbus slave service */
    am_mb_slave_serv_t am_mb_slave_serv;

    union mode {
        struct am_mb_rtu   mb_rtu;   /**< \brief RTU mode private data */
        struct am_mb_ascii mb_ascii; /**< \brief ASCII mode private data */
#if AM_MB_TCP_ENABLE
        struct am_mb_tcp   mb_tcp;   /**< \brief TCP mode private data */
#endif
    } common;

} am_mb_slave_dev_t;

/** \brief modbus slave info  */
typedef struct am_mb_slave_info {
    aw_mb_timer_info_t  timer_info;  /**< \brief 定时器设备信息 */
    am_mb_serial_info_t serial_info; /**< \brief 串行链路设备信息 */
    uint8_t             slave_addr;  /**< \brief 从机地址 */
    enum am_mb_mode     mode;        /**< \brief 模式 */
} am_mb_slave_info_t;

/******************************************************************************/
/**
 * \brief 创建并初始化一个Modbus从站
 * @note  在调用所有从机函数之前，必须先调用该函数
 * @note  允许不同网络创建多个从站
 *
 * @param[in] p_s_dev : 指向从站设备的指针
 * @param[in] mode    : 通信模式
 * @param[in] p_param : 初始化参数，对于MB_RTU/MB_ASCII模式使用
 *                      struct am_mb_serial_param作为参数
 *                      对于AM_MB_TCP模式，param作为端口号参数使用,直接传入形参
 * @param[out] p_err  : 错误号. 如果对错误号不感兴趣可以传递NULL参数。
 * 返回错误值：
 * -AM_MB_ERR_NOERR           : 初始化成功.
 * -AM_MB_ERR_EINVAL          : 传入参数错误.
 * -AM_MB_ERR_ALLOC_FAIL      : 内存不足，分配从站失败.
 * -AM_MB_ERR_MODE_NO_SUPPORT : 模式不支持
 * -AM_MB_ERR_EPORTERR        ： 串行模式：分配串口或硬件定时器失败.
 * -AM_MB_ERR_EIO             : TCP模式：服务端创建失败.
 * @return Modbus从站句柄，如果发生错误返回NULL
 */
am_mb_slave_handle am_mb_slave_init (am_mb_slave_dev_t        *p_dev,
                                     const am_mb_slave_info_t *p_info,
								     am_mb_err_t              *p_err);

/**
 * \brief 关闭Modbus从站.
 * @note  退出系统或者不需要再使用从机时调用
 *
 * @param[in] slave : Modbus从站句柄
 *
 * @retval AM_MB_ERR_NOERR     : 无错误.
 * @retval AM_MB_ERR_EINVAL    : 从站参数错误.
 * @retval AM_MB_ERR_EILLSTATE : 协议栈非法状态.
 */
am_mb_err_t am_mb_slave_close (am_mb_slave_handle slave);

/**
 * \brief 启动Modbus从站.
 * @note  该函数必须在调用am_mb_slave_init函数后或者调用am_mb_slave_stop函数后被调用
 *
 * @param[in] slave            : Modbus从站句柄
 *
 * @retval AM_MB_ERR_NOERR     : 无错误.
 * @retval AM_MB_ERR_EINVAL    : 从站参数错误.
 * @retval AM_MB_ERR_EILLSTATE : 协议栈非法状态.
 */
am_mb_err_t am_mb_slave_start (am_mb_slave_handle slave);

/**
 * \brief 停止Modbus从站.
 *
 * @param[in] slave            : Modbus从站句柄
 *
 * @retval AM_MB_ERR_NOERR     : 无错误.
 * @retval AM_MB_ERR_EINVAL    : 从站参数错误.
 * @retval AM_MB_ERR_EILLSTATE : 协议栈非法状态.
 */
am_mb_err_t am_mb_slave_stop (am_mb_slave_handle slave);

/**
 * \brief 注册功能码处理函数.
 * @note  初始化Modbus从站以后，可以调用该函数来注册对应的功能码处理函数。用来扩展功能码
 *
 * @note 可以注册的功能码处理函数个数在am_mb_config.h中AM_MB_FUNC_CODE_HANDLER_MAX定义。
 *       超过该个数将无法再注册。同一个功能码只能有一个处理函数，如果注册两次同
 *       一个功能码处理函数，将只使用最后注册的处理函数。
 * @param[in] slave    : Modbus从站句柄
 * @param[in] funcode  : 功能码号
 * @param[in] handler  : 功能码处理函数
 *
 * @retval  AM_MB_ERR_NOERR  : 无错误;
 * @retval  AM_MB_ERR_EINVAL : 参数错误;
 * @retval  AM_MB_ERR_ENORES : 无法再注册功能码.
 */
am_mb_err_t am_mb_slave_register_handler (am_mb_slave_handle       slave,
                                    	  uint8_t                  funcode,
                                    	  am_mb_fn_code_handler_t  handler);

/**
 * \brief 注册读写线圈，读离散量，读写保存寄存器，读输入寄存器回调函数.
 * @note  当使用本库提供的读写线圈，读离散量，读写保存寄存器，读输入寄存器功能码
 *           处理函数时, 必须使用该函数进行回调函数注册，否则主机会获得非法功能码异常
 *
 * @param[in] slave 从机句柄
 * @param[in] type 功能码回调函数类型
 * @param[in] op 功能码回调函数操作类型
 * @param[in] callback 回调函数
 *
 * @retval AM_MB_ERR_NOERR  : 注册成功.
 * @retval AM_MB_ERR_EINVAL : 传入参数错误.
 */
am_mb_err_t am_mb_slave_register_callback (am_mb_slave_handle             slave,
                                           enum am_mb_func_cb_type        type,
                                           enum am_mb_func_cb_op          op,
                                           am_mb_slave_fn_code_callback_t callback);

/**
 * \brief Modbus从站查询操作.
 * @note  接收数据帧并处理该数据帧，其为Modbus从站协议的核心。该函数必须被周期性的调用。
 *        其时间间隔依赖具体应用和主站要求的时间间隔。
 * @note  该接口允许RTU、ASCII、TCP模式.
 * @note  RTU/ASCII串行模式下，该接口阻塞时间取决于am_mb_slave_cfg协议栈配置接口。
 *        TCP模式下，该接口为非阻塞。
 *
 * @param[in] slave : Modbus从站句柄
 *
 * @retval AM_MB_ERR_NOERR     : 无错误;
 * @retval AM_MB_ERR_EINVAL    : 参数错误;
 * @retval AM_MB_ERR_EILLSTATE : 协议栈非法状态.
 * @retval AM_MB_ERR_ENORES    : 串行模式：响应时收到新的请求.
 * @retval AM_MB_ERR_EIO       : IO错误. 串行模式：发送状态出错. TCP模式：发送失败.
 */
am_mb_err_t am_mb_slave_poll (am_mb_slave_handle slave);

/** \brief 从站配置类型 */
typedef int am_mbs_cfg_type_t;
#define AM_MBS_TYPE_USED_HW_TMR        0x01   /**< \brief 使用硬件定时器 */
#define AM_MBS_TYPE_TCP_NUM            0x03   /**< \brief TCP模式允许建立连接数配置 */
#define AM_MBS_TYPE_ACCEPT_TCP_NUM_GET 0x04   /**< \brief 获取当前已接收的连接数 */
#define AM_MBS_TYPE_ALLOW_NEW_TCP      0x05   /**< \brief 连接达到设置值后，允许关闭旧连接来接收新连接 */

/**
 * \brief 从站配置设置
 *
 * @note 若无调用该接口，则配置类型按照am_mb_config.h中参数设置.
 *
 * @param[in] type  : 配置类型：
 * - AM_MBS_TYPE_TCP_NUM            : 允许建立连接数，参数p_arg为连接数（uint32_t）;
 * - AM_MBS_TYPE_ACCEPT_TCP_NUM_GET : 获取当前已接收连接数，参数p_arg为获取的连接数（uint32_t *）;
 * - AM_MBS_TYPE_ALLOW_NEW_TCP      : 达到连接上限后，允许接收新连接而断开旧连接,p_arg为1时允许;
 *
 * @param[in] p_arg :  配置参数，取决于请求命令
 *
 * @return 若设置成功，返回AM_MB_ERR_NOERR,反之，返回相应错误值.
 */
am_mb_err_t am_mb_slave_ioctl (am_mb_slave_handle slave,
                               am_mbs_cfg_type_t  type,
                               void              *p_arg);

/**
 * \brief 从站模式获取
 *
 * @param[in]      slave  : Modbus从站句柄
 * @param[in][out] p_mode : 从站工作模式
 *
 * @retval AM_MB_ERR_EINVAL : 传入参数错误
 * @retval AM_MB_ERR_NOERR  : 无错误
 */
am_mb_err_t am_mb_slave_mode_get (am_mb_slave_handle  slave,
                                  enum am_mb_mode    *p_mode);

/**
 * \brief 设置Modbus从站信息.
 * 用来设置Modbus从站类型说明，当前状态和其他一些特殊信息。
 * @note 本实现定义了Modbus从站信息最大长度为32个字节，其中：
 *    - 栈类型说明占一个字节
 *    - 当前状态占一个字节
 *    - 其他信息长度<= 30个信息\n
 *  在使用是要特别注意.
 * @note 该接口支持需配置，默认不支持
 *
 * @param[in] slave        : Modbus从站句柄
 * @param[in] slave_id     : 类型说明
 * @param[in] is_running   : 当前状态，> 0表示运行中，== 0表示停止状态
 * @param[in] p_additional : 其他一些特殊信息
 * @param[in] alen         : 其他一些特殊信息长度
 *
 * @retval AM_MB_ERR_NOERR  : 无错误;
 * @retval AM_MB_ERR_EINVAL : 参数错误;
 * @retval AM_MB_ERR_ENORES : 资源不足.
 */
am_mb_err_t am_mb_slave_set_id (am_mb_slave_handle slave,
                                uint8_t            slave_id,
                                uint8_t            is_running,
                                const uint8_t     *p_additional,
                                uint16_t           alen);

/**
 * \brief 设置Modbus从站地址.
 * @note  默认情况下在调用am_mb_slave_init进行初始化时，初始化参数里面已经有设置从站地址；
 * 		     当如果有需要在运行时修改从站地址，可以使用该函数。
 *
 * @param[in] slave         : Modbus从站句柄
 * @param[in] addr          : 新的Modbus从站地址
 *
 * @retval AM_MB_ERR_NOERR  : 无错误;
 * @retval AM_MB_ERR_EINVAL : 参数错误; 从站不存在，地址超过最大地址， 或设置地址为广播地址
 */
am_mb_err_t am_mb_slave_set_addr (am_mb_slave_handle slave, uint8_t addr);

/**
 * \brief 分配用户自定义功能码内存
 *
 * @param[in] p_slave : 从站设备实例.
 * @param[in] p_buf   : 用户自定义功能码缓冲区起始地址
 * @param[in] num     : 分配个数
 *
 * @return    p_err   错误号，如果没有错误，其值为am_MB_ERR_NOERR
 */
am_mb_err_t am_mbs_user_funcode_alloc (am_mb_slave_handle   slave,
                                       am_funcode_handle_t *p_buf,
                                       uint8_t              num);

#if AM_MB_TCP_ENABLE

/**
 * \brief 接收连接TCP信息
 */
struct sockaddr;
struct am_mb_slave_tcp_info {
    int              socket;      /**< \brief 接收socket，用与设置socket属性  */
    struct sockaddr *p_sockaddr;  /**< \brief IP地址信息  */
};

/**
 * \brief TCP模式socket连接钩子函数注册
 *
 * @param[in] master      : 主站句柄
 * @param[in] accept_hook : 接收新连接钩子函数
 * @param[in] close_hook  : 连接被动断开，或达到连接上限断开处理钩子函数
 * @param[in] p_arg       : 钩子函数参数
 *
 * @retval AM_MB_ERR_NOERR     : 注册成功；
 * @retval AM_MB_ERR_EINVAL    : 参数错误；
 * @retval AM_MB_ERR_EILLSTATE : 协议栈非法状态
 */
am_mb_err_t am_mb_slave_tcp_hook_register (am_mb_slave_t            slave,
                                           am_mb_tcp_accept_hook_t  accept_hook,
                                           am_mb_tcp_close_hook_t   close_hook,
                                           void                    *p_arg);
#endif

#ifdef __cplusplus
}
#endif

/** @} grp_am_if_mb_slave */

#endif /* } __AM_MB_SLAVE_H */

/* end of file */
