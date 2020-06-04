/*******************************************************************************
*                                 AWORKS
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2012 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief modbus register function code head file.
 *
 * \internal
 * \par modification history
 * - 1.03 2017-03-29  mkr, transplant from aworks.
 * - 1.02 2015-05-22  cod, fix code style and comments, a little improve.
 * - 1.01 2013-03-29  liuweiyun, fix code style and comments, a little improve.
 * - 1.00 2012-07-05  ygy, first implementation
 * \endinternal
 */

#ifndef __AM_MB_REGFUNCTION_H /* { */
#define __AM_MB_REGFUNCTION_H

/**
 * \addtogroup grp_am_if_mb_regfunction
 * \copydoc am_mb_regfunction.h
 * @{
 */

#include "am_mb_slave.h"
#include "am_mb_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 获取从站信息
 * @note  获取Modbus从栈类型说明，当前状态和其他一些特殊信息。
 *
 * @param[in]     : slave 从机句柄
 * @param[in,out] : p_frame 从站信息缓冲区
 *
 * @return ID从站信息长度
 */
uint16_t am_mb_get_slave_id (am_mb_slave_handle slave, uint8_t *p_frame);

/**
 * \brief 获取功能码的处理函数
 *
 * @param[in] : funCode 功能码
 *
 * @return    : 功能码对应的处理函数
 */
am_mb_fn_code_handler_t am_mb_fn_code_handler_get (am_mb_slave_handle slave,
                                                   uint8_t       funcode);

/**
 * \brief 读线圈功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mbRegisterFunCodeCallback
 *		      注册其读线圈具体操作回调函数
 *
 * @param[in]     slave   : 从机句柄
 * @param[in,out] p_pdu   : 带功能码的PDU缓冲区
 * @param[in,out] p_len   : PDU长度指针
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_read_coils (am_mb_slave_handle slave,
                                   	   uint8_t 	    *p_pdu,
                                       uint16_t 	*p_len);

/**
 * \brief 读离散量功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mb_register_funcode_callback
 * 		      注册其读离散量具体操作回调函数
 *
 * @param[in]    slave   : 从机句柄
 * @param[in,out] p_pdu  : 带功能码的PDU缓冲区
 * @param[in,out] p_len  : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_read_discrete_inputs (am_mb_slave_handle slave,
                                                 uint8_t 	  *p_pdu,
                                                 uint16_t 	  *p_len);

/**
 * \brief 读保存寄存器功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mbRegisterFunCodeCallback
 * 	                注册其读保存寄存器具体操作回调函数
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu : 带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_read_hold_regs (am_mb_slave_handle slave,
                                           uint8_t 	    *p_pdu,
                                           uint16_t 	*p_len);

/**
 * \brief 读输入寄存器功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mbRegisterFunCodeCallback
 * 		     注册其读输入寄存器具体操作回调函数
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu : 带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_read_input_regs (am_mb_slave_handle slave,
                                            uint8_t 	 *p_pdu,
                                            uint16_t 	 *p_len);

/**
 * \brief 写单个线圈功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mbRegisterFunCodeCallback
 * 		     注册其写线圈具体操作回调函数；如果写多个线圈已经有注册写线圈具体操作回调函数，
 * 		     就不需要再注册。
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu : 带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_write_single_coil (am_mb_slave_handle slave,
                                         	  uint8_t 	   *p_pdu,
                                              uint16_t 	   *p_len);

/**
 * \brief 写单个寄存器功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 *        同时要使用am_mbRegisterFunCodeCallback
 *        注册其写寄存器具体操作回调函数；如果写多个寄存器已经有注册写寄存器具体操作
 *        回调函数，就不需要再注册
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu : 带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_write_single_reg (am_mb_slave_handle slave,
                                             uint8_t 	  *p_pdu,
                                             uint16_t 	  *p_len);

/**
 * \brief 写多个线圈功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mbRegisterFunCodeCallback
 * 		     注册其写线圈具体操作回调函数；如果写单个线圈已经有注册写线圈具体操作回调函数，
 * 		     就不需要再注册
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu : 带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_write_multiple_coils (am_mb_slave_handle slave,
                                                 uint8_t 	  *p_pdu,
                                                 uint16_t 	  *p_len);

/**
 * \brief 写多个寄存器功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mbRegisterFunCodeCallback
 * 		     注册其写寄存器具体操作回调函数；如果写多个寄存器已经有注册写寄存器具体操作
 * 		     回调函数，就不需要再注册
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu : 带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_write_multiple_regs (am_mb_slave_handle slave,
                                                uint8_t 	 *p_pdu,
                                                uint16_t 	 *p_len);

/**
 * \brief 读写多个寄存器功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     同时要使用am_mbRegisterFunCodeCallback
 * 		     注册其读和写寄存器具体操作回调函数；如果已经注册其读和写寄存器具体操作
 * 		     回调函数，就不需要再注册
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu :带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 */
am_mb_exception_t am_mb_fn_rw_multiple_regs (am_mb_slave_handle slave,
										     uint8_t 	  *p_pdu,
										     uint16_t 	  *p_len);

/**
 * \brief 报告从机信息功能码处理函数.
 * @note  如果需要该功能，需要使用函数am_mb_slave_register_handler来注册。
 * 		     从机可以使用am_mb_slave_set_id来设置从机的信息。
 *
 * @param[in]     slave : 从机句柄
 * @param[in,out] p_pdu : 带功能码的PDU缓冲区
 * @param[in,out] p_len : PDU长度
 * @return 如果没有异常，返回am_MB_EXP_NONE，否则返回对应的异常号
 * @see am_mb_slave_set_id
 */
am_mb_exception_t am_mb_fn_report_slave_id (am_mb_slave_handle slave,
                                       	    uint8_t 	 *p_pdu,
                                            uint16_t     *p_len);

#ifdef __cplusplus
}
#endif

/* @} grp_am_if_mb_regfunction */

#endif /* @} __AM_MB_REGFUNCTION_H */

