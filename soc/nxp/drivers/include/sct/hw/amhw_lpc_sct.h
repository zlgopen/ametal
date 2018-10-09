/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief SCTimer/PWM (SCT) interfaces for operating hardware.
 *
 * - Multi-function timer with many capabilities, including:
 *       - PWM
 *       - capture
 *       - Match output
 * - Two 16-bit counters or one 32-bit counter.
 * - Counters clocked by bus clock or selected input.
 * - Up counters or up-down counters.
 * - State variable allows sequencing across multiplecounter cycles.
 * - The following conditions define an event: a counter match condition, 
 *   an input (or output) condition, a combination of a match and/or and 
 *   input/output condition in a specified state.
 * - Events control state changes, outputs, interrupts, and DMA requests.
 *       - Match register 0 can be used as an automatic limit.
 *       - In bi-directional mode, events can be enabled based on the count direction.
 *       - Match events can be held until another qualifying event occurs.
 * - Selected events can limit, halt, start, or stop a counter.
 * - Supports:
 *       - 8 inputs
 *       - 8 outputs
 *       - 13 match/capture registers
 *       - 13 events
 *       - 13 states
 * - Inputs and outputs on the SCT are configured as follows:
 * - 8 inputs are connected to predefined sources.
 * - 8 outputs
 *       - Outputs 0 through 7 can be connected to external pins.
 *       - Output 7 can be connected to the ADC trigger input mux only for lpc5410x
 *       - Output 3 can be connected to the ADC trigger input mux only for lpc82x
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_SCT_H
#define __AMHW_LPC_SCT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/** 
 * \addtogroup amhw_lpc_if_sct  
 * \copydoc amhw_lpc_sct.h
 * @{
 */    

/**                        
 * \name SCT Module configuration
 * @{
 */
 
#define AMHW_LPC_SCT_EVENT_NUM         (13)  /**< \brief Number of events                  */
#define AMHW_LPC_SCT_MATCAP_REG_NUM    (13)  /**< \brief Number of match/compare registers */
#define AMHW_LPC_SCT_STATE_NUM         (13)  /**< \brief Number of states                  */  
#define AMHW_LPC_SCT_OUTPUT_NUM        (8)   /**< \brief Number of outputs                 */
#define AMHW_LPC_SCT_INPUT_NUM         (8)   /**< \brief Number of inputs                  */
#define AMHW_LPC_SCT_DMA_REQUEST_NUM   (2)   /**< \brief Number of DMA request             */

/** @} */

/* Start of section using anonymous unions  */
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

/**
 * \brief State Configurable Timer register block structure
 */
typedef struct amhw_lpc_sct {
    __IO uint32_t config;              /**< \brief configuration register         */
    
    union {
        __IO uint32_t ctrl_u;          /**< \brief control register               */
        struct {
            __IO uint16_t ctrl_l;      /**< \brief low control register           */
            __IO uint16_t ctrl_h;      /**< \brief high control register          */
        };
    };

    __IO uint16_t limit_l;             /**< \brief limit register for counter l   */
    __IO uint16_t limit_h;             /**< \brief limit register for counter h   */
    __IO uint16_t halt_l;              /**< \brief halt register for counter l    */
    __IO uint16_t halt_h;              /**< \brief halt register for counter h    */
    __IO uint16_t stop_l;              /**< \brief stop register for counter l    */
    __IO uint16_t stop_h;              /**< \brief stop register for counter h    */
    __IO uint16_t start_l;             /**< \brief start register for counter l   */
    __IO uint16_t start_h;             /**< \brief start register for counter h   */
    
    uint32_t reserved1[10];            /**< \brief 0x18 ~ 0x03c reserved          */

    union {
        __IO uint32_t count_u;         /**< \brief counter register               */
        struct {
            __IO uint16_t count_l;     /**< \brief counter register for counter l */
            __IO uint16_t count_h;     /**< \brief counter register for counter h */
        };
    };

    __IO uint16_t state_l;             /**< \brief state register for counter l   */
    __IO uint16_t state_h;             /**< \brief state register for counter h   */
    __I  uint32_t input;               /**< \brief input register,read only       */
    
    __IO uint16_t regmode_l;           /**< \brief match/capture mode register l  */
    __IO uint16_t regmode_h;           /**< \brief match/capture mode register h  */
    __IO uint32_t output;              /**< \brief output register                */
    __IO uint32_t outputdirctrl;       /**< \brief output direction control       */
    
    __IO uint32_t res;                 /**< \brief conflict resolution register   */
    
    union {
        
        __IO uint32_t dmarequest[2];
        struct {
            __IO uint32_t dma0request;      /**< \brief dma0 request register     */
            __IO uint32_t dma1request;      /**< \brief dma1 request register     */
        };
    };
    
         uint32_t reserved2[35];    /**< \brief reserved,do not use      */
    
    __IO uint32_t even;             /**< \brief event enable register    */
    __IO uint32_t evflag;           /**< \brief event flag register      */
    __IO uint32_t conen;            /**< \brief conflict enable register */
    __IO uint32_t conflag;          /**< \brief conflict flag register   */

    union {

        __IO union {               /**< \brief match / capture value              */
            __IO uint32_t u;       /**< \brief match[i].u unified 32-bit register */
            struct {
                __IO uint16_t l;   /**< \brief match[i].l  access to l value      */
                __IO uint16_t h;   /**< \brief match[i].h  access to h value      */
            };

        } match[AMHW_LPC_SCT_MATCAP_REG_NUM];

        __I union {
            __IO uint32_t u;       /**< \brief cap[i].u  unified 32-bit register  */
            struct {
                __IO uint16_t l;   /**< \brief cap[i].l  access to l value        */
                __IO uint16_t h;   /**< \brief cap[i].h  access to h value        */
            };
        } cap[AMHW_LPC_SCT_MATCAP_REG_NUM];
    };

    uint32_t reserved3[48 + (16 - AMHW_LPC_SCT_MATCAP_REG_NUM)]; /**< \brief reserved,do not use */

    union {

        __IO union {              /**< \brief  match reload / capture control value   */
            __IO uint32_t u;      /**< \brief  matchrel[i].u  unified 32-bit register */

            struct {
                __IO uint16_t l;  /**< \brief  matchrel[i].l  access to l value     */
                __IO uint16_t h;  /**< \brief  matchrel[i].h  access to h value     */
            };

        } matchrel[AMHW_LPC_SCT_MATCAP_REG_NUM];

        __IO union {
            __IO uint32_t u;      /**< \brief capctrl[i].u  unified 32-bit register */

            struct {
                __IO uint16_t l;  /**< \brief capctrl[i].l  access to l value       */
                __IO uint16_t h;  /**< \brief capctrl[i].h  access to h value       */
            };
        } capctrl[AMHW_LPC_SCT_MATCAP_REG_NUM];

    };

    uint32_t reserved6[48 + (16 - AMHW_LPC_SCT_MATCAP_REG_NUM)]; /**< \brief reserved,do not use */

    __IO struct {                 
        __IO uint32_t state;      /**< \brief event state register                  */
        __IO uint32_t ctrl;       /**< \brief event control register                */
    } event[AMHW_LPC_SCT_EVENT_NUM];  /**< \brief event array for state and control     */

    uint32_t reserved9[128 - 2 * AMHW_LPC_SCT_EVENT_NUM]; /**< \brief reserved,do not use */

    __IO struct {                
        __IO uint32_t set;        /**< \brief output n set   register            */
        __IO uint32_t clr;        /**< \brief output n clear register            */
    } out[AMHW_LPC_SCT_OUTPUT_NUM];   /**< \brief out array for output set and clear */
    
} amhw_lpc_sct_t;


/** \brief define the event num,the n value can use 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1 */
#define AMHW_LPC_SCT_EVT(n)       (n)

/** \brief define the state num,the n value can use 0 ~ AMHW_LPC_SCT_STATE_NUM - 1 */
#define AMHW_LPC_SCT_STATE(n)     (n)

/** \brief define the capture num,the n value can use 0 ~ AMHW_LPC_SCT_MATCAP_REG_NUM - 1 */
#define AMHW_LPC_SCT_CAP(n)       (n)

/** \brief define the match num,the n value can use 0 ~ AMHW_LPC_SCT_MATCAP_REG_NUM - 1 */
#define AMHW_LPC_SCT_MAT(n)       (n)

/** \brief define the output num,the n value can use 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1 */
#define AMHW_LPC_SCT_OUT(n)       (n)

/** \brief define the input num,the n value can use 0 ~ AMHW_LPC_SCT_INPUT_NUM - 1   */
#define AMHW_LPC_SCT_INPUT(n)     (n)

/**
 * \name Macro defines for SCT configuration
 * @{
 */

/** \brief Operate as 2 16-bit counters */
#define AMHW_LPC_SCT_CONFIG_16BIT_COUNTER                   0x00000000   

/** \brief Operate as 1 32-bit counter  */
#define AMHW_LPC_SCT_CONFIG_32BIT_COUNTER                   0x00000001   

/** 
  * \brief Prevents the both the higher and lower registers from being 
  *        reloaded from their respective reload registers.
  *        only used when operate as 1-32bit counter mode.
  */
#define AMHW_LPC_SCT_CONFIG_NORELOAD_U                      (0x1 << 7)

/** 
  * \brief Prevents the lower match registers from being reloaded from their
  *        respective reload registers
  */
#define AMHW_LPC_SCT_CONFIG_NORELOAD_L                      (0x1 << 7)

/** 
  * \brief Prevents the higher match registers from being reloaded from their
  *        respective reload registers
  */
#define AMHW_LPC_SCT_CONFIG_NORELOAD_H                      (0x1 << 8)    

/** \brief CLK mode is System clock(mode 0)            */
#define AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK                  (0x0 << 1)    

/** \brief CLK mode is Prescaled system clock (mode 1) */
#define AMHW_LPC_SCT_CONFIG_CLKMODE_PRESCALED_SYSCLK        (0x1 << 1)    

/** \brief Input clock/edge selected in CKSEL (mode 2) */
#define AMHW_LPC_SCT_CONFIG_CLKMODE_SCT_INPUT               (0x2 << 1)

/** \brief Prescaled input clock/edge selected in CKSEL (mode 3) */
#define AMHW_LPC_SCT_CONFIG_CLKMODE_PRESCALED_SCT_INPUT     (0x3 << 1)    

/**
 * @}
 */

/**
 * \name SCT select for clock mode 2 and mode 3
 * @{
 */
 
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_0    (0x0UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_0   (0x1UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_1    (0x2UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_1   (0x3UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_2    (0x4UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_2   (0x5UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_3    (0x6UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_3   (0x7UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_4    (0x8UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_4   (0x9UL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_5    (0xAUL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_5   (0xBUL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_6    (0xCUL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_6   (0xDUL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_RISING_IN_7    (0xEUL << 3)
#define AMHW_LPC_SCT_CONFIG_CKSEL_FALLING_IN_7   (0xFUL << 3)

/** @} */


/**
 * \name input to synchronization to the SCT clock before it is used to create an event
 *
 * when clkmode is AMHW_LPC_SCT_CONFIG_CLKMODE_SCT_INPUT or AMHW_LPC_SCT_CONFIG_CLKMODE_PRESCALED_SCT_INPUT
 * this config is not used.
 * @{
 */

#define AMHW_LPC_SCT_CONFIG_INSYNC_0     (0x1 <<  9) /**< \brief Synchronization for input 0   */
#define AMHW_LPC_SCT_CONFIG_INSYNC_1     (0x1 << 10) /**< \brief Synchronization for input 1   */
#define AMHW_LPC_SCT_CONFIG_INSYNC_2     (0x1 << 11) /**< \brief Synchronization for input 2   */
#define AMHW_LPC_SCT_CONFIG_INSYNC_3     (0x1 << 12) /**< \brief Synchronization for input 3   */
#define AMHW_LPC_SCT_CONFIG_INSYNC_4     (0x1 << 12) /**< \brief Synchronization for input 4   */
#define AMHW_LPC_SCT_CONFIG_INSYNC_5     (0x1 << 14) /**< \brief Synchronization for input 5   */
                                     
#define AMHW_LPC_SCT_CONFIG_INSYNC_ALL   (0x3F << 9) /**< \brief Synchronization for all input */

/** @} */

/**
 * \name a match on match register 0 to be treated as a de-facto LIMIT condition
 * @{
 */
#define AMHW_LPC_SCT_CONFIG_AUTOLIMIT_U  (0x1UL << 17)   
#define AMHW_LPC_SCT_CONFIG_AUTOLIMIT_L  (0x1UL << 17)
#define AMHW_LPC_SCT_CONFIG_AUTOLIMIT_H  (0x1UL << 18)

/** @} */

/**
 * \name Macro defines for SCT control register
 * @{
 */
 
#define AMHW_LPC_SCT_CTRL_STOP_L         (1 << 1)   /**< \brief Stop low counter             */
#define AMHW_LPC_SCT_CTRL_HALT_L         (1 << 2)   /**< \brief halt low counter             */
#define AMHW_LPC_SCT_CTRL_CLRCTR_L       (1 << 3)   /**< \brief Clear low or unified counter */
 
/** 
 * \brief The counter L or U counts up to its limit condition, 
 *        then is cleared to zero. 
 */
#define AMHW_LPC_SCT_CTRL_BIDIR_L_COUNT_UP       (0 << 4) 

/**
 * \brief The counter L or U counts up to its limit, then  
 *        counts down to a limit condition or  to zero.
 */
#define AMHW_LPC_SCT_CTRL_BIDIR_L_COUNT_UP_DOWN  (1 << 4)

/** \brief Prescale clock for low or unified counter */
#define AMHW_LPC_SCT_CTRL_PRE_L(x)               (((x) & 0xFF) << 5) 
 
#define AMHW_LPC_SCT_CTRL_STOP_H                 (1 << 17)    /**< \brief Stop high counter  */
#define AMHW_LPC_SCT_CTRL_HALT_H                 (1 << 18)    /**< \brief halt high counter  */
#define AMHW_LPC_SCT_CTRL_CLRCTR_H               (1 << 19)    /**< \brief Clear high counter */

/**
 * \brief The counter H counts up to its limit condition, 
 *         then is cleared to zero 
 */
#define AMHW_LPC_SCT_CTRL_BIDIR_H_COUNT_UP       (0 << 20)    

/**
 * \brief The counter H counts up to its limit, then counts down to a limit 
 *        condition or to 0.
 */
#define AMHW_LPC_SCT_CTRL_BIDIR_H_COUNT_UP_DOWN  (1 << 20)

/** \brief Prescale clock for high counter  */
#define AMHW_LPC_SCT_CTRL_PRE_H(x)               (((x) & 0xFF) << 21)

/**
 * @}
 */

/**
 * \name Macro defines for SCT event control register
 * @{
 */
 
/** \brief Match register associated with this event*/
#define AMHW_LPC_SCT_EV_CTRL_MATCHSEL(reg)   (reg << 0)      

/** \brief Selects the L state and the L match register,only used when UNIFY = 0 */
#define AMHW_LPC_SCT_EV_CTRL_HEVENT_L        (0UL << 4)   

/** \brief Selects the H state and the H match register,only used when UNIFY  = 0 */
#define AMHW_LPC_SCT_EV_CTRL_HEVENT_H        (1UL << 4)

/** \brief Input select by IOSEL    */
#define AMHW_LPC_SCT_EV_CTRL_OUTSEL_INPUT    (0UL << 5)

/** \brief Output select by IOSEL  */
#define AMHW_LPC_SCT_EV_CTRL_OUTSEL_OUTPUT   (1UL << 5)

/** \brief Select io signal associated with this event */
#define AMHW_LPC_SCT_EV_CTRL_IOSEL(signal)   (signal << 6)  

/** \brief STATEV value is added into STATE  */
#define AMHW_LPC_SCT_EV_CTRL_STATELD_ADD      (0x0UL << 14)

/** \brief STATEV value is loaded into STATE */
#define AMHW_LPC_SCT_EV_CTRL_STATELD_LOAD     (0x1UL << 14) 

/**
 * \brief define the STATEV value.
 *
 *  only when this event is the highest-numbered event occurring for that state.
 *  Can change the now state value. 
 *  If STATELD and STATEV are both zero, there is no change to the STATE value
 */
#define AMHW_LPC_SCT_EV_CTRL_STATEV(x)        (((x) &(0x1F)) << 15)

/**
 * \name control the match event active state
 * @{
 * If this flag is one and the COMBMODE field specifies a match component to the 
 * triggering of this event, then a match is considered to be active whenever the 
 * counter value is GREATER THAN OR EQUAL TO the value specified in the match 
 * register when counting up, LESS THEN OR EQUAL TO the match value when counting down.
 * 
 * If this bit is zero, a match is only be active during the cycle when the counter 
 * is equal to the match value
 */
#define AMHW_LPC_SCT_EV_CTRL_MATCHMEM_1                (0x1UL << 20)
#define AMHW_LPC_SCT_EV_CTRL_MATCHMEM_0                (0x0UL << 20)

/** @} */

/** \brief This event is triggered regardless of the count direction */
#define AMHW_LPC_SCT_EV_CTRL_DIRECTION_INDEPENDENT     (0x0UL << 21) 

/** \brief This event is triggered only during up-counting when BIDIR = 1  */
#define AMHW_LPC_SCT_EV_CTRL_DIRECTION_UP              (0x1UL << 21)

/** \brief This event is triggeredonly during down-counting when BIDIR = 1 */
#define AMHW_LPC_SCT_EV_CTRL_DIRECTION_DOWN            (0x2UL << 21)

/**
 * @}
 */

/**
 * \name Selects the I/O condition for event
 * @{
 */
 
#define AMHW_LPC_SCT_EV_CTRL_IOCOND_LOW       (0x0UL << 10)
#define AMHW_LPC_SCT_EV_CTRL_IOCOND_RISE      (0x1UL << 10)
#define AMHW_LPC_SCT_EV_CTRL_IOCOND_FALL      (0x2UL << 10)
#define AMHW_LPC_SCT_EV_CTRL_IOCOND_HIGH      (0x3UL << 10)

/** @} */

/**
 * \name specified match and I/O condition are used and combined
 * @{
 */
 
#define AMHW_LPC_SCT_EV_CTRL_COMBMODE_OR      (0x0UL << 12)  
#define AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH   (0x1UL << 12)
#define AMHW_LPC_SCT_EV_CTRL_COMBMODE_IO      (0x2UL << 12)
#define AMHW_LPC_SCT_EV_CTRL_COMBMODE_AND     (0x3UL << 12)

/** @} */

/**
 * \name Macro defines for SCT Conflict resolution register
 * @{
 */
#define AMHW_LPC_SCT_RES_NOCHANGE                (0)  
#define AMHW_LPC_SCT_RES_SET_OUTPUT              (1)
#define AMHW_LPC_SCT_RES_CLEAR_OUTPUT            (2)
#define AMHW_LPC_SCT_RES_TOGGLE_OUTPUT           (3)

/**
 * @}
 */
 
/**
 * \name Macro defines for SCT bidirectional output control register
 * @{
 */
 
/** \brief Set and clear do not depend on any counter */
#define AMHW_LPC_SCT_OUTPUTDIRCTRL_INDEPENDENT   (0)  

/** \brief Set and clear are reversed when counter L or the unified counter is counting down. */
#define AMHW_LPC_SCT_OUTPUTDIRCTRL_L_REVERSED    (1)

/** \brief Set and clear are reversed when counter H is counting down */
#define AMHW_LPC_SCT_OUTPUTDIRCTRL_H_REVERSED    (2)

/** @} */

/**
 * \brief SCT mode flags 
 *
 * These flags are used for configure some registers that could operate as UNIFY ,
 * L and H mode. and when operate as UNIFY , also only the _L bits are used.
 * such as LIMIT,STOP,HALT,START ...
 * 
 */
typedef enum amhw_lpc_sct_mode {
    AMHW_LPC_SCT_MODE_UNIFY      = 0,    /**< \brief UNIFY    mode          */
    AMHW_LPC_SCT_MODE_SEPARATE_L,        /**< \brief SEPARATE mode _L bits  */
    AMHW_LPC_SCT_MODE_SEPARATE_H         /**< \brief SEPARATE mode _H bits  */
} amhw_lpc_sct_mode_t;

/**
 * \brief Match/Capture register mode 
 *
 *  The Match and Capture register have the same address.should select one 
 *  Operate Mode(Match or Capture).
 * 
 */
typedef enum amhw_lpc_sct_matcap_mode {
    AMHW_LPC_SCT_MATCAP_MATCH  = 0,     /**< \brief the reg Operate as match   registers */
    AMHW_LPC_SCT_MATCAP_CAPTURE,        /**< \brief the reg Operate as capture registers */
} amhw_lpc_sct_matcap_mode_t;        


/**
 * \brief Configure the SCT
 *
 * \param[in] p_hw_sct  : Pointer to SCT register block
 * \param[in] cfg_flags : OR'ed AMHW_LPC_SCT_CONFIG_* values
 *                        (example AMHW_LPC_SCT_CONFIG_32BIT_COUNTER)
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_config (amhw_lpc_sct_t *p_hw_sct, uint32_t cfg_flags) 
{
    p_hw_sct->config = cfg_flags;
}

/**
 * \brief Control the SCT according to the flags
 *
 * \param[in] p_hw_sct   :   Pointer to SCT register block
 * \param[in] ctrl_flags :   OR'ed AMHW_LPC_SCT_CTRL_XXX values
 *                             (example AMHW_LPC_SCT_CTRL_HALT_L) 
 * \return None
 * 
 * \note All bits in this register can be written to when the counter is 
 *       stopped or halted. When the counter is running, the only bits
 *       that can be written are STOP or HALT.
 */
am_static_inline 
void amhw_lpc_sct_ctrl (amhw_lpc_sct_t *p_hw_sct, uint32_t ctrl_flags)
{
    p_hw_sct->ctrl_u = ctrl_flags;
}

/**
 * \brief Check the SCT whether HALT
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 *
 * \retval  AM_TRUE  : halt
 * \retval  AM_FALSE : non halt
 */
am_static_inline 
am_bool_t amhw_lpc_sct_halt_check (amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode)
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        return (am_bool_t)((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_H) != 0);
    } else {
        return (am_bool_t)((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_L) != 0);
    }   
}

/**
 * \brief Check the SCT whether STOP
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 *
 * \retval  AM_TRUE  : stop
 * \retval  AM_FALSE : non stop
 */
am_static_inline 
am_bool_t amhw_lpc_sct_stop_check (amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode)
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        return (am_bool_t)((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_STOP_H) != 0);
    } else {
        return (am_bool_t)((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_STOP_L) != 0);
    }   
}

/**
 * \brief Set the Control register bits to 1
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] value    :  - If the bit n is one, the ctrl reg bit n will be set to 1.
 *                        - If the bit n is zero,the ctrl reg bit n not any affect.
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_ctrl_set (amhw_lpc_sct_t *p_hw_sct, uint32_t value)
{
    p_hw_sct->ctrl_u |= value;
}

/**
 * \brief Clear the Control register bits to zero
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] value    :  - If the bit n is one, the ctrl reg bit n will be clr to 0. 
 *                        - If the bit n is zero,the ctrl reg bit n not any affect.   
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_ctrl_clr (amhw_lpc_sct_t *p_hw_sct, uint32_t value)
{
    p_hw_sct->ctrl_u &= (~value);
}

/**
 * \brief Bring the SCT from HALT to STOP(not running)
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * 
 * \return None
 */
void amhw_lpc_sct_ctrl_halt_to_stop(amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode);

/**
 * \brief Set the SCT prescale value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] prescale : The counter clock is clocked at the rate of the 
 *                       SCT clock divided by prescale + 1   
 * \return None
 */
void amhw_lpc_sct_prescale_set(amhw_lpc_sct_t     *p_hw_sct, 
                           amhw_lpc_sct_mode_t mode, 
                           uint8_t         prescale);

/**
 * \brief Get the SCT prescale value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 *
 * \return The SCT prescale value
 */
uint8_t amhw_lpc_sct_prescale_get(amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode);

/**
 * \brief Control the event according to the flags
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] flags    : OR'ed AMHW_LPC_SCT_EV_CTRL_XXXX values
 *                       (example AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH) 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_ctrl (amhw_lpc_sct_t *p_hw_sct, uint32_t evt_num, uint32_t flags)
{
    p_hw_sct->event[evt_num].ctrl = flags;
}

/**
 * \brief Enable The event happens in state.
 *
 * \param[in] p_hw_sct  : Pointer to SCT register block
 * \param[in] evt_num   : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] state_num : 0 ~ AMHW_LPC_SCT_STATE_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_state_enable (amhw_lpc_sct_t *p_hw_sct, 
                                  uint32_t    evt_num,
                                  uint32_t    state_num)
{
    p_hw_sct->event[evt_num].state |= (1 << state_num);
}

/**
 * \brief Disable The event happens in state.
 *
 * \param[in] p_hw_sct  : Pointer to SCT register block
 * \param[in] evt_num   : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] state_num : 0 ~ AMHW_LPC_SCT_STATE_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_state_disable (amhw_lpc_sct_t *p_hw_sct,
                                   uint32_t    evt_num,
                                   uint32_t    state_num)
{
    p_hw_sct->event[evt_num].state &= ~(1 << state_num);
}

/**
 * \brief 配置使能一个事件在某些状态中发生
 *
 * \param[in] p_hw_sct  : 指向SCT寄存器块的指针
 * \param[in] evt_num   : 事件号，0 ~ AMHW_LPC82X_SCT_EVT_NUM - 1
 * \param[in] states    : 状态参数，states的bit0代表状态0，bit0=1时，事件在状态0使能，
 *                        bit1=1时，事件在状态1使能，其他位可类推
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_sct_event_state_cfg (amhw_lpc_sct_t *p_hw_sct,
                                 uint32_t        evt_num,
                                 uint32_t        states)
{
    p_hw_sct->event[evt_num].state = states;
}

/**
 * \brief Enable the event used as a counter limit
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_limit_enable (amhw_lpc_sct_t     *p_hw_sct,
                                  amhw_lpc_sct_mode_t mode, 
                                  uint32_t        evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->limit_h |= 1 << evt_num;
    } else {
        p_hw_sct->limit_l |= 1 << evt_num;
    }   
}

/**
 * \brief Disable the event used as a counter limit
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_limit_disable (amhw_lpc_sct_t     *p_hw_sct,
                                   amhw_lpc_sct_mode_t mode, 
                                   uint32_t        evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->limit_h &= ~(1 << evt_num);
    } else {
        p_hw_sct->limit_l &= ~(1 << evt_num);
    }   
}

/**
 * \brief Enable the event sets the HALT bit in the CTRL register
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_halt_enable (amhw_lpc_sct_t      *p_hw_sct,
                                 amhw_lpc_sct_mode_t  mode, 
                                 uint32_t         evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->halt_h |= 1 << evt_num;
    } else {
        p_hw_sct->halt_l |= 1 << evt_num;
    }   
}

/**
 * \brief Disable the event sets the HALT bit in the CTRL register
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_halt_disable (amhw_lpc_sct_t      *p_hw_sct,
                                  amhw_lpc_sct_mode_t  mode, 
                                  uint32_t         evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->halt_h &= ~(1 << evt_num);
    } else {
        p_hw_sct->halt_l &= ~(1 << evt_num);
    }   
}

/**
 * \brief Enable the event sets the STOP bit in the CTRL register
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_stop_enable (amhw_lpc_sct_t      *p_hw_sct,
                                 amhw_lpc_sct_mode_t  mode, 
                                 uint32_t         evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->stop_h |= 1 << evt_num;
    } else {
        p_hw_sct->stop_l |= 1 << evt_num;
    }   
}

/**
 * \brief Disable the event sets the STOP bit in the CTRL register
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_stop_disable (amhw_lpc_sct_t      *p_hw_sct,
                                  amhw_lpc_sct_mode_t  mode, 
                                  uint32_t         evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->stop_h &= ~(1 << evt_num);
    } else {
        p_hw_sct->stop_l &= ~(1 << evt_num);
    }   
}

/**
 * \brief Enable the event clears the STOP bit in the CTRL register
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 *            
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_start_enable (amhw_lpc_sct_t      *p_hw_sct,
                                  amhw_lpc_sct_mode_t  mode, 
                                  uint32_t         evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->start_h |= 1 << evt_num;
    } else {
        p_hw_sct->start_l |= 1 << evt_num;
    }   
}

/**
 * \brief Disable the event clears the STOP bit in the CTRL register
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 *  
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_event_start_disable (amhw_lpc_sct_t      *p_hw_sct,
                                   amhw_lpc_sct_mode_t  mode, 
                                   uint32_t         evt_num) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->start_h &= ~(1 << evt_num);
    } else {
        p_hw_sct->start_l &= ~(1 << evt_num);
    }   
}

/**
 * \brief Set SCT count value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode.
 * \param[in] value    : The count value to set
 *  
 * \note    Writing to the COUNT_L, COUNT_H, or unified register is only 
 *          allowed when the  corresponding counter is halted  
 *          if the mode is AMHW_LPC_SCT_MODE_SEPARATE_L or
 *          AMHW_LPC_SCT_MODE_SEPARATE_H, the value is only low 16bits valid
 */
void amhw_lpc_sct_count_set(amhw_lpc_sct_t     *p_hw_sct, 
                        amhw_lpc_sct_mode_t mode, 
                        uint32_t        value);

/**
 * \brief Get SCT count value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 *
 * \return The value of Now count,if The SCT mode is not UNIFY mode, the return value
 *         Only _L 16bits valid.
 */
uint32_t amhw_lpc_sct_count_get(amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode);

/**
 * \brief Get now state value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 *
 * \return  the state value of now.
 */
am_static_inline 
uint16_t amhw_lpc_sct_state_get (amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        return p_hw_sct->state_h;
    } else {
        return p_hw_sct->state_l;
    }  
}

/**
 * \brief Set the state value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] value    : The state value to set
 *
 * \note  Writing to the STATE_L, STATE_H, or unified register is only allowed
 *        when the corresponding counter is halted 
 *        (HALT bits are set to 1 in the CTRL register).
 */
am_static_inline 
void amhw_lpc_sct_state_set (amhw_lpc_sct_t      *p_hw_sct,
                         amhw_lpc_sct_mode_t  mode, 
                         uint16_t         value) 
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->state_h = value;
    } else {
        p_hw_sct->state_l = value;
    }
}

/**
 * \brief Get the SCT inputs state
 *
 * \param[in] p_hw_sct  : Pointer to SCT register block
 * 
 * \return The intput state. bit 0 ~ bit7:  AIN0 ~ AIN7(Input state,Direct read)
 *                           bit16 ~ bit23: SIN0 ~ SIN7(Input state)
 *
 * \note SIN0 ~ SIN7:
 *        - If the INSYNC bit in CONFIG is set for the input,this input state       
 *       is synchronized to the SCT clock,using three SCT clock cycles resulting 
 *       in a stablesignal that is delayed by three SCT clock cycles.
 *        - If the INSYNC bit is not set, the SIN bit value is the same as the 
 *       AIN bit value.
 */
am_static_inline 
uint32_t amhw_lpc_sct_input_state_get (amhw_lpc_sct_t *p_hw_sct) 
{
     return p_hw_sct->input;
}

/**
 * \brief Get the SCT outputs state 
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 *   
 * \return the output state. bit 0 ~ bit7: output 0 ~ output7 
 *         the bit value is 1 when output is HIGH              
 *         the bit value is 0 when output is LOW 
 */
am_static_inline 
uint32_t amhw_lpc_sct_output_state_get (amhw_lpc_sct_t *p_hw_sct) 
{
     return p_hw_sct->output;
}


/**
 * \brief Configure the SCT outputs state 
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] state    : 1 to bit n makes the corresponding output n HIGH. 
 *                       0 to bit n makes the corresponding output n LOW.
 *
 * \return None
 *
 * \note Writing to the OUTPUT register is only allowed when all counters 
 *       (L-counter, H-counter, and unified counter) are halted
 *
 */
am_static_inline 
void amhw_lpc_sct_output_state_config (amhw_lpc_sct_t *p_hw_sct, uint8_t state) 
{
     p_hw_sct->output = state;
}

/**
 * \brief Set the SCT outputs state to 1
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *  
 * \return  None
 *
 * \note Writing to the OUT register is only allowed when all counters 
 *       (L-counter, H-counter, and unified counter) are halted
 */
am_static_inline 
void amhw_lpc_sct_output_state_set (amhw_lpc_sct_t *p_hw_sct, uint8_t output_num) 
{
     p_hw_sct->output |= (1ul << output_num);
}

/**
 * \brief Clear the SCT outputs state to 0
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *  
 * \return  None
 *
 * \note Writing to the OUT register is only allowed when all counters 
 *       (L-counter, H-counter, and unified counter) are halted
 *
 */
am_static_inline 
void amhw_lpc_sct_output_state_clr (amhw_lpc_sct_t *p_hw_sct, uint8_t output_num) 
{
     p_hw_sct->output &= ~(1ul << output_num);
}

/**
 * \brief Control the output dir
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 * \param[in] value      : ctrl value
 *                         - AMHW_LPC_SCT_OUTPUTDIRCTRL_INDEPENDENT   
 *                         - AMHW_LPC_SCT_OUTPUTDIRCTRL_L_REVERSED    
 *                         - AMHW_LPC_SCT_OUTPUTDIRCTRL_R_REVERSED    
 *
 * \return  None
 *
 */
void amhw_lpc_sct_output_dir_ctrl(amhw_lpc_sct_t *p_hw_sct, 
                              uint8_t     output_num, 
                              uint8_t     value);

/**
 * \brief Configure the output conflict resolution 
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 * \param[in] value      : Output value
 *                          - AMHW_LPC_SCT_RES_NOCHANGE       :No change
 *                          - AMHW_LPC_SCT_RES_SET_OUTPUT     :Set output
 *                          - AMHW_LPC_SCT_RES_CLEAR_OUTPUT   :Clear output
 *                          - AMHW_LPC_SCT_RES_TOGGLE_OUTPUT  :Toggle output
 * \return None
 */
void amhw_lpc_sct_conflict_res_config(amhw_lpc_sct_t *p_hw_sct, 
                                  uint8_t     output_num, 
                                  uint8_t     value);
    

/**
 * \brief Enable the event to sets DMA request.
 *
 * \param[in] p_hw_sct    : Pointer to SCT register block
 * \param[in] request_num : DMA request num,0 ~ AMHW_LPC_SCT_DMA_REQUEST_NUM - 1
 * \param[in] evt_num     : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_sct_dma_request_event_enable (amhw_lpc_sct_t *p_hw_sct,
                                        uint8_t     request_num,
                                        uint32_t     evt_num) 
{
    p_hw_sct->dmarequest[request_num] |= (1 << evt_num);
}

/**
 * \brief Disable the event to sets DMA request.
 *
 * \param[in] p_hw_sct    : Pointer to SCT register block
 * \param[in] request_num : DMA request num,  0 ~ AMHW_LPC_SCT_DMA_REQUEST_NUM - 1
 * \param[in] evt         : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_sct_dma_request_event_disable (amhw_lpc_sct_t *p_hw_sct,
                                         uint8_t     request_num,
                                         uint32_t     evt) 
{
    p_hw_sct->dmarequest[request_num] &= ~(1ul << evt);
}

/**
 * \brief Enable sets DMA request when counter Match registers are loaded 
 *        from its Reload registers
 *
 * \param[in] p_hw_sct    : Pointer to SCT register block
 * \param[in] request_num : DMA request num,  0 ~ AMHW_LPC_SCT_DMA_REQUEST_NUM - 1
 *  
 * \return  None
 */
am_static_inline 
void amhw_lpc_sct_dma_request_reload_enable (amhw_lpc_sct_t *p_hw_sct, 
                                         uint8_t     request_num)
{
    p_hw_sct->dmarequest[request_num] |= (1 << 30);
}

/**
 * \brief Disable sets DMA request when counter Match registers are loaded 
 *        from its Reload registers
 *
 * \param[in] p_hw_sct    : Pointer to SCT register block
 * \param[in] request_num : DMA request num,  0 ~ AMHW_LPC_SCT_DMA_REQUEST_NUM - 1
 *              
 * \return  None    
 */
am_static_inline 
void amhw_lpc_sct_dma_request_reload_disable (amhw_lpc_sct_t *p_hw_sct, 
                                          uint8_t     request_num)
{
    p_hw_sct->dmarequest[request_num] &= ~(1 << 30);
}

/**
 * \brief Check the DMA request set or not
 *
 * \param[in] p_hw_sct    : Pointer to SCT register block
 * \param[in] request_num : DMA request num,  0 ~ AMHW_LPC_SCT_DMA_REQUEST_NUM - 1
 * 
 * \retval  AM_TRUE  : the DMA request is set.
 * \retval  AM_FALSE : the DMA request is not set.
 */
am_static_inline 
am_bool_t amhw_lpc_sct_dma_request_check (amhw_lpc_sct_t *p_hw_sct, uint8_t request_num)
{
    return (am_bool_t)((p_hw_sct->dmarequest[request_num] & (1ul << 31)) != 0);
}

/**
 * \brief Enable the interrupt for the specified event
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return  None
 */
am_static_inline
void amhw_lpc_sct_event_int_enable (amhw_lpc_sct_t *p_hw_sct, uint32_t evt_num)
{
    p_hw_sct->even |= (1 << evt_num);
}

/**
 * \brief Disable the interrupt for the specified event
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * 
 * \return  None
 */
am_static_inline 
void amhw_lpc_sct_event_int_disable (amhw_lpc_sct_t *p_hw_sct, uint32_t evt_num)
{
    p_hw_sct->even &= ~(1 << evt_num);
}

/**
 * \brief Get event flag
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \return event flag
 */
am_static_inline 
uint32_t amhw_lpc_sct_event_flag_get (amhw_lpc_sct_t *p_hw_sct) 
{
    return p_hw_sct->evflag;
}

/**
 * \brief Check the specified event flag whether set
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 *
 * \retval  AM_TRUE  : the event flag set
 * \retval  AM_FALSE : not set
 */
am_static_inline
am_bool_t amhw_lpc_sct_event_flag_chk (amhw_lpc_sct_t *p_hw_sct, uint32_t evt_num)
{
    return (am_bool_t)((p_hw_sct->evflag & (1 << evt_num)) != 0);
}

/**
 * \brief Clear the specified event flag
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] evt_num  : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 *
 * \return  None 
 */
am_static_inline 
void amhw_lpc_sct_event_flag_clr (amhw_lpc_sct_t *p_hw_sct, uint32_t evt_num) 
{
    p_hw_sct->evflag = (1 << evt_num);
}

/**
 * \brief Enable the interrupt for the conflict on output
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *  
 * \return  None
 */
am_static_inline 
void amhw_lpc_sct_conflict_int_enable (amhw_lpc_sct_t *p_hw_sct, uint8_t output_num) 
{
    p_hw_sct->conen |= (1 << output_num);
}

/**
 * \brief Disable the interrupt for the conflict on output
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_sct_conflict_int_disable (amhw_lpc_sct_t *p_hw_sct, uint8_t output_num) 
{
    p_hw_sct->conen &= ~(1 << output_num);
}

/**
 * \brief Get the conflict flag
 * \param[in] p_hw_sct  : Pointer to SCT register block
 * \return  the conflict flag. Bit n (0 ~ 7) is one if a no-change conflict 
 *                             event occurred on output n
 * 
 * \note  Bit 30 is one if the most recent bus error from this SCT involved 
 *        writing CTR L/Unified, STATE L/Unified, MATCH L/Unified, or the Output
 *        register when the L/U counter was not halted.
 *        Bit31 is one if The most recent bus error from this SCT involved 
 *        writing CTR H, STATE H, MATCH H, or the Output register when the H 
 *        counter was not halted
 */
am_static_inline 
uint32_t amhw_lpc_sct_conflict_flag_get (amhw_lpc_sct_t *p_hw_sct) 
{
    return p_hw_sct->conflag;
}

/**
 * \brief Clear the conflict flag 
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] flags    : '1' in bit0 ~7  output conflict flag,
 *                       '1' in bit30    BUSERRL flag
 *                       '1' in bit31    BUSERRH flag
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_conflict_flag_clr (amhw_lpc_sct_t  *p_hw_sct, uint32_t flags) 
{
    p_hw_sct->conflag = (1 << flags);
}

/**
 * \brief  SCT match/capture registers mode configure
 *
 * \param[in] p_hw_sct    : Pointer to SCT register block
 * \param[in] sct_mode    : The SCT mode
 * \param[in] matcap_num  : 0 ~ AMHW_LPC_SCT_MATCAP_REG_NUM - 1
 * \param[in] matcap_mode : the mode to be configure.
 *                           - AMHW_LPC_SCT_MATCAP_MATCH, operate as match registers.
 *                           - AMHW_LPC_SCT_MATCAP_CAPTURE, operate as capture registers.
 * \return  None
 */
void amhw_lpc_sct_regmode_config(amhw_lpc_sct_t            *p_hw_sct,
                             amhw_lpc_sct_mode_t        sct_mode, 
                             uint32_t               matcap_num,
                             amhw_lpc_sct_matcap_mode_t matcap_mode);


/**
 * \brief Set the match value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : The SCT mode
 * \param[in] mat_num  : 0 ~ AMHW_LPC_SCT_MATCAP_REG_NUM - 1
 * \param[in] value    : The value to set.
 *
 * \return  None
 *
 * \note Writing to the MATCH_L, MATCH_H,or unified register is only allowed
 *       when the corresponding counter is halted (HALT bits are  set to 1 in
 *       the CTRL register).
 *       If the mode is AMHW_LPC_SCT_MODE_SEPARATE_L or
 *       AMHW_LPC_SCT_MODE_SEPARATE_H,the value is only low 16bits valid
 */
void amhw_lpc_sct_mat_val_set(amhw_lpc_sct_t     *p_hw_sct, 
                          amhw_lpc_sct_mode_t mode,
                          uint32_t        mat_num, 
                          uint32_t        value);

/**
 * \brief Set the match reload value in State Configurable Timer
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] mode     : if the mode is AMHW_LPC_SCT_MODE_SEPARATE_L or
 *                       AMHW_LPC_SCT_MODE_SEPARATE_H,the count is only low 16bits valid
 * \param[in] num      : Match reload register number
 * \param[in] value    : The value to set.
 *                     
 * \return  None
 */
void amhw_lpc_sct_mat_reload_val_set(amhw_lpc_sct_t     *p_hw_sct, 
                                 amhw_lpc_sct_mode_t mode,
                                 uint32_t        num, 
                                 uint32_t        value);
/**
 * \brief Control the capture.
 *
 *    Enable or disable the event to causes the cap register to be loaded
 *                    
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] sct_mode : The SCT mode
 * \param[in] cap_num  : 0 ~ AMHW_LPC_SCT_MATCAP_REG_NUM - 1
 * \param[in] evt_num  : the event num to causes the cap register to be loaded
 *                       0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] active   : AM_TRUE: enable  the event to causes the cap register to be loaded;
 *                       AM_FALSE: disable the event to causes the cap register to be loaded
 * \return  None
 */         
void amhw_lpc_sct_cap_ctrl(amhw_lpc_sct_t      *p_hw_sct,
                       amhw_lpc_sct_mode_t  sct_mode,
                       uint32_t         cap_num, 
                       uint32_t         evt_num,
                       am_bool_t           active);

/**
 * \brief Get the capture value
 *
 * \param[in] p_hw_sct : Pointer to SCT register block
 * \param[in] sct_mode : The SCT mode
 * \param[in] cap_num  : 0 ~ AMHW_LPC_SCT_MATCAP_REG_NUM - 1
 *
 * \return  capture value.
 *          if the mode is AMHW_LPC_SCT_MODE_SEPARATE_L or AMHW_LPC_SCT_MODE_SEPARATE_H
 *          the return value is only low 16bits valid.
 */
uint32_t amhw_lpc_sct_cap_val_get(amhw_lpc_sct_t      *p_hw_sct, 
                              amhw_lpc_sct_mode_t  sct_mode,
                              uint32_t         cap_num);
 
/**
 * \brief Enable the event[evt_num] Set the output[output_num] to 1
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] evt_num    : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_out_set_enable (amhw_lpc_sct_t  *p_hw_sct,
                              uint8_t      output_num,
                              uint32_t     evt_num)
{
    p_hw_sct->out[output_num].set |= (1 << evt_num);
}

/**
 * \brief Disable the the event[evt_num] Set the output[output_num] to 1
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] evt_num    : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_out_set_disable (amhw_lpc_sct_t *p_hw_sct,
                               uint8_t     output_num,
                               uint32_t    evt_num) 
{
    p_hw_sct->out[output_num].set &= ~(1 << evt_num);
}

/**
 * \brief Enable the event[evt_num] Clear the output[output_num] to 0
 *
 * \param[in] p_hw_sct   : Pointer to SCT register block
 * \param[in] evt_num    : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] output_num : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_out_clr_enable (amhw_lpc_sct_t  *p_hw_sct,
                              uint8_t      output_num,
                              uint32_t     evt_num) 
{
    p_hw_sct->out[output_num].clr |= (1 << evt_num);
}

/**
 * \brief Disable the event[evt_num] Clear the output[output_num] to 0
 *
 * \param[in] p_hw_sct    : Pointer to SCT register block
 * \param[in] evt_num     : 0 ~ AMHW_LPC_SCT_EVENT_NUM - 1
 * \param[in] output_num  : 0 ~ AMHW_LPC_SCT_OUTPUT_NUM - 1
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_sct_out_clr_disable (amhw_lpc_sct_t *p_hw_sct,
                               uint8_t     output_num,
                               uint32_t    evt_num)
{ 
    p_hw_sct->out[output_num].clr &= ~(1 << evt_num);
}

/* End of section using anonymous unions  */
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

/**
 * @} amhw_lpc_if_sct
 */
 
#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_LPC_SCT_H */

/* end of file */


