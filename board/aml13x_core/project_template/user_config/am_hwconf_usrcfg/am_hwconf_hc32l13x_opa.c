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
 * \brief ZLG OPA 用户配置文件
 * \sa am_hwconf_hc32_opa.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-13
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32.h"
#include "am_hc32_opa.h"
#include "hw/amhw_hc32_opa.h"
#include "hc32x3x_pin.h"

/**
 * \addtogroup am_if_src_hwconf_hc32_opa
 * \copydoc am_hwconf_hc32_opa.c
 * @{
 */
/********************************OPA1配置***************************************/
/**
 * \brief OPA1 平台初始化
 */
void __hc32_plfm_opa1_init (void)
{
    /* 开启OPA时钟 */
    am_clk_enable (CLK_OPA);

    /* 开启BGR时钟 */
    am_clk_enable (CLK_ADC_BGR);

    /**< \brief 不同通道需要初始化不同引脚 */
    am_gpio_pin_cfg (PIOC_6,  PIOC_6_AIN);
    am_gpio_pin_cfg (PIOB_15, PIOB_15_AIN);
    am_gpio_pin_cfg (PIOC_7,  PIOC_7_AOUT);
}

/**
 * \brief OPA1 平台去初始化
 */
void __hc32_plfm_opa1_deinit (void)
{
    /* 关闭OPA时钟 */
    am_clk_disable (CLK_OPA);

    /* 关闭BGR时钟 */
    am_clk_disable (CLK_ADC_BGR);
}

/** \brief OPA 设备信息 */
static const am_hc32_opa_devinfo_t __g_opa1_devinfo =
{
    /**< \brief 指向OPA寄存器块的指针 */
    HC32_OPA_BASE,

    /**
     *  \brief OPA模式，一共七种模式，其值参考amhw_hc32_opa.h宏定义：OPA工作模式选择掩码
     *
     */
    AMHW_HC32_OPA_MODE_UNITY_GAIN,

    /**
     * \brief OPA增益 ,参考amhw_hc32_opa.h宏定义：OPA 正向增益选择掩码   OPA 反向增益选择掩码
     *        注意模式不同选择正向或者反向增益，级联反向输入 级联反向输入不支持7和 14的增益。单位增益
     *        模式无增益，选择AWHW_HC32_OPA_NO_GAIN。
     *        在正向级联和反向级联模式下，需要使能配置三个通道，其增益有三个通道共同决定
     */
    AWHW_HC32_OPA_NO_GAIN,

    /**
     * \brief IO端口与内部连接控制,参考amhw_hc32_opa.h宏定义：IO端口与内部连接控制宏
     */
    AWHW_HC32_OPA_PO_EN,

    /**< \brief OPA平台初始化函数 */
    __hc32_plfm_opa1_init,

    /**< \brief OPA平台解初始化函数  */
    __hc32_plfm_opa1_deinit,
};

/** \brief OPA1设备实例 */
static am_hc32_opa_dev_t __g_opa1_dev;

/** \brief OPA1 实例初始化，获得OPA1标准服务句柄 */
am_opa_handle_t am_hc32_opa1_inst_init (void)
{
    return am_hc32_opa_init(&__g_opa1_dev, &__g_opa1_devinfo);
}

/** \brief OPA1 实例解初始化 */
void am_hc32_opa1_inst_deinit (am_opa_handle_t handle)
{
    am_hc32_opa_deinit(handle);
}

/********************************OPA2配置***************************************/
/**
 * \brief OPA2 平台初始化
 */
void __hc32_plfm_opa2_init (void)
{
    /* 开启OPA时钟 */
    am_clk_enable (CLK_OPA);

    /* 开启BGR时钟 */
    am_clk_enable (CLK_ADC_BGR);

    /**< \brief 不同通道需要初始化不同引脚 */
    /* OPA2 P N OUT端 */
    am_gpio_pin_cfg (PIOB_13, PIOB_13_AIN);
    am_gpio_pin_cfg (PIOB_12, PIOB_12_AIN);
    am_gpio_pin_cfg (PIOB_14, PIOB_14_AOUT);
}

/**
 * \brief OPA2 平台去初始化
 */
void __hc32_plfm_opa2_deinit (void)
{
    /* 关闭OPA时钟 */
    am_clk_disable (CLK_OPA);

    /* 关闭BGR时钟 */
    am_clk_disable (CLK_ADC_BGR);
}

/** \brief OPA 设备信息 */
static const am_hc32_opa_devinfo_t __g_opa2_devinfo =
{
    /**< \brief 指向OPA寄存器块的指针 */
    HC32_OPA_BASE,

    /**
     *  \brief OPA模式，一共七种模式，其值参考amhw_hc32_opa.h宏定义：OPA工作模式选择掩码
     *
     */
    AMHW_HC32_OPA_MODE_UNITY_GAIN,

    /**
     * \brief OPA增益 ,参考amhw_hc32_opa.h宏定义：OPA 正向增益选择掩码   OPA 反向增益选择掩码
     *        注意模式不同选择正向或者反向增益，级联反向输入 级联反向输入不支持7和 14的增益。单位增益
     *        模式无增益，选择AWHW_HC32_OPA_NO_GAIN。
     *        在正向级联和反向级联模式下，需要使能配置三个通道，其增益有三个通道共同决定
     */
    AWHW_HC32_OPA_NO_GAIN,

    /**
     * \brief IO端口与内部连接控制,参考amhw_hc32_opa.h宏定义：IO端口与内部连接控制宏
     */
    AWHW_HC32_OPA_PO_EN,

    /**< \brief OPA2平台初始化函数 */
    __hc32_plfm_opa2_init,

    /**< \brief OPA2平台解初始化函数  */
    __hc32_plfm_opa2_deinit,
};

/** \brief OPA2设备实例 */
static am_hc32_opa_dev_t __g_opa2_dev;

/** \brief OPA2 实例初始化，获得OPA2标准服务句柄 */
am_opa_handle_t am_hc32_opa2_inst_init (void)
{
    return am_hc32_opa_init(&__g_opa2_dev, &__g_opa2_devinfo);
}

/** \brief OPA2 实例解初始化 */
void am_hc32_opa2_inst_deinit (am_opa_handle_t handle)
{
    am_hc32_opa_deinit(handle);
}

/********************************OPA3配置***************************************/
/**
 * \brief OPA3 平台初始化
 */
void __hc32_plfm_opa3_init (void)
{
    /* 开启OPA时钟 */
    am_clk_enable (CLK_OPA);

    /* 开启BGR时钟 */
    am_clk_enable (CLK_ADC_BGR);

    /**< \brief 不同通道需要初始化不同引脚 */
    /* OPA3 P N OUT端 */
    am_gpio_pin_cfg (PIOB_10, PIOB_10_AIN);
    am_gpio_pin_cfg (PIOB_2,  PIOB_2_AIN);
    am_gpio_pin_cfg (PIOB_11, PIOB_11_AOUT);
}

/**
 * \brief OPA3 平台去初始化
 */
void __hc32_plfm_opa3_deinit (void)
{
    /* 关闭OPA时钟 */
    am_clk_disable (CLK_OPA);

    /* 关闭BGR时钟 */
    am_clk_disable (CLK_ADC_BGR);
}

/** \brief OPA 设备信息 */
static const am_hc32_opa_devinfo_t __g_opa3_devinfo =
{
    /**< \brief 指向OPA寄存器块的指针 */
    HC32_OPA_BASE,

    /**
     *  \brief OPA模式，一共七种模式，其值参考amhw_hc32_opa.h宏定义：OPA工作模式选择掩码
     *
     */
    AMHW_HC32_OPA_MODE_FORWARD_IN,

    /**
     * \brief OPA增益 ,参考amhw_hc32_opa.h宏定义：OPA 正向增益选择掩码   OPA 反向增益选择掩码
     *        注意模式不同选择正向或者反向增益，级联反向输入 级联反向输入不支持7和 14的增益。单位增益
     *        模式无增益，选择AWHW_HC32_OPA_NO_GAIN。
     *        在正向级联和反向级联模式下，需要使能配置三个通道，其增益有三个通道共同决定
     */
    AMHW_HC32_OPA_NONGAIN_16,

    /**
     * \brief IO端口与内部连接控制,参考amhw_hc32_opa.h宏定义：IO端口与内部连接控制宏
     */
    AWHW_HC32_OPA_PO_EN,

    /**< \brief OPA2平台初始化函数 */
    __hc32_plfm_opa3_init,

    /**< \brief OPA2平台解初始化函数  */
    __hc32_plfm_opa3_deinit,
};

/** \brief OPA3设备实例  */
static am_hc32_opa_dev_t __g_opa3_dev;

/** \brief OPA3 实例初始化，获得OPA3标准服务句柄 */
am_opa_handle_t am_hc32_opa3_inst_init (void)
{
    return am_hc32_opa_init(&__g_opa3_dev, &__g_opa3_devinfo);
}

/** \brief OPA3 实例解初始化 */
void am_hc32_opa3_inst_deinit (am_opa_handle_t handle)
{
    am_hc32_opa_deinit(handle);
}
/**
 * @}
 */

/* end of file */
