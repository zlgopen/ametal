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
 * \brief SPI主机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI的PIOC_6和PIOC_7用线相连，模拟从机设备，回环测试；
 *
 * - 实验现象:
 *   1. 主机通过MOSI发送数据，发出的数据从MOSI读回；
 *   2. 比较发出和读回的数据，如果相同LED0一直常亮,否则以200ms时间间隔闪烁；
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_spi_master.c src_amks16z_core_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 16-09-27  sdy, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_amks16z_core_if_hw_spi_master
 * \copydoc demo_amks16z_core_hw_spi_master.c
 */
 
/** [src_amks16z_core_hw_spi_master] */
#include "ametal.h"
#include "am_kl26_inst_init.h"
#include "hw/amhw_fsl_spi.h"
#include "hw/amhw_kl26_sim.h"
#include "am_kl26_clk.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"
#include "demo_amks16z_core_all_entries.h"

#define SPI_CS_PIN          PIOC_4     /**< \brief 片选引脚             */

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_spi_master_entry (void)
{
    uint32_t spi_freq;

    AM_DBG_INFO("demo amks16z_core hw spi master!\r\n");

    /* 开启时钟 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_SPI0);

    /* 配置引脚 */
    am_gpio_pin_cfg(SPI_CS_PIN, AM_GPIO_OUTPUT_INIT_HIGH | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOC_5, PIOC_5_SPI0_SCK  | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOC_6, PIOC_6_SPI0_MISO | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOC_7, PIOC_7_SPI0_MOSI | AM_GPIO_PULLUP);

    spi_freq = am_kl26_clk_periph_rate_get(KL26_SPI0);

    demo_fsl_hw_spi_master_entry(KL26_SPI0, SPI_CS_PIN, spi_freq);
}

/** [src_amks16z_core_hw_spi_master] */

/* end of file */
