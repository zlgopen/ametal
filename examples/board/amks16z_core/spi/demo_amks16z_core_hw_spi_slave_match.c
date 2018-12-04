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
 * \brief SPI从机匹配中断演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI从机接口和主机接口对应连接起来(PIOC_4 == PIOD_4, PIOC_5 == PIOD_5,
 *      PIOC_6 == PIOD_6, PIOC_7 == PIOD_7)；；
 *
 * - 实验现象:
 *   1. SPI0从机接收SPI1主机数据，如果数据被匹配，则发送从机特征码给主机；
 *   2. SPI1主机检验收到的特征码是否正确，如果正确点亮LED，否则LED闪烁报警。
 *
 * \par 源代码
 * \snippet demo_kl26_hw_spi_slave_match.c src_kl26_hw_spi_slave_match
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_kl26_if_hw_spi_slave_match
 * \copydoc demo_kl26_hw_spi_slave_match.c
 */
 
/** [src_kl26_hw_spi_slave_match] */
#include "ametal.h"
#include "am_int.h"
#include "am_kl26_inst_init.h"
#include "hw/amhw_fsl_spi.h"
#include "hw/amhw_kl26_sim.h"
#include "am_kl26_clk.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/kl26_inum.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief SPI0从机引脚初始化
 */
static void __spi0_pin_init (void)
{
    /* 必须使用片选,低电平有效 */
    am_gpio_pin_cfg(PIOC_4, PIOC_4_SPI0_PCS0);
    am_gpio_pin_cfg(PIOC_5, PIOC_5_SPI0_SCK );
    am_gpio_pin_cfg(PIOC_6, PIOC_6_SPI0_MOSI);
    am_gpio_pin_cfg(PIOC_7, PIOC_7_SPI0_MISO);
}

/**
 * \brief SPI1主机的引脚初始化
 */
static void __spi1_pin_init (void)
{
    am_gpio_pin_cfg(PIOD_4, PIOD_4_SPI1_PCS0);
    am_gpio_pin_cfg(PIOD_5, PIOD_5_SPI1_SCK  | AM_GPIO_PULLUP );
    am_gpio_pin_cfg(PIOD_6, PIOD_6_SPI1_MOSI | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOD_7, PIOD_7_SPI1_MISO | AM_GPIO_PULLUP);
}

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_spi_slave_match_entry (void)
{
    uint32_t spi_freq;
    __spi0_pin_init();
    __spi1_pin_init();

    /* 开启SPI时钟 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_SPI0);
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_SPI1);

    spi_freq = am_kl26_clk_periph_rate_get(KL26_SPI1);

    demo_fsl_hw_spi_slave_match_entry(KL26_SPI0,
                                      KL26_SPI1,
                                      INUM_SPI0,
                                      spi_freq);
}

/** [src_kl26_hw_spi_slave_match] */

/* end of file */
