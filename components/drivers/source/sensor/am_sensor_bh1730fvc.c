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
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_bh1730fvc.h"
#include "am_gpio.h"
#include "am_vdebug.h"

/*******************************************************************************
 * �궨��
 ******************************************************************************/
#define  __BH1730FVC_REG_CONTROL        0x00  /**< \brief ���ƼĴ�����ַ      */ 
#define  __BH1730FVC_REG_TIMING         0x01  /**< \brief ʱ��Ĵ�����ַ      */
#define  __BH1730FVC_REG_INTERRUPT      0x02  /**< \brief �жϼĴ�����ַ      */
#define  __BH1730FVC_REG_THLLOW         0x03  /**< \brief �ж���ֵ�Ĵ�����ַ  */
#define  __BH1730FVC_REG_THLHIGH        0x04  /**< \brief �ж���ֵ�Ĵ�����ַ  */
#define  __BH1730FVC_REG_THHLOW         0x05  /**< \brief �ж���ֵ�Ĵ�����ַ  */
#define  __BH1730FVC_REG_THHHIGH        0x06  /**< \brief �ж���ֵ�Ĵ�����ַ  */
#define  __BH1730FVC_REG_GAIN           0x07  /**< \brief ����Ĵ�����ַ      */
#define  __BH1730FVC_REG_ID             0x12  /**< \brief ID�Ĵ�����ַ        */
#define  __BH1730FVC_REG_DATA0LOW       0x14  /**< \brief ����0�Ĵ�����ַ     */
#define  __BH1730FVC_REG_DATA0HIGH      0x15  /**< \brief ����0�Ĵ�����ַ     */
#define  __BH1730FVC_REG_DATA1LOW       0x16  /**< \brief ����1�Ĵ�����ַ     */
#define  __BH1730FVC_REG_DATA1HIGH      0x17  /**< \brief ����1�Ĵ�����ַ     */

#define  __BH1730FVC_MY_ID              0x70  /**< \brief IDֵ                */

/** \brief ʹ�����ݿɶ����� */
#define  __BH1730FVC_TRIGGER_READ       0x11

/** \brief �������ݿɶ����� */
#define  __BH1730FVC_TRIGGER_CLOSE      (~(0X01 << 4))

/** \brief �����λ */
#define  __BH1730FVC_SOFT_RESET         0x04

/** \brief ����ж� */
#define  __BH1730FVC_INT_CLEAR          0x01

/** \brief ������uint8ת��Ϊһ��uint16_t���� */
#define  __BH1730FVC_UINT8_TO_UINT16(data)  ((uint16_t)(data[1] << 8 | data[0]))
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
am_local am_const struct am_sensor_drv_funcs __g_sensor_bh1730fvc_funcs = {
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
 * \brief BH1730FVC д����
 */
am_local am_err_t __bh1730fvc_write (am_sensor_bh1730fvc_dev_t *p_this,
                                     uint32_t                   subaddr,
                                     uint8_t                   *p_buf,
                                     uint32_t                   nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, (subaddr | 0x80), p_buf, nbytes);
}

/**
 * \brief BH1730FVC ������
 */
am_local am_err_t __bh1730fvc_read (am_sensor_bh1730fvc_dev_t *p_this,
                                    uint32_t                   subaddr,
                                    uint8_t                   *p_buf,
                                    uint32_t                   nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, (subaddr | 0x80), p_buf, nbytes);
}

/**
 * \brief BH1730FVC ����ж�
 */
am_local am_err_t __bh1730fvc_send_cmd (am_sensor_bh1730fvc_dev_t *p_this, 
                                   uint8_t                    cmd)
{
    return am_i2c_write(&p_this->i2c_dev, (0xE0 | cmd), NULL, 0);
}

/**
 * \brief ������������ֵ�����ǿʵ��ֵ
 */
am_local uint32_t __bh1730fvc_get_light_value (uint32_t *p_data)
{
    uint8_t p = 0;
    uint32_t real_data = 0;
  
    if (p_data[0] != 0) {
        p = (p_data[1] * 100 / p_data[0]);
    } else {
        p = 0xff;
    }

    if (p < 26) {
        real_data = ( 1.290 * p_data[0] - 2.733 * p_data[1] );
    } else if (p < 55) {
        real_data = ( 0.795 * p_data[0] - 0.859 * p_data[1] );
    } else if (p < 109) {
        real_data = ( 0.510 * p_data[0] - 0.345 * p_data[1] );
    } else if (p < 213) {
        real_data = ( 0.276 * p_data[0] - 0.130 * p_data[1] );
    } else {
        real_data = 0;
    }
        
    return real_data;
}

/** \breif �жϻص����� */
am_local void __bh1730fvc_alarm_callback (void *p_arg)
{
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief ����ж��ӳٴ������� */
}

/** \breif �ж��ӳٺ��� */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_arg;

    uint8_t reg_data[2];
    uint32_t tem_data[2];
    
    /** \brief ��ȡ��ǿֵ1 */
    __bh1730fvc_read(p_this, __BH1730FVC_REG_DATA0LOW, reg_data, 2);

    tem_data[0] = __BH1730FVC_UINT8_TO_UINT16(reg_data);
   
    /** \brief ��ȡ��ǿֵ2 */
    __bh1730fvc_read(p_this, __BH1730FVC_REG_DATA1LOW, reg_data, 2);

    tem_data[1] = __BH1730FVC_UINT8_TO_UINT16(reg_data);
    
    /** \brief ��ǿ */
    p_this->data[0].val  = __bh1730fvc_get_light_value(tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_BASE; /*< \brief ��λĬ��Ϊ0:10^(0) */
 
    if (p_this->pfn_trigger_cb[0] &&
            (p_this->flags[0] & AM_SENSOR_TRIGGER_DATA_READY)) {
        p_this->pfn_trigger_cb[0](p_this->p_arg[0],
                                  AM_SENSOR_TRIGGER_DATA_READY);
    }
    /* ����ж� */
    __bh1730fvc_send_cmd(p_this, __BH1730FVC_INT_CLEAR);
}

/** \brief ��ȡ�ô�����ĳһͨ�������� */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_BH1730FVC_CHAN_1) {
        return AM_SENSOR_TYPE_LIGHT;
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
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_drv;
    
    am_err_t ret         = AM_OK;
    uint8_t  reg_data[2] = {0};
    uint32_t tem_data[2] = {0};

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

    /* ��Ϊ1�������������׼�����������ص�������ʹ�� */
    if (num == 1) { 
        cur_id = p_ids[0];
        /* ��������׼������������ʽ����ֱ�Ӹ�ֵ */
        if ((AM_BIT_GET(p_this->trigger, 1)) &&
                (p_this->flags[0] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_buf[0].val = p_this->data[cur_id].val;
            p_buf[0].unit= p_this->data[cur_id].unit;
            return AM_OK;
        }
    }    
    
    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            /** \brief ��ȡ��ǿֵ1 */
            ret = __bh1730fvc_read(p_this, __BH1730FVC_REG_DATA0LOW, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data[0] = __BH1730FVC_UINT8_TO_UINT16(reg_data);

            /** \brief ��ȡ��ǿֵ2 */
            ret = __bh1730fvc_read(p_this, __BH1730FVC_REG_DATA1LOW, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data[1] = __BH1730FVC_UINT8_TO_UINT16(reg_data);

            /** \brief ��ǿ */
            p_buf[i].val = __bh1730fvc_get_light_value(tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_BASE; /*< \brief ��λĬ��Ϊ0:10^(0)*/
            
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
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t ctrl_reg = 0x03;
    
    am_err_t ret = -AM_ENODEV;
    am_err_t curent_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {
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

    if (ret != AM_OK) {    /**< \breif �������û�иô�������ͨ�����룬���˳� */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 7);
        AM_BIT_SET(p_this->trigger, 6);
    } 
    
    /* ����Ϊ����ģʽ */
    ret = __bh1730fvc_write (p_this, __BH1730FVC_REG_CONTROL, &ctrl_reg, 1);
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
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t ctrl_reg = 0x00;
    
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

        } else {
            ret = -AM_ENODEV;
        }

        if (p_result != NULL) {
            p_result[i].val = ret;
        }
    }

    if (AM_BIT_GET(p_this->trigger, 6) == 0) {

        /* ����Ϊ����ģʽ */
        ret = __bh1730fvc_write (p_this, __BH1730FVC_REG_CONTROL, &ctrl_reg, 1);
        if (ret != AM_OK){
            cur_ret = ret;
        } 
      
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
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_drv;

    if (id != 0) {
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
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;
    uint8_t  reg_data[5] = {0};
    
    if (id != 0) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x2) {
        return AM_OK;
    }

    /** \brief ʹ�����ݿɶ����� */
    reg_data[0] = __BH1730FVC_TRIGGER_READ;
    reg_data[1] = 0xff;
    reg_data[2] = 0xff;
    reg_data[3] = 0xff;
    reg_data[4] = 0xff;
    
    ret = __bh1730fvc_write(p_this, __BH1730FVC_REG_INTERRUPT, reg_data, 5);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }

    if (p_this->dev_info->trigger_pin != -1) {
        __bh1730fvc_send_cmd(p_this, __BH1730FVC_INT_CLEAR);
        ret = am_gpio_trigger_on(p_this->dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }
    
    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->trigger, 0);
        AM_BIT_SET(p_this->trigger, 1);
    }

    return cur_ret;
}

/** \brief �رմ��� */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_bh1730fvc_dev_t* p_this = (am_sensor_bh1730fvc_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;
    uint8_t reg_data = 0;
    
    if (id != 0) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_this->pfn_trigger_cb[id] = NULL;
    p_this->p_arg[id]          = NULL;
    p_this->flags[id]          = 0;

    AM_BIT_CLR(p_this->trigger, id);

    if ((p_this->trigger & 0x1) == 0) {

        if ((p_this->dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        ret = __bh1730fvc_read(p_this, __BH1730FVC_REG_INTERRUPT, &reg_data, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
        /** \brief �������ݿɶ����� */
        reg_data &= __BH1730FVC_TRIGGER_CLOSE;
        ret = __bh1730fvc_write(p_this, __BH1730FVC_REG_INTERRUPT, &reg_data, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
        
        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 1);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief ������ BH1730FVC ��ʼ��
 */
am_sensor_handle_t am_sensor_bh1730fvc_init (
        am_sensor_bh1730fvc_dev_t           *p_dev,
        const am_sensor_bh1730fvc_devinfo_t *p_devinfo,
        am_i2c_handle_t                      handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;
    
    uint8_t bh1730fvc_id = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->bh1730fvc_dev.p_drv   = p_dev;
    p_dev->bh1730fvc_dev.p_funcs = &__g_sensor_bh1730fvc_funcs;
    p_dev->dev_info            = p_devinfo;
    p_dev->pfn_trigger_cb[0]   = NULL;
    p_dev->p_arg[0]            = NULL;
    p_dev->flags[0]            = 0;
    p_dev->trigger             = 0;
    p_dev->data[0].val         = 0;
    p_dev->data[0].unit        = AM_SENSOR_UNIT_INVALID;
  
    /* �ⲿ�ж����� */
    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __bh1730fvc_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);
   
    /* ��ȡID */
    ret = __bh1730fvc_read(p_dev, __BH1730FVC_REG_ID, &bh1730fvc_id, 1);
    if (ret != AM_OK || (bh1730fvc_id & 0xF0) != __BH1730FVC_MY_ID) {
        cur_ret = ret;
    }
    
    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor BH1730FVC Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->bh1730fvc_dev);
}

/**
 * \brief ������ BH1730FVC ȥ��ʼ��
 */
am_err_t am_sensor_bh1730fvc_deinit (am_sensor_handle_t handle)
{
    am_sensor_bh1730fvc_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->bh1730fvc_dev.p_drv   = NULL;
    p_dev->bh1730fvc_dev.p_funcs = NULL;
    p_dev->dev_info            = NULL;

    return AM_OK;
}



/* end of file */
