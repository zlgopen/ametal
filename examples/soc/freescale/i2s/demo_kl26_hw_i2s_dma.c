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
 * \brief I2S 发送（DMA方式）例程，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. 按照I2S 相关引脚接线TX_D0、TX_FS、TX_BCLK到逻辑分析仪。
 *   2. 调试程序。
 *
 * - 实验现象：
 *   1. 可以在逻辑分析仪上看到TX_D0、TX_FS、TX_BCLK的数据；
 *   2. 其中TX_D0为发送数据的补码，TX_FS是帧时钟，TX_BCLK是位时钟。
 *
 * \par 源代码
 * \snippet demo_kl26_hw_i2s_dma.c src_kl26_hw_i2s_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-29 mkr , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_hw_i2s_dma
 * \copydoc demo_kl26_hw_i2s_dma.c
 */

/** [src_kl26_hw_i2s_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_kl26_clk.h"
#include "am_kl26_inst_init.h"
#include "hw/amhw_kl26_sim.h"
#include "hw/amhw_fsl_i2s.h"
#include "am_kl26_dma.h"
#include "am_int.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/** \brief 使用I2S0 */
#define I2S_DEV    KL26_I2S0

static volatile uint8_t __g_signal = 0;


/** \brief DAC_DMA传输完成回调函数 */
static void __am_fsl_i2s_dma_isr (void *p_arg , uint8_t flag);

/**
 * \brief I2S 设备初始化
 */
static void __i2s_init (amhw_fsl_i2s_t *p_hw_i2s)
{
    /** \brief I2S时钟配置 */
    static amhw_fsl_i2s_clock_cfg_t i2s0_clk;

    /* 使能I2S 时钟门控 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_I2S);

    /* 配置I2S时钟 */
    i2s0_clk.bclk          = 256000UL ;
    i2s0_clk.bclk_src      = AMHW_FSL_I2S_BCLK_SRC_MCLK;
    i2s0_clk.bclk_src_freq = 4096000UL;
    i2s0_clk.mclk          = 4096000UL;
    i2s0_clk.mclk_src      = AMHW_FSL_I2S_MCLK_SRC_SYSCLK;
    i2s0_clk.mclk_src_freq = am_kl26_clk_core_clkrate_get();

    /* 发送寄存器初始化 */
    amhw_fsl_i2s_tx_init(p_hw_i2s);
    amhw_fsl_i2s_tx_clock_setup(p_hw_i2s, &i2s0_clk);
    amhw_fsl_i2s_tx_sync_mode_set(p_hw_i2s, AMHW_FSL_I2S_SYNC_DISABLED);
    amhw_fsl_i2s_tx_mode_set(p_hw_i2s, AMHW_FSL_I2S_MODE_MASTER);
    amhw_fsl_i2s_tx_protocol_set(p_hw_i2s, AMHW_FSL_I2S_PROTOCOL_TYPE);
    amhw_fsl_i2s_tx_channel_mode_set(p_hw_i2s, AMHW_FSL_I2S_CHANNEL_MODE_STEREO);
    amhw_fsl_i2s_tx_channel_enable(p_hw_i2s);
    amhw_fsl_i2s_tx_word_width_cfg(p_hw_i2s, AMHW_FSL_I2S_PROTOCOL_TYPE, 32);

    amhw_fsl_i2s_tx_int_disable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_FIFO_WARN);
    amhw_fsl_i2s_tx_int_disable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_FIFO_ERROR);

    amhw_fsl_i2s_tx_dma_request_enable(p_hw_i2s);

    amhw_fsl_i2s_tx_enable(p_hw_i2s);

#if 0
    /* 接收寄存器初始化 */
    amhw_fsl_i2s_rx_init(p_hw_i2s);
    amhw_fsl_i2s_rx_clock_setup(p_hw_i2s, &i2s0_clk);
    amhw_fsl_i2s_tx_int_disable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_ALL);
    amhw_fsl_i2s_rx_sync_mode_set(p_hw_i2s, AMHW_FSL_I2S_SYNC_SELF);
    amhw_fsl_i2s_rx_mode_set(p_hw_i2s, AMHW_FSL_I2S_MODE_MASTER);
    amhw_fsl_i2s_rx_protocol_set(p_hw_i2s, AMHW_FSL_I2S_PROTOCOL_TYPE);
    amhw_fsl_i2s_rx_channel_mode_set(p_hw_i2s, AMHW_FSL_I2S_CHANNEL_MODE_MONO);
    amhw_fsl_i2s_rx_channel_enable(p_hw_i2s);
    amhw_fsl_i2s_rx_word_width_cfg(p_hw_i2s, AMHW_FSL_I2S_PROTOCOL_TYPE, 32);
    amhw_fsl_i2s_rx_state_flag_clr(p_hw_i2s, AMHW_FSL_I2S_STATE_FLAG_ALL);
    amhw_fsl_i2s_rx_enable(p_hw_i2s);
#endif
}

/** \brief DMA 初始化配置 */
static void _fsl_i2s_dma_init (uint8_t            dma_chan,
                               uint32_t          *p_dest,
                               uint32_t          *p_src,
                               uint32_t           lenth)
{
    uint32_t flags;
    amhw_kl26_dma_xfer_desc_t dma_desc;

    /* DMA传输配置 */
    flags = KL26_DMA_DCR_PER_REQUEST_ENABLE      |  /* 外设请求源使能    */
            KL26_DMA_DCR_SINGLE_TRANSFERS        |  /* 单次传输          */
            KL26_DMA_DCR_AUTO_ALIGN_DISABLE      |  /* 自动对齐禁能      */
            KL26_DMA_DCR_SOURCE_SIZE_32_BIT      |  /* 源地址4字节读取   */
            KL26_DMA_DCR_SOURCE_SIZE_32_BIT      |  /* 目的地址4字节写入 */
            KL26_DMA_DCR_REQUEST_AFFECTED        |  /* 请求有影响        */
            KL26_DMA_DCR_NO_LINKING              |  /* 无通道连接        */
            KL26_DMA_DCR_INTERRUTP_ENABLE        |  /* DMA中断使能       */
            KL26_DMA_DCR_START_DISABLE    ;         /* DMA开始传输禁能   */

    /* 连接DMA中断服务函数 */
    am_kl26_dma_isr_connect(dma_chan, __am_fsl_i2s_dma_isr, I2S_DEV);

    am_kl26_dma_chan_cfg(dma_chan,
                         KL26_DMA_TRIGGER_DISABLE | /**< \brief DMA正常模式 */
                         DMA_REQUEST_MUX0_I2S0TX);       /**< \brief I2S请求源      */

    /* 建立通道描述符 */
    am_kl26_dma_xfer_desc_build(&dma_desc,              /* 通道描述符   */
                                (uint32_t)(p_src),      /* 源端数据缓冲 */
                                (uint32_t)(p_dest),     /* 目标数据缓冲 */
                                (uint32_t)(lenth << 2), /* 传输字节数   */
                                flags);                 /* 传输配置     */

    /* 启动DMA传输，马上开始传输 */
    if (am_kl26_dma_chan_start(&dma_desc,
                               KL26_DMA_MER_TO_PER, /*  内存到外设  */
                               dma_chan) == AM_ERROR) {
        am_kprintf("DMA init error!\n");
    }

}

/** \brief DAC_DMA传输完成回调函数 */
static void __am_fsl_i2s_dma_isr (void *p_arg , uint8_t flag)
{
    amhw_fsl_i2s_t *p_hw_i2s = (amhw_fsl_i2s_t *)p_arg;

    if (flag == AM_KL26_DMA_INT_NORMAL) {

        /* FIFO错误，清零该位 */
        if ((amhw_fsl_i2s_tcsr_get(p_hw_i2s) & AMHW_FSL_I2S_TCSR_FEF_MASK)) {
            amhw_fsl_i2s_tx_state_flag_clr(p_hw_i2s,AMHW_FSL_I2S_STATE_FLAG_FIFO_ERROR);
            amhw_fsl_i2s_tx_reset_set(p_hw_i2s,AMHW_FSL_I2S_RESET_TYPE_FIFO);
        }
        /* FIFO空 */
        if ((amhw_fsl_i2s_tcsr_get(p_hw_i2s) & AMHW_FSL_I2S_TCSR_FWF_MASK)) {
            __g_signal = 1;
            amhw_fsl_i2s_tx_dma_request_disable(p_hw_i2s);
        }
    }
}

/**
 * \brief i2s dma方式数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_kl26_hw_i2s_dma_entry (void)
{

    /** \brief I2S数据发送缓冲区  */
    uint32_t send_buf[10] = {1,2,3,4,5,6,7,8,9,10};

    am_kl26_dma_inst_init();       /* DMA实例初始化            */
    _fsl_i2s_dma_init(DMA_CHAN_0,  /* DMA传输初始化 */
                      (uint32_t *)&(I2S_DEV->tdr),
                      send_buf,
                      10);
    __i2s_init(I2S_DEV);
    am_kprintf("The I2S dma Demo \r\n");

    while(1) {
        if (1 == __g_signal) {
            __g_signal = 0;
        _fsl_i2s_dma_init(DMA_CHAN_0, /* DMA传输初始化 */
                          (uint32_t *)&(I2S_DEV->tdr),
                          send_buf,
                          10);
            amhw_fsl_i2s_tx_dma_request_enable(I2S_DEV);
        }
    }
}
/** [src_kl26_hw_i2s_dma] */

/* end of file */
