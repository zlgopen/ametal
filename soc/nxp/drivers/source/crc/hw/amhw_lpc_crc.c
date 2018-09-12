/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2012 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief CRC implementation for operating hardware.
 * 
 * - Supports three common polynomials CRC-CCITT, CRC-16, and CRC-32. 
 *    1. CRC-CCITT : x16 + x12 + x5 + 1   (0x1021)
 *    2. CRC-16    : x16 + x15 + x2 + 1   (0x8005)
 *    3. CRC-32    : x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + 
 *                   x5 + x4 + x2+ x + 1  (0x04C11DB7)
 * - Bit order reverse and 1's complement programmable setting for input data 
 *   and CRC sum.
 * - Programmable seed number setting.
 * - Supports CPU PIO back-to-back transfer.
 * - Accept any size of data width per write: 8, 16 or 32-bit.
 *    - 8-bit  write: 1-cycle operation
 *    - 16-bit write: 2-cycle operation (8-bit x 2-cycle)
 *    - 32-bit write: 4-cycle operation (8-bit x 4-cycle)
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-01  tee, first implementation.
 * \endinternal
 */

#include "hw/amhw_lpc_crc.h"
 
/******************************************************************************
* Public functions
*******************************************************************************/
 
/**
 * \brief Writing n 8-bit data to the CRC engine
 */
void amhw_lpc_crc_data8_write (amhw_lpc_crc_t    *p_hw_crc,
                           const uint8_t *p_data,
                           uint32_t       nbytes)
{
    while (nbytes > 0) {
        amhw_lpc_crc_wrdata8(p_hw_crc, *p_data);
        p_data++;
        nbytes--;
    }
}

/**
 * \brief Writing n 16-bit data to the CRC engine
 */
void amhw_lpc_crc_data16_write (amhw_lpc_crc_t      *p_hw_crc,
                            const uint16_t  *p_data,
                            uint32_t         nhwords)
{
    while (nhwords > 0) {
        amhw_lpc_crc_wrdata16(p_hw_crc, *p_data);
        p_data++;
        nhwords--;
    }
}

/**
 * \brief Writing n 32-bit data to the CRC engine
 */
void amhw_lpc_crc_data32_write (amhw_lpc_crc_t     *p_hw_crc, 
                            const uint32_t *p_data,
                            uint32_t        nwords)
{
    while (nwords > 0) {
        amhw_lpc_crc_wrdata32(p_hw_crc, *p_data);
        p_data++;
        nwords--;
    }
}

/* end of file */
