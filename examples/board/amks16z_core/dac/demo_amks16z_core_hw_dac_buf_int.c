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
 * \brief DAC数模转换例程，通过HW接口实现
 *
 * - 实验现象：
 *   1. 配置PIOE_30为DAC模拟输出
 *   2. 用示波器采集数模转换的输出口PE30的信号输出
 *   3. 示波器显示PIOE_30的模拟输出周期大概为7秒的正弦波。
 *
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_dac_buf_int.c src_amks16z_core_hw_dac_buf_int
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-26  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_dac_buf_int
 * \copydoc demo_amks16z_core_hw_dac_buf_int.c
 */

 /** [src_amks16z_core_hw_dac_buf_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_int.h"
#include "hw/amhw_kl26_sim.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \brief DAC 使用缓冲区中断输出正弦波信号，HW层实现
 * \return 无
 */
void demo_amks16z_core_hw_dac_buf_int_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw dac buf int!\r\n");

    /* 配置PIOE_30为DAC0_OUT功能                 */
    am_gpio_pin_cfg (PIOE_30, PIOE_30_DAC0_OUT);

    /* 开启DAC时钟                               */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_DAC0);

    demo_fsl_hw_dac_buf_int_entry(KL26_DAC0, INUM_DAC0);
}

/** [src_amks16z_core_hw_dac_buf_int] */

/* end of file */
