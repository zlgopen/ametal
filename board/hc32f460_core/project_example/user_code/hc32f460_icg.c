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
 * \brief ICG≈‰÷√
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-24  cds, first implementation
 * \endinternal
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f460_icg.h"

const uint32_t u32ICG[] __attribute__((section(".icg"))) =
{
    /* ICG 0~ 3 */
    ICG0_REGISTER_CONSTANT,
    ICG1_REGISTER_CONSTANT,
    ICG2_REGISTER_CONSTANT,
    ICG3_REGISTER_CONSTANT,
    /* ICG 4~ 7 */
    ICG4_REGISTER_CONSTANT,
    ICG5_REGISTER_CONSTANT,
    ICG6_REGISTER_CONSTANT,
    ICG7_REGISTER_CONSTANT,
};




/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
