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
 * \brief ZM516X 模块自动波特率例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 单片机分别尝试通过 2400、4800、9600、19200、38400、57600 和 115200 的波特
 *      率连接 ZM516X 模块；
 *   2. 如果连接成功，通过串口打印当前的波特率并且 LED0 长亮，否则 LED0 闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_8 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. ZigBee 模块内连接关系如下：
 * <pre>
 *           PIO0_26  <-->  ZigBee_TX
 *           PIO0_27  <-->  ZigBee_RX
 *           PIO0_28  <-->  ZigBee_RST
 * </pre>
 *        如果需要使用 ZigBee，这些 IO 口不能用作其它用途。
 *
 * \par 源代码
 * \snippet demo_zm516x_baudrate.c src_zm516x_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 18-04-12  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zm516x_baudrate
 * \copydoc demo_zm516x_baudrate.c
 */

/** [src_zm516x_baudrate] */
#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_zm516x.h"
#include "am_vdebug.h"
#include "am_board.h"

/**
 * \brief 闪烁 LED0
 */
am_local void flash_led (void)
{
    while (1) {
        am_led_on(LED0);
        am_mdelay(50);

        am_led_off(LED0);
        am_mdelay(500);
    }
}

/**
 * \brief 例程入口
 */
void demo_zm516x_baudrate_entry (am_zm516x_handle_t zm516x_handle)
{
    am_zm516x_cfg_info_t zm516x_cfg_info;
    int32_t              i                     = 0;
    uint32_t             zm516x_baudrate_tab[] = {2400,
                                                  4800,
                                                  9600,
                                                  19200,
                                                  38400,
                                                  57600,
                                                  115200};

    /* 等待模块初始化完成 */
    am_mdelay(10);

    /* 尝试连接模块 */
    for (i = 0; i < AM_NELEMENTS(zm516x_baudrate_tab); i++) {

        /* 设置与 ZM516X 通信的串口的波特率 */
        am_uart_ioctl(zm516x_handle->uart_handle->handle,
                      AM_UART_BAUD_SET,
                      (void *)zm516x_baudrate_tab[i]);

        /* 尝试获取 ZM516X 模块的配置信息 */
        if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) == AM_OK) {
            AM_DBG_INFO("zm516x baudrate is %d\r\n", zm516x_baudrate_tab[i]);
            break;
        }
    }

    if (i == AM_NELEMENTS(zm516x_baudrate_tab)) {
        AM_DBG_INFO("get baudrate failed\r\n");
        flash_led();
    }

    am_led_on(LED0);

    while (1) {
        am_mdelay(1000);
    }
}

/** [src_zm516x_baudrate] */

/* end of file */
