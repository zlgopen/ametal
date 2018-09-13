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
 * \brief HTS221 �����ļ�
 * \sa am_hwconf_hts221.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_hts221.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_hts221
 * \copydoc am_hwconf_hts221.c
 * @{
 */

/** \brief �豸��Ϣ */
am_local am_const am_hts221_devinfo_t __g_hts221_devinfo = {
    TEMP_SAMPLE_128,
    HUM_SAMPLE_256,
    POWER_ON,
    UPDATE_CONTINUE,
    DATA_UPDATE_12Hz
};

/*******************************************************************************
  HTS221 ʵ����ʼ��
*******************************************************************************/

/** \brief �豸���� */
am_local am_hts221_dev_t __g_hts221_dev;

/** \brief ʵ����ʼ�� */
am_hts221_handle_t am_hts221_inst_init (void)
{
    /* ��ȡ I2C ʵ����� */
    am_i2c_handle_t i2c_handle = am_zlg116_i2c1_inst_init();

    return am_hts221_init (&__g_hts221_dev, 
                           &__g_hts221_devinfo,
                           i2c_handle);   
}

/**
 * @}
 */

/* end of file */
