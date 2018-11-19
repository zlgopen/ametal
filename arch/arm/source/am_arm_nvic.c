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
 * \brief NVIC实现
 *
 * \internal
 * \par Modification history
 * - 1.01 17-04-10  sdy, modified.
 * - 1.00 14-12-04  hbt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_arm_nvic.h"
#include "hw/amhw_arm_nvic.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

/** \brief 中断未连接标识 */
#define __INT_NOT_CONNECTED      0xFF

/*******************************************************************************
  全局变量
*******************************************************************************/
static void  __enable_isr()
{
    __asm volatile ("cpsie i" : : : "memory");
}
/** \brief 指向中断设备的指针 */
static am_arm_nvic_dev_t *__gp_nvic_dev = NULL;

/******************************************************************************
  公共函数
*******************************************************************************/

/* 中断初始化 */
int am_arm_nvic_init (am_arm_nvic_dev_t           *p_dev,
                      const am_arm_nvic_devinfo_t *p_devinfo)
{
    int i;

    if (NULL == p_dev || NULL == p_devinfo) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo = p_devinfo;
    __gp_nvic_dev    = p_dev;
    p_dev->valid_flg = AM_TRUE;

    if ((NULL == p_devinfo->p_isrmap) || (NULL == p_devinfo->p_isrinfo)) {
        p_dev->valid_flg = AM_FALSE;
    }

    if (p_dev->valid_flg) {
        for (i = 0; i < p_devinfo->input_cnt; i++) {
            p_devinfo->p_isrmap[i] = __INT_NOT_CONNECTED;
        }

        for (i = 0; i < p_devinfo->isrinfo_cnt; i++) {
            p_devinfo->p_isrinfo[i].pfn_isr = NULL;
        }
    }

    amhw_arm_nvic_priority_group_set (p_devinfo->group);

    __enable_isr();

    return AM_OK;
}

/* 中断去初始化 */
void am_arm_nvic_deinit (void)
{
    int i;
    const am_arm_nvic_devinfo_t *p_nvic_devinfo = NULL;

    if (NULL == __gp_nvic_dev) {
        return;
    }

    p_nvic_devinfo = __gp_nvic_dev->p_devinfo;

    if (__gp_nvic_dev->valid_flg) {
        for (i = 0; i < p_nvic_devinfo->input_cnt; i++) {
            p_nvic_devinfo->p_isrmap[i] = __INT_NOT_CONNECTED;
        }

        for (i = 0; i < p_nvic_devinfo->isrinfo_cnt; i++) {
            p_nvic_devinfo->p_isrinfo[i].pfn_isr = NULL;
        }
    }

    for (i = 0; i < p_nvic_devinfo->input_cnt; i++) {
        amhw_arm_nvic_disable(p_nvic_devinfo->int_servinfo.inum_start + i);
    }
}

/* 中断处理函数 */
void am_exc_eint_handler (void)
{
    const am_arm_nvic_devinfo_t *p_nvic_devinfo = NULL;
    int           inum = 0;
    int           slot;
    am_pfnvoid_t  pfn_isr;
    void         *p_arg;


    if (NULL == __gp_nvic_dev) {
        return;
    }

    p_nvic_devinfo = __gp_nvic_dev->p_devinfo;

    inum = amhw_inum_get();

    if (!((inum >= p_nvic_devinfo->int_servinfo.inum_start) &&
          (inum <= p_nvic_devinfo->int_servinfo.inum_end))) {
        return;
    }

    slot = p_nvic_devinfo->p_isrmap[inum];
    if (slot == __INT_NOT_CONNECTED) {
        return;
    }

    pfn_isr = p_nvic_devinfo->p_isrinfo[slot].pfn_isr;
    p_arg   = p_nvic_devinfo->p_isrinfo[slot].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg);
    }
}

/* 连接中断回调函数 */
int am_int_connect (int inum, am_pfnvoid_t pfn_isr, void *p_arg)
{
    const am_arm_nvic_devinfo_t *p_nvic_devinfo = NULL;
    int slot = 0;
    int i;

    if (NULL == __gp_nvic_dev) {
        return -AM_EINVAL;
    }

    p_nvic_devinfo = __gp_nvic_dev->p_devinfo;

    if (!((inum >= p_nvic_devinfo->int_servinfo.inum_start) &&
          (inum <= p_nvic_devinfo->int_servinfo.inum_end))) {
        return -AM_EINVAL;
    }

    if (NULL == pfn_isr) {
        return -AM_EINVAL;
    }

    slot = p_nvic_devinfo->p_isrmap[inum];
    if (slot == __INT_NOT_CONNECTED) {
        for (i = 0; i < p_nvic_devinfo->input_cnt; i++) {
            if (p_nvic_devinfo->p_isrinfo[i].pfn_isr == NULL) {
                slot = i;
                break;
            }
        }
    } else {
        if ((p_nvic_devinfo->p_isrinfo[slot].p_arg == p_arg) &&
            (p_nvic_devinfo->p_isrinfo[slot].pfn_isr == pfn_isr)) {
            return AM_OK;
        }

        slot = __INT_NOT_CONNECTED;
    }

    if (slot != __INT_NOT_CONNECTED) {
        p_nvic_devinfo->p_isrmap[inum]          = slot;
        p_nvic_devinfo->p_isrinfo[slot].p_arg   = p_arg;
        p_nvic_devinfo->p_isrinfo[slot].pfn_isr = pfn_isr;
    }

    if (slot == __INT_NOT_CONNECTED) {
        return -AM_EPERM;                           /* 没有空的内存映射 */
    }

    return AM_OK;
}

/* 删除中断回调函数连接 */
int am_int_disconnect (int inum, am_pfnvoid_t pfn_isr, void *p_arg)
{
    const am_arm_nvic_devinfo_t *p_nvic_devinfo = NULL;
    int slot = 0;

    if (NULL == __gp_nvic_dev) {
        return -AM_EINVAL;
    }

    p_nvic_devinfo = __gp_nvic_dev->p_devinfo;

    if (!((inum >= p_nvic_devinfo->int_servinfo.inum_start) &&
          (inum <= p_nvic_devinfo->int_servinfo.inum_end))) {
        return -AM_EINVAL;
    }

    if (NULL == pfn_isr) {
        return -AM_EINVAL;
    }

    slot =  p_nvic_devinfo->p_isrmap[inum];
    if (slot == __INT_NOT_CONNECTED) {
        return -AM_EPERM;
    }

    p_nvic_devinfo->p_isrmap[inum]          = __INT_NOT_CONNECTED;
    p_nvic_devinfo->p_isrinfo[slot].pfn_isr = NULL;

    return AM_OK;
}

/* 使能中断 */
int am_int_enable (int inum)
{
    if (NULL == __gp_nvic_dev) {
        return -AM_EINVAL;
    }

    amhw_arm_nvic_enable(inum);

    return AM_OK;
}

/* 禁能中断 */
int am_int_disable (int inum)
{
    if (NULL == __gp_nvic_dev) {
        return -AM_EINVAL;
    }

    amhw_arm_nvic_disable(inum);

    return AM_OK;
}

/* 设置中断优先级 */
int am_arm_nvic_priority_set(int inum,
                             uint32_t preempt_priority,
                             uint32_t sub_priority)
{
    uint32_t prio = 0;
    if (NULL == __gp_nvic_dev) {
        return -AM_EINVAL;
    }

    prio = amhw_arm_nvic_encode_priority (__gp_nvic_dev->p_devinfo->group,
                                          preempt_priority,
                                          sub_priority,
                                          __gp_nvic_dev->p_devinfo->prio_bits);

    switch (__gp_nvic_dev->p_devinfo->core) {
    case AM_ARM_NVIC_CORE_M0:
    case AM_ARM_NVIC_CORE_M0PLUS:
        amhw_arm_nvic_m0_priority_set (inum, prio, __gp_nvic_dev->p_devinfo->prio_bits);
        break;
    case AM_ARM_NVIC_CORE_M3:
    case AM_ARM_NVIC_CORE_M4:
        amhw_arm_nvic_m3m4_priority_set (inum, prio, __gp_nvic_dev->p_devinfo->prio_bits);
        break;
    default:
        break;
    }


    return AM_OK;
}

/* end of file */
