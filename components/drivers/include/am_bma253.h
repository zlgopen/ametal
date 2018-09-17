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
 * \brief    BMA253三轴加速度传感器应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-07  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_BMA253_H
#define __AM_BMA253_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
    
/**
 * \addtogroup am_if_bma253
 * \copydoc am_bma253.h
 * @{
 */

/**
 * \brief BMA253传感器配置
 */
typedef enum am_bma253_config {
    /**
     * \brief 可配置的加速度范围     
     */
    PMU_RANGE_2G   =    0X03,             /**< \brief 加速度范围±2g          */
    PMU_RANGE_4G   =    0X05,             /**< \brief 加速度范围±4g          */
    PMU_RANGE_8G   =    0X08,             /**< \brief 加速度范围±8g          */
    PMU_RANGE_16G  =    0X0C,             /**< \brief 加速度范围±16g         */

    /**
     * \brief 可配置的滤波带宽    
     */    
    PMU_BW_8Hz     =    0X08,             /**< \brief 数据滤波带宽7.81Hz      */
    PMU_BW_16Hz    =    0X09,             /**< \brief 数据滤波带宽15.36Hz     */
    PMU_BW_32Hz    =    0X0A,             /**< \brief 数据滤波带宽31.25Hz     */
    PMU_BW_63Hz    =    0X0B,             /**< \brief 数据滤波带宽62.5Hz      */
    PMU_BW_125Hz   =    0X0C,             /**< \brief 数据滤波带宽125Hz       */
    PMU_BW_250Hz   =    0X0D,             /**< \brief 数据滤波带宽250Hz       */
    PMU_BW_500Hz   =    0X0E,             /**< \brief 数据滤波带宽500Hz       */
    PMU_BW_1000Hz  =    0X0F,             /**< \brief 数据滤波带宽1000Hz      */
    
    /**
     * \brief 可配置的电源模式   
     */ 
    NORMAL_MODE       = 0X00,             /**< \brief 正常模式                */
    DEEP_SUSPEND_MODE = 0X20,             /**< \brief 深度挂起模式            */
    LOW_POWER_MODE    = 0X40,             /**< \brief 低功耗模式              */
    SUSPEND_MODE      = 0X80,             /**< \brief 挂起模式                */
    
    /**
     * \brief 可配置的睡眠周期   
     */
    SLEEP_DUR_HALF_MS = AM_SBF(5,1),      /**< \brief 配置的睡眠周期为0.5ms   */
    SLEEP_DUR_1MS     = AM_SBF(6,1),      /**< \brief 配置的睡眠周期为1ms     */
    SLEEP_DUR_2MS     = AM_SBF(7,1),      /**< \brief 配置的睡眠周期为2ms     */
    SLEEP_DUR_4MS     = AM_SBF(8,1),      /**< \brief 配置的睡眠周期为4ms     */
    SLEEP_DUR_6MS     = AM_SBF(9,1),      /**< \brief 配置的睡眠周期为6ms     */
    SLEEP_DUR_10MS    = AM_SBF(10,1),     /**< \brief 配置的睡眠周期为10ms    */
    SLEEP_DUR_25MS    = AM_SBF(11,1),     /**< \brief 配置的睡眠周期为25ms    */
    SLEEP_DUR_50MS    = AM_SBF(12,1),     /**< \brief 配置的睡眠周期为50ms    */
    SLEEP_DUR_100MS   = AM_SBF(13,1),     /**< \brief 配置的睡眠周期为100ms   */
    SLEEP_DUR_500MS   = AM_SBF(14,1),     /**< \brief 配置的睡眠周期为500ms   */
    SLEEP_DUR_1000MS  = AM_SBF(15,1)      /**< \brief 配置的睡眠周期为1000ms  */
    
} am_bma253_config_t;
       
/**
 * \brief BMA253 实例信息结构体定义
 */
typedef struct am_bma253_devinfo {
    uint8_t pmu_range;                    /**< \brief 加速度范围              */
    uint8_t pmu_bw;                       /**< \brief 数据滤波带宽            */
    uint8_t power_mode;                   /**< \brief 电源模式                */
    uint8_t sleep_period;                 /**< \brief 低功耗模式睡眠周期      */
} am_bma253_devinfo_t;
       
/**
 * \brief BMA253 设备结构体
 */
typedef struct am_bma253_dev {
    
    /**< \brief BMA253 I2C设备 */
    am_i2c_device_t i2c_dev; 
    
    /** \brief 用于保存设备信息指针 */
    const am_bma253_devinfo_t  *p_devinfo;    
    
} am_bma253_dev_t;

/** \brief BMA253操作句柄定义 */
typedef am_bma253_dev_t *am_bma253_handle_t;


/**
 * \brief BMA253传感器读取三轴加速度
 *
 * \param[in]  handle  : BMA253服务操作句柄
 * \param[out] p_accel : 指向加速度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_bma253_read_accel (am_bma253_handle_t handle, int16_t* p_accel);

/**
 * \brief BMA253传感器读取温度
 *
 * \param[in]  handle : BMA253服务操作句柄
 * \param[out] p_temp : 指向温度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_bma253_read_temp (am_bma253_handle_t handle,int8_t* p_temp);

/**
 * \brief BMA253传感器软件复位
 * \param[in]  handle : BMA253服务操作句柄
 * \retval AM_OK : 软件复位成功
 * \retval OTHER : 软件复位失败
 */
int am_bma253_soft_reset (am_bma253_handle_t handle);

/**
 * \brief BMA253传感器初始化
 *
 * \param[in] p_dev      : 指向BMA253设备结构体的指针
 * \param[in] p_devinfo  : 指向BMA253设备信息结构体的指针 
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return BMA253服务操作句柄,如果为 NULL，表明初始化失败
 */
am_bma253_handle_t am_bma253_init (am_bma253_dev_t           *p_dev, 
                                   const am_bma253_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t           i2c_handle);

/**
 * \brief BMA253 设备解初始化
 * \param[in]  handle : BMA253服务操作句柄
 * \return 无
 */
void am_bma253_deinit (am_bma253_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BMA253_H */

/* end of file */
