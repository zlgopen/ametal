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
 * - 1.00 17-04-12  ari, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_ZLG_ADC_H
#define __AMHW_ZLG_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zlg_if_adc
 * \copydoc amhw_zlg_adc.h
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
typedef struct amhw_zlg_adc {
    __IO uint32_t addata;    /**< \brief data register */
    __IO uint32_t adcfg;     /**< \brief configure register */
    __IO uint32_t adcr;      /**< \brief control register */
    __IO uint32_t adchs;     /**< \brief channel select register */
    __IO uint32_t adcmpr;    /**< \brief window comparison register */
    __IO uint32_t adsta;     /**< \brief status register */
    __IO uint32_t addr[9];   /**< \brief data address register */
} amhw_zlg_adc_t;

/**
 * \brief check data valid flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \retval AM_TRUE : data valid
 * \retval AM_FALSE: data invalid
 */
am_static_inline
am_bool_t amhw_zlg_adc_data_valid_check (amhw_zlg_adc_t *p_hw_adc)
{
    return (((p_hw_adc->addata >> 21) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief check data overrun flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \retval AM_TRUE : data overrun
 * \retval AM_FALSE: data valid
 */
am_static_inline
am_bool_t amhw_zlg_adc_data_overrun_check (amhw_zlg_adc_t *p_hw_adc)
{
    return (((p_hw_adc->addata >> 20) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief ADC Channel
 * \@{
 */
#define AMHW_ZLG_ADC_CHANNEL0     0U  /**< \brief channel 0 */
#define AMHW_ZLG_ADC_CHANNEL1     1U  /**< \brief channel 1 */
#define AMHW_ZLG_ADC_CHANNEL2     2U  /**< \brief channel 2 */
#define AMHW_ZLG_ADC_CHANNEL3     3U  /**< \brief channel 3 */
#define AMHW_ZLG_ADC_CHANNEL4     4U  /**< \brief channel 4 */
#define AMHW_ZLG_ADC_CHANNEL5     5U  /**< \brief channel 5 */
#define AMHW_ZLG_ADC_CHANNEL6     6U  /**< \brief channel 6 */
#define AMHW_ZLG_ADC_CHANNEL7     7U  /**< \brief channel 7 */
#define AMHW_ZLG_ADC_CHANNEL8     8U  /**< \brief channel 8 */
#define AMHW_ZLG_ADC_CHANNEL9     9U  /**< \brief channel 9 */
#define AMHW_ZLG_ADC_TMEP_SENSOR  10U /**< \brief temperature sensor */
#define AMHW_ZLG_ADC_VOL_SENSOR   11U /**< \brief voltage sensor */
/** @}*/

/**
 * \brief get  ADC channel data
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 * \param[in] data     : data pointer
 *
 * \return ADC channel number
 */
am_static_inline
uint8_t amhw_zlg_adc_data_get (amhw_zlg_adc_t *p_hw_adc,
                               uint16_t       *p_data)
{
    *p_data = (uint16_t)(p_hw_adc->addata & 0xffffu);

    return (uint8_t)((p_hw_adc->addata >> 16) & 0xffu);
}

/**
 * \brief check channel data valid flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 * \param[in] chan     : channel see the filed of  ADC Channel
 *
 * \retval AM_TRUE : data valid
 * \retval AM_FALSE: data invalid
 */
am_static_inline
am_bool_t amhw_zlg_adc_stat_chan_data_valid_check (amhw_zlg_adc_t *p_hw_adc,
                                                   uint8_t         chan)
{
    return (((p_hw_adc->adsta >> (8 + chan)) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief check channel data overrun flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 * \param[in] chan     : channel  see the filed of  ADC Channel
 *
 * \retval AM_TRUE : data overrun
 * \retval AM_FALSE: data valid
 */
am_static_inline
am_bool_t amhw_zlg_adc_stat_chan_data_overrun_check (amhw_zlg_adc_t *p_hw_adc,
                                                     uint8_t         chan)
{
    return (((p_hw_adc->adsta >> (20 + chan)) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief get ADC current transfer channel
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \return ADC channel number
 */
am_static_inline
uint8_t amhw_zlg_adc_currrnt_tran_chan_get (amhw_zlg_adc_t *p_hw_adc)
{
    return (uint8_t)((p_hw_adc->adsta >> 4) & 0xfu);
}

/**
 * \brief check ADC Busy flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \retval AM_TRUE : busy
 * \retval AM_FALSE: idle
 */
am_static_inline
am_bool_t amhw_zlg_adc_stat_busy_check (amhw_zlg_adc_t *p_hw_adc)
{
    return (((p_hw_adc->adsta >> 2) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief check ADC window comparator interrupt  flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \retval AM_TRUE : window comparator interrupt
 * \retval AM_FALSE: no window comparator interrupt
 */
am_static_inline
am_bool_t amhw_zlg_adc_int_win_cmp_check (amhw_zlg_adc_t *p_hw_adc)
{
    return (((p_hw_adc->adsta >> 1) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief check ADC window comparator interrupt  flag clear
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_int_win_cmp_clr (amhw_zlg_adc_t *p_hw_adc)
{
    p_hw_adc->adsta |= 1 << 1;
}

/**
 * \brief check ADC transfer complete interrupt flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \retval AM_TRUE : transfer complete
 * \retval AM_FALSE: transfer not complete
 */
am_static_inline
am_bool_t amhw_zlg_adc_int_tran_complete_check (amhw_zlg_adc_t *p_hw_adc)
{
    return (((p_hw_adc->adsta >> 0) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief check ADC transfer interrupt  flag clear
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_int_tran_complete_clr (amhw_zlg_adc_t *p_hw_adc)
{
    p_hw_adc->adsta |= 1 << 0;
}

/**
 * \brief check channel 0~8 data valid flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 * \param[in] chan     : channel see the filed of  ADC Channel
 *
 * \note channel 0~8
 *
 * \retval AM_TRUE : data valid
 * \retval AM_FALSE: data invalid
 */
am_static_inline
am_bool_t amhw_zlg_adc_chan0_8_valid_check (amhw_zlg_adc_t *p_hw_adc,
                                            uint8_t         chan)
{
    return (((p_hw_adc->addr[chan] >> 21) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief check channel 0~9 data overrun flag
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 * \param[in] chan     : channel  see the filed of  ADC Channel
 *
 * \note channel 0~8
 *
 * \retval AM_TRUE : data overrun
 * \retval AM_FALSE: data valid
 */
am_static_inline
am_bool_t amhw_zlg_adc_chan0_8_overrun_check (amhw_zlg_adc_t *p_hw_adc,
                                              uint8_t         chan)
{
    return (((p_hw_adc->addr[chan] >> 20) & 0x1u) > 0 ? AM_TRUE : AM_FALSE);
}

/**
 * \brief get ADC channel data
 *
 * \param[in] p_hw_adc : The pointer to the structure ADC register
 * \param[in] chan     : channel  see the filed of  ADC Channel
 *
 * \return ADC data
 */
am_static_inline
uint16_t amhw_zlg_adc_chan0_8_data_get (amhw_zlg_adc_t *p_hw_adc,
                                        uint8_t         chan)
{
    return (uint16_t)((p_hw_adc->addr[chan]) & 0xffffu);
}

/**
 * \brief ADC sample time
 */
typedef enum amhw_zlg_adc_sample_time {
    AMHW_ZLG_ADC_CHAN_ST1_5 = 0,
    AMHW_ZLG_ADC_CHAN_ST7_5,
    AMHW_ZLG_ADC_CHAN_ST13_5,
    AMHW_ZLG_ADC_CHAN_ST28_5,
    AMHW_ZLG_ADC_CHAN_ST41_5,
    AMHW_ZLG_ADC_CHAN_ST55_5,
    AMHW_ZLG_ADC_CHAN_ST71_5,
    AMHW_ZLG_ADC_CHAN_ST239_5,
} amhw_zlg_adc_sample_time_t;

/**
 * \brief ADC channel sample time set
 *
 * \param[in] p_hw_adc     : The pointer to the structure ADC register
 * \param[in] sample_time  : sample time
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_chan_sample_time (amhw_zlg_adc_t             *p_hw_adc,
                                    amhw_zlg_adc_sample_time_t  sample_time)
{
    p_hw_adc->adcfg = (p_hw_adc->adcfg & (~(0x7u << 10))) | (sample_time << 10);
}

/**
 * \brief ADC channel sample time get
 *
 * \param[in] p_hw_adc     : The pointer to the structure ADC register
 *
 * \return none
 */
am_static_inline
amhw_zlg_adc_sample_time_t amhw_zlg_adc_chan_sample_time_get (
                                                              amhw_zlg_adc_t *p_hw_adc)
{
    return (amhw_zlg_adc_sample_time_t)((p_hw_adc->adcfg >> 10) & 0x7u);
}

/**
 * \brief ADC resolution
 * \@{
 */
#define AMHW_ZLG_ADC_DATA_VALID_12BIT  12u /**< \brief resolution 12 bit */
#define AMHW_ZLG_ADC_DATA_VALID_11BIT  11u /**< \brief resolution 11 bit */
#define AMHW_ZLG_ADC_DATA_VALID_10BIT  10u /**< \brief resolution 10 bit */
#define AMHW_ZLG_ADC_DATA_VALID_9BIT   9u /**< \brief resolution 9 bit */
#define AMHW_ZLG_ADC_DATA_VALID_8BIT   8u /**< \brief resolution 8 bit */

/** @}*/
/**
 * \brief ADC resolution  set
 *
 * \param[in] p_hw_adc     : The pointer to the structure ADC register
 * \param[in] resolution   : see the field of ADC resolution
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_data_resolution (amhw_zlg_adc_t  *p_hw_adc,
                                   uint8_t          resolution)
{
    p_hw_adc->adcfg = (p_hw_adc->adcfg & (~(0x7u << 7))) | ((12u - resolution) << 7);
}

/**
 * \brief ADC resolution  get
 *
 * \param[in] p_hw_adc     : The pointer to the structure ADC register
 *
 * \return bits
 */
am_static_inline
uint32_t amhw_zlg_adc_data_resolution_get (amhw_zlg_adc_t *p_hw_adc)
{
    return (uint32_t)((p_hw_adc->adcfg >> 7)& 0x7u );
}

/**
 * \brief ADC prescaler value  set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] pre_val    : ADC prescaler value
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_prescaler_val (amhw_zlg_adc_t  *p_hw_adc,
                                 uint32_t         pre_val )
{
    p_hw_adc->adcfg = (p_hw_adc->adcfg & (~(0x7u << 4))) | pre_val << 4;
}

/**
 * \brief ADC prescaler value  get
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 *
 * \return ADC prescaler value
 */
am_static_inline
uint8_t amhw_zlg_adc_prescaler_val_get (amhw_zlg_adc_t  *p_hw_adc)
{
    return ((p_hw_adc->adcfg >> 4)& 0x7u);
}

/**
 * \brief ADC enable mask
 * \@{
 */
#define AMHW_ZLG_ADC_VOL_SENSOR_EN_MASK  (1U << 3) /**< \brief voltage sensor enable */
#define AMHW_ZLG_ADC_TEMP_SENSOR_EN_MASK (1U << 2) /**< \brief temperature sensor enable */
#define AMHW_ZLG_ADC_WINDOWS_EN_MASK     (1U << 1) /**< \brief Window comparison enable */
#define AMHW_ZLG_ADC_MODULE_EN_MASK      (1U << 0) /**< \brief ADC enable */
/** @}*/

/**
 * \brief ADC function enable  set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of ADC enable mask
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_cgf_reg_set (amhw_zlg_adc_t  *p_hw_adc,
                               uint32_t         flag )
{
    p_hw_adc->adcfg |=  flag;
}

/**
 * \brief ADC function disable set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of ADC enable mask
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_cgf_reg_clr (amhw_zlg_adc_t  *p_hw_adc,
                               uint32_t         flag)
{
    p_hw_adc->adcfg &=  ~flag;
}

/**
 * \brief ADC window comparison channel selection
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] chan       : see the filed of ADC  channel
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_win_chan_sel (amhw_zlg_adc_t  *p_hw_adc,
                                uint32_t         chan )
{
    p_hw_adc->adcr =  (p_hw_adc->adcr & (~(0xfu << 12))) | (chan << 12);
}

/**
 * \brief ADC data align way
 * \@{
 */
#define AMHW_ZLG_ADC_DATA_ALIGN_RIGHT (0U) /**< \brief data right align */
#define AMHW_ZLG_ADC_DATA_ALIGN_LEFT  (1U) /**< \brief data left align */

/** @}*/
/**
 * \brief ADC data align way set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of ADC data align way
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_align_way_sel (amhw_zlg_adc_t  *p_hw_adc,
                                 uint32_t         flag)
{
    p_hw_adc->adcr =  (p_hw_adc->adcr & (~(0x1u << 11))) | flag;
}

/**
 * \brief ADC scan mode
 * \@{
 */
#define AMHW_ZLG_ADC_ONE_TIME_SCAN   (0U << 9)  /**< \brief data transfer one time */
#define AMHW_ZLG_ADC_SINGLE_PERIOD_SCAN (1U << 9)  /**< \brief single period scan */
#define AMHW_ZLG_ADC_CONTINUE_SCAN      (2U << 9) /**< \brief continue scan */

/** @}*/
/**
 * \brief ADC scan mode set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of ADC scan mode
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_data_scan_set (amhw_zlg_adc_t  *p_hw_adc,
                                 uint32_t         flag )
{
    p_hw_adc->adcr =  (p_hw_adc->adcr & (~(0x3u << 9))) | flag;
}

/**
 * \brief ADC data transfer status
 * \@{
 */
#define AMHW_ZLG_ADC_DATA_TRANSFER_START   (1U << 8)  /**< \brief data transfer start */
#define AMHW_ZLG_ADC_DATA_TRANSFER_END     (0U << 8)  /**< \brief data transfer end */

/** @}*/

/**
 * \brief ADC data transfer set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of ADC data transfer status
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_data_transfer_set (amhw_zlg_adc_t  *p_hw_adc,
                                     uint32_t         flag)
{
    p_hw_adc->adcr =  (p_hw_adc->adcr & (~(0x1u << 8))) | flag;
}

/**
 * \brief ADC data transfer start check
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of ADC data transfer status
 *
 * \retval   AM_TRUE : Transfer start
 *           AM_FALSE: Transfer end
 */
am_static_inline
am_bool_t amhw_zlg_adc_data_transfer_start_check (amhw_zlg_adc_t *p_hw_adc)
{
    return (((p_hw_adc->adcr >> 8) & 0x1u) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief ADC External trigger selection
 * \@{
 */
#define AMHW_ZLG_ADC_EXTRG_TIM1_CC1  (0U << 4)  /**< \brief External trigger TIM1_CC1 */
#define AMHW_ZLG_ADC_EXTRG_TIM1_CC2  (1U << 4)  /**< \brief External trigger TIM1_CC2 */
#define AMHW_ZLG_ADC_EXTRG_TIM1_CC3  (2U << 4)  /**< \brief External trigger TIM1_CC3 */
#define AMHW_ZLG_ADC_EXTRG_TIM2_CC2  (3U << 4)  /**< \brief External trigger TIM2_CC2 */
#define AMHW_ZLG_ADC_EXTRG_TIM3_TRGO (4U << 4)  /**< \brief External trigger TIM3_TRGO */
#define AMHW_ZLG_ADC_EXTRG_TIM3_CC1  (6U << 4)  /**< \brief External trigger TIM3_CC1 */
#define AMHW_ZLG_ADC_EXTRG_LINE_11   (7U << 4)  /**< \brief External trigger LINE_11 */

/** @}*/

/**
 * \brief ADC External trigger selection
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of ADC External trigger selection
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_extrg_select (amhw_zlg_adc_t  *p_hw_adc,
                                uint32_t         flag)
{
    p_hw_adc->adcr =  (p_hw_adc->adcr & (~(0x7u << 4))) | flag;
}

/**
 * \brief ADC function enable mask
 * \@{
 */
#define AMHW_ZLG_ADC_DMA_EN_MASK     (1U << 3) /**< \brief DMA enable */
#define AMHW_ZLG_ADC_EXTRG_EN_MASK   (1U << 2) /**< \brief External trigger enable */
#define AMHW_ZLG_ADC_INT_WIN_EN_MASK (1U << 1) /**< \brief Window comparison interrupt enable */
#define AMHW_ZLG_ADC_INT_EN_MASK     (1U << 0) /**< \brief ADC interrupt enable */
/** @}*/

/**
 * \brief ADC function enable  set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of function enable mask
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_ctrl_reg_set (amhw_zlg_adc_t  *p_hw_adc,
                                uint32_t         flag)
{
    p_hw_adc->adcr |=  flag;
}

/**
 * \brief ADC function disable set
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] flag       : see the filed of function enable mask
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_ctrl_reg_clr (amhw_zlg_adc_t  *p_hw_adc,
                                uint32_t         flag)
{
    p_hw_adc->adcr &=  ~flag;
}

/**
 * \brief ADC input channel enable
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] chan       : see the filed of ADC  channel
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_input_chan_enable (amhw_zlg_adc_t  *p_hw_adc,
                                     uint32_t         chan)
{
    if (chan == 10 || chan == 11) {
        chan += 4;
    }
    p_hw_adc->adchs |= (1 << chan);
}

/**
 * \brief ADC input channel disable
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] chan       : see the filed of ADC input channel
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_input_chan_disable (amhw_zlg_adc_t  *p_hw_adc,
                                      uint32_t         chan)
{
    p_hw_adc->adchs &= ~ (1 << chan);
}

/**
 * \brief ADC compare data high limit
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] value      : 0 ~ 4095
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_cmp_high_limit (amhw_zlg_adc_t  *p_hw_adc,
                                  uint16_t         value)
{
    p_hw_adc->adcmpr = (p_hw_adc->adcmpr & (~(0xfffu << 16))) |
                       ((value & 0xfffu) << 16);
}

/**
 * \brief ADC compare data low limit
 *
 * \param[in] p_hw_adc   : The pointer to the structure ADC register
 * \param[in] value      : 0 ~ 4095
 *
 * \return none
 */
am_static_inline
void amhw_zlg_adc_cmp_low_limit (amhw_zlg_adc_t  *p_hw_adc,
                                 uint16_t         value)
{
    p_hw_adc->adcmpr = (p_hw_adc->adcmpr & (~(0xfffu << 0))) |
                       ((value & 0xfffu) << 0);
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

#endif /* __AMHW_ZLG_ADC_H */

/* end of file */
