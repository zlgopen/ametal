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
 * \brief RIT drivers for Standard Timer drivers 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_RIT_H
#define __AM_LPC_RIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_lpc_rit.h"

/**
 * \addtogroup am_lpc_if_rit
 * \copydoc am_lpc_rit.h
 * @{
 */

/**
 * \brief RIT initialize parameter structure
 */
typedef struct am_lpc_rit_devinfo {
    uint32_t             rit_regbase;   /**< \brief RIT regbase addr                  */
    uint8_t              inum;          /**< \brief The RIT interrupt number          */
    uint32_t             clk_id;        /**< \brief RIT clk id                        */

    void (*pfn_plfm_init)(void);        /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void);      /**< \brief plfm deinit fuc */

} am_lpc_rit_devinfo_t;
    
/**
 * \brief RIT device structure
 */
typedef struct am_lpc_rit_dev {

    am_timer_serv_t   timer_serv;         /**< \brief for timer service       */

    struct {
        void (*pfn_callback)(void *);     /**< \brief pointer to interrupt function       */
        void *p_arg;                      /**< \brief argument for the interrupt function */
    } callback_info[1];                   /**< \brief Only support one channel            */
    
    /** \brief Pointer to the device information */
    const am_lpc_rit_devinfo_t  *p_devinfo; 

} am_lpc_rit_dev_t;

/**
 * \brief Initialize the RIT for timer
 *
 * \param[in] p_dev     : Pointer to the RIT device
 * \param[in] p_devinfo : Pointer to the RIT device information to initialize
 *
 * \return The handle for standard TIMER port.if NULL,Indicate that initialize failed
 */
am_timer_handle_t am_lpc_rit_init(am_lpc_rit_dev_t            *p_dev,
                                const am_lpc_rit_devinfo_t  *p_devinfo);

/**
 * \brief De-initialize the RIT
 * \param[in] p_dev : Pointer to the RIT device
 * \return None
 */
void am_lpc_rit_deinit(am_lpc_rit_dev_t *p_dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_RIT_H */

/* end of file */
