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
 * \brief 传感器 LPS22HB 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-03  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_lps22hb.h"
#include "am_gpio.h"
#include "am_vdebug.h"

/*******************************************************************************
 * 宏定义
 ******************************************************************************/
#define  __LPS22HB_REG_INTERRUPT_CFG    0X0B  /**< \brief 中断寄存器地址      */   
#define  __LPS22HB_REG_THS_P_L          0X0C  /**< \brief 压力阈值低字节地址  */
#define  __LPS22HB_REG_THS_P_H          0X0D  /**< \brief 压力阈值高字节地址  */
#define  __LPS22HB_REG_WHO_AM_I         0X0F  /**< \brief ID寄存器地址        */
#define  __LPS22HB_REG_CTRL_REG1        0X10  /**< \brief 控制寄存器1地址     */
#define  __LPS22HB_REG_CTRL_REG2        0X11  /**< \brief 控制寄存器2地址     */
#define  __LPS22HB_REG_CTRL_REG3        0X12  /**< \brief 控制寄存器3地址     */
#define  __LPS22HB_REG_FIFO_CTRL        0X14  /**< \brief FIFO配置寄存器地址  */
#define  __LPS22HB_REG_REF_P_XL         0X15  /**< \brief 参考压力低字节地址  */
#define  __LPS22HB_REG_REF_P_L          0X16  /**< \brief 参考压力次低字节地址*/
#define  __LPS22HB_REG_REF_P_H          0X17  /**< \brief 参考压力高字节地址  */
#define  __LPS22HB_REG_RPDS_L           0X18  /**< \brief 压力补偿低字节地址  */
#define  __LPS22HB_REG_RPDS_H           0X19  /**< \brief 压力补偿高字节地址  */                                       
#define  __LPS22HB_REG_RES_CONF         0X1A  /**< \brief 分辨率寄存器地址    */
#define  __LPS22HB_REG_INT_SOURCE       0X25  /**< \brief 中断源寄存器地址    */
#define  __LPS22HB_REG_FIFO_STATUS      0X26  /**< \brief FIFO状态寄存器地址  */
#define  __LPS22HB_REG_STATUS           0X27  /**< \brief 状态寄存器地址      */
#define  __LPS22HB_REG_PRESS_OUT_XL     0X28  /**< \brief 压力低字节地址      */
#define  __LPS22HB_REG_PRESS_OUT_L      0X29  /**< \brief 压力次低字节地址    */
#define  __LPS22HB_REG_PRESS_OUT_H      0X2A  /**< \brief 压力高字节地址      */
#define  __LPS22HB_REG_TEMP_OUT_L       0X2B  /**< \brief 温度低字节地址      */
#define  __LPS22HB_REG_TEMP_OUT_H       0X2C  /**< \brief 温度高字节地址      */
#define  __LPS22HB_REG_LPFP_RES         0X33  /**< \brief 滤波器复位地址      */

#define  __LPS22HB_CMD_RESET            0X04  /**< \brief 复位命令            */
#define  __LPS22HB_MY_ID                0XB1  /**< \brief ID值                */

#define  __LPS22HB_DIFF_EN       (0x01<<3)    /**< \brief 使能中断            */
#define  __LPS22HB_TRIGGER_READ  (0x01<<2)    /**< \brief 使能数据可读触发    */
#define  __LPS22HB_TRIGGER_CLOSE (~(0x01<<2)) /**< \brief 关闭数据可读触发    */

/** \brief 设置频率为一次 */
#define __LPS22HB_RATE_SET_ONE(data) (((data) & (~(0x7 << 4))) | (0x0 << 4))

/** \brief 设置频率为1 */
#define __LPS22HB_RATE_SET_1S(data)  (((data) & (~(0x7 << 4))) | (0x1 << 4))

/** \brief 设置频率为10 */
#define __LPS22HB_RATE_SET_10S(data) (((data) & (~(0x7 << 4))) | (0x2 << 4))

/** \brief 设置频率为25 */
#define __LPS22HB_RATE_SET_25S(data) (((data) & (~(0x7 << 4))) | (0x3 << 4))

/** \brief 设置频率为50 */
#define __LPS22HB_RATE_SET_50S(data) (((data) & (~(0x7 << 4))) | (0x4 << 4))

/** \brief 设置频率为75 */
#define __LPS22HB_RATE_SET_75S(data) (((data) & (~(0x7 << 4))) | (0x5 << 4))

/** \brief 开始一次数据采集 */
#define __LPS22HB_ONE_SHOT_START     (0x1)

/** \brief 获取PRESS状态位 */
#define __LPS22HB_GET_PRESS_STATUS(reg) (reg & 0x1)

/** \brief 获取TEM状态位 */
#define __LPS22HB_GET_TEM_STATUS(reg)   ((reg >> 1) & 0x1)

/** \brief 将两个int8转换为一个int16_t类型 */
#define __LPS22HB_UINT8_TO_UINT16(buff) \
                       (int16_t)(((uint16_t)(buff[1]) << 8) \
                               | ((uint16_t)(buff[0])))

/** \brief 将三个int8转换为一个int32_t类型 */
#define __LPS22HB_UINT8_TO_UINT32(buff) \
                       (uint32_t)(((uint32_t)(buff[2]) << 16) \
                               | ((uint32_t)(buff[1]) << 8)  \
                               | ((uint32_t)(buff[0])))

/** \brief 计算压强 */
#define __LPS22HB_GET_PRESS_VALUE(data)  ((double)(data) * 100 / 4096)
 
/** \brief 计算温度,并扩大10^6倍 */
#define __LPS22HB_GET_TEM_VALUE(data)    (data *1000000 / 100)

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_lps22hb_funcs = {
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
 * \brief LPS22HB 写数据
 */
am_local am_err_t __lps22hb_write (am_sensor_lps22hb_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief LPS22HB 读数据
 */
am_local am_err_t __lps22hb_read (am_sensor_lps22hb_dev_t *p_this,
                                 uint32_t                subaddr,
                                 uint8_t                *p_buf,
                                 uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/** \breif 中断回调函数 */
am_local void __lps22hb_alarm_callback (void *p_arg)
{
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_arg;

    uint8_t reg_data[3];
    int32_t tem_data   = 0;

    /** \brief 获取压力值 */
    __lps22hb_read(p_this, __LPS22HB_REG_PRESS_OUT_XL, reg_data, 3);

    tem_data = __LPS22HB_UINT8_TO_UINT32(reg_data);
   
    /** \brief 压力 */
    p_this->data[0].val = __LPS22HB_GET_PRESS_VALUE(tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_BASE; /*< \brief 单位默认为0:10^(0)*/

    /** \brief 获取温度 */
    __lps22hb_read(p_this, __LPS22HB_REG_TEMP_OUT_L, reg_data, 2);

    tem_data = __LPS22HB_UINT8_TO_UINT16(reg_data);
   
    /** \brief 温度 */
    p_this->data[1].val  = __LPS22HB_GET_TEM_VALUE(tem_data); 
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为-6:10^(-6)*/
    
    if (p_this->pfn_trigger_cb[0] &&
            (p_this->flags[0] & AM_SENSOR_TRIGGER_DATA_READY)) {
        p_this->pfn_trigger_cb[0](p_this->p_arg[0],
                                  AM_SENSOR_TRIGGER_DATA_READY);
    }

    if (p_this->pfn_trigger_cb[1] &&
            (p_this->flags[1] & AM_SENSOR_TRIGGER_DATA_READY)) {
        p_this->pfn_trigger_cb[1](p_this->p_arg[1],
                                  AM_SENSOR_TRIGGER_DATA_READY);
    }
}

/**
 * \brief 配置选择
 */
am_local am_err_t __reg_attr_set (am_sensor_lps22hb_dev_t *p_this, uint8_t rate)
{
     am_err_t ret = AM_OK;

     uint8_t rate_cfg = 0;

     ret = __lps22hb_read(p_this, __LPS22HB_REG_CTRL_REG1, &rate_cfg, 1);
     if (ret != AM_OK) {
         return ret;
     }

     switch (rate) {
     case 0:
         rate_cfg = __LPS22HB_RATE_SET_ONE(rate_cfg);
         break;
     case 1:
         rate_cfg = __LPS22HB_RATE_SET_1S(rate_cfg);
         break;
     case 2:
         rate_cfg = __LPS22HB_RATE_SET_10S(rate_cfg);
         break;
     case 3:
         rate_cfg = __LPS22HB_RATE_SET_25S(rate_cfg);
         break;
     case 4:
         rate_cfg = __LPS22HB_RATE_SET_50S(rate_cfg);
         break;
     case 5:
         rate_cfg = __LPS22HB_RATE_SET_75S(rate_cfg);
         break;     
     default:
         break;
     }

     return __lps22hb_write(p_this, __LPS22HB_REG_CTRL_REG1, &rate_cfg, 1);
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_LPS22HB_CHAN_1) {
        return AM_SENSOR_TYPE_PRESS;
    } else if (id == AM_LPS22HB_CHAN_2) {
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
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;
    
    am_err_t ret        = AM_OK;
    uint8_t status_val  = 0;
    uint8_t reg_data[3] = {0};
    uint32_t tem_data   = 0;

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

    /* 若为1，则可能在数据准备就绪触发回调函数中使用 */
    if (num == 1) {
        cur_id = p_ids[0];
        /* 若打开数据准备就绪触发方式，则直接赋值 */
        if ((AM_BIT_GET(p_this->trigger, 2)) &&
                ((p_this->flags[0] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->flags[1] & AM_SENSOR_TRIGGER_DATA_READY))) {
            p_buf[0].val = p_this->data[cur_id].val;
            p_buf[0].unit= p_this->data[cur_id].unit;
            return AM_OK;
        }
    }    
    
    /** \brief 获取可读状态值 */
    do {
        ret = __lps22hb_read(p_this, __LPS22HB_REG_STATUS, &status_val, 1);
        if (ret != AM_OK) {
            return ret;
        }
    } while (__LPS22HB_GET_PRESS_STATUS(status_val) != 0x1 ||
             __LPS22HB_GET_TEM_STATUS(status_val) != 0x1);

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            /** \brief 获取压力值 */
            ret = __lps22hb_read(p_this, __LPS22HB_REG_PRESS_OUT_XL, reg_data, 3);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LPS22HB_UINT8_TO_UINT32(reg_data);
           
            /** \brief 压力 */
            p_buf[i].val = __LPS22HB_GET_PRESS_VALUE(tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_BASE; /*< \brief 单位默认为0:10^(0)*/

        } else if (cur_id == 1) {

            /** \brief 获取温度 */
            ret = __lps22hb_read(p_this, __LPS22HB_REG_TEMP_OUT_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LPS22HB_UINT8_TO_UINT16(reg_data);
           
            /** \brief 温度 */
            p_buf[i].val = __LPS22HB_GET_TEM_VALUE(tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为-6:10^(-6)*/

        }else {
            return -AM_ENODEV;  /*< \brief 若此次通道不属于该传感器，直接返回 */
        }
    }
    
    ret = __lps22hb_read(p_this, __LPS22HB_REG_CTRL_REG2, &status_val, 1);
    if (ret != AM_OK) {
        return ret;
    }

    status_val |= __LPS22HB_ONE_SHOT_START;
    ret = __lps22hb_write(p_this, __LPS22HB_REG_CTRL_REG2, &status_val, 1);
    if (ret != AM_OK) {
        return ret;
    }        
    return ret;
}

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result)
{
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    uint8_t open_one_shot = 0;
    
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

    if (ret != AM_OK) {    /**< \breif 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 7);
        AM_BIT_SET(p_this->trigger, 6);
        AM_BIT_SET(p_this->trigger, 5);
    }    
    
    ret = __lps22hb_read(p_this, __LPS22HB_REG_CTRL_REG2, &open_one_shot, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }

    open_one_shot |= __LPS22HB_ONE_SHOT_START;
    ret = __lps22hb_write(p_this, __LPS22HB_REG_CTRL_REG2, &open_one_shot, 1);
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
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;

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

/** \brief 配置传感器通道属性 */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val)
{
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    am_sensor_val_t sensor_val = {0, 0};

    am_sensor_val_t __sampling_rate = {0, 0};

    uint8_t rate = 0;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if ((id != AM_LPS22HB_CHAN_1) && (id != AM_LPS22HB_CHAN_2)) {
        return -AM_ENODEV;
    }

    switch (attr) {

    /* 修改频率 */
    case AM_SENSOR_ATTR_SAMPLING_RATE:

        if (p_val->val < 0 || p_val->val > 75) {
            return -AM_ENOTSUP;
        }

        sensor_val.unit = p_val->unit;
        sensor_val.val  = p_val->val;
        am_sensor_val_unit_convert(&sensor_val, 1, AM_SENSOR_UNIT_BASE);

        if (sensor_val.val == 0) {
            rate = 0;
            __sampling_rate.val = 0;
        } else if (sensor_val.val > 0 && sensor_val.val < 5) {
            rate = 1;
            __sampling_rate.val = 1;
        } else if (sensor_val.val >= 5 && sensor_val.val < 18) {
            rate = 2;
            __sampling_rate.val = 10;
        } else if (sensor_val.val >= 18 && sensor_val.val < 38) {
            rate = 3;
            __sampling_rate.val = 25;
        } else if (sensor_val.val >= 38 && sensor_val.val < 52) {
            rate = 4;
            __sampling_rate.val = 50;
        } else if (sensor_val.val >= 52 && sensor_val.val <= 75) {
            rate = 5;
            __sampling_rate.val = 75;
        }
        
        ret = __reg_attr_set(p_this, rate);
        if (ret != AM_OK) {
            cur_ret = ret;
        } else {
            p_this->sam_rate.val  = __sampling_rate.val;
            p_this->sam_rate.unit = __sampling_rate.unit;
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
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;

    am_err_t ret = AM_OK;

    if ((id != AM_LPS22HB_CHAN_1) && (id != AM_LPS22HB_CHAN_2)) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (attr) {

    /* 获取采样频率 */
    case AM_SENSOR_ATTR_SAMPLING_RATE:

        p_val->val  = p_this->sam_rate.val;
        p_val->unit = p_this->sam_rate.unit;
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
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;

    if (id != 0 && id != 1) {
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
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t reg_data = 0;
    uint8_t reg_clear[5];

    if (id != 0 && id != 1) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x4) {
        return AM_OK;
    }

    ret = __lps22hb_read(p_this, __LPS22HB_REG_INTERRUPT_CFG, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief 使能中断生成 */
    reg_data |= __LPS22HB_DIFF_EN;
    ret = __lps22hb_write(p_this, __LPS22HB_REG_INTERRUPT_CFG, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }

    ret = __lps22hb_read(p_this, __LPS22HB_REG_CTRL_REG3, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief 使能数据可读触发 */
    reg_data |= __LPS22HB_TRIGGER_READ;
    ret = __lps22hb_write(p_this, __LPS22HB_REG_CTRL_REG3, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }

    /* 配置触发引脚 */
    if (p_this->dev_info->trigger_pin != -1) {    
        __lps22hb_read(p_this, __LPS22HB_REG_PRESS_OUT_XL, reg_clear, 5);
        ret = am_gpio_trigger_on(p_this->dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }

    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->trigger, 0);
        AM_BIT_SET(p_this->trigger, 2);
    }
    if (cur_ret == AM_OK && id == 1) {
        AM_BIT_SET(p_this->trigger, 1);
        AM_BIT_SET(p_this->trigger, 2);
    }

    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_lps22hb_dev_t* p_this = (am_sensor_lps22hb_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t reg_data = 0;

    am_sensor_val_t p_val = {0, 0};

    if (id != 0 && id != 1) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_this->pfn_trigger_cb[id] = NULL;
    p_this->p_arg[id]          = NULL;
    p_this->flags[id]          = 0;

    AM_BIT_CLR(p_this->trigger, id);

    if ((p_this->trigger & 0x3) == 0) {

        ret = __lps22hb_read(p_this, __LPS22HB_REG_CTRL_REG3, &reg_data, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }

        /** \brief 禁能数据可读触发 */
        reg_data &= __LPS22HB_TRIGGER_CLOSE;
        ret = __lps22hb_write(p_this, __LPS22HB_REG_CTRL_REG3, &reg_data, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }

        ret = __pfn_attr_set(p_drv, id, AM_SENSOR_ATTR_SAMPLING_RATE, &p_val);
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
            AM_BIT_CLR(p_this->trigger, 2);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 LPS22HB 初始化
 */
am_sensor_handle_t am_sensor_lps22hb_init (
        am_sensor_lps22hb_dev_t           *p_dev,
        const am_sensor_lps22hb_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;
    
    uint8_t ctrl_reg2  = 0;
    uint8_t lps22hb_id = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->lps22hb_dev.p_drv   = p_dev;
    p_dev->lps22hb_dev.p_funcs = &__g_sensor_lps22hb_funcs;
    p_dev->dev_info            = p_devinfo;
    p_dev->pfn_trigger_cb[0]   = NULL;
    p_dev->pfn_trigger_cb[1]   = NULL;
    p_dev->p_arg[0]            = NULL;
    p_dev->p_arg[1]            = NULL;
    p_dev->flags[0]            = 0;
    p_dev->flags[1]            = 0;
    p_dev->sam_rate.val        = 0;
    p_dev->sam_rate.unit       = 0;
    p_dev->trigger             = 0;
    p_dev->data[0].val         = 0;
    p_dev->data[0].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->data[1].val         = 0;
    p_dev->data[1].unit        = AM_SENSOR_UNIT_INVALID;

    /* 外部中断连接 */
    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __lps22hb_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_RISE);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    ret = __lps22hb_read(p_dev, __LPS22HB_REG_CTRL_REG2, &ctrl_reg2, 1); 
    if (ret != AM_OK) {
        cur_ret = ret;
    }
    /* 复位LPS22HB */ 
    ctrl_reg2 |= __LPS22HB_CMD_RESET; 
    ret = __lps22hb_write(p_dev, __LPS22HB_REG_CTRL_REG2, &ctrl_reg2, 1); 
    if (ret != AM_OK) {
        cur_ret = ret;
    }
    
    /* 读取ID */
    ret = __lps22hb_read(p_dev, __LPS22HB_REG_WHO_AM_I, &lps22hb_id, 1);
    if (ret != AM_OK || lps22hb_id != __LPS22HB_MY_ID) {
        cur_ret = ret;
    }

    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor LPS22HB Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->lps22hb_dev);
}

/**
 * \brief 传感器 LPS22HB 去初始化
 */
am_err_t am_sensor_lps22hb_deinit (am_sensor_handle_t handle)
{
    am_sensor_lps22hb_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->lps22hb_dev.p_drv   = NULL;
    p_dev->lps22hb_dev.p_funcs = NULL;
    p_dev->dev_info            = NULL;

    return AM_OK;
}



/* end of file */
