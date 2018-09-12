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
 * \brief Capture driver implementation for SCT
 *
 * 1. The SCT can provide 3 service as follows,This driver implement 
 *    the capture function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The SCT resource used as follows:
 *    - The capture 0 to capture SCT input 0,associate with event 0
 *    - The capture 1 to capture SCT input 1,associate with event 1
 *    - The capture 2 to capture SCT input 2,associate with event 2
 *    - The capture 3 to capture SCT input 3,associate with event 3
 *    - The capture 4 to capture SCT input 4,associate with event 4
 *    - The capture 5 to capture SCT input 5,associate with event 5
 *    - The capture 6 to capture SCT input 6,associate with event 6
 *    - The capture 7 to capture SCT input 7,associate with event 7
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-14  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_SCT_CAP_H
#define __AM_LPC_SCT_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "hw/amhw_lpc_sct.h"

/**
 * \addtogroup am_lpc_if_sct_cap
 * \copydoc am_lpc_sct_cap.h
 * @{
 */

/**
 * \brief LPC5410x GPIO information for Capture,The channel 6 and 7 is internal 
 *        Signal, needn't external GPIO
 */
typedef struct am_lpc_sct_cap_ioinfo {
    uint32_t gpio;                          /**< \brief capture IO                                  */
    uint32_t func;                          /**< \brief The IO function for Capture                 */
    uint32_t dfunc;                         /**< \brief The default function after disable capture  */
} am_lpc_sct_cap_ioinfo_t;

/**
 * \brief LPC5410x SCT for capture initialize parameter structure
 */
typedef struct am_lpc_sct_cap_devinfo {
    uint32_t                  sct_regbase;    /**< \brief SCT¼Ä´æÆ÷»ùµØÖ·.                             */
    uint8_t                   inum;           /**< \brief The SCT interrupt number                   */
    uint32_t                  clk_id;         /**< \brief SCT clk id                                 */

    uint8_t                   channels_num;   /**< \brief Total used channels,The maximum value is 8 */
    am_lpc_sct_cap_ioinfo_t  *p_ioinfo;       /**< \brief Pointer to the GPIO information            */

    void (*pfn_plfm_init)(void);              /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void);            /**< \brief plfm deinit fuc */

} am_lpc_sct_cap_devinfo_t;
 
/**
 * \brief LPC5410x SCT for Capture device structure
 */
typedef struct am_lpc_sct_cap_dev {
    
    /** \brief For standard capture service */
    am_cap_serv_t          cap_serv;
    
    /** \brief To store maximum 8 channels callback functions */
    struct {   
        am_cap_callback_t  callback_func;        /**< \brief callback function                   */
        void              *p_arg;                /**< \brief The parameter for callback function */
    } callback_info[8];                          /**< \brief Most support 8 channel              */
    
    /** \brief Pointer to the device information */
    const am_lpc_sct_cap_devinfo_t *p_devinfo; 

} am_lpc_sct_cap_dev_t;

/**
 * \brief Initialize the SCT for CAP
 *
 * \param[in] p_dev     : Pointer to the SCT for CAP device
 * \param[in] p_devinfo : Pointer to the SCT for CAP device information to initialize
 *
 * \return The handle for standard capture port.if NULL,Indicate that initialize failed
 */
am_cap_handle_t am_lpc_sct_cap_init(am_lpc_sct_cap_dev_t           *p_dev,
                                  const am_lpc_sct_cap_devinfo_t *p_devinfo);

/**
 * \brief De-initialize the SCT for CAP
 * \param[in] p_dev   : Pointer to the SCT for CAP device
 * \return None
 */
void am_lpc_sct_cap_deinit(am_lpc_sct_cap_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_SCT_CAP_H */

/* end of file */
