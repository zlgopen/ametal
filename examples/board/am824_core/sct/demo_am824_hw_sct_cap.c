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
 * \brief SCT 32 位捕获例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 使用杜邦线，将外部提供的 PWM 信号与 PIO0_25 连接。
 *
 * - 实验现象：
 *   1. PIO0_25(SCT_IN_2) 配置为捕获功能，捕获 PWM 周期和频率并通过串口输出。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    2. 若有示波器、逻辑分析仪等仪器，可以对比输出波形频率和周期，是否与捕获计算
 *       的一致；
 *    3. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am824_hw_sct_cap.c src_am824_hw_sct_cap
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-19  hgo, modified
 * - 1.01 15-09-23  zxl, modified
 * - 1.00 15-07-14  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_sct_cap
 * \copydoc demo_am824_hw_sct_cap.c
 */

/** [src_am824_hw_sct_cap] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_inmux.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_hw_sct_cap_entry (void)
{
    uint32_t frq = 0;
  
    am_kprintf("demo am824_core hw sct cap!\r\n");
  
    /**
    * 一共有 4 个输入通道, 每个通道可选择的输入功能达到 8 个,
    * 具体的所有功能在 amhw_lpc82x_inmux.h 文件中关于 SCT 的定义
    * 这里配置输入通道 2
    */
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX,
                                   2,
                                   AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN2);

    /* 平台初始化 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SCT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);

    /* 将 SCT_PIN_2 关联到 PIO0_25 引脚 */
    am_gpio_pin_cfg(PIO0_25, PIO_FUNC_SCT_PIN2);
  
    /* 设置 SCT 触发信号源为 SCT 输入通道 2 */
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX,
                                   2,
                                   AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN2);
                                   
    frq = amhw_lpc82x_clk_system_clkrate_get();
    demo_lpc_hw_sct_cap_entry(LPC82X_SCT0, INUM_SCT0, frq);
}
/** [src_am824_hw_sct_cap] */

/* end of file */
