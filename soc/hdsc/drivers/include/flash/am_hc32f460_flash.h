/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief FlashÇý¶¯²ã½Ó¿Ú
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-04  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_FLASH_H
#define __AM_HC32F460_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hw/amhw_hc32f460_flash.h"

/**
 * \addtogroup am_hc32_if_flash
 * \copydoc am_hc32_flash.h
 * @{
 */

void am_hc32f460_flash_unlock(amhw_hc32f460_flash_t            *p_hw_flash);
void am_hc32f460_flash_waitcycle(amhw_hc32f460_flash_t            *p_hw_flash,
                                 amhw_hc32f460_flash_read_waittime waitcycle);
void am_hc32f460_flash_lock(amhw_hc32f460_flash_t            *p_hw_flash);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_HC32_FLASH_H */
