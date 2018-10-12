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
 * \brief book`s slist driver for AM824
 *
 * \internal
 * \par Modification History
 * - 1.00 18-09-26  ipk, first implementation.
 * \endinternal
 */
 
#ifndef __SLIST_H
#define __SLIST_H

#include "ametal.h"
#include "am_list.h"

typedef struct _slist_node{
    struct _slist_node *p_next;
}slist_node_t;

typedef slist_node_t slist_head_t;


inline int slist_init(slist_head_t *p_head)
{
    p_head->p_next = NULL;
    return 0;
}

inline int slist_add_head(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tail = p_head;
    
    if (p_head == NULL || p_node == NULL) {
        return -1;
    }
    
    while (p_tail->p_next != NULL) {
        p_tail = p_tail->p_next;
    }
    p_node->p_next = NULL;
    p_tail->p_next = p_node;
    
    return 0;
}

inline int slist_del(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tail = p_head;
    
    if (p_head == NULL || p_node == NULL) {
        return -1;
    }
    while (p_tail->p_next != p_node) {
        if (p_tail->p_next == NULL) {
            return -1;
        }
        p_tail = p_tail->p_next;
    }
    p_tail->p_next = p_node->p_next;
    
    return 0;
}

#endif /* __SLIST_H */
/* end of file */
