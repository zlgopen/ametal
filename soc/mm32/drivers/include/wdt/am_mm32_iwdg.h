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
 * \brief IWDG驱动层接口
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-07  sss, first implementation
 * \endinternal
 */

#ifndef __AM_MM32_IWDG_H
#define __AM_MM32_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_wdt.h"

/**
 * \addtogroup am_mm32_if_iwdg
 * \copydoc am_mm32_iwdg.h
 * @{
 */

/**
 * \brief IWDG设备信息结构体
 */
typedef struct am_mm32_iwdg_devinfo {

    /** \brief IWDG寄存器块基地址 */
    uint32_t iwdg_regbase;

    /** \brief 平台初始化函数 */
    void   (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void   (*pfn_plfm_deinit)(void);

} am_mm32_iwdg_devinfo_t;

/**
 * \brief IWDG设备结构体
 */
typedef struct am_mm32_iwdg_dev {

    /** \brief IWDG提供的标准服务 */
    am_wdt_serv_t                 wdt_serv;

    /** \brief 预分频因子 */
    uint16_t                      div;

    /** \brief 指向IWDG设备的信息指针 */
    const am_mm32_iwdg_devinfo_t  *p_devinfo;
} am_mm32_iwdg_dev_t;

/**
 * \brief IWDG初始化
 *
 * \param[in] p_dev     : 指向IWDG设备
 * \param[in] p_devinfo : 指向IWDG设备信息
 *
 * \return IWDG标准服务句柄，失败则返回NULL
 */
am_wdt_handle_t am_mm32_iwdg_init (am_mm32_iwdg_dev_t              *p_dev,
                                     const am_mm32_iwdg_devinfo_t *p_devinfo);

/**
 * \brief 解除IWDG设备初始化
 *
 * \param[in] handle : 使用IWDG初始化函数am_mm32_iwdg_init()获取到的handle
 *
 * \return 无
 */
void am_mm32_iwdg_deinit (am_wdt_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __AMHW_IWDG_H */

#endif

/* end of file */

