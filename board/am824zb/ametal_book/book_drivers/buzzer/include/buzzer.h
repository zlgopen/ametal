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
 * \brief book`s buzzer driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __buzzer_H
#define __buzzer_H


#include <stdint.h>

void buzzer_init(void);							        // 板级初始化，默认1KHz频率
void buzzer_freq_set(uint32_t freq);				// 配置发声频率，freq指定发声的频率
void buzzer_on(void);							          // 打开蜂鸣器，开始鸣叫
void buzzer_off(void);							        // 关闭蜂鸣器，停止鸣叫

//蜂鸣器同步鸣叫ms毫秒，会等到鸣叫结束后函数才会返回
void buzzer_beep(unsigned int ms);

//蜂鸣器异步鸣叫ms毫秒，函数会立即返回，蜂鸣器鸣叫指定的时间后自动停止
void buzzer_beep_async(unsigned int ms);



#endif /* __buzzer_H */

/* end of file */
