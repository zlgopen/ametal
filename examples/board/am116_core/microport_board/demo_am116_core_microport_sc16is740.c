/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief MicroPort SC16IS740 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将  MicroPort SC16IS740 配板连接到 AM116-Core 的 MicroPort 接口；
 *   2. 将  MicroPort SC16IS740 配板上的 SC16IS740 接口与 PC 连接并通过串口助手打开。
 *
 * - 实验现象：
 *   1. 串口输出"MicroPort SC16IS740 Test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *   1. XR21V141x 为 SC16IS740 转串口芯片，使用前需要安装驱动程序。用户可以在 EXAR
 *      官方网站(http://www.exarcorp.cn/design-tools/software-drivers)下载；
 *   2. 例程使用 USART0，与 DEBUG 调试使用串口相同。
 *
 * \par 源代码
 * \snippet demo_am116_core_microport_sc16is740.c src_am116_core_microport_sc16is740
 *
 * \internal
 * \par Modification History
 * - 1.00 17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am116_core_microport_sc16is740
 * \copydoc demo_am116_core_microport_sc16is740.c
 */

/** [src_am116_core_microport_sc16is740] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg116_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am116_core_microport_sc16is740_entry (void)
{
    AM_DBG_INFO("demo am116_core microport sc16is740!\r\n");

    demo_std_uart_polling_entry(am_zlg116_uart1_inst_init());
}
/** [src_am116_core_microport_sc16is740] */

/* end of file */
