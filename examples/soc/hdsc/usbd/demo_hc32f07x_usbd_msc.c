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
 * \brief USB模拟U盘例程，通过driver层的接口实现
 *
 * - 操作步骤：
 *   1. 将USB连接上电脑后下载程序；
 *   2. 在电脑上会显示出一个盘符；
 *
 * - 实验现象：
 *   1. 打开盘符，可以看到里面有一个README.TXT文件。
 *   2. 可以往U盘中里面拖动文件,串口会显示你拖动文件的信息。
 *
 * \note
 *
 *
 *
 * \par 源代码
 * \snippet demo_usbd_msc.c src_usbd_msc
 *
 * \internal
 * \par Modification History
 * - 1.00 19-01-15  adw, first implementation
 * \endinternal
 */
/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_usbd_msc.h"
#include "am_rngbuf.h"
#include "demo_hc32f07x_entries.h"

#define   __USND_MSC_REC_BUFFER_SIZE    1024

static void __rec_buffer_cb(void *p_arg, uint8_t *p_buff, uint16_t len)
{
	//保存写入 flash数据
}

/**
 * \brief 例程入口
 */
void demo_usbd_msc_entry (void* p_handle)
{

    am_usbd_msc_handle handle = p_handle;
    am_usbd_msc_recv_callback(handle, __rec_buffer_cb, (void *)handle);

    /* 复位后延时一段时间，模拟USB设备拔出的动作 */
    am_mdelay(3000);

    while (1) {


    }
}
/**
 * \addtogroup demo_if_usbd_msc
 * \copydoc demo_usbd_msc.c
 */

/** [src_usbd_msc] */

/* end of file */
