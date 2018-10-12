/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s mvc driver for AM824
 *
 * \internal
 * \par Modification History
 * - 1.00 18-09-13  ipk, first implementation.
 * \endinternal
 */
#ifndef __MVC_H
#define __MVC_H


#include "slist.h"

struct _model;
struct _observer;


typedef void (*update_view_t)(struct _observer *p_this, struct _model *p_model);

typedef struct _observer{
    slist_node_t    node;
    update_view_t    pfn_update_view;
} observer_t;

typedef struct _model{
    slist_head_t  head;
} model_t;

int view_init(observer_t *p_this,update_view_t pfn_update_view);

int model_init(model_t *p_this);
    
int model_attach(model_t *p_this, observer_t *p_observer);

int model_detach(model_t *p_this, observer_t *p_observer);

int model_notify(model_t *p_this);


#endif /* __MVC_H */
/* end of file */
