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
 * \brief 测温枪功能实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "am_hc32l13x_inst_init.h"
#include "am_hwconf_microport.h"
#include "am_softimer.h"
#include "am_user_lcd_display.h"
#include "zal_infrared_sensor_interpolation.h"
#include "ir_thremometer.h"
#include "ir_thremometer_adc.h"
#include "ir_thremometer_backlight.h"
#include "ir_thremometer_beep.h"
#include "ir_thremometer_key.h"
#include "ir_thremometer_lcd.h"
#include "ir_thremometer_storage.h"
#include "ir_thremometer_timer.h"
#include "ir_thremometer_power.h"
#include "ir_thremometer_calibration.h"

static char _str[100] = {0};
/** \brief 测温枪初始化*/
void ir_thremometer_init(void)
{
	ir_thremometer_power_init();
	ir_thremometer_key_init();
	ir_thremometer_beep_init();
	ir_thremometer_adc_init();
	ir_thremometer_storage_init();
	ir_thremometer_backlight_init();
	ir_thremometer_lcd_init();
}
/** \brief 华氏度转摄氏度*/
float ir_thremometer_unit_ftoc(float temp)
{
	temp -= 32;
	temp /= 1.8;
	return temp;
}
/** \brief 摄氏度转华氏度*/
float ir_thremometer_unit_ctof(float temp)
{
	temp *= 1.8;
	temp += 32;
	return temp;
}
/** \brief 显示配置*/
void ir_thremometer_set_display_config(display_t* p_display, measure_mode_t* p_mode, unit_t* p_unit)
{
	if (p_unit != NULL) {
		if (*p_unit == UNIT_FAHRENHRIT) {	
			p_display->disp_flag |= AM_USER_0F_DISP;
			p_display->disp_flag &= (~AM_USER_0C_DISP);			
		} else {
			p_display->disp_flag |= AM_USER_0C_DISP;
			p_display->disp_flag &= (~AM_USER_0F_DISP);
		}
	}
	if (p_mode != NULL) {
		if (*p_mode == MEASURE_BODY_MODE) {
			p_display->disp_flag |= AM_USER_BODY_DISP;
			p_display->disp_flag &= (~AM_USER_SUR_DISP);

		} else {
			p_display->disp_flag |= AM_USER_SUR_DISP;
			p_display->disp_flag &= (~AM_USER_BODY_DISP);
		}
	}
}
/** \brief 获取显示单位*/
unit_t ir_thremometer_get_display_unit(display_t* p_display)
{
	if ((p_display->disp_flag & AM_USER_0C_DISP) != 0) {
		return UNIT_DAGREE_CELSIUS;
	} else {
		return UNIT_FAHRENHRIT;
	}
}
/** \brief 获取显示测量模式*/
measure_mode_t ir_thremometer_get_display_mode(display_t* p_display)
{
	if ((p_display->disp_flag & AM_USER_SUR_DISP) != 0) {
		return MEASURE_THING_MODE;
	} else {
		return MEASURE_BODY_MODE;
	}
}
/** \brief 单位切换*/
int ir_thremometer_unit_switch(unit_t* p_unit, display_t* p_display)
{
	unit_t unit = ir_thremometer_get_display_unit(p_display);
	if (ir_thremometer_storage_check_in_record(p_display->id)) {
		p_unit = &unit;
	}
	
	if (*p_unit == UNIT_FAHRENHRIT) {		
		*p_unit = UNIT_DAGREE_CELSIUS;
		p_display->num = ir_thremometer_unit_ftoc(p_display->num);
	} else {
		*p_unit = UNIT_FAHRENHRIT;
		p_display->num = ir_thremometer_unit_ctof(p_display->num);
	}
	ir_thremometer_set_display_config(p_display, NULL, p_unit);
	return AM_OK;
}
/** \brief 模式切换*/
int ir_thremometer_mode_switch(measure_mode_t* p_mode, display_t* p_display)
{
	measure_mode_t mode = ir_thremometer_get_display_mode(p_display);
	if (ir_thremometer_storage_check_in_record(p_display->id)) {
		p_mode = &mode;
	}
	
	if (*p_mode == MEASURE_BODY_MODE) {
		*p_mode = MEASURE_THING_MODE;
		p_display->num = ir_thremometer_mode_btot(p_display->num);
	} else {
		*p_mode = MEASURE_BODY_MODE;
		p_display->num = ir_thremometer_mode_ttob(p_display->num);
	}
	ir_thremometer_set_display_config(p_display, p_mode, NULL);
	return AM_OK;
}
/** \brief 温度测量*/
int ir_thremometer_measure(measure_mode_t mode, unit_t unit, display_t* p_display)
{
	p_display->num = ir_thremometer_get_obj_temp();
		
	if (mode == MEASURE_BODY_MODE) {
		p_display->num = ir_thremometer_mode_ttob(p_display->num);
	}
	
	if (unit == UNIT_FAHRENHRIT) {
		p_display->num = ir_thremometer_unit_ctof(p_display->num);
	}

	ir_thremometer_set_display_config(p_display, &mode, &unit);
	
	p_display->id = ir_thremometer_storage_temp(p_display->num, mode, unit);

	return AM_OK;
}
/** \brief 查找记忆值*/
int ir_thremometer_get_temp_record(display_t* p_display)
{
	record_t record;
	measure_mode_t mode;
	unit_t unit;
	ir_thremometer_storage_load_record(&record, &mode, &unit);
	p_display->id = record.id;
	p_display->num = record.temp;
	
	ir_thremometer_set_display_config(p_display, &mode, &unit);
	
	return AM_OK;
}
/** \brief 校准值*/
int ir_thremometer_calibration(void)
{
	float obj_temps[] = {35, 40};/**校准温度**/
	ir_thremometer_beep_on(2, BEEP_SHORT_NOISE);
	for(int i = 0; i < 2; i++) {
		ir_thremometer_lcd_display_float(obj_temps[i]);
		while (ir_thremometer_get_key() != KEY_4_SHORT_PRESSED) {
			ir_thremometer_get_obj_temp();
		}
		AM_DBG_INFO("======%d=====", (int)obj_temps[i]);
		ir_thremometer_release_key();
		ir_thremometer_beep_on(3, BEEP_LONG_NOISE);
		ir_thremometer_set_calib_point(obj_temps[i]);
	}
	ir_thremometer_calib_run();
	
	ir_thremometer_release_key();
	
	return AM_OK;
}

/** \brief 电池电压检查*/
uint32_t ir_thremometer_bat_check(display_t* p_display)
{
	static uint8_t old_vbat_lv = 0;
	uint8_t vbat_lv = 0;
	uint32_t vbat = ir_thremometer_get_bat_vol() * 2;
	uint32_t bat_display_code = 0;

	if (vbat >= 3000) {
		bat_display_code =  AM_USER_BT3_DISP | AM_USER_BT2_DISP | AM_USER_BT1_DISP;
		vbat_lv = 3;
	} else if (vbat >= 2500 && vbat < 3000) {
		bat_display_code = AM_USER_BT2_DISP | AM_USER_BT1_DISP;
		vbat_lv = 2;
	} else {
		bat_display_code =  AM_USER_BT1_DISP;
		vbat_lv = 1;
	}
	
	if (vbat < 2600) {
		p_display->flash = AM_TRUE;
	} else {
		p_display->flash = AM_FALSE;
	}
	p_display->disp_flag |= bat_display_code | AM_USER_BT_DISP;
	if (old_vbat_lv != vbat_lv) {
		AM_DBG_INFO("vbat: %d", vbat);
		old_vbat_lv = vbat_lv;
		p_display->update = AM_TRUE;
	}

	return bat_display_code;
}
/** \brief 校准参数检查*/
am_bool_t ir_thremometer_calib_enter_check(void)
{
	key_press_t key_press;
	
	ir_thremometer_release_key();
	ir_thremometer_backlight_on(BACKLIGNT_RED);
	ir_thremometer_lcd_clear();
	ir_thremometer_lcd_display_float(88.8);
	ir_thremometer_beep_on(3, BEEP_LONG_NOISE);
	ir_thremometer_release_key();
	
	while(ir_thremometer_get_key() == KEY_1_LONG_PRESSED_10S);
	ir_thremometer_release_key();

	while(1) {
		key_press = ir_thremometer_get_key();
		if (key_press != KEY_RELEASE) {
			if (key_press == KEY_2_LONG_PRESSED) {
				return AM_TRUE;
			} else if (key_press == KEY_1_LONG_PRESSED_10S){
				;
			} else {
				return AM_FALSE;
			}
		}
		ir_thremometer_lcd_clear();
		am_mdelay(300);
		ir_thremometer_lcd_display_float(88.8);
		am_mdelay(300);
	}

}
/** \brief 按键事件处理*/
void ir_thremometer_key_process(display_t* p_display)
{
	key_press_t key_press = ir_thremometer_get_key();	
	static unit_t unit = UNIT_DAGREE_CELSIUS;
	static measure_mode_t mode = MEASURE_BODY_MODE;

	if (key_press != KEY_RELEASE) {
		if (key_press == KEY_1_SHORT_PRESSED) {
			ir_thremometer_unit_switch(&unit, p_display);
		} else if (key_press == KEY_2_SHORT_PRESSED) {
			ir_thremometer_mode_switch(&mode, p_display);
		} else if (key_press == KEY_3_SHORT_PRESSED) {
			ir_thremometer_get_temp_record(p_display);
		} else if (key_press == KEY_4_SHORT_PRESSED) {
			ir_thremometer_measure(mode, unit, p_display);
			ir_thremometer_beep_on(2, BEEP_SHORT_NOISE);
		} else if (key_press == KEY_1_LONG_PRESSED_10S) {
			AM_DBG_INFO("check enter calibration\r\n");
			if (ir_thremometer_calib_enter_check()) {
				AM_DBG_INFO("enter calibration\r\n");
				ir_thremometer_calibration();
			} else {
				AM_DBG_INFO("no calibration\r\n");
			}
		}
		p_display->update = AM_TRUE;
		ir_thremometer_release_key();
	}
}
/** \brief 校准参数检查*/
void ir_thremometer_check_calib(void)
{
	if (ir_thremometer_if_already_calib() != AM_TRUE) {
		while(ir_thremometer_calib_enter_check() != AM_TRUE);
		if (ir_thremometer_calibration() == AM_OK) {
			ir_thremometer_calib_init();
		}
	} else {
		ir_thremometer_calib_init();
	}
}
/** \brief 测温枪运行函数*/
void ir_thremometer_run(void)
{
	display_t display = {
		.disp_flag = 0,
		.num = 0,
		.id = 0,
		.flash = AM_FALSE,
		.update = AM_FALSE
	};

	ir_thremometer_check_calib();
	
	while(1) {
		if (ir_thremometer_time_to_powe_off()) {
			ir_thremometer_beep_on(1, BEEP_LONG_NOISE);
			ir_thremometer_power_off();
		}
		ir_thremometer_key_process(&display);
		ir_thremometer_bat_check(&display);
		ir_thremometer_lcd_display(&display);
	}
}
/* end of file */