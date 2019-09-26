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
 * \brief PWR drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-27  yrh, first implementation
 * \endinternal
 */
#include <am_zlg237_clk.h>
#include <am_zlg237_pwr.h>
#include <amhw_zlg237_exti.h>
#include <amhw_zlg237_rcc.h>
#include <amhw_zlg237_usart.h>
#include "am_int.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_arm_nvic.h"
#include "amhw_zlg_flash.h"

#if   defined ( __CC_ARM )
  #define __ASM            __asm         /*!< asm keyword for ARM Compiler */
  #define __INLINE         __inline      /*!< inline keyword for ARM Compiler */
  #define __STATIC_INLINE  static __inline

#endif
#if defined ( __GNUC__ )
  #define __ASM            __asm         /*!< asm keyword for GNU Compiler */
  #define __INLINE         inline        /*!< inline keyword for GNU Compiler */
  #define __STATIC_INLINE  static inline

#endif
#include "core_cmInstr.h"

#define    __LSI_CLK    (40000UL)

/** \brief 指向 PWR 电源管理设备的指针 */
am_zlg237_pwr_dev_t *__gp_pwr_dev;

/* 内核睡眠函数 */
static void __pwr_cpu_wif (am_bool_t deep)
{
    if (deep) {
        AMHW_ARM_SCB->scr |= 0x04;
    } else {
        AMHW_ARM_SCB->scr &= ~0x04;
    }

    AMHW_ARM_SCB->scr &= ~0x02;

#ifdef __CC_ARM
    __ASM("WFI");
#endif

#ifdef __GNUC__
    __WFI();
#endif

}

/**
 * \brief 默认的中断服务函数
 */
static void __dummy_isr (void *p_arg)
{
    ; /* VOID */
}

#if 0

/**
 * \brief 设置AHB 预分频
 *
 * \param[in]  div ：预分频值
 *
 *       - 0-7 ： 不分频
 *       - 8   ： 2分频
 *       - 9   ： 4分频
 *       - 10  ： 8分频
 *       - 11  ： 16分频
 *       - 12  ： 64分频
 *       - 13  ： 128分频
 *       - 14  ： 256分频
 *       - 15  ： 512分频
 *
 * \return 返回AHB分频值
 *
 */
static uint8_t ___zlg237_rcc_ahb_div_get (void)
{
    return (uint8_t)((AMHW_ZLG237_RCC->cfgr >> 4) & 0xful);
}
#endif /* 0 */

/*
 * \brief AHB 总线时钟频率更新
 */
static void __sys_clk_ahbfrq_update (int clk_id, uint32_t clk)
{
    uint32_t div = 0;

    div = am_zlg237_div_get(clk_id);

    if (div < 8)  {
        am_zlg237_clk_update(clk_id, clk / 1);

    } else if (div == 8){
        am_zlg237_clk_update(clk_id, clk / 2);

    } else if(div == 9){
        am_zlg237_clk_update(clk_id, clk / 4);

    } else if (div == 10) {
        am_zlg237_clk_update(clk_id, clk / 8);
    } else if (div == 11) {
        am_zlg237_clk_update(clk_id, clk / 16);
    } else if (div == 12) {
        am_zlg237_clk_update(clk_id, clk / 64);
    } else if (div == 13) {
        am_zlg237_clk_update(clk_id, clk / 128);
    } else if (div == 14) {
        am_zlg237_clk_update(clk_id, clk / 256);
    } else {
        am_zlg237_clk_update(clk_id, clk / 512);
    }
}

/**
 * \brief 不同功耗模式下时钟转变，支持停止模式与正常模式的 PLL 时钟源的转换
 */
static void __sys_clk_change (uint8_t pwr_mode)
{
    const am_zlg237_clk_dev_t *p_clk_dev = am_zlg237_clk_devinfo_get();

    uint32_t apb1_div = 0, apb2_div = 0;
    uint32_t i = 0;

    /* 计算 APB1 时钟频率 */
    apb1_div = 1;
    for (i = 0; i < (p_clk_dev->p_devinfo->apb1_div & 0x7); i++) {
        apb1_div = apb1_div << 1;
    }

    /* 计算 APB2 时钟频率 */
    apb2_div = 1;
    for (i = 0; i < (p_clk_dev->p_devinfo->apb2_div & 0x7); i++) {
        apb2_div = apb2_div << 1;
    }

    if (pwr_mode == AM_ZLG237_PWR_MODE_STOP) {

        /* 停机模式使能 LSI 作为系统时钟 */
        amhw_zlg237_rcc_lsi_enable();

        /* 等待 LSI 时钟就绪 */
        while (amhw_zlg237_rcc_lsirdy_read() == AM_FALSE);

        /* 切换时钟 */
        amhw_zlg237_rcc_sys_clk_set(AMHW_ZLG237_SYSCLK_LSI_OR_LSE);

        /* 更新 AHB、APB1、APB2 总线桥的时钟频率 */
        __sys_clk_ahbfrq_update(__gp_pwr_dev->p_pwrdevinfo->ahb_clk_num,
                                __LSI_CLK);
        am_zlg237_clk_update(__gp_pwr_dev->p_pwrdevinfo->apb1_clk_num,
                             p_clk_dev->ahb_clk  / apb1_div);
        am_zlg237_clk_update(__gp_pwr_dev->p_pwrdevinfo->apb2_clk_num,
                             p_clk_dev->ahb_clk  / apb2_div);
    }

    if (pwr_mode == AM_ZLG237_PWR_MODE_RUN) {

        if (p_clk_dev->p_devinfo->pllin_src == AMHW_ZLG237_PLLCLK_HSE) {
            amhw_zlg237_rcc_hseon_enable ();
            while (amhw_zlg237_rcc_hserdy_read () == AM_FALSE);
        } else {
            amhw_zlg237_rcc_hsion_enable();
            while (amhw_zlg237_rcc_hsirdy_read () == AM_FALSE);
        }

        amhw_zlg237_rcc_pll_enable();
        while (amhw_zlg237_rcc_pllrdy_read() == AM_FALSE);

        /* 系统时钟选为 PLL */
        amhw_zlg237_rcc_sys_clk_set(AMHW_ZLG237_SYSCLK_PLL);

        /* 在正常模式下禁能 LSI 作为系统时钟 */
        amhw_zlg237_rcc_lsi_disable();

        /* 更新 AHB、APB1、APB2 总线桥的时钟频率 */
        __sys_clk_ahbfrq_update(__gp_pwr_dev->p_pwrdevinfo->ahb_clk_num,
                                p_clk_dev->pllout_clk);
        am_zlg237_clk_update(__gp_pwr_dev->p_pwrdevinfo->apb1_clk_num,
                             p_clk_dev->ahb_clk  / apb1_div);
        am_zlg237_clk_update(__gp_pwr_dev->p_pwrdevinfo->apb2_clk_num,
                             p_clk_dev->ahb_clk  / apb2_div);
    }

    if (pwr_mode == AM_ZLG237_PWR_MODE_STANBY) {

        /* 待机模式使能 HSI 作为系统时钟 */
        amhw_zlg237_rcc_hsion_enable();

        /* 等待 HSI 时钟就绪 */
        while (amhw_zlg237_rcc_hsirdy_read() == AM_FALSE);

        /* 系统时钟选为 HSI */
        amhw_zlg237_rcc_sys_clk_set(AMHW_ZLG237_SYSCLK_HSI);

        /* 失能 PLL */
        amhw_zlg237_rcc_pll_disable();

        /* 失能 LSI */
        amhw_zlg237_rcc_lsi_disable();

        /* 更新 AHB、APB1、APB2 总线桥的时钟频率 */
        __sys_clk_ahbfrq_update(__gp_pwr_dev->p_pwrdevinfo->ahb_clk_num,
                                p_clk_dev->ahb_clk);
        am_zlg237_clk_update(
            __gp_pwr_dev->p_pwrdevinfo->apb1_clk_num,
            p_clk_dev->ahb_clk / p_clk_dev->p_devinfo->apb1_div);
        am_zlg237_clk_update(
            __gp_pwr_dev->p_pwrdevinfo->apb2_clk_num,
             p_clk_dev->ahb_clk / p_clk_dev->p_devinfo->apb2_div);
    }
}

/**
 * \brief PWR 初始化
 */
am_zlg237_pwr_handle_t am_zlg237_pwr_init (
    am_zlg237_pwr_dev_t           *p_dev,
    const am_zlg237_pwr_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
           return NULL;
    }

    /* PWR 平台初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_pwrdevinfo = p_devinfo;

    /* 取得当前的 PWR 模式，初始化默认为运行模式 */
    p_dev->pwr_mode  = AM_ZLG237_PWR_MODE_RUN;

    __gp_pwr_dev = p_dev;

    return p_dev;
}

/**
 * \brief 解除 PWR 初始化
 */
void am_zlg237_pwr_deinit (void)
{
    const am_zlg237_pwr_devinfo_t *p_pwr_devinfo = __gp_pwr_dev->p_pwrdevinfo;

    if (__gp_pwr_dev == NULL || p_pwr_devinfo== NULL) {
        return;
    }

    __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_RUN;
    __gp_pwr_dev->p_pwrdevinfo = NULL;

    __gp_pwr_dev = NULL;

    /* PWR 平台解初始化 */
    if (p_pwr_devinfo->pfn_plfm_deinit) {
        p_pwr_devinfo->pfn_plfm_deinit();
    }
}

/**
 * \brief 配置系统模式
 *
 * \note 这个唤醒的配置的 PWR 模式必须与 am_zlg237_pwr_mode_into 函数的选择 PWR
 *       模式相同，在 AM_ZLG237_PWR_MODE_STANBY 模式下，唤醒会复位，注册中断唤醒
 *       函数回调函数不起作用
 */
void am_zlg237_wake_up_cfg (am_zlg237_pwr_mode_t mode,
                            am_pfnvoid_t         pfn_callback,
                            void                *p_arg)
{
    am_zlg237_pwr_mode_t cur_mode = am_zlg237_pwr_mode_get();

    if (cur_mode == mode) {
        return ;
    }

    switch (mode) {

    case AM_ZLG237_PWR_MODE_RUN:    /* 运行模式 */

        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_RUN;

        break;

    case AM_ZLG237_PWR_MODE_SLEEP:  /* 睡眠运行模式 */

        if (pfn_callback != NULL) {

            /* 连接引脚中断服务函数 */
            am_gpio_trigger_connect(
                __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin,
                pfn_callback,
                p_arg);

            /* 配置引脚中断触发方式 */
            am_gpio_trigger_cfg(
                __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin,
                AM_GPIO_TRIGGER_RISE);

            /* 使能引脚触发中断 */
            am_gpio_trigger_on(
                __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin);

        }

        break;

    case AM_ZLG237_PWR_MODE_STOP:  /* 停止模式 */

        if (pfn_callback != NULL) {

            /* 连接引脚中断服务函数 */
            am_gpio_trigger_connect(
                __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin,
                pfn_callback,
                p_arg);

            /* 配置引脚中断触发方式 */
            am_gpio_trigger_cfg(
                __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin,
                AM_GPIO_TRIGGER_RISE);

            /* 使能引脚触发中断 */
            am_gpio_trigger_on(
                __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin);

        }

        break;

    case AM_ZLG237_PWR_MODE_STANBY:  /* 待机模式 */

        if (-1 == __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin) {
            break;
        }

        /* 连接引脚中断服务函数 */
        am_gpio_trigger_connect(
            __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin,
            __dummy_isr,
            p_arg);

        /* 配置引脚中断触发方式 */
        am_gpio_trigger_cfg(
            __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin,
            AM_GPIO_TRIGGER_RISE);

        /* 使能引脚触发中断 */
        am_gpio_trigger_on(
            __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin);

        break;

    default:
        break;
    }
}

/**
 * \brief 配置系统模式
 */
int am_zlg237_pwr_mode_into (am_zlg237_pwr_mode_t mode)
{
    amhw_zlg237_pwr_t   *p_hw_pwr = NULL;
    am_zlg237_pwr_mode_t cur_mode = am_zlg237_pwr_mode_get();

    int wkup_pin = __gp_pwr_dev->p_pwrdevinfo->p_pwr_mode[mode - 1].pin;

    if (cur_mode == mode) {
        return AM_OK;
    }

    p_hw_pwr = (amhw_zlg237_pwr_t *)__gp_pwr_dev->p_pwrdevinfo->pwr_regbase;

    switch (mode) {

    case AM_ZLG237_PWR_MODE_RUN:    /* 运行模式 */

        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_RUN;

        break;

    case AM_ZLG237_PWR_MODE_SLEEP:  /* 睡眠模式 */
        if (cur_mode != AM_ZLG237_PWR_MODE_RUN) {
            return -AM_EPERM;
        }
        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_SLEEP;

        AM_DBG_INFO("enter sleep!\r\n");

        /* 等待串口发送完成 */
        while((ZLG237_USART1->sr & AMHW_ZLG237_USART_TX_COMPLETE_FLAG) ==
              AM_FALSE);

        __pwr_cpu_wif(AM_FALSE);

        /* 唤醒重置模式 */
        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_RUN;

        break;

    case AM_ZLG237_PWR_MODE_STOP:  /* 停止模式 */
        if (cur_mode != AM_ZLG237_PWR_MODE_RUN) {
            return -AM_EPERM;
        }
        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_STOP;

        /* 注意： 电压调节器可能开启，进入停机状态并没有关停 */
        amhw_zlg237_pwr_pdds_lpds_mode_set(
            p_hw_pwr, AM_ZLG237_PDDS_STOP_MODE_LDO_OFF);

        AM_DBG_INFO("enter deepsleep!\r\n");

        /* 等待串口发送完成 */
        while((ZLG237_USART1->sr & AMHW_ZLG237_USART_TX_COMPLETE_FLAG) ==
              AM_FALSE);

        /* CPU进入深度睡眠模式 */
        __pwr_cpu_wif(AM_TRUE);

        /* 停止模式唤醒后恢复时钟 */
        __sys_clk_change(AM_ZLG237_PWR_MODE_RUN);

        /* 唤醒重置模式 */
        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_RUN;

        break;

    case AM_ZLG237_PWR_MODE_STANBY:  /* 待机模式 */
        if (cur_mode != AM_ZLG237_PWR_MODE_RUN) {
            return -AM_EPERM;
        }
        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_STANBY;

        /* 进入待机模式之前 WKUP 引脚必须为低电平 */
        if (wkup_pin != -1) {

            /* 失能 WKUP 引脚 */
            amhw_zlg237_wake_up_enable(p_hw_pwr, AM_ZLG237_WAKEUP_DISABLE);

            /* 将 WKUP 配置为输入，并检测是否为高电平 */
            am_gpio_pin_cfg(wkup_pin, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);
            if (am_gpio_get(wkup_pin)) {
                return -AM_ENOTSUP;
            }

            /* 使能 WAKE_UP 引脚 */
            amhw_zlg237_wake_up_enable(p_hw_pwr, AM_ZLG237_WAKEUP_ENABLE);
        }

        amhw_zlg237_pwr_stauts_flag_clear(p_hw_pwr, AM_ZLG237_WAKEUP_FLAG_CLEAR);

        amhw_zlg237_pwr_pdds_lpds_mode_set(
            p_hw_pwr, AM_ZLG237_PDDS_STANDBY_MODE_LDO_ON);

        AM_DBG_INFO("enter standby!\r\n");

        /* 等待串口发送完成 */
        while((ZLG237_USART1->sr & AMHW_ZLG237_USART_TX_COMPLETE_FLAG) ==
              AM_FALSE);

        /* 待机模式使用 HSI 作为系统时钟 */
        __sys_clk_change(AM_ZLG237_PWR_MODE_STANBY);

        /* CPU 进入待机模式 */
        __pwr_cpu_wif(AM_TRUE);

        /* 恢复时钟源，不应该执行到这里，因为待机模式唤醒之后芯片会直接复位 */
        __sys_clk_change(AM_ZLG237_PWR_MODE_RUN);

        /* 唤醒重置模式 */
        __gp_pwr_dev->pwr_mode = AM_ZLG237_PWR_MODE_RUN;

        break;

    default:

        break;
    }

    return AM_OK;
}

/**
 * \brief 获取 PWR 模式
 */
am_zlg237_pwr_mode_t am_zlg237_pwr_mode_get (void)
{

    /* 低功耗运行模式 */
    return __gp_pwr_dev->pwr_mode;
}

/**
 * \brief 配置 PVD 电压检测信息
 */
int am_zlg237_pwr_pvd_cfg (am_zlg237_pwr_handle_t pwr_handle,
                           am_pfnvoid_t           pfn_callback,
                           void                  *p_arg)
{
    amhw_zlg237_pwr_t   *p_hw_pwr = NULL;
    am_zlg237_pwr_mode_t cur_mode = am_zlg237_pwr_mode_get();

    if(pwr_handle == NULL) {
        return -AM_EINVAL;
    }

    /* 待机模式及停止模式由 CPU 内核停止不支持 PVD 电压检测 */
    if (cur_mode == AM_ZLG237_PWR_MODE_STOP  ||
        cur_mode == AM_ZLG237_PWR_MODE_STANBY ) {

        return AM_ERROR;
    }

    p_hw_pwr = (amhw_zlg237_pwr_t *)__gp_pwr_dev->p_pwrdevinfo->pwr_regbase;

    if (pwr_handle->p_pwrdevinfo->p_pvd_info->enable) {

        amhw_zlg237_pwr_pvd_detect_enable(p_hw_pwr, AM_ZLG237_PVDE_ENABLE);
        amhw_zlg237_pvd_lever_set(
            p_hw_pwr,pwr_handle->p_pwrdevinfo->p_pvd_info->pvd_v_level);

        /* 清除中断线配置，PVD 对应于 AMHW_ZLG237_LINE_NUM16 */
        amhw_zlg237_exti_imr_clear(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);
        amhw_zlg237_exti_emr_clear(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);

        /* 清除中断线触发方式配置，PVD 对应于 AMHW_ZLG237_LINE_NUM16 */
        amhw_zlg237_exti_rtsr_clear(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);
        amhw_zlg237_exti_ftsr_clear(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);

        if (pwr_handle->p_pwrdevinfo->p_pvd_info->pvd_mode == 0) {

            /* 上升沿触发(表示电压从高下降到低于设定阀值时产生中断)，
             * PVD 对应于 AMHW_ZLG237_LINE_NUM16
             */
            amhw_zlg237_exti_rtsr_set(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);
        } else if (pwr_handle->p_pwrdevinfo->p_pvd_info->pvd_mode == 1) {

            /* 下降沿触发(表示电压从低上升到高于设定阀值时产生中断)，
             * PVD 对应于 AMHW_ZLG237_LINE_NUM16
             */
            amhw_zlg237_exti_ftsr_set(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);
        } else {

            /* 双边沿触发(表示电压上升或下降越过设定阀值时都产生中断).
             * PVD对应于 AMHW_ZLG237_LINE_NUM16
             */
            amhw_zlg237_exti_rtsr_set(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);
            amhw_zlg237_exti_ftsr_set(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);
        }

        /* 清 PVD 中断线标志位，PVD 对应于 AMHW_ZLG237_LINE_NUM16 */
        amhw_zlg237_exti_pending_clear(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);

        /* 设置开放中断线请求 , PVD 对应于 AMHW_ZLG237_LINE_NUM16 */
        amhw_zlg237_exti_imr_set(ZLG237_EXTI,  AMHW_ZLG237_LINE_NUM16);

        /*
         * 连接用户注册的中断回调函数
         */
        if (pfn_callback != NULL) {

            /* 中断连接并使能 */
            am_int_connect(__gp_pwr_dev->p_pwrdevinfo->inum,
                           pfn_callback,
                           (void *)p_arg);
            am_int_enable(__gp_pwr_dev->p_pwrdevinfo->inum);

        }

    } else {

        /* 清 PVD 中断线标志位，PVD 对应于 AMHW_ZLG237_LINE_NUM16 */
        amhw_zlg237_exti_pending_clear(ZLG237_EXTI, AMHW_ZLG237_LINE_NUM16);

        /* 设置禁止中断线请求 , PVD 对应于 AMHW_ZLG237_LINE_NUM16 */
        amhw_zlg237_exti_imr_clear(ZLG237_EXTI,  AMHW_ZLG237_LINE_NUM16);

        /* 禁能 NVIC 中断 */
        am_int_disable(__gp_pwr_dev->p_pwrdevinfo->inum);
    }

    return AM_OK;
}

/* end of file */
