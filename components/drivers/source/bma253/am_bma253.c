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
 * \brief  BMA253������ٶȴ�����Ӧ�ýӿ�ʵ��
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-07  yrz, first implementation.
 * \endinternal
 */

#include "am_bma253.h"

/**
 * \brief BMA253�ӻ���ַ
 */    
#define BMA253_DEVICE_ADDR			             0x18 
    
/**
 * \brief BMA253 ID�Ĵ�����ַ
 */    
#define BMA2x2_CHIP_ID_ADDR			             0x00 
    
/**
 * \brief ���ݼĴ�����ַ
 */
#define BMA2x2_X_AXIS_LSB_ADDR                   0x02 
#define BMA2x2_X_AXIS_MSB_ADDR                   0x03 
#define BMA2x2_Y_AXIS_LSB_ADDR                   0x04 
#define BMA2x2_Y_AXIS_MSB_ADDR                   0x05 
#define BMA2x2_Z_AXIS_LSB_ADDR                   0x06 
#define BMA2x2_Z_AXIS_MSB_ADDR                   0x07 
#define BMA2x2_TEMP_ADDR			             0x08 

/**
 * \brief ״̬�Ĵ�����ַ
 */
#define BMA2x2_STAT1_ADDR			             0x09 
#define BMA2x2_STAT2_ADDR			             0x0A 
#define BMA2x2_STAT_TAP_SLOPE_ADDR		         0x0B 
#define BMA2x2_STAT_ORIENT_HIGH_ADDR		     0x0C 
#define BMA2x2_STAT_FIFO_ADDR			         0x0E 
#define BMA2x2_RANGE_SELECT_ADDR		         0x0F 
#define BMA2x2_BW_SELECT_ADDR                    0x10 
#define BMA2x2_MODE_CTRL_ADDR                    0x11 
#define BMA2x2_LOW_NOISE_CTRL_ADDR               0x12 
#define BMA2x2_DATA_CTRL_ADDR                    0x13 
#define BMA2x2_RST_ADDR                          0x14 

/**
 * \brief �жϼĴ�����ַ
 */
#define BMA2x2_INTR_ENABLE1_ADDR                 0x16 
#define BMA2x2_INTR_ENABLE2_ADDR                 0x17 
#define BMA2x2_INTR_SLOW_NO_MOTION_ADDR          0x18 
#define BMA2x2_INTR1_PAD_SELECT_ADDR             0x19 
#define BMA2x2_INTR_DATA_SELECT_ADDR             0x1A 
#define BMA2x2_INTR2_PAD_SELECT_ADDR             0x1B 
#define BMA2x2_INTR_SOURCE_ADDR                  0x1E 
#define BMA2x2_INTR_SET_ADDR                     0x20 
#define BMA2x2_INTR_CTRL_ADDR                    0x21 

/**
 * \brief ���ԼĴ�����ַ
 */
#define BMA2x2_LOW_DURN_ADDR                     0x22 
#define BMA2x2_LOW_THRES_ADDR                    0x23 
#define BMA2x2_LOW_HIGH_HYST_ADDR                0x24 
#define BMA2x2_HIGH_DURN_ADDR                    0x25 
#define BMA2x2_HIGH_THRES_ADDR                   0x26 
#define BMA2x2_SLOPE_DURN_ADDR                   0x27 
#define BMA2x2_SLOPE_THRES_ADDR                  0x28 
#define BMA2x2_SLOW_NO_MOTION_THRES_ADDR         0x29 
#define BMA2x2_TAP_PARAM_ADDR                    0x2A 
#define BMA2x2_TAP_THRES_ADDR                    0x2B 
#define BMA2x2_ORIENT_PARAM_ADDR                 0x2C 
#define BMA2x2_THETA_BLOCK_ADDR                  0x2D 
#define BMA2x2_THETA_FLAT_ADDR                   0x2E 
#define BMA2x2_FLAT_HOLD_TIME_ADDR               0x2F 

#define BMA2x2_SELFTEST_ADDR                     0x32 
#define BMA2x2_EEPROM_CTRL_ADDR                  0x33 
#define BMA2x2_SERIAL_CTRL_ADDR                  0x34 

/**
 * \brief ƫ��Ĵ�����ַ
 */
#define BMA2x2_OFFSET_CTRL_ADDR                  0x36 
#define BMA2x2_OFFSET_PARAMS_ADDR                0x37 
#define BMA2x2_OFFSET_X_AXIS_ADDR                0x38 
#define BMA2x2_OFFSET_Y_AXIS_ADDR                0x39 
#define BMA2x2_OFFSET_Z_AXIS_ADDR                0x3A 

/**
 * \brief GP�Ĵ�����ַ
 */
#define BMA2x2_GP0_ADDR                          0x3B 
#define BMA2x2_GP1_ADDR                          0x3C 

/**
 * \brief FIFO�Ĵ�����ַ
 */
#define BMA2x2_FIFO_MODE_ADDR                    0x3E 
#define BMA2x2_FIFO_DATA_OUTPUT_ADDR             0x3F 
#define BMA2x2_FIFO_WML_TRIG                     0x30 

/**
 * \brief �����λ
 */
#define SOFT_RESET                               0xB6  

/**
 * \brief ��ȡbma253������ٶȼĴ���
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
static int __bma253_read_accel_reg (am_bma253_handle_t handle, 
                                    uint8_t reg_addr,
                                    int16_t *p_accel)
{
    uint8_t accel_temp[2];
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    ret =  am_i2c_read(p_i2c_dev, reg_addr, accel_temp, 2);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* ���ݴ��� */
    if (accel_temp[0] & 0x01) {
        *p_accel = (int16_t)((accel_temp[1]) << 4 | accel_temp[0] >> 4);    
    }
    return ret;
}

/**
 * \brief ��ȡbma253������ٶ�
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
int am_bma253_read_accel (am_bma253_handle_t handle, int16_t* p_accel)
{
    int ret = AM_OK;

    /* ��ȡX����ٶ� */
    ret = __bma253_read_accel_reg(handle, BMA2x2_X_AXIS_LSB_ADDR, &p_accel[0]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  

    /* ��ȡY����ٶ� */    
    ret = __bma253_read_accel_reg(handle, BMA2x2_Y_AXIS_LSB_ADDR, &p_accel[1]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }

    /* ��ȡZ����ٶ� */    
    ret = __bma253_read_accel_reg(handle, BMA2x2_Z_AXIS_LSB_ADDR, &p_accel[2]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    return  ret;  
}

/**
 * \brief ��ȡbma253�¶�
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
int am_bma253_read_temp (am_bma253_handle_t handle, int8_t* p_temp)
{
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);  
    
    ret = am_i2c_read(p_i2c_dev,BMA2x2_TEMP_ADDR, (uint8_t*)(p_temp), 1);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* �����¶� */
    *p_temp = (int8_t)(24 + *p_temp * 1.0 / 2);
    
    return  ret;     
}

/**
 * \brief BMA253�����������λ
 */
int am_bma253_soft_reset (am_bma253_handle_t handle)
{
    int     ret;
    uint8_t soft_reset_cmd = SOFT_RESET;

    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* ��֤��������Ч�� */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* ��handle�л�ȡi2c�豸ָ�� */
    p_i2c_dev = &(handle->i2c_dev);

    /* д��λָ�� */
    ret = am_i2c_write(p_i2c_dev,
                       BMA2x2_RST_ADDR,             
                       &soft_reset_cmd,
                       1);

    /* ��Ҫ15ms�ﵽ����״̬ */
    am_mdelay(15);

    return ret;
}

/**
 * \brief BMA253��������ʼ��
 */
am_bma253_handle_t am_bma253_init (am_bma253_dev_t *p_dev, 
                                   const am_bma253_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle)
{
    uint8_t  bma253_id = 0; 
    /* ���ڼĴ������� */
    uint8_t  pmu_lpw   = 0x00;    
    
    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* ��ʼ���ú�BMA253�豸��Ϣ */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 BMA253_DEVICE_ADDR,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* ��ȡBMA253 ID */
    am_i2c_read(&(p_dev->i2c_dev), BMA2x2_CHIP_ID_ADDR, &bma253_id, 1);
    AM_DBG_INFO("bma253_ID = 0x%x\r\n", bma253_id);
    
    /* ���ü��ٶȷ�ΧΪ��2g */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BMA2x2_RANGE_SELECT_ADDR, 
                 &(p_devinfo->pmu_range), 
                 1);

    /* �����˲�����Ϊ7.81Hz */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BMA2x2_BW_SELECT_ADDR, 
                 &(p_devinfo->pmu_bw), 
                 1);
       
    /* ���õ�Դģʽ��˯������ */
    pmu_lpw = (p_devinfo->power_mode | p_devinfo->sleep_period);
    am_i2c_write(&(p_dev->i2c_dev), BMA2x2_MODE_CTRL_ADDR, &pmu_lpw, 1);    
        
    return p_dev;
}

/**
 * \brief BMA253 �豸���ʼ��
 */
void am_bma253_deinit (am_bma253_handle_t handle)
{

}

/* end of file */
