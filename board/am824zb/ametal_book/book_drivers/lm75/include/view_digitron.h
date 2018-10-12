


#pragma once
#include "ametal.h"
#include "mvc.h"

typedef struct _view_digitron { 
    observer_t  	isa;        				// 基类派生
    int      		digitron_id;  				// 数码管显示器ID号
} view_digitron_temp_t;

typedef struct _view_digitron {
	observer_t   		isa;
	int           		digitron_id;  	// 数码管显示器ID号
} view_digitron_t;


//  数码管视图初始化
int view_digitron_init (view_digitron_t *p_view_digitron, int digitron_id);
