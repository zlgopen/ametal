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
 * \brief book`s view_zigbee.h file for chapter9
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#ifndef __VIEW_ZIGBEE1_H__
#define __VIEW_ZIGBEE1_H__

#include "ametal.h"
#include "mvc.h"
#include "model_bool.h"
#include "am_zm516x.h"

//定义zigbee视图类
typedef struct _view_zigbee{
    
    observer_t               isa;
    
    am_zm516x_handle_t zm516x_handle;
    
} view_zigbee_t;


int view_zigbee_init1 (view_zigbee_t *p_view_zigbee,am_zm516x_handle_t  zm516x_handle);

#endif /* __VIEW_ZIGBEE1_H__ */
/* end of file  */
