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
 * \brief 输入事件管理器相关配置
 *
 * \sa am_servconf_event_input.c
 * \internal
 * \par modification history
 * - 1.00 15-01-19  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_event_category_input.h"
#include "am_event_input_key.h"

/**
 * \addtogroup am_if_src_servconf_event_input
 * \copydoc am_servconf_event_input.c
 * @{
 */

/* 最多支持2个按键同时长按，绝大部分应用，只会使用到一个  */
#define __KEY_LONG_PRESS_MAX_NUM   2

/* 定义缓存，供长按计时使用 */
am_local am_event_input_key_long_press_t __g_key_long_press_buf[__KEY_LONG_PRESS_MAX_NUM];

/**
 * \brief NVRAM 按键服务相关信息
 */
am_local am_const am_event_input_key_info_t __g_event_input_key_info = {
    1000,                              /* 长按1s后开始向应用上报按键事件      */
    100,                               /* 长按时，上报按键事件的周期是100ms   */
    __KEY_LONG_PRESS_MAX_NUM,          /* 同时长按的按键最大数目              */
    __g_key_long_press_buf             /* 用以支持多键同时长按的缓存          */
};

/**
 * \brief 事件输入管理器（包含按键输入）服务初始化
 */
void am_event_input_inst_init (void)
{
    /* 初始化输入事件管理器 */
    am_event_category_input_init();

    /* 初始化按键输入管理器 */
    am_event_input_key_init(&__g_event_input_key_info);
}

/**
 * @}
 */

/* end of file */
