/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief GPIO驱动，服务GPIO标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-08  zxl, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC82X_GPIO_H
#define __AM_LPC82X_GPIO_H

#include "ametal.h"
#include "lpc82x_pin.h"
#include "hw/amhw_lpc82x_pint.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \addtogroup am_lpc82x_if_gpio
 * \copydoc am_lpc82x_gpio.h
 * @{
 */

/**
 * \brief 引脚的触发信息 
 */
struct am_lpc82x_gpio_trigger_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t  pfn_callback;

    /** \brief 回调函数的参数 */
    void *p_arg;
};

/**
 * \brief GPIO 设备信息
 */
typedef struct am_lpc82x_gpio_devinfo {
    
    /** \brief SWM寄存器块基址 */
    uint32_t  swm_regbase;
    
    /** \brief GPIO寄存器块基址 */
    uint32_t  gpio_regbase;
    
    /** \brief IOCON寄存器块基址 */
    uint32_t iocon_regbase;

    /** \brief 引脚中断寄存器块基址 */
    uint32_t  pint_regbase;

    /** \brief GPIO引脚中断号列表 */
    const int8_t inum_pin[AMHW_LPC82X_PINT_CHAN_NUM];

    /** \brief GPIO支持的引脚中断号数量 */
    size_t       pint_count;

    /** \brief 触发信息映射 */
    uint8_t     *p_infomap;

    /** \brief 指向引脚触发信息的指针 */
    struct am_lpc82x_gpio_trigger_info *p_triginfo;
    
    /** \brief 平台初始化函数 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
    
} am_lpc82x_gpio_devinfo_t;

/**
 * \brief GPIO设备实例 
 */
typedef struct am_lpc82x_gpio_dev {
    
    /** \brief 指向GPIO设备信息的指针 */
    const am_lpc82x_gpio_devinfo_t *p_devinfo;

    /** \brief 参数有效标志 */
    am_bool_t                      valid_flg;

    /** \brief 引脚可配置功能表，存储当前可配置的功能编号 */
    uint8_t                         pin_moved_func[LPC82X_PIN_NUM];

} am_lpc82x_gpio_dev_t;

/**
 * \brief GPIO初始化 
 *
 * \param[in] p_dev     : 指向GPIO设备的指针 
 * \param[in] p_devinfo : 指向GPIO设备信息的指针 
 *
 * \retval AM_OK : 操作成功 
 */
int am_lpc82x_gpio_init (am_lpc82x_gpio_dev_t           *p_dev,
                         const am_lpc82x_gpio_devinfo_t *p_devinfo);

/**
 * \brief GPIO去初始化
 * \return 无 
 */
void am_lpc82x_gpio_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC82X_GPIO_H */

/* end of file */
