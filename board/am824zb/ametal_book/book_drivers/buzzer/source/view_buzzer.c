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
 * \brief book`s view_buzzer.h file
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_buzzer.h"
#include "view_buzzer.h"
#include "model_temp_monitor.h"

static void __view_buzzer_update (observer_t *p_view, model_t *p_model)
{
    int status = model_temp_monitor_status_get((model_temp_monitor_t *)p_model);
    if (status == MODEL_TEMP_MONITOR_STATUS_NORMAL) {
		      am_buzzer_off();
    } else {
        	am_buzzer_on();                            		
    }
}

int view_buzzer_init (view_buzzer_t *p_view_buzzer)
{
    if (p_view_buzzer == NULL) {
        return -1;
    }
    view_init(&(p_view_buzzer->isa), __view_buzzer_update);
    return 0;
}

/* end of file */
