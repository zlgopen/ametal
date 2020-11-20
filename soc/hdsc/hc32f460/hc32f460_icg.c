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
 * \brief HC32F460 ≥ı ºªØ≈‰÷√£®ICG£©
 *
 * \internal
 * \par Modification History
 * - 1.00 20-06-02  cds, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "hc32f460_icg.h"

#ifndef APP

#if defined (__CC_ARM)
const uint32_t ICG[8] __attribute__((at(0x400))) =
#elif defined(__GNUC__)
__attribute__ (( section(".icg")))
const uint32_t ICG[8] =
#else
#error "unsupported compiler!!"
#endif
{
    /** \brief ICG 0~ 3 */
    HC32F460_ICG0_REGISTER_CONSTANT,
    HC32F460_ICG1_REGISTER_CONSTANT,
    HC32F460_ICG2_REGISTER_CONSTANT,
    HC32F460_ICG3_REGISTER_CONSTANT,
    /** \brief ICG 4~ 7 */
    HC32F460_ICG4_REGISTER_CONSTANT,
    HC32F460_ICG5_REGISTER_CONSTANT,
    HC32F460_ICG6_REGISTER_CONSTANT,
    HC32F460_ICG7_REGISTER_CONSTANT,
};
#endif
/* end of file */
