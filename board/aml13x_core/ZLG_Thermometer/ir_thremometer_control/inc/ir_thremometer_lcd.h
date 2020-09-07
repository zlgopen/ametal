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
 * \brief LCD显示控制
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#ifndef _IR_THREMOMETER_LCD_H_
#define _IR_THREMOMETER_LCD_H_

typedef struct _display_t{
	uint32_t disp_flag;
	float num;
	uint8_t id;
	am_bool_t flash;
	am_bool_t update;
}display_t;
/**
 * \brief LCD初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_lcd_init(void);
/**
 * \brief LCD显示
 *
 * \param[in] 显示信息
 *
 * \return 无
 */
void ir_thremometer_lcd_display(display_t *p_display);
/**
 * \brief LCD显示温度值
 *
 * \param[in] num  温度值
 *
 * \return 无
 */
void ir_thremometer_lcd_display_float(float num);
/**
 * \brief LCD全显
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_lcd_display_all(void);
/**
 * \brief LCD清屏
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_lcd_clear(void);

#endif
/* end of file */
