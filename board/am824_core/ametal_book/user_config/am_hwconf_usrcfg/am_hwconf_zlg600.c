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
 * \brief MiniPort-ZLG72128 用户配置文件
 * \sa am_hwconf_miniport_zlg72128.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "lpc82x_pin.h"
#include "am_zlg600.h"

static uint8_t g_zlg600_uart_txbuf[128];
static uint8_t g_zlg600_uart_rxbuf[128];

static const am_zlg600_uart_dev_info_t __g_zlg600_uart_devinfo = {
  	g_zlg600_uart_rxbuf,       				// 用于串口接收的缓冲区
 	  g_zlg600_uart_txbuf,       				// 用于串口发送的缓冲区
  	128,                       				// 接收数据缓冲区的大小
  	128,                        			// 发送数据缓冲区的大小
  	AM_ZLG600_BAUDRATE_115200,  			// 与ZLG600A通信的波特率设置为115200
 	  AM_ZLG600_MODE_AUTO_CHECK, 		    // 出厂模式使用的自动侦测模式
  	0x59,  								            // 默认地址
};

static am_zlg600_uart_dev_t  __g_zlg600_uart_dev;

am_zlg600_handle_t am_zlg600_uart_inst_init(void)
{
    am_uart_handle_t uart_handle = am_lpc82x_usart1_inst_init();
    return am_zlg600_uart_old_init(&__g_zlg600_uart_dev, uart_handle, &__g_zlg600_uart_devinfo);
}
/* end of file */
