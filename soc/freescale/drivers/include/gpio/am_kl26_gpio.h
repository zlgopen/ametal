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
 * - 1.01 16-09-15  nwt, make some changes.
 * - 1.00 15-10-21  wxj, first implementation.
 * \endinternal
 */

#ifndef __AM_KL26_GPIO_H
#define __AM_KL26_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "../../../kl26/kl26_pin.h"
#include "hw/amhw_kl26_gpio.h"
#include "hw/amhw_kl26_pinctrl.h"

/**
 * \addtogroup am_kl26_if_gpio
 * \copydoc am_kl26_gpio.h
 * @{
 */

/**
 * \brief 引脚的触发信息
 */
struct am_kl26_gpio_trigger_info {

    /** \brief 触发回调函数   */
    am_pfnvoid_t  pfn_callback;

    /** \brief 回调函数的参数 */
    void         *p_arg;
};

/**
 * \brief GPIO 设备信息
 */
typedef struct am_kl26_gpio_devinfo {

    /** \brief 指向PORT寄存器块的指针.  */
    amhw_kl26_port_t    *p_hw_port;

    /** \brief 指向GPIO寄存器块的指针.  */
    amhw_kl26_gpio_t    *p_hw_gpio;

    /** \brief GPIO引脚中断号列表.     */
    const int8_t    inum_pin[2];

    /** \brief GPIO支持的引脚中断号数量. */
    size_t    pint_count;

    /** \brief 触发方式映射.           */
    uint8_t  *p_infomap;

    /** \brief 指向引脚触发信息的指针.   */
    struct am_kl26_gpio_trigger_info  *p_triginfo;

    /** \brief 平台初始化函数.  */
    void (*pfn_plfm_init)(void);

    /** \brief 平台去初始化函数. */
    void (*pfn_plfm_deinit)(void);

} am_kl26_gpio_devinfo_t;

/**
 * \brief GPIO设备实例
 */
typedef struct am_kl26_gpio_dev {

    /** \brief 指向GPIO设备信息的指针         */
    const am_kl26_gpio_devinfo_t *p_devinfo;

    /** \brief 参数有效标志                   */
    am_bool_t  valid_flg;

} am_kl26_gpio_dev_t;

/**
 * \brief GPIO初始化
 *
 * \param[in] p_dev     : 指向GPIO设备的指针
 * \param[in] p_devinfo : 指向GPIO设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_kl26_gpio_init(am_kl26_gpio_dev_t           *p_dev,
                      const am_kl26_gpio_devinfo_t *p_devinfo);

/**
 * \brief GPIO去初始化
 *
 * \return 无
 */
void am_kl26_gpio_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_KL26_GPIO_H */

/* end of file */
