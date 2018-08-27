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
 * \brief ADC例程，利用中断方式获取软件触发的ADC转换结果，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_29(ADC通道0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. ADC模块以444.44kHz采样率(总线时钟为24MHz)，进行16位采样;
 *   2. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_adc_int.c src_fsl_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-18  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_adc_int
 * \copydoc demo_fsl_hw_adc_int.c
 */

/** [src_fsl_hw_adc_int] */
#include "hw/amhw_fsl_adc.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_board.h"

static volatile uint32_t __g_adc_dat;             /**< \brief 采样值 */
static volatile uint32_t __g_adc_finish_flag;     /**< \brief ADC采样完成标识 */

extern int am_int_disconnect (int inum, am_pfnvoid_t pfn_isr, void *p_arg);
extern int am_int_connect (int inum, am_pfnvoid_t pfn_isr, void *p_arg);
extern int am_int_enable (int inum);
extern int am_int_disable (int inum);


/**
 * \brief  ADC 中断服务函数
 */
static void adc_isr (void *p_arg)
{
    amhw_fsl_adc_t *p_hw_adc = (amhw_fsl_adc_t *)p_arg;

    __g_adc_dat = amhw_fsl_adc_dat_get(p_hw_adc, AMHW_FSL_ADC_SEQ_A);
    __g_adc_finish_flag = 1;
}

/**
 * \brief  ADC 配置函数
 * \param[in] 无
 * \return    无
 */
static void adc_config (amhw_fsl_adc_t     *p_hw_adc,
                        int                 int_num)
{
    /* 设置ADC模块采样率或时钟 */
    amhw_fsl_adc_clkdiv_set(p_hw_adc, AMHW_FSL_ADC_CLK_DIV_2);
    amhw_fsl_adc_clksrc_set(p_hw_adc, AMHW_FSL_ADC_CLK_SRC_BUS);

    /* 设置分辨率 */
    amhw_fsl_adc_mode_set(p_hw_adc, AMHW_FSL_ADC_BITS_16);

    /* 使用软件触发 , 默认使用序列A */
    amhw_fsl_adc_hwtrg_disable(p_hw_adc);

    /* 使能连续采样模式 */
    amhw_fsl_adc_continue_enable(p_hw_adc);

    /* 使能高速转换模式 */
    amhw_fsl_adc_hsc_enable(p_hw_adc);

    /* 不使能低功耗 */
    amhw_fsl_adc_lowpower_disable(p_hw_adc);

    /* 关闭比较功能 */
    amhw_fsl_adc_cmp_disable(p_hw_adc);

    /* 使能硬件平均 */
    amhw_fsl_adc_avg_enable(p_hw_adc);
    amhw_fsl_adc_avgs_set(p_hw_adc, AMHW_FSL_ADC_AVGS_32);

    /* 配置中断服务函数，并使能 */
    am_int_disconnect(int_num, adc_isr, p_hw_adc);
    am_int_connect(int_num, adc_isr, p_hw_adc);
    am_int_enable(int_num);

    /* 配置成单端模式， 通道为0，开中断，同时启动触发，先关闭通道 */
    amhw_fsl_adc_sc1_cfg(p_hw_adc,
                         AMHW_FSL_ADC_SEQ_A,
                         AMHW_FSL_ADC_SC1_CHAN_SEL_CFG(AMHW_FSL_ADC_CHAN_CLOSE) |
                         AMHW_FSL_ADC_SC1_INT_EN_CFG);
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
 * \brief 中断方式获取软件触发的ADC转换结果，通过HW层接口实现
 * \return 无
 */
void demo_fsl_hw_adc_int_entry (amhw_fsl_adc_t     *p_hw_adc,
                                int                 int_num,
                                amhw_fsl_adc_chan_t chan,
                                uint32_t            clk_bus,
                                uint32_t            vref_mv)
{
    uint32_t adc_mv = 0;                /**< \brief 采样电压 */

    /* ADC校准 */
    amhw_fsl_adc_calibrate(p_hw_adc, clk_bus);

    /* ADC配置初始化 */
    adc_config(p_hw_adc, int_num);

    am_kprintf("The ADC HW Int Demo\r\n");

    /* 选择通道B */
    amhw_fsl_adc_seq_set(p_hw_adc, AMHW_FSL_ADC_MUXSEL_B);

    /* 选择通道4 */
    adc_start(p_hw_adc, chan);

    while(1) {
        am_int_disable(int_num);
        if (__g_adc_finish_flag) {
            __g_adc_finish_flag = 0;

            adc_mv = __g_adc_dat * 2500 / ((1UL << 16) - 1);
            am_int_enable(int_num);

            am_kprintf("Vol: %d mv\r\n", adc_mv);
            am_mdelay(100);        /* 如果用PL2102驱动的话，需要延时一段时间，不然电脑很容易蓝屏 */
        } else {
            am_int_enable(int_num);
        }
    }
}
/** [src_fsl_hw_adc_int] */

/* end of file */
