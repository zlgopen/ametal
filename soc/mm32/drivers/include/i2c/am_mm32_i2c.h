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
 * - 1.01 17-12-28  pea, add a timeout mechanism, enable TX_ABRT and STOP_DET
 *                  interrupt
 * - 1.00 17-04-24  sdy, first implementation
 * \endinternal
 */

#ifndef __AM_MM32_I2C_H
#define __AM_MM32_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_int.h"
#include "am_i2c.h"
#include "am_list.h"
#include "am_softimer.h"

#include "am_vdebug.h"

/**
 * \addtogroup am_mm32_if_i2c
 * \copydoc am_mm32_i2c.h
 * @{
 */

/**
 * \brief I2C设备信息参数结构体
 */
typedef struct am_mm32_i2c_devinfo {

    /** \brief I2C寄存器块基址 */
    uint32_t  i2c_regbase;

    /** \brief 时钟ID */
    int       clk_num;

    /** \brief I2C控制器的中断号 */
    uint16_t  inum;

    /** \brief I2C速率 */
    uint32_t  speed;

    /**
     * \brief 超时时间值
     *
     * I2C总线在数据传输期间，若此段时间内时钟线无任何响应,则视为超时。
     * 结束当前传输并复位内部逻辑
     */
    uint32_t  timeout_ms;

    /** \brief I2C总线恢复函数 */
    void    (*pfn_bus_clean)(void);

    /** \brief 平台初始化函数 */
    void    (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void    (*pfn_plfm_deinit)(void);

} am_mm32_i2c_devinfo_t;

/**
 * \brief I2C设备结构体
 */
typedef struct am_mm32_i2c_dev {

    /** \brief 标准I2C服务 */
    am_i2c_serv_t                           i2c_serv;

    /** \brief I2C控制器消息队列 */
    struct am_list_head                     msg_list;

    /** \brief 指向I2C传输结构体的指针,同一时间只能处理一个传输 */
    am_i2c_transfer_t                      *p_cur_trans;

    /** \brief 当前正在处理的消息 */
    am_i2c_message_t                       *p_cur_msg;

    /** \brief 是否完成本次消息 */
    volatile am_bool_t                      is_complete;

    /** \brief 用于数据接收/发送计数 */
    volatile uint32_t                       data_ptr;

    /** \brief 当前传输状态（0：此次传输了从机地址，1：此次没有传输从机地址） */
    volatile uint8_t                        trans_state;

    /** \brief 忙标识 */
    volatile am_bool_t                      busy;

    /** \brief 状态 */
    volatile uint8_t                        state;

    /** \brief 从机地址 */
    volatile uint16_t                       slave_addr;

    /** \brief 超时软件定时器 */
    am_softimer_t                           softimer;

    /** \brief 超时软件定时器状态 */
    volatile am_bool_t                      softimer_state;

    /** \brief 是否中断状态机 */
    volatile am_bool_t                      is_abort;

    /** \brief 指向I2C设备信息的指针 */
    const am_mm32_i2c_devinfo_t             *p_devinfo;

} am_mm32_i2c_dev_t;

/**
 * \brief I2C初始化
 *
 * \param[in] p_dev     : 指向I2C设备结构体的指针
 * \param[in] p_devinfo : 指向I2C设备信息结构体的指针
 *
 * \return I2C标准服务操作句柄
 */
am_i2c_handle_t am_mm32_i2c_init (am_mm32_i2c_dev_t           *p_dev,
                                 const am_mm32_i2c_devinfo_t *p_devinfo);

/**
 * \brief 解除I2C初始化
 *
 * \param[in] handle : 与从设备关联的I2C标准服务操作句柄
 *
 * \return 无
 */
void am_mm32_i2c_deinit (am_i2c_handle_t handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MM32_I2C_H */

/* end of file */
