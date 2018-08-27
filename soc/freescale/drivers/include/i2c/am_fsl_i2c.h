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
 * \brief I2C驱动，服务I2C标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-09-19  ari , first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_I2C_H
#define __AM_FSL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_int.h"
#include "am_i2c.h"
#include "hw/amhw_fsl_i2c.h"
#include "am_list.h"
#include "am_softimer.h"

/**
 * \addtogroup am_fsl_if_i2c
 * \copydoc am_fsl_i2c.h
 * @{
 */

/**
 * \brief I2C 设备信息参数结构体
 */
typedef struct am_fsl_i2c_devinfo {
    struct amhw_fsl_i2c  *p_hw_i2c;    /**< \brief 指向I2C寄存器块地址指针     */
    uint32_t               bus_speed;  /**< \brief I2C 初始化总线时钟     */
    uint16_t               inum;       /**< \brief I2C 控制器的中断号  */
    uint32_t               clk_id;     /**< \brief I2C 时钟号 */
    uint16_t               timeout_ms; /**< \brief I2C 超时时间设置  */
    void  (*pfn_plfm_init)(void);      /**< \brief 平台初始化函数，配置引脚等工作 */

    void  (*pfn_plfm_deinit)(void);    /**< \brief 平台解初始化函数 */

} am_fsl_i2c_devinfo_t;

/**
 * \brief I2C 设备结构体
 */
typedef struct am_fsl_i2c_dev {
    /** \brief 标准I2C服务           */
    am_i2c_serv_t                 i2c_serv;

    /** \brief I2C 控制器消息队列 */
    struct am_list_head           msg_list;

    /** \brief 指向I2C传输结构体的指针，同一时间只能处理一个传输           */
    am_i2c_transfer_t            *p_cur_trans;

    /** \brief 当前正在处理的消息   */
    am_i2c_message_t             *p_cur_msg;

    /** \brief 用于数据的传输计数   */
    uint32_t                      data_ptr;

    /** \brief I2C模块忙标识          */
    am_bool_t                        busy;

    /** \brief I2C模块当前状态      */
    uint8_t                       state;

    /** \brief I2C模块虚拟读取标志      */
    uint8_t                       dummy_rx_flg;

    /** \brief STOP信号产生标志      */
    am_bool_t                     stop_signal;

    /** \brief 超时软件定时器 */
    am_softimer_t                 softimer;

    /** \brief 指向I2C设备信息指针   */
    const am_fsl_i2c_devinfo_t  *p_devinfo;
} am_fsl_i2c_dev_t;

/**
 * \brief I2C 初始化函数
 *
 * \param[in] p_dev     : 指向I2C设备结构体指针
 * \param[in] p_devinfo : 指向I2C设备信息结构体指针
 *
 * \return  I2C 标准服务操作句柄
 */
am_i2c_handle_t am_fsl_i2c_init(am_fsl_i2c_dev_t           *p_dev,
                                 const am_fsl_i2c_devinfo_t *p_devinfo);

/**
 * \brief I2C 解初始化函数
 * \param[in] handle : 指向I2C标准服务操作句柄
 *
 * \return 无
 */
void am_fsl_i2c_deinit(am_i2c_handle_t handle);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* __AM_FSL_I2C_H */
/* end of file */
