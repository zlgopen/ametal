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
 * \brief ������ BMA253 �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-19  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_bma253.h"
#include "am_gpio.h"
#include "am_vdebug.h"

/*******************************************************************************
 * �궨��
 ******************************************************************************/
#define  __BMA253_REG_BGW_CHIPID    0x00  /**< \brief ID�Ĵ�����ַ            */
#define  __BMA253_REG_ACCD_X_LSB    0x02  /**< \brief X����ֽ����ݼĴ�����ַ */
#define  __BMA253_REG_ACCD_X_MSB    0x03  /**< \brief X����ֽ����ݼĴ�����ַ */
#define  __BMA253_REG_ACCD_Y_LSB    0x04  /**< \brief Y����ֽ����ݼĴ�����ַ */
#define  __BMA253_REG_ACCD_Y_MSB    0x05  /**< \brief Y����ֽ����ݼĴ�����ַ */
#define  __BMA253_REG_ACCD_Z_LSB    0x06  /**< \brief Z����ֽ����ݼĴ�����ַ */
#define  __BMA253_REG_ACCD_Z_MSB    0x07  /**< \brief Z����ֽ����ݼĴ�����ַ */
#define  __BMA253_REG_ACCD_TEMP     0x08  /**< \brief �¶����ݼĴ�����ַ      */
#define  __BMA253_REG_INT_STATUS_0  0x09  /**< \brief �ж�״̬�Ĵ���0��ַ     */
#define  __BMA253_REG_INT_STATUS_1  0x0A  /**< \brief �ж�״̬�Ĵ���1��ַ     */ 
#define  __BMA253_REG_INT_STATUS_2  0x0B  /**< \brief �ж�״̬�Ĵ���2��ַ     */ 
#define  __BMA253_REG_INT_STATUS_3  0x0C  /**< \brief �ж�״̬�Ĵ���3��ַ     */ 
#define  __BMA253_REG_FIFO_STATUS   0x0E  /**< \brief FIFO״̬�Ĵ�����ַ      */ 
#define  __BMA253_REG_PMU_RANGE     0x0F  /**< \brief ���ٶȷ�Χ�Ĵ�����ַ    */ 
#define  __BMA253_REG_PMU_BW        0x10  /**< \brief �����˲�����Ĵ�����ַ  */
#define  __BMA253_REG_PMU_LPW       0x11  /**< \brief �͹������üĴ�����ַ    */ 
#define  __BMA253_REG_PMU_LOW_POWER 0x12  /**< \brief �͹���ģʽ�Ĵ�����ַ    */ 
#define  __BMA253_REG_ACCD_HBW      0x13  /**< \brief ���������ʽ�Ĵ�����ַ  */ 
#define  __BMA253_REG_BGW_SOFTRESET 0x14  /**< \brief �����λ�Ĵ�����ַ      */
#define  __BMA253_REG_INT_EN_0      0x16  /**< \brief �ж�ʹ�ܼĴ���0��ַ     */
#define  __BMA253_REG_INT_EN_1      0x17  /**< \brief �ж�ʹ�ܼĴ���1��ַ     */
#define  __BMA253_REG_INT_EN_2      0x18  /**< \brief �ж�ʹ�ܼĴ���2��ַ     */
#define  __BMA253_REG_INT_MAP_0     0x19  /**< \brief �ж��ź����üĴ���0��ַ */
#define  __BMA253_REG_INT_MAP_1     0x1A  /**< \brief �ж��ź����üĴ���1��ַ */
#define  __BMA253_REG_INT_MAP_2     0x1B  /**< \brief �ж��ź����üĴ���2��ַ */
#define  __BMA253_REG_INT_SRC       0x1E  /**< \brief �ж�Դ�Ĵ�����ַ        */
#define  __BMA253_REG_INT_OUT_CTRL  0x20  /**< \brief �ж�������ƼĴ�����ַ  */
#define  __BMA253_REG_INT_RST_LATCH 0x21  /**< \brief �жϸ�λ�Ĵ�����ַ      */
#define  __BMA253_REG_INT_0         0x22  /**< \brief �ж�0�Ĵ�����ַ         */
#define  __BMA253_REG_INT_1         0x23  /**< \brief �ж�1�Ĵ�����ַ         */
#define  __BMA253_REG_INT_2         0x24  /**< \brief �ж�2�Ĵ�����ַ         */
#define  __BMA253_REG_INT_3         0x25  /**< \brief �ж�3�Ĵ�����ַ         */
#define  __BMA253_REG_INT_4         0x26  /**< \brief �ж�4�Ĵ�����ַ         */
#define  __BMA253_REG_INT_5         0x27  /**< \brief �ж�5�Ĵ�����ַ         */

#define  __BMA253_REG_INT_6         0x28  /**< \brief �ж�6�Ĵ�����ַ         */
#define  __BMA253_REG_INT_7         0x29  /**< \brief �ж�7�Ĵ�����ַ         */
#define  __BMA253_REG_INT_8         0x2A  /**< \brief �ж�8�Ĵ�����ַ         */
#define  __BMA253_REG_INT_9         0x2B  /**< \brief �ж�9�Ĵ�����ַ         */
#define  __BMA253_REG_INT_A         0x2C  /**< \brief �ж�A�Ĵ�����ַ         */
#define  __BMA253_REG_INT_B         0x2D  /**< \brief �ж�B�Ĵ�����ַ         */
#define  __BMA253_REG_INT_C         0x2E  /**< \brief �ж�C�Ĵ�����ַ         */
#define  __BMA253_REG_INT_D         0x2F  /**< \brief �ж�D�Ĵ�����ַ         */
#define  __BMA253_REG_FIFO_CONFIG_0 0x30  /**< \brief FIFO���üĴ���0��ַ     */
#define  __BMA253_REG_PMU_SELF_TEST 0x32  /**< \brief �Լ���ԼĴ�����ַ      */
#define  __BMA253_REG_TRIM_NVM_CTRL 0x33  /**< \brief �ڴ���ƼĴ�����ַ      */
#define  __BMA253_REG_BGW_SPI3_WDT  0x34  /**< \brief ���ֽӿڼĴ�����ַ      */
#define  __BMA253_REG_OFC_CTRL      0x36  /**< \brief OFC���ƼĴ�����ַ       */
#define  __BMA253_REG_OFC_SETTING   0x37  /**< \brief OFC���üĴ�����ַ       */
#define  __BMA253_REG_OFC_OFFSET_X  0x38  /**< \brief X��ƫ��Ĵ�����ַ       */
#define  __BMA253_REG_OFC_OFFSET_Y  0x39  /**< \brief Y��ƫ��Ĵ�����ַ       */
#define  __BMA253_REG_OFC_OFFSET_Z  0x3A  /**< \brief Z��ƫ��Ĵ�����ַ       */
#define  __BMA253_REG_TRIM_GP0      0x3B  /**< \brief ����ͨ�üĴ���0��ַ     */
#define  __BMA253_REG_TRIM_GP1      0x3C  /**< \brief ����ͨ�üĴ���1��ַ     */
#define  __BMA253_REG_FIFO_CONFIG_1 0x3E  /**< \brief FIFO���üĴ���1��ַ     */
#define  __BMA253_REG_FIFO_DATA     0x3F  /**< \brief FIFO���ݼĴ�����ַ      */

#define  __BMA253_MY_ID             0XFA  /**< \brief IDֵ                    */

/** \brief ���ò�����Χ */
#define  __BMA253_RANGE_SET_2G      0X03  /**< \brief ���ò�����ΧΪ��2G      */
#define  __BMA253_RANGE_SET_4G      0X05  /**< \brief ���ò�����ΧΪ��4G      */
#define  __BMA253_RANGE_SET_8G      0X08  /**< \brief ���ò�����ΧΪ��8G      */
#define  __BMA253_RANGE_SET_16G     0X0C  /**< \brief ���ò�����ΧΪ��16G     */

/** \brief �жϴ������� */
#define  __BMA253_TRIGGER_READ  (1 << 4)  /**< \brief ʹ�����ݿɶ�����        */
#define  __BMA253_TRIGGER_CLOSE (0 << 4)  /**< \brief �������ݿɶ�����        */
#define  __BMA253_EN_INT1_PIN   (1 << 0)  /**< \brief ʹ���ж�1����           */
#define  __BMA253_INT_CTRL      (1 << 0)  /**< \brief ����Ϊ����������ߵ�ƽ  */
#define  __BMA253_INT_CLEAR     (1 << 7)  /**< \brief ����ж�                */

/** \brief ������int8ת��Ϊһ��int16_t���� */
#define  __BMA253_UINT8_TO_UINT16(buff) \
                       (int16_t)(((int8_t)(buff[1]) << 4) \
                               | ((int8_t)(buff[0]) >> 4))
/** \brief �����¶�,������10^6�� */
#define  __BMA253_GET_TEMP_VALUE(data) ((int32_t)((23 + \
                                          ((int8_t)(data[0]) >> 1)) * 1000000))

/** \brief ������ٶ�,������10^6�� */
/** \brief ������ΧΪ��2G */
#define  __BMA253_GET_ACCEL_VALUE_2G(data)  ((int32_t)(data * 9800))
/** \brief ������ΧΪ��4G */
#define  __BMA253_GET_ACCEL_VALUE_4G(data)  ((int32_t)(data * 19500))
/** \brief ������ΧΪ��8G */
#define  __BMA253_GET_ACCEL_VALUE_8G(data)  ((int32_t)(data * 39100))
/** \brief ������ΧΪ��16G */
#define  __BMA253_GET_ACCEL_VALUE_16G(data) ((int32_t)(data * 78100))

/*******************************************************************************
 * ���غ�������
 ******************************************************************************/
/** \brief ��ȡ�ô�����ĳһͨ�������� */
am_local am_err_t __pfn_type_get (void *p_drv, int id);

/** \brief ��ȡ������ͨ���������� */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf);

/** \brief ʹ�ܴ�����ͨ�� */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result);

/** \brief ���ܴ�����ͨ�� */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result);

/** \brief ���ô�����ͨ������ */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val);

/** \brief ��ȡ������ͨ������ */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val);

/** \brief ���ô�����һ��ͨ����������һ�������ص����� */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg);

/** \brief �򿪴��� */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id);

/** \brief �رմ��� */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id);
                                  
/** \brief ��������׼���� */
am_local am_const struct am_sensor_drv_funcs __g_sensor_bma253_funcs = {
        __pfn_type_get,
        __pfn_data_get,
        __pfn_enable,
        __pfn_disable,
        __pfn_attr_set,
        __pfn_attr_get,
        __pfn_trigger_cfg,
        __pfn_trigger_on,
        __pfn_trigger_off
};
/*******************************************************************************
  Local functions
*******************************************************************************/
/**
 * \brief BMA253 д����
 */
am_local am_err_t __bma253_write (am_sensor_bma253_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief BMA253 ������
 */
am_local am_err_t __bma253_read (am_sensor_bma253_dev_t *p_this,
                                 uint32_t                subaddr,
                                 uint8_t                *p_buf,
                                 uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief �������̽����ת���ɼ��ٶ�ʵ��ֵ
 */
am_local int32_t __bma253_get_accel_value (am_sensor_bma253_dev_t* p_this,
                                           int32_t                data)
{
    int32_t real_data = 0;
    
    if (p_this == NULL) {
        return NULL;
    }
    switch (p_this->sam_range.val) {
    case 2:
        real_data = __BMA253_GET_ACCEL_VALUE_2G(data);
        break;
    case 4:
        real_data = __BMA253_GET_ACCEL_VALUE_4G(data);
        break;
    case 8:
        real_data = __BMA253_GET_ACCEL_VALUE_8G(data);
        break;
    case 16:
        real_data = __BMA253_GET_ACCEL_VALUE_16G(data);
        break;
    default:
        real_data = -AM_EINVAL;
        break;
    }
    return real_data;
}

/** \breif �жϻص����� */
am_local void __bma253_alarm_callback (void *p_arg)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief ����ж��ӳٴ������� */
}

/** \breif �ж��ӳٺ��� */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_arg;

    uint8_t reg_data[2];
    int32_t tem_data   = 0;
    uint8_t i = 0;
    
    /** \brief ��ȡX����ٶ� */
    __bma253_read(p_this, __BMA253_REG_ACCD_X_LSB, reg_data, 2);
    
    tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
    
    /** \brief X����ٶ� */
    p_this->data[0].val = __bma253_get_accel_value(p_this,tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

    /** \brief ��ȡY����ٶ� */
    __bma253_read(p_this, __BMA253_REG_ACCD_Y_LSB, reg_data, 2);

    tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
   
    /** \brief Y����ٶ� */
    p_this->data[1].val = __bma253_get_accel_value(p_this,tem_data);
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

    /** \brief ��ȡZ����ٶ� */
    __bma253_read(p_this, __BMA253_REG_ACCD_Z_LSB, reg_data, 2);
    
    tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
   
    /** \brief Z����ٶ� */
    p_this->data[2].val = __bma253_get_accel_value(p_this,tem_data); 
    p_this->data[2].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

    /** \brief ��ȡ�¶� */
    __bma253_read(p_this, __BMA253_REG_ACCD_TEMP, reg_data, 1);

    /** \brief �¶� */
    p_this->data[3].val = __BMA253_GET_TEMP_VALUE(reg_data); 
    p_this->data[3].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/    
    
    for (i = 0; i < 4; i++) {
        if (p_this->pfn_trigger_cb[i] &&
                (p_this->flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->pfn_trigger_cb[i](p_this->p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);
        }
    }
}

/** \brief ��ȡ�ô�����ĳһͨ�������� */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_BMA253_CHAN_1 || id == AM_BMA253_CHAN_2 ||
        id == AM_BMA253_CHAN_3) {
        return AM_SENSOR_TYPE_ACCELEROMETER;
    } else if (id == AM_BMA253_CHAN_4) {
        return AM_SENSOR_TYPE_TEMPERATURE;
    } else {
        return -AM_ENODEV;
    }
}

/** \brief ��ȡ������ͨ���������� */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;
    
    am_err_t ret        = AM_OK;
    uint8_t reg_data[2] = {0};
    int32_t tem_data    = 0;

    int cur_id = 0;
    int i = 0;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (AM_BIT_GET(p_this->trigger, 15) != 1) {
        return -AM_EPERM;
    }

    for (i = 0; i < num; i++) {
        p_buf[i].unit = AM_SENSOR_UNIT_INVALID;
    }

    /* ��Ϊ1�������������׼�����������ص�������ʹ�� */
    if (num == 1) {
        cur_id = p_ids[0];
        /* ��������׼������������ʽ����ֱ�Ӹ�ֵ */
        if ((AM_BIT_GET(p_this->trigger, 4)) &&
                ((p_this->flags[0] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->flags[1] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->flags[2] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->flags[3] & AM_SENSOR_TRIGGER_DATA_READY))) {
            p_buf[0].val = p_this->data[cur_id].val;
            p_buf[0].unit= p_this->data[cur_id].unit;
            return AM_OK;
        }
    } 
    
    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            do {
                /** \brief ��ȡX����ٶ� */
                ret = __bma253_read(p_this, __BMA253_REG_ACCD_X_LSB, reg_data, 2);
                if (ret != AM_OK) {
                    return ret;
                }
            } while ((reg_data[0] & 0x01) != 0x01);
            
            tem_data = __BMA253_UINT8_TO_UINT16(reg_data);

            /** \brief X����ٶ� */
            p_buf[i].val = __bma253_get_accel_value(p_this,tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 1) {

            do {
                /** \brief ��ȡY����ٶ� */
                ret = __bma253_read(p_this, __BMA253_REG_ACCD_Y_LSB, reg_data, 2);
                if (ret != AM_OK) {
                    return ret;
                }
            } while ((reg_data[0] & 0x01) != 0x01);
            
            tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
           
            /** \brief Y����ٶ� */
            p_buf[i].val = __bma253_get_accel_value(p_this,tem_data);
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 2) {
            
            do {
                /** \brief ��ȡZ����ٶ� */
                ret = __bma253_read(p_this, __BMA253_REG_ACCD_Z_LSB, reg_data, 2);
                if (ret != AM_OK) {
                    return ret;
                }
            } while ((reg_data[0] & 0x01) != 0x01);
            
            tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
         
            /** \brief Z����ٶ� */
            p_buf[i].val = __bma253_get_accel_value(p_this,tem_data);
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 3) {

            /** \brief ��ȡ�¶� */
            ret = __bma253_read(p_this, __BMA253_REG_ACCD_TEMP, reg_data, 1);
            if (ret != AM_OK) {
                return ret;
            }
            
            /** \brief �¶� */
            p_buf[i].val = __BMA253_GET_TEMP_VALUE(reg_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else {
            return -AM_ENODEV;  /*< \brief ���˴�ͨ�������ڸô�������ֱ�ӷ��� */
        }
    }
    
    return ret;
}

/** \brief ʹ�ܴ�����ͨ�� */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t pmu_bw = 0x08;
    uint8_t pmu_lpw = (0x05 << 1);
    
    am_err_t ret = -AM_ENODEV;
    am_err_t curent_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || cur_id == 3) {
            if (p_result != NULL) {
                p_result[i].val = AM_OK;
            }
            
            /* ����ͨ��ʹ�ܱ�־λ */
            AM_BIT_SET(p_this->trigger, 14 - cur_id);
            ret = AM_OK;
            
        } else {
            if (p_result != NULL) {
                p_result[i].val = -AM_ENODEV;
            }
            curent_ret = -AM_ENODEV;
        }
    }

    if (ret != AM_OK) {    /**< \breif �������û�иô�������ͨ�����룬���˳� */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 15);
    }  

    if (p_this->sam_range.val == 0) {
        /* Ĭ������Ϊ��2G */
        p_this->sam_range.val  = 2;    
        p_this->sam_range.unit = AM_SENSOR_UNIT_BASE;
    }

    /* �����˲����� */
    ret = __bma253_write(p_this, __BMA253_REG_PMU_BW, &pmu_bw, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    
    /* ���õ�Դģʽ��˯������ */
    ret = __bma253_write(p_this, __BMA253_REG_PMU_LPW, &pmu_lpw, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    return curent_ret;
}

/** \brief ���ܴ�����ͨ�� */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 ||cur_id == 3) {

            /* ���ʹ�ܱ�־λ */
            AM_BIT_CLR(p_this->trigger, 14 - cur_id);
            ret = AM_OK;

        } else {
            ret = -AM_ENODEV;
        }

        if (p_result != NULL) {
            p_result[i].val = ret;
        }
    }

    if ((AM_BIT_GET(p_this->trigger, 14) == 0)
            && (AM_BIT_GET(p_this->trigger, 13) == 0)
            && (AM_BIT_GET(p_this->trigger, 12) == 0)
            && (AM_BIT_GET(p_this->trigger, 11) == 0)) {

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 15);
        }
    }

    return cur_ret;
}

/*
 * \brief ����ѡ������
 */
am_local am_err_t  __reg_attr_range_set (am_sensor_bma253_dev_t *p_this, 
                                         uint8_t                 range)
{
     uint8_t range_cfg = 0;

     switch (range) {
     case 0:
         range_cfg = __BMA253_RANGE_SET_2G;
         break;
     case 1:
         range_cfg = __BMA253_RANGE_SET_4G;
         break;
     case 2:
         range_cfg = __BMA253_RANGE_SET_8G;
         break;
     case 3:
         range_cfg = __BMA253_RANGE_SET_16G;
         break;
     default:
         break;
     }
     return __bma253_write(p_this, __BMA253_REG_PMU_RANGE, &range_cfg, 1);
}

/** \brief ���ô�����ͨ������ */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    am_sensor_val_t sensor_val = {0, 0};

    am_sensor_val_t __sampling_range = {0, 0};

    uint8_t range = 0;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if ((id != AM_BMA253_CHAN_1) && (id != AM_BMA253_CHAN_2) &&
        (id != AM_BMA253_CHAN_3)) {
        return -AM_ENODEV;
    }

    switch (attr) {

    /* �޸����� */
    case AM_SENSOR_ATTR_FULL_SCALE:

        sensor_val.unit = p_val->unit;
        sensor_val.val  = p_val->val;    
    
        if (sensor_val.val < 2 || sensor_val.val > 16) {
            return -AM_ENOTSUP;
        }

        am_sensor_val_unit_convert(&sensor_val, 1, AM_SENSOR_UNIT_BASE);

        if (sensor_val.val == 2) {
            range = 0;
            __sampling_range.val = 2;
        } else if (sensor_val.val == 4) {
            range = 1;
            __sampling_range.val = 4;
        } else if (sensor_val.val == 8) {
            range = 2;
            __sampling_range.val = 8;
        } else if (sensor_val.val == 16) {
            range = 3;
            __sampling_range.val = 16;
        }

        ret = __reg_attr_range_set(p_this, range);
        if (ret != AM_OK) {
            cur_ret = ret;
        } else {
            p_this->sam_range.val  = __sampling_range.val;
            p_this->sam_range.unit = __sampling_range.unit;
        }

        break;

    default:
        cur_ret = -AM_ENOTSUP;
        break;
    }

    return cur_ret;
}

/** \brief ��ȡ������ͨ������ */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;

    am_err_t ret = AM_OK;

    if ((id != AM_BMA253_CHAN_1) && (id != AM_BMA253_CHAN_2)&&
        (id != AM_BMA253_CHAN_3) && (id != AM_BMA253_CHAN_4)) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (attr) {

    /* ��ȡ���� */
    case AM_SENSOR_ATTR_FULL_SCALE:

        p_val->val  = p_this->sam_range.val;
        p_val->unit = p_this->sam_range.unit;
        break;

    default:
        ret = -AM_ENOTSUP;
        break;

    }

    return ret;
}

/** \brief ���ô�����һ��ͨ����������һ�������ص����� */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;

    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (flags) {
    case AM_SENSOR_TRIGGER_DATA_READY:
        break;
    default:
        return -AM_ENOTSUP;
    }

    p_this->pfn_trigger_cb[id] = pfn_cb;
    p_this->p_arg[id]          = p_arg;
    p_this->flags[id]          = flags;

    return AM_OK;
}

/** \brief �򿪴��� */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;

    am_err_t cur_ret = AM_OK;
    am_err_t ret     = AM_OK;
    uint8_t reg_data = 0;

    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x10) {
        return AM_OK;
    }

    /** \brief ʹ�����ݿɶ����� */
    reg_data = __BMA253_TRIGGER_READ;
    ret = __bma253_write(p_this, __BMA253_REG_INT_EN_1, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief ʹ�ܴ������� */
    reg_data = __BMA253_EN_INT1_PIN;
    ret = __bma253_write(p_this, __BMA253_REG_INT_MAP_1, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief �����ж�����ģʽ */
    reg_data = __BMA253_INT_CTRL;
    ret = __bma253_write(p_this, __BMA253_REG_INT_OUT_CTRL, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }    
    
    if (p_this->dev_info->trigger_pin != -1) {
        /* ����жϱ�־ */
        reg_data = __BMA253_INT_CLEAR;
        __bma253_write(p_this, __BMA253_REG_INT_RST_LATCH, &reg_data, 1);
        
        /* �򿪴������� */
        ret = am_gpio_trigger_on(p_this->dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }
    
    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->trigger, 0);
        AM_BIT_SET(p_this->trigger, 4);
    }
    if (cur_ret == AM_OK && id == 1) {
        AM_BIT_SET(p_this->trigger, 1);
        AM_BIT_SET(p_this->trigger, 4);
    }
    if (cur_ret == AM_OK && id == 2) {
        AM_BIT_SET(p_this->trigger, 2);
        AM_BIT_SET(p_this->trigger, 4);
    }
    if (cur_ret == AM_OK && id == 3) {
        AM_BIT_SET(p_this->trigger, 3);
        AM_BIT_SET(p_this->trigger, 4);
    }
    
    return cur_ret;
}

/** \brief �رմ��� */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_drv;

    am_err_t cur_ret = AM_OK;
    am_err_t ret     = AM_OK;
    uint8_t reg_data = 0;
    
    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_this->pfn_trigger_cb[id] = NULL;
    p_this->p_arg[id]          = NULL;
    p_this->flags[id]          = 0;

    AM_BIT_CLR(p_this->trigger, id);

    if ((p_this->trigger & 0xF) == 0) {

        /** \brief �������ݿɶ����� */
        reg_data = __BMA253_TRIGGER_CLOSE;
        ret = __bma253_write(p_this, __BMA253_REG_INT_EN_1, &reg_data, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }

        if ((p_this->dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 4);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief ������ BMA253 ��ʼ��
 */
am_sensor_handle_t am_sensor_bma253_init (
        am_sensor_bma253_dev_t           *p_dev,
        const am_sensor_bma253_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t bma253_id = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->bma253_dev.p_drv   = p_dev;
    p_dev->bma253_dev.p_funcs = &__g_sensor_bma253_funcs;
    p_dev->dev_info            = p_devinfo;
    p_dev->pfn_trigger_cb[0]   = NULL;
    p_dev->pfn_trigger_cb[1]   = NULL;
    p_dev->pfn_trigger_cb[2]   = NULL;
    p_dev->pfn_trigger_cb[3]   = NULL;
    p_dev->p_arg[0]            = NULL;
    p_dev->p_arg[1]            = NULL;
    p_dev->p_arg[2]            = NULL;
    p_dev->p_arg[3]            = NULL;
    p_dev->flags[0]            = 0;
    p_dev->flags[1]            = 0;
    p_dev->flags[2]            = 0;
    p_dev->flags[3]            = 0; 
    p_dev->trigger             = 0;
    p_dev->data[0].val         = 0;
    p_dev->data[0].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->data[1].val         = 0;
    p_dev->data[1].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->data[2].val         = 0;
    p_dev->data[2].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->data[3].val         = 0;
    p_dev->data[3].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->sam_range.val       = 0;
    p_dev->sam_range.unit      = AM_SENSOR_UNIT_INVALID;   
    
    /* �ⲿ�ж����� */
    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __bma253_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_RISE);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);
   
    /* ��ȡID */
    ret = __bma253_read(p_dev, __BMA253_REG_BGW_CHIPID, &bma253_id, 1);
    if (ret != AM_OK || bma253_id != __BMA253_MY_ID) {
        cur_ret = ret;
    }

    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor BMA253 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->bma253_dev);
}

/**
 * \brief ������ BMA253 ȥ��ʼ��
 */
am_err_t am_sensor_bma253_deinit (am_sensor_handle_t handle)
{
    am_sensor_bma253_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->bma253_dev.p_drv   = NULL;
    p_dev->bma253_dev.p_funcs = NULL;
    p_dev->dev_info           = NULL;

    return AM_OK;
}


/* end of file */
