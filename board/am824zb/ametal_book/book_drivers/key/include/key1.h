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
 * \brief book`s key1 driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __KEY1_H
#define __KEY1_H


#include <am_types.h>

// 回调函数类型定义
// 当使用软件定时器实现自动扫描时，则需要指定该类型的一个函数指针
// 当产生按键事件时，则自动调用回调函数，其传递的参数如下：
// p_arg：初始化时指定的用户参数值
// code为0，说明有键按下；code为1，说明按键释放
typedef void (*pfn_key1_callback_t)(void *p_arg, uint8_t code);	// 回调函数类型定义

void  key1_init(void);              // 按键初始化

// 单个独立按键扫描函数，确保以10ms时间间隔调用
// 返回值为0xFF，说明无按键事件；返回值为0，说明按键按下；返回值为1，说明按键释放
uint8_t key1_scan(void);

// 带软件定时器的按键初始化函数
// p_func用于指定扫描到按键事件时调用的函数（即回调函数）
// p_arg是回调函数的自定义参数，无需使用参数时，设置为NULL
void key1_init_with_softimer(pfn_key1_callback_t p_func, void *p_arg);

#endif /* __KEY1_H */

/* end of file */
