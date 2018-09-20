/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief MiniPort-595 用户配置文件
 * \sa am_hwconf_miniport_595.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc595_spi.h"
#include "am_hc595_gpio.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_595
 * \copydoc am_hwconf_miniport_595.c
 * @{
 */

/** \brief 是否使用 GPIO 模拟 SPI 0: 使用硬件 SPI  1: 使用 GPIO 模拟 SPI */
#define    __USE_GPIO_SPI    0

#if (__USE_GPIO_SPI == 1)

/** \brief MiniPort-595 GPIO 设备信息 */
am_local am_const am_hc595_gpio_info_t __g_miniport_595_gpio_devinfo = {
    PIO0_10,    /* 数据引脚 (SPI_MOSI) */
    PIO0_11,    /* 移位时钟引脚 (SPI_CLK) */
    PIO0_14,    /* 数据锁存引脚 (SPI_CS) */
    -1,         /* OE 固定为低电平，未使用 */
    AM_TRUE     /* 低位先发送 */
};

/** \brief MiniPort-595 GPIO 设备实例 */
am_local am_hc595_gpio_dev_t __g_miniport_595_gpio_dev;
#else

/** \brief MiniPort-595 SPI 设备信息 */
am_local  am_hc595_spi_info_t __g_miniport_595_spi_devinfo = {
    PIO0_14,    /* 数据锁存引脚 */
    -1,         /* 未使用 OE 引脚 */
    300000,     /* 时钟频率 300KHz */
    AM_TRUE     /* 数据低位先发送 */
};

/** \brief MiniPort-595 SPI 设备实例 */
am_local am_hc595_spi_dev_t __g_miniport_595_spi_dev;
#endif /* (__USE_GPIO_SPI != 1) */

/**
 * \brief MiniPort-595 实例初始化
 */
am_hc595_handle_t am_miniport_595_inst_init (void)
{
#if (__USE_GPIO_SPI == 1)
    return am_hc595_gpio_init(&__g_miniport_595_gpio_dev,
                              &__g_miniport_595_gpio_devinfo);
#else
    return am_hc595_spi_init(&__g_miniport_595_spi_dev,
                             &__g_miniport_595_spi_devinfo,
                              am_lpc82x_spi1_dma_inst_init());
#endif
}

/**
 * @}
 */

/* end of file */
