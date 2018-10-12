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
 * \brief book`s key_ process.c file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_led.h"
#include "am_vdebug.h"
#include "key_process.h"
#include "param_adjust_sm.h"
#include "am_digitron_dev.h"
#include "string.h"

#define __ADJ_POS_NUM  2

static void __digitron_disp_num (key_process_t *p_this, int num)
{
    char buf[3];
    am_snprintf(buf, 3, "%2d", num);
    am_digitron_disp_str(p_this -> digitron_id, 0, strlen(buf), buf);
}

static void __digitron_blink_set (key_process_t *p_this, am_bool_t is_blink)
{
    am_digitron_disp_blink_set(
		p_this->digitron_id,PARAM_ADJUST_NUM-1-p_this->adj_pos,is_blink); 
}


static void __timer_callback (void *p_arg)
{
    key_process_t *p_this = (key_process_t *)p_arg;
    am_led_toggle(p_this-> led0_id);
    am_led_toggle(p_this-> led1_id);
}

static uint32_t __pow (uint32_t x, uint32_t y)
{
    uint32_t i = 1;
    while (y--)
        i *= x;
    return i;
}

int key_process_inc (key_process_t *p_this)
{
    if (p_this->adj_state != __ADJ_STATE_NORMAL) {
	    p_this -> adj_val = (p_this -> adj_val + 
				      __pow(10, p_this -> adj_pos))%__pow(10, PARAM_ADJUST_NUM);
         __digitron_disp_num(p_this, p_this->adj_val);
    }
    return AM_OK;
}

int key_process_dec (key_process_t *p_this)
{
    if (p_this->adj_state != __ADJ_STATE_NORMAL) {
	     p_this->adj_val = (p_this->adj_val + __pow(10, __ADJ_POS_NUM) -
			                    __pow(10, p_this->adj_pos)) % __pow(10, __ADJ_POS_NUM);
       __digitron_disp_num(p_this, p_this->adj_val);
    }
    return AM_OK;
}

int key_process_lr (key_process_t *p_this)
{
	if (p_this -> adj_state != __ADJ_STATE_NORMAL) {
	__digitron_blink_set((void *)p_this, AM_FALSE);     // 旧的调节位停止闪烁
		p_this -> adj_pos = (p_this -> adj_pos + 1) % __ADJ_POS_NUM;
	__digitron_blink_set((void *)p_this, AM_TRUE);      // 新的调节位开始闪烁
	}
	return AM_OK;
}


int key_process_init (
	key_process_t         	*p_this,
	model_temp_monitor_t  	*p_model,
	view_digitron_t       	*p_view,
	int                   	led0_id,
	int                   	led1_id,
	int                   	digitron_id)
{
    p_this -> p_model     = p_model;
    p_this -> p_view      = p_view;
    p_this -> led0_id      = led0_id;
    p_this -> led1_id      = led1_id;
    p_this -> digitron_id   = digitron_id;

    p_this -> adj_state     = __ADJ_STATE_NORMAL;
    am_softimer_init(&p_this -> timer, __timer_callback, p_this);
    am_softimer_start(&p_this -> timer, 500);
    model_attach(&(p_this -> p_model -> isa), &(p_this -> p_view -> isa));
    return AM_OK;
}

int  key_process_set (key_process_t *p_this)
{
    if (p_this->adj_state == __ADJ_STATE_NORMAL) {
		    float min;
	      model_temp_monitor_min_get(p_this->p_model, &min);
	      model_detach(&(p_this->p_model->isa), &(p_this->p_view->isa));
	      p_this->adj_val = (int)min;
	      __digitron_disp_num(p_this, p_this->adj_val); 	// 显示调节值
	      p_this->adj_pos = 0;
		    __digitron_blink_set(p_this, AM_TRUE);   	// 调节位个位闪烁
		    am_led_on(1);
		    am_led_off(0);
		    p_this->adj_state = __ADJ_STATE_MIN;

	  } else if (p_this->adj_state == __ADJ_STATE_MIN) {
		     // 调节值作为新的下限值设置到模型中，并从模型中获取上限值作为新的调节值
		     float max;
		     model_temp_monitor_min_set(p_this->p_model, (float)p_this->adj_val);
		     model_temp_monitor_max_get(p_this->p_model, &max);
		     p_this->adj_val = (int)max;
		     __digitron_disp_num(p_this, p_this->adj_val);	// 显示调节值
		     __digitron_blink_set(p_this, AM_FALSE);  	// 原调节位停止闪烁
		     p_this->adj_pos = 0;
		     __digitron_blink_set(p_this, AM_TRUE);   // 调节位个位闪烁
		     am_led_on(0);
		     am_led_off(1);
		     p_this->adj_state = __ADJ_STATE_MAX;
	  } else if (p_this->adj_state == __ADJ_STATE_MAX) {
		     // 调节值作为新的上限值设置到模型中
		     model_temp_monitor_max_set(p_this->p_model, (float)p_this->adj_val);
		     // 调节位停止闪烁
		     __digitron_blink_set(p_this, AM_FALSE);
		     // 回到正常状态，两灯闪烁，重新连接温度模型和数码管视图
		     am_softimer_start(&p_this->timer, 500);   // 两灯闪烁
		     model_attach(&(p_this->p_model->isa), &(p_this->p_view->isa));
		     p_this->adj_state = __ADJ_STATE_NORMAL;
	  }
	  return AM_OK;
}


/* end of file */


