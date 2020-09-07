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
 * \brief OPA不同模式下输入输出关系例程，通过 std 层接口实现
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
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32l13x_core_std_opa.c src_hc32l13x_core_std_opa
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-17  ly, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "am_hc32x3x_opa.h"
#include "am_hc32l13x_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hc32_entries.h"
/**
 * \brief OPA通道
 */
#define OPA_CH    AM_HC32_OPA_CH1

/**
 * \brief 例程入口
 */
void demo_hc32l13x_core_std_opa_entry (void)
{
    AM_DBG_INFO("demo aml13x_core std opa  test!\r\n");

    /* 底层文件需要配置 模式、增益等参数*/
    demo_hc32x3x_drv_opa_entry(am_hc32_opa1_inst_init(), OPA_CH);
}
