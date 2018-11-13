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
 * \brief 程序清单 10.60
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
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
#include "am_lpc82x_inst_init.h"
#include "am_hwconf_lm75.h"
#include "am_hwconf_miniport_view_key.h"

static void key_callback (void *p_arg,int key_code, int key_state, int keep_time)
{
	  if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
	  	  switch (key_code) {
	          case KEY_0:          // SET键按下
	          	  param_adjust_sm_event_key_set(p_arg);
	              break;
	          case KEY_1:          // 加1键按下
	              param_adjust_sm_event_key_inc(p_arg);
	              break;
            case KEY_2:          // L/R键按下
	          		param_adjust_sm_event_key_lr(p_arg);
	              break;
	          case KEY_3:          // 减1键按下
	              param_adjust_sm_event_key_dec(p_arg);
	              break;
	          default:
	              break;
	      }
	  }
}

int am_main (void)
{
	model_temp_monitor_t        	model_temp;
	view_zigbee_t               	view_zigbee;
	view_buzzer_t               	view_buzzer;
	view_digitron_t             	view_digitron;
	int32_t                     	temp_cur;
	
	am_input_key_handler_t      	key_handler;
	param_adjust_sm_t           	param_adjust_sm;
	param_adjust_action_t       	param_adjust_action;
	am_zm516x_handle_t          	zm516x_handle = am_zm516x_inst_init();
	am_temp_handle_t            	temp_handle   = am_temp_lm75_inst_init();

    // 初始化，并设置8段ASCII解码
    am_miniport_view_key_inst_init();
    am_digitron_disp_decode_set(0, am_digitron_seg8_ascii_decode);
    am_temp_read(temp_handle, &temp_cur);
    // 初始化温控器模型，初始下限值 10，上限值40，当前温度值
    model_temp_monitor_init(&model_temp, 10, 40, temp_cur / 1000.0f);
    // 视图初始化
    view_digitron_init(&view_digitron, 0);
    view_zigbee_init1(&view_zigbee, zm516x_handle);
    view_buzzer_init(&view_buzzer);
    // 动作类和状态机初始化
	  param_adjust_action_init(&param_adjust_action, &model_temp, &view_digitron, 0, 1, 0);
		
	  param_adjust_sm_init(&param_adjust_sm, &param_adjust_action);
		
	  model_attach(&(model_temp.isa), &(view_zigbee.isa));
	  model_attach(&(model_temp.isa), &(view_buzzer.isa));
	  am_input_key_handler_register(&key_handler, key_callback, &param_adjust_sm);
	  while (1){
          // 每隔500ms读取一次温度值
          am_temp_read(temp_handle, &temp_cur);
          model_temp_monitor_cur_set(&model_temp, temp_cur / 1000.0f);
          am_mdelay(500);
	  }
}

/* end of file */

