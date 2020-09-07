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
#ifndef _IR_THREMOMETER_TIMER_H_
#define _IR_THREMOMETER_TIMER_H_

#define TIMER_MS 10
#define POWER_OFF_TIMEOUT 1000
#define LCD_FLASH_TIMEOUT 100
/**
 * \brief 定时器初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_timer_init(void);
/**
 * \brief 打开定时器
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_timer_start(void);
/**
 * \brief 关闭定时器
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_timer_stop(void);
/**
 * \brief 清除计数值
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_timer_clear_cnt(void);
/**
 * \brief 获取计数值
 *
 * \param[in] 无
 *
 * \return 无
 */
uint32_t ir_thremometer_timer_get_cnt(void);
/**
 * \brief 开机时间计时
 *
 * \param[in] 无
 *
 * \return 无
 */
am_bool_t ir_thremometer_time_to_powe_off(void);

#endif
/* end of file */
