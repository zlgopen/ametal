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
 * \brief 
 *
 * \internal
 * \par Modification History
 * - 1.00 18-09-26  ipk, first implementation.
 * \endinternal
 */
#include "view_led.h"
#include "am_led.h"

static void __view_led_update (observer_t *p_view, model_t *p_model)
{
    view_led_t *p_view_led = (view_led_t *)p_view;
	  am_bool_t  value;
	
	  if (model_bool_get((model_bool_t *)p_model, &value) == 0) {
	      if (value) {
	          am_led_on(p_view_led -> led_id);
	      } else {
	          am_led_off(p_view_led -> led_id);
	      	}
	  }
}

int view_led_init (view_led_t *p_view_led, int led_id)
{
		if (p_view_led == NULL) {
			 return -1;
		}
		view_init(&(p_view_led -> isa), __view_led_update);
		p_view_led->led_id  = led_id;             		// Ê¹ÓÃµÄLEDºÅ
		return 0;
}

/* end of file */
