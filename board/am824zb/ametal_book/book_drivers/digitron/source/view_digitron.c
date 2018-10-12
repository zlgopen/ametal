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
 * \brief book`s view_digitron.c file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
 
#include "string.h"
#include "ametal.h"
#include "view_digitron.h"
#include "am_digitron_dev.h"
#include "model_temp_monitor.h"
#include "am_vdebug.h"

static void __view_update (observer_t *p_view, model_t *p_model)
{
    view_digitron_t *p_this = (view_digitron_t *)p_view;
    float   cur;
    char   buf[3];


	  model_temp_monitor_cur_get((model_temp_monitor_t *)p_model, &cur);
    
	  am_snprintf(buf, 3, "%2d" , (int)cur);
	  am_digitron_disp_str(p_this -> digitron_id, 0, strlen(buf), buf);
}


int view_digitron_init (view_digitron_t *p_view_digitron, int digitron_id)
{
    if (p_view_digitron == NULL) {
        return -1;
    }
    	view_init(&(p_view_digitron->isa), __view_update);
	  p_view_digitron->digitron_id = digitron_id;
	  return 0;
}
/*end of file */

