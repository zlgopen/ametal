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
 * \brief SCTimer/PWM (SCT) implementation for operating hardware.
 *
 * - Multi-function timer with many capabilities, including:
 *       - PWM
 *       - capture
 *       - Match output
 * - Two 16-bit counters or one 32-bit counter.
 * - Counters clocked by bus clock or selected input.
 * - Up counters or up-down counters.
 * - State variable allows sequencing across multiple counter cycles.
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
 *       - Output 7 can be connected to the ADC trigger input mux
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#include "hw/amhw_lpc_sct.h"

/*******************************************************************************
* Public functions
*******************************************************************************/

/**
 * \brief Set the SCT prescale value
 */
void amhw_lpc_sct_prescale_set (amhw_lpc_sct_t      *p_hw_sct,
                                amhw_lpc_sct_mode_t  mode,
                                uint8_t              prescale)
{
    volatile uint32_t tmp = 0;
    
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_H) == 0) {   /* not in halt,can't set value */
            return;
        }            
        
        tmp              =  p_hw_sct->ctrl_u;
        tmp             &= ~(0xFFul << 21);
        p_hw_sct->ctrl_u = tmp | AMHW_LPC_SCT_CTRL_PRE_H(prescale);  
        
    } else {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_L) == 0) {   /* not in halt,can't set value */
            return;
        }            
        
        tmp              =  p_hw_sct->ctrl_u;
        tmp             &= ~(0xFFul << 5);
        p_hw_sct->ctrl_u = tmp | AMHW_LPC_SCT_CTRL_PRE_L(prescale);       
    }
}

/**
 * \brief Get the SCT prescale value
 */
uint8_t amhw_lpc_sct_prescale_get (amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode)
{
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {         
        return ((p_hw_sct->ctrl_u) >> 21) & 0xFF;        
    } else {       
        return ((p_hw_sct->ctrl_u) >>  5) & 0xFF;             
    }    
}

/**
 * \brief Configure Conflict resolution
 */
void amhw_lpc_sct_conflict_res_config (amhw_lpc_sct_t *p_hw_sct, 
                                       uint8_t     outnum,
                                       uint8_t     value)
{
    uint32_t tmp  = p_hw_sct->res & (~(0x03 << (2 * outnum)));
    p_hw_sct->res = tmp | ((value & 0x03) << (2 * outnum));
}

/**
 * \brief control the output dir
 */
void amhw_lpc_sct_output_dir_ctrl (amhw_lpc_sct_t *p_hw_sct, 
                                   uint8_t     outnum,
                                   uint8_t     value)
{
    uint32_t tmp            = p_hw_sct->outputdirctrl & (~(0x03 << (2 * outnum)));
    p_hw_sct->outputdirctrl = tmp | ((value & 0x03) << (2 * outnum));    
}

/**
 * \brief Set SCT count value 
 */
void amhw_lpc_sct_count_set (amhw_lpc_sct_t     *p_hw_sct, 
                             amhw_lpc_sct_mode_t mode,
                             uint32_t        count)
{
    if (mode == AMHW_LPC_SCT_MODE_UNIFY) {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_L) == 0) {  /* not in halt,can't set value */
            return;
        }
        p_hw_sct->count_u = count;
        
    } else if (mode == AMHW_LPC_SCT_MODE_SEPARATE_L) {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_L) == 0) {  /* not in halt,can't set value */
            return;
        }
        p_hw_sct->count_l = (uint16_t)count;   
        
    } else if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_H) == 0) {  /* not in halt,can't set value */
            return;
        }
        p_hw_sct->count_h = (uint16_t)count;             
    }
}

/**
 * \brief Get SCT count value
 */
uint32_t amhw_lpc_sct_count_get (amhw_lpc_sct_t *p_hw_sct, amhw_lpc_sct_mode_t mode)
{
    uint16_t value = 0;
    
    if (mode == AMHW_LPC_SCT_MODE_UNIFY) {
        return p_hw_sct->count_u;
        
    } else if(mode == AMHW_LPC_SCT_MODE_SEPARATE_L) {
        value = (uint16_t)p_hw_sct->count_l;
        
    } else if(mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        value = (uint16_t)p_hw_sct->count_h;
    }
    
    return value;
}

/**
 * \brief SCT match/capture registers mode configure
 */
void amhw_lpc_sct_regmode_config (amhw_lpc_sct_t            *p_hw_sct,
                              amhw_lpc_sct_mode_t        sct_mode, 
                              uint32_t               num,
                              amhw_lpc_sct_matcap_mode_t matcap_mode)
{
    if (sct_mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        
        /* the reg Operate as capture registers */
        if (matcap_mode == AMHW_LPC_SCT_MATCAP_CAPTURE) { 
            p_hw_sct->regmode_h |= (1 << num);
        } else {
            p_hw_sct->regmode_h &= ~(1 << num);
        }
    } else {
        
        /* the reg Operate as capture registers */
        if (matcap_mode == AMHW_LPC_SCT_MATCAP_CAPTURE) {
            p_hw_sct->regmode_l |= (1 << num);
        } else {
            p_hw_sct->regmode_l &= ~(1 << num);
        }
    }
}

/**
 * \brief Set the match value
 */
void amhw_lpc_sct_mat_val_set (amhw_lpc_sct_t     *p_hw_sct,
                           amhw_lpc_sct_mode_t mode,
                           uint32_t        num,
                           uint32_t        value)
{
    if (mode == AMHW_LPC_SCT_MODE_UNIFY) {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_L) == 0) {  /* not in halt,can't set value */
            return;
        }
        p_hw_sct->match[num].u = value;
        
    } else if (mode == AMHW_LPC_SCT_MODE_SEPARATE_L) {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_L) == 0) {  /* not in halt,can't set value */
            return;
        }
        p_hw_sct->match[num].l = (uint16_t)value;
        
    } else if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        if ((p_hw_sct->ctrl_u & AMHW_LPC_SCT_CTRL_HALT_H) == 0) {  /* not in halt,can't set value */
            return;
        }
        p_hw_sct->match[num].h = (uint16_t)value;
    }   
}

/**
 * \brief Set the match reload value in State Configurable Timer
 */
void amhw_lpc_sct_mat_reload_val_set (amhw_lpc_sct_t     *p_hw_sct, 
                                  amhw_lpc_sct_mode_t mode,
                                  uint32_t        num, 
                                  uint32_t        value)
{
    if (mode == AMHW_LPC_SCT_MODE_UNIFY) {
        p_hw_sct->matchrel[num].u = value;
        
    } else if (mode == AMHW_LPC_SCT_MODE_SEPARATE_L) {
        p_hw_sct->matchrel[num].l = (uint16_t)value;
        
    } else if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        p_hw_sct->matchrel[num].h = (uint16_t)value;
    }
}

/**
 * \brief Control the capture
 */
void amhw_lpc_sct_cap_ctrl (amhw_lpc_sct_t      *p_hw_sct,
                        amhw_lpc_sct_mode_t  sct_mode,
                        uint32_t         cap_num, 
                        uint32_t         evt_num,
                        am_bool_t           active)
{
    if (active == AM_TRUE) {
        if (sct_mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
            p_hw_sct->capctrl[cap_num].h |= (1 << evt_num);
        } else {
            p_hw_sct->capctrl[cap_num].l |= (1 << evt_num);
        }   
    } else {
        if (sct_mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
            p_hw_sct->capctrl[cap_num].h &= ~(1 << evt_num);
        } else {      
            p_hw_sct->capctrl[cap_num].l &= ~(1 << evt_num);
        }           
    }
}

/**
 * \brief Get the capture value
 */ 
uint32_t amhw_lpc_sct_cap_val_get (amhw_lpc_sct_t      *p_hw_sct,
                               amhw_lpc_sct_mode_t  sct_mode,
                               uint32_t         cap_num)
{
    uint32_t value = 0;
    
    if (sct_mode == AMHW_LPC_SCT_MODE_UNIFY) {
        return p_hw_sct->cap[cap_num].u;
        
    } else if (sct_mode == AMHW_LPC_SCT_MODE_SEPARATE_L) {
        value = p_hw_sct->cap[cap_num].l;
        
    } else if (sct_mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        value = p_hw_sct->cap[cap_num].h;
    } 

    return value;
    
}

/**
 * \brief Bring the SCT from HALT to STOP(not running)
 */
void amhw_lpc_sct_ctrl_halt_to_stop (amhw_lpc_sct_t      *p_hw_sct,
                                 amhw_lpc_sct_mode_t  mode)
{
    uint32_t tmp;
    
    tmp = p_hw_sct->ctrl_u;
    
    if (mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {
        tmp &= (~AMHW_LPC_SCT_CTRL_HALT_H);
        tmp |= AMHW_LPC_SCT_CTRL_STOP_H;
        
        p_hw_sct->ctrl_u = tmp;
    } else {
        
        tmp &= (~AMHW_LPC_SCT_CTRL_HALT_L);
        tmp |= AMHW_LPC_SCT_CTRL_STOP_L;
        
        p_hw_sct->ctrl_u = tmp;
    }
}

/* end of file */
