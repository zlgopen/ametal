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
 * - 1.00 19-09-24
 * \endinternal
 */

#ifndef  __AM_ZSL42x_DAC_H
#define  __AM_ZSL42x_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_dac.h"

/**
 * \addtogroup am_zsl42x_if_dac
 * \copydoc am_zsl42x_dac.h
 * @{
 */

/**
 * \brief DAC数据对齐方式
 */
#define AM_ZSL42x_DAC_ALIGN_WAY_12_RIGHT 0  /** \brief 12位数据右对齐 */
#define AM_ZSL42x_DAC_ALIGN_WAY_12_LEFT  1  /** \brief 12位数据左对齐 */
#define AM_ZSL42x_DAC_ALIGN_WAY_8_RIGHT  2  /** \brief 8 位数据右对齐 */

/**
 * \brief DAC触发方式
 */
#define AM_ZSL42x_DAC_CHAN_TIM0_TRGO    (0U)    /** \brief TIM0 */
#define AM_ZSL42x_DAC_CHAN_TIM1_TRGO    (1U)    /** \brief TIM1 */
#define AM_ZSL42x_DAC_CHAN_TIM2_TRGO    (2U)    /** \brief TIM2 */
#define AM_ZSL42x_DAC_CHAN_TIM3_TRGO    (3U)    /** \brief TIM3 */
#define AM_ZSL42x_DAC_CHAN_TIM4_TRGO    (4U)    /** \brief TIM4 */
#define AM_ZSL42x_DAC_CHAN_TIM5_TRGO    (5U)    /** \brief TIM5 */
#define AM_ZSL42x_DAC_CHAN_SOFTWARE_TRG (6U)    /** \brief 软件触发 */
#define AM_ZSL42x_DAC_CHAN_EXTER        (7U)    /** \brief 外部事件触发 */

/**
 * \brief DAC设备信息
 */
typedef struct am_zsl42x_dac_devinfo {

    /** \brief DAC寄存器块基地址 */
    uint32_t   dac_reg_base;

    /** \brief DAC转换精度 */
    uint8_t    bits;

    /**
     * \brief DAC参考电压源
     */
    uint32_t   vref_rsc;

    /**
     * \brief DAC参考电压，单位：mV
     *
     * \note 该参考电压由具体的电路决定
     *
     */
    uint32_t   vref;

    /**
     * \brief DAC数据对齐方式
     */
    uint8_t    align_way;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_zsl42x_dac_devinfo_t;

/**
 * \brief DAC设备实例
 */
typedef struct am_zsl42x_dac_dev {

    /** \brief DAC标准服务 */
    am_dac_serv_t               dac_serve;

    /** \brief 指向DAC设备信息的指针 */
    const am_zsl42x_dac_devinfo_t *p_devinfo;

    /** \brief 当前转换的通道 */
    uint32_t                    chan;

} am_zsl42x_dac_dev_t;

/**
 * \brief DAC初始化
 *
 *
 * \param[in] p_dev     : 指向DAC设备的指针
 * \param[in] p_devinfo : 指向DAC设备信息的指针
 *
 * \return DAC标准服务操作句柄 如果为 NULL，表明初始化失败
 */
am_dac_handle_t am_zsl42x_dac_init (am_zsl42x_dac_dev_t           *p_dev,
                                    const am_zsl42x_dac_devinfo_t *p_devinfo);

/**
 * \brief DAC去初始化
 *
 *
 * \param[in] handle : DAC设备的句柄值
 *
 * \return 无
 */
void am_zsl42x_dac_deinit (am_dac_handle_t handle);

/**
 * \brief DAC 触发方式选择
 */
int am_zsl42x_dac_tri_way_sel (am_dac_handle_t handle, uint32_t tri_way);

/**
 * \brief DAC软件触发
 *
 * \param[in] handle : 设备句柄
 *
 * \return : 无
 */
void am_zsl42x_dac_soft_tri (am_dac_handle_t handle);

/**
 * \brief DAC 转换数据获取
 */
void am_zsl42x_dac_convt_data_get (am_dac_handle_t handle, uint16_t *p_out_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_DAC_H */

/* end of file */
