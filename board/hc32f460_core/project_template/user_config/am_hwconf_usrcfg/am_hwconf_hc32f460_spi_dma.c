/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg118.cn/
*******************************************************************************/

/**
 * \file
 * \brief HC32F460 SPI DMA 用户配置文件
 * \sa am_hwconf_hc32f460_spi.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-30  cds, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460_spi_dma.h"
#include "am_gpio.h"
#include "hw/amhw_hc32f460_spi.h"
#include "am_clk.h"
#include "am_hc32f460.h"
#include "hc32f460_dma_chan.h"
#include "am_hc32f460_spi_dma.h"
/**
 * \addtogroup am_if_src_hwconf_hc32f460_spi_dma
 * \copydoc am_hwconf_hc32f460_spi_dma.c
 * @{
 */
/*********************************SPI3*****************************************/
/* SPI3 引脚，用户需根据自己实际使用引脚修改*/
#define __SPI3_PIN_SCK    PIOE_0
#define __SPI3_PIN_MOSI   PIOE_2
#define __SPI3_PIN_MISO   PIOE_3
/** \brief SPI3 平台初始化 */
static void __hc32f460_plfm_spi3_dma_init (void)
{
    am_gpio_pin_cfg(__SPI3_PIN_SCK,   GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_SCK));
    am_gpio_pin_cfg(__SPI3_PIN_MOSI,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MOSI));
    am_gpio_pin_cfg(__SPI3_PIN_MISO,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MISO));

    am_clk_enable(CLK_SPI3);
    am_clk_enable(CLK_PTDIS);
}

/** \brief 解除SPI3 平台初始化 */
static void __hc32f460_plfm_spi3_dma_deinit (void)
{
    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(__SPI3_PIN_SCK,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI3_PIN_MOSI,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI3_PIN_MISO,  GPIO_AFIO(AM_GPIO_INPUT));

    am_clk_disable(CLK_SPI3);

}
extern am_hc32f460_dma_dev_t __g_dma1_dev;
/** \brief SPI3 设备信息 */
static const  struct am_hc32f460_spi_dma_devinfo  __g_spi3_dma_devinfo = {
    HC32F460_SPI3_BASE,                      /**< \brief SPI3寄存器指针 */
    CLK_SPI3,                                /**< \brief 时钟ID号 */
    3,
    INUM_SPI3,                               /**< \brief SPI3中断号 */
    &__g_dma1_dev,
    DMA_CHAN_2,
    DMA_CHAN_1,
    __hc32f460_plfm_spi3_dma_init,           /**< \brief SPI3平台初始化函数 */
    __hc32f460_plfm_spi3_dma_deinit          /**< \brief SPI3平台解初始化函数 */
};
/** \brief SPI3 设备实例 */
static am_hc32f460_spi_dma_dev_t __g_spi3_dma_dev;


/** \brief SPI3 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_hc32f460_spi3_dma_inst_init (void)
{
    return am_hc32f460_spi_dma_init(&__g_spi3_dma_dev, &__g_spi3_dma_devinfo);
}

/** \brief SPI3 实例解初始化 */
void am_hc32f460_spi3_dma_inst_deinit (am_spi_handle_t handle)
{
    am_hc32f460_spi_dma_deinit(handle);
}

/*********************************SPI4*****************************************/
/* SPI4 引脚，用户需根据自己实际使用引脚修改*/
#define __SPI4_PIN_SCK    PIOE_0
#define __SPI4_PIN_MOSI   PIOE_2
#define __SPI4_PIN_MISO   PIOE_3
/** \brief SPI4 平台初始化 */
static void __hc32f460_plfm_spi4_dma_init (void)
{
    am_gpio_pin_cfg(__SPI4_PIN_SCK,   GPIO_AFIO(AMHW_HC32F460_AFIO_SPI4_SCK));
    am_gpio_pin_cfg(__SPI4_PIN_MOSI,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI4_MOSI));
    am_gpio_pin_cfg(__SPI4_PIN_MISO,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI4_MISO));

    am_clk_enable(CLK_SPI4);
    am_clk_enable(CLK_PTDIS);
}

/** \brief 解除SPI4 平台初始化 */
static void __hc32f460_plfm_spi4_dma_deinit (void)
{
    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(__SPI4_PIN_SCK,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI4_PIN_MOSI,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI4_PIN_MISO,  GPIO_AFIO(AM_GPIO_INPUT));

    am_clk_disable(CLK_SPI4);

}
extern am_hc32f460_dma_dev_t __g_dma1_dev;
/** \brief SPI4 设备信息 */
static const  struct am_hc32f460_spi_dma_devinfo  __g_spi4_dma_devinfo = {
    HC32F460_SPI4_BASE,                      /**< \brief SPI4寄存器指针 */
    CLK_SPI4,                                /**< \brief 时钟ID号 */
    4,
    INUM_SPI4,                               /**< \brief SPI4中断号 */
    &__g_dma1_dev,
    DMA_CHAN_2,
    DMA_CHAN_1,
    __hc32f460_plfm_spi4_dma_init,           /**< \brief SPI4平台初始化函数 */
    __hc32f460_plfm_spi4_dma_deinit          /**< \brief SPI4平台解初始化函数 */
};
/** \brief SPI4 设备实例 */
static am_hc32f460_spi_dma_dev_t __g_spi4_dma_dev;


/** \brief SPI4 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_hc32f460_spi4_dma_inst_init (void)
{
    return am_hc32f460_spi_dma_init(&__g_spi4_dma_dev, &__g_spi4_dma_devinfo);
}

/** \brief SPI4 实例解初始化 */
void am_hc32f460_spi4_dma_inst_deinit (am_spi_handle_t handle)
{
    am_hc32f460_spi_dma_deinit(handle);
}

/*********************************SPI2*****************************************/
/* SPI2 引脚，用户需根据自己实际使用引脚修改*/
#define __SPI2_PIN_SCK    PIOC_0
#define __SPI2_PIN_MOSI   PIOC_2
#define __SPI2_PIN_MISO   PIOC_3
/** \brief SPI2 平台初始化 */
static void __hc32f460_plfm_spi2_dma_init (void)
{
    am_gpio_pin_cfg(__SPI2_PIN_SCK,   GPIO_AFIO(AMHW_HC32F460_AFIO_SPI2_SCK));
    am_gpio_pin_cfg(__SPI2_PIN_MOSI,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI2_MOSI));
    am_gpio_pin_cfg(__SPI2_PIN_MISO,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI2_MISO));

    am_clk_enable(CLK_SPI2);
    am_clk_enable(CLK_PTDIS);
}

/** \brief 解除SPI2 平台初始化 */
static void __hc32f460_plfm_spi2_dma_deinit (void)
{
    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(__SPI2_PIN_SCK,   GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI2_PIN_MOSI,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI2_PIN_MISO,  GPIO_AFIO(AM_GPIO_INPUT));

    am_clk_disable(CLK_SPI2);

}
extern am_hc32f460_dma_dev_t __g_dma1_dev;
/** \brief SPI2 设备信息 */
static const  struct am_hc32f460_spi_dma_devinfo  __g_spi2_dma_devinfo = {
    HC32F460_SPI2_BASE,                      /**< \brief SPI2寄存器指针 */
    CLK_SPI2,                                /**< \brief 时钟ID号 */
    2,
    INUM_SPI2,                               /**< \brief SPI2中断号 */
    &__g_dma1_dev,
    DMA_CHAN_2,
    DMA_CHAN_1,
    __hc32f460_plfm_spi2_dma_init,           /**< \brief SPI2平台初始化函数 */
    __hc32f460_plfm_spi2_dma_deinit          /**< \brief SPI2平台解初始化函数 */
};
/** \brief SPI2 设备实例 */
static am_hc32f460_spi_dma_dev_t __g_spi2_dma_dev;


/** \brief SPI2 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_hc32f460_spi2_dma_inst_init (void)
{
    return am_hc32f460_spi_dma_init(&__g_spi2_dma_dev, &__g_spi2_dma_devinfo);
}

/** \brief SPI2 实例解初始化 */
void am_hc32f460_spi2_dma_inst_deinit (am_spi_handle_t handle)
{
    am_hc32f460_spi_dma_deinit(handle);
}

/*********************************SPI1*****************************************/
/* SPI1 引脚，用户需根据自己实际使用引脚修改*/
#define __SPI1_PIN_SCK    PIOC_0
#define __SPI1_PIN_MOSI   PIOC_2
#define __SPI1_PIN_MISO   PIOC_3
/** \brief SPI1 平台初始化 */
static void __hc32f460_plfm_spi1_dma_init (void)
{
    am_gpio_pin_cfg(__SPI1_PIN_SCK,   GPIO_AFIO(AMHW_HC32F460_AFIO_SPI1_SCK));
    am_gpio_pin_cfg(__SPI1_PIN_MOSI,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI1_MOSI));
    am_gpio_pin_cfg(__SPI1_PIN_MISO,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI1_MISO));

    am_clk_enable(CLK_SPI1);
    am_clk_enable(CLK_PTDIS);
}

/** \brief 解除SPI1 平台初始化 */
static void __hc32f460_plfm_spi1_dma_deinit (void)
{
    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(__SPI1_PIN_SCK,   GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI1_PIN_MOSI,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(__SPI1_PIN_MISO,  GPIO_AFIO(AM_GPIO_INPUT));

    am_clk_disable(CLK_SPI1);

}
extern am_hc32f460_dma_dev_t __g_dma1_dev;
/** \brief SPI1 设备信息 */
static const  struct am_hc32f460_spi_dma_devinfo  __g_spi1_dma_devinfo = {
    HC32F460_SPI1_BASE,                      /**< \brief SPI1寄存器指针 */
    CLK_SPI1,                                /**< \brief 时钟ID号 */
    1,
    INUM_SPI1,                               /**< \brief SPI1中断号 */
    &__g_dma1_dev,
    DMA_CHAN_2,
    DMA_CHAN_1,
    __hc32f460_plfm_spi1_dma_init,           /**< \brief SPI1平台初始化函数 */
    __hc32f460_plfm_spi1_dma_deinit          /**< \brief SPI1平台解初始化函数 */
};
/** \brief SPI1 设备实例 */
static am_hc32f460_spi_dma_dev_t __g_spi1_dma_dev;


/** \brief SPI1 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_hc32f460_spi1_dma_inst_init (void)
{
    return am_hc32f460_spi_dma_init(&__g_spi1_dma_dev, &__g_spi1_dma_devinfo);
}

/** \brief SPI1 实例解初始化 */
void am_hc32f460_spi1_dma_inst_deinit (am_spi_handle_t handle)
{
    am_hc32f460_spi_dma_deinit(handle);
}

/**
 * @}
 */

/* end of file */
