/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 蓝牙模块 LED 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 手机安装蓝牙串口助手软件，连接蓝牙模块；
 *   2. 蓝牙串口助手软件通过字符串方式向模块分别发送 "on"、"off"、"tog"。
 *
 * - 实验现象：
 *   1. ZLG9021 模块初始化并配置成功后 LED0 按 500 ms 的间隔慢闪烁，如果初始化失败，
 *      LED0 按 100 ms 的间隔快闪烁；
 *   2. 蓝牙连接成功后，LED0 熄灭；
 *   3. 接收到 "on"、"off"、"tog" 字符串时，分别点亮、熄灭、翻转 LED0，并向手机端发
 *      送 "ok!"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_8 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、
 *       AM_CFG_KEY_ENABLE 和 AM_CFG_SOFTIMER_ENABLE 定义为 1。但这些宏
 *       已经默认配置为 1， 用户不必再次配置；
 *    4. ZLG9021 模块内连接关系如下：
 * <pre>
 *           PIO0_26  <-->  ZLG9021_RX
 *           PIO0_27  <-->  ZLG9021_TX
 *           PIO0_16  <-->  ZLG9021_EN
 *           PIO0_25  <-->  ZLG9021_BRTS
 *           PIO0_28  <-->  ZLG9021_RESET
 * </pre>
 *    5. 蓝牙串口 APP 可以在 http://www.zlg.cn/wireless/down/down/id/199.html 页面
 *       中下载的手机透传 APP 开发源码中解压得到；
 *    6. 蓝牙模块默认波特率为 9600，用户可以在 am_hwconf_zlg9021.h 中配置。如果用户
 *       不能确定手中开发板蓝牙波特率，可以尝试使用 demo_zlg9021_baudrate
 *       例程自动搜索蓝牙模块波特率。
 *
 * \par 源代码
 * \snippet demo_zlg9021_led.c src_zlg9021_led
 *
 * \internal
 * \par Modification history
 * - 1.00 17-12-08  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg9021_led
 * \copydoc demo_zlg9021_led.c
 */

/** [src_zlg9021_led] */
#include "ametal.h"
#include "am_zlg9021.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_led.h"
#include "string.h"

/** \brief 蓝牙连接标志 */
#define __ZLG9021_STATE_BLE_CONNECTED     1

/**
 * \brief 快闪烁 LED0
 */
static void flash_led (void)
{
    while (1) {
        am_led_on(0);
        am_mdelay(100);
        am_led_off(0);
        am_mdelay(100);
    }
}

/**
 * \brief 例程入口
 */
void demo_zlg9021_led_entry (am_zlg9021_handle_t zlg9021_handle)
{
    uint8_t             buf[4]                 = {0};
    char                zlg9021_name[15]       = {0};
    char                zlg9021_mac_addr[13]   = {0};
    char                zlg9021_pwd[13]        = {0};
    int32_t             zlg9021_tx_power       = 0;
    uint32_t            zlg9021_ble_adv_period = 0;
    uint32_t            zlg9021_version        = 0;
    am_bool_t           zlg9021_enc            = 0;

    /* 等待模块初始化完成(模块初始化完成之后会发送 "TTM:ZLG9021P0-1-TC") */
    am_mdelay(500);

    /* 设置接收超时时间(单位：ms) */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_TIMEOUT,
                         (void *)100) != AM_OK) {
        AM_DBG_INFO("AM_ZLG9021_TIMEOUT failed\r\n");
        flash_led();
    }

    /* 获取模块名 */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_NAME_GET,
                         (void *)zlg9021_name) != AM_OK) {
         AM_DBG_INFO("AM_ZLG9021_NAME_GET failed\r\n");
         flash_led();
     }

    /* 获取 MAC 地址 */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_MAC_GET,
                         (void *)zlg9021_mac_addr) != AM_OK) {
         AM_DBG_INFO("AM_ZLG9021_MAC_GET failed\r\n");
         flash_led();
     }

    /* 获取 BLE 的广播周期(单位: ms) */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_BLE_ADV_PERIOD_GET,
                         (void *)&zlg9021_ble_adv_period) != AM_OK) {
         AM_DBG_INFO("AM_ZLG9021_MAC_GET failed\r\n");
         flash_led();
     }

    /* 获取发送功率(dBm) */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_TX_POWER_GET,
                         (void *)&zlg9021_tx_power) != AM_OK) {
         AM_DBG_INFO("AM_ZLG9021_TX_POWER_GET failed\r\n");
         flash_led();
     }

    /* 获取固件版本号 */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_VERSION_GET,
                         (void *)&zlg9021_version) != AM_OK) {
         AM_DBG_INFO("AM_ZLG9021_VERSION_GET failed\r\n");
         flash_led();
     }

    /* 获取模块的配对码 */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_PWD_GET,
                         (void *)&zlg9021_pwd) != AM_OK) {
         AM_DBG_INFO("AM_ZLG9021_PWD_GET failed\r\n");
         flash_led();
     }

    /* 获取模块的加密状态 */
    if (am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_ENC_GET,
                         (void *)&zlg9021_enc) != AM_OK) {
         AM_DBG_INFO("AM_ZLG9021_ENC_GET failed\r\n");
         flash_led();
     }

    AM_DBG_INFO("\r\nzlg9021_name: %s\r\n"
                "zlg9021_mac_addr: %s\r\n"
                "zlg9021_pwd: %s\r\n"
                "zlg9021_tx_power: %d\r\n"
                "zlg9021_ble_adv_period: %d\r\n"
                "zlg9021_version: %d\r\n"
                "zlg9021_enc: %d\r\n",
                zlg9021_name,
                zlg9021_mac_addr,
                zlg9021_pwd,
                zlg9021_tx_power,
                zlg9021_ble_adv_period,
                zlg9021_version,
                zlg9021_enc);

    while (1) {

        if (zlg9021_handle->connect_state == __ZLG9021_STATE_BLE_CONNECTED) {

            memset(buf, '\0', 4);
            if ( am_zlg9021_recv(zlg9021_handle, buf, 3) > 0) {
                AM_DBG_INFO("zlg9021_recv:%s\r\n", buf);
            }

            if (strcmp((char *)buf, "on") == 0) {
                am_led_on(0);
                am_zlg9021_send(zlg9021_handle, (uint8_t *)"ok!", 3);
            }

            if (strcmp((char *)buf, "off") == 0) {
                am_led_off(0);
                am_zlg9021_send(zlg9021_handle, (uint8_t *)"ok!", 3);
            }

            if (strcmp((char *)buf, "tog") == 0) {
                am_led_toggle(0);
                am_zlg9021_send(zlg9021_handle, (uint8_t *)"ok!", 3);
            }
        } else {
            am_led_on(0);
            am_mdelay(500);
            am_led_off(0);
            am_mdelay(500);
        }
    }
}

/** [src_zlg9021_led] */

/* end of file */
