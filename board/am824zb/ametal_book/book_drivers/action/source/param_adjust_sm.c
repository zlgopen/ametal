#include "param_adjust_sm.h"
#include "am_digitron_dev.h"
#include "key_process.h"
#include "am_led.h"
#include "am_vdebug.h"

static void state_min_entry(param_adjust_sm_t *p_this);
static void state_min_exit (param_adjust_sm_t *p_this);
static void state_event_lr(param_adjust_sm_t *p_this);
static void state_event_inc(param_adjust_sm_t *p_this);
static void state_event_dec(param_adjust_sm_t *p_this);
static void state_transition(param_adjust_sm_t *p_this, const adjust_state_t *p_new_state);
static void state_max_event_set (param_adjust_sm_t *p_this);
static void state_max_entry (param_adjust_sm_t *p_this);
static void state_max_exit (param_adjust_sm_t *p_this);
static void state_min_event_set(param_adjust_sm_t *p_this);
static void state_normal_entry(param_adjust_sm_t *p_this);
static void state_normal_exit(param_adjust_sm_t *p_this);
static void state_normal_event_set(param_adjust_sm_t *p_this);
static void state_event_ignore (param_adjust_sm_t *p_this);

const adjust_state_t  state_normal = {
	state_normal_entry,							  // 详见程序清单9.35
	state_normal_event_set,						// 详见程序清单9.38
	state_event_ignore,						  	// 忽略函数，处理为空
	state_event_ignore,							  // 忽略函数，处理为空
	state_event_ignore,						  	// 忽略函数，处理为空
	state_normal_exit						    	// 详见程序清单9.36
};

static const adjust_state_t state_adjust_max = {
  		state_max_entry,							  // 详见详见程序清单9.46
  		state_max_event_set,						// 详见程序清单9.48
  		state_event_inc,							  // 详见程序清单9.43
  		state_event_lr,							    // 详见程序清单9.42
  		state_event_dec,						  	// 详见程序清单9.44
  		state_max_exit,							    // 详见程序清单9.47
};

static const adjust_state_t state_adjust_min = {
	  state_min_entry,								  // 详见程序清单9.40
	  state_min_event_set,							// 详见程序清单9.48
	  state_event_inc,							  	// 详见程序清单9.43
	  state_event_lr,							    	// 详见程序清单9.42
	  state_event_dec,							  	// 详见程序清单9.44
	  state_min_exit,							    	// 详见程序清单9.41
};

static void state_max_event_set (param_adjust_sm_t *p_this)
{
    state_transition(p_this, &state_normal);			// 转换至normal状态
}

static void state_min_event_set(param_adjust_sm_t *p_this)
{
    state_transition(p_this, &state_adjust_max);		// 转换至adjust_max
}

static void state_normal_event_set(param_adjust_sm_t *p_this)
{
    state_transition(p_this, &state_adjust_min);      	// 转换至最小值调节状态
}

static void state_transition(param_adjust_sm_t *p_this, const adjust_state_t *p_new_state)
{
    p_this -> p_state -> pfn_exit(p_this);
    p_this -> p_state = p_new_state;
    p_this -> p_state -> pfn_entry(p_this);
}


static void state_normal_entry(param_adjust_sm_t *p_this)
{
    param_adjust_action_normal_start(p_this -> p_action);
} 

static void state_normal_exit (param_adjust_sm_t *p_this)
{
    param_adjust_action_normal_stop(p_this -> p_action);
}

static void state_min_entry(param_adjust_sm_t *p_this)
{
    param_adjust_action_min_start(p_this -> p_action);
}

static void state_min_exit (param_adjust_sm_t *p_this)
{
    param_adjust_action_min_stop(p_this -> p_action);
}

static void state_max_entry(param_adjust_sm_t *p_this)
{
    param_adjust_action_max_start(p_this -> p_action);
}

static void state_max_exit(param_adjust_sm_t *p_this)
{
    param_adjust_action_max_stop(p_this -> p_action);
}

static void state_event_inc(param_adjust_sm_t *p_this)
{
	param_adjust_action_inc(p_this -> p_action);
}

static void state_event_lr(param_adjust_sm_t *p_this)
{
	param_adjust_action_lr(p_this -> p_action);
}

static void state_event_dec(param_adjust_sm_t *p_this)
{
    param_adjust_action_dec(p_this -> p_action);
}

static void state_event_ignore (param_adjust_sm_t *p_this)
{
}

static void state_init_transition(param_adjust_sm_t *p_this, const adjust_state_t *p_new_state)
{
    p_this -> p_state = p_new_state;
    p_this -> p_state -> pfn_entry(p_this);
}

int param_adjust_sm_event_key_set (param_adjust_sm_t *p_this)
{
    if (p_this == NULL) {
         return -AM_EINVAL;
    }
    p_this -> p_state-> pfn_event_set(p_this);
    return AM_OK;
}

int param_adjust_sm_event_key_lr (param_adjust_sm_t *p_this)
{
    if (p_this == NULL) {
         return -AM_EINVAL;
    }
    p_this -> p_state -> pfn_event_lr(p_this);
    return AM_OK;
}

int param_adjust_sm_event_key_inc (param_adjust_sm_t *p_this)
{
    if (p_this == NULL) {
         return -AM_EINVAL;
    }
    p_this -> p_state -> pfn_event_inc(p_this);
    return AM_OK;
}

int param_adjust_sm_event_key_dec (param_adjust_sm_t *p_this)
{
    if (p_this == NULL) {
         return -AM_EINVAL;
    }
    p_this -> p_state -> pfn_event_dec(p_this);
    return AM_OK;
}


int param_adjust_sm_init (param_adjust_sm_t *p_this,param_adjust_action_t *p_action)
{
    if ((p_this == NULL) || (p_action == NULL)) {
         return - AM_EINVAL;
    }
    p_this -> p_action = p_action;
    state_init_transition(p_this, &state_normal);		// 初始转换，切换至“正常状态”
    return AM_OK;
}


