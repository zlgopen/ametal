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
 * \brief    LSM6DSL������ٶȴ�����Ӧ�ýӿ��ļ�
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-12  wk, first implementation.
 * \endinternal
 */

#ifndef __AM_LSM6DSL_H
#define __AM_LSM6DSL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
    
/**
 * \addtogroup am_if_lsm6dsl
 * \copydoc am_lsm6dsl.h
 * @{
 */
/**
 * \brief LSM6DSL����������
 */

typedef enum am_lsm6dsl_config {
	
	/**
	 * \brief �����õ�״̬�Ĵ���
	 */
	STATUS_REG         =   0x03,           /* �¶ȡ������ǡ����ٶȼ������ݿ��� */
	
	CTRL1_XL           =   0x10,
	
	/**
	 * \brief �����õ�FIFO�������
	 */
	FIFO_ODR_12point5  =   0x01,           /**< \brief FIFO�������12.5Hz  */
	FIFO_ODR_26        =   0x02,           /**< \brief FIFO�������26Hz    */
	FIFO_ODR_52        =   0x03,           /**< \brief FIFO�������52Hz    */
	FIFO_ODR_104       =   0x04,           /**< \brief FIFO�������104Hz   */
	FIFO_ODR_208       =   0x05,           /**< \brief FIFO�������208Hz   */
	FIFO_ODR_416       =   0x06,           /**< \brief FIFO�������416Hz   */
	FIFO_ODR_833       =   0x07,           /**< \brief FIFO�������833Hz   */
	FIFO_ODR_1point66k =   0x08,           /**< \brief FIFO�������1.66kHz */
	FIFO_ODR_3point33k =   0x09,           /**< \brief FIFO�������3.33kHz */
	FIFO_ODR_6point66k =   0x0A,           /**< \brief FIFO�������6.66kHz */
	
}am_lsm6dsl_config_t;
	
/**
 * \brief BMG160ʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_lsm6dsl_devinfo {    
	
	uint8_t ctrl1_xl;
  uint8_t status_reg;                       /**< \brief ״̬�Ĵ���         */
	uint8_t fifo_odr;                       /**< \brief FIFO�������         */
	

} am_lsm6dsl_devinfo_t;

	/** \brief i2c��ַ */
#define    LSM6DSL_ADDR                         0x6B

/** \brief �豸�Ĵ�����ַ */
#define    LSM6DSL_FUNC_CFG_ACCESS              0x01   /* Ƕ��ʽ�������üĴ��� */

#define    LSM6DSL_SENSOR_SYNC_TIME_FRAME       0x04   /* ������ͬ�����üĴ��� */
#define    LSM6DSL_SENSOR_SYNC_RES_RATIO        0x05

#define    LSM6DSL_FIFO_CTRL1                   0x06   /* FIFO���üĴ��� */
#define    LSM6DSL_FIFO_CTRL2                   0x07
#define    LSM6DSL_FIFO_CTRL3                   0x08
#define    LSM6DSL_FIFO_CTRL4                   0x09
#define    LSM6DSL_FIFO_CTRL5                   0x0A

#define    LSM6DSL_DRDY_PULSE_CFG_G             0x0B
#define    LSM6DSL_RESERVED                     0x0C
#define    LSM6DSL_INT1_CTRL                    0x0D   /* INT1���ſ��� */
#define    LSM6DSL_INT2_CTRL                    0x0E   /* INT2���ſ��� */

#define    LSM6DSL_WHO_AM_I                     0x0F   /* ID */

#define    LSM6DSL_CTRL1_XL                     0x10   /* ���ٶȼƺ������ǿ��ƼĴ��� */
#define    LSM6DSL_CTRL2_G                      0x11
#define    LSM6DSL_CTRL3_C                      0x12
#define    LSM6DSL_CTRL4_C                      0x13
#define    LSM6DSL_CTRL5_C                      0x14
#define    LSM6DSL_CTRL6_C                      0x15
#define    LSM6DSL_CTRL7_G                      0x16
#define    LSM6DSL_CTRL8_XL                     0x17
#define    LSM6DSL_CTRL9_XL                     0x18
#define    LSM6DSL_CTRL10_C                     0x19

#define    LSM6DSL_MASTER_CONFIG                0x1A   /* I2C�����üĴ��� */

#define    LSM6DSL_WAKE_UP_SRC                  0x1B   /* �жϼĴ��� */
#define    LSM6DSL_TAP_SRC                      0x1C
#define    LSM6DSL_D6D_SRC                      0x1D

#define    LSM6DSL_STATUS_REG                   0x1E    /* ״̬���ݼĴ����û��ӿ� */

#define    LSM6DSL_OUT_TEMP_L                   0x20    /* �¶�������ݼĴ��� */
#define    LSM6DSL_OUT_TEMP_H                   0x21

#define    LSM6DSL_OUTX_L_G                     0x22    /* ����������Ĵ����û��ӿ� */
#define    LSM6DSL_OUTX_H_G                     0x23
#define    LSM6DSL_OUTY_L_G                     0x24
#define    LSM6DSL_OUTY_H_G                     0x25
#define    LSM6DSL_OUTZ_L_G                     0x26	
#define    LSM6DSL_OUTZ_H_G                     0x27

#define    LSM6DSL_OUTX_L_XL                    0x28    /* ���ٶȼ�����Ĵ��� */
#define    LSM6DSL_OUTX_H_XL                    0x29
#define    LSM6DSL_OUTY_L_XL                    0x2A
#define    LSM6DSL_OUTY_H_XL                    0x2B
#define    LSM6DSL_OUTZ_L_XL                    0x2C
#define    LSM6DSL_OUTZ_H_XL                    0x2D

#define    LSM6DSL_ENSORHUB1_REG                0x2E   /* ����������������Ĵ��� */
#define    LSM6DSL_ENSORHUB2_REG                0x2F
#define    LSM6DSL_ENSORHUB3_REG                0x30
#define    LSM6DSL_ENSORHUB4_REG                0x31
#define    LSM6DSL_ENSORHUB5_REG                0x32
#define    LSM6DSL_ENSORHUB6_REG                0x33
#define    LSM6DSL_ENSORHUB7_REG                0x34
#define    LSM6DSL_ENSORHUB8_REG                0x35
#define    LSM6DSL_ENSORHUB9_REG                0x36
#define    LSM6DSL_ENSORHUB10_REG               0x37
#define    LSM6DSL_ENSORHUB11_REG               0x38
#define    LSM6DSL_ENSORHUB12_REG               0x39

#define    LSM6DSL_FIFO_STATUS1                 0x3A   /* FIFO״̬�Ĵ��� */
#define    LSM6DSL_FIFO_STATUS2                 0x3B
#define    LSM6DSL_FIFO_STATUS3                 0x3C
#define    LSM6DSL_FIFO_STATUS4                 0x3D

#define    LSM6DSL_FIFO_DATA_OUT_L              0x3E   /* FIFO��������Ĵ��� */
#define    LSM6DSL_FIFO_DATA_OUT_H              0x3F

#define    LSM6DSL_TIMESTAMP0_REG               0x40   /* ʱ�������Ĵ��� */
#define    LSM6DSL_TIMESTAMP1_REG               0x41
#define    LSM6DSL_TIMESTAMP2_REG               0x42

#define    LSM6DSL_STEP_TIMESTAMP_L             0x49   /* ����������ʱ����Ĵ��� */
#define    LSM6DSL_STEP_TIMESTAMP_H             0x4A

#define    LSM6DSL_STEP_COUNTER_L               0x4B   /* ��������������Ĵ��� */
#define    LSM6DSL_STEP_COUNTER_H               0x4C

#define    LSM6DSL_SENSORHUB13_REG              0x4D   /* ����������������Ĵ��� */
#define    LSM6DSL_SENSORHUB14_REG              0x4E
#define    LSM6DSL_SENSORHUB15_REG              0x4F
#define    LSM6DSL_SENSORHUB16_REG              0x50
#define    LSM6DSL_SENSORHUB17_REG              0x51
#define    LSM6DSL_SENSORHUB18_REG              0x52

#define    LSM6DSL_FUNC_SRC1                    0x53   /* �жϼĴ��� */
#define    LSM6DSL_FUNC_SRC2                    0x54

#define    LSM6DSL_WRIST_TILT_IA                0x55   /* �жϼĴ��� */

#define    LSM6DSL_TAP_CFG                      0x58   /* �жϼĴ��� */
#define    LSM6DSL_TAP_THS_6D                   0x59
#define    LSM6DSL_INT_DUR2                     0x5A
#define    LSM6DSL_WAKE_UP_THS                  0x5B
#define    LSM6DSL_WAKE_UP_DUR                  0x5C
#define    LSM6DSL_FREE_FALL                    0x5D
#define    LSM6DSL_MD1_CFG                      0x5E
#define    LSM6DSL_MD2_CFG                      0x5F

#define    LSM6DSL_MASTER_CMD_CODE              0x60
#define    LSM6DSL_SENS_SYNC_SPI_ERROR_CODE     0x61

#define    LSM6DSL_OUT_MAG_RAW_X_L              0x66   /* �ⲿ������ԭʼ��������Ĵ��� */
#define    LSM6DSL_OUT_MAG_RAW_X_H              0x67
#define    LSM6DSL_OUT_MAG_RAW_Y_L              0x68
#define    LSM6DSL_OUT_MAG_RAW_Y_H              0x69
#define    LSM6DSL_OUT_MAG_RAW_Z_L              0x6A
#define    LSM6DSL_OUT_MAG_RAW_Z_H              0x6B

#define    LSM6DSL_X_OFS_USR                    0x73   /* ���ٶȼ��û�ƫ��У�� */
#define    LSM6DSL_Y_OFS_USR                    0x74
#define    LSM6DSL_Z_OFS_USR                    0x75


/**
 * \brief LSM6DSL �豸�ṹ��
 */
typedef struct am_lsm6dsl_dev {
    am_i2c_device_t i2c_dev;       /**< \brief LSM6DSL I2C�豸 */
} am_lsm6dsl_dev_t;

/** \brief LSM6DSL ����������� */
typedef am_lsm6dsl_dev_t *am_lsm6dsl_handle_t;	
	


/**
 * \brief LSM6DSL��������ȡ
 *
 * \param[in]  handle  : LSM6DSL����������
 * \param[out] p_accel : ָ�������ǵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_lsm6dsl_read_temp(am_lsm6dsl_handle_t handle, int16_t* p_temp);


/**
 * \brief LSM6DSL��������ȡ
 *
 * \param[in]  handle  : LSM6DSL����������
 * \param[out] p_accel : ָ�������ǵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_lsm6dsl_read_accel(am_lsm6dsl_handle_t handle, int16_t* p_accel);



/**
 * \brief LSM6DSL��������ʼ��
 *
 * \param[in] p_dev      : ָ��LSM6DSL�豸�ṹ���ָ��
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return LSM6DSL����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_lsm6dsl_handle_t am_lsm6dsl_init (am_lsm6dsl_dev_t *p_dev,
                         const am_lsm6dsl_devinfo_t *p_devinfo,  
																   am_i2c_handle_t i2c_handle);

/**
 * \brief LSM6DSL �豸���ʼ��
 * \param[in]  handle : LSM6DSL����������
 * \return ��
 */
void am_lsm6dsl_deinit(am_lsm6dsl_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LSM6DSL_H */

/* end of file */
