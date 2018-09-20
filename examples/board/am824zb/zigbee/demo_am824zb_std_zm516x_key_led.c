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
 * \brief ZM516X 模块 KEY LED 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 测试本 Demo 需要使用两块 AM824ZB 板，两块板需要下载不同的程序，
 *      其中一块 AM824ZB 板打开宏 USE_BORD0，关闭宏 USE_BORD1，编译并下载程序，
 *      另外一块 AM824ZB 板打开宏 USE_BORD1，关闭宏 USE_BORD0，编译并下载程序。
 *
 * - 实验现象：
 *   1. ZM516X 模块初始化并配置成功后 LED0 长亮，如果初始化失败，LED0 闪烁；
 *   2. 按下 KEY/RST 键，向对方发送 key_code。如果发送成功，将通过串口打印
 *      "send key code xx success"如果发送失败，将通过串口打印
 *      "send key code xx failed"。对方接收到之后，翻转 LED1，并通过串口
 *      打印 "received key code: xx"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_8 控制；
 *    2. 使用按键功能需要将 J14 的 KEY 和 PIO0_1 短接在一起；
 *    3. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    4. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE 定义为 1
 *       但该宏已经默认配置为 1， 用户不必再次配置；
 *    5. ZigBee 模块内连接关系如下：
 * <pre>
 *           PIO0_26  <-->  ZigBee_TX
 *           PIO0_27  <-->  ZigBee_RX
 *           PIO0_28  <-->  ZigBee_RST
 * </pre>
 *        如果需要使用 ZigBee，这些 IO 口不能用作其它用途。
 *
 * \par 源代码
 * \snippet demo_am824zb_std_zm516x_key_led.c src_am824zb_std_zm516x_key_led
 *
 * \internal
 * \par Modification History
 * - 1.01 18-01-16  pea, use the send API with the ACK
 * - 1.00 17-09-20  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824zb_std_zm516x_key_led
 * \copydoc demo_am824zb_std_zm516x_key_led.c
 */

/** [src_am824zb_std_zm516x_key_led] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_vdebug.h"
#include "demo_components_entries.h"
#include "am_lpc82x_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_am824zb_std_zm516x_key_led_entry (void)
{
    am_zm516x_handle_t zm516x_handle  = am_zm516x_inst_init();

    demo_zm516x_key_led_entry(zm516x_handle);
}
/** [src_am824zb_std_zm516x_key_led] */

/* end of file */
