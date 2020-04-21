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
 * \brief 蜂鸣器驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#ifndef _IR_THREMOMETER_BEEP_H_
#define _IR_THREMOMETER_BEEP_H_
/*******************************************************************************
* 私有定义
*******************************************************************************/
/**
 * \brief 模式信息
 */
typedef enum _beep_nosie_t {
	BEEP_LONG_NOISE = 0,
	BEEP_SHORT_NOISE,
}beep_nosie_t;
/**
 * \brief 蜂鸣器初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void ir_thremometer_beep_init(void);
/**
 * \brief 功能函数初始化
 *
 * \param[in] times  鸣叫次数
 * \param[in] nosie  鸣叫类型，长叫 BEEP_LONG_NOISE  短叫 BEEP_SHORT_NOISE
 *
 * \return 无
 */
void ir_thremometer_beep_on(uint32_t times, beep_nosie_t nosie);

#endif
/* end of file */
