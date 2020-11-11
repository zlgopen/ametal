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
 * \brief Software Timer 软件定时器
 *
 * \details
 *        将一个硬件定时器拆分成多个软件定时器，区别于softimer它无需每tick执行一次，
 *        而是利用硬件定时器的匹配（match）功能实现间断地处理，所以它更适用于低功耗场合。
 *        适配的硬件定时器若支持在低功耗模式中工作并能产生可唤醒的中断，
 *        则该接口实现的软件定时器具备低功耗唤醒功能。
 *
 * \internal
 * \par Modification history
 * - 1.0.0 17-06-13  sky, ebi, 接口定义
 * \endinternal
 */

#ifndef __SOFTIMER_H
#define __SOFTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup if_softimer
 * @copydoc softimer.h
 * @{
 */

#include <stdio.h>
#include <stdint.h>

/**
 * \brief softimer 时间域定义
 */
typedef uint32_t am_lpsoftimer_time_t;

/**
 * \brief softimer定义
 */
typedef struct am_lpsoftimer {

    am_lpsoftimer_time_t  dest_time;                 /**< \brief 目标时间(ms) */
    am_lpsoftimer_time_t  reload_time;               /**< \brief 重载的时间(ms) */
    uint8_t               is_running;                /**< \brief 是否正在运行中 */
    void                 (*pfn_cb) (void *p_arg);    /**< \brief 定时器回调函数 */
    void                  *p_arg;                    /**< \brief 回调函数传入参数 */

    struct am_lpsoftimer *p_next;                    /**< \brief 指向下一个定时器 */

} am_lpsoftimer_t;

/**
 * \brief softimer 定时器实例初始化
 *
 * \param[in] p_obj        : 需要初始化的softimer实例
 * \param[in] pfn_callback : 指向该定时器回调函数
 * \param[in] p_cb_arg     : 回调函数的p_arg参数
 *
 * \return 无
 */
void am_lpsoftimer_init (am_lpsoftimer_t *p_obj,
                         void           (*pfn_callback)(void *p_arg),
                         void            *p_arg);

/**
 * \brief softimer 设定延时值
 *
 * \param[in] p_obj : 指向softimer实例
 * \param[in] time  : 指向该定时器的延时值（ms）
 *
 * \note 入参为0时无效，内部会修正为1.
 * \return 无
 */
void am_lpsoftimer_value_set (am_lpsoftimer_t *p_obj, am_lpsoftimer_time_t time);

/**
 * \brief softimer 定时器启动
 * \param[in] p_obj : softimer实例
 * \return 无
 */
void am_lpsoftimer_start (am_lpsoftimer_t *p_obj);

/**
 * \brief softimer 定时器停止
 * \param[in] p_obj : softimer实例
 * \return 无
 */
void am_lpsoftimer_stop (am_lpsoftimer_t *p_obj);

/**
 * \brief softimer 定时器重新启动
 * \param[in] p_obj : softimer实例
 * \return 无
 */
void am_lpsoftimer_restart (am_lpsoftimer_t *p_obj);

/**
 * \brief  获取 lpsoftimer 链表是否为空，即当前是否有定时器工作
 * \return 1 : 当前没有软件定时器工作
 *         0 : 当前至少有一个定时器在工作
 */
uint8_t am_lpsoftimer_list_isempty (void);

/**
 * \brief softimer 获取逝去时间
 * \param[in] saved_time : 参考时间点(不要传入未来的时间，否则会导致计算值溢出)
 * \return 逝去时间
 */
am_lpsoftimer_time_t am_lpsoftimer_elapsed_get (am_lpsoftimer_time_t saved_time);

/**
 * \brief softimer 获取当前毫秒计时值(自系统启动以来)
 * \param[in] 无
 * \return 计时值
 */
am_lpsoftimer_time_t am_lpsoftimer_current_get (void);

/**
 * \brief softimer 设置下一次超时
 * \param[in] timeout : 从现在开始多久之后进入定时器中断
 * \return 无
 */
void am_lpsoftimer_timeout_set (am_lpsoftimer_time_t timeout);

/**
 * \brief softimer 温度补偿
 *
 * \param[in] period      : 时间周期
 * \param[in] temperature : 温度
 *
 * \return 补偿后的时间周期
 */
uint32_t am_lpsoftimer_temp_compensation (uint32_t period, float temperature);

/**
 * \brief softimer Ms时间转化为系统tick时间
 * \param[in] timeout : 从现在开始多久之后进入定时器中断
 * \return 无
 */
uint32_t am_lpsoftimer_ms_to_tick (am_lpsoftimer_time_t ms, float temp);

/**
 * \brief softimer 系统tick转化为Ms时间
 * \param[in] timeout : 从现在开始多久之后进入定时器中断
 * \return 无
 */
am_lpsoftimer_time_t am_lpsoftimer_tick_to_ms (uint32_t tick, float temp);

/**
 * \brief softimer 中断服务函数
 * \param[in] 无
 * \return 无
 * \note 在平台的定时器中断服务函数中回调该函数
 */
void am_lpsoftimer_isr (void);

/**
 * \brief 关闭CPU锁
 * \attention 关闭时间一定要短且必须与 am_lpsoftimer_cpu_unlock() 函数搭配使用
 * \param[in] 无
 * \return    CPU锁信息
 */
uint32_t am_lpsoftimer_cpu_lock (void);

/**
 * \brief 打开CPU锁
 * \attention 必须与 am_lpsoftimer_cpu_lock() 函数搭配使用
 * \param[in] lock : 锁信息
 * \return    无
 */
void am_lpsoftimer_cpu_unlock (uint32_t lock);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __SOFTIMER_H */

/* end of file */
