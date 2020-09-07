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
#ifndef _IR_THREMOMETER_ADC_H_
#define _IR_THREMOMETER_ADC_H_

#include "ir_thremometer.h"
/**
 * \brief 采样ADC初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_adc_init(void);
/**
 * \brief 获取NTC阻值
 *
 * \param[in] 无
 *
 * \return NTC阻值
 */
float ir_thremometer_get_ntc_res(void);
/**
 * \brief 获取电堆两端压差
 *
 * \param[in] 无
 *
 * \return 电堆压差
 */
float ir_thremometer_get_ir_vol(void);
/**
 * \brief 获取目标温度
 *
 * \param[in] 无
 *
 * \return 目标温度
 */
float ir_thremometer_get_obj_temp(void);
/**
 * \brief 获取电池电压
 *
 * \param[in] 无
 *
 * \return 电池电压
 */
uint32_t ir_thremometer_get_bat_vol(void);
/**
 * \brief 获取校准参数函数
 *
 * \param[in] obj_temp  目标温度
 * \param[in] vol_real  真实压降
 * \param[in] vol_in_table  VT表参数
 *
 * \return 校准系数
 */
void ir_thremometer_calibration_vol(float obj_temp, float* vol_real, float* vol_in_table);
/**
 * \brief 重置校准系数
 *
 * \param[in] k  校准系数K
 * \param[in] b  校准系数B
 *
 * \return 无
 */
void ir_thremometer_set_calib_pram(float k, float b);
/**
 * \brief ADC参考源校准
 *
 * \param[in] 无
 *
 * \return 无
  */
void ir_thremometer_adc_calib(void);

#endif

/* end of file */
