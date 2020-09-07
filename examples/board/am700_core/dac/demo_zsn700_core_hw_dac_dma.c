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
 * \brief DAC转换 DMA传输数据例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   1. 串口打印出 "DAC DMA transfer done!"。
 *   2. 打印出当前DAC输出电压值。(输出电压类似为一个正弦波，波谷值0mv 、波峰值 3300 mv)
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_dac_dma.c src_zsn700_hw_dac_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_dac_dma
 * \copydoc demo_zsn700_hw_dac_dma.c
 */

/** [src_zsn700_hw_dac_dma] */
#include "ametal.h"
#include "am_zsn700.h"
#include "demo_zlg_entries.h"
#include "am_zsn700_inst_init.h"
#include "demo_am700_core_entries.h"
#include "hw/amhw_zsn700_dac.h"
#include "math.h"

#define PI 3.14159265358979           //圆周率

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
void demo_zsn700_core_hw_dac_dma_entry (void)
{
    AM_DBG_INFO("demo am700_core hw dac dma test!\r\n");

    /* 配置PIOA_4为DAC1_OUT功能 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_AOUT);

    /* 开启DAC时钟 */
    am_clk_enable(CLK_DAC);

    _BufProduce(vol_val);

    demo_zsn700_hw_dac_dma_entry(ZSN700_DAC,
                                 ZSN700_DMA_SRC_TYPE_DAC0,
                                 DMA_CHAN_1,
                                 vol_val);

}

/* end of file */
