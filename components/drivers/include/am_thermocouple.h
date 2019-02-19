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
 * \brief FM175XX底层驱动库头文件
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-23  sky, first implementation.
 * - 1.01 17-10-31  sdq, make some changes.
 * \endinternal
 */

#ifndef __AM_THERMOCOUPLE_H
#define __AM_THERMOCOUPLE_H

#include "am_spi.h"
#include "am_types.h"
#include "am_softimer.h"
#include <stdint.h>


/*******************************************************************************
 热电偶类型定义
*******************************************************************************/
#define THERMOCOUPLIE_J        0     /**< \brief J型热电偶 */
#define THERMOCOUPLIE_K        1     /**< \brief K型热电偶 */
#define THERMOCOUPLIE_T        2     /**< \brief T型热电偶 */
#define THERMOCOUPLIE_N        3     /**< \brief N型热电偶 */
#define THERMOCOUPLIE_R        4     /**< \brief R型热电偶 */


/**< \brief 热电偶电压值转温度函数定义 */
typedef am_err_t (*pf__n_v_to_temperature_t) (double ,double *);
/**< \brief 热电偶温度转电压函数 */
typedef am_err_t (*pf__n_temperature_to_v_t) (double ,double *);
/**
 * \brief  热电偶计算公式初始化
 *
 */
void am_thermocouplie_init(void);
/**
 * \brief  热电偶计算电压转温度
 * \param[in] type   : 热电偶类型
 * \param[in] voltage   : 电压
 * \param[out] p_temperature   :温度
 */
void am_thermocouplie_v2t(uint8_t            type,
                          double             voltage,
                          double            *p_temperature);
/**
 * \brief  热电偶计算温度转电压
 * \param[in] type   : 热电偶类型
 * \param[in] temperature   : 温度
 * \param[out] p_voltage   : 电压
 */
void am_thermocouplie_t2v(uint8_t            type,
                          double             temperature,
                          double            *p_voltage );

#endif
