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
 * \brief RAM 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 写入成功：串口打印256个十六进制数据。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f07x_hw_ram.c src_hc32f07x_hw_ram
 *
 * \internal
 * \par Modification History
 * - 1.00 15-7-13  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f07x_hw_ram
 * \copydoc demo_hc32f07x_hw_ram.c
 */

/** [src_hc32f07x_hw_clk] */
#include "hw/amhw_hc32_ram.h"
#include "am_hc32_ram.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_hc32f07x.h"
#include "am_clk.h"
#include "demo_amf07x_core_entries.h"

#define START_ADDR    0X20002000
#define SIZE          0X100
amhw_hc32_ram_t *p_hw_ram = (amhw_hc32_ram_t *)HC32_RAM_BASE;

/**
 * \brief 例程入口
 */
void demo_hc32f07x_core_hw_ram_entry (void)
{
    int    i = 0;
    static uint8_t data[SIZE] = {0}; /* 要写入 RAM 的数据 */
    static uint8_t temp[SIZE] = {0}; /* 从 RAM 中读出的数据 */

    /* 数据初始化 */
    for (i = 0; i < SIZE; i++) {
        data[i] = i;
    }

    /* RAM控制器初始化 */
    am_hc32_ram_init(p_hw_ram);

    /* RAM擦除 */
    am_hc32_ram_erase (p_hw_ram,
                       START_ADDR,
                       SIZE);

    /* RAM写入 */
    am_hc32_ram_write (p_hw_ram,
                       START_ADDR,
                       data,
                       SIZE);

    /* RAM读取 */
    am_hc32_ram_read (p_hw_ram,
                      START_ADDR,
                      temp,
                      SIZE);

    for(i = 0; i < SIZE; i++) {
        AM_DBG_INFO("%04d  ", temp[i]);
            if(data[i] != temp[i]) {
                AM_DBG_INFO("\r\nverify error!\r\n");
                AM_FOREVER;
            }
    }
    AM_DBG_INFO("\r\nram test success!\r\n");

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f07x_hw_ram] */

/* end of file */
