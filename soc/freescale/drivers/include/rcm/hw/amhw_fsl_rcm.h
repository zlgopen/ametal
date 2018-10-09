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
 * \brief RCM 硬件操作接口
 * 
 * 1. 获取复位原因；
 * 2. 配置复位引脚滤波参数。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-14  mkr, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_RCM_H
#define __AMHW_FSL_RCM_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_bitops.h"
/**
 * \addtogroup amhw_fsl_if_rcm
 * \copydoc amhw_fsl_rcm.h
 * @{
 */

/**
 * \brief RCM寄存器块结构体
 */
typedef struct amhw_fsl_rcm {
    __I  uint16_t srs;            /**< \brief RCM 复位状态寄存器0        */
    __IO uint8_t  rpfc;           /**< \brief RCM 复位滤波控制寄存器     */
    __IO uint8_t  rpfw;           /**< \brief RCM 复位滤波宽度寄存器     */
} amhw_fsl_rcm_t;

/**
 * \name 复位原因标志
 * @{
 */

#define AMHW_FSL_RCM_RESTFLAG_SACKERR AM_BIT(13)  /**< \brief 停止模式进入错误复位 */
#define AMHW_FSL_RCM_RESTFLAG_MDM_AP  AM_BIT(11)  /**< \brief 调试复位             */
#define AMHW_FSL_RCM_RESTFLAG_SW      AM_BIT(10)  /**< \brief 软件复位             */
#define AMHW_FSL_RCM_RESTFLAG_LOCKUP  AM_BIT(9)   /**< \brief 内核Lockup事件复位   */
#define AMHW_FSL_RCM_RESTFLAG_POR     AM_BIT(7)   /**< \brief 上电复位             */
#define AMHW_FSL_RCM_RESTFLAG_PIN     AM_BIT(6)   /**< \brief REST引脚复位         */
#define AMHW_FSL_RCM_RESTFLAG_WDOG    AM_BIT(5)   /**< \brief 看门狗复位           */
#define AMHW_FSL_RCM_RESTFLAG_LOL     AM_BIT(3)   /**< \brief 锁相环异常           */
#define AMHW_FSL_RCM_RESTFLAG_LOC     AM_BIT(2)   /**< \brief 时钟丢失复位         */
#define AMHW_FSL_RCM_RESTFLAG_LVD     AM_BIT(1)   /**< \brief 低电压检测复位       */
#define AMHW_FSL_RCM_RESTFLAG_WAKEUP  AM_BIT(0)   /**< \brief 唤醒复位             */

/** @} */

/**
 * \name 停止模式，复位引脚的滤波时钟选项
 * @{
 */

/** \brief 停止模式不对复位引脚滤波 */
#define AMHW_FSL_RCM_RESPIN_FILT_STOP_DISABLE    0

 /** \brief 停止模式使用根据LPO时钟对复位引脚滤波 */
#define AMHW_FSL_RCM_RESPIN_FILT_STOP_LPO     AM_BIT(2)

/** @} */

/**
 * \name 运行和等待模式，复位引脚滤波时钟选项
 * @{
 */
 
/** \brief 运行和等待模式不对复位引脚滤波 */
#define AMHW_FSL_RCM_RESPIN_FILT_RUNW_DISABLE   00

/** \brief 运行和等待模式使用LPO时钟对复位引脚滤波 */
#define AMHW_FSL_RCM_RESPIN_FILT_RUNW_LPO      0x01

/** \brief 运行和等待模式使用BUS时钟复位引脚滤波 */
#define AMHW_FSL_RCM_RESPIN_FILT_RUNW_BUS      0x10

/** @} */

/**
 * \brief 获取复位原因标志
 * \param[in] p_hw_rcm : 指向PMC寄存器块的指针
 * \return 复位原因标志
 */
am_static_inline
uint16_t amhw_fsl_rcm_rest_flags_get (amhw_fsl_rcm_t *p_hw_rcm)
{
    return p_hw_rcm->srs;
}

/**
 * \brief 配置复位引脚停止模式滤波时钟选项
 *
 * \param[in] p_hw_rcm : 指向PMC寄存器块的指针
 * \param[in] flag     : 滤波时钟配置选项，AMHW_FSL_RCM_RESPIN_FILT_STOP_*宏值
 *                       (#AMHW_FSL_RCM_RESPIN_FILT_STOP_LPO)
 * \return 无
 *
 * \note 如果LPO时钟没有开启，那么滤波禁能
 */
am_static_inline
void amhw_fsl_rcm_respin_filt_stop_cfg (amhw_fsl_rcm_t *p_hw_rcm, uint8_t flag)
{
    p_hw_rcm->rpfc = flag | (p_hw_rcm->rpfc & (~0x4));
}

/**
 * \brief 配置复位引脚运行和等待模式滤波时钟选项
 *
 * \param[in] p_hw_rcm : 指向PMC寄存器块的指针
 * \param[in] flag     : 滤波时钟配置选项，AMHW_FSL_RCM_RESPIN_FILT_RUNW_*宏值
 *                      (#AMHW_FSL_RCM_RESPIN_FILT_RUNW_BUS)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_rcm_respin_filt_runw_cfg (amhw_fsl_rcm_t *p_hw_rcm, uint8_t flag)
{
    p_hw_rcm->rpfc = flag | (p_hw_rcm->rpfc & (~0x03));
}

/**
 * \brief 配置复位引脚运行模式滤波BUS时钟个数
 *
 * \param[in] p_hw_rcm : 指向PMC寄存器块的指针
 * \param[in] count    : BUS时钟的个数(1-32)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_rcm_respin_filt_busc_cfg (amhw_fsl_rcm_t *p_hw_rcm, uint8_t count)
{
    p_hw_rcm->rpfw = (count - 1) & 0x1F;
}

/** 
 * @} amhw_if_fsl_rcm
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_RCM_H */

/*end of file */
