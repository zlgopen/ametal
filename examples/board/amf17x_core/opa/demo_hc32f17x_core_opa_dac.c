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
 * \brief OPA DAC电压跟随例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   DAC输出正弦波电压。对应OPA输出引脚PA04大小相等的正弦波电压。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_core_opa_dac.c src_hc32f17x_core_opa_dac
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-10  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_core_opa_dac
 * \copydoc demo_hc32f17x_core_opa_dac.c
 */

/** [src_hc32f17x_core_opa_dac] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32_pin.h"
#include "am_hc32_opa.h"
#include "demo_hc32_entries.h"
#include "math.h"

#define PI          3.14159265358979     /**< \brief圆周率 */
#define OPA_MODE    AM_HC32_OPA_DAC    /**< \brief OPA DAC电压跟随模式*/

/**< \brief DAC设置电压值缓存区  */
static uint16_t vol_val[128] = {0};

/*******************************************************************************
** \brief  产生正弦波的采样点
** \param  buf:用于存放正弦波采样点的缓存
** \return 无
******************************************************************************/
static void _BufProduce(uint16_t* vol_val)
{
   uint8_t tmp;
   double tmp1;
   tmp1=PI/64;
   for(tmp=0; tmp<128; tmp++)
   {
       vol_val[tmp] =(uint16_t)(((sin(tmp1*tmp))*2047)+2048);
   }
}

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_opa_dac_entry (void)
{

    AM_DBG_INFO("demo amf17x_core hw opa dac test!\r\n");

    /* 开启DAC时钟 */
    am_clk_enable(CLK_DAC);

    /* 开启OPA时钟 */
    am_clk_enable (CLK_OPA);

    /* 开启BGR时钟 */
    am_clk_enable (CLK_ADC_BGR);

    _BufProduce(vol_val);

    demo_hc32_hw_opa_dac_entry(HC32_OPA,
                                 HC32_DAC,
                                 OPA_MODE,
                                 vol_val,
                                 128);
}

/* end of file */
