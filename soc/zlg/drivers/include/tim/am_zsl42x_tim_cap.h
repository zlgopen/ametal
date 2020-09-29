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

#ifndef __AM_ZSL42x_TIM_CAP_H
#define __AM_ZSL42x_TIM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "hw/amhw_zsl42x_tim.h"

/**
 * \addtogroup am_zsl42x_if_tim_cap
 * \copydoc am_zsl42x_tim_cap.h
 * @{
 */

/**
 * \brief 通道标识符
 *
 * \note 对于定时器012，只有CH0A和CH0B两个通道。
 *       对于定时器3，有6个通道，分别为CH0A、CH0B，
 *                              CH1A、CH1B
 *                              CH2A、CH2B
 */
#define  AM_ZSL42x_TIM_CAP_CH0A        0x00   /**< \brief CAP通道0标识符*/
#define  AM_ZSL42x_TIM_CAP_CH0B        0x01   /**< \brief CAP通道1标识符*/
#define  AM_ZSL42x_TIM_CAP_CH1A        0x02   /**< \brief CAP通道2标识符*/
#define  AM_ZSL42x_TIM_CAP_CH1B        0x03   /**< \brief CAP通道3标识符*/
#define  AM_ZSL42x_TIM_CAP_CH2A        0x04   /**< \brief CAP通道4标识符*/
#define  AM_ZSL42x_TIM_CAP_CH2B        0x05   /**< \brief CAP通道5标识符*/

/**
 * \brief TIM捕获功能相关的GPIO信息
 */
typedef struct am_zsl42x_tim_cap_ioinfo {
    int8_t   channel;        /**< \brief CAP所使用的通道标识符 */
    uint32_t gpio;           /**< \brief 对应的GPIO管脚 */
    uint32_t func;           /**< \brief 为捕获功能时的GPIO功能设置 */
    uint32_t dfunc;          /**< \brief 禁能管脚捕获功能时的默认GPIO功能设置 */
} am_zsl42x_tim_cap_ioinfo_t;

/**
 * \brief TIM捕获功能相关的设备信息
 */
typedef struct am_zsl42x_tim_cap_devinfo {
    uint32_t                    tim_regbase;   /**< \brief TIM寄存器块基址 */
    uint8_t                     inum;          /**< \brief TIM对应的中断号 */
    amhw_zsl42x_tim_clkdiv_t    clk_div;       /**< \brief 时钟分频系数 */
    uint8_t                     channels_num;  /**< \brief 使用的通道数，最大为2 */
    amhw_zsl42x_tim_type_t      tim_type;      /**< \brief 定时器类型 */
    am_zsl42x_tim_cap_ioinfo_t *p_ioinfo;      /**< \brief 指向捕获管脚信息结构体 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                      (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                      (*pfn_plfm_deinit)(void);
} am_zsl42x_tim_cap_devinfo_t;

/**
 * \brief TIM捕获功能设备
 */
typedef struct am_zsl42x_tim_cap_dev {
    am_cap_serv_t          cap_serv;      /**< \brief 标准CAP服务 */

    struct {
        am_cap_callback_t  callback_func; /**< \brief 回调函数 */
        void              *p_arg;         /**< \brief 回调函数的用户参数 */
    } callback_info[6];                   /**< \brief 存储最大6通道的回调函数信息 */

    /** \brief 指向TIM(捕获功能)设备信息常量的指针 */
    const am_zsl42x_tim_cap_devinfo_t *p_devinfo;

    uint8_t                chan_max;      /**< \brief 有效的最大通道数  */

} am_zsl42x_tim_cap_dev_t;

/**
 * \brief 初始化TIM为捕获功能
 *
 * \param[in] p_dev     : 指向TIM(捕获功能)设备的指针
 * \param[in] p_devinfo : 指向TIM(捕获功能)设备信息常量的指针
 *
 * \return CAP标准服务操作句柄，值为NULL时表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim_cap_init (am_zsl42x_tim_cap_dev_t              *p_dev,
                                        const am_zsl42x_tim_cap_devinfo_t *p_devinfo);

/**
 * \brief 不使用TIM捕获功能时，解初始化TIM捕获功能，释放相关资源
 *
 * \param[in] handle : am_zsl42x_tim_cap_init() 初始化函数获得的cap服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim_cap_deinit (am_cap_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_TIM_CAP_H */

/* end of file */
