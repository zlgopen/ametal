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
 *    如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *    PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc845_drv_dma_hwtrigger_burst.c src_lpc845_drv_dma_hwtrigger_burst
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-30  mem,modified
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc845_drv_dma_hwtrigger_burst
 * \copydoc demo_lpc845_drv_dma_hwtrigger_burst.c
 */

/** [src_lpc845_drv_dma_hwtrigger_burst] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_lpc84x_dma.h"
#include "hw/amhw_lpc_usart.h"
#include "lpc84x_periph_map.h"
#include "hw/amhw_lpc82x_pint.h"
#include "hw/amhw_lpc84x_syscon.h"

#define __GET_CHAR(ch) \
            amhw_lpc_usart_poll_receive(LPC84X_USART0, (&ch), 1);

#define __TEST_COUNT   48               /**< \brief 测试数据大小 */

am_local uint8_t __g_buf_src[__TEST_COUNT] = {0}; /**< \brief 源端数据缓冲区 */
am_local uint8_t __g_buf_dst[__TEST_COUNT] = {0}; /**< \brief 通道 0 目标数据缓冲区 */

/**
 * \brief 缓冲区打印函数
 */
am_local void __buffer_printf (uint8_t *p_buf, uint16_t count)
{
    uint16_t i;

    AM_DBG_INFO("-------------------");
    for (i = 0; i < __TEST_COUNT; i++) {
        if ((i % 10) == 0) {
            AM_DBG_INFO("\r\n");
        }
        AM_DBG_INFO("%d ", p_buf[i]);
    }
    AM_DBG_INFO("\r\n -------------------\r\n");
}

/**
 * \brief 硬件触发电平突发模式测试
 */
am_local void __dma_hwtrigger_bust_level_test (uint8_t  chan, int pin)
{
    int                         i     = 0;
    uint8_t                     ch    = 1;
    am_lpc84x_dma_controller_t *p_ctr = NULL;
    am_lpc84x_dma_transfer_t    transfer;

    AM_DBG_INFO("DMA PINT0 level triger burst test!\r\n");

    for (i = 0; i < __TEST_COUNT; i++) {
        __g_buf_src[i] = i;
    }
    AM_DBG_INFO("Src buffer data:\r\n");
    __buffer_printf(__g_buf_src, __TEST_COUNT);

    for (i = 0; i < __TEST_COUNT; i++) {
        __g_buf_dst[i] = 0;
    }
    AM_DBG_INFO("Dst buffer init data:\r\n");
    __buffer_printf(__g_buf_dst, __TEST_COUNT);

    /* 配置 PINT 高电平触发(不要开启 NVIC 中断) */
    amhw_lpc82x_pint_trigger_set(LPC84X_PINT,
                                 AMHW_LPC82X_PINT_CHAN_4,
                                 AMHW_LPC82X_PINT_TRIGGER_HIGH);

    p_ctr = am_lpc84x_dma_controller_get(
                chan,                             /* DMA 通道 0 */
                DMA_CHAN_OPT_PRIO_0             | /* 优先级 0 */
                DMA_CHAN_OPT_HWTRIG_MODE_HIGH   | /* 信号高电平触发 */
                DMA_CHAN_OPT_HWTRIG_SRC_PININT4 | /* 中断输入 4 触发 */
                DMA_CHAN_OPT_HWTRIG_BURST_EN);    /* 使能突发模式 */

    if (p_ctr == NULL) {
        AM_DBG_INFO("Geting a Controller Faild!\r\n");
        AM_FOREVER {};
    }

    am_lpc84x_dma_transfer_build(
       &transfer,                          /* 传输结构体 */
        (uint32_t)__g_buf_src,             /* 源地址 */
        (uint32_t)__g_buf_dst,             /* 目标地址 */
        __TEST_COUNT,                      /* 传输字节数 */
        AM_LPC84X_DMA_TRANS_WIDTH_8BIT  |  /* 传输宽度 */
        AM_LPC84X_DMA_TRANS_SRCINC_1X   |  /* 源地址增 1 */
        AM_LPC84X_DMA_TRANS_DSTINC_1X);    /* 目标地址增 1 */

    am_lpc84x_dma_transfer(p_ctr, &transfer, NULL, (void*)0);

    /* 延迟一段时间 */
    am_mdelay(100);

    /* 打印缓冲区数据 */
    AM_DBG_INFO("Dest buffer data: (transfered but not triggered)\r\n");
    __buffer_printf(__g_buf_dst, __TEST_COUNT);

    /* 触发 DMA 传输 */
    AM_DBG_INFO("Make a trigger...");
    am_gpio_set(pin, AM_GPIO_LEVEL_HIGH);
    am_mdelay(10);
    am_gpio_set(pin, AM_GPIO_LEVEL_LOW);

    /* 打印缓冲区数据 */
    AM_DBG_INFO("Dest buffer data: (transfered and triggered)\r\n");
    __buffer_printf(__g_buf_dst, __TEST_COUNT);

    /* 释放 DMA */
    AM_DBG_INFO("Release DMA resource!\r\n");
    am_lpc84x_dma_controller_release(p_ctr);

    AM_DBG_INFO("Press any key to finish level test!\r\n");
    __GET_CHAR(ch);
}

/**
 * \brief DAM 边沿硬件突发模式测试
 */
am_local void __dma_hwtrigger_bust_edge_test (uint8_t  chan, int pin)
{
    int                         i     = 0;
    uint8_t                     ch    = 0;
    am_lpc84x_dma_controller_t *p_ctr = NULL;
    am_lpc84x_dma_transfer_t    transfer;

    AM_DBG_INFO("DMA PINT0 edge triger burst test!\r\n");

    for (i = 0; i < __TEST_COUNT; i++) {
        __g_buf_src[i] = i;
    }
    AM_DBG_INFO("Src buffer data:\r\n");
    __buffer_printf(__g_buf_src, __TEST_COUNT);

    for (i = 0; i < __TEST_COUNT; i++) {
        __g_buf_dst[i] = 0;
    }
    AM_DBG_INFO("Dst buffer init data:\r\n");
    __buffer_printf(__g_buf_dst, __TEST_COUNT);

    /* 配置 PINT 上升沿触发 */
   amhw_lpc82x_pint_trigger_set(LPC84X_PINT,
                                AMHW_LPC82X_PINT_CHAN_4,
                                AMHW_LPC82X_PINT_TRIGGER_RISE);

    p_ctr = am_lpc84x_dma_controller_get(
                chan,
                DMA_CHAN_OPT_PRIO_0              | /* 优先级 0 */
                DMA_CHAN_OPT_HWTRIG_MODE_RISE    | /* 硬件上升沿触发 */
                DMA_CHAN_OPT_HWTRIG_BURST_8      | /* 突发传输大小为 8 */
                DMA_CHAN_OPT_HWTRIG_SRC_PININT4  | /* 中断输入 0 触发 */
                DMA_CHAN_OPT_HWTRIG_BURST_EN);     /* 使能突发模式 */

    if (p_ctr == NULL) {
        AM_DBG_INFO("Geting a controller faild!\r\n");
        AM_FOREVER {};
    }
    am_lpc84x_dma_transfer_build(
       &transfer,                          /* 传输结构体 */
        (uint32_t)__g_buf_src,             /* 源地址 */
        (uint32_t)__g_buf_dst,             /* 目标地址 */
        __TEST_COUNT,                      /* 传输字节数 */
        AM_LPC84X_DMA_TRANS_WIDTH_8BIT  |  /* 传输宽度 */
        AM_LPC84X_DMA_TRANS_SRCINC_1X   |  /* 源地址增 1 */
        AM_LPC84X_DMA_TRANS_DSTINC_1X);    /* 目标地址增 1 */

    am_lpc84x_dma_transfer(p_ctr, &transfer, NULL, (void*)0);

    am_mdelay(10);

    /* 打印缓冲区数据 */
    AM_DBG_INFO("Dest buffer data: (transfered but not triggered)\r\n");
    __buffer_printf(__g_buf_dst, __TEST_COUNT);

    /* 产生输入 count 次边沿触发 DAM 传输 */
    do {
        AM_DBG_INFO("Enter the trigger count to make burst!\r\n");
        __GET_CHAR(ch);
    } while (ch < 49 && ch > 57);

    while ((ch--) > 48) {

        /* 触发 DMA 传输 */
        am_mdelay(10);
        am_gpio_set(pin, AM_GPIO_LEVEL_HIGH);
        am_mdelay(10);
        am_gpio_set(pin, AM_GPIO_LEVEL_LOW);
    }

    am_mdelay(100);

    /* 打印缓冲区数据 */
    AM_DBG_INFO("Dest buffer data:(transfered and triggered)\r\n");
    __buffer_printf(__g_buf_dst, __TEST_COUNT);

    /* 数据还没传完，但是可以终止传输 */
    AM_DBG_INFO("Abort the Transfer!\r\n");
    am_lpc84x_dma_controller_abort(p_ctr);

    /* 释放 DMA */
    AM_DBG_INFO("Release DMA resource!\r\n");
    am_lpc84x_dma_controller_release(p_ctr);

    AM_DBG_INFO("Press any key to finish edge test!\r\n");
    __GET_CHAR(ch);
}

/**
 * \brief DAM 边沿硬件突发地址环绕测试
 */
am_local void __dma_hwtrigger_bust_wrap_test (uint8_t chan, int pin)
{
    int                         i     = 0;
    uint8_t                     ch    = 0;
    am_lpc84x_dma_controller_t *p_ctr = NULL;
    am_lpc84x_dma_transfer_t    transfer;

    AM_DBG_INFO("DMA PINT0 wrap triger burst test!\r\n");

    for (i = 0; i < __TEST_COUNT; i++) {
        __g_buf_src[i] = i;
    }
    AM_DBG_INFO("Src buffer data:\r\n");
    __buffer_printf(__g_buf_src, __TEST_COUNT);

    for (i = 0; i < __TEST_COUNT; i++) {
        __g_buf_dst[i] = 0;
    }
    AM_DBG_INFO("Dst buffer init data:\r\n");
    __buffer_printf(__g_buf_dst, __TEST_COUNT);

    /* 配置 PINT 上升沿触发 */
    amhw_lpc82x_pint_trigger_set(LPC84X_PINT,
    		                     AMHW_LPC82X_PINT_CHAN_4,
    		                     AMHW_LPC82X_PINT_TRIGGER_RISE);

    p_ctr = am_lpc84x_dma_controller_get(
                chan,
                DMA_CHAN_OPT_PRIO_0                | /* 优先级 0 */
                DMA_CHAN_OPT_HWTRIG_MODE_FALL      | /* 硬件上升沿触发 */
                DMA_CHAN_OPT_HWTRIG_BURST_8        | /* 突发传输大小为 8 */
                DMA_CHAN_OPT_HWTRIG_BURST_DST_WRAP | /* 目标地址环绕 */
                DMA_CHAN_OPT_HWTRIG_SRC_PININT4    | /* 中断输入 4 触发 */
                DMA_CHAN_OPT_HWTRIG_BURST_EN);       /* 使能突发模式 */

    if (p_ctr == NULL) {
        AM_DBG_INFO("Geting a controller faild!\r\n");
        AM_FOREVER {};
    }
    am_lpc84x_dma_transfer_build(
       &transfer,                          /* 传输结构体 */
        (uint32_t)__g_buf_src,             /* 源地址 */
        (uint32_t)__g_buf_dst,             /* 目标地址 */
        __TEST_COUNT,                      /* 传输字节数 */
        AM_LPC84X_DMA_TRANS_WIDTH_8BIT  |  /* 传输宽度 */
        AM_LPC84X_DMA_TRANS_SRCINC_1X   |  /* 源地址增 1 */
        AM_LPC84X_DMA_TRANS_DSTINC_1X);    /* 目标地址增 1 */

    am_lpc84x_dma_transfer(p_ctr, &transfer, NULL, (void*)0);

    while(__g_buf_dst[__TEST_COUNT-1] < (__TEST_COUNT-1)) {
        AM_DBG_INFO("Enter any key make a burst!\r\n");
        __GET_CHAR(ch);

        /* 触发 DMA 传输 */
        am_mdelay(10);
        am_gpio_set(pin, AM_GPIO_LEVEL_LOW);
        am_mdelay(10);
        am_gpio_set(pin, AM_GPIO_LEVEL_HIGH);

        AM_DBG_INFO("Dst buffer init data:\r\n");
        __buffer_printf(__g_buf_dst, __TEST_COUNT);
    }

    /* 释放 DMA */
    AM_DBG_INFO("Release DMA resource!\r\n");
    am_lpc84x_dma_controller_release(p_ctr);

    AM_DBG_INFO("Press any key to finish wrap test!\r\n");
    __GET_CHAR(ch);
}

void demo_lpc845_drv_dma_hwtrigger_burst_entry (uint8_t  chan,
                                                int      pin)
{
    int i = 0;

    /* 构造待传输的数据 */
    for (i = 0; i < __TEST_COUNT; i++) {
        __g_buf_src[i] = i+1;
    }

    /* 硬件触发电平突发模式测试 */
    __dma_hwtrigger_bust_level_test(chan, pin);

    /* 硬件触发边沿突发模式测试 */
    __dma_hwtrigger_bust_edge_test(chan, pin);

    /* 硬件触发突发环绕模式测试 */
    __dma_hwtrigger_bust_wrap_test(chan, pin);

    AM_DBG_INFO("Finish all test!\r\n");

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_lpc845_drv_dma_hwtrigger_burst] */

/* end of file */
