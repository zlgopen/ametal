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
 * \brief MK100 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-20  mxy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_uart_rngbuf.h"
#include "am_mm32l073_inst_init.h"
#include "am_hwconf_mk100_uart.h"
#define UART_RX_BUF_SIZE  128  /**< \brief 接收环形缓冲区大小，应该为2^n  */
#define UART_TX_BUF_SIZE  128  /**< \brief 发送环形缓冲区大小，应该为2^n  */

/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief UART 接收环形缓冲区  */
static uint8_t __uart_rxbuf[UART_RX_BUF_SIZE];

/** \brief UART 发送环形缓冲区  */
static uint8_t __uart_txbuf[UART_TX_BUF_SIZE];

/** \brief MK100 设备实例 */
static am_mk100_dev_t __g_mk100_uart_dev ;

/** \brief MK100 设备信息实例 */
static am_mk100_devinfo_t __g_mk100_uart_dev_info = {
    __uart_rxbuf,
    __uart_txbuf,
    UART_RX_BUF_SIZE,
    UART_TX_BUF_SIZE,
};

/** \brief MK100 设备初始化 */
am_mk100_handle_t am_mk100_uart2_inst_init (void)
{
    uint32_t pam = AM_MK100_DEFAULT_BAUDRATE;
    __g_mk100_uart_dev.uart_handle = am_mm32l073_uart2_inst_init();
    am_uart_ioctl(__g_mk100_uart_dev.uart_handle, AM_UART_BAUD_SET, (void *)pam);
    return am_mk100_init(&__g_mk100_uart_dev, &__g_mk100_uart_dev_info, __g_mk100_uart_dev.uart_handle);
}

/** \brief MK100 设备解初始化 */
void am_mk100_uart2_inst_deinit(am_mk100_handle_t handle)
{
    am_mm32l073_uart2_inst_deinit(handle->uart_handle);
}
/* end of file */
