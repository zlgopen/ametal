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
 * \brief 传感器 AS5600 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-30  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as5600.h"
#include "am_sensor.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**< 调试信息输出 */
#define __AS5600_DEBUG                          (0)

/*********************************************************************************************
 * 宏定义
 ********************************************************************************************/
#define __AS5600_REG_ZMCO                       (0x00)
#define __AS5600_REG_ZPOS_H                     (0x01)
#define __AS5600_REG_ZPOS_L                     (0x02)
#define __AS5600_REG_MPOS_H                     (0x03)
#define __AS5600_REG_MPOS_L                     (0x04)
#define __AS5600_REG_MANG_H                     (0x05)
#define __AS5600_REG_MANG_L                     (0x06)
#define __AS5600_REG_CONF_H                     (0x07)
#define __AS5600_REG_CONF_L                     (0x08)
#define __AS5600_REG_RAW_ANGLE_H                (0x0C)
#define __AS5600_REG_RAW_ANGLE_L                (0x0D)
#define __AS5600_REG_ANGLE_H                    (0x0E)
#define __AS5600_REG_ANGLE_L                    (0x0F)
#define __AS5600_REG_STATUS                     (0x0B)
#define __AS5600_REG_AGC                        (0x1A)
#define __AS5600_REG_MAGNITUDE_H                (0x1B)
#define __AS5600_REG_MAGNITUDE_L                (0x1C)
#define __AS5600_REG_BURN                       (0xFF)

/** \brief 获取ZMCO */
#define __AS5600_ZMCO_GET(reg)                  (((reg) >> 0) & 0x3)

/** \brief 设置WD */
#define __AS5600_MASK_CONF_H_WD                 (0x1 << 5)
#define __AS5600_WD_SET(val)                    ((val) << 5)

/** \brief 设置FTH */
#define __AS5600_MASK_CONF_H_FTH                (0x7 << 2)
#define __AS5600_FTH_SET(val)                   ((val) << 2)

/** \brief 设置SF */
#define __AS5600_MASK_CONF_H_SF                 (0x3 << 0)
#define __AS5600_SF_SET(val)                    ((val) << 0)

/** \brief 设置PWMF */
#define __AS5600_MASK_CONF_L_PWMF               (0x3 << 6)
#define __AS5600_PWMF_SET(val)                  ((val) << 6)

/** \brief 设置OUTS */
#define __AS5600_MASK_CONF_L_OUTS               (0x3 << 4)
#define __AS5600_OUTS_SET(val)                  ((val) << 4)

/** \brief 设置HYST */
#define __AS5600_MASK_CONF_L_HYST               (0x3 << 2)
#define __AS5600_HYST_SET(val)                  ((val) << 2)

/** \brief 设置PM */
#define __AS5600_MASK_CONF_L_PM                 (0x3 << 0)
#define __AS5600_PM_SET(val)                    ((val) << 0)

/** \brief 获取WD */
#define __AS5600_CONF_WD_GET(reg)               (((reg) >> 13) & 0x1)

/** \brief 获取FTH */
#define __AS5600_CONF_FTH_GET(reg)              (((reg) >> 10) & 0x7)

/** \brief 获取SF */
#define __AS5600_CONF_SF_GET(reg)               (((reg) >> 8) & 0x3)

/** \brief 获取PWMF */
#define __AS5600_CONF_PWMF_GET(reg)             (((reg) >> 6) & 0x3)

/** \brief 获取OUTS */
#define __AS5600_CONF_OUTS_GET(reg)             (((reg) >> 4) & 0x3)

/** \brief 获取HYST */
#define __AS5600_CONF_HYST_GET(reg)             (((reg) >> 2) & 0x3)

/** \brief 获取PM */
#define __AS5600_CONF_PM_GET(reg)               (((reg) >> 0) & 0x3)

/** \brief 获取MD */
#define __AS5600_STATUS_MD_GET(reg)             (((reg) >> 5) & 0x1)

/** \brief 获取ML */
#define __AS5600_STATUS_ML_GET(reg)             (((reg) >> 4) & 0x1)

/** \brief 获取MH */
#define __AS5600_STATUS_MH_GET(reg)             (((reg) >> 3) & 0x1)

/** \brief 将两个uint8转换为一个uint16_t类型 */
#define __AS5600_UINT8_TO_UINT16(buff)          ((uint16_t)((*buff) << 8 | (*(buff + 1))))

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_as5600_funcs = {
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
 * \brief as5600 写数据
 */
am_local am_err_t __as5600_write (am_sensor_as5600_dev_t   *p_this,
                                  uint32_t                  subaddr,
                                  uint8_t                  *p_buf,
                                  uint32_t                  nbytes)
{
    return am_i2c_write(&p_this->i2c_dev,
                        subaddr,
                        p_buf,
                        nbytes);
}

/*
 * \brief as5600 读数据
 */
am_local am_err_t __as5600_read (am_sensor_as5600_dev_t    *p_this,
                                 uint32_t                   subaddr,
                                 uint8_t                   *p_buf,
                                 uint32_t                   nbytes)
{
    return am_i2c_read(&p_this->i2c_dev,
                       subaddr,
                       p_buf,
                       nbytes);
}

/*
 * \brief as5600 修改寄存器值
 */
am_local am_err_t __as5600_modify_reg_val(am_sensor_as5600_dev_t   *p_this,
                                          uint8_t                   reg,
                                          uint8_t                   mask,
                                          uint8_t                   val)
{
    uint8_t regval = 0;
    am_err_t ret = AM_OK;

    if (mask == 0xFF) {
        regval = val;
    } else {
        ret = __as5600_read(p_this, reg, &regval, 1);
        if (ret != AM_OK) {
            return ret;
        }

        regval &= ~mask;
        regval |= (val & mask);
    }

    ret = __as5600_write(p_this, reg, &regval, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置阈值 */
am_local am_err_t __as5600_threshold_set(am_sensor_as5600_dev_t    *p_this,
                                         uint32_t                   subaddr,
                                         uint16_t                   value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    reg[0] = (value & 0xff00) >> 8;
    reg[1] = value & 0xff;

    ret = __as5600_write(p_this, subaddr, reg, 2);

    return ret;
}

/** \brief 获取阈值 */
am_local am_err_t __as5600_threshold_get(am_sensor_as5600_dev_t    *p_this,
                                         uint32_t                   subaddr,
                                         uint16_t                  *p_value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    ret = __as5600_read(p_this, subaddr, reg, 2);

    *p_value = __AS5600_UINT8_TO_UINT16(&reg[0]);

    return ret;
}

/*******************************************************************************/

am_local am_err_t __as5600_ioctl (am_sensor_as5600_dev_t   *p_dev,
                                  int                       cmd,
                                  void                     *p_arg)
{
    uint8_t reg = 0x00;
    uint8_t regmask = 0xFF;
    uint8_t regval = 0x00;

    am_err_t ret = AM_OK;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    switch (cmd) {

    /* 获取ZMCO */
    case AM_SENSOR_AS5600_ZMCO_GET:
        ret = __as5600_read(p_dev, __AS5600_REG_ZMCO, &regval, 1);
        regval = __AS5600_ZMCO_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 设置ZPOS*/
    case AM_SENSOR_AS5600_ZPOS_SET:
        ret = __as5600_threshold_set(p_dev, __AS5600_REG_ZPOS_H, (uint16_t)(int)p_arg);
        break;

    /* 获取ZPOS */
    case AM_SENSOR_AS5600_ZPOS_GET:
        ret = __as5600_threshold_get(p_dev, __AS5600_REG_ZPOS_H, (uint16_t *)p_arg);
        break;

    /* 设置MPOS*/
    case AM_SENSOR_AS5600_MPOS_SET:
        ret = __as5600_threshold_set(p_dev, __AS5600_REG_MPOS_H, (uint16_t)(int)p_arg);
        break;

    /* 获取MPOS */
    case AM_SENSOR_AS5600_MPOS_GET:
        ret = __as5600_threshold_get(p_dev, __AS5600_REG_MPOS_H, (uint16_t *)p_arg);
        break;

    /* 设置MANG*/
    case AM_SENSOR_AS5600_MANG_SET:
        ret = __as5600_threshold_set(p_dev, __AS5600_REG_MANG_H, (uint16_t)(int)p_arg);
        break;

    /* 获取MANG */
    case AM_SENSOR_AS5600_MANG_GET:
        ret = __as5600_threshold_get(p_dev, __AS5600_REG_MANG_H, (uint16_t *)p_arg);
        break;

    /* 设置WD */
    case AM_SENSOR_AS5600_WD_SET:
        reg = __AS5600_REG_CONF_H;
        regmask = __AS5600_MASK_CONF_H_WD;
        regval = (uint8_t)(int)p_arg;
        regval = __AS5600_WD_SET(regval);
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置FTH */
    case AM_SENSOR_AS5600_FTH_SET:
        reg = __AS5600_REG_CONF_H;
        regmask = __AS5600_MASK_CONF_H_FTH;
        regval = (uint8_t)(int)p_arg;
        regval = __AS5600_FTH_SET(regval);
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SF */
    case AM_SENSOR_AS5600_SF_SET:
        reg = __AS5600_REG_CONF_H;
        regmask = __AS5600_MASK_CONF_H_SF;
        regval = (uint8_t)(int)p_arg;
        regval = __AS5600_SF_SET(regval);
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PWMF */
    case AM_SENSOR_AS5600_PWMF_SET:
        reg = __AS5600_REG_CONF_L;
        regmask = __AS5600_MASK_CONF_L_PWMF;
        regval = (uint8_t)(int)p_arg;
        regval = __AS5600_PWMF_SET(regval);
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置OUTS */
    case AM_SENSOR_AS5600_OUTS_SET:
        reg = __AS5600_REG_CONF_L;
        regmask = __AS5600_MASK_CONF_L_OUTS;
        regval = (uint8_t)(int)p_arg;
        regval = __AS5600_OUTS_SET(regval);
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置HYST */
    case AM_SENSOR_AS5600_HYST_SET:
        reg = __AS5600_REG_CONF_L;
        regmask = __AS5600_MASK_CONF_L_HYST;
        regval = (uint8_t)(int)p_arg;
        regval = __AS5600_HYST_SET(regval);
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PM */
    case AM_SENSOR_AS5600_PM_SET:
        reg = __AS5600_REG_CONF_L;
        regmask = __AS5600_MASK_CONF_L_PM;
        regval = (uint8_t)(int)p_arg;
        regval = __AS5600_PM_SET(regval);
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取CONF寄存器 */
    case AM_SENSOR_AS5600_CONF_GET:
        ret = __as5600_threshold_get(p_dev, __AS5600_REG_CONF_H, (uint16_t *)p_arg);
        break;

    /* 获取RAW_ANGLE */
    case AM_SENSOR_AS5600_RAW_ANGLE_GET:
        ret = __as5600_threshold_get(p_dev, __AS5600_REG_RAW_ANGLE_H, (uint16_t *)p_arg);
        break;

    /* 获取ANGLE */
    case AM_SENSOR_AS5600_ANGLE_GET:
        ret = __as5600_threshold_get(p_dev, __AS5600_REG_ANGLE_H, (uint16_t *)p_arg);
        break;

    /* 获取MD */
    case AM_SENSOR_AS5600_MD_GET:
        ret = __as5600_read(p_dev, __AS5600_REG_STATUS, &regval, 1);
        regval = __AS5600_STATUS_MD_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 获取ML */
    case AM_SENSOR_AS5600_ML_GET:
        ret = __as5600_read(p_dev, __AS5600_REG_STATUS, &regval, 1);
        regval = __AS5600_STATUS_ML_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 获取MH */
    case AM_SENSOR_AS5600_MH_GET:
        ret = __as5600_read(p_dev, __AS5600_REG_STATUS, &regval, 1);
        regval = __AS5600_STATUS_MH_GET(regval);
        *(uint8_t *)p_arg = regval;
        break;

    /* 获取AGC */
    case AM_SENSOR_AS5600_AGC_GET:
        ret = __as5600_read(p_dev, __AS5600_REG_AGC, (uint8_t *)p_arg, 1);
        break;

    /* 获取MAGNITUDE */
    case AM_SENSOR_AS5600_MAGNITUDE_GET:
        ret = __as5600_threshold_get(p_dev, __AS5600_REG_MAGNITUDE_H, (uint16_t *)p_arg);
        break;

    /* 设置BURN */
    case AM_SENSOR_AS5600_BURN_SET:
        reg = __AS5600_REG_BURN;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        ret = __as5600_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/*******************************************************************************/

/** \brief AS5600 清空STATUS寄存器 */
am_local am_err_t __as5600_status_clr_all(am_sensor_as5600_dev_t    *p_this)
{
    am_err_t ret = AM_OK;

    return ret;
}

/**
 * \brief AS5600 中断使能
 */
am_local void __as5600_int_enable(am_sensor_as5600_dev_t    *p_this)
{

}

/**
 * \brief AS5600 中断禁能
 */
am_local void __as5600_int_disable(am_sensor_as5600_dev_t   *p_this)
{

}

/** \brief AS5600 参数初始化 */
am_local am_err_t __as5600_param_init(am_sensor_as5600_dev_t    *p_this)
{
    as5600_param_config_t *p_param_config = (as5600_param_config_t *)p_this->dev_info->p_param_default;

    am_err_t ret = AM_OK;

    /* MANG 寄存器配置 */
    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_MANG_SET, (void *)(uint32_t)(p_param_config->maximum_angle));
    if (ret != AM_OK) {
        return ret;
    }

    /* CONF 寄存器配置 */
    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_WD_SET, (void *)(uint32_t)(p_param_config->watchdog));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_FTH_SET, (void *)(uint32_t)(p_param_config->fast_filter_threshold));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_SF_SET, (void *)(uint32_t)(p_param_config->slow_filter));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_PWMF_SET, (void *)(uint32_t)(p_param_config->pwm_frequency));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_OUTS_SET, (void *)(uint32_t)(p_param_config->output_stage));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_HYST_SET, (void *)(uint32_t)(p_param_config->hysteresis));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_PM_SET, (void *)(uint32_t)(p_param_config->power_mode));
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief AS5600 调整角度范围时间 */
am_local void __as5600_adjust_angular_range_time(uint32_t   time,
                                                 uint8_t    position)
{
    uint32_t i = time;

    if (position == 0) {

        while (i) {
            am_kprintf("please turn the magnet to the start position in %d seconds. \r\n", i);
            am_mdelay(1000);
            i--;
        }

    } else if (position == 1) {

        while (i) {
            am_kprintf("please rotate the magnet to the stop position in %d seconds. \r\n", i);
            am_mdelay(1000);
            i--;
        }

    }
}

/** \brief AS5600 编程角度范围 */
am_local am_err_t __as5600_adjust_angular_range(am_sensor_as5600_dev_t  *p_this)
{
    as5600_param_config_t *p_param_config = (as5600_param_config_t *)p_this->dev_info->p_param_default;

    am_err_t ret = AM_OK;

    uint8_t zmco = 0;
    uint8_t md = 0;

    uint16_t raw_angle = 0;
    uint16_t zpos = 0;
    uint16_t mpos = 0;

    if (p_param_config->option_enable == AM_AS5600_OPTION_DISABLE) {
        return AM_OK;
    }

    ret = __as5600_ioctl(p_this, AM_SENSOR_AS5600_ZMCO_GET, &zmco);
    if (ret != AM_OK) {
        return ret;
    }

    switch (p_param_config->option) {

    case AM_AS5600_OPTION_A:
        /* Angle Programming Through the I2C Interface */

        __as5600_ioctl(p_this, AM_SENSOR_AS5600_MD_GET, &md);
        if ((md != 1) || (zmco == 3)) {
            return -AM_EPERM;
        }

        __as5600_adjust_angular_range_time(5, 0); /* Wait 5s */
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_RAW_ANGLE_GET, &raw_angle);
        zpos = raw_angle;
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_ZPOS_SET, (void *)(uint32_t)zpos);
        am_mdelay(5);
        __as5600_adjust_angular_range_time(5, 1); /* Wait 5s */
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_RAW_ANGLE_GET, &raw_angle);
        mpos = raw_angle;
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_MPOS_SET, (void *)(uint32_t)mpos);
        am_mdelay(5);
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)AM_AS5600_BURN_ANGLE);
        am_mdelay(5);
        /* Verify the BURN_ANGLE command */
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)0x01);
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)0x11);
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)0x10);

        __as5600_ioctl(p_this, AM_SENSOR_AS5600_ZPOS_GET, &raw_angle);
        if (zpos != raw_angle) {
            am_kprintf("ZPOS ERROR! \r\n");
            return AM_ERROR;
        }

        __as5600_ioctl(p_this, AM_SENSOR_AS5600_MPOS_GET, &raw_angle);
        if (mpos != raw_angle) {
            am_kprintf("MPOS ERROR! \r\n");
            return AM_ERROR;
        }
        am_kprintf("verify the BURN_ANGLE command was successful! \r\n");
        break;

    case AM_AS5600_OPTION_B:
        /* Angle Programming Through the OUT Pin.
           (This procedure can be executed only one time) */

        __as5600_ioctl(p_this, AM_SENSOR_AS5600_MD_GET, &md);
        if ((md != 1) || (zmco != 0)) {
            return -AM_EPERM;
        }

        __as5600_adjust_angular_range_time(5, 0); /* Wait 5s */
        if (p_this->dev_info->out_pin != -1) {
            am_gpio_pin_cfg(p_this->dev_info->out_pin, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);
            am_mdelay(150);
            am_gpio_pin_cfg(p_this->dev_info->out_pin, AM_GPIO_INPUT | AM_GPIO_FLOAT);
        }
        __as5600_adjust_angular_range_time(5, 1); /* Wait 5s */
        if (p_this->dev_info->out_pin != -1) {
            am_gpio_pin_cfg(p_this->dev_info->out_pin, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);
            am_mdelay(150);
            am_gpio_pin_cfg(p_this->dev_info->out_pin, AM_GPIO_INPUT | AM_GPIO_FLOAT);
        }
        am_mdelay(5);
        /* Check if the OUT pin is permanently driven to GND.
           This indicates an error occurred during programming */
        
        break;

    case AM_AS5600_OPTION_C:
        /* Programming a Maximum Angular Range Through the I2C Interface */

        __as5600_ioctl(p_this, AM_SENSOR_AS5600_MD_GET, &md);
        if (md != 1) {
            return -AM_EPERM;
        }

        /* configuration settings  */
        ret = __as5600_param_init(p_this);
        if (ret != AM_OK) {
            return ret;
        }
        am_mdelay(5);
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)AM_AS5600_BURN_SETTING);
        am_mdelay(5);
        /* Verify the BURN_SETTINGS command */
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)0x01);
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)0x11);
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_BURN_SET, (void *)0x10);

        __as5600_ioctl(p_this, AM_SENSOR_AS5600_MANG_GET, &raw_angle);
        if (p_param_config->maximum_angle != raw_angle) {
            am_kprintf("MANG ERROR! \r\n");
            return AM_ERROR;
        }
        raw_angle = 0;
        __as5600_ioctl(p_this, AM_SENSOR_AS5600_CONF_GET, &raw_angle);
        if ((p_param_config->watchdog != __AS5600_CONF_WD_GET(raw_angle))               || \
            (p_param_config->fast_filter_threshold != __AS5600_CONF_FTH_GET(raw_angle)) || \
            (p_param_config->slow_filter != __AS5600_CONF_SF_GET(raw_angle))            || \
            (p_param_config->pwm_frequency != __AS5600_CONF_PWMF_GET(raw_angle))        || \
            (p_param_config->output_stage != __AS5600_CONF_OUTS_GET(raw_angle))         || \
            (p_param_config->hysteresis != __AS5600_CONF_HYST_GET(raw_angle))           || \
            (p_param_config->power_mode != __AS5600_CONF_PM_GET(raw_angle))) {

            am_kprintf("CONF ERROR! \r\n");
            return AM_ERROR;
        }
        am_kprintf("verify the BURN_SETTINGS command was successful! \r\n");
        __as5600_adjust_angular_range_time(5, 0); /* Wait 5s */
        if (p_this->dev_info->out_pin != -1) {
            am_gpio_pin_cfg(p_this->dev_info->out_pin, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);
            am_mdelay(150);
            am_gpio_pin_cfg(p_this->dev_info->out_pin, AM_GPIO_INPUT | AM_GPIO_FLOAT);
        }
        am_mdelay(5);
        /* Check if the OUT pin is permanently driven to GND.
           This indicates an error occurred during programming */
        
        break;

    default:
        return -AM_ENOTSUP;
    }

    return ret;
}

/**
 * \brief AS5600 获取结果
 */
am_local am_err_t __as5600_result_get(am_sensor_as5600_dev_t    *p_this,
                                      as5600_result_info_t      *p_result)
{
    am_err_t ret = AM_OK;

    uint8_t result_buf[4]  = {0};         /* 缓存寄存器值 */

    ret = __as5600_read(p_this, __AS5600_REG_RAW_ANGLE_H, result_buf, 4);

    p_result->raw_angle = (__AS5600_UINT8_TO_UINT16(&result_buf[0]) & 0x0fff);
    p_result->angle = (__AS5600_UINT8_TO_UINT16(&result_buf[2]) & 0x0fff);

    return ret;
}

/*******************************************************************************/

/** \breif 中断回调函数 */
am_local void __as5600_alarm_callback (void *p_arg)
{
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_arg;

    as5600_result_info_t result;

    /* 获取数据 */
    __as5600_result_get(p_this, &result);

    /* 获取数据后，清中断标志位 */
    __as5600_status_clr_all(p_this);

    /* 获取ANGLE */
    p_this->data[0].val = result.angle;
    p_this->data[0].unit = AM_SENSOR_UNIT_BASE;

    if (p_this->pfn_trigger_cb[0] &&
            (p_this->flags[0] & AM_SENSOR_TRIGGER_THRESHOLD)) {
        p_this->pfn_trigger_cb[0](p_this->p_arg[0],
                                  AM_SENSOR_TRIGGER_THRESHOLD);
    }
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_AS5600_CHAN_1) {
        return AM_SENSOR_TYPE_ROTATION_VECTOR;
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
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    as5600_result_info_t result;

    int cur_id = 0;
    int i = 0;

    uint16_t times = 0xff;       /* 读取数据时间 */

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (AM_BIT_GET(p_this->trigger, 7) != 1) {
        return -AM_EPERM;
    }

    for (i = 0; i < num; i++) {
        p_buf[i].unit = AM_SENSOR_UNIT_INVALID;
    }

    /* 若为1，则可能在门限触发回调函数中使用 */
    if (num == 1) {
        cur_id = p_ids[0];
        /* 若打开门限触发方式，则直接赋值 */
        if ((AM_BIT_GET(p_this->trigger, 1)) &&
                (p_this->flags[0] & AM_SENSOR_TRIGGER_THRESHOLD)) {
            p_buf[0].val = p_this->data[cur_id].val;
            p_buf[0].unit= p_this->data[cur_id].unit;
            return AM_OK;
        }
    }

    /** \brief 读取结果数据 */
    do {
        ret = __as5600_result_get(p_this, &result);
        if (times != 0) {
            times--;
        }
    } while ((ret != AM_OK) && (times != 0));

    if (ret != AM_OK) {
        return ret;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            p_buf[i].val  = result.angle;           /*< \brief ANGLE */
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;    /*< \brief 单位默认为0:10^(0)*/

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
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

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

    if (ret != AM_OK) {     /*< \breif 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }

    /* 编程角度范围 */
    ret = __as5600_adjust_angular_range(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 7);
        AM_BIT_SET(p_this->trigger, 6);
    }

    return curent_ret;
}

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_drv;

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

        } else {
            ret = -AM_ENODEV;
        }

        if (p_result != NULL) {
            p_result[i].val = ret;
        }
    }

    if (AM_BIT_GET(p_this->trigger, 6) == 0) {

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
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_drv;

    if (id != 0) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (flags) {
    case AM_SENSOR_TRIGGER_THRESHOLD:       /* 门限触发 */
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
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x2) {
        return AM_OK;
    }

    if (p_this->dev_info->trigger_pin != -1) {
        ret = am_gpio_trigger_on(p_this->dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }

    /* 清中断标志位 */
    __as5600_status_clr_all(p_this);

    /* 使能中断 */
    __as5600_int_enable(p_this);

    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->trigger, 0);
        AM_BIT_SET(p_this->trigger, 1);
    }

    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_as5600_dev_t* p_this = (am_sensor_as5600_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

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

        if (cur_ret == AM_OK) {

            /* 禁能中断 */
            __as5600_int_disable(p_this);

            AM_BIT_CLR(p_this->trigger, 1);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 AS5600 初始化
 */
am_sensor_handle_t am_sensor_as5600_init (
        am_sensor_as5600_dev_t              *p_dev,
        const am_sensor_as5600_devinfo_t    *p_devinfo,
        am_i2c_handle_t                      handle)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->as5600_dev.p_drv   = p_dev;
    p_dev->as5600_dev.p_funcs = &__g_sensor_as5600_funcs;
    p_dev->dev_info           = p_devinfo;
    p_dev->pfn_trigger_cb[0]  = NULL;
    p_dev->p_arg[0]           = NULL;
    p_dev->flags[0]           = 0;
    p_dev->trigger            = 0;
    p_dev->data[0].val        = 0;
    p_dev->data[0].unit       = AM_SENSOR_UNIT_INVALID;

    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __as5600_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);  /* 下降沿触发 */
    }

    if (p_devinfo->dir_pin != -1) {
        if (p_devinfo->p_param_default->direction_polarity == AM_AS5600_INCREASE_CLOCKWISE) {
            am_gpio_pin_cfg(p_devinfo->dir_pin, AM_GPIO_OUTPUT_INIT_LOW);
        } else {
            am_gpio_pin_cfg(p_devinfo->dir_pin, AM_GPIO_OUTPUT_INIT_HIGH);
        }
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    return &(p_dev->as5600_dev);
}

/**
 * \brief 传感器 AS5600 去初始化
 */
am_err_t am_sensor_as5600_deinit (am_sensor_handle_t handle)
{
    am_sensor_as5600_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->as5600_dev.p_drv   = NULL;
    p_dev->as5600_dev.p_funcs = NULL;
    p_dev->dev_info           = NULL;

    return AM_OK;
}

/**
 * \brief 传感器 AS5600 控制函数
 */
am_err_t am_sensor_as5600_ioctl (am_sensor_handle_t     handle,
                                 int                    cmd,
                                 void                  *p_arg)
{
    am_sensor_as5600_dev_t   *p_dev = (am_sensor_as5600_dev_t *)handle->p_drv;

    am_err_t ret = AM_OK;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    ret = __as5600_ioctl(p_dev, cmd, p_arg);

    return ret;
}

/* end of file */

