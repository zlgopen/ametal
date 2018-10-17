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
 * \brief LPS22HB驱动头文件
 * \sa am_lsp22hb.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#ifndef __AM_LPS22HB_H
#define __AM_LPS22HB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"

/**
 * \addtogroup am_if_lps22hb
 * \copydoc am_lps22hb.h
 * @{
 */
    
/**
 * \brief LPS22HB传感器配置
 */
typedef enum am_lps22hb_config {

    /**
     * \brief 可配置的数据更新速度     
     */    
    DATA_UPDATE_1Hz   = AM_SBF(1,4),      /**< \brief 数据更新速度为1Hz       */
    DATA_UPDATE_10Hz  = AM_SBF(2,4),      /**< \brief 数据更新速度为10Hz      */
    DATA_UPDATE_25Hz  = AM_SBF(3,4),      /**< \brief 数据更新速度为25Hz      */    
    DATA_UPDATE_50Hz  = AM_SBF(4,4),      /**< \brief 数据更新速度为50Hz      */    
    DATA_UPDATE_75Hz  = AM_SBF(5,4),      /**< \brief 数据更新速度为75Hz      */    
    
    /**
     * \brief 低通滤波配置    
     */   
    ENABLE_LPFP       = AM_SBF(1,3),      /**< \brief 使能低通滤波            */
    DISABLE_LPFP      = AM_SBF(0,3),      /**< \brief 不使能低通滤波          */
    
    /**
     * \brief 低通滤波带宽    
     */   
    LPFP_BW_ODR9       = AM_SBF(0,2),      /**< \brief 滤波带宽为 更新速度/9  */
    LPFP_BW_ODR20      = AM_SBF(1,2),      /**< \brief 滤波带宽为 更新速度/20 */    
 
} am_lps22hb_config_t;



/**
 * \brief LPS22HB 实例信息结构体定义
 */
typedef struct am_lps22hb_devinfo {
    uint8_t output_data_rate;             /**< \brief 配置数据更新速度        */
    uint8_t enable_lpfp;                  /**< \brief 是否使能低通滤波        */
    uint8_t lpfp_bw;                      /**< \brief 配置低通滤波带宽        */
} am_lps22hb_devinfo_t;    
    
/**
 * \brief LPS22HB 设备结构体
 */
typedef struct am_lps22hb_dev {
    
    /**< \brief LPS22HB I2C设备 */
    am_i2c_device_t i2c_dev; 
    
    /** \brief 用于保存设备信息指针 */
    const am_lps22hb_devinfo_t  *p_devinfo;    
    
} am_lps22hb_dev_t;

/** \brief LPS22HB操作句柄定义 */
typedef am_lps22hb_dev_t *am_lps22hb_handle_t;

/**
 * \brief LPS22HB传感器初始化
 *
 * \param[in] p_dev      : 指向LPS22HB设备结构体的指针
 * \param[in] p_devinfo  : 指向LPS22HB设备信息结构体的指针 
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return LPS22HB服务操作句柄,如果为 NULL，表明初始化失败
 */
am_lps22hb_handle_t am_lsp22hb_init (am_lps22hb_dev_t *p_dev, 
                                   const am_lps22hb_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle);

/**
 * \brief LPS22HB传感器读取温度
 *
 * \param[in]  handle : LPS22HB服务操作句柄
 * \param[out] temp   : 指向温度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */                                   
int am_lsp22hb_read_temp (am_lps22hb_handle_t handle, int16_t *temp);
                                   
/**
 * \brief LPS22HB传感器读取压力
 *
 * \param[in]  handle : LPS22HB服务操作句柄
 * \param[out] press  : 指向压力的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */                                   
int am_lsp22hb_read_press (am_lps22hb_handle_t handle, uint32_t *press); 

/**
 * @}
 */
 
#ifdef __cplusplus
}
#endif

#endif

/* end of file */ 

 


