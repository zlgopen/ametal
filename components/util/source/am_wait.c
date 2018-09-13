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
 * \brief WAIT implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-09-01  tee, first implementation.
 * \endinternal
 */
#include "am_wait.h"


/******************************************************************************/

/* 使用值附带信息 */
#define __WAIT_VAL_INIT     0   /* 初始值为0                    */
#define __WAIT_VAL_DONE     1   /* 使用am_wait_done()设置值为 1 */
#define __WAIT_VAL_TIMEOUT  2   /* 超时时，设置值为2             */

/* 状态用以表示当前等待结构的状态 */
#define __WAIT_STAT_INIT             0   /* 初始状态            */
#define __WAIT_STAT_WAIT_ON          1   /* 等待状态            */
#define __WAIT_STAT_WAIT_ON_TIMEOUT  2   /* 等待状态（带超时）   */

/******************************************************************************/

static void __timer_callback (void *p_arg)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;
    
    p_wait->val = __WAIT_VAL_TIMEOUT;
    
    am_softimer_stop(&p_wait->timer);
}

/******************************************************************************/

/* 等待信号初始化 */
int am_wait_init (am_wait_t *p_wait)
{
    if (p_wait == NULL) {
        return -AM_EINVAL;
    }
    
    p_wait->val   = __WAIT_VAL_INIT;
    p_wait->stat  = __WAIT_STAT_INIT;
    
    am_softimer_init(&p_wait->timer,__timer_callback, p_wait);
    
    return AM_OK;
}

int am_wait_on (am_wait_t *p_wait)
{
    if (p_wait == NULL) {
        return -AM_EINVAL;
    }
    
    p_wait->stat = __WAIT_STAT_WAIT_ON;
    
    while(p_wait->val == __WAIT_VAL_INIT);
    
    p_wait->val  = __WAIT_VAL_INIT;
    p_wait->stat = __WAIT_STAT_INIT;
    
    return AM_OK;
}

int am_wait_done(am_wait_t *p_wait)
{
    if (p_wait == NULL) {
        return -AM_EINVAL;
    }
    
    p_wait->val  = __WAIT_VAL_DONE;

    return AM_OK;
}

int am_wait_on_timeout(am_wait_t *p_wait, uint32_t timeout_ms)
{
    int ret;
    
    /* 开始计时 */
    am_softimer_start(&p_wait->timer,timeout_ms);
    
    p_wait->stat = __WAIT_STAT_WAIT_ON_TIMEOUT;
    
    while(p_wait->val == __WAIT_VAL_INIT);
    
    if (p_wait->val == __WAIT_VAL_DONE) {  /* 成功等到结束信号 */
        ret = AM_OK;
    } else {                               /* 超时信号     */
        ret = -AM_ETIME;
    }
    
    p_wait->val  = __WAIT_VAL_INIT;
    p_wait->stat = __WAIT_STAT_INIT;       /* 回到初始态   */
    
    am_softimer_stop(&p_wait->timer);
    
    return ret;
}

/* end of file */
