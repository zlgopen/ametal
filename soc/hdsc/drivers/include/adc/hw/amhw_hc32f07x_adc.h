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
 * \brief ADC Peripheral Access Layer
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-24  zp, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_HC32F07X_ADC_H
#define __AMHW_HC32F07X_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_hc32f07x_if_adc
 * \copydoc amhw_hc32f07x_adc.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

/* 默认使能匿名联合体 */
#elif defined(__TMS470__)

/* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

/**
 * \brief ADC - Register Layout Typedef
 */
typedef struct amhw_hc32f07x_adc {
    __I  uint32_t reserve0;       /**< \brief 保留 */
    __IO uint32_t cr[2];          /**< \brief 基本配置寄存器0、1 偏移地址0x004、0x008*/
    __I  uint32_t reserve2[13];   /**< \brief 保留 */
    __IO uint32_t sqr[3];         /**< \brief 顺序扫描转换通道配置寄存器0、1、2 */
    __IO uint32_t jqr;            /**< \brief 插队扫描转换通道配置寄存器 */
    __I  uint32_t sqrresult[16];  /**< \brief 顺序扫描转换通道转换结果(0~15) */
    __I  uint32_t jqrresult[4];   /**< \brief 插队扫描转换通道转换结果(0~3) */
    __I  uint32_t result;         /**< \brief 转换结果 */
    __I  uint32_t resultacc;      /**< \brief 转换结果累加值 */
    __IO uint32_t ht;             /**< \brief 比较上阈值 */
    __IO uint32_t lt;             /**< \brief 比较下阈值 */
    __IO uint32_t ifr;            /**< \brief 中断标志寄存器 */
    __IO uint32_t icr;            /**< \brief 中断清除寄存器 */
    __IO uint32_t exttrigger0;    /**< \brief 单次转换或顺序扫描转换外部中断触发源配置寄存器 */
    __IO uint32_t exttrigger1;    /**< \brief 插队扫描转换外部中断触发源配置寄存器 */
    __IO uint32_t sglstart;       /**< \brief 单次转换启动控制寄存器 */
    __IO uint32_t sqrstart;       /**< \brief 顺序扫描转换启动控制寄存器 */
    __IO uint32_t jqrstart;       /**< \brief 插队扫描转换启动控制寄存器 */
    __IO uint32_t allstart;       /**< \brief 一直转换启动停止控制寄存器 */
} amhw_hc32f07x_adc_t;

typedef enum {
    AMHW_HC32F07X_WORK_MODE_SINGLE = 0,      /**< \brief 单次转换（至多1个通道）*/
    AMHW_HC32F07X_WORK_MODE_SQR,             /**< \brief 顺序扫描规则转换（至多16通道） */
    AMHW_HC32F07X_WORK_MODE_JQR,             /**< \brief 插入扫描规则转换 （至多4通道）*/
}amhw_hc32f07x_adc_work_mode_t;

typedef enum {
	
	/**< \brief 未开启通道映射后 */
    AMHW_HC32F07X_CHAN_AIN0_PA0 = 0,      /**< \brief 转换通道0 */
    AMHW_HC32F07X_CHAN_AIN1_PA1,          /**< \brief 转换通道1 */
    AMHW_HC32F07X_CHAN_AIN2_PA2,          /**< \brief 转换通道2 */
    AMHW_HC32F07X_CHAN_AIN3_PA3,          /**< \brief 转换通道3 */	
    AMHW_HC32F07X_CHAN_AIN4_PA4,          /**< \brief 转换通道4 */
    AMHW_HC32F07X_CHAN_AIN5_PA5,          /**< \brief 转换通道5 */
    AMHW_HC32F07X_CHAN_AIN6_PA6,          /**< \brief 转换通道6 */
    AMHW_HC32F07X_CHAN_AIN7_PA7,          /**< \brief 转换通道7 */
    AMHW_HC32F07X_CHAN_AIN8_PB0,          /**< \brief 转换通道8 */
    AMHW_HC32F07X_CHAN_AIN9_PB1,          /**< \brief 转换通道9 */
    AMHW_HC32F07X_CHAN_AIN10_PC0,         /**< \brief 转换通道10 */
    AMHW_HC32F07X_CHAN_AIN11_PC1,         /**< \brief 转换通道11 */
    AMHW_HC32F07X_CHAN_AIN12_PC2,         /**< \brief 转换通道12 */
    AMHW_HC32F07X_CHAN_AIN13_PC3,         /**< \brief 转换通道13 */
    AMHW_HC32F07X_CHAN_AIN14_PC4,         /**< \brief 转换通道14 */
    AMHW_HC32F07X_CHAN_AIN15_PC5,         /**< \brief 转换通道15 */
    AMHW_HC32F07X_CHAN_AIN16_PB2,         /**< \brief 转换通道16 */
    AMHW_HC32F07X_CHAN_AIN17_PB10,        /**< \brief 转换通道17 */
    AMHW_HC32F07X_CHAN_AIN18_PB11,        /**< \brief 转换通道18 */
    AMHW_HC32F07X_CHAN_AIN19_PB12,        /**< \brief 转换通道19 */
    AMHW_HC32F07X_CHAN_AIN20_PB13,        /**< \brief 转换通道20 */
    AMHW_HC32F07X_CHAN_AIN21_PB14,        /**< \brief 转换通道21 */
    AMHW_HC32F07X_CHAN_AIN22_PB15,        /**< \brief 转换通道22 */
    AMHW_HC32F07X_CHAN_AIN23_PE15,        /**< \brief 转换通道23 */
    AMHW_HC32F07X_CHAN_AIN24_PC7,         /**< \brief 转换通道24 */
	
    AMHW_HC32F07X_CHAN_DAC_OUT0,           /**< \brief 与DAC输出连接，ADC_CR0.Buf必须为1 */
    AMHW_HC32F07X_CHAN_DAC_OUT1,           /**< \brief 与DAC输出连接，ADC_CR0.Buf必须为1 */
    AMHW_HC32F07X_CHAN_VCC_DIV3,          /**< \brief 与内部1/3VCC连接，ADC_CR0.Buf必须为1 */
    AMHW_HC32F07X_CHAN_INSIDE_TEMP_SENSOR,/**< \brief 与内置温度传感器输出电压连接，ADC_CR0.Buf必须为1 */
    AMHW_HC32F07X_CHAN_INSIDE_REFVCC_12,  /**< \brief 与内部基准1.2V输出电压连接，ADC_CR0.Buf必须为1 */
	
    /**< \brief 开启通道映射后，以下通道将映射到新端口 */
    AMHW_HC32F07X_CHAN_AIN0_PD8 = 0,       /**< \brief 转换通道0 */
    AMHW_HC32F07X_CHAN_AIN1_PD9,           /**< \brief 转换通道1 */
    AMHW_HC32F07X_CHAN_AIN2_PD10,          /**< \brief 转换通道2 */
    AMHW_HC32F07X_CHAN_AIN3_PD11,          /**< \brief 转换通道3 */
    AMHW_HC32F07X_CHAN_AIN6_PE8 = 6,       /**< \brief 转换通道6 */
    AMHW_HC32F07X_CHAN_AIN7_PE9,           /**< \brief 转换通道7 */
    AMHW_HC32F07X_CHAN_AIN8_PE10,          /**< \brief 转换通道8 */
    AMHW_HC32F07X_CHAN_AIN10_PE11 = 10,    /**< \brief 转换通道10 */
    AMHW_HC32F07X_CHAN_AIN11_PE12,         /**< \brief 转换通道11 */
    AMHW_HC32F07X_CHAN_AIN12_PE13,         /**< \brief 转换通道12 */
    AMHW_HC32F07X_CHAN_AIN13_PE14,         /**< \brief 转换通道13 */
}amhw_hc32f07x_adc_chan_sel_t;

/**
 * \brief IE 中断使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_int_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] |= (0x1ul << 15);
}

/**
 * \brief IE 中断禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_int_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] &= ~(0x1ul << 15);
}

/**
 * \brief En 输入映射开启
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_chmap_set (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] |= (0x1ul << 1);
}

/**
 * \brief En 输入映射关闭
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_chmap_clr (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] &= ~(0x1ul << 1);
}

/**
 * \brief En ADC使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] |= (0x1ul << 0);
}

/**
 * \brief En ADC禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] &= ~(0x1ul << 0);
}

/**
 * \brief InRefEn ADC内部参考电压使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_inrefvol_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] |= (0x1ul << 14);
}

/**
 * \brief InRefEn ADC内部参考电压禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_inrefvol_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] &= ~(0x1ul << 14);
}

/**
 * \brief ADC 采样周期
 */
typedef enum {
    AMHW_HC32F07X_ADC_SAMPLE_PERIOD_4 = 0,     /**< \brief 4个转换周期 */
    AMHW_HC32F07X_ADC_SAMPLE_PERIOD_6,         /**< \brief 6个转换周期 */
    AMHW_HC32F07X_ADC_SAMPLE_PERIOD_8,         /**< \brief 8个转换周期 */
    AMHW_HC32F07X_ADC_SAMPLE_PERIOD_12,        /**< \brief 12个转换周期 */
}amhw_hc32f07x_adc_sample_period_t;

/**
 * \brief SAM ADC采样周期选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] choice   : amhw_hc32f07x_adc_sample_period_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sample_period_sel(amhw_hc32f07x_adc_t              *p_hw_adc,
                                       amhw_hc32f07x_adc_sample_period_t choice)
{
    p_hw_adc->cr[0] = (p_hw_adc->cr[0] & (~(0x3ul << 12))) |
                      ((choice & 0x3ul) << 12);
}

/**
 * \brief SAM ADC采样周期获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return amhw_hc32f07x_adc_sample_period_t枚举量
 */
am_static_inline
amhw_hc32f07x_adc_sample_period_t amhw_hc32f07x_adc_sample_period_get(
                                    amhw_hc32f07x_adc_t *p_hw_adc)
{
    return (amhw_hc32f07x_adc_sample_period_t)((p_hw_adc->cr[0] >> 12) & 0x3ul);
}

/**
 * \brief Buf ADC输入信号放大器使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \note 以下几种情况需要打开BUF功能，使用BUF功能时，最大速率200kbps
 *            1）外部驱动很弱信号
 *            2）测量1/3AVCC
 *            3）测量temp sensor
 *            4）测量VREF1P2
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_inamplify_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] |= (0x1ul << 11);
}

/**
 * \brief Buf ADC输入信号放大器禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \note 以下几种情况需要打开BUF功能，使用BUF功能时，最大速率200kbps
 *            1）外部驱动很弱信号
 *            2）测量1/3AVCC
 *            3）测量temp sensor
 *            4）测量VREF1P2
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_inamplify_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[0] &= ~(0x1ul << 11);
}

/**
 * \brief ADC 参考电压选择
 */
typedef enum {
    AMHW_HC32F07X_ADC_REFVOL_INSIDE_1500MV = 0,
    AMHW_HC32F07X_ADC_REFVOL_INSIDE_2500MV,
    AMHW_HC32F07X_ADC_REFVOL_OUTSIDE_PB1,
    AMHW_HC32F07X_ADC_REFVOL_AVCC,
}amhw_hc32f07x_adc_refvoltage_choice_t;

/**
 * \brief Ref ADC参考电压选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] choice   : amhw_hc32f07x_adc_refvoltage_choice_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_refvol_sel(amhw_hc32f07x_adc_t                  *p_hw_adc,
                                amhw_hc32f07x_adc_refvoltage_choice_t choice)
{
    p_hw_adc->cr[0] = (p_hw_adc->cr[0] & (~(0x3ul << 9))) |
                      ((choice & 0x3ul) << 9);
}

/**
 * \brief Ref ADC参考电压选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] choice   : amhw_hc32f07x_adc_refvoltage_choice_t枚举量
 *
 * \return none
 */
am_static_inline
amhw_hc32f07x_adc_refvoltage_choice_t amhw_hc32f07x_adc_refvol_get(
                                        amhw_hc32f07x_adc_t *p_hw_adc)
{
    return (amhw_hc32f07x_adc_refvoltage_choice_t)((p_hw_adc->cr[0] >> 9) & 0x3ul);
}

/**
 * \brief SGLMux 单次转换模式转换通道选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] choice   : amhw_hc32f07x_adc_chan_sel_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sgl_chan_sel(amhw_hc32f07x_adc_t         *p_hw_adc,
                                  amhw_hc32f07x_adc_chan_sel_t choice)
{
    p_hw_adc->cr[0] = (p_hw_adc->cr[0] & (~(0x1ful << 4))) |
                      ((choice & 0x1ful) << 4);
}

/**
 * \brief ADC时钟选择
 */
typedef enum {
    AMHW_HC32F07X_ADC_PCLK_DIV1 = 0,
    AMHW_HC32F07X_ADC_PCLK_DIV2,
    AMHW_HC32F07X_ADC_PCLK_DIV4,
    AMHW_HC32F07X_ADC_PCLK_DIV8,
}amhw_hc32f07x_adc_pclk_div_t;

/**
 * \brief CkDiv ADC时钟分频选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] choice   : amhw_hc32f07x_adc_pclk_div_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_pclk_div_sel(amhw_hc32f07x_adc_t         *p_hw_adc,
                                  amhw_hc32f07x_adc_pclk_div_t choice)
{
    p_hw_adc->cr[0] = (p_hw_adc->cr[0] & (~(0x3ul << 2))) |
                      ((choice & 0x3ul) << 2);
}

/**
 * \brief CkDiv ADC时钟分频获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return amhw_hc32f07x_adc_pclk_div_t枚举量中的一个
 */
am_static_inline
amhw_hc32f07x_adc_pclk_div_t amhw_hc32f07x_adc_pclk_div_get(amhw_hc32f07x_adc_t
                                                            *p_hw_adc)
{
    return (amhw_hc32f07x_adc_pclk_div_t)((p_hw_adc->cr[0] >> 2) & 0x3ul);
}

/**
 * \brief RAccClr ADC转换结果累加寄存器（ADC_ResultAcc）清零
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_acc_clr (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] &= ~(0x1ul << 15);
}

/**
 * \brief RegCmp ADC区间比较使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_interval_cmp_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] |= (0x1ul << 14);
}

/**
 * \brief RegCmp ADC区间比较禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_interval_cmp_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] &= ~(0x1ul << 14);
}

/**
 * \brief HtCmp ADC高阈值比较使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_ht_cmp_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] |= (0x1ul << 13);
}

/**
 * \brief HtCmp ADC高阈值比较禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_ht_cmp_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] &= ~(0x1ul << 13);
}

/**
 * \brief LtCmp ADC低阈值比较使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_lt_cmp_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] |= (0x1ul << 12);
}

/**
 * \brief LtCmp ADC低阈值比较禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_lt_cmp_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] &= ~(0x1ul << 12);
}

/**
 * \brief RAccEn ADC转换结果自动累加使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_acc_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] |= (0x1ul << 11);
}

/**
 * \brief RAccEn ADC转换结果自动累加禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_acc_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] &= ~(0x1ul << 11);
}

/**
 * \brief ADC转换模式选择
 */
typedef enum {
    AMHW_HC32F07X_ADC_MODE_SCAN   = 1,
    AMHW_HC32F07X_ADC_MODE_SINGLE = 0,
}amhw_hc32f07x_adc_mode_t;
/**
 * \brief Mode ADC转换模式选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] mode     : amhw_hc32f07x_adc_mode_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_mode_sel (amhw_hc32f07x_adc_t     *p_hw_adc,
                               amhw_hc32f07x_adc_mode_t mode)
{
    p_hw_adc->cr[1] =  (p_hw_adc->cr[1] & (~(0x1ul << 10))) |
                       ((mode & 0X1ul) << 10);
}

/**
 * \brief DmaJqr 插队扫描触发DMA读取使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_dma_jqr_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] |= (0x1ul << 9);
}

/**
 * \brief DmaJqr 插队扫描触发DMA读取禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_dma_jqr_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] &= ~(0x1ul << 9);
}

/**
 * \brief DmaSqr 顺序扫描触发DMA读取使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_dma_sqr_enable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] |= (0x1ul << 8);
}

/**
 * \brief DmaSqr 顺序扫描触发DMA读取禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_dma_sqr_disable (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->cr[1] &= ~(0x1ul << 8);
}

/**
 * \brief ThCh 阈值比较通道选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] choice   : amhw_hc32f07x_adc_chan_sel_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_threshold_chan_sel(amhw_hc32f07x_adc_t         *p_hw_adc,
                                        amhw_hc32f07x_adc_chan_sel_t choice)
{
    p_hw_adc->cr[1] = (p_hw_adc->cr[1] & (~(0x1ful << 3))) |
                      ((choice & 0x1ful) << 3);
}

typedef enum {
    AMHW_HC32F07X_ADC_DATA_ALIGN_RIGHT = 0,
    AMHW_HC32F07X_ADC_DATA_ALIGN_LEFT  = 1,
}amhw_hc32f07x_adc_data_align_t;

/**
 * \brief ADC 转换结果对齐控制
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] choice   : amhw_hc32f07x_adc_data_align_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_align_way_sel (amhw_hc32f07x_adc_t           *p_hw_adc,
                                    amhw_hc32f07x_adc_data_align_t mode)
{
    p_hw_adc->cr[1] =  (p_hw_adc->cr[1] & (~(0x1ul << 2))) |
                       ((mode & 0x1ul) << 2);
}

/**
 * \brief 顺序扫描转换通道配置 （顺序扫描转换通道和具体的ADC输入通道相互链接）
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] sqr_chan : sqr通道编号
 * \param[in] choice   : amhw_hc32f07x_adc_chan_sel_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sqr_chan_sel(amhw_hc32f07x_adc_t         *p_hw_adc,
                                  uint8_t                    sqr_chan,
                                  amhw_hc32f07x_adc_chan_sel_t choice)
{
    if(sqr_chan < 16) {
        p_hw_adc->sqr[sqr_chan / 6] =
            (p_hw_adc->sqr[sqr_chan / 6] & (~(0x1ful<<((sqr_chan % 6) * 5)))) |
            ((choice & 0x1ful) << ((sqr_chan % 6) * 5));
    }
}

/**
 * \brief 顺序扫描转换次数设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] num      : 转换次数
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sqr_num_set(amhw_hc32f07x_adc_t *p_hw_adc,
                                 uint8_t            num)
{
    p_hw_adc->sqr[2] = (p_hw_adc->sqr[2] & (~(0xful << 20))) |
                       ((num & 0xful) << 20);
}

/**
 * \brief 插队扫描转换通道配置（ 插队扫描转换通道和具体的ADC输入通道相互链接）
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] jqr_chan : jqr通道编号
 * \param[in] choice   : amhw_hc32f07x_adc_chan_sel_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_jqr_chan_sel(amhw_hc32f07x_adc_t         *p_hw_adc,
                                  uint8_t                    jqr_chan,
                                  amhw_hc32f07x_adc_chan_sel_t choice)
{
    if(jqr_chan < 4) {
        p_hw_adc->jqr = (p_hw_adc->jqr & (~(0x1ful << (jqr_chan * 5)))) |
                         ((choice & 0x1ful) << (jqr_chan * 5));
    }
}

/**
 * \brief 插队扫描转换次数设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] num      : 转换次数
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_jqr_num_set(amhw_hc32f07x_adc_t *p_hw_adc,
                                 uint8_t            num)
{
    p_hw_adc->jqr = (p_hw_adc->jqr & (~(0x3ul << 20))) |
                    ((num & 0x3ul) << 20);
}

/**
 * \brief 顺序扫描转换通道 x转换结果（ 插队扫描AD数据获取）
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] sqr_chan : sqr通道编号
 *
 * \return 12bitADC转换数据
 */
am_static_inline
uint16_t amhw_hc32f07x_adc_sqr_chan_data_get(amhw_hc32f07x_adc_t *p_hw_adc,
                                           uint8_t            sqr_chan)
{
    if(sqr_chan < 16) {
        return p_hw_adc->sqrresult[sqr_chan];
    } else {
        return 0;
    }
}

/**
 * \brief 插队扫描转换通道 x转换结果（ 插队扫描AD数据获取）
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] jqr_chan : jqr通道编号
 *
 * \return 12bitADC转换数据
 */
am_static_inline
uint16_t amhw_hc32f07x_adc_jqr_chan_data_get(amhw_hc32f07x_adc_t *p_hw_adc,
                                           uint8_t            jqr_chan)
{
    if(jqr_chan < 4) {
        return p_hw_adc->jqrresult[jqr_chan];
    } else {
        return 0;
    }
}

/**
 * \brief ADC转换结果获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return 12bitADC转换数据
 */
am_static_inline
uint16_t amhw_hc32f07x_adc_data_get(amhw_hc32f07x_adc_t *p_hw_adc)
{
    return p_hw_adc->result;
}

/**
 * \brief ADC转换结果累加值获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return 16bit数据
 */
am_static_inline
uint16_t amhw_hc32f07x_adc_acc_data_get(amhw_hc32f07x_adc_t *p_hw_adc)
{
    return p_hw_adc->resultacc;
}

/**
 * \brief ADC比较上阈值获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return 12bit数据
 */
am_static_inline
uint16_t amhw_hc32f07x_adc_ht_get(amhw_hc32f07x_adc_t *p_hw_adc)
{
    return p_hw_adc->ht;
}

/**
 * \brief ADC比较上阈值设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] data     : 待设置上阈值数据
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_ht_set(amhw_hc32f07x_adc_t *p_hw_adc,
                            uint16_t           data)
{
    p_hw_adc->ht = (p_hw_adc->ht & (~(0xffful))) |
                   (data & 0xffful);
}

/**
 * \brief ADC比较下阈值获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return 12bit数据
 */
am_static_inline
uint16_t amhw_hc32f07x_adc_lt_get(amhw_hc32f07x_adc_t *p_hw_adc)
{
    return p_hw_adc->lt;
}

/**
 * \brief ADC比较下阈值设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] data     : 待设置下阈值数据
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_lt_set(amhw_hc32f07x_adc_t *p_hw_adc,
                            uint16_t           data)
{
    p_hw_adc->lt = (p_hw_adc->lt & (~(0xffful))) |
                   (data & 0xffful);
}

typedef enum {
    AMHW_HC32F07X_ADC_FLAG_ALL         = 6,
    AMHW_HC32F07X_ADC_FLAG_CONVERT_JQR = 5,   /* 插队扫描转换状态标志    1：完成    0：未完成*/
    AMHW_HC32F07X_ADC_FLAG_CONVERT_SQR = 4,   /* 顺序扫描转换状态标志    1：完成    0：未完成*/
    AMHW_HC32F07X_ADC_FLAG_COMPARE_REG = 3,   /* 转换结果是否位于[LT ， HT)区间    1：位于    0：不位于*/
    AMHW_HC32F07X_ADC_FLAG_COMPARE_HT = 2,    /* 转换结果是否位于[HT ， 4095)区间    1：位于    0：不位于*/
    AMHW_HC32F07X_ADC_FLAG_COMPARE_LT = 1,    /* 转换结果是否位于[0 ， LT)区间    1：位于    0：不位于*/
    AMHW_HC32F07X_ADC_FLAG_CONVERT_SINGLE = 0,/* 单次转换状态标志    1：完成    0：未完成*/
}amhw_hc32f07x_adc_flag_type_t;

/**
 * \brief ADC标志状态获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f07x_adc_flag_type_t枚举量）
 *
 * \return AM_TRUE  : 1
 *         AM_FALSE : 0
 */
am_static_inline
am_bool_t amhw_hc32f07x_adc_flag_check (amhw_hc32f07x_adc_t          *p_hw_adc,
                                      amhw_hc32f07x_adc_flag_type_t type)
{
    return ((p_hw_adc->ifr >> type) & 0x1u) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief ADC标志状态清除
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f07x_adc_flag_type_t枚举量）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_flag_clr (amhw_hc32f07x_adc_t          *p_hw_adc,
                               amhw_hc32f07x_adc_flag_type_t type)
{
    if(type == AMHW_HC32F07X_ADC_FLAG_ALL) {
        p_hw_adc->icr &= ~(0x3ful);
    } else {
        p_hw_adc->icr &= ~(0x1ul << type);
    }

}

typedef enum {
    AMHW_HC32F07X_ADC_INT_TRIGGER_PC15 = 31,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PB15 = 30,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PA15 = 29,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PC11 = 28,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PB11 = 27,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PA11 = 26,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PD7 = 25,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PC7 = 24,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PB7 = 23,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PA7 = 22,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PD3 = 21,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PC3 = 20,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PB3 = 19,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PA3 = 18,
    AMHW_HC32F07X_ADC_INT_TRIGGER_DMA = 17,
    AMHW_HC32F07X_ADC_INT_TRIGGER_SPI1 = 16,
    AMHW_HC32F07X_ADC_INT_TRIGGER_SPI0 = 15,
    AMHW_HC32F07X_ADC_INT_TRIGGER_PCA = 14,
    AMHW_HC32F07X_ADC_INT_TRIGGER_RTC = 13,
    AMHW_HC32F07X_ADC_INT_TRIGGER_VC1 = 12,
    AMHW_HC32F07X_ADC_INT_TRIGGER_VC0 = 11,
    AMHW_HC32F07X_ADC_INT_TRIGGER_LPUART1 = 10,
    AMHW_HC32F07X_ADC_INT_TRIGGER_LPUART0 = 9,
    AMHW_HC32F07X_ADC_INT_TRIGGER_UART1 = 8,
    AMHW_HC32F07X_ADC_INT_TRIGGER_UART0 = 7,
    AMHW_HC32F07X_ADC_INT_TRIGGER_TIM6 = 6,
    AMHW_HC32F07X_ADC_INT_TRIGGER_TIM5 = 5,
    AMHW_HC32F07X_ADC_INT_TRIGGER_TIM4 = 4,
    AMHW_HC32F07X_ADC_INT_TRIGGER_TIM3 = 3,
    AMHW_HC32F07X_ADC_INT_TRIGGER_TIM2 = 2,
    AMHW_HC32F07X_ADC_INT_TRIGGER_TIM1 = 1,
    AMHW_HC32F07X_ADC_INT_TRIGGER_TIM0 = 0,
}amhw_hc32f07x_adc_int_trigger_src_t;

/**
 * \brief 单次转换或顺序扫描转换外部中断触发源设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f07x_adc_int_trigger_src_t枚举量）
 *
 * \note TIM4/5/6中断触发 ADC自动转换，除了需要使能 TIM4/5/6的相应中断外，还需要配置Advanced Timer
 *       的展频及中断触发选择寄存器 TIMX_CR选择可以触发 ADC 的中断源
 *
 *       触发 ADC使用的是各中断标志位的上升沿。如果需要重复触发，需要清除中断标志。如果不需要进入中断服务程序，
 *       请不要使能 NVIC的中断使能。
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sgl_sqr_trigger_src_set (
         amhw_hc32f07x_adc_t                *p_hw_adc,
         amhw_hc32f07x_adc_int_trigger_src_t type)
{
    p_hw_adc->exttrigger0 |= (0x1ul << type);
}

/**
 * \brief 单次转换或顺序扫描转换外部中断触发源清除
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f07x_adc_int_trigger_src_t枚举量）
 *
 * \note TIM4/5/6中断触发 ADC自动转换，除了需要使能 TIM4/5/6 的相应中断外，还需要配置Advanced Timer
 *       的展频及中断触发选择寄存器 TIMX_CR 选择可以触发 ADC 的中断源
 *
 *       触发 ADC 使用的是各中断标志位的上升沿。如果需要重复触发，需要清除中断标志。如果不需要进入中断服务程序，
 *       请不要使能 NVIC 的中断使能。
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sgl_sqr_trigger_src_clr (
         amhw_hc32f07x_adc_t                *p_hw_adc,
         amhw_hc32f07x_adc_int_trigger_src_t type)
{
    p_hw_adc->exttrigger0 &= ~(0x1ul << type);
}

/**
 * \brief 插队扫描转换外部中断触发源设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f07x_adc_int_trigger_src_t枚举量）
 *
 * \note TIM4/5/6中断触发 ADC自动转换，除了需要使能 TIM4/5/6 的相应中断外，还需要配置Advanced Timer
 *       的展频及中断触发选择寄存器 TIMX_CR 选择可以触发 ADC 的中断源
 *
 *       触发 ADC 使用的是各中断标志位的上升沿。如果需要重复触发，需要清除中断标志。如果不需要进入中断服务程序，
 *       请不要使能 NVIC 的中断使能。
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_jqr_trigger_src_set (
         amhw_hc32f07x_adc_t                *p_hw_adc,
         amhw_hc32f07x_adc_int_trigger_src_t type)
{
    p_hw_adc->exttrigger1 |= (0x1ul << type);
}

/**
 * \brief 插队扫描转换外部中断触发源清除
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f07x_adc_int_trigger_src_t枚举量）
 *
 * \note TIM4/5/6中断触发 ADC自动转换，除了需要使能 TIM4/5/6 的相应中断外，还需要配置Advanced Timer
 *       的展频及中断触发选择寄存器 TIMX_CR选择可以触发 ADC的中断源
 *
 *       触发 ADC使用的是各中断标志位的上升沿。如果需要重复触发，需要清除中断标志。如果不需要进入中断服务程序，
 *       请不要使能 NVIC的中断使能。
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_jqr_trigger_src_clr (
         amhw_hc32f07x_adc_t                *p_hw_adc,
         amhw_hc32f07x_adc_int_trigger_src_t type)
{
    p_hw_adc->exttrigger1 &= ~(0x1ul << type);
}

/**
 * \brief 单次转换启动
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_single_convert_start (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->sglstart |= (0x1ul << 0);
}

/**
 * \brief 单次转换停止
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_single_convert_stop (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->sglstart &= ~(0x1ul << 0);
}

/**
 * \brief 顺序扫描转换启动
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sqr_convert_start (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->sqrstart |= (0x1ul << 0);
}

/**
 * \brief 顺序扫描转换停止
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_sqr_convert_stop (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->sqrstart &= ~(0x1ul << 0);
}

/**
 * \brief 插队扫描转换启动
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_jqr_convert_start (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->jqrstart |= (0x1ul << 0);
}

/**
 * \brief 插队扫描转换停止
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_jqr_convert_stop (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->jqrstart &= ~(0x1ul << 0);
}


/**
 * \brief 一直转换启动
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \note 停止ADC一直转换,需要等待到当次转换结束
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_always_convert_start (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->jqrstart |= (0x1ul << 0);
}

/**
 * \brief 一直转换停止
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \note 停止ADC一直转换,需要等待到当次转换结束
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_adc_always_convert_stop (amhw_hc32f07x_adc_t *p_hw_adc)
{
    p_hw_adc->jqrstart &= ~(0x1ul << 0);
}

/**
 * \brief 一直转换停止
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \note 停止ADC一直转换,需要等待到当次转换结束
 *
 * \return AM_TRUE : 正在转换
 *         AM_FALSE: 无转换
 */
am_static_inline
am_bool_t amhw_hc32f07x_adc_always_convert_get (amhw_hc32f07x_adc_t *p_hw_adc)
{
    return (p_hw_adc->jqrstart & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

/* 允许匿名联合体使能 */
#elif defined(__GNUC__)

/* 默认使用匿名联合体 */
#elif defined(__TMS470__)

/* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F07X_ADC_H */

/* end of file */
