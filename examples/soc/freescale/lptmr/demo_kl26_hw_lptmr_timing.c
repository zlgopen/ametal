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
 * \brief LPTMR实现定时器例程，通过HW层接口实现
 *
 * - 实验现象：
 *   1. LED0以10Hz的频率闪烁。
 *
 * \note LPTMR时钟源有4种选择，在amdr_hwconfig_lptmr_timing.c中进行配置，选择不同大小
 *       的时钟源，定时的范围和精度存在差别。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_lptmr_timing.c src_fsl_hw_lptmr_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_lptmr_timing
 * \copydoc demo_fsl_hw_lptmr_timing.c
 */

/** [src_fsl_hw_lptmr_timing] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "hw/amhw_fsl_lptmr.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/**
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
void hw_lptmr_timing_callback (void *p_arg)
{
    amhw_fsl_lptmr_t *p_hw_lptmr = (amhw_fsl_lptmr_t *)p_arg;

    if (amhw_fsl_lptmr_flag_check(p_hw_lptmr)) {
        am_led_toggle(LED0);

        /* 清除溢出标志，写1清零 */
        amhw_fsl_lptmr_ctl_set(p_hw_lptmr, AMHW_FSL_LPTMR_CSR_TCF);
    }
}

/**
 * \brief 初始化LPTMR
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] freq       : 中断频率。
 *
 * \retval  AM_OK      : 初始化成功
 * \retval -AM_EINVAL  : 参数有误
 */
int lptmr_timer_init(amhw_fsl_lptmr_t *p_hw_lptmr,
                     uint32_t          count,
                     int               inum)
{
    uint32_t pre_reg = 0, pre_real;
    uint32_t temp;

    if (count > (0xFFFFu * 65536)) {
        return -AM_EINVAL;
    }

    /* 不需要分频，禁能分频器 */
    if (count < 0xFFFF) {
        pre_real = 1;
        amhw_fsl_lptmr_prescaler_filter_set(p_hw_lptmr, AM_TRUE);
    } else {
        temp = count / 0xFFFFu + 1;

        /* 计算分频值 2^n */
        for (pre_real = 2; pre_real < temp;) {
            pre_reg++;
            pre_real = pre_real << 1;
        }

        /* 分频器有效 */
        amhw_fsl_lptmr_prescaler_filter_set(p_hw_lptmr, AM_FALSE);

        /* 设置预分频值 */
        amhw_fsl_lptmr_prescaler_set(p_hw_lptmr, (amhw_fsl_lptmr_ps_t)pre_reg);
    }

    count = count / pre_real;

    /* 设置装载值 */
    amhw_fsl_lptmr_compare_set(p_hw_lptmr, count - 1);

    amhw_fsl_lptmr_ctl_set(p_hw_lptmr, AMHW_FSL_LPTMR_CSR_TIE);        /* 开中断 */
    am_int_connect(inum, hw_lptmr_timing_callback, p_hw_lptmr);
    am_int_enable(inum);

    return AM_OK;
}

/**
 * \brief 使能LPTMR定时器(开始计数)
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return 无
 */
void lptmr_timer_enable (amhw_fsl_lptmr_t *p_hw_lptmr)
{
    /* 关闭时钟，LPTMR开始运行 */
    amhw_fsl_lptmr_ctl_set(p_hw_lptmr, AMHW_FSL_LPTMR_CSR_TEN);
}

/**
 * \brief  禁止LPTMR定时器(停止计数)
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return  无
 */
void lptmr_timer_disable (amhw_fsl_lptmr_t *p_hw_lptmr)
{
    /* 关闭时钟，TPM停止运行 */
    amhw_fsl_lptmr_ctl_clear(p_hw_lptmr, AMHW_FSL_LPTMR_CSR_TEN);
}

/**
 * \brief 例程入口
 */
void demo_fsl_hw_lptmr_timing_entry (amhw_fsl_lptmr_t *p_hw_lptmr,
                                     int               inum,
                                     uint32_t          lptmr_clock)
{

    /* 关闭LPTMR模块 */
    amhw_fsl_lptmr_ctl_clear(p_hw_lptmr, AMHW_FSL_LPTMR_CSR_TEN);

    /* 时钟源选择 */
    amhw_fsl_lptmr_clock_set(p_hw_lptmr, AMHW_FSL_LPTMR_CLOCK_SRC_MCGIRCLK);

    AM_DBG_INFO("The LPTMR demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");

    /* 中断频率10Hz */
    lptmr_timer_init(p_hw_lptmr, lptmr_clock / 10, inum);

    /* 使能LPTMR,开始计数 */
    lptmr_timer_enable(p_hw_lptmr);

    while (1) {

    }
}

/** [src_fsl_hw_lptmr_timing] */

/* end of file */
