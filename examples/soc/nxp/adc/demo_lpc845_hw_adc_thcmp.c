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
 * \brief ADC 阈值比较例程，通过 HW 层接口实现
 *
 * - 本例程展示了 2 种阈值比较：
 *   1. 阈值比较 0: 范围比较；
 *   2. 阈值比较 1: 穿越比较。
 *
 *
 * - 实验现象：
 *   1. 模拟输入 1 的电压低于 1100mV 或高于 2000mV 时，串口会输出电压值及比较结果；
 *   2. 模拟输入 2 的电压变化经过 1100mV 时，串口会输出电压值及比较结果。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc845_hw_adc_thcmp.c src_lpc845_hw_adc_thcmp
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc845_hw_adc_thcmp
 * \copydoc demo_lpc845_hw_adc_thcmp.c
 */

/** [src_lpc845_hw_adc_thcmp] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_adc.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** \brief 低比较阈值(mV) */
#define __THRE_LOW            ((1100 * 4095) / vref_mv)

/** \brief 高比较阈值(mV) */
#define __THRE_HIGH           ((2000 * 4095) / vref_mv)

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

am_local volatile am_bool_t __g_tc0_flag; /** \brief 阈值比较 0 中断标志 */
am_local volatile am_bool_t __g_tc1_flag; /** \brief 阈值比较 1 中断标志 */

am_local volatile uint32_t __g_val_chan0; /** \brief ADC 通道 0 数据缓冲区 */
am_local volatile uint32_t __g_val_chan1; /** \brief ADC 通道 1 数据缓冲区 */

/**
 * \brief ADC 中断服务函数
 */
am_local void __adc_isr (void *p_arg)
{
    uint32_t            int_flags =  0;
    amhw_lpc84x_adc_t  *p_hw_adc  = (amhw_lpc84x_adc_t *)p_arg;
  
    int_flags = amhw_lpc84x_adc_flags_get(p_hw_adc);

    /* 是否为阈值比较中断 */
    if (int_flags & AMHW_LPC84X_ADC_FLAGS_THCMP_INT_MASK) {

        /* 判断是否为 ADC 通道 0 阈值比较中断 */
        if (int_flags &
            AMHW_LPC84X_ADC_FLAGS_THCMP_MASK(AMHW_LPC84X_ADC_CHAN_0)) {
            __g_tc0_flag  = AM_TRUE;
            __g_val_chan0 = amhw_lpc84x_adc_chan_data_get(
                                p_hw_adc, AMHW_LPC84X_ADC_CHAN_0);
            amhw_lpc84x_adc_flags_clr(
                p_hw_adc,
                AMHW_LPC84X_ADC_FLAGS_THCMP_MASK(AMHW_LPC84X_ADC_CHAN_0));
        }

        /* 判断是否为 ADC 通道 1 阈值比较中断 */
        if (int_flags &
            AMHW_LPC84X_ADC_FLAGS_THCMP_MASK(AMHW_LPC84X_ADC_CHAN_1)) {
            __g_tc1_flag  = AM_TRUE;
            __g_val_chan1 = amhw_lpc84x_adc_chan_data_get(
                                p_hw_adc, AMHW_LPC84X_ADC_CHAN_1);
            amhw_lpc84x_adc_flags_clr(
                p_hw_adc,
                AMHW_LPC84X_ADC_FLAGS_THCMP_MASK(AMHW_LPC84X_ADC_CHAN_1));
        }

        amhw_lpc84x_adc_flags_clr(p_hw_adc,
                                  AMHW_LPC84X_ADC_FLAGS_THCMP_INT_MASK);
    }
}

/**
 * \brief ADC 配置
 */
am_local void __adc_config (amhw_lpc84x_adc_t  *p_hw_adc,
                            uint32_t            vref_mv)
{
    uint32_t adc_flags = 0;
    uint32_t seq_flags = 0;

    adc_flags = AMHW_LPC84X_ADC_CTRL_CLK_DIV(0);   /* 时钟分频为0（1 分频） */

    /* ADC 配置 */
    amhw_lpc84x_adc_config(p_hw_adc, adc_flags);

    seq_flags = AMHW_LPC84X_ADC_SEQ_CTRL_TRIG_POL_POS |  /* 使用正边沿触发方式 */
                AMHW_LPC84X_ADC_SEQ_CTRL_MODE_EOS     |  /* 序列转换模式 */
                AMHW_LPC84X_ADC_SEQ_CTRL_ENABLE_CH(1) |  /* 使能 ADC 通道 1 */
                AMHW_LPC84X_ADC_SEQ_CTRL_ENABLE_CH(0);   /* 使能 ADC 通道 0 */

    /* ADC 序列 A 配置 */
    amhw_lpc84x_adc_seq_config(p_hw_adc,
                               AMHW_LPC84X_ADC_SEQ_A, seq_flags);

    /* 使能中断 */
    amhw_lpc84x_adc_int_enable(
        p_hw_adc,
        AMHW_LPC84X_ADC_INTEN_CMP_OUTSIDETH(AMHW_LPC84X_ADC_CHAN_0) |
        AMHW_LPC84X_ADC_INTEN_CMP_CROSSTH(AMHW_LPC84X_ADC_CHAN_1));

    /* 为通道选择阈值比较 */
    amhw_lpc84x_adc_thr0_sel(p_hw_adc, AMHW_LPC84X_ADC_CHAN_0);
    amhw_lpc84x_adc_thr1_sel(p_hw_adc, AMHW_LPC84X_ADC_CHAN_1);

    /* 设置比较阈值 */
    amhw_lpc84x_adc_thr_low_set(p_hw_adc,
                                AMHW_LPC84X_ADC_COMP_THRE_0,
                                __THRE_LOW);
    amhw_lpc84x_adc_thr_high_set(p_hw_adc,
                                 AMHW_LPC84X_ADC_COMP_THRE_0,
                                 __THRE_HIGH);

    amhw_lpc84x_adc_thr_low_set(p_hw_adc,
                                AMHW_LPC84X_ADC_COMP_THRE_1,
                                __THRE_LOW);

    /* 使能序列 A */
    amhw_lpc84x_adc_seq_enable(p_hw_adc, AMHW_LPC84X_ADC_SEQ_A);
}

void demo_lpc845_hw_adc_thcmp_entry(amhw_lpc84x_adc_t  *p_hw_adc,
                                    int                 inum,
                                    uint32_t            vref_mv)
{
    /* 连接 ADC 中断服务函数，并使能中断 */
    am_int_connect(inum, __adc_isr, (void *)p_hw_adc);
    am_int_enable(inum);

    /* ADC 配置 */
    __adc_config(p_hw_adc, vref_mv);

    AM_FOREVER {
        amhw_lpc84x_adc_seq_start(p_hw_adc, AMHW_LPC84X_ADC_SEQ_A);

        if (__g_tc0_flag == AM_TRUE) {
            __g_tc0_flag = AM_FALSE;

            AM_DBG_INFO("The ADC channel 0 voltage: %4d mV \r\n",
                        AMHW_LPC84X_ADC_DR_RESULT(__g_val_chan0) * vref_mv / 4095);

            /* 判断通道 0 阈值比较结果 */
            if (AMHW_LPC84X_ADC_DR_THCMPRANGE(__g_val_chan0) == 0x00) {
                AM_DBG_INFO("The ADC channel 0 voltage is in range.\r\n");

            } else if (AMHW_LPC84X_ADC_DR_THCMPRANGE(__g_val_chan0) == 0x01) {
                AM_DBG_INFO("The ADC channel 0 voltage is blow range.\r\n");

            } else if (AMHW_LPC84X_ADC_DR_THCMPRANGE(__g_val_chan0) == 0x02) {
                AM_DBG_INFO("The ADC channel 0 voltage is above range.\r\n");
            }
        }

        if (__g_tc1_flag == AM_TRUE) {
            __g_tc1_flag = AM_FALSE;

            AM_DBG_INFO("The ADC channel 1 voltage: %4d mV \r\n",
                        AMHW_LPC84X_ADC_DR_RESULT(__g_val_chan1) * vref_mv / 4095);

            /* 判断通道 1 阈值比较结果 */
            if (AMHW_LPC84X_ADC_DR_THCMPCROSS(__g_val_chan1) == 0x00) {
                AM_DBG_INFO("The ADC channel 1 voltage "
                            "no threshold Crossing detected.\r\n");

            } else if (AMHW_LPC84X_ADC_DR_THCMPCROSS(__g_val_chan1) == 0x02) {
                AM_DBG_INFO("The ADC channel 1 voltage "
                            "downward Threshold Crossing Detected.\r\n");

            } else if (AMHW_LPC84X_ADC_DR_THCMPCROSS(__g_val_chan1) == 0x03) {
                AM_DBG_INFO("The ADC channel 1 voltage "
                            "upward Threshold Crossing Detected.\r\n");
            }
        }
        am_mdelay(200);
    }
}
/** [src_lpc845_hw_adc_thcmp] */

/* end of file */
