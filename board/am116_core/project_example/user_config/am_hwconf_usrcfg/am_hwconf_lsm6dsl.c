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
 * \brief LSM6DSL �����ļ�
 * \sa am_hwconf_lsm6dsl.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  wk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_lsm6dsl.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_lsm6dsl
 * \copydoc am_hwconf_lsm6dsl.c
 * @{
 */
 
 
/** \brief �豸��Ϣ */
am_local am_const am_lsm6dsl_devinfo_t __g_lsm6dsl_devinfo = {
  
	 CTRL1_XL,
	 STATUS_REG,
   FIFO_ODR_12point5       /**< \brief FIFO�������Ϊ12.5Hz   */

};
 
/*******************************************************************************
  LSM6DSL ʵ����ʼ��
*******************************************************************************/

/** \brief �豸ʵ������ */
am_local am_lsm6dsl_dev_t __g_lsm6dsl_dev;

/** \brief ʵ����ʼ�� */
am_lsm6dsl_handle_t am_lsm6dsl_inst_init (void)
{
    /* ��ȡ I2C ʵ����� */
    am_i2c_handle_t i2c_handle =  am_zlg116_i2c1_inst_init();

    return am_lsm6dsl_init (&__g_lsm6dsl_dev, 
                          &__g_lsm6dsl_devinfo,
                                     i2c_handle);   
}

/**
 * @}
 */

/* end of file */
