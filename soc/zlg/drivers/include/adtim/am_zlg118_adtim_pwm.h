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
 * \brief 定时器ADTIM驱动，服务PWM标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-09  zp, first implementation
 * \endinternal
 */

#ifndef __AM_ZLG118_ADTIM_PWM_H
#define __AM_ZLG118_ADTIM_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_pwm.h"
#include "hw/amhw_zlg118_adtim.h"

/**
 * \addtogroup am_zlg118_if_adtim_pwm
 * \copydoc am_zlg118_adtim_pwm.h
 * @{
 */

/* PWM输出极性 */
#define  AM_ZLG118_ADTIM_PWM_OCPOLARITY_HIGH 0  /**< \brief 高电平有效 */
#define  AM_ZLG118_ADTIM_PWM_OCPOLARITY_LOW  1  /**< \brief 低电平有效 */

/**
 * \brief 通道标识符
 *
 * \note CHA 和 CHB可用于单独PWM输出，也可用于互补PWM输出。
 */
#define  AM_ZLG118_ADTIM_PWM_CHA  0x00   /**< \brief PWM通道A标识符*/
#define  AM_ZLG118_ADTIM_PWM_CHB  0x01   /**< \brief PWM通道B标识符*/

/**
 * \brief ADTIMPWM输出功能相关的GPIO信息
 */
typedef struct am_zlg118_adtim_pwm_chaninfo {
    int8_t   channel;                  /**< \brief PWM所使用的通道标识符 */
    int8_t   gpio;                     /**< \brief PWM输出所用的GPIO引脚 */
    uint32_t func;                     /**< \brief PWM功能的GPIO功能设置值 */
    uint32_t dfunc;                    /**< \brief 禁能PWM模式后，默认GPIO功能设置值 */
} am_zlg118_adtim_pwm_chaninfo_t;

/**
 * \brief ADTIMPWM输出功能相关的设备信息
 */
typedef struct am_zlg118_adtim_pwm_devinfo {
    uint32_t                     adtim_regbase;    /**< \brief ADTIM寄存器块基址 */

    uint8_t                      channels_num;   /**< \brief 使用的通道数，最大为2 */

    /** \brief 互补PWM选择  1：互补PWM   0：独立PWM */
    uint8_t                     comp;

    /** \brief PWM输出极性,AM_ZLG118_LPTIM_PWM_OCPOLARITY_HIGH或AM_ZLG118_LPTIM_PWM_OCPOLARITY_LOW */
    uint8_t                     ocpolarity;

    am_zlg118_adtim_pwm_chaninfo_t  *p_chaninfo;     /**< \brief 指向PWM输出通道信息结构体 */


    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                      (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                      (*pfn_plfm_deinit)(void);

} am_zlg118_adtim_pwm_devinfo_t;

/**
 * \brief ADTIMPWM输出功能设备结构体
 */
typedef struct am_zlg118_adtim_pwm_dev {

    am_pwm_serv_t                      pwm_serv;   /**< \brief 标准PWM服务 */

    /** \brief 指向ADTIM(PWM输出功能)设备信息常量的指针 */
    const am_zlg118_adtim_pwm_devinfo_t *p_devinfo;

    uint8_t                            chan_max;   /**< \brief 有效的最大通道数  */

} am_zlg118_adtim_pwm_dev_t;

/**
 * \brief 初始化ADTIM为PWM输出功能
 *
 * \param[in] p_dev     : 指向ADTIM(PWM输出功能)设备的指针
 * \param[in] p_devinfo : 指向ADTIM(PWM输出功能)设备信息常量的指针
 *
 * \return PWM标准服务操作句柄，值为NULL时表明初始化失败
 */
am_pwm_handle_t am_zlg118_adtim_pwm_init(
                    am_zlg118_adtim_pwm_dev_t           *p_dev,
                    const am_zlg118_adtim_pwm_devinfo_t *p_devinfo);

/**
 * \brief 不使用ADTIMPWM输出功能时，解初始化ADTIMPWM输出功能，释放相关资源
 *
 * \param[in] handle : am_zlg118_adtim_pwm_init() 初始化函数获得的PWM服务句柄
 *
 * \return 无
 */
void am_zlg118_adtim_pwm_deinit (am_pwm_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG118_ADTIM_PWM_H */

/* end of file */
