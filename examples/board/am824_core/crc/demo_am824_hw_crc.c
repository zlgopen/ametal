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
 * \brief CRC 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印 "CRC-16 result is     : 0xbb3d\r\n"；
 *   2. 串口打印 "CRC-CCITT result is  : 0x29b1\r\n"；
 *   3. 串口打印 "CRC-32 result is     : 0xcbf43926\r\n"。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_hw_crc.c src_am824_hw_crc
 *
 * \internal
 * \par Modification History
 * - 1.00 15-7-13  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_crc
 * \copydoc demo_am824_hw_crc.c
 */

/** [src_am824_hw_crc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x.h"
#include "am_lpc82x_clk.h"
#include "hw/amhw_lpc_crc.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/** \brief CRC 测试数据 */
am_local char __g_data_test[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

/** \brief CRC 测试数据的长度 */
am_local uint32_t __g_length_test = AM_NELEMENTS(__g_data_test);

/**
 * \brief 例程入口
 */
void demo_am824_hw_crc_entry (void)
{

    am_kprintf("demo am824_core hw crc!\r\n");

    /* 使能并复位 CRC 时钟 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_CRC);

    demo_lpc_hw_crc_entry(LPC82X_CRC, 
                          __g_data_test, 
                          __g_length_test);
}
/** [src_am824_hw_crc] */

/* end of file */
