/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USB printer_counter 例程
 *
 * - 实验现象：
 * 1.将USB的另端点接入PC机。
 * 2.给板子烧录该例程，等待3秒(程序中有3秒延时，为了模拟USB拔出过程)，3秒后pc机会提示安装驱动，
 *   这里默认安装windows 通用打印机驱动，详细可看驱动安装说明文档，如果用户想要用自己的驱动，可以自行更新。
 * 3.在电脑桌面新建一个txt文件，输入字符串,之后点击文件打印，即在串口中可以看到文件内容.
 *
 * \par 源代码
 * \snippet demo_usbd_printer.c src_usbd_printer
 *
 * \internal
 * \par Modification History
 * - 1.00 19-1-09  adw, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_usbd_printer
 * \copydoc demo_usbd_printer.c
 */

/** [src_usbd_printer] */
#include "ametal.h"
#include "string.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_rngbuf.h"
#include "am_vdebug.h"
#include "am_softimer.h"
#include "am_zmf159_usbd.h"
#include "am_usbd_printer.h"
#include "am_zmf159_inst_init.h"

#define __RNG_BUFF_SIZE    512                   /**< 缓冲区大小. */

static char __g_rng_buff[__RNG_BUFF_SIZE] = {0}; /**< \brief 环形缓冲区buff*/

static char __g_buff[__RNG_BUFF_SIZE]     = {0}; /**< \brief 数据读取buff*/

static struct am_rngbuf __g_rngbuff;             /**< \brief 定义一个环形缓冲区实例*/

/**
 * \brief 打印机接收中断请求回调函数
 *
 * param[in] p_arg  : 回调函数参数
 * param[in] p_data : 接收数据buff
 * param[in] len    : 接收数据的有效长度（数据量）
 */
static void __printer_recv_callback(void *p_arg, uint8_t *p_data, uint8_t len)
{
    am_rngbuf_put(&__g_rngbuff, (char *)p_data, len); /* 填充环形缓冲区*/
}

/**
 * \brief 打印机发送中断请求回调函数
 *
 * param[in] p_arg  : 回调函数参数
 */
static void __printer_send_callback(void *p_arg)
{
    //该函数为打印机发送请求,即主机有请求USB device 发送数据(定义的端点数据)时，就会进入该函数
    uint8_t data[] = "ZLG printer demo test string";
    am_usbd_printer_handle handle = (am_usbd_printer_handle)p_arg;

    am_usbd_printer_send(handle, AM_USBD_PRINTER_BULK_EP_IN, data, sizeof(data));
}

/**
 * \brief 例程入口
 */
void demo_zmf159_usbd_printer_entry (void)
{
    uint32_t key = 0;
    am_kprintf("printer demo\n");

    am_mdelay(3000);                               /* 模拟USB设备拔出的动作 */
    am_usbd_printer_handle handle = NULL;

    /* 初始化环形缓冲区*/
    am_rngbuf_init(&__g_rngbuff, __g_rng_buff, __RNG_BUFF_SIZE);

    handle = am_zmf159_usbd_printer_inst_init();    /* usb打印机实例初始化*/

    /* 定义软件定时器接收和发送请求回调函数*/
    am_usbd_printer_recv_request_callback(handle, __printer_recv_callback, handle);
    am_usbd_printer_send_request_callback(handle, __printer_send_callback, handle);


    while (1) {
        /* 如果环形缓冲区不为空，处理数据*/
        if (!am_rngbuf_isempty(&__g_rngbuff)) {
            key = am_int_cpu_lock();
            am_rngbuf_get(&__g_rngbuff, __g_buff, __RNG_BUFF_SIZE);

            am_kprintf("%s", __g_buff);

            memset(__g_buff, 0, __RNG_BUFF_SIZE);
            am_int_cpu_unlock(key);
        }
    }
}
/** [src_usbd_printer] */

/* end of file */
