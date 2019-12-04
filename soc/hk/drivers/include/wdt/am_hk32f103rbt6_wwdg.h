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
 * \brief WWDG驱动层接口
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-07  sss, first implementation
 * \endinternal
 */

#ifndef __AM_HK32F103RBT6_WWDG_H
#define __AM_HK32F103RBT6_WWDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_wdt.h"

/**
 * \addtogroup am_hk32f103rbt6_if_wwdg
 * \copydoc am_hk32f103rbt6_wwdg.h
 * @{
 */

/**
 * \brief WWDG（版本0）设备信息结构体
 */
typedef struct am_hk32f103rbt6_wwdg_devinfo {

    /** \brief WWDG寄存器块基地址 */
    uint32_t  wwdg_regbase;

    /** \brief 时钟ID */
    int       clk_num;

    /** \brief 平台初始化函数 */
    void    (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void    (*pfn_plfm_deinit)(void);

} am_hk32f103rbt6_wwdg_devinfo_t;

/**
 * \brief WWDG（版本0）设备结构体
 */
typedef struct am_hk32f103rbt6_wwdg_dev {

    /** \brief WWDG提供的标准服务 */
    am_wdt_serv_t                   wdt_serv;

    /** \brief 预分频因子 */
    uint16_t                        div;

    /** \brief 计数值 */
    uint8_t                         count;

    /** \brief 指向WWDG（版本0）设备的信息指针 */
    const am_hk32f103rbt6_wwdg_devinfo_t *p_devinfo;
} am_hk32f103rbt6_wwdg_dev_t;

/**
 * \brief WWDG（版本0）初始化
 *
 * \param[in] p_dev     : 指向WWDG（版本0）设备
 * \param[in] p_devinfo : 指向WWDG（版本0）设备信息
 *
 * \return WWDG标准服务句柄，失败则返回NULL
 */
am_wdt_handle_t am_hk32f103rbt6_wwdg_init (am_hk32f103rbt6_wwdg_dev_t               *p_dev,
                                  const am_hk32f103rbt6_wwdg_devinfo_t  *p_devinfo);

/**
 * \brief 解除WWDG（版本0）设备初始化
 *
 * \param[in] handle : 使用WWDG（版本0）初始化函数am_hk32f103rbt6_wwdg_init()获取到的handle
 *
 * \return 无
 */
void am_hk32f103rbt6_wwdg_deinit (am_wdt_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __AM_HK32F103RBT6_WWDG_H */

#endif

/* end of file */

