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
 * \brief LPUART 轮询方式例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口输出 "HW example---LPUART test in polling mode:"；
 *   2. 指定串口输出接收到的字符串。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_lpuart_polling.c src_zsn700_hw_lpuart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-16  YRZ, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_zsn700_hw_lpuart_polling
 * \copydoc demo_zsn700_hw_lpuart_polling.c
 */

/** [src_zsn700_hw_lpuart_polling] */
#include "ametal.h"
#include "hw/amhw_zsn700_lpuart.h"
#include "hw/amhw_zsn700_rcc.h"

/*******************************************************************************
  全局变量
*******************************************************************************/
static const uint8_t lpuart_hw_str[] =
                         {"HW example---LPUART test in polling mode:\r\n"};

static uint32_t lpuart_baudrate = 0;

/**
 * \brief LPUART HW 查询收发初始化
 */
static void lpuart_polling_init (amhw_zsn700_lpuart_t *p_hw_lpuart,
                                 uint8_t               clk_src,
                                 uint32_t              clk_rate)
{
    /* 工作模式设置  */
    amhw_zsn700_lpuart_mode_sel(p_hw_lpuart, AMHW_ZSN700_LPUART_WORK_MODE_1);

    /* 设置通信时钟源 */
    amhw_zsn700_lpuart_clk_src_sel(p_hw_lpuart,
                                   (amhw_zsn700_lpuart_sclk_src_t)clk_src);

    if(clk_src == AMHW_ZSN700_LPUART_SCLK_SRC_XTL) {

        /* 外部晶振驱动能力选择 */
        amhw_zsn700_rcc_xtl_xtal_driver_set(AMHW_ZSN700_XTL_XTAL_DRIVER_DEFAULT);

        /* XTL晶振振荡幅度的调整 */
        amhw_zsn700_rcc_xtl_xtal_amplitude_set(
            AMHW_ZSN700_XTL_XTAL_AMPLITUDE_BIG_DEFAULT);

        /* 外部高速时钟XTL稳定时间选择 */
        amhw_zsn700_rcc_xtl_waittime_set(AMHW_ZSN700_XTL_WAITTIME_16384);

        amhw_zsn700_rcc_set_start(0x5A5A);
        amhw_zsn700_rcc_set_start(0xA5A5);

        amhw_zsn700_rcc_xtl_enable();

        /* 等待稳定*/
        while(amhw_zsn700_rcc_xtl_state_get() == AM_FALSE);

    } else if(clk_src == AMHW_ZSN700_LPUART_SCLK_SRC_RCL) {

        /* 设置RCL时钟频率校准值 （38.4KHz）*/
        amhw_zsn700_rcc_rcl_trim_set(*((uint16_t*)0x00100C20));

        amhw_zsn700_rcc_rcl_waittime_set(AMHW_ZSN700_RCL_WAITTIME_256);

        amhw_zsn700_rcc_set_start(0x5A5A);
        amhw_zsn700_rcc_set_start(0xA5A5);

        amhw_zsn700_rcc_rcl_enable();

        /* 等待稳定*/
        while(amhw_zsn700_rcc_rcl_state_get() == AM_FALSE);
    }

    /* 分频系数设置  */
    amhw_zsn700_lpuart_clk_div_sel(p_hw_lpuart,
                                   AMHW_ZSN700_LPUART_SCLK_DIV_MODE13_8);

    /* 等待发送空完成 */
    while (amhw_zsn700_lpuart_flag_check(p_hw_lpuart,
                                         AMHW_ZSN700_LPUART_FLAG_TX_EMPTY) ==
                                         AM_FALSE);

    /* 停止位设置 */
    amhw_zsn700_lpuart_stop_bit_sel(p_hw_lpuart,
                                    AMHW_ZSN700_LPUART_STOP_1_0_BIT);

    /* 校验设置 */
    amhw_zsn700_lpuart_parity_bit_sel(p_hw_lpuart,
                                      AMHW_ZSN700_LPUART_PARITY_NO);

    /* 波特率设置 */
    amhw_zsn700_lpuart_baudrate_set(p_hw_lpuart, clk_rate, lpuart_baudrate);

    /* 接收使能
     * Mode0:     0：发送; 1：接收
     * Mode1/2/3: 0：发送; 1： 接收/发送
     */
    amhw_zsn700_lpuart_enable(p_hw_lpuart,AMHW_ZSN700_LPUART_RX);
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_lpuart_polling_entry (void     *p_hw_lpuart,
                                          uint8_t   clk_src,
                                          uint32_t  clk_rate,
                                          uint32_t  baudrate)
{

    /* 数据缓冲区 */
    uint8_t buf_src[256];
    amhw_zsn700_lpuart_t * p_lpuart = (amhw_zsn700_lpuart_t * )p_hw_lpuart;

    lpuart_baudrate = baudrate;

    /* LPUART查询初始化 */
    lpuart_polling_init(p_lpuart, clk_src, clk_rate);

    amhw_zsn700_lpuart_poll_send(p_lpuart,
                                 lpuart_hw_str,
                                 sizeof(lpuart_hw_str));

    while (1) {

        /* 接收字符 */
        amhw_zsn700_lpuart_poll_receive(p_lpuart, buf_src, 1);

        /* 发送刚刚接收的字符 */
        amhw_zsn700_lpuart_poll_send(p_lpuart, buf_src, 1);
    }
}
/** [src_zsn700116_hw_lpuart_polling] */

/* end of file */
