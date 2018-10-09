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
 * \brief TPM实现捕获功能，通过HW层的接口实现
 *
 * - 实验现象：
 *   1. 输出利用捕获功能得到的PWM信号的周期和频率。
 *
 * - 注意：
 *   1. TPM1通过PIOE_23引脚输出PWM；
 *   2. TPM0捕获输入通道0使用PIOD_0引脚；
 *   3. 关联PIOE_23引脚和PIOD_0引脚，使用捕获测量PWM频率。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_tpm_cap.c src_fsl_hw_tpm_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_fsl_if_hw_tpm_cap
 * \copydoc demo_fsl_hw_tpm_cap.c
 */
 
/** [src_fsl_hw_tpm_cap] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_pwm.h"
#include "am_vdebug.h"
#include "hw/amhw_fsl_tpm.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/** \brief 上升沿捕获 */
#define TPM_CAP_TRIGGER_RISE  1

/** \brief 下降沿捕获 */
#define TPM_CAP_TRIGGER_FALL  2

/** \brief TPM通道号 */
#define TPM_CAP_CH_NUM        AMHW_FSL_TPM_CH(0)

static volatile am_bool_t  __g_flag    = AM_FALSE;  /**< \brief 捕获标志，是否捕获到新的事件   */
static volatile uint64_t   __g_time_ns = 0;         /**< \brief 捕获计数值 */
uint32_t                   __g_tpm_clock;           /**< \brief tpm采用的时钟源大小 */

void tpm_cap_isr (void *p_arg)
{
    static uint32_t count1;
    static am_bool_t first = AM_TRUE;

    amhw_fsl_tpm_t   *p_hw_tpm = (amhw_fsl_tpm_t *)p_arg;
    uint32_t          cap_val;
    
    if (amhw_fsl_tpm_stat_flag_get(p_hw_tpm) & AMHW_FSL_TPM_STAT_FLAG(TPM_CAP_CH_NUM)) {
        cap_val = amhw_fsl_tpm_ch_value_get(p_hw_tpm, TPM_CAP_CH_NUM);
        if (__g_flag == AM_FALSE) {
            if (first == AM_TRUE) {
                count1 = cap_val;
                first = AM_FALSE;
            } else {
                if(cap_val > count1) {
                    /* 将两次读取值的差转换成时间  */
                    __g_time_ns = (uint64_t)1000000000 * (uint64_t)(cap_val - count1) / __g_tpm_clock;
                }
                first = AM_TRUE;
                /* 置标志为真，表明捕获完成 */
                __g_flag = AM_TRUE;
            } 
        }

        /* 清除通道标志 */
        amhw_fsl_tpm_stat_flag_clear(p_hw_tpm, AMHW_FSL_TPM_STAT_FLAG(TPM_CAP_CH_NUM));
    }
}

/**
 * \brief 捕获初始化
 * \param[in] p_tpm : 指向TPM寄存器块的指针
 * \return 无
 * \note 默认配置TPM0的通道0引脚，如选择其它通道则需在此修改
 */
void tpm_cap_init (amhw_fsl_tpm_t *p_hw_tpm)
{
    /* 关闭时钟，停止计数 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_NONE);

    /* 清零计数器 */
    amhw_fsl_tpm_count_clear(p_hw_tpm);

    /* 预分频设置 */
    amhw_fsl_tpm_prescale_set(p_hw_tpm, AMHW_FSL_TPM_DIVIDED_BY_1);

//    /* 获取tpm时钟源频率，便于后面计算 */
//    __g_tpm_clock = am_kl26_clk_periph_rate_get(p_hw_tpm);

}

/**
 * \brief 初始化一路PWM输出。
 *
 * \param[in] p_tpm    : 指向TPM寄存器块的指针。
 * \param[in] cap_num  : 捕获编号。
 * \param[in] cap_edge : 捕获边沿选择 (#TPM_CAP_TRIGGER_RISE) 或 (#TPM_CAP_TRIGGER_FALL)
 *
 * \return  无
 */
void tpm_cap_chan_config (amhw_fsl_tpm_t *p_hw_tpm, uint8_t cap_num, uint8_t cap_edge)
{
    if (cap_edge == TPM_CAP_TRIGGER_RISE) {
        amhw_fsl_tpm_ch_mode(p_hw_tpm, cap_num, AMHW_FSL_TPM_COUNTING_UP,
                              AMHW_FSL_TPM_CHSC_MS(0) | AMHW_FSL_TPM_CHSC_EL_SEL(1));
    } else {
        amhw_fsl_tpm_ch_mode(p_hw_tpm, cap_num, AMHW_FSL_TPM_COUNTING_UP,
                              AMHW_FSL_TPM_CHSC_MS(0) | AMHW_FSL_TPM_CHSC_EL_SEL(2));
    }
}

void tpm_cap_chan_enable (amhw_fsl_tpm_t *p_hw_tpm, uint32_t cap_num)
{
    /* 使能通道中断 */
    amhw_fsl_tpm_ch_sc_set(p_hw_tpm, cap_num, AMHW_FSL_TPM_CHSC_IE);

    /* 打开时钟，启动计数 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_MODULE);
}

/**
 * \brief 例程入口
 */
void demo_fsl_hw_tpm_cap_entry (am_pwm_handle_t  tpm2_pwm_handle,
                                amhw_fsl_tpm_t  *p_hw_tpm,
                                int              inum,
                                uint32_t         tpm_clock)
{
    uint32_t        freq;               /**< \brief 捕获到的频率 */
    uint32_t        time_ns;

    AM_DBG_INFO("The TPM demo for CAP service\r\n");

    __g_tpm_clock = tpm_clock;

    /* TPM2通道1使用PIOE_23，在 amhw_timer_cap_config.c 中配置 */
    am_pwm_config(tpm2_pwm_handle, 1, 500000 / 2, 500000); /* T频率为2KHz */
    am_pwm_enable(tpm2_pwm_handle, 1);
    
    am_int_connect(inum, tpm_cap_isr, (void *)p_hw_tpm);
    am_int_enable(inum);

    tpm_cap_init(p_hw_tpm);
    tpm_cap_chan_config(p_hw_tpm, TPM_CAP_CH_NUM, TPM_CAP_TRIGGER_RISE);
    tpm_cap_chan_enable(p_hw_tpm, TPM_CAP_CH_NUM);

    while (1) {
        uint32_t key = am_int_cpu_lock();
        if (__g_flag == AM_TRUE) {
            time_ns = __g_time_ns;
            __g_flag = AM_FALSE;
            am_int_cpu_unlock(key);
            freq = (uint32_t)1000000000 / time_ns;           /* 转换成频率 */
            AM_DBG_INFO("The period is %d ns, The freq is %d Hz \r\n", time_ns, freq);
        } else {
            am_int_cpu_unlock(key);
        }
        am_mdelay(200);
        am_led_toggle(LED0);
    }
}
 
/** [src_fsl_hw_tpm_cap] */

/* end of file */
