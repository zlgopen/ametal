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
 * \brief 传感器 AS7262 (I2C模式) 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as7262_i2c.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**< 调试信息输出 */
#define __AS7262_DEBUG                              (0)


/************************************************************
 * 宏定义
 ***********************************************************/
/** \brief 物理寄存器 */
#define __AS7262_STATUS_REG                         (0x00)
#define __AS7262_WRITE_REG                          (0x01)
#define __AS7262_READ_REG                           (0x02)
#define __AS7262_TX_VALID                           (0x02)
#define __AS7262_RX_VALID                           (0x01)

/** \brief 虚拟寄存器 */
#define __AS7262_REG_DEVICE_TYPE                    (0x00)
#define __AS7262_REG_HW_VERSION                     (0x01)
#define __AS7262_REG_FW_VERSION_H                   (0x02)
#define __AS7262_REG_FW_VERSION_L                   (0x03)
#define __AS7262_REG_CONTROL_SETUP                  (0x04)
#define __AS7262_REG_INT_T                          (0x05)
#define __AS7262_REG_DEVICE_TEMP                    (0x06)
#define __AS7262_REG_LED_CONTROL                    (0x07)
#define __AS7262_REG_V_H                            (0x08)
#define __AS7262_REG_V_L                            (0x09)
#define __AS7262_REG_B_H                            (0x0A)
#define __AS7262_REG_B_L                            (0x0B)
#define __AS7262_REG_G_H                            (0x0C)
#define __AS7262_REG_G_L                            (0x0D)
#define __AS7262_REG_Y_H                            (0x0E)
#define __AS7262_REG_Y_L                            (0x0F)
#define __AS7262_REG_O_H                            (0x10)
#define __AS7262_REG_O_L                            (0x11)
#define __AS7262_REG_R_H                            (0x12)
#define __AS7262_REG_R_L                            (0x13)
#define __AS7262_REG_V_CAL_XH                       (0x14)
#define __AS7262_REG_V_CAL_H                        (0x15)
#define __AS7262_REG_V_CAL_L                        (0x16)
#define __AS7262_REG_V_CAL_XL                       (0x17)
#define __AS7262_REG_B_CAL_XH                       (0x18)
#define __AS7262_REG_B_CAL_H                        (0x19)
#define __AS7262_REG_B_CAL_L                        (0x1A)
#define __AS7262_REG_B_CAL_XL                       (0x1B)
#define __AS7262_REG_G_CAL_XH                       (0x1C)
#define __AS7262_REG_G_CAL_H                        (0x1D)
#define __AS7262_REG_G_CAL_L                        (0x1E)
#define __AS7262_REG_G_CAL_XL                       (0x1F)
#define __AS7262_REG_Y_CAL_XH                       (0x20)
#define __AS7262_REG_Y_CAL_H                        (0x21)
#define __AS7262_REG_Y_CAL_L                        (0x22)
#define __AS7262_REG_Y_CAL_XL                       (0x23)
#define __AS7262_REG_O_CAL_XH                       (0x24)
#define __AS7262_REG_O_CAL_H                        (0x25)
#define __AS7262_REG_O_CAL_L                        (0x26)
#define __AS7262_REG_O_CAL_XL                       (0x27)
#define __AS7262_REG_R_CAL_XH                       (0x28)
#define __AS7262_REG_R_CAL_H                        (0x29)
#define __AS7262_REG_R_CAL_L                        (0x2A)
#define __AS7262_REG_R_CAL_XL                       (0x2B)

/** \brief AS7262 ID (DEVICE TYPE) */
#define __AS7262_ID                                 (0x40)

/** \brief 软件复位 */
#define __AS7262_SOFTWARE_RESET                     (0x1 << 7)

/** \brief 设置INT */
#define __AS7262_MASK_CONTROL_SETUP_INT             (0x1 << 6)
#define __AS7262_INT_SET(val)                       ((val) << 6)

/** \brief 设置GAIN */
#define __AS7262_MASK_CONTROL_SETUP_GAIN            (0x3 << 4)
#define __AS7262_GAIN_SET(val)                      ((val) << 4)

/** \brief 获取GAIN */
#define __AS7262_CONTROL_SETUP_GAIN_GET(reg)        (((reg) >> 4) & 0x3)

/** \brief 设置BANK */
#define __AS7262_MASK_CONTROL_SETUP_BANK            (0x3 << 2)
#define __AS7262_BANK_SET(val)                      ((val) << 2)

/** \brief 获取BANK */
#define __AS7262_CONTROL_SETUP_BANK_GET(reg)        (((reg) >> 2) & 0x3)

/** \brief 获取DATA_RDY */
#define __AS7262_CONTROL_SETUP_DATA_RDY_GET(reg)    (((reg) >> 1) & 0x1)

/** \brief 设置ICL_DRV */
#define __AS7262_MASK_LED_CONTROL_ICL_DRV           (0x3 << 4)
#define __AS7262_ICL_DRV_SET(val)                   ((val) << 4)

/** \brief 获取ICL_DRV */
#define __AS7262_LED_CONTROL_ICL_DRV_GET(reg)       (((reg) >> 4) & 0x3)

/** \brief 设置LED_DRV */
#define __AS7262_MASK_LED_CONTROL_LED_DRV           (0x1 << 3)
#define __AS7262_LED_DRV_SET(val)                   ((val) << 3)

/** \brief 获取LED_DRV */
#define __AS7262_LED_CONTROL_LED_DRV_GET(reg)       (((reg) >> 3) & 0x1)

/** \brief 设置ICL_IND */
#define __AS7262_MASK_LED_CONTROL_ICL_IND           (0x3 << 1)
#define __AS7262_ICL_IND_SET(val)                   ((val) << 1)

/** \brief 获取ICL_IND */
#define __AS7262_LED_CONTROL_ICL_IND_GET(reg)       (((reg) >> 1) & 0x3)

/** \brief 设置LED_IND */
#define __AS7262_MASK_LED_CONTROL_LED_IND           (0x1 << 0)
#define __AS7262_LED_IND_SET(val)                   ((val) << 0)

/** \brief 获取LED_IND */
#define __AS7262_LED_CONTROL_LED_IND_GET(reg)       (((reg) >> 0) & 0x1)

/** \brief 将两个uint8转换为一个uint16_t类型 */
#define __AS7262_UINT8_TO_UINT16(buff)              ((uint16_t)((*buff) << 8 | (*(buff + 1))))

/** \brief 将四个uint8转换为一个uint32_t类型 */
#define __AS7262_UINT8_TO_UINT32(buff)              ((uint32_t)((*(buff + 0) << 24)      | \
                                                     (uint32_t)(*(buff + 1) << 16)       | \
                                                     (uint32_t)(*(buff + 2) << 8)        | \
                                                     (uint32_t)(*(buff + 3) << 0)))

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_as7262_i2c_funcs = {
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
 * \brief as7262 写数据
 */
am_local am_err_t __as7262_write (am_sensor_as7262_i2c_dev_t   *p_this,
                                  uint32_t                      subaddr,
                                  uint8_t                      *p_buf,
                                  uint32_t                      nbytes)
{
    am_err_t            ret = AM_OK;

    volatile uint8_t    status;
    uint8_t             write_subaddr = (subaddr | 0x80);

    while (1) {
        // Read slave I2C status to see if the write buffer is ready.
        am_i2c_read(&p_this->i2c_dev,
                    __AS7262_STATUS_REG,
                    (uint8_t *)&status,
                    1);

        if ((status & __AS7262_TX_VALID) == 0) {
            // No inbound TX pending at slave. Okay to write now.
            break;
        }
    }

    // Send the virtual register address (setting bit 7 to indicate a pending write).
    am_i2c_write(&p_this->i2c_dev,
                 __AS7262_WRITE_REG,
                 &write_subaddr,
                 1);

    while (1) {
        // Read the slave I2C status to see if the write buffer is ready.
        am_i2c_read(&p_this->i2c_dev,
                    __AS7262_STATUS_REG,
                    (uint8_t *)&status,
                    1);

        if ((status & __AS7262_TX_VALID) == 0) {
            // No inbound TX pending at slave. Okay to write now.
            break;
        }
    }

    // Send the data to complete the operation.
    ret = am_i2c_write(&p_this->i2c_dev,
                       __AS7262_WRITE_REG,
                       p_buf,
                       1);

    return ret;
}

/*
 * \brief as7262 读数据
 */
am_local am_err_t __as7262_read (am_sensor_as7262_i2c_dev_t    *p_this,
                                 uint32_t                       subaddr,
                                 uint8_t                       *p_buf,
                                 uint32_t                       nbytes)
{
    am_err_t            ret = AM_OK;

    volatile uint8_t    status;
    uint8_t             write_subaddr = subaddr;

    while (1) {
        // Read slave I2C status to see if the read buffer is ready.
        am_i2c_read(&p_this->i2c_dev,
                    __AS7262_STATUS_REG,
                    (uint8_t *)&status,
                    1);

        if ((status & __AS7262_TX_VALID) == 0) {
            // No inbound TX pending at slave. Okay to write now.
            break;
        }
    }

    // Send the virtual register address (setting bit 7 to indicate a pending write).
    am_i2c_write(&p_this->i2c_dev,
                 __AS7262_WRITE_REG,
                 &write_subaddr,
                 1);

    while (1) {
        // Read the slave I2C status to see if our read data is available.
        am_i2c_read(&p_this->i2c_dev,
                    __AS7262_STATUS_REG,
                    (uint8_t *)&status,
                    1);

        if ((status & __AS7262_RX_VALID) != 0) {
            // Read data is ready.
            break;
        }
    }

    // Read the data to complete the operation.
    ret = am_i2c_read(&p_this->i2c_dev,
                      __AS7262_READ_REG,
                      p_buf,
                      1);

    return ret;
}

/*
 * \brief as7262 修改寄存器值
 */
am_local am_err_t __as7262_modify_reg_val(am_sensor_as7262_i2c_dev_t   *p_this,
                                          uint8_t                       reg,
                                          uint8_t                       mask,
                                          uint8_t                       val)
{
    uint8_t regval = 0;
    am_err_t ret = AM_OK;

    if (mask == 0xFF) {
        regval = val;
    } else {
        ret = __as7262_read(p_this, reg, &regval, 1);
        if (ret != AM_OK) {
            return ret;
        }

        regval &= ~mask;
        regval |= (val & mask);
    }

    ret = __as7262_write(p_this, reg, &regval, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/*******************************************************************************/

am_local am_err_t __as7262_i2c_ioctl (void                      *p_drv,
                                      int                        cmd,
                                      void                      *p_arg)
{
    uint8_t reg = 0x00;
    uint8_t regmask = 0xFF;
    uint8_t regval = 0x00;

    am_err_t ret = AM_OK;

    am_sensor_as7262_i2c_dev_t  *p_dev = (am_sensor_as7262_i2c_dev_t  *)p_drv;

    switch (cmd) {
    /* 设置ITIME */
    case AM_SENSOR_AS7262_ITIME_SET:
        reg = __AS7262_REG_INT_T;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取ITIME */
    case AM_SENSOR_AS7262_ITIME_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_INT_T, (uint8_t *)p_arg, 1);
        break;

    /* 设置GAIN */
    case AM_SENSOR_AS7262_GAIN_SET:
        reg = __AS7262_REG_CONTROL_SETUP;
        regmask = __AS7262_MASK_CONTROL_SETUP_GAIN;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7262_GAIN_SET(regval);
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取GAIN */
    case AM_SENSOR_AS7262_GAIN_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_CONTROL_SETUP, &regval, 1);
        regval = __AS7262_CONTROL_SETUP_GAIN_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 获取DEVICE TEMP */
    case AM_SENSOR_AS7262_DEVICE_TEMP_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_DEVICE_TEMP, (uint8_t *)p_arg, 1);
        break;

    /* 设置BANK */
    case AM_SENSOR_AS7262_BANK_SET:
        reg = __AS7262_REG_CONTROL_SETUP;
        regmask = __AS7262_MASK_CONTROL_SETUP_BANK;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7262_BANK_SET(regval);
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取BANK */
    case AM_SENSOR_AS7262_BANK_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_CONTROL_SETUP, &regval, 1);
        regval = __AS7262_CONTROL_SETUP_BANK_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 设置LED_IND */
    case AM_SENSOR_AS7262_LED_IND_SET:
        reg = __AS7262_REG_LED_CONTROL;
        regmask = __AS7262_MASK_LED_CONTROL_LED_IND;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7262_LED_IND_SET(regval);
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取LED_IND */
    case AM_SENSOR_AS7262_LED_IND_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_LED_CONTROL, &regval, 1);
        regval = __AS7262_LED_CONTROL_LED_IND_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 设置LED_DRV */
    case AM_SENSOR_AS7262_LED_DRV_SET:
        reg = __AS7262_REG_LED_CONTROL;
        regmask = __AS7262_MASK_LED_CONTROL_LED_DRV;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7262_LED_DRV_SET(regval);
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取LED_DRV */
    case AM_SENSOR_AS7262_LED_DRV_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_LED_CONTROL, &regval, 1);
        regval = __AS7262_LED_CONTROL_LED_DRV_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 设置ICL_IND */
    case AM_SENSOR_AS7262_ICL_IND_SET:
        reg = __AS7262_REG_LED_CONTROL;
        regmask = __AS7262_MASK_LED_CONTROL_ICL_IND;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7262_ICL_IND_SET(regval);
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置ICL_DRV */
    case AM_SENSOR_AS7262_ICL_DRV_SET:
        reg = __AS7262_REG_LED_CONTROL;
        regmask = __AS7262_MASK_LED_CONTROL_ICL_DRV;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7262_ICL_DRV_SET(regval);
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取ICL_IND */
    case AM_SENSOR_AS7262_ICL_IND_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_LED_CONTROL, &regval, 1);
        regval = __AS7262_LED_CONTROL_ICL_IND_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 获取ICL_DRV */
    case AM_SENSOR_AS7262_ICL_DRV_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_LED_CONTROL, &regval, 1);
        regval = __AS7262_LED_CONTROL_ICL_DRV_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 软件复位 */
    case AM_SENSOR_AS7262_SW_RST:
        reg = __AS7262_REG_CONTROL_SETUP;
        regmask = __AS7262_SOFTWARE_RESET;
        regval = __AS7262_SOFTWARE_RESET;
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取HW VERSION */
    case AM_SENSOR_AS7262_HW_VERSION_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_HW_VERSION, (uint8_t *)p_arg, 1);
        break;

    /* 获取DEVICE TYPE */
    case AM_SENSOR_AS7262_DEVICE_TYPE_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_DEVICE_TYPE, (uint8_t *)p_arg, 1);
        break;

    /* 设置INT */
    case AM_SENSOR_AS7262_INT_SET:
        reg = __AS7262_REG_CONTROL_SETUP;
        regmask = __AS7262_MASK_CONTROL_SETUP_INT;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7262_INT_SET(regval);
        ret = __as7262_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取DATA_RDY */
    case AM_SENSOR_AS7262_DATA_RDY_GET:
        ret = __as7262_read(p_dev, __AS7262_REG_CONTROL_SETUP, &regval, 1);
        regval = __AS7262_CONTROL_SETUP_DATA_RDY_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/*******************************************************************************/

/** \brief AS7262 参数初始化 */
am_local am_err_t __as7262_param_init(am_sensor_as7262_i2c_dev_t    *p_this)
{
    as7262_param_config_t *p_param_config = (as7262_param_config_t *)(p_this->isa.dev_info->p_param_default);

    am_err_t ret = AM_OK;

    /* CONTROL_SETUP 寄存器配置 */
    ret = __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_GAIN_SET, (void *)(uint32_t)(p_param_config->gain_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* INT_T 寄存器配置 */
    ret = __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_ITIME_SET, (void *)(uint32_t)(p_param_config->itime_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* LED_CONTROL 寄存器配置 */
    ret = __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_ICL_DRV_SET, (void *)(uint32_t)(p_param_config->icl_drv));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_LED_DRV_SET, (void *)(uint32_t)(p_param_config->led_drv));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_ICL_IND_SET, (void *)(uint32_t)(p_param_config->icl_ind));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_LED_IND_SET, (void *)(uint32_t)(p_param_config->led_ind));
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief AS7262 校准 */
am_local am_err_t __as7262_calibration(am_sensor_as7262_i2c_dev_t   *p_this)
{
    am_err_t ret    = AM_OK;

    

    return ret;
}

/**
 * \brief AS7262 中断使能
 */
am_local void __as7262_int_enable(am_sensor_as7262_i2c_dev_t    *p_this)
{
    __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_INT_SET,
                                (void *)(AM_AS7262_INT_OUTPUT_ENABLE));
}

/**
 * \brief AS7262 中断禁能
 */
am_local void __as7262_int_disable(am_sensor_as7262_i2c_dev_t   *p_this)
{
    __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_INT_SET,
                                (void *)(AM_AS7262_INT_OUTPUT_DISABLE));
}

/**
 * \brief AS7262 获取结果
 */
am_local am_err_t __as7262_result_get(am_sensor_as7262_i2c_dev_t    *p_this,
                                      as7262_result_info_t          *p_result)
{

    am_err_t ret = AM_OK;

    uint16_t i = 0;

    uint8_t result_buf[13]  = {0};         /* 缓存寄存器值 */

    uint8_t cal_buf[24];

    uint32_t caldata[6] = {0};

    for (i = 0; i < 12; i++) {
        ret = __as7262_read(p_this, (__AS7262_REG_V_H + i), &result_buf[i], 1);
        if (ret != AM_OK) {
            return ret;
        }
    }

    for (i = 0; i < 24; i++) {
        ret = __as7262_read(p_this, (__AS7262_REG_V_CAL_XH + i), &cal_buf[i], 1);
        if (ret != AM_OK) {
            return ret;
        }
    }

    ret = __as7262_read(p_this, __AS7262_REG_DEVICE_TEMP, &result_buf[12], 1);
    if (ret != AM_OK) {
        return ret;
    }

    /* 原始数据 */
    p_result->raw_data[0] = __AS7262_UINT8_TO_UINT16(&result_buf[0]);   //v
    p_result->raw_data[1] = __AS7262_UINT8_TO_UINT16(&result_buf[2]);   //b
    p_result->raw_data[2] = __AS7262_UINT8_TO_UINT16(&result_buf[4]);   //g
    p_result->raw_data[3] = __AS7262_UINT8_TO_UINT16(&result_buf[6]);   //y
    p_result->raw_data[4] = __AS7262_UINT8_TO_UINT16(&result_buf[8]);   //o
    p_result->raw_data[5] = __AS7262_UINT8_TO_UINT16(&result_buf[10]);  //r

    /* 校准数据 */
    caldata[0] = __AS7262_UINT8_TO_UINT32(&cal_buf[0]);   //v
    caldata[1] = __AS7262_UINT8_TO_UINT32(&cal_buf[4]);   //b
    caldata[2] = __AS7262_UINT8_TO_UINT32(&cal_buf[8]);   //g
    caldata[3] = __AS7262_UINT8_TO_UINT32(&cal_buf[12]);  //y
    caldata[4] = __AS7262_UINT8_TO_UINT32(&cal_buf[16]);  //o
    caldata[5] = __AS7262_UINT8_TO_UINT32(&cal_buf[20]);  //r
    memcpy((void *)&(p_result->cal_data[0]), (void *)&caldata[0], sizeof(float));
    memcpy((void *)&(p_result->cal_data[1]), (void *)&caldata[1], sizeof(float));
    memcpy((void *)&(p_result->cal_data[2]), (void *)&caldata[2], sizeof(float));
    memcpy((void *)&(p_result->cal_data[3]), (void *)&caldata[3], sizeof(float));
    memcpy((void *)&(p_result->cal_data[4]), (void *)&caldata[4], sizeof(float));
    memcpy((void *)&(p_result->cal_data[5]), (void *)&caldata[5], sizeof(float));

    for (i = 0; i < 6; i++) {
        p_result->cal_data[i] = p_this->isa.cal_val[0].cal[i];    /* 此处将校准值清零，不使用校准值 */
    }

    /* 校准后的数据值 */
    p_result->channel_v = p_result->raw_data[0] - p_result->cal_data[0];
    p_result->channel_b = p_result->raw_data[1] - p_result->cal_data[1];
    p_result->channel_g = p_result->raw_data[2] - p_result->cal_data[2];
    p_result->channel_y = p_result->raw_data[3] - p_result->cal_data[3];
    p_result->channel_o = p_result->raw_data[4] - p_result->cal_data[4];
    p_result->channel_r = p_result->raw_data[5] - p_result->cal_data[5];

    /* 温度 */
    p_result->device_temp = result_buf[12];
//    #if __AS7262_DEBUG
        am_kprintf("\r\nDevice temperature = %d ℃ \r\n", p_result->device_temp);
//    #endif

    return ret;
}

/*******************************************************************************/

/** \breif 中断回调函数 */
am_local void __as7262_alarm_callback (void *p_arg)
{
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_arg;

    am_isr_defer_job_add(&(p_this->isa.g_myjob));   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_arg;

    uint8_t i = 0;

    uint8_t bank_mode = 0;

    as7262_result_info_t result;

    /* 获取数据 */
    __as7262_result_get(p_this, &result);

    p_this->isa.data[0].val = result.channel_v;
    p_this->isa.data[0].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[1].val = result.channel_b;
    p_this->isa.data[1].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[2].val = result.channel_g;
    p_this->isa.data[2].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[3].val = result.channel_y;
    p_this->isa.data[3].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[4].val = result.channel_o;
    p_this->isa.data[4].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[5].val = result.channel_r;
    p_this->isa.data[5].unit = AM_SENSOR_UNIT_BASE;

    for (i = 0; i < 6; i++) {
        if (p_this->isa.pfn_trigger_cb[i] &&
                (p_this->isa.flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->isa.pfn_trigger_cb[i](p_this->isa.p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);
        }
    }

    /* 获取 BANK_MODE */
    __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_BANK_GET, &bank_mode);
    /* 若 BANK_MODE 为单次触发模式 */
    if (bank_mode == AM_AS7262_BANK_MODE_3) {
        __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_BANK_SET, (void *)AM_AS7262_BANK_MODE_3);
    }
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_AS7262_CHAN_1 || id == AM_AS7262_CHAN_2 || 
        id == AM_AS7262_CHAN_3 || id == AM_AS7262_CHAN_4 || 
        id == AM_AS7262_CHAN_5 || id == AM_AS7262_CHAN_6 ) {
        return AM_SENSOR_TYPE_SPECTRAL;
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
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    as7262_result_info_t result;

    int cur_id = 0;
    int i = 0;

    uint8_t control_setup_reg = 0x00;
    uint8_t bank_mode = 0;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (AM_BIT_GET(p_this->isa.trigger, 15) != 1) {
        return -AM_EPERM;
    }

    for (i = 0; i < num; i++) {
        p_buf[i].unit = AM_SENSOR_UNIT_INVALID;
    }

    /* 若为1，则可能在数据准备就绪触发回调函数中使用 */
    if (num == 1) {
        cur_id = p_ids[0];
        /* 若打开数据准备就绪触发方式，则直接赋值 */
        if ((AM_BIT_GET(p_this->isa.trigger, 6)) &&
                ((p_this->isa.flags[0] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[1] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[2] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[3] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[4] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[5] & AM_SENSOR_TRIGGER_DATA_READY))) {
            p_buf[0].val = p_this->isa.data[cur_id].val;
            p_buf[0].unit= p_this->isa.data[cur_id].unit;
            return AM_OK;
        }
    }

    /** \brief 读取结果数据 */
    do {

        __as7262_read(p_this, __AS7262_REG_CONTROL_SETUP, &control_setup_reg, 1);

    } while (__AS7262_CONTROL_SETUP_DATA_RDY_GET(control_setup_reg) != 0x1);

    ret = __as7262_result_get(p_this, &result);
    if (ret != AM_OK) {
        return ret;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            p_buf[i].val  = result.channel_v;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 1) {

            p_buf[i].val  = result.channel_b;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 2) {

            p_buf[i].val  = result.channel_g;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 3) {

            p_buf[i].val  = result.channel_y;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 4) {

            p_buf[i].val  = result.channel_o;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 5) {

            p_buf[i].val  = result.channel_r;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else {
            return -AM_ENODEV;  /*< \brief 若此次通道不属于该传感器，直接返回 */
        }
    }

    /* 获取 BANK_MODE */
    __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_BANK_GET, &bank_mode);
    /* 若 BANK_MODE 为单次触发模式 */
    if (bank_mode == AM_AS7262_BANK_MODE_3) {
        __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_BANK_SET, (void *)AM_AS7262_BANK_MODE_3);
    }

    return ret;
}

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result)
{
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_drv;
    as7262_param_config_t *p_param_config = (as7262_param_config_t *)(p_this->isa.dev_info->p_param_default);

    int i = 0;
    int cur_id = 0;

    am_err_t ret = -AM_ENODEV;
    am_err_t curent_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || 
            cur_id == 3 || cur_id == 4 || cur_id == 5) {
            if (p_result != NULL) {
                p_result[i].val = AM_OK;
            }

            AM_BIT_SET(p_this->isa.trigger, 14 - cur_id);

            ret = AM_OK;
        } else {
            if (p_result != NULL) {
                p_result[i].val = -AM_ENODEV;
            }
            curent_ret = -AM_ENODEV;
        }
    }

    if (ret != AM_OK) {     /*< \breif 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->isa.trigger, 15);
    }    

    /* 寄存器参数初始化 */
    ret = __as7262_param_init(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* 获取校准数据 */
    ret = __as7262_calibration(p_this);     //未添加校准功能xxxxxxxxxxxx
    if (ret != AM_OK) {
        return ret;
    }

    /* 使能模式 */
    ret = __as7262_i2c_ioctl(p_this, AM_SENSOR_AS7262_BANK_SET,
                                (void *)(uint32_t)(p_param_config->bank_mode));
    if (ret != AM_OK) {
        return ret;
    }

    return curent_ret;
}

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || 
            cur_id == 3 || cur_id == 4 || cur_id == 5) {

            AM_BIT_CLR(p_this->isa.trigger, 14 - cur_id);
            ret = AM_OK;

        } else {
            ret = -AM_ENODEV;
        }

        if (p_result != NULL) {
            p_result[i].val = ret;
        }
    }

    if ((AM_BIT_GET(p_this->isa.trigger, 14) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 13) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 12) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 11) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 10) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 9) == 0)) {

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->isa.trigger, 15);
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
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_drv;

    if (id != 0 && id != 1 && id != 2 && 
        id != 3 && id != 4 && id != 5) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (flags) {
    case AM_SENSOR_TRIGGER_DATA_READY:       /* 数据准备就绪触发 */
        break;
    default:
        return -AM_ENOTSUP;
    }

    p_this->isa.pfn_trigger_cb[id] = pfn_cb;
    p_this->isa.p_arg[id]          = p_arg;
    p_this->isa.flags[id]          = flags;

    return AM_OK;
}

/** \brief 打开触发 */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id)
{
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0 && id != 1 && id != 2 && 
        id != 3 && id != 4 && id != 5) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->isa.trigger & 0x40) {
        return AM_OK;
    }

    if (p_this->isa.dev_info->trigger_pin != -1) {
        ret = am_gpio_trigger_on(p_this->isa.dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }

    /* 使能中断 */
    __as7262_int_enable(p_this);

    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->isa.trigger, 0);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 1) {
        AM_BIT_SET(p_this->isa.trigger, 1);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 2) {
        AM_BIT_SET(p_this->isa.trigger, 2);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 3) {
        AM_BIT_SET(p_this->isa.trigger, 3);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 4) {
        AM_BIT_SET(p_this->isa.trigger, 4);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 5) {
        AM_BIT_SET(p_this->isa.trigger, 5);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }

    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_as7262_i2c_dev_t* p_this = (am_sensor_as7262_i2c_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0 && id != 1 && id != 2 && 
        id != 3 && id != 4 && id != 5) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_this->isa.pfn_trigger_cb[id] = NULL;
    p_this->isa.p_arg[id]          = NULL;
    p_this->isa.flags[id]          = 0;

    AM_BIT_CLR(p_this->isa.trigger, id);

    if ((p_this->isa.trigger & 0x3F) == 0) {

        if ((p_this->isa.dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->isa.dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        if (cur_ret == AM_OK) {

            /* 禁能中断 */
            __as7262_int_disable(p_this);

            AM_BIT_CLR(p_this->isa.trigger, 6);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 AS7262 (I2C模式) 初始化
 */
am_sensor_handle_t am_sensor_as7262_i2c_init (
        am_sensor_as7262_i2c_dev_t              *p_sensor_i2c_dev,
        const am_sensor_as7262_i2c_devinfo_t    *p_sensor_i2c_devinfo)
{
    am_err_t ret = AM_OK;

    uint8_t as7262_id = 0;
    uint8_t i = 0;
    uint8_t reg = 0;

    am_i2c_handle_t handle;

    if (p_sensor_i2c_dev == NULL || p_sensor_i2c_devinfo == NULL) {
        return NULL;
    }

    handle = p_sensor_i2c_devinfo->pfn_i2c_handle_get();
    p_sensor_i2c_dev->isa.pfn_ioctl = __as7262_i2c_ioctl;

    am_i2c_mkdev(&p_sensor_i2c_dev->i2c_dev,
                 handle,
                 p_sensor_i2c_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_sensor_i2c_dev->isa.as7262_dev.p_drv   = p_sensor_i2c_dev;
    p_sensor_i2c_dev->isa.as7262_dev.p_funcs = &__g_sensor_as7262_i2c_funcs;
    p_sensor_i2c_dev->sensor_i2c_dev_info    = p_sensor_i2c_devinfo;
    p_sensor_i2c_dev->isa.dev_info           = p_sensor_i2c_devinfo->p_devinfo;
    p_sensor_i2c_dev->isa.trigger            = 0;
    for (i = 0; i < 6; i++) {
        p_sensor_i2c_dev->isa.pfn_trigger_cb[i]  = NULL;
        p_sensor_i2c_dev->isa.p_arg[i]           = NULL;
        p_sensor_i2c_dev->isa.flags[i]           = 0;
        p_sensor_i2c_dev->isa.data[i].val        = 0;
        p_sensor_i2c_dev->isa.data[i].unit       = AM_SENSOR_UNIT_INVALID;
    }

    if (p_sensor_i2c_devinfo->p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_sensor_i2c_devinfo->p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_sensor_i2c_devinfo->p_devinfo->trigger_pin,
                                __as7262_alarm_callback,
                                (void*)p_sensor_i2c_dev);
        am_gpio_trigger_cfg(p_sensor_i2c_devinfo->p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);  /* 下降沿触发 */
    }

    am_isr_defer_job_init(&p_sensor_i2c_dev->isa.g_myjob, __am_pfnvoid_t, p_sensor_i2c_dev, 1);

    /* AS7262硬件复位 */
    if (p_sensor_i2c_devinfo->p_devinfo->rst_pin != -1) {
        am_gpio_set(p_sensor_i2c_devinfo->p_devinfo->rst_pin, AM_GPIO_LEVEL_LOW);
        am_mdelay(150);
        am_gpio_set(p_sensor_i2c_devinfo->p_devinfo->rst_pin, AM_GPIO_LEVEL_HIGH);
    }

    /* AS7262软件复位 */
    reg = __AS7262_SOFTWARE_RESET;
    __as7262_write(p_sensor_i2c_dev, __AS7262_REG_CONTROL_SETUP, &reg, 1);
    am_mdelay(1000);  /* 此处延时必须大于 760 ms */

    /* 等待复位完成 */
    do {
        __as7262_read(p_sensor_i2c_dev, __AS7262_REG_CONTROL_SETUP, &reg, 1);

    } while (reg & __AS7262_SOFTWARE_RESET);

    /* 读取ID (Device Type) */
    ret = __as7262_read(p_sensor_i2c_dev, __AS7262_REG_DEVICE_TYPE, &as7262_id, 1);
    if ((ret != AM_OK) || (as7262_id != __AS7262_ID)) {
    #if __AS7262_DEBUG
        am_kprintf("as7262_id = 0x%x \r\n", as7262_id);
    #endif
        am_kprintf("\r\nSensor AS7262 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_sensor_i2c_dev->isa.as7262_dev);
}

/**
 * \brief 传感器 AS7262 (I2C模式) 去初始化
 */
am_err_t am_sensor_as7262_i2c_deinit (am_sensor_handle_t handle)
{
    am_sensor_as7262_i2c_dev_t *p_dev = (am_sensor_as7262_i2c_dev_t *)(handle->p_drv);
    am_i2c_handle_t     i2c_handle = p_dev->sensor_i2c_dev_info->pfn_i2c_handle_get();

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->sensor_i2c_dev_info->pfn_i2c_deinit(i2c_handle);

    p_dev->isa.as7262_dev.p_drv   = NULL;
    p_dev->isa.as7262_dev.p_funcs = NULL;
    p_dev->isa.dev_info           = NULL;
    p_dev->sensor_i2c_dev_info    = NULL;

    return AM_OK;
}

/* end of file */

