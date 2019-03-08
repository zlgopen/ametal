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
 * \brief 传感器 LIS3MDL 驱动文件
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
 * 宏定义
 ******************************************************************************/
#define  __LIS3MDL_REG_ID           0x0F/**< \brief ID寄存器地址              */
#define  __LIS3MDL_REG_CTRL1        0x20/**< \brief 控制寄存器1地址           */
#define  __LIS3MDL_REG_CTRL2        0x21/**< \brief 控制寄存器2地址           */
#define  __LIS3MDL_REG_CTRL3        0x22/**< \brief 控制寄存器3地址           */
#define  __LIS3MDL_REG_CTRL4        0x23/**< \brief 控制寄存器4地址           */
#define  __LIS3MDL_REG_CTRL5        0x24/**< \brief 控制寄存器5地址           */
#define  __LIS3MDL_REG_STATUS       0x27/**< \brief 状态寄存器地址            */
#define  __LIS3MDL_REG_OUT_X_L      0x28/**< \brief X轴低字节数据寄存器地址   */
#define  __LIS3MDL_REG_OUT_X_H      0x29/**< \brief X轴高字节数据寄存器地址   */
#define  __LIS3MDL_REG_OUT_Y_L      0x2A/**< \brief Y轴低字节数据寄存器地址   */
#define  __LIS3MDL_REG_OUT_Y_H      0x2B/**< \brief Y轴高字节数据寄存器地址   */
#define  __LIS3MDL_REG_OUT_Z_L      0x2C/**< \brief Z轴低字节数据寄存器地址   */
#define  __LIS3MDL_REG_OUT_Z_H      0x2D/**< \brief Z轴高字节数据寄存器地址   */
#define  __LIS3MDL_REG_TEMP_OUT_L   0x2E/**< \brief 温度低字节数据寄存器地址  */
#define  __LIS3MDL_REG_TEMP_OUT_H   0x2F/**< \brief 温度数据寄存器地址        */
#define  __LIS3MDL_REG_INT_CFG      0x30/**< \brief 中断配置寄存器地址        */
#define  __LIS3MDL_REG_INT_SRC      0x31/**< \brief 中断源寄存器地址          */
#define  __LIS3MDL_REG_INT_THS_L    0x32/**< \brief 中断阈值低字节寄存器地址  */
#define  __LIS3MDL_REG_INT_THS_H    0x33/**< \brief 中断阈值高字节寄存器地址  */

#define  __LIS3MDL_MY_ID            0X3D  /**< \brief ID值                    */

/* 使能温度传感器 */
#define  __LIS3MDL_TEMP_START       0X80

/* 使能引脚中断 */
#define  __LIS3MDL_EN_DRDY          0x03
/* 禁能引脚中断 */
#define  __LIS3MDL_EN_CLOSE         (~0x01)

/** \brief 设置采样范围 */
#define  __LIS3MDL_RANGE_SET_4G     0X00  /**< \brief 设置采样范围为±4G      */
#define  __LIS3MDL_RANGE_SET_8G     0X20  /**< \brief 设置采样范围为±8G      */
#define  __LIS3MDL_RANGE_SET_12G    0X40  /**< \brief 设置采样范围为±12G     */
#define  __LIS3MDL_RANGE_SET_16G    0X60  /**< \brief 设置采样范围为±16G     */

/** \brief 将两个int8转换为一个int16_t类型 */
#define __LIS3MDL_UINT8_TO_UINT16(buff) \
                       (int16_t)(((int8_t)(buff[1]) << 8) \
                               | ((int8_t)(buff[0])))
/** \brief 计算温度,并扩大10^6倍 */
#define __LIS3MDL_GET_TEMP_VALUE(data) \
            ((int32_t)((25 + ((int8_t)(data) >> 3)) * 1000000))

/** \brief 计算磁力,并扩大10^6倍 */
#define __LIS3MDL_GET_MAG_VALUE_4G(data)  ((int32_t)(data * 10000 / 6842) * 100)
#define __LIS3MDL_GET_MAG_VALUE_8G(data)  ((int32_t)(data * 10000 / 3421) * 100)
#define __LIS3MDL_GET_MAG_VALUE_12G(data) ((int32_t)(data * 10000 / 2281) * 100)
#define __LIS3MDL_GET_MAG_VALUE_16G(data) ((int32_t)(data * 10000 / 1711) * 100)
/*******************************************************************************
 * 本地函数声明
 ******************************************************************************/
/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id);

/** \brief 获取传感器通道采样数据 */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf);

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result);

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result);

/** \brief 配置传感器通道属性 */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val);

/** \brief 获取传感器通道属性 */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val);

/** \brief 设置触发，一个通道仅能设置一个触发回调函数 */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg);

/** \brief 打开触发 */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id);

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id);
                                  
/** \brief 传感器标准服务 */
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
 * \brief LIS3MDL 写数据
 */
am_local am_err_t __lis3mdl_write (am_sensor_lis3mdl_dev_t *p_this,
                                   uint32_t                subaddr,
                                   uint8_t                *p_buf,
                                   uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief LIS3MDL 读数据
 */
am_local am_err_t __lis3mdl_read (am_sensor_lis3mdl_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief 根据量程将结果转换成磁力实际值
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

/** \breif 中断回调函数 */
am_local void __lis3mdl_alarm_callback (void *p_arg)
{
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_lis3mdl_dev_t* p_this = (am_sensor_lis3mdl_dev_t*)p_arg;

    uint8_t reg_data[2];
    int32_t tem_data = 0;
    uint8_t i = 0;
    uint8_t int_cfg = 0;

    /** \brief 禁能触发引脚 */
    __lis3mdl_read(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);

    int_cfg &= __LIS3MDL_EN_CLOSE;
    __lis3mdl_write(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);

    /** \brief 获取X轴磁力 */
    __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_X_L, reg_data, 2);
    
    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);

    /** \brief X轴磁力 */
    p_this->data[0].val = __lis3mdl_get_accel_value(p_this, tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取Y轴磁力 */
    __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Y_L, reg_data, 2);

    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Y轴磁力 */
    p_this->data[1].val = __lis3mdl_get_accel_value(p_this, tem_data); 
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取Z轴磁力 */
    __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Z_L, reg_data, 2);
    
    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Z轴磁力 */
    p_this->data[2].val = __lis3mdl_get_accel_value(p_this, tem_data); 
    p_this->data[2].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取温度 */
    __lis3mdl_read(p_this, __LIS3MDL_REG_TEMP_OUT_L, reg_data, 2);
    
    tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);

    /** \brief 温度 */
    p_this->data[3].val = __LIS3MDL_GET_TEMP_VALUE(tem_data); 
    p_this->data[3].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/    

    for (i = 0; i < 4; i++) {
        if (p_this->pfn_trigger_cb[i] &&
                (p_this->flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->pfn_trigger_cb[i](p_this->p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);        
        } 
    }

    /** \brief 使能中断触发 */
    __lis3mdl_read(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);

    int_cfg |= __LIS3MDL_EN_DRDY;
    __lis3mdl_write(p_this, __LIS3MDL_REG_INT_CFG, &int_cfg, 1);   
}

/** \brief 获取该传感器某一通道的类型 */
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

/** \brief 获取传感器通道采样数据 */
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

    /* 若为1，则可能在数据准备就绪触发回调函数中使用 */
    if (num == 1) {
        cur_id = p_ids[0];

        /* 若打开数据准备就绪触发方式，则直接赋值 */
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

            /** \brief 获取X轴磁力 */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_X_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
           
            /** \brief X轴磁力 */
            p_buf[i].val = __lis3mdl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 1) {

            /** \brief 获取Y轴磁力 */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Y_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Y轴磁力 */
            p_buf[i].val = __lis3mdl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 2) {

            /** \brief 获取Z轴磁力 */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_OUT_Z_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Z轴磁力 */
            p_buf[i].val = __lis3mdl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 3) {

            /** \brief 获取温度 */
            ret = __lis3mdl_read(p_this, __LIS3MDL_REG_TEMP_OUT_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }

            tem_data = __LIS3MDL_UINT8_TO_UINT16(reg_data);

            /** \brief 温度 */
            p_buf[i].val = __LIS3MDL_GET_TEMP_VALUE(tem_data);
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else {
            return -AM_ENODEV;  /*< \brief 若此次通道不属于该传感器，直接返回 */
        }
    }
    
    return ret;
}

/** \brief 使能传感器通道 */
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

    if (ret != AM_OK) {    /**< \breif 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }
    
    /** \brief 连续转换模式 */
    ctrl_reg3 = 0x00;
    ret = __lis3mdl_write(p_this, __LIS3MDL_REG_CTRL3, &ctrl_reg3, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    
    if (AM_BIT_GET(p_this->trigger, 11) == 1) {
        
        /** \brief 使能温度通道 */
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
        /* 默认量程为±4G */
        p_this->sam_range.val  = 4;    
        p_this->sam_range.unit = AM_SENSOR_UNIT_BASE;
    }
    
    return curent_ret;
}

/** \brief 禁能传感器通道 */
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

        /** \brief 掉电模式 */
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
 * \brief 配置选择量程
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

/** \brief 配置传感器通道属性 */
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

    /* 修改量程 */
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

/** \brief 获取传感器通道属性 */
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

    /* 获取量程 */
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

/** \brief 设置触发，一个通道仅能设置一个触发回调函数 */
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

/** \brief 打开触发 */
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

    /** \brief 使能触发引脚 */
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
        
        /* 打开触发引脚 */
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

/** \brief 关闭触发 */
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

        /** \brief 禁能数据可读触发 */
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
 * \brief 传感器 LIS3MDL 初始化
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
    
    /* 外部中断连接 */
    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __lis3mdl_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    /* 读取ID */
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
 * \brief 传感器 LIS3MDL 去初始化
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
