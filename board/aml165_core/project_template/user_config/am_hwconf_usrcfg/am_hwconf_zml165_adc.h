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
 * \brief aml165 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-11  htf, first implementation.
 * \endinternal
 */
#ifndef __AM_HWCONF_ZMF165_ADC_H
#define __AM_HWCONF_ZMF165_ADC_H

#include "ametal.h"
#include "am_zml165_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief aml165实例初始化
 * \return aml165服务句柄，若为NULL，表明初始化失败
 */
am_zml165_adc_handle_t am_zml165_adc_inst_init(void);

#ifdef __cplusplus
}
#endif

#endif
