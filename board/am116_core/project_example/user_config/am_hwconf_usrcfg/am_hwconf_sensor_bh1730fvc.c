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
 * \brief ������ BH1730FVC �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_bh1730fvc.h"
#include "am_common.h"
#include "zlg116_pin.h"
#include "am_zlg116_inst_init.h"

/** \brief ������ BH1730FVC �豸��Ϣʵ�� */
am_const am_local struct am_sensor_bh1730fvc_devinfo __g_bh1730fvc_info = {
    PIOB_0,            /*< \brief �������Ŷ���    */
    0x29               /*< \breif BH1730FVC I2C��ַ */
};

/** \breif ������ BH1730FVC �豸�ṹ�嶨�� */
am_local struct am_sensor_bh1730fvc_dev __g_bh1730fvc_dev;

/** \brief ������ BH1730FVC �豸ʵ���� */
am_sensor_handle_t am_sensor_bh1730fvc_inst_init (void)
{
    return am_sensor_bh1730fvc_init(&__g_bh1730fvc_dev,
                                    &__g_bh1730fvc_info,
                                    am_zlg116_i2c1_inst_init());
}

/** \brief ������ BH1730FVC ʵ�����ʼ�� */
am_err_t am_sensor_bh1730fvc_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_bh1730fvc_deinit(handle);
}

/* end of file */
