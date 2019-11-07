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
 * - 1.00 19-09-05  zp, first implementation
 * \endinternal
 */
 
#include "hw/amhw_hc32_rcc_reset.h"

/* 使某些外设复位或正常工作 */
static void __reset_cmd(amhw_hc32_rcc_reset_peripheral peri, uint8_t cmd)
{
    if(peri > 31) {

        if (cmd) {
            HC32_RCC_RESET->reset[1] |= (1ul << (peri - 31));
        } else {
            HC32_RCC_RESET->reset[1] &= ~(1ul << (peri - 31));
        }
    } else {

        if (cmd) {
            HC32_RCC_RESET->reset[0] |= (1ul << peri);
        } else {
            HC32_RCC_RESET->reset[0] &= ~(1ul << peri);
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
void amhw_hc32_rcc_reset (amhw_hc32_rcc_reset_peripheral peri)
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
void amhw_hc32_rcc_reset_stop (amhw_hc32_rcc_reset_peripheral peri)
{
    __reset_cmd(peri, 1);
}

/* end of file */
