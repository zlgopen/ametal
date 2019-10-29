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
 * \brief 高级定时器ADTIM接口
 *
 * \internal
 * \par History
 * - 1.00 19-10-08  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG118_ADTIM_H
#define __AMHW_ZLG118_ADTIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "zlg118_regbase.h"

/**
 * \addtogroup amhw_zlg118_if_adtim
 * \copydoc amhw_zlg118_adtim.h
 * @{
 */

/**
 * \brief 定时器寄存器块结构体
 */
typedef struct {
    __IO uint32_t cnter;          /**< \brief 0x000 通用计数基准值寄存器 */
    __IO uint32_t perar;          /**< \brief 0x004 通用周期基准值寄存器 */
    __IO uint32_t perbr;          /**< \brief 0x008 通用周期基准值缓存寄存器*/
    __IO uint32_t reserve0;       /**< \brief 保留*/
    __IO uint32_t gcmxr[4];       /**< \brief 0x010 通用比较A、B、C、D基准值寄存器 */
    __IO uint32_t reserve1[2];    /**< \brief 保留*/
    __IO uint32_t scmxr[2];       /**< \brief 0x028 专用比较A、B基准值寄存器 */
    __IO uint32_t reserve2[4];    /**< \brief 保留*/
    __IO uint32_t dtuar;          /**< \brief 0x040 死区时间基准值寄存器 */
    __IO uint32_t dtdar;          /**< \brief 0x044 死区时间基准值寄存器 */
    __IO uint32_t reserve3[2];    /**< \brief 保留*/
    __IO uint32_t gconr;          /**< \brief 0x050 通用控制寄存器 */
    __IO uint32_t iconr;          /**< \brief 0x054 中断控制寄存器 */
    __IO uint32_t pconr;          /**< \brief 0x058 端口控制寄存器 */
    __IO uint32_t bconr;          /**< \brief 0x05C 缓存控制寄存器 */
    __IO uint32_t dconr;          /**< \brief 0x060 死区控制寄存器 */
    __IO uint32_t reserve4;       /**< \brief 保留*/
    __IO uint32_t fconr;          /**< \brief 0x068 滤波控制寄存器 */
    __IO uint32_t vperr;          /**< \brief 0x06C 有效周期寄存器 */
    __IO uint32_t stflr;          /**< \brief 0x070 状态标志寄存器 */
    __IO uint32_t hstar;          /**< \brief 0x074 硬件启动事件选择寄存器 */
    __IO uint32_t hstpr;          /**< \brief 0x078 硬件停止事件选择寄存器 */
    __IO uint32_t hcelr;          /**< \brief 0x07C 硬件清零事件选择寄存器 */
    __IO uint32_t hcpar;          /**< \brief 0x080 硬件捕获事件选择寄存器 */
    __IO uint32_t hcpbr;          /**< \brief 0x084 硬件捕获事件选择寄存器 */
    __IO uint32_t hcupr;          /**< \brief 0x088 硬件递减事件选择寄存器 */
    __IO uint32_t hcdor;          /**< \brief 0x08C 硬件递减事件选择寄存器 */
    __IO uint32_t reserve5[28];   /**< \brief 保留*/
    __IO uint32_t ifr;            /**< \brief 0x100 中断标志寄存器 */
    __IO uint32_t iclr;           /**< \brief 0x104 中断清除寄存器 */
    __IO uint32_t cr;             /**< \brief 0x108 展频及中断触发选择寄存器 */
    __IO uint32_t reserve6;       /**< \brief 保留*/
    __IO uint32_t aossr;          /**< \brief 0x110 AOS选择寄存器,三个通道共用 */
    __IO uint32_t aoscl;          /**< \brief 0x114 AOS刹车标志清除寄存器,三个通道共用 */
    __IO uint32_t ptbks;          /**< \brief 0x118 端口刹车控制寄存器,三个通道共用 */
    __IO uint32_t ttrig;          /**< \brief 0x11C 端口触发控制寄存器,三个通道共用 */
    __IO uint32_t itrig;          /**< \brief 0x120 AOS 触发控制寄存器,三个通道共用 */
    __IO uint32_t ptbkp;          /**< \brief 0x124 端口刹车极性控制寄存器, 三个通道共用 */
    __IO uint32_t reserve7[179];  /**< \brief 保留*/
    __IO uint32_t sstar;          /**< \brief 0x3F4 软件同步启动寄存器 */
    __IO uint32_t sstpr;          /**< \brief 0x3F8 软件同步停止寄存器 */
    __IO uint32_t sclrr;          /**< \brief 0x3FC 软件同步清零寄存器 */
}amhw_zlg118_adtim_t;

/**
 * \brief ADT CHx通道定义
 */
typedef enum {
    AMHW_ZLG118_ADTIM_CHX_A = 0u,            /**< CHx A通道 */
    AMHW_ZLG118_ADTIM_CHX_B = 1u,            /**< CHx B通道 */
}amhw_zlg118_adtim_chx_t;

/**
 * \brief ADT TRIG端口定义
 */
typedef enum {
    AMHW_ZLG118_ADTIM_TRIG_PORT_A = 0u,   // TIMx 触发A端口
    AMHW_ZLG118_ADTIM_TRIG_PORT_B = 1u,   // TIMx 触发B端口
    AMHW_ZLG118_ADTIM_TRIG_PORT_C = 2u,   // TIMx 触发C端口
    AMHW_ZLG118_ADTIM_TRIG_PORT_D = 3u,   // TIMx 触发D端口
}amhw_zlg118_adtim_trig_port_t;

/**
 * \brief ADT通用控制 - Z相输入屏蔽周期数
 */
typedef enum {
    AMHW_ZLG118_ADTIM_ZMASK_NO = 0u,  // Z相输入屏蔽功能无效
    AMHW_ZLG118_ADTIM_ZMASK_4  = 1u,  // 位置计数上溢后或下溢后的4个计数周期内的Z相输入被屏蔽
    AMHW_ZLG118_ADTIM_ZMASK_8  = 2u,  // 位置计数上溢后或下溢后的8个计数周期内的Z相输入被屏蔽
    AMHW_ZLG118_ADTIM_ZMASK_16 = 3u,  // 位置计数上溢后或下溢后的16个计数周期内的Z相输入被屏蔽
}amhw_zlg118_adtim_zmask_t;

/**
 * \brief ADT通用控制 - 计数时钟选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_PCLK_DIV1    = 0ul,  // PCLK0
    AMHW_ZLG118_ADTIM_PCLK_DIV2    = 1ul,  // PCLK0/2
    AMHW_ZLG118_ADTIM_PCLK_DIV4    = 2ul,  // PCLK0/4
    AMHW_ZLG118_ADTIM_PCLK_DIV8    = 3ul,  // PCLK0/8
    AMHW_ZLG118_ADTIM_PCLK_DIV16   = 4ul,  // PCLK0/16
    AMHW_ZLG118_ADTIM_PCLK_DIV64   = 5ul,  // PCLK0/64
    AMHW_ZLG118_ADTIM_PCLK_DIV256  = 6ul,  // PCLK0/256
    AMHW_ZLG118_ADTIM_PCLK_DIV1024 = 7ul,  // PCLK0/1024
}amhw_zlg118_adtim_pclk_div_t;

/**
 * \brief ADT计数模式
 */
typedef enum {
    AMHW_ZLG118_ADTIM_CNT_MODE_SAWTOOTH   = 0u,  // 锯齿波模式
    AMHW_ZLG118_ADTIM_CNT_MODE_TRIANGLE_A = 4u,  // 三角波A模式
    AMHW_ZLG118_ADTIM_CNT_MODE_TRIANGLE_B = 5u,  // 三角波B模式
}amhw_zlg118_adtim_cnt_mode_t;

/**
 * \brief ADT计数方向
 */
typedef enum {
    AMHW_ZLG118_ADTIM_CNT_DIR_DOWN = 0u,        // 递减计数
    AMHW_ZLG118_ADTIM_CNT_DIR_UP   = 1u,        // 递加计数
}amhw_zlg118_adtim_cnt_dir_t;

/**
 * \brief ADT通用比较基准
 */
typedef enum {
    AMHW_ZLG118_ADTIM_COMPARE_A = 0u,           // 通用比较基准A
    AMHW_ZLG118_ADTIM_COMPARE_B = 1u,           // 通用比较基准B
    AMHW_ZLG118_ADTIM_COMPARE_C = 2u,           // 通用比较基准C
    AMHW_ZLG118_ADTIM_COMPARE_D = 3u,           // 通用比较基准D
}amhw_zlg118_adtim_compare_x_t;

/**
 * \brief ADT专用比较基准
 */
typedef enum {
    AMHW_ZLG118_ADTIM_SPCL_COMP_A = 0u,         // 专用比较基准A
    AMHW_ZLG118_ADTIM_SPCL_COMP_B = 1u,         // 专用比较基准B
}amhw_zlg118_adtim_special_compare_x_t;

/**
 * \brief ADT端口控制 - TIMx输出状态控制
 */
typedef enum {
    AMHW_ZLG118_ADTIM_DISVAL_NORM = 0u,  // 强制输出无效条件0~3中条件成立时，CHx端口正常输出
    AMHW_ZLG118_ADTIM_DISVAL_HIZ  = 1u,  // 强制输出无效条件0~3中条件成立时，CHx端口输出高阻态
    AMHW_ZLG118_ADTIM_DISVAL_LOW  = 2u,  // 强制输出无效条件0~3中条件成立时，CHx端口输出低电平
    AMHW_ZLG118_ADTIM_DISVAL_HIGH = 3u,  // 强制输出无效条件0~3中条件成立时，CHx端口输出高电平
}amhw_zlg118_adtim_disval_t;

/**
 * \brief ADT端口控制 - CHx强制输出无效条件选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_DISSEL0 = 0u,      // 选择强制输出无效条件0
    AMHW_ZLG118_ADTIM_DISSEL1 = 1u,      // 选择强制输出无效条件1
    AMHW_ZLG118_ADTIM_DISSEL2 = 2u,      // 选择强制输出无效条件2
    AMHW_ZLG118_ADTIM_DISSEL3 = 3u,      // 选择强制输出无效条件3
}amhw_zlg118_adtim_dissel_t;

/**
 * \brief ADT端口控制 - CHx周期值匹配时端口状态设定
 */
typedef enum {
    AMHW_ZLG118_ADTIM_PERIOD_LOW  = 0u,  // 计数器计数值与周期值相等时，CHx端口输出保持为低电平
    AMHW_ZLG118_ADTIM_PERIOD_HIGH = 1u,  // 计数器计数值与周期值相等时，CHx端口输出设定为高电平
    AMHW_ZLG118_ADTIM_PERIOD_KEEP = 2u,  // 计数器计数值与周期值相等时，CHx端口输出设定为先前状态
    AMHW_ZLG118_ADTIM_PERIOD_INV  = 3u,  // 计数器计数值与周期值相等时，CHx端口输出设定为反转电平
}amhw_zlg118_adtim_perc_t;

/**
 * \brief ADT端口控制 - CHx比较值匹配时端口状态设定
 */
typedef enum {
    AMHW_ZLG118_ADTIM_COMPARE_LOW  = 0u, // 计数器计数与GCMxR相等时，CHx端口输出保持为低电平
    AMHW_ZLG118_ADTIM_COMPARE_HIGH = 1u, // 计数器计数与GCMxR相等时，CHx端口输出设定为高电平
    AMHW_ZLG118_ADTIM_COMPARE_KEEP = 2u, // 计数器计数与GCMxR相等时，CHx端口输出设定为先前状态
    AMHW_ZLG118_ADTIM_COMPARE_INV  = 3u, // 计数器计数与GCMxR相等时，CHx端口输出设定为反转电平
}amhw_zlg118_adtim_cmpc_t;

/**
  * \brief ADT端口控制 - CHx端口输出
  */
typedef enum {
    AMHW_ZLG118_ADTIM_PORT_OUT_LOW  = 0u, // CHx端口输出设定为低电平
    AMHW_ZLG118_ADTIM_PORT_OUT_HIGH = 1u, // CHx端口输出设定为高电平
}amhw_zlg118_adtim_port_out_t;

/**
 * \brief ADT端口控制 - CHx端口功能模式选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_PORT_MODE_CMP_OUTPUT = 0u, // CHx端口设定为比较输出功能
    AMHW_ZLG118_ADTIM_PORT_MODE_CAP_INPUT  = 1u, // CHx端口设定为捕获输入功能
}amhw_zlg118_adtim_port_mode_t;

/**
 * \brief ADT端口控制 - CHx计数开始停止端口状态选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_STATE_SEL_SS   = 0u, // 计数开始或停止时，CHx端口输出由STACB、STPCB决定
    AMHW_ZLG118_ADTIM_STATE_SEL_KEEP = 1u, // 计数开始或停止时，CHx端口输出设定为先前状态
}amhw_zlg118_adtim_stastps_t;

/**
 * \brief ADT死区控制 - CHx死区分离设定
 */
typedef enum {
    AMHW_ZLG118_ADTIM_DtSeperate = 0u,      // DTUAR和DTDAR分别设定
    AMHW_ZLG118_ADTIM_DtEqual    = 1u,      // DTDAR的值和DTUAR的值自动相等
}amhw_zlg118_adtim_sepa_t;

/**
 * \brief ADT滤波控制 - TRIx/TIMxIx端口滤波采样基准时钟选择
 */
typedef enum
{
    AMHW_ZLG118_ADTIM_FLTCLK_PCLK_DIV1  = 0u, // PCLK0
    AMHW_ZLG118_ADTIM_FLTCLK_PCLK_DIV4  = 1u, // PCLK0/4
    AMHW_ZLG118_ADTIM_FLTCLK_PCLK_DIV16 = 2u, // PCLK0/16
    AMHW_ZLG118_ADTIM_FLTCLK_PCLK_DIV64 = 3u, // PCLK0/64
}amhw_zlg118_adtim_fltclk_pclkdiv_t;

/**
 * \brief ADT有效周期 - TIMx有效周期选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_PeriodCnts0 = 0u,       // 有效周期选择功能无效
    AMHW_ZLG118_ADTIM_PeriodCnts1 = 1u,       // 每隔1个周期有效一次
    AMHW_ZLG118_ADTIM_PeriodCnts2 = 2u,       // 每隔2个周期有效一次
    AMHW_ZLG118_ADTIM_PeriodCnts3 = 3u,       // 每隔3个周期有效一次
    AMHW_ZLG118_ADTIM_PeriodCnts4 = 4u,       // 每隔4个周期有效一次
    AMHW_ZLG118_ADTIM_PeriodCnts5 = 5u,       // 每隔5个周期有效一次
    AMHW_ZLG118_ADTIM_PeriodCnts6 = 6u,       // 每隔6个周期有效一次
    AMHW_ZLG118_ADTIM_PeriodCnts7 = 7u,       // 每隔7个周期有效一次
}amhw_zlg118_adtim_pcnts_t;

/**
 * \brief ADT有效周期 - 计数条件选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_PeriodCnteDisable = 0u,  // 有效周期选择功能无效
    AMHW_ZLG118_ADTIM_PeriodCnteMin     = 1u,  // 锯齿波计数上、下溢点或三角波波谷做为计数条件
    AMHW_ZLG118_ADTIM_PeriodCnteMax     = 2u,  // 锯齿波计数上、下溢点或三角波波峰做为计数条件
    AMHW_ZLG118_ADTIM_PeriodCnteBoth    = 3u,  // 锯齿波计数上、下溢点或三角波波峰，波谷做为计数条件
}amhw_zlg118_adtim_pcnte_t;

/**
 * \brief ADT端口触发控制 - 触发源选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_TRIG_PA3  = 0u,    // PA3
    AMHW_ZLG118_ADTIM_TRIG_PB3  = 1u,    // PB3
    AMHW_ZLG118_ADTIM_TRIG_PC3  = 2u,    // PC3
    AMHW_ZLG118_ADTIM_TRIG_PD3  = 3u,    // PD3
    AMHW_ZLG118_ADTIM_TRIG_PA7  = 4u,    // PA7
    AMHW_ZLG118_ADTIM_TRIG_PB7  = 5u,    // PB7
    AMHW_ZLG118_ADTIM_TRIG_PC7  = 6u,    // PC7
    AMHW_ZLG118_ADTIM_TRIG_PD7  = 7u,    // PD7
    AMHW_ZLG118_ADTIM_TRIG_PA11 = 8u,    // PA11
    AMHW_ZLG118_ADTIM_TRIG_PB11 = 9u,    // PB11
    AMHW_ZLG118_ADTIM_TRIG_PC11 = 10u,   // PC11
    AMHW_ZLG118_ADTIM_TRIG_PD1  = 11u,   // PD1
    AMHW_ZLG118_ADTIM_TRIG_PA15 = 12u,   // PA15
    AMHW_ZLG118_ADTIM_TRIG_PB15 = 13u,   // PB15
    AMHW_ZLG118_ADTIM_TRIG_PC5  = 14u,   // PC5
    AMHW_ZLG118_ADTIM_TRIG_PD5  = 15u,   // PD5
}amhw_zlg118_adtim_trigsel_t;

/**
 * \brief ADT AOS触发控制 - AOSx触发源选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_AosxTrigSelTim0Int   = 0u,    // TIM0_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelTim1Int   = 1u,    // TIM1_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelTim2Int   = 2u,    // TIM2_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelLpTimInt  = 3u,    // LPTIMER_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelTim4Int   = 4u,    // TIM4_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelTim5Int   = 5u,    // TIM5_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelTim6Int   = 6u,    // TIM6_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelUart0Int  = 7u,    // UART0_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelUart1Int  = 8u,    // UART1_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelLpUartInt = 9u,    // LPUART_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelVc0Int    = 10u,   // VC0_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelVc1Int    = 11u,   // VC1_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelRtcInt    = 12u,   // RTC_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelPcaInt    = 13u,   // PCA_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelSpiInt    = 14u,   // SPI_INT
    AMHW_ZLG118_ADTIM_AosxTrigSelAdcInt    = 15u,   // ADC_INT
}amhw_zlg118_adtim_iaosxs_t;

/**
 * \brief ADT硬件(启动/停止/清零/捕获)事件触发选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_HwTrigAos0        = 0u,       // 从AOS来的事件触发0有效
    AMHW_ZLG118_ADTIM_HwTrigAos1        = 1u,       // 从AOS来的事件触发1有效
    AMHW_ZLG118_ADTIM_HwTrigAos2        = 2u,       // 从AOS来的事件触发2有效
    AMHW_ZLG118_ADTIM_HwTrigAos3        = 3u,       // 从AOS来的事件触发3有效
    AMHW_ZLG118_ADTIM_HwTrigCHxARise    = 4u,       // CHxA端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwTrigCHxAFall    = 5u,       // CHxA端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwTrigCHxBRise    = 6u,       // CHxB端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwTrigCHxBFall    = 7u,       // CHxB端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriARise = 8u,       // TIMTRIA端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriAFall = 9u,       // TIMTRIA端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriBRise = 10u,      // TIMTRIB端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriBFall = 11u,      // TIMTRIB端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriCRise = 12u,      // TIMTRIC端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriCFall = 13u,      // TIMTRIC端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriDRise = 14u,      // TIMTRID端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwTrigTimTriDFall = 15u,      // TIMTRID端口上采样到下降沿
}amhw_zlg118_adtim_hw_trig_t;

/**
 * \brief ADT硬件(递加/递减)事件触发选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_HwCntCHxALowCHxBRise   = 0u,  // CHxA端口为低电平时，CHxB端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntCHxALowCHxBFall   = 1u,  // CHxA端口为低电平时，CHxB端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntCHxAHighCHxBRise  = 2u,  // CHxA端口为高电平时，CHxB端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntCHxAHighCHxBFall  = 3u,  // CHxA端口为高电平时，CHxB端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntCHxBLowCHxARise   = 4u,  // CHxB端口为低电平时，CHxA端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntCHxBLowCHxAFall   = 5u,  // CHxB端口为低电平时，CHxA端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntCHxBHighChxARise  = 6u,  // CHxB端口为高电平时，CHxA端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntCHxBHighCHxAFall  = 7u,  // CHxB端口为高电平时，CHxA端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntTimTriARise       = 8u,  // TIMTRIA端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntTimTriAFall       = 9u,  // TIMTRIA端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntTimTriBRise       = 10u, // TIMTRIB端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntTimTriBFall       = 11u, // TIMTRIB端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntTimTriCRise       = 12u, // TIMTRIC端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntTimTriCFall       = 13u, // TIMTRIC端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntTimTriDRise       = 14u, // TIMTRID端口上采样到上升沿
    AMHW_ZLG118_ADTIM_HwCntTimTriDFall       = 15u, // TIMTRID端口上采样到下降沿
    AMHW_ZLG118_ADTIM_HwCntAos0              = 16u, // 从AOS来的事件触发0有效
    AMHW_ZLG118_ADTIM_HwCntAos1              = 17u, // 从AOS来的事件触发1有效
    AMHW_ZLG118_ADTIM_HwCntAos2              = 18u, // 从AOS来的事件触发2有效
    AMHW_ZLG118_ADTIM_HwCntAos3              = 19u, // 从AOS来的事件触发3有效
}amhw_zlg118_adtim_hw_cnt_t;

/**
 * \brief ADT端口刹车极性控制
 */
typedef enum {
    AMHW_ZLG118_ADTIM_PtBrkHigh = 0u,              // 端口刹车极性高电平有效
    AMHW_ZLG118_ADTIM_PtBrkLow  = 1u,              // 端口刹车极性低电平有效
}amhw_zlg118_adtim_ptbrk_polarity_t;

/**
 * \brief ADT PWM展频计数选择
 */
typedef enum {
    AMHW_ZLG118_ADTIM_PwmDitherUnderFlow = 0u,     // PWM展频计数下溢出
    AMHW_ZLG118_ADTIM_PwmDitherOverFlow  = 1u,     // PWM展频计数上溢出
}amhw_zlg118_adtim_pwm_dither_type_t;

/**
 * \brief ADT中断类型
 */
typedef enum {
    AMHW_ZLG118_ADTIM_CMAIrq  = 0u,                // 计数匹配A（或捕获输入）中断
    AMHW_ZLG118_ADTIM_CMBIrq  = 1u,                // 计数匹配B（或捕获输入）中断
    AMHW_ZLG118_ADTIM_CMCIrq  = 2u,                // 计数匹配C中断
    AMHW_ZLG118_ADTIM_CMDIrq  = 3u,                // 计数匹配D中断
    AMHW_ZLG118_ADTIM_OVFIrq  = 6u,                // 上溢匹配中断
    AMHW_ZLG118_ADTIM_UDFIrq  = 7u,                // 下溢匹配中断
    AMHW_ZLG118_ADTIM_DTEIrq  = 8u,                // 死区时间错误中断
    AMHW_ZLG118_ADTIM_SAMLIrq = 14u,               // 同低中断
    AMHW_ZLG118_ADTIM_SAMHIrq = 15u,               // 同高中断
}amhw_zlg118_adtim_irq_type_t;

/**
 * \brief ADT标志类型
 */
typedef enum {
    AMHW_ZLG118_ADTIM_CMAF = 0,                    // 计数匹配A标志
    AMHW_ZLG118_ADTIM_CMBF = 1,                    // 计数匹配B标志
    AMHW_ZLG118_ADTIM_CMCF = 2,                    // 计数匹配C标志
    AMHW_ZLG118_ADTIM_CMDF = 3,                    // 计数匹配D标志
    AMHW_ZLG118_ADTIM_OVFF = 6,                    // 上溢匹配标志
    AMHW_ZLG118_ADTIM_UDFF = 7,                    // 下溢匹配标志
    AMHW_ZLG118_ADTIM_DTEF = 8,                    // 死区时间错误标志
    AMHW_ZLG118_ADTIM_CMSAUF = 9,                  // 向上计数专用比较基准值匹配A标志
    AMHW_ZLG118_ADTIM_CMSADF = 10,                 // 向下计数专用比较基准值匹配B标志
    AMHW_ZLG118_ADTIM_CMSBUF = 11,                 // 向上计数专用比较基准值匹配A标志
    AMHW_ZLG118_ADTIM_CMSBDF = 12,                 // 向下计数专用比较基准值匹配B标志
    AMHW_ZLG118_ADTIM_CntDir = 31,                 // 计数方向
}amhw_zlg118_adtim_state_type_t;

/**
 * \brief ADT软件同步配置
 */
typedef struct {
    am_bool_t adtim4;                              // Timer 4
    am_bool_t adtim5;                              // Timer 5
    am_bool_t adtim6;                              // Timer 6
}amhw_zlg118_adtim_sw_sync_t;

/**
 * \brief ADT AOS触发配置
 */
typedef struct {
    amhw_zlg118_adtim_iaosxs_t   aos0trig_src;     // AOS0触发源选择
    amhw_zlg118_adtim_iaosxs_t   aos1trig_src;     // AOS1触发源选择
    amhw_zlg118_adtim_iaosxs_t   aos2trig_src;     // AOS2触发源选择
    amhw_zlg118_adtim_iaosxs_t   aos3trig_src;     // AOS3触发源选择
}amhw_zlg118_adtim_aos_trig_cfg_t;

/**
 * \brief ADT 中断触发配置
 */
typedef struct {
    am_bool_t   specilmatch_B_trigdma;  // 专用比较基准值匹配B使能触发DMA
    am_bool_t   specilmatch_A_trigdma;  // 专用比较基准值匹配A使能触发DMA
    am_bool_t   underflow_trigdma;      // 下溢匹配使能触发DMA
    am_bool_t   overflow_trigdma;       // 上溢匹配使能触发DMA
    am_bool_t   cntmatch_D_trigdma;     // 计数匹配D使能触发DMA
    am_bool_t   cntmatch_C_trigdma;     // 计数匹配C使能触发DMA
    am_bool_t   cntmatch_B_trigdma;     // 计数匹配B使能触发DMA
    am_bool_t   cntmatch_A_trigdma;     // 计数匹配A使能触发DMA
    am_bool_t   specilmatch_B_trig;     // 专用比较基准值匹配B使能触发ADC
    am_bool_t   specilmatch_A_trig;     // 专用比较基准值匹配A使能触发ADC
    am_bool_t   underflow_trig;         // 下溢匹配使能触发ADC
    am_bool_t   overflow_trig;          // 上溢匹配使能触发ADC
    am_bool_t   cntmatch_D_trig;        // 计数匹配D使能触发ADC
    am_bool_t   cntmatch_C_trig;        // 计数匹配C使能触发ADC
    am_bool_t   cntmatch_B_trig;        // 计数匹配B使能触发ADC
    am_bool_t   cntmatch_A_trig;        // 计数匹配A使能触发ADC
}amhw_zlg118_adtim_irq_trig_cfg_t;

/**
 * \brief ADT Trig端口配置
 */
typedef struct {
    amhw_zlg118_adtim_trigsel_t        trigsrc;      // 触发源选择
    am_bool_t                          flt_enable;   // 触发源捕获输入滤波使能
    amhw_zlg118_adtim_fltclk_pclkdiv_t fltclk;       // 滤波采样基准时钟
}amhw_zlg118_adtim_port_trig_cfg_t;

/**
 * \brief ADT Z相输入屏蔽功能配置
 */
typedef struct {
    amhw_zlg118_adtim_zmask_t zmask_cycle;   // Z相输入屏蔽计数周期选择
    am_bool_t                 fltposcntmask; // Z相输入时的屏蔽周期内，位置计数器的清零功能不屏蔽（FALSE）或屏蔽(TRUE)
    am_bool_t                 fltrevcntmask; // Z相输入时的屏蔽周期内，公转计数器的计数功能不屏蔽（FALSE）或屏蔽(TRUE)
}amhw_zlg118_adtim_zmask_cfg_t;

/**
 * \brief ADT TIMxX端口配置
 */
typedef struct {
    amhw_zlg118_adtim_port_mode_t portmode;    // 端口功能模式
    am_bool_t                     outenable;   // 输出使能
    amhw_zlg118_adtim_perc_t      perc;        // 周期值匹配时端口状态
    amhw_zlg118_adtim_cmpc_t      cmpc;        // 比较值匹配时端口状态
    amhw_zlg118_adtim_stastps_t   stastps;     // 计数开始停止端口状态选择
    amhw_zlg118_adtim_port_out_t  staout;      // 计数开始端口输出状态
    amhw_zlg118_adtim_port_out_t  stpout;      // 计数停止端口输出状态
    amhw_zlg118_adtim_disval_t    disval;      // 强制输出无效时输出状态控制
    amhw_zlg118_adtim_dissel_t    dissel;      // 强制输出无效条件选择
    am_bool_t                     fltenable;   // 端口捕获输入滤波使能
    amhw_zlg118_adtim_fltclk_pclkdiv_t fltclk; // 端口滤波采样基准时钟
}amhw_zlg118_adtim_timx_chx_port_cfg_t;

/**
 * \brief ADT刹车端口配置
 */
typedef struct {
    am_bool_t                          portenable;    // 端口使能
    amhw_zlg118_adtim_ptbrk_polarity_t polaritysel;   // 极性选择
}amhw_zlg118_adtim_break_port_cfg_t;

/**
 * \brief ADT无效条件3配置
 */
typedef struct amhw_zlg118_adtim_disable_3_cfg
{
    amhw_zlg118_adtim_break_port_cfg_t stcbrkptcfg[16]; // 刹车端口配置
    am_bool_t                          fltenable;       // 刹车端口滤波使能
    amhw_zlg118_adtim_fltclk_pclkdiv_t fltclk;          // 滤波采样基准时钟
}amhw_zlg118_adtim_disable_3_cfg_t;

/**
 * \brief ADT无效条件1配置
 */
typedef struct {
    am_bool_t   tim6_outsh;     // TIM6输出同高
    am_bool_t   tim5_outsh;     // TIM5输出同高
    am_bool_t   tim4_outsh;     // TIM4输出同高
    am_bool_t   tim6_outsl;     // TIM6输出同低
    am_bool_t   tim5_outsl;     // TIM5输出同低
    am_bool_t   tim4_outsl;     // TIM4输出同低
}amhw_zlg118_adtim_disable_1_cfg_t;

/**
 * \brief ADT PWM展频计数配置
 */
typedef struct {
    amhw_zlg118_adtim_pwm_dither_type_t enAMHW_ZLG118_ADTIM_PDType;// PWM展频计数选择
    am_bool_t   bTimxBPDEn;                      // PWM通道B展频使能
    am_bool_t   bTimxAPDEn;                      // PWM通道A展频使能
}amhw_zlg118_adtim_pwm_dither_cfg_t;

/**
 * \brief ADT基本计数配置
 */
typedef struct {
    amhw_zlg118_adtim_cnt_mode_t cntmode;        // 计数模式
    amhw_zlg118_adtim_cnt_dir_t  cntdir;         // 计数方向
    amhw_zlg118_adtim_pclk_div_t cntclkdiv;      // 计数时钟选择
}amhw_zlg118_adtim_basecnt_cfg_t;

/**
 * \brief ADT计数状态
 */
typedef struct {
    uint16_t  u16counter;                        // 当前计数器的计数值
    am_bool_t cntdir;                            // 计数方向
    uint8_t   u8validperiod;                     // 有效周期计数
    am_bool_t cmsbdf;                            // 向下计数专用比较基准值匹配B标志
    am_bool_t cmsbuf;                            // 向上计数专用比较基准值匹配A标志
    am_bool_t cmsadf;                            // 向下计数专用比较基准值匹配B标志
    am_bool_t cmsauf;                            // 向上计数专用比较基准值匹配A标志
    am_bool_t dtff;                              // 死区时间错误标志
    am_bool_t udff;                              // 下溢匹配标志
    am_bool_t ovff;                              // 上溢匹配标志
    am_bool_t cmdf;                              // 计数匹配D标志
    am_bool_t cmcf;                              // 计数匹配C标志
    am_bool_t cmbf;                              // 计数匹配B标志
    am_bool_t cmaf;                              // 计数匹配A标志
}amhw_zlg118_adtim_cntstate_cfg_t;

/**
 * \brief ADT有效计数周期
 */
typedef struct {
    amhw_zlg118_adtim_pcnts_t    validcnt;       // 有效周期选择
    amhw_zlg118_adtim_pcnte_t    validcdt;       // 有效周期计数条件
    am_bool_t                    period_D;       // 通用信号有效周期选择D
    am_bool_t                    period_C;       // 通用信号有效周期选择C
    am_bool_t                    period_B;       // 通用信号有效周期选择B
    am_bool_t                    period_A;       // 通用信号有效周期选择A
}amhw_zlg118_adtim_validper_cfg_t;

/**
 * \brief 配置硬件递加事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_zlg118_adtim_hw_cnt_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfghwcntup(amhw_zlg118_adtim_t       *p_hw_adtim,
                                       amhw_zlg118_adtim_hw_cnt_t choice) {

    if(choice > AMHW_ZLG118_ADTIM_HwCntAos3) {
        return AM_FALSE;
    }

    p_hw_adtim->hcupr |= (1ul << choice);

    return AM_TRUE;
}

/**
 * \brief 清除硬件递加事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearhwcntup(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hcupr = 0;

    return AM_TRUE;
}

/**
 * \brief 配置硬件递减事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_zlg118_adtim_hw_cnt_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfghwcntdown(amhw_zlg118_adtim_t       *p_hw_adtim,
                                         amhw_zlg118_adtim_hw_cnt_t choice) {
    if(choice > AMHW_ZLG118_ADTIM_HwCntAos3) {
        return AM_FALSE;
    }

    p_hw_adtim->hcdor |= (1ul << choice);

    return AM_TRUE;
}

/**
 * \brief 清除硬件递减事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearhwcntdown(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hcdor = 0;

    return AM_TRUE;
}

/**
 * \brief 配置硬件启动事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_zlg118_adtim_hw_trig_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfghwstart(amhw_zlg118_adtim_t        *p_hw_adtim,
                                       amhw_zlg118_adtim_hw_trig_t choice) {
    if(choice > AMHW_ZLG118_ADTIM_HwTrigTimTriDFall) {
        return AM_FALSE;
    }

    p_hw_adtim->hstar |= (1ul << choice);

    return AM_TRUE;
}

/**
 * \brief 清除硬件启动事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearhwstart(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hstar = 0;

    return AM_TRUE;
}

/**
 * \brief 使能硬件启动事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_enablehwstart(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hstar |= (1ul << 31);

    return AM_TRUE;
}

/**
 * \brief 禁止硬件启动事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_disablehwstart(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hstar &= ~(1ul << 31);

    return AM_TRUE;
}

/**
 * \brief 配置硬件停止事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_zlg118_adtim_hw_trig_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfghwstop(amhw_zlg118_adtim_t        *p_hw_adtim,
                                      amhw_zlg118_adtim_hw_trig_t choice) {

    if(choice > AMHW_ZLG118_ADTIM_HwTrigTimTriDFall) {
        return AM_FALSE;
    }

    p_hw_adtim->hstpr |= (1ul << choice);

    return AM_TRUE;
}

/**
 * \brief 清除硬件停止事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearhwstop(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hstpr = 0;

    return AM_TRUE;
}

/**
 * \brief 使能硬件停止事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_enablehwstop(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hstpr |= (1ul << 31);

    return AM_TRUE;
}

/**
 * \brief 禁止硬件停止事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_disablehwstop(amhw_zlg118_adtim_t *p_hw_adtim){

    p_hw_adtim->hstpr &= ~(1ul << 31);

    return AM_TRUE;
}

/**
 * \brief 配置硬件清零事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_zlg118_adtim_hw_trig_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfghwclear(amhw_zlg118_adtim_t        *p_hw_adtim,
                                       amhw_zlg118_adtim_hw_trig_t choice) {

    if(choice > AMHW_ZLG118_ADTIM_HwTrigTimTriDFall) {
        return AM_FALSE;
    }

    p_hw_adtim->hcelr |= (1ul << choice);

    return AM_TRUE;
}

/**
 * \brief 清除硬件清零事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearhwclear(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hcelr = 0;

    return AM_TRUE;
}

/**
 * \brief 使能硬件清零事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_enablehwclear(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hcelr |= (1ul << 31);

    return AM_TRUE;
}

/**
 * \brief 禁止硬件清零事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_disablehwclear(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hcelr &= ~(1ul << 31);

    return AM_TRUE;
}

/**
 * \brief 配置A通道硬件捕获事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_zlg118_adtim_hw_trig_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfghwcaptureA(amhw_zlg118_adtim_t        *p_hw_adtim,
                                          amhw_zlg118_adtim_hw_trig_t choice) {

    if(choice > AMHW_ZLG118_ADTIM_HwTrigTimTriDFall) {
        return AM_FALSE;
    }

    p_hw_adtim->hcpar |= (1ul << choice);
    p_hw_adtim->pconr |= (1ul << 0 );

    return AM_TRUE;
}

/**
 * \brief 清除A通道硬件捕获事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearhwcaptureA(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hcpar = 0;

    return AM_TRUE;
}

/**
 * \brief 配置B通道硬件捕获事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_zlg118_adtim_hw_trig_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfghwcaptureB(amhw_zlg118_adtim_t        *p_hw_adtim,
                                          amhw_zlg118_adtim_hw_trig_t choice) {

    if(choice > AMHW_ZLG118_ADTIM_HwTrigTimTriDFall) {
        return AM_FALSE;
    }

    p_hw_adtim->hcpbr |= (1ul << choice);
    p_hw_adtim->pconr |= (1ul << 16 );

    return AM_TRUE;
}

/**
 * \brief 清除B通道硬件捕获事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearhwcaptureB(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->hcpbr = 0;

    return AM_TRUE;
}

/**
 * \brief 软件同步启动
 *
 * \param[in] p_cfg : 指向ADT软件同步配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_swsyncstart(amhw_zlg118_adtim_sw_sync_t *p_cfg) {

    if(p_cfg->adtim4) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sstar = (1ul << 0);
    }

    if(p_cfg->adtim5) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sstar = (1ul << 1);
    }

    if(p_cfg->adtim6) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sstar = (1ul << 2);
    }

    return AM_TRUE;
}

/**
 * \brief 软件同步停止
 *
 * \param[in] p_cfg : 指向ADT软件同步配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_swsyncstop(amhw_zlg118_adtim_sw_sync_t *p_cfg) {

    if(p_cfg->adtim4) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sstpr = (1ul << 0);
    }
    if(p_cfg->adtim5) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sstpr = (1ul << 1);
    }
    if(p_cfg->adtim6) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sstpr = (1ul << 2);
    }

    return AM_TRUE;
}

/**
 * \brief 软件同步清零
 *
 * \param[in] p_cfg : 指向ADT软件同步配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_swsyncclear(amhw_zlg118_adtim_sw_sync_t *p_cfg) {

    if(p_cfg->adtim4) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sclrr = (1ul << 0);
    }
    if(p_cfg->adtim5) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sclrr = (1ul << 1);
    }
    if(p_cfg->adtim6) {
        ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->sclrr = (1ul << 2);
    }

    return AM_TRUE;
}

/**
 * \brief 获取软件同步状态
 *
 * \param[in] p_cfg : 指向ADT软件同步配置结构体的指针
 *
 * \return AM_FALSE ：获取失败
 *         AM_TRUE  ：获取成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setswsyncstate(amhw_zlg118_adtim_sw_sync_t *p_cfg) {

    amhw_zlg118_adtim_t *p_hw_adtim4 = (amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE;

    p_cfg->adtim4 = (p_hw_adtim4->sstar >> 0) & 1ul ? AM_TRUE : AM_FALSE;
    p_cfg->adtim5 = (p_hw_adtim4->sstar >> 1) & 1ul ? AM_TRUE : AM_FALSE;
    p_cfg->adtim6 = (p_hw_adtim4->sstar >> 2) & 1ul ? AM_TRUE : AM_FALSE;

    return AM_TRUE;
}

/**
 * \brief AOS触发配置
 *
 * \param[in] p_cfg : 指向AOS触发配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_aostrigcfg(amhw_zlg118_adtim_aos_trig_cfg_t *p_cfg){

    amhw_zlg118_adtim_t *p_hw_adtim4 = (amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE;

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    p_hw_adtim4->itrig  = 0;
    p_hw_adtim4->itrig |= (p_cfg->aos0trig_src << 0);
    p_hw_adtim4->itrig |= (p_cfg->aos1trig_src << 4);
    p_hw_adtim4->itrig |= (p_cfg->aos2trig_src << 8);
    p_hw_adtim4->itrig |= (p_cfg->aos3trig_src << 12);

    return AM_TRUE;
}

/**
 * \brief 中断触发使能配置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] p_cfg      : 指向中断使能配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_zlg118_adtim_irqtrigcfg(
              amhw_zlg118_adtim_t              *p_hw_adtim,
              amhw_zlg118_adtim_irq_trig_cfg_t *p_cfg);

/**
 * \brief 端口触发配置
 *
 * \param[in] trigport : amhw_zlg118_adtim_trig_port_t端口枚举量
 * \param[in] p_cfg    : 指向端口配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_zlg118_adtim_porttrigcfg(
              amhw_zlg118_adtim_trig_port_t      trigport,
              amhw_zlg118_adtim_port_trig_cfg_t *p_cfg);

/**
 * \brief CHxX端口配置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_zlg118_adtim_chx_t通道枚举量
 * \param[in] p_cfg      : 指向TIMxX端口配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_zlg118_adtim_timxchxportcfg(
              amhw_zlg118_adtim_t                   *p_hw_adtim,
              amhw_zlg118_adtim_chx_t                port,
              amhw_zlg118_adtim_timx_chx_port_cfg_t *p_cfg);

/**
 * \brief 使能端口刹车
 *
 * \param[in] port       : 端口编号（0~15）
 * \param[in] p_cfg      : 指向TADT刹车端口配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_enablebrakeport(
              uint8_t                             port,
              amhw_zlg118_adtim_break_port_cfg_t *p_cfg) {

    amhw_zlg118_adtim_t *p_hw_adtim4 = (amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE;

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    p_hw_adtim4->ptbkp &= ~(1ul << port);
    p_hw_adtim4->ptbkp |= (p_cfg->polaritysel << port);
    p_hw_adtim4->ptbks |= (1ul << port);

    return AM_TRUE;
}

/**
 * \brief 清除端口刹车
 */
am_static_inline
void amhw_zlg118_adtim_clearbrakeport(void) {

    ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->ptbks = 0;
}

/**
 * \brief 无效条件3配置(端口刹车)
 *
 * \param[in] p_cfg      : 指向TADT无效条件3配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_zlg118_adtim_disable3cfg(
              amhw_zlg118_adtim_disable_3_cfg_t *p_cfg);

/**
 * \brief 软件刹车 Enable/Disable(仅适用于无效条件3使能的情况下)
 *
 * \param[in] flag      : AM_FALSE(禁能) 或       AM_TRUE(使能)
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_swbrake(am_bool_t flag) {

    amhw_zlg118_adtim_t *p_hw_adtim4 = (amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE;

    p_hw_adtim4->aossr &= ~(1ul << 7);
    p_hw_adtim4->aossr |= (flag << 7);

    return AM_TRUE;
}

/**
 * \brief 获取端口刹车标志
 *
 * \return AM_FALSE ：禁能状态
 *         AM_TRUE  ：使能状态
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_getportbrakeflag(void) {
    return ((((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->aossr >> 0) & 1ul) ?
           AM_TRUE :
           AM_FALSE;
}

/**
 * \brief 清除端口刹车标志
 *
 */
am_static_inline
void amhw_zlg118_adtim_clearportbrakeflag(void) {

    ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->aoscl &= ~(1ul << 0);
}

/**
 * \brief 无效条件1配置(同高同低刹车)
 *
 * \param[in] p_cfg : 指向TADT无效条件1配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_disable1Cfg(
              amhw_zlg118_adtim_disable_1_cfg_t *p_cfg) {

    amhw_zlg118_adtim_t *p_hw_adtim4 = (amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE;

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    p_hw_adtim4->aossr = (p_hw_adtim4->aossr & (~(1ul << 13))) |
                         (p_cfg->tim6_outsh << 13);
    p_hw_adtim4->aossr = (p_hw_adtim4->aossr & (~(1ul << 12))) |
                         (p_cfg->tim5_outsh << 12);
    p_hw_adtim4->aossr = (p_hw_adtim4->aossr & (~(1ul << 11))) |
                         (p_cfg->tim4_outsh << 11);
    p_hw_adtim4->aossr = (p_hw_adtim4->aossr & (~(1ul << 10))) |
                         (p_cfg->tim6_outsl << 10);
    p_hw_adtim4->aossr = (p_hw_adtim4->aossr & (~(1ul << 9))) |
                         (p_cfg->tim5_outsl << 9);
    p_hw_adtim4->aossr = (p_hw_adtim4->aossr & (~(1ul << 8))) |
                         (p_cfg->tim4_outsl << 8);

    return AM_TRUE;
}

/**
 * \brief 获取同高同低刹车标志
 *
 * \return AM_FALSE ：0
 *         AM_TRUE  ：1
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_getsamebrakeflag(void) {
    return ((((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->aossr >> 1) & 1ul) ?
           AM_TRUE :
           AM_FALSE;
}

/**
 * \brief 清除同高同低刹车标志
 *
 */
am_static_inline
void amhw_zlg118_adtim_clearsamebrakeflag(void) {
    ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)->aoscl &= ~(1ul << 1);
}

/**
 * \brief PWM展频配置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] p_cfg      : 指向PWM展频计数配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_pwmdithercfg(
              amhw_zlg118_adtim_t                *p_hw_adtim,
              amhw_zlg118_adtim_pwm_dither_cfg_t *p_cfg) {
    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    p_hw_adtim->cr = (p_hw_adtim->cr & (~(1ul << 10))) |
                     (p_cfg->enAMHW_ZLG118_ADTIM_PDType << 10);
    p_hw_adtim->cr = (p_hw_adtim->cr & (~(1ul << 9))) |
                     (p_cfg->bTimxBPDEn << 9);
    p_hw_adtim->cr = (p_hw_adtim->cr & (~(1ul << 8))) |
                     (p_cfg->bTimxAPDEn << 8);

    return AM_TRUE;
}

/**
 * \brief ADTIM初始化
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] p_cfg      : 指向ADT基本计数配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_init(amhw_zlg118_adtim_t             *p_hw_adtim,
                                 amhw_zlg118_adtim_basecnt_cfg_t *p_cfg) {
    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    if((p_cfg->cntmode) > AMHW_ZLG118_ADTIM_CNT_MODE_TRIANGLE_B) {
        return AM_FALSE;
    }

    p_hw_adtim->gconr &= ~(7ul << 1);
    p_hw_adtim->gconr &= ~(7ul << 4);
    p_hw_adtim->gconr &= ~(1ul << 8);

    p_hw_adtim->gconr |= (p_cfg->cntmode << 1);
    p_hw_adtim->gconr |= (p_cfg->cntclkdiv << 4);
    p_hw_adtim->gconr |= (p_cfg->cntdir << 8);

    return AM_TRUE;
}

am_static_inline
amhw_zlg118_adtim_pclk_div_t
amhw_zlg118_adtim_clkdiv_get(amhw_zlg118_adtim_t *p_hw_adtim) {

    return (amhw_zlg118_adtim_pclk_div_t)((p_hw_adtim->gconr >> 4) & 7ul);
}


/**
 * \brief ADTIM去初始化
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_zlg118_adtim_deinit(amhw_zlg118_adtim_t *p_hw_adtim);

/**
 * \brief ADTIM启动
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_startcount(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->gconr |= (1ul << 0);

    return AM_TRUE;
}

/**
 * \brief ADTIM停止
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_stopcount(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->gconr &= ~(1ul << 0);

    return AM_TRUE;
}

/**
 * \brief 设置计数值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] cnt        ： 计数值设定
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setcount(amhw_zlg118_adtim_t *p_hw_adtim,
                                     uint16_t             cnt) {

    p_hw_adtim->cnter = cnt;

    return AM_TRUE;
}

/**
 * \brief 获取计数值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return 计数值
 */
am_static_inline
uint16_t amhw_zlg118_adtim_getcount(amhw_zlg118_adtim_t *p_hw_adtim) {
    return p_hw_adtim->cnter;
}

/**
 * \brief 清除计数值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearcount(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->cnter = 0;

    return AM_TRUE;
}

/**
 * \brief 获取有效周期计数值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return 有效周期计数值
 */
am_static_inline
uint8_t amhw_zlg118_adtim_getvpernum(amhw_zlg118_adtim_t *p_hw_adtim) {
    return ((p_hw_adtim->stflr >> 21) & 0x7ul);
}

/**
 * \brief 获取状态标志
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] type       : amhw_zlg118_adtim_state_type_t状态标志枚举量
 *
 * \return AM_FALSE ：标志无效
 *         AM_TRUE  ：标志有效
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_getstate(amhw_zlg118_adtim_t           *p_hw_adtim,
                                     amhw_zlg118_adtim_state_type_t type) {
    return ((p_hw_adtim->stflr >> type) & 1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 配置计数周期
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] data       : 计数周期设定值
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setperiod(amhw_zlg118_adtim_t *p_hw_adtim,
                                      uint16_t             data) {

    p_hw_adtim->perar = data;

    return AM_TRUE;
}

/**
 * \brief 获取计数周期
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return 计数周期
 */
am_static_inline
uint16_t amhw_zlg118_adtim_getperiod(amhw_zlg118_adtim_t *p_hw_adtim) {

    return p_hw_adtim->perar;
}

/**
 * \brief 配置计数周期缓冲
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] data       : 周期缓冲设定值
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setperiodbuf(amhw_zlg118_adtim_t *p_hw_adtim,
                                         uint16_t             data) {
    p_hw_adtim->perbr  = data;
    p_hw_adtim->bconr |= (1ul << 8);

    return AM_TRUE;
}

/**
 * \brief 清除计数周期缓冲
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearperiodbuf(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->bconr &= ~(1ul << 8);
    p_hw_adtim->perbr  = 0;

    return AM_TRUE;
}

/**
 * \brief 配置有效计数周期
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] p_cfg      : 指向ADT有效计数周期配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setvalidperiod(
              amhw_zlg118_adtim_t              *p_hw_adtim,
              amhw_zlg118_adtim_validper_cfg_t *p_cfg) {
    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    p_hw_adtim->vperr  = 0;
    p_hw_adtim->vperr |= (p_cfg->validcnt << 18);
    p_hw_adtim->vperr |= (p_cfg->validcdt << 16);
    p_hw_adtim->vperr |= (p_cfg->period_D << 3);
    p_hw_adtim->vperr |= (p_cfg->period_C << 2);
    p_hw_adtim->vperr |= (p_cfg->period_B << 1);
    p_hw_adtim->vperr |= (p_cfg->period_A << 0);

    return AM_TRUE;
}

/**
 * \brief 配置比较输出计数基准值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] compare    : amhw_zlg118_adtim_compare_x_t枚举量
 * \param[in] data       : 设定值
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setcomparevalue(
              amhw_zlg118_adtim_t          *p_hw_adtim,
              amhw_zlg118_adtim_compare_x_t compare,
              uint16_t                      data) {

    switch(compare) {
    case AMHW_ZLG118_ADTIM_COMPARE_A:
        p_hw_adtim->gcmxr[0] = data;
        break;
    case AMHW_ZLG118_ADTIM_COMPARE_B:
        p_hw_adtim->gcmxr[1] = data;
        break;
    case AMHW_ZLG118_ADTIM_COMPARE_C:
        p_hw_adtim->gcmxr[2] = data;
        break;
    case AMHW_ZLG118_ADTIM_COMPARE_D:
        p_hw_adtim->gcmxr[3] = data;
        break;
    default:
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * \brief 配置专用比较计数基准值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] compare    : amhw_zlg118_adtim_special_compare_x_t枚举量
 * \param[in] data       : 设定值
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setspecilcomparevalue(
              amhw_zlg118_adtim_t                  *p_hw_adtim,
              amhw_zlg118_adtim_special_compare_x_t compare,
              uint16_t                              data) {
    switch(compare) {
    case AMHW_ZLG118_ADTIM_SPCL_COMP_A:
        p_hw_adtim->scmxr[0] = data;
        break;
    case AMHW_ZLG118_ADTIM_SPCL_COMP_B:
        p_hw_adtim->scmxr[1] = data;
        break;
    default:
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * \brief 配置通用比较值/捕获值的缓存传送
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] compare    : amhw_zlg118_adtim_chx_t枚举量
 * \param[in] enable     : 使能状态（AM_FALSE或AM_TRUE）
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_enablevaluebuf(amhw_zlg118_adtim_t    *p_hw_adtim,
                                           amhw_zlg118_adtim_chx_t port,
                                           am_bool_t               enable) {
    switch(port) {
    case AMHW_ZLG118_ADTIM_CHX_A:
        p_hw_adtim->bconr = (p_hw_adtim->bconr & (~(1ul << 0))) |
                            (enable << 0);
        break;
    case AMHW_ZLG118_ADTIM_CHX_B:
        p_hw_adtim->bconr = (p_hw_adtim->bconr & (~(1ul << 2))) |
                            (enable << 2);
        break;
    default:
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * \brief 清除比较输出计数值/捕获值缓存
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_zlg118_adtim_chx_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearvaluebuf(amhw_zlg118_adtim_t    *p_hw_adtim,
                                          amhw_zlg118_adtim_chx_t port) {
    switch(port) {
    case AMHW_ZLG118_ADTIM_CHX_A:
        p_hw_adtim->gcmxr[2] = 0;
        break;
    case AMHW_ZLG118_ADTIM_CHX_B:
        p_hw_adtim->gcmxr[3] = 0;
        break;
    default:
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * \brief 获取捕获值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_zlg118_adtim_chx_t枚举量
 *
 * \return 捕获值
 */
am_static_inline
uint16_t amhw_zlg118_adtim_getcapturevalue(amhw_zlg118_adtim_t    *p_hw_adtim,
                                           amhw_zlg118_adtim_chx_t port) {
    switch(port) {
    case AMHW_ZLG118_ADTIM_CHX_A:
        return p_hw_adtim->gcmxr[0];
    case AMHW_ZLG118_ADTIM_CHX_B:
        return p_hw_adtim->gcmxr[1];
    default:
        return AM_FALSE;
    }
}

/**
 * \brief 获取捕获缓存值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_zlg118_adtim_chx_t枚举量
 *
 * \return 捕获缓存值
 */
am_static_inline
uint16_t amhw_zlg118_adtim_getcapturebuf(amhw_zlg118_adtim_t     *p_hw_adtim,
                                          amhw_zlg118_adtim_chx_t port) {
    switch(port) {
    case AMHW_ZLG118_ADTIM_CHX_A:
        return p_hw_adtim->gcmxr[2];
    case AMHW_ZLG118_ADTIM_CHX_B:
        return p_hw_adtim->gcmxr[3];
    default:
        return AM_FALSE;
    }
}

/**
 * \brief 设置死区时间上基准值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] data       : 设定值
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setdtua(amhw_zlg118_adtim_t *p_hw_adtim,
                                    uint16_t             data) {
    p_hw_adtim->dtuar = data;

    return AM_TRUE;
}

/**
 * \brief 设置死区时间下基准值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] data       : 设定值
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_setdtda(amhw_zlg118_adtim_t *p_hw_adtim,
                                    uint16_t data) {
    p_hw_adtim->dtdar = data;

    return AM_TRUE;
}

/**
 * \brief 配置死区时间功能
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] enable     : 死区功能使能（AM_FALSE或AM_TRUE）
 * \param[in] equal      : DTDAR的值和DTUAR的值自动相等使能（AM_FALSE或AM_TRUE）
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfgdt(amhw_zlg118_adtim_t *p_hw_adtim,
                                  am_bool_t            enable,
                                  am_bool_t            equal) {

    p_hw_adtim->dconr = (p_hw_adtim->dconr &(~(1ul << 0))) |
                        (enable << 0);

    p_hw_adtim->dconr = (p_hw_adtim->dconr &(~(1ul << 8))) |
                        (equal << 8);

    return AM_TRUE;
}

/**
 * \brief 配置中断
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] type       : amhw_zlg118_adtim_irq_type_t中断类型枚举量
 * \param[in] equal      : 相应中断使能（AM_FALSE或AM_TRUE）
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfgirq(amhw_zlg118_adtim_t         *p_hw_adtim,
                                   amhw_zlg118_adtim_irq_type_t type,
                                   am_bool_t                    enable) {
    if(enable == AM_TRUE) {
        p_hw_adtim->iconr |= (1ul << type);
    } else {
        p_hw_adtim->iconr &= ~(1ul << type);
    }

    return AM_TRUE;
}

/**
 * \brief 获取中断标志
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] type       : amhw_zlg118_adtim_irq_type_t中断类型枚举量
 *
 * \return AM_FALSE:标志无效
 *         AM_TRUE :标志有效
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_getirqflag(amhw_zlg118_adtim_t         *p_hw_adtim,
                                       amhw_zlg118_adtim_irq_type_t type) {
    return ((p_hw_adtim->ifr >> type) & 1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除中断标志
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] type       : amhw_zlg118_adtim_irq_type_t中断类型枚举量
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearirqflag(amhw_zlg118_adtim_t         *p_hw_adtim,
                                         amhw_zlg118_adtim_irq_type_t type) {
    p_hw_adtim->iclr &= ~(1ul << type);

    return AM_TRUE;
}

/**
 * \brief 清除所有中断标志
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_clearallirqflag(amhw_zlg118_adtim_t *p_hw_adtim) {

    p_hw_adtim->iclr = 0;

    return AM_TRUE;
}

/**
 * \brief Z相输入屏蔽设置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] p_cfg      : 指向Z相输入屏蔽功能配置结构体的指针
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_zlg118_adtim_cfgzmask(amhw_zlg118_adtim_t           *p_hw_adtim,
                                     amhw_zlg118_adtim_zmask_cfg_t *p_cfg) {
    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    p_hw_adtim->gconr = (p_hw_adtim->gconr & (~(3ul << 18))) |
                        (p_cfg->zmask_cycle << 18);

    p_hw_adtim->gconr = (p_hw_adtim->gconr & (~(1ul << 17))) |
                        (p_cfg->fltposcntmask << 17);

    p_hw_adtim->gconr = (p_hw_adtim->gconr & (~(1ul << 16))) |
                        (p_cfg->fltrevcntmask << 16);

    return AM_TRUE;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG118_ADTIM_H */

/* end of file */
