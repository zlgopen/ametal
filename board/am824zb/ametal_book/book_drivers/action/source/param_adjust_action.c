#include "param_adjust_action.h"
#include "key_process.h"
#include "am_led.h"
#include "am_digitron_dev.h"
#include "param_adjust_sm.h"
#include "am_vdebug.h"

static uint32_t __pow (uint32_t x, uint32_t y)
{
    uint32_t i = 1;
    while (y--)
        i *= x;
    return i;
}
	
static void __timer_callback (void *p_arg)
{
    key_process_t *p_this = (key_process_t *)p_arg;
    am_led_toggle(p_this-> led0_id);
    am_led_toggle(p_this-> led1_id);
}

static void __digitron_blink_set (param_adjust_action_t *p_this, am_bool_t is_blink)
{
    am_digitron_disp_blink_set(
		p_this->digitron_id,PARAM_ADJUST_NUM-1-p_this->adj_pos, is_blink);
}

static void __digitron_disp_num (param_adjust_action_t *p_this, int num)
{
    char buf[3];
	  am_snprintf(buf, 3, "%2d", num);  
	  am_digitron_disp_str(p_this->digitron_id, 0, PARAM_ADJUST_NUM, buf);
}


int param_adjust_action_init (param_adjust_action_t 	*p_this,
	                            model_temp_monitor_t  	*p_model,
	                            view_digitron_t       	*p_model_view,
	                            int                      led0_id,
	                            int                      led1_id,
	                            int                      digitron_id)
{
    if ((p_this == NULL) || (p_model == NULL)) {
	        return -AM_EINVAL;
	    }
	    p_this->p_model    = p_model;
	    p_this->p_view     = p_model_view;
	    p_this->led0_id     = led0_id;
	    p_this->led1_id     = led1_id;
	    p_this->digitron_id = digitron_id;
	    am_softimer_init(&p_this->timer, __timer_callback, p_this);
	    return AM_OK;
}

void param_adjust_action_min_start (param_adjust_action_t *p_this)
{
	  float min;
	  model_temp_monitor_min_get(p_this->p_model, &min);
	  p_this->adj_val = (int)min;
	  p_this->adj_pos = 0;
	  am_led_on(p_this->led1_id);
	  __digitron_blink_set(p_this, AM_TRUE);    		
	  __digitron_disp_num(p_this, p_this->adj_val);
}

void param_adjust_action_normal_start(param_adjust_action_t *p_this)
{
	  am_softimer_start(&p_this->timer, 500);			
	  model_attach(&(p_this->p_model->isa), &(p_this->p_view->isa));
}

void param_adjust_action_normal_stop(param_adjust_action_t *p_this)
{
		am_softimer_stop(&p_this->timer);		
		am_led_off(p_this->led0_id);					
		am_led_off(p_this->led1_id);							
		model_detach(&(p_this->p_model->isa), &(p_this->p_view->isa));
}

void param_adjust_action_min_stop(param_adjust_action_t *p_this)
{
    model_temp_monitor_min_set(p_this->p_model, p_this->adj_val);
    am_led_off(p_this->led1_id);
    __digitron_blink_set(p_this, AM_FALSE);    
}

void param_adjust_action_max_start(param_adjust_action_t *p_this)
{
    float max;
    model_temp_monitor_max_get(p_this->p_model, &max);
    p_this->adj_val = (int)max;
    p_this->adj_pos = 0;
    am_led_on(p_this->led0_id);
    __digitron_blink_set(p_this, AM_TRUE);
	  __digitron_disp_num(p_this, p_this->adj_val);
}

void param_adjust_action_max_stop(param_adjust_action_t *p_this)
{
    model_temp_monitor_max_set(p_this->p_model, p_this->adj_val);
    am_led_off(p_this->led0_id);
    __digitron_blink_set(p_this, AM_FALSE); 			
}

void param_adjust_action_inc(param_adjust_action_t *p_this)
{
    p_this -> adj_val = (p_this -> adj_val + 
	                      __pow(10, p_this -> adj_pos)) % __pow(10, PARAM_ADJUST_NUM);
	  __digitron_disp_num(p_this, p_this->adj_val);
}

void param_adjust_action_lr(param_adjust_action_t *p_this)
{
	  __digitron_blink_set(p_this, AM_FALSE);			
	  p_this -> adj_pos = (p_this -> adj_pos + 1) % PARAM_ADJUST_NUM;
	  __digitron_blink_set(p_this, AM_TRUE);			
}

void param_adjust_action_dec(param_adjust_action_t *p_this)
{
	  p_this -> adj_val = (p_this -> adj_val + __pow(10, PARAM_ADJUST_NUM) - 
	                      __pow(10, p_this -> adj_pos)) %__pow(10, PARAM_ADJUST_NUM);
	  __digitron_disp_num(p_this, p_this->adj_val);
}

