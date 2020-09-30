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
 * \brief softimer implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 17-03-28  sky, first implementation.
 * \endinternal
 */

#include "am_zsl42x_lpsoftimer.h"

/**
 * 定时器链头
 */
static am_lpsoftimer_t *__gp_timer_list_head = NULL;

/**
 * \brief 插入新的链头定时器
 * \param[in] p_obj : 指向目标定时器
 * \return 无
 */
static void __insert_new_head_timer (am_lpsoftimer_t      *p_obj,
                                     am_lpsoftimer_time_t  current_time)
{
    uint32_t cpu_key;

    am_lpsoftimer_t *p_cur = __gp_timer_list_head;

    /* 将当前头运行状态切换为静止 */
    if (p_cur != NULL) {
        p_cur->is_running = 0;
    }

    /* 目标定时器替换为新的链头 */
    p_obj->p_next        = p_cur;
    p_obj->is_running    = 1;
    __gp_timer_list_head = p_obj;

    cpu_key = am_lpsoftimer_cpu_lock();
    am_lpsoftimer_timeout_set(__gp_timer_list_head->dest_time - current_time);
    am_lpsoftimer_cpu_unlock(cpu_key);
}

/**
 * \brief 插入新的定时器
 * \param[in] p_obj : 指向目标定时器
 * \return 无
 */
static void __insert_new_timer (am_lpsoftimer_t *p_obj)
{
    am_lpsoftimer_t *p_prev = __gp_timer_list_head;
    am_lpsoftimer_t *p_cur  = __gp_timer_list_head->p_next;

    /* 链头后空则直接插入 */
    if (p_cur == NULL) {

        p_prev->p_next = p_obj;
        p_obj->p_next  = NULL;

    } else {

        while (p_prev != NULL) {

            /* 目标定时器先计到则插入到前面（当前终点减去目标终点小于32位满偏的一半） */
            if ((p_cur->dest_time - p_obj->dest_time) < 0x80000000) {

                p_prev->p_next = p_obj;
                p_obj->p_next  = p_cur;
                break;

            } else {

                p_prev = p_cur;
                p_cur  = p_cur->p_next;

                /* 插入到链尾 */
                if (p_cur == NULL) {
                    p_prev->p_next = p_obj;
                    p_obj->p_next  = NULL;
                    break;
                }
            }
        }
    }
}

/**
 * \brief 检查定时器是否已经存在
 * \param[in] p_obj : 指向目标定时器
 * \return true of false
 */
static uint8_t __if_timer_exists (am_lpsoftimer_t *p_obj)
{
    am_lpsoftimer_t *p_cur = __gp_timer_list_head;

    while (p_cur != NULL) {
        if (p_cur == p_obj) {
            return 1;
        }
        p_cur = p_cur->p_next;
    }

    return 0;
}

/******************************************************************************/

void am_lpsoftimer_init (am_lpsoftimer_t *p_obj,
                         void           (*pfn_callback)(void *p_arg),
                         void            *p_arg)
{
    if (__if_timer_exists(p_obj)) {
        am_lpsoftimer_stop(p_obj);
    }

    p_obj->dest_time   = 0;
    p_obj->reload_time = 0;
    p_obj->is_running  = 0;
    p_obj->pfn_cb      = pfn_callback;
    p_obj->p_next      = NULL;
    p_obj->p_arg       = p_arg;
}

void am_lpsoftimer_start (am_lpsoftimer_t *p_obj)
{
    uint32_t             cpu_key;
    am_lpsoftimer_time_t current_time;

    cpu_key = am_lpsoftimer_cpu_lock();

    /* 若定时器已经存在于链上面，则不启动（因为已经启动） */
    if ((p_obj == NULL) || (__if_timer_exists(p_obj) == 1)) {
        am_lpsoftimer_cpu_unlock(cpu_key);
        return;
    }

    /* 加载重载ticks */
    current_time      = am_lpsoftimer_current_get();
    p_obj->dest_time  = current_time + p_obj->reload_time;
    p_obj->is_running = 0;

    /* 若空头或在头前则替换链头 */
    if (__gp_timer_list_head == NULL) {
        __insert_new_head_timer(p_obj, current_time);
    } else {
        /* 目标定时器先计到则插入到前面（当前终点减去目标终点小于32位满偏的一半） */
        if ((__gp_timer_list_head->dest_time - p_obj->dest_time) < 0x80000000) {
            __insert_new_head_timer(p_obj, current_time);
        } else {
            __insert_new_timer(p_obj);
        }
    }

    am_lpsoftimer_cpu_unlock(cpu_key);
}

void am_lpsoftimer_stop (am_lpsoftimer_t *p_obj)
{
    uint32_t cpu_key;

    am_lpsoftimer_t *p_prev = __gp_timer_list_head;
    am_lpsoftimer_t *p_cur  = __gp_timer_list_head;

    cpu_key = am_lpsoftimer_cpu_lock();

    // List is empty or the Obj to stop does not exist
    if ((__gp_timer_list_head == NULL) || (p_obj == NULL)) {
        am_lpsoftimer_cpu_unlock(cpu_key);
        return;
    }

    // Stop the Head
    if (__gp_timer_list_head == p_obj) {

        // The head is already running
        if (__gp_timer_list_head->is_running == 1) {

            if (__gp_timer_list_head->p_next != NULL) {
                __gp_timer_list_head->is_running  = 0;
                __gp_timer_list_head              = __gp_timer_list_head->p_next;
                __gp_timer_list_head->is_running  = 1;
                am_lpsoftimer_timeout_set(__gp_timer_list_head->dest_time - am_lpsoftimer_current_get());
            } else {
                __gp_timer_list_head = NULL;
            }

        // Stop the head before it is started
        } else {

            if (__gp_timer_list_head->p_next != NULL) {
                __gp_timer_list_head = __gp_timer_list_head->p_next;
            } else {
                __gp_timer_list_head = NULL;
            }
        }

    // Stop an object within the list
    } else {
        while (p_cur != NULL) {
            if (p_cur == p_obj) {
                if (p_cur->p_next != NULL) {
                    p_cur          = p_cur->p_next;
                    p_prev->p_next = p_cur;
                } else {
                    p_cur          = NULL;
                    p_prev->p_next = p_cur;
                }
                break;
            } else {
                p_prev = p_cur;
                p_cur  = p_cur->p_next;
            }
        }
    }

    am_lpsoftimer_cpu_unlock(cpu_key);
}

void am_lpsoftimer_restart (am_lpsoftimer_t *p_obj)
{
    am_lpsoftimer_stop(p_obj);
    am_lpsoftimer_start(p_obj);
}

uint8_t am_lpsoftimer_list_isempty (void)
{
    return (uint8_t)(__gp_timer_list_head == NULL);
}

void am_lpsoftimer_value_set (am_lpsoftimer_t *p_obj, uint32_t time)
{
    /* 停止定时器 */
    am_lpsoftimer_stop(p_obj);

    /* 定时值有效性校验 */
    if (0 == time) {
        time = 1;
    }

    /* 赋值及tick换算 */
    p_obj->reload_time = time;
}

am_lpsoftimer_time_t am_lpsoftimer_elapsed_get (am_lpsoftimer_time_t saved_time)
{
    uint32_t cpu_key;

    am_lpsoftimer_time_t current_time;

    cpu_key = am_lpsoftimer_cpu_lock();
    current_time = am_lpsoftimer_current_get();
    am_lpsoftimer_cpu_unlock(cpu_key);

    return current_time - saved_time;
}

void am_lpsoftimer_isr (void)
{
    uint32_t cpu_key;

    am_lpsoftimer_time_t current_time;

    /* Early out when __gp_timer_list_head is null to prevent null pointer */
    if (__gp_timer_list_head == NULL) {
        return;
    }

    /* 切换头定时器运行状态 */
    __gp_timer_list_head->is_running = 0;

    cpu_key = am_lpsoftimer_cpu_lock();
    current_time = am_lpsoftimer_current_get();
    am_lpsoftimer_cpu_unlock(cpu_key);

    /* 扫描符合回调条件的定时器（当前时间减去目标终点小于32位满偏的一半即计到） */
    while ((__gp_timer_list_head != NULL) &&
           ((current_time - __gp_timer_list_head->dest_time) < 0x80000000)) {

        am_lpsoftimer_t *p_elapsed_timer;

        p_elapsed_timer      = __gp_timer_list_head;
        __gp_timer_list_head = __gp_timer_list_head->p_next;

        if (p_elapsed_timer->pfn_cb != NULL) {
            p_elapsed_timer->pfn_cb(p_elapsed_timer->p_arg);
        }

        cpu_key = am_lpsoftimer_cpu_lock();
        current_time = am_lpsoftimer_current_get();
        am_lpsoftimer_cpu_unlock(cpu_key);
    }

    /* 如果更新后的链头定时器存在则启动它 */
    if (__gp_timer_list_head != NULL) {
        if (__gp_timer_list_head->is_running != 1) {
            __gp_timer_list_head->is_running = 1;
            cpu_key = am_lpsoftimer_cpu_lock();
            am_lpsoftimer_timeout_set(__gp_timer_list_head->dest_time -
                                      current_time);
            am_lpsoftimer_cpu_unlock(cpu_key);
        }
    }
}

/* end of file */
