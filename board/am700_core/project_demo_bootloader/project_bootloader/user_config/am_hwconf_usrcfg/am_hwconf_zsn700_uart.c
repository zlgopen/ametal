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
 * \brief ZSN700 UART 用户配置文件
 * \sa am_zsn700l_hwconfig_uart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-19  zp, first implementation
 * \endinternal
 */

#include "am_gpio.h"
#include "am_zsn700_uart.h"
#include "hw/amhw_zsn700_uart.h"
#include "am_clk.h"
#include "am_zsn700.h"
#include "hw/amhw_zsn700_gpio.h"
#include "hw/amhw_zsn700_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_zsn700_uart
 * \copydoc am_hwconf_zsn700_uart.c
 * @{
 */
/** \brief 串口0平台初始化 */
static void __zsn700_plfm_uart0_init (void)
{
    am_clk_enable(CLK_UART0);

    am_gpio_pin_cfg(PIOA_9,  PIOA_9_UART0_TXD  | PIOA_9_OUT_PP );
    am_gpio_pin_cfg(PIOA_10, PIOA_10_UART0_RXD | PIOA_10_INPUT_FLOAT);
}

/** \brief 解除串口0平台初始化 */
static void __zsn700_plfm_uart0_deinit (void)
{
    am_clk_disable(CLK_UART0);

    am_gpio_pin_cfg(PIOA_9,  PIOA_9_GPIO  | PIOA_9_INPUT_PU);
    am_gpio_pin_cfg(PIOA_10, PIOA_10_GPIO | PIOA_10_INPUT_PU);
}

/** \brief 串口0设备信息 */
static const am_zsn700_uart_devinfo_t __g_uart0_devinfo = {

    ZSN700_UART0_BASE,            /**< \brief 串口0 */
    INUM_UART0_2,                 /**< \brief 串口0的中断编号 */
    AMHW_ZSN700_UART_WORK_MODE_1, /**< \brief 串口工作模式 */

    AMHW_ZSN700_UART_PARITY_NO |  /**< \brief 偶校验  MODE0/1:无需校验  MODE2/3:必须校验 */
    AMHW_ZSN700_UART_STOP_1_0_BIT,/**< \brief 1个停止位 */

    115200,                       /**< \brief 设置的波特率 */

    0,                            /**< \brief 无其他中断 */

    {
        AM_FALSE,                 /**< \brief 禁能多机地址自动识别 */
        0x00,                     /**< \brief 地址0x00 */
        0x00,                     /**< \brief 地址全部不关心 */
    },

    {
        AM_FALSE,                 /**< \brief 禁能流控 */
        0,                        /**< \brief CTS引脚编号 */
        0,                        /**< \brief RTS引脚编号*/
    },

    NULL,                         /**< \brief 使用RS485 */
    __zsn700_plfm_uart0_init,     /**< \brief UART0的平台初始化 */
    __zsn700_plfm_uart0_deinit,   /**< \brief UART0的平台去初始化 */
};

/* 串口中断源复用需要调用设备信息 */
am_zsn700_uart_dev_t  *__gp_uart0 = NULL;

/**< \brief 定义串口0 设备 */
static am_zsn700_uart_dev_t  __g_uart0_dev;

/** \brief UART0实例初始化，获得uart0标准服务句柄 */
am_uart_handle_t am_zsn700_uart0_inst_init (void)
{
    __gp_uart0 = &__g_uart0_dev;
    return am_zsn700_uart_init(&__g_uart0_dev, &__g_uart0_devinfo);
}

/** \brief UART0实例解初始化 */
void am_zsn700_uart0_inst_deinit (am_uart_handle_t handle)
{
    am_zsn700_uart_deinit((am_zsn700_uart_dev_t *)handle);
}

/** \brief uart1平台初始化 */
static void __zsn700_plfm_uart1_init (void)
{
    am_clk_enable(CLK_UART1);

    am_gpio_pin_cfg(PIOA_2, PIOA_2_UART1_TXD | PIOA_2_OUT_PP );
    am_gpio_pin_cfg(PIOA_3, PIOA_3_UART1_RXD | PIOA_3_INPUT_FLOAT);
}

/** \brief 解除串口1平台初始化 */
static void __zsn700_plfm_uart1_deinit (void)
{
    am_clk_disable(CLK_UART1);

    am_gpio_pin_cfg(PIOA_2, PIOA_2_GPIO | PIOA_2_INPUT_PU);
    am_gpio_pin_cfg(PIOA_3, PIOA_3_GPIO | PIOA_3_INPUT_PU);
}

/** \brief 串口1设备信息 */
static const am_zsn700_uart_devinfo_t __g_uart1_devinfo = {

    ZSN700_UART1_BASE,            /**< \brief 串口1 */
    INUM_UART1_3,                 /**< \brief 串口1的中断编号 */
    AMHW_ZSN700_UART_WORK_MODE_1, /**< \brief 串口工作模式 */

    AMHW_ZSN700_UART_PARITY_NO |  /**< \brief 无极性  MODE0/1:无需校验  MODE2/3:必须校验*/
    AMHW_ZSN700_UART_STOP_1_0_BIT,/**< \brief 1个停止位 */

    115200,                       /**< \brief 设置的波特率 */

    0,                            /**< \brief 无其他中断 */

    {
        AM_FALSE,                 /**< \brief 禁能多机地址自动识别 */
        0x00,                     /**< \brief 地址0x00 */
        0x00,                     /**< \brief 地址全部不关心 */
    },

    {
        AM_FALSE,                 /**< \brief 禁能流控 */
        0,                        /**< \brief CTS引脚编号 */
        0,                        /**< \brief RTS引脚编号*/
    },

    NULL,                         /**< \brief 使用RS485 */
    __zsn700_plfm_uart1_init,     /**< \brief UART1的平台初始化 */
    __zsn700_plfm_uart1_deinit,   /**< \brief UART1的平台去初始化 */
};

/* 串口中断源复用需要调用设备信息 */
am_zsn700_uart_dev_t  *__gp_uart1 = NULL;

/**< \brief 定义串口1 设备 */
static am_zsn700_uart_dev_t  __g_uart1_dev;

/** \brief UART1实例初始化，获得uart1标准服务句柄 */
am_uart_handle_t am_zsn700_uart1_inst_init (void)
{
    __gp_uart1 = &__g_uart1_dev;
    return am_zsn700_uart_init(&__g_uart1_dev, &__g_uart1_devinfo);
}

/** \brief UART1实例解初始化 */
void am_zsn700_uart1_inst_deinit (am_uart_handle_t handle)
{
    am_zsn700_uart_deinit((am_zsn700_uart_dev_t *)handle);
}

/** \brief 串口2平台初始化 */
static void __zsn700_plfm_uart2_init (void)
{
    am_clk_enable(CLK_UART2);

    am_gpio_pin_cfg(PIOC_3, PIOC_3_UART2_TXD | PIOC_3_OUT_PP );
    am_gpio_pin_cfg(PIOC_2, PIOC_2_UART2_RXD | PIOC_2_INPUT_FLOAT);
}

/** \brief 解除串口2平台初始化 */
static void __zsn700_plfm_uart2_deinit (void)
{
    am_clk_disable(CLK_UART2);

    am_gpio_pin_cfg(PIOC_3, PIOC_3_GPIO | PIOC_3_INPUT_PU);
    am_gpio_pin_cfg(PIOC_2, PIOC_2_GPIO | PIOC_2_INPUT_PU);
}

/** \brief 串口2设备信息 */
static const am_zsn700_uart_devinfo_t __g_uart2_devinfo = {

    ZSN700_UART2_BASE,            /**< \brief 串口2 */
    INUM_UART0_2,                 /**< \brief 串口2的中断编号 */
    AMHW_ZSN700_UART_WORK_MODE_1, /**< \brief 串口工作模式 */

    AMHW_ZSN700_UART_PARITY_NO |  /**< \brief 无极性  MODE0/1:无需校验  MODE2/3:必须校验*/
    AMHW_ZSN700_UART_STOP_1_0_BIT,/**< \brief 1个停止位 */

    115200,                       /**< \brief 设置的波特率 */

    0,                            /**< \brief 无其他中断 */

    {
        AM_FALSE,                 /**< \brief 禁能多机地址自动识别 */
        0x00,                     /**< \brief 地址0x00 */
        0x00,                     /**< \brief 地址全部不关心 */
    },

    {
        AM_FALSE,                 /**< \brief 禁能流控 */
        0,                        /**< \brief CTS引脚编号 */
        0,                        /**< \brief RTS引脚编号*/
    },

    NULL,                         /**< \brief 使用RS485 */
    __zsn700_plfm_uart2_init,     /**< \brief UART2的平台初始化 */
    __zsn700_plfm_uart2_deinit,   /**< \brief UART2的平台去初始化 */
};

/* 串口中断源复用需要调用设备信息 */
am_zsn700_uart_dev_t  *__gp_uart2 = NULL;

/**< \brief 定义串口2 设备 */
static am_zsn700_uart_dev_t  __g_uart2_dev;

/** \brief UART2实例初始化，获得uart2标准服务句柄 */
am_uart_handle_t am_zsn700_uart2_inst_init (void)
{
    __gp_uart2 = &__g_uart2_dev;
    return am_zsn700_uart_init(&__g_uart2_dev, &__g_uart2_devinfo);
}

/** \brief UART2实例解初始化 */
void am_zsn700_uart2_inst_deinit (am_uart_handle_t handle)
{
    am_zsn700_uart_deinit((am_zsn700_uart_dev_t *)handle);
}

/** \brief 串口3平台初始化 */
static void __zsn700_plfm_uart3_init (void)
{
    am_clk_enable(CLK_UART3);

    am_gpio_pin_cfg(PIOC_7, PIOC_7_UART3_TXD | PIOC_7_OUT_PP );
    am_gpio_pin_cfg(PIOC_6, PIOC_6_UART3_RXD | PIOC_6_INPUT_FLOAT);
}

/** \brief 解除串口3平台初始化 */
static void __zsn700_plfm_uart3_deinit (void)
{
    am_clk_disable(CLK_UART3);

    am_gpio_pin_cfg(PIOC_6, PIOC_6_GPIO | PIOC_6_INPUT_PU);
    am_gpio_pin_cfg(PIOC_7, PIOC_7_GPIO | PIOC_7_INPUT_PU);
}

/** \brief 串口3设备信息 */
static const am_zsn700_uart_devinfo_t __g_uart3_devinfo = {

    ZSN700_UART3_BASE,            /**< \brief 串口3 */
    INUM_UART1_3,                 /**< \brief 串口3的中断编号 */
    AMHW_ZSN700_UART_WORK_MODE_1, /**< \brief 串口工作模式 */

    AMHW_ZSN700_UART_PARITY_NO |  /**< \brief 无极性  MODE0/1:无需校验  MODE2/3:必须校验*/
    AMHW_ZSN700_UART_STOP_1_0_BIT,/**< \brief 1个停止位 */

    115200,                       /**< \brief 设置的波特率 */

    0,                            /**< \brief 无其他中断 */

    {
        AM_FALSE,                 /**< \brief 禁能多机地址自动识别 */
        0x00,                     /**< \brief 地址0x00 */
        0x00,                     /**< \brief 地址全部不关心 */
    },

    {
        AM_FALSE,                 /**< \brief 禁能流控 */
        0,                        /**< \brief CTS引脚编号 */
        0,                        /**< \brief RTS引脚编号*/
    },

    NULL,                         /**< \brief 使用RS485 */
    __zsn700_plfm_uart3_init,     /**< \brief UART3的平台初始化 */
    __zsn700_plfm_uart3_deinit,   /**< \brief UART3的平台去初始化 */
};

/* 串口中断源复用需要调用设备信息 */
am_zsn700_uart_dev_t  *__gp_uart3 = NULL;

/**< \brief 定义串口3 设备 */
static am_zsn700_uart_dev_t  __g_uart3_dev;

/** \brief UART3实例初始化，获得uart3标准服务句柄 */
am_uart_handle_t am_zsn700_uart3_inst_init (void)
{
    __gp_uart3 = &__g_uart3_dev;
    return am_zsn700_uart_init(&__g_uart3_dev, &__g_uart3_devinfo);
}

/** \brief UART3实例解初始化 */
void am_zsn700_uart3_inst_deinit (am_uart_handle_t handle)
{
    am_zsn700_uart_deinit((am_zsn700_uart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
