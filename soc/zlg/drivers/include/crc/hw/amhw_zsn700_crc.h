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
 * - 1.00 20-03-18
 *
 * \endinternal
 */

#ifndef __AMHW_ZSN700_CRC_H
#define __AMHW_ZSN700_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zsn700_if_crc
 * \copydoc amhw_zsn700_crc.h
 * @{
 */

/**
 * \brief CRC - 寄存器结构体
 */
typedef struct amhw_zsn700_crc {
    __IO uint32_t crccr;      /**< \brief CRC 控制寄存器 */
    __IO uint32_t crcres;     /**< \brief CRC 结果寄存器 */
         uint8_t  res[120];   /**< \brief 保留 */
    __IO uint32_t crcdat;     /**< \brief CRC 数据寄存器 */
} amhw_zsn700_crc_t;

/**
 * \brief CRC 32位写数据
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 * \param[in] data     : 用于校验的数据
 *
 * retval : none
 */
am_static_inline
void amhw_zsn700_crc_32bit_write_data (amhw_zsn700_crc_t    *p_hw_crc,
                                       uint32_t              data)
{
    p_hw_crc->crcdat = (p_hw_crc->crcdat & ~(0xffffffff)) | data;
}

/**
 * \brief CRC 16位写数据
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 * \param[in] data     : 用于校验的数据
 *
 * retval : none
 *
 * \note this register can not  caculate data
 */
am_static_inline
void amhw_zsn700_crc_16bit_write_data (amhw_zsn700_crc_t  *p_hw_crc,
                                       uint16_t            data)
{
    p_hw_crc->crcdat = (p_hw_crc->crcdat & ~(0xffff)) | data;
}


/**
 * \brief CRC 8位写数据
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 * \param[in] data     : 用于校验的数据
 *
 * retval : none
 *
 * \note this register can not  caculate data
 */
am_static_inline
void amhw_zsn700_crc_8bit_write_data (amhw_zsn700_crc_t *p_hw_crc, uint8_t data)
{
    p_hw_crc->crcdat = (p_hw_crc->crcdat & ~(0xff)) | data;
}

/**
 * \brief CRC 32位校验方式初始化结果寄存器值
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 *
 * \retval : none
 */
am_static_inline
void amhw_zsn700_crc_res_32data_init (amhw_zsn700_crc_t *p_hw_crc)
{
    p_hw_crc->crcres = 0xffffffff;
}

/**
 * \brief CRC 16位校验方式初始化结果寄存器值
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 *
 * \retval : none
 */
am_static_inline
void amhw_zsn700_crc_res_16data_init (amhw_zsn700_crc_t *p_hw_crc)
{
    p_hw_crc->crcres = 0xffff;
}

/**
 * \brief CRC 32位读数据
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 *
 * \retval : CRC计算结果值
 */
am_static_inline
uint32_t amhw_zsn700_crc_32bit_read_data (amhw_zsn700_crc_t *p_hw_crc)
{
    return (p_hw_crc->crcres);
}

/**
 * \brief CRC 16位读数据
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 *
 * \retval : CRC计算结果值
 */
am_static_inline
uint16_t amhw_zsn700_crc_16bit_read_data (amhw_zsn700_crc_t *p_hw_crc)
{
    return (uint16_t)(p_hw_crc->crcres);
}

/**
 * \brief CRC 校验判断
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 *
 * \retval : none
 */
am_static_inline
uint32_t amhw_zsn700_crc_judge (amhw_zsn700_crc_t *p_hw_crc)
{
    return (p_hw_crc->crccr & (0x2)) >> 1;
}

/**
 * \brief CRC 校验方式选择
 *
 * \param[in] p_hw_crc : 指向CRC寄存器组基地址
 *
 * \retval : none
 */
am_static_inline
void amhw_zsn700_crc_cal_way (amhw_zsn700_crc_t *p_hw_crc, uint32_t flag)
{
    if (flag == 1){
        p_hw_crc->crccr |= (flag << 0);
    }else{
        p_hw_crc->crccr &= (flag << 0);
    }
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG_CRC_H */

/* end of file */
