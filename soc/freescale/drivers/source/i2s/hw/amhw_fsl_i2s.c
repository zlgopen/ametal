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

#include "hw/amhw_fsl_i2s.h"

/**
 * \brief I2S主时钟分频配置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] mclk     : i2s主时钟频率
 * \param[in] src_clk  : 主时钟使用的时钟源频率
 *
 * \return  无
 */
void amhw_fsl_i2s_mclk_divider_cfg (amhw_fsl_i2s_t *p_hw_i2s,
                                     uint32_t         mclk,
                                     uint32_t         src_clk)
{
    uint32_t freq    = src_clk;
    uint16_t fract, divide;
    uint32_t remaind = 0;
    uint32_t current_remainder = 0xffffffff;
    uint16_t current_fract     = 0;
    uint16_t current_divide    = 0;
    uint32_t mul_freq          = 0;
    uint32_t max_fract         = 256;

    /* 防止溢出 */
    freq /= 100;
    mclk /= 100;

    max_fract = mclk * 4096 / freq + 1;

    if(max_fract > 256) {
        max_fract = 256;
    }

    /* 寻找最接近的频率 */
    for (fract = 1; fract < max_fract; fract ++) {
        mul_freq = freq * fract;
        remaind  = mul_freq % mclk;
        divide   = mul_freq / mclk;

        /* 找到正确的频率 */
        if (remaind == 0) {
            current_fract  = fract;
            current_divide = mul_freq / mclk;
            break;
        }

        /* 最靠近的或下一个 */
        if (remaind > mclk/2) {
            remaind = mclk - remaind;
            divide += 1;
        }

        /* 更新分频系数  */
        if (remaind < current_remainder) {
            current_fract = fract;
            current_divide = divide;
            current_remainder = remaind;
        }
    }

    amhw_fsl_i2s_mclk_div_set(p_hw_i2s, current_divide - 1);

    /* 等待分频器更新频率 */
    while(amhw_fsl_i2s_mclk_duf_get(p_hw_i2s)) {
    }

    amhw_fsl_i2s_mclk_fract_set(p_hw_i2s, current_fract - 1);

    /* 等待分频器更新频率 */
    while(amhw_fsl_i2s_mclk_duf_get(p_hw_i2s)) {
    }
}

/**
 * \brief I2S发送寄存器初始化
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_init (amhw_fsl_i2s_t *p_hw_i2s)
{
    /** \brief 复位I2S发送器,软件复位和FIFO复位 */
    amhw_fsl_i2s_tcsr_set(p_hw_i2s, AMHW_FSL_I2S_TCSR_SR_MASK);
    amhw_fsl_i2s_tcsr_set(p_hw_i2s, AMHW_FSL_I2S_TCSR_FR_MASK);

    /** \brief 清空I2S所有相关寄存器 */
    p_hw_i2s->tcsr = 0;
    p_hw_i2s->tcr2 = 0;
    p_hw_i2s->tcr3 = 0;
    p_hw_i2s->tcr4 = 0;
    p_hw_i2s->tcr5 = 0;
//    p_hw_i2s->tdr  = 0;
    p_hw_i2s->tmr  = 0;
}

/**
 * \brief I2S接收寄存器初始化
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \return  无
 */
void amhw_fsl_i2s_rx_init (amhw_fsl_i2s_t *p_hw_i2s)
{
    /** \brief 复位I2S接收器,软件复位和FIFO复位 */
    amhw_fsl_i2s_rcsr_set(p_hw_i2s, AMHW_FSL_I2S_RCSR_SR_MASK);
    amhw_fsl_i2s_rcsr_set(p_hw_i2s, AMHW_FSL_I2S_RCSR_FR_MASK);

    /** \brief 清空I2S所有接收相关寄存器 */
    p_hw_i2s->rcsr = 0;
    p_hw_i2s->rcr2 = 0;
    p_hw_i2s->rcr3 = 0;
    p_hw_i2s->rcr4 = 0;
    p_hw_i2s->rcr5 = 0;
    p_hw_i2s->rmr = 0;
}

/**
 * \brief I2S发协议设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_protocol_set (amhw_fsl_i2s_t          *p_hw_i2s,
                                    amhw_fsl_i2s_protocol_t  protocol)
{
    switch (protocol) {

    case AMHW_FSL_I2S_PROTOCOL_LEFT:
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_BCP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_MF_MASK);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSE_MASK);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FRSZ_MASK);
        amhw_fsl_i2s_tcr3_clr(p_hw_i2s, AMHW_FSL_I2S_TCR3_WDFL_MASK);
        break;

    case AMHW_FSL_I2S_PROTOCOL_RIGHET:
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_BCP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_MF_MASK);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSE_MASK);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FRSZ_MASK);
        amhw_fsl_i2s_tcr3_clr(p_hw_i2s, AMHW_FSL_I2S_TCR3_WDFL_MASK);
        break;

    case AMHW_FSL_I2S_PROTOCOL_TYPE:
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_BCP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_MF_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSE_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FRSZ_MASK);
        amhw_fsl_i2s_tcr3_clr(p_hw_i2s, AMHW_FSL_I2S_TCR3_WDFL_MASK);
        break;

    case AMHW_FSL_I2S_PROTOCOL_PCMA:
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_BCP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_MF_MASK);
        amhw_fsl_i2s_tx_sywd_set(p_hw_i2s, 0u);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSE_MASK);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FRSZ_MASK);
        amhw_fsl_i2s_tcr3_clr(p_hw_i2s, AMHW_FSL_I2S_TCR3_WDFL_MASK);
        break;

    case AMHW_FSL_I2S_PROTOCOL_PCMB:
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_BCP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_MF_MASK);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSE_MASK);
        amhw_fsl_i2s_tx_sywd_set(p_hw_i2s, 0u);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSP_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FRSZ_MASK);
        amhw_fsl_i2s_tcr3_clr(p_hw_i2s, AMHW_FSL_I2S_TCR3_WDFL_MASK);
        break;

    default:
        break;
    }
}

/**
 * \brief I2S接收协议设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 * \return  无
 */
void amhw_fsl_i2s_rx_protocol_set (amhw_fsl_i2s_t *p_hw_i2s, amhw_fsl_i2s_protocol_t protocol)
{
    switch (protocol)
    {
        case AMHW_FSL_I2S_PROTOCOL_LEFT:
            amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_BCP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_MF_MASK);
            amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSE_MASK);
            amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FRSZ_MASK);
            amhw_fsl_i2s_rcr3_clr(p_hw_i2s, AMHW_FSL_I2S_RCR3_WDFL_MASK);
            break;

        case AMHW_FSL_I2S_PROTOCOL_RIGHET:
            amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_BCP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_MF_MASK);
            amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSE_MASK);
            amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FRSZ_MASK);
            amhw_fsl_i2s_rcr3_clr(p_hw_i2s, AMHW_FSL_I2S_RCR3_WDFL_MASK);
            break;

        case AMHW_FSL_I2S_PROTOCOL_TYPE:
            amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_BCP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_MF_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSE_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FRSZ_MASK);
            amhw_fsl_i2s_rcr3_clr(p_hw_i2s, AMHW_FSL_I2S_RCR3_WDFL_MASK);
            break;

        case AMHW_FSL_I2S_PROTOCOL_PCMA:
            amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_BCP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_MF_MASK);
            amhw_fsl_i2s_rx_sywd_set(p_hw_i2s, 0u);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSE_MASK);
            amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FRSZ_MASK);
            amhw_fsl_i2s_rcr3_clr(p_hw_i2s, AMHW_FSL_I2S_RCR3_WDFL_MASK);
            break;

        case AMHW_FSL_I2S_PROTOCOL_PCMB:
            amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_BCP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_MF_MASK);
            amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSE_MASK);
            amhw_fsl_i2s_rx_sywd_set(p_hw_i2s, 0u);
            amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSP_MASK);
            amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FRSZ_MASK);
            amhw_fsl_i2s_rcr3_clr(p_hw_i2s, AMHW_FSL_I2S_RCR3_WDFL_MASK);
            break;

        default:
            break;
        }
}


/**
 * \brief I2S发送时钟配置
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] clk_config : i2s发送时钟配置参数
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_clock_setup (amhw_fsl_i2s_t           *p_hw_i2s,
                                  amhw_fsl_i2s_clock_cfg_t *clk_config)
{
    uint32_t bclk_div;

    bclk_div = clk_config->bclk_src_freq / clk_config->bclk;
    amhw_fsl_i2s_tx_bclk_src_set(p_hw_i2s, clk_config->bclk_src);
    amhw_fsl_i2s_tx_bclk_div_set(p_hw_i2s, (bclk_div / 2 - 1));

    /* 如果位时钟源是MCLK, 配置MCLK时钟分频 */
    if (clk_config->bclk_src == AMHW_FSL_I2S_BCLK_SRC_MCLK) {
        /* 使能 MCLK */
        amhw_fsl_i2s_mclk_enable(p_hw_i2s);

        /* 配置 MCLK 时钟源*/
        amhw_fsl_i2s_mclk_src_set(p_hw_i2s, clk_config->mclk_src);

        /* 配置 MCLK 分频器 */
        amhw_fsl_i2s_mclk_divider_cfg(p_hw_i2s,
                                      clk_config->mclk,
                                      clk_config->mclk_src_freq);
    }
}

/**
 * \brief I2S接收时钟配置
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] clk_config : i2s接收时钟配置参数
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_clock_setup (amhw_fsl_i2s_t *p_hw_i2s,
                              amhw_fsl_i2s_clock_cfg_t *clk_config)
{
    uint32_t bclk_div;

    bclk_div = clk_config->bclk_src_freq / clk_config->bclk;
    amhw_fsl_i2s_rx_bclk_src_set(p_hw_i2s, clk_config->bclk_src);
    amhw_fsl_i2s_rx_bclk_div_set(p_hw_i2s, (bclk_div / 2 - 1));

    /* 如果位时钟源是MCLK, 配置MCLK时钟分频 */
    if (clk_config->bclk_src == AMHW_FSL_I2S_BCLK_SRC_MCLK) {
        /* 使能 MCLK */
        amhw_fsl_i2s_mclk_enable(p_hw_i2s);

        /* 配置 MCLK 时钟源*/
        amhw_fsl_i2s_mclk_src_set(p_hw_i2s, clk_config->mclk_src);

        /* 配置 MCLK 分频器 */
        amhw_fsl_i2s_mclk_divider_cfg(p_hw_i2s,
                                       clk_config->mclk,
                                       clk_config->mclk_src_freq);
    }
}

/**
 * \brief I2S发送字长度配置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 * \param[in] bits     : 位长度
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_word_width_cfg (amhw_fsl_i2s_t          *p_hw_i2s,
                                      amhw_fsl_i2s_protocol_t  protocol,
                                      uint32_t                  bits)
{
    if ((protocol == AMHW_FSL_I2S_PROTOCOL_LEFT)   ||
        (protocol == AMHW_FSL_I2S_PROTOCOL_RIGHET) ||
        (protocol == AMHW_FSL_I2S_PROTOCOL_TYPE)) {
        amhw_fsl_i2s_tx_sywd_set(p_hw_i2s, bits - 1);
    }

    amhw_fsl_i2s_tx_w0w_set(p_hw_i2s, bits - 1);
    amhw_fsl_i2s_tx_wnw_set(p_hw_i2s, bits - 1);
    amhw_fsl_i2s_tx_fbt_set(p_hw_i2s, bits - 1);
}

/**
 * \brief I2S接收字长度配置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 * \param[in] bits     : 位长度
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_word_width_cfg (amhw_fsl_i2s_t          *p_hw_i2s,
                                      amhw_fsl_i2s_protocol_t  protocol,
                                      uint32_t                  bits)
{
    if ((protocol == AMHW_FSL_I2S_PROTOCOL_LEFT)   ||
        (protocol == AMHW_FSL_I2S_PROTOCOL_RIGHET) ||
        (protocol == AMHW_FSL_I2S_PROTOCOL_TYPE)) {
        amhw_fsl_i2s_rx_sywd_set(p_hw_i2s, bits - 1);
    }

    amhw_fsl_i2s_rx_w0w_set(p_hw_i2s, bits - 1);
    amhw_fsl_i2s_rx_wnw_set(p_hw_i2s, bits - 1);
    amhw_fsl_i2s_rx_fbt_set(p_hw_i2s, bits - 1);
}

/**
 * \brief I2S发送通道模式配置（单声道或立体声）
 * \param[in] p_hw_i2s     : 指向I2S寄存器块的指针
 * \param[in] channel_mode : 发送通道模式
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_channel_mode_set (amhw_fsl_i2s_t              *p_hw_i2s,
                                        amhw_fsl_i2s_channel_mode_t  channel_mode)
{
    if (channel_mode == AMHW_FSL_I2S_CHANNEL_MODE_MONO) {
        amhw_fsl_i2s_tx_mask_set(p_hw_i2s, 2);
    } else {
        amhw_fsl_i2s_tx_mask_set(p_hw_i2s, 0);
    }
}

/**
 * \brief I2S接收通道模式配置（单声道或立体声）
 * \param[in] p_hw_i2s     : 指向I2S寄存器块的指针
 * \param[in] channel_mode : 接收通道模式
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_channel_mode_set (amhw_fsl_i2s_t              *p_hw_i2s,
                                        amhw_fsl_i2s_channel_mode_t  channel_mode)
{
    if (channel_mode == AMHW_FSL_I2S_CHANNEL_MODE_MONO) {
        amhw_fsl_i2s_rx_mask_set(p_hw_i2s, 2);
    } else {
        amhw_fsl_i2s_rx_mask_set(p_hw_i2s, 0);
    }
}

/**
 * \brief I2S发送主从模式设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] tx_mode  : 发送模式
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_mode_set (amhw_fsl_i2s_t      *p_hw_i2s,
                                amhw_fsl_i2s_mode_t  tx_mode)

{
    if (tx_mode == AMHW_FSL_I2S_MODE_MASTER) {
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_BCD_MASK);
        amhw_fsl_i2s_tcr4_set(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSD_MASK);
        amhw_fsl_i2s_mclk_enable(p_hw_i2s);
    } else {
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_BCD_MASK);
        amhw_fsl_i2s_tcr4_clr(p_hw_i2s, AMHW_FSL_I2S_TCR4_FSD_MASK);
        amhw_fsl_i2s_mclk_disable(p_hw_i2s);
    }
}

/**
 * \brief I2S接收主从模式设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] rx_mode  : 接收模式
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_mode_set (amhw_fsl_i2s_t *p_hw_i2s,
                                amhw_fsl_i2s_mode_t rx_mode)

{
    if (rx_mode == AMHW_FSL_I2S_MODE_MASTER) {
        amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_BCD_MASK);
        amhw_fsl_i2s_rcr4_set(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSD_MASK);
        amhw_fsl_i2s_mclk_enable(p_hw_i2s);
    } else {
        amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_BCD_MASK);
        amhw_fsl_i2s_rcr4_clr(p_hw_i2s, AMHW_FSL_I2S_RCR4_FSD_MASK);
        amhw_fsl_i2s_mclk_disable(p_hw_i2s);
    }
}

/**
 * \brief I2S发送同步的模式配置
 * \param[in] p_hw_i2s  : 指向I2S寄存器块的指针
 * \param[in] sync_mode : 发送同步的模式
 * \return  无
 */
void amhw_fsl_i2s_tx_sync_mode_set (amhw_fsl_i2s_t           *p_hw_i2s,
                                     amhw_fsl_i2s_sync_mode_t  sync_mode)
{
    switch (sync_mode) {

    case AMHW_FSL_I2S_SYNC_DISABLED:
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_MASK);
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_DISABLE);
        break;

    case AMHW_FSL_I2S_SYNC_SELF:
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_MASK);
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_SELF_RX);
        /* 接收器必须工作在异步模式 */
        amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_MASK);
        amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_DISABLE);
        break;

    case AMHW_FSL_I2S_SYNC_ANOTHER_TX:
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_MASK);
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_ANOTHER_TX);
        break;

    case AMHW_FSL_I2S_SYNC_ANOTHER_RX:
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_MASK);
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_ANOTHER_RX);
        break;

    default:
        break;
    }
}

/**
 * \brief I2S接收同步的模式配置
 * \param[in] p_hw_i2s  : 指向I2S寄存器块的指针
 * \param[in] sync_mode : 接收同步的模式
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_sync_mode_set (amhw_fsl_i2s_t *p_hw_i2s,
                                amhw_fsl_i2s_sync_mode_t sync_mode)
{
    switch (sync_mode) {

    case AMHW_FSL_I2S_SYNC_DISABLED:
        amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_MASK);
        amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_DISABLE);
        break;

    case AMHW_FSL_I2S_SYNC_SELF:
        amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_MASK);
        amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_SELF_TX);
        /* 发送器必须工作在异步模式 */
        amhw_fsl_i2s_tcr2_clr(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_MASK);
        amhw_fsl_i2s_tcr2_set(p_hw_i2s, AMHW_FSL_I2S_TCR2_SYNC_DISABLE);
        break;

    case AMHW_FSL_I2S_SYNC_ANOTHER_TX:
        amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_MASK);
        amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_ANOTHER_TX);
        break;

    case AMHW_FSL_I2S_SYNC_ANOTHER_RX:
        amhw_fsl_i2s_rcr2_clr(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_MASK);
        amhw_fsl_i2s_rcr2_set(p_hw_i2s, AMHW_FSL_I2S_RCR2_SYNC_ANOTHER_RX);
        break;

    default:
        break;
    }
}

/**
 * \brief I2S 发送清除状态标志
 * \param[in] p_hw_i2s  : 指向I2S寄存器块的指针
 * \param[in] flag_mask : 状态标志类型
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_state_flag_clr (amhw_fsl_i2s_t            *p_hw_i2s,
                                     amhw_fsl_i2s_state_flag_t  flag_mask)
{
    /* FIFO警告不能清除 */
    if (flag_mask & AMHW_FSL_I2S_STATE_FLAG_FIFO_WARNING) {
        flag_mask &= (uint32_t)(~AMHW_FSL_I2S_STATE_FLAG_FIFO_WARNING);
    }

    /* 检查是否要清除软件复位 */
    if (flag_mask & AMHW_FSL_I2S_STATE_FLAG_SOFT_RESET) {
        flag_mask &= (uint32_t)(~AMHW_FSL_I2S_STATE_FLAG_SOFT_RESET);
        amhw_fsl_i2s_tcsr_clr(p_hw_i2s, AMHW_FSL_I2S_TCSR_SR_MASK);
    }

    /* 写1清除其他状态标志 */
    amhw_fsl_i2s_tcsr_set(p_hw_i2s, flag_mask);
}

/**
 * \brief I2S 接收清除状态标志
 * \param[in] p_hw_i2s  : 指向I2S寄存器块的指针
 * \param[in] flag_mask : 状态标志类型
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_state_flag_clr (amhw_fsl_i2s_t            *p_hw_i2s,
                                      amhw_fsl_i2s_state_flag_t  flag_mask)
{
    /* FIFO警告不能清楚 */
    if (flag_mask & AMHW_FSL_I2S_STATE_FLAG_FIFO_WARNING) {
        flag_mask &= (uint32_t)(~AMHW_FSL_I2S_STATE_FLAG_FIFO_WARNING);
    }

    /* 检查是否要清除软件复位 */
    if (flag_mask & AMHW_FSL_I2S_STATE_FLAG_SOFT_RESET) {
        flag_mask &= (uint32_t)(~AMHW_FSL_I2S_STATE_FLAG_SOFT_RESET);
        amhw_fsl_i2s_rcsr_clr(p_hw_i2s, AMHW_FSL_I2S_RCSR_SR_MASK);
    }

    /* 写1清除其他状态标志 */
    amhw_fsl_i2s_rcsr_set(p_hw_i2s, flag_mask);
}

/**
 * \brief I2S发送器复位
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] reset_mask : 复位的类型
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_reset_set (amhw_fsl_i2s_t            *p_hw_i2s,
                                 amhw_fsl_i2s_reset_type_t  reset_mask)
{
    amhw_fsl_i2s_tcsr_set(p_hw_i2s, reset_mask);
}

/**
 * \brief I2S接收器复位
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] reset_mask : 复位的类型
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_reset_set (amhw_fsl_i2s_t            *p_hw_i2s,
                                amhw_fsl_i2s_reset_type_t  reset_mask)
{
    amhw_fsl_i2s_rcsr_set(p_hw_i2s, reset_mask);
}

/**
 * \brief I2S发送数据（查询模式）
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] txBuff   : 指向要发送缓冲区的指针
 * \param[in] size     : 要发送数据的长度
 *
 * \return  无
 */
void amhw_fsl_i2s_senddata_polling (amhw_fsl_i2s_t  *p_hw_i2s,
                                    uint32_t        *txbuff,
                                    uint32_t         size)
{
    while (size--) {
        /* 清除FIFO 错误 */
        if ((amhw_fsl_i2s_tcsr_get(p_hw_i2s) & AMHW_FSL_I2S_TCSR_FEF_MASK)) {
            amhw_fsl_i2s_tx_reset_set(p_hw_i2s, AMHW_FSL_I2S_RESET_TYPE_FIFO);
            amhw_fsl_i2s_tx_state_flag_clr(p_hw_i2s, AMHW_FSL_I2S_STATE_FLAG_FIFO_ERROR);
        }

        /* 等待FIFO 为空 */
        while (!(amhw_fsl_i2s_tcsr_get(p_hw_i2s) & AMHW_FSL_I2S_TCSR_FWF_MASK)) {
            if ((amhw_fsl_i2s_tcsr_get(p_hw_i2s) & AMHW_FSL_I2S_TCSR_FEF_MASK)) {
                amhw_fsl_i2s_tx_reset_set(p_hw_i2s, AMHW_FSL_I2S_RESET_TYPE_FIFO);
                amhw_fsl_i2s_tx_state_flag_clr(p_hw_i2s, AMHW_FSL_I2S_STATE_FLAG_FIFO_ERROR);
            }
        }

        amhw_fsl_i2s_txdata_write(p_hw_i2s, *txbuff);
        txbuff++;
    }

}

/**
 * \brief I2S接收数据（查询模式）
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] rxBuff   : 指向要接收缓冲区的指针
 * \param[in] size     : 要接收数据的长度
 *
 * \return  无
 */
void amhw_fsl_i2s_receivedata_polling (amhw_fsl_i2s_t *p_hw_i2s,
                                       uint8_t         *rxBuff,
                                       uint32_t         size)
{
    while (size --) {
        /* 清除FIFO错误标志  */
        if ((amhw_fsl_i2s_rcsr_get(p_hw_i2s) & AMHW_FSL_I2S_RCSR_FEF_MASK)) {
            amhw_fsl_i2s_rx_state_flag_clr(p_hw_i2s, AMHW_FSL_I2S_STATE_FLAG_FIFO_ERROR);
        }

        /* 等待FIFO 为空 */
        while (!(amhw_fsl_i2s_rcsr_get(p_hw_i2s) & AMHW_FSL_I2S_RCSR_FWF_MASK)) {
        }

        *rxBuff = amhw_fsl_i2s_rxdata_read(p_hw_i2s);
         rxBuff++;
    }
}

/* end of file */
