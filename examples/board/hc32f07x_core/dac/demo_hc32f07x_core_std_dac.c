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
 *   1. DAC输出1523mv电压。
 *
 *
 * \par 源代码
 * \snippet demo_hc32f07x_std_dac.c src_hc32f07x_std_dac
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  ly, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f07x_std_dac
 * \copydoc demo_hc32f07x_std_dac.c
 */

 /** [src_std_dac] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f07x_inst_init.h"
#include "demo_std_entries.h"
#include "am_hc32_opa.h"
#include "hw/amhw_hc32f07x_dac.h"
#include "hw/amhw_hc32_opa.h"
const static uint32_t mv_out = 1520;

/**
 * \brief DAC输出电压，标准接口层实现
 * \return 无
 */
void demo_hc32f07x_core_std_dac_entry (void)
{
    am_dac_handle_t adc0_handle = NULL;

    AM_DBG_INFO("demo amf07x_core std dac convert test!\r\n");
	
    adc0_handle = am_hc32_dac0_inst_init();
    demo_std_dac_entry(adc0_handle, 0, mv_out);
}


 /** [src_std_dac] */

/* end of file */

