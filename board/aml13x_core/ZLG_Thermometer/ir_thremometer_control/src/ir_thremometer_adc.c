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
 * \brief 传感器参数采样
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
#include "zal_infrared_sensor_interpolation.h"
#include "ir_thremometer_adc.h"
#include "am_hc32_adc.h"
#include <stdlib.h>
#include "ir_thremometer_calibration.h"

#define OP_GAIN 500

char str[100] = {0};

#define ADC_CALIBRATION_PA4_VOL 1200
#define ADC_CALIBRATION_PA5_VOL 600
#define ADC_SAMPLE_LEN 100


static am_adc_handle_t adc_handle = 0;
static float adc_calib_k = 1;
static float adc_calib_b = 0;
static uint16_t adc_samples[ADC_SAMPLE_LEN] = {0};

/**
 * \brief 获取 ADC 转换值
 */
static uint32_t __adc_code_get (am_adc_handle_t handle, int chan)
{
    int      adc_bits = am_adc_bits_get(handle, chan);
//    int      i;
//    uint32_t sum;

    /*
     * 启动ADC转换器，采集12次 CODE 值
     *
     * 实际应用中，所使用的 ADC 位数是确定的，无需这样分多种情况考虑
     */
	uint16_t val_buf;
	am_adc_read(handle, chan, &val_buf, 1);
	return val_buf;
}
/**
 * \brief ADC均值获取
 */
static uint32_t __adc_code_get_agv (am_adc_handle_t handle, int chan)
{
    int adc_bits = am_adc_bits_get(handle, chan);
	uint32_t agv = 0;
	uint32_t sum = 0;
	uint16_t min = 0xffff;
	uint16_t max = 0;
	for(int i = 0; i < 50; i++) {
		am_adc_read(handle, chan, adc_samples, ADC_SAMPLE_LEN);
		
		for (int i = 0; i < ADC_SAMPLE_LEN; i++){
			if (adc_samples[i] > max) {
				max = adc_samples[i];
			}
			
			if (adc_samples[i] < min) {
				min = adc_samples[i];
			}
			
			agv += adc_samples[i];
		}
		agv -= (max + min);
		agv /= (ADC_SAMPLE_LEN - 2);
		sum += agv;
		
		am_mdelay(1);
	}
	
	return sum / 50;
}
/**
 * \brief 获取通道电压
 */
static float __get_adc_vol(am_adc_handle_t handle, int chan)
{
    int adc_bits = am_adc_bits_get(handle , chan);        /* 获取ADC转换精度 */
    int adc_vref = am_adc_vref_get(handle , chan);

    uint32_t adc_code;                                    /* 采样 Code 值   */
    uint32_t adc_mv;                                      /* 采样电压 */
	am_mdelay(10);
	adc_code = __adc_code_get_agv(handle, chan);
	adc_mv   = adc_code * adc_vref / ((1UL << adc_bits) - 1);

	return adc_mv;
}
/**
 * \brief 获取NTC阻值
 */
float ir_thremometer_get_ntc_res(void)
{
	uint32_t ain0 = 0, ain1 = 0;
	float res = 0;
	ain0 = __get_adc_vol(adc_handle, 0);
	ain1 = __get_adc_vol(adc_handle, 1);

	AM_DBG_INFO("ain0 = %d, ain1 = %d, ", ain0, ain1);
	res = (float)ain1 * 100 / (ain0 - ain1);
	return res;
}
/**
 * \brief 获取热电堆压差
 */
float ir_thremometer_get_ir_vol(void)
{
	uint32_t ain2 = 0, ain3 = 0;
	float vol = 0;
	ain2 = __get_adc_vol(adc_handle, 2);
	ain3 = __get_adc_vol(adc_handle, 3);
	AM_DBG_INFO("ain2 = %d, ain3 = %d\r\n", ain2, ain3);
	vol = fabs((float) ain2 - (float) ain3) / OP_GAIN;
	if (ain2 < ain3) {
		vol *= -1;
	}
	return  vol;
}
/**
 * \brief 获取目标温度
 */
float ir_thremometer_get_obj_temp(void)
{
	double res = 0, vol = -1;
	float ntc_temp = 0, obj_temp = 0;
	
	res = ir_thremometer_get_ntc_res();
	vol = ir_thremometer_get_ir_vol();
	
	vol = ir_thremometer_calib_vol(vol);
	
	ntc_temp = zal_environment_tem_value_get(res); 
	obj_temp = zal_temperature_get(ntc_temp, vol);

	sprintf(str, "res = %.2f k, vol = %.2f u, ntc_temp = %.2f, obj_temp = %.2f\r\n", res, vol, ntc_temp, obj_temp);
	AM_DBG_INFO("%s", str);
	return obj_temp;
}
/**
 * \brief 获取电池电压
 */
uint32_t ir_thremometer_get_bat_vol(void)
{
	return __get_adc_vol(adc_handle, 6);
}
/**
 * \brief 获取参考电压校准
 */
void ir_thremometer_adc_calib(void)
{
	uint32_t ain4 = 0, ain5 = 0;
	float t = 0;
	ain4 = __get_adc_vol(adc_handle, 4);
	ain5 = __get_adc_vol(adc_handle, 5);

	AM_DBG_INFO("ain4 = %d, ain5 = %d\r\n", ain4, ain5);
	
	adc_calib_k = ADC_CALIBRATION_PA4_VOL - ADC_CALIBRATION_PA5_VOL;
	t = ain4 - ain5;
	adc_calib_k /= t;
	t = adc_calib_k * ain5;
	adc_calib_b = ADC_CALIBRATION_PA5_VOL - t;
}
/**
 * \brief 电池低电压检测
 */
void ir_thremometer_bat_low_check(void)
{
	uint16_t vol[3] = {0};
	const uint16_t DIFF_CODE = 68;
	do {
		am_adc_read(adc_handle, 6, &vol, 3);
	} while(fabs(vol[0] - vol[1]) > DIFF_CODE ||
	        fabs(vol[1] - vol[2]) > DIFF_CODE ||
	        fabs(vol[0] - vol[2]) > DIFF_CODE);
}
/**
 * \brief 传感器ADC采样初始化
 */
void ir_thremometer_adc_init(void)
{
	adc_handle = am_hc32_adc_inst_init();
//	ir_thremometer_bat_low_check();
//	ir_thremometer_adc_calib();
}
/* end of file */
