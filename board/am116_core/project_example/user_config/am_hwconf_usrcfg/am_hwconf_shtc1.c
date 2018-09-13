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
 * \brief SHTC1 �����ļ�
 * \sa am_hwconf_shtc1.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_shtc1.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_shtc1
 * \copydoc am_hwconf_shtc1.c
 * @{
 */

/*******************************************************************************
  SHTC1 ʵ����ʼ��
*******************************************************************************/

/** \brief �豸���� */
am_local am_shtc1_dev_t __g_shtc1_dev;

/** \brief ʵ����ʼ�� */
am_shtc1_handle_t am_shtc1_inst_init (void)
{
    /* ��ȡ I2C ʵ����� */
    am_i2c_handle_t i2c_handle = am_zlg116_i2c1_inst_init();

    return am_shtc1_init (&__g_shtc1_dev, i2c_handle);   
}

/**
 * @}
 */

/* end of file */
