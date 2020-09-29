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
 * \brief ZLG116 UART 用户配置文件
 * \sa am_hwconf_zlg116_uart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-10  ari, first implementation.
 * \endinternal
 */

#include "am_zlg116.h"
#include "am_zlg_uart.h"
#include "am_gpio.h"
#include "hw/amhw_zlg_uart.h"
#include "hw/amhw_zlg_gpio.h"
#include "hw/amhw_zlg116_rcc.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_zlg116_uart
 * \copydoc am_hwconf_zlg116_uart.c
 * @{
 */

/** \brief 串口1平台初始化 */
static void __zlg_plfm_uart1_init (void)
{

     /* 引脚初始化 PIOA_9_UART1_TX  PIOA_10_UART1_RX */
     am_gpio_pin_cfg(PIOA_9, PIOA_9_UART1_TX | PIOA_9_AF_PP);
     am_gpio_pin_cfg(PIOA_10, PIOA_10_UART1_RX | PIOA_10_INPUT_FLOAT);

}

/** \brief 解除串口1平台初始化 */
static void __zlg_plfm_uart1_deinit (void)
{

     /* 关闭UART1时钟 */
     am_clk_disable(CLK_UART1);

     am_gpio_pin_cfg(PIOA_9, AM_GPIO_INPUT);
     am_gpio_pin_cfg(PIOA_10, AM_GPIO_INPUT);
}

/** \brief 串口1设备信息 */
static const am_zlg_uart_devinfo_t __g_uart1_devinfo = {
    ZLG116_UART1_BASE,              /**< \brief 串口1 */
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

/** \brief 串口2平台初始化 */
static void __zlg_plfm_uart2_init (void)
{

    /* 引脚初始化 PIOA_3_UART2_RX  PIOA_2_UART2_TX */
     am_gpio_pin_cfg(PIOA_3, PIOA_3_UART2_RX | PIOA_3_INPUT_FLOAT);
     am_gpio_pin_cfg(PIOA_2, PIOA_2_UART2_TX | PIOA_2_AF_PP);
}

/** \brief 解除串口2 平台初始化 */
static void __zlg_plfm_uart2_deinit (void)
{

    /* 关闭UART2时钟 */
    am_clk_disable(CLK_UART2);

    am_gpio_pin_cfg(PIOA_3, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_2, AM_GPIO_INPUT);
}

/** \brief 串口2 设备信息 */
static const am_zlg_uart_devinfo_t __g_uart2_devinfo = {

    ZLG116_UART2_BASE,                /**< \brief 串口2 */
    INUM_UART2,                       /**< \brief 串口2的中断编号 */
    CLK_UART2,                        /**< \brief 串口2的时钟 */

    AMHW_ZLG_UART_DATA_8BIT   |    /**< \brief 8位数据 */
    AMHW_ZLG_UART_PARITY_NO   |    /**< \brief 无极性 */
    AMHW_ZLG_UART_STOP_1BIT,       /**< \brief 1个停止位 */

    115200,                           /**< \brief 设置的波特率 */

    0,                                /**< \brief 无其他中断 */

    NULL,                             /**< \brief USART2使用RS485 */
    __zlg_plfm_uart2_init,            /**< \brief USART2的平台初始化 */
    __zlg_plfm_uart2_deinit,          /**< \brief USART2的平台去初始化 */
};

static am_zlg_uart_dev_t  __g_uart2_dev;   /**< \brief 定义串口2设备 */

/** \brief UART1实例初始化，获得uart1标准服务句柄 */
am_uart_handle_t am_zlg116_uart1_inst_init (void)
{
    return am_zlg_uart_init(&__g_uart1_dev, &__g_uart1_devinfo);
}

/** \brief UART1实例解初始化 */
void am_zlg116_uart1_inst_deinit (am_uart_handle_t handle)
{
    am_zlg_uart_deinit((am_zlg_uart_dev_t *)handle);
}

/** \brief UART2实例初始化，获得uart2标准服务句柄 */
am_uart_handle_t am_zlg116_uart2_inst_init (void)
{
    return am_zlg_uart_init(&__g_uart2_dev, &__g_uart2_devinfo);
}

/** \brief UART2实例解初始化 */
void am_zlg116_uart2_inst_deinit (am_uart_handle_t handle)
{
    am_zlg_uart_deinit((am_zlg_uart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
