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
 * \brief RESET
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-18  yrz, first implementation
 * \endinternal
 */
 
#include "hw/amhw_zsn700_rcc_reset.h"

/* 使某些外设复位或正常工作 */
static void __reset_cmd(amhw_zsn700_rcc_reset_peripheral peri, uint8_t cmd)
{
    if(peri > 31) {

        if (cmd) {
            ZSN700_RCC_RESET->reset[1] |= (1ul << (peri - 31));
        } else {
            ZSN700_RCC_RESET->reset[1] &= ~(1ul << (peri - 31));
        }
    } else {

        if (cmd) {
            ZSN700_RCC_RESET->reset[0] |= (1ul << peri);
        } else {
            ZSN700_RCC_RESET->reset[0] &= ~(1ul << peri);
        }
    }
}

/**
 * \brief 使某些外设复位
 *
 * \param[in]  peri ：需要复位的外设
 *
 * \return 无
 *
 */
void amhw_zsn700_rcc_reset (amhw_zsn700_rcc_reset_peripheral peri)
{
    __reset_cmd(peri, 0);
}

/**
 * \brief 使某些外设正常工作
 *
 * \param[in]  peri ：需要正常工作的外设
 *
 * \return 无
 *
 */
void amhw_zsn700_rcc_reset_stop (amhw_zsn700_rcc_reset_peripheral peri)
{
    __reset_cmd(peri, 1);
}

/* end of file */
