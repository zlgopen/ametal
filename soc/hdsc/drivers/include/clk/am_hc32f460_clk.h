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
 * - 1.00 19-12-23  cds, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_CLK_H
#define __AM_HC32F460_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_clk.h"
#include "hw/amhw_hc32f460_rcc.h"

/**
 * \addtogroup am_hc32f460_if_clk
 * \copydoc am_hc32f460_clk.h
 * @{
 */

/**
 * \brief CLK 设备信息参数结构体
 */
typedef struct am_hc32f460_clk_devinfo {

    /**
     * \brief XTHOSC外部晶振频率
     */
    uint32_t xth_osc;

    /**
     * \brief XTLOSC外部晶振频率
     */
    uint32_t xtl_osc;

    /**
     * \brief HRC内部高速时钟晶振频率（Hz）
     *
     * \note 频率选择    24M、 22.12M、16M、 8M或4M
     *       设置其他频率，将默认使用4M
     */
    amhw_hc32f460_clk_hrc_freq hrc_osc;


    /** \brief
     *    PLL 时钟源选择
     *    -# AMHW_HC32F460_CLK_PLLSRC_XTAL : 外部高速振荡器作为输入时钟
     *    -# AMHW_HC32F460_CLK_PLLSRC_HRC : 内部高速振荡器作为输入时钟
     */
    amhw_hc32f460_clk_pll_src pllin_src;

    /**
     * \brief MPLL 输出时钟配置
     *        MPLLP = PLLIN / mpll_div * mpll_mul / mpllp_div。
     *        MPLLQ = PLLIN / mpll_div * mpll_mul / mpllq_div。
     *        MPLLR = PLLIN / mpll_div * mpll_mul / mpllr_div。
     */
    uint8_t mpll_div;     /* MPLL 输入时钟分频系数,应确保MPLLIN介于1MHz到12MHz之间 */
    uint8_t mpll_mul;     /* MPLL 输出时钟倍频系数 */
    uint8_t mpllp_div;    /* MPLLP时钟分频系数 */
    uint8_t mpllq_div;    /* MPLLQ时钟分频系数 */
    uint8_t mpllr_div;    /* MPLLR时钟分频系数 */

    /**
     * \brief UPLL 输入时钟分频系数
     *        UPLLP = PLLIN / upll_div * upll_mul / upllp_div。
     *        UPLLQ = PLLIN / upll_div * upll_mul / upllq_div。
     *        UPLLR = PLLIN / upll_div * upll_mul / upllr_div。
     */
    uint8_t upll_div;     /* UPLL 输入时钟分频系数,应确保UPLLIN介于1MHz到12MHz之间 */
    uint8_t upll_mul;     /* UPLL 输出时钟倍频系数 */
    uint8_t upllp_div;    /* UPLLP时钟分频系数 */
    uint8_t upllq_div;    /* UPLLQ时钟分频系数 */
    uint8_t upllr_div;    /* UPLLR时钟分频系数 */

    /** \brief
     *    系统时钟源选择
     *    -# AMHW_HC32F460_SYSCLK_RCH : 内部高速时钟作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_XTH : 外部高速时钟作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_RCL : 内部低速时钟作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_XTL ：  外部低速时钟作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_PLL ： 内部PLL作为系统时钟
     */
    amhw_hc32f460_sys_clk_src sysclk_src;

    /** \brief
     *    USBCK时钟源选择
     *    -# AMHW_HC32F460_CLK_USBCKS_SYSCLK_DIV_2 : 系统时钟2分频
     *    -# AMHW_HC32F460_CLK_USBCKS_SYSCLK_DIV_3 : 系统时钟3分频
     *    -# AMHW_HC32F460_CLK_USBCKS_SYSCLK_DIV_4 : 系统时钟4分频
     *    -# AMHW_HC32F460_CLK_USBCKS_MPLL_P ： MPLL/P
     *    -# AMHW_HC32F460_CLK_USBCKS_MPLL_Q ： MPLL/Q
     *    -# AMHW_HC32F460_CLK_USBCKS_MPLL_R : MPLL/R
     *    -# AMHW_HC32F460_CLK_USBCKS_UPLL_P : UPLL/P
     *    -# AMHW_HC32F460_CLK_USBCKS_UPLL_Q : UPLL/Q
     *    -# AMHW_HC32F460_CLK_USBCKS_UPLL_R ：  UPLL/R
     */
    amhw_hc32f460_clk_usbcks_t usbck_src;

    uint8_t hclk_div;

    uint8_t exclk_div;

    uint8_t pclk0_div;

    uint8_t pclk1_div;

    uint8_t pclk2_div;

    uint8_t pclk3_div;

    uint8_t pclk4_div;

    /** \brief 平台初始化函数，配置引脚等工作 */
    void  (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void  (*pfn_plfm_deinit)(void);

} am_hc32f460_clk_devinfo_t;

/**
 * \brief CLK 设备结构体
 */
typedef struct am_hc32f460_clk_dev {

    /** \brief 指向CLK 设备信息的指针 */
    const am_hc32f460_clk_devinfo_t *p_devinfo;

    /** \brief PLL输入频率 */
    uint32_t pllin_clk;

    /** \brief MPLL输出频率 */
    uint32_t mpllout_clk;

    /** \brief UPLL输出频率 */
    uint32_t upllout_clk;

    /** \brief MPLLP输出频率 */
    uint32_t mpllpout_clk;
    /** \brief MPLLQ输出频率 */
    uint32_t mpllqout_clk;
    /** \brief MPLLR输出频率 */
    uint32_t mpllrout_clk;

    /** \brief UPLLP输出频率 */
    uint32_t upllpout_clk;
    /** \brief UPLLQ输出频率 */
    uint32_t upllqout_clk;
    /** \brief UPLLR输出频率 */
    uint32_t upllrout_clk;

    /** \brief sys输出频率 */
    uint32_t sys_clk;

    /** \brief sys类型 */
    amhw_hc32f460_sys_clk_src sys_type;

    /** \brief 系统时钟频率 */
    uint32_t hclk;

    /** \brief 外设时钟频率 */
    uint32_t pclk0;

    /** \brief 外设时钟频率 */
    uint32_t pclk1;

    /** \brief 外设时钟频率 */
    uint32_t pclk2;

    /** \brief 外设时钟频率 */
    uint32_t pclk3;

    /** \brief 外设时钟频率 */
    uint32_t pclk4;
} am_hc32f460_clk_dev_t;

/**
 * \brief CLK 初始化
 *
 * \param[in] p_dev     : 指向CLK设备的指针
 * \param[in] p_devinfo : 指向CLK设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_hc32f460_clk_init (am_hc32f460_clk_dev_t           *p_dev,
                          const am_hc32f460_clk_devinfo_t *p_devinfo);

/**
 * \brief CLK 外设复位
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \retval AM_OK : 操作成功
 */
int am_hc32f460_clk_reset (am_clk_id_t clk_id);

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
int am_hc32f460_clk_update (am_clk_id_t clk_id, int clk);

/**
 * \brief 获取PLL的输入时钟源
 *  仅对CLK_AHB，CLK_APB1，CLK_APB2有效
 *
 * \return clk的时钟配置信息
 */
const am_hc32f460_clk_dev_t *am_hc32f460_clk_devinfo_get (void);

/**
 * \brief 返回总线或外设的CLK分频值
 *
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \return: 返回对应总线或外设的分频值
 *
 */
int am_hc32f460_div_get (am_clk_id_t clk_id);

void SystemClk_Init(void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_CLK_H */

/* end of file */
