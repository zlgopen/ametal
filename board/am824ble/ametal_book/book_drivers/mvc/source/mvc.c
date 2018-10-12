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
 * \brief 
 *
 * \internal
 * \par Modification History
 * - 1.00 18-09-26  ipk, first implementation.
 * \endinternal
 */
#include "mvc.h"
#include "ametal.h"

typedef int (*slist_node_process_t) (void *p_arg, slist_node_t *p_node);

static int __view_process(void *p_arg, slist_node_t *p_node)
{
	observer_t	*p_observer = (observer_t *)p_node;
  model_t	    *p_model    = (model_t *)p_arg;		// 此处的p_arg为指向模型自身的指针
	p_observer -> pfn_update_view(p_observer, p_model);
	return 0;
}

int view_init(observer_t *p_this,update_view_t pfn_update_view)
{
	if ((p_this == NULL) || (pfn_update_view == NULL)){
		return -1;
	}
	p_this->pfn_update_view  = pfn_update_view;
	return 0;
}

int model_init(model_t *p_this)
{
	if (p_this == NULL){
        return -1;
	}
	slist_init(&p_this->head);
	return 0;
}

int model_attach(model_t *p_this, observer_t *p_observer)
{
//	observer_t *p;

	if ((p_this == NULL) || (p_observer == NULL)) {
     		return -1;
   	}
    slist_add_head(&(p_this -> head), &( p_observer -> node));	
	  p_observer -> pfn_update_view(p_observer, p_this); 
		return 0;
}

int model_detach(model_t *p_this, observer_t *p_observer)
{
//	observer_t *p;

   	if ((p_this == NULL) || (p_observer == NULL)){
		return -1;
   	}
     slist_del(&(p_this->head), &( p_observer->node));		
	return 0;
}

int slist_foreach(slist_head_t *p_head, slist_node_process_t pfn_proc, void *p_arg)
{
	slist_node_t *p_node;
	if (p_head == NULL || pfn_proc == NULL) {
		return -1;
	}
	p_node = p_head;
	
	while(p_node->p_next != NULL) {
		pfn_proc(p_arg, p_node->p_next);
		p_node = p_node->p_next;
	}
	
	return 0;
}

int model_notify(model_t *p_this)
{
		if (p_this == NULL){
			return -1;
		}
		slist_foreach(&p_this -> head, __view_process, p_this);// 遍历链表，模型自身作为遍历函数的参数
		return 0;
}
/* end of file */

