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
 * \brief CRC Peripheral Access Layer
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-30  fra, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_MM32_CRC_H
#define __AMHW_MM32_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_mm32_if_crc
 * \copydoc amhw_mm32_crc.h
 * @{
 */

/**
 * \brief CRC - Register Layout Typedef
 */
typedef struct amhw_mm32_crc {
    __IO uint32_t crcdr;   /**< \brief CRC Data register */
    __IO uint32_t crcidr;  /**< \brief CRC independent register */
    __O  uint32_t crcctl;  /**< \brief CRC control register */
} amhw_mm32_crc_t;

/**
 * \brief CRC 32bit write data
 *
 * \param[in] p_hw_crc : the pointer to the CRC register base address
 * \param[in] data     : the data to be caculated
 *
 * retval : none
 */
am_static_inline
void amhw_mm32_crc_32bit_write_data (amhw_mm32_crc_t    *p_hw_crc,
                                    uint32_t           data)
{
    p_hw_crc->crcdr = data;
}

/**
 * \brief CRC 32 bit read data
 *
 * \param[in] p_hw_crc : the pointer to the CRC register base address
 *
 * \retval : the result of caculate value
 */
am_static_inline
uint32_t amhw_mm32_crc_32bit_read_data (amhw_mm32_crc_t *p_hw_crc)
{
    return p_hw_crc->crcdr;
}

/**
 * \brief CRC 8bit write data
 *
 * \param[in] p_hw_crc : the pointer to the CRC register base address
 * \param[in] data     : the data to be storaged
 *
 * retval : none
 *
 * \note this register can not  caculate data
 */
am_static_inline
void amhw_mm32_crc_8bit_write_data (amhw_mm32_crc_t *p_hw_crc, uint8_t data)
{
    p_hw_crc->crcidr = data;
}

/**
 * \brief CRC 8 bit read data
 *
 * \param[in] p_hw_crc : the pointer to the CRC register base address
 *
 * \retval : the storaged data
 *
 * \note this register can not  caculate data
 */
am_static_inline
uint8_t amhw_mm32_crc_8bit_read_data (amhw_mm32_crc_t *p_hw_crc)
{
    return p_hw_crc->crcidr;
}

/**
 * \brief CRC reset
 *
 * \param[in] p_hw_crc : the pointer to the CRC register base address
 *
 * \retval : none
 */
am_static_inline
void amhw_mm32_crc_reset (amhw_mm32_crc_t *p_hw_crc)
{
    p_hw_crc->crcctl = 1;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32_CRC_H */

/* end of file */
