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
 * \brief DMA 硬件触发例程，通过驱动层接口实现
 *
 * - 操作步骤：
 *   1. 将 PIO0_17 和 PIO0_18 短接；
 *   2. 观察串口输出信息，当串口输出 "Press any key to finish level test!" 后，
 *      通过串口输入任意字符以继续测试；
 *   3. 观察串口输出信息，当串口输出 "Enter the trigger count to make burst!" 后，
 *      通过串口以字符串的方式输入待触发的次数，每次触发传送 8 字节的数据；
 *   4. 观察串口输出信息，当串口输出 "Press any key to finish edge test!" 后，通过
 *      串口输入任意字符以继续测试；
 *   5. 观察串口输出信息，当串口输出 "Enter any key make a burst!" 后，通过串口输
 *      入任意字符以继续测试。重复此过程 6 次。
 *
 * - 实验现象：
 *   1. DMA 通道 0 传输配置为中断输入电平触发，没有硬件触发条件下，数据不被传送；
 *   2. DMA 通道 0 传输配置为中断输入边沿触发，每次硬件触发传输大小为配置的突发长度；
 *   3. DMA 通道 0 传输配置为中断输入边沿触发，目标地址环绕，每次硬件触发，
 *      只在最后的突发大小的地址范围内传输数据。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_drv_dma_hwtrigger_burst.c src_am824_drv_dma_hwtrigger_burst
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-30  mem,modified
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_drv_dma_hwtrigger_burst
 * \copydoc demo_am824_drv_dma_hwtrigger_burst.c
 */

/** [src_am824_drv_dma_hwtrigger_burst] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc82x_pint.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "demo_nxp_entries.h"

#define __GET_CHAR(ch) \
            amhw_lpc_usart_poll_receive(AMHW_LPC82X_USART0, (&ch), 1);

#define __GPIO_PIN     PIO0_17          /**< \brief 产生中断信号的引脚 */
#define __INT_PIN      PIO0_18          /**< \brief 作为中断 0 输入的引脚 */


/**
 * \brief 初始化中断产生和检测引脚
 */
am_local void __pin_init()
{

    /* 用于产生中断信号 */
    am_gpio_pin_cfg(__GPIO_PIN, AM_GPIO_PULLUP | AM_GPIO_OUTPUT_INIT_HIGH);

    /* 中断引脚配置为输入 */
    am_gpio_pin_cfg(__INT_PIN, AM_GPIO_PULLUP | AM_GPIO_INPUT);

    /* 中断 0 引脚选择 */
    amhw_lpc82x_syscon_pint_sel(AMHW_LPC82X_PINT_CHAN_0, __INT_PIN);
}

/**
 * \brief 例程入口
 */
void demo_am824_drv_dma_hwtrigger_burst_entry (void)
{

    /* 触发引脚配置 */
    __pin_init();
  
    demo_lpc824_drv_dma_hwtrigger_burst_entry(DMA_CHAN_0, __GPIO_PIN);
}
/** [src_am824_drv_dma_hwtrigger_burst] */

/* end of file */
