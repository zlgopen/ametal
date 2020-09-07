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
 * \brief ADC INT 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将指定ADC通道对应的引脚连接模拟输入。
 *
 * - 实验现象：
 *   1. 调试串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_hw_adc_int.c src_hk32f103rbt6_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-18  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hk32f103rbt6_hw_adc_int
 * \copydoc demo_hk32f103rbt6_hw_adc_int.c
 */

/** [src_hk32f103rbt6_hw_adc_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_int.h"
#include "hw/amhw_hk32f103rbt6_adc.h"

static volatile uint16_t  __g_adc_dat[10];              /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;

/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    static uint8_t i = 0;

    amhw_hk32f103rbt6_adc_t *p_hw_adc = (amhw_hk32f103rbt6_adc_t *)p_arg;

    __g_adc_dat[i]  = amhw_hk32f103rbt6_adc_regular_data_get(p_hw_adc);
    __g_adc_dat[i] &= 0x0fff;

    i++;

    amhw_hk32f103rbt6_adc_swstart_enable(p_hw_adc);

    if (i == 10) {
        i = 0;
        __g_adc_complete = AM_TRUE;

        amhw_hk32f103rbt6_adc_swstart_disable(p_hw_adc);

    }
}

/** \brief 例程入口  */
void demo_hk32f103rbt6_hw_adc_int_entry (amhw_hk32f103rbt6_adc_t *p_hw_adc,
                                   int                int_num,
                                   int                chan,
                                   uint32_t           vref_mv)
{
    uint8_t  i      = 0;
    uint32_t adc_mv = 0;    /* 采样电压 */
    uint32_t sum    = 0;

    am_kprintf("The ADC HW Int Demo\r\n");

    /* ADC禁能 */
    amhw_hk32f103rbt6_adc_disable(p_hw_adc);

    /* 关闭扫描模式、注入通道间断模式、规则通道间断模式*/
    amhw_hk32f103rbt6_adc_scan_mode_disable(p_hw_adc);
    amhw_hk32f103rbt6_adc_injected_disc_disable(p_hw_adc);
    amhw_hk32f103rbt6_adc_regular_disc_disable(p_hw_adc);

    /* 单次转换 */
    amhw_hk32f103rbt6_adc_cont_set(p_hw_adc, AMHW_HK32F103RBT6_ADC_CONVERSION_SINGLE);
    amhw_hk32f103rbt6_adc_extsel_set(p_hw_adc, AMHW_HK32F103RBT6_ADC12_REGULAR_SWSTART);

    /* 设置成独立模式*/
    amhw_hk32f103rbt6_adc_dul_mode_set(p_hw_adc, AMHW_HK32F103RBT6_ADC_DUL_MODE_0);

    /* 设置某一通道的采样时间*/
    amhw_hk32f103rbt6_adc_smpr_set(p_hw_adc,
                             AMHW_HK32F103RBT6_ADC_CHAN_ST239_5,
                             AMHW_HK32F103RBT6_ADC_CHAN_0);

    /* 设置规则通道个数  1个 */
    amhw_hk32f103rbt6_adc_regular_channel_length_set(
        p_hw_adc, AMHW_HK32F103RBT6_ADC_REGULAR_CHAN_LENGTH_1);

    /* 连接规则序列通道和ADC采样通道 */
    amhw_hk32f103rbt6_adc_regular_channel_order_set(
        p_hw_adc,
        AMHW_HK32F103RBT6_ADC_REGULAR_CHAN_ORDER_1st,
        AMHW_HK32F103RBT6_ADC_CHAN_0);

    /* 启用规则通道外部触发，并设置成软件触发方式 */
    amhw_hk32f103rbt6_adc_extirig_enable(p_hw_adc);
    amhw_hk32f103rbt6_adc_extsel_set(p_hw_adc, AMHW_HK32F103RBT6_ADC12_REGULAR_SWSTART);

    /* 对齐方式--右对齐*/
    amhw_hk32f103rbt6_adc_data_alignment_set(p_hw_adc,AMHW_HK32F103RBT6_ADC_DATA_RIGHT);

    /* 关闭ADC16通道内部温度传感器（仅ADC1有效）*/
    amhw_hk32f103rbt6_adc_tsvrefe_disable(p_hw_adc);

    /* ADC使能*/
    amhw_hk32f103rbt6_adc_enable(p_hw_adc);

    /* 启用复位校准*/
    amhw_hk32f103rbt6_adc_rstcal_enable(p_hw_adc);

    /* 等待复位校准结束*/
    while(amhw_hk32f103rbt6_adc_rstcal_check(p_hw_adc) == AM_FALSE);

    /* 启用AD校准*/
    amhw_hk32f103rbt6_adc_cal_enable(p_hw_adc);

    /* 等待AD校准结束*/
    while(amhw_hk32f103rbt6_adc_cal_check(p_hw_adc) == AM_FALSE);

    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_hw_adc);
    am_int_enable(int_num);

    /* 转换结束中断使能*/
    amhw_hk32f103rbt6_adc_int_enable(p_hw_adc, AMHW_HK32F103RBT6_ADC_INT_END);

    while(1) {

        /* 开始转换 */
        amhw_hk32f103rbt6_adc_swstart_enable(p_hw_adc);

        /* 等待转换完成 */
        while (__g_adc_complete == AM_FALSE);

        for (sum = 0,i = 0; i < 10 ; i++) {
        sum += __g_adc_dat[i];
        }

        sum = sum / 10;

        /* 转换为电压值对应的整数值 */
        adc_mv = sum * 3300 / ((1UL << 12) -1);

        am_kprintf("Sample : %d, Vol: %d mv\r\n", sum, adc_mv);

        am_mdelay(500);
    }

}
/** [src_hk32f103rbt6_hw_adc_int] */

/* end of file */
