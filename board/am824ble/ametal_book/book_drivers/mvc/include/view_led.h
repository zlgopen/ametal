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
 * \brief book`s view_led driver for AM824
 *
 * \internal
 * \par Modification History
 * - 1.00 18-09-26  ipk, first implementation.
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
