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
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
    
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
    BW_230Hz     =    0X01,             /**< \brief 数据滤波带宽230Hz     */
    BW_116Hz     =    0X02,             /**< \brief 数据滤波带宽116Hz     */
    BW_47Hz      =    0X03,             /**< \brief 数据滤波带宽47Hz      */
    BW_23Hz      =    0X04,             /**< \brief 数据滤波带宽23Hz      */
    BW_12Hz      =    0X05,             /**< \brief 数据滤波带宽12Hz      */
    BW_64Hz      =    0X06,             /**< \brief 数据滤波带宽64Hz      */
    BW_32Hz      =    0X07,             /**< \brief 数据滤波带宽32Hz      */
    
    /**
     * \brief 可配置的电源模式   
     */ 
    NORMAL_MODE       = 0X00,             /**< \brief 正常模式                */
    DEEP_SUSPEND_MODE = 0X20,             /**< \brief 深度挂起模式            */
    SUSPEND_MODE      = 0X80,             /**< \brief 挂起模式                */
    
    /**
     * \brief 可配置的睡眠周期   
     */
    SLEEP_DUR_2MS   = AM_SBF(0,1),      /**< \brief 配置的睡眠周期为2ms   */
    SLEEP_DUR_4MS   = AM_SBF(1,1),      /**< \brief 配置的睡眠周期为4ms   */
    SLEEP_DUR_5MS   = AM_SBF(2,1),      /**< \brief 配置的睡眠周期为5ms   */
    SLEEP_DUR_8MS   = AM_SBF(3,1),      /**< \brief 配置的睡眠周期为8ms   */
    SLEEP_DUR_10MS  = AM_SBF(4,1),      /**< \brief 配置的睡眠周期为10ms  */
    SLEEP_DUR_15MS  = AM_SBF(5,1),      /**< \brief 配置的睡眠周期为15ms  */
    SLEEP_DUR_18MS  = AM_SBF(6,1),      /**< \brief 配置的睡眠周期为18ms  */
    SLEEP_DUR_20MS  = AM_SBF(7,1),      /**< \brief 配置的睡眠周期为20ms  */ 
} am_bmg160_config_t;
	
/**
 * \brief BMG160实例信息结构体定义
 */
typedef struct am_bmg160_devinfo {    
    uint8_t range;                    /**< \brief 加速度范围              */
    uint8_t bw;                       /**< \brief 数据滤波带宽            */
    uint8_t power_mode;               /**< \brief 电源模式                */
    uint8_t sleep_period;             /**< \brief 低功耗模式睡眠周期      */
} am_bmg160_devinfo_t;
	
/** \brief i2c地址 */
#define BMG160_ADDR          (0x68)

/** \brief 设备寄存器地址 */
#define BMG160_CHIP_ID       (0x00)  /* 包含芯片编码 */
                              
#define BMG160_RATE_X_LSB    (0x02)  /* 包含X通道角速率读数值的最低有效位 */
#define BMG160_RATE_X_MSB    (0x03)  /* 包含X通道角速率读数值的最高有效位 */
#define BMG160_RATE_Y_LSB    (0x04)  /* 包含Y通道角速率读数值的最低有效位 */
#define BMG160_RATE_Y_MSB    (0x05)  /* 包含Y通道角速率读数值的最高有效位 */
#define BMG160_RATE_Z_LSB    (0x06)  /* 包含Z通道角速率读数值的最低有效位 */
#define BMG160_RATE_Z_MSB    (0x07)  /* 包含Z通道角速率读数值的最高有效位 */
#define BMG160_TEMP          (0x08)  /* 包含当前芯片温度 */
#define BMG160_INT_STATUS_0  (0x09)  /* 包含中断状态位 */
#define BMG160_INT_STATUS_1  (0x0A)  /* 包含中断状态位 */
#define BMG160_INT_STATUS_2  (0x0B)  /* 包含任何运动中断状态位 */
#define BMG160_INT_STATUS_3  (0x0C)  /* 包含高速率中断状态位 */
                             
#define BMG160_FIFO_STATUS   (0x0E)  /* 包含FIFO状态标志 */
#define BMG160_RANGE         (0x0F)  /* 包含BMG160支持四种不同的角速率测量范围 */
#define BMG160_BW            (0x10)  /* 该寄存器允许选择速率数据滤波器带宽 */
#define BMG160_LPM1          (0x11)  /* 选择主电源模式 */
#define BMG160_LPM2          (0x12)  /* 快速上电和外部触发的配置设置 */
#define BMG160_RATE_HBW      (0x13)  /* 角速率数据采集和数据输出格式 */
#define BMG160_BGW_SOFTRESET (0x14)	 /* 控制用户触发的传感器复位 */ 
#define BMG160_INT_EN_0      (0x15)  /* 控制启用哪些中断 */
#define BMG160_INT_EN_1      (0x16)  /* 包含中断引脚配置 */
#define BMG160_INT_MAP_0     (0x17)  /* 控制哪些中断信号映射到INT1引脚 */
#define BMG160_INT_MAP_1     (0x18)  /* 控制哪些中断信号映射到INT1引脚和INT2引脚 */
#define BMG160_INT_MAP_2     (0x19)  /* 控制哪些中断信号映射到INT2引脚 */
                              
#define BMG160_INT_RST_LATCH (0x21)  /* 包含中断复位位和中断模式选择 */
#define BMG160_High_Th_x     (0x22)  /* 包含x轴的高速率阈值和高速率滞后设置 */
#define BMG160_High_Dur_x    (0x23)  /* 包含x轴的高速率持续时间设置 */
#define BMG160_High_Th_y     (0x24)  /* 包含y轴的高速率阈值和高速率滞后设置 */
#define BMG160_High_Dur_y    (0x25)  /* 包含y轴的高速率持续时间设置 */
#define BMG160_High_Th_z     (0x26)  /* 包含z轴的高速率阈值和高速率滞后设置 */
#define BMG160_High_Dur_z    (0x27)  /* 包含z轴的高速率持续时间设置 */
                              
#define BMG160_SoC           (0x31)  /* 包含慢速偏移取消设置 */
#define BMG160_A_FOC         (0x32)  /* 包含快速偏移取消设置 */
#define BMG160_TRIM_NVM_CTRL (0x33)  /* 包含几次可编程非易失性存储器（NVM）的控制设置 */
#define BMG160_BGW_SPI3_WDT  (0x34)  /* 包含数字接口的设置 */
                              
#define BMG160_OFC1          (0x36)  /* 包含偏移补偿值 */
#define BMG160_OFC2          (0x37)  /* 包含X通道的偏移补偿值 */
#define BMG160_OFC3          (0x38)  /* 包含Y通道的偏移补偿值 */
#define BMG160_OFC4          (0x39)  /* 包含Z通道的偏移补偿值 */
#define BMG160_TRIM_GP0      (0x3A)  /* 包含具有NVM备份的通用数据寄存器 */
#define BMG160_TRIM_GP1      (0x3B)  /* 包含具有NVM备份的通用数据寄存器 */
#define BMG160_BIST          (0x3C)  /* 包含内置自测（BIST）功能 */
#define BMG160_FIFO_CONFIG_0 (0x3D)  /* 包含FIFOwatermark级别 */
#define BMG160_FIFO_CONFIG_1 (0x3E)  /* 包含FIFO配置设置 */
#define BMG160_FIFO_DATA     (0x3F)  /* FIFO数据读出寄存器 */

/**
 * \brief BMG160 设备结构体
 */
typedef struct am_bmg160_dev {
    am_i2c_device_t i2c_dev;       /**< \brief BMG160 I2C设备 */
} am_bmg160_dev_t;

/** \brief BMG160操作句柄定义 */
typedef am_bmg160_dev_t *am_bmg160_handle_t;

/**
 * \brief BMG160传感器读取三轴陀螺仪
 *
 * \param[in]  handle  : BMG160服务操作句柄
 * \param[out] p_accel : 指向陀螺仪的指针
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
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return BMG160服务操作句柄,如果为 NULL，表明初始化失败
 */
am_bmg160_handle_t am_bmg160_init (am_bmg160_dev_t *p_dev,
                                   const am_bmg160_devinfo_t *p_devinfo,  
                                   am_i2c_handle_t i2c_handle);

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
