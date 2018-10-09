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
 * \brief TPM实现标准定时器例程，通过HW层的接口实现
 *
 * - 实验现象：
 *   1. LED以10Hz的频率闪烁。
 *
 *\note TPM有4种时钟源可以选择，默认配置时钟源大小为20M,则中断的最小频率为:
 *              fre = 20000000/(128*65535) = 2.38Hz  （16位定时器，最高128分频）
 *
 * \par 源代码
 * \snippet demo_fsl_hw_tpm_timing.c src_fsl_hw_tpm_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_fsl_if_hw_tpm_timing
 * \copydoc demo_fsl_hw_tpm_timing.c
 */
 
/** [src_fsl_hw_tpm_timing] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "hw/amhw_fsl_tpm.h"
#include "am_board.h"

/** 
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
void tpm_timing_callback (void *p_arg)
{

    amhw_fsl_tpm_t *p_hw_tpm = (amhw_fsl_tpm_t *)p_arg;
    /*
     * 判断是否是定时器溢出中断
     */
    if (amhw_fsl_tpm_stat_flag_get(p_hw_tpm) & AMHW_FSL_TPM_STAT_OV_FLAG ) {

        am_led_toggle(LED0);
        /* 清除溢出标志 */
        amhw_fsl_tpm_stat_flag_clear(p_hw_tpm, AMHW_FSL_TPM_STAT_OV_FLAG);
    }
}

/**
 * \brief 初始化TPM为定时器功能。
 *
 * \param[in] p_tpm    : 指向TPM寄存器块的指针
 * \param[in] freq     : 中断频率。
 * \param[in] inum     : 中断号。
 * \param[in] tpm_rate : 定时器外设频率。
 *
 * \retval  AM_OK      : 初始化成功
 * \retval  -AM_EINVAL : 参数有误
 */
int tpm_timer_init (amhw_fsl_tpm_t *p_hw_tpm,
                    uint32_t        freq,
                    int             inum,
                    uint32_t        tpm_clk)
{
    uint32_t  match, temp;
    uint8_t  pre_real, pre_reg = 0;

    match    = tpm_clk / freq;

    if (match > 0xffff * 128) {             /* 16位定时器，128分频 */
        return -AM_EINVAL;
    }

    temp = match / 0xffffu + 1;
    /* 只支持分频大小1,2,4,8...128，求得大于分频数中最小的数2^n */
    for (pre_real = 1; pre_real < temp; ) {
        pre_reg++;                        /* 计算写入寄存器的分频值0,1,2,... */
        pre_real = pre_real << 1;           /* 分频数2^n */
    }
    match = match / pre_real;
    amhw_fsl_tpm_prescale_set(p_hw_tpm, (amhw_fsl_tpm_prescale_t)pre_reg);
    amhw_fsl_tpm_count_clear(p_hw_tpm);
    amhw_fsl_tpm_mod_set(p_hw_tpm, match - 1);

    amhw_fsl_tpm_ctrl_set(p_hw_tpm, AMHW_FSL_TPM_SC_TOI_EN);        /* 开中断 */

    am_int_connect(inum, tpm_timing_callback, p_hw_tpm);
    am_int_enable(inum);

    return AM_OK;
}

/**
 * \brief 使能TPM定时器(开始计数)
 * \param[in] p_tpm : 指向TPM寄存器块的指针
 * \return 无
 */
void tpm_timer_enable (amhw_fsl_tpm_t *p_hw_tpm)
{
    /* 关闭时钟，TPM开始运行 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_MODULE);
}

/**
 * \brief  禁止TPM定时器(停止计数)
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \return  无
 */
void hw_tpm_timer_disable (amhw_fsl_tpm_t *p_hw_tpm)
{
    /* 关闭时钟，TPM停止运行 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_NONE);
}

/**
 * \brief 例程入口
 */
void demo_fsl_hw_tpm_timing_entry (amhw_fsl_tpm_t *p_hw_tpm0,
                                   uint32_t        freq,
                                   int             inum,
                                   uint32_t        tpm_clk)
{
    
    AM_DBG_INFO("The demo for TPM timing:\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");
  
    /* 使用TPM0,中断频率为10Hz */
    tpm_timer_init(p_hw_tpm0, freq, inum, tpm_clk);
    
    /* 开始计数 */
    tpm_timer_enable(p_hw_tpm0);

    while (1) {
    }
}

/** [src_fsl_hw_tpm_timing] */

/* end of file */
