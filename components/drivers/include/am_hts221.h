/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief HTS221驱动头文件
 * \sa hts221.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#ifndef __HTS221_H
#define __HTS221_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"

/**
 * \brief HTS221传感器配置
 */
typedef enum am_hts221_config {

    /**
     * \brief 可配置的温度采样样本数     
     */    
    TEMP_SAMPLE_4     =   AM_SBF(0,3),    /**< \brief 温度样本数为4           */
    TEMP_SAMPLE_8     =   AM_SBF(1,3),    /**< \brief 温度样本数为8           */
    TEMP_SAMPLE_16    =   AM_SBF(2,3),    /**< \brief 温度样本数为16          */
    TEMP_SAMPLE_32    =   AM_SBF(3,3),    /**< \brief 温度样本数为32          */
    TEMP_SAMPLE_64    =   AM_SBF(4,3),    /**< \brief 温度样本数为64          */
    TEMP_SAMPLE_128   =   AM_SBF(5,3),    /**< \brief 温度样本数为128         */
    TEMP_SAMPLE_256   =   AM_SBF(6,3),    /**< \brief 温度样本数为256         */
    TEMP_SAMPLE_512   =   AM_SBF(7,3),    /**< \brief 温度样本数为512         */     
    
    /**
     * \brief 可配置的湿度采样样本数     
     */    
    HUM_SAMPLE_4      =   AM_SBF(0,0),    /**< \brief 湿度样本数为4           */
    HUM_SAMPLE_8      =   AM_SBF(1,0),    /**< \brief 湿度样本数为8           */
    HUM_SAMPLE_16     =   AM_SBF(2,0),    /**< \brief 湿度样本数为16          */
    HUM_SAMPLE_32     =   AM_SBF(3,0),    /**< \brief 湿度样本数为32          */
    HUM_SAMPLE_64     =   AM_SBF(4,0),    /**< \brief 湿度样本数为64          */
    HUM_SAMPLE_128    =   AM_SBF(5,0),    /**< \brief 湿度样本数为128         */
    HUM_SAMPLE_256    =   AM_SBF(6,0),    /**< \brief 湿度样本数为256         */
    HUM_SAMPLE_512    =   AM_SBF(7,0),    /**< \brief 湿度样本数为512         */    

    /**
     * \brief 可配置的电源模式     
     */
    POWER_ON          =   AM_SBF(1,7),    /**< \brief 打开电源                */
    POWER_DOWN        =   AM_SBF(0,7),    /**< \brief 关闭电源                */
    
    /**
     * \brief 可配置的数据更新方式     
     */
    UPDATE_CONTINUE   =   AM_SBF(0,2),    /**< \brief 数据更新一次            */
    UPDATE_ONE        =   AM_SBF(1,2),    /**< \brief 数据连续更新            */
    
    /**
     * \brief 可配置的数据更新速度     
     */ 
    DATA_UPDATE_1Hz   = AM_SBF(1,0),      /**< \brief 数据更新速度为1Hz       */
    DATA_UPDATE_7Hz   = AM_SBF(2,0),      /**< \brief 数据更新速度为7Hz       */
    DATA_UPDATE_12Hz  = AM_SBF(3,0),      /**< \brief 数据更新速度为12.5Hz    */    

} am_hts221_config_t;
  
/**
 * \brief HTS221温湿度校准参数
 */ 
typedef struct {
    int16_t  x0;
    uint8_t  y0;
    int16_t  x1;
    uint8_t  y1;  
} am_hts221_liner_t;

/**
 * \brief HTS221 实例信息结构体定义
 */
typedef struct am_hts221_devinfo {
    uint8_t temp_sample;             /**< \brief 配置温度样本数               */
    uint8_t hum_sample;              /**< \brief 配置湿度样本数               */
    uint8_t power_mode;              /**< \brief 配置电源模式                 */
    uint8_t data_update_config;      /**< \brief 配置数据更新方式             */
    uint8_t data_update_rate;        /**< \brief 配置数据更新速度             */
} am_hts221_devinfo_t;    
    
/**
 * \brief HTS221 设备结构体
 */
typedef struct am_hts221_dev {
    
    /**< \brief hts221 I2C设备 */
    am_i2c_device_t i2c_dev; 
    
    /**< \brief 用于保存湿度校准参数 */
    am_hts221_liner_t calib_param_hum;
    
    /**< \brief 用于保存温度校准参数 */
    am_hts221_liner_t calib_param_temp;
    
    /** \brief 用于保存设备信息指针 */
    const am_hts221_devinfo_t  *p_devinfo;    
    
} am_hts221_dev_t;

/** \brief HTS221操作句柄定义 */
typedef am_hts221_dev_t *am_hts221_handle_t;

/**
 * \brief HTS221传感器初始化
 *
 * \param[in] p_dev      : 指向HTS221设备结构体的指针
 * \param[in] p_devinfo  : 指向HTS221设备信息结构体的指针 
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return HTS221服务操作句柄,如果为 NULL，表明初始化失败
 */
am_hts221_handle_t am_hts221_init (am_hts221_dev_t *p_dev, 
                                   const am_hts221_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle);

/**
 * \brief HTS221传感器读取温度
 *
 * \param[in]  handle : HTS221服务操作句柄
 * \param[out] temp   : 指向温度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */                                      
int am_hts221_read_temp (am_hts221_handle_t handle, int16_t* temp);
                                   
/**
 * \brief HTS221传感器读取湿度
 *
 * \param[in]  handle : HTS221服务操作句柄
 * \param[out] hum   : 指向湿度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */                                      
int am_hts221_read_hum (am_hts221_handle_t handle, uint8_t * hum);
                                    
#ifdef __cplusplus
}
#endif

#endif

/* end of file */ 



