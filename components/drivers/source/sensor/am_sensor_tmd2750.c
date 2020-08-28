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
 * \brief 传感器 TMD2750 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_tmd2750.h"
#include "am_sensor.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**< 调试信息输出 */
#define __TMD2750_DEBUG             (0)

/**********************************************************************************************
 * 宏定义
 **********************************************************************************************/
#define __TMD2750_REG_ENABLE        (0x80)  /**< \brief 使能状态                              */
#define __TMD2750_REG_ATIME         (0x81)  /**< \brief 控制ALS ADC的积分时间                 */
#define __TMD2750_REG_PRATE         (0x82)  /**< \brief Prox采样持续时间:(PRATE + 1) * 88us   */
#define __TMD2750_REG_WTIME         (0x83)  /**< \brief ALS与Prox之间的等待时间               */
#define __TMD2750_REG_AILTL         (0x84)  /**< \brief ALS中断低阈值低字节                   */
#define __TMD2750_REG_AILTH         (0x85)  /**< \brief ALS中断低阈值高字节                   */
#define __TMD2750_REG_AIHTL         (0x86)  /**< \brief ALS中断高阈值低字节                   */
#define __TMD2750_REG_AIHTH         (0x87)  /**< \brief ALS中断高阈值高字节                   */
#define __TMD2750_REG_PILTL         (0x88)  /**< \brief Prox中断低阈值低字节                  */
#define __TMD2750_REG_PILTH         (0x89)  /**< \brief Prox中断低阈值高字节                  */
#define __TMD2750_REG_PIHTL         (0x8A)  /**< \brief Prox中断高阈值低字节                  */
#define __TMD2750_REG_PIHTH         (0x8B)  /**< \brief Prox中断高阈值高字节                  */
#define __TMD2750_REG_PERS          (0x8C)  /**< \brief ALS and prox中断过滤器                */
#define __TMD2750_REG_CFG0          (0x8D)  /**< \brief 配置寄存器0                           */
#define __TMD2750_REG_PCFG0         (0x8E)  /**< \brief Prox配置寄存器0                       */
#define __TMD2750_REG_PCFG1         (0x8F)  /**< \brief Prox配置寄存器1                       */
#define __TMD2750_REG_CFG1          (0x90)  /**< \brief 配置寄存器1                           */
#define __TMD2750_REG_REVID         (0x91)  /**< \brief 修订ID                                */
#define __TMD2750_REG_ID            (0x92)  /**< \brief 设备ID                                */
#define __TMD2750_REG_STATUS        (0x93)  /**< \brief 设备状态                              */
#define __TMD2750_REG_ALSL          (0x94)  /**< \brief ALS 通道ADC数据低字节                 */
#define __TMD2750_REG_ALSH          (0x95)  /**< \brief ALS 通道ADC数据高字节                 */
#define __TMD2750_REG_IRL           (0x96)  /**< \brief IR 通道ADC数据低字节                  */
#define __TMD2750_REG_IRH           (0x97)  /**< \brief IR 通道ADC数据高字节                  */
#define __TMD2750_REG_PDATAL        (0x9C)  /**< \brief Prox ADC数据低字节                    */
#define __TMD2750_REG_PDATAH        (0x9D)  /**< \brief Prox ADC数据高字节                    */
#define __TMD2750_REG_REVID2        (0x9E)  /**< \brief 修订ID 2                              */
#define __TMD2750_REG_CFG2          (0x9F)  /**< \brief 配置寄存器2                           */
#define __TMD2750_REG_SOFTRST       (0xA0)  /**< \brief 软复位                                */
#define __TMD2750_REG_CFG3          (0xAB)  /**< \brief 配置寄存器3                           */
#define __TMD2750_REG_CFG4          (0xAC)  /**< \brief 配置寄存器4 (must be set to 0x3D)     */
#define __TMD2750_REG_CFG6          (0xAE)  /**< \brief 配置寄存器6                           */
#define __TMD2750_REG_POFFSETL      (0xC0)  /**< \brief Prox偏移低字节数据                    */
#define __TMD2750_REG_POFFSETH      (0xC1)  /**< \brief Prox偏移高字节数据                    */
#define __TMD2750_REG_CALIB         (0xD7)  /**< \brief Prox偏移校准                          */
#define __TMD2750_REG_CALIBCFG      (0xD9)  /**< \brief Prox偏移校准控制                      */
#define __TMD2750_REG_CALIBSTAT     (0xDC)  /**< \brief Prox偏移校准状态                      */
#define __TMD2750_REG_INTENAB       (0xDD)  /**< \brief 中断使能                              */
#define __TMD2750_REG_FAC_L         (0xE6)  /**< \brief 出厂设置低数据 (lot code data)        */
#define __TMD2750_REG_FAC_H         (0xE7)  /**< \brief 出厂设置高数据 (lot code data)        */
#define __TMD2750_REG_TEST3         (0xF2)  /**< \brief 测试3 (must be set to 0xC4)           */

/** \brief 设置PPERS */
#define __TMD2750_MASK_PERS_PPERS                   (0xf << 4)
#define __TMD2750_PPERS_SET(val)                    ((val) << 4)

/** \brief 设置APERS */
#define __TMD2750_MASK_PERS_APERS                   (0xf << 0)
#define __TMD2750_APERS_SET(val)                    ((val) << 0)

/** \brief 设置WLONG(wait long) */
#define __TMD2750_MASK_CFG0_WLONG                   (0x1 << 2)
#define __TMD2750_WLONG_SET(val)                    ((val) << 2)

/** \brief 设置PPULSE_LEN_16X */
#define __TMD2750_MASK_CFG0_PPULSE_LEN_16X          (0x1 << 0)
#define __TMD2750_PPULSE_LEN_16X_SET(val)           ((val) << 0)

/** \brief 设置PPULSE_LEN */
#define __TMD2750_MASK_PCFG0_PPULSE_LEN             (0x3 << 6)
#define __TMD2750_PPULSE_LEN_SET(val)               ((val) << 6)

/** \brief 设置PPULSE */
#define __TMD2750_MASK_PCFG0_PPULSE                 (0x3f << 0)
#define __TMD2750_PPULSE_SET(val)                   ((val) << 0)

/** \brief 设置PGAIN */
#define __TMD2750_MASK_PCFG1_PGAIN                  (0x3 << 6)
#define __TMD2750_PGAIN_SET(val)                    ((val) << 6)

/** \brief 设置PLDRIVE */
#define __TMD2750_MASK_PCFG1_PLDRIVE                (0xf << 0)
#define __TMD2750_PLDRIVE_SET(val)                  ((val) << 0)

/** \brief 设置AGAIN */
#define __TMD2750_MASK_CFG1_AGAIN                   (0x1f << 0)
#define __TMD2750_AGAIN_SET(val)                    ((val) << 0)

/** \brief TMD2750 ID */
#define __TMD2750_ID                                (0x10)

/** \brief 获取PINT状态 */
#define __TMD2750_PINT_STATUS(reg)                  (((reg) >> 5) & 0x1)

/** \brief 获取AINT状态 */
#define __TMD2750_AINT_STATUS(reg)                  (((reg) >> 4) & 0x1)

/** \brief 获取CINT状态 */
#define __TMD2750_CINT_STATUS(reg)                  (((reg) >> 3) & 0x1)

/** \brief TMD2750软件复位 */
#define __TMD2750_SW_RST                            (0x1 << 0)

/** \brief 设置SAI */
#define __TMD2750_MASK_CFG3_SAI                     (0x1 << 4)
#define __TMD2750_SAI_SET(val)                      ((val) << 4)

/** \brief 设置APC */
#define __TMD2750_MASK_CFG6_APC                     (0x1 << 6)
#define __TMD2750_APC_SET(val)                      ((val) << 6)

/** \brief 选择Prox校准类型 */
#define __TMD2750_MASK_CALIB_ELECTRICAL_CALIBRATION (0x1 << 5)
#define __TMD2750_PROX_CAL_TYPE_SEL(val)            ((val) << 5)

/** \brief 开启偏移校准设置 */
#define __TMD2750_MASK_CALIB_START_OFFSET_CAL       (0x1 << 0)
#define __TMD2750_START_OFFSET_CAL(val)             ((val) << 0)

/** \brief 设置BINSRCH_TARGET */
#define __TMD2750_MASK_CALIBCFG_BINSRCH_TARGET      (0x7 << 5)
#define __TMD2750_BINSRCH_TARGET_SET(val)           ((val) << 5)

/** \brief 设置AUTO_OFFSET_ADJ */
#define __TMD2750_MASK_CALIBCFG_AUTO_OFFSET_ADJ     (0x1 << 3)
#define __TMD2750_AUTO_OFFSET_ADJ_SET(val)          ((val) << 3)

/** \brief 设置PROX_AVG */
#define __TMD2750_MASK_CALIBCFG_PROX_AVG            (0x7 << 0)
#define __TMD2750_PROX_AVG_SET(val)                 ((val) << 0)

/** \brief 获取SOFTRST位 */
#define __TMD2750_SOFTRST_GET(reg)                  (((reg) >> 0) & 0x1)

/** \brief 获取CALIB_FINISHED位 */
#define __TMD2750_CALIB_FINISHED_GET(reg)           (((reg) >> 0) & 0x1)

/** \brief 将两个uint8转换为一个uint16_t类型 */
#define __TMD2750_UINT8_TO_UINT16(buff)             ((uint16_t)((*(buff + 1)) << 8 | (*buff)))

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_tmd2750_funcs = {
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
 * \brief tmd2750 写数据
 */
am_local am_err_t __tmd2750_write (am_sensor_tmd2750_dev_t  *p_this,
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
 * \brief tmd2750 读数据
 */
am_local am_err_t __tmd2750_read (am_sensor_tmd2750_dev_t   *p_this,
                                 uint32_t                    subaddr,
                                 uint8_t                    *p_buf,
                                 uint32_t                    nbytes)
{
    return am_i2c_read(&p_this->i2c_dev,
                       subaddr,
                       p_buf,
                       nbytes);
}

/*
 * \brief tmd2750 修改寄存器值
 */
am_local am_err_t __tmd2750_modify_reg_val(am_sensor_tmd2750_dev_t  *p_this,
                                           uint8_t                   reg,
                                           uint8_t                   mask,
                                           uint8_t                   val)
{
    uint8_t regval = 0;
    am_err_t ret = AM_OK;

    if (mask == 0xFF) {
        regval = val;
    } else {
        ret = __tmd2750_read(p_this, reg, &regval, 1);
        if (ret != AM_OK) {
            return ret;
        }

        regval &= ~mask;
        regval |= (val & mask);
    }

    ret = __tmd2750_write(p_this, reg, &regval, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置阈值 */
am_local am_err_t __tmd2750_threshold_set(am_sensor_tmd2750_dev_t   *p_this,
                                          uint32_t                   subaddr,
                                          uint16_t                   value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    reg[0] = value & 0xff;
    reg[1] = (value & 0xff00) >> 8;

    ret = __tmd2750_write(p_this, subaddr, reg, 2);

    return ret;
}

/** \brief 获取阈值 */
am_local am_err_t __tmd2750_threshold_get(am_sensor_tmd2750_dev_t   *p_this,
                                          uint32_t                   subaddr,
                                          uint16_t                  *p_value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    ret = __tmd2750_read(p_this, subaddr, reg, 2);

    *p_value = __TMD2750_UINT8_TO_UINT16(&reg[0]);

    return ret;
}

/*******************************************************************************/

/** \brief STATUS寄存器位清除 */
am_local am_err_t __tmd2750_status_flag_clr(am_sensor_tmd2750_dev_t *p_this,
                                            uint8_t                  flag)
{
    uint8_t status_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2750_read(p_this, __TMD2750_REG_STATUS, &status_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    if (status_reg & flag) {
        status_reg = flag;
        ret = __tmd2750_write(p_this, __TMD2750_REG_STATUS, &status_reg, 1);
        if (ret != AM_OK) {
            return ret;
        }
    }

    return ret;
}

/** \brief 软件复位 */
am_local am_err_t __tmd2750_softrst(am_sensor_tmd2750_dev_t *p_this)
{
    uint8_t softrst_reg = 0x00;

    am_err_t ret = AM_OK;

    softrst_reg |= __TMD2750_SW_RST;
    ret = __tmd2750_write(p_this, __TMD2750_REG_SOFTRST, &softrst_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

am_local am_err_t __tmd2750_ioctl (am_sensor_tmd2750_dev_t  *p_dev,
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

    /* 设置功能使能 */
    case AM_SENSOR_TMD2750_FUNC_ENABLE:
        reg = __TMD2750_REG_ENABLE;
        regmask = (uint8_t)(int)p_arg;
        regval = (uint8_t)(int)p_arg;
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置功能禁能 */
    case AM_SENSOR_TMD2750_FUNC_DISABLE:
        reg = __TMD2750_REG_ENABLE;
        regmask = (uint8_t)(int)p_arg;
        regval = 0x00;
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置ATIME */
    case AM_SENSOR_TMD2750_ATIME_SET:
        reg = __TMD2750_REG_ATIME;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PRATE */
    case AM_SENSOR_TMD2750_PRATE_SET:
        reg = __TMD2750_REG_PRATE;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        regval = regval / 88 - 1;
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置WTIME */
    case AM_SENSOR_TMD2750_WTIME_SET:
        reg = __TMD2750_REG_WTIME;
        regmask = 0xFF;
        regval = (uint8_t)(int)p_arg;
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置ALS低阈值 */
    case AM_SENSOR_TMD2750_LOW_ALS_THRESHOLD_SET:
        ret = __tmd2750_threshold_set(p_dev, __TMD2750_REG_AILTL, (uint16_t)(int)p_arg);
        break;

    /* 设置ALS高阈值 */
    case AM_SENSOR_TMD2750_HIGH_ALS_THRESHOLD_SET:
        ret = __tmd2750_threshold_set(p_dev, __TMD2750_REG_AIHTL, (uint16_t)(int)p_arg);
        break;

    /* 设置Prox低阈值 */
    case AM_SENSOR_TMD2750_LOW_PROX_THRESHOLD_SET:
        if (p_dev->dev_info->p_param_default->apc_disable == AM_TMD2750_APC_ENABLE) {
            ret = __tmd2750_threshold_set(p_dev, __TMD2750_REG_PILTL, (uint16_t)(int)p_arg);
        } else {
            ret = __tmd2750_threshold_set(p_dev, __TMD2750_REG_PILTL, (uint16_t)(((int)p_arg) >> 2));
        }
        break;

    /* 设置Prox高阈值 */
    case AM_SENSOR_TMD2750_HIGH_PROX_THRESHOLD_SET:
        if (p_dev->dev_info->p_param_default->apc_disable == AM_TMD2750_APC_ENABLE) {
            ret = __tmd2750_threshold_set(p_dev, __TMD2750_REG_PIHTL, (uint16_t)(int)p_arg);
        } else {
            ret = __tmd2750_threshold_set(p_dev, __TMD2750_REG_PIHTL, (uint16_t)(((int)p_arg) >> 2));
        }
        break;

    /* 获取ALS低阈值 */
    case AM_SENSOR_TMD2750_LOW_ALS_THRESHOLD_GET:
        ret = __tmd2750_threshold_get(p_dev, __TMD2750_REG_AILTL, (uint16_t *)p_arg);
        break;

    /* 获取ALS高阈值 */
    case AM_SENSOR_TMD2750_HIGH_ALS_THRESHOLD_GET:
        ret = __tmd2750_threshold_get(p_dev, __TMD2750_REG_AIHTL, (uint16_t *)p_arg);
        break;

    /* 获取Prox低阈值 */
    case AM_SENSOR_TMD2750_LOW_PROX_THRESHOLD_GET:
        ret = __tmd2750_threshold_get(p_dev, __TMD2750_REG_PILTL, (uint16_t *)p_arg);
        break;

    /* 获取Prox高阈值 */
    case AM_SENSOR_TMD2750_HIGH_PROX_THRESHOLD_GET:
        ret = __tmd2750_threshold_get(p_dev, __TMD2750_REG_PIHTL, (uint16_t *)p_arg);
        break;

    /* 设置PPERS */
    case AM_SENSOR_TMD2750_PPERS_SET:
        reg = __TMD2750_REG_PERS;
        regmask = __TMD2750_MASK_PERS_PPERS;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_PPERS_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置APERS */
    case AM_SENSOR_TMD2750_APERS_SET:
        reg = __TMD2750_REG_PERS;
        regmask = __TMD2750_MASK_PERS_APERS;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_APERS_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置WLONG(wait long) */
    case AM_SENSOR_TMD2750_WLONG_SET:
        reg = __TMD2750_REG_CFG0;
        regmask = __TMD2750_MASK_CFG0_WLONG;
        regval = (am_bool_t)(int)p_arg;
        regval = __TMD2750_WLONG_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PPULSE_LEN_16X */
    case AM_SENSOR_TMD2750_PPULSE_LEN_16X_SET:
        reg = __TMD2750_REG_CFG0;
        regmask = __TMD2750_MASK_CFG0_PPULSE_LEN_16X;
        regval = (am_bool_t)(int)p_arg;
        regval = __TMD2750_PPULSE_LEN_16X_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PPULSE_LEN */
    case AM_SENSOR_TMD2750_PPULSE_LEN_SET:
        reg = __TMD2750_REG_PCFG0;
        regmask = __TMD2750_MASK_PCFG0_PPULSE_LEN;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_PPULSE_LEN_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置Prox周期脉冲的最大数量 */
    case AM_SENSOR_TMD2750_PPULSE_SET:
        reg = __TMD2750_REG_PCFG0;
        regmask = __TMD2750_MASK_PCFG0_PPULSE;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_PPULSE_SET(regval - 1);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置Prox IR传感器增益 */
    case AM_SENSOR_TMD2750_PGAIN_SET:
        reg = __TMD2750_REG_PCFG1;
        regmask = __TMD2750_MASK_PCFG1_PGAIN;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_PGAIN_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置Prox IR VCSEL电流驱动能力 */
    case AM_SENSOR_TMD2750_PLDRIVE_SET:
        reg = __TMD2750_REG_PCFG1;
        regmask = __TMD2750_MASK_PCFG1_PLDRIVE;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_PLDRIVE_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置ALS/Color传感器增益 */
    case AM_SENSOR_TMD2750_AGAIN_SET:
        reg = __TMD2750_REG_CFG1;
        regmask = __TMD2750_MASK_CFG1_AGAIN;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_AGAIN_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 读取REVID */
    case AM_SENSOR_TMD2750_REVID_GET:
        ret = __tmd2750_read(p_dev, __TMD2750_REG_REVID, (uint8_t *)p_arg, 1);
        if (ret != AM_OK) {
        #if __TMD2750_DEBUG
            am_kprintf("\r\nSensor TMD2750 read REVID failed! \r\n");
        #endif
        }
        break;

    /* 读取ID */
    case AM_SENSOR_TMD2750_ID_GET:
        ret = __tmd2750_read(p_dev, __TMD2750_REG_ID, (uint8_t *)p_arg, 1);
        if (ret != AM_OK) {
        #if __TMD2750_DEBUG
            am_kprintf("\r\nSensor TMD2750 read ID failed! \r\n");
        #endif
        }
        break;

    /* STATUS寄存器位清除 */
    case AM_SENSOR_TMD2750_STATUS_FLAG_CLR:
        ret = __tmd2750_status_flag_clr(p_dev,
                                       (uint8_t)(int)p_arg);
        break;

    /* 读取REVID2 */
    case AM_SENSOR_TMD2750_REVID2_GET:
        ret = __tmd2750_read(p_dev, __TMD2750_REG_REVID2, (uint8_t *)p_arg, 1);
        if (ret != AM_OK) {
        #if __TMD2750_DEBUG
            am_kprintf("\r\nSensor TMD2750 read REVID2 failed! \r\n");
        #endif
        }
        break;

    /* 软件复位 */
    case AM_SENSOR_TMD2750_SOFTRST:
        ret = __tmd2750_softrst(p_dev);
        break;

    /* 设置APC */
    case AM_SENSOR_TMD2750_APC_SET:
        reg = __TMD2750_REG_CFG6;
        regmask = __TMD2750_MASK_CFG6_APC;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_APC_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 选择Prox校准类型 */
    case AM_SENSOR_TMD2750_PROX_CAL_TYPE_SEL:
        reg = __TMD2750_REG_CALIB;
        regmask = __TMD2750_MASK_CALIB_ELECTRICAL_CALIBRATION;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_PROX_CAL_TYPE_SEL(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置偏移校准 */
    case AM_SENSOR_TMD2750_START_OFFSET_CAL_SET:
        reg = __TMD2750_REG_CALIB;
        regmask = __TMD2750_MASK_CALIB_START_OFFSET_CAL;
        regval = (am_bool_t)(int)p_arg;
        regval = __TMD2750_START_OFFSET_CAL(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置BINSRCH_TARGET */
    case AM_SENSOR_TMD2750_BINSRCH_TARGET_SET:
        reg = __TMD2750_REG_CALIBCFG;
        regmask = __TMD2750_MASK_CALIBCFG_BINSRCH_TARGET;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_BINSRCH_TARGET_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置AUTO_OFFSET_ADJ */
    case AM_SENSOR_TMD2750_AUTO_OFFSET_ADJ_SET:
        reg = __TMD2750_REG_CALIBCFG;
        regmask = __TMD2750_MASK_CALIBCFG_AUTO_OFFSET_ADJ;
        regval = (am_bool_t)(int)p_arg;
        regval = __TMD2750_AUTO_OFFSET_ADJ_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 设置PROX_AVG */
    case AM_SENSOR_TMD2750_PROX_AVG_SET:
        reg = __TMD2750_REG_CALIBCFG;
        regmask = __TMD2750_MASK_CALIBCFG_PROX_AVG;
        regval = (uint8_t)(int)p_arg;
        regval = __TMD2750_PROX_AVG_SET(regval);
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 中断使能 */
    case AM_SENSOR_TMD2750_INT_ENABLE:
        reg = __TMD2750_REG_INTENAB;
        regmask = (uint8_t)(int)p_arg;
        regval = (uint8_t)(int)p_arg;
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    /* 中断禁能 */
    case AM_SENSOR_TMD2750_INT_DISABLE:
        reg = __TMD2750_REG_INTENAB;
        regmask = (uint8_t)(int)p_arg;
        regval = 0x00;
        ret = __tmd2750_modify_reg_val(p_dev, reg, regmask, regval);
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/*******************************************************************************/

/** \brief TMD2750 获取offset值 */
am_local am_err_t __tmd2750_offset_cal(am_sensor_tmd2750_dev_t  *p_this,
                                       uint8_t                  *p_regval)
{
    am_err_t ret = AM_OK;

    if ((p_this == NULL) || (p_regval == NULL)) {
        return AM_ERROR;
    }

    if (p_regval[1] == 1) {
        /* offset为负数 */
        p_this->cal_val[0].prox_offset_adj = (int16_t)(0 - p_regval[0]);

    } else {
        /* offset为正数 */
        p_this->cal_val[0].prox_offset_adj = (int16_t)p_regval[0];
    }

    return ret;
}

/** \brief TMD2750 清空STATUS寄存器 */
am_local am_err_t __tmd2750_status_clr_all(am_sensor_tmd2750_dev_t  *p_this)
{
    uint8_t status_reg = 0xFF;

    am_err_t ret = AM_OK;

    ret = __tmd2750_write(p_this, __TMD2750_REG_STATUS, &status_reg, 1);

    return ret;
}

/** \brief TMD2750 参数初始化 */
am_local am_err_t __tmd2750_param_init(am_sensor_tmd2750_dev_t  *p_this)
{
    tmd2750_param_config_t *p_param_config = (tmd2750_param_config_t *)p_this->dev_info->p_param_default;

    am_err_t ret = AM_OK;

    uint8_t reg = 0x00;     /* 缓存寄存器值 */

    /* ATIME 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_ATIME_SET, (void *)(uint32_t)(p_param_config->atime_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* PRATE 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PRATE_SET, (void *)(uint32_t)(p_param_config->prox_sample_time_us));
    if (ret != AM_OK) {
        return ret;
    }

    /* WTIME 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_WTIME_SET, (void *)(uint32_t)(p_param_config->wtime_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* PERS 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PPERS_SET, (void *)(uint32_t)(p_param_config->ppers_val));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_APERS_SET, (void *)(uint32_t)(p_param_config->apers_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG0 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_WLONG_SET, (void *)(uint32_t)(p_param_config->wlong_en));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PPULSE_LEN_16X_SET, (void *)(uint32_t)(p_param_config->ppulse_len_16x_en));
    if (ret != AM_OK) {
        return ret;
    }

    /* PCFG0 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PPULSE_LEN_SET, (void *)(uint32_t)(p_param_config->pulse_len));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PPULSE_SET, (void *)(uint32_t)(p_param_config->pulse));
    if (ret != AM_OK) {
        return ret;
    }

    /* PCFG1 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PGAIN_SET, (void *)(uint32_t)(p_param_config->pgain_val));
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PLDRIVE_SET, (void *)(uint32_t)(p_param_config->pldrive_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG1 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_AGAIN_SET, (void *)(uint32_t)(p_param_config->again_val));
    if (ret != AM_OK) {
        return ret;
    }

    __tmd2750_read(p_this, __TMD2750_REG_CFG1, &reg, 1);
    reg |= (1 << 5);        /* CFG1寄存器的bit5必须置1 */
    ret = __tmd2750_write(p_this, __TMD2750_REG_CFG1, &reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG3 寄存器配置 */
    __tmd2750_status_clr_all(p_this);      /* 清除STATUS寄存器 */

    reg = 0x00;
    reg = __TMD2750_SAI_SET(p_param_config->sleep_after_interrupt);
    ret = __tmd2750_write(p_this, __TMD2750_REG_CFG3, &reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG4 寄存器配置 */
    reg = 0x3D;
    ret = __tmd2750_write(p_this, __TMD2750_REG_CFG4, &reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG6 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_APC_SET, (void *)(uint32_t)(p_param_config->apc_disable));
    if (ret != AM_OK) {
        return ret;
    }

    /* CALIBCFG 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PROX_AVG_SET, (void *)(uint32_t)(p_param_config->prox_avg));
    if (ret != AM_OK) {
        return ret;
    }

    /* TEST3 寄存器配置 */
    reg = 0xC4;
    ret = __tmd2750_write(p_this, __TMD2750_REG_TEST3, &reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

#if __TMD2750_DEBUG
const char *__tmd2750_reg_name_string[] = {
"ENABLE   ",
"ATIME    ",
"PRATE    ",
"WTIME    ",
"AILTL    ",
"AILTH    ",
"AIHTL    ",
"AIHTH    ",
"PILTL    ",
"PILTH    ",
"PIHTL    ",
"PIHTH    ",
"PERS     ",
"CFG0     ",
"PCFG0    ",
"PCFG1    ",
"CFG1     ",
"REVID    ",
"ID       ",
"STATUS   ",
"ALSL     ",
"ALSH     ",
"IRL      ",
"IRH      ",
"PDATAL   ",
"PDATAH   ",
"REVID2   ",
"CFG2     ",
"SOFTRST  ",
"CFG3     ",
"CFG4     ",
"CFG6     ",
"POFFSETL ",
"POFFSETH ",
"CALIB    ",
"CALIBCFG ",
"CALIBSTAT",
"INTENAB  ",
"FAC_L    ",
"FAC_H    ",
"TEST3    ",
};

/* 用于查看寄存器值 */
am_local void __tmd2750_regval_check(am_sensor_tmd2750_dev_t *p_this)
{
    uint8_t reg_val[41]         = {0};         /* 缓存寄存器值 */
    uint8_t counter = 0;

    __tmd2750_read(p_this, __TMD2750_REG_ENABLE, reg_val, 24);
    for (counter = 0; counter < 24;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2750_read(p_this, __TMD2750_REG_PDATAL, &reg_val[24], 5);
    for (counter = 24; counter < 29;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2750_read(p_this, __TMD2750_REG_CFG3, &reg_val[29], 2);
    for (counter = 29; counter < 31;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2750_read(p_this, __TMD2750_REG_CFG6, &reg_val[31], 1);
    counter = 31;
    am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);

    __tmd2750_read(p_this, __TMD2750_REG_POFFSETL, &reg_val[32], 2);
    for (counter = 32; counter < 34;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2750_read(p_this, __TMD2750_REG_CALIB, &reg_val[34], 1);
    counter = 34;
    am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);

    __tmd2750_read(p_this, __TMD2750_REG_CALIBCFG, &reg_val[35], 1);
    counter = 35;
    am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);

    __tmd2750_read(p_this, __TMD2750_REG_CALIBSTAT, &reg_val[36], 2);
    for (counter = 36; counter < 38;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2750_read(p_this, __TMD2750_REG_FAC_L, &reg_val[38], 2);
    for (counter = 38; counter < 40;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2750_read(p_this, __TMD2750_REG_TEST3, &reg_val[40], 1);
    counter = 40;
    am_kprintf("%s : 0x%02x \r\n",__tmd2750_reg_name_string[counter], reg_val[counter]);
}
#endif

/** \brief TMD2750 校准 */
am_local am_err_t __tmd2750_open_cal(am_sensor_tmd2750_dev_t    *p_this)
{
    am_err_t ret    = AM_OK;

    tmd2750_param_config_t *p_param_config = (tmd2750_param_config_t *)p_this->dev_info->p_param_default;

    /* enable 寄存器配置禁能 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_FUNC_DISABLE,
                               (void *)(AM_TMD2750_WEN | \
                                        AM_TMD2750_PEN | \
                                        AM_TMD2750_AEN | \
                                        AM_TMD2750_PON ));
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器配置使能 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_FUNC_ENABLE,
                                   (void *)AM_TMD2750_PON);
    if (ret != AM_OK) {
        return ret;
    }

    /* 清中断标志位 */
    __tmd2750_status_clr_all(p_this);

    /* INTENAB 寄存器配置 */

    /* 校准中断使能 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_INT_ENABLE, (void *)AM_TMD2750_CIEN);
    if (ret != AM_OK) {
        return ret;
    }

    /* CALIBCFG 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_BINSRCH_TARGET_SET, (void *)(uint32_t)p_param_config->binsrch_target);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_AUTO_OFFSET_ADJ_SET, (void *)(uint32_t)p_param_config->auto_offset_adj);
    if (ret != AM_OK) {
        return ret;
    }

    /* CALIB 寄存器配置 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_PROX_CAL_TYPE_SEL, (void *)(uint32_t)p_param_config->prox_cal_type);
    if (ret != AM_OK) {
        return ret;
    }

    /* 开启偏移校准 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_START_OFFSET_CAL_SET, (void *)AM_TRUE);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

am_local am_err_t __tmd2750_close_cal(am_sensor_tmd2750_dev_t   *p_this)
{
    am_err_t ret    = AM_OK;

    /* 清除STATUS */
    __tmd2750_status_clr_all(p_this);

    /* 校准中断禁能 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_INT_DISABLE, (void *)AM_TMD2750_CIEN);
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器配置禁能 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_FUNC_DISABLE,
                                                (void *)AM_TMD2750_PON);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

am_local am_err_t __tmd2750_calibration(am_sensor_tmd2750_dev_t *p_this)
{
    am_err_t ret    = AM_OK;

    uint8_t reg             = 0x00;         /* 缓存寄存器值 */
    uint8_t offset_data[2]  = {0};          /* 缓存偏移寄存器值 */
    uint32_t cal_timeout    = 0xffff;       /* 校准超时计数 */

    /* 开启校准 */
    ret = __tmd2750_open_cal(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    while(1) {

        /* 等待校准数据生成完毕 */
        __tmd2750_read(p_this, __TMD2750_REG_CALIBSTAT, &reg, 1);

        if (__TMD2750_CALIB_FINISHED_GET(reg) == 0x01) {     /* CALIB_FINISHED = 1 */

            am_kprintf("tmd2750_calibration finished!\r\n");

            break;
        }
        if (cal_timeout != 0) {
            cal_timeout--;
        }
        if (!cal_timeout) {

            am_kprintf("tmd2750_calibration timeout!\r\n");

            /* 关闭校准 */
            __tmd2750_close_cal(p_this);

            return AM_ERROR;
        }
    }

    /* 读取POFFSETL、POFFSETH */
    am_mdelay(100);
    __tmd2750_read(p_this, __TMD2750_REG_POFFSETL, offset_data, 2);
#if __TMD2750_DEBUG
    am_kprintf("offset_data[0] : 0x%02x \r\n", offset_data[0]);
    am_kprintf("offset_data[1] : 0x%02x \r\n", offset_data[1]);
#endif
    __tmd2750_offset_cal(p_this, offset_data);

    /* 关闭校准 */
    ret = __tmd2750_close_cal(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/**
 * \brief TMD2750 获取结果
 */
am_local am_err_t __tmd2750_result_get(am_sensor_tmd2750_dev_t  *p_this,
                                       tmd2750_result_info_t    *p_result)
{
    am_err_t ret = AM_OK;

    uint8_t result_buf[6]  = {0};         /* 缓存寄存器值 */

    __tmd2750_read(p_this, __TMD2750_REG_ALSL, result_buf, 4);
    __tmd2750_read(p_this, __TMD2750_REG_PDATAL, &result_buf[4], 2);

    /* ALS (CH0) */
    p_result->als_channel_data = __TMD2750_UINT8_TO_UINT16(&result_buf[0]);

    /* IR  (CH1) */
    p_result->ir_channel_data = __TMD2750_UINT8_TO_UINT16(&result_buf[2]);

    /* Prox AD值 */
    if (p_this->dev_info->p_param_default->apc_disable == AM_TMD2750_APC_ENABLE) {
        p_result->raw_prox_value = __TMD2750_UINT8_TO_UINT16(&result_buf[4]);
    } else {
        p_result->raw_prox_value = (uint16_t)((result_buf[4] << 2) | (result_buf[5] & 0x03));
    }

    /* prox 偏移值 */
    p_result->prox_offset_adjust_value = p_this->cal_val[0].prox_offset_adj;

    /* Prox */
    p_result->prox_value = p_result->raw_prox_value;

    return ret;
}

/*******************************************************************************/

/** \breif 中断回调函数 */
am_local void __tmd2750_alarm_callback (void *p_arg)
{
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_arg;

    tmd2750_result_info_t result;

    /* 获取数据 */
    __tmd2750_result_get(p_this, &result);

    /* 获取数据后，清中断标志位 */
    __tmd2750_status_clr_all(p_this);

    /* 获取Prox */
    p_this->data[0].val = result.prox_value;
    p_this->data[0].unit = AM_SENSOR_UNIT_BASE;

    /* 获取ALS */
    p_this->data[1].val = result.als_channel_data;
    p_this->data[1].unit = AM_SENSOR_UNIT_BASE;

    /* 获取IR */
    p_this->data[2].val = result.ir_channel_data;
    p_this->data[2].unit = AM_SENSOR_UNIT_BASE;

    if (p_this->pfn_trigger_cb[0] &&
            (p_this->flags[0] & AM_SENSOR_TRIGGER_THRESHOLD)) {
        p_this->pfn_trigger_cb[0](p_this->p_arg[0],
                                  AM_SENSOR_TRIGGER_THRESHOLD);
    }

    if (p_this->pfn_trigger_cb[1] &&
            (p_this->flags[1] & AM_SENSOR_TRIGGER_THRESHOLD)) {
        p_this->pfn_trigger_cb[1](p_this->p_arg[1],
                                  AM_SENSOR_TRIGGER_THRESHOLD);
    }
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_TMD2750_CHAN_1) {
        return AM_SENSOR_TYPE_PROXIMITY;
    } else if (id == AM_TMD2750_CHAN_2) {
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
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    tmd2750_result_info_t result;

    int cur_id = 0;
    int i = 0;

    uint8_t j;
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
        if ((AM_BIT_GET(p_this->trigger, 2)) &&
                ((p_this->flags[0] & AM_SENSOR_TRIGGER_THRESHOLD) ||
                 (p_this->flags[1] & AM_SENSOR_TRIGGER_THRESHOLD))) {

            if (cur_id == 0) {

                p_buf[0].val = p_this->data[cur_id].val;
                p_buf[0].unit= p_this->data[cur_id].unit;

            } else if (cur_id == 1) {

                for (j = 0; j < 2; j++) {
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
    do {
        ret = __tmd2750_result_get(p_this, &result);
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

            p_buf[i].val  = result.prox_value;                  /*< \brief Prox */
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;                /*< \brief 单位默认为0:10^(0)*/

        } else if (cur_id == 1) {

            p_buf[i].val  = result.als_channel_data;            /* ALS */
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;                /*< \brief 单位默认为0:10^(0)*/

            p_buf[i+1].val  = result.ir_channel_data;           /* IR */
            p_buf[i+1].unit = AM_SENSOR_UNIT_BASE;              /*< \brief 单位默认为0:10^(0)*/

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
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_drv;

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

    if (ret != AM_OK) {     /*< \breif 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }

    /* 寄存器参数初始化 */
    ret = __tmd2750_param_init(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* 校准 */
    ret = __tmd2750_calibration(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器配置使能 */
    ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_FUNC_ENABLE,
                            (void *)(AM_TMD2750_WEN | \
                                     AM_TMD2750_PEN | \
                                     AM_TMD2750_AEN | \
                                     AM_TMD2750_PON ));
    if (ret != AM_OK) {
        return ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 7);
        AM_BIT_SET(p_this->trigger, 6);
        AM_BIT_SET(p_this->trigger, 5);
    }

    return curent_ret;
}

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_drv;

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

        /* enable 寄存器配置禁能 */
        ret = __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_FUNC_DISABLE,
                                    (void *)(AM_TMD2750_WEN | \
                                             AM_TMD2750_PEN | \
                                             AM_TMD2750_AEN | \
                                             AM_TMD2750_PON ));
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
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_drv;

    if (id != 0 && id != 1) {
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
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0 && id != 1) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x4) {
        return AM_OK;
    }

    if (p_this->dev_info->trigger_pin != -1) {
        ret = am_gpio_trigger_on(p_this->dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }

    /* 清中断标志位 */
    __tmd2750_status_clr_all(p_this);

    /* 使能中断 */
    __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_INT_ENABLE,
                                (void *)(AM_TMD2750_PIEN |    /* Prox中断 */
                                         AM_TMD2750_AIEN));    /* ALS中断 */

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
    am_sensor_tmd2750_dev_t* p_this = (am_sensor_tmd2750_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

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

        if ((p_this->dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        if (cur_ret == AM_OK) {

            /* 禁能中断 */
            __tmd2750_ioctl(p_this, AM_SENSOR_TMD2750_INT_DISABLE,
                                               (void *)(AM_TMD2750_PIEN |    /* Prox中断 */
                                                        AM_TMD2750_AIEN));   /* ALS中断 */
            AM_BIT_CLR(p_this->trigger, 2);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 TMD2750 初始化
 */
am_sensor_handle_t am_sensor_tmd2750_init (
        am_sensor_tmd2750_dev_t           *p_dev,
        const am_sensor_tmd2750_devinfo_t *p_devinfo,
        am_i2c_handle_t                     handle)
{
    am_err_t ret = AM_OK;

    uint8_t tmd2750_id = 0;
    uint8_t reg = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->tmd2750_dev.p_drv   = p_dev;
    p_dev->tmd2750_dev.p_funcs = &__g_sensor_tmd2750_funcs;
    p_dev->dev_info           = p_devinfo;
    p_dev->pfn_trigger_cb[0]  = NULL;
    p_dev->pfn_trigger_cb[1]  = NULL;
    p_dev->p_arg[0]           = NULL;
    p_dev->p_arg[1]           = NULL;
    p_dev->flags[0]           = 0;
    p_dev->flags[1]           = 0;
    p_dev->trigger            = 0;
    p_dev->data[0].val        = 0;
    p_dev->data[0].unit       = AM_SENSOR_UNIT_INVALID;
    p_dev->data[1].val        = 0;
    p_dev->data[1].unit       = AM_SENSOR_UNIT_INVALID;
    p_dev->data[2].val        = 0;
    p_dev->data[2].unit       = AM_SENSOR_UNIT_INVALID;

    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __tmd2750_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);  /* 下降沿触发 */
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    /* TMD2750软件复位 */
    __tmd2750_softrst(p_dev);
    am_mdelay(10);

    /* 等待复位完成 */
    do {
        __tmd2750_read(p_dev, __TMD2750_REG_SOFTRST, &reg, 1);

    } while (__TMD2750_SOFTRST_GET(reg) == 0x1);

    ret = __tmd2750_read(p_dev, __TMD2750_REG_ID, &tmd2750_id, 1);
    if ((ret != AM_OK) || (tmd2750_id != __TMD2750_ID)) {
    #if __TMD2750_DEBUG
        am_kprintf("tmd2750_id = 0x%x \r\n", tmd2750_id);
    #endif
        am_kprintf("\r\nSensor TMD2750 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->tmd2750_dev);
}

/**
 * \brief 传感器 TMD2750 去初始化
 */
am_err_t am_sensor_tmd2750_deinit (am_sensor_handle_t handle)
{
    am_sensor_tmd2750_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->tmd2750_dev.p_drv   = NULL;
    p_dev->tmd2750_dev.p_funcs = NULL;
    p_dev->dev_info           = NULL;

    return AM_OK;
}

/**
 * \brief 传感器 TMD2750 控制函数
 */
am_err_t am_sensor_tmd2750_ioctl (am_sensor_handle_t  handle,
                                  int                 cmd,
                                  void               *p_arg)
{
    am_sensor_tmd2750_dev_t   *p_dev = (am_sensor_tmd2750_dev_t *)handle->p_drv;

    am_err_t ret = AM_OK;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    ret = __tmd2750_ioctl(p_dev, cmd, p_arg);

    return ret;
}

/* end of file */

