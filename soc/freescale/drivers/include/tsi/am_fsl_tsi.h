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
 * \brief TSI驱动层
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-26  nwt, make some changes.
 * - 1.00 15-10-21  wxj, first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_TSI_H
#define __AM_FSL_TSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "hw/amhw_fsl_tsi.h"
#include "am_int.h"

/**
 * \addtogroup am_fsl_if_tsi
 * \copydoc am_fsl_tsi.h
 * @{
 */

/**
 * \brief TSI 设备信息结构体
 */
typedef struct am_fsl_tsi_devinfo {

    amhw_fsl_tsi_t  *p_hw_tsi;  /**< \brief 指向TSI寄存器块的指针 */
    am_bool_t           lp_mode;    /**< \brief 低功耗唤醒MCU功能 */

    /** \brief TSI中断号 */
    uint8_t    inum;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_fsl_tsi_devinfo_t;

/**
 * \brief TSI设备实例
 */
typedef struct am_fsl_tsi_dev {

    /** \brief 指向TSI设备信息的指针 */
    const am_fsl_tsi_devinfo_t *p_devinfo;

} am_fsl_tsi_dev_t;

/**
 * \brief TSI功能配置
 *
 * \param[in] chan      : TSI 通道号
 * \param[in] work_mode : TSI 工作模式
 * \param[in] trig_mode : TSI 扫描触发模式
 *
 * \return 功能配置操作成功
 */
int am_fsl_tsi_fun_cfg (amhw_fsl_tsi_channel_t    chan,
                         amhw_fsl_tsi_mode_t       work_mode,
                         amhw_fsl_tsi_trig_mode_t  trig_mode);

/**
 * \brief TSI中断触发模式
 *
 * \param[in] flag  : 中断触发模式
 *
 * \return 读取的转换值
 */
void am_fsl_tsi_trigger_cfg (amhw_fsl_tsi_irq_mode_t flag);

/**
 * \brief TSI中断使能
 *
 * \return 无
 */
void am_fsl_tsi_int_enable (void);

/**
 * \brief TSI中断禁能
 *
 * \return 无
 */
void am_fsl_tsi_int_disable (void);

/**
 * \brief TSI中断服务函数连接
 *
 * \param[in] pfn_callback  : 中断服务函数接口
 *
 * \return 无
 */
int am_fsl_tsi_trigger_connect (am_pfnvoid_t pfn_callback);

 /**
 * \brief TSI 灵敏度设置
 *
 * \param[in] val   : 灵敏度值
 *
 * \return 无
 */
void am_fsl_tsi_sens_set (uint16_t val);

/**
 * \brief TSI初始化
 *
 * \param[in] p_dev     : 指向TSI设备的指针
 * \param[in] p_devinfo : 指向TSI设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_fsl_tsi_init (am_fsl_tsi_dev_t *p_dev, const am_fsl_tsi_devinfo_t *p_devinfo);

/**
 * \brief TSI 解初始化
 *
 * \param[in] p_dev : 指向TSE设备的指针
 *
 * \return 无
 */
void am_fsl_tsi_deinit (am_fsl_tsi_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_FSL_TSI_H */

/* end of file */
