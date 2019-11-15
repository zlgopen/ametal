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
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32l13x_core_aes_ency_decy.c src_hc32l13x_core_aes_ency_decy
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-09  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l13x_core_aes_ency_decy
 * \copydoc demo_hc32l13x_core_aes_ency_decy.c
 */

/** [src_hc32l13x_core_aes_ency_decy] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "am_hc32_aes.h"
#include "demo_hc32_entries.h"

/**
 * \brief 密钥长度
 */
#define AES_KEY_LEN    AM_HC32_AES_KEY_LEN_128_BIT

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
void demo_hc32l13x_core_hw_aes_ency_decy_entry (void)
{
    AM_DBG_INFO("Test start: demo-aes-ency-decy test !\r\n");

    /* 开启AES时钟 */
    am_clk_enable (CLK_AES);

    /* 例程入口 */
    demo_hc32_hw_aes_ency_decy_entry(HC32_AES,
                                       AES_KEY_LEN,
                                       aes_data,
                                       aes_key);
}

/* end of file */
