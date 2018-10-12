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
 * \brief book`s model_bool.c file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "model_bool.h"

int model_bool_init (model_bool_t *p_this, am_bool_t init_value)
{
	if (p_this == NULL) {
		return -1;
	}
	model_init(&(p_this -> isa));
	p_this -> value = init_value;
	return 0;
}

int model_bool_set(model_bool_t *p_this, am_bool_t value)
{
    if (p_this == NULL){
        return -1;
    }
    if (p_this -> value != value){
        p_this -> value = value;
        model_notify((model_t *)p_this);
    }
    return 0;
}

int model_bool_get(model_bool_t *p_this, am_bool_t *p_value)
{
    if ((p_this == NULL) || (p_value == NULL)){
        return -1;
    }
    *p_value = p_this -> value;
    return 0;
}
/*end of file */
