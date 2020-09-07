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
 * \brief ADC1 和 ADC2同时采集的例程，通过 HW 层接口实现
 *
 *        ADC1 使用 规则组 DMA通道, ADC2 使用 注入组 中断通道
 *
 * - 操作步骤：
 *   1. PIOA_2 (ADC 通道 2) 连接模拟输入。
 *   2. PIOA_3 (ADC 通道 3) 连接模拟输入。
 *   3. PIOA_4 (ADC 通道 4) 连接模拟输入。
 *   4. PIOA_5 (ADC 通道 5) 连接模拟输入。
 *   5. PIOA_6 (ADC 通道 6) 连接模拟输入。
 *   6. PIOA_7 (ADC 通道 7) 连接模拟输入。
 *   7. PIOC_1 (ADC 通道 11) 连接模拟输入。
 *   8. PIOC_2 (ADC 通道 12) 连接模拟输入。
 *   9. PIOC_3 (ADC 通道 13) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 对应ADC通道打印电压值。
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_hw_adc_dma_int.c src_hk32f103rbt6_hw_adc_dma_int
 *
 * \internal
 * \par Modification History
 * - 1.00 19-02-27  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hk32f103rbt6_hw_adc_dma_int
 * \copydoc demo_hk32f103rbt6_hw_adc_dma_int.c
 */

/** [src_hk32f103rbt6_hw_adc_dma_int] */
#include "am_hk32f103rbt6.h"
#include "ametal.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hk32f103rbt6_dma.h"
#include "am_hk32f103rbt6_clk.h"
#include "hw/amhw_hk32f103rbt6_dma.h"
#include "hw/amhw_hk32f103rbt6_adc.h"

#define  BUF_SIZE1    6                 /**< \brief 用于设定缓冲区大小
                                          *    建议修改成adc_chan_num的2倍关系
                                          */
#define  BUF_SIZE2    3                 /**< \brief 用于设定缓冲区大小
                                          *    建议修改成adc_chan_num的2倍关系
                                          */
static uint32_t __g_buf_dst1[BUF_SIZE1];   /**< \brief 目标端数据缓冲区 */

static uint16_t __g_adc_dat1[BUF_SIZE1];   /**< \brief ADC数据缓冲区 */
static uint16_t __g_adc_dat2[BUF_SIZE2];   /**< \brief ADC数据缓冲区 */

static am_bool_t g_trans_done1;           /**< \brief 传输完成标志 */
static am_bool_t g_trans_done2;           /**< \brief 传输完成标志 */

static amhw_hk32f103rbt6_dma_xfer_desc_t g_desc1;  /**< \brief DMA描述符 */

/**
 * \brief DMA 中断服务程序
 */
static void dma_isr1 (void *p_arg , uint32_t flag)
{
    int i;

    if (flag == AM_HK32F103RBT6_DMA_INT_NORMAL) {

        for (i = 0; i < BUF_SIZE1; i++) {

            /* 低12位为ADC采集数据 */
            __g_adc_dat1[i] = __g_buf_dst1[i] & 0xfff;
        }

        g_trans_done1 = AM_TRUE;
    }
}

static void __adc2_isr (void *p_arg)
{
    int i;
    amhw_hk32f103rbt6_adc_t *p_hw_adc = (amhw_hk32f103rbt6_adc_t *)p_arg;

    amhw_hk32f103rbt6_adc_status_flag_clr(p_hw_adc, AMHW_HK32F103RBT6_ADC_INJECTED_CHAN_END_FLAG);

    for (i = 0 ; i < 3; i++) {

        /* AD值的获取 */
        /* 特别注意 ：从数据寄存器的读到的值 = AD实际转换值 - 数据偏移寄存器的偏移值*/
        __g_adc_dat2[i]  = amhw_hk32f103rbt6_adc_injected_data_get(p_hw_adc, i);
        __g_adc_dat2[i] &= 0x0fff;
    }

    g_trans_done2 = AM_TRUE;
}

static void __hk32f103rbt6_adc1_init (amhw_hk32f103rbt6_adc_t *p_hw_adc,
                            int                *p_adc_chan,
                            int                 adc_chan_num,
                            uint8_t             dma_chan)
{
    int i;
    uint32_t flags;

    /* ADC禁能 */
    amhw_hk32f103rbt6_adc_disable(p_hw_adc);

    /* 设置ADC工规则通道长度 */
    if(adc_chan_num < 16) {
        amhw_hk32f103rbt6_adc_regular_channel_length_set(
            p_hw_adc,
            (amhw_hk32f103rbt6_adc_regular_channel_length_t)adc_chan_num-1);
    }

    for(i=0;i<adc_chan_num;i++) {

        /* 链接规则通道序列和ADC采样通道 */
        amhw_hk32f103rbt6_adc_regular_channel_order_set(
            p_hw_adc,
            AMHW_HK32F103RBT6_ADC_REGULAR_CHAN_ORDER_1st + i,
           (amhw_hk32f103rbt6_adc_channel_t)p_adc_chan[i]);

        /* 设置采样通道的采样周期 */
        amhw_hk32f103rbt6_adc_smpr_set(
            p_hw_adc,
            AMHW_HK32F103RBT6_ADC_CHAN_ST55_5,
            (amhw_hk32f103rbt6_adc_channel_t)p_adc_chan[i]);
    }
    /* 启用规则通道外部触发，并设置成软件触发方式 */
    amhw_hk32f103rbt6_adc_extirig_enable(p_hw_adc);
    amhw_hk32f103rbt6_adc_extsel_set(p_hw_adc, AMHW_HK32F103RBT6_ADC12_REGULAR_SWSTART);


    /* 开启扫描模式 */
    amhw_hk32f103rbt6_adc_scan_mode_enable(p_hw_adc);

    /* 单次转换 */
    amhw_hk32f103rbt6_adc_cont_set(p_hw_adc, AMHW_HK32F103RBT6_ADC_CONVERSION_SINGLE);

    /* 设置成独立模式 */
    amhw_hk32f103rbt6_adc_dul_mode_set(p_hw_adc, AMHW_HK32F103RBT6_ADC_DUL_MODE_0);

    /* 对齐方式--右对齐 */
    amhw_hk32f103rbt6_adc_data_alignment_set(p_hw_adc,AMHW_HK32F103RBT6_ADC_DATA_RIGHT);

    /* 关闭ADC16通道内部温度传感器（仅ADC1有效）*/
    amhw_hk32f103rbt6_adc_tsvrefe_disable(p_hw_adc);

    /* 开启ADC的DMA功能 */
    amhw_hk32f103rbt6_adc_dma_enable(p_hw_adc);

    /* ADC使能*/
    amhw_hk32f103rbt6_adc_enable(p_hw_adc);

    /* DMA 传输配置 */
    flags = AMHW_HK32F103RBT6_DMA_CHAN_PRIORITY_HIGH         |  /* 中断优先级 高 */
            AMHW_HK32F103RBT6_DMA_CHAN_MEM_SIZE_32BIT        |  /* 内存数据宽度 2 字节 */
            AMHW_HK32F103RBT6_DMA_CHAN_PER_SIZE_32BIT        |  /* 外设数据宽度 2 字节 */
            AMHW_HK32F103RBT6_DMA_CHAN_MEM_ADD_INC_ENABLE    |  /* 内存地址自增 */
            AMHW_HK32F103RBT6_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 外设地址不自增 */
            AMHW_HK32F103RBT6_DMA_CHAN_CIRCULAR_MODE_ENABLE  |  /* 打开循环模式 */
            AMHW_HK32F103RBT6_DMA_CHAN_INT_TX_CMP_ENABLE;       /* 使能DMA传输完成中断 */


    /* 连接 DMA 中断服务函数 */
    am_hk32f103rbt6_dma_isr_connect(dma_chan, dma_isr1, (void *)p_hw_adc);

    /* 建立通道描述符 */
    am_hk32f103rbt6_dma_xfer_desc_build(&g_desc1,                      /* 通道描述符 */
                               (uint32_t)(&p_hw_adc->dr),    /* 源端数据缓冲区 */
                               (uint32_t)(__g_buf_dst1),      /* 目标端数据缓冲区 */
                               sizeof(__g_buf_dst1),          /* 传输字节数 */
                               flags);                       /* 传输配置 */
    /* DMA寄存器配置 */
    am_hk32f103rbt6_dma_xfer_desc_chan_cfg(&g_desc1,
                                   AMHW_HK32F103RBT6_DMA_PER_TO_MER, /* 外设到内存 */
                                   dma_chan);

    /* 启动 DMA 传输，马上开始传输 */
    am_hk32f103rbt6_dma_chan_start(dma_chan);
}

static void __hk32f103rbt6_adc2_init (amhw_hk32f103rbt6_adc_t *p_hw_adc,
                            int                *p_adc_chan,
                            int                 adc_chan_num)
{
    int i;
    /* ADC禁能 */
    amhw_hk32f103rbt6_adc_disable(p_hw_adc);
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
   am_int_connect(INUM_ADC1_2, __adc2_isr, p_hw_adc);
   am_int_enable(INUM_ADC1_2);

   /* 转换结束中断使能*/
   amhw_hk32f103rbt6_adc_int_enable(p_hw_adc, AMHW_HK32F103RBT6_ADC_INT_INJECTED_END);

}
/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_hw_adc_dma_double_entry (amhw_hk32f103rbt6_adc_t *p_hw_adc1,
                                          int               *p_adc1_chan,
                                          int                adc1_chan_num,
                                          uint8_t            dma_chan,
                                          amhw_hk32f103rbt6_adc_t *p_hw_adc2,
                                          int               *p_adc2_chan,
                                          int                adc2_chan_num)
{
    uint32_t adc_mv = 0;    /* 采样电压 */
    int      i;

    /* 使用非默认demo测试时，请先根据具体使用的ADC通道数量修改BUF_SIZE宏定义*/
    am_kprintf("The ADC HW dma Demo\r\n");
    /* 设置ADC工作频率，72MHz的8分频 */
    amhw_hk32f103rbt6_rcc_adc_div_set (2);

    /* adc相关初始化配置 */
    __hk32f103rbt6_adc1_init(p_hw_adc1, p_adc1_chan, adc1_chan_num, dma_chan);
    __hk32f103rbt6_adc2_init(p_hw_adc2, p_adc2_chan, adc2_chan_num);


    while(1) {
        /* 开始转换 */
        amhw_hk32f103rbt6_adc_swstart_enable(p_hw_adc1);
        amhw_hk32f103rbt6_adc_jswstart_enable(p_hw_adc2);

        while((g_trans_done1 == AM_FALSE) && (g_trans_done2 == AM_FALSE) );
        /* 等待传输完成 */
        if(g_trans_done1 == AM_TRUE) {
            for (i = 0; i < adc1_chan_num; i++) {

                /* 转换为电压值对应的整数值 */
                adc_mv = __g_adc_dat1[i] * 3300 / ((1UL << 12) -1);

                /* 串口打印数据*/
                am_kprintf("ADC1 chan: %d, Sample : %d, Vol: %d mv\r\n", p_adc1_chan[i],
                                                                       __g_adc_dat1[i],
                                                                        adc_mv);
            }
            am_kprintf("\r\n");
            g_trans_done1 = AM_FALSE;
        }

        if(g_trans_done2 == AM_TRUE) {
            for (i = 0; i < adc2_chan_num; i++) {

                /* 转换为电压值对应的整数值 */
                adc_mv = __g_adc_dat2[i] * 3300 / ((1UL << 12) -1);

                /* 串口打印数据*/
                am_kprintf("ADC2 chan: %d, Sample : %d, Vol: %d mv\r\n", p_adc2_chan[i],
                                                                       __g_adc_dat2[i],
                                                                         adc_mv);
            }
            am_kprintf("\r\n");
            g_trans_done2 = AM_FALSE;
        }

        am_mdelay(2000);

        /* DMA传输标志失效 */
    }
}
/** [src_hk32f103rbt6_hw_adc_dma_int] */

/* end of file */
