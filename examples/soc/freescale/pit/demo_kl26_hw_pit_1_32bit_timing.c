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
 * \brief PIT 定时器实现(32位)定时功能的例程，通过HW层的接口实现
 *
 * - 实验现象：
 *   1. LED以10Hz的频率闪烁。
 *
 * - 注意：
 *    如果要运行此例程，需要将am_prj_config.h中AM_CFG_SOFTIMER_ENABLE、
 *    AM_CFG_KEY_GPIO_ENABLE和AM_CFG_SYSTEM_TICK_ENABLE三个宏的值都设为0。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_pit_1_32bit_timing.c src_fsl_hw_pit_1_32bit_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */


/**
 * \addtogroup demo_fsl_if_hw_pit_1_32bit_timing
 * \copydoc demo_fsl_hw_pit_1_32bit_timing.c
 */

/** [src_fsl_hw_pit_1_32bit_timing] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "hw/amhw_fsl_pit.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/**
 * \brief  PIT0中断服务函数
 * \param[in]  p_arg : 用户自定义参数，由 am_int_connect() 函数传递。
 * \return  无
 */
void pit0_irq_handler(void *p_arg)
{
    amhw_fsl_pit_t *p_hw_pit = (amhw_fsl_pit_t *)p_arg;
    /* 通道0产生中断 */
    if (amhw_fsl_pit_timerout_flag_check(p_hw_pit , AMHW_FSL_PIT_CH(0)) == AM_TRUE ) {

        am_led_toggle(LED0);

        /* 清除标志 */
        amhw_fsl_pit_timerout_flag_clr(p_hw_pit , AMHW_FSL_PIT_CH(0));
    }
}

/**
 * \brief 初始化PIT为定时器功能。
 *
 * \param[in] p_pit : 指向PIT寄存器块的指针
 * \param[in] freq  : 中断频率。
 *
 * \return 无
 */
void pit_timer_init (amhw_fsl_pit_t *p_hw_pit,
                     uint32_t        freq,
                     int             inum,
                     uint32_t        pit_clk)
{
    uint32_t load_val;

    load_val = pit_clk / freq;

    /* 使能PIT模块 */
    amhw_fsl_pit_module_enable(p_hw_pit);

    /* 设置装载值 */
    amhw_fsl_pit_ldval_set(p_hw_pit, AMHW_FSL_PIT_CH(0), load_val);

    /* 打开中断并使能计数 */
    amhw_fsl_pit_timer_ctrl_set(p_hw_pit,
                                 AMHW_FSL_PIT_CH(0),
                                 AMHW_FSL_PIT_TCTL_TI_EN | AMHW_FSL_PIT_TCTL_TMR_EN);

    am_int_connect(inum, pit0_irq_handler, p_hw_pit);
    am_int_enable(inum);
}


/**
 * \brief 使能PIT为定时器(开始计数)
 * \param[in] p_pit : 指向PIT寄存器块的指针
 * \return 无
 */
void pit_timer_enable (amhw_fsl_pit_t *p_hw_pit)
{
    /* 开始运行 */
    amhw_fsl_pit_timer_ctrl_set(p_hw_pit,
                                 AMHW_FSL_PIT_CH(0),
                                 AMHW_FSL_PIT_TCTL_TMR_EN);
}

/**
 * \brief  禁止PIT为定时器(停止计数)
 * \param[in] p_pit : 指向PIT寄存器块的指针
 * \return  无
 */
void pit_timer_disable (amhw_fsl_pit_t *p_hw_pit)
{
    /* 置停止位，PIT停止运行 */
    amhw_fsl_pit_timer_ctrl_clear(p_hw_pit,
                                   AMHW_FSL_PIT_CH(0),
                                   AMHW_FSL_PIT_TCTL_TMR_EN);
}

/**
 * \brief 例程入口
 */
void demo_fsl_hw_pit_1_32bit_timing_entry (amhw_fsl_pit_t *p_hw_pit,
                                           int             inum,
                                           uint32_t        pit_clk)
{

    AM_DBG_INFO("The demo for PIT timing:\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");

    /* 使用PIT实现定时功能，中断频率为10Hz */
    pit_timer_init(p_hw_pit , 10, inum, pit_clk);

    /* 使能PIT计数 */
    pit_timer_enable(p_hw_pit);

    while (1) {

    }
}

/** [src_fsl_hw_pit_1_32bit_timing] */

/* end of file */
