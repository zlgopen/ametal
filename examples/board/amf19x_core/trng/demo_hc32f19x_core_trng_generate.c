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
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   TRNG 模块上电产生一次随机数与非上电产生随机数对比，如果不同则TRNG随机数产生正确，打印
 *   “Trng generate tset success!\r\n”,反之打印 “Trng generate tset failed!\r\n”。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f19x_core_trng_dac.c src_hc32f19x_core_trng_generate
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-12  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f19x_core_trng_generate
 * \copydoc demo_hc32f19x_core_trng_generate.c
 */

/** [src_hc32f19x_core_trng_generate] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32_pin.h"
#include "am_hc32_trng.h"
#include "demo_hc32_entries.h"
/**
 * \brief 例程入口
 */
void demo_hc32f19x_core_hw_trng_generate_entry (void)
{

    AM_DBG_INFO("demo amf19x_core hw trng generate test!\r\n");

    /* 开启trng时钟 */
    am_clk_enable (CLK_RNG);

    demo_hc32_hw_trng_generate_entry(HC32_TRNG);
}

/* end of file */
