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
 * \brief 传感器 BMA253 驱动文件
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
 * 宏定义
 ******************************************************************************/
#define  __BMA253_REG_BGW_CHIPID    0x00  /**< \brief ID寄存器地址            */
#define  __BMA253_REG_ACCD_X_LSB    0x02  /**< \brief X轴低字节数据寄存器地址 */
#define  __BMA253_REG_ACCD_X_MSB    0x03  /**< \brief X轴高字节数据寄存器地址 */
#define  __BMA253_REG_ACCD_Y_LSB    0x04  /**< \brief Y轴低字节数据寄存器地址 */
#define  __BMA253_REG_ACCD_Y_MSB    0x05  /**< \brief Y轴高字节数据寄存器地址 */
#define  __BMA253_REG_ACCD_Z_LSB    0x06  /**< \brief Z轴低字节数据寄存器地址 */
#define  __BMA253_REG_ACCD_Z_MSB    0x07  /**< \brief Z轴高字节数据寄存器地址 */
#define  __BMA253_REG_ACCD_TEMP     0x08  /**< \brief 温度数据寄存器地址      */
#define  __BMA253_REG_INT_STATUS_0  0x09  /**< \brief 中断状态寄存器0地址     */
#define  __BMA253_REG_INT_STATUS_1  0x0A  /**< \brief 中断状态寄存器1地址     */ 
#define  __BMA253_REG_INT_STATUS_2  0x0B  /**< \brief 中断状态寄存器2地址     */ 
#define  __BMA253_REG_INT_STATUS_3  0x0C  /**< \brief 中断状态寄存器3地址     */ 
#define  __BMA253_REG_FIFO_STATUS   0x0E  /**< \brief FIFO状态寄存器地址      */ 
#define  __BMA253_REG_PMU_RANGE     0x0F  /**< \brief 加速度范围寄存器地址    */ 
#define  __BMA253_REG_PMU_BW        0x10  /**< \brief 数据滤波带宽寄存器地址  */
#define  __BMA253_REG_PMU_LPW       0x11  /**< \brief 低功耗配置寄存器地址    */ 
#define  __BMA253_REG_PMU_LOW_POWER 0x12  /**< \brief 低功耗模式寄存器地址    */ 
#define  __BMA253_REG_ACCD_HBW      0x13  /**< \brief 数据输出格式寄存器地址  */ 
#define  __BMA253_REG_BGW_SOFTRESET 0x14  /**< \brief 软件复位寄存器地址      */
#define  __BMA253_REG_INT_EN_0      0x16  /**< \brief 中断使能寄存器0地址     */
#define  __BMA253_REG_INT_EN_1      0x17  /**< \brief 中断使能寄存器1地址     */
#define  __BMA253_REG_INT_EN_2      0x18  /**< \brief 中断使能寄存器2地址     */
#define  __BMA253_REG_INT_MAP_0     0x19  /**< \brief 中断信号配置寄存器0地址 */
#define  __BMA253_REG_INT_MAP_1     0x1A  /**< \brief 中断信号配置寄存器1地址 */
#define  __BMA253_REG_INT_MAP_2     0x1B  /**< \brief 中断信号配置寄存器2地址 */
#define  __BMA253_REG_INT_SRC       0x1E  /**< \brief 中断源寄存器地址        */
#define  __BMA253_REG_INT_OUT_CTRL  0x20  /**< \brief 中断输出控制寄存器地址  */
#define  __BMA253_REG_INT_RST_LATCH 0x21  /**< \brief 中断复位寄存器地址      */
#define  __BMA253_REG_INT_0         0x22  /**< \brief 中断0寄存器地址         */
#define  __BMA253_REG_INT_1         0x23  /**< \brief 中断1寄存器地址         */
#define  __BMA253_REG_INT_2         0x24  /**< \brief 中断2寄存器地址         */
#define  __BMA253_REG_INT_3         0x25  /**< \brief 中断3寄存器地址         */
#define  __BMA253_REG_INT_4         0x26  /**< \brief 中断4寄存器地址         */
#define  __BMA253_REG_INT_5         0x27  /**< \brief 中断5寄存器地址         */

#define  __BMA253_REG_INT_6         0x28  /**< \brief 中断6寄存器地址         */
#define  __BMA253_REG_INT_7         0x29  /**< \brief 中断7寄存器地址         */
#define  __BMA253_REG_INT_8         0x2A  /**< \brief 中断8寄存器地址         */
#define  __BMA253_REG_INT_9         0x2B  /**< \brief 中断9寄存器地址         */
#define  __BMA253_REG_INT_A         0x2C  /**< \brief 中断A寄存器地址         */
#define  __BMA253_REG_INT_B         0x2D  /**< \brief 中断B寄存器地址         */
#define  __BMA253_REG_INT_C         0x2E  /**< \brief 中断C寄存器地址         */
#define  __BMA253_REG_INT_D         0x2F  /**< \brief 中断D寄存器地址         */
#define  __BMA253_REG_FIFO_CONFIG_0 0x30  /**< \brief FIFO配置寄存器0地址     */
#define  __BMA253_REG_PMU_SELF_TEST 0x32  /**< \brief 自检测试寄存器地址      */
#define  __BMA253_REG_TRIM_NVM_CTRL 0x33  /**< \brief 内存控制寄存器地址      */
#define  __BMA253_REG_BGW_SPI3_WDT  0x34  /**< \brief 数字接口寄存器地址      */
#define  __BMA253_REG_OFC_CTRL      0x36  /**< \brief OFC控制寄存器地址       */
#define  __BMA253_REG_OFC_SETTING   0x37  /**< \brief OFC设置寄存器地址       */
#define  __BMA253_REG_OFC_OFFSET_X  0x38  /**< \brief X轴偏差寄存器地址       */
#define  __BMA253_REG_OFC_OFFSET_Y  0x39  /**< \brief Y轴偏差寄存器地址       */
#define  __BMA253_REG_OFC_OFFSET_Z  0x3A  /**< \brief Z轴偏差寄存器地址       */
#define  __BMA253_REG_TRIM_GP0      0x3B  /**< \brief 备份通用寄存器0地址     */
#define  __BMA253_REG_TRIM_GP1      0x3C  /**< \brief 备份通用寄存器1地址     */
#define  __BMA253_REG_FIFO_CONFIG_1 0x3E  /**< \brief FIFO配置寄存器1地址     */
#define  __BMA253_REG_FIFO_DATA     0x3F  /**< \brief FIFO数据寄存器地址      */

#define  __BMA253_MY_ID             0XFA  /**< \brief ID值                    */

/** \brief 设置采样范围 */
#define  __BMA253_RANGE_SET_2G      0X03  /**< \brief 设置采样范围为±2G      */
#define  __BMA253_RANGE_SET_4G      0X05  /**< \brief 设置采样范围为±4G      */
#define  __BMA253_RANGE_SET_8G      0X08  /**< \brief 设置采样范围为±8G      */
#define  __BMA253_RANGE_SET_16G     0X0C  /**< \brief 设置采样范围为±16G     */

/** \brief 中断触发配置 */
#define  __BMA253_TRIGGER_READ  (1 << 4)  /**< \brief 使能数据可读触发        */
#define  __BMA253_TRIGGER_CLOSE (0 << 4)  /**< \brief 禁能数据可读触发        */
#define  __BMA253_EN_INT1_PIN   (1 << 0)  /**< \brief 使能中断1引脚           */
#define  __BMA253_INT_CTRL      (1 << 0)  /**< \brief 配置为推挽输出，高电平  */
#define  __BMA253_INT_CLEAR     (1 << 7)  /**< \brief 清除中断                */

/** \brief 将两个int8转换为一个int16_t类型 */
#define  __BMA253_UINT8_TO_UINT16(buff) \
                       (int16_t)(((int8_t)(buff[1]) << 4) \
                               | ((int8_t)(buff[0]) >> 4))
/** \brief 计算温度,并扩大10^6倍 */
#define  __BMA253_GET_TEMP_VALUE(data) ((int32_t)((23 + \
                                          ((int8_t)(data[0]) >> 1)) * 1000000))

/** \brief 计算加速度,并扩大10^6倍 */
/** \brief 采样范围为±2G */
#define  __BMA253_GET_ACCEL_VALUE_2G(data)  ((int32_t)(data * 9800))
/** \brief 采样范围为±4G */
#define  __BMA253_GET_ACCEL_VALUE_4G(data)  ((int32_t)(data * 19500))
/** \brief 采样范围为±8G */
#define  __BMA253_GET_ACCEL_VALUE_8G(data)  ((int32_t)(data * 39100))
/** \brief 采样范围为±16G */
#define  __BMA253_GET_ACCEL_VALUE_16G(data) ((int32_t)(data * 78100))

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
 * \brief BMA253 写数据
 */
am_local am_err_t __bma253_write (am_sensor_bma253_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief BMA253 读数据
 */
am_local am_err_t __bma253_read (am_sensor_bma253_dev_t *p_this,
                                 uint32_t                subaddr,
                                 uint8_t                *p_buf,
                                 uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief 根据量程将结果转换成加速度实际值
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

/** \breif 中断回调函数 */
am_local void __bma253_alarm_callback (void *p_arg)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_bma253_dev_t* p_this = (am_sensor_bma253_dev_t*)p_arg;

    uint8_t reg_data[2];
    int32_t tem_data   = 0;
    uint8_t i = 0;
    
    /** \brief 获取X轴加速度 */
    __bma253_read(p_this, __BMA253_REG_ACCD_X_LSB, reg_data, 2);
    
    tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
    
    /** \brief X轴加速度 */
    p_this->data[0].val = __bma253_get_accel_value(p_this,tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

    /** \brief 获取Y轴加速度 */
    __bma253_read(p_this, __BMA253_REG_ACCD_Y_LSB, reg_data, 2);

    tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
   
    /** \brief Y轴加速度 */
    p_this->data[1].val = __bma253_get_accel_value(p_this,tem_data);
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

    /** \brief 获取Z轴加速度 */
    __bma253_read(p_this, __BMA253_REG_ACCD_Z_LSB, reg_data, 2);
    
    tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
   
    /** \brief Z轴加速度 */
    p_this->data[2].val = __bma253_get_accel_value(p_this,tem_data); 
    p_this->data[2].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

    /** \brief 获取温度 */
    __bma253_read(p_this, __BMA253_REG_ACCD_TEMP, reg_data, 1);

    /** \brief 温度 */
    p_this->data[3].val = __BMA253_GET_TEMP_VALUE(reg_data); 
    p_this->data[3].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/    
    
    for (i = 0; i < 4; i++) {
        if (p_this->pfn_trigger_cb[i] &&
                (p_this->flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->pfn_trigger_cb[i](p_this->p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);
        }
    }
}

/** \brief 获取该传感器某一通道的类型 */
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

/** \brief 获取传感器通道采样数据 */
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

            do {
                /** \brief 获取X轴加速度 */
                ret = __bma253_read(p_this, __BMA253_REG_ACCD_X_LSB, reg_data, 2);
                if (ret != AM_OK) {
                    return ret;
                }
            } while ((reg_data[0] & 0x01) != 0x01);
            
            tem_data = __BMA253_UINT8_TO_UINT16(reg_data);

            /** \brief X轴加速度 */
            p_buf[i].val = __bma253_get_accel_value(p_this,tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 1) {

            do {
                /** \brief 获取Y轴加速度 */
                ret = __bma253_read(p_this, __BMA253_REG_ACCD_Y_LSB, reg_data, 2);
                if (ret != AM_OK) {
                    return ret;
                }
            } while ((reg_data[0] & 0x01) != 0x01);
            
            tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
           
            /** \brief Y轴加速度 */
            p_buf[i].val = __bma253_get_accel_value(p_this,tem_data);
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 2) {
            
            do {
                /** \brief 获取Z轴加速度 */
                ret = __bma253_read(p_this, __BMA253_REG_ACCD_Z_LSB, reg_data, 2);
                if (ret != AM_OK) {
                    return ret;
                }
            } while ((reg_data[0] & 0x01) != 0x01);
            
            tem_data = __BMA253_UINT8_TO_UINT16(reg_data);
         
            /** \brief Z轴加速度 */
            p_buf[i].val = __bma253_get_accel_value(p_this,tem_data);
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 3) {

            /** \brief 获取温度 */
            ret = __bma253_read(p_this, __BMA253_REG_ACCD_TEMP, reg_data, 1);
            if (ret != AM_OK) {
                return ret;
            }
            
            /** \brief 温度 */
            p_buf[i].val = __BMA253_GET_TEMP_VALUE(reg_data); 
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
            
            /* 设置通道使能标志位 */
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

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 15);
    }  

    if (p_this->sam_range.val == 0) {
        /* 默认量程为±2G */
        p_this->sam_range.val  = 2;    
        p_this->sam_range.unit = AM_SENSOR_UNIT_BASE;
    }

    /* 配置滤波带宽 */
    ret = __bma253_write(p_this, __BMA253_REG_PMU_BW, &pmu_bw, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    
    /* 配置电源模式、睡眠周期 */
    ret = __bma253_write(p_this, __BMA253_REG_PMU_LPW, &pmu_lpw, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    return curent_ret;
}

/** \brief 禁能传感器通道 */
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

            /* 清除使能标志位 */
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
 * \brief 配置选择量程
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

/** \brief 配置传感器通道属性 */
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

    /* 修改量程 */
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

/** \brief 获取传感器通道属性 */
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

/** \brief 打开触发 */
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

    /** \brief 使能数据可读触发 */
    reg_data = __BMA253_TRIGGER_READ;
    ret = __bma253_write(p_this, __BMA253_REG_INT_EN_1, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief 使能触发引脚 */
    reg_data = __BMA253_EN_INT1_PIN;
    ret = __bma253_write(p_this, __BMA253_REG_INT_MAP_1, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief 配置中断引脚模式 */
    reg_data = __BMA253_INT_CTRL;
    ret = __bma253_write(p_this, __BMA253_REG_INT_OUT_CTRL, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }    
    
    if (p_this->dev_info->trigger_pin != -1) {
        /* 清除中断标志 */
        reg_data = __BMA253_INT_CLEAR;
        __bma253_write(p_this, __BMA253_REG_INT_RST_LATCH, &reg_data, 1);
        
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

        /** \brief 禁能数据可读触发 */
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
 * \brief 传感器 BMA253 初始化
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
    
    /* 外部中断连接 */
    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __bma253_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_RISE);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);
   
    /* 读取ID */
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
 * \brief 传感器 BMA253 去初始化
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
