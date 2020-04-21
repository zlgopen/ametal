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
 * \brief 测温枪功能函数
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#ifndef _IR_THREMOMETER_H_
#define _IR_THREMOMETER_H_

/*******************************************************************************
* 私有定义
*******************************************************************************/
/**
 * \brief 模式信息
 */
typedef enum _measure_mode_t {
	MEASURE_BODY_MODE = 0,
	MEASURE_THING_MODE,
} measure_mode_t;
/**
 * \brief 单位信息
 */
typedef enum _unit_t {
	UNIT_DAGREE_CELSIUS = 0,
	UNIT_FAHRENHRIT,
} unit_t;
/**
 * \brief 功能函数初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_init(void);
/**
 * \brief 功能函数
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_run(void);

#endif
/* end of file */
