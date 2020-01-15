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
 * - 1.00 17-8-28  fra, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32_DAC_H
#define __AMHW_MM32_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_mm32_if_dac
 * \copydoc amhw_mm32_dac.h
 * @{
 */

/**
 * \brief DAC - Register Layout Typedef
 */
typedef struct amhw_mm32_dac {
    __IO uint32_t daccr;        /**< \brief configure register */
    __O  uint32_t dacswt;       /**< \brief software trigle register */
    __IO uint32_t dacdhr12r1;   /**< \brief DAC channel1 12-bit right-aligned data */
    __IO uint32_t dacdhr12l1;   /**< \brief DAC channel1 12-bit left-aligned data */
    __IO uint32_t dacdhr8r1;    /**< \brief DAC channel1 8-bit right-aligned data */
    __IO uint32_t dacdhr12r2;   /**< \brief DAC channel2 12-bit right-aligned data */
    __IO uint32_t dacdhr12l2;   /**< \brief DAC channel2 12-bit left-aligned data */
    __IO uint32_t dacdhr8r2;    /**< \brief DAC channel2 8-bit right-aligned data */
    __IO uint32_t dacdhr12rd;   /**< \brief DAC channel2 12-bit right-aligned data */
    __IO uint32_t dacdhr12ld;   /**< \brief DAC channel2 12-bit left-aligned data */
    __IO uint32_t dacdhr8rd;    /**< \brief DAC channel2 8-bit right-aligned data */
    __I  uint32_t dacdor1;      /**< \brief DAC channel1 data output */
    __I  uint32_t dacdor2;      /**< \brief DAC channel2 data output */
} amhw_mm32_dac_t;

/**
 * \brief DAC 通道编号
 *
 * \note 通道1对应DAC1 通道2对对应DAC2
 */
#define AMHW_MM32_DAC_CHAN_1          0      /**< \brief DAC通道 1 */
#define AMHW_MM32_DAC_CHAN_2          1      /**< \brief DAC通道 2 */

/**
 * \brief DAC channel2 DMA enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval : none
 */
am_static_inline
void amhw_mm32_dac_chan2_dma_enbale (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 28);
}

/**
 * \brief DAC channel2 DMA disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval : none
 */
am_static_inline
void amhw_mm32_dac_chan2_dma_disbale (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 28);
}

/*
 * \brief DAC channel2 mask/amplitude selector
 */

#define AMHW_MM32_DAC_CHAN2_MASK_BIT0_AMPLITUDE1     (0U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT1_AMPLITUDE3     (1U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT2_AMPLITUDE7     (2U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT3_AMPLITUDE15    (3U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT4_AMPLITUDE31    (4U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT5_AMPLITUDE63    (5U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT6_AMPLITUDE127   (6U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT7_AMPLITUDE255   (7U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT8_AMPLITUDE511   (8U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT9_AMPLITUDE1023  (9U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT10_AMPLITUDE2047 (10U << 24)
#define AMHW_MM32_DAC_CHAN2_MASK_BIT11_AMPLITUDE4095 (11U << 24)

/*
 * \brief DAC channel2 mask/amplitude selector
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] flag     : see the filed of DAC channel2 mask/amplitude selector
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_mask_ampl_sel (amhw_mm32_dac_t *p_hw_dac,
                                          uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & ~(0xf << 24)) | flag;
}

/*
 * \brief DAC channel2 noise/triangle wave generation enable mask
 */

#define AMHW_MM32_DAC_CHAN2_CLOSE_WAV_GEN (0U << 22)
#define AMHW_MM32_DAC_CHAN2_NOIS_WAV_GEN  (1U << 22)
#define AMHW_MM32_DAC_CHAN2_TRIAN_WAV_GEN (2U << 22)

/*
 * \brief DAC channel2 noise/triangle wave generation enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] flag     : see the filed of DAC channel2 noise/triangle wave generation enable mask
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_wav_gen_enable (amhw_mm32_dac_t *p_hw_dac,
                                           uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(3U << 22))) | flag;
}

/*
 * \brief DAC channel2 trigger selection mask
 *
 * \note can be only set at DAC channel2 trigger enable
 */
#define AMHW_MM32_DAC_CHAN2_TIM1_TRGO    (0U << 19)
#define AMHW_MM32_DAC_CHAN2_TIM2_TRGO    (1U << 19)
#define AMHW_MM32_DAC_CHAN2_TIM3_TRGO    (4U << 19)
#define AMHW_MM32_DAC_CHAN2_TIM4_TRGO    (5U << 19)
#define AMHW_MM32_DAC_CHAN2_EXTER_9      (6U << 19)
#define AMHW_MM32_DAC_CHAN2_SOFTWARE_TRG (7U << 19)

/*
 * \brief DAC channel2 trigger selection
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] flag     : see the filed of DAC channel2 trigger selection mask
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_trg_sel (amhw_mm32_dac_t *p_hw_dac, uint32_t flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(0x7u << 19))) | flag;
}

/*
 * \brief DAC channel2 trigger enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_trg_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 18);
}

/*
 * \brief DAC channel2 trigger disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_trg_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 18);
}

/*
 * \brief DAC channel2 output buffer disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_output_buf_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 17);
}

/*
 * \brief DAC channel2 output buffer enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_output_buf_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 17);
}

/*
 * \brief DAC channel2 enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 16);
}

/*
 * \brief DAC channel2 disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 16);
}

/**
 * \brief DAC channel1 DMA enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval : none
 */
am_static_inline
void amhw_mm32_dac_chan1_dma_enbale (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 12);
}

/**
 * \brief DAC channel1 DMA disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval : none
 */
am_static_inline
void amhw_mm32_dac_chan1_dma_disbale (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 12);
}

/*
 * \brief DAC channel1 mask/amplitude selector
 */

#define AMHW_MM32_DAC_CHAN1_MASK_BIT0_AMPLITUDE1     (0U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT1_AMPLITUDE3     (1U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT2_AMPLITUDE7     (2U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT3_AMPLITUDE15    (3U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT4_AMPLITUDE31    (4U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT5_AMPLITUDE63    (5U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT6_AMPLITUDE127   (6U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT7_AMPLITUDE255   (7U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT8_AMPLITUDE511   (8U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT9_AMPLITUDE1023  (9U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT10_AMPLITUDE2047 (10U << 8)
#define AMHW_MM32_DAC_CHAN1_MASK_BIT11_AMPLITUDE4095 (11U << 8)

/*
 * \brief DAC channel1 mask/amplitude selector
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] flag     : see the filed of DAC channel1 mask/amplitude selector
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_mask_ampl_sel (amhw_mm32_dac_t *p_hw_dac,
                                          uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & ~(0xf << 8)) | flag;
}

/*
 * \brief DAC channel1 noise/triangle wave generation enable mask
 */

#define AMHW_MM32_DAC_CHAN1_CLOSE_WAV_GEN (0U << 6)
#define AMHW_MM32_DAC_CHAN1_NOIS_WAV_GEN  (1U << 6)
#define AMHW_MM32_DAC_CHAN1_TRIAN_WAV_GEN (2U << 6)

/*
 * \brief DAC channel1 noise/triangle wave generation enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] flag     : see the filed of DAC channel1 noise/triangle wave generation enable mask
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_wav_gen_enable (amhw_mm32_dac_t *p_hw_dac,
                                           uint32_t           flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(3U << 6))) | flag;
}

/*
 * \brief DAC channel1 trigger selection mask
 *
 * \note can be only set at DAC channel1 trigger enable
 */
#define AMHW_MM32_CHAN1_DAC_TIM1_TRGO    (0U << 3)
#define AMHW_MM32_CHAN1_DAC_TIM2_TRGO    (1U << 3)
#define AMHW_MM32_CHAN1_DAC_TIM3_TRGO    (4U << 3)
#define AMHW_MM32_CHAN1_DAC_TIM4_TRGO    (5U << 3)
#define AMHW_MM32_CHAN1_DAC_EXTER_9      (6U << 3)
#define AMHW_MM32_CHAN1_DAC_SOFTWARE_TRG (7U << 3)

/*
 * \brief DAC channel1 trigger selection
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] flag     : see the filed of DAC channel1 trigger selection mask
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_trg_sel (amhw_mm32_dac_t *p_hw_dac, uint32_t flag)
{
    p_hw_dac->daccr = (p_hw_dac->daccr & (~(0x7u << 3))) | flag;
}

/*
 * \brief DAC channel1 trigger enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_trg_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 2);
}

/*
 * \brief DAC channel1 trigger disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_trg_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 2);
}

/*
 * \brief DAC channel1 output buffer disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_output_buf_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 1);
}

/*
 * \brief DAC channel1 output buffer enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_output_buf_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 1);
}

/*
 * \brief DAC channel1 enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 0);
}

/*
 * \brief DAC channel1 disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 0);
}

/**
 * \brief DAC prescaler value  set
 *
 * \param[in] p_hw_dac   : The pointer to the structure DAC register
 * \param[in] pre_val    : DAC prescaler value
 *
 * \return none
 */
am_static_inline
void amhw_mm32_dac_prescaler_val (amhw_mm32_dac_t  *p_hw_dac,
                                    uint32_t            pre_val)
{
    p_hw_dac->dacswt = (p_hw_dac->dacswt & (~(0x7f << 8))) | (pre_val << 8);
}

/*
 * \brief DAC channel2 software trigle enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_software_trg_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 1);
}

/*
 * \brief DAC channel2 software trigle disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_software_trg_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 1);
}

/*
 * \brief DAC channel1 software trigle enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_software_trg_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 0);
}

/*
 * \brief DAC channel1 software trigle disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_software_trg_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 0);
}

/*
 * \brief DAC channel1 12-bit right-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] data : channel1 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_12bit_right_aligned_data (
                                                    amhw_mm32_dac_t *p_hw_dac,
                                                    uint16_t           data)
{
    p_hw_dac->dacdhr12r1 = (p_hw_dac->dacdhr12r1 & (~(0xfff))) | data;
}

/*
 * \brief DAC channel1 12-bit left-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] data : channel1 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_12bit_left_aligned_data (amhw_mm32_dac_t *p_hw_dac,
                                                    uint16_t           data)
{
    p_hw_dac->dacdhr12l1 = (p_hw_dac->dacdhr12r1 & (~(0xfff << 4))) | (data << 4);

}

/*
 * \brief DAC channel1 8-bit right-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] data : channel1 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_8bit_right_aligned_data (amhw_mm32_dac_t *p_hw_dac,
                                                    uint8_t            data)
{
    p_hw_dac->dacdhr8r1 = (p_hw_dac->dacdhr8r1 & (~(0xff << 8))) | data;
}

/*
 * \brief DAC channel2 12-bit right-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] data : channel2 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_12bit_right_aligned_data (
                                                    amhw_mm32_dac_t *p_hw_dac,
                                                    uint16_t           data)
{
    p_hw_dac->dacdhr12r2 = (p_hw_dac->dacdhr12r2 & (~(0xfff))) | data;
}

/*
 * \brief DAC channel2 12-bit left-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] data : channel2 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_12bit_left_aligned_data (amhw_mm32_dac_t *p_hw_dac,
                                                    uint16_t           data)
{
    p_hw_dac->dacdhr12l2 = (p_hw_dac->dacdhr12l2 & (~(0xfff << 4))) | (data << 4);

}

/*
 * \brief DAC channel2 8-bit right-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] chan1_data : channel1 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan2_8bit_right_aligned_data (amhw_mm32_dac_t *p_hw_dac,
                                                    uint8_t            data)
{
    p_hw_dac->dacdhr8r2 = (p_hw_dac->dacdhr8r2 & (~(0xff << 8))) | data;
}

/*
 * \brief DAC channel1-2 12-bit right-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] chan1_data : channel1 data
 * \param[in] chan2_data : channel2 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_chan2_12bit_right_aligned_data (
                                                  amhw_mm32_dac_t *p_hw_dac,
                                                  uint16_t           chan1_data,
                                                  uint16_t           chan2_data)
{
    p_hw_dac->dacdhr12rd = ((p_hw_dac->dacdhr12rd & (~0xfff)) | chan1_data) |
                           ((p_hw_dac->dacdhr12rd & (~(0xfff << 16))) | (chan2_data << 16));
}

/*
 * \brief DAC channel1-2 12-bit left-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] chan1_data : channel1 data
 * \param[in] chan2_data : channel2 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_chan2_12bit_left_aligned_data (
                                                  amhw_mm32_dac_t *p_hw_dac,
                                                  uint16_t           chan1_data,
                                                  uint16_t           chan2_data)
{
    p_hw_dac->dacdhr12ld = ((p_hw_dac->dacdhr12ld & (~0xffful << 4)) | chan1_data) |
                           ((p_hw_dac->dacdhr12ld & (~(0xffful << 20))) | (chan2_data << 20));
}

/*
 * \brief DAC channel1-2 8-bit right-aligned data
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 * \param[in] chan1_data : channel1 data
 * \param[in] chan2_data : channel2 data
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_chan1_chan2_8bit_right_aligned_data (
                                                  amhw_mm32_dac_t *p_hw_dac,
                                                  uint8_t            chan1_data,
                                                  uint8_t            chan2_data)
{
    p_hw_dac->dacdhr8rd = ((p_hw_dac->dacdhr8rd & (~0xff)) | chan1_data) |
                          ((p_hw_dac->dacdhr8rd & (~(0xff << 8))) | (chan2_data << 8));
}

/*
 * \brief DAC channel1 data output
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: channel1 output data
 */
am_static_inline
uint16_t amhw_mm32_dac_chan1_output_data (amhw_mm32_dac_t *p_hw_dac)
{
    return (p_hw_dac->dacdor1 & 0xfff);
}

/*
 * \brief DAC channel2 data output
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: channel2 output data
 */
am_static_inline
uint16_t amhw_mm32_dac_chan2_output_data (amhw_mm32_dac_t *p_hw_dac)
{
    return (p_hw_dac->dacdor2 & 0xfff);
}

/*
 * \brief DAC enable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_enable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr |= (1U << 16);
    p_hw_dac->daccr |= (1U << 0);
}

/*
 * \brief DAC disable
 *
 * \param[in] p_hw_dac : The pointer to the structure DAC register
 *
 * \retval: none
 */
am_static_inline
void amhw_mm32_dac_disable (amhw_mm32_dac_t *p_hw_dac)
{
    p_hw_dac->daccr &= ~(1U << 16);
    p_hw_dac->daccr &= ~(1U << 0);
}

/**
 * @} amhw_if_mm32_dac
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32_DAC_H */

/* end of file */
