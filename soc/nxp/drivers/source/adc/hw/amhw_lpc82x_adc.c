/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief ADC 硬件操作接口实现
 *
 * 1. 12位逐次逼近型数模转换器;
 * 2. 模拟输入引脚多达12个；
 * 3. 2个独立触发的转换序列；
 * 4. 可选择高/低阈值比较和“零点穿越”监测；
 * 5. 支持掉电模式和低功耗模式；
 * 6. 电压测量范围从 Vrefn 到 Vrefp (典型值为 3V; 不可超出 VDD 的电压值)；
 * 7. 12位的转换速率高达 1.2 MHz采样速率；
 * 8. 突发转换模式支持单通道或多通道输入；
 * 9. 支持DMA模式；
 * 10. 硬件校正模式。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-25  hgo, add annotation.
 * - 1.00 15-07-15  bob, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "hw/amhw_lpc82x_adc.h"

/******************************************************************************
* Public functions
*******************************************************************************/

/**
 * \brief ADC 校验
 */
 
void amhw_lpc82x_adc_calibrate (amhw_lpc82x_adc_t *p_hw_adc,
                                uint32_t           sysclk_freq)
{
    uint32_t ctrl = amhw_lpc82x_adc_ctrl_get(p_hw_adc);
    uint32_t tmp = ctrl;
  
    if (0 == (ctrl & AMHW_LPC82X_ADC_CTRL_CALMODE)) {

         /* 矫正模式位域分频系数一起写进，矫正频率为500KHz */
        ctrl |= (AMHW_LPC82X_ADC_CTRL_CALMODE | (sysclk_freq / 500000 - 1));
        amhw_lpc82x_adc_config(p_hw_adc, ctrl);
      
        /* 直到校验完成 */
        while(amhw_lpc82x_adc_ctrl_get(p_hw_adc) &
              AMHW_LPC82X_ADC_CTRL_CALMODE);

        amhw_lpc82x_adc_config(p_hw_adc, tmp);
    }
}

/* end of file */
