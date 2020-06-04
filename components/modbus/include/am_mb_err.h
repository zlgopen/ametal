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
 * \brief modbus错误号头文件.
 *
 * \internal
 * \par modification history
 * - 1.02 2017-03-29  mkr, transplant from aworks.
 * - 1.02 2015-05-18  cod, fix code style and comments, a little improve.
 * - 1.01 2013-03-29  liuweiyun, fix code style and comments, a little improve.
 * - 1.00 2012-05-03  ygy, first implementation
 * \endinternal
 */

#ifndef __AM_MB_ERR_H /* { */
#define __AM_MB_ERR_H

/**
 * \addtogroup grp_am_if_mb_err
 * \copydoc am_mb_err.h
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** 错误号 */
typedef enum am_mb_err {
	AM_MB_ERR_NOERR,                   /**< \brief 没有错误 */
	AM_MB_ERR_ALLOC_FAIL,              /**< \brief 分配内存失败 */
	AM_MB_ERR_ENOREG,                  /**< \brief 非法注册地址 */
	AM_MB_ERR_EINVAL,                  /**< \brief 非法参数 */
	AM_MB_ERR_EPORTERR,                /**< \brief 端口错误 */
	AM_MB_ERR_ENORES,                  /**< \brief 资源不足 */
	AM_MB_ERR_EIO,                     /**< \brief I/O错误 */
	AM_MB_ERR_EILLSTATE,               /**< \brief 协议栈非法状态 */
	AM_MB_ERR_ETIMEDOUT,               /**< \brief 超时错误 */
	AM_MB_ERR_ECRC,                    /**< \brief CRC错误  */
	AM_MB_ERR_EFRAME_LEN,              /**< \brief 帧长度错误 */
	AM_MB_ERR_EUNEXECPTION_FUNCTION,   /**< \brief 非期待功能错误 */
	AM_MB_ERR_ILLEGAL_FUNCTION,        /**< \brief 非法功能 */
	AM_MB_ERR_ILLEGAL_DATA_ADDRESS,    /**< \brief 非法数据地址 */
	AM_MB_ERR_ILLEGAL_DATA_VALUE,      /**< \brief 非法数据值 */
	AM_MB_ERR_SLAVE_DEVICE_FAILURE,    /**< \brief 从机设备故障 */
	AM_MB_ERR_ACKNOWLEDGE,             /**< \brief 确认 */
	AM_MB_ERR_SLAVE_BUSY,              /**< \brief 从机设备忙 */
	AM_MB_ERR_MEMORY_PARITY_ERROR,     /**< \brief 存储奇偶性差错*/
	AM_MB_ERR_GATEWAY_PATH_FAILED,     /**< \brief 网关路径不可用 */
	AM_MB_ERR_GATEWAY_TGT_FAILED,      /**< \brief 网关目标设备响应失败 */

	AM_MB_ERR_MODE_NO_SUPPORT,         /**< \brief 不支持的通讯模式 */
	AM_MB_ERR_THREAD_FAIL,             /**< \brief 线程创建失败 */
	AM_MB_ERR_EVENT_FAIL,              /**< \brief 事件创建失败 */
	AM_MB_ERR_EVENT_WAIT,              /**< \brief 等待事件时发送未知错误 */
	AM_MB_ERR_OPEN_FILE,               /**< \brief 打开通信文件失败 */
	AM_MB_ERR_UNKOWN_EXP,              /**< \brief 未知异常 */

	AM_MB_ERR_MASTER_RCV_ERR,          /**< \brief 主站接收数据错误  */
} am_mb_err_t;

#ifdef __cplusplus
}
#endif

/** @} grp_am_if_mb_err */

#endif /* } __AM_MB_ERR_H */

/* end of file */
