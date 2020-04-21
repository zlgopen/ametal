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
 * \brief 软定时器
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_board.h"
#include "am_softimer.h"
#include "ir_thremometer_timer.h"

static am_softimer_t key_softimer;
static uint32_t cnt = 0;
/** \brief 按键回调函数*/
void key_callback (void *p)
{
	cnt++;
}
/** \brief 软定时器*/
void ir_thremometer_timer_init(void)
{
    am_softimer_init(&key_softimer, key_callback, NULL);
    
}
/** \brief 开启软定时器*/
void ir_thremometer_timer_start(void)
{
	am_softimer_start(&key_softimer, TIMER_MS);
}
/** \brief 关闭软定时器*/
void ir_thremometer_timer_stop(void)
{
	am_softimer_stop(&key_softimer);
}
/** \brief 清除计数*/
void ir_thremometer_timer_clear_cnt(void)
{
	cnt = 0;
}
/** \brief 获取计数*/
uint32_t ir_thremometer_timer_get_cnt(void)
{
	return cnt;
}
/** \brief 关机计时器*/
am_bool_t ir_thremometer_time_to_powe_off(void)
{
	if (ir_thremometer_timer_get_cnt() >= POWER_OFF_TIMEOUT) {
		return AM_TRUE;
	} else {
		return AM_FALSE;
	}
}
/* end of file */
