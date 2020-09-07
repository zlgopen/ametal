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
 * \brief 传感器 AS7341 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as7341.h"
#include "am_sensor.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**< 调试信息输出 */
#define __AS7341_DEBUG                  (0)

/************************************************************
 * 宏定义
 ***********************************************************/
#define __AS7341_REG_ITIME_L            (0x63)
#define __AS7341_REG_ITIME_M            (0x64)
#define __AS7341_REG_ITIME_H            (0x65)
#define __AS7341_REG_CONFIG             (0x70)
#define __AS7341_REG_STAT               (0x71)
#define __AS7341_REG_EDGE               (0x72)
#define __AS7341_REG_GPIO               (0x73)
#define __AS7341_REG_LED                (0x74)
#define __AS7341_REG_ENABLE             (0x80)
#define __AS7341_REG_ATIME              (0x81)
#define __AS7341_REG_WTIME              (0x83)
#define __AS7341_REG_SP_TH_L_LSB        (0x84)
#define __AS7341_REG_SP_TH_L_MSB        (0x85)
#define __AS7341_REG_SP_TH_H_LSB        (0x86)
#define __AS7341_REG_SP_TH_H_MSB        (0x87)
#define __AS7341_REG_AUXID              (0x90)
#define __AS7341_REG_REVID              (0x91)
#define __AS7341_REG_ID                 (0x92)
#define __AS7341_REG_STATUS             (0x93)
#define __AS7341_REG_ASTATUS            (0x94)
#define __AS7341_REG_CH0_DATA_L         (0x95)
#define __AS7341_REG_CH0_DATA_H         (0x96)
#define __AS7341_REG_CH1_DATA_L         (0x97)
#define __AS7341_REG_CH1_DATA_H         (0x98)
#define __AS7341_REG_CH2_DATA_L         (0x99)
#define __AS7341_REG_CH2_DATA_H         (0x9A)
#define __AS7341_REG_CH3_DATA_L         (0x9B)
#define __AS7341_REG_CH3_DATA_H         (0x9C)
#define __AS7341_REG_CH4_DATA_L         (0x9D)
#define __AS7341_REG_CH4_DATA_H         (0x9E)
#define __AS7341_REG_CH5_DATA_L         (0x9F)
#define __AS7341_REG_CH5_DATA_H         (0xA0)
#define __AS7341_REG_STATUS2            (0xA3)
#define __AS7341_REG_STATUS3            (0xA4)
#define __AS7341_REG_STATUS5            (0xA6)
#define __AS7341_REG_STATUS6            (0xA7)
#define __AS7341_REG_CFG0               (0xA9)
#define __AS7341_REG_CFG1               (0xAA)
#define __AS7341_REG_CFG3               (0xAC)
#define __AS7341_REG_CFG6               (0xAF)
#define __AS7341_REG_CFG8               (0xB1)
#define __AS7341_REG_CFG9               (0xB2)
#define __AS7341_REG_CFG10              (0xB3)
#define __AS7341_REG_CFG12              (0xB5)
#define __AS7341_REG_PERS               (0xBD)
#define __AS7341_REG_GPIO2              (0xBE)
#define __AS7341_REG_ASTEPL             (0xCA)
#define __AS7341_REG_ASTEPH             (0xCB)
#define __AS7341_REG_AGC_GAIN_MAX       (0xCF)
#define __AS7341_REG_AZ_CONFIG          (0xD6)
#define __AS7341_REG_FD_TIME1           (0xD8)
#define __AS7341_REG_FD_TIME2           (0xDA)
#define __AS7341_REG_FIFO_CFG0          (0xD7)
#define __AS7341_REG_FD_STATUS          (0xDB)
#define __AS7341_REG_INTENAB            (0xF9)
#define __AS7341_REG_CONTROL            (0xFA)
#define __AS7341_REG_FIFO_MAP           (0xFC)
#define __AS7341_REG_FIFO_LVL           (0xFD)
#define __AS7341_REG_FDATAL             (0xFE)
#define __AS7341_REG_FDATAH             (0xFF)

/** \brief 设置LED_SEL */
#define __AS7341_MASK_CONFIG_LED_SEL                        (0x1 << 3)
#define __AS7341_LED_SEL_SET(val)                           ((val) << 3)

/** \brief 设置INT_SEL */
#define __AS7341_MASK_CONFIG_INT_SEL                        (0x1 << 2)
#define __AS7341_INT_SEL_SET(val)                           ((val) << 2)

/** \brief 设置INT_MODE */
#define __AS7341_MASK_CONFIG_INT_MODE                       (0x3 << 0)
#define __AS7341_INT_MODE_SET(val)                          ((val) << 0)

/** \brief 获取WAIT_SYNC */
#define __AS7341_STAT_WAIT_SYNC_GET(reg)                    (((reg) >> 1) & 0x1)

/** \brief 获取READY */
#define __AS7341_STAT_READY_GET(reg)                        (((reg) >> 0) & 0x1)

/** \brief 设置PD_GPIO */
#define __AS7341_MASK_GPIO_PD_GPIO                          (0x1 << 1)
#define __AS7341_PD_GPIO_SET(val)                           ((val) << 1)

/** \brief 设置PD_INT */
#define __AS7341_MASK_GPIO_PD_INT                           (0x1 << 0)
#define __AS7341_PD_INT_SET(val)                            ((val) << 0)

/** \brief 设置LED_ACT */
#define __AS7341_MASK_LED_LED_ACT                           (0x1 << 7)
#define __AS7341_LED_ACT_SET(val)                           ((val) << 7)

/** \brief 设置LED_DRIVE */
#define __AS7341_MASK_LED_LED_DRIVE                         (0x7f << 0)
#define __AS7341_LED_DRIVE_SET(val)                         ((val) << 0)

/** \brief AS7341 ID */
#define __AS7341_MASK_ID                                    (0xFC)
#define __AS7341_ID                                         (0x24)

/** \brief 获取ASAT */
#define __AS7341_STATUS_ASAT_GET(reg)                       (((reg) >> 7) & 0x1)

/** \brief 获取AINT */
#define __AS7341_STATUS_AINT_GET(reg)                       (((reg) >> 3) & 0x1)

/** \brief 获取FINT */
#define __AS7341_STATUS_FINT_GET(reg)                       (((reg) >> 2) & 0x1)

/** \brief 获取CINT */
#define __AS7341_STATUS_CINT_GET(reg)                       (((reg) >> 1) & 0x1)

/** \brief 获取SINT */
#define __AS7341_STATUS_SINT_GET(reg)                       (((reg) >> 0) & 0x1)

/** \brief 获取ASAT_STATUS */
#define __AS7341_ASTATUS_ASAT_STATUS_GET(reg)               (((reg) >> 7) & 0x1)

/** \brief 获取AGAIN_STATUS */
#define __AS7341_ASTATUS_AGAIN_STATUS_GET(reg)              (((reg) >> 0) & 0xf)

/** \brief 获取AVALID */
#define __AS7341_STATUS2_AVALID_GET(reg)                    (((reg) >> 6) & 0x1)

/** \brief 获取ASAT_DIGITAL */
#define __AS7341_STATUS2_ASAT_DIGITAL_GET(reg)              (((reg) >> 4) & 0x1)

/** \brief 获取ASAT_ANALOG */
#define __AS7341_STATUS2_ASAT_ANALOG_GET(reg)               (((reg) >> 3) & 0x1)

/** \brief 获取FDSAT_ANALOG */
#define __AS7341_STATUS2_FDSAT_ANALOG_GET(reg)              (((reg) >> 1) & 0x1)

/** \brief 获取FDSAT_DIGITAL */
#define __AS7341_STATUS2_FDSAT_DIGITAL_GET(reg)             (((reg) >> 0) & 0x1)

/** \brief 获取INT_SP_H */
#define __AS7341_STATUS3_INT_SP_H_GET(reg)                  (((reg) >> 5) & 0x1)

/** \brief 获取INT_SP_L */
#define __AS7341_STATUS3_INT_SP_L_GET(reg)                  (((reg) >> 4) & 0x1)

/** \brief 获取SINT_FD */
#define __AS7341_STATUS5_SINT_FD_GET(reg)                   (((reg) >> 3) & 0x1)

/** \brief 获取SINT_SMUX */
#define __AS7341_STATUS5_SINT_SMUX_GET(reg)                 (((reg) >> 2) & 0x1)

/** \brief 获取FIFO_OV */
#define __AS7341_STATUS6_FIFO_OV_GET(reg)                   (((reg) >> 7) & 0x1)

/** \brief 获取OVTEMP */
#define __AS7341_STATUS6_OVTEMP_GET(reg)                    (((reg) >> 5) & 0x1)

/** \brief 获取FD_TRIG */
#define __AS7341_STATUS6_FD_TRIG_GET(reg)                   (((reg) >> 4) & 0x1)

/** \brief 获取SP_TRIG */
#define __AS7341_STATUS6_SP_TRIG_GET(reg)                   (((reg) >> 2) & 0x1)

/** \brief 获取SAI_ACTIVE */
#define __AS7341_STATUS6_SAI_ACTIVE_GET(reg)                (((reg) >> 1) & 0x1)

/** \brief 获取INT_BUSY */
#define __AS7341_STATUS6_INT_BUSY_GET(reg)                  (((reg) >> 0) & 0x1)

/** \brief 设置LOWPOWER_IDLE */
#define __AS7341_MASK_CFG0_LOWPOWER_IDLE                    (0x1 << 5)
#define __AS7341_LOWPOWER_IDLE_SET(val)                     ((val) << 5)

/** \brief 设置REG_BANK */
#define __AS7341_MASK_CFG0_REG_BANK                         (0x1 << 4)
#define __AS7341_REG_BANK_SET(val)                          ((val) << 4)

/** \brief 获取REG_BANK */
#define __AS7341_CFG0_REG_BANK_GET(reg)                     (((reg) >> 4) & 0x1)

/** \brief 设置WLONG(wait long) */
#define __AS7341_MASK_CFG0_WLONG                            (0x1 << 2)
#define __AS7341_WLONG_SET(val)                             ((val) << 2)

/** \brief 设置AGAIN */
#define __AS7341_MASK_CFG1_AGAIN                            (0x1f << 0)
#define __AS7341_AGAIN_SET(val)                             ((val) << 0)

/** \brief 设置SAI */
#define __AS7341_MASK_CFG3_SAI                              (0x1 << 4)
#define __AS7341_SAI_SET(val)                               ((val) << 4)

/** \brief 设置SMUX_CMD */
#define __AS7341_MASK_CFG6_SMUX_CMD                         (0x3 << 3)
#define __AS7341_SMUX_CMD_SET(val)                          ((val) << 3)

/** \brief 设置FIFO_TH */
#define __AS7341_MASK_CFG8_FIFO_TH                          (0x3 << 6)
#define __AS7341_FIFO_TH_SET(val)                           ((val) << 6)

/** \brief 设置FD_AGC */
#define __AS7341_MASK_CFG8_FD_AGC                           (0x1 << 3)
#define __AS7341_FD_AGC_SET(val)                            ((val) << 3)

/** \brief 设置SP_AGC */
#define __AS7341_MASK_CFG8_SP_AGC                           (0x1 << 2)
#define __AS7341_SP_AGC_SET(val)                            ((val) << 2)

/** \brief 设置SIEN_FD */
#define __AS7341_MASK_CFG9_SIEN_FD                          (0x1 << 6)
#define __AS7341_SIEN_FD_SET(val)                           ((val) << 6)

/** \brief 设置SIEN_SMUX */
#define __AS7341_MASK_CFG9_SIEN_SMUX                        (0x1 << 4)
#define __AS7341_SIEN_SMUX_SET(val)                         ((val) << 4)

/** \brief 设置AGC_H */
#define __AS7341_MASK_CFG10_AGC_H                           (0x3 << 6)
#define __AS7341_AGC_H_SET(val)                             ((val) << 6)

/** \brief 设置AGC_L */
#define __AS7341_MASK_CFG10_AGC_L                           (0x3 << 4)
#define __AS7341_AGC_L_SET(val)                             ((val) << 4)

/** \brief 设置FD_PERS */
#define __AS7341_MASK_CFG10_FD_PERS                         (0x7 << 0)
#define __AS7341_FD_PERS_SET(val)                           ((val) << 0)

/** \brief 设置SP_TH_CH */
#define __AS7341_MASK_CFG12_SP_TH_CH                        (0x7 << 0)
#define __AS7341_SP_TH_CH_SET(val)                          ((val) << 0)

/** \brief 设置APERS */
#define __AS7341_MASK_PERS_APERS                            (0xf << 0)
#define __AS7341_APERS_SET(val)                             ((val) << 0)

/** \brief 设置GPIO_INV */
#define __AS7341_MASK_GPIO2_GPIO_INV                        (0x1 << 3)
#define __AS7341_GPIO_INV_SET(val)                          ((val) << 3)

/** \brief 设置GPIO_IN_EN */
#define __AS7341_MASK_GPIO2_GPIO_IN_EN                      (0x1 << 2)
#define __AS7341_GPIO_IN_EN_SET(val)                        ((val) << 2)

/** \brief 设置GPIO_OUT */
#define __AS7341_MASK_GPIO2_GPIO_OUT                        (0x1 << 1)
#define __AS7341_GPIO_OUT_SET(val)                          ((val) << 1)

/** \brief 获取GPIO_IN */
#define __AS7341_GPIO2_GPIO_IN_GET(reg)                     (((reg) >> 0) & 0x1)

/** \brief 设置AGC_FD_GAIN_MAX */
#define __AS7341_MASK_AGC_GAIN_MAX_AGC_FD_GAIN_MAX          (0xf << 4)
#define __AS7341_AGC_FD_GAIN_MAX_SET(val)                   ((val) << 4)

/** \brief 设置AGC_AGAIN_MAX */
#define __AS7341_MASK_AGC_GAIN_MAX_AGC_AGAIN_MAX            (0xf << 0)
#define __AS7341_AGC_AGAIN_MAX_SET(val)                     ((val) << 0)

/** \brief 设置FD_GAIN */
#define __AS7341_MASK_FD_TIME2_FD_GAIN                      (0x1f << 3)
#define __AS7341_FD_GAIN_SET(val)                           ((val) << 3)

/** \brief 设置FD_TIME_MSB */
#define __AS7341_MASK_FD_TIME2_FD_TIME_MSB                  (0x7 << 0)
#define __AS7341_FD_TIME_MSB_SET(val)                       ((val) << 0)

/** \brief 设置FIFO_WRITE_FD */
#define __AS7341_MASK_FIFO_CFG0_FIFO_WRITE_FD               (0x1 << 7)
#define __AS7341_FIFO_WRITE_FD_SET(val)                     ((val) << 7)

/** \brief 获取FD_MEASUREMENT_VALID */
#define __AS7341_FD_STATUS_FD_MEASUREMENT_VALID_GET(reg)    (((reg) >> 5) & 0x1)

/** \brief 获取FD_SATURATION_DETECTED */
#define __AS7341_FD_STATUS_FD_SATURATION_DETECTED_GET(reg)  (((reg) >> 4) & 0x1)

/** \brief 获取FD_120HZ_FLICKER_VALID */
#define __AS7341_FD_STATUS_FD_120HZ_FLICKER_VALID_GET(reg)  (((reg) >> 3) & 0x1)

/** \brief 获取FD_100HZ_FLICKER_VALID */
#define __AS7341_FD_STATUS_FD_100HZ_FLICKER_VALID_GET(reg)  (((reg) >> 2) & 0x1)

/** \brief 获取FD_120HZ_FLICKER */
#define __AS7341_FD_STATUS_FD_120HZ_FLICKER_GET(reg)        (((reg) >> 1) & 0x1)

/** \brief 获取FD_100HZ_FLICKER */
#define __AS7341_FD_STATUS_FD_100HZ_FLICKER_GET(reg)        (((reg) >> 0) & 0x1)

/** \brief 设置SP_MAN_AZ */
#define __AS7341_MASK_CONTROL_SP_MAN_AZ                     (0x1 << 2)
#define __AS7341_SP_MAN_AZ_SET(val)                         ((val) << 2)

/** \brief 设置FIFO_CLR */
#define __AS7341_MASK_CONTROL_FIFO_CLR                      (0x1 << 1)
#define __AS7341_FIFO_CLR(val)                              ((val) << 1)

/** \brief 设置CLEAR_SAI_ACT */
#define __AS7341_MASK_CONTROL_CLEAR_SAI_ACT                 (0x1 << 0)
#define __AS7341_CLEAR_SAI_ACT(val)                         ((val) << 0)

/** \brief 设置FIFO_WRITE_CH5_DATA */
#define __AS7341_MASK_FIFO_MAP_FIFO_WRITE_CH5_DATA          (0x1 << 6)
#define __AS7341_FIFO_WRITE_CH5_DATA_SET(val)               ((val) << 6)

/** \brief 设置FIFO_WRITE_CH4_DATA */
#define __AS7341_MASK_FIFO_MAP_FIFO_WRITE_CH4_DATA          (0x1 << 5)
#define __AS7341_FIFO_WRITE_CH4_DATA_SET(val)               ((val) << 5)

/** \brief 设置FIFO_WRITE_CH3_DATA */
#define __AS7341_MASK_FIFO_MAP_FIFO_WRITE_CH3_DATA          (0x1 << 4)
#define __AS7341_FIFO_WRITE_CH3_DATA_SET(val)               ((val) << 4)

/** \brief 设置FIFO_WRITE_CH2_DATA */
#define __AS7341_MASK_FIFO_MAP_FIFO_WRITE_CH2_DATA          (0x1 << 3)
#define __AS7341_FIFO_WRITE_CH2_DATA_SET(val)               ((val) << 3)

/** \brief 设置FIFO_WRITE_CH1_DATA */
#define __AS7341_MASK_FIFO_MAP_FIFO_WRITE_CH1_DATA          (0x1 << 2)
#define __AS7341_FIFO_WRITE_CH1_DATA_SET(val)               ((val) << 2)

/** \brief 设置FIFO_WRITE_CH0_DATA */
#define __AS7341_MASK_FIFO_MAP_FIFO_WRITE_CH0_DATA          (0x1 << 1)
#define __AS7341_FIFO_WRITE_CH0_DATA_SET(val)               ((val) << 1)

/** \brief 设置FIFO_WRITE_ASTATUS */
#define __AS7341_MASK_FIFO_MAP_FIFO_WRITE_ASTATUS           (0x1 << 0)
#define __AS7341_FIFO_WRITE_ASTATUS_SET(val)                ((val) << 0)

/** \brief 将两个uint8转换为一个uint16_t类型 */
#define __AS7341_UINT8_TO_UINT16(buff)                      ((uint16_t)((*(buff + 1)) << 8 | (*buff)))

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_as7341_funcs = {
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
 * \brief as7341 写数据
 */
am_local am_err_t __as7341_write (am_sensor_as7341_dev_t    *p_this,
                                  uint32_t                   subaddr,
                                  uint8_t                   *p_buf,
                                  uint32_t                   nbytes)
{
    return am_i2c_write(&p_this->i2c_dev,
                        subaddr,
                        p_buf,
                        nbytes);
}

/*
 * \brief as7341 读数据
 */
am_local am_err_t __as7341_read (am_sensor_as7341_dev_t *p_this,
                                 uint32_t                subaddr,
                                 uint8_t                *p_buf,
                                 uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev,
                       subaddr,
                       p_buf,
                       nbytes);
}

/*
 * \brief as7341 访问寄存器相关配置
 */
am_local am_err_t __as7341_access_reg(am_sensor_as7341_dev_t    *p_this,
                                      uint8_t                    reg_bank)
{
    uint8_t cfg0_reg = p_this->cfg0_regval;
    uint8_t cfg0_mask = __AS7341_MASK_CFG0_REG_BANK;
    uint8_t reg_bank_val = 0;

    am_err_t ret = AM_OK;

    reg_bank_val = __AS7341_REG_BANK_SET(reg_bank);
    cfg0_reg &= ~cfg0_mask;
    cfg0_reg |= (reg_bank_val & cfg0_mask);
    ret = __as7341_write(p_this, __AS7341_REG_CFG0, &cfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/*
 * \brief as7341 修改寄存器值
 */
am_local am_err_t __as7341_modify_reg_val(am_sensor_as7341_dev_t    *p_this,
                                          uint8_t                    reg,
                                          uint8_t                    mask,
                                          uint8_t                    val)
{
    uint8_t regval = 0;
    am_err_t ret = AM_OK;

    if ((reg >= 0x60 ) && (reg < __AS7341_REG_ENABLE)) {
        /* CFG0 的 REG_BANK 位写1 */
        ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X60_TO_0X74);
        if (ret != AM_OK) {
            return ret;
        }
    }

    if (mask == 0xFF) {
        regval = val;
    } else {
        ret = __as7341_read(p_this, reg, &regval, 1);
        if (ret != AM_OK) {
            return ret;
        }

        regval &= ~mask;
        regval |= (val & mask);
    }

    ret = __as7341_write(p_this, reg, &regval, 1);
    if (ret != AM_OK) {
        return ret;
    }

    if (reg == __AS7341_REG_CFG0) {
        /* 更新 CFG0 值 */
        p_this->cfg0_regval = regval;
    }

    if ((reg >= 0x60 ) && (reg < __AS7341_REG_ENABLE)) {
        /* CFG0 的 REG_BANK 位写0 */
        ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X80_AND_ABOVE);
        if (ret != AM_OK) {
            return ret;
        }
    }

    return ret;
}

/*
 * \brief as7341 获取寄存器值
 */
am_local am_err_t __as7341_regval_get(am_sensor_as7341_dev_t    *p_this,
                                      uint8_t                    reg,
                                      uint8_t                   *p_regval)
{
    am_err_t ret = AM_OK;

    if ((reg >= 0x60 ) && (reg < __AS7341_REG_ENABLE)) {
        /* CFG0 的 REG_BANK 位写1 */
        ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X60_TO_0X74);
        if (ret != AM_OK) {
            return ret;
        }
    }

    ret = __as7341_read(p_this, reg, p_regval, 1);
    if (ret != AM_OK) {
        return ret;
    }

    if ((reg >= 0x60 ) && (reg < __AS7341_REG_ENABLE)) {
        /* CFG0 的 REG_BANK 位写0 */
        ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X80_AND_ABOVE);
        if (ret != AM_OK) {
            return ret;
        }
    }

    return ret;
}

/** \brief 获取SYND模式的积分时间(ITIME) */
am_local am_err_t __as7341_itime_get(am_sensor_as7341_dev_t *p_this,
                                     uint32_t               *p_itime)
{
    am_err_t ret = AM_OK;

    uint8_t reg[3] = {0};

    /* CFG0 的 REG_BANK 位写1 */
    ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X60_TO_0X74);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_read(p_this, __AS7341_REG_ITIME_L, reg, 3);

    *p_itime = ((uint32_t)reg[2] << 16) |
               ((uint32_t)reg[1] << 8)  |
               ((uint32_t)reg[0] << 0);

    /* CFG0 的 REG_BANK 位写0 */
    ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X80_AND_ABOVE);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置阈值 */
am_local am_err_t __as7341_threshold_set(am_sensor_as7341_dev_t *p_this,
                                         uint32_t                subaddr,
                                         uint16_t                value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    reg[0] = value & 0xff;
    reg[1] = (value & 0xff00) >> 8;

    ret = __as7341_write(p_this, subaddr, reg, 2);

    return ret;
}

/** \brief 获取阈值 */
am_local am_err_t __as7341_threshold_get(am_sensor_as7341_dev_t *p_this,
                                         uint32_t                subaddr,
                                         uint16_t               *p_value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    ret = __as7341_read(p_this, subaddr, reg, 2);

    *p_value = __AS7341_UINT8_TO_UINT16(&reg[0]);

    return ret;
}

/** \brief STATUS寄存器类 位清除 */
am_local am_err_t __as7341_status_flag_clr(am_sensor_as7341_dev_t   *p_this,
                                           uint32_t                  subaddr,
                                           uint8_t                   flag)
{
    uint8_t regval = 0x00;

    am_err_t ret = AM_OK;

    ret = __as7341_read(p_this, subaddr, &regval, 1);
    if (ret != AM_OK) {
        return ret;
    }

    if (regval & flag) {
        regval = flag;
        ret = __as7341_write(p_this, subaddr, &regval, 1);
        if (ret != AM_OK) {
            return ret;
        }
    }

    return ret;
}

/** \brief 设置FD_TIME */
am_local am_err_t __as7341_fd_time_set(am_sensor_as7341_dev_t   *p_this,
                                       uint16_t                  fd_integration_time)
{
    uint8_t reg[2] = {0};

    am_err_t ret = AM_OK;

    reg[0] = fd_integration_time & 0xff;
    reg[1] = (fd_integration_time & 0xff00) >> 8;

    reg[1] = __AS7341_FD_TIME_MSB_SET(reg[1]);

    ret = __as7341_write(p_this, __AS7341_REG_FD_TIME1, &reg[0], 1);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_modify_reg_val(p_this, __AS7341_REG_FD_TIME2, __AS7341_MASK_FD_TIME2_FD_TIME_MSB, reg[1]);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/*******************************************************************************/

am_local am_err_t __as7341_ioctl (am_sensor_as7341_dev_t    *p_dev,
                                  int                        cmd,
                                  void                      *p_arg)
{
    uint8_t reg = 0x00;
    uint8_t regmask = 0xFF;
    uint8_t regval = 0x00;

    am_err_t ret = AM_OK;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    switch (cmd) {
    /* 获取ITIME */
    case AM_SENSOR_AS7341_ITIME_GET:
        ret = __as7341_itime_get(p_dev, (uint32_t *)p_arg);
        break;

    /* 设置LED_SEL */
    case AM_SENSOR_AS7341_LED_SEL:
        reg = __AS7341_REG_CONFIG;
        regmask = __AS7341_MASK_CONFIG_LED_SEL;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_LED_SEL_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置INT_SEL */
    case AM_SENSOR_AS7341_INT_SEL:
        reg = __AS7341_REG_CONFIG;
        regmask = __AS7341_MASK_CONFIG_INT_SEL;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_INT_SEL_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置INT_MODE */
    case AM_SENSOR_AS7341_INT_MODE_SET:
        reg = __AS7341_REG_CONFIG;
        regmask = __AS7341_MASK_CONFIG_INT_MODE;
        regval = (uint8_t)(int)p_arg;
        p_dev->int_mode = regval;       /* 记录INT_MODE值 */
        regval = __AS7341_INT_MODE_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取STAT寄存器 */
    case AM_SENSOR_AS7341_STAT_GET:
        ret = __as7341_regval_get(p_dev, __AS7341_REG_STAT, (uint8_t *)p_arg);
        break;

    /* 设置EDGE */
    case AM_SENSOR_AS7341_EDGE_SET:
        reg = __AS7341_REG_EDGE;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        regval = regval - 1;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PD_GPIO */
    case AM_SENSOR_AS7341_PD_GPIO_SET:
        reg = __AS7341_REG_GPIO;
        regmask = __AS7341_MASK_GPIO_PD_GPIO;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_PD_GPIO_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PD_INT */
    case AM_SENSOR_AS7341_PD_INT_SET:
        reg = __AS7341_REG_GPIO;
        regmask = __AS7341_MASK_GPIO_PD_INT;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_PD_INT_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置LED_ACT */
    case AM_SENSOR_AS7341_LED_ACT_SET:
        reg = __AS7341_REG_LED;
        regmask = __AS7341_MASK_LED_LED_ACT;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_LED_ACT_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置LED_DRIVE */
    case AM_SENSOR_AS7341_LED_DRIVE_SET:
        reg = __AS7341_REG_LED;
        regmask = __AS7341_MASK_LED_LED_DRIVE;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_LED_DRIVE_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置功能使能 */
    case AM_SENSOR_AS7341_FUNC_ENABLE:
        reg = __AS7341_REG_ENABLE;
        regmask = (uint8_t)(int)p_arg;
        regval = (uint8_t)(int)p_arg;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置功能禁能 */
    case AM_SENSOR_AS7341_FUNC_DISABLE:
        reg = __AS7341_REG_ENABLE;
        regmask = (uint8_t)(int)p_arg;
        regval = 0x00;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置ATIME */
    case AM_SENSOR_AS7341_ATIME_SET:
        reg = __AS7341_REG_ATIME;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置WTIME */
    case AM_SENSOR_AS7341_WTIME_SET:
        reg = __AS7341_REG_WTIME;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置光谱低阈值 */
    case AM_SENSOR_AS7341_LOW_SPECTRAL_THRESHOLD_SET:
        ret = __as7341_threshold_set(p_dev, __AS7341_REG_SP_TH_L_LSB, (uint16_t)(int)p_arg);
        break;

    /* 设置光谱高阈值 */
    case AM_SENSOR_AS7341_HIGH_SPECTRAL_THRESHOLD_SET:
        ret = __as7341_threshold_set(p_dev, __AS7341_REG_SP_TH_H_LSB, (uint16_t)(int)p_arg);
        break;

    /* 获取光谱低阈值 */
    case AM_SENSOR_AS7341_LOW_SPECTRAL_THRESHOLD_GET:
        ret = __as7341_threshold_get(p_dev, __AS7341_REG_SP_TH_L_LSB, (uint16_t *)p_arg);
        break;

    /* 获取光谱高阈值 */
    case AM_SENSOR_AS7341_HIGH_SPECTRAL_THRESHOLD_GET:
        ret = __as7341_threshold_get(p_dev, __AS7341_REG_SP_TH_H_LSB, (uint16_t *)p_arg);
        break;

    /* 读取AUXID */
    case AM_SENSOR_AS7341_AUXID_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_AUXID, (uint8_t *)p_arg, 1);
        if (ret != AM_OK) {
        #if __AS7341_DEBUG
            am_kprintf("\r\nSensor AS7341 read AUXID failed! \r\n");
        #endif
        }
        break;

    /* 读取REVID */
    case AM_SENSOR_AS7341_REVID_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_REVID, (uint8_t *)p_arg, 1);
        if (ret != AM_OK) {
        #if __AS7341_DEBUG
            am_kprintf("\r\nSensor AS7341 read REVID failed! \r\n");
        #endif
        }
        break;

    /* 读取ID */
    case AM_SENSOR_AS7341_ID_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_ID, (uint8_t *)p_arg, 1);
        if (ret != AM_OK) {
        #if __AS7341_DEBUG
            am_kprintf("\r\nSensor AS7341 read ID failed! \r\n");
        #endif
        }
        break;

    /* STATUS寄存器位清除 */
    case AM_SENSOR_AS7341_STATUS_FLAG_CLR:
        ret = __as7341_status_flag_clr(p_dev,
                                       __AS7341_REG_STATUS,
                                      (uint8_t)(int)p_arg);
        break;

    /* STATUS寄存器清除所有标志 */
    case AM_SENSOR_AS7341_STATUS_FLAG_ALL_CLR:
        reg = __AS7341_REG_STATUS;
        regmask = 0x8F;
        regval = 0x8F;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取STATUS寄存器 */
    case AM_SENSOR_AS7341_STATUS_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_STATUS, (uint8_t *)p_arg, 1);
        break;

    /* 获取ASTATUS寄存器 */
    case AM_SENSOR_AS7341_ASTATUS_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_ASTATUS, (uint8_t *)p_arg, 1);
        break;

    /* 获取STATUS2寄存器 */
    case AM_SENSOR_AS7341_STATUS2_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_STATUS2, (uint8_t *)p_arg, 1);
        break;

    /* 获取STATUS3寄存器 */
    case AM_SENSOR_AS7341_STATUS3_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_STATUS3, (uint8_t *)p_arg, 1);
        break;

    /* 获取STATUS5寄存器 */
    case AM_SENSOR_AS7341_STATUS5_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_STATUS5, (uint8_t *)p_arg, 1);
        break;

    /* 获取STATUS6寄存器 */
    case AM_SENSOR_AS7341_STATUS6_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_STATUS6, (uint8_t *)p_arg, 1);
        break;

    /* 设置LOWPOWER_IDLE */
    case AM_SENSOR_AS7341_LOWPOWER_IDLE_SET:
        reg = __AS7341_REG_CFG0;
        regmask = __AS7341_MASK_CFG0_LOWPOWER_IDLE;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_LOWPOWER_IDLE_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置WLONG(wait long) */
    case AM_SENSOR_AS7341_WLONG_SET:
        reg = __AS7341_REG_CFG0;
        regmask = __AS7341_MASK_CFG0_WLONG;
        regval = (am_bool_t)(int)p_arg;
        regval = __AS7341_WLONG_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置光谱增益 */
    case AM_SENSOR_AS7341_AGAIN_SET:
        reg = __AS7341_REG_CFG1;
        regmask = __AS7341_MASK_CFG1_AGAIN;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_AGAIN_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SAI */
    case AM_SENSOR_AS7341_SAI_SET:
        reg = __AS7341_REG_CFG3;
        regmask = __AS7341_MASK_CFG3_SAI;
        regval = (am_bool_t)(int)p_arg;
        regval = __AS7341_SAI_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SMUX_CMD */
    case AM_SENSOR_AS7341_SMUX_CMD_SET:
        reg = __AS7341_REG_CFG6;
        regmask = __AS7341_MASK_CFG6_SMUX_CMD;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_SMUX_CMD_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置FIFO_TH */
    case AM_SENSOR_AS7341_FIFO_TH_SET:
        reg = __AS7341_REG_CFG8;
        regmask = __AS7341_MASK_CFG8_FIFO_TH;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_FIFO_TH_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置FD_AGC */
    case AM_SENSOR_AS7341_FD_AGC_SET:
        reg = __AS7341_REG_CFG8;
        regmask = __AS7341_MASK_CFG8_FD_AGC;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_FD_AGC_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SP_AGC */
    case AM_SENSOR_AS7341_SP_AGC_SET:
        reg = __AS7341_REG_CFG8;
        regmask = __AS7341_MASK_CFG8_SP_AGC;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_SP_AGC_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SIEN_FD */
    case AM_SENSOR_AS7341_SIEN_FD_SET:
        reg = __AS7341_REG_CFG9;
        regmask = __AS7341_MASK_CFG9_SIEN_FD;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_SIEN_FD_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SIEN_SMUX */
    case AM_SENSOR_AS7341_SIEN_SMUX_SET:
        reg = __AS7341_REG_CFG9;
        regmask = __AS7341_MASK_CFG9_SIEN_SMUX;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_SIEN_SMUX_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置AGC_H */
    case AM_SENSOR_AS7341_AGC_H_SET:
        reg = __AS7341_REG_CFG10;
        regmask = __AS7341_MASK_CFG10_AGC_H;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_AGC_H_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置AGC_L */
    case AM_SENSOR_AS7341_AGC_L_SET:
        reg = __AS7341_REG_CFG10;
        regmask = __AS7341_MASK_CFG10_AGC_L;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_AGC_L_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置FD_PERS */
    case AM_SENSOR_AS7341_FD_PERS_SET:
        reg = __AS7341_REG_CFG10;
        regmask = __AS7341_MASK_CFG10_FD_PERS;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_FD_PERS_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SP_TH_CH */
    case AM_SENSOR_AS7341_SP_TH_CH_SET:
        reg = __AS7341_REG_CFG12;
        regmask = __AS7341_MASK_CFG12_SP_TH_CH;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_SP_TH_CH_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置APERS */
    case AM_SENSOR_AS7341_APERS_SET:
        reg = __AS7341_REG_PERS;
        regmask = __AS7341_MASK_PERS_APERS;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_APERS_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置GPIO_INV */
    case AM_SENSOR_AS7341_GPIO_INV_SET:
        reg = __AS7341_REG_GPIO2;
        regmask = __AS7341_MASK_GPIO2_GPIO_INV;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_GPIO_INV_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置GPIO_IN_EN */
    case AM_SENSOR_AS7341_GPIO_IN_EN_SET:
        reg = __AS7341_REG_GPIO2;
        regmask = __AS7341_MASK_GPIO2_GPIO_IN_EN;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_GPIO_IN_EN_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置GPIO_OUT */
    case AM_SENSOR_AS7341_GPIO_OUT_SET:
        reg = __AS7341_REG_GPIO2;
        regmask = __AS7341_MASK_GPIO2_GPIO_OUT;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_GPIO_OUT_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取GPIO_IN */
    case AM_SENSOR_AS7341_GPIO_IN_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_GPIO2, (uint8_t *)p_arg, 1);
        break;

    /* 设置ASTEP */
    case AM_SENSOR_AS7341_ASTEP_SET:
        ret = __as7341_threshold_set(p_dev, __AS7341_REG_ASTEPL, (uint16_t)(int)p_arg);
        break;

    /* 设置AGC_FD_GAIN_MAX */
    case AM_SENSOR_AS7341_AGC_FD_GAIN_MAX_SET:
        reg = __AS7341_REG_AGC_GAIN_MAX;
        regmask = __AS7341_MASK_AGC_GAIN_MAX_AGC_FD_GAIN_MAX;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_AGC_FD_GAIN_MAX_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置AGC_AGAIN_MAX */
    case AM_SENSOR_AS7341_AGC_AGAIN_MAX_SET:
        reg = __AS7341_REG_AGC_GAIN_MAX;
        regmask = __AS7341_MASK_AGC_GAIN_MAX_AGC_AGAIN_MAX;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_AGC_AGAIN_MAX_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置AZ_NTH_ITERATION */
    case AM_SENSOR_AS7341_AZ_NTH_ITERATION_SET:
        reg = __AS7341_REG_AZ_CONFIG;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置FD_TIME */
    case AM_SENSOR_AS7341_FD_TIME_SET:
        ret = __as7341_fd_time_set(p_dev, (uint16_t)(int)p_arg);
        break;

    /* 设置FD_GAIN */
    case AM_SENSOR_AS7341_FD_GAIN_SET:
        reg = __AS7341_REG_FD_TIME2;
        regmask = __AS7341_MASK_FD_TIME2_FD_GAIN;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_FD_GAIN_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置FIFO_WRITE_FD */
    case AM_SENSOR_AS7341_FIFO_WRITE_FD_SET:
        reg = __AS7341_REG_FIFO_CFG0;
        regmask = __AS7341_MASK_FIFO_CFG0_FIFO_WRITE_FD;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_FIFO_WRITE_FD_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 获取FD_STATUS寄存器 */
    case AM_SENSOR_AS7341_FD_STATUS_GET:
        ret = __as7341_read(p_dev, __AS7341_REG_FD_STATUS, (uint8_t *)p_arg, 1);
        break;

    /* FD_STATUS寄存器位清除 */
    case AM_SENSOR_AS7341_FD_STATUS_FLAG_CLR:
        ret = __as7341_status_flag_clr(p_dev,
                                       __AS7341_REG_FD_STATUS,
                                      (uint8_t)(int)p_arg);
        break;

    /* FD_STATUS寄存器清除所有标志 */
    case AM_SENSOR_AS7341_FD_STATUS_FLAG_ALL_CLR:
        reg = __AS7341_REG_FD_STATUS;
        regmask = 0x3C;
        regval = 0x3C;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 中断使能 */
    case AM_SENSOR_AS7341_INT_ENABLE:
        reg = __AS7341_REG_INTENAB;
        regmask = (uint8_t)(int)p_arg;
        regval = (uint8_t)(int)p_arg;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 中断禁能 */
    case AM_SENSOR_AS7341_INT_DISABLE:
        reg = __AS7341_REG_INTENAB;
        regmask = (uint8_t)(int)p_arg;
        regval = 0x00;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置SP_MAN_AZ */
    case AM_SENSOR_AS7341_SP_MAN_AZ_SET:
        reg = __AS7341_REG_CONTROL;
        regmask = __AS7341_MASK_CONTROL_SP_MAN_AZ;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_SP_MAN_AZ_SET(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置FIFO_CLR */
    case AM_SENSOR_AS7341_FIFO_CLR:
        reg = __AS7341_REG_CONTROL;
        regmask = __AS7341_MASK_CONTROL_FIFO_CLR;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_FIFO_CLR(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置CLEAR_SAI_ACT */
    case AM_SENSOR_AS7341_CLEAR_SAI_ACT:
        reg = __AS7341_REG_CONTROL;
        regmask = __AS7341_MASK_CONTROL_CLEAR_SAI_ACT;
        regval = (uint8_t)(int)p_arg;
        regval = __AS7341_CLEAR_SAI_ACT(regval);
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* FIFO_MAP使能 */
    case AM_SENSOR_AS7341_FIFO_MAP_ENABLE:
        reg = __AS7341_REG_FIFO_MAP;
        regmask = (uint8_t)(int)p_arg;
        regval = (uint8_t)(int)p_arg;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* FIFO_MAP禁能 */
    case AM_SENSOR_AS7341_FIFO_MAP_DISABLE:
        reg = __AS7341_REG_FIFO_MAP;
        regmask = (uint8_t)(int)p_arg;
        regval = 0x00;
        ret = __as7341_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/*******************************************************************************/

/** \brief AS7341 清除所有中断标志 */
am_local am_err_t __as7341_status_clr_all(am_sensor_as7341_dev_t    *p_this)
{
    am_err_t ret = AM_OK;

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_STATUS_FLAG_ALL_CLR, NULL);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FD_STATUS_FLAG_ALL_CLR, NULL);
    if (ret != AM_OK) {
        return ret;
    } 

    return ret;
}

/** \brief AS7341 参数初始化 */
am_local am_err_t __as7341_param_init(am_sensor_as7341_dev_t    *p_this)
{
    as7341_param_config_t *p_param_config = (as7341_param_config_t *)p_this->dev_info->p_param_default;

    am_err_t ret = AM_OK;

    /* CONFIG 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_LED_SEL, (void *)(uint32_t)(p_param_config->led_sel));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_INT_SEL, (void *)(uint32_t)(p_param_config->int_sel));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_INT_MODE_SET, (void *)(uint32_t)(p_param_config->int_mode));
    if (ret != AM_OK) {
        return ret;
    }

    /* GPIO 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_PD_GPIO_SET, (void *)(uint32_t)(p_param_config->pd_gpio));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_PD_INT_SET, (void *)(uint32_t)(p_param_config->pd_int));
    if (ret != AM_OK) {
        return ret;
    }

    /* LED 寄存器配置 */
    if (p_param_config->led_sel == AM_AS7341_LED_CONTROLLED) {
        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_LED_ACT_SET, (void *)(uint32_t)(p_param_config->led_act));
        if (ret != AM_OK) {
            return ret;
        }

        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_LED_DRIVE_SET, (void *)(uint32_t)(p_param_config->led_drive));
        if (ret != AM_OK) {
            return ret;
        }
    }

    /* CFG0 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_LOWPOWER_IDLE_SET, (void *)(uint32_t)(p_param_config->lowpower_idle));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_WLONG_SET, (void *)(uint32_t)(p_param_config->wlong_en));
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG1 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_AGAIN_SET, (void *)(uint32_t)(p_param_config->again_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG3 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_SAI_SET, (void *)(uint32_t)(p_param_config->sleep_after_interrupt));
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG8 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FIFO_TH_SET, (void *)(uint32_t)(p_param_config->fifo_th));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FD_AGC_SET, (void *)(uint32_t)(p_param_config->fd_agc));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_SP_AGC_SET, (void *)(uint32_t)(p_param_config->sp_agc));
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG9 寄存器配置 */

    /* CFG10 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_AGC_H_SET, (void *)(uint32_t)(p_param_config->agc_h));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_AGC_L_SET, (void *)(uint32_t)(p_param_config->agc_l));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FD_PERS_SET, (void *)(uint32_t)(p_param_config->fd_pers));
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG12 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_SP_TH_CH_SET, (void *)(uint32_t)(p_param_config->sp_th_ch));
    if (ret != AM_OK) {
        return ret;
    }

    /* PERS 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_APERS_SET, (void *)(uint32_t)(p_param_config->apers_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* AGC_GAIN_MAX 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_AGC_FD_GAIN_MAX_SET, (void *)(uint32_t)(p_param_config->agc_fd_gain_max));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_AGC_AGAIN_MAX_SET, (void *)(uint32_t)(p_param_config->agc_again_max));
    if (ret != AM_OK) {
        return ret;
    }

    /* AZ_CONFIG 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_AZ_NTH_ITERATION_SET, (void *)(uint32_t)(p_param_config->az_nth_iteration));
    if (ret != AM_OK) {
        return ret;
    }

    /* FD_TIME1 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FD_TIME_SET, (void *)(uint32_t)(p_param_config->fd_time));
    if (ret != AM_OK) {
        return ret;
    }

    /* FD_TIME2 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FD_GAIN_SET, (void *)(uint32_t)(p_param_config->fd_gain));
    if (ret != AM_OK) {
        return ret;
    }

    /* FIFO_CFG0 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FIFO_WRITE_FD_SET, (void *)(uint32_t)(p_param_config->fifo_write_fd));
    if (ret != AM_OK) {
        return ret;
    }

    /* CONTROL 寄存器配置 */

    /* FIFO_MAP 寄存器配置 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FIFO_MAP_ENABLE, (void *)(uint32_t)(p_param_config->fifo_map_en));
    if (ret != AM_OK) {
        return ret;
    }

    /* GPIO2 寄存器配置 */
    if (p_param_config->int_mode == AM_AS7341_INT_MODE_SPM) {

        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_GPIO_INV_SET, (void *)(uint32_t)(p_param_config->gpio_inv));
        if (ret != AM_OK) {
            return ret;
        }

        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_GPIO_IN_EN_SET, (void *)(uint32_t)(p_param_config->gpio_in_en));
        if (ret != AM_OK) {
            return ret;
        }

        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_GPIO_OUT_SET, (void *)(uint32_t)(p_param_config->gpio_out));
        if (ret != AM_OK) {
            return ret;
        }

    } else {

        /* 将 GPIO 配置为输入 */
        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_GPIO_INV_SET, (void *)(uint32_t)(0));
        if (ret != AM_OK) {
            return ret;
        }

        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_GPIO_IN_EN_SET, (void *)(uint32_t)(1));
        if (ret != AM_OK) {
            return ret;
        }

        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_GPIO_OUT_SET, (void *)(uint32_t)(0));
        if (ret != AM_OK) {
            return ret;
        }

    }

    if (p_param_config->int_mode == AM_AS7341_INT_MODE_SYND) {

        /* EDGE 寄存器配置 */
        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_EDGE_SET, (void *)(uint32_t)(p_param_config->edge));
        if (ret != AM_OK) {
            return ret;
        }

    } else {

        /* ASTEP 寄存器配置 */
        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_ASTEP_SET, (void *)(uint32_t)(p_param_config->astep));
        if (ret != AM_OK) {
            return ret;
        }

        /* ATIME 寄存器配置 */
        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_ATIME_SET, (void *)(uint32_t)(p_param_config->atime_val));
        if (ret != AM_OK) {
            return ret;
        }

        /* WTIME 寄存器配置 */
        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_WTIME_SET, (void *)(uint32_t)(p_param_config->wtime_val));
        if (ret != AM_OK) {
            return ret;
        }

    }

    return ret;
}

/** \brief AS7341 SMUX 配置 */
am_local am_err_t __as7341_smux_cfg(am_sensor_as7341_dev_t  *p_this,
                                    uint8_t                  bankmode)
{
    am_err_t ret = AM_OK;

    uint8_t regvalbuf[20] = {0};

    if(bankmode == 0) {
    
        /* smux configuration for first channels */
        regvalbuf[0] = 0x30;
        regvalbuf[1] = 0x01;
        regvalbuf[2] = 0x00;
        regvalbuf[3] = 0x00;
        regvalbuf[4] = 0x06;
        regvalbuf[5] = 0x42;
        regvalbuf[6] = 0x50;
        regvalbuf[7] = 0x00;
        regvalbuf[8] = 0x00;
        regvalbuf[9] = 0x50;
        regvalbuf[10] = 0x00;
        regvalbuf[11] = 0x00;
        regvalbuf[12] = 0x20;
        regvalbuf[13] = 0x04;
        regvalbuf[14] = 0x60;
        regvalbuf[15] = 0x30;
        regvalbuf[16] = 0x01;
        regvalbuf[17] = 0x00;
        regvalbuf[18] = 0x00;
        regvalbuf[19] = 0x00;
    }
    else if (bankmode == 1) {
       
        /* smux configuration for rest of channels */
        regvalbuf[0]  = 0x00;
        regvalbuf[1]  = 0x00;
        regvalbuf[2]  = 0x00;
        regvalbuf[3]  = 0x20;
        regvalbuf[4]  = 0x00;
        regvalbuf[5]  = 0x00;
        regvalbuf[6]  = 0x00;
        regvalbuf[7]  = 0x01;
        regvalbuf[8]  = 0x30;
        regvalbuf[9]  = 0x00;
        regvalbuf[10] = 0x01;
        regvalbuf[11] = 0x00;
        regvalbuf[12] = 0x00;
        regvalbuf[13] = 0x00;
        regvalbuf[14] = 0x02;
        regvalbuf[15] = 0x00;
        regvalbuf[16] = 0x00;
        regvalbuf[17] = 0x30;
        regvalbuf[18] = 0x40;
        regvalbuf[19] = 0x65;
    }

    /* write smux config (20 values) to ram address 0x00 */
    ret = __as7341_write(p_this, 0x00, regvalbuf, 20);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG6 寄存器配置 write smux conf from ram to smux chain */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_SMUX_CMD_SET, (void *)(uint32_t)(AM_AS7341_SMUX_CMD_2));
    if (ret != AM_OK) {
        return ret;
    }

    /* power on, start smux command */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FUNC_ENABLE,
                                (void *)(AM_AS7341_SMUXEN  | \
                                         AM_AS7341_PON ));
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief AS7341 启动转换 */
am_local am_err_t __as7341_start_conversion(am_sensor_as7341_dev_t  *p_this,
                                            uint8_t                  normal,
                                            uint8_t                  flicker)
{
    am_err_t ret = AM_OK;

    uint8_t enable_reg = 0x00;

    enable_reg |= AM_AS7341_PON;
    if (normal) {
        enable_reg |= (AM_AS7341_WEN | AM_AS7341_SP_EN);
    }

    if (flicker) {
        enable_reg |= AM_AS7341_FDEN;
    }

    /* enable 寄存器配置使能 */
    ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FUNC_ENABLE,
                                (void *)(uint32_t)enable_reg);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}


/**
 * \brief AS7341 中断使能
 */
am_local void __as7341_int_enable(am_sensor_as7341_dev_t    *p_this)
{
    __as7341_ioctl(p_this, AM_SENSOR_AS7341_INT_ENABLE,
                                (void *)(AM_AS7341_SP_IEN |
                                         AM_AS7341_SIEN));

    __as7341_ioctl(p_this, AM_SENSOR_AS7341_SIEN_FD_SET, 
                            (void *) AM_AS7341_SIEN_FD_ENABLE);
}

/**
 * \brief AS7341 中断禁能
 */
am_local void __as7341_int_disable(am_sensor_as7341_dev_t   *p_this)
{
    __as7341_ioctl(p_this, AM_SENSOR_AS7341_INT_DISABLE,
                                (void *)(AM_AS7341_SP_IEN |
                                         AM_AS7341_SIEN));

    __as7341_ioctl(p_this, AM_SENSOR_AS7341_SIEN_FD_SET, 
                            (void *) AM_AS7341_SIEN_FD_DISABLE);
}

#if __AS7341_DEBUG

struct as7341_debug_info {
    const char  **reg_name_str;
    uint8_t     *reg;
};

const char *__as7341_reg_name_string[] = {
"ASTATUS     ",
"CH0_DATA_L  ",
"CH0_DATA_H  ",
"ITIME_L     ",
"ITIME_M     ",
"ITIME_H     ",
"CH1_DATA_L  ",
"CH1_DATA_H  ",
"CH2_DATA_L  ",
"CH2_DATA_H  ",
"CH3_DATA_L  ",
"CH3_DATA_H  ",
"CH4_DATA_L  ",
"CH4_DATA_H  ",
"CH5_DATA_L  ",
"CH5_DATA_H  ",
"CONFIG      ",
"STAT        ",
"EDGE        ",
"GPIO        ",
"LED         ",
"ENABLE      ",
"ATIME       ",
"WTIME       ",
"SP_TH_L_LSB ",
"SP_TH_L_MSB ",
"SP_TH_H_LSB ",
"SP_TH_H_MSB ",
"AUXID       ",
"REVID       ",
"ID          ",
"STATUS      ",
"ASTATUS     ",
"CH0_DATA_L  ",
"CH0_DATA_H  ",
"CH1_DATA_L  ",
"CH1_DATA_H  ",
"CH2_DATA_L  ",
"CH2_DATA_H  ",
"CH3_DATA_L  ",
"CH3_DATA_H  ",
"CH4_DATA_L  ",
"CH4_DATA_H  ",
"CH5_DATA_L  ",
"CH5_DATA_H  ",
"STATUS2     ",
"STATUS3     ",
"STATUS5     ",
"STATUS6     ",
"CFG0        ",
"CFG1        ",
"CFG3        ",
"CFG6        ",
"CFG8        ",
"CFG9        ",
"CFG10       ",
"CFG12       ",
"PERS        ",
"GPIO2       ",
"ASTEPL      ",
"ASTEPH      ",
"AGC_GAIN_MAX",
"AZ_CONFIG   ",
"FD_TIME1    ",
"FD_TIME2    ",
"FIFO_CFG0   ",
"FD_STATUS   ",
"INTENAB     ",
"CONTROL     ",
"FIFO_MAP    ",
"FIFO_LVL    ",
"FDATAL      ",
"FDATAH      ",

};

static uint8_t __as7341_reg_name[] = {
0x60,
0x61,
0x62,
__AS7341_REG_ITIME_L,
__AS7341_REG_ITIME_M,
__AS7341_REG_ITIME_H,
0x66,
0x67,
0x68,
0x69,
0x6A,
0x6B,
0x6C,
0x6D,
0x6E,
0x6F,
__AS7341_REG_CONFIG,
__AS7341_REG_STAT,
__AS7341_REG_EDGE,
__AS7341_REG_GPIO,
__AS7341_REG_LED,
__AS7341_REG_ENABLE,
__AS7341_REG_ATIME,
__AS7341_REG_WTIME,
__AS7341_REG_SP_TH_L_LSB,
__AS7341_REG_SP_TH_L_MSB,
__AS7341_REG_SP_TH_H_LSB,
__AS7341_REG_SP_TH_H_MSB,
__AS7341_REG_AUXID,
__AS7341_REG_REVID,
__AS7341_REG_ID,
__AS7341_REG_STATUS,
__AS7341_REG_ASTATUS,
__AS7341_REG_CH0_DATA_L,
__AS7341_REG_CH0_DATA_H,
__AS7341_REG_CH1_DATA_L,
__AS7341_REG_CH1_DATA_H,
__AS7341_REG_CH2_DATA_L,
__AS7341_REG_CH2_DATA_H,
__AS7341_REG_CH3_DATA_L,
__AS7341_REG_CH3_DATA_H,
__AS7341_REG_CH4_DATA_L,
__AS7341_REG_CH4_DATA_H,
__AS7341_REG_CH5_DATA_L,
__AS7341_REG_CH5_DATA_H,
__AS7341_REG_STATUS2,
__AS7341_REG_STATUS3,
__AS7341_REG_STATUS5,
__AS7341_REG_STATUS6,
__AS7341_REG_CFG0,
__AS7341_REG_CFG1,
__AS7341_REG_CFG3,
__AS7341_REG_CFG6,
__AS7341_REG_CFG8,
__AS7341_REG_CFG9,
__AS7341_REG_CFG10,
__AS7341_REG_CFG12,
__AS7341_REG_PERS,
__AS7341_REG_GPIO2,
__AS7341_REG_ASTEPL,
__AS7341_REG_ASTEPH,
__AS7341_REG_AGC_GAIN_MAX,
__AS7341_REG_AZ_CONFIG,
__AS7341_REG_FD_TIME1,
__AS7341_REG_FD_TIME2,
__AS7341_REG_FIFO_CFG0,
__AS7341_REG_FD_STATUS,
__AS7341_REG_INTENAB,
__AS7341_REG_CONTROL,
__AS7341_REG_FIFO_MAP,
__AS7341_REG_FIFO_LVL,
__AS7341_REG_FDATAL,
__AS7341_REG_FDATAH,

};

struct as7341_debug_info __g_as7341_debug_info = {
    __as7341_reg_name_string,
    __as7341_reg_name,
};

/** \brief AS7341 调试输出所有寄存器值 */
am_local void __as7341_regval_check(am_sensor_as7341_dev_t *p_this)
{
    uint8_t reg_buf[0xFF - 0x60 + 1]    = {0};         /* 缓存寄存器值 */
    uint8_t counter = 0;
    uint8_t reg_num = 0;

    __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X60_TO_0X74);
    __as7341_read(p_this, 0x60, &reg_buf[0], (0x80 - 0x60));
    __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X80_AND_ABOVE);

    __as7341_read(p_this, 0x80, &reg_buf[32], (0xFF - 0x80 + 1));

    reg_num = AM_NELEMENTS(__as7341_reg_name);

    for (counter = 0; counter < reg_num; counter++) {
        am_kprintf("%s : 0x%02x \r\n",
                    __g_as7341_debug_info.reg_name_str[counter], 
                    reg_buf[__g_as7341_debug_info.reg[counter] - 0x60]);
    }
}
#endif

am_local am_err_t __as7341_get_adc_data(am_sensor_as7341_dev_t  *p_this,
                                        uint8_t                 *p_result_buf)
{
    am_err_t ret = AM_OK;

    uint8_t status2_reg = 0;

    do {

        __as7341_ioctl(p_this, AM_SENSOR_AS7341_STATUS2_GET, &status2_reg);

    } while (__AS7341_STATUS2_AVALID_GET(status2_reg) != 0x1);

    /* 光谱数据转换完成 */

    /* CFG0 的 REG_BANK 位写1 */
    ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X60_TO_0X74);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __as7341_read(p_this, 0x60, p_result_buf, 16);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG0 的 REG_BANK 位写0 */
    ret = __as7341_access_reg(p_this, AM_AS7341_ACCESS_REG_0X80_AND_ABOVE);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/**
 * \brief AS7341 获取结果
 */
am_local am_err_t __as7341_result_get(am_sensor_as7341_dev_t    *p_this,
                                      as7341_result_info_t      *p_result)
{
    am_err_t ret = AM_OK;

    uint8_t result_buf[16]  = {0};              /* 缓存寄存器值 */

    /* f1 ~ f6 */
    __as7341_smux_cfg(p_this, 0);
    __as7341_start_conversion(p_this, 1, 0);    /* 启动转换 */

    ret = __as7341_get_adc_data(p_this, result_buf);
    if (ret != AM_OK) {
        return ret;
    }

    p_result->synd_itime = ((uint32_t)result_buf[5] << 16) |
                           ((uint32_t)result_buf[4] << 8)  |
                           ((uint32_t)result_buf[3] << 0);

    /* CHx AD值 */
    p_result->raw_data[0] = __AS7341_UINT8_TO_UINT16(&result_buf[1]);
    p_result->raw_data[1] = __AS7341_UINT8_TO_UINT16(&result_buf[6]);
    p_result->raw_data[2] = __AS7341_UINT8_TO_UINT16(&result_buf[8]);
    p_result->raw_data[3] = __AS7341_UINT8_TO_UINT16(&result_buf[10]);
    p_result->raw_data[4] = __AS7341_UINT8_TO_UINT16(&result_buf[12]);
    p_result->raw_data[5] = __AS7341_UINT8_TO_UINT16(&result_buf[14]);

    /* 获取数据后，清中断标志位 */
    __as7341_status_clr_all(p_this);

    /* f7, f8, clear, dark, prox, flicker */
    __as7341_smux_cfg(p_this, 1);
    __as7341_start_conversion(p_this, 1, 0);  /* 启动转换 */

    ret = __as7341_get_adc_data(p_this, result_buf);
    if (ret != AM_OK) {
        return ret;
    }

    p_result->synd_itime = ((uint32_t)result_buf[5] << 16) |
                           ((uint32_t)result_buf[4] << 8)  |
                           ((uint32_t)result_buf[3] << 0);

    /* CHx AD值 */
    p_result->raw_data[6] = __AS7341_UINT8_TO_UINT16(&result_buf[1]);
    p_result->raw_data[7] = __AS7341_UINT8_TO_UINT16(&result_buf[6]);
    p_result->raw_data[8] = __AS7341_UINT8_TO_UINT16(&result_buf[8]);
    p_result->raw_data[9] = __AS7341_UINT8_TO_UINT16(&result_buf[10]);
    p_result->raw_data[10] = __AS7341_UINT8_TO_UINT16(&result_buf[12]);
    p_result->raw_data[11] = __AS7341_UINT8_TO_UINT16(&result_buf[14]);

    /* 由于出现SMUX切换通道异常，此处重新配置SMUX为0 */
    __as7341_smux_cfg(p_this, 0);

    return ret;
}

/*******************************************************************************/

/** \breif 中断回调函数 */
am_local void __as7341_alarm_callback (void *p_arg)
{
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_arg;

    uint8_t i = 0;

    as7341_result_info_t result;

    /* 获取数据 */
    __as7341_result_get(p_this, &result);

    /* 获取数据后，清中断标志位 */
    __as7341_status_clr_all(p_this);

    /* 获取CHx */
    for (i = 0; i < 12; i++) { 
        p_this->data[i].val = result.raw_data[i];
        p_this->data[i].unit = AM_SENSOR_UNIT_BASE;
    }

    for (i = 0; i < 1; i++) {
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

    if (id == AM_AS7341_CHAN_1) {
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
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    as7341_result_info_t result;

    int cur_id = 0;
    int i = 0;

    uint8_t j;

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

            if (cur_id == 0) {

                for (j = 0; j < 12; j++) {
                    p_buf[j].val = p_this->data[cur_id + j].val;
                    p_buf[j].unit= p_this->data[cur_id + j].unit;
                }

            } else {
                return -AM_ENODEV;
            }
            return AM_OK;
        }
    }

    /** \brief 读取结果数据 */
    ret = __as7341_result_get(p_this, &result);
    if (ret != AM_OK) {
        return ret;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            for (j = 0; j < 12; j++) {
                p_buf[j].val = result.raw_data[j];
                p_buf[j].unit= AM_SENSOR_UNIT_BASE;
            }

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
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_drv;
    

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

    /* 寄存器参数初始化 */
    ret = __as7341_param_init(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器操作在获取数据时使能 */

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
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_drv;

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

        /* enable 寄存器配置禁能 */
        ret = __as7341_ioctl(p_this, AM_SENSOR_AS7341_FUNC_DISABLE,
                                    (void *)(AM_AS7341_FDEN  | \
                                             AM_AS7341_WEN   | \
                                             AM_AS7341_SP_EN | \
                                             AM_AS7341_PON ));
        if (ret != AM_OK){
            cur_ret = ret;
        }

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
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_drv;

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
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_drv;

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
    __as7341_status_clr_all(p_this);

    /* 使能中断 */
    __as7341_int_enable(p_this);

    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->trigger, 0);
        AM_BIT_SET(p_this->trigger, 1);
    }

    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_as7341_dev_t* p_this = (am_sensor_as7341_dev_t*)p_drv;

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
            __as7341_int_disable(p_this);

            AM_BIT_CLR(p_this->trigger, 1);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 AS7341 初始化
 */
am_sensor_handle_t am_sensor_as7341_init (
        am_sensor_as7341_dev_t              *p_dev,
        const am_sensor_as7341_devinfo_t    *p_devinfo,
        am_i2c_handle_t                      handle)
{
    am_err_t ret = AM_OK;

    uint8_t as7341_id = 0;

    uint8_t i = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->as7341_dev.p_drv   = p_dev;
    p_dev->as7341_dev.p_funcs = &__g_sensor_as7341_funcs;
    p_dev->dev_info           = p_devinfo;
    p_dev->pfn_trigger_cb[0]  = NULL;
    p_dev->p_arg[0]           = NULL;
    p_dev->flags[0]           = 0;
    p_dev->trigger            = 0;
    for (i = 0; i < 12; i++) {
        p_dev->data[i].val    = 0;
        p_dev->data[i].unit   = AM_SENSOR_UNIT_INVALID;
    }

    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __as7341_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);  /* 下降沿触发 */
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    ret = __as7341_read(p_dev, __AS7341_REG_ID, &as7341_id, 1);
    as7341_id &= __AS7341_MASK_ID;
    if ((ret != AM_OK) || (as7341_id != __AS7341_ID)) {
    #if __AS7341_DEBUG
        am_kprintf("as7341_id = 0x%x \r\n", as7341_id);
    #endif
        am_kprintf("\r\nSensor AS7341 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->as7341_dev);
}

/**
 * \brief 传感器 AS7341 去初始化
 */
am_err_t am_sensor_as7341_deinit (am_sensor_handle_t handle)
{
    am_sensor_as7341_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->as7341_dev.p_drv   = NULL;
    p_dev->as7341_dev.p_funcs = NULL;
    p_dev->dev_info           = NULL;

    return AM_OK;
}

/**
 * \brief 传感器 AS7341 控制函数
 */
am_err_t am_sensor_as7341_ioctl (am_sensor_handle_t  handle,
                                 int                 cmd,
                                 void               *p_arg)
{
    am_sensor_as7341_dev_t   *p_dev = (am_sensor_as7341_dev_t *)handle->p_drv;

    am_err_t ret = AM_OK;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    ret = __as7341_ioctl(p_dev, cmd, p_arg);

    return ret;
}

/* end of file */

