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
 * \brief OPA 电压跟随例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PB00输入模拟电压。对应OPA输出引脚PA04也输出与PB00大小相等电压。
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
 * - 1.00 19-10-10  ly, first implementation
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
#include "am_hc32_opa.h"
#include "hw/amhw_hc32_opa.h"

/** \brief 校准脉宽 */
#define __OPA_CLK        AMHW_HC32_OPA_CLK_16

/** \brief 校准方式 */
#define __OPA_AZ_WAY     AMHW_HC32_OPA_AZ_SW

/**
 * \brief OPA初始化
 */
static void __opa_general_purpose_init (amhw_hc32_opa_t *p_hw_opa)
{
    /* 使能 OPA */
    amhw_hc32_opa_en(p_hw_opa);

    /* 自动校准使能 */
    amhw_hc32_opa_az_en(p_hw_opa);

    /* 自动校准脉冲宽度设置 */
    amhw_hc32_opa_clk_sel(p_hw_opa, __OPA_CLK);

    /* 选择校准方式 */
    amhw_hc32_opa_az_way_sel(p_hw_opa, __OPA_AZ_WAY);
}

/**
 * \brief OPA 通用运算放大器模式，通过 HW 层接口实现
 */
void demo_hc32_hw_opa_entry (void *p_hw_opa)
{
    /* 使能BGR */
    amhw_hc32_bgr_enable(AM_TRUE);

    /* OPA初始化 */
    __opa_general_purpose_init((amhw_hc32_opa_t *)p_hw_opa);

    /* 启动校准 */
    amhw_hc32_az_start (p_hw_opa, AMHW_HC32_AZ_SW_START);

    /* 延时 20us*/
    am_udelay(20);

    while (1) {
        ;
    }
}

/* end of file */
