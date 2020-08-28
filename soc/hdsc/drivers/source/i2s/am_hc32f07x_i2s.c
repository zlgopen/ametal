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
 * \brief  I2S硬件层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-27  mkr, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f07x_i2s.h"
#include "am_vdebug.h"

/**
 * \brief I2S 初始化
 */
void am_hc32f07x_i2s_init (amhw_hc32f07x_i2s_t *p_hw_i2s,
                           am_hc32f07x_i2s_dev_t *p_dev)
{
    int src = 0;
    uint8_t div=0;
    uint8_t odd=0;
    uint8_t fract=0;
    uint8_t chlen = 0;
	

    if(p_dev->data_len  == 0){
        chlen = 16;
    }else {
        chlen = 32;
    }

    src = p_dev->fre_mclk / p_dev->fshz;
    if(p_dev->mclk_en == AM_FALSE) {       
        div = src/(chlen*4);
        src = src-src/(chlen*4);
        fract = src * (64 / (chlen * 4));

    } else {
        div = src / (64*8);
        src = src - src / (64 *8);
        fract = src / 8;
    }

    /* 设置模式 */
    amhw_hc32f07x_i2s_mode_sel(p_hw_i2s, p_dev->mode);	

    /* 主模式时钟选择 */
    amhw_hc32f07x_i2s_clk(p_hw_i2s, p_dev->clk_sel);

    /* 标准选择 */
    amhw_hc32f07x_i2s_std(p_hw_i2s, p_dev->std);

    /* 传输数据长度设置 */
    amhw_hc32f07x_i2s_data_length(p_hw_i2s, p_dev->data_len);

    /* 通道长度设置 */
    if(chlen == 16){
        amhw_hc32f07x_i2s_ch_length(p_hw_i2s, AMHW_HC32F07X_I2S_CH_LEN16);
    }else {
        amhw_hc32f07x_i2s_ch_length(p_hw_i2s, AMHW_HC32F07X_I2S_CH_LEN32);
    }

    if(p_dev->mclk_en){

        /* 主时钟输出使能 */
        amhw_hc32f07x_i2s_clk_enable(p_hw_i2s);
    } else {

        /* 主时钟输出禁能 */
        amhw_hc32f07x_i2s_clk_disable(p_hw_i2s);
    }

        /* 分频器设置 */
    amhw_hc32f07x_i2s_div_set (p_hw_i2s, div);
    amhw_hc32f07x_i2s_fract_set(p_hw_i2s, fract);		
    if(odd){
        amhw_hc32f07x_i2s_odd_enable(p_hw_i2s);
    } else {
        amhw_hc32f07x_i2s_odd_disable(p_hw_i2s);
    }
    amhw_hc32f07x_i2s_tx_empty_enable (p_hw_i2s);
    amhw_hc32f07x_i2s_enable (p_hw_i2s);

}

