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
#ifndef _IR_THREMOMETER_STORAGE_H_
#define _IR_THREMOMETER_STORAGE_H_

#include "ir_thremometer.h"

typedef struct _record_t{
	uint8_t id;
	float temp;
}record_t;
/**
 * \brief 存储初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_storage_init(void);
/**
 * \brief 存储温度、模式单位信息
 *
 * \param[in] temp 目标单位
 * \param[in] mode 模式
 * \param[in] unit 单位
 *
 * \return 无
 */
int ir_thremometer_storage_temp(float temp, measure_mode_t mode, unit_t unit);
/**
 * \brief 存储校准值
 *
 * \param[in] data 校准系数
 *
 * \return 无
 */
int ir_thremometer_storage_k(float data);
/**
 * \brief 存储校准值
 *
 * \param[in] data 校准系数
 *
 * \return 无
 */
int ir_thremometer_storage_b(float data);
/**
 * \brief 读取存储温度、模式单位信息
 *
 * \param[in] temp 目标单位
 * \param[in] mode 模式
 * \param[in] unit 单位
 *
 * \return 无
 */
void ir_thremometer_storage_load_record(record_t* p_record, measure_mode_t* p_mode, unit_t* p_unit);
/**
 * \brief 校准参数初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_calib_init(void);
/**
 * \brief 校准参数检查
 *
 * \param[in] 无
 *
 * \return 校验结果
 */
int ir_thremometer_storage_check_k_b(void);
/**
 * \brief 设置校准值
 *
 * \param[in] 无
 *
 * \return 设置结果
 */
int ir_thremometer_storage_set_k_b_flag(void);
/**
 * \brief 检查 存储记忆值ID
 *
 * \param[in] 无
 *
 * \return 检查结果
 */
am_bool_t ir_thremometer_storage_check_in_record(uint8_t id);
/**
 * \brief 存储温度、模式单位信息
 *
 * \param[in] start_addr 地址
 * \param[in] p_buf 存储内容
 * \param[in] len 长度
 *
 * \return 无
 */
int ir_thremometer_storage_write(int start_addr, uint8_t *p_buf, int len);
/**
 * \brief 读取存储温度、模式单位信息
 *
 * \param[in] start_addr 地址
 * \param[in] p_buf 存储内容
 * \param[in] len 长度
 *
 * \return 无
 */
int ir_thremometer_storage_reead(int start_addr, uint8_t *p_buf, int len);

#endif
/* end of file */
