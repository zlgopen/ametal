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
 * \brief BH1730FVC光强及距离传感器驱动
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */
  
#ifndef __AM_BH_1730FVC_H
#define __AM_BH_1730FVC_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
    
/**
 * \brief BH1730FVC传感器配置
 */
typedef enum am_bh1730fvc_config {
 
    /**
     * \brief 可配置的测量模式    
     */     
    MEAS_CONTINUE   =  AM_SBF(0,3),       /**< \brief 连续测量                */
    MEAS_ONE_TIME   =  AM_SBF(1,3),       /**< \brief 测量一次                */
    
    /**
     * \brief 可配置的数据类型   
     */     
    TYPE0_AND_TYPE1 =  AM_SBF(0,2),       /**< \brief 测量类型0和1            */
    TYPE0_ONLY      =  AM_SBF(1,2),       /**< \brief 仅测量类型0             */
 
    /**
     * \brief 可配置的测量状态   
     */    
    ADC_DIS         =  AM_SBF(0,1),       /**< \brief 禁能ADC转换             */    
    ADC_EN          =  AM_SBF(1,1),       /**< \brief 使能ADC转换             */
    
    /**
     * \brief 可配置的电源模式   
     */    
    POWER_ON        =  AM_SBF(1,0),       /**< \brief 打开电源                */    
    POWER_DOWN      =  AM_SBF(0,0),       /**< \brief 关闭电源                */ 

    /**
     * \brief 可配置的ADC分辨率   
     */   
    ADC_MODE_X1     =  AM_SBF(0,0),       /**< \brief X1增益模式              */ 
    ADC_MODE_X2     =  AM_SBF(1,0),       /**< \brief X2增益模式              */
    ADC_MODE_X64    =  AM_SBF(2,0),       /**< \brief X64增益模式             */
    ADC_MODE_X128   =  AM_SBF(3,0),       /**< \brief X128增益模式            */

} am_bh1730fvc_config_t;


/**
 * \brief BH1730FVC 实例信息结构体定义
 */
typedef struct am_bh1730fvc_devinfo {

    uint8_t meas_mode;                    /**< \brief 配置测量模式            */
    uint8_t data_sel;                     /**< \brief 配置数据类型            */
    uint8_t adc_en;                       /**< \brief 配置测量状态            */
    uint8_t power_mode;                   /**< \brief 配置电源模式            */
    /**
     * \brief Integration Time : ITIME_ms = Tint * 964 * (256 - integrate_time)
     * \brief Measurement time : Tmt = ITIME_ms + Tint * 714
     */
    uint8_t integrate_time;               /**< \brief 配置积分时间            */ 
    uint8_t adc_res_setting;              /**< \brief 配置ADC分辨率           */ 
    
} am_bh1730fvc_devinfo_t;     
    
/**
 * \brief BH1730FVC 设备结构体
 */
typedef struct am_bh1730fvc_dev {
    
    /**< \brief BH1730FVC I2C设备 */
    am_i2c_device_t i2c_dev; 

    /** \brief 用于保存设备信息指针 */
    const am_bh1730fvc_devinfo_t  *p_devinfo;    
    
} am_bh1730fvc_dev_t;

/** \brief BH1730FVC操作句柄定义 */
typedef am_bh1730fvc_dev_t *am_bh1730fvc_handle_t;    


/**
 * \brief BH1730FVC传感器初始化
 *
 * \param[in] p_dev      : 指向BH1730FVC设备结构体的指针
 * \param[in] p_devinfo  : 指向BH1730FVC设备信息结构体的指针 
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return BH1730FVC服务操作句柄,如果为 NULL，表明初始化失败
 */
am_bh1730fvc_handle_t am_bh1730fvc_init (am_bh1730fvc_dev_t *p_dev, 
                                        const am_bh1730fvc_devinfo_t *p_devinfo, 
                                        am_i2c_handle_t i2c_handle);

/**
 * \brief BH1730FVC传感器读取温度
 *
 * \param[in]  handle     : BH1730FVC服务操作句柄
 * \param[out] p_als_adc  : 指向光照强度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */                                         
int am_bh1730fvc_read_als_adc (am_bh1730fvc_handle_t handle, 
                               uint16_t *p_als_adc);
                                    

#ifdef __cplusplus
}
#endif

#endif /* __BH_1730FVC_H */

/* end of file */
