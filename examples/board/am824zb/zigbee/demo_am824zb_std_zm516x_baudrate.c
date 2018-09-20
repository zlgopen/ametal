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
 * \snippet demo_am824zb_std_zm516x_baudrate.c src_am824zb_std_zm516x_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 18-04-12  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824zb_std_zm516x_baudrate
 * \copydoc demo_am824zb_std_zm516x_baudrate.c
 */

/** [src_am824zb_std_zm516x_baudrate] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_vdebug.h"
#include "demo_components_entries.h"
#include "am_lpc82x_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_am824zb_std_zm516x_baudrate_entry (void)
{
    am_zm516x_handle_t   zm516x_handle = am_zm516x_inst_init();

    demo_zm516x_baudrate_entry(zm516x_handle);
}

/** [src_am824zb_std_zm516x_baudrate] */

/* end of file */
