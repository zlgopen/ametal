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
 * \brief PWR 驱动接口声明
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-23  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HK32F103RBT6_PWR_H
#define __AM_HK32F103RBT6_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hw/amhw_hk32f103rbt6_pwr.h"
#include "../bkp/hw/amhw_hk32f103rbt6_bkp.h"

/**
 * \addtogroup am_hk32f103rbt6_if_pwr
 * \copydoc am_hk32f103rbt6_pwr.h
 * @{
 */

/**
 * \brief PWR 模式
 */
typedef enum am_hk32f103rbt6_pwr_mode {
    AM_HK32F103RBT6_PWR_MODE_RUN = 0,      /**< \brief 运行模式 */
    AM_HK32F103RBT6_PWR_MODE_SLEEP,        /**< \brief 睡眠模式 */
    AM_HK32F103RBT6_PWR_MODE_STOP,         /**< \brief 停止模式 */
    AM_HK32F103RBT6_PWR_MODE_STANBY,       /**< \brief 待机模式 */
} am_hk32f103rbt6_pwr_mode_t;

/** \brief PWR PVD 监测电压信息配置 */
typedef struct am_hk32f103rbt6_pwr_pvd_info {
    amhw_hk32f103rbt6_pvd_lever_set_t pvd_v_level; /**< \brief PVD 监测电压选择 */

    /**
     *  \brief PVD 监测电压模式先择.
     *   0：表示电压从高下降到低于设定阀值时产生中断
     *   1：表示电压从低上升到高于设定阀值时产生中断；
     *   2：表示电压上升或下降越过设定阀值时都产生中断
     */
    uint8_t                     pvd_mode;

    am_bool_t                   enable;      /**< \brief 是否使能 PVD 电压监测 */
} am_hk32f103rbt6_pwr_pvd_info_t;

/**
 * \brief PWR 模式及唤醒引脚
 */
typedef struct am_hk32f103rbt6_pwr_mode_init {
    am_hk32f103rbt6_pwr_mode_t  pwr_mode; /**< \brief 指向 PWR 寄存器块的指针 */
    int                   pin;      /**< \brief 唤醒引脚 */
} am_hk32f103rbt6_pwr_mode_init_t;

/**
 * \brief PWR 设备信息结构体
 */
typedef struct am_hk32f103rbt6_pwr_devinfo {

    uint32_t  pwr_regbase;                   /**< \brief PWR 寄存器块基址 */

    uint32_t  bkp_regbase;                   /**< \brief BKP 寄存器块基址 */

    int       ahb_clk_num;                   /**< \brief AHB 时钟 ID */
    int       apb1_clk_num;                  /**< \brief APB1 时钟 ID */
    int       apb2_clk_num;                  /**< \brief APB2 时钟 ID */

    uint8_t   inum;                          /**< \brief PVD 中断号 */

    am_hk32f103rbt6_pwr_mode_init_t *p_pwr_mode;   /**< \brief 指向 PWR 模式数组的指针 */

    am_hk32f103rbt6_pwr_pvd_info_t  *p_pvd_info;   /**< \brief 指向 PVD 电压监控信息的指针 */

    void (*pfn_plfm_init)(void);             /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void);           /**< \brief 平台去初始化函数 */

} am_hk32f103rbt6_pwr_devinfo_t;

/**
 * \brief PWR 设备结构体
 */
typedef struct am_hk32f103rbt6_pwr_dev {

    /** \brief 指向 PWR 设备信息指针 */
    const am_hk32f103rbt6_pwr_devinfo_t *p_pwrdevinfo;

    /** \brief PWR 模式 */
    am_hk32f103rbt6_pwr_mode_t           pwr_mode;

} am_hk32f103rbt6_pwr_dev_t;

/** \brief PWR 设备操作句柄类型定义 */
typedef am_hk32f103rbt6_pwr_dev_t *am_hk32f103rbt6_pwr_handle_t;

/**
 * \brief PWR 初始化
 *
 * \param[in] p_dev     指向 PWR 设备结构体的指针
 * \param[in] p_devinfo 指向 PWR 设备信息结构体常量的指针
 *
 * \return 返回 am_hk32f103rbt6_pwr_handle_t 类型的句柄
 */
am_hk32f103rbt6_pwr_handle_t
am_hk32f103rbt6_pwr_init (am_hk32f103rbt6_pwr_dev_t           *p_dev,
                    const am_hk32f103rbt6_pwr_devinfo_t *p_devinfo);

/**
 * \brief PWR 去初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void am_hk32f103rbt6_pwr_deinit (void);

/**
 * \brief 获取系统模式
 *
 * \param[in] mode         PWR 模式定义，值为 am_hk32f103rbt6_pwr_mode_t 这一枚举类型
 * \param[in] pfn_callback 唤醒回调函数
 * \param[in] p_arg        唤醒回调函数 参数
 *
 * \return 无
 *
 * \note 这个唤醒的配置的 PWR 模式必须与 am_hk32f103rbt6_pwr_mode_into 函数的选择 PWR
 *       模式相同，在 AM_HK32F103RBT6_PWR_MODE_STANBY 模式下，唤醒会复位，注册中断唤醒
 *       函数回调函数不起作用
 */
void am_hk32f103rbt6_wake_up_cfg (am_hk32f103rbt6_pwr_mode_t mode,
                            am_pfnvoid_t         pfn_callback,
                            void                *p_arg);

/**
 * \brief 配置 PWR 模式
 *
 * \param[in] mode PWR 模式定义
 *
 * \retval  AM_OK      模式切换有效
 * \retval  AM_ERROR   模式切换错误
 * \retval -AM_EINVAL  式参数错误
 * \retval -AM_ENOTSUP WAKE_UP 为高电平，不能进入待机模式
 *
 * \note 1. 只有在 WKUP 为低电平的时候才能进入待机模式， 且待机模式唤醒后将执行复位操作
 *       2. 配置 PWR 模式为 AM_HK32F103RBT6_PWR_MODE_STANBY 失败后，WKUP 引脚会被配置为下拉
 *          输入模式
 */
int am_hk32f103rbt6_pwr_mode_into (am_hk32f103rbt6_pwr_mode_t mode);

/**
 * \brief 获取 PWR 模式
 *
 * \param[in] 无
 *
 * \return am_hk32f103rbt6_pwr_mode_t 这一枚举类型的值
 */
am_hk32f103rbt6_pwr_mode_t am_hk32f103rbt6_pwr_mode_get (void);

/**
 * \brief 配置 PVD 电压检测信息
 *
 * \param[in] pwr_handle   PWR 电源管理设备句柄
 * \param[in] pfn_callback 电压监测回调函数
 * \param[in] p_arg        电压监测回调函数参数
 *
 * \retval AM_OK 配置设置成功
 */
int am_hk32f103rbt6_pwr_pvd_cfg (am_hk32f103rbt6_pwr_handle_t pwr_handle,
                           am_pfnvoid_t           pfn_callback,
                           void                  *p_arg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HK32F103RBT6_PWR_H */

/* end of file */
