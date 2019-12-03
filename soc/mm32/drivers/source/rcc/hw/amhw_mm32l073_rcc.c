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
 * 驱动直接使用了 MM32L073 外设寄存器基地址， 因此，该驱动仅适用于 MM32L073
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-17  sdy, first implementation
 * \endinternal
 */
#include "hw/amhw_mm32l073_rcc.h"

/* 使能或禁能外设 */
static void __rcc_ahb_cmd (amhw_mm32l073_ahb_peripheral peri, uint8_t cmd)
{
    uint32_t tempa = 0;
    uint32_t tempb = 0;

    tempa = MM32L073_RCC->ahbenr;
    tempb = 0x0f000000 & tempa;
    tempb = tempb >> 7;

    tempa = 0x000000ff & tempa;
    tempa = tempa | tempb;

    if (cmd) {
        tempa = (1ul << peri) | tempa;
    } else {
        tempa = (~(1ul << peri)) & tempa;
    }

    MM32L073_RCC->ahbenr = tempa;
}

/**
 * \brief 使能AHB外设
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 无
 *
 */
void amhw_mm32l073_rcc_ahb_enable (amhw_mm32l073_ahb_peripheral peri)
{
    __rcc_ahb_cmd(peri, 1);
}

/**
 * \brief 禁能AHB外设
 *
 * \param[in]  peri ：需要禁能的外设
 *
 * \return 无
 *
 */
void amhw_mm32l073_rcc_ahb_disable (amhw_mm32l073_ahb_peripheral peri)
{
    __rcc_ahb_cmd(peri, 0);
}

/* end of file */
