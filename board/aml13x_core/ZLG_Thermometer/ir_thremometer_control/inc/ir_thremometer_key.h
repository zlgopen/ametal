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
 * \brief 按键检测
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#ifndef _IR_THREMOMETER_KEY_H_
#define _IR_THREMOMETER_KEY_H_
/**
 * \brief 按键事件
 */
typedef enum _key_press_t {
	KEY_1_SHORT_PRESSED = 0,
	KEY_2_SHORT_PRESSED,
	KEY_3_SHORT_PRESSED,
	KEY_4_SHORT_PRESSED,
	KEY_1_LONG_PRESSED,
	KEY_2_LONG_PRESSED,
	KEY_3_LONG_PRESSED,
	KEY_4_LONG_PRESSED,
	KEY_1_LONG_PRESSED_10S,
	KEY_PRESSING,
	KEY_RELEASE,
} key_press_t;
/**
 * \brief 按键初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_key_init(void);
/**
 * \brief 按键事件获取
 *
 * \param[in] 无
 *
 * \return 按键事件
 */
key_press_t ir_thremometer_get_key(void);
/**
 * \brief 按键释放
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_release_key(void);
/**
 * \brief 设置按键键值
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_set_default_key(void);

#endif
/* end of file */
