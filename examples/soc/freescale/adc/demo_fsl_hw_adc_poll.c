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
 * \brief ADC例程，利用查询方式获取软件触发的ADC转换结果，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（参考电压的大小和芯片VREF管脚连接的电平有关，此处的参考电压为2.5V）；
 *   4. PIOE_20(ADC通道0，单通道输入) 连接模拟输入。
 *
 * - 实验现象：
 *   1. ADC模块以444.44kHz采样率(总线时钟为24MHz)，进行16位采样;
 *   2. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_adc_poll.c src_fsl_hw_adc_poll
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-14  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_adc_poll
 * \copydoc demo_fsl_hw_adc_poll.c
 */

/** [src_fsl_hw_adc_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "hw/amhw_fsl_adc.h"
#include "am_board.h"

/**
 * \brief  ADC 配置函数
 * \param[in] 无
 * \return    无
 */
static void adc_config (amhw_fsl_adc_t     *p_hw_adc)
{
    /* 设置ADC模块采样率或时钟 */
    amhw_fsl_adc_clkdiv_set(p_hw_adc, AMHW_FSL_ADC_CLK_DIV_2);
    amhw_fsl_adc_clksrc_set(p_hw_adc, AMHW_FSL_ADC_CLK_SRC_BUS);

    /* 设置分辨率 */
    amhw_fsl_adc_mode_set(p_hw_adc, AMHW_FSL_ADC_BITS_16);

    /* 使用软件触发 , 默认使用序列A*/
    amhw_fsl_adc_hwtrg_disable(p_hw_adc);

    /* 使能连续采样模式 */
    amhw_fsl_adc_continue_enable(p_hw_adc);

    /* 使能高速转换模式 */
    amhw_fsl_adc_hsc_enable(p_hw_adc);

    /* 不使能低功耗 */
    amhw_fsl_adc_lowpower_disable(p_hw_adc);

    /* 关闭硬件平均和比较功能 */
    amhw_fsl_adc_cmp_disable(p_hw_adc);
    amhw_fsl_adc_avg_disable(p_hw_adc);

    /**
     * 配置成单端模式， 通道为0，不开中断，
     * 软件触发ADC转换模式时，写入ADC转换通道值，
     * 即开启ADC转换，此处先不启动转换
     */
    amhw_fsl_adc_sc1_cfg(p_hw_adc,
                          AMHW_FSL_ADC_SEQ_A,
                          AMHW_FSL_ADC_SC1_CHAN_SEL_CFG(AMHW_FSL_ADC_CHAN_CLOSE));
}

/**
 * \brief  ADC 软件启动转换函数
 * \param[in] 无
 * \return    无
 */
static void adc_start (amhw_fsl_adc_t     *p_hw_adc,
                       amhw_fsl_adc_chan_t chan)
{
    amhw_fsl_adc_chan_set(p_hw_adc, AMHW_FSL_ADC_SEQ_A, chan);
}

/**
 * \brief ADC查询获取采样值，使用HW层接口函数
 * \return 无
 */
void demo_fsl_hw_adc_poll_entry (amhw_fsl_adc_t     *p_hw_adc,
                                 amhw_fsl_adc_chan_t chan,
                                 uint32_t            clk_bus,
                                 uint32_t            vref_mv)
{
    uint32_t adc_mv;                /**< \brief 采样电压 */
    uint32_t adc_dat;               /**< \brief 采样值 */
    uint32_t cnt = 0;               /**< \brief 获取次数 */

    /* ADC校准 */
    amhw_fsl_adc_calibrate(p_hw_adc, clk_bus);

    /* ADC配置初始化 */
    adc_config(p_hw_adc);

    am_kprintf("The ADC HW Poll Demo\r\n");

    /* 连续转换使能 */
    amhw_fsl_adc_continue_enable(p_hw_adc);

    /* 选择通道0 */
    adc_start(p_hw_adc, chan);

    while(1) {

        while (!amhw_fsl_adc_complete_is(p_hw_adc, AMHW_FSL_ADC_SEQ_A));    /* 等待ADC转换完成 */
        am_led_toggle(LED1);
        adc_dat = amhw_fsl_adc_dat_get(p_hw_adc, AMHW_FSL_ADC_SEQ_A);
        cnt++;
        if (cnt >= 2000UL) { /* 延时，控制串口打印快慢  */
            cnt = 0;
            adc_mv = adc_dat * vref_mv / ((1UL << 16) - 1);
            /* 串口输出采样电压值 */
            am_kprintf("Vol: %d mv\r\n", adc_mv);
        }
    }
}
/** [src_fsl_hw_adc_poll] */

/* end of file */
