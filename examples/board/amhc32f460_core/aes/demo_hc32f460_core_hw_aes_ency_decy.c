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
 * \brief AES 加密解密例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   AES对数据加密然后解密，如果解密出来数据与加密前数据一样，则串口打印“Aes ency-decy success!\r\n”。
 *   反之打印"Aes ency-decy failed!\r\n"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *       PIOE_5 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f460_core_aes_ency_decy.c src_hc32f460_core_aes_ency_decy
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-26  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_core_hw_aes_ency_decy
 * \copydoc demo_hc32f460_core_hw_aes_ency_decy.c
 */

/** [src_hc32f460_core_aes_ency_decy] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_hc32f460_aes.h"
#include "am_vdebug.h"
#include "demo_hc32f460_entries.h"

/*******************************************************************************
 * 全局变量
 ******************************************************************************/
/**< \brief 存放待加密的数据*/
static uint32_t aes_data[4] = {0x33221100, 0x77665544, 0xBBAA9988, 0xFFEEDDCC};

/**< \brief AES密匙 */
static uint32_t aes_key[8]  = {0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C,\
                               0x13121110, 0x17161514, 0x1B1A1918, 0x1F1E1D1C};

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_aes_ency_decy_entry (void)
{
    AM_DBG_INFO("demo amhc32f460_core hw aes-ency-decy !\r\n");

    /* 开启AES时钟 */
    am_clk_enable (CLK_AES);

    /* 例程入口 */
    demo_hc32f460_hw_aes_ency_decy_entry(HC32F460_AES,
                                         aes_data,
                                         aes_key);
}

/* end of file */
