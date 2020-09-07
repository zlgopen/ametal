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
 * \brief HK32F103RBT6 DMA 用户配置文件
 * \sa am_hwconf_hk32f103rbt6_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-22  fra, first implementation
 * \endinternal
 */

#include "am_hk32f103rbt6.h"
#include "am_clk.h"
#include "amhw_hk32f103rbt6_rcc.h"
#include "am_hk32f103rbt6_dma.h"
/**
 * \addtogroup am_if_src_hwconf_hk32f103rbt6_dma
 * \copydoc am_hwconf_hk32f103rbt6_dma.c
 * @{
 */

/**
 * \brief DMA 平台初始化。
 */
static void __hk32f103rbt6_plfm_dma_init (void)
{
    am_clk_enable(CLK_DMA);
}

/**
 * \brief DMA 平台去初始化。
 */
static void __hk32f103rbt6_plfm_dma_deinit (void)
{
    am_clk_disable(CLK_DMA);
}

/** \brief DMA 设备信息 */
static const am_hk32f103rbt6_dma_devinfo_t __g_dma_devinfo = {
    HK32F103RBT6_DMA_BASE,           /**< \brief 指向DMA寄存器块的指针 */
    INUM_DMA1_1,               /**< \brief DMA中断向量号开始 */
    INUM_DMA1_7,               /**< \brief DMA中断向量号结束 */
    __hk32f103rbt6_plfm_dma_init,    /**< \brief DMA平台初始化 */
    __hk32f103rbt6_plfm_dma_deinit   /**< \brief DMA平台解初始化 */
};

/** \brief DMA设备实例 */
static am_hk32f103rbt6_dma_dev_t __g_dma_dev;

/**
 * \brief DMA 实例初始化
 * \return 无
 */
int am_hk32f103rbt6_dma_inst_init (void)
{
    return am_hk32f103rbt6_dma_init(&__g_dma_dev, &__g_dma_devinfo);
}

/**
 * \brief DMA 实例解初始化
 * \return 无
 */
void am_hk32f103rbt6_dma_inst_deinit (void)
{
    am_hk32f103rbt6_dma_deinit();
}

/**
 * @}
 */

/* end of file */
