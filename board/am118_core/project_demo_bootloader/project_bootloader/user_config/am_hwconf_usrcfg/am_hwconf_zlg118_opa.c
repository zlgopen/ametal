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
 * \sa am_hwconf_zlg118_opa.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_zlg118.h"
#include "am_zlg118_opa.h"
#include "hw/amhw_zlg118_opa.h"
#include "zlg118_pin.h"

/**
 * \addtogroup am_if_src_hwconf_zlg118_opa
 * \copydoc am_hwconf_zlg118_opa.c
 * @{
 */

/**
 * \brief OPA 平台初始化
 */
void __zlg118_plfm_opa_init (void)
{
    /* 开启OPA时钟 */
    am_clk_enable (CLK_OPA);

    /* 开启BGR时钟 */
    am_clk_enable (CLK_ADC_BGR);

    /* PB00 OPA输入 */
    am_gpio_pin_cfg (PIOB_0, PIOB_0_AIN);
}

/**
 * \brief OPA 平台去初始化
 */
void __zlg118_plfm_opa_deinit (void)
{
    /* 关闭OPA时钟 */
    am_clk_disable (CLK_OPA);

    /* 关闭BGR时钟 */
    am_clk_disable (CLK_ADC_BGR);
}

/** \brief OPA 设备信息 */
static const am_zlg118_opa_devinfo_t __g_opa_devinfo =
{
    /**< \brief 指向OPA寄存器块的指针 */
    ZLG118_OPA_BASE,

    /**
     *  \brief OPA自动校准脉宽，用户配置值参考枚举：自动校准脉冲宽度设置
     *
     *  \note :建议校准时间10us
     */
    AMHW_ZLG118_OPA_CLK_16,

    /**
     * \brief OPA校准方式,参考宏定义：OPA 自动校零选择掩码
     */
    AMHW_ZLG118_OPA_AZ_SW,

    /**
     * \brief OPA工作模式 ,参考am_zlg118_opa.h宏定义：OPA工作模式选择
     */
    AM_ZLG118_OPA_GENERAL,

    /**< \brief LVD平台初始化函数 */
    __zlg118_plfm_opa_init,

    /**< \brief LVD平台解初始化函数  */
    __zlg118_plfm_opa_deinit,
};

/** \brief OPA设备实例 */
static am_zlg118_opa_dev_t __g_opa_dev;

/** \brief OPA 实例初始化，获得OPA标准服务句柄 */
am_opa_handle_t am_zlg118_opa_inst_init (void)
{
    return am_zlg118_opa_init(&__g_opa_dev, &__g_opa_devinfo);
}

/** \brief OPA 实例解初始化 */
void am_zlg118_opa_inst_deinit (am_opa_handle_t handle)
{
    am_zlg118_opa_deinit(handle);
}

/**
 * @}
 */

/* end of file */
