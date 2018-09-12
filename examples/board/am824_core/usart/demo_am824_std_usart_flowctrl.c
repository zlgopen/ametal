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
 * \brief USART 流控制例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 配置上位机串口波特率为 115200，8 位数据长度 1 位停止位 无奇偶校验；
 *   2. 打开 PC 端上位机的软件流控，设置 XON = 0x11， XOFF = 0x13。
 *
 * - 实验现象：
 *   1. 串口输出 "USART flowctrl test:\r\n"；
 *   2. 串口输出接收到的字符串；
 *   3. 例程中下位机每 100 毫秒只能处理 5 个字符（回送），因为开启了软件流控，所以
 *      PC 端大规模(使用带软件流控的上位机)发送数据也不会丢失数据，
 *      PC 发送的数据量等于接收的数据量。
 *
 * \note
 *    1. 例程使用 USART0，与 DEBUG 调试使用串口相同；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_std_usart_flowctrl.c src_am824_std_usart_flowctrl
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_usart_flowctrl
 * \copydoc demo_am824_std_usart_flowctrl.c
 */

/** [src_am824_std_usart_flowctrl] */
#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "am_vdebug.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_std_usart_flowctrl_entry (void)
{
   am_kprintf("demo am824 std usart flowctrl!\r\n");
  
   demo_lpc_std_usart_flowctrl_entry(am_lpc82x_usart0_inst_init());

}
/** [src_am824_std_usart_flowctrl] */

/* end of file */
