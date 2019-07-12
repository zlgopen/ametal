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
 * \brief 睡眠模式例程，使用定时器周期唤醒，通过驱动层接口实现
 *
 * - 实现现象
 *   1. LED0 点亮，等待 5 秒后，开始低功耗测试，这段时间用户可测量运行模式的功耗；
 *   2. 串口输出"enter sleep!"，熄灭 LED0，启动定时器，
 *      进入睡眠模式后，J-Link 调试断开，此时用户可测量睡眠模式的功耗；
 *   3. 等待定时时间到，MCU 被唤醒，串口输出"wake_up!"，点亮 LED0，然后重新进入
 *      睡眠模式。
 *
 * \note
 *   由于 TIM4 默认初始化并作为系统滴答使用，会定期产生中断导致唤醒， 测试本例程
 *   之前应将 am_prj_config.h 中的宏 AM_CFG_SYSTEM_TICK_ENABLE、
 *   AM_CFG_SOFTIMER_ENABLE 和   AM_CFG_KEY_GPIO_ENABLE 设置为 0。
 *
 * \par 源代码
 * \snippet demo_zmf237_drv_sleepmode_timer_wake_up.c src_zmf237_drv_sleepmode_timer_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 18-05-16  pea, first implementation
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_zmf237_drv_sleepmode_timer_wake_up
 * \copydoc demo_zmf237_drv_sleepmode_timer_wake_up.c
 */
 
/** [src_zmf237_drv_sleepmode_timer_wake_up] */
#include "ametal.h"
#include "am_int.h"
#include "am_board.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zmf237.h"
#include "am_zmf237_pwr.h"
#include "am_zmf237_clk.h"
#include "am_zmf237_inst_init.h"
#include "zmf237_periph_map.h"
#include "amhw_zlg_uart.h"
#include "demo_am237_core_entries.h"

/** \brief LSI 时钟频率 */
#define    __LSI_CLK    (40000UL)

/**
 * \brief 等待 RTC 外设为空闲状态
 */
am_local void __wait_rtc_idle (void)
{
    while(!amhw_zmf237_rtc_crl_read_statu(ZMF237_RTC, AMHW_ZMF237_RTC_RSF));
    while(!amhw_zmf237_rtc_crl_read_statu(ZMF237_RTC, AMHW_ZMF237_RTC_RTOFF));
}

/**
 * \brief 定时器回调函数
 */
am_local void __rtc_handler (void *p_arg)
{

    /* 等待 RTC 外设为空闲状态 */
    __wait_rtc_idle();
    
    /* 判断中断源 */
    if (amhw_zmf237_rtc_crl_read_statu(ZMF237_RTC, AMHW_ZMF237_RTC_ALRF)) {
        
        /* 等待 RTC 外设为空闲状态 */
        __wait_rtc_idle();
        
        /* 清除中断标志位 */
        amhw_zmf237_rtc_clr_status_clear(ZMF237_RTC, AMHW_ZMF237_RTC_ALRF);
    }
}

/**
 *  \brief 设置 RTC 闹钟时间
 */
am_local void __rtc_alarm_set (uint32_t second)
{
    uint32_t sec = 0;

    /* 等待 RTC 外设为空闲状态 */
    __wait_rtc_idle();

    /* 获取当前 RTC 时间 */
    sec = amhw_zmf237_rtc_cnth_get(ZMF237_RTC) << 16;
    sec |= amhw_zmf237_rtc_cntl_get(ZMF237_RTC);

    sec += second - 1;
    
    /* 等待 RTC 外设为空闲状态 */
    __wait_rtc_idle();
    
    amhw_zmf237_rtc_crl_cnf_enter(ZMF237_RTC); /* 允许配置 */
    amhw_zmf237_rtc_alrl_set(ZMF237_RTC, sec & 0xffff);
    amhw_zmf237_rtc_alrh_set(ZMF237_RTC, sec >> 16);
    amhw_zmf237_rtc_crl_cnf_out(ZMF237_RTC); /* 配置更新 */
}

/** \brief RTC 平台初始化 */
am_local void __rtc_init (void)
{
    
    /* 使能 LSI */
    amhw_zmf237_rcc_lsi_enable();
    while (amhw_zmf237_rcc_lsirdy_read() == AM_FALSE);

    amhw_zmf237_rcc_apb1_enable(AMHW_ZMF237_RCC_APB1_PWR); /* 使能电源时钟 */
    amhw_zmf237_rcc_apb1_enable(AMHW_ZMF237_RCC_APB1_BKP); /* 使能备份时钟 */
    amhw_zlg_pwr_bkp_access_enable(ZMF237_PWR, 1); /* 取消备份域的写保护 */
    amhw_zmf237_rcc_bdcr_bdrst_reset();                    /* 备份区域软件复位 */
    am_udelay(5);
    amhw_zmf237_rcc_bdcr_bdrst_reset_end();                /* 备份域软件复位结束 */
    
    /* RTC 时钟源选择为 LSI */
    amhw_zmf237_rcc_bdcr_rtc_clk_set(AMHW_ZMF237_RTCCLK_LSI);
    am_mdelay(1);
    amhw_zmf237_rcc_bdcr_rtc_enable();                     /* RTC时钟使能 */

    /* 延时等待预分频寄存器稳定 */
    am_mdelay(10);

    /* 等待 RTC 外设为空闲状态 */
    __wait_rtc_idle();

    /* 允许闹钟中断 */
    amhw_zmf237_rtc_crh_allow_int(ZMF237_RTC, AMHW_ZMF237_RTC_ALRIE);

    /* 等待 RTC 外设为空闲状态 */
    __wait_rtc_idle();

    amhw_zmf237_rtc_crl_cnf_enter(ZMF237_RTC); /* 允许配置 RTC */
    amhw_zmf237_rtc_prll_div_write(ZMF237_RTC, (__LSI_CLK - 1) & 0xffff);
    amhw_zmf237_rtc_prlh_div_write(ZMF237_RTC, (__LSI_CLK - 1) >> 16);
    amhw_zmf237_rtc_crl_cnf_out(ZMF237_RTC); /* 配置更新 */

    /* 连接并使能 RTC 闹钟中断 */
    am_int_connect(INUM_RTC, __rtc_handler, NULL);
    am_int_enable(INUM_RTC);
    
}

/**
 * \brief 例程入口
 */
void demo_zmf237_drv_sleepmode_timer_wake_up_entry (void)
{
    uint32_t i;
    
    AM_DBG_INFO("low power test!\r\n");
    am_led_on(LED0);

    /* 初始化 RTC */
    __rtc_init();
    
    /* 初始化 PWR */
    am_zmf237_pwr_inst_init();

    /* 唤醒配置 */
    am_zmf237_wake_up_cfg(AM_ZMF237_PWR_MODE_SLEEP, NULL, NULL);

    for (i = 0; i < 5; i++) {
        am_mdelay(1000);
    }

    while (1) {

        /* 睡眠之前关闭 LED */
        am_led_off(LED0);

        /* 设置定时中断周期为 1S，并启动定时器 */        
        __rtc_alarm_set(1);

        /* 进入睡眠模式 */
        am_zmf237_pwr_mode_into(AM_ZMF237_PWR_MODE_SLEEP);

        AM_DBG_INFO("wake_up!\r\n");

        /* 唤醒之后点亮 LED */
        am_led_on(LED0);
        am_mdelay(10);
    }
}

/** [src_zmf237_drv_sleepmode_timer_wake_up] */

/* end of file */
