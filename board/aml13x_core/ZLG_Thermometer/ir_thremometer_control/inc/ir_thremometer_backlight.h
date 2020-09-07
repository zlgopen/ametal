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
 * \brief 背光控制
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#ifndef _IR_THREMOMETER_BACKLIGHT_H_
#define _IR_THREMOMETER_BACKLIGHT_H_
/*******************************************************************************
* 私有定义
*******************************************************************************/
/**
 * \brief 模式信息
 */
typedef enum _backlight_mode_t {
	BACKLIGNT_RED = 0,
	BACKLIGNT_GREEN,
	BACKLIGNT_ORANGE,
} backlight_mode_t;
/**
 * \brief 背光灯初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_backlight_init(void);
/**
 * \brief 开启背光
 *
 * \param[in] mode 背光灯模式  不同模式灯
 *
 * \return 无
 */
void ir_thremometer_backlight_on(backlight_mode_t mode);
/**
 * \brief 关闭背光灯
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_backlight_off(void);


#endif
/* end of file */
