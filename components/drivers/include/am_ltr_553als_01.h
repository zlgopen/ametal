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
 * \brief    LTR_553ALS光距离传感器应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-07  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_LTR_553ALS_H
#define __AM_LTR_553ALS_H

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
 * \addtogroup am_if_ltr_553als.h
 * \copydoc am_ltr_553als.h
 * @{
 */

/**
 * \brief LTR_553ALS光强测量范围 
 */    
#define   ALS_GAIN_1X    AM_SBF(0, 2)        /**< \brief 1~64K     lux       */
#define   ALS_GAIN_2X    AM_SBF(1, 2)        /**< \brief 0.5~32K   lux       */
#define   ALS_GAIN_4X    AM_SBF(2, 2)        /**< \brief 0.25~16K  lux       */
#define   ALS_GAIN_8X    AM_SBF(3, 2)        /**< \brief 0.125~8K  lux       */
#define   ALS_GAIN_48X   AM_SBF(6, 2)        /**< \brief 0.02~1.3K lux       */
#define   ALS_GAIN_96X   AM_SBF(7, 2)        /**< \brief 0.01~600  lux       */

/**
 * \brief LTR_553ALS距离测量范围 
 */
#define   PS_GAIN_16X    AM_SBF(0, 2)        /**< \brief X16                 */
#define   PS_GAIN_32X    AM_SBF(2, 2)        /**< \brief X32                 */
#define   PS_GAIN_64X    AM_SBF(3, 2)        /**< \brief X64                 */ 
 
/**
 * \brief LTR_553ALS LED脉冲频率
 */ 
#define   LED_PULSE_FREQ_30KHz  AM_SBF(0, 5) /**< \brief LED脉冲频率30KHz    */
#define   LED_PULSE_FREQ_40KHz  AM_SBF(1, 5) /**< \brief LED脉冲频率40KHz    */
#define   LED_PULSE_FREQ_50KHz  AM_SBF(2, 5) /**< \brief LED脉冲频率50KHz    */
#define   LED_PULSE_FREQ_60KHz  AM_SBF(3, 5) /**< \brief LED脉冲频率60KHz    */
#define   LED_PULSE_FREQ_70KHz  AM_SBF(4, 5) /**< \brief LED脉冲频率70KHz    */
#define   LED_PULSE_FREQ_80KHz  AM_SBF(5, 5) /**< \brief LED脉冲频率80KHz    */
#define   LED_PULSE_FREQ_90KHz  AM_SBF(6, 5) /**< \brief LED脉冲频率90KHz    */
#define   LED_PULSE_FREQ_100KHz AM_SBF(7, 5) /**< \brief LED脉冲频率100KHz   */

/**
 * \brief LTR_553ALS LED电流占空比
 */ 
#define   LED_CURRENT_DUTY_25   AM_SBF(0, 3) /**< \brief LED电流占空比为25%  */
#define   LED_CURRENT_DUTY_50   AM_SBF(1, 3) /**< \brief LED电流占空比为50%  */
#define   LED_CURRENT_DUTY_75   AM_SBF(2, 3) /**< \brief LED电流占空比为75%  */
#define   LED_CURRENT_DUTY_100  AM_SBF(3, 3) /**< \brief LED电流占空比为100% */

/**
 * \brief LTR_553ALS LED电流
 */  
#define   LED_CURRENT_5MA       AM_SBF(0, 0) /**< \brief LED电流5mA          */
#define   LED_CURRENT_10MA      AM_SBF(1, 0) /**< \brief LED电流10mA         */
#define   LED_CURRENT_20MA      AM_SBF(2, 0) /**< \brief LED电流20mA         */
#define   LED_CURRENT_50MA      AM_SBF(3, 0) /**< \brief LED电流50mA         */
#define   LED_CURRENT_100MA     AM_SBF(4, 0) /**< \brief LED电流100mA        */

/**
 * \brief LTR_553ALS LED发射的脉冲数
 */ 
#define   LED_PULSE_NUM_1      AM_SBF(1, 0)  /**< \brief LED发射1个脉冲      */
#define   LED_PULSE_NUM_2      AM_SBF(2, 0)  /**< \brief LED发射2个脉冲      */
#define   LED_PULSE_NUM_3      AM_SBF(3, 0)  /**< \brief LED发射3个脉冲      */
#define   LED_PULSE_NUM_4      AM_SBF(4, 0)  /**< \brief LED发射4个脉冲      */
#define   LED_PULSE_NUM_5      AM_SBF(5, 0)  /**< \brief LED发射5个脉冲      */
#define   LED_PULSE_NUM_6      AM_SBF(6, 0)  /**< \brief LED发射6个脉冲      */
#define   LED_PULSE_NUM_7      AM_SBF(7, 0)  /**< \brief LED发射7个脉冲      */
#define   LED_PULSE_NUM_8      AM_SBF(8, 0)  /**< \brief LED发射8个脉冲      */
#define   LED_PULSE_NUM_9      AM_SBF(9, 0)  /**< \brief LED发射9个脉冲      */
#define   LED_PULSE_NUM_10     AM_SBF(10, 0) /**< \brief LED发射10个脉冲     */
#define   LED_PULSE_NUM_11     AM_SBF(11, 0) /**< \brief LED发射11个脉冲     */
#define   LED_PULSE_NUM_12     AM_SBF(12, 0) /**< \brief LED发射12个脉冲     */
#define   LED_PULSE_NUM_13     AM_SBF(13, 0) /**< \brief LED发射13个脉冲     */
#define   LED_PULSE_NUM_14     AM_SBF(14, 0) /**< \brief LED发射14个脉冲     */
#define   LED_PULSE_NUM_15     AM_SBF(15, 0) /**< \brief LED发射15个脉冲     */

/**
 * \brief LTR_553ALS 距离测量速度
 */
#define   PS_MEAS_RATE_10MS    AM_SBF(8, 0)  /**< \brief 距离测量速度为10ms   */
#define   PS_MEAS_RATE_50MS    AM_SBF(0, 0)  /**< \brief 距离测量速度为50ms   */
#define   PS_MEAS_RATE_70MS    AM_SBF(1, 0)  /**< \brief 距离测量速度为70ms   */
#define   PS_MEAS_RATE_100MS   AM_SBF(2, 0)  /**< \brief 距离测量速度为100ms  */
#define   PS_MEAS_RATE_200MS   AM_SBF(3, 0)  /**< \brief 距离测量速度为200ms  */
#define   PS_MEAS_RATE_500MS   AM_SBF(4, 0)  /**< \brief 距离测量速度为500ms  */
#define   PS_MEAS_RATE_1000MS  AM_SBF(5, 0)  /**< \brief 距离测量速度为1000ms */
#define   PS_MEAS_RATE_2000MS  AM_SBF(6, 0)  /**< \brief 距离测量速度为2000ms */

/**
 * \brief LTR_553ALS 光照积分时间
 */
#define   ALS_INTEGRATE_50MS   AM_SBF(1, 3)  /**< \brief 光照积分时间为50ms   */
#define   ALS_INTEGRATE_100MS  AM_SBF(0, 3)  /**< \brief 光照积分时间为100ms  */
#define   ALS_INTEGRATE_150MS  AM_SBF(4, 3)  /**< \brief 光照积分时间为150ms  */
#define   ALS_INTEGRATE_200MS  AM_SBF(2, 3)  /**< \brief 光照积分时间为200ms  */
#define   ALS_INTEGRATE_250MS  AM_SBF(5, 3)  /**< \brief 光照积分时间为250ms  */
#define   ALS_INTEGRATE_300MS  AM_SBF(6, 3)  /**< \brief 光照积分时间为300ms  */
#define   ALS_INTEGRATE_350MS  AM_SBF(7, 3)  /**< \brief 光照积分时间为350ms  */
#define   ALS_INTEGRATE_400MS  AM_SBF(3, 3)  /**< \brief 光照积分时间为400ms  */

/**
 * \brief LTR_553ALS 光强测量速度
 */
#define   ALS_MEAS_RATE_50MS   AM_SBF(0, 0)  /**< \brief 光强测量速度为50ms   */
#define   ALS_MEAS_RATE_100MS  AM_SBF(1, 0)  /**< \brief 光强测量速度为100ms  */
#define   ALS_MEAS_RATE_200MS  AM_SBF(2, 0)  /**< \brief 光强测量速度为200ms  */
#define   ALS_MEAS_RATE_500MS  AM_SBF(3, 0)  /**< \brief 光强测量速度为500ms  */
#define   ALS_MEAS_RATE_1000MS AM_SBF(4, 0)  /**< \brief 光强测量速度为1000ms */
#define   ALS_MEAS_RATE_2000MS AM_SBF(5, 0)  /**< \brief 光强测量速度为2000ms */

/**
 * \brief LTR_553ALS实例信息结构体定义
 */
typedef struct am_ltr_553als_devinfo {    
    uint8_t  als_gain;                       /**< \brief 光强测量范围        */
    uint8_t  ps_gain;                        /**< \brief 距离测量范围        */
    uint8_t  led_pulse_freq;                 /**< \brief LED脉冲频率         */
    uint8_t  led_current_duty;               /**< \brief LED电流占空比       */
    uint8_t  led_current;                    /**< \brief LED电流             */
    uint8_t  led_pulse_num;                  /**< \brief LED脉冲数           */
    uint8_t  ps_meas_rate;                   /**< \brief 距离测量速度        */
    uint8_t  als_integre_time;               /**< \brief 光照积分时间        */
    uint8_t  als_meas_rate;                  /**< \brief 光照测量速度        */
} am_ltr_553als_devinfo_t;

/**
 * \brief LTR_553ALS 设备结构体
 */
typedef struct am_ltr_553als_dev {
    
    /**< \brief LTR_553ALS I2C设备 */
    am_i2c_device_t i2c_dev;          
    
    /** \brief 用于保存设备信息指针 */
    const am_ltr_553als_devinfo_t  *p_devinfo;
    
} am_ltr_553als_dev_t;

/** \brief LTR_553ALS操作句柄定义 */
typedef am_ltr_553als_dev_t *am_ltr_553als_handle_t;

/**
 * \brief LTR_553ALS传感器读取光强
 *
 * \param[in]  handle  : LTR_553ALS服务操作句柄
 * \param[out] p_als   : 指向光强的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_ltr_553als_read_als (am_ltr_553als_handle_t handle, uint16_t* p_als);

/**
 * \brief LTR_553ALS传感器读取距离
 *
 * \param[in]  handle  : LTR_553ALS服务操作句柄
 * \param[out] p_als   : 指向距离的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_ltr_553als_read_ps (am_ltr_553als_handle_t handle, uint16_t* p_ps);

/**
 * \brief LTR_553ALS传感器初始化
 *
 * \param[in] p_dev      : 指向LTR_553ALS设备结构体的指针
 * \param[in] p_devinfo  : 指向LTR_553ALS设备信息的指针 
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return LTR_553ALS服务操作句柄,如果为 NULL，表明初始化失败
 */
am_ltr_553als_handle_t am_ltr_553als_init (am_ltr_553als_dev_t *p_dev,
                                       const am_ltr_553als_devinfo_t *p_devinfo,
                                       am_i2c_handle_t i2c_handle);

/**
 * \brief LTR_553ALS 设备解初始化
 * \param[in]  handle : LTR_553ALS服务操作句柄
 * \return 无
 */
void am_ltr_553als_deinit (am_ltr_553als_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LTR_553ALS_H */

/* end of file */
