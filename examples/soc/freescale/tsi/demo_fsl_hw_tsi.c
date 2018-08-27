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
 * \brief   TSI触摸检测，硬件层实现
 *
 *  实验现象：
 *  1.当手不接触PIOA_1引脚时，输出检测到的基本值
 *  2.当触摸PIOA_1引脚后，检测到的值变大，并串口输出
 *
 *  注意：
 *  1.运行该例程时PIOA_1不能配置成串口引脚。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_tsi.c src_fsl_hw_tsi
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-26  nwt, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_fsl_if_hw_tsi
 * \copydoc demo_fsl_hw_tsi.c
 */
 
/** [src_fsl_hw_tsi] */
#include "ametal.h"
#include "hw/amhw_fsl_tsi.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
    
/**
 * \brief 主函数入口
 */
void demo_fsl_hw_tsi_entry (amhw_fsl_tsi_t *p_hw_tsi)
{  
    uint16_t value = 0;

    /* 设置外部振荡器恒定电流为16uA */
    amhw_fsl_tsi_extchrg_val_set(p_hw_tsi, AMHW_FSL_TSI_EXTCHRG_16UA);

    /* 设置电压增量为600mV (Vp - Vm)*/
    amhw_fsl_tsi_dvolt_set (p_hw_tsi, AMHW_FSL_TSI_DV073);

    /* 扫描预分频设置为2分频 */
    amhw_fsl_tsi_oscfreq_div_set(p_hw_tsi, AMHW_FSL_TSI_OSCFRQ_DIV_2);

    /* 设置电极扫描次数为16 */
    amhw_fsl_tsi_scan_num_set(p_hw_tsi, 16);

    /* 设置参考振荡器充放电电流为32uA */
    amhw_fsl_tsi_refchrg_val_set(p_hw_tsi, AMHW_FSL_TSI_REFCHRG_32UA);

    /* 中断禁能 */
    amhw_fsl_tsi_int_disable(p_hw_tsi);

    /* 配置触发扫描模式 */
    amhw_fsl_tsi_scan_trig_set(p_hw_tsi, AMHW_FSL_TSI_TRIG_SOFTWARE);

    /* 使能TSI通道 */
    amhw_fsl_tsi_chan_set(p_hw_tsi, AMHW_FSL_TSI_CHAN2);

    /* 使能TSI模块 */
    amhw_fsl_tsi_mode_enable(p_hw_tsi);
    
    while (1) {

        /* 启动软件触发扫描 */
        amhw_fsl_tsi_soft_tirg_start(p_hw_tsi);

        /* 获取扫描结束标志,等待扫描结束 */
        while ( !amhw_fsl_tsi_eosf_get(p_hw_tsi));

        /* 读取检测结结果 */
        value = amhw_fsl_tsi_convert_value_get(p_hw_tsi);

        /* 清扫描结束标志 */
        amhw_fsl_tsi_eosf_clr(p_hw_tsi);

        am_kprintf("%d\r\n",value);
        am_led_toggle (LED1);
        am_mdelay(1000);

    }
}

/** [src_fsl_hw_tsi] */
 
/* end of file */

