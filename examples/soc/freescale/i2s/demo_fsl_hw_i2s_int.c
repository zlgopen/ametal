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
 * \brief I2S 发送（中断方式）例程，通过HW层接口实现
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
 * \snippet demo_fsl_hw_i2s_int.c src_fsl_hw_i2s_int
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-28 mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_i2s_int
 * \copydoc demo_fsl_hw_i2s_int.c
 */

/** [src_fsl_hw_i2s_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "hw/amhw_fsl_i2s.h"
#include "am_int.h"
#include "demo_fsl_entrys.h"

static uint8_t __g_kl26_fifo_err_flag  = 0;
static uint8_t __g_kl26_fifo_warn_flag = 0;

/**
 * \brief I2S 设备初始化
 */
static void __i2s_init (amhw_fsl_i2s_t           *p_hw_i2s,
                        amhw_fsl_i2s_clock_cfg_t *p_i2s0_clk)
{

    /** \brief 配置I2S时钟 */
    p_i2s0_clk->bclk          = 256000UL ;
    p_i2s0_clk->bclk_src      = AMHW_FSL_I2S_BCLK_SRC_MCLK;
    p_i2s0_clk->bclk_src_freq = 4096000UL;
    p_i2s0_clk->mclk          = 4096000UL;
    p_i2s0_clk->mclk_src      = AMHW_FSL_I2S_MCLK_SRC_SYSCLK;

    /* 发送寄存器初始化 */
    amhw_fsl_i2s_tx_init(p_hw_i2s);
    amhw_fsl_i2s_tx_clock_setup(p_hw_i2s, p_i2s0_clk);
    amhw_fsl_i2s_tx_sync_mode_set(p_hw_i2s, AMHW_FSL_I2S_SYNC_DISABLED);
    amhw_fsl_i2s_tx_mode_set(p_hw_i2s, AMHW_FSL_I2S_MODE_MASTER);
    amhw_fsl_i2s_tx_protocol_set(p_hw_i2s, AMHW_FSL_I2S_PROTOCOL_TYPE);
    amhw_fsl_i2s_tx_channel_mode_set(p_hw_i2s, AMHW_FSL_I2S_CHANNEL_MODE_STEREO);
    amhw_fsl_i2s_tx_channel_enable(p_hw_i2s);
    amhw_fsl_i2s_tx_word_width_cfg(p_hw_i2s, AMHW_FSL_I2S_PROTOCOL_TYPE, 32);

    amhw_fsl_i2s_tx_int_enable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_FIFO_WARN);
    amhw_fsl_i2s_tx_int_enable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_FIFO_ERROR);

    amhw_fsl_i2s_tx_enable(p_hw_i2s);

#if 0
    /* 接收寄存器初始化 */
    amhw_fsl_i2s_rx_init(p_hw_i2s);
    amhw_fsl_i2s_rx_clock_setup(p_hw_i2s, p_i2s0_clk);
    amhw_fsl_i2s_rx_int_disable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_ALL);
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

/** \brief I2S中断服务函数 */
static void __i2s_isr_handler (void *p_var)
{
    /** \brief 要发送的数据   */
    static uint32_t dat = 0;
    amhw_fsl_i2s_t *p_hw_i2s = (amhw_fsl_i2s_t *)(p_var);

    /* FIFO错误，清零该位 */
    if ((amhw_fsl_i2s_tcsr_get(p_hw_i2s) & AMHW_FSL_I2S_TCSR_FEF_MASK)) {
        amhw_fsl_i2s_tx_state_flag_clr(p_hw_i2s, AMHW_FSL_I2S_STATE_FLAG_FIFO_ERROR);
        amhw_fsl_i2s_tx_reset_set(p_hw_i2s, AMHW_FSL_I2S_RESET_TYPE_FIFO);

        __g_kl26_fifo_err_flag = 1;
    }

    /* FIFO空，填充数据发送 */
    if ((amhw_fsl_i2s_tcsr_get(p_hw_i2s) & AMHW_FSL_I2S_TCSR_FWF_MASK)) {
        /* 发送数据 */
        amhw_fsl_i2s_txdata_write (p_hw_i2s, dat);
        dat  = dat + 1;
        dat %= 127;
        __g_kl26_fifo_warn_flag = 1;
    }

    /* 接收中断处理 */

}

/**
 * \brief i2s中断数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_fsl_hw_i2s_int_entry (amhw_fsl_i2s_t           *p_hw_i2s,
                                int                       inum_i2s,
                                amhw_fsl_i2s_clock_cfg_t *p_i2s0_clk)
{
    uint32_t key;
    extern int am_int_connect (int inum, am_pfnvoid_t pfn_isr, void *p_arg);
    extern int am_int_enable (int inum);

    am_int_connect(inum_i2s, __i2s_isr_handler, p_hw_i2s);
    am_int_enable(inum_i2s);

    __i2s_init(p_hw_i2s, p_i2s0_clk);

    am_kprintf("The I2S int Demo\r\n");

    while(1) {
        key = am_int_cpu_lock();
        if (1 == __g_kl26_fifo_err_flag) {
            __g_kl26_fifo_err_flag = 0;
            am_int_cpu_unlock(key);
            am_kprintf("fifo error interrupt take care !!\r\n");
        } else {
            am_int_cpu_unlock(key);
        }

        key = am_int_cpu_lock();
        if (1 == __g_kl26_fifo_warn_flag) {
            __g_kl26_fifo_warn_flag = 0;
            am_int_cpu_unlock(key);
            am_kprintf("fifo warning interrupt take care !!\r\n");
        } else {
            am_int_cpu_unlock(key);
        }

        am_kprintf("The program is running...\r\n");

        am_mdelay(200);

    }
}
/** [src_fsl_hw_i2s_int] */

/* end of file */
