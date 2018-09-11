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
 * \brief SPI 从机接收数据例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 本例程作为从机 SPI，将主机与从机 SPI 进行物理连接。
 *
 * - 实验现象：
 *   1. SPI 接收到数据，通过串口将数据打印出来，并如果接收到的数据是 "nihao"，
 *      则 LED0 会闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 本程序需要与 demo_am824_hw_spi_master.c 一同调试；
 *    3. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    4. 由于 PIO0_12 拉低会导致单片机复位时进入 ISP 模式，所以 PIO0_12 应
 *       单片机复位后再连接。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_spi_slave.c src_lpc_hw_spi_slave
 *
 * \internal
 * \par History
 * - 1.01 15-11-26  sky, modified
 * - 1.00 15-07-15  aii, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_spi_slave
 * \copydoc demo_lpc_hw_spi_slave.c
 */

/** [src_lpc_hw_spi_slave] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc_spi.h"
#include <string.h>

#define __SPI_SPEED        100000  /**< \brief SPI 速率宏定义 */
#define __SPI_PRE_DELAY    0       /**< \brief SSEL 置位与起始帧间延时 */
#define __SPI_POST_DELAY   0       /**< \brief 帧末尾与 SSEL 解除置位间延时 */
#define __SPI_FRAME_DELAY  0       /**< \brief SPI 相邻数据帧间延时 */
#define __SPI_TRANS_DELAY  0       /**< \brief 两次传输 SSEL 解除置位间延时 */

#define __TIME1            100000  /**< \brief 宏定义为时钟等待时间（循环次数） */

am_local uint8_t __g_spi_rev_buf[255] = {0}; /**< \brief SPI接收缓冲数组变量 */

/**
 * \brief SPI0 硬件初始化
 *
 * \param[in] p_hw_spi 指向 SPI 寄存器块的指针
 *
 * \return 无
 */
am_local void __spi_slave_hard_init (amhw_lpc_spi_t *p_hw_spi,
                                     uint32_t        clk)
{

    /* 配置 SPI 分频值 */
    amhw_lpc_spi_div_set(p_hw_spi, (clk / __SPI_SPEED));

    /* 配置 SPI 传输延时 */
    amhw_lpc_spi_pre_delay(p_hw_spi, __SPI_PRE_DELAY);
    amhw_lpc_spi_post_delay(p_hw_spi, __SPI_POST_DELAY);
    amhw_lpc_spi_frame_delay(p_hw_spi, __SPI_FRAME_DELAY);
    amhw_lpc_spi_trans_delay(p_hw_spi, __SPI_TRANS_DELAY);

    /* 配置 SPI 传输参数 */
    amhw_lpc_spi_cfg_set(p_hw_spi, AMHW_LPC_SPI_CFG_CHANGE);
    amhw_lpc_spi_enable(p_hw_spi);

    /* 设置 SPI 一个数据长度为 8 位 */
    amhw_lpc_spi_data_flen_set(p_hw_spi, 8);
}

/**
 * \brief SPI 接收字符串函数
 *
 * \param[in] p_hw_spi 指向 SPI 寄存器块的指针
 *
 * \return 接收缓冲数组首地址
 */
am_local uint8_t *__spi_recive (amhw_lpc_spi_t *p_hw_spi)
{
    am_local uint8_t  i;                               /* 声明计数变量 i */
    volatile uint32_t timer1 = __TIME1;                /* 声明为时钟变量 */

    AM_FOREVER {

        /* 在一段时间内等待接收到数据状态置位 */
        while (!(amhw_lpc_spi_stat_get(p_hw_spi) & AMHW_LPC_SPI_STAT_RXRDY)
               && --timer1) {};

        /* 接收到数据而并非超时跳出等待 */
        if (timer1 != 0) {

            /* 将数据写入缓冲数组变量中 */
            __g_spi_rev_buf[i] = (uint8_t)amhw_lpc_spi_rxdat_with_flags(p_hw_spi);
            i++;
            __g_spi_rev_buf[i] = 0;   /* 对接收到的数据末尾写 0 保护字符串 */
            timer1 = __TIME1;         /* 重置等待为时钟时间 */
        } else {                      /* 当超时跳出等待时，退出接收数据 */
            i = 0;
            break;                    /* 跳出 while（1） */
        }
    }

    return __g_spi_rev_buf;           /* 返回接收缓冲数组变量首地址 */
}

void demo_lpc_hw_spi_slave_entry (amhw_lpc_spi_t *p_hw_spi,
                                  uint32_t        clk)
{
    /* SPI0 硬件初始化 */
    __spi_slave_hard_init(p_hw_spi, clk);
    
    /* 接收就绪 */
    AM_DBG_INFO("recv ready!\r\n");
  
    AM_FOREVER {

        /* 接收 SPI 数据 */
        __spi_recive(p_hw_spi);

        if ('\0' != __g_spi_rev_buf[0]) {

            /* 通过串口发出 SPI 数据 */
            AM_DBG_INFO("SPI: %s\r\n", __g_spi_rev_buf);
        }

        /* 判断从 SPI 接收到 "nihao" */
        if (strstr((am_const char *)__g_spi_rev_buf, "nihao")) {

            /* 翻转 LED 状态 */
            am_led_toggle(LED0);
        }
        /* 清空接收缓冲数组 */
        __g_spi_rev_buf[0] = 0;
    }
}
/** [src_lpc_hw_spi_slave] */

/* end of file */
