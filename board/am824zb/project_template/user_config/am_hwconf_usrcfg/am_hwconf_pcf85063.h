/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief PCF85063 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_PCF85063_H
#define __AM_HWCONF_PCF85063_H

#include "ametal.h"
#include "am_pcf85063.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief PCF85063 实例初始化（使用芯片特殊功能）
 * \return PCF85063芯片句柄，若为NULL，表明初始化失败
 * \note 当使用芯片特殊功能时，使用该函数获取芯片的句柄
 */
am_pcf85063_handle_t am_pcf85063_inst_init (void);

/**
 * \brief PCF85063 实例初始化 （使用通用的RTC功能）
 * \return RTC标准服务句柄，若为NULL，表明初始化失败
 * \note 当使用通用的RTC功能时，使用该函数获取RTC标准服务句柄
 */
am_rtc_handle_t am_pcf85063_rtc_inst_init(void);

/**
 * \brief PCF85063 实例初始化 （使用通用的闹钟功能）
 * \return 闹钟标准服务句柄，若为NULL，表明初始化失败
 * \note 当使用通用的闹钟功能时，使用该函数获取闹钟标准服务句柄
 */
am_alarm_clk_handle_t  am_pcf85063_alarm_clk_inst_init(void);

/**
 * \brief PCF85063 实例初始化 （将PCF85063用作系统时间）
 *
 * \retval  AW_OK  初始化成功
 * \retval   < 0   初始化失败，请检查C文件中各配置项是否合法
 */
int am_pcf85063_time_inst_init(void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
