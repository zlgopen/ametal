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
 * \brief  WWDT drivers for WDT standard interface
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-01-07  fft, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_WWDT_H
#define __AM_LPC_WWDT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_wdt.h"
#include "hw/amhw_lpc_wwdt.h"
    
/** 
 * \addtogroup am_lpc_if_wwdt
 * \copydoc am_lpc_wwdt.h
 * @{
 */

/**
 * \brief LPC5410x WWDT device information
 */
typedef struct am_lpc_wwdt_devinfo {
    uint32_t         wwdt_regbase; /**< \brief WWDT register base addr */

    uint32_t         clk_id;       /**< \brief WWDT clk id     */

    void (*pfn_plfm_init)(void);   /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void); /**< \brief plfm deinit fuc */

} am_lpc_wwdt_devinfo_t;
    
/**
 * \brief LPC5410x WWDT device structure
 */
typedef struct am_lpc_wwdt_dev {
    am_wdt_serv_t              wdt_serv;   /**< \brief for standard WWDT service         */
    const am_lpc_wwdt_devinfo_t *p_devinfo;  /**< \brief Pointer to the device information */
} am_lpc_wwdt_dev_t;

/**
 * \brief Initialize the WWDT as WDT
 *
 * \param[in] p_dev     : Pointer to the WWDT device
 * \param[in] p_devinfo : Pointer to the WWDT device information to initialize
 *
 * \return The handle for standard WWDT port.if NULL,Indicate that initialize failed
 */
am_wdt_handle_t am_lpc_wwdt_init(am_lpc_wwdt_dev_t           *p_dev,
                                 const am_lpc_wwdt_devinfo_t *p_devinfo);

/**
 * \brief De-initialize the WWDT
 * \param[in] p_dev : Pointer to the WWDT device
 * \return None
 */
void am_lpc_wwdt_deinit(am_lpc_wwdt_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_WWDT_H */

/* end of file */
