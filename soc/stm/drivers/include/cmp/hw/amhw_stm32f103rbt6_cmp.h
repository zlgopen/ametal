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
 * \brief CMP Peripheral Access Layer
 *
 * 1.  CMP0 has its own independent 8-bit DAC.
 * 2.  CMP0 supports up to 8 analog inputs from external pins.
 * 3.  CMP0 is able to convert an internal reference from the bandgap (1 V
 *     reference voltage).
 * 4.  CMP0 supports the round-robin sampling scheme. In summary, this allow
 *     the CMP to operate independently in STOP1 and VLPS mode, whilst being
 *     triggered periodically to sample up to 8 inputs. Only if an input
 *     changes state is a full wakeup generated.
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-13  ari, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_STM32F103RBT6_CMP_H
#define __AMHW_STM32F103RBT6_CMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_stm32f103rbt6_if_cmp
 * \copydoc amhw_stm32f103rbt6_cmp.h
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
 * \brief CMP - Register Layout Typedef
 */
typedef struct amhw_stm32f103rbt6_cmp {
    __IO uint32_t csr[2];          /**< CMP Control and status Register, offset: 0x0 */
} amhw_stm32f103rbt6_cmp_t;

/**
 * \brief comparator channel
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_CHAN0    0  /**< \brief comparator 1 */
#define AMHW_STM32F103RBT6_CMP_CHAN2    1  /**< \brief comparator 2 */
/** @}*/

/**
 * \brief get  comparator lock
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : TRUE or FALSE
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_lock (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                        uint8_t            chan,
                        am_bool_t          flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(1U << 31))) | (flag << 31);
}

/**
 * \brief comparator output status
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_OUT_HIGH    (1U << 30)  /**< \brief high output */
#define AMHW_STM32F103RBT6_CMP_OUT_LOW     (0U << 30)  /**< \brief low output */
/** @}*/

/**
 * \brief get  comparator CSR value
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 *
 * \return none
 */
am_static_inline
uint32_t amhw_stm32f103rbt6_cmp_csr_stat_get (amhw_stm32f103rbt6_cmp_t *p_hw_cmp,
                                    uint8_t            chan)
{
    return p_hw_cmp->csr[chan];
}

/**
 * \brief comparator hysteresis
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_HYST_27MV     (3U << 16)  /**< \brief Comparator hysteresis 27mV */
#define AMHW_STM32F103RBT6_CMP_HYST_18MV     (2U << 16)  /**< \brief Comparator hysteresis 18mV */
#define AMHW_STM32F103RBT6_CMP_HYST_9MV      (1U << 16)  /**< \brief Comparator hysteresis 9mV */
#define AMHW_STM32F103RBT6_CMP_HYST_0MV      (0U << 16)  /**< \brief Comparator hysteresis 0mV */

/** @}*/

/**
 * \brief comparator hysteresis
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : see the field of comparator hysteresis
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_hyst_set (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                            uint8_t            chan,
                            uint32_t           flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(3U << 16))) | flag;
}

/**
 * \brief comparator output polarity
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_REVERSE_PHASE     (1U << 15)  /**< \brief Comparator reverse phase output */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_IN_PHASE          (0U << 15)  /**< \brief Comparator in-phase  output */

/** @}*/

/**
 * \brief comparator output polarity
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : see the field of comparator output polarity
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_output_polarity (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                                   uint8_t            chan,
                                   uint32_t           flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(1U << 15))) | flag;
}

/**
 * \brief comparator output selection
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_TIM1_BK         (2U << 10)  /**< \brief Timer 1 brake input */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_TIM1_OCREF_CLR  (6U << 10)  /**< \brief Timer 1 ocrefclear input */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_TIM1_IC1        (7U << 10)  /**< \brief Timer 1 input capture 1 */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_TIM2_IC4        (8U << 10)  /**< \brief Timer 2 input capture 4 */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_TIM2_OCREF_CLR  (9U << 10)  /**< \brief Timer 2 ocrefclear input */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_TIM3_IC1        (10U << 10) /**< \brief Timer 3 input capture 1t */
#define AMHW_STM32F103RBT6_CMP_OUTPUT_TIM3_OCREF_CLR  (11U << 10) /**< \brief Timer 3 ocrefclear input */

/** @}*/

/**
 * \brief comparator output selection
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : see the field of comparator output selection
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_output_sel (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                              uint8_t            chan,
                              uint32_t           flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(0xfU << 10))) | flag;
}

/**
 * \brief comparator normal phase input selection
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP0  (0U << 7)  /**< \brief INP0 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP1  (1U << 7)  /**< \brief INP1 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP2  (2U << 7)  /**< \brief INP2 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP3  (3U << 7)  /**< \brief INP3 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP4  (4U << 7)  /**< \brief INP4 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP5  (0U << 7)  /**< \brief INP5 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP6  (6U << 7)  /**< \brief INP6 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INP7  (7U << 7)  /**< \brief INP7 */

/** @}*/

/**
 * \brief comparator normal phase  input selection
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : see the field of comparator input selection
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_input_normal_sel (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                                    uint8_t            chan,
                                    uint32_t           flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(0x7U << 7))) | flag;
}

/**
 * \brief comparator inverting input selection
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_INPUT_VREFINT_1DIV4  (0U << 4)  /**< \brief Vrefint 1/4 */
#define AMHW_STM32F103RBT6_CMP_INPUT_VREFINT_1DIV2  (1U << 4)  /**< \brief Vrefint 1/2 */
#define AMHW_STM32F103RBT6_CMP_INPUT_VREFINT_3DIV4  (2U << 4)  /**< \brief Vrefint 3/4 */
#define AMHW_STM32F103RBT6_CMP_INPUT_VREFINT        (3U << 4)  /**< \brief Vrefint */
#define AMHW_STM32F103RBT6_CMP_INPUT_INM4           (4U << 4)  /**< \brief INM4 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INM5           (0U << 4)  /**< \brief INM5 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INM6           (6U << 4)  /**< \brief INM6 */
#define AMHW_STM32F103RBT6_CMP_INPUT_INM7           (7U << 4)  /**< \brief INM7 */

/** @}*/

/**
 * \brief comparator inverting input selection
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : see the field of comparator input selection
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_input_invert_sel (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                                    uint8_t            chan,
                                    uint32_t           flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(0x7U << 4))) | flag;
}

/**
 * \brief comparator mode select
 * \@{
 */
#define AMHW_STM32F103RBT6_CMP_MODE_SPEED_HIGH     (3U << 2)  /**< \brief High speed */
#define AMHW_STM32F103RBT6_CMP_MODE_SPEED_MID      (2U << 2)  /**< \brief middle speed */
#define AMHW_STM32F103RBT6_CMP_MODE_SPEED_LOW      (1U << 2)  /**< \brief low speed */
#define AMHW_STM32F103RBT6_CMP_MODE_SPEED_VERY_LOW (0U << 2)  /**< \brief very low speed */

/** @}*/

/**
 * \brief comparator mode set
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : see the field of comparator mode select
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_mode_sel (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                            uint8_t            chan,
                            uint32_t           flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(0x3U << 2))) | flag;
}

/**
 * \brief comparator enable
 *
 * \param[in] p_hw_adc : The pointer to the structure CMP register
 * \param[in] chan     : channel  see the field of comparator channel
 * \param[in] flag     : TRUE or FALSE
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_cmp_module_enable (amhw_stm32f103rbt6_cmp_t    *p_hw_cmp,
                                 uint8_t            chan,
                                 am_bool_t          flag)
{
    p_hw_cmp->csr[chan] = (p_hw_cmp->csr[chan] & (~(0x1U << 0))) | flag;
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
#endif /* __AMHW_STM32F103RBT6_CMP_H */

/* end of file */
