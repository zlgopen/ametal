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
 * \brief Modbus用户配置文件
 *
 * \internal
 * \par modification history
 * - 1.03 2017-03-29  mkr, transplant from aworks
 * - 1.02 2015-05-13  cod, Modify for aworks1.0, fix code style
 * - 1.01 2012-12-04  liuweiyun, Modify for aworks1.0, fix code style
 * - 1.00 2012-05-13  ygy, first implementation
 * \endinternal
 */

#ifndef __AM_MB_CONFIG_H
#define __AM_MB_CONFIG_H

/**
 * \addtogroup grp_am_if_mb_config
 * \copydoc am_mb_config.h
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Modbus Common Configuration
 ******************************************************************************/

/**
 * \brief 串行最大ADU长度
 * - 该长度包括从机地址和检验数据长度
 * - 对于某些应用，可能不需要很长的一帧数据，可以考虑适当剪裁以降低RAM使用
 * - RTU模式长度为4~256， ASCII模式长度为3~255
 */
#define AM_MB_MAX_SER_ADU_LENGTH   256

/**
 * \brief TCP模式下最大ADU长度
 * - 对于某些应用，可能不需要很长的一帧数据，可以考虑适当剪裁以降低RAM使用
 */
#define AM_MB_MAX_TCP_ADU_LENGTH   256

/**
 * \brief RTU模式T3.5时间设置（单位：微秒）
 * - 在RTU模式中，隐含着对t3.5（帧间延迟时间）引起的大量中断的处理，
 *   在较高的通信波特率下，这将导致CPU负担加重，
 *   因此当波特率小于或等于19200位/秒的情况下，必须严格遵守这两个定时
 * - 而当波特率大于19200位/秒时，应该使用固定值，建议帧间延迟时间设置为1.750ms
 * - 当使用ASCII模式时，不需要对该宏进行配置
 */
#define AM_MB_SERIAL_RTU_BAUDRATE_OVER_19200_FRAME_DELAY_US    1750ul

/**
 * \brief ASCII模式字符间超时设置
 * 在ASCII模式中，字符间超时时间没有固定，可以根据具体需求来设置
 */
#define AM_MB_ASCII_TIMEOUT_SEC    1

/*******************************************************************************
 * Modbus Slave Station Configuration
 ******************************************************************************/
/** 从机通信模式配置 */
#define AM_MB_SERIAL_ASCII_ENABLE  1   /**< \brief 串口ASCII模式 */
#define AM_MB_SERIAL_RTU_ENABLE    1   /**< \brief 串口RTU模式 */
#define AM_MB_TCP_ENABLE           0   /**< \brief TCP/IP模式,暂不支持TCP/IP模式 */

/**
 * \brief 使能接收双缓冲（RTU和ASCII模式）
 * 如果使能该配置，将使用2*AM_MB_MAX_SER_ADU_LENGTH RAM空间来处理存放PDU数据帧。
 * 使用该功能，可以保证在第一帧处理超时，新一帧数据到来时，保证第二帧数据完整性
 */
#define AM_MB_SER_PDU_RECV_BUF_ENABLE  1

/*******************************************************************************
 * Modbus Master Station Configuration
 ******************************************************************************/
/** 主站通信模式配置 */
#define AM_MB_MASTER_RTU_ENABLE    1   /**< \brief 串口RTU模式 */
#define AM_MB_MASTER_ASCII_ENABLE  1   /**< \brief 串口ASCII模式 */
#define AM_MB_MASTER_TCP_ENABLE    0   /**< \brief TCP/IP模式 */

/**
 * \brief 主站等待从站应答超时时间（毫秒）
 */
#define AM_MB_MASTER_TIMEOUT_MS_RESPOND  2000
#if ((AM_MB_MASTER_TIMEOUT_MS_RESPOND < 50)  ||\
     (AM_MB_MASTER_TIMEOUT_MS_RESPOND > 100000))
#error "AM_MB_MASTER_TIMEOUT_MS_RESPOND value must be in 50~100000ms"
#endif

/**
 * \brief 主站广播时的转换延迟时间（毫秒）
 */
#define AM_MB_MASTER_DELAY_MS_CONVERT   100

/**
 * \brief TCP模式下单元标示符使用从站地址使能
 * - 对于某些应用，如Modbus服务器连接到串行链路，可使用单元标示符识别从站地址
 * - 若使能，单元标示符当做目标从站地址使用；若不使能，单元标示符值默认为0xFF
 */
#define AM_MB_TCP_UID_AS_SLAVEADDR   0

/**
 * \brief 主站TCP模式下，默认连接超时时间(ms)
 */
#define AM_MB_TCP_CONNECT_TIMEOUT    5000

#ifdef __cplusplus
}
#endif

/* @} grp_am_if_mb_config */

#endif /* } __AM_MB_CONFIG_H */


/* end of file */
