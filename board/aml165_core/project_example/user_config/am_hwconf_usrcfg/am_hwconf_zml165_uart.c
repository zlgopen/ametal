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
 * \brief ZML165 UART 用户配置文件
 * \sa am_hwconf_zml165_uart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-10  ari, first implementation.
 * \endinternal
 */

#include "am_zml165.h"
#include "am_zlg_uart.h"
#include "am_gpio.h"
#include "hw/amhw_zlg_uart.h"
#include "hw/amhw_zlg_gpio.h"
#include "hw/amhw_zml165_rcc.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_zml165_uart
 * \copydoc am_hwconf_zml165_uart.c
 * @{
 */

/** \brief 串口1平台初始化 */
static void __zlg_plfm_uart1_init (void)
{
    /* 引脚初始化 PIOB_3_UART1_TX  PIOB_4_UART1_RX */
    am_gpio_pin_cfg(PIOB_3, PIOB_3_UART1_TX | PIOB_3_AF_PP);
    am_gpio_pin_cfg(PIOB_4, PIOB_4_UART1_RX | PIOB_4_INPUT_FLOAT);
}

/** \brief 解除串口1平台初始化 */
static void __zlg_plfm_uart1_deinit (void)
{
     /* 关闭UART1时钟 */
     am_clk_disable(CLK_UART1);
     am_gpio_pin_cfg(PIOB_3, AM_GPIO_INPUT);
     am_gpio_pin_cfg(PIOB_4, AM_GPIO_INPUT);
}

/** \brief 串口1设备信息 */
static const am_zlg_uart_devinfo_t __g_uart1_devinfo = {
    ZML165_UART1_BASE,              /**< \brief 串口1 */
    INUM_UART1,                     /**< \brief 串口1的中断编号 */
    CLK_UART1,                      /**< \brief 串口1的时钟 */

    AMHW_ZLG_UART_DATA_8BIT   |  /**< \brief 8位数据 */
    AMHW_ZLG_UART_PARITY_NO   |  /**< \brief 无极性 */
    AMHW_ZLG_UART_STOP_1BIT,     /**< \brief 1个停止位 */

    115200,                         /**< \brief 设置的波特率 */

    0,                              /**< \brief 无其他中断 */

    NULL,                           /**< \brief USART1使用RS485 */
    __zlg_plfm_uart1_init,          /**< \brief USART1的平台初始化 */
    __zlg_plfm_uart1_deinit,        /**< \brief USART1的平台去初始化 */
};

static am_zlg_uart_dev_t  __g_uart1_dev;   /**< \brief 定义串口1 设备 */

/** \brief UART1实例初始化，获得uart1标准服务句柄 */
am_uart_handle_t am_zml165_uart1_inst_init (void)
{
    return am_zlg_uart_init(&__g_uart1_dev, &__g_uart1_devinfo);
}

/** \brief UART1实例解初始化 */
void am_zml165_uart1_inst_deinit (am_uart_handle_t handle)
{
    am_zlg_uart_deinit((am_zlg_uart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
