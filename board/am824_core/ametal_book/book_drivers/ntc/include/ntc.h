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
 * \brief book`s ntc driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-18  adw, first implementation
 * \endinternal
 */

#ifndef __NTC_H
#define __NTC_H

#include "ametal.h"

void     ntc_init(void);         // NTC热敏电阻相关初始化
int16_t  ntc_temp_read (void);   // 读取温度值

#endif /* __NTC_H */

/* end of file */
