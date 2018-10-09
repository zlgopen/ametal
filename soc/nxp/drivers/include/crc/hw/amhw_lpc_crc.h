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
 * \brief CRC interfaces for operating hardware.
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
 *    1. 8-bit  write: 1-cycle operation
 *    2. 16-bit write: 2-cycle operation (8-bit x 2-cycle)
 *    3. 32-bit write: 4-cycle operation (8-bit x 4-cycle)
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_CRC_H
#define __AMHW_LPC_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
    
/** 
 * \addtogroup amhw_lpc_if_crc 
 * \copydoc amhw_lpc_crc.h
 * @{
 */
    
/*
 * \brief Start of section using anonymous unions    
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif    

/**
 * \brief CRC register block structure
 */
typedef struct amhw_lpc_crc {          
    __IO    uint32_t    mode;           /**< \brief CRC Mode Register               */
    __IO    uint32_t    seed;           /**< \brief CRC SEED Register               */
    union {
        __I     uint32_t    sum;        /**< \brief CRC Checksum Register           */
        __O     uint32_t    wrdata32;   /**< \brief CRC write Data Register, 32-bit */
        __O     uint16_t    wrdata16;   /**< \brief CRC write Data Register, 16-bit */
        __O     uint8_t     wrdata8;    /**< \brief CRC write Data Register, 8-bit */
    };
} amhw_lpc_crc_t;
 

/**
 * \name Macro defines for CRC MODE register
 * @{
 */

#define AMHW_LPC_CRC_MODE_WRDATA_BIT_RVS (1 << 2)  /**< \brief Select Bit order reverse for WR_DATA */
#define AMHW_LPC_CRC_MODE_WRDATA_CMPL    (1 << 3)  /**< \brief Select One's complement for WR_DATA  */
#define AMHW_LPC_CRC_MODE_SUM_BIT_RVS    (1 << 4)  /**< \brief Select Bit order reverse for CRC_SUM */
#define AMHW_LPC_CRC_MODE_SUM_CMPL       (1 << 5)  /**< \brief Select One's complement for CRC_SUM  */

/** @} */

/**
 * \brief CRC polynomial enum 
 */
typedef enum amhw_lpc_crc_poly {
    AMHW_LPC_CRC_POLY_CCITT = 0,              /**< \brief CRC-CCIT polynomial   */
    AMHW_LPC_CRC_POLY_CRC16 = 1,              /**< \brief CRC-16   polynomial   */
    AMHW_LPC_CRC_POLY_CRC32 = 2,              /**< \brief CRC-32   polynomial   */
} amhw_lpc_crc_poly_t;


/**
 * \brief Set the CRC Mode
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] poly  : The enumerated polynomial to be used
 * \param[in] flags : An Or'ed value of flags that setup the mode
 *                    - AMHW_LPC_CRC_MODE_WRDATA_BIT_RVS
 *                    - AMHW_LPC_CRC_MODE_WRDATA_CMPL
 *                    - AMHW_LPC_CRC_MODE_SUM_BIT_RVS
 *                    - AMHW_LPC_CRC_MODE_SUM_CMPL
 *                    if no flags need to set, the flags value is 0.
 *
 * \return None
 */
am_static_inline
void amhw_lpc_crc_mode_set (amhw_lpc_crc_t     *p_hw_crc, 
                        amhw_lpc_crc_poly_t poly, 
                        uint32_t        flags)
{
    p_hw_crc->mode = (uint32_t)poly | flags;
}

/**
 * \brief Get the CRC Mode value
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \return The current value of the CRC Mode
 */
am_static_inline 
uint32_t amhw_lpc_crc_mode_get (amhw_lpc_crc_t *p_hw_crc)
{
    return p_hw_crc->mode;
}

/**
 * \brief Set the seed value
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] seed     : Seed value
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_crc_seed_set (amhw_lpc_crc_t *p_hw_crc, uint32_t seed)
{
    p_hw_crc->seed = seed;
}


/**
 * \brief Get the CRC seed value
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \return The seed value
 */
am_static_inline 
uint32_t amhw_lpc_crc_seed_get (amhw_lpc_crc_t *p_hw_crc)
{
    return p_hw_crc->seed;
}

/**
 * \brief writing one 8-bit data to the CRC engine
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] data     : 8-bit data to write
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_crc_wrdata8 (amhw_lpc_crc_t *p_hw_crc, uint8_t data)
{
    p_hw_crc->wrdata8 = data;
}

/**
 * \brief writing 16-bit data to the CRC engine
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] data     : 16-bit data to write
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_crc_wrdata16 (amhw_lpc_crc_t *p_hw_crc, uint16_t data)
{
    p_hw_crc->wrdata16 = data;
}

/**
 * \brief writing 32-bit data to the CRC engine
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] data     : 32-bit data to write
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_crc_wrdata32 (amhw_lpc_crc_t *p_hw_crc, uint32_t data)
{
    p_hw_crc->wrdata32 = data;
}

/**
 * \brief writing n 8-bit data to the CRC engine
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] p_data   : pointer to data buffer to write
 * \param[in] nbytes   : The length of data buffer to write.
 *
 * \return None
 */
void amhw_lpc_crc_data8_write(amhw_lpc_crc_t    *p_hw_crc, 
                          const uint8_t *p_data, 
                          uint32_t       nbytes);

/**
 * \brief writing n 16-bit data to the CRC engine
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] p_data   : Pointer to data buffer to write
 * \param[in] nhwords  : The length of data buffer to write.
 *
 * \return None
 */
void amhw_lpc_crc_data16_write(amhw_lpc_crc_t     *p_hw_crc, 
                           const uint16_t *p_data, 
                           uint32_t        nhwords);


/**
 * \brief writing n 32-bit data to the CRC engine
 *
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \param[in] p_data   : pointer to data buffer to write
 * \param[in] nwords   : The length of data buffer to write.
 *
 * \return None
 */
void amhw_lpc_crc_data32_write(amhw_lpc_crc_t     *p_hw_crc, 
                           const uint32_t *p_data, 
                           uint32_t        nwords);

/**
 * \brief Gets the CRC Sum based on the Mode and Seed as previously configured
 * \param[in] p_hw_crc : Pointer to CRC register block
 * \return CRC Checksum value
 */
am_static_inline 
uint32_t amhw_lpc_crc_sum_get (amhw_lpc_crc_t *p_hw_crc)
{
    return p_hw_crc->sum;
}
 
/*
 * \brief End of section using anonymous unions    
 */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_CRC_H */

/* end of file */
