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
 * \brief HC32F460 QSPI 用户配置文件
 * \sa am_hc32f460_hwconfig_qspi.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-18  cds, first implementation
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32f460.h"
#include "am_hc32f460_qspi.h"
#include "hw/amhw_hc32f460_qspi.h"


/**
 * \addtogroup am_if_src_hwconf_hc32f460_uart
 * \copydoc am_hwconf_hc32f460_uart.c
 * @{
 */

/** \brief QSPI平台初始化 */
static void __hc32f460_plfm_qspi_init(void)
{
    am_clk_enable(CLK_QSPI);
    am_gpio_pin_cfg(PIOC_6, GPIO_AFIO(AMHW_HC32F460_AFIO_QSPI));
    am_gpio_pin_cfg(PIOC_7, GPIO_AFIO(AMHW_HC32F460_AFIO_QSPI));
    am_gpio_pin_cfg(PIOD_8, GPIO_AFIO(AMHW_HC32F460_AFIO_QSPI));
    am_gpio_pin_cfg(PIOD_9, GPIO_AFIO(AMHW_HC32F460_AFIO_QSPI));
}

/** \brief QSPI平台解初始化 */
static void __hc32f460_plfm_qspi_deinit(void)
{
    am_clk_disable(CLK_QSPI);
    am_gpio_pin_cfg(PIOC_6, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOC_7, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOD_8, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOD_9, AM_GPIO_INPUT);
}

/** \brief QSPI设备信息 */
static const am_hc32f460_qspi_devinfo_t __g_qspi_devinfo = {


    HC32F460_QSPI_BASE,                    /**< \brief QSPI */
    CLK_QSPI,                              /**< \brief 时钟编号 */
    INUM_QSPI,                             /**< \brief 中断编号 */

    AMHW_HC32F460_QSPI_HCLK_DIV4,          /**< \brief 基准时钟为4个HCLK时钟 */
    AMHW_HC32F460_QSPI_PROTOCOL_EXTEND,    /**< \brief 数据接收阶段采用扩展式SPI协议 */
    AMHW_HC32F460_QSPI_PROTOCOL_EXTEND,    /**< \brief 地址发送阶段采用扩展式SPI协议 */
    AMHW_HC32F460_QSPI_PROTOCOL_EXTEND,    /**< \brief 指令发送阶段采用扩展式SPI协议 */
    AMHW_HC32F460_QSPI_SPI_MODE0,          /**< \brief SPI 协议采用模式0 */
    AMHW_HC32F460_QSPI_READ_MODE_STANDARD, /**< \brief 标准读 */

    __hc32f460_plfm_qspi_init,             /**< \brief 平台初始化函数 */
    __hc32f460_plfm_qspi_deinit            /**< \brief 平台解初始化函数 */

};

/**< \brief 定义QSPI设备 */
am_hc32f460_qspi_dev_t __g_qspi_dev;


/** \brief QSPI实例初始化 */
am_qspi_handle_t am_hc32f460_qspi_inst_init(void)
{
    return am_hc32f460_qspi_init(&__g_qspi_dev, &__g_qspi_devinfo);
}

/** \brief QSPI实例解初始化 */
void am_hc32f460_qspi_inst_deinit(am_qspi_handle_t handle)
{
    am_hc32f460_qspi_deinit(handle);
}


/**
 * @}
 */

/* end of file */
