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
 * \brief ������ SHTC1 �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_shtc1.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_delay.h"

/*******************************************************************************
 * SHTC1 I2C����
 ******************************************************************************/
const uint8_t __SHTC1_RESET[] = {0x80, 0x5D};   /** \brief �豸��λ����       */
const uint8_t __SHTC1_ID[]    = {0xEF, 0xC8};   /** \brief ��ȡID����         */
const uint8_t __SHTC1_CSE_T[] = {0x7C, 0xA2};   /** \brief ������ȡ�¶�����   */
const uint8_t __SHTC1_CSE_H[] = {0x5C, 0x24};   /** \brief ������ȡʪ������   */
const uint8_t __SHTC1_CSD_T[] = {0x78, 0x66};   /** \brief ���¶�ȡ�¶�����   */
const uint8_t __SHTC1_CSD_H[] = {0x58, 0xE0};   /** \brief ���¶�ȡ�¶�����   */

#define  __SHTC1_MY_ID           0x07           /** \brief IDֵ               */

/** \brief ������uint8ת��Ϊһ��uint16_t���� */
#define  __SHTC1_UINT8_TO_UINT16(data)  ((uint16_t)(data[0] << 8 | data[1]))

#define  __SHTC1_GET_HUM(data)   (((uint32_t)(data) * 100 / \
                                                  (1 << 16)) * 1000000)
#define  __SHTC1_GET_TEMP(data)  (((uint32_t)(data) * 175 / \
                                                  (1 << 16) - 45) * 1000000)

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_shtc1_funcs = {
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
/*
 * \brief shtc1 д����
 */
am_local am_err_t __shtc1_write (am_sensor_shtc1_dev_t  *p_this,
                                 uint8_t                *p_buf,
                                 uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, 0x00, p_buf, nbytes);
}

/*
 * \brief shtc1 ������
 */
am_local am_err_t __shtc1_read (am_sensor_shtc1_dev_t  *p_this,
                                uint8_t                *p_buf,
                                uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, 0x00, p_buf, nbytes);
}

/** \brief ��ȡ�ô�����ĳһͨ�������� */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_SHTC1_CHAN_1) {
        return AM_SENSOR_TYPE_HUMIDITY;
    } else if (id == AM_SHTC1_CHAN_2) {
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
    am_sensor_shtc1_dev_t* p_this = (am_sensor_shtc1_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    uint8_t reg_data[6] = {0};

    uint16_t hum_data = 0;
    int16_t  tem_data = 0;

    int cur_id = 0;
    int i = 0;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (AM_BIT_GET(p_this->trigger, 7) != 1) {
        return -AM_EPERM;
    }

    for (i = 0; i < num; i++) {
        p_buf[i].unit = AM_SENSOR_UNIT_INVALID;
    }

    /* ��ȡ������� */
    ret = __shtc1_read (p_this, reg_data, 6);
    
    if (ret != AM_OK) {
        return ret;
    }
    
    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {
            
            hum_data = __SHTC1_UINT8_TO_UINT16((&reg_data[0]));
            p_buf[i].val  = __SHTC1_GET_HUM(hum_data); /*< \brief ʪ�� */
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ-6:10^(-6)*/

        } else if (cur_id == 1) {

            tem_data = __SHTC1_UINT8_TO_UINT16((&reg_data[3]));
            p_buf[i].val  = __SHTC1_GET_TEMP(tem_data); /*< \brief �¶� */
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief ��λĬ��Ϊ-6:10^(-6)*/

        } else {
            return -AM_ENODEV;  /*< \brief ���˴�ͨ�������ڸô�������ֱ�ӷ��� */
        }
    }

    /* д�������� */
    __shtc1_write (p_this, (uint8_t *)(__SHTC1_CSE_H), 2);
    
    return ret;
}

/** \brief ʹ�ܴ�����ͨ�� */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result)
{
    am_sensor_shtc1_dev_t* p_this = (am_sensor_shtc1_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    am_err_t ret = -AM_ENODEV;
    am_err_t curent_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1) {
            if (p_result != NULL) {
                p_result[i].val = AM_OK;
            }
            ret = AM_OK;
        } else {
            if (p_result != NULL) {
                p_result[i].val = -AM_ENODEV;
            }
            curent_ret = -AM_ENODEV;
        }
    }

    if (ret != AM_OK) {     /*< \breif �������û�иô�������ͨ�����룬���˳� */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 7);
        AM_BIT_SET(p_this->trigger, 6);
        AM_BIT_SET(p_this->trigger, 5);
    }

    /* д�������� */
    __shtc1_write (p_this, (uint8_t *)(__SHTC1_CSE_H), 2);

    return curent_ret;
}

/** \brief ���ܴ�����ͨ�� */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_shtc1_dev_t* p_this = (am_sensor_shtc1_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            AM_BIT_CLR(p_this->trigger, 6);
            ret = AM_OK;

        } else if (cur_id == 1) {

            AM_BIT_CLR(p_this->trigger, 5);
            ret = AM_OK;

        } else {
            ret = -AM_ENODEV;
        }

        if (p_result != NULL) {
            p_result[i].val = ret;
        }
    }

    if ((AM_BIT_GET(p_this->trigger, 6) == 0)
            && (AM_BIT_GET(p_this->trigger, 5) == 0)) {

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 7);
        }
    }

    return cur_ret;
}

/** \brief ���ô�����ͨ������ */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val)
{
    return -AM_ENOTSUP;
}

/** \brief ��ȡ������ͨ������ */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val)
{
    return -AM_ENOTSUP;
}

/** \brief ���ô�����һ��ͨ����������һ�������ص����� */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg)
{
    return -AM_ENOTSUP;
}

/** \brief �򿪴��� */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id)
{
    return -AM_ENOTSUP;
}

/** \brief �رմ��� */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    return -AM_ENOTSUP;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief ������ SHTC1 ��ʼ��
 */
am_sensor_handle_t am_sensor_shtc1_init (
        am_sensor_shtc1_dev_t            *p_dev,
        const am_sensor_shtc1_devinfo_t  *p_devinfo,
        am_i2c_handle_t                   handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t shtc1_id[2] = {0};

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_NONE);

    p_dev->shtc1_dev.p_drv   = p_dev;
    p_dev->shtc1_dev.p_funcs = &__g_sensor_shtc1_funcs;
    p_dev->dev_info           = p_devinfo;

    p_dev->trigger            = 0;
    p_dev->data[0].val        = 0;
    p_dev->data[0].unit       = AM_SENSOR_UNIT_INVALID;
    p_dev->data[1].val        = 0;
    p_dev->data[1].unit       = AM_SENSOR_UNIT_INVALID;

    /* ��ȡID */
    __shtc1_write(p_dev, (uint8_t*)(__SHTC1_ID), 2);

    ret = __shtc1_read(p_dev, shtc1_id, 2);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    if (ret != AM_OK || (shtc1_id[1] & 0x0F) != __SHTC1_MY_ID) {
        cur_ret = ret;
    }

    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor SHTC1 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->shtc1_dev);
}

/**
 * \brief ������ SHTC1 ȥ��ʼ��
 */
am_err_t am_sensor_shtc1_deinit (am_sensor_handle_t handle)
{
    am_sensor_shtc1_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->shtc1_dev.p_drv   = NULL;
    p_dev->shtc1_dev.p_funcs = NULL;
    p_dev->dev_info           = NULL;

    return AM_OK;
}

/* end of file */
