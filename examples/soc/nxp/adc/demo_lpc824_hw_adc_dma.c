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
 * \brief ADC 例程，转换结果数据通过 DMA 传输，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 短接 J12 跳线帽，使得参考电压为 2.5V；
 *   2. PIO0_7(ADC 通道 0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. ADC 转换完成 100 次，串口打印出 100 个采样电压值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_adc_dma.c src_lpc824_hw_adc_dma
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_adc_dma
 * \copydoc demo_lpc824_hw_adc_dma.c
 */

/** [src_lpc824_hw_adc_dma] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_dma.h"
#include "hw/amhw_lpc82x_adc.h"
#include "hw/amhw_lpc82x_syscon.h"

/** \brief 缓冲区大小 */
#define __ADC_BUF_LEN   100

/** \brief ADC 转换结果缓冲区 */
am_local uint32_t __g_adc_abuf[__ADC_BUF_LEN];
am_local uint32_t __g_adc_bbuf[__ADC_BUF_LEN];

/** \brief DMA 重载描述符(16 字节对其) */
am_local __attribute__((aligned(16))) am_lpc82x_dma_xfer_desc_t __g_desc[2];

/** \brief DMA 传输完成标志 */
am_local volatile am_bool_t __g_trans_adone;
am_local volatile am_bool_t __g_trans_bdone;

/** \brief DMA 传输控制器 */
am_local am_lpc82x_dma_controller_t *__gp_dma_ctr = NULL;

/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg 用户自定义参数，通过 am_lpc82x_dma_xfer_desc_startup() 函数传递
 * \param[in] state DMA 中断状态标志，由底层驱动传入，该参数的可能取值：
 *                  (#AM_LPC82X_DMA_STAT_INTERR)、(#AM_LPC82X_DMA_STAT_INTA)或
 *                  (#AM_LPC82X_DMA_STAT_INTB)或者它们的或值组合
 *
 * \return 无
 */
am_local void __dma_isr (void *p_arg, int state)
{
    if (state & AM_LPC82X_DMA_STAT_INTERR) {

        /* 传输发生错误 */
        return;
    }

    if ((state & AM_LPC82X_DMA_STAT_INTA) &&
        (state & AM_LPC82X_DMA_STAT_INTB)) {

        /* 采样率过高 */
        /* 用户需降低采样率 */
    } else if (state & AM_LPC82X_DMA_STAT_INTA) {

        /* DMA传输完成 */
        __g_trans_adone = AM_TRUE;
    } else if (state & AM_LPC82X_DMA_STAT_INTB) {
        __g_trans_bdone = AM_TRUE;
    }
}

/**
 * \brief ADC 使用 DMA 模式时启动配置
 */
am_local void __adc_dma_startup (am_lpc82x_dma_controller_t *p_dma_ctr)
{
    uint32_t dma_aflags = 0;
    uint32_t dma_bflags = 0;

    p_dma_ctr = am_lpc82x_dma_controller_get(
                    DMA_CHAN_0,
                    DMA_CHAN_OPT_PRIO_1           |         /* 通道优先级为 1 */
                    DMA_CHAN_OPT_HWTRIG_MODE_RISE |         /* 上升沿触发 */
                    DMA_CHAN_OPT_HWTRIG_BURST_EN  |         /* 使能突发模式 */
                    DMA_CHAN_OPT_HWTRIG_BURST_1   |         /* 突发传输大小为 1 */
                    DMA_CHAN_OPT_HWTRIG_SRC_ADC0_SEQA_IRQ); /* ADC0 序列 A 中断触发 DMA 传输 */

    if (p_dma_ctr == NULL) {
        AM_DBG_INFO("Geting a Controller Faild !\r\n");
        AM_FOREVER {};
    }

    /* DMA 传输配置 */
    dma_aflags = AM_LPC82X_DMA_XFER_VALID        |  /* 当前通道描述符有效 */
                 AM_LPC82X_DMA_XFER_RELOAD       |  /* 传输完成重载通道描述符 */
                 AM_LPC82X_DMA_XFER_WIDTH_32BIT  |  /* 传输数据宽度：32 位 */
                 AM_LPC82X_DMA_XFER_SRCINC_NOINC |  /* 源端数据地址不递增 */
                 AM_LPC82X_DMA_XFER_DSTINC_1X    |  /* 目标地址按 1x 宽度递增 */
                 AM_LPC82X_DMA_XFER_SETINTA;        /* 使能中断 A */

    /* DMA 传输配置 */
    dma_bflags = AM_LPC82X_DMA_XFER_VALID        |  /* 当前通道描述符有效 */
                 AM_LPC82X_DMA_XFER_RELOAD       |  /* 传输完重载通道描述符 */
                 AM_LPC82X_DMA_XFER_WIDTH_32BIT  |  /* 传输数据宽度：32 位 */
                 AM_LPC82X_DMA_XFER_SRCINC_NOINC |  /* 源端数据地址不递增 */
                 AM_LPC82X_DMA_XFER_DSTINC_1X    |  /* 目标地址按 1x 宽度递增 */
                 AM_LPC82X_DMA_XFER_SETINTB;        /* 使能中断 B */

    /* 建立通道描述符 A */
    am_lpc82x_dma_xfer_desc_build(
       &__g_desc[0],                         /* A 的重载描述符 */
        (uint32_t)(&LPC82X_ADC0->dat0), /* 源端数据地址 */
        (uint32_t)&__g_adc_abuf[0],          /* A 的目标端地址 */
        (__ADC_BUF_LEN << 2),                /* 传输字节数 */
        dma_aflags);                         /* 传输配置 */

    /* 建立通道描述符 B */
    am_lpc82x_dma_xfer_desc_build(
       &__g_desc[1],                         /* B 的重载描述符 */
        (uint32_t)(&LPC82X_ADC0->dat0), /* 源端数据地址 */
        (uint32_t)&__g_adc_bbuf[0],          /* B 的目标端地址 */
        (__ADC_BUF_LEN << 2),                /* 传输字节数 */
        dma_bflags);                         /* 传输配置 */

    /* 链接重载描述符 */
    am_lpc82x_dma_xfer_desc_link(&__g_desc[1], &__g_desc[0]);
    am_lpc82x_dma_xfer_desc_link(&__g_desc[0], &__g_desc[1]);

    /* 启动 DMA 传输，马上开始传输 */
    am_lpc82x_dma_xfer_desc_startup(p_dma_ctr,
                                   &__g_desc[0],
                                    __dma_isr,
                                    (void *)0);
}

/**
 * \brief ADC 配置，通过 HW 层函数接口实现
 */
am_local void __adc_config (amhw_lpc82x_adc_t *p_hw_adc, int ch)
{
    uint32_t seq_flags = 0;

    /* 使能序列 A 中断 */
    amhw_lpc82x_adc_int_enable(p_hw_adc,
                               AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE);

    /* 使能序列 A 转换 */
    amhw_lpc82x_adc_seq_enable(p_hw_adc, AMHW_LPC82X_ADC_SEQ_A);

    /* 256 分频 */
    amhw_lpc82x_adc_clkdiv_set(p_hw_adc, 256 - 1);

    /* 进行 DMA 传输时，NVIC 中 ADC 对应的中断必须关闭 */
    am_int_disable(INUM_ADC0_SEQA);

    seq_flags = AMHW_LPC82X_ADC_SEQ_CTRL_MODE_BURST   | /* 采用突发转换模式 */
                AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_POL_POS | /* 推荐使用正边沿触发方式 */
                AMHW_LPC82X_ADC_SEQ_CTRL_MODE_EOS     | /* 序列转换完成后触发 DMA */
                AMHW_LPC82X_ADC_SEQ_CTRL_ENA          | /* 序列 A 使能 */
                AMHW_LPC82X_ADC_SEQ_CTRL_ENABLE_CH(ch); /* 使能 ADC 通道 */

    /* ADC 序列 A 配置 */
    amhw_lpc82x_adc_seq_config(p_hw_adc,
                               AMHW_LPC82X_ADC_SEQ_A,
                               seq_flags);
}

void demo_lpc824_hw_adc_dma_entry(amhw_lpc82x_adc_t *p_hw_adc,
                                  int                ch,
                                  uint32_t        vref_mv)
{
    int i = 0;
  
    /* 启动 DMA 传输，这里需要先配置 DMA，再配置 ADC */
    __adc_dma_startup(__gp_dma_ctr);

    /* ADC 配置 */
    __adc_config(p_hw_adc, ch);

    AM_FOREVER {
        if (__g_trans_adone) {
            __g_trans_adone =  AM_FALSE;

            for (i = 0; i < 100; i++) {

                /* 串口输出采样电压 */
                AM_DBG_INFO("The buffer A conversion %2d: %4d mV \r\n",
                            i,
                            AMHW_LPC82X_ADC_DR_RESULT(__g_adc_abuf[i]) *
                            2500 / ((1 << 12) - 1));
            }
            am_mdelay(50);
        }

        if (__g_trans_bdone) {
            __g_trans_bdone =  AM_FALSE;

            for (i = 0; i < 100; i++) {

                /* 串口输出采样电压 */
                AM_DBG_INFO("The buffer B conversion %2d: %4d mV \r\n",
                            i,
                            AMHW_LPC82X_ADC_DR_RESULT(__g_adc_bbuf[i]) *
                            vref_mv / ((1 << 12) - 1));
            }
            am_mdelay(50);
        }
    }
}

/** [src_lpc824_hw_adc_dma] */

/* end of file */
