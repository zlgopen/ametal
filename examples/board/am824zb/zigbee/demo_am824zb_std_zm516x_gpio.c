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
 * \brief ZM516X 模块 GPIO 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. ZM516X 模块初始化并配置成功后 LED0 长亮，如果初始化失败，LED0 闪烁；
 *   2. 串口打印读取到的模块配置信息；
 *   3. 读取 IO1(JOIN) 引脚的电平并将相同的电平输出到 IO2(DETECH) 引脚。
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
 * \snippet demo_am824zb_std_zm516x_gpio.c src_am824zb_std_zm516x_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 18-01-15  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824zb_std_zm516x_gpio
 * \copydoc demo_am824zb_std_zm516x_gpio.c
 */

/** [src_am824zb_std_zm516x_gpio] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_vdebug.h"
#include "demo_components_entries.h"
#include "am_lpc82x_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_am824zb_std_zm516x_gpio_entry (void)
{

    am_zm516x_handle_t zm516x_handle  = am_zm516x_inst_init();

    demo_zm516x_gpio_entry(zm516x_handle);
}
/** [src_am824zb_std_zm516x_gpio] */

/* end of file */
