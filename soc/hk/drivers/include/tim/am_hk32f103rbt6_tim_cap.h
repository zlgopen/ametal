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
 * \brief 定时器TIM驱动，服务CAP标准接口
 *
 * 1. TIM支持提供如下三种标准服务，本驱动提供的是服务“捕获”标准服务的驱动：
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. TIM1、TIM2、TIM3可配置4个捕获通道，TIM14、TIM16、TIM17可配置1个捕获通道。
 *
 * \internal
 * \par Modification history
 * - 1.00 17-4-21  nwt, first implementation
 * \endinternal
 */

#ifndef __AM_HK32F103RBT6_TIM_CAP_H
#define __AM_HK32F103RBT6_TIM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "hw/amhw_hk32f103rbt6_tim.h"

/**
 * \addtogroup am_hk32f103rbt6_if_tim_cap
 * \copydoc am_hk32f103rbt6_tim_cap.h
 * @{
 */

/**
 * \brief TIM捕获功能相关的GPIO信息
 */
typedef struct am_hk32f103rbt6_tim_cap_ioinfo {
    uint32_t gpio;           /**< \brief 对应的GPIO管脚 */
    uint32_t func;           /**< \brief 为捕获功能时的GPIO功能设置 */
    uint32_t dfunc;          /**< \brief 禁能管脚捕获功能时的默认GPIO功能设置 */
} am_hk32f103rbt6_tim_cap_ioinfo_t;

/**
 * \brief TIM捕获功能相关的设备信息
 */
typedef struct am_hk32f103rbt6_tim_cap_devinfo {
    uint32_t                    tim_regbase;   /**< \brief TIM寄存器块基址 */
    uint8_t                     inum;          /**< \brief TIM对应的中断号 */
    int                         clk_num;       /**< \brief 时钟ID */
    uint8_t                     channels_num;  /**< \brief 使用的通道数，最大为6 */
    am_hk32f103rbt6_tim_cap_ioinfo_t    *p_ioinfo;      /**< \brief 指向捕获管脚信息结构体 */
    amhw_hk32f103rbt6_tim_type_t         tim_type;      /**< \brief 定时器类型 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                      (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                      (*pfn_plfm_deinit)(void);
} am_hk32f103rbt6_tim_cap_devinfo_t;

/**
 * \brief TIM捕获功能设备
 */
typedef struct am_hk32f103rbt6_tim_cap_dev {
    am_cap_serv_t          cap_serv;      /**< \brief 标准CAP服务 */

    struct {
        am_cap_callback_t  callback_func; /**< \brief 回调函数 */
        void              *p_arg;         /**< \brief 回调函数的用户参数 */
    } callback_info[4];                   /**< \brief 存储最大6通道的回调函数信息 */

    /** \brief 指向TIM(捕获功能)设备信息常量的指针 */
    const am_hk32f103rbt6_tim_cap_devinfo_t *p_devinfo;

} am_hk32f103rbt6_tim_cap_dev_t;

/**
 * \brief 初始化TIM为捕获功能
 *
 * \param[in] p_dev     : 指向TIM(捕获功能)设备的指针
 * \param[in] p_devinfo : 指向TIM(捕获功能)设备信息常量的指针
 *
 * \return CAP标准服务操作句柄，值为NULL时表明初始化失败
 */
am_cap_handle_t am_hk32f103rbt6_tim_cap_init (am_hk32f103rbt6_tim_cap_dev_t              *p_dev,
                                        const am_hk32f103rbt6_tim_cap_devinfo_t *p_devinfo);

/**
 * \brief 不使用TIM捕获功能时，解初始化TIM捕获功能，释放相关资源
 *
 * \param[in] handle : am_hk32f103rbt6_tim_cap_init() 初始化函数获得的cap服务句柄
 *
 * \return 无
 */
void am_hk32f103rbt6_tim_cap_deinit (am_cap_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HK32F103RBT6_TIM_CAP_H */

/* end of file */
