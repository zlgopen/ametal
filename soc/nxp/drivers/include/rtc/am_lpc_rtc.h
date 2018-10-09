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
 * \brief  RTC drivers for standard interface 
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-27  fft, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_RTC_H
#define __AM_LPC_RTC_H


#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_rtc.h"
#include "hw/amhw_lpc_rtc.h"


/**
 * \addtogroup am_lpc_if_rtc
 * \copydoc am_lpc_rtc.h
 * @{
 */

/**
 * \brief LPC5410x RTC initialize parameter structure
 */
typedef struct am_lpc_rtc_devinfo {
    uint32_t rtc_regbase;          /**< \brief rtc regbase addr */

    void (*pfn_plfm_init)(void);   /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void); /**< \brief plfm deinit fuc */
} am_lpc_rtc_devinfo_t;
    
/**
 * \brief LPC5410x RTC device structure
 */
typedef struct am_lpc_rtc_dev {
    am_rtc_serv_t             rtc_serv;     /**< \brief for standard RTC service          */    
    const am_lpc_rtc_devinfo_t *p_devinfo;    /**< \brief Pointer to the device information */
} am_lpc_rtc_dev_t;

/**
 * \brief Initialize the RTC as RTC
 *
 * \param[in] p_dev     : Pointer to the RTC device
 * \param[in] p_devinfo : Pointer to the RTC device information to initialize
 *
 * \return The handle for standard RTC port.if NULL,Indicate that initialize failed
 */
am_rtc_handle_t am_lpc_rtc_init(am_lpc_rtc_dev_t           *p_dev,
                              const am_lpc_rtc_devinfo_t *p_devinfo);

/**
 * \brief De-initialize the RTC
 * \param[in] p_dev : Pointer to the RTC device
 * \return None
 */
void am_lpc_rtc_deinit(am_lpc_rtc_dev_t *p_dev);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
