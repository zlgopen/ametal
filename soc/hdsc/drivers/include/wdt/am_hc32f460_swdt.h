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
 * \brief SWDT驱动层接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

#ifndef __AM_HC32F460_SWDT_H
#define __AM_HC32F460_SWDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_wdt.h"
#include "hw/amhw_hc32f460_swdt.h"

/**
 * \addtogroup am_hc32f460_if_swdt
 * \copydoc am_hc32f460_swdt.h
 * @{
 */

/**
 * \name swdt状态标志
 * \anchor am_hc32f460_swdt_statud_flags
 * @{
 */
#define AM_HC32F460_SWDT_STATUS_REFRESH_ERROR        (0)  /** \brief 刷新错误标志*/
#define AM_HC32F460_SWDT_STATUS_COUNT_UNDERFLOW      (1)  /** \brief 计数下溢标志 */

typedef enum {
    AM_HC32F460_SWDT_TRIGGER_EVENT_REQUEST_INT   = 0,
    AM_HC32F460_SWDT_TRIGGER_EVENT_REQUEST_RESET = 1,
}am_hc32f460_swdt_trigger_event_type_t;


/** \brief SWDT中断回调函数类型 */
typedef void (*am_hc32f460_pfn_swdt_isr_t)(void *p_arg, uint32_t flag);



/**
 * \brief SWDT设备信息结构体
 */
typedef struct am_hc32f460_swdt_devinfo {

    /** \brief SWDT寄存器块基地址 */
    uint32_t  swdt_regbase;

    /** \brief 时钟ID */
    int       clk_num;

    /** \brief 中断号 */
    int       inum;

    /** \brief 触发事件类型（中断/复位） */
    uint16_t  event_type;

    /** \brief 平台初始化函数 */
    void    (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void    (*pfn_plfm_deinit)(void);

} am_hc32f460_swdt_devinfo_t;



/**
 * \brief WDT设备结构体
 */
typedef struct am_hc32f460_swdt_dev {

    /** \brief SWDT提供的标准服务 */
    am_wdt_serv_t                   swdt_serv;

    /** \brief swdt触发回调函数 */
    void (*pfn_isr)(void *p_arg, uint32_t flag);

    /** \brief 回调函数的第一个入口参数 */
    void                    *p_arg;

    /** \brief 指向WDT设备的信息指针 */
    const am_hc32f460_swdt_devinfo_t  *p_devinfo;
} am_hc32f460_swdt_dev_t;

/**
 * \brief 连接SWDT中断服务函数
 *
 * \param[in] p_dev     : 指向SWDT设备
 * \param[in] p_devinfo : 指向连接SWDT中断服务函数
 * \param[in] p_arg     : 回调函数参数
 *
 * \return 成功则返回AM_OK，其他则为失败
 */
int am_hc32f460_swdt_isr_connect (am_hc32f460_swdt_dev_t     *p_dev,
                                  am_hc32f460_pfn_swdt_isr_t  pfn_isr,
                                  void                       *p_arg);

/**
 * \brief 删除SWDT中断服务函数
 *
 * \param[in] p_dev     : 指向SWDT设备
 * \param[in] p_devinfo : 指向连接SWDT中断服务函数
 * \param[in] p_arg     : 回调函数参数
 *
 * \return 成功则返回AM_OK，其他则为失败
 */
int am_hc32f460_swdt_isr_disconnect (am_hc32f460_swdt_dev_t    *p_dev,
                                     am_hc32f460_pfn_swdt_isr_t pfn_isr,
                                     void                      *p_arg);


/**
 * \brief SWDT初始化
 *
 * \param[in] p_dev     : 指向SWDT设备
 * \param[in] p_devinfo : 指向SWDT设备信息
 *
 * \return WDT标准服务句柄，失败则返回NULL
 */
am_wdt_handle_t am_hc32f460_swdt_init (am_hc32f460_swdt_dev_t            *p_dev,
                                       const am_hc32f460_swdt_devinfo_t  *p_devinfo);

/**
 * \brief 解除SWDT设备初始化
 *
 * \param[in] handle : 使用WDT初始化函数am_hc32f460_swdt_init()获取到的handle
 *
 * \return 无
 */
void am_hc32f460_swdt_deinit (am_wdt_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __AM_HC32F460_SWDT_H */

#endif

/* end of file */

