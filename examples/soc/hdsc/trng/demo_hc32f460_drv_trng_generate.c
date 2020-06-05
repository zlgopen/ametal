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
 *   “Trng generate test success!\r\n”,反之打印 “Trng generate test failed!\r\n”。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *       PIOE_5 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_trng_dac.c src_hc32f460_hw_trng_generate
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-26  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_trng_generate
 * \copydoc demo_hc32f460_hw_trng_generate.c
 */

/** [src_hc32f460_hw_trng_generate] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_hc32f460_trng.h"
#include "hw/amhw_hc32f460_trng.h"

/*******************************************************************************
  全局变量
*******************************************************************************/

/**< \brief 随机数缓存区 */
static uint32_t rng_data[2] = {0};


/**
 * \brief 例程入口
 */
void demo_hc32f460_drv_trng_generate_entry (am_trng_handle_t handle)
{

    while(1) {
        am_hc32f460_trng_generate(handle, AM_HC32F460_TRNG_LOAD_NEW_INIT_VALUE, rng_data);

        am_kprintf ("Trng generate tset success!\r\n");
        am_kprintf ("Rand number: 0x%08x 0x%08x.\r\n", rng_data[0], rng_data[1]);

        am_mdelay(1000);
    }
}

/* end of file */

