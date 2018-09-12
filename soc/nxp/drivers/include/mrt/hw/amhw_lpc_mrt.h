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
 * \brief MRT(Multi-Rate Timer) interfaces for operating hardware.
 *
 * - 24-bit interrupt timer
 * - Four channels independently counting down from individually set values
 * - Repeat interrupt, one-shot interrupt, and one-shot bus stall modes
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_MRT_H
#define __AMHW_LPC_MRT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/** 
 * \addtogroup amhw_lpc_if_mrt
 * \copydoc amhw_lpc_mrt.h
 * @{
 */
 
/**
 * \brief The MRT configuration
 */
#define AMHW_LPC_MRT_CHANNELS_NUM      (4)    /**< \brief the total MRT channels num    */
    
/** \brief the MRT channel id, 0 ~ AMHW_LPC_MRT_CHANNELS_NUM - 1 */
#define AMHW_LPC_MRT_CH(n)             (n)
    
/**
 * \brief MRT channel register block structure
 */
typedef struct amhw_lpc_mrt_chan {
    __IO uint32_t intval;                /**< \brief Timer interval register */
    __O  uint32_t timer;                 /**< \brief Timer register          */
    __IO uint32_t ctrl;                  /**< \brief Timer control register  */
    __IO uint32_t stat;                  /**< \brief Timer status register   */
} amhw_lpc_mrt_chan_t;

/**
 * \brief MRT register block structure
 */
typedef struct amhw_lpc_mrt {

    /** \brief The channel's register */
    amhw_lpc_mrt_chan_t channel[AMHW_LPC_MRT_CHANNELS_NUM]; 
    uint32_t       unused[44];     /**< \brief reserved,do not use           */
    __IO uint32_t  modcfg;         /**< \brief The MODCFG register           */
    __O  uint32_t  idle_ch;        /**< \brief The idle channel register     */
    __IO uint32_t  irq_flag;       /**< \brief The interrupt flags register  */ 
} amhw_lpc_mrt_t;

/**
 * \brief MRT Modes enumeration
 */
typedef enum {
    AMHW_LPC_MRT_MODE_REPEAT        = (0 << 1), /**< \brief MRT Repeat interrupt mode   */
    AMHW_LPC_MRT_MODE_ONESHOT       = (1 << 1), /**< \brief MRT One-shot interrupt mode */
    AMHW_LPC_MRT_MODE_ONESHOT_STALL = (2 << 1)  /**< \brief MRT One-shot stall mode     */
} amhw_lpc_mrt_mode_t;

/**
 * \name Macro defines for MRT register
 * @{
 */
 
/** \brief 24-bit max value                */
#define AMHW_LPC_MRT_INTVAL_MAX       (0xFFFFFF)  

/** \brief Force immediate load of intval  */
#define AMHW_LPC_MRT_INTVAL_LOAD      (1ul << 31)

/** \brief MRT interrupt enable control bit */
#define AMHW_LPC_MRT_CTRL_INTEN       (1ul << 0)

/** \brief MRT mode mask control bit */
#define AMHW_LPC_MRT_CTRL_MODE_MASK   (3ul << 1)
 
/** \brief MRT monitors the interrupt flag */
#define AMHW_LPC_MRT_STAT_INTFLAG     (1ul << 0)

/** \brief MRT indicates the state of TIMERn */
#define AMHW_LPC_MRT_STAT_RUNNING     (1ul << 1)

/** \brief MRT channel in use flag */
#define AMHW_LPC_MRT_STAT_INUSE       (1ul << 2)

/** @}*/

/**
 * \brief Sets the channel's interval value
 *
 *  The interval value is loaded into the TIMERn register and the MRT 
 *  channel n will start counting down from value -1.
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 * \param[in] value    : The interval time out (low 24-bits valid)
 * \param[in] forced   : AM_TRUE : the interval value to load immediately.
 *                       AM_FALSE: the time interval value will be loaded 
 *                            in next timer cycle
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_mrt_intval_set (amhw_lpc_mrt_t *p_hw_mrt,
                              uint8_t         chan,
                              uint32_t        value,
                              am_bool_t       forced)
{
    value &= AMHW_LPC_MRT_INTVAL_MAX; 
    
    /* Value is loaded into timer immediately */
    if (forced == AM_TRUE) {
        value |= AMHW_LPC_MRT_INTVAL_LOAD;
    }
    
    p_hw_mrt->channel[chan].intval = value;
}

/**
 * \brief get the channel interval value
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return    The channel interval value
 */
am_static_inline
uint32_t amhw_lpc_mrt_intval_get (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    return (p_hw_mrt->channel[chan].intval) & AMHW_LPC_MRT_INTVAL_MAX;
}

/**
 * \brief get the channel current count value of the down-counter
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return the channel current count value of the down-counter
 */
am_static_inline 
uint32_t amhw_lpc_mrt_chan_count_get (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    return (p_hw_mrt->channel[chan].timer);
}


/**
 * \brief Enable the MRT Channel's interrupt 
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return    None
 */
am_static_inline  
void amhw_lpc_mrt_int_enable (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    p_hw_mrt->channel[chan].ctrl |= AMHW_LPC_MRT_CTRL_INTEN;
}

/**
 * \brief disable the MRT Channel's interrupt 
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_mrt_int_disable (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    p_hw_mrt->channel[chan].ctrl &= ~AMHW_LPC_MRT_CTRL_INTEN;
}

/**
 * \brief Set the timer mode
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 * \param[in] mode     : The timer mode 
 *
 * \return None 
 */
am_static_inline
void amhw_lpc_mrt_mode_set (amhw_lpc_mrt_t     *p_hw_mrt, 
                            uint8_t         chan,
                            amhw_lpc_mrt_mode_t mode)
{
    p_hw_mrt->channel[chan].ctrl = (p_hw_mrt->channel[chan].ctrl 
                                 & (~AMHW_LPC_MRT_CTRL_MODE_MASK))
                                 | (uint32_t) mode;
}
/**
 * \brief Get the timer mode
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return    The channel's mode now.
 */
am_static_inline
amhw_lpc_mrt_mode_t amhw_lpc_mrt_mode_get (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    return (amhw_lpc_mrt_mode_t)(p_hw_mrt->channel[chan].ctrl 
                                     & AMHW_LPC_MRT_CTRL_MODE_MASK);
}


/**
 * \brief Get the channel's Status
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block.
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return    The Status of the channel.
 */
am_static_inline
uint32_t amhw_lpc_mrt_stat_get (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    return p_hw_mrt->channel[chan].stat;
}

/**
 * \brief  Clears the pending interrupt (if any)
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block.
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_mrt_int_flag_clr (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    p_hw_mrt->channel[chan].stat |= AMHW_LPC_MRT_STAT_INTFLAG;
}


/**
 * \brief Get the lowest idle timer channel
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \return    The lowest idle timer channel.
 */
am_static_inline
uint8_t amhw_lpc_mrt_idle_channel_get (amhw_lpc_mrt_t *p_hw_mrt)
{
    return (uint8_t)((p_hw_mrt->idle_ch) >> 4);
}

/**
 * \brief Get the interrupt pending status for specified channel.
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
* \retval  AM_TRUE  : interrupt pending
* \retval  AM_FALSE : not pending
 */
am_static_inline
am_bool_t amhw_lpc_mrt_int_pending_get (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    return (am_bool_t)((p_hw_mrt->irq_flag & (1 << chan)) != 0);
}

/**
 * \brief Clears the channel interrupt pending status 
 *
 * \param[in] p_hw_mrt : Pointer to MRT register block 
 * \param[in] chan     : The channel id, AMHW_LPC_MRT_CH(n).
 *
 * \return  None
 */
am_static_inline
void amhw_lpc_mrt_int_pending_clr (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    p_hw_mrt->irq_flag = (1 << chan);
}

/**
 * \brief 清除通道的中断挂起状态
 *
 * \param[in] p_hw_mrt : 指向MRT寄存器块的指针
 * \param[in] masks    : 通道掩码，bit-n对应通道n，为1的位中断挂起状态被清除
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_mrt_int_pending_maks_clr (amhw_lpc_mrt_t *p_hw_mrt,
                                        uint8_t            masks)
{
    p_hw_mrt->irq_flag = masks;
}

/**
 * \brief 判断指定通道的中断是否挂起
 *
 * \param[in] p_hw_mrt : 指向MRT寄存器块的指针
 * \param[in] chan     : 通道编号，AMHW_LPC_MRT_CH(n) (# AMHW_LPC_MRT_CH(0))
 *
 * \retval TRUE  : 中断挂起
 * \retval FALSE : 中断未挂起
 */
am_static_inline
am_bool_t amhw_lpc_mrt_chan_pending_chk (amhw_lpc_mrt_t *p_hw_mrt, uint8_t chan)
{
    return (AM_BIT_ISSET(p_hw_mrt->channel[chan].stat, 0) ? AM_TRUE : AM_FALSE);
}

/**
 * @} amhw_lpc_if_mrt
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_MRT_H */

/* end of file */
