/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.hc32.cn/
*******************************************************************************/

/**
 * \file
 * \brief HC32 DMA 用户配置文件
 * \sa am_hwconf_hc32_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-18  zp, first implementation
 * \endinternal
 */

#include "am_clk.h"
#include "am_hc32f07x.h"
#include "hw/amhw_hc32_rcc.h"
#include "am_hc32_dma.h"
/**
 * \addtogroup am_if_src_hwconf_hc32_dma
 * \copydoc am_hwconf_hc32_dma.c
 * @{
 */

/**
 * \brief DMA 平台初始化。
 */
static void __hc32_plfm_dma_init (void)
{
    am_clk_enable(CLK_DMA);
}

/**
 * \brief DMA 平台去初始化。
 */
static void __hc32_plfm_dma_deinit (void)
{
    am_clk_disable(CLK_DMA);
}

/** \brief DMA 设备信息 */
static const am_hc32_dma_devinfo_t __g_dma_devinfo = {
    HC32_DMA_BASE,           /**< \brief 指向DMA寄存器块的指针 */
    INUM_DMA,                  /**< \brief DMA中断向量号 */
    __hc32_plfm_dma_init,    /**< \brief DMA平台初始化 */
    __hc32_plfm_dma_deinit   /**< \brief DMA平台解初始化 */
};

/** \brief DMA设备实例 */
static am_hc32_dma_dev_t __g_dma_dev;

/**
 * \brief DMA 实例初始化
 * \return 无
 */
int am_hc32_dma_inst_init (void)
{
    return am_hc32_dma_init(&__g_dma_dev, &__g_dma_devinfo);
}

/**
 * \brief DMA 实例解初始化
 * \return 无
 */
void am_hc32_dma_inst_deinit (void)
{
    am_hc32_dma_deinit();
}

/**
 * @}
 */

/* end of file */
