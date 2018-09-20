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
 * \brief AM824BLE 蓝牙模块 LED 例程，通过标准接口实现
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
 *       不能确定手中开发板蓝牙波特率，可以尝试使用 demo_am824ble_std_zlg9021_baudrate
 *       例程自动搜索蓝牙模块波特率。
 *
 * \par 源代码
 * \snippet demo_am824ble_std_zlg9021_led.c src_am824ble_std_zlg9021_led
 *
 * \internal
 * \par Modification history
 * - 1.00 17-12-08  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824ble_std_zlg9021_led
 * \copydoc demo_am824ble_std_zlg9021_led.c
 */

/** [src_am824ble_std_zlg9021_led] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_lpc82x_inst_init.h"
#include "string.h"
#include "demo_components_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824ble_std_zlg9021_led_entry (void)
{
    am_zlg9021_handle_t zlg9021_handle = am_zlg9021_inst_init();

    demo_zlg9021_led_entry(zlg9021_handle);
}

/** [src_am824ble_std_zlg9021_led] */

/* end of file */
