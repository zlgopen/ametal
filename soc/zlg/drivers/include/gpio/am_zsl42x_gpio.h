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
 * \brief GPIO驱动，服务GPIO标准接口
 *
 * \internal
 * \par Modification history
 * - 1.01 20-04-26 zcb, Fix the problem that only one pin can be
 *                      configured as external interrupt
 * - 1.00 19-09-06  zp, first implementation
 * \endinternal
 */

#ifndef __AM_ZSL42x_GPIO_H
#define __AM_ZSL42x_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hw/amhw_zsl42x_gpio.h"
#include "am_zsl42x_gpio_util.h"

/**
 * \addtogroup am_zsl42x_if_gpio
 * \copydoc am_zsl42x_gpio.h
 * @{
 */

/**
 * \brief 引脚的触发信息
 */
struct am_zsl42x_gpio_trigger_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t                        pfn_callback;

    /** \brief 回调函数的参数 */
    void                               *p_arg;
};

/**
 * \brief GPIO 设备信息
 */
typedef struct am_zsl42x_gpio_devinfo {

    /** \brief GPIO寄存器块基址 */
    uint32_t                            gpio_regbase;

    /** \brief 引脚中断号列表 */
    const int8_t                        inum_pin[4];

    /** \brief 支持外部中断的 GPIO 最大引脚编号 */
    const uint8_t                       exti_num_max;

    /** \brief 触发方式 */
    uint8_t                            *p_trigger;

    /** \brief 指向引脚触发信息的指针 */
    struct am_zsl42x_gpio_trigger_info   *p_triginfo;

    /** \brief 平台初始化函数 */
    void                              (*pfn_plfm_init) (void);

    /** \brief 平台去初始化函数 */
    void                              (*pfn_plfm_deinit) (void);

} am_zsl42x_gpio_devinfo_t;

/**
 * \brief GPIO设备实例
 */
typedef struct am_zsl42x_gpio_dev {

    /** \brief 指向GPIO设备信息的指针 */
    const am_zsl42x_gpio_devinfo_t       *p_devinfo;

    /** \brief 参数有效标志 */
    am_bool_t                           valid_flg;

} am_zsl42x_gpio_dev_t;

/**
 * \brief GPIO初始化
 *
 * \param[in] p_dev     : 指向GPIO设备的指针
 * \param[in] p_devinfo : 指向GPIO设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_zsl42x_gpio_init (am_zsl42x_gpio_dev_t           *p_dev,
                       const am_zsl42x_gpio_devinfo_t *p_devinfo);

/**
 * \brief GPIO解初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void am_zsl42x_gpio_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_GPIO_H */

/* end of file */
