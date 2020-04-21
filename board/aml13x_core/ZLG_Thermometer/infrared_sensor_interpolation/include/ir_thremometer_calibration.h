/***************************Copyright(c)***********************************
**                Guangzhou ZHIYUAN electronics Co.,LTD.                   
**                                                                         
**                      http://www.zlg.cn                         
**                                                                         
**-------------File Info---------------------------------------------------
**File Name:            zal_infrared_sensor_interpolation.h
**Latest modified Date: 
**Latest Version:       
**Description:          
**                      
**-------------------------------------------------------------------------
**Created By:            moxinyu
**Created Date:         2020-3-18
**Version:              v1.0.0
**Description:          
**                      
**-------------------------------------------------------------------------
**Modified By:         
**Modified Date:      
**Version:              
**Description:          
**                      
**************************************************************************/
#ifndef _IR_THREMOMETER_CALIBRATION_H_
#define _IR_THREMOMETER_CALIBRATION_H_

/**
 * \brief 校准初始化
 */
void ir_thremometer_calib_init(void);

/**
 * \brief 设置校准温度点，一次只能设置两个校准点，设置两点后必须调用ir_thremometer_calib_run，之后才能再次校准
 *
 * \param[in] obj_temp : 校准温度点
 *
 * \return：AM_OK：成功；AM_ERROR：失败
 */
int ir_thremometer_set_calib_point(float obj_temp);

/**
 * \brief 运行校准算法
 *
 * \return：AM_OK：成功；AM_ERROR：失败
 */
int ir_thremometer_calib_run(void);

/**
 * \brief 人体模式切测物模式
 *
 * \param[in] temp : 人体模式温度
 *
 * \return：测物模式温度
 */
float ir_thremometer_mode_btot(float temp);

/**
 * \brief 测物模式切人体模式
 *
 * \param[in] temp : 测物模式温度
 *
 * \return：人体模式温度
 */
float ir_thremometer_mode_ttob(float temp);

/**
 * \brief 校准电压值
 *
 * \param[in] vol : 需要校准的电压值
 *
 * \return：校准后的电压值
 */
float ir_thremometer_calib_vol(float vol);

/**
 * \brief 检测是否校准过
 *
 * \return：AM_TRUE: 已经校准过；AM_FALSE：没有校准过
 */
am_bool_t ir_thremometer_if_already_calib(void);

#endif
