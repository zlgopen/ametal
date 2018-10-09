/*******************************************************************************
*                                AMetal
*                      ----------------------------
*                      innovating embedded platform
*
*Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
*All rights reserved.
*
*Contact information:
*web site:    http://www.zlg.cn/
*e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC interfaces for operating hardware.
 *
 * - 12-bit successive approximation analogy to digital converter.
 * - Input multiplexing among up to 12 pins.
 * - Two configurable conversion sequences with independent triggers.
 * - Optional automatic high/low threshold comparison and zero crossing detection.
 * - Measurement range VREFNto VREFP(typically 3 V; not to exceed VDDAvoltage level).
 * - 12-bit conversion rate of 4.8 MHz. Options for reduced resolution at higher conversion
 *   rates.
 * - Burst conversion mode for single or multiple inputs.
 * - Synchronous or asynchronous operation. Asynchronous operation maximizes
 *   flexibility in choosing the ADC clock frequency, Synchronous mode minimizes trigger
 *   latency and can eliminate uncertainty and jitter in response to a trigger.
 *
 * \internal
 * \par Modification history
 * - 1.10 14-12-01  hbt, modified.
 * - 1.00 14-12-01  tee, first implementation.
 * \endinternal
 */
#ifndef  __AMHW_LPC_ADC_H
#define  __AMHW_LPC_ADC_H

#ifdef __cplusplus
extern "C" {
#endif
#include "am_types.h"
#include "am_common.h"

/**
 * \addtogroup amhw_lpc_if_adc 
 * \copydoc amhw_lpc_adc.h
 * @{
 */

/**
 * \brief Start of section using anonymous unions
 * @{
 */
 
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
    
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)

  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */


/**
 * \brief ADC register block structure
 */
typedef struct amhw_lpc_adc {
    volatile uint32_t ctrl;                      /**< \brief ADC control register         */
    volatile uint32_t inputsel;                  /**< \brief ADC input control register   */
    union {
        volatile uint32_t seq_ctrl[2];
        struct {
            volatile uint32_t seqa_ctrl;         /**< \brief ADC SEQA control register    */
            volatile uint32_t seqb_ctrl;         /**< \brief ADC SEQB control Register    */
        };
    };
    union {
        const volatile uint32_t seq_gdat[2];
        struct {
            const volatile uint32_t seqa_gdat;   /**< \brief ADC SEQA global data register */
            const volatile uint32_t seqb_gdat;   /**< \brief ADC SEQB global data register */
        };
    };
    uint32_t reserved[2];                        /**< \brief Reserved,do not use           */
    union {
        const volatile uint32_t dat[12];         /**< \brief ADC channel data register     */
        struct {
            const volatile uint32_t dat0;        /**< \brief ADC channel data register 0   */
            const volatile uint32_t dat1;        /**< \brief ADC channel data register 1   */
            const volatile uint32_t dat2;        /**< \brief ADC channel data register 2   */
            const volatile uint32_t dat3;        /**< \brief ADC channel data register 3   */
            const volatile uint32_t dat4;        /**< \brief ADC channel data register 4   */
            const volatile uint32_t dat5;        /**< \brief ADC channel data register 5   */
            const volatile uint32_t dat6;        /**< \brief ADC channel data register 6   */
            const volatile uint32_t dat7;        /**< \brief ADC channel data register 7   */
            const volatile uint32_t dat8;        /**< \brief ADC channel data register 8   */
            const volatile uint32_t dat9;        /**< \brief ADC channel data register 9   */
            const volatile uint32_t dat10;       /**< \brief ADC channel data register 10  */
            const volatile uint32_t dat11;       /**< \brief ADC channel data register 11  */
        };
    };
    union {
        volatile uint32_t thr_low[2];
        struct {
            volatile uint32_t thr0_low;          /**< \brief ADC low threshold register 0. */
            volatile uint32_t thr1_low;          /**< \brief ADC low threshold register 1. */
        };
    };
    union {
        volatile uint32_t thr_high[2];
        struct {
            volatile uint32_t thr0_high;         /**< \brief ADC high threshold register 0. */
            volatile uint32_t thr1_high;         /**< \brief ADC high threshold register 1. */
        };
    };
    volatile uint32_t chan_thrsel;       /**< \brief ADC channel threshold select register. */
    volatile uint32_t inten;             /**< \brief ADC interrupt enable register.         */
    volatile uint32_t flags;             /**< \brief ADC interrupt request flags register.  */
    volatile uint32_t startup;           /**< \brief ADC start up register.                 */
    volatile uint32_t calibr;            /**< \brief ADC calibration register.              */
} amhw_lpc_adc_t;


/**
 * \name ADC channel register index
 * \anchor grp_amhw_lpc_adc_chan
 * @{
 */

#define AMHW_LPC_ADC0_CHAN_0          0          /**< \brief ADC channel 0  */
#define AMHW_LPC_ADC0_CHAN_1          1          /**< \brief ADC channel 1  */
#define AMHW_LPC_ADC0_CHAN_2          2          /**< \brief ADC channel 2  */
#define AMHW_LPC_ADC0_CHAN_3          3          /**< \brief ADC channel 3  */
#define AMHW_LPC_ADC0_CHAN_4          4          /**< \brief ADC channel 4  */
#define AMHW_LPC_ADC0_CHAN_5          5          /**< \brief ADC channel 5  */
#define AMHW_LPC_ADC0_CHAN_6          6          /**< \brief ADC channel 6  */
#define AMHW_LPC_ADC0_CHAN_7          7          /**< \brief ADC channel 7  */
#define AMHW_LPC_ADC0_CHAN_8          8          /**< \brief ADC channel 8  */
#define AMHW_LPC_ADC0_CHAN_9          9          /**< \brief ADC channel 9  */
#define AMHW_LPC_ADC0_CHAN_10         10         /**< \brief ADC channel 10 */
#define AMHW_LPC_ADC0_CHAN_11         11         /**< \brief ADC channel 11 */

/** @} */

#define AMHW_LPC_ADC_CHAN_CNT         12         /**< \brief Maximum channel count */

/**
 * \brief ADC Sequence register index
 */
typedef enum amhw_lpc_adc_seq_index {
    AMHW_LPC_ADC_SEQ_A = 0,                      /**< \brief Index for Sequence A */
    AMHW_LPC_ADC_SEQ_B,                          /**< \brief Index for Sequence B */
} amhw_lpc_adc_seq_index_t;


/**
 * \brief ADC Compare Threshold index
 */
typedef enum amhw_lpc_adc_comp_thre_index {
    AMHW_LPC_ADC_COMP_THRE_0,                    /**< \brief Index for compare threshold 0 */
    AMHW_LPC_ADC_COMP_THRE_1,                    /**< \brief Index for compare threshold 1 */
} amhw_lpc_adc_comp_thre_index_t;


/**
 * \name ADC Control register bit fields 
 * \anchor grp_amhw_lpc_adc_ctrl
 * @{
 */
 
#define AMHW_LPC_ADC_CTRL_CLK_DIV(x)     ((x) & 0xFF)         /**< \brief Set the divided value to @a x */

#define AMHW_LPC_ADC_CTRL_MODE_SYNC      (0UL << 8)           /**< \brief Set ADC to synchoronous mode  */
#define AMHW_LPC_ADC_CTRL_MODE_ASYNC     (1UL << 8)           /**< \brief Set ADC to asynchoronous mode */

#define AMHW_LPC_ADC_CTRL_RESOL_6BIT     (0UL << 9)           /**< \brief Set ADC resolution to 6 bits  */
#define AMHW_LPC_ADC_CTRL_RESOL_8BIT     (1UL << 9)           /**< \brief Set ADC resolution to 8 bits  */
#define AMHW_LPC_ADC_CTRL_RESOL_10BIT    (2UL << 9)           /**< \brief Set ADC resolution to 10 bits */
#define AMHW_LPC_ADC_CTRL_RESOL_12BIT    (3UL << 9)           /**< \brief Set ADC resolution to 12 bits */
#define AMHW_LPC_ADC_CTRL_RESOL_NBIT(n)  (((n / 2) - 3) << 9) /**< \brief Set ADC resolution to 12 bits */

#define AMHW_LPC_ADC_CTRL_BYPASS_CALIB   (1UL << 11)          /**< \brief Bypass calibration data       */

#define AMHW_LPC_ADC_CTRL_CALIB          (0UL << 11)          /**< \brief calibration data              */

#define AMHW_LPC_ADC_CTRL_SAMPLE_TIME(x) (((x) & 7) << 12)    /**< \brief Set the sample time to @a x   */

/** @} */

/**
 * \brief ADC conversion sequence configure.
 *
 * \param[in] p_hw_adc : Pointer to A register block.
 *
 * \retval  AM_OK      : Start up succeed.
 * \retval -AM_ETIME   : Time out.
 */
int amhw_lpc_adc_startup(amhw_lpc_adc_t *p_hw_adc);

/**
 * \brief Set the ADC Calibration
 *
 * \param[in] p_hw_adc    : Pointer to ADC register block
 * \param[in] sysclk_freq : main clock frequency
 *
 * \return  None
 */
void amhw_lpc_adc_calibrate(amhw_lpc_adc_t *p_hw_adc, uint32_t sysclk_freq);

/**
 * \brief ADC control configure
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] flags    : the function state to set
 *                       \ref grp_amhw_lpc_adc_ctrl
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_adc_config (amhw_lpc_adc_t *p_hw_adc, uint32_t flags)
{
    p_hw_adc->ctrl = flags;
}

/**
 * \brief Get the ADC control register State 
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \return The control register state 
 */
am_static_inline
uint32_t amhw_lpc_adc_ctrl_get (amhw_lpc_adc_t *p_hw_adc)
{
    return p_hw_adc->ctrl;
}


/**
 * \name ADC Sequence Control register bit fields  
 * \anchor grp_amhw_lpc_adc_seq_ctrl
 * @{
 */

#define AMHW_LPC_ADC_SEQ_CTRL_TRIG_SOFT      (0UL << 12)     /**< \brief Software trigger                 */
#define AMHW_LPC_ADC_SEQ_CTRL_TRIG_PININT0   (0UL << 12)     /**< \brief Hardware trigger on pinint0      */
#define AMHW_LPC_ADC_SEQ_CTRL_TRIG_PININT1   (1UL << 12)     /**< \brief Hardware trigger on pinint1      */
#define AMHW_LPC_ADC_SEQ_CTRL_TRIG_ARMTXEV   (5UL << 12)     /**< \brief Hardware trigger on arm_txev     */
#define AMHW_LPC_ADC_SEQ_CTRL_TRIG_POL_NEG   (0UL << 18)     /**< \brief Trigger polarity is negative     */
#define AMHW_LPC_ADC_SEQ_CTRL_TRIG_POL_POS   (1UL << 18)     /**< \brief Trigger polarity is positive     */

#define AMHW_LPC_ADC_SEQ_CTRL_BYPASS_SYNC    (1UL << 19)     /**< \brief Bypass Synchronization filp-flop */

/** \brief Enable burst mode */
#define AMHW_LPC_ADC_SEQ_CTRL_MODE_BURST     ((1UL << 27) | (0UL << 26))
                                             
#define AMHW_LPC_ADC_SEQ_CTRL_MODE_SINGLE    (1UL << 28)     /**< \brief Enable single mode               */

#define AMHW_LPC_ADC_SEQ_CTRL_SEQA_PIRO_HIGH (1UL << 29)     /**< \brief Set SEQA as high priority        */

#define AMHW_LPC_ADC_SEQ_CTRL_MODE_EOC       (0UL << 30)     /**< \brief Event after end of conversion    */
#define AMHW_LPC_ADC_SEQ_CTRL_MODE_EOS       (1UL << 30)     /**< \brief Event after end of sequence      */

#define AMHW_LPC_ADC_SEQ_CTRL_ENABLE_CH(ch)  (1UL << (ch))   /**< \brief Enable the channel number        */

/** @} */


/**
 * \brief ADC conversion sequence configure
 *
 * \param[in] p_hw_adc : Pointer to A register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 * \param[in] flags    : the function state to set, see
 *                       \ref grp_amhw_lpc_adc_seq_ctrl
 *
 * \return  None
 */
am_static_inline
void amhw_lpc_adc_seq_config (amhw_lpc_adc_t          *p_hw_adc,
                          amhw_lpc_adc_seq_index_t index, 
                          uint32_t             flags)
{
    p_hw_adc->seq_ctrl[index] = flags;
}

/**
 * \brief ADC conversion sequence register state get
 *
 * \param[in] p_hw_adc : Pointer to A register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 *
 * \return  ADC conversion sequence register state
 */
am_static_inline
uint32_t amhw_lpc_adc_seq_ctrl_get (amhw_lpc_adc_t *p_hw_adc, amhw_lpc_adc_seq_index_t index)
{
    return p_hw_adc->seq_ctrl[index];
}

/**
 * \brief Start up the ADC sequence convert
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 *
 * \return  None
 */
am_static_inline
void amhw_lpc_adc_seq_start (amhw_lpc_adc_t *p_hw_adc, amhw_lpc_adc_seq_index_t index)
{
    p_hw_adc->seq_ctrl[index] |= (1UL << 26);
}

/**
 * \brief Stop up the ADC sequence convert
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 *
 * \return  None
 */
am_static_inline
void amhw_lpc_adc_seq_stop (amhw_lpc_adc_t *p_hw_adc, amhw_lpc_adc_seq_index_t index)
{
    p_hw_adc->seq_ctrl[index] &= ~(1UL << 26);
}

/**
 * \brief Set the ADC sequence enable
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 *
 * \return  None
 */
am_static_inline
void amhw_lpc_adc_seq_enable (amhw_lpc_adc_t *p_hw_adc, amhw_lpc_adc_seq_index_t index)
{
    p_hw_adc->seq_ctrl[index] |= (1UL << 31);
}

/**
 * \brief Disable the ADC sequence 
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 *
 * \return  None
 */
am_static_inline
void amhw_lpc_adc_seq_disable (amhw_lpc_adc_t *p_hw_adc, amhw_lpc_adc_seq_index_t index)
{
    p_hw_adc->seq_ctrl[index] &= ~(1UL << 31);
}

/**
 * \brief Set the sequence enable and start conversion
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 *
 * \return  None
 */
am_static_inline  
void amhw_lpc_adc_seq_start_and_enable (amhw_lpc_adc_t          *p_hw_adc, 
                                    amhw_lpc_adc_seq_index_t index)
{
    p_hw_adc->seq_ctrl[index] |= (1UL << 31) | (1UL << 26);
}

/**
 * \brief Enable ADC channel.
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 * \param[in] chan     : The ADC channel, see
 *                       \ref grp_amhw_lpc_adc_chan
 * \return  None
 */
am_static_inline  
void amhw_lpc_adc_seq_chan_set (amhw_lpc_adc_t          *p_hw_adc, 
                            amhw_lpc_adc_seq_index_t index, 
                            int                  chan)
{
    p_hw_adc->seq_ctrl[index] &= ~(1UL << 31);
    p_hw_adc->seq_ctrl[index] |= ((1UL << chan) | (1UL << 31 ));
}

/**
 * \name Data register bit fields 
 * \anchor grp_amhw_lpc_adc_data_reg
 * @{
 */

#define AMHW_LPC_ADC_DR_RESULT(n)     (((n) >> 4) & 0xFFF) /**< \brief Macro for getting the ADC data value */

#define AMHW_LPC_ADC_DR_THCMPRANGE(n) (((n) >> 16) & 0x3)  /**< \brief Threshold Crossing Comparison result */
#define AMHW_LPC_ADC_DR_THCMPCROSS(n) (((n) >> 18) & 0x3)  /**< \brief Threshold Range Comparison result    */

#define AMHW_LPC_ADC_DR_CHANNEL(n)    (((n) >> 26) & 0xF)  /**< \brief Channel number bit position          */

#define AMHW_LPC_ADC_DR_OVERRUN       (1UL << 30)          /**< \brief Overrun bit                          */
#define AMHW_LPC_ADC_DR_DATAVALID     (1UL << 31)          /**< \brief Data valid bit                       */

/** @} */

/**
 * \brief Get the ADC conversion data from channel data register
 *
 * The return value is 'n' of \ref grp_amhw_lpc_adc_data_reg member.
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] chan     : The ADC channel, see
 *                       \ref grp_amhw_lpc_adc_chan
 *
 * \return  the ADC conversion data
 */
am_static_inline
uint32_t amhw_lpc_adc_chan_data_get (amhw_lpc_adc_t *p_hw_adc, int chan)
{
    return (p_hw_adc->dat[chan]);
}

/**
 * \brief Get the ADC conversion data from global data register.
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected ADC conversion sequence,
 *                       (#AMHW_LPC_ADC_SEQ_A) or (#AMHW_LPC_ADC_SEQ_B)
 *
 * \return  The ADC conversion data
 */
am_static_inline
uint32_t amhw_lpc_adc_glo_data_get (amhw_lpc_adc_t *p_hw_adc, amhw_lpc_adc_seq_index_t index)
{
    return (p_hw_adc->seq_gdat[index]);
}

/**
 * \brief Set the ADC compare low threshold
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected register serial number(0-1)
 * \param[in] value    : The threshold value setting
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_adc_thr_low_set (amhw_lpc_adc_t                 *p_hw_adc,
                           amhw_lpc_adc_comp_thre_index_t  index,
                           uint32_t                    value)
{
    p_hw_adc->thr_low[index] |= (value << 4); 
}

/**
 * \brief Set the ADC compare high threshold
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] index    : The selected register serial number(0-1)
 * \param[in] value    : The threshold value setting
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_adc_thr_high_set (amhw_lpc_adc_t                 *p_hw_adc,
                            amhw_lpc_adc_comp_thre_index_t  index,
                            uint32_t                    value)
{
    p_hw_adc->thr_high[index] |= (value << 4);
}

/**
 * \brief Select the ADC Compare Threshold0
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] chan     : The ADC channel, see
 *                       \ref grp_amhw_lpc_adc_chan
 *
 * \return  None
 */
am_static_inline 
void am_nxp_adc_thr0_sel (amhw_lpc_adc_t *p_hw_adc, int chan)
{
    p_hw_adc->chan_thrsel &= ~(1UL << chan); 
}

/**
 * \brief Select the ADC Compare Threshold1
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] chan     : The ADC channel, see
 *                       \ref grp_amhw_lpc_adc_chan
 *
 * \return  None
 */
am_static_inline 
void am_nxp_adc_thr1_sel (amhw_lpc_adc_t *p_hw_adc, int chan)
{
    p_hw_adc->chan_thrsel |= (1UL << chan);
}

/**
 * \name ADC Interrupt Enable register bit fields 
 * \anchor grp_amhw_lpc_adc_inten
 * @{
 */
 
#define AMHW_LPC_ADC_INTEN_SEQA_ENABLE       (1UL << 0)                /**< \brief Sequence a interrupt enable bit    */
#define AMHW_LPC_ADC_INTEN_SEQB_ENABLE       (1UL << 1)                /**< \brief Sequence b interrupt enable bit    */
#define AMHW_LPC_ADC_INTEN_OVRRUN_ENABLE     (1UL << 2)                /**< \brief Overrun interrupt enable bit       */

#define AMHW_LPC_ADC_INTEN_CMP_DISBALE(ch)   (0x0UL << (2 * (ch) + 3)) /**< \brief Disable threshold comparison interrupt         */
#define AMHW_LPC_ADC_INTEN_CMP_OUTSIDETH(ch) (0x1UL << (2 * (ch) + 3)) /**< \brief Enable outside threshold comparison interrupt  */
#define AMHW_LPC_ADC_INTEN_CMP_CROSSTH(ch)   (0x2UL << (2 * (ch) + 3)) /**< \brief Enable Crossing threshold comparison interrupt */

/** @} */

/**
 * \brief Interrupt enable
 *
 * \param[in] p_hw_adc : Pointer to ADC register block
 * \param[in] flag     : The function state to set, see
 *                       \ref grp_amhw_lpc_adc_inten
 * \return  None
 */
am_static_inline 
void amhw_lpc_adc_int_enable (amhw_lpc_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->inten = flag;
}

/**
 * \name ADC Flags register bit fields 
 * \anchor grp_amhw_lpc_adc_flags
 * @{
 */
 
#define AMHW_LPC_ADC_FLAGS_THCMP_MASK(ch)   (1UL << (ch))         /**< \brief Threshold comparison status for channel */
#define AMHW_LPC_ADC_FLAGS_OVRRUN_MASK(ch)  (1UL << (12 + (ch)))  /**< \brief Overrun status for channel */
#define AMHW_LPC_ADC_FLAGS_SEQA_OVRRUN_MASK (1UL << 24)           /**< \brief Seq A overrun status       */
#define AMHW_LPC_ADC_FLAGS_SEQB_OVRRUN_MASK (1UL << 25)           /**< \brief Seq B overrun status       */
#define AMHW_LPC_ADC_FLAGS_SEQA_INT_MASK    (1UL << 28)           /**< \brief Seq A interrupt status     */
#define AMHW_LPC_ADC_FLAGS_SEQB_INT_MASK    (1UL << 29)           /**< \brief Seq B interrupt status     */
#define AMHW_LPC_ADC_FLAGS_THCMP_INT_MASK   (1UL << 30)           /**< \brief Threshold comparison interrupt status */
#define AMHW_LPC_ADC_FLAGS_OVRRUN_INT_MASK  (1UL << 31)           /**< \brief Overrun interrupt status              */

/** @} */

/**
 * \brief Get the flag of interrupt event.
 * 
 * Operating with parameter from \ref grp_amhw_lpc_adc_flags
 *
 * \par Demo
 * \code 
 * if (amhw_lpc_adc_flags_get(p_hw_adc) & AMHW_LPC_ADC_FLAGS_SEQA_INT_MASK) (
 * // do something.
 * )
 * \endcode
 *
 * \param[in] p_hw_adc : Pointer to ADC register block.
 * \return  Interrupt flags.
 */
am_static_inline 
uint32_t amhw_lpc_adc_flags_get (amhw_lpc_adc_t *p_hw_adc)
{
    return p_hw_adc->flags;
}

/**
 * \brief Clear the flag of interrupt event.
 * 
 * Operating with parameter from \ref grp_amhw_lpc_adc_flags.
 *
 * \param[in] p_hw_adc : Pointer to ADC register block.
 * \param[in] flags    : set flags, see \ref grp_amhw_lpc_adc_flags 
 * 
 * 
 * \return  None.
 */
am_static_inline 
void amhw_lpc_adc_flags_clr (amhw_lpc_adc_t *p_hw_adc, uint32_t flags)
{
    p_hw_adc->flags = flags;
}

/**
 * \name Start up setting parameter.
 * \anchor grp_amhw_lpc_adc_startup
 * @{
 */

#define AMHW_LPC_ADC_STARTUP_ENA      (1UL << 0)    /**< \brief ADC Enable         */
#define AMHW_LPC_ADC_STARTUP_INIT     (1UL << 1)    /**< \brief ADC Initialization */

/** @} */

/**
 * \brief ADC start up setting
 *
 * \param[in] p_hw_adc : Pointer to ADC register block   
 * \param[in] flags    : set value, see \ref grp_amhw_lpc_adc_startup 
 * 
 * \return  None
 */
am_static_inline
void amhw_lpc_adc_startup_set (amhw_lpc_adc_t *p_hw_adc, uint32_t flags)
{
    p_hw_adc->startup = flags;
}

/**
 * \brief Get ADC start up
 * \param[in] p_hw_adc : Pointer to ADC register block.
 * \return  start up state
 */
am_static_inline
uint32_t amhw_lpc_adc_startup_get (amhw_lpc_adc_t *p_hw_adc)
{
    return p_hw_adc->startup;
}

/**
 * \brief ADC Calibration request enable.
 * \param[in] p_hw_adc : Pointer to ADC register block.
 * \return  None
 */
am_static_inline
void amhw_lpc_adc_calib_enable (amhw_lpc_adc_t *p_hw_adc)
{
    p_hw_adc->calibr |= (1UL << 0);
}

/**
 * \brief Get ADC Calibration request state.
 * \param[in] p_hw_adc : Pointer to ADC register block.
 * \return  None
 */
am_static_inline
am_bool_t amhw_lpc_adc_calib_get (amhw_lpc_adc_t *p_hw_adc)
{
    return (am_bool_t)((p_hw_adc->calibr & 0x1) ? 0 : 1);
}

/**
 * \brief Whether the ADC Calibration request is running.
 * \param[in] p_hw_adc : Pointer to ADC register block.
 *
 * \retval  AM_TRUE  : The ADC Calibration request is running.
 * \retval  AM_FALSE : The ADC Calibration request isn't run.
 */
am_static_inline
am_bool_t amhw_lpc_adc_calib_isrun (amhw_lpc_adc_t *p_hw_adc)
{
    return (am_bool_t)((p_hw_adc->calibr >> 1) & 0x01);
}

/**
 * \brief Get the ADC Calibration value.
 *
 * \param[in] p_hw_adc : Pointer to ADC register block.
 *
 * \return The ADC Calibration value
 */
am_static_inline
uint32_t amhw_lpc_adc_calib_value_get (amhw_lpc_adc_t *p_hw_adc)
{
    return (p_hw_adc->calibr >> 2);
}

/**
 * \brief End of section using anonymous unions
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
 
  /* leave anonymous unions enabled          */
#elif defined(__GNUC__)

  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)

  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

/** @} */


/**
 *@} amhw_lpc_if_adc
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMHW_LPC_ADC_H */

/* end of file */
