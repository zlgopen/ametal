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
 * \brief SPI 从机接收数据例程（DMA 方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 接口和数据源对应的 SPI 接口连接起来。
 *
 * - 实验现象：
 *   1. 接收到数据，通过串口打印出来。
 *
 * \note
 *    1. 数据源可选用 demo_am824_std_spi_master_dma.c 的程序；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. 由于 PIO0_12 拉低会导致单片机复位时进入 ISP 模式，所以 PIO0_12 应
 *       单片机复位后再连接。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_spi_slave_dma.c src_lpc824_hw_spi_slave_dma
 *
 * \internal
 * \par History
 * - 1.01 15-11-27  sky, modified
 * - 1.00 15-07-16  aii, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_spi_slave_dma
 * \copydoc demo_lpc824_hw_spi_slave_dma.c
 */

/** [src_lpc824_hw_spi_slave_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x_dma.h"
#include "hw/amhw_lpc_spi.h"
#include "hw/amhw_lpc84x_clk.h"

/** \brief 重载描述映射 */
am_local __attribute__((aligned(16)))  am_lpc84x_dma_xfer_desc_t __g_desc[2];

am_local uint16_t __g_buf_dsta[16];                /**< \brief A 描述符缓存区 */
am_local uint16_t __g_buf_dstb[16];                /**< \brief B 描述符缓存区 */

am_local volatile uint8_t __g_a_transfer_done;     /**< \brief A 传输完成标志 */
am_local volatile uint8_t __g_b_transfer_done;     /**< \brief B 传输完成标志 */

/**
 * \brief DMA 中断服务函数
 *
 * \param[in] p_arg 中断服务函数入口参数
 * \param[in] p_flg DMA 中断标志（例如 A 传输中断，B 传输中断，错误中断等）
 *
 * \return 无
 */
am_local void __dma_isr (void *p_arg, int stat)
{
    if (stat & AM_LPC84X_DMA_STAT_INTA) {
        __g_a_transfer_done = 1;
    }

    if (stat & AM_LPC84X_DMA_STAT_INTB) {
        __g_b_transfer_done = 1;
    }

    if (stat & AM_LPC84X_DMA_STAT_INTERR) {

        /* 此处没有代码 */
    }
}

/**
 * \brief DMA 乒乓模式传输测试函数
 */
am_local void __dma_ping_pong_test (amhw_lpc_spi_t *p_hw_spi, uint8_t chan)
{
    uint32_t                    flags1 = 0;
    uint32_t                    flags2 = 0;
    am_lpc84x_dma_controller_t *p_ctr  = NULL;

    p_ctr = am_lpc84x_dma_controller_get(chan,
                                         DMA_CHAN_OPT_PRIO_0 |
                                         DMA_CHAN_OPT_PERIPH_REQ_EN);

    if (p_ctr == NULL) {
        AM_DBG_INFO("Geting a Controller Faild !\r\n");
        AM_FOREVER {};
    }

    /* DMA 传输配置 1 */
    flags1 = AM_LPC84X_DMA_XFER_VALID         | /* 当前通道描述符有效 */
             AM_LPC84X_DMA_XFER_RELOAD        | /* 传输完成重载通道描述符 */
             AM_LPC84X_DMA_XFER_SWTRIG        | /* 软件触发 */
             AM_LPC84X_DMA_XFER_WIDTH_16BIT   | /* 传输数据宽度：16 bit */
             AM_LPC84X_DMA_XFER_SRCINC_NOINC  | /* 源端数据地址不递增 */
             AM_LPC84X_DMA_XFER_DSTINC_1X     | /* 目标数据地址按 1 个宽度递增 */
             AM_LPC84X_DMA_XFER_SETINTA;        /* 使能中断 A */

    /* DMA 传输配置 2 */
    flags2 = AM_LPC84X_DMA_XFER_VALID         | /* 当前通道描述符有效 */
             AM_LPC84X_DMA_XFER_RELOAD        | /* 传输完成重载通道描述符 */
             AM_LPC84X_DMA_XFER_SWTRIG        | /* 软件触发 */
             AM_LPC84X_DMA_XFER_WIDTH_16BIT   | /* 传输数据宽度：16 bit */
             AM_LPC84X_DMA_XFER_SRCINC_NOINC  | /* 源端数据地址不递增 */
             AM_LPC84X_DMA_XFER_DSTINC_1X     | /* 目标数据地址按 1 个宽度递增 */
             AM_LPC84X_DMA_XFER_SETINTB;        /* 使能中断 B */

    /* 建立通道描述符 A */
    am_lpc84x_dma_xfer_desc_build(
       &__g_desc[0],                          /* A 的重载描述符 */
        (uint32_t)&(p_hw_spi->rxdat),         /* 源端数据地址 */
        (uint32_t)__g_buf_dsta,               /* A 的缓冲区地址 */
        32,                                   /* 传输字节数 */
        flags1);                              /* 传输配置 1 */

    /* 建立通道描述符 B */
    am_lpc84x_dma_xfer_desc_build(
       &__g_desc[1],                          /* B 的重载描述符 */
        (uint32_t)&(p_hw_spi->rxdat),         /* 源端数据地址 */
        (uint32_t)__g_buf_dstb,               /* B 的缓冲区地址 */
        32,                                   /* 传输字节数 */
        flags2);                              /* 传输配置 2 */

    am_lpc84x_dma_xfer_desc_link(&__g_desc[1], &__g_desc[0]); /* 链接重载描述符 */
    am_lpc84x_dma_xfer_desc_link(&__g_desc[0], &__g_desc[1]); /* 链接重载描述符 */

    /* DMA 传输启动 */
    am_lpc84x_dma_xfer_desc_startup(p_ctr,
                                   &__g_desc[0],
                                    __dma_isr,
                                    (void*)0);
}

/**
 * \brief SPI 从机启动
 *
 * \param[in] p_hw_spi 指向 SPI 寄存器块的指针
 * \param[in] len      数据帧长度，范围 1 ~ 16.
 *
 * \return 无
 */
am_local void __spi_slv_start (amhw_lpc_spi_t *p_hw_spi, uint8_t len)
{
    amhw_lpc_spi_data_flen_set(p_hw_spi, len);
}

/**
 * \brief SPI 从机初始化配置
 *
 * \param[in] p_hw_spi 指向 SPI 寄存器块的指针
 *
 * \retval AM_OK      配置完成
 * \retval -AM_EINVAL 参数无效
 */
am_local int __spi_slv_init (amhw_lpc_spi_t *p_hw_spi)
{
    amhw_lpc_spi_enable(p_hw_spi);         /* 配置完成，使能 SPI(默认为从机) */

    return AM_OK;
}


void demo_lpc824_hw_spi_slave_dma_entry(amhw_lpc_spi_t *p_hw_spi, uint8_t chan)
{
    uint8_t i = 0;

    /* SPI 从机初始化配置 */
    __spi_slv_init(p_hw_spi);

    /* SPI 字长度配置 */
    __spi_slv_start(p_hw_spi, 16);

    AM_DBG_INFO("Transform Start \r\n");

    __dma_ping_pong_test(p_hw_spi, chan);

    AM_FOREVER {

        /* A 传输完成标志位置位，通过串口将接收到数据打印出来 */
        if (__g_a_transfer_done) {
            __g_a_transfer_done = 0;
            for (i = 0; i < 16; i++){
                AM_DBG_INFO("A_buf %2dst is :0x%04x\r\n", i, __g_buf_dsta[i]);
            }
        }

        /* B 传输完成标志位置位，通过串口将接收到数据打印出来 */
        if (__g_b_transfer_done) {
            __g_b_transfer_done = 0;
            for (i = 0; i < 16; i++){
                AM_DBG_INFO("B_buf %2dst is :0x%04x\r\n", i, __g_buf_dstb[i]);
            }
        }
    }
}
/** [src_lpc824_hw_spi_slave_dma] */

/* end of file */
