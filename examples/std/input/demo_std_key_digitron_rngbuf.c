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
 * \brief 按键例程，使用数码管实时显示当前按下的按键（按键事件缓存到环形缓冲区中）
 *
 *  本应用需要使用到两位数码管，指定的数码管显示器必须至少包含两位数码管。同时，
 *  使用到4个按键， 4个按键对应的按键编码固定为：KEY_0、KEY_1、KEY_2、KEY_3。
 *  运行该例程时，应确保系统有4个按键，且它们对应的按键编码为：KEY_0 ~ KEY_4
 * 
 *  按键回调通常在中断环境中运行，因此，不建议在按键回调中作过多的处理，特别是
 *  一些耗时较长的处理。此时，可以将按键缓存到环形缓冲区中，在main()函数中再依次
 *  处理。
 * 
 * - 操作步骤
 *   1. 调用入口函数
 *
 * - 实验现象：
 *   1. 按下一个按键，两位数码管显示按键编号 0 ~ 4；
 *   2. 偶数编号时，数码管闪烁，奇数编号时，数码管不闪烁。
 *
 * \par 源代码
 * \snippet demo_std_key_digitron_rngbuf.c src_std_key_digitron_rngbuf
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-03  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_key_digitron_rngbuf
 * \copydoc demo_std_key_digitron_rngbuf.c
 */

/** [src_std_key_digitron_rngbuf] */
#include "ametal.h"
#include "am_input.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_digitron_disp.h"
#include "am_rngbuf.h"
 
static char             __g_key_buf[256];              /* 定义 256 字节的缓存 */
static struct am_rngbuf __g_key_rngbuf;                /* 定义环形缓冲区实例  */
 
static void __digitron_disp_key_code (int id, int code)
{
    am_digitron_disp_char_at(id, 0, '0' + code / 10);  /* 十位                */
    am_digitron_disp_char_at(id, 1, '0' + code % 10);  /* 个位                */
 
    if ((code & 0x01) == 0) {                          /* 偶数时数码管闪烁    */
        am_digitron_disp_blink_set(id, 0, AM_TRUE);
        am_digitron_disp_blink_set(id, 1, AM_TRUE);
    } else {
        am_digitron_disp_blink_set(id, 0, AM_FALSE);
        am_digitron_disp_blink_set(id, 1, AM_FALSE);
    }
}
  
/* 按键实际处理函数 */
static void __input_key_proc (int id, int key_code, int key_state, int keep_time)
{
    AM_DBG_INFO("key_code: %d, key_state :%d, keep_time is %d \r\n",
                                                key_code, key_state, keep_time);

    switch (key_code) {

    case KEY_0:
        __digitron_disp_key_code(id, 0);
        break;

    case KEY_1:
        __digitron_disp_key_code(id, 1);
        break;

    case KEY_2:
        __digitron_disp_key_code(id, 2);
        break;

    case KEY_3:
        __digitron_disp_key_code(id, 3);
        break;

    default :
        break;
    }
}

/* 按键回调函数 */
static void __input_key_cb (void *p_arg, int key_code, int key_state, int keep_time)
{
    if (am_rngbuf_freebytes(&__g_key_rngbuf) < (sizeof(int) * 3)) {
        
        /* 剩余空间不能存放下按键事件， 按键事件丢弃 */
        AM_DBG_INFO("Buffer full, the key event discard!\r\n");
        
    } else {
        
        am_rngbuf_put(&__g_key_rngbuf, (char *)&key_code, sizeof(int));
        am_rngbuf_put(&__g_key_rngbuf, (char *)&key_state, sizeof(int));
        am_rngbuf_put(&__g_key_rngbuf, (char *)&keep_time, sizeof(int));
    }
}

/**
 * \brief 例程入口
 */
void demo_std_key_digitron_rngbuf_entry (int32_t id)
{
    static am_input_key_handler_t key_handler;

    /* 初始化，设置 8 段 ASCII 解码 */
    am_digitron_disp_decode_set(id, am_digitron_seg8_ascii_decode);
    
    /* 初始无按键按下，显示 -- */
    am_digitron_disp_char_at(id, 0, '-');
    am_digitron_disp_char_at(id, 1, '-');

    am_input_key_handler_register(&key_handler, __input_key_cb, NULL);

    am_rngbuf_init(&__g_key_rngbuf, __g_key_buf, sizeof(__g_key_buf));
	
    AM_FOREVER {

        int key_code;
        int key_state;
        int keep_time;
        
        /* key_code 是 int 类型
         *
         * 只要缓存超过 3 个 int 所占字节数（key_state 和 key_code）
         * 表明存在按键事件 
         */
        if (am_rngbuf_nbytes(&__g_key_rngbuf) >= (sizeof(int) * 3)) {
            
            am_rngbuf_get(&__g_key_rngbuf, (char *)&key_code,  sizeof(int));
            am_rngbuf_get(&__g_key_rngbuf, (char *)&key_state, sizeof(int));
            am_rngbuf_get(&__g_key_rngbuf, (char *)&keep_time, sizeof(int));

            __input_key_proc(id, key_code, key_state, keep_time);
        }
    }
}
/** [src_std_key_digitron_rngbuf] */

/* end of file */
