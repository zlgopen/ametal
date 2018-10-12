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
 * \brief book`s view_buzzer.h file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#ifndef __VIEW_BUZZER_H__
#define __VIEW_BUZZER_H__
#include "ametal.h"
#include "mvc.h"

typedef struct _view_buzzer { 
    observer_t  isa;         					   
} view_buzzer_t;

int view_buzzer_init (view_buzzer_t *p_view_buzzer);		

#endif /* __VIEW_BUZZER_H__ */
/* end of file */
