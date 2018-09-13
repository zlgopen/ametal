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
 * \brief    MMC5883MA����Ŵ�����Ӧ�ýӿ��ļ�
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-12  wk, first implementation.
 * \endinternal
 */

#ifndef __AM_MMC5883MA_H
#define __AM_MMC5883MA_H

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
 * \addtogroup am_if_mmc5883ma
 * \copydoc am_mmc5883ma.h
 * @{
 */
/**
 * \brief MMC5883MA����������
 */
	
typedef enum am_mmc5883ma_config {
/**
 * \brief �����õĲ���Ƶ��     
 */
	Start_Measurement                  = 0x57,            /** �������� */
	FREQUENCY_14                       = 0x01,            /** ����Ƶ��14Hz */
	FREQUENCY_5                        = 0x02,            /** ����Ƶ��5Hz */
	FREQUENCY_2point2                  = 0x03,            /** ����Ƶ��2.2Hz */
	FREQUENCY_1                        = 0x04,            /** ����Ƶ��1Hz */
	FREQUENCY_half                     = 0x05,            /** ����Ƶ��1/2Hz */
	FREQUENCY_one_fourth               = 0x06,            /** ����Ƶ��1/4Hz */
	FREQUENCY_one_eighths              = 0x07,            /** ����Ƶ��1/8Hz */
	FREQUENCY_one_sixteenth            = 0x08,            /** ����Ƶ��1/16Hz */
	FREQUENCY_one_thirty_two           = 0x09,            /** ����Ƶ��1/32Hz */
	FREQUENCY_one_sixty_four           = 0x0A,            /** ����Ƶ��1/64Hz */
} am_mmc5883ma_config_t;
	
/** \brief i2c��ַ */
#define MMC5883MA_ADDR                  0x30

/** \brief �豸�Ĵ�����ַ */
#define MMC5883MA_XOUT_LOW              0x00            /* Xout LSB */
#define MMC5883MA_XOUT_HIGH             0x01            /* Xout MSB */
#define MMC5883MA_YOUT_LOW              0x02            /* Yout LSB */
#define MMC5883MA_YOUT_HIGH             0x03            /* Yout MSB */
#define MMC5883MA_ZOUT_LOW              0x04            /* Zout LSB */
#define MMC5883MA_ZOUT_HIGH             0x05            /* Zout MSB */
#define MMC5883MA_TEMPERATURE           0x06            /* Temperature output*/
#define MMC5883MA_STATUS                0x07            /* Device status */
#define MMC5883MA_INTERNAL_CONTROL_0    0x08            /* Control register 0 */
#define MMC5883MA_INTERNAL_CONTROL_1    0x09            /* Control register 1 */
#define MMC5883MA_INTERNAL_CONTROL_2    0x0A            /* Control register 2 */
#define MMC5883MA_X_THRESHOLD           0x0B            /* Motion detection threshold of X  */
#define MMC5883MA_Y_THRESHOLD           0x0C            /* Motion detection threshold of Y  */
#define MMC5883MA_Z_THRESHOLD           0x0D            /* Motion detection threshold of Z  */
#define MMC5883MA_PRODUCT_ID_1          0x2F            /* Product ID */

/**
 * \brief MMC5883MAʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_mmc5883ma_devinfo {    
    uint8_t frequency;                                  /**< \brief ����Ƶ��            */
	  uint8_t start_measurement;                          /**< \brief ��������            */
} am_mmc5883ma_devinfo_t;

/**
 * \brief MMC5883MA �豸�ṹ��
 */
typedef struct am_mmc5883ma_dev {
    am_i2c_device_t i2c_dev;                            /**< \brief MMC5883MA I2C�豸 */
} am_mmc5883ma_dev_t;

/** \brief BMG160����������� */
typedef am_mmc5883ma_dev_t *am_mmc5883ma_handle_t;

/**
 * \brief MMC5883MA��������ȡ
 *
 * \param[in]  handle  : MMC5883MA����������
 * \param[out] p_accel : ָ�������ǵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_mmc5883ma_read_magnetic(am_mmc5883ma_handle_t handle, int16_t* p_magnetic);

/**
 * \brief MMC5883MA��������ȡ�¶�
 *
 * \param[in]  handle : MMC5883MA����������
 * \param[out] p_temp : ָ���¶ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_mmc5883ma_read_temp (am_mmc5883ma_handle_t handle,int8_t* p_temp);

/**
 * \brief MMC5883MA��������ʼ��
 *
 * \param[in] p_dev      : ָ��MMC5883MA�豸�ṹ���ָ��
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return MMC5883MA����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_mmc5883ma_handle_t am_mmc5883ma_init (am_mmc5883ma_dev_t *p_dev,
                           const am_mmc5883ma_devinfo_t *p_devinfo,  
																       am_i2c_handle_t i2c_handle);

/**
 * \brief MMC5883MA �豸���ʼ��
 * \param[in]  handle : MMC5883MA����������
 * \return ��
 */
void am_mmc5883ma_deinit(am_mmc5883ma_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MMC5883MA_H */

/* end of file */
