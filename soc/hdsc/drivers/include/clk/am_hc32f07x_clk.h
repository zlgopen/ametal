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
 * \brief CLK驱动，时钟标准服务接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-04  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F07X_CLK_H
#define __AM_HC32F07X_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_clk.h"
#include "hw/amhw_hc32f07x_rcc.h"

/**
 * \addtogroup am_hc32f07x_if_clk
 * \copydoc am_hc32f07x_clk.h
 * @{
 */

/**
 * \brief CLK 设备信息参数结构体
 */
typedef struct am_hc32f07x_clk_devinfo {

    /**
     * \brief XTHOSC外部晶振频率
     */
    uint32_t xth_osc;

    /**
     * \brief XTLOSC外部晶振频率
     */
    uint32_t xtl_osc;

    /**
     * \brief RCH内部高速时钟晶振频率（Hz）
     *
     * \note 频率选择    24M、 22.12M、16M、 8M或4M
     *       设置其他频率，将默认使用4M
     */
    amhw_hc32f07x_rch_fre_t rch_osc;

    /**
     * \brief RCL内部低速时钟晶振频率（Hz）
     *
     * \note 频率选择    38.4k  或      32.768K
     *       设置其他频率，将默认使用38.4k
     */
    amhw_hc32f07x_rcl_fre_t rcl_osc;

    /** \brief
     *    PLL 时钟源选择
     *    -# AMHW_HC32_PLL_INPUT_FRE_SRC_XTH_XTAL : XTH晶振生成的时钟
     *    -# AMHW_HC32_PLL_INPUT_FRE_SRC_XTH_PF00 : XTH从管脚PF00输入的时钟
     *    -# AMHW_HC32_PLL_INPUT_FRE_SRC_XTH_RCH  : RCH时钟
     */
    amhw_hc32f07x_pll_input_fre_src pllin_src;

    /**
     * \brief PLL 倍频系数，允许范围0X2~0XC
     *        PLLOUT = PLLIN * pll_mul
     */
    uint8_t pll_mul;

    /** \brief
     *    系统时钟源选择
     *    -# AMHW_HC32_SYSCLK_RCH : 内部高速时钟作为系统时钟
     *    -# AMHW_HC32_SYSCLK_XTH : 外部高速时钟作为系统时钟
     *    -# AMHW_HC32_SYSCLK_RCL : 内部低速时钟作为系统时钟
     *    -# AMHW_HC32_SYSCLK_XTL ：  外部低速时钟作为系统时钟
     *    -# AMHW_HC32_SYSCLK_PLL ： 内部PLL作为系统时钟
     */
    amhw_hc32f07x_sys_clk_src sysclk_src;

    /**
     * \brief HCLK = SYSCLK / (2 ^ hclk_div)
     */
    uint8_t hclk_div;

    /**
     * \brief PCLK分频系数，PCLK = HCLK / (2 ^ pclk_div)
     */
    uint8_t pclk_div;

    /** \brief 平台初始化函数，配置引脚等工作 */
    void  (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void  (*pfn_plfm_deinit)(void);

} am_hc32f07x_clk_devinfo_t;

/**
 * \brief CLK 设备结构体
 */
typedef struct am_hc32f07x_clk_dev {

    /** \brief 指向CLK 设备信息的指针 */
    const am_hc32f07x_clk_devinfo_t *p_devinfo;

    /** \brief PLL输入频率 */
    uint32_t pllin_clk;

    /** \brief PLL输出频率 */
    uint32_t pllout_clk;

    /** \brief sys输出频率 */
    uint32_t sys_clk;

    /** \brief sys类型 */
    amhw_hc32f07x_sys_clk_src sys_type;

    /** \brief 系统时钟频率 */
    uint32_t hclk;

    /** \brief 外设时钟频率 */
    uint32_t pclk;
} am_hc32f07x_clk_dev_t;

/**
 * \brief CLK 初始化
 *
 * \param[in] p_dev     : 指向CLK设备的指针
 * \param[in] p_devinfo : 指向CLK设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_hc32f07x_clk_init (am_hc32f07x_clk_dev_t           *p_dev,
                        const am_hc32f07x_clk_devinfo_t *p_devinfo);

/**
 * \brief CLK 外设复位
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \retval AM_OK : 操作成功
 */
int am_hc32f07x_clk_reset (am_clk_id_t clk_id);

/**
 * \brief 更新总线频率值
 *
 *  当总线频率被重新设置时，需要调用此函数更新驱动内部存储的频率值
 *  仅对CLK_AHB，CLK_APB1，CLK_APB2有效
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 * \param[in] clk     更新的频率值
 *
 * \retval AM_OK : 操作成功
 *
 * \note 注：此函数慎用
 */
int am_hc32f07x_clk_update (am_clk_id_t clk_id, int clk);

/**
 * \brief 获取PLL的输入时钟源
 *  仅对CLK_AHB，CLK_APB1，CLK_APB2有效
 *
 * \return clk的时钟配置信息
 */
const am_hc32f07x_clk_dev_t *am_hc32_clk_devinfo_get (void);

/**
 * \brief 返回总线或外设的CLK分频值
 *
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \return: 返回对应总线或外设的分频值
 *
 */
int am_hc32f07x_div_get (am_clk_id_t clk_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_CLK_H */

/* end of file */
