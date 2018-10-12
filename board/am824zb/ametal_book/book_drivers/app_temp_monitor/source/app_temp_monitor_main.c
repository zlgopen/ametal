#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_input.h"
#include "am_digitron_disp.h"
#include "am_temp.h"
#include "am_zm516x.h"
#include "model_temp_monitor.h"
#include "view_zigbee1.h"
#include "view_digitron.h"
#include "view_buzzer.h"
#include "param_adjust_sm.h"
#include "param_adjust_action.h"
#include "app_temp_monitor_main.h"

static void key_callback (void *p_arg,int key_code, int key_state)
{
    if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
		    switch (key_code) {
	          case APP_KEY_CODE_SET:                    	// SET键按下
	          	  param_adjust_sm_event_key_set(p_arg);
	              break;
	          case APP_KEY_CODE_ADD:                     	// 加1键按下
	              param_adjust_sm_event_key_inc(p_arg);
	              break;
	          case APP_KEY_CODE_LR:                      	// L/R键按下
	              param_adjust_sm_event_key_lr(p_arg);
	              break;
	          case APP_KEY_CODE_DEC:                    	// 减1键按下
	              param_adjust_sm_event_key_dec(p_arg);
	              break;
	          default:
	              break;
        }
    }
}

int app_temp_monitor_main (am_zm516x_handle_t zm516x_handle,am_temp_handle_t temp_handle)
{
	model_temp_monitor_t        	model_temp;
	view_zigbee_t               	view_zigbee;
	view_buzzer_t               	view_buzzer;
	view_digitron_t             	view_digitron;
	int32_t                     	temp_cur;
	am_input_key_handler_t      	key_handler;
	param_adjust_sm_t           	param_adjust_sm;
	param_adjust_action_t       	param_adjust_action;

	am_temp_read(temp_handle, &temp_cur);
	// 初始化温控器模型，初始下限值 10，上限值40，当前温度值
	model_temp_monitor_init(&model_temp, 10, 40, temp_cur / 1000.0f);
	// 视图初始化
	view_digitron_init(&view_digitron, 0);
	view_zigbee_init1(&view_zigbee, zm516x_handle);
	view_buzzer_init(&view_buzzer);
	
	param_adjust_action_init(&param_adjust_action, 
	                         &model_temp, 
	                         &view_digitron,
		                       APP_LED0_ID, 
	                         APP_LED1_ID, 
													 APP_DIGITRON_ID);
													 
	param_adjust_sm_init(&param_adjust_sm, &param_adjust_action);
	model_attach(&(model_temp.isa), &(view_zigbee.isa));
	model_attach(&(model_temp.isa), &(view_buzzer.isa));
	am_input_key_handler_register(&key_handler, key_callback, &param_adjust_sm);
	while (1) {
		// 每隔500ms读取一次温度值
		am_temp_read(temp_handle, &temp_cur);
		model_temp_monitor_cur_set(&model_temp, temp_cur / 1000.0f);
		am_mdelay(500);
	}
//	return 0;
}




