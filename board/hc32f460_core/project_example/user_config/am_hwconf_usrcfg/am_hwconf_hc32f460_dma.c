/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg118.cn/
*******************************************************************************/

/**
 * \file
 * \brief HC32F460 DMA 用户配置文件
 * \sa am_hwconf_hc32f460_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-02-27  cds, first implementation
 * \endinternal
 */

#include "am_clk.h"
#include "am_hc32f460.h"
#include "hw/amhw_hc32f460_rcc.h"
#include "am_hc32f460_dma.h"
/**
 * \addtogroup am_if_src_hwconf_hc32f460_dma
 * \copydoc am_hwconf_hc32f460_dma.c
 * @{
 */

/**
 * \brief DMA1 平台初始化。
 */
static void __hc32f460_plfm_dma1_init (void)
{
    am_clk_enable(CLK_DMA1);
}

/**
 * \brief DMA1 平台去初始化。
 */
static void __hc32f460_plfm_dma1_deinit (void)
{
    am_clk_disable(CLK_DMA1);
}

/** \brief DMA1 设备信息 */
static const am_hc32f460_dma_devinfo_t __g_dma1_devinfo = {
    HC32F460_DMA1_BASE,           /**< \brief 指向DMA寄存器块的指针 */
	INUM_DMA1,                    /**< \brief DMA中断向量号 */
	1,                            /**< \brief DMA设备ID */
    __hc32f460_plfm_dma1_init,    /**< \brief DMA平台初始化 */
    __hc32f460_plfm_dma1_deinit   /**< \brief DMA平台解初始化 */
};

/** \brief DMA1设备实例 */
am_hc32f460_dma_dev_t __g_dma1_dev;

/**
 * \brief DMA1 实例初始化
 * \return 无
 */
int am_hc32f460_dma1_inst_init (void)
{
    return am_hc32f460_dma_init(&__g_dma1_dev, &__g_dma1_devinfo);
}

/**
 * \brief DMA1 实例解初始化
 * \return 无
 */
void am_hc32f460_dma1_inst_deinit (void)
{
    am_hc32f460_dma_deinit(&__g_dma1_dev);
}


/**
 * \brief DMA2 平台初始化。
 */
static void __hc32f460_plfm_dma2_init (void)
{
    am_clk_enable(CLK_DMA2);
}

/**
 * \brief DMA1 平台去初始化。
 */
static void __hc32f460_plfm_dma2_deinit (void)
{
    am_clk_disable(CLK_DMA2);
}

/** \brief DMA2 设备信息 */
static const am_hc32f460_dma_devinfo_t __g_dma2_devinfo = {
    HC32F460_DMA2_BASE,           /**< \brief 指向DMA2寄存器块的指针 */
	INUM_DMA2,                    /**< \brief DMA2中断向量号 */
	2,                            /**< \brief DMA2设备ID */
    __hc32f460_plfm_dma2_init,    /**< \brief DMA2平台初始化 */
    __hc32f460_plfm_dma2_deinit   /**< \brief DMA2平台解初始化 */
};

/** \brief DMA2设备实例 */
am_hc32f460_dma_dev_t __g_dma2_dev;

/**
 * \brief DMA2 实例初始化
 * \return 无
 */
int am_hc32f460_dma2_inst_init (void)
{
    return am_hc32f460_dma_init(&__g_dma2_dev, &__g_dma2_devinfo);
}

/**
 * \brief DMA2 实例解初始化
 * \return 无
 */
void am_hc32f460_dma2_inst_deinit (void)
{
    am_hc32f460_dma_deinit(&__g_dma2_dev);
}

/**
 * @}
 */

/* end of file */
