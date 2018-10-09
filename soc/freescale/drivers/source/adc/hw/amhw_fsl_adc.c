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
 * \brief ADC 硬件操作接口实现。
 *
 * 1. 16位逐次逼近型数模转换器；
 * 2. 高达4对差分输入，24个单端模拟输入；
 * 3. 单次或连续转换，转换完成自动范围空闲状态；
 * 4. 可配置采样时间，转换速度或功耗；
 * 5. 可选择高/低阈值比较和硬件平均；
 * 6. 转换完成或硬件平均完成中断请求；
 * 7. 四个可选时钟源；
 * 8. 自带温度传感器；
 * 9. 可选的参考电压源；
 * 10. 硬件校正模式;
 * 11. 2个独立触发的转换序列；
 * 12. 支持输出模式有：
 *     ---16位、13位、11位、9位差分输入模式
 *     ---16位、12位、18位、8位单端输入模式
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-15  bob, first implementation.
 * \endinternal
 */

#include "hw/amhw_fsl_adc.h"
#include "am_clk.h"
//#include "kl26_clk.h"

/******************************************************************************
* Public functions
*******************************************************************************/

/**
 * \brief 延时函数
 */
void __amhw_adc_delay (int dly)
{
    int x = 1000, y;
    while (x-- > 0) {
        y = dly;
        while (y-- > 0) {
            ;
        }
    }
}

/**
 * \brief ADC 校验, 默认矫正时钟频率为2M
 */

uint8_t amhw_fsl_adc_calibrate (amhw_fsl_adc_t *p_hw_adc, am_clk_id_t clk_id)
{
    uint16_t cal = 0;
    uint32_t cnt = 0;
    uint32_t bus_clk;
    uint8_t  adc_clkdiv;

    bus_clk = am_clk_rate_get(clk_id);		/* 获取ADC的时钟频率 */
    adc_clkdiv = bus_clk / 2000000UL;

    if (adc_clkdiv <= 1) {
        adc_clkdiv = AMHW_FSL_ADC_CLK_DIV_1;
    } else if (adc_clkdiv <= 2) {
        adc_clkdiv = AMHW_FSL_ADC_CLK_DIV_2;
    } else if (adc_clkdiv <= 4) {
        adc_clkdiv = AMHW_FSL_ADC_CLK_DIV_4;
    } else {
        adc_clkdiv = AMHW_FSL_ADC_CLK_DIV_8;
    }
    amhw_fsl_adc_clkdiv_set(p_hw_adc, (amhw_fsl_adc_clk_div_t)adc_clkdiv);
    amhw_fsl_adc_clksrc_set(p_hw_adc, AMHW_FSL_ADC_CLK_SRC_BUS);


    /* 设置硬件平均 */
    amhw_fsl_adc_avgs_set(p_hw_adc, AMHW_FSL_ADC_AVGS_32);
    amhw_fsl_adc_avg_enable(p_hw_adc);

    /* 配置SC2, 软件触发 */
    amhw_fsl_adc_hwtrg_disable(p_hw_adc);
    /* 使能矫正 ， 开始矫正 */
    amhw_fsl_adc_cal_enable(p_hw_adc);

    /* 循环在等待 ，直到矫正完成 */
    while (amhw_fsl_adc_cal_get(p_hw_adc)) { /* 正常矫正花费2ms左右 */
        __amhw_adc_delay(1);
    }

    if (amhw_fsl_adc_cal_fail_is(p_hw_adc)) {
        /* 矫正失败 */
        return AM_FALSE;
    }

    cal = 0;
    for (cnt = 1; cnt < 7; cnt++) {
        cal += amhw_fsl_adc_clp_get(p_hw_adc, cnt);
    }
    cal >>= 1;
    cal |= 0x8000;
    amhw_fsl_adc_pg_set(p_hw_adc, cal);

    cal = 0;
    for (cnt = 1; cnt < 7; cnt++) {
       cal += amhw_fsl_adc_clm_get(p_hw_adc, cnt);
    }
    cal >>= 1;
    cal |= 0x8000;
    amhw_fsl_adc_mg_set(p_hw_adc, cal);
    return AM_TRUE;
}

/* end of file */
