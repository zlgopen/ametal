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
 * \brief HC32F460 UART 用户配置文件
 * \sa am_hc32f460_hwconfig_uart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-10  ari, first implementation
 * \endinternal
 */

#include "am_gpio.h"
#include "am_hc32f460_uart.h"
#include "am_clk.h"
#include "am_hc32f460.h"
#include "hw/amhw_hc32f460_gpio.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_uart
 * \copydoc am_hwconf_hc32f460_uart.c
 * @{
 */

/** \brief 串口3平台初始化 */
static void __hc32f460_plfm_uart3_init(void)
{
    am_clk_enable(CLK_USART3);
    am_gpio_pin_cfg(PIOE_4, GPIO_AFIO(AMHW_HC32F460_AFIO_USART3_RX));
    am_gpio_pin_cfg(PIOE_5, GPIO_AFIO(AMHW_HC32F460_AFIO_USART3_TX));
}

/** \brief 解除串口1平台初始化 */
static void __hc32f460_plfm_uart3_deinit(void)
{
    am_clk_disable(CLK_USART3);

    am_gpio_pin_cfg(PIOE_4, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOE_5, AM_GPIO_INPUT);
}

/** \brief 串口3设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart3_devinfo = {

        HC32F460_UART3_BASE, /**< \brief 串口3 */
        INUM_USART3,              /**< \brief 串口3的中断编号 */
        3,               /**< \brief 串口3的设备编号 */
        CLK_USART3, /**< \brief 串口3的时钟 */

        AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
        AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
        AMHW_HC32F460_UART_STOP_1BIT, /**< \brief 1个停止位 */

        115200, /**< \brief 设置的波特率 */

        0, /**< \brief 无其他中断 */

        NULL, /**< \brief UART1使用RS485 */
        __hc32f460_plfm_uart3_init, /**< \brief UART1的平台初始化 */
        __hc32f460_plfm_uart3_deinit, /**< \brief UART1的平台去初始化 */
};

/**< \brief 定义串口3 设备 */
am_hc32f460_uart_dev_t __g_uart3_dev;

/** \brief UART1实例初始化，获得uart1标准服务句柄 */
am_uart_handle_t am_hc32f460_uart3_inst_init(void)
{
    return am_hc32f460_uart_init(&__g_uart3_dev, &__g_uart3_devinfo);
}

/** \brief UART1实例解初始化 */
void am_hc32f460_uart3_inst_deinit(am_uart_handle_t handle)
{
    am_hc32f460_uart_deinit((am_hc32f460_uart_dev_t *) handle);
}

/*******************************************************************************/
#if 0
/** \brief 串口2平台初始化 */
static void __hc32f460_plfm_uart2_init (void)
{
    am_gpio_pin_cfg(PIOA_2, PIOA_2_UART2_TX | PIOA_2_AF_PP);
    am_gpio_pin_cfg(PIOA_3, PIOA_3_UART2_RX | PIOA_3_INPUT_FLOAT);
}

/** \brief 解除串口2 平台初始化 */
static void __hc32f460_plfm_uart2_deinit (void)
{

    /* 关闭UART2时钟 */
    am_clk_disable(CLK_UART2);

    am_gpio_pin_cfg(PIOA_3, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_2, AM_GPIO_INPUT);
}

/** \brief 串口2 设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart2_devinfo = {

    ZMF159_UART2_BASE, /**< \brief 串口2 */
    INUM_UART2, /**< \brief 串口2的中断编号 */
    CLK_UART2, /**< \brief 串口2的时钟 */

    AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
    AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
    AMHW_HC32F460_UART_STOP_1BIT, /**< \brief 1个停止位 */

    115200, /**< \brief 设置的波特率 */

    0, /**< \brief 无其他中断 */

    NULL, /**< \brief UART2使用RS485 */
    __hc32f460_plfm_uart2_init, /**< \brief UART2的平台初始化 */
    __hc32f460_plfm_uart2_deinit, /**< \brief UART2的平台去初始化 */
};

/**< \brief 定义串口2设备 */
static am_hc32f460_uart_dev_t __g_uart2_dev;

/** \brief UART2实例初始化，获得uart2标准服务句柄 */
am_uart_handle_t am_hc32f460_uart2_inst_init (void)
{
    return am_hc32f460_uart_init(&__g_uart2_dev, &__g_uart2_devinfo);
}

/** \brief UART2实例解初始化 */
void am_hc32f460_uart2_inst_deinit (am_uart_handle_t handle)
{
    am_hc32f460_uart_deinit((am_hc32f460_uart_dev_t *)handle);
}

/*******************************************************************************/

/** \brief 串口3平台初始化 */
static void __hc32f460_plfm_uart3_init (void)
{
//     am_gpio_pin_cfg(PIOB_10, PIOB_10_UART3_TX | PIOB_10_AF_PP | PIOB_10_SPEED_10MHz);
//     am_gpio_pin_cfg(PIOB_11, PIOB_11_UART3_RX | PIOB_11_INPUT_FLOAT);
}

/** \brief 解除串口3 平台初始化 */
static void __hc32f460_plfm_uart3_deinit (void)
{
    am_clk_disable(CLK_UART3);

    am_gpio_pin_cfg(PIOB_10, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOB_11, AM_GPIO_INPUT);
}

/** \brief 串口3 设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart3_devinfo = {

    ZMF159_UART3_BASE, /**< \brief 串口3 */
    INUM_UART3, /**< \brief 串口2的中断编号 */
    CLK_UART3, /**< \brief 串口2的时钟 */

    AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
    AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
    AMHW_HC32F460_UART_STOP_1BIT, /**< \brief 1个停止位 */

    115200, /**< \brief 设置的波特率 */

    0, /**< \brief 无其他中断 */

    NULL, /**< \brief UART3使用RS485 */
    __hc32f460_plfm_uart3_init, /**< \brief UART3的平台初始化 */
    __hc32f460_plfm_uart3_deinit, /**< \brief UART3的平台去初始化 */
};

/**< \brief 定义串口3设备 */
static am_hc32f460_uart_dev_t __g_uart3_dev;

/** \brief UART3实例初始化，获得uart3标准服务句柄 */
am_uart_handle_t am_hc32f460_uart3_inst_init (void)
{
    return am_hc32f460_uart_init(&__g_uart3_dev, &__g_uart3_devinfo);
}

/** \brief UART3实例解初始化 */
void am_hc32f460_uart3_inst_deinit (am_uart_handle_t handle)
{
    am_hc32f460_uart_deinit((am_hc32f460_uart_dev_t *)handle);
}

/*******************************************************************************/

/** \brief 串口4平台初始化 */
static void __hc32f460_plfm_uart4_init (void)
{
}

/** \brief 解除串口4 平台初始化 */
static void __hc32f460_plfm_uart4_deinit (void)
{
    am_clk_disable(CLK_UART4);

}

/** \brief 串口4 设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart4_devinfo = {

    ZMF159_UART4_BASE, /**< \brief 串口4 */
    INUM_UART4, /**< \brief 串口4的中断编号 */
    CLK_UART4, /**< \brief 串口4的时钟 */

    AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
    AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
    AMHW_HC32F460_UART_STOP_1BIT, /**< \brief 1个停止位 */

    115200, /**< \brief 设置的波特率 */

    0, /**< \brief 无其他中断 */

    NULL, /**< \brief UART4使用RS485 */
    __hc32f460_plfm_uart4_init, /**< \brief UART4的平台初始化 */
    __hc32f460_plfm_uart4_deinit, /**< \brief UART4的平台去初始化 */
};

/**< \brief 定义串口4设备 */
static am_hc32f460_uart_dev_t __g_uart4_dev;

/** \brief UART4实例初始化，获得uart4标准服务句柄 */
am_uart_handle_t am_hc32f460_uart4_inst_init (void)
{
    return am_hc32f460_uart_init(&__g_uart4_dev, &__g_uart4_devinfo);
}

/** \brief UART4实例解初始化 */
void am_hc32f460_uart4_inst_deinit (am_uart_handle_t handle)
{
    am_hc32f460_uart_deinit((am_hc32f460_uart_dev_t *)handle);
}
#endif
/**
 * @}
 */

/* end of file */
