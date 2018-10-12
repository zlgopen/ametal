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
 * \brief book`s key_ process.h file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#ifndef __KEY_PROCESS_H__
#define __KEY_PROCESS_H__
#include "ametal.h"
#include "model_temp_monitor.h"
#include "view_digitron.h"
#include "am_softimer.h"


#define __ADJ_STATE_NORMAL    	0
#define __ADJ_STATE_MIN       	1
#define __ADJ_STATE_MAX       	2

typedef struct _key_process {
    uint8_t       			     adj_state;
    uint32_t      			     adj_val;
	  uint8_t       			     adj_pos;
	  int           			     led0_id;
	  int           			     led1_id;
	  int           			     digitron_id;
	  model_temp_monitor_t  	*p_model;
	  view_digitron_t       	*p_view;
	  am_softimer_t          	 timer;
} key_process_t;
	
int  key_process_set (key_process_t *p_this);  		// SET键处理
int  key_process_lr (key_process_t *p_this);   		// L/R键处理
int  key_process_inc (key_process_t *p_this);  		// 加键处理
int  key_process_dec (key_process_t *p_this);  		// 减键处理

int key_process_init (key_process_t         	*p_this,
	                    model_temp_monitor_t  	*p_model,
	                    view_digitron_t       	*p_view,
	                    int                   	 led0_id,
	                    int                   	 led1_id,
	                    int                   	 digitron_id);

#endif /* __KEY_PROCESS_H__ */
/*end of file*/
