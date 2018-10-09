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
 * \brief DAC数模转换例程，通过STD接口实现
 *
 * - 实验现象：
 *   1. PE30输出1523mv电压。
 *
 *
 * \par 源代码
 * \snippet demo_kl26_std_dac.c src_kl26_std_dac
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-27  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_std_dac
 * \copydoc demo_fsl_std_dac.c
 */

 /** [src_fsl_std_dac] */
#include "ametal.h"
#include "am_dac.h"
#include "am_fsl_dac.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_kl26_inst_init.h"
#include "demo_fsl_entrys.h"

/**
 * \brief DAC输出电压，标准接口层实现
 * \return 无
 */
void demo_fsl_std_dac_entry (am_dac_handle_t dac0_handle,
                             uint32_t        mv_out)
{
    uint32_t        dac_bits    = 0;
    uint32_t        dac_vref    = 0;

    /* DAC实例初始化，并获取DAC句柄值 */
    dac0_handle = am_kl26_dac0_inst_init();

    /* 获取DAC转换精度 */
    dac_bits = am_dac_bits_get(dac0_handle);

    /* 获取DAC参考电压，单位：mV */
    dac_vref = am_dac_vref_get(dac0_handle);

    /* 设置DAC通道输出电压值 */
    am_dac_mv_set(dac0_handle, AMHW_FSL_DAC0_CHAN_0, mv_out);

    /* 使能DAC输出 */
    am_dac_enable(dac0_handle, AMHW_FSL_DAC0_CHAN_0);

    am_kprintf("DAC Std Demo test\n");

    am_kprintf("dac_bits = %d\n", dac_bits);
    am_kprintf("dac_vref = %d\n", dac_vref);

    while (1) {
        am_kprintf("DAC Output...\n");
        am_mdelay(500);
    }
}


 /** [src_fsl_std_dac] */

/* end of file */

