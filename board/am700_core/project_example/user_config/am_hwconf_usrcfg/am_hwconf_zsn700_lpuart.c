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
 * \brief ZSN700 LPUART 用户配置文件
 * \sa am_zsn700l_hwconfig_lpuart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

#include "am_gpio.h"
#include "am_zsn700_lpuart.h"
#include "hw/amhw_zsn700_lpuart.h"
#include "am_clk.h"
#include "am_zsn700.h"
#include "hw/amhw_zsn700_gpio.h"
#include "hw/amhw_zsn700_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_zsn700_lpuart
 * \copydoc am_hwconf_zsn700_lpuart.c
 * @{
 */
/** \brief LPUART0平台初始化 */
static void __zsn700_plfm_lpuart0_init (void)
{
    am_clk_enable(CLK_LPUART0);

    am_gpio_pin_cfg(PIOB_11, PIOB_11_LPUART0_RXD | PIOB_11_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOB_12, PIOB_12_LPUART0_TXD | PIOB_12_OUT_PP );
}

/** \brief 解除LPUART0平台初始化 */
static void __zsn700_plfm_lpuart0_deinit (void)
{
    am_clk_disable(CLK_LPUART0);

    am_gpio_pin_cfg(PIOB_11, PIOB_11_GPIO | PIOB_11_INPUT_PU);
    am_gpio_pin_cfg(PIOB_12, PIOB_12_GPIO | PIOB_12_INPUT_PU);
}

/** \brief LPUART0设备信息 */
static const am_zsn700_lpuart_devinfo_t __g_lpuart0_devinfo = {

    ZSN700_LPUART0_BASE,              /**< \brief LPUART0 */
    INUM_LPUART0,                     /**< \brief LPUART0的中断编号 */
    AMHW_ZSN700_LPUART_WORK_MODE_1,   /**< \brief LPUART工作模式 */
    AMHW_ZSN700_LPUART_SCLK_SRC_PCLK, /**< \brief 通信传输时钟  */
    AMHW_ZSN700_LPUART_PARITY_NO |    /**< \brief 无极性 */
    AMHW_ZSN700_LPUART_STOP_1_0_BIT,  /**< \brief 1个停止位 */

    115200,                           /**< \brief 设置的波特率 */

    0,                                /**< \brief 无其他中断 */

    {
        AM_FALSE,                     /**< \brief 禁能多机地址自动识别 */
        0x00,                         /**< \brief 地址0x00 */
        0x00,                         /**< \brief 地址全部不关心 */
    },

    {
        NULL,                         /**< \brief 硬件流控引脚初始化、解初始化函数 */
        0,                            /**< \brief CTS引脚编号 */
        0,                            /**< \brief RTS引脚编号*/
    },

    NULL,                             /**< \brief 使用RS485 */
    __zsn700_plfm_lpuart0_init,       /**< \brief LPUART0的平台初始化 */
    __zsn700_plfm_lpuart0_deinit,     /**< \brief LPUART0的平台去初始化 */
};

/**< \brief 定义LPUART0 设备 */
static am_zsn700_lpuart_dev_t  __g_lpuart0_dev;

/** \brief LPUART0实例初始化，获得lpuart0标准服务句柄 */
am_uart_handle_t am_zsn700_lpuart0_inst_init (void)
{
    return am_zsn700_lpuart_init(&__g_lpuart0_dev, &__g_lpuart0_devinfo);
}

/** \brief LPUART0实例解初始化 */
void am_zsn700_lpuart0_inst_deinit (am_uart_handle_t handle)
{
    am_zsn700_lpuart_deinit((am_zsn700_lpuart_dev_t *)handle);
}

/** \brief lpuart2平台初始化 */
static void __zsn700_plfm_lpuart1_init (void)
{
    am_clk_enable(CLK_LPUART1);

    am_gpio_pin_cfg(PIOC_11, PIOC_11_LPUART1_RXD | PIOC_11_INPUT_FLOAT );
    am_gpio_pin_cfg(PIOC_12, PIOC_12_LPUART1_TXD | PIOC_12_OUT_PP);
}

/** \brief 解除LPUART1平台初始化 */
static void __zsn700_plfm_lpuart1_deinit (void)
{
    am_clk_disable(CLK_LPUART1);

    am_gpio_pin_cfg(PIOC_11, PIOC_11_GPIO | PIOC_11_INPUT_PU);
    am_gpio_pin_cfg(PIOC_12, PIOC_12_GPIO | PIOC_12_INPUT_PU);
}

/** \brief LPUART1设备信息 */
static const am_zsn700_lpuart_devinfo_t __g_lpuart1_devinfo = {

    ZSN700_LPUART1_BASE,              /**< \brief LPUART1 */
    INUM_LPUART1,                     /**< \brief LPUART1的中断编号 */
    AMHW_ZSN700_LPUART_WORK_MODE_1,   /**< \brief LPUART工作模式 */
    AMHW_ZSN700_LPUART_SCLK_SRC_PCLK, /**< \brief 通信传输时钟  */
    AMHW_ZSN700_LPUART_PARITY_NO |    /**< \brief 无极性 */
    AMHW_ZSN700_LPUART_STOP_1_0_BIT,  /**< \brief 1个停止位 */

    115200,                           /**< \brief 设置的波特率 */

    0,                                /**< \brief 无其他中断 */

    {
        AM_FALSE,                     /**< \brief 禁能多机地址自动识别 */
        0x00,                         /**< \brief 地址0x00 */
        0x00,                         /**< \brief 地址全部不关心 */
    },

    {
        NULL,                         /**< \brief 硬件流控引脚初始化、解初始化函数 */
        0,                            /**< \brief CTS引脚编号 */
        0,                            /**< \brief RTS引脚编号*/
    },

    NULL,                             /**< \brief 使用RS485 */
    __zsn700_plfm_lpuart1_init,       /**< \brief LPUART1的平台初始化 */
    __zsn700_plfm_lpuart1_deinit,     /**< \brief LPUART1的平台去初始化 */
};

/**< \brief 定义LPUART1 设备 */
static am_zsn700_lpuart_dev_t  __g_lpuart1_dev;

/** \brief LPUART1实例初始化，获得lpuart1标准服务句柄 */
am_uart_handle_t am_zsn700_lpuart1_inst_init (void)
{
    return am_zsn700_lpuart_init(&__g_lpuart1_dev, &__g_lpuart1_devinfo);
}

/** \brief LPUART1实例解初始化 */
void am_zsn700_lpuart1_inst_deinit (am_uart_handle_t handle)
{
    am_zsn700_lpuart_deinit((am_zsn700_lpuart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
