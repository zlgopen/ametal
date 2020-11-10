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
 * \internal
 * \par Modification history
 * - 1.00 19-10-08  zp, first implementation
 * \endinternal
 */

#ifndef __AM_ZSL42x_ADTIM_CAP_H
#define __AM_ZSL42x_ADTIM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "hw/amhw_zsl42x_adtim.h"

/**
 * \addtogroup am_zsl42x_if_adtim_cap
 * \copydoc am_zsl42x_adtim_cap.h
 * @{
 */

#define  AM_ZSL42x_ADTIM_CAP_CHA  0x00   /**< \brief CAP通道A标识符*/
#define  AM_ZSL42x_ADTIM_CAP_CHB  0x01   /**< \brief CAP通道B标识符*/

/**
 * \brief TIM捕获功能相关的GPIO信息
 */
typedef struct am_zsl42x_adtim_cap_ioinfo {
    int8_t   channel;        /**< \brief CAP所使用的通道标识符 */
    uint32_t gpio;           /**< \brief 对应的GPIO管脚 */
    uint32_t func;           /**< \brief 为捕获功能时的GPIO功能设置 */
    uint32_t dfunc;          /**< \brief 禁能管脚捕获功能时的默认GPIO功能设置 */
} am_zsl42x_adtim_cap_ioinfo_t;

/**
 * \brief TIM捕获功能相关的设备信息
 */
typedef struct am_zsl42x_adtim_cap_devinfo {
    uint32_t                      tim_regbase;   /**< \brief TIM寄存器块基址 */
    uint8_t                       inum;          /**< \brief TIM对应的中断号 */
    amhw_zsl42x_adtim_pclk_div_t  clk_div;       /**< \brief 时钟分频系数 */
    uint8_t                       channels_num;  /**< \brief 使用的通道数，最大为2 */
    am_zsl42x_adtim_cap_ioinfo_t *p_ioinfo;      /**< \brief 指向捕获管脚信息结构体 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                          (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                          (*pfn_plfm_deinit)(void);
} am_zsl42x_adtim_cap_devinfo_t;

/**
 * \brief TIM捕获功能设备
 */
typedef struct am_zsl42x_adtim_cap_dev {
    am_cap_serv_t          cap_serv;      /**< \brief 标准CAP服务 */

    struct {
        am_cap_callback_t  callback_func; /**< \brief 回调函数 */
        void              *p_arg;         /**< \brief 回调函数的用户参数 */
    } adtim_callback_info[2];             /**< \brief 存储最大2通道的回调函数信息 */

    /** \brief 指向TIM(捕获功能)设备信息常量的指针 */
    const am_zsl42x_adtim_cap_devinfo_t *p_devinfo;

    uint8_t                chan_max;      /**< \brief 有效的最大通道数  */

} am_zsl42x_adtim_cap_dev_t;

/**
 * \brief 初始化TIM为捕获功能
 *
 * \param[in] p_dev     : 指向TIM(捕获功能)设备的指针
 * \param[in] p_devinfo : 指向TIM(捕获功能)设备信息常量的指针
 *
 * \return CAP标准服务操作句柄，值为NULL时表明初始化失败
 */
am_cap_handle_t am_zsl42x_adtim_cap_init (am_zsl42x_adtim_cap_dev_t              *p_dev,
                                        const am_zsl42x_adtim_cap_devinfo_t *p_devinfo);

/**
 * \brief 不使用TIM捕获功能时，解初始化TIM捕获功能，释放相关资源
 *
 * \param[in] handle : am_zsl42x_adtim_cap_init() 初始化函数获得的cap服务句柄
 *
 * \return 无
 */
void am_zsl42x_adtim_cap_deinit (am_cap_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_ADTIM_CAP_H */

/* end of file */
