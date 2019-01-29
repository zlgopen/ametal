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
 * \brief SWM (开关矩阵) 硬件操作实现
 * 
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  mem ,modified.
 * - 1.01 15-07-28  zxl ,modified.
 * - 1.00 15-07-08  liqing, first implementation.
 * \endinternal
 */

#include <am_lpc82x.h>
#include "ametal.h"
#include "hw/amhw_lpc82x_swm.h"

#define AMHW_LPC82X_SWM_NO_FIXED_FUNC    30   /**< \brief 引脚没有特殊功能   */
#define AMHW_LPC82X_SWM_MULTI_FIXED_FUNC 255  /**< \brief 引脚有多个特殊功能 */

/*
 * LPC8XX PINENABLE 引脚特殊功能号偏移表
 * amhw_lpc82x_swm_fixedfunc_offset_pin[PIO0_5]: 
 *                                           PIO0_5引脚特殊功能在寄存器中的位偏移
 * 注：AMHW_LPC82X_SWM_NO_FIXED_FUNC           ：引脚没有特殊功能
 *     AMHW_LPC82X_SWM_MULTI_FIXED_FUNC        ：引脚有多个特殊功能
 */
const static uint8_t amhw_lpc82x_swm_fixedfunc_offset_pin[]=
{
    AMHW_LPC82X_SWM_PIO0_0_ACMP_I1,
    AMHW_LPC82X_SWM_MULTI_FIXED_FUNC,
    AMHW_LPC82X_SWM_PIO0_2_SWDIO,
    AMHW_LPC82X_SWM_PIO0_3_SWCLK,
    AMHW_LPC82X_SWM_PIO0_4_ADC_11,
    AMHW_LPC82X_SWM_PIO0_5_RESETN,
    AMHW_LPC82X_SWM_MULTI_FIXED_FUNC,
    AMHW_LPC82X_SWM_PIO0_7_ADC_0,
    AMHW_LPC82X_SWM_PIO0_8_XTALIN,
    AMHW_LPC82X_SWM_PIO0_9_XTALOUT,
    AMHW_LPC82X_SWM_PIO0_10_I2C0_SCL,
    AMHW_LPC82X_SWM_PIO0_11_I2C0_SDA,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC,
    AMHW_LPC82X_SWM_PIO0_13_ADC_10,
    AMHW_LPC82X_SWM_MULTI_FIXED_FUNC,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC,
    AMHW_LPC82X_SWM_PIO0_17_ADC_9,
    AMHW_LPC82X_SWM_PIO0_18_ADC_8,
    AMHW_LPC82X_SWM_PIO0_19_ADC_7,
    AMHW_LPC82X_SWM_PIO0_20_ADC_6,
    AMHW_LPC82X_SWM_PIO0_21_ADC_5,
    AMHW_LPC82X_SWM_PIO0_22_ADC_4,
    AMHW_LPC82X_SWM_MULTI_FIXED_FUNC,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC,
    AMHW_LPC82X_SWM_NO_FIXED_FUNC
};

/**
 * \brief 取消引脚的特殊功能
 */
void amhw_lpc82x_swm_pin_fixed_func_disable (amhw_lpc82x_swm_t *p_hw_swm, 
                                             int                pin)
{
    /* 没有特殊功能的引脚操作无效 */
    if((pin > LPC82X_PIN_NUM) ||
       (amhw_lpc82x_swm_fixedfunc_offset_pin[pin] == 
                                               AMHW_LPC82X_SWM_NO_FIXED_FUNC)) {
        return;
    }

    switch(pin) {
        
    /* 两个功能：PIO0_1_ACMP_I2，PIO0_1_CLKIN */
    case PIO0_1: 
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_1_ACMP_I2);
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_1_CLKIN);
        break;
    
    /* 两个功能：PIO0_6_VDDCMP，PIO0_6_ADC_1 */
    case PIO0_6:
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_6_VDDCMP);
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_6_ADC_1);
        break;
    
    /* 两个功能：PIO0_14_ACMP_I3，PIO0_14_ADC_2 */
    case PIO0_14:
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_14_ACMP_I3);
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_14_ADC_2);
        break;

    /* 两个功能：PIO0_23_ACMP_I4，PIO0_23_ADC_3 */
    case PIO0_23:
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_23_ACMP_I4);
        amhw_lpc82x_swm_fixed_func_disable(p_hw_swm, 
                                           AMHW_LPC82X_SWM_PIO0_23_ADC_3);
        break;
    
    /* 其它引脚查表 */
    default:    
        amhw_lpc82x_swm_fixed_func_disable(
        p_hw_swm,
        (amhw_lpc82x_swm_fixed_func_t)amhw_lpc82x_swm_fixedfunc_offset_pin[pin]);
        break;
    }
}

/**
 * \brief 使能引脚的特殊功能
 */
void amhw_lpc82x_swm_pin_fixed_func_enable (amhw_lpc82x_swm_t *p_hw_swm,
                                            int                pin,
                                            uint32_t           func)
{
    /* 没有特殊功能的引脚操作无效 */
    if((pin > LPC82X_PIN_NUM) ||
       (amhw_lpc82x_swm_fixedfunc_offset_pin[pin] ==
                                               AMHW_LPC82X_SWM_NO_FIXED_FUNC)) {
        return;
    }

    switch(pin) {
     
    /* 两个功能：PIO0_1_ACMP_I2，PIO0_1_CLKIN */
    case PIO0_1: 
        if (func == PIO0_1_ACMP_I2) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_1_ACMP_I2);
        }
        else if (func == PIO0_1_CLKIN) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_1_CLKIN);
        }
        break;
    
    /* 两个功能：PIO0_6_VDDCMP，PIO0_6_ADC_1 */
    case PIO0_6:
        if (func == PIO0_6_VDDCMP) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_6_VDDCMP);
        }
        else if (func == PIO0_6_ADC_1) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_6_ADC_1);
        }
        break;
        
    /* 两个功能：PIO0_14_ACMP_I3，PIO0_14_ADC_2 */
    case PIO0_14:
        if (func == PIO0_14_ACMP_I3) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_14_ACMP_I3);
        }
        else if (func == PIO0_14_ADC_2) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_14_ADC_2);
        }
        break;
        
    /* 两个功能：PIO0_23_ACMP_I4，PIO0_23_ADC_3 */
    case PIO0_23:
        if (func == PIO0_23_ACMP_I4) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_23_ACMP_I4);
        }
        else if (func == PIO0_23_ADC_3) {
            amhw_lpc82x_swm_fixed_func_enable(p_hw_swm, 
                                              AMHW_LPC82X_SWM_PIO0_23_ADC_3);
        }
        break;
    
    /* 其它引脚查表 */
    default:
        amhw_lpc82x_swm_fixed_func_enable(
        p_hw_swm,
        (amhw_lpc82x_swm_fixed_func_t)amhw_lpc82x_swm_fixedfunc_offset_pin[pin]);
        break;
    }
}

/* end of file */
