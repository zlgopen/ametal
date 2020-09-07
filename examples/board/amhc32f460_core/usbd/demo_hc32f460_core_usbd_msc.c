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
 *   1. 在usb通用配置文件按如下配置user_config\am_hwconf_usrcfg\am_usb_common_config.h
 *   #define USB_DEVICE_CLASS_MSC_ON   1
 *   //#define USB_DEVICE_CLASS_HID_MOUSE   1
 *   //#define USB_DEVICE_CLASS_HID_CUSTOM   1
 *   2. 将USB连接上电脑后下载程序；
 *   3. 在电脑上会显示出一个盘符；
 *
 * - 实验现象：
 *   1. 打开盘符，可以看到里面有一个README.TXT文件。
 *   2. 可以往U盘中里面拖动文件,串口会显示你拖动文件的信息。
 *
 * \par 源代码
 * \snippet demo_hc32f460_core_usbd_msc.c src_hc32f460_core_usbd_msc
 *
 * \internal
 * \par Modification History
 * - 1.00 19-05-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_demo_hc32f460_core_usbd_msc
 * \copydoc demo_hc32f460_core_usbd_msc.c
 */

/** [src_demo_hc32f460_core_usbd_msc] */

#include "demo_hc32f460_entries.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_core_entries.h"
#include "am_usbd_msc.h"
#include "am_vdebug.h"

void demo_hc32f460_usbd_msc_entry (void)
{

    AM_DBG_INFO("demo hc32f460_core usbd msc!\r\n");

    /* usb U盘实例初始化*/
    am_usbd_msc_handle handle = am_hc32f460_usb_msc_inst_init();

    demo_usbd_msc_entry(handle);
}

/** [src_demo_hc32f460_core_usbd_msc] */

/* end of file */
