#ifndef __PARAM_ADJUST_ACTION_H__
#define __PARAM_ADJUST_ACTION_H__

#include "am_softimer.h"
#include "model_temp_monitor.h"
#include "view_digitron.h"

typedef struct _param_adjust_action {
    int                      led0_id;     	// LED0的ID号
    int                      led1_id;     	// LED1的ID号
    am_softimer_t            timer;       	// 用于LED闪烁的软件定时器
    model_temp_monitor_t		*p_model;  	    // 温度模型
    view_digitron_t       	*p_view;    	  // 温度模型的数码管视图
    int                      digitron_id; 	// 数码管显示器编号
    uint32_t                 adj_val;     	// 当前调节的值
    uint8_t                  adj_pos;     	// 调节状态下的调节位置，个位、十位......
} param_adjust_action_t;

	
int param_adjust_action_init (param_adjust_action_t 	*p_this,
                              model_temp_monitor_t  	*p_model,
                              view_digitron_t			    *p_model_view,
                              int                      led0_id,
                              int                      led1_id,
                              int                      digitron_id);

void param_adjust_action_min_start (param_adjust_action_t *p_this);
void param_adjust_action_normal_start(param_adjust_action_t *p_this);
void param_adjust_action_normal_stop(param_adjust_action_t *p_this);
void param_adjust_action_min_stop(param_adjust_action_t *p_this);
void param_adjust_action_max_start(param_adjust_action_t *p_this);															
void param_adjust_action_max_stop(param_adjust_action_t *p_this);														
void param_adjust_action_inc(param_adjust_action_t *p_this);														
void param_adjust_action_lr(param_adjust_action_t *p_this);	
void param_adjust_action_dec(param_adjust_action_t *p_this);
															
#endif
