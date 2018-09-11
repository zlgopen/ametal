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
 * \brief GPIO引脚中断例程，通过标准接口实现
 *
 * - 操作：
 * - 1.将J14的key和PIOC_0短接在一起;
 *   2.将J9的LED0和PIOC_4短接在一起;
 *   3.将J10的LED1和PIOC_8短接在一起;
 *   4.PIOA_1 引脚连接PC串口的TXD;
 *   5.PIOA_2 引脚连接PC串口的RXD;
 *   6.配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 *- 实现现象：程序下载到demo板后，按下按键，demo板上的LED0会闪,
 *          同时串口输出 "the gpio interrupt happen!"
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_gpio_int.c src_amks16z_core_hw_gpio_int
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  nwt, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_gpio_int
 * \copydoc demo_amks16z_core_hw_gpio_int.c
 */

/** [src_amks16z_core_hw_gpio_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_fsl_entrys.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_gpio_int_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw gpio int!\r\n");

    demo_kl26_hw_gpio_int_entry();
}

/** [src_amks16z_core_hw_gpio_int] */

/* end of file */
