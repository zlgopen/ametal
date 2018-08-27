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
 *         可以看出BME指令实现的执行效率更高效，程序最后点亮led灯

 *         systick end value: 0xff9d63
 *
 *         systick current value read overhead: 0x7
 *
 *         extracted data with las1 normal C is 1
 *
 *         systick start value: 0xff9d5c
 *
 *         systick end value: 0xff9d30
 *
 *         actual execution cycle for las1 operation with normal C code: 0x25
 *
 *         extracted data with las1 BME macro is 1
 *
 *         systick start value: 0xff9d6a
 *
 *         systick end value: 0xff9d58
 *
 *         actual execution cycle for las1 operation with BME macros: 0xb
 *
 *         extracted data with ubfx normal C is 17
 *
 *         systick start value: 0xff9d56
 *
 *         systick end value: 0xff9d2d
 *
 *         actual execution cycle for ubfx operation with normal C: 0x22
 *
 *         extracted data with ubfx BME macro is 17
 *
 *         systick start value: 0xff9d6a
 *
 *         systick end value: 0xff9d59
 *
 *         actual execution cycle for ubfx operation with BME macro: 0xa
 *
 * \note BME的 读回写1清零命令AMHW_KL26_BME_LAS1_*不在使用在有许多的w1c清零位的寄存器中，如外设的状态寄存器.
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_bme_deco_loads.c src_amks16z_core_hw_bme_deco_loads
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-18  nwt, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_hw_bme_deco_loads
 * \copydoc demo_amks16z_core_hw_bme_deco_loads.c
 */
 
/** [src_amks16z_core_hw_bme_deco_loads] */
#include "ametal.h"
#include "am_delay.h"
#include "hw/amhw_kl26_gpio.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_bme_deco_loads_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw bme deco loads!\r\n");

    demo_kl26_hw_bme_deco_loads_entry();
}

/** [src_amks16z_core_hw_bme_deco_loads] */
 
/* end of file */
