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
 * - 1.00 19-12-23  cds, first implementation
 * \endinternal
 */

#include "am_common.h"
#include "hw/amhw_hc32f460_rcc.h"

/* 外设功能使能位检查 */
int amhw_hc32f460_rcc_peripheral_enable_check(amhw_hc32f460_peripheral peri)
{
    uint32_t num = 0;
    uint32_t peri_shift = 0;

    num = peri / 32;
    peri_shift = peri % 32;

    switch (num) {

    case 0:
        return ((HC32F460_MSTP->FCG0 >> peri_shift) & (0x01ul));
    case 1:
        return ((HC32F460_MSTP->FCG1 >> peri_shift) & (0x01ul));
    case 2:
        return ((HC32F460_MSTP->FCG2 >> peri_shift) & (0x01ul));
    case 3:
        return ((HC32F460_MSTP->FCG3 >> peri_shift) & (0x01ul));
    default :
        return AM_ERROR;
    }
}

/* 外设功能使能 */
void amhw_hc32f460_rcc_peripheral_enable(amhw_hc32f460_peripheral peri)
{
    uint32_t num = 0;
    uint32_t peri_shift = 0;

    num = peri / 32;
    peri_shift = peri % 32;

    switch (num) {

    case 0:
        amhw_hc32f460_clk_fcg0_reg_write_enable();       
        HC32F460_MSTP->FCG0 = (HC32F460_MSTP->FCG0 & (~(0x01 << peri_shift)));
        amhw_hc32f460_clk_fcg0_reg_write_disable();      
        break;
    case 1:
        HC32F460_MSTP->FCG1 = (HC32F460_MSTP->FCG1 & (~(0x01 << peri_shift)));
        break;
    case 2:
        HC32F460_MSTP->FCG2 = (HC32F460_MSTP->FCG2 & (~(0x01 << peri_shift)));
        break;
    case 3:
        HC32F460_MSTP->FCG3 = (HC32F460_MSTP->FCG3 & (~(0x01 << peri_shift)));
        break;
    default :
        break;
    }
}

/* 外设功能禁能 */
void amhw_hc32f460_rcc_peripheral_disable(amhw_hc32f460_peripheral peri)
{
    uint32_t num = 0;
    uint32_t peri_shift = 0;

    num = peri / 32;
    peri_shift = peri % 32;

    switch (num) {

    case 0:
        amhw_hc32f460_clk_fcg0_reg_write_enable();        
        HC32F460_MSTP->FCG0 = (HC32F460_MSTP->FCG0 | ((0x01ul) << peri_shift));
        amhw_hc32f460_clk_fcg0_reg_write_disable();        
        break;
    case 1:
        HC32F460_MSTP->FCG1 = (HC32F460_MSTP->FCG1 | ((0x01ul) << peri_shift));
        break;
    case 2:
        HC32F460_MSTP->FCG2 = (HC32F460_MSTP->FCG2 | ((0x01ul) << peri_shift));
        break;
    case 3:
        HC32F460_MSTP->FCG3 = (HC32F460_MSTP->FCG3 | ((0x01ul) << peri_shift));
        break;
    default :
        break;
    }
}

/* end of file */
