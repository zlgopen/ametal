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
#include "am_hk32f103rbt6_clk.h"
#include "hw/amhw_hk32f103rbt6_adc.h"

static volatile uint16_t  __g_adc_dat[4];               /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;

/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    static uint8_t i = 0;

    amhw_hk32f103rbt6_adc_t *p_hw_adc = (amhw_hk32f103rbt6_adc_t *)p_arg;

    amhw_hk32f103rbt6_adc_status_flag_clr(p_hw_adc, AMHW_HK32F103RBT6_ADC_INJECTED_CHAN_END_FLAG);

    for (i = 0 ; i < 4; i++) {

        /* AD值的获取 */
        /* 特别注意 ：从数据寄存器的读到的值 = AD实际转换值 - 数据偏移寄存器的偏移值*/
        __g_adc_dat[i]  = amhw_hk32f103rbt6_adc_injected_data_get(p_hw_adc, i);
        __g_adc_dat[i] &= 0x0fff;
    }

    __g_adc_complete = AM_TRUE;
}

/** \brief 例程入口  */
void demo_hk32f103rbt6_hw_adc_injected_int_entry (amhw_hk32f103rbt6_adc_t *p_hw_adc,
                                            int                int_num,
                                            uint32_t           vref_mv,
                                            int               *p_adc_chan,
                                            int                adc_chan_num)
{
    uint8_t  i      = 0;
    uint32_t adc_mv = 0;    /* 采样电压 */

    am_kprintf("The ADC HW Int Demo\r\n");

    /* ADC禁能 */
    amhw_hk32f103rbt6_adc_disable(p_hw_adc);

    /* 设置ADC工作频率，72MHz的8分频 */
    amhw_hk32f103rbt6_rcc_adc_div_set (2);

    /* 设置ADC注入通道长度 */
    if(adc_chan_num < 4) {
        amhw_hk32f103rbt6_adc_injected_channel_length_set(
            p_hw_adc,
            (amhw_hk32f103rbt6_adc_injected_channel_length_t)(adc_chan_num-1));
    }
    else
    {
        amhw_hk32f103rbt6_adc_injected_channel_length_set(
            p_hw_adc, AMHW_HK32F103RBT6_ADC_INJECTED_CHAN_LENGTH_4);

    }

    /**
     * \note : 不同于规则转换序列，如果 JL[1:0]的长度小于 4，则转换的序列顺序是从(4-JL)开始。
     *         例如：ADC_JSQR[21:0] =   10    00011   00011   00111  00010
     *                               3个转换            3       3       7      2
     *         意味着扫描转换将按下列通道顺序转换：7、 3、3，而不是 2、7、3。
     *
     *         所以下面的通道序列与ADC采样通道的链接是反向的
     */
    for(i=0;i<adc_chan_num;i++) {

    /* 链接注入通道序列和ADC采样通道 */
    amhw_hk32f103rbt6_adc_injected_channel_order_set(
        p_hw_adc,
        AMHW_HK32F103RBT6_ADC_INJECTED_CHAN_ORDER_4th - i,
        p_adc_chan[adc_chan_num - 1 - i]);

    /* 注入通道数据偏移寄存器赋值（复位值为0） */
    /* 数据寄存器的值 = 实际转换值 - 偏移值*/
    amhw_hk32f103rbt6_adc_jofr_set(
        p_hw_adc,
        AMHW_HK32F103RBT6_ADC_INJECTED_DATA_CHAN_4 - i,
        0);

    /* 设置采样通道的采样周期 */
    amhw_hk32f103rbt6_adc_smpr_set(
        p_hw_adc,
        AMHW_HK32F103RBT6_ADC_CHAN_ST55_5,
        p_adc_chan[adc_chan_num - 1 - i]);

    }

    /* 开启扫描模式 */
    amhw_hk32f103rbt6_adc_scan_mode_enable(p_hw_adc);

    /* 关闭注入通道间断模式、规则通道间断模式*/
    amhw_hk32f103rbt6_adc_injected_disc_disable(p_hw_adc);
    amhw_hk32f103rbt6_adc_regular_disc_disable(p_hw_adc);

    /* 单次转换 */
    amhw_hk32f103rbt6_adc_cont_set(p_hw_adc, AMHW_HK32F103RBT6_ADC_CONVERSION_SINGLE);

    /* 启用注入通道外部触发，并设置成软件触发方式 */
    amhw_hk32f103rbt6_adc_jextirig_enable(p_hw_adc);
    amhw_hk32f103rbt6_adc_jextsel_set(p_hw_adc, AMHW_HK32F103RBT6_ADC12_INJECTED_JSWSTART);

    /* 设置成独立模式,ADC1和ADC2独立工作*/
    amhw_hk32f103rbt6_adc_dul_mode_set(p_hw_adc, AMHW_HK32F103RBT6_ADC_DUL_MODE_0);

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
    amhw_hk32f103rbt6_adc_int_enable(p_hw_adc, AMHW_HK32F103RBT6_ADC_INT_INJECTED_END);

    while(1) {

        /* 开始转换 */
        amhw_hk32f103rbt6_adc_jswstart_enable(p_hw_adc);

        /* 等待转换完成 */
        while (__g_adc_complete == AM_FALSE);

        for (i = 0; i < 4 ; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[i] * vref_mv / ((1UL << 12) -1);

            am_kprintf("chan : %d, Sample : %d, Vol: %d mv\r\n", p_adc_chan[i], __g_adc_dat[i], adc_mv);
        }

        am_kprintf("\r\n");

        am_mdelay(500);
    }

}
/** [src_hk32f103rbt6_hw_adc_int] */

/* end of file */
