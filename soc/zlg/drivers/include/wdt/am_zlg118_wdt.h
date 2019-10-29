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
 * \brief WDT驱动层接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10
 * \endinternal
 */

#ifndef __AM_ZLG118_WDT_H
#define __AM_ZLG118_WDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_wdt.h"
#include "hw/amhw_zlg118_wdt.h"

/**
 * \addtogroup am_zlg118_if_wdt
 * \copydoc am_zlg118_wdt.h
 * @{
 */

/* WDT溢出时间配置 */
typedef enum am_zlg118_wdt_wov {
    ZLG118_WDT_1_6MS = 0x0,
    ZLG118_WDT_3_2MS = 0x1,
    ZLG118_WDT_6_4MS = 0x2,
    ZLG118_WDT_13MS  = 0x3,
    ZLG118_WDT_26MS  = 0x4,
    ZLG118_WDT_51MS  = 0x5,
    ZLG118_WDT_102MS = 0x6,
    ZLG118_WDT_205MS = 0x7,
    ZLG118_WDT_500MS = 0x8,
    ZLG118_WDT_820MS = 0x9,
    ZLG118_WDT_1_64S = 0xa,
    ZLG118_WDT_3_28S = 0xb,
    ZLG118_WDT_6_55S = 0xc,
    ZLG118_WDT_13_1S = 0xd,
    ZLG118_WDT_26_2S = 0xe,
    ZLG118_WDT_52_4S = 0xf
} am_zlg118_wdt_wov_t;

/**
 * \brief WDT设备信息结构体
 */
typedef struct am_zlg118_wdt_devinfo {

    /** \brief WDT寄存器块基地址 */
    uint32_t  wdt_regbase;

    /** \brief 时钟ID */
    int       clk_num;

    /** \brief 平台初始化函数 */
    void    (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void    (*pfn_plfm_deinit)(void);

} am_zlg118_wdt_devinfo_t;

/**
 * \brief WDT设备结构体
 */
typedef struct am_zlg118_wdt_dev {

    /** \brief WDT提供的标准服务 */
    am_wdt_serv_t                   wdt_serv;

    /** \brief 预分频因子 */
    uint16_t                        div;

    /** \brief 指向WDT设备的信息指针 */
    const am_zlg118_wdt_devinfo_t  *p_devinfo;
} am_zlg118_wdt_dev_t;


/**
 * \brief WDT初始化
 *
 * \param[in] p_dev     : 指向WDT设备
 * \param[in] p_devinfo : 指向WDT设备信息
 *
 * \return WDT标准服务句柄，失败则返回NULL
 */
am_wdt_handle_t am_zlg118_wdt_init (am_zlg118_wdt_dev_t            *p_dev,
                                    const am_zlg118_wdt_devinfo_t  *p_devinfo);

/**
 * \brief 解除WDT设备初始化
 *
 * \param[in] handle : 使用WDT初始化函数am_zlg118_wdt_init()获取到的handle
 *
 * \return 无
 */
void am_zlg118_wdt_deinit (am_wdt_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __AM_ZLG118_WDT_H */

#endif

/* end of file */

