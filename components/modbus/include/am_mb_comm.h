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
 * \brief modbus公共信息
 *
 * \internal
 * \par modification history
 * - 1.03 2017-03-29  mkr, transplant from aworks.
 * - 1.02 2015-05-18  cod, fix code style and comments, a little improve.
 * - 1.01 2013-03-29  liuweiyun, fix code style and comments, a little improve.
 * - 1.00 2012-05-03  ygy, first implementation
 * \endinternal
 */

#ifndef __AM_MB_COMM_H /* { */
#define __AM_MB_COMM_H

/**
 * \addtogroup grp_am_if_mb_comm
 * \copydoc am_mb_comm.h
 * @{
 */
#include "am_types.h"
#include "am_uart.h"
#include "am_timer.h"
#include "am_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/**
 * \name Modbus地址
 * @{
 */
#define AM_MB_ADDRESS_BROADCAST    0   /**< \brief Modbus广播地址 */
#define AM_MB_ADDRESS_MIN          1   /**< \brief 最小从机地址 */
#define AM_MB_ADDRESS_MAX          247 /**< \brief 最大从机地址 */
/** @} */

/**
 * \name 常用功能码
 * @{
 */
#define AM_MB_FUNC_NONE                           0   /**< \brief 不使用的功能码 */
#define AM_MB_FUNC_READ_COILS                     1   /**< \brief 读线圈功能码 */
#define AM_MB_FUNC_READ_DISCRETE_INPUTS           2   /**< \brief 读离散量功能码 */
#define AM_MB_FUNC_READ_HOLDING_REGISTER          3   /**< \brief 读保存寄存器功能码 */
#define AM_MB_FUNC_READ_INPUT_REGISTER            4   /**< \brief 读输入寄存器功能码 */
#define AM_MB_FUNC_WRITE_SINGLE_COIL              5   /**< \brief 写单个线圈功能码 */
#define AM_MB_FUNC_WRITE_REGISTER                 6   /**< \brief 写单个寄存器功能码 */
#define AM_MB_FUNC_WRITE_MULTIPLE_COILS          15   /**< \brief 写多个线功能码 */
#define AM_MB_FUNC_WRITE_MULTIPLE_REGISTERS      16   /**< \brief 写多个寄存器功能码 */
#define AM_MB_FUNC_READWRITE_MULTIPLE_REGISTERS  23   /**< \brief 读写多个寄存器功能码 */
#define AM_MB_FUNC_OTHER_REPORT_SLAVEID          17   /**< \brief 获取从机信息 */

#define AM_MB_FUNC_ERROR                         128  /**< \brief 异常功能码 */
/** @} */

/**
 * \name PDU(协议数据单元) defines
 * @{
 */
#define AM_MB_PDU_SIZE_MAX     253 /**< \brief 最大PDU长度 */
#define AM_MB_PDU_SIZE_MIN     1   /**< \brief 功能码长度 */
#define AM_MB_PDU_FUNC_OFF     0   /**< \brief 功能码在PDU中偏移 */
#define AM_MB_PDU_DATA_OFF     1   /**< \brief 数据在PDU中偏移 */
/** @} */

/**
 * \name Modbus TCP MBAP defines
 * @{
 */
#define AM_MB_TCP_TID          0	/**< \brief 事务处理标识符 */
#define AM_MB_TCP_PID          2	/**< \brief 协议标识符 */
#define AM_MB_TCP_LEN          4	/**< \brief 长度 */
#define AM_MB_TCP_UID          6	/**< \brief 单元标识符 */
#define AM_MB_TCP_FUNC         7	/**< \brief 功能码 */
/** @} */

/**
 * \name PID define
 * @{
 */
#define AM_MB_TCP_PROTOCOL_ID 	   0    /**< \brief 协议标识符 ID*/
/** @} */

/**
 * \name  TCP pseudo address define
 * @{
 */
#define AM_MB_TCP_PSEUDO_ADDRESS   255     /**< \brief Modbus TCP伪地址 */
/** @} */
/******************************************************************************/

/** 异常类型 */
typedef enum am_mb_exception {
	AM_MB_EXP_NONE                 = 0x00,     /**< \brief 没有异常 */
    AM_MB_EXP_ILLEGAL_FUNCTION     = 0x01,     /**< \brief 非法功能 */
    AM_MB_EXP_ILLEGAL_DATA_ADDRESS = 0x02,     /**< \brief 非法数据地址 */
    AM_MB_EXP_ILLEGAL_DATA_VALUE   = 0x03,     /**< \brief 非法数据值 */
    AM_MB_EXP_SLAVE_DEVICE_FAILURE = 0x04,     /**< \brief 从机设备故障 */
    AM_MB_EXP_ACKNOWLEDGE          = 0x05,     /**< \brief 确认 */
    AM_MB_EXP_SLAVE_BUSY           = 0x06,     /**< \brief 从机设备忙 */
    AM_MB_EXP_MEMORY_PARITY_ERROR  = 0x08,     /**< \brief 存储奇偶性差错*/
    AM_MB_EXP_GATEWAY_PATH_FAILED  = 0x0A,     /**< \brief 网关路径不可用 */
    AM_MB_EXP_GATEWAY_TGT_FAILED   = 0x0B      /**< \brief 网关目标设备响应失败 */
} am_mb_exception_t;

/** 串口奇偶校验模式 */
enum am_mb_parity {
	AM_MB_PAR_NONE,    /**< \brief 不校验 */
	AM_MB_PAR_ODD,     /**< \brief 奇校验 */
	AM_MB_PAR_EVEN     /**< \brief 偶校验 */
};

/** 通信模式类型 */
enum am_mb_mode {
	AM_MB_RTU,         /**< \brief 基于RTU模式的串口MODBUS   */
	AM_MB_ASCII,       /**< \brief 基于ASCII模式的串口MODBUS */
	AM_MB_TCP,         /**< \brief 基于TCP/IP协议栈的MODBUS  */
};

#ifdef __cplusplus
}
#endif

/** @} grp_am_if_mb_comm */

#endif /* } __AM_MB_COMM_H */

/* end of file */
