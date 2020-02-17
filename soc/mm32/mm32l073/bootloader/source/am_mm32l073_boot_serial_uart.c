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
 * \brief bootloader 串行数据操作标准设备实现（uart驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 18-12-11  yrh, first implementation.
 * \endinternal
 */
#include "am_boot_serial_byte.h"
#include "am_zlg116_boot_serial_uart.h"
#include "am_int.h"
#include "amhw_zlg_uart.h"
#include "am_zlg116.h"
#include "am_zlg116_inst_init.h"
#include "am_bootconf_zlg116.h"

#define UART_CH       ZLG116_UART1_BASE  /**< \brief UART通道 */
#define UART_INT_NUM  INUM_UART1         /**< \brief UART中断号*/

static serial_byte_receive_func_t __g_uart_byte_receive_callback;

static int __uart_send(void *p_arg, const uint8_t *buffer, uint32_t byte_count)
{
    am_zlg116_boot_serial_uart_dev_t  *p_dev = (am_zlg116_boot_serial_uart_dev_t *)p_arg;
    return am_uart_poll_send(p_dev->uart_handle,buffer,byte_count);
}

static int __uart_receive(void *p_arg, uint8_t *buffer, uint32_t requested_bytes)
{
    am_zlg116_boot_serial_uart_dev_t *p_dev = (am_zlg116_boot_serial_uart_dev_t  *)p_arg;
    return am_uart_poll_receive(p_dev->uart_handle, buffer, requested_bytes);

}

static int __uart_int_callback_enable(void *p_arg, serial_byte_receive_func_t callback_fun)
{
    __g_uart_byte_receive_callback = callback_fun;
    return AM_OK;
}

static void __uart_irq_handler(void *p_arg)
{
    uint8_t data;
    if (amhw_zlg_uart_int_flag_check((amhw_zlg_uart_t *)UART_CH, AMHW_ZLG_UART_INT_RX_VAL_FLAG) == AM_TRUE) {

        amhw_zlg_uart_int_flag_clr((amhw_zlg_uart_t *)UART_CH,AMHW_ZLG_UART_INT_RX_VAL_FLAG);
        /* 获取新接收数据，并发送出去 */
        data = amhw_zlg_uart_data_read((amhw_zlg_uart_t *)UART_CH);

        __g_uart_byte_receive_callback(data);
    }
}

static void __zlg116_uart_int_init()
{
    /* 使能RDRF接收准中断 */
    amhw_zlg_uart_int_enable((amhw_zlg_uart_t *)UART_CH, AMHW_ZLG_UART_INT_RX_VAL_ENABLE);
    amhw_zlg_uart_int_flag_clr((amhw_zlg_uart_t *)UART_CH,AMHW_ZLG_UART_INT_RX_VAL_ENABLE);

    /* 关联中断向量号，开启中断 */
    am_int_enable(UART_INT_NUM);
    am_int_connect(UART_INT_NUM, (am_pfnvoid_t)__uart_irq_handler, (void *)NULL);
}

const static struct am_boot_serial_byte_funcs  __g_serial_uart_funcs = {
    __uart_send,
    __uart_receive,
    __uart_int_callback_enable,
};
static am_zlg116_boot_serial_uart_dev_t __g_serial_uart_dev;

am_boot_serial_handle_t am_zlg116_boot_serial_uart_init(am_boot_autobaud_handle_t autobaud_handle)
{
    int ret;
    uint32_t baund;
    am_uart_handle_t uart_handle = NULL;
    if(autobaud_handle == NULL) {
        return NULL;
    }

    __g_serial_uart_dev.serial_serv.p_funcs = &__g_serial_uart_funcs;
    __g_serial_uart_dev.serial_serv.p_drv   = &__g_serial_uart_dev;

    while(1) {
            ret = am_boot_baudrate_get(autobaud_handle, &baund);
            if (ret == AM_OK) {

                if(baund <= 7200 && baund > 3000) {
                    baund = 4800;
                } else if(baund <=384000  && baund > 192000) {
                    baund = 256000;
                }

                break;
            }
    }
    am_zlg116_boot_autobaud_inst_deinit (autobaud_handle);

    uart_handle = am_zlg116_uart1_inst_init();

    ret = am_uart_ioctl(uart_handle,
                        AM_UART_BAUD_SET,
                        (void *)baund);


    if(uart_handle == NULL) {
        return NULL;
    }
    __g_serial_uart_dev.uart_handle = uart_handle;
    __zlg116_uart_int_init();

    return &(__g_serial_uart_dev.serial_serv);
}
/* end of file */
