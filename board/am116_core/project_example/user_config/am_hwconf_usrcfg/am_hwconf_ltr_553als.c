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
 * \brief LTR_553ALS �����ļ�
 * \sa am_hwconf_ltr_553als.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-10  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_ltr_553als_01.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_ltr_553als
 * \copydoc am_hwconf_ltr_553als.c
 * @{
 */

/** \brief �豸��Ϣ */
am_local am_const am_ltr_553als_devinfo_t __g_ltr_553als_devinfo = {
    ALS_GAIN_48X,                             /**< \brief 0.02~1.3K lux       */
    PS_GAIN_32X,                              /**< \brief X32                 */
    LED_PULSE_FREQ_60KHz,                     /**< \brief LED����Ƶ��60KHz    */
    LED_CURRENT_DUTY_100,                     /**< \brief LED����ռ�ձ�Ϊ100% */
    LED_CURRENT_100MA,                        /**< \brief LED����100mA        */
    LED_PULSE_NUM_1,                          /**< \brief LED����1������      */ 
    PS_MEAS_RATE_100MS,                       /**< \brief ��������ٶ�Ϊ100ms */
    ALS_INTEGRATE_100MS,                      /**< \brief ���ջ���ʱ��Ϊ100ms */
    ALS_MEAS_RATE_500MS                       /**< \brief ��ǿ�����ٶ�Ϊ500ms */
};

/*******************************************************************************
  LTR_553ALS ʵ����ʼ��
*******************************************************************************/

/** \brief �豸���� */
am_local am_ltr_553als_dev_t __g_ltr_553als_dev;

/** \brief ʵ����ʼ�� */
am_ltr_553als_handle_t am_ltr_553als_inst_init (void)
{
    /* ��ȡ I2C ʵ����� */
    am_i2c_handle_t i2c_handle =  am_zlg116_i2c1_inst_init();

    return am_ltr_553als_init (&__g_ltr_553als_dev, 
                               &__g_ltr_553als_devinfo,
                               i2c_handle);   
}

/**
 * @}
 */

/* end of file */
