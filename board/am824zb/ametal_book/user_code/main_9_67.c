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
 * \brief 程序清单 9.67
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "model_bool.h"

//需要将value值设置为AM_TRUE
#define   SET_VALUE      AM_TRUE 

void __view_update(observer_t *p_view, model_t *p_model)
{
	am_bool_t		value;
	model_bool_get((model_bool_t *)p_model, &value);
	if (value) {
		// value为AM_TRUE，进行相应的显示
	} else {
		// value为AM_FALSE，进行相应的显示
	}
}

int am_main()
{
	observer_t  	  view;
	model_bool_t  	model_bool;
	
  model_bool_init(&model_bool, AM_FALSE);
	view_init(&view, __view_update);
	model_attach(&(model_bool.isa), &view);
	
  while(1){
     if (SET_VALUE){
	       model_bool_set(&model_bool, AM_TRUE);
     }else {
	       model_bool_set(&model_bool, AM_FALSE);
     }
  }
}
/* end of file */
