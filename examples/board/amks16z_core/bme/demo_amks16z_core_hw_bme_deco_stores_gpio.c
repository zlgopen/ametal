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
 * \brief  BME封装式存储指令实现例子
 *
 * - 操作：
 *   1.PIOA_1 引脚连接PC串口的TXD;
 *   2.PIOA_2 引脚连接PC串口的RXD;
 *   3.配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 * -实现现象：串口依次打印出以下字符串，用于C语言实现与BME指令执行效率上的差别，
 *         可以看出BME指令实现的执行效率更高效
 *
 *         systick start value: 0xff9d6a
 *
 *         systick end value: 0xff9d63
 *
 *         systick current value read overhead: 0x7
 *
 *         systick start value: 0xff9cb3
 *
 *         systick end value: 0xff9c9b
 *
 *         actual execution cycle for xor operation with normal C code: 0x11
 *
 *         systick start value: 0xff9cb0
 *
 *         systick end value: 0xff9c9f
 *
 *         actual execution cycle for xor operation with BME macros: 0xa
 *
 *         systick start value: 0xff9d57
 *
 *         systick end value: 0xff9d22
 *
 *         actual execution cycle for BFI operation with normal C: 0x2e
 *
 *         systick start value: 0xff9d19
 *
 *         systick end value: 0xff9d03
 *
 *         actual execution cycle for BFI operation with BME macro: 0xf
 *
 * \note BME 指令操作GPIO模块的，应该使用KL26_GPIO_BASE的别名地址为 0x4000_F000
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_bme_deco_stores_gpio.c src_amks16z_core_hw_bme_deco_stores_gpio
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-18  nwt, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_hw_bme_deco_stores_gpio
 * \copydoc demo_amks16z_core_hw_bme_deco_stores_gpio.c
 */
 
/** [src_amks16z_core_hw_bme_deco_stores_gpio] */
#include "ametal.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_bme_deco_stores_gpio_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw bme deco stores gpio!\r\n");

    demo_kl26_hw_bme_deco_stores_gpio_entry();
}

/** [src_amks16z_core_hw_bme_deco_stores_gpio] */
 
/* end of file */
