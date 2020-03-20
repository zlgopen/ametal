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
 * \brief OPA不同模式下输入输出关系例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *    OPA_UintMode_Test
 *    此时通过示波器观察PC06和PC07信号，PC07输出信号是PC06信号通信幅值是一致的。
 *    OPA_ForWardMode_Test
 *    此时通过示波器观察PC06和PC07信号，PC07输出信号是PC06信号通信幅值是其两倍。
 *    OPA_GpMode_Test
 *    PC06输入VCOR1.5V，PB15和PC07接电阻22K，PB15接电阻10K对地，
 *    此时通过示波器观察PC06和PC07信号，PC07输出信号是PC06信号通信幅值是其两倍。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32_hw_opa_one.c src_hc32_hw_opa_one
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-16  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32_hw_opa_one
 * \copydoc demo_hc32_hw_opa_one.c
 */

/** [src_hc32_hw_opa_one] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_hc32x3x_opa.h"
#include "hw/amhw_hc32x3x_opa.h"

/**
 * \brief 例程入口
 */
void demo_hc32x3x_hw_opa_entry (void *p_hw_opa, uint8_t mode, uint8_t ch)
{

    /* 使能BGR */
    amhw_hc32_bgr_enable(AM_TRUE);

    /* 模式配置 */
    switch (mode){
        case AM_HC32_OPA_MODE_UNITY_GAIN:
            amhw_hc32_opa_unity_gain_mode(p_hw_opa, ch);
            amhw_hc32_opa_po_ctrl (p_hw_opa, ch, AWHW_HC32_OPA_PO_EN);
            break;
        case AM_HC32_OPA_MODE_FORWARD_IN:
            amhw_hc32_opa_forwar_in_mode(p_hw_opa,
                                         ch,
                                         AMHW_HC32_OPA_NONGAIN_2);
            amhw_hc32_opa_po_ctrl (p_hw_opa, ch, AWHW_HC32_OPA_PO_EN);
            break;
        case AM_HC32_OPA_MODE_UNIVERSAL:
            amhw_hc32_uinversal_mode (p_hw_opa,
                                      ch);
            amhw_hc32_opa_po_ctrl (p_hw_opa, ch, AWHW_HC32_OPA_PO_EN);
            break;
        default:
            break;
    }

    /* 使能OPA */
    amhw_hc32_opa_en(p_hw_opa);

    while(1){
        ;
    }
}

/* end of file */
