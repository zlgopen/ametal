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
 * \brief BMG160 �����ļ�
 * \sa am_hwconf_bmg160.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  wk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_bmg160.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_bmg160
 * \copydoc am_hwconf_bmg160.c
 * @{
 */
 
 /** \brief �豸��Ϣ */
am_local am_const am_bmg160_devinfo_t __g_bmg160_devinfo = {

    RANGE_125,                          /**< \brief �����ʷ�Χ��125��/s   */
    BW_230Hz,                           /**< \brief �����˲�����230Hz     */
    NORMAL_MODE,                        /**< \brief ����ģʽ              */
    SLEEP_DUR_2MS                       /**< \brief ���õ�˯������Ϊ2ms   */
	
};

/*******************************************************************************
  BMG160 ʵ����ʼ��
*******************************************************************************/

/** \brief �豸���� */
am_local am_bmg160_dev_t __g_bmg160_dev;

/** \brief ʵ����ʼ�� */
am_bmg160_handle_t am_bmg160_inst_init (void)
{
    /* ��ȡ I2C ʵ����� */
    am_i2c_handle_t i2c_handle =  am_zlg116_i2c1_inst_init();

    return am_bmg160_init (&__g_bmg160_dev, 
                       &__g_bmg160_devinfo,
                               i2c_handle);   
}

/**
 * @}
 */

/* end of file */

