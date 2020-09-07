/***************************Copyright(c)***********************************
**                Guangzhou ZHIYUAN electronics Co.,LTD.                   
**                                                                         
**                      http://www.zlg.cn                         
**                                                                         
**-------------File Info---------------------------------------------------
**File Name:            zal_infrared_sensor_interpolation.h
**Latest modified Date: 
**Latest Version:       
**Description:          
**                      
**-------------------------------------------------------------------------
**Created By:            yanjianxiong
**Created Date:         2020-2-27
**Version:              v1.0.0
**Description:          
**                      
**-------------------------------------------------------------------------
**Modified By:          yanjianxiong
**Modified Date:        2020-3-12
**Version:              
**Description:          补充25度下的VT表查表算法
**                      
**************************************************************************/

#ifndef __ZAL_INFRARED_SENSOR_INTERPOLATION_H__
#define __ZAL_INFRARED_SENSOR_INTERPOLATION_H__

#include <math.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif
/******RT 表相关**************/
	/**
	* @brief	    通过电阻值获取环境温度
	*
	* @param[in]	rInput		  输入电阻值 rInput
	* @return		返回当前的环境温度；
	*/
	__declspec(dllimport) float
		zal_environment_tem_value_get(float rInput);
	/**
	* @brief	    通过电压值获取温度值（标准25摄氏度下）
	*
	* @param[in]	vInput		  输入电压值vInput
	* @return		返回当前的环境温度；
	*/
	__declspec(dllimport) float
		zal_environment_tem25_value_get(float vInput);
	/**
	* @brief	    通过环境温度和电压值，获取测量温度值
	*
	* @param[in]	env_value	输入环境温度值 env_value
	* @param[in]	vInput		输入电压值 vInput
	* @return		返回当前的测量温度；(建议传感器只有25°标准精细表的传感器使用)
	*/
	__declspec(dllimport) float
		zal_temperature_get(float env_value, float vInput);

	/**
	* @brief	    通过环境温度和测量温度，获取电压值
	*
	* @param[in]	rInput	    输入环境温度值 env_v
	* @param[in]	vInput		输入测量温度值 obj_v
	* @return		返回当前电压值；
	*/
	__declspec(dllimport) float
		zal_voltage_value_get(float env_v, float obj_v);

		/**
	* @brief	    通过环境温度，获取电阻值
	*
	* @param[in]	env_v	    输入环境温度env_v
	* @return		返回当前电阻值;
	*/
	__declspec(dllimport) float
		zal_resistance_value_get(float env_v);

				/**
	* @brief	    通过环境温度和当前电压值，获取相对25摄氏度下温度值
	*
	* @param[in]	tInput	    输入环境温度tInput
	* @param[in]	vInput	    输入采样电压vInput
	* @return		返回目标温度值 (传感器只有25°标准精细表的传感器使用)
	*/
	__declspec(dllimport) float
		zal_temperature25_value_get(float tInput,float vInput);
	
#ifdef __cplusplus
}
#endif
#endif
