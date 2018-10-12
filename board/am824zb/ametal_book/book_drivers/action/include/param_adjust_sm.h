#ifndef __PARAM_ADJUST_SM_H__
#define __PARAM_ADJUST_SM_H__

#include "ametal.h"
#include "am_softimer.h"
#include "model_temp_monitor.h"
#include "view_digitron.h"
#include "param_adjust_action.h"

#define PARAM_ADJUST_NUM  2

struct _adjust_state;
typedef struct _adjust_state adjust_state_t;
		 
// 定义参数调节状态机
typedef struct _param_adjust_sm {
	const struct _adjust_state 	*p_state;      			// 当前状态
	param_adjust_action_t     *p_action;        		// 动作类
} param_adjust_sm_t;

typedef void (*event_handle_t) (param_adjust_sm_t *p_this);

typedef struct _adjust_state {
	event_handle_t  pfn_entry;						  // 进入动作
	event_handle_t  pfn_event_set;					// SET事件处理函数
	event_handle_t  pfn_event_inc;					// 加1事件处理函数
	event_handle_t  pfn_event_lr;					  // 左移/右移事件处理函数
	event_handle_t  pfn_event_dec;					// 减1事件处理函数
	event_handle_t  pfn_exit;						    // 退出动作
} adjust_state_t;


// 状态机初始化
int param_adjust_sm_init (param_adjust_sm_t *p_this,param_adjust_action_t *p_action);

int param_adjust_sm_event_key_set (param_adjust_sm_t *p_this);			// SET键按下
int param_adjust_sm_event_key_lr  (param_adjust_sm_t *p_this);			// L/R键按下
int param_adjust_sm_event_key_inc (param_adjust_sm_t *p_this);			// 加键按下
int param_adjust_sm_event_key_dec (param_adjust_sm_t *p_this);			// 减键按下
		 
	 
#endif
