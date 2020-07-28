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
 * \brief 传感器 TCS3430 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_tcs3430.h"
#include "am_sensor.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**< 调试信息输出 */
#define __TCS3430_DEBUG             (0)

/*********************************************************************************************
 * 宏定义
 ********************************************************************************************/
#define __TCS3430_REG_ENABLE        (0x80)  /**< \brief 使能状态                            */
#define __TCS3430_REG_ATIME         (0x81)  /**< \brief 控制ALS ADC的积分时间               */
#define __TCS3430_REG_WTIME         (0x83)  /**< \brief ALS等待时间                         */
#define __TCS3430_REG_AILTL         (0x84)  /**< \brief ALS中断低阈值低字节 (Channel 0)     */
#define __TCS3430_REG_AILTH         (0x85)  /**< \brief ALS中断低阈值高字节 (Channel 0)     */
#define __TCS3430_REG_AIHTL         (0x86)  /**< \brief ALS中断高阈值低字节 (Channel 0)     */
#define __TCS3430_REG_AIHTH         (0x87)  /**< \brief ALS中断高阈值高字节 (Channel 0)     */
#define __TCS3430_REG_PERS          (0x8C)  /**< \brief ALS中断过滤器                       */
#define __TCS3430_REG_CFG0          (0x8D)  /**< \brief 配置寄存器0                         */
#define __TCS3430_REG_CFG1          (0x90)  /**< \brief 配置寄存器1                         */
#define __TCS3430_REG_REVID         (0x91)  /**< \brief 修订ID                              */
#define __TCS3430_REG_ID            (0x92)  /**< \brief 设备ID                              */
#define __TCS3430_REG_STATUS        (0x93)  /**< \brief 设备状态                            */
#define __TCS3430_REG_CH0DATAL      (0x94)  /**< \brief CH0 通道ADC数据低字节               */
#define __TCS3430_REG_CH0DATAH      (0x95)  /**< \brief CH0 通道ADC数据高字节               */
#define __TCS3430_REG_CH1DATAL      (0x96)  /**< \brief CH1 通道ADC数据低字节               */
#define __TCS3430_REG_CH1DATAH      (0x97)  /**< \brief CH1 通道ADC数据高字节               */
#define __TCS3430_REG_CH2DATAL      (0x98)  /**< \brief CH2 通道ADC数据低字节               */
#define __TCS3430_REG_CH2DATAH      (0x99)  /**< \brief CH2 通道ADC数据高字节               */
#define __TCS3430_REG_CH3DATAL      (0x9A)  /**< \brief CH3 通道ADC数据低字节               */
#define __TCS3430_REG_CH3DATAH      (0x9B)  /**< \brief CH3 通道ADC数据高字节               */
#define __TCS3430_REG_CFG2          (0x9F)  /**< \brief 配置寄存器2                         */
#define __TCS3430_REG_CFG3          (0xAB)  /**< \brief 配置寄存器3                         */
#define __TCS3430_REG_AZ_CONFIG     (0xD6)  /**< \brief 自动调零                            */
#define __TCS3430_REG_INTENAB       (0xDD)  /**< \brief 中断使能                            */

/** \brief 设置APERS */
#define __TCS3430_APERS_SET(data, mask)             (((data) & (~(0xf << 0))) | (mask << 0))

/** \brief 设置WLONG(wait long) */
#define __TCS3430_WLONG_SET(data, mask)             (((data) & (~(0x1 << 2))) | (mask << 2))

/** \brief 设置AMUX */
#define __TCS3430_AMUX_SET(data, mask)              (((data) & (~(0x1 << 3))) | (mask << 3))

/** \brief 设置AGAIN */
#define __TCS3430_AGAIN_SET(data, mask)             (((data) & (~(0x3 << 0))) | (mask << 0))

/** \brief TCS3430 ID */
#define __TCS3430_ID                                (0xDC)

/** \brief 获取ASAT状态 */
#define __TCS3430_ASAT_STATUS(reg)                  (((reg) >> 7) & 0x1)

/** \brief 获取AINT状态 */
#define __TCS3430_AINT_STATUS(reg)                  (((reg) >> 4) & 0x1)

/** \brief 设置HGAIN */
#define __TCS3430_HGAIN_SET(data, mask)             (((data) & (~(0x1 << 4))) | (mask << 4))

/** \brief 设置SAI */
#define __TCS3430_SAI_SET(data, mask)               (((data) & (~(0x1 << 4))) | (mask << 4))

/** \brief 设置AZ_MODE */
#define __TCS3430_AZ_MODE_SET(data, mask)           (((data) & (~(0x1 << 7))) | (mask << 7))

/** \brief 设置AZ_NTH_ITERATION */
#define __TCS3430_AZ_NTH_ITERATION_SET(data, mask)  (((data) & (~(0x7f << 0))) | (mask << 0))

/** \brief 将两个uint8转换为一个uint16_t类型 */
#define __TCS3430_UINT8_TO_UINT16(buff)             ((uint16_t)((*(buff + 1)) << 8 | (*buff)))

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_tcs3430_funcs = {
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
 * \brief tcs3430 写数据
 */
am_local am_err_t __tcs3430_write (am_sensor_tcs3430_dev_t  *p_this,
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
 * \brief tcs3430 读数据
 */
am_local am_err_t __tcs3430_read (am_sensor_tcs3430_dev_t   *p_this,
                                  uint32_t                   subaddr,
                                  uint8_t                   *p_buf,
                                  uint32_t                   nbytes)
{
    return am_i2c_read(&p_this->i2c_dev,
                       subaddr,
                       p_buf,
                       nbytes);
}

/** \brief 设置阈值 */
am_local am_err_t __tcs3430_threshold_set(am_sensor_tcs3430_dev_t   *p_this,
                                          uint32_t                   subaddr,
                                          uint16_t                   value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    reg[0] = value & 0xff;
    reg[1] = (value & 0xff00) >> 8;

    ret = __tcs3430_write(p_this, subaddr, reg, 2);

    return ret;
}

/** \brief 获取阈值 */
am_local am_err_t __tcs3430_threshold_get(am_sensor_tcs3430_dev_t   *p_this,
                                          uint32_t                   subaddr,
                                          uint16_t                  *p_value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    ret = __tcs3430_read(p_this, subaddr, reg, 2);

    *p_value = __TCS3430_UINT8_TO_UINT16(&reg[0]);

    return ret;
}

/*******************************************************************************/

/** \brief 设置功能使能 */
am_local am_err_t __tcs3430_func_en_enable(am_sensor_tcs3430_dev_t  *p_this,
                                           uint8_t                   flag)
{
    uint8_t enable_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    enable_reg = (enable_reg & (~(flag))) | flag;
    ret = __tcs3430_write(p_this, __TCS3430_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置功能禁能 */
am_local am_err_t __tcs3430_func_disen_enable(am_sensor_tcs3430_dev_t   *p_this,
                                              uint8_t                    flag)
{
    uint8_t enable_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    enable_reg = enable_reg & (~(flag));
    ret = __tcs3430_write(p_this, __TCS3430_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置ATIME */
am_local am_err_t __tcs3430_atime(am_sensor_tcs3430_dev_t   *p_this,
                                  uint8_t                    atime)
{
    uint8_t atime_reg = 0x00;

    am_err_t ret = AM_OK;

    atime_reg = atime;
    ret = __tcs3430_write(p_this, __TCS3430_REG_ATIME, &atime_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置WTIME */
am_local am_err_t __tcs3430_wtime(am_sensor_tcs3430_dev_t   *p_this,
                                  uint8_t                    wtime)
{
    uint8_t wtime_reg = 0x00;

    am_err_t ret = AM_OK;

    wtime_reg = wtime;
    ret = __tcs3430_write(p_this, __TCS3430_REG_WTIME, &wtime_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置ALS低阈值 */
am_local am_err_t __tcs3430_low_als_threshold_set(am_sensor_tcs3430_dev_t   *p_this,
                                                  uint16_t                   low_als_value)
{
    return __tcs3430_threshold_set(p_this, __TCS3430_REG_AILTL, low_als_value);
}

/** \brief 设置ALS高阈值 */
am_local am_err_t __tcs3430_high_als_threshold_set(am_sensor_tcs3430_dev_t  *p_this,
                                                   uint16_t                  high_als_value)
{
    return __tcs3430_threshold_set(p_this, __TCS3430_REG_AIHTL, high_als_value);
}

/** \brief 获取ALS低阈值 */
am_local am_err_t __tcs3430_low_als_threshold_get(am_sensor_tcs3430_dev_t   *p_this,
                                                  uint16_t                  *p_low_als_value)
{
    return __tcs3430_threshold_get(p_this, __TCS3430_REG_AILTL, p_low_als_value);
}

/** \brief 获取ALS高阈值 */
am_local am_err_t __tcs3430_high_als_threshold_get(am_sensor_tcs3430_dev_t  *p_this,
                                                   uint16_t                 *p_high_als_value)
{
    return __tcs3430_threshold_get(p_this, __TCS3430_REG_AIHTL, p_high_als_value);
}

/** \brief 设置APERS */
am_local am_err_t __tcs3430_apers_pers(am_sensor_tcs3430_dev_t  *p_this,
                                       uint8_t                   apers)
{
    uint8_t pers_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_PERS, &pers_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    pers_reg = __TCS3430_APERS_SET(pers_reg, apers);
    ret = __tcs3430_write(p_this, __TCS3430_REG_PERS, &pers_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置WLONG(wait long) */
am_local am_err_t __tcs3430_wlong_cfg0(am_sensor_tcs3430_dev_t  *p_this,
                                       am_bool_t                 wlong_enable)
{
    uint8_t cfg0_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_CFG0, &cfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    cfg0_reg = __TCS3430_WLONG_SET(cfg0_reg, wlong_enable);
    ret = __tcs3430_write(p_this, __TCS3430_REG_CFG0, &cfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置AMUX */
am_local am_err_t __tcs3430_amux_cfg1(am_sensor_tcs3430_dev_t *p_this,
                                      am_bool_t                amux)
{
    uint8_t cfg1_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_CFG1, &cfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    cfg1_reg = __TCS3430_AMUX_SET(cfg1_reg, amux);
    ret = __tcs3430_write(p_this, __TCS3430_REG_CFG1, &cfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置ALS传感器增益 */
am_local am_err_t __tcs3430_again_cfg1(am_sensor_tcs3430_dev_t  *p_this,
                                       uint8_t                   again)
{
    uint8_t cfg1_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_CFG1, &cfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    cfg1_reg = __TCS3430_AGAIN_SET(cfg1_reg, again);
    ret = __tcs3430_write(p_this, __TCS3430_REG_CFG1, &cfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 读取REVID */
am_local am_err_t __tcs3430_read_revid(am_sensor_tcs3430_dev_t  *p_this,
                                       uint8_t                  *p_revid)
{
    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_REVID, p_revid, 1);
    if (ret != AM_OK) {
    #if __TCS3430_DEBUG
        am_kprintf("\r\nSensor TCS3430 read REVID failed! \r\n");
    #endif
    }

    return ret;
}

/** \brief 读取ID */
am_local am_err_t __tcs3430_read_id(am_sensor_tcs3430_dev_t *p_this,
                                    uint8_t                 *p_id)
{
    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_ID, p_id, 1);
    if (ret != AM_OK) {
    #if __TCS3430_DEBUG
        am_kprintf("\r\nSensor TCS3430 read ID failed! \r\n");
    #endif
    }

    return ret;
}

/** \brief STATUS寄存器位清除 */
am_local am_err_t __tcs3430_status_flag_clr(am_sensor_tcs3430_dev_t *p_this,
                                            uint8_t                  flag)
{
    uint8_t status_reg = 0x00;

    am_err_t ret = AM_OK;

    status_reg |= flag;
    ret = __tcs3430_write(p_this, __TCS3430_REG_STATUS, &status_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置HGAIN */
am_local am_err_t __tcs3430_hgain_cfg2(am_sensor_tcs3430_dev_t  *p_this,
                                       uint8_t                   hgain)
{
    uint8_t cfg2_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_CFG2, &cfg2_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    cfg2_reg = __TCS3430_HGAIN_SET(cfg2_reg, hgain);
    ret = __tcs3430_write(p_this, __TCS3430_REG_CFG2, &cfg2_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置AZ_MODE */
am_local am_err_t __tcs3430_az_mode_az_config(am_sensor_tcs3430_dev_t   *p_this,
                                              uint8_t                    az_mode)
{
    uint8_t az_config_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_AZ_CONFIG, &az_config_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    az_config_reg = __TCS3430_AZ_MODE_SET(az_config_reg, az_mode);
    ret = __tcs3430_write(p_this, __TCS3430_REG_AZ_CONFIG, &az_config_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置AZ_NTH_ITERATION */
am_local am_err_t __tcs3430_az_nth_iteration_az_config(am_sensor_tcs3430_dev_t  *p_this,
                                                       uint8_t                   az_nth_iteration)
{
    uint8_t az_config_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_AZ_CONFIG, &az_config_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    az_config_reg = __TCS3430_AZ_NTH_ITERATION_SET(az_config_reg, az_nth_iteration);
    ret = __tcs3430_write(p_this, __TCS3430_REG_AZ_CONFIG, &az_config_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 中断使能 */
am_local am_err_t __tcs3430_interrupt_enable_intenab(am_sensor_tcs3430_dev_t    *p_this,
                                                     uint8_t                     flag)
{
    uint8_t intenab_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    intenab_reg = (intenab_reg & (~(flag))) | flag;
    ret = __tcs3430_write(p_this, __TCS3430_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 中断禁能 */
am_local am_err_t __tcs3430_interrupt_disable_intenab(am_sensor_tcs3430_dev_t   *p_this,
                                                      uint8_t                    flag)
{
    uint8_t intenab_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tcs3430_read(p_this, __TCS3430_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    intenab_reg = intenab_reg & (~(flag));
    ret = __tcs3430_write(p_this, __TCS3430_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/*******************************************************************************/

/** \brief TCS3430 设置ALS传感器增益 */
am_local am_err_t __tcs3430_als_gain_set(am_sensor_tcs3430_dev_t   *p_this,
                                         uint8_t                    als_gain)
{
    am_err_t ret = AM_OK;

    switch(als_gain) {

    /* 1 Gain */
    case AM_TCS3430_AGAIN_1:
        __tcs3430_again_cfg1(p_this, 0);
        __tcs3430_hgain_cfg2(p_this, 0);
        break;

    /* 4 Gain */
    case AM_TCS3430_AGAIN_4:
        __tcs3430_again_cfg1(p_this, 1);
        __tcs3430_hgain_cfg2(p_this, 0);
        break;

    /* 16 Gain */
    case AM_TCS3430_AGAIN_16:
        __tcs3430_again_cfg1(p_this, 2);
        __tcs3430_hgain_cfg2(p_this, 0);
        break;

    /* 64 Gain */
    case AM_TCS3430_AGAIN_64:
        __tcs3430_again_cfg1(p_this, 3);
        __tcs3430_hgain_cfg2(p_this, 0);
        break;

    /* 128 Gain */
    case AM_TCS3430_AGAIN_128:
        __tcs3430_again_cfg1(p_this, 3);
        __tcs3430_hgain_cfg2(p_this, 1);
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/** \brief TCS3430 清空STATUS寄存器 */
am_local uint8_t __tcs3430_status_clr_all (am_sensor_tcs3430_dev_t  *p_this)
{
    uint8_t status_reg = 0x00;
    uint8_t cfg3_reg = 0x00;

    cfg3_reg |= (1 << 7);   /* CFG3的INT_READ_CLEAR置1 */

    __tcs3430_write(p_this, __TCS3430_REG_CFG3, &cfg3_reg, 1);

    __tcs3430_read(p_this, __TCS3430_REG_STATUS, &status_reg, 1);

    cfg3_reg &= ~(1 << 7);  /* CFG3的INT_READ_CLEAR置0 */

    __tcs3430_write(p_this, __TCS3430_REG_CFG3, &cfg3_reg, 1);

    return status_reg;
}

/** \brief TCS3430 参数初始化 */
am_local am_err_t __tcs3430_param_init(am_sensor_tcs3430_dev_t  *p_this)
{
    tcs3430_param_config_t *p_param_config = (tcs3430_param_config_t *)p_this->dev_info->p_param_default;

    am_err_t ret = AM_OK;

    uint8_t reg = 0x00;     /* 缓存寄存器值 */

    /* ATIME 寄存器配置 */
    ret = __tcs3430_atime(p_this, p_param_config->atime_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* WTIME 寄存器配置 */
    ret = __tcs3430_wtime(p_this, p_param_config->wtime_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* PERS 寄存器配置 */
    ret = __tcs3430_apers_pers(p_this, p_param_config->apers_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG0 寄存器配置 */
    ret = __tcs3430_wlong_cfg0(p_this, p_param_config->wlong_en);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG1 寄存器配置 */
    ret = __tcs3430_amux_cfg1(p_this, p_param_config->a_mux);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG1 寄存器配置 */   /* CFG2 寄存器配置 */
    ret = __tcs3430_als_gain_set(p_this, p_param_config->again_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG3 寄存器配置 */
    __tcs3430_status_clr_all(p_this);      /* 清除STATUS寄存器 */

    reg = 0x00;
    reg = __TCS3430_SAI_SET(reg, p_param_config->sleep_after_interrupt);
    ret = __tcs3430_write(p_this, __TCS3430_REG_CFG3, &reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    /* AZ_CONFIG 寄存器配置 */
    ret = __tcs3430_az_mode_az_config(p_this, p_param_config->az_mode);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tcs3430_az_nth_iteration_az_config(p_this, p_param_config->az_nth_iteration_val);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

#if __TCS3430_DEBUG
const char *__tcs3430_reg_name_string[] = {
"ENABLE   ",
"ATIME    ",
"WTIME    ",
"AILTL    ",
"AILTH    ",
"AIHTL    ",
"AIHTH    ",
"PERS     ",
"CFG0     ",
"CFG1     ",
"REVID    ",
"ID       ",
"STATUS   ",
"CH0DATAL ",
"CH0DATAH ",
"CH1DATAL ",
"CH1DATAH ",
"CH2DATAL ",
"CH2DATAH ",
"CH3DATAL ",
"CH3DATAH ",
"CFG2     ",
"CFG3     ",
"AZ_CONFIG",
"INTENAB  ",
};

am_local void __tcs3430_regval_check(am_sensor_tcs3430_dev_t *p_this)
{
    uint8_t reg_val[25]         = {0};         /* 缓存寄存器值 */
    uint8_t counter = 0;

    __tcs3430_read(p_this, __TCS3430_REG_ENABLE, reg_val, 2);
    for (counter = 0; counter < 2;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);
    }

    __tcs3430_read(p_this, __TCS3430_REG_WTIME, &reg_val[2], 5);
    for (counter = 2; counter < 7;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);
    }

    __tcs3430_read(p_this, __TCS3430_REG_PERS, &reg_val[7], 2);
    for (counter = 7; counter < 9;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);
    }

    __tcs3430_read(p_this, __TCS3430_REG_CFG1, &reg_val[9], 12);
    for (counter = 9; counter < 21;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);
    }

    __tcs3430_read(p_this, __TCS3430_REG_CFG2, &reg_val[21], 1);
    counter = 21;
    am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);

    __tcs3430_read(p_this, __TCS3430_REG_CFG3, &reg_val[22], 1);
    counter = 22;
    am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);

    __tcs3430_read(p_this, __TCS3430_REG_AZ_CONFIG, &reg_val[23], 1);
    counter = 23;
    am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);

    __tcs3430_read(p_this, __TCS3430_REG_INTENAB, &reg_val[24], 1);
    counter = 24;
    am_kprintf("%s : 0x%02x \r\n",__tcs3430_reg_name_string[counter], reg_val[counter]);
}
#endif

/**
 * \brief TCS3430 获取结果
 */
am_local am_err_t __tcs3430_result_get(am_sensor_tcs3430_dev_t  *p_this,
                                       tcs3430_result_info_t    *p_result)
{
    am_err_t ret = AM_OK;

    uint8_t result_buf[8]  = {0};         /* 缓存寄存器值 */

    __tcs3430_read(p_this, __TCS3430_REG_CH0DATAL, result_buf, 8);

    /* CH0 */
    p_result->channel_0_data = __TCS3430_UINT8_TO_UINT16(&result_buf[0]);

    /* CH1 */
    p_result->channel_1_data = __TCS3430_UINT8_TO_UINT16(&result_buf[2]);

    /* CH2 */
    p_result->channel_2_data = __TCS3430_UINT8_TO_UINT16(&result_buf[4]);

    /* CH3 */
    p_result->channel_3_data = __TCS3430_UINT8_TO_UINT16(&result_buf[6]);

    return ret;
}

/*******************************************************************************/

/** \breif 中断回调函数 */
am_local void __tcs3430_alarm_callback (void *p_arg)
{
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_arg;

    uint8_t i = 0;

    tcs3430_result_info_t result;

    /* 获取数据 */
    __tcs3430_result_get(p_this, &result);

    /* 获取数据后，清中断标志位 */
    __tcs3430_status_clr_all(p_this);

    /* 获取CH0 */
    p_this->data[0].val = result.channel_0_data;
    p_this->data[0].unit = AM_SENSOR_UNIT_BASE;

    /* 获取CH1 */
    p_this->data[1].val = result.channel_1_data;
    p_this->data[1].unit = AM_SENSOR_UNIT_BASE;

    /* 获取CH2 */
    p_this->data[2].val = result.channel_2_data;
    p_this->data[2].unit = AM_SENSOR_UNIT_BASE;

    /* 获取CH3 */
    p_this->data[3].val = result.channel_3_data;
    p_this->data[3].unit = AM_SENSOR_UNIT_BASE;

    for (i = 0; i < 4; i++) {
        if (p_this->pfn_trigger_cb[i] &&
                (p_this->flags[i] & AM_SENSOR_TRIGGER_THRESHOLD)) {
            p_this->pfn_trigger_cb[i](p_this->p_arg[i],
                                      AM_SENSOR_TRIGGER_THRESHOLD);
        }
    }
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if ((id == AM_TCS3430_CHAN_1) || (id == AM_TCS3430_CHAN_2) ||
        (id == AM_TCS3430_CHAN_3) || (id == AM_TCS3430_CHAN_4)) {
        return AM_SENSOR_TYPE_ALS;
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
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    tcs3430_result_info_t result;

    int cur_id = 0;
    int i = 0;

    uint16_t times = 0xff;       /* 读取数据时间 */

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (AM_BIT_GET(p_this->trigger, 15) != 1) {
        return -AM_EPERM;
    }

    for (i = 0; i < num; i++) {
        p_buf[i].unit = AM_SENSOR_UNIT_INVALID;
    }

    /* 若为1，则可能在门限触发回调函数中使用 */
    if (num == 1) {
        cur_id = p_ids[0];
        /* 若打开门限触发方式，则直接赋值 */
        if ((AM_BIT_GET(p_this->trigger, 4)) &&
                ((p_this->flags[0] & AM_SENSOR_TRIGGER_THRESHOLD) ||
                 (p_this->flags[1] & AM_SENSOR_TRIGGER_THRESHOLD) ||
                 (p_this->flags[2] & AM_SENSOR_TRIGGER_THRESHOLD) ||
                 (p_this->flags[3] & AM_SENSOR_TRIGGER_THRESHOLD))) {
            p_buf[0].val = p_this->data[cur_id].val;
            p_buf[0].unit= p_this->data[cur_id].unit;
            return AM_OK;
        }
    }

    /** \brief 读取结果数据 */
    do {
        ret = __tcs3430_result_get(p_this, &result);
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

            p_buf[i].val  = result.channel_0_data;  /*< \brief CH0 */
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;    /*< \brief 单位默认为0:10^(0)*/

        } else if (cur_id == 1) {

            p_buf[i].val  = result.channel_1_data;  /*< \brief CH1 */
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;    /*< \brief 单位默认为0:10^(0)*/

        } else if (cur_id == 2) {

            p_buf[i].val  = result.channel_2_data;  /*< \brief CH2 */
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;    /*< \brief 单位默认为0:10^(0)*/

        } else if (cur_id == 3) {

            p_buf[i].val  = result.channel_3_data;  /*< \brief CH3 */
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
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    am_err_t ret = -AM_ENODEV;
    am_err_t curent_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 ||
            cur_id == 2 || cur_id == 3) {
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

    /* 寄存器参数初始化 */
    ret = __tcs3430_param_init(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器配置使能 */
    ret = __tcs3430_func_en_enable(p_this,
                                  (AM_TCS3430_WEN | \
                                   AM_TCS3430_AEN | \
                                   AM_TCS3430_PON ));
    if (ret != AM_OK) {
        return ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 15);
        AM_BIT_SET(p_this->trigger, 14);
        AM_BIT_SET(p_this->trigger, 13);
        AM_BIT_SET(p_this->trigger, 12);
        AM_BIT_SET(p_this->trigger, 11);
    }

    return curent_ret;
}

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_drv;

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

            AM_BIT_CLR(p_this->trigger, 14);
            ret = AM_OK;

        } else if (cur_id == 1) {

            AM_BIT_CLR(p_this->trigger, 13);
            ret = AM_OK;

        } else if (cur_id == 2) {

            AM_BIT_CLR(p_this->trigger, 12);
            ret = AM_OK;

        } else if (cur_id == 3) {

            AM_BIT_CLR(p_this->trigger, 11);
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

        /* enable 寄存器配置禁能 */
        ret = __tcs3430_func_disen_enable(p_this,
                                         (AM_TCS3430_WEN | \
                                          AM_TCS3430_AEN | \
                                          AM_TCS3430_PON ));
        if (ret != AM_OK){
            cur_ret = ret;
        }

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
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_drv;

    if (id != 0 && id != 1 && 
        id != 2 && id != 3) {
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
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0 && id != 1 && 
        id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x10) {
        return AM_OK;
    }

    if (p_this->dev_info->trigger_pin != -1) {
        ret = am_gpio_trigger_on(p_this->dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }

    /* 清中断标志位 */
    __tcs3430_status_clr_all(p_this);

    /* 使能中断 */
    __tcs3430_interrupt_enable_intenab(p_this, AM_TCS3430_AIEN);    /* ALS中断 */

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

    #if __TCS3430_DEBUG
        __tcs3430_regval_check(p_this);
    #endif

    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_tcs3430_dev_t* p_this = (am_sensor_tcs3430_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0 && id != 1 && 
        id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_this->pfn_trigger_cb[id] = NULL;
    p_this->p_arg[id]          = NULL;
    p_this->flags[id]          = 0;

    AM_BIT_CLR(p_this->trigger, id);

    if ((p_this->trigger & 0xf) == 0) {

        if ((p_this->dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        if (cur_ret == AM_OK) {

            /* 禁能中断 */
            __tcs3430_interrupt_disable_intenab(p_this, AM_TCS3430_AIEN);    /* ALS中断 */
            AM_BIT_CLR(p_this->trigger, 4);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 TCS3430 初始化
 */
am_sensor_handle_t am_sensor_tcs3430_init (
        am_sensor_tcs3430_dev_t             *p_dev,
        const am_sensor_tcs3430_devinfo_t   *p_devinfo,
        am_i2c_handle_t                      handle)
{
    am_err_t ret = AM_OK;

    uint8_t tcs3430_id = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->tcs3430_dev.p_drv    = p_dev;
    p_dev->tcs3430_dev.p_funcs  = &__g_sensor_tcs3430_funcs;
    p_dev->dev_info             = p_devinfo;
    p_dev->pfn_trigger_cb[0]    = NULL;
    p_dev->pfn_trigger_cb[1]    = NULL;
    p_dev->pfn_trigger_cb[2]    = NULL;
    p_dev->pfn_trigger_cb[3]    = NULL;
    p_dev->p_arg[0]             = NULL;
    p_dev->p_arg[1]             = NULL;
    p_dev->p_arg[2]             = NULL;
    p_dev->p_arg[3]             = NULL;
    p_dev->flags[0]             = 0;
    p_dev->flags[1]             = 0;
    p_dev->flags[2]             = 0;
    p_dev->flags[3]             = 0;
    p_dev->trigger              = 0;
    p_dev->data[0].val          = 0;
    p_dev->data[0].unit         = AM_SENSOR_UNIT_INVALID;
    p_dev->data[1].val          = 0;
    p_dev->data[1].unit         = AM_SENSOR_UNIT_INVALID;
    p_dev->data[2].val          = 0;
    p_dev->data[2].unit         = AM_SENSOR_UNIT_INVALID;
    p_dev->data[3].val          = 0;
    p_dev->data[3].unit         = AM_SENSOR_UNIT_INVALID;

    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __tcs3430_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);  /* 下降沿触发 */
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    ret = __tcs3430_read_id(p_dev, &tcs3430_id);
    if ((ret != AM_OK) || (tcs3430_id != __TCS3430_ID)) {
    #if __TCS3430_DEBUG
        am_kprintf("tcs3430_id = 0x%x \r\n", tcs3430_id);
    #endif
        am_kprintf("\r\nSensor TCS3430 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->tcs3430_dev);
}

/**
 * \brief 传感器 TCS3430 去初始化
 */
am_err_t am_sensor_tcs3430_deinit (am_sensor_handle_t handle)
{
    am_sensor_tcs3430_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->tcs3430_dev.p_drv    = NULL;
    p_dev->tcs3430_dev.p_funcs  = NULL;
    p_dev->dev_info             = NULL;

    return AM_OK;
}

/**
 * \brief 传感器 TCS3430 控制函数
 */
am_err_t am_sensor_tcs3430_ioctl (am_sensor_handle_t    handle,
                                  int                   cmd,
                                  void                 *p_arg)
{
    am_sensor_tcs3430_dev_t   *p_dev = (am_sensor_tcs3430_dev_t *)handle->p_drv;

    am_err_t ret = AM_OK;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    switch (cmd) {

    /* 设置功能使能 */
    case AM_SENSOR_TCS3430_FUNC_ENABLE:
        ret = __tcs3430_func_en_enable(p_dev,
                                      (uint8_t)(int)p_arg);
        break;

    /* 设置功能禁能 */
    case AM_SENSOR_TCS3430_FUNC_DISABLE:
        ret = __tcs3430_func_disen_enable(p_dev,
                                         (uint8_t)(int)p_arg);
        break;

    /* 设置ATIME */
    case AM_SENSOR_TCS3430_ATIME_SET:
        ret = __tcs3430_atime(p_dev,
                             (uint8_t)(int)p_arg);
        break;

    /* 设置WTIME */
    case AM_SENSOR_TCS3430_WTIME_SET:
        ret = __tcs3430_wtime(p_dev,
                             (uint8_t)(int)p_arg);
        break;

    /* 设置ALS低阈值 */
    case AM_SENSOR_TCS3430_LOW_ALS_THRESHOLD_SET:
        ret = __tcs3430_low_als_threshold_set(p_dev,
                                             (uint16_t)(int)p_arg);
        break;

    /* 设置ALS高阈值 */
    case AM_SENSOR_TCS3430_HIGH_ALS_THRESHOLD_SET:
        ret = __tcs3430_high_als_threshold_set(p_dev,
                                              (uint16_t)(int)p_arg);
        break;

    /* 获取ALS低阈值 */
    case AM_SENSOR_TCS3430_LOW_ALS_THRESHOLD_GET:
        ret = __tcs3430_low_als_threshold_get(p_dev,
                                             (uint16_t *)p_arg);
        break;

    /* 获取ALS高阈值 */
    case AM_SENSOR_TCS3430_HIGH_ALS_THRESHOLD_GET:
        ret = __tcs3430_high_als_threshold_get(p_dev,
                                              (uint16_t *)p_arg);
        break;

    /* 设置APERS */
    case AM_SENSOR_TCS3430_APERS_SET:
        ret = __tcs3430_apers_pers(p_dev,
                                  (uint8_t)(int)p_arg);
        break;

    /* 设置WLONG(wait long) */
    case AM_SENSOR_TCS3430_WLONG_SET:
        ret = __tcs3430_wlong_cfg0(p_dev,
                                  (am_bool_t)(int)p_arg);
        break;

    /* 设置AMUX */
    case AM_SENSOR_TCS3430_AMUX_SET:
        ret = __tcs3430_amux_cfg1(p_dev,
                                 (am_bool_t)(int)p_arg);
        break;

    /* 设置ALS传感器增益 */
    case AM_SENSOR_TCS3430_AGAIN_SET:
        ret = __tcs3430_als_gain_set(p_dev,
                                    (uint8_t)(int)p_arg);
        break;

    /* 读取REVID */
    case AM_SENSOR_TCS3430_REVID_GET:
        ret = __tcs3430_read_revid(p_dev, (uint8_t *)p_arg);
        break;

    /* 读取ID */
    case AM_SENSOR_TCS3430_ID_GET:
        ret = __tcs3430_read_id(p_dev, (uint8_t *)p_arg);
        break;

    /* STATUS寄存器位清除 */
    case AM_SENSOR_TCS3430_STATUS_FLAG_CLR:
        ret = __tcs3430_status_flag_clr(p_dev,
                                       (uint8_t)(int)p_arg);
        break;

    /* 设置AZ_MODE */
    case AM_SENSOR_TCS3430_AZ_MODE_SET:
        ret = __tcs3430_az_mode_az_config(p_dev,
                                         (uint8_t)(int)p_arg);
        break;

    /* 设置AZ_NTH_ITERATION */
    case AM_SENSOR_TCS3430_AZ_NTH_ITERATION_SET:
        ret = __tcs3430_az_nth_iteration_az_config(p_dev,
                                                  (uint8_t)(int)p_arg);
        break;

    /* 中断使能 */
    case AM_SENSOR_TCS3430_INT_ENABLE:
        ret = __tcs3430_interrupt_enable_intenab(p_dev,
                                                (uint8_t)(int)p_arg);
        break;

    /* 中断禁能 */
    case AM_SENSOR_TCS3430_INT_DISABLE:
        ret = __tcs3430_interrupt_disable_intenab(p_dev,
                                                 (uint8_t)(int)p_arg);
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/* end of file */

