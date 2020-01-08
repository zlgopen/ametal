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
 * \brief 传感器 MAG3110 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-03  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_mag3110.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "math.h"

/*******************************************************************************
 * 宏定义
 ******************************************************************************/
#define __MAG3110_REG_DR_STATUS         0x00    /**< \brief 数据读状态寄存器        */
#define __MAG3110_REG_OUT_X_MSB         0x01    /**< \brief X轴数据的高八位         */
#define __MAG3110_REG_OUT_X_LSB         0x02    /**< \brief X轴数据的低八位         */
#define __MAG3110_REG_OUT_Y_MSB         0x03    /**< \brief Y轴数据的高八位         */
#define __MAG3110_REG_OUT_Y_LSB         0x04    /**< \brief Y轴数据的低八位         */
#define __MAG3110_REG_OUT_Z_MSB         0x05    /**< \brief Z轴数据的高八位         */
#define __MAG3110_REG_OUT_Z_LSB         0x06    /**< \brief Z轴数据的低八位         */
#define __MAG3110_REG_WHO_AM_I          0x07    /**< \brief 器件ID寄存器            */
#define __MAG3110_REG_SYSMOD            0x08    /**< \brief 系统模式寄存器          */
#define __MAG3110_REG_OFF_X_MSB         0x09    /**< \brief X轴高八位的用户定义补偿 */
#define __MAG3110_REG_OFF_X_LSB         0x0A    /**< \brief X轴低八位的用户定义补偿 */
#define __MAG3110_REG_OFF_Y_MSB         0x0B    /**< \brief Y轴高八位的用户定义补偿 */
#define __MAG3110_REG_OFF_Y_LSB         0x0C    /**< \brief Y轴低八位的用户定义补偿 */
#define __MAG3110_REG_OFF_Z_MSB         0x0D    /**< \brief Z轴高八位的用户定义补偿 */
#define __MAG3110_REG_OFF_Z_LSB         0x0E    /**< \brief Z轴低八位的用户定义补偿 */
#define __MAG3110_REG_DIE_TEMP          0x0F    /**< \brief 器件内部温度寄存器      */
#define __MAG3110_REG_CTRL_REG1         0x10    /**< \brief 控制寄存器1             */
#define __MAG3110_REG_CTRL_REG2         0x11    /**< \brief 控制寄存器2             */

#define __MAG3110_MY_ID                 0xC4    /**< \brief ID值                    */
#define __MAG3110_CMD_AUTO_MRST_EN      (0x01 << 7)/**< \brief 自动磁传感器复位命令 */

/** \brief 几种输出数据速率和过采样率，更多设置见数据手册 */
#define __MAG3110_DR80_OS16             (0x00)/**< \brief 输出数据速率80Hz，过采样率16      */
#define __MAG3110_DR10_OS128            (0x03)/**< \brief 输出数据速率10Hz，过采样率128     */
#define __MAG3110_DR1_25_OS64           (0x12)/**< \brief 输出数据速率1.25Hz，过采样率64    */

/** \brief 快速读模式使能或失能 */
#define __MAG3110_FR_DISABLE            0x00/**< \brief 正常读模式 */
#define __MAG3110_FR_ENABLE             0x01/**< \brief 快速读模式 */

/** \brief 触发即时测量 */
#define __MAG3110_TMI_DISABLE           0x00/**< \brief 基于AC条件的正常操作    */
#define __MAG3110_TMI_ENABLE            0x01/**< \brief 立即触发测量            */

/** \brief 操作模式 */
#define __MAG3110_STANDBY_MODE          0x00/**< \brief STANDBY模式 */
#define __MAG3110_ACTIVE_MODE           0x01/**< \brief ACTIVE模式  */

/** \brief 数据输出校正 */
#define __MAG3110_CAL_ENABLE            0x00/**< \brief 正常模式：数据值由用户补偿寄存器值修正      */
#define __MAG3110_CAL_DISABLE           0x01/**< \brief 原始模式：数据值不会被用户补偿寄存器值修正  */

/** \brief 设置输出数据速率和过采样率 */
#define __MAG3110_DR_OS_SET(data, mask) (((data) & (~(0xF8 << 3))) | (mask << 3))

/** \brief 快速读选择 */
#define __MAG3110_FR_SET(data, mask)    (((data) & (~(0x01 << 2))) | (mask << 2))

/** \brief 设置触发测量 */
#define __MAG3110_TM_SET(data, mask)    (((data) & (~(0x01 << 1))) | (mask << 1))

/** \brief 操作模式选择 */
#define __MAG3110_OM_SET(data, mask)    (((data) & (~(0x01 << 0))) | (mask << 0))

/** \brief 数据输出校正设置 */
#define __MAG3110_CAL_SET(data, mask)   (((data) & (~(0x01 << 5))) | (mask << 5))

/** \brief 获取三轴数据状态位 */
#define __MAG3110_GET_ZYXDR_STATUS(reg) ((reg >> 3) & 0x1)

/** \brief 将两个int8转换为一个int16_t类型 */
#define __MAG3110_UINT8_TO_UINT16(buff) \
                       (int16_t)(((int8_t)(buff[0]) << 8) \
                               | ((int8_t)(buff[1])))

/** \brief 计算温度,并扩大10^6倍 */
#define __MAG3110_GET_TEMP_VALUE(data) \
            ((int32_t)((22 + (int8_t)(data)) * 1000000))

/** \brief 计算磁力,并扩大10^6倍 (UNIT:gauss)*/
#define __MAG3110_GET_MAG_VALUE(data)  ((int32_t)(data * 1000))
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
am_local am_const struct am_sensor_drv_funcs __g_sensor_mag3110_funcs = {
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
 * \brief MAG3110 写数据
 */
am_local am_err_t __mag3110_write (am_sensor_mag3110_dev_t *p_this,
                                   uint32_t                subaddr,
                                   uint8_t                *p_buf,
                                   uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief MAG3110 读数据
 */
am_local am_err_t __mag3110_read (am_sensor_mag3110_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief MAG3110 获取校准值
 */
#if 0
am_local am_err_t __mag3110_get_cal_value(am_sensor_mag3110_dev_t* p_this)
{
    static int  mag_x_max = 0,    /* 存储旋转一圈中XYZ三轴的最大值 */
                mag_y_max = 0, 
                mag_z_max = 0;
    static int  mag_x_min = 0,    /* 存储旋转一圈中XYZ三轴的最小值 */
                mag_y_min = 0,  
                mag_z_min = 0;

    static int  mag_off_x = 0,    /* XYZ三轴的中间标定值 */
                mag_off_y = 0,  
                mag_off_z = 0;

    uint8_t buff[2] = {0};
    uint32_t     i = 20000;
    uint8_t ctrl_reg1 = 0;
    uint8_t ctrl_reg2  = 0;

    int32_t tem_data   = 0;

    am_err_t ret = AM_OK;

    for ( ;i > 0; i--) {
        /* 获取X轴的最大最小磁场 */
        ret = __mag3110_read(p_this, __MAG3110_REG_OUT_X_MSB, buff, 2);
        if (ret != AM_OK) {
            return ret;
        }
        tem_data = __MAG3110_UINT8_TO_UINT16(buff);

        if (tem_data > mag_x_max) {
            mag_x_max = tem_data;
        } else if (tem_data < mag_x_min) {
            mag_x_min = tem_data;
        }

        /* 获取Y轴的最大最小磁场 */
        ret = __mag3110_read(p_this, __MAG3110_REG_OUT_Y_MSB, buff, 2);
        if (ret != AM_OK) {
            return ret;
        }
        tem_data = __MAG3110_UINT8_TO_UINT16(buff);

        if (tem_data > mag_y_max) {
            mag_y_max = tem_data;
        } else if (tem_data < mag_y_min) {
            mag_y_min = tem_data;
        }

        /* 获取Z轴的最大最小磁场 */
        ret = __mag3110_read(p_this, __MAG3110_REG_OUT_Z_MSB, buff, 2);
        if (ret != AM_OK) {
            return ret;
        }
        tem_data = __MAG3110_UINT8_TO_UINT16(buff);

        if (tem_data > mag_z_max) {
            mag_z_max = tem_data;
        } else if (tem_data < mag_z_min) {
            mag_z_min = tem_data;
        }

        /* 轮询状态下默认STANDBY模式，TM置1，启动下一个测量 */
        ret = __mag3110_read(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
        if (ret != AM_OK) {
            return -AM_EPERM;
        }
        ctrl_reg1 = __MAG3110_TM_SET(ctrl_reg1, __MAG3110_TMI_ENABLE);
        ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
        if (ret != AM_OK) {
            return -AM_EPERM;
        }

    }

    mag_off_x = (mag_x_max + mag_x_min) >> 1;      /* 得到X轴的中间标定值          */
    mag_off_y = (mag_y_max + mag_y_min) >> 1;      /* 得到Y轴的中间标定值          */
    mag_off_z = -(mag_z_max + mag_z_min) >> 1;     /* 得到Z轴的中间标定值          */

    if ((mag_off_x < -10000) || (mag_off_x > 10000) ||
        (mag_off_y < -10000) || (mag_off_y > 10000) ||
        (mag_off_z < -10000) || (mag_off_z > 10000) ) {
        am_kprintf("Calibration value is out of range! \r\n");
    } else {
        mag_off_x <<= 1;
        mag_off_y <<= 1;
        mag_off_z <<= 1;

        buff[0] = ((mag_off_x >> 8) & 0xFF);
        buff[1] = (mag_off_x & 0xFF);
        __mag3110_write(p_this, __MAG3110_REG_OFF_X_MSB, buff, 2);
        buff[0] = ((mag_off_y >> 8) & 0xFF);
        buff[1] = (mag_off_y & 0xFF);
        __mag3110_write(p_this, __MAG3110_REG_OFF_Y_MSB, buff, 2);
        buff[0] = ((mag_off_z >> 8) & 0xFF);
        buff[1] = (mag_off_z & 0xFF);
        __mag3110_write(p_this, __MAG3110_REG_OFF_X_MSB, buff, 2);

        /* RAW = 0，使能校正 */
        ret = __mag3110_read(p_this, __MAG3110_REG_CTRL_REG2, &ctrl_reg2, 1);
        if (ret != AM_OK) {
            return ret;
        }
        ctrl_reg2 = __MAG3110_CAL_SET(ctrl_reg2, __MAG3110_CAL_ENABLE);
        ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG2, &ctrl_reg2, 1);
        if (ret != AM_OK) {
            return ret;
        }
    }

    return ret;
}
#endif

/**
 * \brief 将结果转换成磁力实际值
 * Sensitivity: 0.1uT/LSB
 * Full-Scale range: +-1000 uT (+-10 gauss)
 * data range: +-30000 LSB
 * 单位换算 ：1 uT = 0.01 gauss
 */
am_local int32_t __mag3110_get_accel_value (am_sensor_mag3110_dev_t* p_this,
                                            int32_t                  data)
{
    int32_t real_data = 0;

    if (p_this == NULL) {
        return -AM_EINVAL;
    }
//    am_kprintf("Data: %d\r\n",data);
    real_data = __MAG3110_GET_MAG_VALUE(data);

    return real_data;
}

/**
 * \brief MAG3110 XY磁场数据转换为角度
 */
void __mag3110_mag_to_angle(int16_t mag_x_data, int16_t mag_y_data)
{
    int16_t mag3110_angle = 0;

    if (mag_x_data == 0) {          /* 处理x轴数据为0的情况         */
        if (mag_y_data > 0) {
            mag3110_angle = 90;
        } else {
            mag3110_angle = 270;
        }
    } else if (mag_y_data == 0) {   /* 处理y轴数据为0的情况         */
        if (mag_x_data > 0) {
            mag3110_angle = 0;
        } else {
            mag3110_angle = 180;
        }
    } else if ((mag_x_data > 0) && (mag_y_data > 0)) {      /* 将第一象限的弧度转换成角度   */
        mag3110_angle = (atan(((float)mag_y_data) / ((float)mag_x_data))) * 180 / 3.14;
    } else if ((mag_x_data < 0) && (mag_y_data > 0)) {      /* 将第二象限的弧度转换成角度   */
        mag_x_data = -mag_x_data;
        mag3110_angle   = 180 - (atan(((float)mag_y_data) / ((float)mag_x_data))) * 180 / 3.14;
    } else if ((mag_x_data < 0) && (mag_y_data < 0)) {      /* 将第三象限的弧度转换成角度   */
        mag_x_data = -mag_x_data;
        mag_y_data = -mag_y_data;
        mag3110_angle   = (atan(((float)mag_y_data) / ((float)mag_x_data)))* 180 / 3.14 + 180;
    } else if ((mag_x_data > 0) && (mag_y_data < 0)) {      /* 将第四象限的弧度转换成角度   */
        mag_y_data = -mag_y_data;
        mag3110_angle   = 360 - (atan(((float)mag_y_data) / ((float)mag_x_data))) * 180 / 3.14;
    }

    am_kprintf("X轴偏离正北%d \r\n",mag3110_angle);
}

/** \breif 中断回调函数 */
am_local void __mag3110_alarm_callback (void *p_arg)
{
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_arg;

    uint8_t reg_data[2];
    int32_t tem_data = 0;
    uint8_t i = 0;

    /** \brief 获取X轴磁力 */
    __mag3110_read(p_this, __MAG3110_REG_OUT_X_MSB, reg_data, 2);
    
    tem_data = __MAG3110_UINT8_TO_UINT16(reg_data);

    /** \brief X轴磁力 */
    p_this->data[0].val = __mag3110_get_accel_value(p_this, tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取Y轴磁力 */
    __mag3110_read(p_this, __MAG3110_REG_OUT_Y_MSB, reg_data, 2);

    tem_data = __MAG3110_UINT8_TO_UINT16(reg_data);
   
    /** \brief Y轴磁力 */
    p_this->data[1].val = __mag3110_get_accel_value(p_this, tem_data); 
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取Z轴磁力 */
    __mag3110_read(p_this, __MAG3110_REG_OUT_Z_MSB, reg_data, 2);
    
    tem_data = __MAG3110_UINT8_TO_UINT16(reg_data);
   
    /** \brief Z轴磁力 */
    p_this->data[2].val = __mag3110_get_accel_value(p_this, tem_data); 
    p_this->data[2].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取温度 */
    __mag3110_read(p_this, __MAG3110_REG_DIE_TEMP, reg_data, 1);

    tem_data = (int32_t)(reg_data[0]);

    /** \brief 温度 */
    p_this->data[3].val = __MAG3110_GET_TEMP_VALUE(tem_data); 
    p_this->data[3].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/    

//    __mag3110_mag_to_angle((p_this->data[0].val/1000), (p_this->data[1].val/1000));

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

    if (id == AM_MAG3110_CHAN_1 || id == AM_MAG3110_CHAN_2 ||
        id == AM_MAG3110_CHAN_3) {
        return AM_SENSOR_TYPE_MAGNETIC;
    } else if (id == AM_MAG3110_CHAN_4) {
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
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_drv;
    
    am_err_t ret        = AM_OK;
    uint8_t status_val  = 0;
    uint8_t ctrl_reg1 = 0;
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

    /** \brief 获取可读状态值 */
    do {
        ret = __mag3110_read(p_this, __MAG3110_REG_DR_STATUS, &status_val, 1);
        if (ret != AM_OK) {
            return ret;
        }
    } while (__MAG3110_GET_ZYXDR_STATUS(status_val) != 0x1);

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            /** \brief 获取X轴磁力 */
            ret = __mag3110_read(p_this, __MAG3110_REG_OUT_X_MSB, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __MAG3110_UINT8_TO_UINT16(reg_data);
           
            /** \brief X轴磁力 */
            p_buf[i].val = __mag3110_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 1) {

            /** \brief 获取Y轴磁力 */
            ret = __mag3110_read(p_this, __MAG3110_REG_OUT_Y_MSB, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __MAG3110_UINT8_TO_UINT16(reg_data);
           
            /** \brief Y轴磁力 */
            p_buf[i].val = __mag3110_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 2) {

            /** \brief 获取Z轴磁力 */
            ret = __mag3110_read(p_this, __MAG3110_REG_OUT_Z_MSB, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __MAG3110_UINT8_TO_UINT16(reg_data);
           
            /** \brief Z轴磁力 */
            p_buf[i].val = __mag3110_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 3) {

            /** \brief 获取温度 */
            ret = __mag3110_read(p_this, __MAG3110_REG_DIE_TEMP, reg_data, 1);
            if (ret != AM_OK) {
                return ret;
            }

            tem_data = (int32_t)(reg_data[0]);

            /** \brief 温度 */
            p_buf[i].val = __MAG3110_GET_TEMP_VALUE(tem_data);
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为0:10^(-6)*/

        } else {
            return -AM_ENODEV;  /*< \brief 若此次通道不属于该传感器，直接返回 */
        }
    }

//    __mag3110_mag_to_angle((p_buf[0].val/1000), (p_buf[1].val/1000));

    /* 轮询状态下默认STANDBY模式，TM置1，启动下一个测量 */
    ret = __mag3110_read(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        return -AM_EPERM;
    }
    ctrl_reg1 = __MAG3110_TM_SET(ctrl_reg1, __MAG3110_TMI_ENABLE);
    ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        return -AM_EPERM;
    }

    return ret;
}

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result)
{
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t ctrl_reg1 = 0;
    uint8_t ctrl_reg2 = 0;

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

    if (ret != AM_OK) {    /**< \brief 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 15);
    }    

    /** \brief 首先获取控制寄存器1、2的值 */
    ret = __mag3110_read(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    ret = __mag3110_read(p_this, __MAG3110_REG_CTRL_REG2, &ctrl_reg2, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }

    /** \brief RAW = 1，数据值不会被用户补偿寄存器值更正 */
    ctrl_reg2 = __MAG3110_CAL_SET(ctrl_reg2, __MAG3110_CAL_DISABLE);
    ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG2, &ctrl_reg2, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }

    /** \brief 进入STANDBY模式，才能修改控制寄存器1内除AC位的其他字段 */
    ctrl_reg1 = __MAG3110_OM_SET(ctrl_reg1, __MAG3110_STANDBY_MODE);
    ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }

    /** \brief 设置输出数据速率10Hz，过采样率128 */
    ctrl_reg1 = __MAG3110_DR_OS_SET(ctrl_reg1, __MAG3110_DR10_OS128);

    /** \brief 禁能快速读模式 */
    ctrl_reg1 = __MAG3110_FR_SET(ctrl_reg1, __MAG3110_FR_DISABLE);

    /** \brief 操作模式选择，STANDBY模式 */
    ctrl_reg1 = __MAG3110_OM_SET(ctrl_reg1, __MAG3110_STANDBY_MODE);

    /** \brief 默认轮询状态，TM置1，立即触发测量 */
    ctrl_reg1 = __MAG3110_TM_SET(ctrl_reg1, __MAG3110_TMI_ENABLE);

    ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
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
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

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

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 15);
        }
    }

    return cur_ret;
}

/** \brief 配置传感器通道属性 */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val)
{
    return -AM_ENOTSUP;
}

/** \brief 获取传感器通道属性 */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val)
{
    return -AM_ENOTSUP;
}

/** \brief 设置触发，一个通道仅能设置一个触发回调函数 */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg)
{
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_drv;

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
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;
    uint8_t  ctrl_reg1 = 0;

    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x10) {
        return AM_OK;
    }

    ret = __mag3110_read(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* 先进入STANDBY模式 */
    ctrl_reg1 = __MAG3110_OM_SET(ctrl_reg1, __MAG3110_STANDBY_MODE);
    ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* TM位清零，根据数据速率和过采样率中的值进行定期测量 */
    ctrl_reg1 = __MAG3110_TM_SET(ctrl_reg1, __MAG3110_TMI_DISABLE);

    /* 进入ACTIVE模式 */
    ctrl_reg1 = __MAG3110_OM_SET(ctrl_reg1, __MAG3110_ACTIVE_MODE);
    ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
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
    am_sensor_mag3110_dev_t* p_this = (am_sensor_mag3110_dev_t*)p_drv;

    am_err_t cur_ret = AM_OK;
    am_err_t ret     = AM_OK;

    uint8_t ctrl_reg1  = 0;

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

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 4);
        }

        /* 恢复轮询状态，先进入STANDBY模式 */
        __mag3110_read(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
        ctrl_reg1 = __MAG3110_OM_SET(ctrl_reg1, __MAG3110_STANDBY_MODE);
        __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);

        /* 再将TM置1 */
        ctrl_reg1 = __MAG3110_TM_SET(ctrl_reg1, __MAG3110_TMI_ENABLE);
        ret = __mag3110_write(p_this, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 MAG3110 初始化
 */
am_sensor_handle_t am_sensor_mag3110_init (
        am_sensor_mag3110_dev_t           *p_dev,
        const am_sensor_mag3110_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t ctrl_reg1  = 0;
    uint8_t ctrl_reg2  = 0;
    uint8_t mag3110_id = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->mag3110_dev.p_drv   = p_dev;
    p_dev->mag3110_dev.p_funcs = &__g_sensor_mag3110_funcs;
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
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __mag3110_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_RISE);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    /* 读取并检测ID */
    ret = __mag3110_read(p_dev, __MAG3110_REG_WHO_AM_I, &mag3110_id, 1);
    if (ret != AM_OK || mag3110_id != __MAG3110_MY_ID) {
        cur_ret = ret;
        am_kprintf("Device ID:%x \r\n", mag3110_id);
    } else {

//        /* 获取磁力校准值 */
//        ret = __mag3110_get_cal_value(p_dev);
//        if (ret != AM_OK) {
//            cur_ret = ret;
//        }
    }

    /* 首先获取控制寄存器1、2的值 */
    ret = __mag3110_read(p_dev, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        cur_ret = ret;
    }
    ret = __mag3110_read(p_dev, __MAG3110_REG_CTRL_REG2, &ctrl_reg2, 1);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* 进入STANDBY模式 */
    ctrl_reg1 = __MAG3110_OM_SET(ctrl_reg1, __MAG3110_STANDBY_MODE);
    ret = __mag3110_write(p_dev, __MAG3110_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* 启用自动磁传感器复位 */
    ctrl_reg2 |= __MAG3110_CMD_AUTO_MRST_EN; 
    ret = __mag3110_write(p_dev, __MAG3110_REG_CTRL_REG2, &ctrl_reg2, 1);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor MAG3110 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->mag3110_dev);
}

/**
 * \brief 传感器 MAG3110 去初始化
 */
am_err_t am_sensor_mag3110_deinit (am_sensor_handle_t handle)
{
    am_sensor_mag3110_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->mag3110_dev.p_drv   = NULL;
    p_dev->mag3110_dev.p_funcs = NULL;
    p_dev->dev_info            = NULL;

    return AM_OK;
}


/* end of file */
