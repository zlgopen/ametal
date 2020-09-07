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
 * \brief CLK 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印各总线频率(HCLK:48MHz, PCLK:24MHz,)
 *   2. PIOA_1引脚输出HCLK的8分频的频率信号，即6MHz
 *      PIOA_2引脚输出PCLK的8分频的频率信号，即3MHz
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_clk.c src_zsn700_hw_clk
 *
 * \internal
 * \par Modification History
 * - 1.00 15-7-13  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_clk
 * \copydoc demo_zsn700_hw_clk.c
 */

/** [src_zsn700_hw_clk] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_zsn700.h"
#include "hw/amhw_zsn700_rcc.h"
#include "am_zsn700_inst_init.h"
#include "demo_zlg_entries.h"
#include "demo_am700_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsn700_core_hw_clk_entry (void)
{
    am_clk_id_t clk_id[] = {CLK_HCLK, CLK_PCLK};

    AM_DBG_INFO("demo am700_core hw clk!\r\n");

    amhw_zsn700_gpio_sup_hclk_div_set(ZSN700_GPIO0,
                                      AMHW_ZSN700_GPIO_SUP_CLK_DIV_8);
    amhw_zsn700_gpio_sup_hclk_output(ZSN700_GPIO0);
    am_gpio_pin_cfg(PIOA_1, PIOA_1_HCLK_OUT | PIOA_1_OUT_PP);

    amhw_zsn700_gpio_sup_pclk_div_set(ZSN700_GPIO0,
                                      AMHW_ZSN700_GPIO_SUP_CLK_DIV_8);
    amhw_zsn700_gpio_sup_pclk_output(ZSN700_GPIO0);
    am_gpio_pin_cfg(PIOA_2, PIOA_2_PCLK_OUT | PIOA_2_OUT_PP);

    demo_zlg_hw_clk_entry(&clk_id[0], AM_NELEMENTS(clk_id));
}
/** [src_zsn700_hw_clk] */

/* end of file */
