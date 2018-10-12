#pragma once
#include "ametal.h"
#include "mvc.h"

// 定义模块状态
#define MODEL_TEMP_MONITOR_STATUS_NORMAL	0   // 温度正常
#define MODEL_TEMP_MONITOR_STATUS_UPPER		1   // 超过上限温度值
#define MODEL_TEMP_MONITOR_STATUS_LOWER	  2   // 低于下限温度值

// 定义温度检测模型类型
typedef struct _model_temp_monitor {
	model_t   isa;          					  // model基类派生
	int       status;         			    // 当前温度状态
	float		  temp_cur;      				    // 当前温度值
	float     temp_max;      				    // 上限温度值
	float		  temp_min;      				    // 下限温度值
} model_temp_monitor_t; 

int model_temp_monitor_init (model_temp_monitor_t *p_model, float max, float min, float cur);

int model_temp_monitor_cur_set (model_temp_monitor_t *p_model, float value);
int model_temp_monitor_max_set (model_temp_monitor_t *p_model, float value);
int model_temp_monitor_min_set (model_temp_monitor_t *p_model, float value);
int model_temp_monitor_cur_get (model_temp_monitor_t *p_model, float *p_value);
int model_temp_monitor_max_get (model_temp_monitor_t *p_model, float *p_value);
int model_temp_monitor_min_get (model_temp_monitor_t *p_model, float *p_value);
int model_temp_monitor_status_get(model_temp_monitor_t *p_model);
