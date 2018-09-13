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
 * \brief    LTR_553ALS����봫����Ӧ�ýӿ��ļ�
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
 * \brief LTR_553ALS��ǿ������Χ 
 */    
#define   ALS_GAIN_1X    AM_SBF(0, 2)        /**< \brief 1~64K     lux       */
#define   ALS_GAIN_2X    AM_SBF(1, 2)        /**< \brief 0.5~32K   lux       */
#define   ALS_GAIN_4X    AM_SBF(2, 2)        /**< \brief 0.25~16K  lux       */
#define   ALS_GAIN_8X    AM_SBF(3, 2)        /**< \brief 0.125~8K  lux       */
#define   ALS_GAIN_48X   AM_SBF(6, 2)        /**< \brief 0.02~1.3K lux       */
#define   ALS_GAIN_96X   AM_SBF(7, 2)        /**< \brief 0.01~600  lux       */

/**
 * \brief LTR_553ALS���������Χ 
 */
#define   PS_GAIN_16X    AM_SBF(0, 2)        /**< \brief X16                 */
#define   PS_GAIN_32X    AM_SBF(2, 2)        /**< \brief X32                 */
#define   PS_GAIN_64X    AM_SBF(3, 2)        /**< \brief X64                 */ 
 
/**
 * \brief LTR_553ALS LED����Ƶ��
 */ 
#define   LED_PULSE_FREQ_30KHz  AM_SBF(0, 5) /**< \brief LED����Ƶ��30KHz    */
#define   LED_PULSE_FREQ_40KHz  AM_SBF(1, 5) /**< \brief LED����Ƶ��40KHz    */
#define   LED_PULSE_FREQ_50KHz  AM_SBF(2, 5) /**< \brief LED����Ƶ��50KHz    */
#define   LED_PULSE_FREQ_60KHz  AM_SBF(3, 5) /**< \brief LED����Ƶ��60KHz    */
#define   LED_PULSE_FREQ_70KHz  AM_SBF(4, 5) /**< \brief LED����Ƶ��70KHz    */
#define   LED_PULSE_FREQ_80KHz  AM_SBF(5, 5) /**< \brief LED����Ƶ��80KHz    */
#define   LED_PULSE_FREQ_90KHz  AM_SBF(6, 5) /**< \brief LED����Ƶ��90KHz    */
#define   LED_PULSE_FREQ_100KHz AM_SBF(7, 5) /**< \brief LED����Ƶ��100KHz   */

/**
 * \brief LTR_553ALS LED����ռ�ձ�
 */ 
#define   LED_CURRENT_DUTY_25   AM_SBF(0, 3) /**< \brief LED����ռ�ձ�Ϊ25%  */
#define   LED_CURRENT_DUTY_50   AM_SBF(1, 3) /**< \brief LED����ռ�ձ�Ϊ50%  */
#define   LED_CURRENT_DUTY_75   AM_SBF(2, 3) /**< \brief LED����ռ�ձ�Ϊ75%  */
#define   LED_CURRENT_DUTY_100  AM_SBF(3, 3) /**< \brief LED����ռ�ձ�Ϊ100% */

/**
 * \brief LTR_553ALS LED����
 */  
#define   LED_CURRENT_5MA       AM_SBF(0, 0) /**< \brief LED����5mA          */
#define   LED_CURRENT_10MA      AM_SBF(1, 0) /**< \brief LED����10mA         */
#define   LED_CURRENT_20MA      AM_SBF(2, 0) /**< \brief LED����20mA         */
#define   LED_CURRENT_50MA      AM_SBF(3, 0) /**< \brief LED����50mA         */
#define   LED_CURRENT_100MA     AM_SBF(4, 0) /**< \brief LED����100mA        */

/**
 * \brief LTR_553ALS LED�����������
 */ 
#define   LED_PULSE_NUM_1      AM_SBF(1, 0)  /**< \brief LED����1������      */
#define   LED_PULSE_NUM_2      AM_SBF(2, 0)  /**< \brief LED����2������      */
#define   LED_PULSE_NUM_3      AM_SBF(3, 0)  /**< \brief LED����3������      */
#define   LED_PULSE_NUM_4      AM_SBF(4, 0)  /**< \brief LED����4������      */
#define   LED_PULSE_NUM_5      AM_SBF(5, 0)  /**< \brief LED����5������      */
#define   LED_PULSE_NUM_6      AM_SBF(6, 0)  /**< \brief LED����6������      */
#define   LED_PULSE_NUM_7      AM_SBF(7, 0)  /**< \brief LED����7������      */
#define   LED_PULSE_NUM_8      AM_SBF(8, 0)  /**< \brief LED����8������      */
#define   LED_PULSE_NUM_9      AM_SBF(9, 0)  /**< \brief LED����9������      */
#define   LED_PULSE_NUM_10     AM_SBF(10, 0) /**< \brief LED����10������     */
#define   LED_PULSE_NUM_11     AM_SBF(11, 0) /**< \brief LED����11������     */
#define   LED_PULSE_NUM_12     AM_SBF(12, 0) /**< \brief LED����12������     */
#define   LED_PULSE_NUM_13     AM_SBF(13, 0) /**< \brief LED����13������     */
#define   LED_PULSE_NUM_14     AM_SBF(14, 0) /**< \brief LED����14������     */
#define   LED_PULSE_NUM_15     AM_SBF(15, 0) /**< \brief LED����15������     */

/**
 * \brief LTR_553ALS ��������ٶ�
 */
#define   PS_MEAS_RATE_10MS    AM_SBF(8, 0)  /**< \brief ��������ٶ�Ϊ10ms   */
#define   PS_MEAS_RATE_50MS    AM_SBF(0, 0)  /**< \brief ��������ٶ�Ϊ50ms   */
#define   PS_MEAS_RATE_70MS    AM_SBF(1, 0)  /**< \brief ��������ٶ�Ϊ70ms   */
#define   PS_MEAS_RATE_100MS   AM_SBF(2, 0)  /**< \brief ��������ٶ�Ϊ100ms  */
#define   PS_MEAS_RATE_200MS   AM_SBF(3, 0)  /**< \brief ��������ٶ�Ϊ200ms  */
#define   PS_MEAS_RATE_500MS   AM_SBF(4, 0)  /**< \brief ��������ٶ�Ϊ500ms  */
#define   PS_MEAS_RATE_1000MS  AM_SBF(5, 0)  /**< \brief ��������ٶ�Ϊ1000ms */
#define   PS_MEAS_RATE_2000MS  AM_SBF(6, 0)  /**< \brief ��������ٶ�Ϊ2000ms */

/**
 * \brief LTR_553ALS ���ջ���ʱ��
 */
#define   ALS_INTEGRATE_50MS   AM_SBF(1, 3)  /**< \brief ���ջ���ʱ��Ϊ50ms   */
#define   ALS_INTEGRATE_100MS  AM_SBF(0, 3)  /**< \brief ���ջ���ʱ��Ϊ100ms  */
#define   ALS_INTEGRATE_150MS  AM_SBF(4, 3)  /**< \brief ���ջ���ʱ��Ϊ150ms  */
#define   ALS_INTEGRATE_200MS  AM_SBF(2, 3)  /**< \brief ���ջ���ʱ��Ϊ200ms  */
#define   ALS_INTEGRATE_250MS  AM_SBF(5, 3)  /**< \brief ���ջ���ʱ��Ϊ250ms  */
#define   ALS_INTEGRATE_300MS  AM_SBF(6, 3)  /**< \brief ���ջ���ʱ��Ϊ300ms  */
#define   ALS_INTEGRATE_350MS  AM_SBF(7, 3)  /**< \brief ���ջ���ʱ��Ϊ350ms  */
#define   ALS_INTEGRATE_400MS  AM_SBF(3, 3)  /**< \brief ���ջ���ʱ��Ϊ400ms  */

/**
 * \brief LTR_553ALS ��ǿ�����ٶ�
 */
#define   ALS_MEAS_RATE_50MS   AM_SBF(0, 0)  /**< \brief ��ǿ�����ٶ�Ϊ50ms   */
#define   ALS_MEAS_RATE_100MS  AM_SBF(1, 0)  /**< \brief ��ǿ�����ٶ�Ϊ100ms  */
#define   ALS_MEAS_RATE_200MS  AM_SBF(2, 0)  /**< \brief ��ǿ�����ٶ�Ϊ200ms  */
#define   ALS_MEAS_RATE_500MS  AM_SBF(3, 0)  /**< \brief ��ǿ�����ٶ�Ϊ500ms  */
#define   ALS_MEAS_RATE_1000MS AM_SBF(4, 0)  /**< \brief ��ǿ�����ٶ�Ϊ1000ms */
#define   ALS_MEAS_RATE_2000MS AM_SBF(5, 0)  /**< \brief ��ǿ�����ٶ�Ϊ2000ms */

/**
 * \brief LTR_553ALSʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_ltr_553als_devinfo {    
    uint8_t  als_gain;                       /**< \brief ��ǿ������Χ        */
    uint8_t  ps_gain;                        /**< \brief ���������Χ        */
    uint8_t  led_pulse_freq;                 /**< \brief LED����Ƶ��         */
    uint8_t  led_current_duty;               /**< \brief LED����ռ�ձ�       */
    uint8_t  led_current;                    /**< \brief LED����             */
    uint8_t  led_pulse_num;                  /**< \brief LED������           */
    uint8_t  ps_meas_rate;                   /**< \brief ��������ٶ�        */
    uint8_t  als_integre_time;               /**< \brief ���ջ���ʱ��        */
    uint8_t  als_meas_rate;                  /**< \brief ���ղ����ٶ�        */
} am_ltr_553als_devinfo_t;

/**
 * \brief LTR_553ALS �豸�ṹ��
 */
typedef struct am_ltr_553als_dev {
    
    /**< \brief LTR_553ALS I2C�豸 */
    am_i2c_device_t i2c_dev;          
    
    /** \brief ���ڱ����豸��Ϣָ�� */
    const am_ltr_553als_devinfo_t  *p_devinfo;
    
} am_ltr_553als_dev_t;

/** \brief LTR_553ALS����������� */
typedef am_ltr_553als_dev_t *am_ltr_553als_handle_t;

/**
 * \brief LTR_553ALS��������ȡ��ǿ
 *
 * \param[in]  handle  : LTR_553ALS����������
 * \param[out] p_als   : ָ���ǿ��ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_ltr_553als_read_als (am_ltr_553als_handle_t handle, uint16_t* p_als);

/**
 * \brief LTR_553ALS��������ȡ����
 *
 * \param[in]  handle  : LTR_553ALS����������
 * \param[out] p_als   : ָ������ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_ltr_553als_read_ps (am_ltr_553als_handle_t handle, uint16_t* p_ps);

/**
 * \brief LTR_553ALS��������ʼ��
 *
 * \param[in] p_dev      : ָ��LTR_553ALS�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��LTR_553ALS�豸��Ϣ��ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return LTR_553ALS����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_ltr_553als_handle_t am_ltr_553als_init (am_ltr_553als_dev_t *p_dev,
                                       const am_ltr_553als_devinfo_t *p_devinfo,
                                       am_i2c_handle_t i2c_handle);

/**
 * \brief LTR_553ALS �豸���ʼ��
 * \param[in]  handle : LTR_553ALS����������
 * \return ��
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
