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
 * \brief RCC
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-18  yrz, first implementation
 * \endinternal
 */
 
#include "hw/amhw_zsn700_rcc.h"

/* 使能或禁能外设 */
static void __rcc_cmd(amhw_zsn700_peripheral peri, uint8_t cmd)
{
    if(peri > 31) {

        if (cmd) {
            ZSN700_RCCCTRL->peri_clken[1] |= (1ul << (peri - 31));
        } else {
            ZSN700_RCCCTRL->peri_clken[1] &= ~(1ul << (peri - 31));
        }
    } else {

        if (cmd) {
            ZSN700_RCCCTRL->peri_clken[0] |= (1ul << (peri));
        } else {
            ZSN700_RCCCTRL->peri_clken[0] &= ~(1ul << (peri));
        }
    }
}

/**
 * \brief 使能AHB外设
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 无
 *
 */
void amhw_zsn700_rcc_peripheral_enable (amhw_zsn700_peripheral peri)
{
    __rcc_cmd(peri, 1);
}

/**
 * \brief 检查某外设时钟是否使能
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 1: 当前外设时钟已使能，无需再次使能
 *         0: 当前外设时钟未使能
 *
 */
int amhw_zsn700_rcc_peripheral_enable_check (amhw_zsn700_peripheral peri)
{
    if(peri > 31) {
        return ((ZSN700_RCCCTRL->peri_clken[1] >> (peri - 31)) & 0x01);
    } else {
        return ((ZSN700_RCCCTRL->peri_clken[0] >> peri) & 0x01);
    }
}


/**
 * \brief 禁能AHB外设
 *
 * \param[in]  peri ：需要禁能的外设
 *
 * \return 无
 *
 */
void amhw_zsn700_rcc_peripheral_disable (amhw_zsn700_peripheral peri)
{
    __rcc_cmd(peri, 0);
}

/* end of file */
