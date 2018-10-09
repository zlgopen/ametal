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
 * \brief LPC82X PMU 硬件操作接口描述
 * 
 * 1. 节能模式控制；
 * 2. 低功耗振荡器控制；
 * 3. 五个通用备份寄存器可在深度掉电模式下保存数据；
 * 4. 在温度范围(-40°~ 105°)，Vdd电压值范围(1.8V~3.35V)可以正常工作。
 * 注意：
 *     深度掉电模式在以下情况不能工作(勘误手册说明)：
 *         - 温度小于等于25度，Vdd电压值大于3.4V
 *         - 温度大于25度，Vdd电压值大于3.35V
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-01  sky, modified.
 * - 1.00 15-07-13  zxl, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_PMU_H
#define __AMHW_LPC82X_PMU_H

#include "ametal.h"
#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_syscon.h"

#ifdef __cplusplus
extern "C" {
#endif

 /** 
 * \addtogroup amhw_lpc82x_if_pmu
 * \copydoc amhw_lpc82x_pmu.h
 * @{
 */

/**
  * \brief LPC82x PMU 电源管理寄存器块结构体
 */
typedef struct amhw_lpc82x_pmu {
    __IO uint32_t pcon;      /**< \brief 偏移: 0x00 电源控制寄存器(R/W) */
    __IO uint32_t gpreg[4];  /**< \brief 偏移: 0x04 通用寄存器0到3(R/W) */
    __IO uint32_t dpdctrl;   /**< \brief 偏移: 0x14 深度掉电控制寄存器0(R/W) */
} amhw_lpc82x_pmu_t;


/**
 * \brief 低功耗模式
 */
typedef enum amhw_lpc82x_pmu_pm_src {
    AMHW_LPC82X_PMU_PCON_MODE_NORMAL = 0,   /**< \brief 工作模式或睡眠模式 */
    AMHW_LPC82X_PMU_PCON_MODE_DEEPSLP,      /**< \brief 深度睡眠模式 */
    AMHW_LPC82X_PMU_PCON_MODE_PD,           /**< \brief 掉电模式 */
    AMHW_LPC82X_PMU_PCON_MODE_DEEPPD,       /**< \brief 深度掉电模式 */
} amhw_lpc82x_pmu_pm_src_t;


/**
 * \brief 通用寄存器
 */
typedef enum amhw_lpc82x_pmu_gp_reg_src {
    GP_REG_0 = 0,                /**< \brief 通用寄存器0 */
    GP_REG_1,                    /**< \brief 通用寄存器1 */
    GP_REG_2,                    /**< \brief 通用寄存器2 */
    GP_REG_3,                    /**< \brief 通用寄存器3 */
} amhw_lpc82x_pmu_gp_reg_src_t;

/**
 * \brief PMU PCON PM 选择电源模式
 *
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \param[in] value    : 配置参数值为 amhw_lpc82x_pmu_pm_src_t
 *                       #AMHW_LPC82X_PMU_PCON_MODE_NORMAL   : 工作或睡眠模式
 *                       #AMHW_LPC82X_PMU_PCON_MODE_DEEPSLP  : 深度睡眠模式
 *                       #AMHW_LPC82X_PMU_PCON_MODE_PD       : 掉电模式
 *                       #AMHW_LPC82X_PMU_PCON_MODE_DEEPPD   : 深度掉电模式
 *                                               （ARM Cortex-M0+ 内核掉电）
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_pm_cfg (amhw_lpc82x_pmu_t        *p_hw_pmu,
                             amhw_lpc82x_pmu_pm_src_t  value)
{
    AM_BIT_CLR_MASK(p_hw_pmu->pcon, 0x03);
    AM_BIT_SET_MASK(p_hw_pmu->pcon, value);
}

/**
 * \brief PMU PCON NODPD 阻止器件进入深度掉电模式
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 *
 * \note 该位只有通过上电复位才可清零，因此将1写入该位将锁定器件，
 *       使其无法进入深度掉电模式。
 */
am_static_inline
void amhw_lpc82x_pmu_nodpd_enable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    AM_BIT_SET(p_hw_pmu->pcon, 3);
}

/**
 * \brief PMU PCON SLEEPFLAG 获取睡眠模式标志状态
 *
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 *
 * \retval 0 : 器件处于工作模式
 * \retval 1 : 器件已进入睡眠/深度睡眠或深度掉电模式
 */
am_static_inline
uint8_t amhw_lpc82x_pmu_sleepflag_get (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    return (((p_hw_pmu->pcon & AM_BIT(8)) != 0) ? 1 : 0);
}

/**
 * \brief PMU PCON SLEEPFLAG 写 1 SLEEPFLAG位清零
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_sleepflag_clear (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    p_hw_pmu->pcon = ((p_hw_pmu->pcon) & 0xF) | AM_BIT(8);
}

/**
 * \brief PMU PCON DPDFLAG 获取深度掉电标志状态
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \retval 0 : 不会进入深度掉电模
 * \retval 1 : 器件已进入深度掉电模式
 */
am_static_inline
uint8_t amhw_lpc82x_pmu_dpdflag_get (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    return (((p_hw_pmu->pcon & AM_BIT(11)) != 0) ? 1 : 0);
}

/**
 * \brief PMU PCON DPDFLAG 写 1 DPDFLAG位清零
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_dpdflag_clear (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    p_hw_pmu->pcon = ((p_hw_pmu->pcon) & 0xF) | AM_BIT(11);
}

/**
 * \brief PMU GPDATA 设置深度掉电模式下保留数据
 *
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \param[in] chan     : 通用寄存器 0~3 (#GP_REG_0)
 * \param[in] value    : 设置需要保留的数据
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_pmu_gpdata_save (amhw_lpc82x_pmu_t            *p_hw_pmu,
                                  amhw_lpc82x_pmu_gp_reg_src_t  chan,
                                  uint32_t                      value)
{
    p_hw_pmu->gpreg[chan] = value;
}

/**
 * \brief PMU GPDATA 读取深度掉电模式下保留数据
 *
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \param[in] chan     : 通用寄存器 0~3 (#GP_REG_0)
 *
 * \return 返回对应寄存器所保留的值
 */
am_static_inline 
uint32_t amhw_lpc82x_pmu_gpdata_get (amhw_lpc82x_pmu_t            *p_hw_pmu,
                                     amhw_lpc82x_pmu_gp_reg_src_t  chan)
{
    return (p_hw_pmu->gpreg[chan]);
}

/**
 * \brief PMU DPDCTRL WAKEUPHYS WAKEUP引脚迟滞使能
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakeuphys_enable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    AM_BIT_SET(p_hw_pmu->dpdctrl, 0);
}

/**
 * \brief PMU DPDCTRL WAKEUPHYS WAKEUP引脚迟滞禁能
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakeuphys_disable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    AM_BIT_CLR(p_hw_pmu->dpdctrl, 0);
}

/**
 * \brief PMU DPDCTRL WAKEPAD_DISABLE WAKEUP引脚使能(使能PIO0_4唤醒功能)
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakepad_enable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
     AM_BIT_CLR(p_hw_pmu->dpdctrl, 1);
}

/**
 * \brief PMU DPDCTRL WAKEPAD_DISABLE WAKEUP引脚禁能(禁能PIO0_4唤醒功能)
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakepad_disable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
     AM_BIT_SET(p_hw_pmu->dpdctrl, 1);
}

/**
 * \brief PMU DPDCTRL LPOSCEN 使能低功耗振荡器
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_lposcen_enable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
     AM_BIT_SET(p_hw_pmu->dpdctrl, 2);
}

/**
 * \brief PMU DPDCTRL LPOSCEN 禁能低功耗振荡器
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_lposcen_disable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
     AM_BIT_CLR(p_hw_pmu->dpdctrl, 2);
}

/**
 * \brief PMU DPDCTRL LPOSCDPDEN 深度掉电模式下使能低功耗振荡器
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_lposcdpden_enable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
     AM_BIT_SET(p_hw_pmu->dpdctrl, 3);
}

/**
 * \brief PMU DPDCTRL LPOSCDPDEN 深度掉电模式下禁能低功耗振荡器
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_lposcdpden_disable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
     AM_BIT_CLR(p_hw_pmu->dpdctrl, 3);
}

/**
 * \brief PMU DPDCTRL WAKEUPCLKHYS WAKEUP引脚迟滞使能
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakeupclkhys_enable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    AM_BIT_SET(p_hw_pmu->dpdctrl, 4);
}

/**
 * \brief PMU DPDCTRL WAKEUPCLKHYS WAKEUP引脚迟滞禁能
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakeupclkhys_disable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    AM_BIT_CLR(p_hw_pmu->dpdctrl, 4);
}

/**
 * \brief PMU DPDCTRL WAKECLKPAD_DISABLE WAKEUP引脚使能(使能PIO0_28外部时钟功能)
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakeclkpad_enable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    AM_BIT_SET(p_hw_pmu->dpdctrl, 5);
}

/**
 * \brief PMU DPDCTRL WAKECLKPAD_DISABLE WAKEUP引脚禁能(禁能PIO0_28外部时钟功能)
 * \param[in] p_hw_pmu : 指向PMU电源控制寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc82x_pmu_wakeclkpad_disable (amhw_lpc82x_pmu_t *p_hw_pmu)
{
    AM_BIT_CLR(p_hw_pmu->dpdctrl, 5);
}


/**
 * @} amhw_lpc82x_if_pmu
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_PMU_H */

/* end of file */
