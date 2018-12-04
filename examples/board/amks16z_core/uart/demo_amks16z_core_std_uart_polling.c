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
 * \brief USART查询方式下接收发送数据例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. PIOC_3 引脚连接PC串口的TXD;
 *   2. PIOC_4 引脚连接PC串口的RXD。
 *   3. 配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 * - 实验现象：
 *   1. 串口输出"HW example---UART test in polling mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_uart_polling.c src_amks16z_core_std_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-19  nwt, make some changes.
 * - 1.01 15-07-06  xym, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_std_uart_polling
 * \copydoc demo_amks16z_core_std_uart_polling.c
 */
 
/** [src_amks16z_core_std_uart_polling] */
#include "ametal.h"
#include "am_kl26_inst_init.h"
#include "am_board.h"
#include "demo_std_entries.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_uart_polling_entry (void)
{
    am_uart_handle_t handle;

    /* UART初始化  */
    handle = am_kl26_uart1_inst_init();

    demo_std_uart_polling_entry(handle);
}


 /** [src_amks16z_core_std_uart_polling] */
 
/* end of file */

