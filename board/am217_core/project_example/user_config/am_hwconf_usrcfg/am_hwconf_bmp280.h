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
 * \brief BMP280 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_BMP280_H
#define __AM_HWCONF_BMP280_H

#include "ametal.h"
#include "am_bmp280.h"

#ifdef __cplusplus
extern "C" {
#endif


/** \brief 实例初始化 */
am_bmp280_handle_t am_bmp280_inst_init (void);    
    

#ifdef __cplusplus
}
#endif

#endif

/* end of file */


