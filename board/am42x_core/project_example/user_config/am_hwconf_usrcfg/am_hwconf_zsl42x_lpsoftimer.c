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
 * \brief zsl42x LPSOFTIMER 定时功能用户配置文件
 * \sa am_hwconf_zsl42x_lpsoftimer.c
 * 
 * \note LPSOFTIMER 使用 LPTM0 和 LPTIM1 来实现，注意不可同时使用。
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-17 zcb, first implementation
 * - 1.01 20-09-23  yg, adaptation zsl42x.
 * \endinternal
 */

/**
 * \addtogroup am_if_src_hwconf_zsl42x_lpsoftimer
 * \copydoc am_hwconf_zsl42x_lpsoftimer.c
 * @{
 */

#include "am_board.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_types.h"

#include "am_zsl42x_lpsoftimer_if.h"
#include "am_hwconf_zsl42x_lpsoftimer.h"

#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_lptim.h"
#include "zsl42x_clk.h"

/**
 * \brief LPTMR 平台初始化
 */
am_local void __zsl42x_plfm_lpsoftimer_init (void)
{
    /* 使能LPTIM时钟 */
    am_clk_enable(CLK_LPTIM1);
    am_clk_enable(CLK_LPTIM0);

    /* 时钟源设置 */
    amhw_zsl42x_lptim_clk_src_sel(ZSL42x_LPTIM0, AMHW_ZSL42x_LPTIM_CLK_SRC_XTL);
    amhw_zsl42x_lptim_clk_src_sel(ZSL42x_LPTIM1, AMHW_ZSL42x_LPTIM_CLK_SRC_XTL);
}

/**
 * \brief 解除 LPTMR 平台初始化
 */
am_local void __zsl42x_plfm_lpsoftimer_deinit (void)
{
    am_clk_disable(CLK_LPTIM1);
    am_clk_disable(CLK_LPTIM0);
}

/**
 * \brief LPTMR 设备信息
 */
am_local const am_zsl42x_lptmr_lpsoftimer_devinfo_t __g_lpsoftimer_devinfo = {
    ZSL42x_LPTIM0,                   /**< \brief 指向LPTMR寄存器块的指针 */
    ZSL42x_LPTIM1,                   /**< \brief 指向LPTMR寄存器块的指针 */

    INUM_LPTIM0_1,                 /**< \brief LPTMR0中断编号 */

    32768,                         /**< \brief 内部低速时钟 */

    -0.030,
    0.003,
    25.0,
    5.0,

    1,

    __zsl42x_plfm_lpsoftimer_init,
    __zsl42x_plfm_lpsoftimer_deinit
};

/**
 * \brief LPTMR低功耗定时器初始化
 */
void am_zsl42x_lpsoftimer_init (void)
{
    am_zsl42x_lptmr_lpsoftimer_init(&__g_lpsoftimer_devinfo);
}

/**
 * \brief LPTMR低功耗定时器去初始化
 */
void am_zsl42x_lpsoftimer_deinit (void)
{
    am_zsl42x_lptmr_lpsoftimer_deinit();
}

/**
 * @}
 */

/* end of file */
