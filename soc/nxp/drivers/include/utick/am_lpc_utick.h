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
 * \brief UTICK drivers for standard timer
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_UTICK_H
#define __AM_LPC_UTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_lpc_utick.h"

/**
 * \addtogroup am_lpc_if_utick
 * \copydoc am_lpc_utick.h
 * @{
 */

/**
 * \brief LPC54xxx UTICK initialize parameter structure
 */
typedef struct am_lpc_utick_devinfo {
    uint32_t          utick_regbase; /**< \brief UTICK register baseaddr */
    uint16_t          inum;          /**< \brief The UTICK interrupt number    */
    uint32_t          clk_id;        /**< \brief UTICK clk id                  */

    void (*pfn_plfm_init)(void);    /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void);  /**< \brief plfm deinit fuc */
} am_lpc_utick_devinfo_t;
    
/**
 * \brief LPC54xxx standard UTICK device structure
 */
typedef struct am_lpc_utick_dev {

    am_timer_serv_t   timer_serv;       /**< \brief for timer service */

    struct {
        void (*pfn_callback)(void *);   /**< \brief pointer to callback function        */
        void *p_arg;                    /**< \brief argument for the callback function */
    } callback_info[1];                 /**< \brief Only support one channel            */
    
    /** \brief Pointer to the device information */
    const am_lpc_utick_devinfo_t  *p_devinfo; 

} am_lpc_utick_dev_t;

/**
 * \brief Initialize the UTICK as TIMER
 *
 * \param[in] p_dev     : Pointer to the UTICK device
 * \param[in] p_devinfo : Pointer to the UTICK device information to initialize
 *
 * \return The handle for standard TIMER port.if NULL,Indicate that initialize failed
 */
am_timer_handle_t am_lpc_utick_init(am_lpc_utick_dev_t            *p_dev,
                                  const am_lpc_utick_devinfo_t  *p_devinfo);

/**
 * \brief De-initialize the UTICK
 * \param[in] p_dev : Pointer to the UTICK device
 * \return None
 */
void am_lpc_utick_deinit(am_lpc_utick_dev_t *p_dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_UTICK_H */

/* end of file */
