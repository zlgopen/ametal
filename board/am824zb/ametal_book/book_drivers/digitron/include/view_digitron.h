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
 * \brief book`s view_digitron.h file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#ifndef __VIEW_DIGITRON_H__
#define __VIEW_DIGITRON_H__
#include "ametal.h"
#include "mvc.h"

typedef struct _view_digitron { 
    observer_t  isa;        				  // 基类派生
    int      		digitron_id;  				// 数码管显示器ID号
} view_digitron_t;



//  数码管视图初始化
int view_digitron_init (view_digitron_t *p_view_digitron, int digitron_id);


#endif /* __VIEW_DIGITRON_H__ */

/* end of file */
