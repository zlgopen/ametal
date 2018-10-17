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
 * \brief    BMG160三轴陀螺仪传感器应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-11  wk, first implementation.
 * \endinternal
 */

#ifndef __AM_BMG160_H
#define __AM_BMG160_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
  
/**
 * \addtogroup am_if_bmg160
 * \copydoc am_bmg160.h
 * @{
 */

/**
 * \brief BMG160传感器配置
 */
typedef enum am_bmg160_config {
    /**
     * \brief 可配置的角速率范围     
     */
    RANGE_2000   =    0X00,             /**< \brief 角速率范围±2000°/s      */
    RANGE_1000   =    0X01,             /**< \brief 角速率范围±1000°/s      */
    RANGE_500    =    0X02,             /**< \brief 角速率范围±500°/s       */
    RANGE_250    =    0X03,             /**< \brief 角速率范围±250°/s       */
    RANGE_125    =    0X04,             /**< \brief 角速率范围±125°/s       */

    /**
     * \brief 可配置的滤波带宽    
     */    
    BW_230Hz     =    0X01,         /**< \brief 数据滤波带宽230Hz */
    BW_116Hz     =    0X02,         /**< \brief 数据滤波带宽116Hz */
    BW_47Hz      =    0X03,         /**< \brief 数据滤波带宽47Hz */
    BW_23Hz      =    0X04,         /**< \brief 数据滤波带宽23Hz */
    BW_12Hz      =    0X05,         /**< \brief 数据滤波带宽12Hz */
    BW_64Hz      =    0X06,         /**< \brief 数据滤波带宽64Hz */
    BW_32Hz      =    0X07,         /**< \brief 数据滤波带宽32Hz */
    
    /**
     * \brief 可配置的电源模式   
     */ 
    NORMAL_MODE       = 0X00,       /**< \brief 正常模式 */
    DEEP_SUSPEND_MODE = 0X20,       /**< \brief 深度挂起模式 */
    SUSPEND_MODE      = 0X80,       /**< \brief 挂起模式 */
    
    /**
     * \brief 可配置的睡眠周期   
     */
    SLEEP_DUR_2MS   = AM_SBF(0,1),  /**< \brief 配置的睡眠周期为2ms */
    SLEEP_DUR_4MS   = AM_SBF(1,1),  /**< \brief 配置的睡眠周期为4ms */
    SLEEP_DUR_5MS   = AM_SBF(2,1),  /**< \brief 配置的睡眠周期为5ms */
    SLEEP_DUR_8MS   = AM_SBF(3,1),  /**< \brief 配置的睡眠周期为8ms */
    SLEEP_DUR_10MS  = AM_SBF(4,1),  /**< \brief 配置的睡眠周期为10ms */
    SLEEP_DUR_15MS  = AM_SBF(5,1),  /**< \brief 配置的睡眠周期为15ms */
    SLEEP_DUR_18MS  = AM_SBF(6,1),  /**< \brief 配置的睡眠周期为18ms */
    SLEEP_DUR_20MS  = AM_SBF(7,1),  /**< \brief 配置的睡眠周期为20ms */ 
} am_bmg160_config_t;
	
/**
 * \brief BMG160实例信息结构体定义
 */
typedef struct am_bmg160_devinfo {    
    uint8_t range;                  /**< \brief 加速度范围 */
    uint8_t bw;                     /**< \brief 数据滤波带宽 */
    uint8_t power_mode;             /**< \brief 电源模式 */
    uint8_t sleep_period;           /**< \brief 低功耗模式睡眠周期 */
} am_bmg160_devinfo_t;
	
/**
 * \brief BMG160 设备结构体
 */
typedef struct am_bmg160_dev {
    am_i2c_device_t i2c_dev;        /**< \brief BMG160 I2C设备 */
} am_bmg160_dev_t;

/** \brief BMG160操作句柄定义 */
typedef am_bmg160_dev_t *am_bmg160_handle_t;

/**
 * \brief BMG160传感器读取三轴陀螺仪
 *
 * \param[in]  handle  : BMG160服务操作句柄
 * \param[out] p_rate  : 指向陀螺仪的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_bmg160_read_rate (am_bmg160_handle_t handle, int16_t* p_rate);

/**
 * \brief BMG160传感器读取温度
 *
 * \param[in]  handle : BMG160服务操作句柄
 * \param[out] p_temp : 指向温度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_bmg160_read_temp (am_bmg160_handle_t handle,int8_t* p_temp);

/**
 * \brief BMG160传感器初始化
 *
 * \param[in] p_dev      : 指向BMG160设备结构体的指针
 * \param[in] p_devinfo  : 指向BMG160设备信息结构体的指针 
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return BMG160服务操作句柄,如果为 NULL，表明初始化失败
 */
am_bmg160_handle_t am_bmg160_init (am_bmg160_dev_t           *p_dev,
                                   const am_bmg160_devinfo_t *p_devinfo,  
                                   am_i2c_handle_t           i2c_handle);

/**
 * \brief BMG160 设备解初始化
 * \param[in]  handle : BMG160服务操作句柄
 * \return 无
 */
void am_bmg160_deinit (am_bmg160_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BMG160_H */

/* end of file */
