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
 * \brief ZLG VC 用户配置文件
 * \sa am_hwconf_hc32_vc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32.h"
#include "am_hc32_vc.h"
#include "hw/amhw_hc32_vc.h"
#include "hc32x3x_pin.h"

/**
 * \addtogroup am_if_src_hwconf_hc32_vc
 * \copydoc am_hwconf_hc32_vc.c
 * @{
 */

/**
 * \brief VC 平台初始化
 */
void __hc32_plfm_vc_init (void)
{

    /* 配置PIOA_6为VC_OUT功能 */
    am_gpio_pin_cfg(PIOA_6, PIOA_6_VC0_OUT | PIOA_6_OUT_PP);

    /* 配置PIOC_0为VC0 P端输入 */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_AIN);

    /* 配置PIOA_7为VC0 P端输入 */
    am_gpio_pin_cfg(PIOA_7, PIOA_7_AIN);

    /* 开启VC时钟 */
    am_clk_enable(CLK_VC_LVD);

    /* 开启BGR时钟 */
    am_clk_enable(CLK_ADC_BGR);
}

/**
 * \brief VC 平台去初始化
 */
void __hc32_plfm_vc_deinit (void)
{
    /* 关闭DAC时钟 */
    am_clk_disable(CLK_VC_LVD);
}

/** \brief VC 设备信息 */
static const am_hc32_vc_devinfo_t __g_vc_devinfo =
{
    /**< \brief 指向VC寄存器块的指针 */
    HC32_VC_BASE,

    /**< \brief VC中断编号 */
    INUM_VC0,

    /**
     * \brief VC 通道0，用户配置值查看宏定义：模拟比较器通道选择掩码
     */
    AMHW_HC32_VC0,

    /**
     * \brief VC参考 选择，用户配置值查看宏定义：VC_DIV 参考电压Vref选择掩码
     *
     */
    AMHW_HC32_VC_REF2P5_VCC,

    /**
     * \brief VC 通道0迟滞电压10mv ,为NULL则表示没有迟滞
     *        用户配置值查看宏定义：VC0-2 迟滞电压选择掩码
     */
    AMHW_HC32_VC0_HYS_10_MV,

    /**
     * \brief VC 功耗选择，用户配置值查看宏定义：VC0-2 功耗选择掩码
     *
     * \note :功耗越大，响应时间越快。当功耗选择>=10uA需要开启BGR，BGR启动时间大约20us
     */
    AMHW_HC32_VC0_BIAS_1_2_UA,

    /**
     * \brief VC 滤波时间，用户配置值查看枚举amhw_hc32_vc_deb_time：
     *
     * \note ：滤波时间的配置只有在 FLTEN=1 时才有效,如不需要滤波值为：AMHW_HC32_DEB_TIME_NO
     */
    AMHW_HC32_DEB_TIME_28_US,

    /**
     * \brief VC p端输入 选择，用户配置值查看枚举：VC0-2 “+”端输入选择
     *
     * \note :每个通道输入端口枚举不同，需要查看相应使用通道的枚举定义
     */
    AMHW_HC32_VC0_P_INPUT_PC0,

    /**
     * \brief VC n端输入 选择，用户配置值查看枚举：VC0-2 “-”端输入选择
     *
     * \note :每个通道输入端口枚举不同，需要查看相应使用通道的枚举定义
     */
    AMHW_HC32_VC0_N_INPUT_PA7,

    /**
     * \brief VC 输出配置，用户配置值参考：VC0-2 输出寄存器功能使能宏
     *
     * \note :不用时输出禁能，不能写为NULL，需写：AMHW_HC32_VC_OUT_CFG_DISABLE
     */
    AMHW_HC32_VC_OUT_CFG_DISABLE,

    /**
     *  \brief VC 输出触发中断类型选择，用户配置参考宏定义：VC0-2 输出信号触发中断选择掩码
     *
     */
    AMHW_HC32_VC_OUT_TRI_INT_RISING,

    /**< \brief VC平台初始化函数 */
    __hc32_plfm_vc_init,

    /**< \brief VC平台解初始化函数  */
    __hc32_plfm_vc_deinit,
};

/** \brief VC设备实例 */
static am_hc32_vc_dev_t __g_vc_dev;

/** \brief VC 实例初始化，获得VC标准服务句柄 */
am_hc32_vc_handle_t am_hc32_vc_inst_init (void)
{
    return am_hc32_vc_init(&__g_vc_dev, &__g_vc_devinfo);
}

/** \brief VC 实例解初始化 */
void am_hc32_vc_inst_deinit (am_hc32_vc_handle_t handle)
{
    am_hc32_vc_deinit(handle);
}

/**
 * @}
 */

/* end of file */
