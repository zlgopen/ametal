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
 * \brief LIS3MDL三轴磁传感器驱动
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */
 
#ifndef __AM_LIS3MDL_H
#define __AM_LIS3MDL_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
 
/**
 * \brief LIS3MDL传感器配置
 */
typedef enum am_lis3mdl_config {
    
    /**
     * \brief 温度传感器配置
     */
    TEMP_ENABLE       =   AM_SBF(1,7),  /**< \brief 使能温度测量              */
    TEMP_DISABLE      =   AM_SBF(0,7),  /**< \brief 禁能温度测量              */
    
    /**
     * \brief X、Y轴操作模式选择
     */    
    LOW_POWER_MODE_XY   =   AM_SBF(0,5),  /**< \brief 低功耗模式              */
    MEDIUM_MODE_XY      =   AM_SBF(1,5),  /**< \brief 中等性能模式            */    
    HIGH_MODE_XY        =   AM_SBF(2,5),  /**< \brief 高性能模式              */    
    ULTRA_HIGH_MODE_XY  =   AM_SBF(3,5),  /**< \brief 超高性能模式            */    

    /**
     * \brief 数据输出速率
     */ 
    ODR_1Hz           =   AM_SBF(0,2),  /**< \brief 数据输出频率0.625Hz       */
    ODR_2Hz           =   AM_SBF(1,2),  /**< \brief 数据输出频率1.25Hz        */
    ODR_3Hz           =   AM_SBF(2,2),  /**< \brief 数据输出频率2.5Hz         */
    ODR_5Hz           =   AM_SBF(3,2),  /**< \brief 数据输出频率5Hz           */
    ODR_10Hz          =   AM_SBF(4,2),  /**< \brief 数据输出频率10Hz          */
    ODR_20Hz          =   AM_SBF(5,2),  /**< \brief 数据输出频率20Hz          */
    ODR_40Hz          =   AM_SBF(6,2),  /**< \brief 数据输出频率40Hz          */
    ODR_80Hz          =   AM_SBF(7,2),  /**< \brief 数据输出频率80Hz          */    
   
    /**
     * \brief 数据快速输出使能控制
     */    
    FAST_ODR_ENABLE   =   AM_SBF(1,1),  /**< \brief 使能数据快速输出          */
    FAST_ODR_DISABLE  =   AM_SBF(0,1),  /**< \brief 不使能数据快速输出        */
    
    /**
     * \brief 自检使能控制
     */    
    SELF_TEST_ENABLE   =   AM_SBF(1,0),  /**< \brief 使能自检                 */
    SELF_TEST_DISABLE  =   AM_SBF(0,0),  /**< \brief 不使能自检               */ 

    /**
     * \brief 可配置的磁场测量范围
     */ 
    FULL_4GAUSS        =   AM_SBF(0,5),  /**< \brief 磁场测量范围±4 gauss    */
    FULL_8GAUSS        =   AM_SBF(1,5),  /**< \brief 磁场测量范围±8 gauss    */     
    FULL_12GAUSS       =   AM_SBF(2,5),  /**< \brief 磁场测量范围±12 gauss   */
    FULL_16GAUSS       =   AM_SBF(3,5),  /**< \brief 磁场测量范围±16 gauss   */    
  
    /**
     * \brief 可配置的系统模式
     */ 
    CONTINUE_MODE      =   AM_SBF(0,0),  /**< \brief 连续转换模式             */  
    SINGLE_MODE        =   AM_SBF(1,0),  /**< \brief 单次转换模式             */
    POWER_DOWN         =   AM_SBF(2,0),  /**< \brief 掉电模式                 */
 
    /**
     * \brief Z轴操作模式选择
     */    
    LOW_POWER_MODE_Z   =   AM_SBF(0,2),  /**< \brief 低功耗模式              */
    MEDIUM_MODE_Z      =   AM_SBF(1,2),  /**< \brief 中等性能模式            */    
    HIGH_MODE_Z        =   AM_SBF(2,2),  /**< \brief 高性能模式              */    
    ULTRA_HIGH_MODE_Z  =   AM_SBF(3,2),  /**< \brief 超高性能模式            */ 

} am_lis3mdl_config_t;
    
/**
 * \brief LIS3MDL 实例信息结构体定义
 */
typedef struct am_lis3mdl_devinfo {

    uint8_t temp_en;                    /**< \brief 温度测量使能位            */
    uint8_t operate_mode_xy;            /**< \brief X、Y轴操作模式选择        */
    uint8_t output_data_rate;           /**< \brief 数据输出速率              */
    uint8_t fast_odr_en;                /**< \brief 数据快速输出使能位        */
    uint8_t self_test_en;               /**< \brief 自检使能位                */
    uint8_t full_scale;                 /**< \brief 磁场测量范围              */
    uint8_t system_mode;                /**< \brief 系统模式                  */
    uint8_t operate_mode_z;             /**< \brief Z轴操作模式选择           */    
    
} am_lis3mdl_devinfo_t;     
    
/**
 * \brief LIS3MDL 设备结构体
 */
typedef struct am_lis3mdl_dev {
    
    /**< \brief LIS3MDL I2C设备 */
    am_i2c_device_t i2c_dev; 

    /** \brief 用于保存设备信息指针 */
    const am_lis3mdl_devinfo_t  *p_devinfo;    
    
} am_lis3mdl_dev_t;

/** \brief LIS3MDL操作句柄定义 */
typedef am_lis3mdl_dev_t *am_lis3mdl_handle_t;  

/**
 * \brief LIS3MDL传感器初始化
 *
 * \param[in] p_dev      : 指向LIS3MDL设备结构体的指针
 * \param[in] p_devinfo  : 指向LIS3MDL设备信息结构体的指针 
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return LIS3MDL服务操作句柄,如果为 NULL，表明初始化失败
 */
am_lis3mdl_handle_t am_lis3mdl_init (am_lis3mdl_dev_t *p_dev, 
                                     const am_lis3mdl_devinfo_t *p_devinfo, 
                                     am_i2c_handle_t i2c_handle);
 
/**
 * \brief LIS3MDL传感器读取磁场强度数值
 *
 * \param[in]  handle : LIS3MDL服务操作句柄
 * \param[out] p_data : 指向数据的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_lis3mdl_read (am_lis3mdl_handle_t handle, uint16_t *p_data);                                     

/**
 * \brief LIS3MDL传感器读取温度
 *
 * \param[in]  handle : LIS3MDL服务操作句柄
 * \param[out] p_temp : 指向数据的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */                                     
int am_lis3mdl_read_temp (am_lis3mdl_handle_t handle, int8_t *p_temp);                                     
                                     
#ifdef __cplusplus
}
#endif

#endif /* __AM_LIS3MDL_H */

/* end of file */
