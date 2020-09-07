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
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "ametal.h"
#include "am_user_lcd_display.h"
#include "ir_thremometer_lcd.h"
#include "ir_thremometer_backlight.h"
#include "ir_thremometer_timer.h"
#include "am_delay.h"

#define BACKLIGHT_GREEN_LIMIT_C 38.000001
#define BACKLIGHT_GREEN_LIMIT_F 100.400001

#define BAT_FLASH_TIMEOUT 50
#define BAT_DISPLAY_CODE (AM_USER_BT3_DISP | AM_USER_BT2_DISP | AM_USER_BT1_DISP)

#define BODY_DISPLAY_UPPER_C_LIMIT 43.000001
#define BODY_DISPLAY_LOWER_C_LIMIT 32.000001
#define BODY_DISPLAY_UPPER_F_LIMIT 109.400001
#define BODY_DISPLAY_LOWER_F_LIMIT 89.600001

static am_lcd_handle_t lcd_handle = 0;
/** \brief LCD初始化*/
void ir_thremometer_lcd_init(void)
{
	lcd_handle = am_user_lcd_disp_init ();
	ir_thremometer_lcd_display_all();
	ir_thremometer_backlight_init();
	am_mdelay(1000);
}
/** \brief LCD刷新计时*/
am_bool_t ir_thremometer_time_to_flash(void)
{
	static uint32_t old_cnt = 0;
	uint32_t cnt = ir_thremometer_timer_get_cnt() / LCD_FLASH_TIMEOUT;
	if (cnt != old_cnt) {
		old_cnt = ir_thremometer_timer_get_cnt() / LCD_FLASH_TIMEOUT;
		return AM_TRUE;
	} else {
		return AM_FALSE;
	}
}
/** \brief LCD显示电池电压*/
void ir_thremometer_lcd_set_bat(display_t *p_display)
{	
	static am_bool_t bat_display = AM_FALSE; 
	if (p_display->flash == AM_TRUE) {
		if (ir_thremometer_time_to_flash()) {
			bat_display = !bat_display;
		}
		if (bat_display == AM_FALSE) {
			am_user_lcd_disp_flag_disable(lcd_handle, BAT_DISPLAY_CODE);
		} else {
			p_display->update = AM_TRUE;
		}
//			p_display->disp_flag &= (~BAT_DISPLAY_CODE);
	}

}
/** \brief LCD显示背光显示级背景标志显示*/
void ir_thremometer_lcd_set_backlight(display_t *p_display)
{	
	float greem_limit = 0;

	if (p_display->disp_flag & AM_USER_0F_DISP) {
		greem_limit = BACKLIGHT_GREEN_LIMIT_F;
	} else {
		greem_limit = BACKLIGHT_GREEN_LIMIT_C;
	}
	
	if (p_display->num <= greem_limit) {
		ir_thremometer_backlight_on(BACKLIGNT_GREEN);
		p_display->disp_flag |= AM_USER_XIAO_DISP;
		p_display->disp_flag &= (~AM_USER_KU_DISP);
	} else {
		ir_thremometer_backlight_on(BACKLIGNT_RED);
		p_display->disp_flag |= AM_USER_KU_DISP;
		p_display->disp_flag &= (~AM_USER_XIAO_DISP);
	}
}
/** \brief 记忆值记忆数显示*/
static void ir_thremometer_lcd_display_num(display_t *p_display)
{
	float upper = 0, lower = 0;
	
	if (p_display->disp_flag & AM_USER_BODY_DISP) {
		if (p_display->disp_flag & AM_USER_0F_DISP) {
			upper = BODY_DISPLAY_UPPER_F_LIMIT;
			lower = BODY_DISPLAY_LOWER_F_LIMIT;
		} else {
			upper = BODY_DISPLAY_UPPER_C_LIMIT;
			lower = BODY_DISPLAY_LOWER_C_LIMIT;
		}
		
		if (p_display->num > upper) {
			am_user_lcd_disp_alarm(lcd_handle, AM_USER_DISP_HO);
		} else if (p_display->num < lower) {
			am_user_lcd_disp_alarm(lcd_handle, AM_USER_DISP_LO);
		} else {
			am_user_lcd_disp_float(lcd_handle, p_display->num);
		}
	} else {
		am_user_lcd_disp_float(lcd_handle, p_display->num);
	}
}
/** \brief LCD显示内容*/
void ir_thremometer_lcd_display(display_t *p_display)
{
	if (p_display->update) {
		
		ir_thremometer_lcd_set_backlight(p_display);
		
		ir_thremometer_lcd_set_bat(p_display);
		
		ir_thremometer_lcd_display_num(p_display);
		
//		am_user_lcd_disp_float(lcd_handle, p_display->num);
		
		am_user_lcd_disp_mem(lcd_handle, p_display->id);
		
		am_user_lcd_disp_flag_enable(lcd_handle, p_display->disp_flag | AM_USER_BZ_DISP | AM_USER_LOG_DISP);
		
		p_display->update = AM_FALSE;
	}
	ir_thremometer_lcd_set_bat(p_display);
}
/** \brief LCD显示温度值浮点数*/
void ir_thremometer_lcd_display_float(float num)
{
	am_user_lcd_disp_float(lcd_handle, num);
}
/** \brief 清屏*/
void ir_thremometer_lcd_clear(void)
{
	am_user_lcd_disp_clr(lcd_handle);
}
/** \brief 全显*/
void ir_thremometer_lcd_display_all(void)
{
	uint32_t display  = 0x3FFF;
	am_user_lcd_disp_float(lcd_handle, 888.8);
	am_user_lcd_disp_flag_enable(lcd_handle, display);
}
/* end of file */
