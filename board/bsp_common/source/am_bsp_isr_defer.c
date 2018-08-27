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
 * \brief ISR DEFER 板级支持
 *
 * \internal
 * \par modification history
 * - 1.00 12-10-23  orz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_isr_defer.h"
#include "am_bsp_isr_defer.h"

/*******************************************************************************
  内部宏定义
*******************************************************************************/

/** \brief 中断控制和状态寄存器,0xE000ED04，用于触发PendSV中断 */
#define __ICSR_ADDRESS  0xE000ED04

/** \brief 系统异常优先级设置寄存器地址 0xE000ED20 */
#define __SHP2_ADDRESS  0xE000ED20

/*******************************************************************************
  内部函数
*******************************************************************************/
static void __isr_defer_trigger (void *p_arg)
{
    /* 触发PendSV中断 */
    *(volatile unsigned int *)__ICSR_ADDRESS |= 0x10000000;
}

/*******************************************************************************
  软中断处理函数
*******************************************************************************/
void PendSV_Handler (void)
{
    am_isr_defer_job_process();
}

/*******************************************************************************
  定义优先级数目
*******************************************************************************/
AM_ISR_DEFER_PRIORITY_NUM_DEF(AM_BSP_ISR_DEFER_PRIORITY_NUM);

/*******************************************************************************
  公共函数
*******************************************************************************/
void am_bsp_isr_defer_init (void)
{

    /* 将 PendSV 的优先级设置为最低 */
    *(volatile unsigned int *)__SHP2_ADDRESS  |= (0xC0ul << 16);

    am_isr_defer_init(__isr_defer_trigger, NULL);
}

/* end of file */
