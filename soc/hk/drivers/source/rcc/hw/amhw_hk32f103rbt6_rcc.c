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
 * - 1.00 17-04-17  sdy, first implementation
 * \endinternal
 */
 
#include <amhw_hk32f103rbt6_rcc.h>

/* 使能或禁能外设 */
static void __rcc_ahb_cmd(amhw_hk32f103rbt6_ahb_peripheral peri, uint8_t cmd)
{
    uint32_t temp = 0;

    temp = HK32F103RBT6_RCC->ahbenr;

    temp = 0x000000ff & temp;

    if (cmd) {
        temp = (1ul << peri) | temp;
    } else {
        temp = (~(1ul << peri)) & temp;
    }

    HK32F103RBT6_RCC->ahbenr = temp;
}

/**
 * \brief 使能AHB外设
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 无
 *
 */
void amhw_hk32f103rbt6_rcc_ahb_enable (amhw_hk32f103rbt6_ahb_peripheral peri)
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
void amhw_hk32f103rbt6_rcc_ahb_disable (amhw_hk32f103rbt6_ahb_peripheral peri)
{

    __rcc_ahb_cmd(peri, 0);
}

/* end of file */
