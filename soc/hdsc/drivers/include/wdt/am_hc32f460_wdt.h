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
 * - 1.00 20-05-12
 * \endinternal
 */

#ifndef __AM_HC32F460_WDT_H
#define __AM_HC32F460_WDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_wdt.h"
#include "hw/amhw_hc32f460_wdt.h"

/**
 * \addtogroup am_hc32f460_if_wdt
 * \copydoc am_hc32f460_wdt.h
 * @{
 */
/**
 * \name wdt状态标志
 * \anchor am_hc32f460_wdt_status_flags
 * @{
 */
#define AM_HC32F460_WDT_STATUS_REFRESH_ERROR        (0)  /** \brief 刷新错误标志*/
#define AM_HC32F460_WDT_STATUS_COUNT_UNDERFLOW      (1)  /** \brief 计数下溢标志 */




/** \brief WDT中断回调函数类型 */
typedef void (*am_hc32f460_pfn_wdt_isr_t)(void *p_arg, uint32_t flag);

typedef enum {
    AM_HC32F460_WDT_TRIGGER_EVENT_REQUEST_INT   = 0,
    AM_HC32F460_WDT_TRIGGER_EVENT_REQUEST_RESET = 1,
}am_hc32f460_wdt_trigger_event_type_t;

typedef enum {
    AM_HC32F460_WDT_SLEEPOFF   = 0,
    AM_HC32F460_WDT_SLEEPON    = 1,
}am_hc32f460_wdt_sleepoff_t;

typedef enum {
    AM_HC32F460_WDT_REFRESH_RANGE_100              = 0,  /* 100% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_25             = 1,  /* 0%~25% */
    AM_HC32F460_WDT_REFRESH_RANGE_25_50            = 2,  /* 25%~50% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_50             = 3,  /* 0%~50% */
    AM_HC32F460_WDT_REFRESH_RANGE_50_75            = 4,  /* 50%~75% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_25_AND_50_75   = 5,  /* 0%~25% & 50%~75% */
    AM_HC32F460_WDT_REFRESH_RANGE_25_75            = 6,  /* 25%~75% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_75             = 7,  /* 0%~75% */
    AM_HC32F460_WDT_REFRESH_RANGE_75_100           = 8,  /* 75%~100% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_25_AND_75_100  = 9,  /* 0%~25% & 75%~100% */
    AM_HC32F460_WDT_REFRESH_RANGE_25_50_AND_75_100 = 10, /* 25%~50% & 75%~100% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_50_AND_75_100  = 11, /* 0%~50% & 75%~100% */
    AM_HC32F460_WDT_REFRESH_RANGE_50_100           = 12, /* 50%~100% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_25_AND_50_100  = 13, /* 0%~25% & 50%~100% */
    AM_HC32F460_WDT_REFRESH_RANGE_25_100           = 14, /* 25%~100% */
    AM_HC32F460_WDT_REFRESH_RANGE_0_100            = 15, /* 0%~100% */
}am_hc32f460_wdt_refresh_region_t;



/**
 * \brief WDT设备信息结构体
 */
typedef struct am_hc32f460_wdt_devinfo {

    /** \brief WDT寄存器块基地址 */
    uint32_t  wdt_regbase;

    /** \brief 时钟ID */
    int       clk_num;

    /** \brief 中断号 */
    int       inum;

    /** \brief 刷新错误/溢出后产生的事件类型（中断/复位） */
    am_hc32f460_wdt_trigger_event_type_t   EVENT_TYPE;

    /** \brief WDT在sleep模式下计数禁止/许可 */
    am_hc32f460_wdt_sleepoff_t             SLPOFF;

    /** \brief 刷新允许区域 */
    am_hc32f460_wdt_refresh_region_t       REFRESH_REGION;

    /** \brief 平台初始化函数 */
    void    (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void    (*pfn_plfm_deinit)(void);

} am_hc32f460_wdt_devinfo_t;



/**
 * \brief WDT设备结构体
 */
typedef struct am_hc32f460_wdt_dev {

    /** \brief WDT提供的标准服务 */
    am_wdt_serv_t                   wdt_serv;

    /** \brief 预分频因子 */
    uint16_t                          div;

    /** \brief 刷新错误/计数溢出触发事件类型（中断/复位）  */
    uint16_t                          trigger_type;

    /** \brief sleep模式下是否使能计数  */
    am_bool_t                         sleep_count;

    /** \brief wdt触发回调函数 */
    void (*pfn_isr)(void *p_arg, uint32_t flag);

    /** \brief 回调函数的第一个入口参数 */
    void                    *p_arg;

    /** \brief 指向WDT设备的信息指针 */
    const am_hc32f460_wdt_devinfo_t  *p_devinfo;
} am_hc32f460_wdt_dev_t;


/**
 * \brief 连接WDT中断服务函数
 *
 * \param[in] p_dev     : 指向WDT设备
 * \param[in] p_devinfo : 指向连接WDT中断服务函数
 * \param[in] p_arg     : 回调函数参数
 *
 * \return 成功则返回AM_OK，其他则为失败
 */
int am_hc32f460_wdt_isr_connect (am_hc32f460_wdt_dev_t     *p_dev,
                                 am_hc32f460_pfn_wdt_isr_t  pfn_isr,
                                 void                      *p_arg);

/**
 * \brief 删除WDT中断服务函数
 *
 * \param[in] p_dev     : 指向WDT设备
 * \param[in] p_devinfo : 指向连接WDT中断服务函数
 * \param[in] p_arg     : 回调函数参数
 *
 * \return 成功则返回AM_OK，其他则为失败
 */
int am_hc32f460_wdt_isr_disconnect (am_hc32f460_wdt_dev_t     *p_dev,
                                    am_hc32f460_pfn_wdt_isr_t  pfn_isr,
                                    void                      *p_arg);

/**
 * \brief WDT初始化
 *
 * \param[in] p_dev     : 指向WDT设备
 * \param[in] p_devinfo : 指向WDT设备信息
 *
 * \return WDT标准服务句柄，失败则返回NULL
 */
am_wdt_handle_t am_hc32f460_wdt_init (am_hc32f460_wdt_dev_t            *p_dev,
                                      const am_hc32f460_wdt_devinfo_t  *p_devinfo);

/**
 * \brief 解除WDT设备初始化
 *
 * \param[in] handle : 使用WDT初始化函数am_hc32f460_wdt_init()获取到的handle
 *
 * \return 无
 */
void am_hc32f460_wdt_deinit (am_wdt_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __AM_HC32F460_WDT_H */

#endif

/* end of file */

