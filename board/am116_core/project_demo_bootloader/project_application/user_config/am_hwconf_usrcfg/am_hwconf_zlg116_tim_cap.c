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
 * \brief ZLG116 TIM用于捕获功能的用户配置文件
 * \sa am_hwconf_zlg116_tim_cap.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-04-21  nwt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg116.h"
#include "am_zlg_tim_cap.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_zlg116_clk.h"

/**
 * \addtogroup am_if_src_hwconf_zlg116_tim_cap
 * \copydoc am_hwconf_zlg116_tim_cap.c
 * @{
 */
 
/*******************************************************************************
 * TIM1 配置
 ******************************************************************************/

/** \brief TIM1用于捕获功能的平台初始化 */
void __zlg_plfm_tim1_cap_init (void)
{

    /* 使能高级定时器1时钟 */
    am_clk_enable(CLK_TIM1);

    /* 复位高级定时器1 */
    am_zlg116_clk_reset(CLK_TIM1);
}

/** \brief 解除TIM1平台初始化 */
void __zlg_plfm_tim1_cap_deinit (void)
{

    /* 复位高级定时器1 */
    am_zlg116_clk_reset(CLK_TIM1);

    /* 禁能高级定时器1时钟 */
    am_clk_disable(CLK_TIM1);
}

/** \brief TIM1用于捕获功能的引脚配置信息列表 */
am_zlg_tim_cap_ioinfo_t __g_tim1_cap_ioinfo_list[] = {

    /** \brief 通道1 */
    {PIOA_8,  PIOA_8_TIM1_CH1  | PIOA_8_INPUT_FLOAT,  PIOA_8_GPIO  | PIOA_8_INPUT_FLOAT},

    /** \brief 通道2 */
    {PIOA_9,  PIOA_9_TIM1_CH2  | PIOA_9_INPUT_FLOAT,  PIOA_9_GPIO  | PIOA_9_INPUT_FLOAT},

    /** \brief 通道3 */
    {PIOA_10, PIOA_10_TIM1_CH3 | PIOA_10_INPUT_FLOAT, PIOA_10_GPIO | PIOA_10_INPUT_FLOAT},

    /** \brief 通道4 */
    {PIOA_11, PIOA_11_TIM1_CH4 | PIOA_11_INPUT_FLOAT, PIOA_11_GPIO | PIOA_11_INPUT_FLOAT}
};

/** \brief TIM1用于捕获功能的设备信息 */
const am_zlg_tim_cap_devinfo_t  __g_tim1_cap_devinfo = {
    ZLG116_TIM1_BASE,               /**< \brief TIM1寄存器块的基地址 */
    INUM_TIM1_CC,                   /**< \brief TIM1中断编号 */
    CLK_TIM1,                       /**< \brief TIM1时钟ID */
    4,                              /**< \brief 4个捕获通道 */
    &__g_tim1_cap_ioinfo_list[0],   /**< \brief 引脚配置信息列表 */
    AMHW_ZLG_TIM_TYPE0,             /**< \brief 定时器类型 */
    __zlg_plfm_tim1_cap_init,       /**< \brief 平台初始化函数 */
    __zlg_plfm_tim1_cap_deinit      /**< \brief 平台解初始化函数 */
};

/** \brief TIM1用于捕获功能的设备定义 */
am_zlg_tim_cap_dev_t  __g_tim1_cap_dev;

/** \brief tim1 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_zlg116_tim1_cap_inst_init (void)
{
    return am_zlg_tim_cap_init(&__g_tim1_cap_dev,
                                  &__g_tim1_cap_devinfo);
}

/** \brief tim1 cap实例解初始化 */
void am_zlg116_tim1_cap_inst_deinit (am_cap_handle_t handle)
{
    am_zlg_tim_cap_deinit(handle);
}

/*******************************************************************************
 * TIM2 配置
 ******************************************************************************/

/** \brief TIM2用于捕获功能的平台初始化 */
void __zlg_plfm_tim2_cap_init (void)
{

    /* 使能通用定时器2时钟 */
    am_clk_enable(CLK_TIM2);

    /* 复位通用定时器2 */
    am_zlg116_clk_reset(CLK_TIM2);
}

/** \brief 解除TIM2平台初始化 */
void __zlg_plfm_tim2_cap_deinit (void)
{

    /* 复位通用定时器2 */
    am_zlg116_clk_reset(CLK_TIM2);

    /* 禁能通用定时器2时钟 */
    am_clk_disable(CLK_TIM2);
}

/** \brief TIM2用于捕获功能的引脚配置信息列表 */
am_zlg_tim_cap_ioinfo_t __g_tim2_cap_ioinfo_list[] = {

    /** \brief 通道1 */
    {PIOA_0, PIOA_0_TIM2_CH1_ETR | PIOA_0_INPUT_FLOAT, PIOA_0_GPIO | PIOA_0_INPUT_FLOAT},

    /** \brief 通道2 */
    {PIOA_1, PIOA_1_TIM2_CH2     | PIOA_1_INPUT_FLOAT, PIOA_1_GPIO | PIOA_1_INPUT_FLOAT},

    /** \brief 通道2 */
    {PIOA_2, PIOA_2_TIM2_CH3     | PIOA_2_INPUT_FLOAT, PIOA_2_GPIO | PIOA_2_INPUT_FLOAT},

    /** \brief 通道2 */
    {PIOA_3, PIOA_3_TIM2_CH4     | PIOA_3_INPUT_FLOAT, PIOA_3_GPIO | PIOA_3_INPUT_FLOAT},
};

/** \brief TIM2用于捕获功能的设备信息 */
const am_zlg_tim_cap_devinfo_t  __g_tim2_cap_devinfo = {
    ZLG116_TIM2_BASE,               /**< \brief TIM2寄存器块的基地址 */
    INUM_TIM2,                      /**< \brief TIM2中断编号 */
    CLK_TIM2,                       /**< \brief TIM2时钟ID */
    4,                              /**< \brief 4个捕获通道 */
    &__g_tim2_cap_ioinfo_list[0],   /**< \brief 引脚配置信息列表 */
    AMHW_ZLG_TIM_TYPE1,             /**< \brief 定时器类型 */
    __zlg_plfm_tim2_cap_init,       /**< \brief 平台初始化函数 */
    __zlg_plfm_tim2_cap_deinit      /**< \brief 平台解初始化函数 */
};

/** \brief TIM2用于捕获功能的设备定义 */
am_zlg_tim_cap_dev_t  __g_tim2_cap_dev;

/** \brief tim2 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_zlg116_tim2_cap_inst_init (void)
{
    return am_zlg_tim_cap_init(&__g_tim2_cap_dev,
                                  &__g_tim2_cap_devinfo);
}

/** \brief tim2 cap实例解初始化 */
void am_zlg116_tim2_cap_inst_deinit (am_cap_handle_t handle)
{
    am_zlg_tim_cap_deinit(handle);
}

/*******************************************************************************
 * TIM3 配置
 ******************************************************************************/

/** \brief TIM3用于捕获功能的平台初始化 */
void __zlg_plfm_tim3_cap_init (void)
{

    /* 使能通用定时器3时钟 */
    am_clk_enable(CLK_TIM3);

    /* 复位通用定时器3 */
    am_zlg116_clk_reset(CLK_TIM3);
}

/** \brief 解除TIM3平台初始化 */
void __zlg_plfm_tim3_cap_deinit (void)
{

    /* 复位通用定时器3 */
    am_zlg116_clk_reset(CLK_TIM3);

    /* 禁能通用定时器3时钟 */
    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3用于捕获功能的引脚配置信息列表 */
am_zlg_tim_cap_ioinfo_t __g_tim3_cap_ioinfo_list[] = {

    /** \brief 通道1 */
    {PIOB_4,  PIOB_4_TIM3_CH1 | PIOB_4_INPUT_FLOAT, PIOB_4_GPIO | PIOB_4_INPUT_FLOAT},

    /** \brief 通道2 */
    {PIOB_5,  PIOB_5_TIM3_CH2 | PIOB_5_INPUT_FLOAT, PIOB_5_GPIO | PIOB_5_INPUT_FLOAT},

    /** \brief 通道3 */
    {PIOB_0,  PIOB_0_TIM3_CH3 | PIOB_0_INPUT_FLOAT, PIOB_0_GPIO | PIOB_0_INPUT_FLOAT},

    /** \brief 通道4 */
    {PIOB_1,  PIOB_1_TIM3_CH4 | PIOB_1_INPUT_FLOAT, PIOB_1_GPIO | PIOB_1_INPUT_FLOAT},
};

/** \brief TIM3用于捕获功能的设备信息 */
const am_zlg_tim_cap_devinfo_t  __g_tim3_cap_devinfo = {
    ZLG116_TIM3_BASE,               /**< \brief TIM3寄存器块的基地址 */
    INUM_TIM3,                      /**< \brief TIM3中断编号 */
    CLK_TIM3,                       /**< \brief TIM3时钟ID */
    4,                              /**< \brief 4个捕获通道 */
    &__g_tim3_cap_ioinfo_list[0],   /**< \brief 引脚配置信息列表 */
    AMHW_ZLG_TIM_TYPE1,             /**< \brief 定时器类型 */
    __zlg_plfm_tim3_cap_init,       /**< \brief 平台初始化函数 */
    __zlg_plfm_tim3_cap_deinit      /**< \brief 平台解初始化函数 */

};

/** \brief TIM3用于捕获功能的设备定义 */
am_zlg_tim_cap_dev_t  __g_tim3_cap_dev;

/** \brief tim3 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_zlg116_tim3_cap_inst_init (void)
{
    return am_zlg_tim_cap_init(&__g_tim3_cap_dev,
                                  &__g_tim3_cap_devinfo);
}

/** \brief tim3 cap实例解初始化 */
void am_zlg116_tim3_cap_inst_deinit (am_cap_handle_t handle)
{
    am_zlg_tim_cap_deinit(handle);
}

/*******************************************************************************
 * TIM14 配置
 ******************************************************************************/

/** \brief TIM14用于捕获功能的平台初始化 */
void __zlg_plfm_tim14_cap_init (void)
{

    /* 使能通用定时器14时钟 */
    am_clk_enable(CLK_TIM14);

    /* 复位通用定时器14 */
    am_zlg116_clk_reset(CLK_TIM14);
}

/** \brief 解除TIM14平台初始化 */
void __zlg_plfm_tim14_cap_deinit (void)
{

    /* 复位通用定时器14 */
    am_zlg116_clk_reset(CLK_TIM14);

    /* 禁能通用定时器14时钟 */
    am_clk_disable(CLK_TIM14);
}

/** \brief TIM14用于捕获功能的引脚配置信息列表 */
am_zlg_tim_cap_ioinfo_t __g_tim14_cap_ioinfo_list[] = {

    /** \brief 通道1 */
    {PIOA_4,  PIOA_4_TIM14_CH1 | PIOA_4_INPUT_FLOAT, PIOA_4_GPIO | PIOA_4_INPUT_FLOAT},
};

/** \brief TIM14用于捕获功能的设备信息 */
const am_zlg_tim_cap_devinfo_t  __g_tim14_cap_devinfo = {
    ZLG116_TIM14_BASE,               /**< \brief TIM14寄存器块的基地址 */
    INUM_TIM14,                      /**< \brief TIM14中断编号 */
    CLK_TIM14,                       /**< \brief TIM14时钟ID */
    1,                               /**< \brief 1个捕获通道 */
    &__g_tim14_cap_ioinfo_list[0],   /**< \brief 引脚配置信息列表 */
    AMHW_ZLG_TIM_TYPE2,              /**< \brief 定时器类型 */
    __zlg_plfm_tim14_cap_init,       /**< \brief 平台初始化函数 */
    __zlg_plfm_tim14_cap_deinit      /**< \brief 平台解初始化函数 */
};

/** \brief TIM14用于捕获功能的设备定义 */
am_zlg_tim_cap_dev_t  __g_tim14_cap_dev;

/** \brief tim14 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_zlg116_tim14_cap_inst_init (void)
{
    return am_zlg_tim_cap_init(&__g_tim14_cap_dev,
                                  &__g_tim14_cap_devinfo);
}

/** \brief tim14 cap实例解初始化 */
void am_zlg116_tim14_cap_inst_deinit (am_cap_handle_t handle)
{
    am_zlg_tim_cap_deinit(handle);
}


/*******************************************************************************
 * TIM16 配置
 ******************************************************************************/

/** \brief TIM16用于捕获功能的平台初始化 */
void __zlg_plfm_tim16_cap_init (void)
{

    /* 使能通用定时器16时钟 */
    am_clk_enable(CLK_TIM16);

    /* 复位通用定时器16 */
    am_zlg116_clk_reset(CLK_TIM16);
}

/** \brief 解除TIM16平台初始化 */
void __zlg_plfm_tim16_cap_deinit (void)
{

    /* 复位通用定时器16 */
    am_zlg116_clk_reset(CLK_TIM16);

    /* 禁能通用定时器16时钟 */
    am_clk_disable(CLK_TIM16);
}

/** \brief TIM16用于捕获功能的引脚配置信息列表 */
am_zlg_tim_cap_ioinfo_t __g_tim16_cap_ioinfo_list[] = {

    /** \brief 通道1 */
    {PIOA_6,  PIOA_6_TIM16_CH1 | PIOA_6_INPUT_FLOAT, PIOA_6_GPIO | PIOA_6_INPUT_FLOAT},
};

/** \brief TIM16用于捕获功能的设备信息 */
const am_zlg_tim_cap_devinfo_t  __g_tim16_cap_devinfo = {
    ZLG116_TIM16_BASE,               /**< \brief TIM16寄存器块的基地址 */
    INUM_TIM16,                      /**< \brief TIM16中断编号 */
    CLK_TIM16,                       /**< \brief TIM16时钟ID */
    1,                               /**< \brief 1个捕获通道 */
    &__g_tim16_cap_ioinfo_list[0],   /**< \brief 引脚配置信息列表 */
    AMHW_ZLG_TIM_TYPE3,              /**< \brief 定时器类型 */
    __zlg_plfm_tim16_cap_init,       /**< \brief 平台初始化函数 */
    __zlg_plfm_tim16_cap_deinit      /**< \brief 平台解初始化函数 */

};

/** \brief TIM16用于捕获功能的设备定义 */
am_zlg_tim_cap_dev_t  __g_tim16_cap_dev;

/** \brief tim16 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_zlg116_tim16_cap_inst_init (void)
{
    return am_zlg_tim_cap_init(&__g_tim16_cap_dev,
                                  &__g_tim16_cap_devinfo);
}

/** \brief tim16 cap实例解初始化 */
void am_zlg116_tim16_cap_inst_deinit (am_cap_handle_t handle)
{
    am_zlg_tim_cap_deinit(handle);
}

/*******************************************************************************
 * TIM17 配置
 ******************************************************************************/

/** \brief TIM17用于捕获功能的平台初始化 */
void __zlg_plfm_tim17_cap_init (void)
{

    /* 使能通用定时器17时钟 */
    am_clk_enable(CLK_TIM17);

    /* 复位通用定时器17 */
    am_zlg116_clk_reset(CLK_TIM17);
}

/** \brief 解除TIM17平台初始化 */
void __zlg_plfm_tim17_cap_deinit (void)
{

    /* 复位通用定时器17 */
    am_zlg116_clk_reset(CLK_TIM17);

    /* 禁能通用定时器17时钟 */
    am_clk_disable(CLK_TIM17);
}

/** \brief TIM17用于捕获功能的引脚配置信息列表 */
am_zlg_tim_cap_ioinfo_t __g_tim17_cap_ioinfo_list[] = {

    /** \brief 通道1 */
    {PIOA_7, PIOA_7_TIM17_CH1 | PIOA_7_INPUT_FLOAT, PIOA_7_GPIO | PIOA_7_INPUT_FLOAT},
};

/** \brief TIM17用于捕获功能的设备信息 */
const am_zlg_tim_cap_devinfo_t  __g_tim17_cap_devinfo = {
    ZLG116_TIM17_BASE,               /**< \brief TIM17寄存器块的基地址 */
    INUM_TIM17,                      /**< \brief TIM17中断编号 */
    CLK_TIM17,                       /**< \brief TIM17时钟ID */
    1,                               /**< \brief 1个捕获通道 */
    &__g_tim17_cap_ioinfo_list[0],   /**< \brief 引脚配置信息列表 */
    AMHW_ZLG_TIM_TYPE3,              /**< \brief 定时器类型 */
    __zlg_plfm_tim17_cap_init,       /**< \brief 平台初始化函数 */
    __zlg_plfm_tim17_cap_deinit      /**< \brief 平台解初始化函数 */

};

/** \brief TIM17用于捕获功能的设备定义 */
am_zlg_tim_cap_dev_t  __g_tim17_cap_dev;

/** \brief tim17 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_zlg116_tim17_cap_inst_init (void)
{
    return am_zlg_tim_cap_init(&__g_tim17_cap_dev,
                                  &__g_tim17_cap_devinfo);
}

/** \brief tim17 cap实例解初始化 */
void am_zlg116_tim17_cap_inst_deinit (am_cap_handle_t handle)
{
    am_zlg_tim_cap_deinit(handle);
}

/**
 * @}
 */

/* end of file */
