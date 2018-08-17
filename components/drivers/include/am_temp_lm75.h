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
 * \brief LM75温度传感器驱动
 *
 * \internal
 * \par modification history:
 * - 1.00 17-06-19  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_TEMP_LM75_H
#define __AM_TEMP_LM75_H

#include "am_common.h"
#include "am_temp.h"
#include "am_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_temp_lm75
 * \copydoc am_temp_lm75.h
 * @{
 */

/**
 * \brief LM75信息
 */
typedef struct am_temp_lm75_info {
    uint8_t            i2c_addr;        /** <\brief 7-bit 从机地址  */
} am_temp_lm75_info_t;

/**
 * \brief LED设备（GPIO驱动）
 */
typedef struct am_temp_lm75 {
    am_temp_dev_t               isa;
    am_i2c_device_t             dev;
    const am_temp_lm75_info_t  *p_info;
} am_temp_lm75_t;

/**
 * \brief LM75温度传感器初始化
 *
 * \param[in] p_lm75  : LM75温度传感器设备
 * \param[in] p_info  : LM75温度传感器信息
 * \param[in] handle  : I2C标准服务句柄
 *
 * \retval AM_OK      : 初始化成功
 * \retval -AM_EINVAL : 初始化失败
 */
am_temp_handle_t am_temp_lm75_init (am_temp_lm75_t            *p_lm75,
                                    const am_temp_lm75_info_t *p_info,
                                    am_i2c_handle_t            handle);

/**
 * \brief LM75温度传感器解初始化
 *
 * \param[in] p_lm75  : LM75温度传感器设备
 *
 * \retval AM_OK      : 解初始化成功
 * \retval -AM_EINVAL : 解初始化失败
 */
int am_temp_lm75_deinit (am_temp_lm75_t *p_lm75);


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_TEMP_LM75_H */

/* end of file */
