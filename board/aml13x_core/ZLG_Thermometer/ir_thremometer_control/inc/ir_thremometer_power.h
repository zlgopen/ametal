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
 * \brief 电源控制
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#ifndef _IR_THREMOMETER_POWER_H_
#define _IR_THREMOMETER_POWER_H_
/**
 * \brief 电源IO初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_power_init(void);
/**
 * \brief 电源打开
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_power_on(void);
/**
 * \brief 电源关闭
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_power_off(void);

#endif
/* end of file */
