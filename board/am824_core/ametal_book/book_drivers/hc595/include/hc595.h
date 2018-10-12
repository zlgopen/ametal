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
 * \brief book`s hc595 driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __HC595_H
#define __HC595_H

#include "ametal.h"

void hc595_init(void);                                    // 初始化74HC595相关的操作
//  串行输入并行输出8位data（同步模式），等到数据全部发送完毕后才会返回
void hc595_send_data(uint8_t data);                       // 串行输入并行输出函数

// 串行输入并行输出8位data（异步模式），函数立即返回，数据发送完成后调用指定的回调函数
void hc595_send_data_async(uint8_t        data,           // 发送的数据
                           am_pfnvoid_t   pfn_callback,   // 数据发送完成回调函数
                           void          *p_arg);         // 回调函数的参数
                                                           



#endif /* __HC595_H */

/* end of file */
