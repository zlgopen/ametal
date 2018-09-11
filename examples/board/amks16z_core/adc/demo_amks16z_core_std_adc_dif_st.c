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
 * \brief ADC例程，软件触发转换，DMA传输差分转换结果，通过标准接口实现
 *
 * \note 使用此demo需要配置使能am_kl26_hwconfig_adc0.c文件中的AM_FSL_ADC_DIF_EN宏
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_20(ADC_DP0)和PIOE_21(ADC_DM0)连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_adc_dif_st.c src_amks16z_core_std_adc_dif_st
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-19  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_std_adc_dif_st
 * \copydoc demo_amks16z_core_std_adc_dif_st.c
 */

/** [src_amks16z_core_std_adc_dif_st] */
#include "demo_fsl_entrys.h"
#include "ametal.h"
#include "am_adc.h"
#include "am_vdebug.h"
#include "am_kl26_inst_init.h"
#include "am_board.h"

/**
 * \brief ADC差分输入，DMA传输，软件触发
 * \return 无
 */
void demo_amks16z_core_std_adc_dif_st_entry (void)
{
    am_adc_handle_t adc0_handle;

    AM_DBG_INFO("demo amks16z_core std adc dif!\r\n");

    am_kl26_dma_inst_init();        /* DMA实例初始化 */
    adc0_handle = am_kl26_adc0_inst_init(); /* ADC实例初始化 */

    demo_fsl_std_adc_dif_st_entry(adc0_handle);
}

/** [src_amks16z_core_std_adc_dif_st] */

/* end of file */
