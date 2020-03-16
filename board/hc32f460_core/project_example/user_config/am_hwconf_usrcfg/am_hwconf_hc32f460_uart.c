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

#if 1
/*******************************************************************************/
/** \brief 串口1平台初始化 */
static void __hc32f460_plfm_uart1_init(void)
{
//    am_clk_enable(CLK_USART1);
//    am_gpio_pin_cfg(PIOE_4, GPIO_AFIO(AMHW_HC32F460_AFIO_USART1_RX));
//    am_gpio_pin_cfg(PIOE_5, GPIO_AFIO(AMHW_HC32F460_AFIO_USART1_TX));
}

/** \brief 解除串口1平台初始化 */
static void __hc32f460_plfm_uart1_deinit(void)
{
//    am_clk_disable(CLK_USART1);
//
//    am_gpio_pin_cfg(PIOE_4, AM_GPIO_INPUT);
//    am_gpio_pin_cfg(PIOE_5, AM_GPIO_INPUT);
}

/** \brief 串口1设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart1_devinfo = {

        HC32F460_UART1_BASE,           /**< \brief 串口1 */
        0,                             /**< \brief 串口3的接收错误中断编号 */
        1,                             /**< \brief 串口3的接收满中断编号 */
        2,                             /**< \brief 串口3的发送空中断编号 */
        3,                             /**< \brief 串口3的发送完成中断编号 */
        4,                             /**< \brief 串口3的超时中断编号 */

        1,                             /**< \brief 串口1的设备编号 */
        CLK_USART1,                    /**< \brief 串口1的时钟 */

        AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
        AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
        AMHW_HC32F460_UART_STOP_1BIT,  /**< \brief 1个停止位 */

        115200,                        /**< \brief 设置的波特率 */

        0,                             /**< \brief 无其他中断 */

        NULL,                          /**< \brief 不使用RS485 */
        __hc32f460_plfm_uart1_init,    /**< \brief UART3的平台初始化 */
        __hc32f460_plfm_uart1_deinit,  /**< \brief UART3的平台去初始化 */
};

/**< \brief 定义串口1 设备 */
am_hc32f460_uart_dev_t __g_uart1_dev;

/** \brief UART1实例初始化，获得uart1标准服务句柄 */
am_uart_handle_t am_hc32f460_uart1_inst_init(void)
{
    return am_hc32f460_uart_init(&__g_uart1_dev, &__g_uart1_devinfo);
}

/** \brief UART1实例解初始化 */
void am_hc32f460_uart1_inst_deinit(am_uart_handle_t handle)
{
    am_hc32f460_uart_deinit((am_hc32f460_uart_dev_t *) handle);
}
#endif

/*******************************************************************************/
/** \brief 串口2平台初始化 */
static void __hc32f460_plfm_uart2_init (void)
{
	am_clk_enable(CLK_USART2);
	am_gpio_pin_cfg(PIOA_3, GPIO_AFIO(AMHW_HC32F460_AFIO_USART2_RX));
    am_gpio_pin_cfg(PIOA_2, GPIO_AFIO(AMHW_HC32F460_AFIO_USART2_TX));
}

/** \brief 解除串口2 平台初始化 */
static void __hc32f460_plfm_uart2_deinit (void)
{

    /* 关闭UART2时钟 */
    am_clk_disable(CLK_USART2);

    am_gpio_pin_cfg(PIOA_3, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_2, AM_GPIO_INPUT);
}

/** \brief 串口2 设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart2_devinfo = {

        HC32F460_UART2_BASE,           /**< \brief 串口2 */

        5,                             /**< \brief 串口2的接收错误中断编号 */
        6,                             /**< \brief 串口2的接收满中断编号 */
        7,                             /**< \brief 串口2的发送空中断编号 */
        8,                             /**< \brief 串口2的发送完成中断编号 */
        9,                             /**< \brief 串口2的超时中断编号 */

        2,                             /**< \brief 串口2的设备编号 */
        CLK_USART2,                    /**< \brief 串口2的时钟 */

        AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
        AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
        AMHW_HC32F460_UART_STOP_1BIT,  /**< \brief 1个停止位 */

        9600,                        /**< \brief 设置的波特率 */

        0,                             /**< \brief 无其他中断 */

        NULL,                          /**< \brief 不使用RS485 */
        __hc32f460_plfm_uart2_init,    /**< \brief UART2的平台初始化 */
        __hc32f460_plfm_uart2_deinit,  /**< \brief UART2的平台去初始化 */
};

/**< \brief 定义串口2设备 */
am_hc32f460_uart_dev_t __g_uart2_dev;

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
static void __hc32f460_plfm_uart3_init(void)
{
    am_clk_enable(CLK_USART3);
    am_gpio_pin_cfg(PIOE_4, GPIO_AFIO(AMHW_HC32F460_AFIO_USART3_RX));
    am_gpio_pin_cfg(PIOE_5, GPIO_AFIO(AMHW_HC32F460_AFIO_USART3_TX));
}

/** \brief 解除串口3平台初始化 */
static void __hc32f460_plfm_uart3_deinit(void)
{
    am_clk_disable(CLK_USART3);

    am_gpio_pin_cfg(PIOE_4, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOE_5, AM_GPIO_INPUT);
}

/** \brief 串口3设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart3_devinfo = {

        HC32F460_UART3_BASE,           /**< \brief 串口3 */

        10,                            /**< \brief 串口3的接收错误中断编号 */
        11,                            /**< \brief 串口3的接收满中断编号 */
        12,                            /**< \brief 串口3的发送空中断编号 */
        13,                            /**< \brief 串口3的发送完成中断编号 */
        14,                            /**< \brief 串口3的超时中断编号 */

        3,                             /**< \brief 串口3的设备编号 */
        CLK_USART3,                    /**< \brief 串口3的时钟 */

        AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
        AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
        AMHW_HC32F460_UART_STOP_1BIT,  /**< \brief 1个停止位 */

		115200,                          /**< \brief 设置的波特率 */

        0,                             /**< \brief 无其他中断 */

        NULL,                          /**< \brief 不使用RS485 */
        __hc32f460_plfm_uart3_init,    /**< \brief UART3的平台初始化 */
        __hc32f460_plfm_uart3_deinit,  /**< \brief UART3的平台去初始化 */
};

/**< \brief 定义串口3 设备 */
am_hc32f460_uart_dev_t __g_uart3_dev;

/** \brief UART3实例初始化，获得uart3标准服务句柄 */
am_uart_handle_t am_hc32f460_uart3_inst_init(void)
{
    return am_hc32f460_uart_init(&__g_uart3_dev, &__g_uart3_devinfo);
}

/** \brief UART3实例解初始化 */
void am_hc32f460_uart3_inst_deinit(am_uart_handle_t handle)
{
    am_hc32f460_uart_deinit((am_hc32f460_uart_dev_t *) handle);
}



/*******************************************************************************/

/** \brief 串口4平台初始化 */
static void __hc32f460_plfm_uart4_init(void)
{
    am_clk_enable(CLK_USART4);
    am_gpio_pin_cfg(PIOC_13, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_RX));
    am_gpio_pin_cfg(PIOH_2, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_TX));
}

/** \brief 解除串口4平台初始化 */
static void __hc32f460_plfm_uart4_deinit(void)
{
    am_clk_disable(CLK_USART4);

    am_gpio_pin_cfg(PIOH_2, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOC_13, AM_GPIO_INPUT);
}

/** \brief 串口4设备信息 */
static const am_hc32f460_uart_devinfo_t __g_uart4_devinfo = {

        HC32F460_UART4_BASE,           /**< \brief 串口4 */
        15,                            /**< \brief 串口4的接收错误中断编号 */
        16,                            /**< \brief 串口4的接收满中断编号 */
        17,                            /**< \brief 串口4的发送空中断编号 */
        18,                            /**< \brief 串口4的发送完成中断编号 */
        19,                            /**< \brief 串口4的超时中断编号 */

        4,                             /**< \brief 串口4的设备编号 */
        CLK_USART4,                    /**< \brief 串口4的时钟 */

        AMHW_HC32F460_UART_DATA_8BIT | /**< \brief 8位数据 */
        AMHW_HC32F460_UART_PARITY_NO | /**< \brief 无极性 */
        AMHW_HC32F460_UART_STOP_1BIT,  /**< \brief 1个停止位 */

        115200,                        /**< \brief 设置的波特率 */

        0,                             /**< \brief 无其他中断 */

        NULL,                          /**< \brief 不使用RS485 */
        __hc32f460_plfm_uart4_init,    /**< \brief UART4的平台初始化 */
        __hc32f460_plfm_uart4_deinit,  /**< \brief UART4的平台去初始化 */
};

/**< \brief 定义串口4 设备 */
am_hc32f460_uart_dev_t __g_uart4_dev;

/** \brief UART4实例初始化，获得uart4标准服务句柄 */
am_uart_handle_t am_hc32f460_uart4_inst_init(void)
{
    return am_hc32f460_uart_init(&__g_uart4_dev, &__g_uart4_devinfo);
}

/** \brief UART4实例解初始化 */
void am_hc32f460_uart4_inst_deinit(am_uart_handle_t handle)
{
    am_hc32f460_uart_deinit((am_hc32f460_uart_dev_t *) handle);
}

/**
 * @}
 */

/* end of file */
