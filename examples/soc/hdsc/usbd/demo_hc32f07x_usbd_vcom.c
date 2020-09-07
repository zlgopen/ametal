/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USB Vcom 例程
 *
 * - 实验现象：
 * 1. 将板子与PC机用USB电缆线连接。
 * 2. PC端用串口助手(xcom)给板子发送数据，如果板子收到数据，会回传“recv success”.
 *
 * \par 源代码
 * \snippet demo_usbd_vcom.c src_usbd_vcom
 *
 * \internal
 * \par Modification History
 * - 1.00 19-1-09  adw, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_usbd_vcom
 * \copydoc demo_usbd_vcom.c
 */

/** [src_usbd_vcom] */
#include "ametal.h"
#include "string.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_rngbuf.h"
#include "am_vdebug.h"
#include "am_softimer.h"
#include "am_usbd_cdc_vcom.h"
#include "demo_hc32f07x_entries.h"

#define __RNG_BUFF_SIZE    256                   /**< 缓冲区大小. */

static char __g_rng_buff[__RNG_BUFF_SIZE] = {0}; /**< \brief 环形缓冲区buff*/

static char __g_buff[__RNG_BUFF_SIZE]     = {0}; /**< \brief 数据读取buff*/

static struct am_rngbuf __g_rngbuff;             /**< \brief 定义一个环形缓冲区实例*/


static void __test_function(void *p_arg, uint8_t *p_buffer, uint8_t len)
{
    am_rngbuf_put(&__g_rngbuff, (char *)p_buffer, len); /* 填充环形缓冲区*/
}

/**
 * \brief 例程入口
 */
void demo_usbd_vcom_entry (void* p_handle)
{
    uint32_t key = 0;
    am_usbd_cdc_vcom_handle handle = p_handle;
    // 设置串口接收中断
    am_usbd_cdc_vcom_recv_cb(handle, __test_function, NULL);

    am_rngbuf_init(&__g_rngbuff, __g_rng_buff, __RNG_BUFF_SIZE);

    for (;;){

        /* 如果环形缓冲区不为空，处理数据*/
        if (!am_rngbuf_isempty(&__g_rngbuff)) {
            key = am_int_cpu_lock();
            am_rngbuf_get(&__g_rngbuff, __g_buff, __RNG_BUFF_SIZE);

            am_kprintf("%s", __g_buff);

            // 给串口发数据 最多只能发64 字节，如果发送64字节以上，建议分段发送
            am_usbd_cdc_vcom_send(handle,
                                 (uint8_t *)"recv success\r\n",
                                 sizeof("recv success\r\n"));

            memset(__g_buff, 0, __RNG_BUFF_SIZE);
            am_int_cpu_unlock(key);
        }
    }
}
/** [src_usbd_vcom] */

/* end of file */
