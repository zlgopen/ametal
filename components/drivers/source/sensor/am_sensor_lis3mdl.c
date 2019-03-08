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
 * \brief ������ LIS3MDL �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_lis3mdl.h"
#include "am_gpio.h"
#include "am_vdebug.h"

/*******************************************************************************
 * �궨��
 ******************************************************************************/
#define  __LIS3MDL_REG_ID           0x0F/**< \brief ID�Ĵ�����ַ              */
#define  __LIS3MDL_REG_CTRL1        0x20/**< \brief ���ƼĴ���1��ַ           */
#define  __LIS3MDL_REG_CTRL2        0x21/**< \brief ���ƼĴ���2��ַ           */
#define  __LIS3MDL_REG_CTRL3        0x22/**< \brief ���ƼĴ���3��ַ           */
#define  __LIS3MDL_REG_CTRL4        0x23/**< \brief ���ƼĴ���4��ַ           */
#define  __LIS3MDL_REG_CTRL5        0x24/**< \brief ���ƼĴ���5��ַ           */
#define  __LIS3MDL_REG_STATUS       0x27/**< \brief ״̬�Ĵ�����ַ            */
#define  __LIS3MDL_REG_OUT_X_L      0x28/**< \brief X����ֽ����ݼĴ�����ַ   */
#define  __LIS3MDL_REG_OUT_X_H      0x29/**< \brief X����ֽ����ݼĴ�����ַ   */
#define  __LIS3MDL_REG_OUT_Y_L      0x2A/**< \brief Y����ֽ����ݼĴ�����ַ   */
#define  __LIS3MDL_REG_OUT_Y_H      0x2B/**< \brief Y����ֽ����ݼĴ�����ַ   */
#define  __LIS3MDL_REG_OUT_Z_L      0x2C/**< \brief Z����ֽ����ݼĴ�����ַ   */
#define  __LIS3MDL_REG_OUT_Z_H      0x2D/**< \brief Z����ֽ����ݼĴ�����ַ   */
#define  __LIS3MDL_REG_TEMP_OUT_L   0x2E/**< \brief �¶ȵ��ֽ����ݼĴ�����ַ  */
#define  __LIS3MDL_REG_TEMP_OUT_H   0x2F/**< \brief �¶����ݼĴ�����ַ        */
#define  __LIS3MDL_REG_INT_CFG      0x30/**< \brief �ж����üĴ�����ַ        */
#define  __LIS3MDL_REG_INT_SRC      0x31/**< \brief �ж�Դ�Ĵ�����ַ          */
#define  __LIS3MDL_REG_INT_THS_L    0x32/**< \brief �ж���ֵ���ֽڼĴ�����ַ  */
#define  __LIS3MDL_REG_INT_THS_H    0x33/**< \brief �ж���ֵ���ֽڼĴ�����ַ  */

#define  __LIS3MDL_MY_ID            0X3D  /**< \brief IDֵ                    */

/* ʹ���¶ȴ����� */
#define  __LIS3MDL_TEMP_START       0X80

/* ʹ�������ж� */
#define  __LIS3MDL_EN_DRDY          0x03
/* ���������ж� */
#define  __LIS3MDL_EN_CLOSE         (~0x01)

/** \brief ���ò�����Χ */
#define  __LIS3MDL_RANGE_SET_4G     0X00  /**< \brief ���ò�����ΧΪ��4G      */
#define  __LIS3MDL_RANGE_SET_8G     0X20  /**< \brief ���ò�����ΧΪ��8G      */
#define  __LIS3MDL_RANGE_SET_12G    0X40  /**< \brief ���ò�����ΧΪ��12G     */
#define  __LIS3MDL_RANGE_SET_16G    0X60  /**< \brief ���ò�����ΧΪ��16G     */

/** \brief ������int8ת��Ϊһ��int16_t���� */
#define __LIS3MDL_UINT8_TO_UINT16(buff) \
                       (int16_t)(((int8_t)(buff[1]) << 8) \
                               | ((int8_t)(buff[0])))
/** \brief �����¶�,������10^6�� */
#define __LIS3MDL_GET_TEMP_VALUE(data) \
            ((int32_t)((25 + ((int8_t)(data) >> 3)) * 1000000))

/** \brief �������,������10^6�� */
#define __LIS3MDL_GET_MAG_VALUE_4G(data)  ((int32_t)(data * 10000 / 6842) * 100)
#define __LIS3MDL_GET_MAG_VALUE_8G(data)  ((int32_t)(data * 10000 / 3421) * 100)
#define __LIS3MDL_GET_MAG_VALUE_12G(data) ((int32_t)(data * 10000 / 2281) * 100)
#define __LIS3MDL_GET_MAG_VALUE_16G(data) ((int32_t)(data * 10000 / 1711) * 100)
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
am_local am_const struct am_sensor_drv_funcs __g_sensor_lis3mdl_funcs = {
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
 * \brief LIS3MDL д����
 */
am_local am_err_t __lis3mdl_write (am_sensor_lis3mdl_dev_t *p_this,
                                   uint32_t                subaddr,
                                   uint8_t                *p_buf,
                                   uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief LIS3MDL ������
 */
am_local am_err_t __lis3mdl_read (am_sensor_lis3mdl_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief �������̽����ת���ɴ���ʵ��ֵ
 */
am_local int32_t __lis3mdl_get_accel_value (am_sensor_lis3mdl_dev_t* p_this,
                                            int32_t                  data)
{
    int32_t real_data = 0;
    
    if (p_this == NULL) {
        return NULL;
    }
    
    switch (p_this->sam_range.val) {
    case 4:
        real_data = __LIS3MDL_GET_MAG_VALUE_4G(data);
        break;
    case 8:
        real_data = __LIS3MDL_GET_MAG_VALUE_8G(data);
        break;
    case 12:
        real_data = __LIS3MDL_GET_MAG_VALUE_12G(data);
        break;
    case 16:
        real_data = __LIS3MDL_GET_MAG_VALUE_16G(data);
        break;
    default:
        real_data = -AM_EINVAL;
        break;
    }
    return real_data;
}

/** \breif �жϻص����� */
am_local void __lis3mdl_alarm_callback (void *p_arg)
{
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief ����ж��ӳٴ������� */
}

/** \breif �ж��ӳٺ��� */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_arg;

    uint8_t reg_data[2];
    int32_t tem_data = 0;
    uint8_t i = 0;
    uint8_t int_cfg = 0;

    /** \brief ���ܴ������� */
    __lis3mdl_read(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);

    int_cfg &= __LIS3MDL_EN_CLOSE;
    __lis3mdl_write(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);

    /** \brief ��ȡX����� */
    __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_X_L, reg_data, 2);
    
    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);

    /** \brief X����� */
    p_this->data[0].val = __lis3mdl_get_accel_value(p_this, tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ0:10^(-6) */

    /** \brief ��ȡY����� */
    __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Y_L, reg_data, 2);

    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Y����� */
    p_this->data[1].val = __lis3mdl_get_accel_value(p_this, tem_data); 
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ0:10^(-6) */

    /** \brief ��ȡZ����� */
    __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Z_L, reg_data, 2);
    
    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Z����� */
    p_this->data[2].val = __lis3mdl_get_accel_value(p_this, tem_data); 
    p_this->data[2].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ0:10^(-6) */

    /** \brief ��ȡ�¶� */
    __lis3mdl_read(p_this, __LIS3MDL_REG_TEMP_OUT_L, reg_data, 2);
    
    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);

    /** \brief �¶� */
    p_this->data[3].val = __LIS3MDL_GET_TEMP_VALUE(tem_data); 
    p_this->data[3].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/    

    for (i = 0; i < 4; i++) {
        if (p_this->pfn_trigger_cb[i] &&
                (p_this->flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->pfn_trigger_cb[i](p_this->p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);        
        } 
    }

    /** \brief ʹ���жϴ��� */
    __lis3mdl_read(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);

    int_cfg |= __LIS3MDL_EN_DRDY;
    __lis3mdl_write(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);   
}

/** \brief ��ȡ�ô�����ĳһͨ�������� */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_LIS3MDL_CHAN_1 || id == AM_LIS3MDL_CHAN_2 ||
        id == AM_LIS3MDL_CHAN_3) {
        return AM_SENSOR_TYPE_MAGNETIC;
    } else if (id == AM_LIS3MDL_CHAN_4) {
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
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;
    
    am_err_t ret        = AM_OK;
    uint8_t reg_data[2] = {0};
    int32_t tem_data   = 0;

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

            /** \brief ��ȡX����� */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_X_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
           
            /** \brief X����� */
            p_buf[i].val = __lis3mdl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 1) {

            /** \brief ��ȡY����� */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Y_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Y����� */
            p_buf[i].val = __lis3mdl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 2) {

            /** \brief ��ȡZ����� */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Z_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Z����� */
            p_buf[i].val = __lis3mdl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 3) {

            /** \brief ��ȡ�¶� */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_TEMP_OUT_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }

            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);

            /** \brief �¶� */
            p_buf[i].val = __LIS3MDL_GET_TEMP_VALUE(tem_data);
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
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t ctrl_reg1 = 0;
    uint8_t ctrl_reg3 = 0;
    
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
    
    /** \brief ����ת��ģʽ */
    ctrl_reg3 = 0x00;
    ret = __lis3mdl_write(p_this, __LIS3MDL_REG_CTRL3, &ctrl_reg3, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    
    if (AM_BIT_GET(p_this->trigger, 11) == 1) {
        
        /** \brief ʹ���¶�ͨ�� */
        ret = __lis3mdl_read(p_this, __LIS3MDL_REG_CTRL1, &ctrl_reg1, 1);
        if (ret != AM_OK) {
            curent_ret = ret;
        }

        ctrl_reg1 |= __LIS3MDL_TEMP_START;
        ret = __lis3mdl_write(p_this, __LIS3MDL_REG_CTRL1, &ctrl_reg1, 1);
        if (ret != AM_OK) {
            curent_ret = ret;
        }  
    }
    
    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 15);
    }    

    if (p_this->sam_range.val == 0) {
        /* Ĭ������Ϊ��4G */
        p_this->sam_range.val  = 4;    
        p_this->sam_range.unit = AM_SENSOR_UNIT_BASE;
    }
    
    return curent_ret;
}

/** \brief ���ܴ�����ͨ�� */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t ctrl_reg3 = 0;
    
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || cur_id == 3) {

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

        /** \brief ����ģʽ */
        ctrl_reg3 = 0x03;
        ret = __lis3mdl_write(p_this, __LIS3MDL_REG_CTRL3, &ctrl_reg3, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }      
        
        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 15);
        }
    }

    return cur_ret;
}

/*
 * \brief ����ѡ������
 */
am_local am_err_t  __reg_attr_range_set (am_sensor_lis3mdl_dev_t *p_this, 
                                         uint8_t                 range)
{
    uint8_t range_cfg = 0;

    switch (range) {
    case 4:
        range_cfg = __LIS3MDL_RANGE_SET_4G;
        break;
    case 8:
        range_cfg = __LIS3MDL_RANGE_SET_8G;
        break;
    case 12:
        range_cfg = __LIS3MDL_RANGE_SET_12G;
        break;
    case 16:
        range_cfg = __LIS3MDL_RANGE_SET_16G;
        break;
    default:
        break;
    }
    return __lis3mdl_write(p_this, __LIS3MDL_REG_CTRL2, &range_cfg, 1);
}

/** \brief ���ô�����ͨ������ */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val)
{
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    am_sensor_val_t sensor_val = {0, 0};
    am_sensor_val_t __sampling_range = {0, 0};

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if ((id != AM_LIS3MDL_CHAN_1) && (id != AM_LIS3MDL_CHAN_2) &&
        (id != AM_LIS3MDL_CHAN_3)) {
        return -AM_ENODEV;
    }

    switch (attr) {

    /* �޸����� */
    case AM_SENSOR_ATTR_FULL_SCALE:

        sensor_val.unit = p_val->unit;
        sensor_val.val  = p_val->val;    
    
        if (sensor_val.val < 4 || sensor_val.val > 16) {
            return -AM_ENOTSUP;
        }

        am_sensor_val_unit_convert(&sensor_val, 1, AM_SENSOR_UNIT_BASE);

        if (sensor_val.val == 4) {
            __sampling_range.val = 4;
        } else if (sensor_val.val == 8) {
            __sampling_range.val = 8;
        } else if (sensor_val.val == 12) {
            __sampling_range.val = 12;
        } else if (sensor_val.val == 16) {
            __sampling_range.val = 16;
        }

        ret = __reg_attr_range_set(p_this, __sampling_range.val);
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
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;

    am_err_t ret = AM_OK;

    if ((id != AM_LIS3MDL_CHAN_1) && (id != AM_LIS3MDL_CHAN_2)&&
        (id != AM_LIS3MDL_CHAN_3) && (id != AM_LIS3MDL_CHAN_4)) {
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
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;

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
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;
    uint8_t  int_cfg = 0;

    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x10) {
        return AM_OK;
    }

    /** \brief ʹ�ܴ������� */
    ret = __lis3mdl_read(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }

    int_cfg |= __LIS3MDL_EN_DRDY;
    ret = __lis3mdl_write(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    if (p_this->dev_info->trigger_pin != -1) {
        
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
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_drv;

    am_err_t cur_ret = AM_OK;
    am_err_t ret     = AM_OK;
    uint8_t  int_cfg = 0;

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

        if ((p_this->dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        /** \brief �������ݿɶ����� */
        ret = __lis3mdl_read(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);
        if ( ret != AM_OK) {
            cur_ret = ret;
        }

        int_cfg &= __LIS3MDL_EN_CLOSE;
        ret = __lis3mdl_write(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);
        if ( ret != AM_OK) {
            cur_ret = ret;
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
 * \brief ������ LIS3MDL ��ʼ��
 */
am_sensor_handle_t am_sensor_lis3mdl_init (
        am_sensor_lis3mdl_dev_t           *p_dev,
        const am_sensor_lis3mdl_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t lis3mdl_id = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->lis3mdl_dev.p_drv   = p_dev;
    p_dev->lis3mdl_dev.p_funcs = &__g_sensor_lis3mdl_funcs;
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
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __lis3mdl_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    /* ��ȡID */
    ret = __lis3mdl_read(p_dev, __LIS3MDL_REG_ID, &lis3mdl_id, 1);

    if (ret != AM_OK || lis3mdl_id != __LIS3MDL_MY_ID) {
        cur_ret = ret;
    }

    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor LIS3MDL Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->lis3mdl_dev);
}

/**
 * \brief ������ LIS3MDL ȥ��ʼ��
 */
am_err_t am_sensor_lis3mdl_deinit (am_sensor_handle_t handle)
{
    am_sensor_lis3mdl_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->lis3mdl_dev.p_drv   = NULL;
    p_dev->lis3mdl_dev.p_funcs = NULL;
    p_dev->dev_info            = NULL;

    return AM_OK;
}


/* end of file */
