/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s app_digitron_count_down driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_digitron_disp.h"
#include "am_vdebug.h"
#include "am_softimer.h"

static void __digitron_show_num (int id, int num)
{
    char buf[3];
    am_snprintf(buf, 3, "%2d", num);
    am_digitron_disp_str(id, 0, 2, buf);
}

static void __count_down_timer_cb (void *p_arg)  // 定时器回调函数，每秒调用一次
{
    static unsigned int num = 60;

    if (num > 0) {
        num--;
    } else {
        num = 60;
    }
    __digitron_show_num((int)p_arg, num);
    if (num < 5) {
        am_digitron_disp_blink_set(0, 1, AM_TRUE);
    } else {
        am_digitron_disp_blink_set(0, 1, AM_FALSE);
    }
}

int app_digitron_count_down (int id)                            // 应用函数入口
{
    static am_softimer_t timer;

    am_digitron_disp_decode_set(0, am_digitron_seg8_ascii_decode);     // 使用默认的解码函数
    __digitron_show_num(id, 60);                                  // 初始显示60
    am_softimer_init(&timer, __count_down_timer_cb, (void *)id);
    am_softimer_start(&timer, 1000);                              // 启动定时器，定时周期1s
    return AM_OK;
}


/* end of file */
