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
 * \brief book`s matrixkey driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __MATRIXKEY_H
#define __MATRIXKEY_H

#include "ametal.h"

void matrixkey_init(void);                // 矩阵键盘初始化


// 矩阵按键扫描函数，确保以10ms时间间隔调用
// 返回值为0xFF，说明无按键事件
//最高位为0表示有键按下，最高位为1表示按键释放，低7位表示按键编号（0 ~ N - 1）
uint8_t matrixkey_scan(void);


// 矩阵按键扫描函数（与数码管联合使用该函数），确保以10ms时间间隔调用
// 返回值为0xFF，说明无按键事件
//最高位为0表示有键按下，最高位为1表示按键释放，低7位表示按键编号（0 ~ N - 1）
uint8_t matrixkey_scan_with_digitron(void);	// 矩阵键盘扫描函数，与数码管联合使用


// 回调函数类型定义
// 当使用软件定时器实现键盘自动扫描时，需要指定该类型的一个函数指针
// 当产生按键事件时，则会自动调用回调函数，传递的参数如下
// p_arg：初始化时指定的用户参数值
// code：最高位为0表示按键按下，最高位为1表示按键释放；低7位表示按键编号（0 ~ N - 1）
typedef void (*pfn_matrixkey_callback_t) (void *p_arg, uint8_t code);       // 定义按键回调函数类型


//带软件定时器的按键初始化函数
// p_func用于指定扫描到按键事件时调用的函数（即回调函数）
// p_arg是回调函数的自定义参数，无需使用参数时，可以设置为NULL
void matrixkey_init_with_softimer(pfn_matrixkey_callback_t p_func, void *p_arg);



#endif /* __MATRIXKEY_H */

/* end of file */
