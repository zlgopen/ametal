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
 * \brief DAC数模转换例程，通过HW接口实现，DMA传输数据
 *
 * - 实验现象：
 *   1. 采集数模转换的输出口PE30的电压值
 *
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_dac_dma_st.c src_amks16z_core_hw_dac_dma_st
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-27  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_dac_dma_st
 * \copydoc demo_amks16z_core_hw_dac_dma_st.c
 */

 /** [src_amks16z_core_hw_dac_dma_st] */
#include "ametal.h"
#include "am_dac.h"
#include "am_fsl_dac.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_kl26_inst_init.h"
#include "am_kl26_dma.h"
#include "am_int.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief DAC输出电压，DMA传输数据，HW层接口实现
 * \return 无
 */
void demo_amks16z_core_hw_dac_dma_st_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw dac dma st!\r\n");

    demo_kl26_hw_dac_dma_st_entry();
}


 /** [src_amks16z_core_hw_dac_dma_st] */

/* end of file */

