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
 * \brief 蓝牙模块自动波特率例程，通过标准接口实现
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
 * \snippet demo_zlg9021_baudrate.c src_zlg9021_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 17-12-08  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg9021_baudrate
 * \copydoc demo_zlg9021_baudrate.c
 */

/** [src_zlg9021_baudrate] */
#include "ametal.h"
#include "am_zlg9021.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_lpc82x_inst_init.h"

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
void demo_zlg9021_baudrate_entry (am_zlg9021_handle_t zlg9021_handle)
{
    int32_t   i                      = 0;
    uint32_t  zlg9021_baudrate       = 0;
    uint32_t  zlg9021_baudrate_tab[] = {4800, 9600, 19200, 38400, 57600, 115200};

    /* 等待模块初始化完成(模块初始化完成之后会发送 "TTM:ZLG9021P0-1-TC") */
    am_mdelay(500);

    /* 尝试连接模块 */
    for (i = 0; i < AM_NELEMENTS(zlg9021_baudrate_tab); i++) {

        /* 设置与 ZLG9021 通信的串口的波特率 */
        am_uart_ioctl(zlg9021_handle->uart_handle,
                      AM_UART_BAUD_SET,
                      (void *)zlg9021_baudrate_tab[i]);

        /* 尝试获取 ZLG9021 的波特率 */
        am_zlg9021_ioctl(zlg9021_handle,
                         AM_ZLG9021_BAUD_GET,
                         (void *)&zlg9021_baudrate);

        if (zlg9021_baudrate_tab[i] == zlg9021_baudrate) {
            AM_DBG_INFO("ZLG9021 BAUDRATE: %d\r\n", zlg9021_baudrate);
            break;
        }
    }

    if (i == AM_NELEMENTS(zlg9021_baudrate_tab)) {
        AM_DBG_INFO("AM_ZLG9021_BAUD_GET failed\r\n");
        flash_led();
    }

    while (1) {
        am_led_on(0);
        am_mdelay(500);
        am_led_off(0);
        am_mdelay(500);
    }
}

/** [src_zlg9021_baudrate] */

/* end of file */
