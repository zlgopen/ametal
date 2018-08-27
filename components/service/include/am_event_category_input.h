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
 * \brief 输入事件种类：管理所有输入事件种类
 *
 * 作为一个输入事件种类，可以管理多种输入事件类型，如：
 * 1. 按键输入事件
 * 2. 绝对坐标输入事件
 * 3. 相对坐标输入事件
 *
 * \internal
 * \par Modification history
 * - 1.00 16-08-23  tee, first implementation.
 * \endinternal
 */
#ifndef __AM_EVENT_CATEGORY_INPUT_H
#define __AM_EVENT_CATEGORY_INPUT_H

#include "am_common.h"
#include "am_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 系统输入事件管理初始化
 * \return 无
 */
void am_event_category_input_init (void);

/**
 * \brief 向输入事件管理器中注册一种具体事件类型（如按键事件类型等）
 * \param[in] p_evt_type : 注册的事件类型
 * \return 标准错误号
 */
int am_event_category_intput_event_register (am_event_type_t *p_evt_type);

/**
 * \brief 注销一种具体事件类型（如按键事件类型等）
 * \param[in] p_evt_type : 注册的事件类型
 * \return 标准错误号
 */
int am_event_category_intput_event_unregister (am_event_type_t *p_evt_type);

/**
 * \brief 向输入事件管理器中注册一种具体事件处理器（便于管理）
 *
 * 任何一种输入事件发生，都将触发该处注册的事件处理器
 *
 * \param[in] p_evt_type : 注册的事件处理器
 *
 * \return 标准错误号
 */
int am_event_category_intput_handler_register (am_event_handler_t *p_evt_handler);


/**
 * \brief 注销一种具体事件处理器（便于管理）
 *
 * 任何一种输入事件发生，都将触发该处注册的事件处理器
 *
 * \param[in] p_evt_type : 注册的事件处理器
 *
 * \return 标准错误号
 */
int am_event_category_intput_handler_unregister (am_event_handler_t *p_evt_handler);


#ifdef __cplusplus
}
#endif

#endif /* __AM_FTL_H */
