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
 * - 1.00 19-02-21  ipk,first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_GPIO_H
#define __AM_HC32F460_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hw/amhw_hc32f460_gpio.h"

/**
 * \addtogroup am_hc32f460_if_gpio
 * \copydoc am_hc32f460_gpio.h
 * @{
 */

/**
 * \brief 引脚的触发信息
 */
struct am_hc32f460_gpio_trigger_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t  pfn_callback;

    /** \brief 回调函数的参数 */
    void         *p_arg;
};

/**
 * \brief GPIO 设备信息
 */
typedef struct am_hc32f460_gpio_devinfo {

    /** \brief GPIO寄存器块基址 */
    uint32_t      gpio_regbase;

    /** \brief EXTI寄存器块基址 */
    uint32_t      exti_regbase;

    /** \brief SYSCFG寄存器块基址 */
    uint32_t      syscfg_regbase;

    /** \brief EXIT中断号列表 */
    const uint8_t  inum_pin[16];

    /** \brief GPIO引脚数量 */
    const uint8_t pin_count;

    /** \brief GPIO使用的最大外部中断线编号+1 */
    const uint8_t exti_num_max;

    /** \brief 触发信息映射 */
    uint8_t      *p_infomap;

    /** \brief 指向引脚触发信息的指针 */
    struct am_hc32f460_gpio_trigger_info   *p_triginfo;

    void (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void); /**< \brief 平台去初始化函数 */

} am_hc32f460_gpio_devinfo_t;

/**
 * \brief GPIO设备实例
 */
typedef struct am_hc32f460_gpio_dev {

    /** \brief 指向GPIO设备信息的指针 */
    const am_hc32f460_gpio_devinfo_t *p_devinfo;

    /** \brief 参数有效标志 */
    am_bool_t                         valid_flg;

    /** \brief 引脚触发中断类别 */
    uint8_t                         int_type;

} am_hc32f460_gpio_dev_t;

/**
 * \brief GPIO初始化
 *
 * \param[in] p_dev     : 指向GPIO设备的指针
 * \param[in] p_devinfo : 指向GPIO设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_hc32f460_gpio_init (am_hc32f460_gpio_dev_t           *p_dev,
                           const am_hc32f460_gpio_devinfo_t *p_devinfo);

/**
 * \brief GPIO解初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void am_hc32f460_gpio_deinit (void);

void Extint00_IrqHandler (void);
void Extint01_IrqHandler (void);
void Extint02_IrqHandler (void);
void Extint03_IrqHandler (void);
void Extint04_IrqHandler (void);
void Extint05_IrqHandler (void);
void Extint06_IrqHandler (void);
void Extint07_IrqHandler (void);
void Extint08_IrqHandler (void);
void Extint09_IrqHandler (void);
void Extint10_IrqHandler (void);
void Extint11_IrqHandler (void);
void Extint12_IrqHandler (void);
void Extint13_IrqHandler (void);
void Extint14_IrqHandler (void);
void Extint15_IrqHandler (void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_GPIO_H */

/* end of file */
