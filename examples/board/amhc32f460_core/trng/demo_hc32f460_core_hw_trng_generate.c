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
 *   “Trng generate test success!\r\n”并打印产生的随机数,
 *   反之打印 “Trng generate test failed!\r\n”。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *       PIOE_5 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f460_core_trng_dac.c src_hc32f460_core_trng_generate
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-26  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_core_trng_generate
 * \copydoc demo_hc32f460_core_trng_generate.c
 */

/** [src_hc32f460_core_trng_generate] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32f460_trng.h"
#include "demo_hc32f460_entries.h"
/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_trng_generate_entry (void)
{

    AM_DBG_INFO("demo amhc32f460_core hw trng generate test!\r\n");

    /* 开启trng时钟 */
    am_clk_enable (CLK_TRNG);

    demo_hc32f460_hw_trng_generate_entry(HC32F460_TRNG);
}

/* end of file */
