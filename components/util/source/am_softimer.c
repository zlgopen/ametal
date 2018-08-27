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
 * \brief 软件定时器接口函数实现
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-08-03  tee, first implementation.
 * \endinternal
 */

#include "am_softimer.h"
#include "am_common.h"
#include "am_int.h"

/** \brief 用于软件定时器的硬件定时器工作频率，正常运行时，频率不会为0 */
static unsigned int __g_hwtimer_freq = 0;

/** \brief 软件定时器链表表头 */
static struct am_list_head g_softimer_head;

/******************************************************************************/
static unsigned int __ms_to_ticks (unsigned int ms)
{
    if (__g_hwtimer_freq != 0) {
        return  AM_DIV_ROUND_UP(__g_hwtimer_freq * ms, 1000u);
    }
    return 0;
}

/******************************************************************************/
static void __softimer_add (am_softimer_t *p_timer, unsigned int ticks)
{
    struct am_list_head  *p;
    am_softimer_t        *p_timer_iterator = NULL;
    
    am_list_for_each(p, &g_softimer_head) {
        p_timer_iterator = am_list_entry(p, am_softimer_t, node);
        if (ticks >= p_timer_iterator->ticks ) {  /* 相同时，应插入在节点后面 */
            ticks -= p_timer_iterator->ticks;
        } else {
            break;
        }
    }
    p_timer->ticks = ticks;

    am_list_add_tail( &p_timer->node, p);         /* 在正确的位置插入定时器   */
    
    if (p != &g_softimer_head ) {                 /* p所指定时器延时要更新    */
        p_timer_iterator->ticks -= ticks;
    }
}

/******************************************************************************/
static void __softimer_remove (am_softimer_t *p_timer)
{
    am_softimer_t *p_next = NULL;

    if (am_list_empty(&p_timer->node) ) {         /* 节点未加入链表中         */
        return ;
    }
    
    /* 当前节点不是最后一个节点, 删除该节点前，将下一个节点的延时tick更新 */
    if ( (&p_timer->node)->next != &g_softimer_head ) {
        p_next = am_list_entry((&p_timer->node)->next, am_softimer_t, node);
        p_next->ticks += p_timer->ticks;
    }
    
    am_list_del_init(&p_timer->node);
}

/* 必须以初始化指定的频率调用该函数  */
void am_softimer_module_tick (void)
{
    int old = am_int_cpu_lock();

    if ( !am_list_empty( &g_softimer_head ) ) {

        struct am_list_head *p;
        am_softimer_t       *p_timer;

        /* 指向第一个有效元素节点 */
        p       = (&g_softimer_head)->next;
        p_timer = am_list_entry(p, am_softimer_t, node);

        /*
         * 链表首节点值减1
         */
        if (p_timer->ticks) {
            p_timer->ticks--;
        }

        while (!am_list_empty(&g_softimer_head)) {

            /*
             * 处理第一个有效元素,处理后该结点必然不处于第一个结点
             * (若处于第一个结点，其值也不会为 0，即下次扫描就会退出)
             */
            if (p_timer->ticks == 0) {

                /* 该节点本次定时时间到，删除该节点                  */
                am_list_del_init(&p_timer->node);

                /* 可能在回调函数中停止，因此先将其重新添加进链表中  */
                __softimer_add(p_timer, p_timer->repeat_ticks);

                if (p_timer->timeout_callback ) {
                    /* 执行定时器处理函数时，打开总中断 */
                    am_int_cpu_unlock(old);
                    p_timer->timeout_callback(p_timer->p_arg);
                    old = am_int_cpu_lock();
                }

                /* 处理完一个节点，继续判断下一个首节点时间是否为0 */
                p       = (&g_softimer_head)->next;
                p_timer = am_list_entry(p, am_softimer_t, node);


            /* 只要遇到不为0的结点，就退出循环 */
            } else {
                break;
            }
        }
    }
    am_int_cpu_unlock(old);
}

int am_softimer_module_init (unsigned int clkrate)
{
    if ((clkrate == 0)) {

        return -AM_EINVAL;
    }

    AM_INIT_LIST_HEAD(&g_softimer_head);
    __g_hwtimer_freq = clkrate;
    return 0;
}
 
/******************************************************************************/
int am_softimer_init (am_softimer_t *p_timer, 
                      am_pfnvoid_t   p_func,
                      void          *p_arg)
{
    int old;
    
    if (__g_hwtimer_freq == 0) {                      /* 硬件定时器未正常工作 */
        return -AM_EPERM;
    }
    
    AM_INIT_LIST_HEAD(&p_timer->node);

    old = am_int_cpu_lock();
    
    p_timer->timeout_callback = p_func;
    p_timer->p_arg            = p_arg;
    
    am_int_cpu_unlock(old);
    
    return AM_OK;
}

/******************************************************************************/
void am_softimer_start (am_softimer_t *p_timer, unsigned int ms)
{
    int old;
    unsigned int ticks =  __ms_to_ticks(ms);
    old = am_int_cpu_lock();
    
    /* ticks最小值为1 */
    ticks = ticks > 0 ? ticks : 1;
    
    p_timer->repeat_ticks = ticks;
    
    __softimer_remove(p_timer);
    __softimer_add(p_timer, ticks);
    am_int_cpu_unlock(old);
}

/******************************************************************************/
void am_softimer_stop (am_softimer_t *p_timer)
{
    int old;
    old = am_int_cpu_lock();
    __softimer_remove(p_timer);
    am_int_cpu_unlock(old);
}

/* end of file */
