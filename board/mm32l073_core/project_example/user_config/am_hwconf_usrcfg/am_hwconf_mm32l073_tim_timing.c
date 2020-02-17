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
 * \brief MM32L073 TIM 定时功能用户配置文件
 * \sa am_hwconf_mm32l073_tim_timing.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-04-21  nwt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_mm32l073.h"
#include "am_mm32l073_clk.h"
#include "am_mm32_tim_timing.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l073_tim_timing
 * \copydoc am_hwconf_mm32l073_tim_timing.c
 * @{
 */

/*******************************************************************************
 * TIM1 配置
 ******************************************************************************/

/** \brief TIM1 平台初始化 */
void __mm32_plfm_tim1_timing_init (void)
{

    /* 使能高级定时器1时钟 */
    am_clk_enable(CLK_TIM1);

    /* 复位高级定时器1 */
    am_mm32l073_clk_reset(CLK_TIM1);
}

/** \brief 解除 TIM1 平台初始化 */
void __mm32_plfm_tim1_timing_deinit (void)
{

    /* 复位高级定时器1 */
    am_mm32l073_clk_reset(CLK_TIM1);

    /* 禁能高级定时器1时钟 */
    am_clk_disable(CLK_TIM1);
}


/** \brief TIM1 设备信息 */
const am_mm32_tim_timing_devinfo_t  __g_tim1_timing_devinfo = {
    MM32L073_TIM1_BASE,                /**< \brief TIM1寄存器块的基地址 */
    INUM_TIM1_BRK_UP_TRG_COM,        /**< \brief TIM1中断编号 */
    CLK_TIM1,                        /**< \brief TIM1时钟ID */
    AMHW_MM32_TIM_TYPE0,              /**< \brief 定时器类型 */
    __mm32_plfm_tim1_timing_init,     /**< \brief 平台初始化函数 */
    __mm32_plfm_tim1_timing_deinit    /**< \brief 平台解析初始化函数 */
};

/** \brief TIM1 设备定义 */
am_mm32_tim_timing_dev_t  __g_tim1_timing_dev;

/** \brief 定时器TIM1 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_mm32l073_tim1_timing_inst_init (void)
{
    return am_mm32_tim_timing_init(&__g_tim1_timing_dev,
                                  &__g_tim1_timing_devinfo);
}

/** \brief TIM1 Timing实例解初始化 */
void am_mm32l073_tim1_timing_inst_deinit (am_timer_handle_t handle)
{
    am_mm32_tim_timing_deinit(handle);
}

/*******************************************************************************
 * TIM2 配置
 ******************************************************************************/

/** \brief TIM2 平台初始化 */
void __mm32_plfm_tim2_timing_init (void)
{

    /* 使能通用定时器2时钟 */
    am_clk_enable(CLK_TIM2);

    /* 复位通用定时器2 */
    am_mm32l073_clk_reset(CLK_TIM2);
}

/** \brief 解除 TIM2 平台初始化 */
void __mm32_plfm_tim2_timing_deinit (void)
{

    /* 复位通用定时器2 */
    am_mm32l073_clk_reset(CLK_TIM2);

    /* 禁能通用定时器2时钟 */
    am_clk_disable(CLK_TIM2);
}

/** \brief TIM2 设备信息 */
const am_mm32_tim_timing_devinfo_t  __g_tim2_timing_devinfo = {
    MM32L073_TIM2_BASE,                /**< \brief TIM2寄存器块的基地址 */
    INUM_TIM2,                       /**< \brief TIM2中断编号 */
    CLK_TIM2,                        /**< \brief TIM2时钟ID */
    AMHW_MM32_TIM_TYPE1,              /**< \brief 定时器类型 */
    __mm32_plfm_tim2_timing_init,     /**< \brief 平台初始化函数 */
    __mm32_plfm_tim2_timing_deinit    /**< \brief 平台解析初始化函数 */
};

/** \brief TIM2 设备定义 */
am_mm32_tim_timing_dev_t  __g_tim2_timing_dev;

/** \brief TIM2 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_mm32l073_tim2_timing_inst_init (void)
{
    return am_mm32_tim_timing_init(&__g_tim2_timing_dev,
                                  &__g_tim2_timing_devinfo);
}

/** \brief TIM2 Timing实例解初始化 */
void am_mm32l073_tim2_timing_inst_deinit (am_timer_handle_t handle)
{
    am_mm32_tim_timing_deinit(handle);
}


/*******************************************************************************
 * TIM3配置
 ******************************************************************************/
/** \brief TIM3平台初始化 */
void __mm32_plfm_tim3_timing_init (void)
{

    /* 使能通用定时器3时钟 */
    am_clk_enable(CLK_TIM3);

    /* 复位通用定时器3 */
    am_mm32l073_clk_reset(CLK_TIM3);
}

/** \brief 解除TIM3平台初始化 */
void __mm32_plfm_tim3_timing_deinit (void)
{

    /* 复位通用定时器3 */
    am_mm32l073_clk_reset(CLK_TIM3);

    /* 禁能通用定时器3时钟 */
    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3 设备信息 */
const am_mm32_tim_timing_devinfo_t  __g_tim3_timing_devinfo = {
    MM32L073_TIM3_BASE,                /**< \brief TIM3寄存器块的基地址 */
    INUM_TIM3,                       /**< \brief TIM3中断编号 */
    CLK_TIM3,                        /**< \brief TIM3时钟ID */
    AMHW_MM32_TIM_TYPE2,              /**< \brief 定时器类型 */
    __mm32_plfm_tim3_timing_init,     /**< \brief 平台初始化函数 */
    __mm32_plfm_tim3_timing_deinit    /**< \brief 平台解初始化函数 */
};

/** \brief TIM3设备定义 */
am_mm32_tim_timing_dev_t  __g_tim3_timing_dev;

/** \brief TIM3 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_mm32l073_tim3_timing_inst_init (void)
{
    return am_mm32_tim_timing_init(&__g_tim3_timing_dev,
                                  &__g_tim3_timing_devinfo);
}

/** \brief TIM3 Timing实例解初始化 */
void am_mm32l073_tim3_timing_inst_deinit (am_timer_handle_t handle)
{
    am_mm32_tim_timing_deinit(handle);
}

/*******************************************************************************
 * TIM14配置
 ******************************************************************************/
/** \brief TIM14平台初始化 */
void __mm32_plfm_tim14_timing_init (void)
{

    /* 使能通用定时器14时钟 */
    am_clk_enable(CLK_TIM14);

    /* 复位通用定时器14 */
    am_mm32l073_clk_reset(CLK_TIM14);
}

/** \brief 解除TIM14平台初始化 */
void __mm32_plfm_tim14_timing_deinit (void)
{

    /* 复位通用定时器14 */
    am_mm32l073_clk_reset(CLK_TIM14);

    /* 禁能通用定时器14时钟 */
    am_clk_disable(CLK_TIM14);
}

/** \brief TIM14设备信息 */
const am_mm32_tim_timing_devinfo_t  __g_tim14_timing_devinfo = {
    MM32L073_TIM14_BASE,                /**< \brief TIM14寄存器块的基地址 */
    INUM_TIM14,                       /**< \brief TIM14中断编号 */
    CLK_TIM14,                        /**< \brief TIM14时钟ID */
    AMHW_MM32_TIM_TYPE3,               /**< \brief 定时器类型 */
    __mm32_plfm_tim14_timing_init,     /**< \brief 平台初始化函数 */
    __mm32_plfm_tim14_timing_deinit    /**< \brief 平台解初始化函数 */
};

/** \brief TIM14设备定义 */
am_mm32_tim_timing_dev_t  __g_tim14_timing_dev;

/** \brief TIM14 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_mm32l073_tim14_timing_inst_init (void)
{
    return am_mm32_tim_timing_init(&__g_tim14_timing_dev,
                                  &__g_tim14_timing_devinfo);
}

/** \brief TIM14 Timing实例解初始化 */
void am_mm32l073_tim14_timing_inst_deinit (am_timer_handle_t handle)
{
    am_mm32_tim_timing_deinit(handle);
}

/*******************************************************************************
 * TIM16配置
 ******************************************************************************/

/** \brief TIM16平台初始化 */
void __mm32_plfm_tim16_timing_init (void)
{

    /* 使能通用定时器16时钟 */
    am_clk_enable(CLK_TIM16);

    /* 复位通用定时器16 */
    am_mm32l073_clk_reset(CLK_TIM16);
}

/** \brief 解除TIM16平台初始化 */
void __mm32_plfm_tim16_timing_deinit (void)
{

    /* 复位通用定时器16 */
    am_mm32l073_clk_reset(CLK_TIM16);

    /* 禁能通用定时器16时钟 */
    am_clk_disable(CLK_TIM16);
}

/** \brief TIM16设备信息 */
const am_mm32_tim_timing_devinfo_t  __g_tim16_timing_devinfo = {
    MM32L073_TIM16_BASE,                /**< \brief TIM16寄存器块的基地址 */
    INUM_TIM16,                       /**< \brief TIM16中断编号 */
    CLK_TIM16,                        /**< \brief TIM16时钟ID */
    AMHW_MM32_TIM_TYPE3,               /**< \brief 定时器类型 */
    __mm32_plfm_tim16_timing_init,     /**< \brief 平台初始化函数 */
    __mm32_plfm_tim16_timing_deinit    /**< \brief 平台解初始化函数 */
};

/** \brief TIM16设备定义 */
am_mm32_tim_timing_dev_t  __g_tim16_timing_dev;

/** \brief 定时器TIM16 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_mm32l073_tim16_timing_inst_init (void)
{
    return am_mm32_tim_timing_init(&__g_tim16_timing_dev,
                                  &__g_tim16_timing_devinfo);
}

/** \brief TIM16 Timing实例解初始化 */
void am_mm32l073_tim16_timing_inst_deinit (am_timer_handle_t handle)
{
    am_mm32_tim_timing_deinit(handle);
}

/*******************************************************************************
 * TIM17配置
 ******************************************************************************/

/** \brief TIM17平台初始化 */
void __mm32_plfm_tim17_timing_init (void)
{

    /* 使能通用定时器17时钟 */
    am_clk_enable(CLK_TIM17);

    /* 复位通用定时器17 */
    am_mm32l073_clk_reset(CLK_TIM17);
}

/** \brief 解除TIM17平台初始化 */
void __mm32_plfm_tim17_timing_deinit (void)
{

    /* 复位通用定时器17 */
    am_mm32l073_clk_reset(CLK_TIM17);

    /* 禁能通用定时器17时钟 */
    am_clk_disable(CLK_TIM17);
}

/** \brief TIM17设备信息 */
const am_mm32_tim_timing_devinfo_t  __g_tim17_timing_devinfo = {
    MM32L073_TIM17_BASE,                /**< \brief TIM17寄存器块的基地址 */
    INUM_TIM17,                       /**< \brief TIM17中断编号 */
    CLK_TIM17,                        /**< \brief TIM17时钟ID */
    AMHW_MM32_TIM_TYPE3,               /**< \brief 定时器类型 */
    __mm32_plfm_tim17_timing_init,     /**< \brief 平台初始化函数 */
    __mm32_plfm_tim17_timing_deinit    /**< \brief 平台解初始化函数 */
};

/** \brief TIM17设备定义 */
am_mm32_tim_timing_dev_t  __g_tim17_timing_dev;

/** \brief 定时器TIM17 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_mm32l073_tim17_timing_inst_init (void)
{
    return am_mm32_tim_timing_init(&__g_tim17_timing_dev,
                                     &__g_tim17_timing_devinfo);
}

/** \brief TIM17 Timing实例解初始化 */
void am_mm32l073_tim17_timing_inst_deinit (am_timer_handle_t handle)
{
    am_mm32_tim_timing_deinit(handle);
}

/**
 * @}
 */

/* end of file */
