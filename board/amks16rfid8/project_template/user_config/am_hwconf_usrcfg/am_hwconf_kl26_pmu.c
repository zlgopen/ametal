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
 * \brief KL16 PMU 用户配置文件
 * \sa am_kl26_hwconfig_pmu.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "kl26_periph_map.h"
#include "hw/amhw_arm_nvic.h"
#include "am_kl26_pmu.h"
#include "kl26_inum.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_pmu
 * \copydoc am_kl26_hwconfig_pmu.c
 * @{
 */

/** \brief PMU 平台初始化 */
void __kl26_plfm_pmu_init (void)
{

}

/** \brief 解除PMU 平台初始化 */
void __kl26_plfm_pmu_deinit (void)
{
	amhw_arm_nvic_disable(INUM_LLWU);
}
/**
 * \brief PMU 设备信息
 */
const  struct am_kl26_pmu_devinfo  __g_pmu_devinfo = {
    KL26_SMC,                                /**< \brief SMC寄存器指针       */
    KL26_PMC,                                /**< \brief PMC寄存器指针       */
    KL26_RCM,                                /**< \brief RCM寄存器指针       */
    KL26_LLWU,                               /**< \brief LLWU寄存器指针      */
    AM_KL26_PMU_ARG_VLLS0_POR_DISABLE |      /**< \brief VLLS0禁能电源检测   */
    AM_KL26_PMU_ARG_VLLSx_ISOACK_CLEAR,      /**< \brief 自动清除唤醒ACK标志 */
    __kl26_plfm_pmu_init,                    /**< \brief PMU平台初始化 */
    __kl26_plfm_pmu_deinit                   /**< \brief PMU平台去初始化 */
};

/** \brief PMU 设备实例 */
am_kl26_pmu_dev_t __g_pmu_dev;

/**
 * \brief  PMU 实例初始化，初始化系统时钟
 * \retval AM_OK : 成功初始化
 */
int am_kl26_pmu_inst_init (void)
{
    return am_kl26_pmu_init(&__g_pmu_dev, &__g_pmu_devinfo);
}

/** \brief PMU实例解初始化 */
void am_kl26_pmu_inst_deinit (void)
{
    am_kl26_pmu_deinit(&__g_pmu_dev);
}

/**
 * @}
 */

/* end of file */
