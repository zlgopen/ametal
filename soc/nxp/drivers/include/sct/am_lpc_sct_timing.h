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
 * \brief Timing driver implementation for SCT
 *
 * 1. The SCT can provide 3 service as follows,This driver implement 
 *    the timing function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The SCT resource used as follows:
 *    - The match 0 to save the period time,associate with event 0 to generate 
 *      interrupt.
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-14  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_SCT_TIMING_H
#define __AM_LPC_SCT_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_lpc_sct.h"

/**
 * \addtogroup am_lpc_if_sct_timing
 * \copydoc am_lpc_sct_timing.h
 * @{
 */

/** \brief The SCT operate as 1-32bit timer, only can provide 1 timing channel  */
#define AM_LPC_SCT_TIMING_1_32BIT      1

/** \brief The SCT operate as 2-16bit timer, can provide 2 timing channel      */
#define AM_LPC_SCT_TIMING_2_16BIT      2

/**
 * \brief LPC5410x SCT for timing initialize parameter structure
 */
typedef struct am_lpc_sct_timing_devinfo {
    uint32_t          sct_regbase; /**< \brief The SCT register block       */
    uint8_t           inum;        /**< \brief The SCT interrupt number                */
    uint32_t          clk_id;      /**< \brief SCT clk id                              */

    uint8_t           flag;        /**< \brief The SCT Timing mode(1-32bit or 2-16bit) */

    void (*pfn_plfm_init)(void);              /**< \brief plfm init  fuc  */
    void (*pfn_plfm_deinit)(void);             /**< \brief plfm deinit fuc */
} am_lpc_sct_timing_devinfo_t;
    
/**
 * \brief LPC5410x SCT for timing device structure
 */
typedef struct am_lpc_sct_timing_dev {

    am_timer_serv_t   timer_serv;    /**< \brief for timer service                       */

    struct {
        void (*pfn_callback)(void *);     /**< \brief pointer to callback function           */
        void *p_arg;                      /**< \brief argument for the callback function     */
    } callback_info[2];                   /**< \brief Most support two channel(2-16bit timer) */
    
    /** \brief Pointer to the device information */
    const am_lpc_sct_timing_devinfo_t  *p_devinfo; 

} am_lpc_sct_timing_dev_t;

/**
 * \brief Initialize the SCT for timer
 *
 * \param[in] p_dev     : Pointer to the SCT for timing device
 * \param[in] p_devinfo : Pointer to the SCT for timing device information to initialize
 *
 * \return The handle for standard Timer port.if NULL,Indicate that initialize failed
 */
am_timer_handle_t am_lpc_sct_timing_init(am_lpc_sct_timing_dev_t            *p_dev,
                                       const am_lpc_sct_timing_devinfo_t  *p_devinfo);

/**
 * \brief De-initialize the SCT
 * \param[in] p_dev : Pointer to the SCT device
 * \return None
 */
void am_lpc_sct_timing_deinit(am_lpc_sct_timing_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_SCT_TIMING_H */

/* end of file */
