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
 * \brief ADC implementation
 * 
 * \internal
 * \par Modification history
 * - 1.02 18-04-12  zcb, del am_adc_read_mv() am_adc_val_mv().
 * - 1.01 15-11-26  hgo, modify data align.
 * - 1.00 15-04-27  tee, first implementation.
 * \endinternal
 */
#include "am_adc.h"
#include "am_wait.h"

/*******************************************************************************/

volatile static int  g_adc_satus = 0;

/* adc转换回调函数 */
static void adc_convert_cb (void *p_arg, int state)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;
    
    g_adc_satus = state;
    
    am_wait_done(p_wait);
}

/*******************************************************************************/

int am_adc_read (am_adc_handle_t  handle, 
                 int              chan, 
                 void            *p_val,
                 uint32_t         length)
{
    
    am_adc_buf_desc_t desc;
    am_wait_t  wait;
    
    am_wait_init(&wait);

    am_adc_mkbufdesc(&desc, p_val, length, adc_convert_cb,(void *) &wait);
 
    am_adc_start(handle, chan, &desc, 1, 1, AM_ADC_DATA_ALIGN_RIGHT, NULL, NULL);
    
    am_wait_on(&wait);
    
    return g_adc_satus;
}

/* end of file */
