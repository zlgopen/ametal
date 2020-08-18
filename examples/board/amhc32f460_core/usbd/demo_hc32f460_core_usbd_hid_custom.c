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
 * \brief USB_hid_custom例程，通过driver层的接口实现
 *
 * - 操作步骤：
 *   1. 在usb通用配置文件按如下配置user_config\am_hwconf_usrcfg\am_usb_common_config.h
 *   #define USB_DEVICE_CLASS_HID_CUSTOM   1
 *   //#define USB_DEVICE_CLASS_MSC_ON     1
 *   //#define USB_DEVICE_CLASS_HID_MOUSE  1
 *   2. 将USB连接上电脑后下载程序；
 *
 *
 * - 实验现象：
 *   1.上电后，可在PC设备管理器查看到一个新的hid device识别到了
 *   2.使用软件bus hound，找到这个设备节点，双击这个节点，进入命令发送界面
 *   3.在该界面通过OUT端点向设备发送数据可以控制LED1-LED4的状态
 *     发送 0x01 0x00 LED1灭
 *     发送 0x01 0x01 LED1点亮
 *     发送 0x02 0x00 LED2灭
 *     发送 0x02 0x01 LED2点亮
 *     发送 0x03 0x00 LED3灭
 *     发送 0x03 0x01 LED3点亮
 *     发送 0x04 0x00 LED4灭
 *     发送 0x04 0x01 LED4点亮
 *
 * \par 源代码
 * \snippet demo_hc32f460_core_usbd_hid_custom.c src_hc32f460_core_usbd_hid_custom
 *
 * \internal
 * \par Modification History
 * - 1.00 
 * \endinternal
 */

/**
 * \addtogroup demo_if_demo_hc32f460_core_usbd_hid_custom
 * \copydoc demo_hc32f460_core_usbd_hid_custom.c
 */

/** [src_demo_hc32f460_core_usbd_mouse] */

#include "demo_hc32f460_entries.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_core_entries.h"
#include "am_usbd_msc.h"
#include "am_vdebug.h"

extern void demo_usbd_hid_custom_entry (void* p_handle);

void demo_hc32f460_usbd_hid_custom_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core usbd hid custom!\r\n");

    am_usbd_dev_t handle = am_hc32f460_usb_hid_custom_inst_init();

    demo_usbd_hid_custom_entry(&handle);  
}

/** [src_demo_hc32f460_core_usbd_mouse] */

/* end of file */
