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
 * - 1.00 17-04-07  ari,first implementation
 * \endinternal
 */

#ifndef __AM_MM32L073_GPIO_H
#define __AM_MM32L073_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hw/amhw_mm32_gpio.h"
#include "hw/amhw_mm32_syscfg.h"
#include "hw/amhw_mm32_exti.h"
#include "am_mm32l073_gpio_util.h"

/**
 * \addtogroup am_mm32l073_if_gpio
 * \copydoc am_mm32l073_gpio.h
 * @{
 */

/**
 * \brief 引脚的触发信息
 */
struct am_mm32l073_gpio_trigger_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t pfn_callback;

    /** \brief 回调函数的参数 */
    void        *p_arg;
};

/**
 * \brief GPIO 设备信息
 */
typedef struct am_mm32l073_gpio_devinfo {

    /** \brief GPIO寄存器块基址 */
    uint32_t     gpio_regbase;

    /** \brief 系统配置SYSCFG寄存器块基址 */
    uint32_t     syscfg_regbase;

    /** \brief 外部事件EXTI控制寄存器块基址 */
    uint32_t     exti_regbase;

    /** \brief GPIO引脚中断号列表 */
    const int8_t inum_pin[3];

    /** \brief GPIO支持的引脚中断号数量 */
    size_t       pint_count;

    /** \brief 触发信息映射 */
    uint8_t     *p_infomap;

    /** \brief 指向引脚触发信息的指针 */
    struct am_mm32l073_gpio_trigger_info *p_triginfo;

    void (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void); /**< \brief 平台去初始化函数 */

} am_mm32l073_gpio_devinfo_t;

/**
 * \brief GPIO设备实例
 */
typedef struct am_mm32l073_gpio_dev {

    /** \brief 指向GPIO设备信息的指针 */
    const am_mm32l073_gpio_devinfo_t *p_devinfo;

    /** \brief 参数有效标志 */
    am_bool_t                       valid_flg;

} am_mm32l073_gpio_dev_t;

/**
 * \brief GPIO初始化
 *
 * \param[in] p_dev     : 指向GPIO设备的指针
 * \param[in] p_devinfo : 指向GPIO设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_mm32l073_gpio_init (am_mm32l073_gpio_dev_t           *p_dev,
                         const am_mm32l073_gpio_devinfo_t *p_devinfo);

/**
 * \brief GPIO去初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void am_mm32l073_gpio_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MM32L073_GPIO_H */

/* end of file */
