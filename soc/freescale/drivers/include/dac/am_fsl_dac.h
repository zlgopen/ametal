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
 * \brief DAC驱动，服务DAC标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-26  mkr, first implementation.
 * \endinternal
 */

#ifndef  __AM_FSL_DAC_H
#define  __AM_FSL_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_dac.h"
#include "hw/amhw_fsl_dac.h"

/**
 * \addtogroup am_fsl_if_dac
 * \copydoc am_fsl_dac.h
 * @{
 */
/**
 * \brief DAC 设备信息
 */
typedef struct am_fsl_dac_devinfo {

    /** \brief 指向DAC寄存器块的指针 */
    amhw_fsl_dac_t *p_hw_dac;

    /** \brief DAC中断号 */
    uint8_t    inum;

    /** \brief DAC转换精度 */
    uint8_t    bits;

    /**
     * \brief DAC参考电压，单位：mV
     *
     * \note 该参考电压由具体的电路决定
     *
     */
    uint32_t    vref;

    /**
     * \brief DAC参考电压源选择
     *
     * \note 该参考电压由具体的电路决定
     *
     */
    amhw_fsl_dac_vref_sel_t    vref_source;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_fsl_dac_devinfo_t;

/**
 * \brief DAC设备实例
 */
typedef struct am_fsl_dac_dev {

    /** \brief DAC标准服务 */
    am_dac_serv_t             dac_serve;

    /** \brief 指向DAC设备信息的指针 */
    const am_fsl_dac_devinfo_t *p_devinfo;

    /** \brief 当前转换的通道 */
    uint32_t                 chan;

} am_fsl_dac_dev_t;

/**
 * \brief DAC初始化
 *
 * 默认初始化序列A
 *
 * \param[in] p_dev     : 指向DAC设备的指针
 * \param[in] p_devinfo : 指向DAC设备信息的指针
 *
 * \return DAC标准服务操作句柄 如果为 NULL，表明初始化失败
 */
am_dac_handle_t am_fsl_dac_init(am_fsl_dac_dev_t           *p_dev,
                                 const am_fsl_dac_devinfo_t *p_devinfo);

/**
 * \brief DAC去初始化
 *
 * 默认去初始化序列A
 *
 * \param[in] handle : DAC设备的句柄值
 *
 * \return 无
 */
void am_fsl_dac_deinit(am_dac_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__AM_FSL_DAC_H  */

/* end of file */
