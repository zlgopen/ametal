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
 * \brief KL26 DMA 用户配置文件
 * \sa am_kl26_hwconfig_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-17  mkr, first implementation.
 * \endinternal
 */

#include "am_kl26.h"
#include "hw/amhw_kl26_sim.h"
#include "am_kl26_dma.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_dma
 * \copydoc am_kl26_hwconfig_dma.c
 * @{
 */

/**
 * \brief DMA 平台初始化。
 */
void __kl26_plfm_dma_init (void)
{
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_DMAMUX);
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_DMA);
}

/**
 * \brief DMA 平台去初始化。
 */
void __kl26_plfm_dma_deinit (void)
{
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_DMAMUX);
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_DMA);
}

/** \brief DMA 设备信息 */
static const am_kl26_dma_devinfo_t __g_dma_devinfo = {
    KL26_DMAMUX,              /**< \brief 指向DMAMUX寄存器块的指针  */
    KL26_DMA,                 /**< \brief 指向DMA寄存器块的指针          */
    INUM_DMA_CH0,                  /**< \brief DMA通道0中断向量号                 */

    __kl26_plfm_dma_init,
    __kl26_plfm_dma_deinit
};

/** \brief DMA设备实例 */
static am_kl26_dma_dev_t __g_dma_dev;

/** \brief DMA 实例初始化，获得ADC标准服务句柄 */
int am_kl26_dma_inst_init (void)
{
    return am_kl26_dma_init(&__g_dma_dev, &__g_dma_devinfo);
}

/** \brief DMA 实例解初始化 */
void am_kl26_dma_inst_deinit (void)
{
    am_kl26_dma_deinit();
}

/**
 * @}
 */

/* end of file */

