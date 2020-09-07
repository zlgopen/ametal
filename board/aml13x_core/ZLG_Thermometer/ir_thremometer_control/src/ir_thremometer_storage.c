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
 * \brief 存储控制
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#include "am_gpio.h"
#include "am_hc32l13x_inst_init.h"
#include "am_hwconf_microport.h"
#include "ir_thremometer_storage.h"
#include "ir_thremometer_adc.h"
#include "am_vdebug.h"

#define EEPROM_UNIT_C 0x01
#define EEPROM_UNIT_F 0x02
#define EEPROM_MODE_T 0x10
#define EEPROM_MODE_B 0x20

#define EEPROM_CODE1 0xAB
#define EEPROM_CODE2 0xCD

#define STROGE_NUMBER_LEN 4

#define EEPROM_STROGE_TEMP_NUMBER 32
#define EEPROM_STROGE_HEADER_SIZE 2
#define EEPROM_STROGE_CNT_SZIE 1
#define EEPROM_STROGE_BEGIN_INDEX 0
#define EEPROM_STROGE_CNT_INDEX 2
#define EEPROM_STROGE_DATA_BEGIN_INDEX 3
#define EEPROM_STROGE_TOTALLY_SIZE (EEPROM_STROGE_HEADER_SIZE + EEPROM_STROGE_CNT_SZIE + STROGE_NUMBER_LEN * EEPROM_STROGE_TEMP_NUMBER)
#define EEPROM_ALL_SIZE 255


static am_ep24cxx_handle_t ep24cxx_handle;
static uint8_t eeprom_record_number = 0;
static uint8_t record_read = 0;
static char _str[50] = {0};
/** \brief 存储参数*/
int ir_thremometer_storage_write(int start_addr, uint8_t *p_buf, int len)
{
	uint8_t data = 0;
	am_ep24cxx_write(ep24cxx_handle, start_addr, p_buf, len);
	for (int i = 0; i < len; i++) {
		am_ep24cxx_read(ep24cxx_handle, start_addr + i, &data, 1);
		if (data != p_buf[i]) {
			return AM_ERROR;
		}
	}
	return AM_OK;
}
/** \brief 读取存储参数*/
int ir_thremometer_storage_reead(int start_addr, uint8_t *p_buf, int len)
{
	return am_ep24cxx_read(ep24cxx_handle, start_addr, p_buf, len);
}
/** \brief 存储初始化*/
void ir_thremometer_storage_init(void)
{
	uint8_t code[EEPROM_STROGE_HEADER_SIZE] = {0};
	ep24cxx_handle = am_microport_eeprom_inst_init();
	am_ep24cxx_read(ep24cxx_handle, EEPROM_STROGE_BEGIN_INDEX, code, sizeof(code));
	if (code[0] != EEPROM_CODE1 || code[1] != EEPROM_CODE2) {
		code[0] = EEPROM_CODE1;
		code[1] = EEPROM_CODE2;
		ir_thremometer_storage_write(EEPROM_STROGE_BEGIN_INDEX, code, sizeof(code));
		code[0] = 0;
		for(int i = EEPROM_STROGE_CNT_INDEX; i < EEPROM_ALL_SIZE; i++) {
			ir_thremometer_storage_write(i, code, 1);
		}
		eeprom_record_number = 0;
	} else {
		am_ep24cxx_read(ep24cxx_handle, EEPROM_STROGE_CNT_INDEX, &eeprom_record_number, 1);
	}
	
}
/** \brief 存储温度值、模式、单位*/
int ir_thremometer_storage_temp(float temp, measure_mode_t mode, unit_t unit)
{
	uint8_t data[STROGE_NUMBER_LEN] = {0};
	uint8_t mode_uint_byte = 0;
	
	if (mode == MEASURE_THING_MODE) {
		mode_uint_byte |= EEPROM_MODE_T;
	} else {
		mode_uint_byte |= EEPROM_MODE_B;
	}
	
	if (unit == UNIT_DAGREE_CELSIUS) {
		mode_uint_byte |= EEPROM_UNIT_C;
	} else {
		mode_uint_byte |= EEPROM_UNIT_F;
	}
		
	data[0] = (uint32_t) temp / 10;
	data[1] = (uint32_t) temp % 10;
	data[2] = (uint32_t) (temp * 10) % 10;
	data[3] = mode_uint_byte;
		
	if (eeprom_record_number < EEPROM_STROGE_TEMP_NUMBER) {
		eeprom_record_number++;
	} else {
		eeprom_record_number = 1;
	}
		
	if(ir_thremometer_storage_write(EEPROM_STROGE_CNT_INDEX, &eeprom_record_number, 1) != AM_OK) {
		return AM_ERROR;
	}
	if (ir_thremometer_storage_write(EEPROM_STROGE_DATA_BEGIN_INDEX + (eeprom_record_number - 1) * STROGE_NUMBER_LEN, data, sizeof(data)) != AM_OK) {
		return AM_ERROR;
	}
	record_read = 0;
	
	return eeprom_record_number;
}
/** \brief 读取存储温度值、模式、单位*/
void ir_thremometer_storage_load_record(record_t* p_record, measure_mode_t* p_mode, unit_t* p_unit)
{
	uint8_t buf[STROGE_NUMBER_LEN] = {0};
	static int record_read_idx = 0;

	if (record_read <  eeprom_record_number - 1) {
		record_read++;
		record_read_idx = (eeprom_record_number - record_read - 1);
	} else {
		if (record_read == eeprom_record_number) {
			record_read_idx = EEPROM_STROGE_TEMP_NUMBER - 1;
			record_read++;
		} else {
			record_read_idx -= 1;
		}
		if (record_read_idx < 0) {
			record_read_idx = EEPROM_STROGE_TEMP_NUMBER - 1;
		}
	}
	
	am_ep24cxx_read(ep24cxx_handle, 
		EEPROM_STROGE_DATA_BEGIN_INDEX + record_read_idx * STROGE_NUMBER_LEN, 
		buf, sizeof(buf));
	p_record->id = record_read_idx + 1;
	p_record->temp = buf[0] * 10 + buf[1] + buf[2] * 0.1;
	if ((buf[3] & 0x0f) == EEPROM_UNIT_C) {
		*p_unit = UNIT_DAGREE_CELSIUS;
	} else {
		*p_unit = UNIT_FAHRENHRIT;
	}
	if ((buf[3] & 0xf0) == EEPROM_MODE_T) {
		*p_mode = MEASURE_THING_MODE;
	} else {
		*p_mode = MEASURE_BODY_MODE;
	}
}
/** \brief 记忆值查找*/
am_bool_t ir_thremometer_storage_check_in_record(uint8_t id)
{	
	return (id == eeprom_record_number) ? AM_FALSE : AM_TRUE;
}
/* end of file */
