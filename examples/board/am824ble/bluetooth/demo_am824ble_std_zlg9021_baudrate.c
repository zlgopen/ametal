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
 * \brief AM824BLE 蓝牙模块自动波特率例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 单片机分别尝试通过 4800、9600、19200、38400、57600 和 115200 的波特率连接
 *      ZLG7021；
 *   2. 如果连接成功，通过串口打印当前的波特率并按 500 ms 的间隔慢闪烁 LED0，否则
 *      LED0 按 100 ms 的间隔快闪烁。
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
 *       如果需要使用 ZLG9021，这些 IO 口不能用作其它用途。
 *
 * \par 源代码
 * \snippet demo_am824ble_std_zlg9021_baudrate.c src_am824ble_std_zlg9021_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 17-12-08  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824ble_std_zlg9021_baudrate
 * \copydoc demo_am824ble_std_zlg9021_baudrate.c
 */

/** [src_am824ble_std_zlg9021_baudrate] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_lpc82x_inst_init.h"
#include "demo_components_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824ble_std_zlg9021_baudrate_entry (void)
{
    am_zlg9021_handle_t zlg9021_handle = am_zlg9021_inst_init();

    demo_zlg9021_baudrate_entry(zlg9021_handle);
}

/** [src_am824ble_std_zlg9021_baudrate] */

/* end of file */
