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
 * \brief 按键输入事件
 *
 * \internal
 * \par Modification history
 * - 1.00 16-08-23  tee, first implementation.
 * \endinternal
 */
#ifndef __AM_EVENT_INPUT_KEY_H
#define __AM_EVENT_INPUT_KEY_H

#include "am_common.h"
#include "am_event.h"
#include "am_softimer.h"

#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * \brief 用于按键长按，用户无需深入了解各成员，仅用于内存分配
 */
typedef struct am_event_input_key_long_press {
    am_softimer_t timer;         /**< \brief 软件定时器，用于按键长按计时     */
    int           key_code;      /**< \brief 当前长按的按键键码，-1为无效值   */
    int           keep_time;     /**< \brief 长按计时                         */
} am_event_input_key_long_press_t;

/**
 * \brief 按键输入管理系统相关信息
 */
typedef struct am_event_input_key_info {

    /**
     * \brief 长按多长时间后开始上报按键长按事件
     *
     * 例如，需要长按1s后开始上报长按事件，则该值为 1000
     */
    int  key_long_press_time_start_ms;

    /**
     * \biref 达到启动时间后，每隔一定的时间上报一次长按事件
     *
     * 例如，若有如下配置：
     *  - key_long_press_time_start_ms  为 1000
     *  - key_long_press_time_period_ms 为 200
     *
     *  则表示当按键按住1s后上报一次长按事件，以后每隔200ms再上报一次长按事件
     */
    int  key_long_press_time_period_ms;

    /**
     * \brief 同时长按的按键最大个数
     *
     *  - 若无需支持按键长按，则该值为0
     *  - 若需支持按键长按，则该值为同一时刻长按按键的最大个数，通常为1。
     *
     * 一般情况下，同一时刻只会长按单个按键，一般不会同时长按多个按键。
     * 若需支持同时长按多个按键，则必须为按键长按计时分配必要的内存空间。
     *
     * \note 这里并非支持长按的按键个数，而是同时长按按键的最大个数。
     */
    int  key_long_press_max_num;

    /**
     * \brief 按键长按必备缓存，缓存大小为： key_long_press_max_num
     *
     * 该缓存仅需定义，用以内存分配，无需用户对成员赋值
     *
     * 若 key_long_press_max_num 为 0，将不支持长按，则该值为NULL。
     */
    am_event_input_key_long_press_t *p_buf;

} am_event_input_key_info_t;

/**
 * \brief 按键输入管理系统初始化
 *
 * \return 无
 *
 * \note 按键输入是 "输入事件管理的" 一个子类，在初始化按键输入管理模块前，应确
 * 保已经使用 am_event_category_input_init() 函数完成了输入事件管理器的初始化。
 */
void am_event_input_key_init (const am_event_input_key_info_t *p_info);


/**
 * \brief 上报按键按下事件（底层按键驱动调用，用于普通按键）
 *
 * 普通按键在按键按下或释放时，都应上报事件。AMetal上层将在接收到按键按下
 * 事件时，启动按键长按计时（若使能了该功能），在按键释放前，定期向应用
 * 层上报按键事件。
 *
 * 对于驱动来讲，该按键接下来将被释放，产生按键释放事件，即驱动接下来应在按键释
 * 放时调用 am_event_input_key_released() 函数。
 *
 * \param[in] key_code   : 按键编码
 *
 * \return 标准错误号
 */
int am_event_input_key_pressed  (int key_code);

/**
 * \brief 上报按键释放事件（底层按键驱动调用，用于普通按键）
 *
 * 普通按键在按键按下或释放时，都应上报事件。AMetal上层将在接收到按键按下
 * 事件时，启动按键长按计时（若使能了该功能），在按键释放前，定期向应用
 * 层上报按键事件。
 *
 * 该函数用于通知指定按键已释放，上层应停止该按键的长按计时。
 *
 * \param[in] key_code   : 按键编码
 *
 * \return 标准错误号
 */
int am_event_input_key_released (int key_code);

/**
 * \brief 上报按键按下事件（底层按键驱动调用，用于特殊按键，不启动长按计时）
 *
 * 对于一些特殊按键/键盘，主要是外接键盘模块时（如ZLG72128键盘），可能不期望
 * AMetal上层进行长按计时，主要有两种情况：
 *
 * 1.键盘模块可能只会上报按键按下事件，不会上报按键释放事件。（对于绝大部分应用，
 *   可能不关心按键的释放）。 无按键释放，无法对长按进行准确计时。
 *
 * 2. 当键盘自身支持长按时（如ZLG72128键盘、PS/2键盘），当按键长按时，其本身会定
 * 期上报按键事件，此时，AMetal上层无需再对长按做任何额外处理，应用层同样可以使
 * 用长按功能，只不过长按计时是由具体键盘模块完成的，无需AMetal上层参与。
 *
 * \param[in] key_code  : 按键编码
 * \param[in] keep_time : 按键长按时间（单位：ms），若按键模块本身支持长按功能，
 * 则该值为键盘模块本身的计时结果（0为首次按下），若按键模块本身不支持长按功能，
 * 则该值为-1。
 *
 * \return 标准错误号
 */
int am_event_input_key_pressed_without_timing (int key_code, int keep_time);


#ifdef __cplusplus
}
#endif

#endif /* __AM_EVENT_INPUT_KEY_H */
