/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief PT100温度转换算法
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-29  tee, first implementation.
 * \endinternal
 */

#ifndef __PT100_TO_TEMPERATURE_H
#define __PT100_TO_TEMPERATURE_H

/**
 * \brief 依据PT100的电阻测量值（单位：Ω）计算温度值（单位：℃）
 * \param[in] : PT100的测量电阻，单位：Ω
 * \return 温度值
 */
float pt100_to_temperature(float pt100);

#endif

/* end of file */

