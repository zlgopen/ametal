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
 * \brief TIM 用于捕获功能的用户配置文件
 * \sa am_hwconf_hc32f460_tim_cap.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-12  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460.h"
#include "am_hc32f460_timea_cap.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_hc32f460_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_tim_cap
 * \copydoc am_hwconf_hc32f460_tim_cap.c
 * @{
 */
/*******************************************************************************
  TIMEA1 配置
*******************************************************************************/

/** \brief TIMEA1用于捕获功能的平台初始化 */
void __hc32f460_plfm_timea1_cap_init (void)
{
    am_clk_enable(CLK_TIMERA_1);
    am_clk_enable(CLK_PTDIS);      
}

/** \brief 解除TIMEA1平台初始化 */
void __hc32f460_plfm_timea1_cap_deinit (void)
{
    am_clk_disable(CLK_TIMERA_1);
    am_clk_disable(CLK_PTDIS);        
}

/** \brief TIMEA1用于捕获功能的引脚配置信息列表 */
am_hc32f460_timea_cap_ioinfo_t __g_timea1_cap_ioinfo_list[] = {
    /** \brief 通道0 */
    {
        TIMERA_CH1,
        PIOE_9,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },

    /** \brief 通道1 */
    {
        TIMERA_CH2,
        PIOE_11,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMERA_CH3,
        PIOE_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },   
    /** \brief 通道4引脚配置 */
    {
        TIMERA_CH4,
        PIOE_14,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道5引脚配置 */
    {
        TIMERA_CH5,
        PIOE_8,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMERA_CH6,
        PIOE_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道7引脚配置 */
    {
        TIMERA_CH7,
        PIOE_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道8引脚配置 */
    {
        TIMERA_CH8,
        PIOE_15,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },        
};

/** \brief TIM0用于捕获功能的设备信息 */
const am_hc32f460_timea_cap_devinfo_t  __g_timea1_cap_devinfo = {
    HC32F460_TMRA1_BASE,                    /**< \brief TIMEA1寄存器块的基地址 */
    INUM_TIMERA_1,                          /**< \brief TIMEA1中断编号 */
    AMHW_HC32F460_TIMEA_CLK_DIV1024,        /**< \brief 时钟分频系数 */
    8,                                      /**< \brief 8个捕获通道 */
    0,                                      /**< \brief 是否使能同步模式
                                             *          1：使能
                                             *          0：失能
                                             */

    &__g_timea1_cap_ioinfo_list[0],
    __hc32f460_plfm_timea1_cap_init,         /**< \brief 平台初始化函数 */
    __hc32f460_plfm_timea1_cap_deinit        /**< \brief 平台解初始化函数 */
};

/** \brief TIMEA1用于捕获功能的设备定义 */
am_hc32f460_timea_cap_dev_t  __g_timea1_cap_dev;

/** \brief timea1 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_timea1_cap_inst_init (void)
{
    return am_hc32f460_timea_cap_init(&__g_timea1_cap_dev,
                                      &__g_timea1_cap_devinfo);
}

/** \brief timea1 cap实例解初始化 */
void am_hc32f460_timea1_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_timea_cap_deinit(handle);
}

/*******************************************************************************
  TIMEA2 配置
*******************************************************************************/

/** \brief TIMEA2用于捕获功能的平台初始化 */
void __hc32f460_plfm_timea2_cap_init (void)
{
    am_clk_enable(CLK_TIMERA_2);
    am_clk_enable(CLK_PTDIS);      
}

/** \brief 解除TIMEA2平台初始化 */
void __hc32f460_plfm_timea2_cap_deinit (void)
{
    am_clk_disable(CLK_TIMERA_2);
    am_clk_disable(CLK_PTDIS);        
}

/** \brief TIMEA2用于捕获功能的引脚配置信息列表 */
am_hc32f460_timea_cap_ioinfo_t __g_timea2_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        TIMERA_CH1,
        PIOA_0,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMERA_CH2,
        PIOA_1,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMERA_CH3,
        PIOA_2,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },   
    /** \brief 通道4引脚配置 */
    {
        TIMERA_CH4,
        PIOA_3,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道5引脚配置 */
    {
        TIMERA_CH5,
        PIOA_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMERA_CH6,
        PIOA_14,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道7引脚配置 */
    {
        TIMERA_CH7,
        PIOC_2,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道8引脚配置 */
    {
        TIMERA_CH8,
        PIOC_3,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },   
};

/** \brief TIM0用于捕获功能的设备信息 */
const am_hc32f460_timea_cap_devinfo_t  __g_timea2_cap_devinfo = {
    HC32F460_TMRA2_BASE,                    /**< \brief TIMEA1寄存器块的基地址 */
    INUM_TIMERA_2,                          /**< \brief TIMEA1中断编号 */
    AMHW_HC32F460_TIMEA_CLK_DIV256,         /**< \brief 时钟分频系数 */
    2,                                      /**< \brief 2个捕获通道 */
    0,                                      /**< \brief 是否使能同步模式
                                             *          1：使能
                                             *          0：失能
                                             */

    &__g_timea2_cap_ioinfo_list[0],
    __hc32f460_plfm_timea2_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f460_plfm_timea2_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIMEA1用于捕获功能的设备定义 */
am_hc32f460_timea_cap_dev_t  __g_timea2_cap_dev;

/** \brief timea1 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_timea2_cap_inst_init (void)
{
    return am_hc32f460_timea_cap_init(&__g_timea2_cap_dev,
                                      &__g_timea2_cap_devinfo);
}

/** \brief timea1 cap实例解初始化 */
void am_hc32f460_timea2_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_timea_cap_deinit(handle);
}

/*******************************************************************************
  TIMEA3 配置
*******************************************************************************/

/** \brief TIMEA3用于捕获功能的平台初始化 */
void __hc32f460_plfm_timea3_cap_init (void)
{
    am_clk_enable(CLK_TIMERA_3);
    am_clk_enable(CLK_PTDIS);      
}

/** \brief 解除TIMEA3平台初始化 */
void __hc32f460_plfm_timea3_cap_deinit (void)
{
    am_clk_disable(CLK_TIMERA_3);
    am_clk_disable(CLK_PTDIS);        
}

/** \brief TIMEA3用于捕获功能的引脚配置信息列表 */
am_hc32f460_timea_cap_ioinfo_t __g_timea3_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        TIMERA_CH1,
        PIOC_6,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMERA_CH2,
        PIOB_5,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMERA_CH3,
        PIOB_0,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },   
    /** \brief 通道4引脚配置 */
    {
        TIMERA_CH4,
        PIOB_1,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道5引脚配置 */
    {
        TIMERA_CH5,
        PIOE_2,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMERA_CH6,
        PIOE_3,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道7引脚配置 */
    {
        TIMERA_CH7,
        PIOE_4,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道8引脚配置 */
    {
        TIMERA_CH8,
        PIOE_5,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },  
};

/** \brief TIM3用于捕获功能的设备信息 */
const am_hc32f460_timea_cap_devinfo_t  __g_timea3_cap_devinfo = {
    HC32F460_TMRA3_BASE,                    /**< \brief TIMEA1寄存器块的基地址 */
    INUM_TIMERA_3,                          /**< \brief TIMEA1中断编号 */
    AMHW_HC32F460_TIMEA_CLK_DIV256,         /**< \brief 时钟分频系数 */
    2,                                      /**< \brief 2个捕获通道 */
    0,                                      /**< \brief 是否使能同步模式
                                             *          1：使能
                                             *          0：失能
                                             */

    &__g_timea3_cap_ioinfo_list[0],
    __hc32f460_plfm_timea3_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f460_plfm_timea3_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIMEA1用于捕获功能的设备定义 */
am_hc32f460_timea_cap_dev_t  __g_timea3_cap_dev;

/** \brief timea1 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_timea3_cap_inst_init (void)
{
    return am_hc32f460_timea_cap_init(&__g_timea3_cap_dev,
                                      &__g_timea3_cap_devinfo);
}

/** \brief timea1 cap实例解初始化 */
void am_hc32f460_timea3_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_timea_cap_deinit(handle);
}

/*******************************************************************************
  TIMEA4 配置
*******************************************************************************/

/** \brief TIMEA4用于捕获功能的平台初始化 */
void __hc32f460_plfm_timea4_cap_init (void)
{
    am_clk_enable(CLK_TIMERA_4);
    am_clk_enable(CLK_PTDIS);      
}

/** \brief 解除TIMEA3平台初始化 */
void __hc32f460_plfm_timea4_cap_deinit (void)
{
    am_clk_disable(CLK_TIMERA_4);
    am_clk_disable(CLK_PTDIS);        
}

/** \brief TIMEA3用于捕获功能的引脚配置信息列表 */
am_hc32f460_timea_cap_ioinfo_t __g_timea4_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        TIMERA_CH1,
        PIOD_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMERA_CH2,
        PIOD_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMERA_CH3,
        PIOD_14,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },   
    /** \brief 通道4引脚配置 */
    {
        TIMERA_CH4,
        PIOD_15,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道5引脚配置 */
    {
        TIMERA_CH5,
        PIOC_14,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMERA_CH6,
        PIOC_15,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道7引脚配置 */
    {
        TIMERA_CH7,
        PIOH_2,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },
    /** \brief 通道8引脚配置 */
    {
        TIMERA_CH8,
        PIOC_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA0),
        AM_GPIO_INPUT
    },    
};

/** \brief TIM3用于捕获功能的设备信息 */
const am_hc32f460_timea_cap_devinfo_t  __g_timea4_cap_devinfo = {
    HC32F460_TMRA4_BASE,                    /**< \brief TIMEA1寄存器块的基地址 */
    INUM_TIMERA_4,                          /**< \brief TIMEA1中断编号 */
    AMHW_HC32F460_TIMEA_CLK_DIV256,         /**< \brief 时钟分频系数 */
    2,                                      /**< \brief 2个捕获通道 */
    0,                                      /**< \brief 是否使能同步模式
                                             *          1：使能
                                             *          0：失能
                                             */

    &__g_timea4_cap_ioinfo_list[0],
    __hc32f460_plfm_timea4_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f460_plfm_timea4_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIMEA1用于捕获功能的设备定义 */
am_hc32f460_timea_cap_dev_t  __g_timea4_cap_dev;

/** \brief timea4 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_timea4_cap_inst_init (void)
{
    return am_hc32f460_timea_cap_init(&__g_timea4_cap_dev,
                                      &__g_timea4_cap_devinfo);
}

/** \brief timea4 cap实例解初始化 */
void am_hc32f460_timea4_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_timea_cap_deinit(handle);
}

/*******************************************************************************
  TIMEA5 配置
*******************************************************************************/

/** \brief TIMEA5用于捕获功能的平台初始化 */
void __hc32f460_plfm_timea5_cap_init (void)
{
    am_clk_enable(CLK_TIMERA_5);
    am_clk_enable(CLK_PTDIS);      
}

/** \brief 解除TIMEA5平台初始化 */
void __hc32f460_plfm_timea5_cap_deinit (void)
{
    am_clk_disable(CLK_TIMERA_5);
    am_clk_disable(CLK_PTDIS);        
}

/** \brief TIMEA5用于捕获功能的引脚配置信息列表 */
am_hc32f460_timea_cap_ioinfo_t __g_timea5_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        TIMERA_CH1,
        PIOC_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMERA_CH2,
        PIOC_11,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMERA_CH3,
        PIOC_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },   
    /** \brief 通道4引脚配置 */
    {
        TIMERA_CH4,
        PIOD_0,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道5引脚配置 */
    {
        TIMERA_CH5,
        PIOD_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMERA_CH6,
        PIOD_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道7引脚配置 */
    {
        TIMERA_CH7,
        PIOD_14,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道8引脚配置 */
    {
        TIMERA_CH8,
        PIOD_15,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },    
};

/** \brief TIM3用于捕获功能的设备信息 */
const am_hc32f460_timea_cap_devinfo_t  __g_timea5_cap_devinfo = {
    HC32F460_TMRA5_BASE,                    /**< \brief TIMEA1寄存器块的基地址 */
    INUM_TIMERA_5,                          /**< \brief TIMEA1中断编号 */
    AMHW_HC32F460_TIMEA_CLK_DIV256,         /**< \brief 时钟分频系数 */
    2,                                      /**< \brief 2个捕获通道 */
    0,                                      /**< \brief 是否使能同步模式
                                             *          1：使能
                                             *          0：失能
                                             */

    &__g_timea5_cap_ioinfo_list[0],
    __hc32f460_plfm_timea5_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f460_plfm_timea5_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIMEA1用于捕获功能的设备定义 */
am_hc32f460_timea_cap_dev_t  __g_timea5_cap_dev;

/** \brief timea5 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_timea5_cap_inst_init (void)
{
    return am_hc32f460_timea_cap_init(&__g_timea5_cap_dev,
                                      &__g_timea5_cap_devinfo);
}

/** \brief timea5 cap实例解初始化 */
void am_hc32f460_timea5_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_timea_cap_deinit(handle);
}

/*******************************************************************************
  TIMEA6 配置
*******************************************************************************/

/** \brief TIMEA6用于捕获功能的平台初始化 */
void __hc32f460_plfm_timea6_cap_init (void)
{
    am_clk_enable(CLK_TIMERA_6);
    am_clk_enable(CLK_PTDIS);      
}

/** \brief 解除TIMEA6平台初始化 */
void __hc32f460_plfm_timea6_cap_deinit (void)
{
    am_clk_disable(CLK_TIMERA_6);
    am_clk_disable(CLK_PTDIS);        
}

/** \brief TIMEA6用于捕获功能的引脚配置信息列表 */
am_hc32f460_timea_cap_ioinfo_t __g_timea6_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        TIMERA_CH1,
        PIOD_8,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMERA_CH2,
        PIOD_9,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMERA_CH3,
        PIOD_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },   
    /** \brief 通道4引脚配置 */
    {
        TIMERA_CH4,
        PIOD_11,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道5引脚配置 */
    {
        TIMERA_CH5,
        PIOD_1,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMERA_CH6,
        PIOD_2,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道7引脚配置 */
    {
        TIMERA_CH7,
        PIOD_3,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },
    /** \brief 通道8引脚配置 */
    {
        TIMERA_CH8,
        PIOD_4,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIMA1),
        AM_GPIO_INPUT
    },   
};

/** \brief TIM3用于捕获功能的设备信息 */
const am_hc32f460_timea_cap_devinfo_t  __g_timea6_cap_devinfo = {
    HC32F460_TMRA6_BASE,                    /**< \brief TIMEA1寄存器块的基地址 */
    INUM_TIMERA_6,                          /**< \brief TIMEA1中断编号 */
    AMHW_HC32F460_TIMEA_CLK_DIV256,         /**< \brief 时钟分频系数 */
    2,                                      /**< \brief 2个捕获通道 */
    0,                                      /**< \brief 是否使能同步模式
                                             *          1：使能
                                             *          0：失能
                                             */

    &__g_timea6_cap_ioinfo_list[0],
    __hc32f460_plfm_timea6_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f460_plfm_timea6_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIMEA1用于捕获功能的设备定义 */
am_hc32f460_timea_cap_dev_t  __g_timea6_cap_dev;

/** \brief timea6 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_timea6_cap_inst_init (void)
{
    return am_hc32f460_timea_cap_init(&__g_timea6_cap_dev,
                                      &__g_timea6_cap_devinfo);
}

/** \brief timea6 cap实例解初始化 */
void am_hc32f460_timea6_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_timea_cap_deinit(handle);
}

/**
 * @}
 */

/* end of file */
