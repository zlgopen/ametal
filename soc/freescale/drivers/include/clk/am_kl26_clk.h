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
 * \brief CLK驱动，时钟标准服务接口实现
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-13  ari, first implementation.
 * \endinternal
 */
 
#ifndef __AM_KL26_CLK_H
#define __AM_KL26_CLK_H

#include "ametal.h"
#include "am_clk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_kl26_if_clk
 * \copydoc am_kl26_clk.h
 * @{
 */


/**
  * \brief 时钟工作模式
  */
typedef enum am_kl26_clk_mode {
    AM_KL26_CLK_MODE_FEI   = 0,   /**< \brief FEI  */
    AM_KL26_CLK_MODE_FBI   = 1,   /**< \brief FBI  */
    AM_KL26_CLK_MODE_BLPI  = 2,   /**< \brief BLPI */
    AM_KL26_CLK_MODE_FEE   = 3,   /**< \brief FEE  */
    AM_KL26_CLK_MODE_FBE   = 4,   /**< \brief FBE  */
    AM_KL26_CLK_MODE_BLPE  = 5,   /**< \brief BLPE */
    AM_KL26_CLK_MODE_PBE   = 6,   /**< \brief PBE  */
    AM_KL26_CLK_MODE_PEE   = 7,   /**< \brief PEE  */
    AM_KL26_CLK_MODE_STOP  = 8,   /**< \brief STOP  */
    AM_KL26_CLK_MODE_ERROR = 9    /**< \brief ERROR  */
} am_kl26_clk_mode_t;

/**
 * \brief CLK 设备信息参数结构体
 */

typedef struct am_kl26_clk_devinfo {
    /** \brief 时钟工作模式 */
    am_kl26_clk_mode_t clk_mode;

    /** \brief 外部晶振频率 */
    uint32_t xtal_rate;

    /* 内核时钟core_clk分频系数(1-16),
     * core_clk = MCGOUTCLK / outdiv1 ,core_clk upto 48MHz
     */
    uint8_t core_clk_div;

    /** \brief 平台初始化函数，配置引脚等工作 */
    void  (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void  (*pfn_plfm_deinit)(void);

} am_kl26_clk_devinfo_t;

/**
 * \brief CLK 设备结构体
 */
typedef struct am_kl26_clk_dev {

    /** \brief 指向CLK 设备信息的指针   */
    const am_kl26_clk_devinfo_t *p_devinfo;

    /* PLL分频系数， 使得PLL 输入频率在2-4MHz 间*/
    uint8_t pll_div;

    /* PLL倍频系数，MCGPLLCLK = 2MHz * (24 + pll_div), MCGPLLCLK upto 100MHz*/
    uint8_t pll_mul;

    /* FLL分频系数，使得FLL 输入频率在32.768K 左右 */
    uint8_t fll_div;

    /* FLL锁频频率，MCGFLLCLK = 24MHz或48MHz或72MHz或96MHz */
    uint32_t fll_rate;

    /* 内核时钟core_clk分频系数(1-16),
    * core_clk = MCGOUTCLK / outdiv1 ,core_clk upto 48MHz
    */
    uint8_t outdiv1;
    /* 总线时钟bus_clk分频系数(2-8)，
    * bus_clk = core_clk / outdiv4, bus_clk upto 24MHz
    */
    uint8_t outdiv4;
} am_kl26_clk_dev_t;

/**
 * \brief CLK 初始化
 *
 * \param[in] p_dev     : 指向CLK设备的指针
 * \param[in] p_devinfo : 指向CLK设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_kl26_clk_init (am_kl26_clk_dev_t           *p_dev,
                      const am_kl26_clk_devinfo_t *p_devinfo);

/**
 * \brief 得到core/platform/system时钟频率
 * \return core/platform/system时钟频率
 */
uint32_t am_kl26_clk_core_clkrate_get(void);

/**
 * \brief 得到bus/flash时钟频率
 * \return bus/flash时钟频率
 */
uint32_t am_kl26_clk_bus_clkrate_get(void);

/**
 * \brief 得到外设的输入频率
 *
 * \param[in] p_periph : 指向外设的寄存器块
 *
 * \return 外设的输入频率
 */
uint32_t am_kl26_clk_periph_rate_get(void *p_periph);

/**
 * \brief 获取时钟工作模式
 */
am_kl26_clk_mode_t am_kl26_clk_mode_get (void);

/**
 * \brief 配置时钟工作模式PEE到BLPE模式（低功耗模式）
 * \return 无
 * \note BLPE模式，CORE_CLK = 4MHZ
 *                 BUS_CLK = 1MHZ
 */
int am_kl26_clk_mode_pee2blpe (void);

/**
 * \brief 配置时钟工作模式BLPE到PEE模式
 * \return 无
 * \note PEE模式,CORE_CLK = 48MHZ
 *               BUS_CLK = 24MHZ
 */
int am_kl26_clk_mode_blpe2pee (void);

/**
 * \brief 配置时钟工作模式PBE到PEE模式
 * \return 无
 * \note PEE模式,CORE_CLK = 48MHZ
 *               BUS_CLK = 24MHZ
 */
int am_kl26_clk_mode_pbe2pee(void);

/**
 * \brief 配置时钟工作模式FEI到BLPI模式（低功耗模式）
 * \return 无
 * \note BLPI模式,
 *       Core clock = 4MHz
 *       Bus clock  = 1MHz
 */
int am_kl26_clk_mode_fei2blpi (void);

/**
 * \brief 配置时钟工作模式BLPI到FEI模式
 * \return 无
 * \note FBI模式
 *       Core clock = 20.97152MHz
 *       Bus clock  = 20.97152MHz
 */
int am_kl26_clk_mode_blpi2fei (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_KL26_CLK_H */

/* end of file */
