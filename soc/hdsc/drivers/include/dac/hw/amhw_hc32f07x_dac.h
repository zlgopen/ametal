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
 * \brief DAC操作接口
 * \internal
 * \par Modification History
 * - 1.00 19-09-24
 * \endinternal
 */

#ifndef __AMHW_HC32_DAC_H
#define __AMHW_HC32_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_hc32_if_dac
 * \copydoc amhw_hc32_dac.h
 * @{
 */

/**
 * \brief DAC - 寄存器组
 */
typedef struct amhw_hc32f07x_dac {
    __IO uint32_t daccr;        /**< \brief 0x000 DAC 控制寄存器 */
    __O  uint32_t dacswt;       /**< \brief 0x004 DAC 软件触发定时器 */
    __IO uint32_t dacdhr12r0;   /**< \brief 0x008 DAC 12位右对齐数据寄存器 */
    __IO uint32_t dacdhr12l0;   /**< \brief 0x00c DAC 12位左对齐数据寄存器 */
    __IO uint32_t dacdhr8r0;    /**< \brief 0x010 DAC 8位右对齐数据寄存器 */
    __IO uint32_t dacdhr12r1;   /**< \brief 0x014 DAC 12位右对齐数据寄存器 */
    __IO uint32_t dacdhr12l1;   /**< \brief 0x018 DAC 12位左对齐数据寄存器 */
    __IO uint32_t dacdhr8r1;    /**< \brief 0x01c DAC 8位右对齐数据寄存器 */
	  __IO uint32_t dacdhr12rd;   /**< \brief 0x020 双DAC 12位右对齐数据寄存器 */
    __IO uint32_t dacdhr12ld;   /**< \brief 0x024 双DAC 12位左对齐数据寄存器 */
    __IO uint32_t dacdhr8rd;    /**< \brief 0x028 DAC 8位右对齐数据寄存器 */
    __IO uint32_t dacdor0;      /**< \brief 0x02c DAC 数据输出寄存器 */
	  __IO uint32_t dacdor1;      /**< \brief 0x030 DAC 数据输出寄存器 */
    __IO uint32_t dacsr;        /**< \brief 0x034 DAC 状态寄存器 */
    __IO uint32_t dacetrs;      /**< \brief 0x038 DAC 测试寄存器/外部触发选择寄存器 */
} amhw_hc32f07x_dac_t;

/**
 * \brief DAC 通道编号
 *
 * \note 通道1对应DAC1 通道2对对应DAC2
 */
#define AMHW_HC32F07X_DAC_CHAN_0          0      /**< \brief DAC通道 0 */
#define AMHW_HC32F07X_DAC_CHAN_1          1      /**< \brief DAC通道 1 */

/*
 * \brief DAC 0/1通道参考电压选择掩码
 */
#define AMHW_HC32F07X_DAC_CHAN_MASK_INTER_1_5     (0U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_INTER_2_5     (1U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_EXTER_REF     (2U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_AVCC_VOLT     (3U)

/**
 * \brief DAC 0通道参考电压选择
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC 0通道参考电压选择掩码
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_sref_sel (amhw_hc32f07x_dac_t *p_hw_dac, uint32_t  flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(3U << 14))) | (flag << 14);
}

/**
 * \brief DAC 1通道参考电压选择
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC 1通道参考电压选择掩码
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_sref_sel (amhw_hc32f07x_dac_t *p_hw_dac, uint32_t  flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(3U << 30))) | (flag << 30);
}

/**
 * \brief DAC 0通道DMA使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_dma_enbale (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 12);
}

/**
 * \brief DAC 1通道DMA使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_dma_enbale (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 28);
}

/**
 * \brief DAC 0通道DMA失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_dma_disbale (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 12);
}

/**
 * \brief DAC 1通道DMA失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_dma_disbale (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 28);
}

/*
 * \brief DAC 0/1通道掩码/振幅选择器
 */

#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT0_AMPLITUDE1     (0U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT1_AMPLITUDE3     (1U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT2_AMPLITUDE7     (2U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT3_AMPLITUDE15    (3U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT4_AMPLITUDE31    (4U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT5_AMPLITUDE63    (5U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT6_AMPLITUDE127   (6U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT7_AMPLITUDE255   (7U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT8_AMPLITUDE511   (8U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT9_AMPLITUDE1023  (9U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT10_AMPLITUDE2047 (10U)
#define AMHW_HC32F07X_DAC_CHAN_MASK_BIT11_AMPLITUDE4095 (11U)

/*
 * \brief DAC 0通道掩码/振幅选择器 选择
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC0 通道掩码/振幅选择器
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_mask_ampl_sel (amhw_hc32f07x_dac_t *p_hw_dac,
                                         uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & ~(0xf << 8)) | (flag << 8);
}

/*
 * \brief DAC 1通道掩码/振幅选择器 选择
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC1 通道掩码/振幅选择器
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_mask_ampl_sel (amhw_hc32f07x_dac_t *p_hw_dac,
                                         uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & ~(0xf << 24)) | (flag << 24);
}

/*
 * \brief DAC 0通道噪声/三角波生成使能 掩码
 */

#define AMHW_HC32F07X_DAC_CHAN_CLOSE_WAV_GEN (0U)
#define AMHW_HC32F07X_DAC_CHAN_NOIS_WAV_GEN  (1U)
#define AMHW_HC32F07X_DAC_CHAN_TRIAN_WAV_GEN (2U)

/*
 * \brief DAC 0通道噪声/三角波生成使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC0 通道噪声/三角波生成使能 掩码
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_wav_gen_enable (amhw_hc32f07x_dac_t *p_hw_dac,
                                           uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(3U << 6))) | (flag << 6);
}

/*
 * \brief DAC 1通道噪声/三角波生成使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC0 通道噪声/三角波生成使能 掩码
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_wav_gen_enable (amhw_hc32f07x_dac_t *p_hw_dac,
                                           uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(3U << 22))) | (flag << 22);
}

/*
 * \brief DAC 0通道触发器选择掩码
 *
 * \note 只有在通道使能后才能设置
 */
#define AMHW_HC32F07X_DAC_CHAN_TIM0_TRGO    (0U)
#define AMHW_HC32F07X_DAC_CHAN_TIM1_TRGO    (1U)
#define AMHW_HC32F07X_DAC_CHAN_TIM2_TRGO    (2U)
#define AMHW_HC32F07X_DAC_CHAN_TIM3_TRGO    (3U)
#define AMHW_HC32F07X_DAC_CHAN_TIM4_TRGO    (4U)
#define AMHW_HC32F07X_DAC_CHAN_TIM5_TRGO    (5U)
#define AMHW_HC32F07X_DAC_CHAN_SOFTWARE_TRG (6U)
#define AMHW_HC32F07X_DAC_CHAN_EXTER        (7U)

/*
 * \brief DAC 0通道触发器选择
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC0 通道触发器选择掩码
 *
 * \retval: n无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_trg_sel (amhw_hc32f07x_dac_t *p_hw_dac, uint32_t flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(0x7u << 3))) | (flag << 3);
}

/*
 * \brief DAC 1通道触发器选择
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag     : 参考宏定义：DAC1 通道触发器选择掩码
 *
 * \retval: n无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_trg_sel (amhw_hc32f07x_dac_t *p_hw_dac, uint32_t flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(0x7u << 19))) | (flag << 19);
}

/*
 * \brief DAC 0通道触发使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_trg_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 2);
}

/*
 * \brief DAC 1通道触发使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_trg_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 18);
}

/*
 * \brief DAC 0通道触发失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_trg_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 2);
}

/*
 * \brief DAC 1通道触发失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_trg_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 18);
}

/*
 * \brief DAC 0通道输出缓冲器禁止
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: none
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_output_buf_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 1);
}

/*
 * \brief DAC 1通道输出缓冲器禁止
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: none
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_output_buf_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 17);
}

/*
 * \brief  DAC 0通道输出缓冲器使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_output_buf_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 1);
}

/*
 * \brief  DAC 1通道输出缓冲器使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_output_buf_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 17);
}

/*
 * \brief DAC 0通道使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: none
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 0);
}

/*
 * \brief DAC 1通道使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: none
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 16);
}

/*
 * \brief DAC 0通道失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 0);
}

/*
 * \brief DAC 1通道失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 16);
}

/*
 * \brief DAC0 软件触发使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_software_trg_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->dacswt |= (1U << 0);
}

/*
 * \brief DAC1 软件触发使能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_software_trg_enable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->dacswt |= (1U << 1);
}


/*
 * \brief DAC0 软件触发失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: none
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_software_trg_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->dacswt &= ~(1U << 0);
}

/*
 * \brief DAC1 软件触发失能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: none
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_software_trg_disable (amhw_hc32f07x_dac_t *p_hw_dac)
{
    p_hw_dac->dacswt &= ~(1U << 1);
}

/*
 * \brief DAC 通道0 12 位右对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 通道0数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_12bit_right_aligned_data (
	                                              amhw_hc32f07x_dac_t *p_hw_dac,
                                                uint16_t           data)
{
    p_hw_dac->dacdhr12r0 = (p_hw_dac->dacdhr12r0 & (~(0xfff))) | data;
}

/*
 * \brief DAC 通道1 12 位右对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 通道1数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_12bit_right_aligned_data (
                                                amhw_hc32f07x_dac_t *p_hw_dac,
                                                uint16_t           data)
{
    p_hw_dac->dacdhr12r1 = (p_hw_dac->dacdhr12r1 & (~(0xfff))) | data;
}

/*
 * \brief DAC 双通道 12 位右对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 双通道数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32_dacf07x_chan0_1_12bit_right_aligned_data (
                                              amhw_hc32f07x_dac_t *p_hw_dac,
                                              uint16_t           data)
{
    p_hw_dac->dacdhr12rd = (p_hw_dac->dacdhr12rd & (~(0xfff0fff))) | data | (data << 16);
}

/*
 * \brief DAC 通道0 12 位左对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 通道0数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_12bit_left_aligned_data (amhw_hc32f07x_dac_t *p_hw_dac,
                                                   uint16_t           data)
{
    p_hw_dac->dacdhr12l0 = (p_hw_dac->dacdhr12l0 & (~(0xfff << 4))) | (data << 4);

}

/*
 * \brief DAC 通道1 12 位左对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 通道1数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_12bit_left_aligned_data (amhw_hc32f07x_dac_t *p_hw_dac,
                                                   uint32_t           data)
{
    p_hw_dac->dacdhr12l1 = (p_hw_dac->dacdhr12l1 & (~(0xfff << 4))) | (data << 4);
}

/*
 * \brief DAC 双通道 12 位左对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 双通道数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_1_12bit_left_aligned_data (amhw_hc32f07x_dac_t *p_hw_dac,
                                                        uint32_t           data)
{
    p_hw_dac->dacdhr12ld = (data << 20) | (data << 4);	
}

/*
 * \brief DAC 通道0 8 位右对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 通道0数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_8bit_right_aligned_data (amhw_hc32f07x_dac_t *p_hw_dac,
                                                   uint8_t            data)
{
    p_hw_dac->dacdhr8r0 = (p_hw_dac->dacdhr8r0 & ~(0xff)) | data;
}

/*
 * \brief DAC 通道1 8 位右对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 通道1数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan1_8bit_right_aligned_data (amhw_hc32f07x_dac_t *p_hw_dac,
                                                   uint8_t            data)
{
    p_hw_dac->dacdhr8r1 = (p_hw_dac->dacdhr8r1 & ~(0xff)) | data;
}
/*
 * \brief DAC 双通道 8 位右对齐数据
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] data : 双通道数据
 *
 * \retval: 无
 */
am_static_inline
void amhw_hc32f07x_dac_chan0_1_8bit_right_aligned_data (amhw_hc32f07x_dac_t *p_hw_dac,
                                                   uint8_t            data)
{
    p_hw_dac->dacdhr8rd = (p_hw_dac->dacdhr8rd & ~(0xffff)) | data | (data << 8);
}
/*
 * \brief DAC 0通道数据输出
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: DAC 0通道输出数据
 */
am_static_inline
uint16_t amhw_hc32f07x_dac_chan0_output_data (amhw_hc32f07x_dac_t *p_hw_dac)
{
    return (p_hw_dac->dacdor0 & 0xfff);
}

/*
 * \brief DAC 1通道数据输出
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: DAC 1通道输出数据
 */
am_static_inline
uint16_t amhw_hc32f07x_dac_chan1_output_data (amhw_hc32f07x_dac_t *p_hw_dac)
{
    return (p_hw_dac->dacdor1 & 0xfff);
}

/*
 * \brief DAC 0通道 DMA 下溢标志
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: DAC 0通道DMA下溢错误状况标志
 */
am_static_inline
uint16_t amhw_hc32f07x_dac_chan0_dma_status (amhw_hc32f07x_dac_t *p_hw_dac)
{
    return (p_hw_dac->dacsr & 0x00002000 >> 13);
}

/*
 * \brief DAC 1通道 DMA 下溢标志
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 *
 * \retval: DAC 1通道DMA下溢错误状况标志
 */
am_static_inline
uint16_t amhw_hc32f07x_dac_chan1_dma_status (amhw_hc32f07x_dac_t *p_hw_dac)
{
    return (p_hw_dac->dacsr & 0x00002000 >> 29);
}

/*
 * \brief DAC 触发端口选择 掩码
 *
 */
#define AMHW_HC32F07X_DAC_CHAN_TRI_POT_PA9 (0U << 4)
#define AMHW_HC32F07X_DAC_CHAN_TRI_POT_PB9 (1U << 4)
#define AMHW_HC32F07X_DAC_CHAN_TRI_POT_PC9 (2U << 4)
#define AMHW_HC32F07X_DAC_CHAN_TRI_POT_PD9 (3U << 4)
#define AMHW_HC32F07X_DAC_CHAN_TRI_POT_PE9 (4U << 4)
#define AMHW_HC32F07X_DAC_CHAN_TRI_POT_PF9 (5U << 4)

/*
 * \brief DAC 端口触发选择
 *
 * \param[in] p_hw_dac : 指向DAC寄存器组基地址指针
 * \param[in] flag : 参考宏定义：DAC 触发端口选择 掩码
 *
 * \retval: 无
 */
am_static_inline
uint16_t amhw_hc32f07x_dac_chan_tri_port_sel (amhw_hc32f07x_dac_t *p_hw_dac,
                                            uint32_t           flag)
{
    return (p_hw_dac->dacetrs & ~(0x7 << 4)) | flag;
}

/**
 * @} amhw_if_hc32_dac
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_DAC_H */

/* end of file */
