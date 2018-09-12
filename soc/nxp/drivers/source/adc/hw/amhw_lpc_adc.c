/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2012 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC implementation for operating hardware.
 *
 * - 12-bit successive approximation analogy to digital converter.
 * - Input multiplexing among up to 12 pins.
 * - Two configurable conversion sequences with independent triggers.
 * - Optional automatic high/low threshold comparison and zero crossing detection.
 * - Measurement range VREFNto VREFP(typically 3 V; not to exceed VDDAvoltage level).
 * - 12-bit conversion rate of 4.8 MHz. Options for reduced resolution at higher conversion
 *   rates.
 * - Burst conversion mode for single or multiple inputs.
 * - Synchronous or asynchronous operation. Asynchronous operation maximizes
 *   flexibility in choosing the ADC clock frequency, Synchronous mode minimizes trigger
 *   latency and can eliminate uncertainty and jitter in response to a trigger.
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-21  hbt, first implementation.
 * \endinternal
 */

#include "hw/amhw_lpc_adc.h"


/******************************************************************************
* Public functions
*******************************************************************************/

/**
 * \brief ADC start up
 */
int amhw_lpc_adc_startup (amhw_lpc_adc_t *p_hw_adc)
{
    volatile int i;
    
    amhw_lpc_adc_startup_set(p_hw_adc, AMHW_LPC_ADC_STARTUP_ENA);
    
    for (i = 0; i < 0x10; i++) {}

    amhw_lpc_adc_startup_set(p_hw_adc, AMHW_LPC_ADC_STARTUP_ENA | 
                                   AMHW_LPC_ADC_STARTUP_INIT);

    i = 0x7FFFF;
    while ((amhw_lpc_adc_startup_get(p_hw_adc) & AMHW_LPC_ADC_STARTUP_INIT) && --i );
        
    return i ? AM_OK : -AM_ETIME;
}

/**
 * \brief the ADC calibrate
 */
void amhw_lpc_adc_calibrate (amhw_lpc_adc_t *p_hw_adc, uint32_t sysclk_freq)
{
    int i;
    
    /* 
     * if no calibration has been, do the calibrate 
     */
    if (amhw_lpc_adc_calib_isrun(p_hw_adc)) {
        uint32_t ctrl = amhw_lpc_adc_ctrl_get(p_hw_adc) & (0x00007FFF);
        uint32_t tmp  = ctrl;

        /* Set ADC to SYNC mode */
        tmp &= ~(1 << 8);
        
        /* To be safe run calibration at 1MHz UM permits up to 30MHz */
        if (sysclk_freq > 1000000UL) {
            tmp |= (((sysclk_freq / 1000000UL) - 1) & 0xFF);
            amhw_lpc_adc_config(p_hw_adc, tmp);
        }

        /* Calibration is needed, do it now. */
        amhw_lpc_adc_calib_enable(p_hw_adc);
        
        i = 0xF0000;
        while (amhw_lpc_adc_calib_get(p_hw_adc) && --i);
        
        amhw_lpc_adc_config(p_hw_adc, ctrl);
    }
}

/* end of file */
