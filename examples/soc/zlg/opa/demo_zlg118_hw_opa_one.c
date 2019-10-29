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
 * \snippet demo_zlg118_hw_opa_one.c src_zlg118_hw_opa_one
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-10  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_opa_one
 * \copydoc demo_zlg118_hw_opa_one.c
 */

/** [src_zlg118_hw_opa_one] */
#include "ametal.h"
#include "am_zlg118.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_zlg118_opa.h"
#include "hw/amhw_zlg118_opa.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define OPA_CLK      AMHW_ZLG118_OPA_CLK_16    /**< \brief 校准脉宽 */
#define OPA_AZ_WAY   AMHW_ZLG118_OPA_AZ_SW     /**< \brief 校准方式 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zlg118_opa_t       *gp_hw_opa   = NULL;  /**< \brief OPA 外设 */

/**
 * \brief OPA初始化
 */
void opa_init (uint8_t mode)
{
    if (mode == AM_ZLG118_OPA_GENERAL){
        /* 使能opa */
        amhw_zlg118_opa_en (gp_hw_opa);

        /* 自动校准使能 */
        amhw_zlg118_opa_az_en (gp_hw_opa);

        /* 自动校准脉冲宽度设置 */
        amhw_zlg118_opa_clk_sel (gp_hw_opa, OPA_CLK);

        /* 选择校准方式 */
        amhw_zlg118_opa_az_way_sel (gp_hw_opa, OPA_AZ_WAY);
    }else{
        ;
    }
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_opa_one_entry (void *p_hw_opa, uint8_t mode)
{
    gp_hw_opa  = (amhw_zlg118_opa_t *)p_hw_opa;

    /* 使能BGR */
    amhw_zlg118_bgr_enable(AM_TRUE);

    /* OPA初始化 */
    opa_init (mode);

    /* 启动校准 */
    amhw_zlg118_az_start (p_hw_opa,AMHW_ZLG118_AZ_SW_START);

    /* 延时 20us*/
    am_udelay(20);

    while (1)
    {
        ;
    }
}

/* end of file */
