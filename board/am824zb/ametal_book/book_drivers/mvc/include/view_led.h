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
 * \brief book`s view_led.h file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#ifndef __VIEW_LED_H__
#define __VIEW_LED_H__

#include "mvc.h"
#include "model_bool.h"

// 定义LED视图类
typedef struct _view_led{
    observer_t  isa;
    int		led_id;
}view_led_t;

int view_led_init (view_led_t *p_view_led, int led_id); 



#endif /* __VIEW_LED_H__ */
/* end of file */
