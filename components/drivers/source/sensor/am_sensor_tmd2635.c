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
 * \brief 传感器 TMD2635 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_tmd2635.h"
#include "am_sensor.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**< 调试信息输出 */
#define __TMD2635_DEBUG             (0)

/*********************************************************************************************
 * 宏定义
 ********************************************************************************************/
#define __TMD2635_REG_ENABLE        (0x80)  /**< \brief 使能状态                            */
#define __TMD2635_REG_PRATE         (0x82)  /**< \brief Prox采样持续时间:(PRATE + 1) * 88us */
#define __TMD2635_REG_PILTL         (0x88)  /**< \brief Prox中断低阈值低字节                */
#define __TMD2635_REG_PILTH         (0x89)  /**< \brief Prox中断低阈值高字节                */
#define __TMD2635_REG_PIHTL         (0x8A)  /**< \brief Prox中断高阈值低字节                */
#define __TMD2635_REG_PIHTH         (0x8B)  /**< \brief Prox中断高阈值高字节                */
#define __TMD2635_REG_PERS          (0x8C)  /**< \brief prox中断过滤器                      */
#define __TMD2635_REG_CFG0          (0x8D)  /**< \brief 配置寄存器0                         */
#define __TMD2635_REG_PCFG0         (0x8E)  /**< \brief Prox配置寄存器0                     */
#define __TMD2635_REG_PCFG1         (0x8F)  /**< \brief Prox配置寄存器1                     */
#define __TMD2635_REG_REVID         (0x91)  /**< \brief 修订ID                              */
#define __TMD2635_REG_ID            (0x92)  /**< \brief 设备ID                              */
#define __TMD2635_REG_STATUS        (0x9B)  /**< \brief 设备状态                            */
#define __TMD2635_REG_PDATAL        (0x9C)  /**< \brief Prox ADC数据低字节                  */
#define __TMD2635_REG_PDATAH        (0x9D)  /**< \brief Prox ADC数据高字节                  */
#define __TMD2635_REG_REVID2        (0xA6)  /**< \brief 修订ID 2                            */
#define __TMD2635_REG_SOFTRST       (0xA8)  /**< \brief 软复位                              */
#define __TMD2635_REG_PWTIME        (0xA9)  /**< \brief Prox之间的等待时间                  */
#define __TMD2635_REG_CFG8          (0xAA)  /**< \brief 配置寄存器8                         */
#define __TMD2635_REG_CFG3          (0xAB)  /**< \brief 配置寄存器3                         */
#define __TMD2635_REG_CFG6          (0xAE)  /**< \brief 配置寄存器6                         */
#define __TMD2635_REG_PFILTER       (0xB3)  /**< \brief prox值过滤器                        */
#define __TMD2635_REG_POFFSETL      (0xC0)  /**< \brief Prox偏移低字节数据                  */
#define __TMD2635_REG_POFFSETH      (0xC1)  /**< \brief Prox偏移高字节数据                  */
#define __TMD2635_REG_CALIB         (0xD7)  /**< \brief Prox偏移校准                        */
#define __TMD2635_REG_CALIBCFG      (0xD9)  /**< \brief Prox偏移校准控制                    */
#define __TMD2635_REG_CALIBSTAT     (0xDC)  /**< \brief Prox偏移校准状态                    */
#define __TMD2635_REG_INTENAB       (0xDD)  /**< \brief 中断使能                            */
#define __TMD2635_REG_FAC_L         (0xE5)  /**< \brief 出厂设置低数据 (lot code data)      */
#define __TMD2635_REG_FAC_H         (0xE6)  /**< \brief 出厂设置高数据 (lot code data)      */
#define __TMD2635_REG_TEST9         (0xF9)  /**< \brief 测试9 (must be set to 0x0D)         */

/** \brief 设置PPERS */
#define __TMD2635_PPERS_SET(data, mask)             (((data) & (~(0xf << 0))) | (mask << 0))

/** \brief 设置PWLONG(wait long) */
#define __TMD2635_PWLONG_SET(data, mask)            (((data) & (~(0x1 << 3))) | (mask << 3))

/** \brief 设置PGAIN */
#define __TMD2635_PGAIN_SET(data, mask)             (((data) & (~(0x3 << 6))) | (mask << 6))

/** \brief 设置PPULSE */
#define __TMD2635_PPULSE_SET(data, mask)            (((data) & (~(0x3f << 0))) | (mask << 0))

/** \brief 设置PPULSE_LEN */
#define __TMD2635_PPULSE_LEN_SET(data, mask)        (((data) & (~(0x7 << 5))) | (mask << 5))

/** \brief 设置PLDRIVE */
#define __TMD2635_PLDRIVE_SET(data, mask)           (((data) & (~(0xf << 0))) | (mask << 0))

/** \brief TMD2635 ID */
#define __TMD2635_ID                                (0x44)

/** \brief 获取PHIGH状态 */
#define __TMD2635_PHIGH_STATUS(reg)                 (((reg) >> 7) & 0x1)

/** \brief 获取PLOW状态 */
#define __TMD2635_PLOW_STATUS(reg)                  (((reg) >> 6) & 0x1)

/** \brief 获取PINT状态 */
#define __TMD2635_PINT_STATUS(reg)                  (((reg) >> 4) & 0x1)

/** \brief 获取CINT状态 */
#define __TMD2635_CINT_STATUS(reg)                  (((reg) >> 3) & 0x1)

/** \brief 获取ZINT状态 */
#define __TMD2635_ZINT_STATUS(reg)                  (((reg) >> 2) & 0x1)

/** \brief TMD2635软件复位 */
#define __TMD2635_SW_RST                            (0x1 << 0)

/** \brief 获取SOFTRST位 */
#define __TMD2635_SOFTRST_GET(reg)                  (((reg) >> 0) & 0x1)

/** \brief 设置PDSELECT */
#define __TMD2635_PDSELECT_SET(data, mask)          (((data) & (~(0x3 << 0))) | (mask << 0))

/** \brief 设置SAI */
#define __TMD2635_SAI_SET(data, mask)               (((data) & (~(0x1 << 4))) | (mask << 4))

/** \brief 设置APC */
#define __TMD2635_APC_SET(data, mask)               (((data) & (~(0x1 << 6))) | (mask << 6))

/** \brief 设置PMAVG */
#define __TMD2635_PMAVG_SET(data, mask)             (((data) & (~(0x3 << 0))) | (mask << 0))

/** \brief 设置CALAVG */
#define __TMD2635_CALAVG_SET(data, mask)            (((data) & (~(0x1 << 7))) | (mask << 7))

/** \brief 选择Prox校准类型 */
#define __TMD2635_PROX_CAL_TYPE_SEL(data, mask)     (((data) & (~(0x1 << 5))) | (mask << 5))

/** \brief 设置CALPRATE */
#define __TMD2635_CALPRATE_SET(data, mask)          (((data) & (~(0x1 << 4))) | (mask << 4))

/** \brief 开启偏移校准设置 */
#define __TMD2635_START_OFFSET_CAL(data, mask)      (((data) & (~(0x1 << 0))) | (mask << 0))

/** \brief 设置BINSRCH_TARGET */
#define __TMD2635_BINSRCH_TARGET_SET(data, mask)    (((data) & (~(0x7 << 5))) | (mask << 5))

/** \brief 设置AUTO_OFFSET_ADJ */
#define __TMD2635_AUTO_OFFSET_ADJ_SET(data, mask)   (((data) & (~(0x1 << 3))) | (mask << 3))

/** \brief 设置PROX_AVG */
#define __TMD2635_PROX_AVG_SET(data, mask)          (((data) & (~(0x7 << 0))) | (mask << 0))

/** \brief 获取CALIB_FINISHED位 */
#define __TMD2635_CALIB_FINISHED_GET(reg)           (((reg) >> 0) & 0x1)

/** \brief 将两个uint8转换为一个uint16_t类型 */
#define __TMD2635_UINT8_TO_UINT16(buff)             ((uint16_t)((*(buff + 1)) << 8 | (*buff)))

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_tmd2635_funcs = {
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
 * \brief tmd2635 写数据
 */
am_local am_err_t __tmd2635_write (am_sensor_tmd2635_dev_t  *p_this,
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
 * \brief tmd2635 读数据
 */
am_local am_err_t __tmd2635_read (am_sensor_tmd2635_dev_t   *p_this,
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
am_local am_err_t __tmd2635_threshold_set(am_sensor_tmd2635_dev_t   *p_this,
                                          uint32_t                   subaddr,
                                          uint16_t                   value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    reg[0] = value & 0xff;
    reg[1] = (value & 0xff00) >> 8;

    ret = __tmd2635_write(p_this, subaddr, reg, 2);

    return ret;
}

/** \brief 获取阈值 */
am_local am_err_t __tmd2635_threshold_get(am_sensor_tmd2635_dev_t   *p_this,
                                          uint32_t                   subaddr,
                                          uint16_t                  *p_value)
{
    am_err_t ret = AM_OK;

    uint8_t reg[2] = {0};

    ret = __tmd2635_read(p_this, subaddr, reg, 2);

    *p_value = __TMD2635_UINT8_TO_UINT16(&reg[0]);

    return ret;
}

/*******************************************************************************/

/** \brief 设置功能使能 */
am_local am_err_t __tmd2635_func_en_enable(am_sensor_tmd2635_dev_t  *p_this,
                                           uint8_t                   flag)
{
    uint8_t enable_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    enable_reg = (enable_reg & (~(flag))) | flag;
    ret = __tmd2635_write(p_this, __TMD2635_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置功能禁能 */
am_local am_err_t __tmd2635_func_disen_enable(am_sensor_tmd2635_dev_t   *p_this,
                                              uint8_t                    flag)
{
    uint8_t enable_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    enable_reg = enable_reg & (~(flag));
    ret = __tmd2635_write(p_this, __TMD2635_REG_ENABLE, &enable_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置PRATE */
am_local am_err_t __tmd2635_prate(am_sensor_tmd2635_dev_t   *p_this,
                                  uint32_t                   prox_sample_time_us)
{
    uint8_t prate_reg = 0x00;

    am_err_t ret = AM_OK;

    prate_reg = prox_sample_time_us / 88 - 1;
    ret = __tmd2635_write(p_this, __TMD2635_REG_PRATE, &prate_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置Prox低阈值 */
am_local am_err_t __tmd2635_low_prox_threshold_set(am_sensor_tmd2635_dev_t  *p_this,
                                                   uint16_t                  low_prox_value)
{
    return __tmd2635_threshold_set(p_this, __TMD2635_REG_PILTL, low_prox_value);
}

/** \brief 设置Prox高阈值 */
am_local am_err_t __tmd2635_high_prox_threshold_set(am_sensor_tmd2635_dev_t *p_this,
                                                    uint16_t                 high_prox_value)
{
    return __tmd2635_threshold_set(p_this, __TMD2635_REG_PIHTL, high_prox_value);
}

/** \brief 获取Prox低阈值 */
am_local am_err_t __tmd2635_low_prox_threshold_get(am_sensor_tmd2635_dev_t  *p_this,
                                                   uint16_t                 *p_low_prox_value)
{
    return __tmd2635_threshold_get(p_this, __TMD2635_REG_PILTL, p_low_prox_value);
}

/** \brief 获取Prox高阈值 */
am_local am_err_t __tmd2635_high_prox_threshold_get(am_sensor_tmd2635_dev_t *p_this,
                                                    uint16_t                *p_high_prox_value)
{
    return __tmd2635_threshold_get(p_this, __TMD2635_REG_PIHTL, p_high_prox_value);
}

/** \brief 设置PPERS */
am_local am_err_t __tmd2635_ppers_pers(am_sensor_tmd2635_dev_t  *p_this,
                                       uint8_t                   ppers)
{
    uint8_t pers_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_PERS, &pers_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    pers_reg = __TMD2635_PPERS_SET(pers_reg, ppers);
    ret = __tmd2635_write(p_this, __TMD2635_REG_PERS, &pers_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置PWLONG(wait long) */
am_local am_err_t __tmd2635_pwlong_cfg0(am_sensor_tmd2635_dev_t *p_this,
                                        am_bool_t                pwlong_enable)
{
    uint8_t cfg0_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CFG0, &cfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    cfg0_reg = __TMD2635_PWLONG_SET(cfg0_reg, pwlong_enable);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CFG0, &cfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置Prox IR传感器增益 */
am_local am_err_t __tmd2635_pgain_pcfg0(am_sensor_tmd2635_dev_t *p_this,
                                        uint8_t                  pgain)
{
    uint8_t pcfg0_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_PCFG0, &pcfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    pcfg0_reg = __TMD2635_PGAIN_SET(pcfg0_reg, pgain);
    ret = __tmd2635_write(p_this, __TMD2635_REG_PCFG0, &pcfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置Prox周期脉冲的最大数量 */
am_local am_err_t __tmd2635_ppulse_pcfg0(am_sensor_tmd2635_dev_t    *p_this,
                                         uint8_t                     ppulse)
{
    uint8_t pcfg0_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_PCFG0, &pcfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    pcfg0_reg = __TMD2635_PPULSE_SET(pcfg0_reg, (ppulse - 1));
    ret = __tmd2635_write(p_this, __TMD2635_REG_PCFG0, &pcfg0_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置PPULSE_LEN */
am_local am_err_t __tmd2635_ppulse_len_pcfg1(am_sensor_tmd2635_dev_t    *p_this,
                                             uint8_t                     ppulse_len)
{
    uint8_t pcfg1_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_PCFG1, &pcfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    pcfg1_reg = __TMD2635_PPULSE_LEN_SET(pcfg1_reg, ppulse_len);
    ret = __tmd2635_write(p_this, __TMD2635_REG_PCFG1, &pcfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置Prox IR VCSEL电流驱动能力 */
am_local am_err_t __tmd2635_pldrive_pcfg1(am_sensor_tmd2635_dev_t   *p_this,
                                            uint8_t                     pldrive)
{
    uint8_t pcfg1_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_PCFG1, &pcfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    pcfg1_reg = __TMD2635_PLDRIVE_SET(pcfg1_reg, pldrive);
    ret = __tmd2635_write(p_this, __TMD2635_REG_PCFG1, &pcfg1_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 读取REVID */
am_local am_err_t __tmd2635_read_revid(am_sensor_tmd2635_dev_t  *p_this,
                                         uint8_t                    *p_revid)
{
    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_REVID, p_revid, 1);
    if (ret != AM_OK) {
    #if __TMD2635_DEBUG
        am_kprintf("\r\nSensor TMD2635 read REVID failed! \r\n");
    #endif
    }

    return ret;
}

/** \brief 读取ID */
am_local am_err_t __tmd2635_read_id(am_sensor_tmd2635_dev_t *p_this,
                                      uint8_t                   *p_id)
{
    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_ID, p_id, 1);
    if (ret != AM_OK) {
    #if __TMD2635_DEBUG
        am_kprintf("\r\nSensor TMD2635 read ID failed! \r\n");
    #endif
    }

    return ret;
}

/** \brief STATUS寄存器位清除 */
am_local am_err_t __tmd2635_status_flag_clr(am_sensor_tmd2635_dev_t *p_this,
                                              uint8_t                   flag)
{
    uint8_t status_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_STATUS, &status_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    if (status_reg & flag) {
        status_reg = flag;
        ret = __tmd2635_write(p_this, __TMD2635_REG_STATUS, &status_reg, 1);
        if (ret != AM_OK) {
            return ret;
        }
    }

    return ret;
}

/** \brief 读取REVID2 */
am_local am_err_t __tmd2635_read_revid2(am_sensor_tmd2635_dev_t *p_this,
                                          uint8_t                   *p_revid2)
{
    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_REVID2, p_revid2, 1);
    if (ret != AM_OK) {
    #if __TMD2635_DEBUG
        am_kprintf("\r\nSensor TMD2635 read REVID2 failed! \r\n");
    #endif
    }

    return ret;
}

/** \brief 软件复位 */
am_local am_err_t __tmd2635_softrst(am_sensor_tmd2635_dev_t *p_this)
{
    uint8_t softrst_reg = 0x00;

    am_err_t ret = AM_OK;

    softrst_reg |= __TMD2635_SW_RST;
    ret = __tmd2635_write(p_this, __TMD2635_REG_SOFTRST, &softrst_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置PWTIME */
am_local am_err_t __tmd2635_pwtime(am_sensor_tmd2635_dev_t  *p_this,
                                   uint8_t                   pwtime)
{
    uint8_t pwtime_reg = 0x00;

    am_err_t ret = AM_OK;

    pwtime_reg = pwtime;
    ret = __tmd2635_write(p_this, __TMD2635_REG_PWTIME, &pwtime_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置PDSELECT */
am_local am_err_t __tmd2635_pdselect_cfg8(am_sensor_tmd2635_dev_t   *p_this,
                                          uint8_t                    pdselect)
{
    uint8_t cfg8_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CFG8, &cfg8_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    cfg8_reg = __TMD2635_PDSELECT_SET(cfg8_reg, pdselect);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CFG8, &cfg8_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置APC */
am_local am_err_t __tmd2635_apc_cfg6(am_sensor_tmd2635_dev_t    *p_this,
                                     uint8_t                     apc)
{
    uint8_t cfg6_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CFG6, &cfg6_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    cfg6_reg = __TMD2635_APC_SET(cfg6_reg, apc);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CFG6, &cfg6_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置PMAVG */
am_local am_err_t __tmd2635_pmavg_pfilter(am_sensor_tmd2635_dev_t   *p_this,
                                          uint8_t                    pmavg)
{
    uint8_t pfilter_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_PFILTER, &pfilter_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    pfilter_reg = __TMD2635_PMAVG_SET(pfilter_reg, pmavg);
    ret = __tmd2635_write(p_this, __TMD2635_REG_PFILTER, &pfilter_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置CALAVG */
am_local am_err_t __tmd2635_calavg_calib(am_sensor_tmd2635_dev_t    *p_this,
                                         uint8_t                     calavg)
{
    uint8_t calib_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    calib_reg = __TMD2635_CALAVG_SET(calib_reg, calavg);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 选择Prox校准类型 */
am_local am_err_t __tmd2635_prox_cal_type_sel_calib(am_sensor_tmd2635_dev_t *p_this,
                                                    uint8_t                  prox_cal_type)
{
    uint8_t calib_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    calib_reg = __TMD2635_PROX_CAL_TYPE_SEL(calib_reg, prox_cal_type);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置CALPRATE */
am_local am_err_t __tmd2635_calprate_calib(am_sensor_tmd2635_dev_t  *p_this,
                                           uint8_t                   calprate)
{
    uint8_t calib_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    calib_reg = __TMD2635_CALPRATE_SET(calib_reg, calprate);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置偏移校准 */
am_local am_err_t __tmd2635_start_offset_cal_calib(am_sensor_tmd2635_dev_t  *p_this,
                                                     am_bool_t                  start_offset_cal_enable)
{
    uint8_t calib_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    calib_reg = __TMD2635_START_OFFSET_CAL(calib_reg, start_offset_cal_enable);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

#if __TMD2635_DEBUG
    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIB, &calib_reg, 1);
    am_kprintf("calib_reg : 0x%02x \r\n", calib_reg);
    if (ret != AM_OK) {
        return ret;
    }
#endif

    return ret;
}

/** \brief 设置BINSRCH_TARGET */
am_local am_err_t __tmd2635_binsrch_target_calibcfg(am_sensor_tmd2635_dev_t *p_this,
                                                      uint8_t                   binsrch_target)
{
    uint8_t calibcfg_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIBCFG, &calibcfg_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    calibcfg_reg = __TMD2635_BINSRCH_TARGET_SET(calibcfg_reg, binsrch_target);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CALIBCFG, &calibcfg_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置AUTO_OFFSET_ADJ */
am_local am_err_t __tmd2635_auto_offset_adj_calibcfg(am_sensor_tmd2635_dev_t    *p_this,
                                                       am_bool_t                    auto_offset_adj)
{
    uint8_t calibcfg_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIBCFG, &calibcfg_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    calibcfg_reg = __TMD2635_AUTO_OFFSET_ADJ_SET(calibcfg_reg, auto_offset_adj);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CALIBCFG, &calibcfg_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 设置PROX_AVG */
am_local am_err_t __tmd2635_prox_avg_calibcfg(am_sensor_tmd2635_dev_t   *p_this,
                                                uint8_t                     prox_avg)
{
    uint8_t calibcfg_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_CALIBCFG, &calibcfg_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    calibcfg_reg = __TMD2635_PROX_AVG_SET(calibcfg_reg, prox_avg);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CALIBCFG, &calibcfg_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 中断使能 */
am_local am_err_t __tmd2635_interrupt_enable_intenab(am_sensor_tmd2635_dev_t    *p_this,
                                                       uint8_t                      flag)
{
    uint8_t intenab_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    intenab_reg = (intenab_reg & (~(flag))) | flag;
    ret = __tmd2635_write(p_this, __TMD2635_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief 中断禁能 */
am_local am_err_t __tmd2635_interrupt_disable_intenab(am_sensor_tmd2635_dev_t   *p_this,
                                                        uint8_t                     flag)
{
    uint8_t intenab_reg = 0x00;

    am_err_t ret = AM_OK;

    ret = __tmd2635_read(p_this, __TMD2635_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    intenab_reg = intenab_reg & (~(flag));
    ret = __tmd2635_write(p_this, __TMD2635_REG_INTENAB, &intenab_reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/*******************************************************************************/

/** \brief TMD2635 获取offset值 */
am_local am_err_t __tmd2635_offset_cal(am_sensor_tmd2635_dev_t  *p_this,
                                         uint8_t                    *p_regval)
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

/** \brief TMD2635 清空STATUS寄存器 */
am_local am_err_t __tmd2635_status_clr_all(am_sensor_tmd2635_dev_t *p_this)
{
    uint8_t status_reg = 0xFF;

    am_err_t ret = AM_OK;

    ret = __tmd2635_write(p_this, __TMD2635_REG_STATUS, &status_reg, 1);

    return ret;
}

/** \brief TMD2635 参数初始化 */
am_local am_err_t __tmd2635_param_init(am_sensor_tmd2635_dev_t *p_this)
{
    tmd2635_param_config_t *p_param_config = (tmd2635_param_config_t *)p_this->dev_info->p_param_default;

    am_err_t ret = AM_OK;

    uint8_t reg = 0x00;     /* 缓存寄存器值 */

    /* PRATE 寄存器配置 */
    ret = __tmd2635_prate(p_this, p_param_config->prox_sample_time_us);
    if (ret != AM_OK) {
        return ret;
    }

    /* PERS 寄存器配置 */
    ret = __tmd2635_ppers_pers(p_this, p_param_config->ppers_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG0 寄存器配置 */
    ret = __tmd2635_pwlong_cfg0(p_this, p_param_config->pwlong_en);
    if (ret != AM_OK) {
        return ret;
    }

    /* PCFG0 寄存器配置 */
    ret = __tmd2635_pgain_pcfg0(p_this, p_param_config->pgain_val);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2635_ppulse_pcfg0(p_this, p_param_config->pulse);
    if (ret != AM_OK) {
        return ret;
    }

    /* PCFG1 寄存器配置 */
    ret = __tmd2635_ppulse_len_pcfg1(p_this, p_param_config->pulse_len);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2635_pldrive_pcfg1(p_this, p_param_config->pldrive_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* PWTIME 寄存器配置 */
    ret = __tmd2635_pwtime(p_this, p_param_config->pwtime_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG8 寄存器配置 */
    ret = __tmd2635_pdselect_cfg8(p_this, p_param_config->pdselect);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG3 寄存器配置 */
    __tmd2635_status_clr_all(p_this);      /* 清除STATUS寄存器 */

    reg = 0x00;
    reg = __TMD2635_SAI_SET(reg, p_param_config->sleep_after_interrupt);
    ret = __tmd2635_write(p_this, __TMD2635_REG_CFG3, &reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    /* CFG6 寄存器配置 */
    ret = __tmd2635_apc_cfg6(p_this, p_param_config->apc_disable);
    if (ret != AM_OK) {
        return ret;
    }

    /* PFILTER 寄存器配置 */
    ret = __tmd2635_pmavg_pfilter(p_this, p_param_config->pmavg_val);
    if (ret != AM_OK) {
        return ret;
    }

    /* CALIBCFG 寄存器配置 */
    ret = __tmd2635_prox_avg_calibcfg(p_this, p_param_config->prox_avg);
    if (ret != AM_OK) {
        return ret;
    }

    /* TEST9 寄存器配置 */
    reg = 0x0D;
    ret = __tmd2635_write(p_this, __TMD2635_REG_TEST9, &reg, 1);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

#if __TMD2635_DEBUG
const char *__tmd2635_reg_name_string[] = {
"ENABLE   ",
"PRATE    ",
"PILTL    ",
"PILTH    ",
"PIHTL    ",
"PIHTH    ",
"PERS     ",
"CFG0     ",
"PCFG0    ",
"PCFG1    ",
"REVID    ",
"ID       ",
"STATUS   ",
"PDATAL   ",
"PDATAH   ",
"REVID2   ",
"SOFTRST  ",
"PWTIME   ",
"CFG8     ",
"CFG3     ",
"CFG6     ",
"PFILTER  ",
"POFFSETL ",
"POFFSETH ",
"CALIB    ",
"CALIBCFG ",
"CALIBSTAT",
"INTENAB  ",
"FAC_L    ",
"FAC_H    ",
"TEST9    ",
};

am_local void __tmd2635_regval_check(am_sensor_tmd2635_dev_t *p_this)
{
    uint8_t reg_val[31]         = {0};         /* 缓存寄存器值 */
    uint8_t counter = 0;

    __tmd2635_read(p_this, __TMD2635_REG_ENABLE, &reg_val[0], 1);
    counter = 0;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);

    __tmd2635_read(p_this, __TMD2635_REG_PRATE, &reg_val[1], 1);
    counter = 1;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);

    __tmd2635_read(p_this, __TMD2635_REG_PILTL, &reg_val[2], 8);
    for (counter = 2; counter < 10;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2635_read(p_this, __TMD2635_REG_REVID, &reg_val[10], 2);
    for (counter = 10; counter < 12;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2635_read(p_this, __TMD2635_REG_STATUS, &reg_val[12], 3);
    for (counter = 12; counter < 15;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2635_read(p_this, __TMD2635_REG_REVID2, &reg_val[15], 1);
    counter = 15;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);

    __tmd2635_read(p_this, __TMD2635_REG_SOFTRST, &reg_val[16], 4);
    for (counter = 16; counter < 20;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2635_read(p_this, __TMD2635_REG_CFG6, &reg_val[20], 1);
    counter = 20;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);

    __tmd2635_read(p_this, __TMD2635_REG_PFILTER, &reg_val[21], 1);
    counter = 21;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);

    __tmd2635_read(p_this, __TMD2635_REG_POFFSETL, &reg_val[22], 2);
    for (counter = 22; counter < 24;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2635_read(p_this, __TMD2635_REG_CALIB, &reg_val[24], 1);
    counter = 24;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);

    __tmd2635_read(p_this, __TMD2635_REG_CALIBCFG, &reg_val[25], 1);
    counter = 25;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);

    __tmd2635_read(p_this, __TMD2635_REG_CALIBSTAT, &reg_val[26], 2);
    for (counter = 26; counter < 28;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2635_read(p_this, __TMD2635_REG_FAC_L, &reg_val[28], 2);
    for (counter = 28; counter < 30;counter++) {
        am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
    }

    __tmd2635_read(p_this, __TMD2635_REG_TEST9, &reg_val[30], 1);
    counter = 30;
    am_kprintf("%s : 0x%02x \r\n",__tmd2635_reg_name_string[counter], reg_val[counter]);
}
#endif

/** \brief TMD2635 校准 */
am_local am_err_t __tmd2635_open_cal(am_sensor_tmd2635_dev_t    *p_this)
{
    am_err_t ret    = AM_OK;

    tmd2635_param_config_t *p_param_config = (tmd2635_param_config_t *)p_this->dev_info->p_param_default;

    /* enable 寄存器配置禁能 */
    ret = __tmd2635_func_disen_enable(p_this,
                                     (AM_TMD2635_PWEN | \
                                      AM_TMD2635_PEN | \
                                      AM_TMD2635_PON ));
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器配置使能 */
    ret = __tmd2635_func_en_enable(p_this,
                                   AM_TMD2635_PON);
    if (ret != AM_OK) {
        return ret;
    }

    /* 清中断标志位 */
    __tmd2635_status_clr_all(p_this);

    /* INTENAB 寄存器配置 */

    /* 校准中断使能 */
    ret = __tmd2635_interrupt_enable_intenab(p_this, AM_TMD2635_CIEN);
    if (ret != AM_OK) {
        return ret;
    }

    /* CALIBCFG 寄存器配置 */
    ret = __tmd2635_binsrch_target_calibcfg(p_this, p_param_config->binsrch_target);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2635_auto_offset_adj_calibcfg(p_this, p_param_config->auto_offset_adj);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __tmd2635_prox_cal_type_sel_calib(p_this, p_param_config->prox_cal_type);
    if (ret != AM_OK) {
        return ret;
    }

    /* CALIB 寄存器配置 */

    /* 开启偏移校准 */
    ret = __tmd2635_start_offset_cal_calib(p_this, AM_TRUE);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

am_local am_err_t __tmd2635_close_cal(am_sensor_tmd2635_dev_t   *p_this)
{
    am_err_t ret    = AM_OK;

    /* 清除STATUS */
    __tmd2635_status_clr_all(p_this);

    /* 校准中断禁能 */
    ret = __tmd2635_interrupt_disable_intenab(p_this, AM_TMD2635_CIEN);
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器配置禁能 */
    ret = __tmd2635_func_disen_enable(p_this,
                                        AM_TMD2635_PON);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

am_local am_err_t __tmd2635_calibration(am_sensor_tmd2635_dev_t *p_this)
{
    am_err_t ret    = AM_OK;

    uint8_t reg             = 0x00;         /* 缓存寄存器值 */
    uint8_t offset_data[2]  = {0};          /* 缓存偏移寄存器值 */
    uint32_t cal_timeout    = 0xffff;       /* 校准超时计数 */

    /* 开启校准 */
    ret = __tmd2635_open_cal(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    while(1) {

        /* 等待校准数据生成完毕 */
        __tmd2635_read(p_this, __TMD2635_REG_CALIBSTAT, &reg, 1);

        if (__TMD2635_CALIB_FINISHED_GET(reg) == 0x01) {     /* CALIB_FINISHED = 1 */

            am_kprintf("tmd2635_calibration finished!\r\n");

            break;
        }
        if (cal_timeout != 0) {
            cal_timeout--;
        }
        if (!cal_timeout) {

            am_kprintf("tmd2635_calibration timeout!\r\n");

            /* 关闭校准 */
            __tmd2635_close_cal(p_this);

            return AM_ERROR;
        }
    }

    /* 读取POFFSETL、POFFSETH */
    am_mdelay(100);
    __tmd2635_read(p_this, __TMD2635_REG_POFFSETL, offset_data, 2);
#if __TMD2635_DEBUG
    am_kprintf("offset_data[0] : 0x%02x \r\n", offset_data[0]);
    am_kprintf("offset_data[1] : 0x%02x \r\n", offset_data[1]);
#endif
    __tmd2635_offset_cal(p_this, offset_data);

    /* 关闭校准 */
    ret = __tmd2635_close_cal(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/**
 * \brief TMD2635 获取结果
 */
am_local am_err_t __tmd2635_result_get(am_sensor_tmd2635_dev_t  *p_this,
                                         tmd2635_result_info_t    *p_result)
{
    am_err_t ret = AM_OK;

    uint8_t result_buf[2]  = {0};         /* 缓存寄存器值 */

    __tmd2635_read(p_this, __TMD2635_REG_PDATAL, result_buf, 2);

    /* Prox AD值 */
    if (p_this->dev_info->p_param_default->apc_disable == AM_TMD2635_APC_ENABLE) {
        p_result->raw_prox_value = __TMD2635_UINT8_TO_UINT16(&result_buf[0]);
    } else {
        p_result->raw_prox_value = (uint16_t)((result_buf[0] << 2) | (result_buf[1] & 0x03));
    }

    /* prox 偏移值 */
    p_result->prox_offset_adjust_value = p_this->cal_val[0].prox_offset_adj;

    /* Prox */
    p_result->prox_value = p_result->raw_prox_value;

    return ret;
}

/*******************************************************************************/

/** \breif 中断回调函数 */
am_local void __tmd2635_alarm_callback (void *p_arg)
{
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_arg;

    tmd2635_result_info_t result;

    /* 获取数据 */
    __tmd2635_result_get(p_this, &result);

    /* 获取数据后，清中断标志位 */
    __tmd2635_status_clr_all(p_this);

    /* 获取Prox */
    p_this->data[0].val = result.prox_value;
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

    if (id == AM_TMD2635_CHAN_1) {
        return AM_SENSOR_TYPE_PROXIMITY;
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
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    tmd2635_result_info_t result;

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
        ret = __tmd2635_result_get(p_this, &result);
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

            p_buf[i].val  = result.prox_value;    /*< \brief Prox */
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
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_drv;

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
    ret = __tmd2635_param_init(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* 校准 */
    ret = __tmd2635_calibration(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* enable 寄存器配置使能 */
    ret = __tmd2635_func_en_enable(p_this,
                                    (AM_TMD2635_PWEN | \
                                     AM_TMD2635_PEN | \
                                     AM_TMD2635_PON ));
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
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_drv;

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
        ret = __tmd2635_func_disen_enable(p_this,
                                           (AM_TMD2635_PWEN | \
                                            AM_TMD2635_PEN | \
                                            AM_TMD2635_PON ));
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
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_drv;

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
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_drv;

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
    __tmd2635_status_clr_all(p_this);

    /* 使能中断 */
    __tmd2635_interrupt_enable_intenab(p_this,
                                       AM_TMD2635_PIEN);    /* Prox中断 */

    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->trigger, 0);
        AM_BIT_SET(p_this->trigger, 1);
    }

    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_tmd2635_dev_t* p_this = (am_sensor_tmd2635_dev_t*)p_drv;

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
            __tmd2635_interrupt_disable_intenab(p_this,
                                                AM_TMD2635_PIEN);    /* Prox中断 */
            AM_BIT_CLR(p_this->trigger, 1);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 TMD2635 初始化
 */
am_sensor_handle_t am_sensor_tmd2635_init (
        am_sensor_tmd2635_dev_t           *p_dev,
        const am_sensor_tmd2635_devinfo_t *p_devinfo,
        am_i2c_handle_t                     handle)
{
    am_err_t ret = AM_OK;

    uint8_t tmd2635_id = 0;
    uint8_t reg = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->tmd2635_dev.p_drv   = p_dev;
    p_dev->tmd2635_dev.p_funcs = &__g_sensor_tmd2635_funcs;
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
                                __tmd2635_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);  /* 下降沿触发 */
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    /* TMD2635软件复位 */
    __tmd2635_softrst(p_dev);
    am_mdelay(10);

    /* 等待复位完成 */
    do {
        __tmd2635_read(p_dev, __TMD2635_REG_SOFTRST, &reg, 1);

    } while (__TMD2635_SOFTRST_GET(reg) == 0x1);

    ret = __tmd2635_read_id(p_dev, &tmd2635_id);
    if ((ret != AM_OK) || (tmd2635_id != __TMD2635_ID)) {
    #if __TMD2635_DEBUG
        am_kprintf("tmd2635_id = 0x%x \r\n", tmd2635_id);
    #endif
        am_kprintf("\r\nSensor TMD2635 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->tmd2635_dev);
}

/**
 * \brief 传感器 TMD2635 去初始化
 */
am_err_t am_sensor_tmd2635_deinit (am_sensor_handle_t handle)
{
    am_sensor_tmd2635_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->tmd2635_dev.p_drv   = NULL;
    p_dev->tmd2635_dev.p_funcs = NULL;
    p_dev->dev_info           = NULL;

    return AM_OK;
}

/**
 * \brief 传感器 TMD2635 控制函数
 */
am_err_t am_sensor_tmd2635_ioctl (am_sensor_handle_t  handle,
                                    int                 cmd,
                                    void               *p_arg)
{
    am_sensor_tmd2635_dev_t   *p_dev = (am_sensor_tmd2635_dev_t *)handle->p_drv;

    am_err_t ret = AM_OK;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    switch (cmd) {

    /* 设置功能使能 */
    case AM_SENSOR_TMD2635_FUNC_ENABLE:
        ret = __tmd2635_func_en_enable(p_dev,
                                      (uint8_t)(int)p_arg);
        break;

    /* 设置功能禁能 */
    case AM_SENSOR_TMD2635_FUNC_DISABLE:
        ret = __tmd2635_func_disen_enable(p_dev,
                                         (uint8_t)(int)p_arg);
        break;

    /* 设置PRATE */
    case AM_SENSOR_TMD2635_PRATE_SET:
        ret = __tmd2635_prate(p_dev,
                             (uint32_t)(int)p_arg);
        break;

    /* 设置Prox低阈值 */
    case AM_SENSOR_TMD2635_LOW_PROX_THRESHOLD_SET:
        if (p_dev->dev_info->p_param_default->apc_disable == AM_TMD2635_APC_ENABLE) {
            ret = __tmd2635_low_prox_threshold_set(p_dev,
                                                    (uint16_t)(int)p_arg);
        } else {
            ret = __tmd2635_low_prox_threshold_set(p_dev,
                                                    (uint16_t)(((int)p_arg) >> 2));
        }
        break;

    /* 设置Prox高阈值 */
    case AM_SENSOR_TMD2635_HIGH_PROX_THRESHOLD_SET:
        if (p_dev->dev_info->p_param_default->apc_disable == AM_TMD2635_APC_ENABLE) {
            ret = __tmd2635_high_prox_threshold_set(p_dev,
                                                     (uint16_t)(int)p_arg);
        } else {
            ret = __tmd2635_high_prox_threshold_set(p_dev,
                                                    (uint16_t)(((int)p_arg) >> 2));
        }
        break;

    /* 获取Prox低阈值 */
    case AM_SENSOR_TMD2635_LOW_PROX_THRESHOLD_GET:
        ret = __tmd2635_low_prox_threshold_get(p_dev,
                                                (uint16_t *)p_arg);
        break;

    /* 获取Prox高阈值 */
    case AM_SENSOR_TMD2635_HIGH_PROX_THRESHOLD_GET:
        ret = __tmd2635_high_prox_threshold_get(p_dev,
                                                 (uint16_t *)p_arg);
        break;

    /* 设置PPERS */
    case AM_SENSOR_TMD2635_PPERS_SET:
        ret = __tmd2635_ppers_pers(p_dev,
                                  (uint8_t)(int)p_arg);
        break;

    /* 设置PWLONG(wait long) */
    case AM_SENSOR_TMD2635_PWLONG_SET:
        ret = __tmd2635_pwlong_cfg0(p_dev,
                                    (am_bool_t)(int)p_arg);
        break;

    /* 设置Prox IR传感器增益 */
    case AM_SENSOR_TMD2635_PGAIN_SET:
        ret = __tmd2635_pgain_pcfg0(p_dev,
                                   (uint8_t)(int)p_arg);
        break;

    /* 设置Prox周期脉冲的最大数量 */
    case AM_SENSOR_TMD2635_PPULSE_SET:
        ret = __tmd2635_ppulse_pcfg0(p_dev,
                                    (uint8_t)(int)p_arg);
        break;

    /* 设置PPULSE_LEN */
    case AM_SENSOR_TMD2635_PPULSE_LEN_SET:
        ret = __tmd2635_ppulse_len_pcfg1(p_dev,
                                        (uint8_t)(int)p_arg);
        break;

    /* 设置Prox IR VCSEL电流驱动能力 */
    case AM_SENSOR_TMD2635_PLDRIVE_SET:
        ret = __tmd2635_pldrive_pcfg1(p_dev,
                                     (uint8_t)(int)p_arg);
        break;

    /* 读取REVID */
    case AM_SENSOR_TMD2635_REVID_GET:
        ret = __tmd2635_read_revid(p_dev, (uint8_t *)p_arg);
        break;

    /* 读取ID */
    case AM_SENSOR_TMD2635_ID_GET:
        ret = __tmd2635_read_id(p_dev, (uint8_t *)p_arg);
        break;

    /* STATUS寄存器位清除 */
    case AM_SENSOR_TMD2635_STATUS_FLAG_CLR:
        ret = __tmd2635_status_flag_clr(p_dev,
                                       (uint8_t)(int)p_arg);
        break;

    /* 读取REVID2 */
    case AM_SENSOR_TMD2635_REVID2_GET:
        ret = __tmd2635_read_revid2(p_dev, (uint8_t *)p_arg);
        break;

    /* 软件复位 */
    case AM_SENSOR_TMD2635_SOFTRST:
        ret = __tmd2635_softrst(p_dev);
        break;

    /* 设置PWTIME */
    case AM_SENSOR_TMD2635_PWTIME_SET:
        ret = __tmd2635_pwtime(p_dev,
                              (uint8_t)(int)p_arg);
        break;

    /* 设置PDSELECT */
    case AM_SENSOR_TMD2635_PDSELECT_SET:
        ret = __tmd2635_pdselect_cfg8(p_dev,
                                     (uint8_t)(int)p_arg);
        break;

    /* 设置APC */
    case AM_SENSOR_TMD2635_APC_SET:
        ret = __tmd2635_apc_cfg6(p_dev,
                                (uint8_t)(int)p_arg);
        break;

    /* 设置PMAVG */
    case AM_SENSOR_TMD2635_PMAVG_SET:
        ret = __tmd2635_pmavg_pfilter(p_dev,
                                     (uint8_t)(int)p_arg);
        break;

    /* 设置CALAVG */
    case AM_SENSOR_TMD2635_CALAVG_SET:
        ret = __tmd2635_calavg_calib(p_dev,
                                    (uint8_t)(int)p_arg);
        break;

    /* 选择Prox校准类型 */
    case AM_SENSOR_TMD2635_PROX_CAL_TYPE_SEL:
        ret = __tmd2635_prox_cal_type_sel_calib(p_dev,
                                               (uint8_t)(int)p_arg);
        break;

    /* 设置CALPRATE */
    case AM_SENSOR_TMD2635_CALPRATE_SET:
        ret = __tmd2635_calprate_calib(p_dev,
                                      (uint8_t)(int)p_arg);
        break;

    /* 设置偏移校准 */
    case AM_SENSOR_TMD2635_START_OFFSET_CAL_SET:
        ret = __tmd2635_start_offset_cal_calib(p_dev,
                                              (am_bool_t)(int)p_arg);
        break;

    /* 设置BINSRCH_TARGET */
    case AM_SENSOR_TMD2635_BINSRCH_TARGET_SET:
        ret = __tmd2635_binsrch_target_calibcfg(p_dev,
                                               (uint8_t)(int)p_arg);
        break;

    /* 设置AUTO_OFFSET_ADJ */
    case AM_SENSOR_TMD2635_AUTO_OFFSET_ADJ_SET:
        ret = __tmd2635_auto_offset_adj_calibcfg(p_dev,
                                                (am_bool_t)(int)p_arg);
        break;

    /* 设置PROX_AVG */
    case AM_SENSOR_TMD2635_PROX_AVG_SET:
        ret = __tmd2635_prox_avg_calibcfg(p_dev,
                                         (uint8_t)(int)p_arg);
        break;

    /* 中断使能 */
    case AM_SENSOR_TMD2635_INT_ENABLE:
        ret = __tmd2635_interrupt_enable_intenab(p_dev,
                                                (uint8_t)(int)p_arg);
        break;

    /* 中断禁能 */
    case AM_SENSOR_TMD2635_INT_DISABLE:
        ret = __tmd2635_interrupt_disable_intenab(p_dev,
                                                 (uint8_t)(int)p_arg);
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/* end of file */

