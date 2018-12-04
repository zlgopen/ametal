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
 * \brief I2S 发送（DMA方式）例程，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. 按照I2S 相关引脚接线TX_D0、TX_FS、TX_BCLK到逻辑分析仪。
 *   2. 调试程序。
 *
 * - 实验现象：
 *   1. 可以在逻辑分析仪上看到TX_D0、TX_FS、TX_BCLK的数据；
 *   2. 其中TX_D0为发送数据的补码，TX_FS是帧时钟，TX_BCLK是位时钟。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_i2s_dma.c src_amks16z_core_hw_i2s_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-29 mkr , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_i2s_dma
 * \copydoc demo_amks16z_core_hw_i2s_dma.c
 */

/** [src_amks16z_core_hw_i2s_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_kl26_inst_init.h"
#include "am_int.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief i2s dma方式数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_amks16z_core_hw_i2s_dma_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw i2s dma!\r\n");

    /* 配置I2S 功能引脚 */
    am_gpio_pin_cfg(PIOC_1, PIOC_1_I2S0_TXD0);     /* TX_D0   */
    am_gpio_pin_cfg(PIOC_2, PIOC_2_I2S0_TX_FS);    /* TX_FS   */
    am_gpio_pin_cfg(PIOC_3, PIOC_3_I2S0_TX_BCLK);  /* TX_BCLK */
    am_gpio_pin_cfg(PIOC_4, PIOC_4_I2S0_MCLK);     /* MCLK    */
    am_gpio_pin_cfg(PIOC_5, PIOC_5_I2S0_RXD0);     /* RX_D0   */
    am_gpio_pin_cfg(PIOC_6, PIOC_6_I2S0_RX_BCLK);  /* RX_BCLK */
    am_gpio_pin_cfg(PIOC_7, PIOC_7_I2S0_RX_FS);    /* RX_FS   */

    demo_kl26_hw_i2s_dma_entry();
}
/** [src_amks16z_core_hw_i2s_dma] */

/* end of file */
