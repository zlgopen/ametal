/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief CLK驱动，时钟标准服务接口
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-01  lnk, first implementation.
 * \endinternal
 */
 
#ifndef __AM_LPC82X_CLK_H
#define __AM_LPC82X_CLK_H

#include "ametal.h"
#include "am_clk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_lpc82x_if_clk
 * \copydoc am_lpc82x_clk.h
 * @{
 */

/**
 * \brief CLK 设备信息参数结构体
 *
 *  PLL频率控制
 *
 *  M = pll_msel + 1
 *  P = 2^(pll_psel)
 *
 *  PLLOUT = M * PLLIN;
 *  FCCO = 2 * P * PLLOUT;
 */

typedef struct am_lpc82x_clk_devinfo {

    /** \brief PLL 时钟源选择  */
    uint8_t pllin_src;

    /** 
     * \brief PLL 控制寄存器MSEL，使FCLKOUT 在范围 100MHz内
     *        FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 5 = 60 MHz
     */
    uint8_t pll_msel;

    /** 
     * \brief PLL 控制寄存器PSEL ,使FCCO 在范围 156 - 320MHz
     *        FCCO = FCLKOUT * 2 * 2^(PSEL) = 60MHz * 2 * 2 = 240MHz
     */
    uint8_t pll_psel;

    /** \brief MAIN 时钟源选择 */
    uint8_t main_src;
    
    /** \brief 系统时钟分频数，可填 1- 255 之间数值 */
    uint8_t sysclk_div;

    /**
     * \brief WDTOSC频率分频系数，可填 2 - 64 之间的偶数
     *       
     */
    uint8_t wdtosc_div;

    /**
     * \brief WDTOSC频率选择，参见 \ref grp_amhw_lpc82x_clk_wdtosc_rate 
     */
    uint8_t wdtosc_freq;

    /** \brief 平台初始化函数，配置引脚等工作 */
    void  (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void  (*pfn_plfm_deinit)(void);

} am_lpc82x_clk_devinfo_t;

/**
 * \brief CLK 设备结构体
 */
typedef struct am_lpc82x_clk_dev {

    /** \brief 指向CLK 设备信息的指针   */
    const am_lpc82x_clk_devinfo_t *p_devinfo;

    /** \brief 主时钟源频率*/
    uint32_t main_clkrate;

    /** \brief 系统时钟源频率*/
    uint32_t system_clkrate;

} am_lpc82x_clk_dev_t;

/**
 * \brief CLK 初始化
 *
 * \param[in] p_dev     : 指向CLK设备的指针
 * \param[in] p_devinfo : 指向CLK设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_lpc82x_clk_init (am_lpc82x_clk_dev_t           *p_dev,
                        const am_lpc82x_clk_devinfo_t *p_devinfo);

/**
 * \brief 设置时钟频率
 *
 * \param[in] clk_id  时钟 ID (由平台定义)
 * \param[in] rate    时钟频率
 *
 * \retval  AM_OK   成功
 * \retval -AM_ENXIO   时钟频率ID不存在
 * \retval -AM_ENOTSUP 不支持要设置频率
 */
int am_lpc82x_clk_rate_set (am_clk_id_t clk_id, uint32_t rate);


/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_CLK_H */

/* end of file */
