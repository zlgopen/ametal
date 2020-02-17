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
 * \brief I2C从机驱动，服务I2C从机标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-09-06  vir, first implementation
 * \endinternal
 */

#ifndef __AM_MM32_I2C_SLV_H
#define __AM_MM32_I2C_SLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_int.h"
#include "am_i2c_slv.h"
#include "am_softimer.h"

/**
 * \addtogroup am_mm32_if_i2c_slv
 * \copydoc am_mm32_i2c_slv.h
 * @{
 */

/**
 * \brief I2C设备信息参数结构体
 */
typedef struct am_mm32_i2c_slv_devinfo {

    /** \brief I2C 寄存器块基址 */
    uint32_t  i2c_regbase;

    /** \brief I2C 控制器的中断号 */
    uint16_t  inum;

    /** \brief 平台初始化函数 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_mm32_i2c_slv_devinfo_t;

/**
 * \brief I2C设备结构体
 */
typedef struct am_mm32_i2c_slv_dev {

    /** \brief 标准I2C从设备服务 */
    am_i2c_slv_serv_t                  i2c_slv_serv;

   /** \brief 从机设备,只能存一个从机地址,只能生成一个设备 */
    am_i2c_slv_device_t               *p_i2c_slv_dev[1];

    /** \brief ZLG可生成最多的从机个数即可放多少个从机地址,与 p_i2c_slv_dev数组个数保持一致, */
    uint8_t                            mm32_i2c_slv_dev_num;

    /** \brief 广播呼叫 标志 */
    am_bool_t                          is_gen_call;

    /** 软件定时器,用于超时处理 */
    am_softimer_t                      softimer;

    /** \brief 指向I2C设备信息的指针 */
    const am_mm32_i2c_slv_devinfo_t    *p_devinfo;

} am_mm32_i2c_slv_dev_t;

/**
 * \brief I2C初始化
 *
 * \param[in] p_dev     : 指向从I2C设备结构体的指针
 * \param[in] p_devinfo : 指向从I2C设备信息结构体的指针
 *
 * \return 从I2C标准服务操作句柄
 */
am_i2c_slv_handle_t am_mm32_i2c_slv_init (am_mm32_i2c_slv_dev_t           *p_dev,
                                         const am_mm32_i2c_slv_devinfo_t *p_devinfo);

/**
 * \brief 解除I2C初始化
 * \param[in] handle : 与从设备关联的从I2C标准服务操作句柄
 * \return 无
 */
void am_mm32_i2c_slv_deinit (am_i2c_slv_handle_t handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MM32_I2C_SLV_H */

/* end of file */
