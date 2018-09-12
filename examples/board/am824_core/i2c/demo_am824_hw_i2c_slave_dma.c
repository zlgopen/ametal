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
 * \brief I2C 从机例程（DMA 方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_10 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIO0_11 引脚连接 I2C 主机的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 接收主机发送过来的数据，通过串口发送给上位机；
 *   2. 传输失败则 LED0 灯长亮。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. DMA 初始化工作已经在 am_prj_config.c 文件内部完成。
 *
 * \par 源代码
 * \snippet demo_am824_hw_i2c_slave_dma.c src_am824_hw_i2c_slave_dma
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-04  cyl, modified the using of dma interface
 * - 1.00 15-07-16  win, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_am824_hw_i2c_slave_dma
 * \copydoc demo_am824_hw_i2c_slave_dma.c
 */

/** [src_am824_hw_i2c_slave_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_hw_i2c_slave_dma_entry (void)
{

    AM_DBG_INFO("demo am824 hw i2c slave dma mode!\r\n");

    am_gpio_pin_cfg(PIO0_10, PIO0_10_I2C0_SCL);
    am_gpio_pin_cfg(PIO0_11, PIO0_11_I2C0_SDA);

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_I2C0);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_I2C0);

    demo_lpc824_hw_i2c_slave_dma_entry(LPC82X_I2C0, DMA_CHAN_I2C0_SLV_REQ);
}
/** [src_am824_hw_i2c_slave_dma] */

/* end of file */
