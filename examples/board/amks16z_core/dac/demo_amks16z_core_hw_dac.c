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
 *   1. 采集数模转换的输出口PE30的电压值
 *   2. 使用DACREF_2参考电压，DACREF_2连接VDDA管脚，电压值为3.3V。
 *   3. 输出电压值为1.823V
 *
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_dac.c src_amks16z_core_hw_dac
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-26  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_dac
 * \copydoc demo_amks16z_core_hw_dac.c
 */

 /** [src_amks16z_core_hw_dac] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_kl26_inst_init.h"
#include "hw/amhw_kl26_sim.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \brief DAC输出电压值，调用HW层实现
 */
void demo_amks16z_core_hw_dac_entry ()
{
    /* 配置PIOE_30为DAC0_OUT功能                 */
    am_gpio_pin_cfg (PIOE_30,PIOE_30_DAC0_OUT);

    /* 开启DAC时钟                               */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_DAC0);

	  /* 输出1832mv电压 */
    demo_fsl_hw_dac_entry(KL26_DAC0, 1832);
}


 /** [src_amks16z_core_hw_dac] */

/* end of file */


