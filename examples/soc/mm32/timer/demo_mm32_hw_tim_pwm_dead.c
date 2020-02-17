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
 * \brief 定时器带死区时间的互补 PWM 输出例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 对应引脚输出互补 PWM，频率为 100KHz，占空比为 40%，死区时间为 100ns。
 *
 * \par 源代码
 * \snippet demo_mm32_hw_tim_pwm_dead.c src_mm32_hw_tim_pwm_dead
 *
 * \internal
 * \par Modification history
 * - 1.00 18-06-06  pea, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_mm32_hw_tim_pwm_dead
 * \copydoc demo_mm32_hw_tim_pwm_dead.c
 */

/** [src_mm32_hw_tim_pwm_dead] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_bitops.h"
//#include "am_mm32l073.h"
#include "hw/amhw_mm32_tim.h"

/**
 * \brief PWM 输出频率定义，单位为 Hz
 *        如果此频率不能由定时器时钟整数分频得到，实际频率会有误差
 */
#define  __PWM_FREQ  100000

/**
 * \brief PWM 占空比定义，单位为 %
 *        100% / (arr 寄存器值 + 1)为占空比调整的精度，
 */
#define  __PWM_DUTY  40

/**
 * \brief PWM 占空比除数定义，__PWM_DUTY / __PWM_UNIT 的单位为 %
 *        比如需要配置值的精度为 1%，则 __PWM_UNIT 配置为 100，
 *        如果需要配置值的精度为 0.1%，则 __PWM_UNIT 配置为 1000，以此类推
 */
#define  __PWM_UNIT  100

/**
 * \brief 死区时间定义，单位为皮秒，精度与 TIMx_CR1 中的 CKD 有关，
 *        比如定时器时钟为 48MHz，CKD 配置为 0，则死区时间精度为 1 / 48MHz = 20.83ns
 *        如果死区时间过大，会导致 PWM 无输出，
 *        同时，由于死区时间的插入，会导致占空比误差，
 *        占空比误差 = (死区时间 / PWM 周期) * 100%
 *        比如，PWM 频率为 100KHz，周期为 10us，死区时间配置为 100ns 时，占空比误差为 1%
 *        如果此时占空比配置为 40%，则 TIM_CHx 上输出的占空比为 40% - 1% = 39%
 *        TIM_CHxN 上输出的占空比为 40% + 1% = 41%
 */
#define  __DEAD_TIME  100000

/**
 * \brief 配置定时器 TIM 为 PWM 输出(PWM 模式 2)
 *
 * \param[in] p_hw_tim 指向定时器寄存器块的指针
 * \param[in] chan     PWM 输出的通道，TIM1、TIM2、TIM3 有 4 个通道，
 *                     而 TIM14、TIM16、TIM17 只有一个
 * \param[in] freq     PWM 频率，单位为 Hz，实际可能会有误差
 * \param[in] duty     占空比，单位为 %，实际可能会有误差
 * \param[in] clk_rate 定时器时钟频率
 *
 * \return 无
 *
 * \note 由 ARR 寄存器决定所有 PWM 通道使用相同的周期参数
 */
am_local void __tim_pwm_chan_config (amhw_mm32_tim_t *p_hw_tim,
                                     uint32_t        chan,
                                     uint32_t        freq,
                                     uint32_t        duty,
                                     uint32_t        clk_rate)
{
    uint16_t pre_real = 1;
    uint16_t pre_reg  = 0;
    uint32_t period_c;
    uint32_t duty_c;
    uint32_t temp;

    /* 计算出来得到的是计数值 CNT */
    period_c = clk_rate / freq;
    duty_c   = period_c * (__PWM_UNIT - duty) / __PWM_UNIT;

    /* 当计数小于 65536 时，不分频(值为 1, 1 代表为 1 分频) */
    temp = period_c / 65536 + 1;

    /* 16 位定时器需要运算取得合适的分频值 */
    for (pre_real = 1; pre_real < temp; ) {
        pre_reg++;           /* 计算写入寄存器的分频值 0, 1, 2, ... */
        pre_real++;          /* 分频数 */
    }

    /* 设置分频值 */
    amhw_mm32_tim_prescale_set(p_hw_tim, pre_reg);

    /* 重新计算 PWM 的周期及脉冲频率 */
    period_c = period_c / pre_real;
    duty_c = duty_c / pre_real;

    /* 设置自动重装寄存器的值 */
    amhw_mm32_tim_arr_set(p_hw_tim, period_c - 1);

    /* 设置比较输出通道的匹配值 */
    amhw_mm32_tim_ccr_ouput_reload_val_set(p_hw_tim,  duty_c, chan);

    /* 计数器清 0 */
    amhw_mm32_tim_count_set(p_hw_tim, 0);

    /* 使能定 ARR 预装载 */
    amhw_mm32_tim_arpe_enable(p_hw_tim);

    /* 选择该通道为输出 */
    amhw_mm32_tim_ccs_set(p_hw_tim, 0, chan);

    /* 选择该通道的模式为 PWM 模式 2 */
    amhw_mm32_tim_ocm_set(p_hw_tim, AMHW_MM32_TIM_PWM_MODE2, chan);

    /* PWM 输出通道预装载使能 */
    amhw_mm32_tim_ccs_ocpe_enable(p_hw_tim, chan);

    /* 设置比较输出通道 CCP 高电平极性有效 */
    amhw_mm32_tim_ccp_output_set(p_hw_tim, 0, chan);
}

/**
 * \brief 使能 PWM 输出
 *
 * \param[in] p_hw_tim 指向定时器寄存器块的指针
 * \param[in] chan     PWM 输出的通道，TIM1、TIM2、TIM3 有 4 个通道，
 *                     而 TIM14、TIM16、TIM17 只有一个
 *
 * \return 无
 */
am_local void __tim_pwm_enable (amhw_mm32_tim_t *p_hw_tim, uint32_t chan)
{

    /* 使能通道 PWM 输出 */
    amhw_mm32_tim_cce_output_enable(p_hw_tim, chan);

    /* 高级定时器使能主输出 MOE */
    amhw_mm32_tim_bdtr_enable(p_hw_tim, AMHW_MM32_TIM_MOE);

    /* 产生更新事件，重新初始化 Prescaler 计数器及 Repetition 计数器 */
    amhw_mm32_tim_egr_set(p_hw_tim, AMHW_MM32_TIM_UG);

    if (amhw_mm32_tim_status_flg_get(p_hw_tim, AMHW_MM32_TIM_UG) != 0) {

        /* 更新定时器时会产生更新事件，清除标志位 */
        amhw_mm32_tim_status_flg_clr(p_hw_tim, AMHW_MM32_TIM_UG);
    }

    /* 使能定时器 TIM 允许计数 */
    amhw_mm32_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器 TIM PWM 输出初始化函数
 *
 * \param[in] p_hw_tim 指向定时器寄存器块的指针
 * \param[in] clk_id   时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \return 无
 */
am_local void __tim_pwm_init (amhw_mm32_tim_t *p_hw_tim)
{
    /* 边沿对齐模式 */
    amhw_mm32_tim_cms_set(p_hw_tim, 0);

    /* 向上计数 */
    amhw_mm32_tim_dir_set(p_hw_tim, 0);

    /* 设置时钟分割:TDTS = Tck_tin */
    amhw_mm32_tim_ckd_set(p_hw_tim, 0);

    /* 允许更新事件 */
    amhw_mm32_tim_udis_enable(p_hw_tim);
}

/**
 * \brief 根据 DTG 配置计算死区时间
 *
 * \param[in] p_hw_tim 指向定时器寄存器块的指针
 * \param[in] dtg      死区发生器设置，TIMx_BDTR 的 DTG 字段
 *
 * \return 死区时间，单位为皮秒
 */
am_local int32_t __dead_time_get (amhw_mm32_tim_t *p_hw_tim,
                                  uint8_t         dtg,
                                  uint32_t        clk_rate)
{
    int32_t  dt;
    uint32_t tck_int = 1000000000000 / clk_rate;
    uint8_t  ckd     = AM_BITS_GET(p_hw_tim->cr[0], 8, 2);
    uint32_t tdts    = tck_int * (ckd ? ckd * 2 : 1);
    uint32_t tdtg;

    if (7 == AM_BITS_GET(dtg, 5, 3)) {
        tdtg = 16 * tdts;
        dt = (32 + AM_BITS_GET(dtg, 0, 5)) * tdtg;
    } else if (6 == AM_BITS_GET(dtg, 5, 3)) {
        tdtg = 8 * tdts;
        dt = (32 + AM_BITS_GET(dtg, 0, 5)) * tdtg;
    } else if (2 == AM_BITS_GET(dtg, 6, 2)) {
        tdtg = 2 * tdts;
        dt = (64 + AM_BITS_GET(dtg, 0, 6)) * tdtg;
    } else if (0 == AM_BITS_GET(dtg, 7, 1)) {
        tdtg = tdts;
        dt = AM_BITS_GET(dtg, 0, 7) * tdtg;
    } else {
        dt = AM_ERROR;
    }

    /* 这里加上一个 tck_int 是因为实测 dtg 配置为 0 的时候，也有一个 tck_int 的死区时间 */
    return dt + tck_int;
}

/**
 * \brief 根据需要的死区时间计算最接近的 DTG 配置
 *
 * \param[in] p_hw_tim  指向定时器寄存器块的指针
 * \param[in] dead_time 需要配置的死区时间，单位为皮秒
 *
 * \return DTG 配置值
 */
am_local int32_t __dtg_calculate (amhw_mm32_tim_t *p_hw_tim,
                                  uint32_t        dead_time,
                                  uint32_t        clk_rate)
{
    int32_t i;
    uint32_t dt;

    for (i = 0; i <= 0xFF; i++) {
        dt = __dead_time_get(p_hw_tim, i, clk_rate);
        if (dt >= dead_time) {
            break;
        }
    }

    return (i > 0xFF) ? 0xFF : i;
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_tim_pwm_dead_entry (amhw_mm32_tim_t     *p_hw_tim,
                                     amhw_mm32_tim_type_t type,
                                     uint32_t            chan,
                                     uint32_t            clk_rate)
{
    if ((AMHW_MM32_TIM_TYPE0 != type) &&
        (AMHW_MM32_TIM_TYPE2 != type) &&
        (AMHW_MM32_TIM_TYPE3 != type)) {
        AM_DBG_INFO("this timer don't support pwm dead!\r\n");
    }

    /* 初始化定时器 TIM 为 PWM 功能 */
    __tim_pwm_init(p_hw_tim);

    /* 配置定时器 TIM PWM 输出通道 */
    __tim_pwm_chan_config(p_hw_tim, 0, __PWM_FREQ, __PWM_DUTY, clk_rate);

    /* 使能互补输出 */
    amhw_mm32_tim_ccne_enable(p_hw_tim, AMHW_MM32_TIM_CC1S);

    /* 使能互补输出高电平有效 */
    amhw_mm32_tim_ccne_set(p_hw_tim, 0, AMHW_MM32_TIM_CC1S);

    /* 设置死区时间 */
    amhw_mm32_tim_utg_set(p_hw_tim, __dtg_calculate(p_hw_tim, __DEAD_TIME, clk_rate));

    /* 启动定时器 */
    __tim_pwm_enable(p_hw_tim, 0);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_mm32_hw_tim_pwm_dead] */

/* end of file */
