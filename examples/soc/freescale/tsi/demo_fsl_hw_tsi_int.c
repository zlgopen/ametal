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
 * \brief   TSI触摸中断检测，硬件层实现
 *
 *  实验现象：
 *  1.当手不接触PIOA_1引脚时，输出检测到的基本值
 *  2.当触摸PIOA_1引脚后，电容检测大于阀值上限1000时，进入中断，LED1状态翻转
 *
 *  注意：
 *  1.运行该例程时PIOA_1不能配置成串口引脚。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_tsi_int.c src_fsl_hw_tsi_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_fsl_if_hw_tsi_int
 * \copydoc demo_fsl_hw_tsi_int.c
 */
 
/** [src_fsl_hw_tsi_int] */
#include "ametal.h"
#include "hw/amhw_fsl_tsi.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_led.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
    
static void tsi_isr(void *p_arg)
{
    amhw_fsl_tsi_t *p_hw_tsi = (amhw_fsl_tsi_t *)p_arg;

    /* 清超阀值标志 */
    amhw_fsl_tsi_outrgf_clr(p_hw_tsi);
       
    /* 清扫描结束标志 */
    amhw_fsl_tsi_eosf_clr(p_hw_tsi);
    am_led_toggle(LED1);
}

/**
 * \brief 主函数入口
 */
void demo_fsl_hw_tsi_int_entry(amhw_fsl_tsi_t *p_hw_tsi,
                               int             inum)
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

    /* 配置触发扫描模式 */
    amhw_fsl_tsi_scan_trig_set(p_hw_tsi, AMHW_FSL_TSI_TRIG_SOFTWARE);

    /* 使能TSI通道 */
    amhw_fsl_tsi_chan_set(p_hw_tsi, AMHW_FSL_TSI_CHAN2);

    /* 使能TSI模块 */
    amhw_fsl_tsi_mode_enable(p_hw_tsi);
    
    /* 连接TSI中断服务函数 */
    am_int_connect(inum, tsi_isr, p_hw_tsi);
    
    /* TSI中断使能 */
    amhw_fsl_tsi_int_enable(p_hw_tsi);
    
    /* 系统中断使能 */
    am_int_enable(inum);
    
    /* 设置阀值上限 */
    amhw_fsl_tsi_wake_chan_thresholdh_write(p_hw_tsi, 1000);
    
    /* 设置阀值下限 */
    amhw_fsl_tsi_wake_chan_thresholdl_write(p_hw_tsi, 100);
       
    while (1) {

        /* 启动软件触发扫描 */
        amhw_fsl_tsi_soft_tirg_start(p_hw_tsi);
        
        /* 读取检测结结果 */
        value = amhw_fsl_tsi_convert_value_get(p_hw_tsi);

        am_kprintf("%d\r\n",value);
        am_mdelay(100);

    }
}

/** [src_fsl_hw_tsi_int] */
 
/* end of file */

