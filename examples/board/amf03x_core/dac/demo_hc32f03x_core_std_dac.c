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
 * \snippet demo_hc32f03x_std_dac.c src_hc32f03x_std_dac
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  ly, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f03x_std_dac
 * \copydoc demo_hc32f03x_std_dac.c
 */

 /** [src_std_dac] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f03x_inst_init.h"
#include "demo_std_entries.h"

const static uint32_t mv_out = 1523;

/**
 * \brief DAC输出电压，标准接口层实现
 * \return 无
 */
void demo_hc32f03x_core_std_dac_entry (void)
{
    am_dac_handle_t adc_handle = NULL;
    
    AM_DBG_INFO("demo amf03x_core std dac convert test!\r\n");
    
    adc_handle = am_hc32_dac_inst_init();

    demo_std_dac_entry(adc_handle, 0, mv_out);
}


 /** [src_std_dac] */

/* end of file */

