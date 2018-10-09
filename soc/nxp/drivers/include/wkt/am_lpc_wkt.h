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
 * \brief WKT驱动，服务Timer标准接口
 *
 * 该驱动实现了WKT的定时功能
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-02  sky, modified.
 * - 1.00 15-09-22  zxl, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_WKT_H
#define __AM_LPC_WKT_H

#include "am_timer.h"
#include "hw/amhw_lpc_wkt.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_lpc_if_wkt
 * \copydoc am_lpc_wkt.h
 * @{
 */

/**
 * \brief WKT设备信息结构体
 */
typedef struct am_lpc_wkt_devinfo {
    uint32_t                      wkt_regbase;  /**< \brief WKT寄存器块的基地址 */
    uint8_t                       inum;         /**< \brief WKT中断号 */
    uint8_t                       channel_nums; /**< \brief 支持的通道数，最大为1 */
    amhw_lpc_wkt_clksel_str_t  clksel_str;   /**< \brief 选择WKT时钟源 */

    /** \brief WKT使用外部时钟频率（没有使用WKT外部时钟则配置为0） */
    uint32_t                      wkt_ext_clock_freq;

    /** \brief WKT平台时钟初始化函数*/
    void     (*pfn_plfm_clk_init)(const struct am_lpc_wkt_devinfo * p_devinfo);

    /** \brief WKT平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief WKT平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_lpc_wkt_devinfo_t;


/**
 * \brief WKT设备
 */
typedef struct am_lpc_wkt_dev {
    am_timer_serv_t                timer_serv; /**< \brief 标准定时(Timer)服务 */
    struct {
        void (*pfn_callback)(void *);          /**< \brief 回调函数 */
        void *p_arg;                           /**< \brief 回调函数的用户参数 */
    } callback_info;                           /**< \brief 回调函数信息 */

    /** \brief 指向WKT设备信息常量的指针 */
    const am_lpc_wkt_devinfo_t *p_devinfo;
} am_lpc_wkt_dev_t;


/**
 * \brief 初始化WKT为定时功能
 *
 * \param[in] p_dev     : 指向WKT设备的指针
 * \param[in] p_devinfo : 指向WKT设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t am_lpc_wkt_init (am_lpc_wkt_dev_t     *p_dev,
                                const am_lpc_wkt_devinfo_t *p_devinfo);

/**
 * \brief 不使用WKT时，解初始化WKT，释放相关资源
 * \param[in] handle : 定时器标准服务操作句柄
 * \return 无
 */
void am_lpc_wkt_deinit (am_timer_handle_t handle);

/** @} */


#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_WKT_H */
