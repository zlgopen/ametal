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
 * \brief CRC drivers for standard interface
 *
 * The CRC can support follow patterns:
 * 
 * 1. width     = 16; \n
 *    poly      = 0x1021; \n
 *    initvalue = can be any value; \n
 *    refin     = AM_TRUE or AM_FALSE; \n
 *    refout    = AM_TRUE or AM_FALSE; \n
 *    xorout    = 0x0000 or 0xFFFF; \n
 * 2. width     = 16; \n
 *    poly      = 0x8005; \n
 *    initvalue = can be any value; \n
 *    refin     = AM_TRUE or AM_FALSE; \n
 *    refout    = AM_TRUE or AM_FALSE; \n
 *    xorout    = 0x0000 or 0xFFFF; \n
 * 3. width     = 32; \n
 *    poly      = 0x04C11DB7; \n
 *    initvalue = can be any value; \n
 *    refin     = AM_TRUE or AM_FALSE; \n
 *    refout    = AM_TRUE or AM_FALSE; \n
 *    xorout    = 0x00000000 or 0xFFFFFFFF; \n
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-19  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_CRC_H
#define __AM_LPC_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_crc.h"
#include "hw/amhw_lpc_crc.h"

/**
 * \addtogroup am_lpc_if_crc
 * \copydoc am_lpc_crc.h
 * @{
 */

/**
 * \brief LPC54xxx CRC initialize structure
 */
typedef struct am_lpc_crc_devinfo {
    uint32_t   crc_regbase;                     /**< \brief CRC register baseaddr */
    void (*pfn_plfm_init)(void);                /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void);              /**< \brief plfm deinit fuc */
} am_lpc_crc_devinfo_t;
    
/**
 * \brief LPC54xxx CRC device structure
 */
typedef struct am_lpc_crc_dev {

    am_crc_serv_t              crc_serv;   /**< \brief for CRC service                   */
    am_crc_pattern_t          *p_patern;   /**< \brief Pointer to the CRC pattern        */
    const am_lpc_crc_devinfo_t  *p_devinfo;  /**< \brief Pointer to the device information */

} am_lpc_crc_dev_t;

/**
 * \brief Initialize the CRC
 *
 * \param[in] p_dev     : Pointer to the CRC device
 * \param[in] p_devinfo : Pointer to the CRC device information to initialize
 *
 * \return The handle for standard CRC port.if NULL,Indicate that Initialize failed
 */
am_crc_handle_t am_lpc_crc_init(am_lpc_crc_dev_t            *p_dev,
                              const am_lpc_crc_devinfo_t  *p_devinfo);

/**
 * \brief De-initialize the CRC
 *
 * \return None
 */
void am_lpc_crc_deinit(void);

/** 
 * @}  
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_CRC_H */

/* end of file */
