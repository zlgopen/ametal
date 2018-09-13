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
 * \brief BMA253 �����ļ�
 * \sa am_hwconf_bma253.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-10  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_bma253.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_bma253
 * \copydoc am_hwconf_bma253.c
 * @{
 */

/** \brief �豸��Ϣ */
am_local am_const am_bma253_devinfo_t __g_bma253_devinfo = {
    PMU_RANGE_2G,                         /**< \brief ���ٶȷ�Χ��2g          */
    PMU_BW_8Hz,                           /**< \brief �����˲�����7.81Hz      */
    NORMAL_MODE,                          /**< \brief ����ģʽ                */
    SLEEP_DUR_HALF_MS                     /**< \brief ���õ�˯������Ϊ0.5ms   */
};

/*******************************************************************************
  BMA253 ʵ����ʼ��
*******************************************************************************/

/** \brief �豸���� */
am_local am_bma253_dev_t __g_bma253_dev;

/** \brief ʵ����ʼ�� */
am_bma253_handle_t am_bma253_inst_init (void)
{
    /* ��ȡ I2C ʵ����� */
    am_i2c_handle_t i2c_handle =  am_zlg116_i2c1_inst_init();

    return am_bma253_init (&__g_bma253_dev, 
                           &__g_bma253_devinfo,
                           i2c_handle);   
}

/**
 * @}
 */

/* end of file */
