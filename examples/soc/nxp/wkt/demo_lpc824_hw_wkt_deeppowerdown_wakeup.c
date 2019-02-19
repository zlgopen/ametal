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
 * \brief WKT 深度掉电唤醒例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 运行程序，一秒后 LED0 闪烁一次后进入深度掉电模式；
 *   2. 等待 5s 后，WKT 计时时间到，CPU 被唤醒，如果通用寄存器数据校验正确，LED0
 *      闪烁一次；如果通用寄存器数据校验错误，LED0 持续闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 进入深度掉电模式后，只有 WAKEUP 引脚和 WKT 定时器能唤醒芯片。对于默认状态，
 *       WAKEUP 引脚唤醒使能，所以一定确保 WAKEUP 外部上拉到电源，保证有效的高电平
 *       状态，否则会引起误唤醒操作，而观察不到 WKT 唤醒 CPU；而本例程将 WAKEUP 引脚
 *       (PIO0_4)唤醒功能禁止了，所以不需要将 PIO0_4 上拉；
 *    3. 使用该程序后，会导致下次程序烧写的时候检测不到芯片（由于芯片进入深度掉电
 *       模式将 SWD 调试模式关闭），下次下载时将 P0_12 拉低或按动复位键之后一秒内
 *       即可正常下载调试。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_wkt_deeppowerdown_wakeup.c src_lpc824_hw_wkt_deeppowerdown_wakeup
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-01  sky, modified
 * - 1.00 15-07-14  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_wkt_deeppowerdown_wakeup
 * \copydoc demo_lpc824_hw_wkt_deeppowerdown_wakeup.c
 */

/** [src_lpc824_hw_wkt_deeppowerdown_wakeup] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_pmu.h"
#include "hw/amhw_lpc_wkt.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** \brief 选择 IRC 时钟 频率为 750kHz */
#define __IRC_CLOCK_FREQ          750

/** \brief 选择低功耗时钟 频率为 10kHz */
#define __LOW_POWER_CLOCK_FREQ    10

/**
 * \name 系统控制寄存器宏定义
 * @{
 */

/** \brief 处理器返回到线程模式时不进入睡眠 */
#define __AMHW_LPC82X_SCR_ISRBACK_NTO_SLP     AM_SBF(0, 1)

/** \brief 处理器返回到线程模式时进入睡眠 */
#define __AMHW_LPC82X_SCR_ISRBACK_TO_SLP      AM_SBF(1, 1)

/** \brief 将睡眠模式作为低功耗模式 */
#define __AMHW_LPC82X_SCR_LOWPWR_MODE_SLP     AM_SBF(0, 2)

/** \brief 将深度睡眠模式作为低功耗模式 */
#define __AMHW_LPC82X_SCR_LOWPWR_MODE_DPSLP   AM_SBF(1, 2)

/** \brief 只有使能的中断能够唤醒处理器 */
#define __AMHW_LPC82X_SCR_WKUP_BY_ENAISR      AM_SBF(0, 4)

/** \brief 所有中断能够唤醒处理器 */
#define __AMHW_LPC82X_SCR_WKUP_BY_ALLISR      AM_SBF(1, 4)

/**
 * @}
 */

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief 中断标志变量 */
am_local volatile uint8_t __g_deeppowerdown_wkt_flag = 0;

/*******************************************************************************
  本地函数定义
*******************************************************************************/

/**
 * \brief 设置当前定时器的计数值
 *
 * \param[in] delay_inms 设置定时时间 delay_inms，延时时间(单位：ms)
 *
 * \return 无
 */
am_local void __wkt_delayms_set (amhw_lpc_wkt_t  *p_hw_wkt, uint32_t delay_inms)
{

    /* 定时 (delay_inms) ms */
    if(amhw_lpc_wkt_clksel_get(p_hw_wkt)) {
        amhw_lpc_wkt_count_set(p_hw_wkt,
                               __LOW_POWER_CLOCK_FREQ * delay_inms);
    } else {
        amhw_lpc_wkt_count_set(p_hw_wkt, __IRC_CLOCK_FREQ * delay_inms);
    }
}

/**
 * \brief 系统控制寄存器设定
 *
 * \param[in] flags 系统控制寄存器宏 (#__AMHW_LPC82X_SCR_ISRBACK_NTO_SLP)
 *
 * \return 无
 */
am_local void __scb_scr_set (uint32_t flags)
{
    SCB->SCR = flags;
}

/**
 * \brief WKT 中断服务函数
 */
am_local void __deeppowerdown_wkt_isr (void *p_arg)
{
    amhw_lpc_wkt_t  *p_hw_wkt = (amhw_lpc_wkt_t *)p_arg;

    if (amhw_lpc_wkt_alarmflag_get(p_hw_wkt)) {
        amhw_lpc_wkt_alarmflag_clear(p_hw_wkt); /* 清除中断标志 */
        __g_deeppowerdown_wkt_flag = 1;                /* 改变中断标志变量 */
    }
}

/**
 * \brief 深度掉电模式初始化
 */
am_local void __wkt_deeppowerdown_wakeup (amhw_lpc82x_pmu_t *p_hw_pmu,
                                          amhw_lpc_wkt_t    *p_hw_wkt,
                                          int                inum,
                                          uint32_t           delay_inms)
{

    /* 已经从深度掉电中唤醒 */
    if (amhw_lpc82x_pmu_dpdflag_get(p_hw_pmu) != 0x0) {

        /* 清除深度掉电模式标志位 */
        amhw_lpc82x_pmu_dpdflag_clear(p_hw_pmu);

        if ((amhw_lpc82x_pmu_gpdata_get(p_hw_pmu, GP_REG_0) != 0x12345678) ||
            (amhw_lpc82x_pmu_gpdata_get(p_hw_pmu, GP_REG_1) != 0x87654321) ||
            (amhw_lpc82x_pmu_gpdata_get(p_hw_pmu, GP_REG_2) != 0x56781234) ||
            (amhw_lpc82x_pmu_gpdata_get(p_hw_pmu, GP_REG_3) != 0x43218765)) {

            /* 从深度掉电唤醒后，通用寄存器数据检查失败，LED0 持续闪烁 */
            AM_FOREVER {

                /* LED0 以 0.5s 的间隔一直闪烁 */
                am_led_toggle(LED0);
                am_mdelay(500);
            }
        }
    } else {

        /* 未进入深度掉电模式，设置通用寄存器数据，用于测试 */
        amhw_lpc82x_pmu_gpdata_save(p_hw_pmu, GP_REG_0, 0x12345678);
        amhw_lpc82x_pmu_gpdata_save(p_hw_pmu, GP_REG_1, 0x87654321);
        amhw_lpc82x_pmu_gpdata_save(p_hw_pmu, GP_REG_2, 0x56781234);
        amhw_lpc82x_pmu_gpdata_save(p_hw_pmu, GP_REG_3, 0x43218765);

        /* 禁用引脚 PIO0_4 上的唤醒功能 */
        amhw_lpc82x_pmu_wakepad_disable(p_hw_pmu);

        /* ARM Cortex-M0+ 内核的低功耗模式，设置为深度睡眠模式 */
        __scb_scr_set(__AMHW_LPC82X_SCR_LOWPWR_MODE_DPSLP);

        /* 使能低功耗振荡器 */
        amhw_lpc82x_pmu_lposcen_enable(p_hw_pmu);

        /* 深度掉电模式下使能低功耗振荡器 */
        amhw_lpc82x_pmu_lposcdpden_enable(p_hw_pmu);

        /* ARM WFI 将进入深度掉电模式 */
        amhw_lpc82x_pmu_pm_cfg(p_hw_pmu,
                               AMHW_LPC82X_PMU_PCON_MODE_DEEPPD);

        /* 定时 (delayInMs) delay_inmss */
        __wkt_delayms_set(p_hw_wkt, delay_inms);
       
        /* 打开 WKT 中断 */
        am_int_enable(inum);
       
        /* 连接中断服务函数 */
        am_int_connect(inum, __deeppowerdown_wkt_isr, (void *)p_hw_wkt);

        /* 进入深度掉电模式 */
        __WFI();
    }
}

/*******************************************************************************
  外部函数定义
*******************************************************************************/

void demo_lpc824_hw_wkt_deeppowerdown_wakeup_entry(amhw_lpc82x_pmu_t *p_hw_pmu,
                                                   amhw_lpc_wkt_t    *p_hw_wkt,
                                                   int                inum,
                                                   uint32_t           delay_inms)
{

    /* 延时一秒，方便下次下载程序 */
    am_mdelay(1000);

    /* LED0 闪烁一次 */
    am_led_on(LED0);
    am_mdelay(500);
    am_led_off(LED0);

    /* 进入 DeepPowerDown 模式 */
    __wkt_deeppowerdown_wakeup(p_hw_pmu, p_hw_wkt, inum, delay_inms);

    /* 从深度掉电模式唤醒后，通用寄存器数据检查正确，LED0 闪烁一次 */
    am_led_on(LED0);
    am_mdelay(500);
    am_led_off(LED0);

    AM_FOREVER {
        ; /* VOID */
    }
}

/** [src_lpc824_hw_wkt_deeppowerdown_wakeup] */

/* end of file */
