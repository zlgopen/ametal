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
 * \brief RS200接口函数
 *
 * \internal
 * \par Modification History
 * - 1.00 19-12-12  RZ, first implementation.
 * \endinternal
 */

#ifndef __AM_RS200_H
#define __AM_RS200_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "am_uart_rngbuf.h"

/**
 * @addtogroup am_if_rs200
 * @copydoc am_rs200.h
 * @{
 */

/**
 * \name RS200控制命令
 * 
 *     控制命令用于 am_rs200_ioctl() 函数的 cmd 参数。其p_arg参数的实际类型随着
 * 命令的不同而不同。
 *
 * @{
 */

/**
 * \brief 获取RS200固件版本
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示固件版本
 *
 */
#define  AM_RS200_VERSION_GET                      1

/**
 * \brief 读取雨量状态
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示雨量状态
 *
 */
#define  AM_RS200_RAIN_STA_GET                     2

/**
 * \brief 读取系统状态
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示系统状态
 *
 */
#define  AM_RS200_SYS_STA_GET                      3

/**
 * \brief 读取光学系统校准值
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示光学系统校准值
 *
 */
#define  AM_RS200_OPTICAL_SYS_VALUE_GET            4

/**
 * \brief 设置雨量状态输出频率
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示雨量状态输出频率
 *
 */
#define  AM_RS200_RAIN_STA_OUT_FRE_SET             5

/**
 * \brief 读取雨量状态输出频率
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示雨量状态输出频率
 *
 */
#define  AM_RS200_RAIN_STA_OUT_FRE_GET             6

/**
 * \brief 设置无雨与小雨的阈值V1
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示无雨与小雨的阈值V1
 *
 */
#define  AM_RS200_THRESHOLD_V1_SET                 7

/**
 * \brief 读取无雨与小雨的阈值V1
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示无雨与小雨的阈值V1
 *
 */
#define  AM_RS200_THRESHOLD_V1_GET                 8

/**
 * \brief 设置小雨与中雨的阈值V2
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示小雨与中雨的阈值V2
 *
 */
#define  AM_RS200_THRESHOLD_V2_SET                 9

/**
 * \brief 读取小雨与中雨的阈值V2
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示小雨与中雨的阈值V2
 *
 */
#define  AM_RS200_THRESHOLD_V2_GET                 10

/**
 * \brief 设置中雨与大雨的阈值V3
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示小雨与中雨的阈值V2
 *
 */
#define  AM_RS200_THRESHOLD_V3_SET                 11

/**
 * \brief 读取中雨与大雨的阈值V3
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示小雨与中雨的阈值V2
 *
 */
#define  AM_RS200_THRESHOLD_V3_GET                 12

/**
 * \brief 设置无雨与小雨的阈值S1
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示无雨与小雨的阈值S1
 *
 */
#define  AM_RS200_THRESHOLD_S1_SET                 13

/**
 * \brief 读取无雨与小雨的阈值S1
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示无雨与小雨的阈值S1
 *
 */
#define  AM_RS200_THRESHOLD_S1_GET                 14

/**
 * \brief 设置小雨与中雨的阈值S2
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示小雨与中雨的阈值S2
 *
 */
#define  AM_RS200_THRESHOLD_S2_SET                 15

/**
 * \brief 读取小雨与中雨的阈值S2
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示小雨与中雨的阈值S2
 *
 */
#define  AM_RS200_THRESHOLD_S2_GET                 16

/**
 * \brief 设置中雨与大雨的阈值S3
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示中雨与大雨的阈值S3
 *
 */
#define  AM_RS200_THRESHOLD_S3_SET                 17

/**
 * \brief 读取中雨与大雨的阈值S3
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示中雨与大雨的阈值S3
 *
 */
#define  AM_RS200_THRESHOLD_S3_GET                 18

/**
 * \brief 设置10次判定为大雨的阈值N1
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示10次判定为大雨的阈值N1
 *
 */
#define  AM_RS200_THRESHOLD_N1_SET                 19

/**
 * \brief 读取10次判定为大雨的阈值N1
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示10次判定为大雨的阈值N1
 *
 */
#define  AM_RS200_THRESHOLD_N1_GET                 20

/**
 * \brief 设置10次判定为中雨的阈值N2
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示10次判定为中雨的阈值N2
 *
 */
#define  AM_RS200_THRESHOLD_N2_SET                 21

/**
 * \brief 读取10次判定为中雨的阈值N2
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示10次判定为中雨的阈值N2
 *
 */
#define  AM_RS200_THRESHOLD_N2_GET                 22

/**
 * \brief 设置10次判定为大雨的阈值N3
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型，其值表示10次判定为大雨的阈值N3
 *
 */
#define  AM_RS200_THRESHOLD_N3_SET                 23

/**
 * \brief 读取10次判定为大雨的阈值N3
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示10次判定为大雨的阈值N3
 *
 */
#define  AM_RS200_THRESHOLD_N3_GET                 24

/**
 * \brief 设置环境光测模式
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型
 * 0：退出环境光测模式，1：进入环境光测模式
 *
 */
#define  AM_RS200_OPTICAL_TEST_MODE_SET            25

/**
 * \brief 读取环境光值
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示环境光值
 *
 */
#define  AM_RS200_OPTICAL_TEST_VALUE_GET           26

/**
 * \brief 读取芯片温度
 *
 * 此时， p_arg 参数的类型为  uint16_t 指针类型，其值表示芯片温度
 *
 */
#define  AM_RS200_TEMP_GET                         27

/**
 * \brief 设置光学睡眠状态
 *
 * 此时， p_arg 参数的类型为  uint16_t 类型
 * 0：退出光学眠，1：进入光学睡眠
 *
 */
#define  AM_RS200_LIGHT_SLEEP_MODE_SET             28

/**
 * \brief RS200实例信息结构体定义
 */
typedef struct am_rs200_devinfo {

    /** \brief 用于串口接收的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_rxbuf;

    /** \brief 用于串口发送的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_txbuf;

    /** \brief 用于串口接收的缓冲区大小 */
    uint16_t     rxbuf_size;

    /** \brief 用于串口发送的缓冲区大小 */
    uint16_t     txbuf_size;

    /** \brief 用于设置串口接收超时 */
    uint32_t     timeout_ms;
} am_rs200_devinfo_t;

/**
 * \brief RS200设备结构体定义
 */
typedef struct am_rs200_dev {

    /** \brief UART环形缓冲区设备 */
    am_uart_rngbuf_dev_t      rngbuf_dev;
    
    /** \brief UART环形缓冲区实例句柄 */
    am_uart_rngbuf_handle_t   rngbuf_handle;
    
    /** \brief UART实例句柄 */
    am_uart_handle_t          uart_handle;

    /** \brief 用于保存设备信息指针 */
    const am_rs200_devinfo_t *p_devinfo;

} am_rs200_dev_t;

/** \brief 定义RS200操作句柄  */
typedef struct am_rs200_dev *am_rs200_handle_t;

/**
 * \brief RS200初始化函数
 *
 * 使用RS200前，应该首先调用该初始化函数，以获取操作RS200的handle。
 *
 * \param[in] p_dev      : 指向RS200设备的指针
 * \param[in] p_devinfo  : 指向RS200设备信息的指针
 * \param[in] handle     : UART标准服务操作句柄（使用该UART句柄与RS200通信）
 *
 * \return 操作RS200的handle，若初始化失败，则返回值为NULL。
 *
 * \note 设备指针p_dev指向的设备只需要定义，不需要在调用本函数前初始化。
 */
am_rs200_handle_t am_rs200_init (am_rs200_dev_t           *p_dev,
                                 const am_rs200_devinfo_t *p_devinfo,
                                 am_uart_handle_t          handle);

/**
 * \brief RS200控制函数
 *
 * \param[in] handle    : RS200实例句柄
 * \param[in] cmd       : 控制指令，例如：#AM_RS200_RAIN_STA_OUT_FRE_SET
 * \param[in,out] p_arg : 命令对应的参数，例如：2
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_EIO     : 执行错误
 * \retval -AM_ENOTSUP : 指令不支持
 *
 * \note 处理
 */
int am_rs200_ioctl (am_rs200_handle_t  handle,
                    int                cmd,
                    void              *p_arg);

/**
 * \brief 获取RS200雨量状态
 *
 * \param[in] handle : RS200实例句柄
 * \param[in] p_data : 状态存放地址
 *
 * \retval AM_OK     : 控制指令执行成功
 * \retval 其他      : 执行错误
 *
 */
int am_rs200_rain_sta_get (am_rs200_handle_t handle, uint16_t *p_data);

/**
 * \brief 获取RS200接收的数据
 *
 * \param[in] handle : RS200实例句柄
 * \param[in] p_buf  : 数据存放地址 
 * \param[in] len    : 接收的数据长度               
 *
 * \retval 接收成功字节数
 *
 */
int am_rs200_recv (am_rs200_handle_t  handle,
                   uint8_t           *p_buf,
                   uint32_t           len);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_RS200_H */

/* end of file */
