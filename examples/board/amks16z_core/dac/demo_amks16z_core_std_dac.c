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
 * \snippet demo_amks16z_core_std_dac.c src_amks16z_core_std_dac
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-27  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_std_dac
 * \copydoc demo_amks16z_core_std_dac.c
 */

 /** [src_amks16z_core_std_dac] */
#include "ametal.h"
#include "am_dac.h"
#include "am_fsl_dac.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_kl26_inst_init.h"
#include "demo_fsl_entrys.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief DAC输出电压，标准接口层实现
 * \return 无
 */
void demo_amks16z_core_std_dac_entry (void)
{
    uint32_t        mv_out      = 1523; /* 需要输出的电压值  单位：mv   */
    am_dac_handle_t dac0_handle = NULL; /* ADC标准服务操作句柄 */

    /* DAC实例初始化，并获取DAC句柄值 */
    dac0_handle = am_kl26_dac0_inst_init();

    demo_fsl_std_dac_entry(dac0_handle, mv_out);
}


 /** [src_amks16z_core_std_dac] */

/* end of file */

