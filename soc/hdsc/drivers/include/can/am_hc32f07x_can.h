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
 * \brief LVD驱动，服务LVD标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-04-26
 * \endinternal
 */
 
#ifndef __AM_HC32F07X_CAN_H
#define __AM_HC32F07X_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_can.h"
#include "hw/amhw_hc32f07x_can.h"

/**
 * \addtogroup am_hc32_if_lvd
 * \copydoc am_hc32_lvd.h
 * @{
 */

/**
 * \brief can 初始化配置信息
 */
typedef struct am_hc32f07x_can_init_info {

        /** \brief 时间触发通信方式
     *   0 禁能
     *   1 使能
     */
    am_bool_t   can_ttcm;

    /** \brief 接收缓冲器数据存储模式
     *   0 正常模式
     *   1 存储所有 
     */
    amhw_hc32f07x_can_rxbuf_all_t  rx_all;

    /** \brief 接收缓冲器上溢模式
     *   0 最早接收的数据被覆盖
     *   1 最新接收到的数据不被存储
     */
    amhw_hc32f07x_can_rxbuf_mode_t  over_mode;

    /** \brief 缓冲器发送模式选择
     *   0 FIFO模式
     *   1 优先级模式
     */
    amhw_hc32f07x_can_stb_mode_t    can_stb_mode;

    /** \brief 接收缓冲器将满设定值 */
    uint8_t warning_limitval;

    /** \brief 错误警告限定值 */
    uint8_t error_warning_limitval;    

}am_hc32f07x_can_init_info_t;

/**
 * \brief 中断回调信息
 */
typedef struct am_hc32f07x_can_intcb_info{

    /** \brief 触发回调函数 */
    am_pfnvoid_t  pfn_callback;

    /** \brief 回调函数的参数 */
    void         *p_arg;
} am_hc32f07x_can_intcb_info_t;

/**
 * \brief CAN设备信息结构体
 */
typedef struct am_hc32f07x_can_info {

    /** \brief 寄存器基地址 */
    uint32_t                       regbase;

    /** \brief 中断号 */	
    uint32_t                       int_num; 

    amhw_hc32f07x_can_type_t        type;

    /** \brief 回调信息 */
    am_hc32f07x_can_intcb_info_t  *p_intcb_info;

    /** \brief 回调信息内存大小 */
    uint8_t                        p_intcb_num;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

    am_hc32f07x_can_init_info_t    *init_info;

}am_hc32f07x_can_devinfo_t;

/**
 * \brief 设备
 */
typedef struct am_hc32f07x_can_dev {
    am_can_serv_t                   handle;

    const am_hc32f07x_can_devinfo_t  *p_devinfo;

} am_hc32f07x_can_dev_t;

/**
 * \brief CAN初始化
 *
 * \param[in] p_dev     : 设备实例
 * \param[in] p_devinfo : 设备信息
 *
 * \return handle
 */
am_can_handle_t am_hc32f07x_can_init (am_hc32f07x_can_dev_t           *p_dev,
                                      const am_hc32f07x_can_devinfo_t *p_devinfo);

/**
 * \brief 解初始化
 *
 * \param[in] p_dev     : 设备实例
 *
 * \return 无
 */
void am_hc32f07x_can_deinit (am_can_handle_t handle);

/**
 * @} am_hc32f07x_if_can
 */


#ifdef __cplusplus
}
#endif

#endif /* __AM_S32K14X_CAN_H */

/* end of file */
