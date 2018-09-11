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
 * \brief I2S 发送（查询方式）例程，通过HW层接口实现
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
 * \snippet demo_fsl_hw_i2s_polling.c src_fsl_hw_i2s_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-28 mkr , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_i2s_polling
 * \copydoc demo_fsl_hw_i2s_polling.c
 */

/** [src_fsl_hw_i2s_polling] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "hw/amhw_fsl_i2s.h"
#include "demo_fsl_entrys.h"

/**
 * \brief I2S 设备初始化
 */
static void __i2s_init (amhw_fsl_i2s_t           *p_hw_i2s,
                        amhw_fsl_i2s_clock_cfg_t *p_i2s0_clk)
{
    /* 配置I2S时钟 */
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

    amhw_fsl_i2s_tx_int_disable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_FIFO_WARN);
    amhw_fsl_i2s_tx_int_disable(p_hw_i2s, AMHW_FSL_I2S_INT_REQ_FIFO_ERROR);

    amhw_fsl_i2s_tx_enable(p_hw_i2s);

#if 0
    /* 接收寄存器初始化 */
    amhw_fsl_i2s_rx_init(p_hw_i2s);
    amhw_fsl_i2s_rx_clock_setup(p_hw_i2s, p_i2s0_clk);
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

/**
 * \brief i2s数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_fsl_hw_i2s_polling_entry (amhw_fsl_i2s_t           *p_hw_i2s,
                                    amhw_fsl_i2s_clock_cfg_t *p_i2s0_clk)
{
    /** \brief I2S数据发送缓冲区  */
    static uint32_t send_buf[6] = {0,1,2,3,4,5};

    __i2s_init(p_hw_i2s, p_i2s0_clk);

    am_kprintf("The I2S polling Demo \r\n");

    while(1) {

        /* 循环发送缓冲区的数据 */
        amhw_fsl_i2s_senddata_polling(p_hw_i2s, send_buf, 6);
    }
}
/** [src_fsl_hw_i2s_polling] */

/* end of file */
