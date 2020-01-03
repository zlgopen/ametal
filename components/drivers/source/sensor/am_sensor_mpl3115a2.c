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
 * \brief 传感器 MPL3115A2 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-11  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_mpl3115a2.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "math.h"
#include "am_delay.h"

/*******************************************************************************
 * defines
 ******************************************************************************/
#define __MPL3115A2_REG_STATUS              0x00  /**< \brief 传感器状态寄存器      */
#define __MPL3115A2_REG_OUT_P_MSB           0x01  /**< \brief 气压数据高八位        */
#define __MPL3115A2_REG_OUT_P_CSB           0x02  /**< \brief 气压数据中八位        */
#define __MPL3115A2_REG_OUT_P_LSB           0x03  /**< \brief 气压数据低八位        */
#define __MPL3115A2_REG_OUT_T_MSB           0x04  /**< \brief 温度数据高八位        */
#define __MPL3115A2_REG_OUT_T_LSB           0x05  /**< \brief 温度数据低八位        */
#define __MPL3115A2_REG_DR_STATUS           0x06  /**< \brief 状态寄存器            */
#define __MPL3115A2_REG_OUT_P_DELTA_MSB     0x07  /**< \brief 气压增量数据高八位    */
#define __MPL3115A2_REG_OUT_P_DELTA_CSB     0x08  /**< \brief 气压增量数据中八位    */
#define __MPL3115A2_REG_OUT_P_DELTA_LSB     0x09  /**< \brief 气压增量数据低八位    */
#define __MPL3115A2_REG_OUT_T_DELTA_MSB     0x0A  /**< \brief 温度增量数据高八位    */
#define __MPL3115A2_REG_OUT_T_DELTA_LSB     0x0B  /**< \brief 温度增量数据低八位    */
#define __MPL3115A2_REG_WHO_AM_I            0x0C  /**< \brief 器件辨别寄存器（读ID）*/
#define __MPL3115A2_REG_F_STATUS            0x0D  /**< \brief FIFO状态寄存器        */
#define __MPL3115A2_REG_F_DATA              0x0E  /**< \brief FIFO8位数据通道       */
#define __MPL3115A2_REG_F_SETUP             0x0F  /**< \brief FIFO配置寄存器        */
#define __MPL3115A2_REG_TIME_DLY            0x10  /**< \brief 时间延迟寄存器        */
#define __MPL3115A2_REG_SYSMOD              0x11  /**< \brief 系统模式寄存器        */
#define __MPL3115A2_REG_INT_SOURCE          0x12  /**< \brief 中断源寄存器          */
#define __MPL3115A2_REG_PT_DATA_CFG         0x13  /**< \brief PT数据配置寄存器      */
#define __MPL3115A2_REG_BAR_IN_MSB          0x14  /**< \brief 海拔计算气压输入高八位*/
#define __MPL3115A2_REG_BAR_IN_LSB          0x15  /**< \brief 海拔计算气压输入低八位*/
#define __MPL3115A2_REG_P_TGT_MSB           0x16  /**< \brief 目标气压高八位        */
#define __MPL3115A2_REG_P_TGT_LSB           0x17  /**< \brief 目标气压低八位        */
#define __MPL3115A2_REG_T_TGT               0x18  /**< \brief 目标温度寄存器        */
#define __MPL3115A2_REG_P_WND_MSB           0x19  /**< \brief 气压/海拔窗口高八位   */
#define __MPL3115A2_REG_P_WND_LSB           0x1A  /**< \brief 气压/海拔窗口低八位   */
#define __MPL3115A2_REG_T_WND               0x1B  /**< \brief 温度窗口寄存器        */
#define __MPL3115A2_REG_P_MIN_MSB           0x1C  /**< \brief 最小气压数据高八位    */
#define __MPL3115A2_REG_P_MIN_CSB           0x1D  /**< \brief 最小气压数据中八位    */
#define __MPL3115A2_REG_P_MIN_LSB           0x1E  /**< \brief 最小气压数据低八位    */
#define __MPL3115A2_REG_T_MIN_MSB           0x1F  /**< \brief 最小温度数据高八位    */
#define __MPL3115A2_REG_T_MIN_LSB           0x20  /**< \brief 最小温度数据低八位    */
#define __MPL3115A2_REG_P_MAX_MSB           0x21  /**< \brief 最大气压数据高八位    */
#define __MPL3115A2_REG_P_MAX_CSB           0x22  /**< \brief 最大气压数据中八位    */
#define __MPL3115A2_REG_P_MAX_LSB           0x23  /**< \brief 最大气压数据低八位    */
#define __MPL3115A2_REG_T_MAX_MSB           0x24  /**< \brief 最高温度数据高八位    */
#define __MPL3115A2_REG_T_MAX_LSB           0x25  /**< \brief 最高温度数据低八位    */
#define __MPL3115A2_REG_CTRL_REG1           0x26  /**< \brief 控制寄存器1           */
#define __MPL3115A2_REG_CTRL_REG2           0x27  /**< \brief 控制寄存器2           */
#define __MPL3115A2_REG_CTRL_REG3           0x28  /**< \brief 控制寄存器3           */
#define __MPL3115A2_REG_CTRL_REG4           0x29  /**< \brief 控制寄存器4           */
#define __MPL3115A2_REG_CTRL_REG5           0x2A  /**< \brief 控制寄存器5           */
#define __MPL3115A2_REG_OFF_P               0x2B  /**< \brief 气压数据偏移寄存器    */
#define __MPL3115A2_REG_OFF_T               0x2C  /**< \brief 温度数据偏移寄存器    */
#define __MPL3115A2_REG_OFF_H               0x2D  /**< \brief 海拔数据偏移寄存器    */

#define __MPL3115A2_MY_ID                   0xC4    /**< \brief ID值                */
#define __MPL3115A2_CMD_RESET               0x04    /**< \brief 复位命令            */

/**
 * \brief 温度报警值
          目标阈值中断：__MPL3115A2_T_WND_VALUE = 0；窗口中断 ！= 0
          目标阈值中断跨过目标阈值即产生一次中断，窗口中断在窗口范围内重复中断
 */
#define __MPL3115A2_T_TGT_VALUE             27      /* 温度报警目标值  */
#define __MPL3115A2_T_WND_VALUE             2       /* 温度报警窗口值  */

/**
 * \brief MPL3115中断引脚及配置
 */
#define __MPL3115A2_INT1                    1   /* 中断INT1                  */
#define __MPL3115A2_INT2                    0   /* 中断INT2                  */

#define __MPL3115A2_OD_MODE                 1   /* 引脚INT，1：开漏模式       */
#define __MPL3115A2_PP_MODE                 0   /* 引脚INT，0：推挽模式（内部上拉） */

#define __MPL3115A2_IPOL_H                  1   /* 引脚INT，1：中断高电平有效  */
#define __MPL3115A2_IPOL_L                  0   /* 引脚INT，0：中断低电平有效  */

/**
 * \brief MPL3115中断事件标识与中断使能禁能命令 
 */
#define __MPL3115A2_INT_CFG_DRDY            7   /* 数据就绪中断 */
#define __MPL3115A2_INT_CFG_FIFO            6   /* FIFO中断    */
#define __MPL3115A2_INT_CFG_PW              5   /* 气压窗口中断 */
#define __MPL3115A2_INT_CFG_TW              4   /* 温度窗口中断 */
#define __MPL3115A2_INT_CFG_PTH             3   /* 气压阈值中断 */
#define __MPL3115A2_INT_CFG_TTH             2   /* 温度阈值中断 */
#define __MPL3115A2_INT_CFG_PCHG            1   /* 气压变化中断 */
#define __MPL3115A2_INT_CFG_TCHG            0   /* 温度变化中断 */

/** \brief 设置采样时间步长 */
#define __MPL3115A2_RATE_SET(data, mask)        (((data) & (~0xF)) | mask)

/** \brief 设置过采样率 */
#define __MPL3115A2_OS_SET(data, mask)          (((data) & (~(0x7 << 3))) | (mask << 3))

/** \brief 气压计/海拔计模式设置 */
#define __MPL3115A2_MODE_SET(data, mask)        (((data) & (~(0x1 << 7))) | (mask << 7))

/** \brief 中断相关位清零 */
#define __MPL3115A2_INT_CLR_BIT(data, bit)      ((data) & (~(0x1 << bit)))

/** \brief 中断相关位段清零 */
#define __MPL3115A2_INT_CLR_MASK(data, mask)    ((data) & (~(0x3 << mask)))

/** \brief 新气压/海拔数据事件标志使能 */
#define __MPL3115A2_PDEFE                       (0x1 << 1)

/** \brief 新气压/海拔数据事件标志使能 */
#define __MPL3115A2_TDEFE                       (0x1 << 0) 

/** \brief 开始一次数据采集 */
#define __MPL3115A2_ONE_SHOT_START              (0x1)

/** \brief OST位置1 */
#define __MPL3115A2_OST_SET_BIT                 (0x1 << 1)

/** \brief OST位清零 */
#define __MPL3115A2_OST_CLEAR_BIT               (~(0x1 << 1))

/** \brief 获取PRESS状态位 */
#define __MPL3115A2_GET_PRESS_STATUS(reg)       ((reg >> 2) & 0x1)

/** \brief 获取TEM状态位 */
#define __MPL3115A2_GET_TEM_STATUS(reg)         ((reg >> 1) & 0x1)

/** \brief 获取当前模式 */
#define __MPL3115A2_GET_MODE(reg)               ((reg >> 7) & 0x1)

/** \brief 将两个int8转换为一个int16_t类型 */
#define __MPL3115A2_UINT8_TO_UINT16(buff) \
                       (int16_t)(((uint16_t)(buff[0]) << 8) \
                               | ((uint16_t)(buff[1])))

/** \brief 将三个int8转换为一个int32_t类型 */
#define __MPL3115A2_UINT8_TO_UINT32(buff) \
                       (uint32_t)(((uint32_t)(buff[0]) << 16) \
                               | ((uint32_t)(buff[1]) << 8)  \
                               | ((uint32_t)(buff[2])))

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
am_local am_const struct am_sensor_drv_funcs __g_sensor_mpl3115a2_funcs = {
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
 * \brief MPL3115A2 写数据
 */
am_local am_err_t __mpl3115a2_write (am_sensor_mpl3115a2_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief MPL3115A2 读数据
 */
am_local am_err_t __mpl3115a2_read (am_sensor_mpl3115a2_dev_t *p_this,
                                 uint32_t                subaddr,
                                 uint8_t                *p_buf,
                                 uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief 获取海拔值，并扩大10^6倍
 */
am_local int32_t __mpl3115a2_get_altitude_value (uint32_t altitude)
{
    uint8_t pmsb = (altitude >> 16) & 0xff;
    uint8_t pcsb = (altitude >> 8) & 0xff;
    uint8_t plsb = altitude & 0xff;
    uint16_t plsb_dec = 0;
    uint16_t alt_word = 0;

    int32_t alt;

    if (pmsb > 0x7F) {      /* 负海拔补码 */
        am_kprintf("Altitude: Negative \r\n");
        alt_word = (pmsb << 8) | pcsb;
        alt_word = ~alt_word + 1;
    } else {
        am_kprintf("Altitude: Positive \r\n");
        alt_word = (pmsb << 8) | pcsb;
    }
    plsb_dec = (plsb >> 4) * 625;   /* 海拔小数部分 */

    alt = (int32_t)((alt_word * 1000000) + (plsb_dec * 100));

    return alt;
}

/**
 * \brief 获取气压值（单位：KPa），并扩大10^6倍
 */
am_local int32_t __mpl3115a2_get_press_value (uint32_t press)
{
    uint8_t pmsb = (press >> 16) & 0xff;
    uint8_t pcsb = (press >> 8) & 0xff;
    uint8_t plsb = press & 0xff;
    uint16_t plsb_dec = 0;
    uint32_t pre_word = 0;
    uint32_t pre;

    pre_word = ((pmsb << 10) |                    /* 气压整数部分 */
                (pcsb << 2)  | 
                (plsb_dec >> 6)) & 0x3ffff;

    plsb_dec = ((plsb >> 4) & 0x03) * 25;         /* 气压小数部分 */

    /* 原始数据单位为Pa，此处转化为KPa，并扩大10^6倍 */
    pre = (uint32_t)((pre_word * 1000) + (plsb_dec * 10));

    return pre;
}

/**
 * \brief 获取温度值，并扩大10^6倍
 */
am_local int32_t __mpl3115a2_get_tem_value (uint32_t temperature)
{
    int8_t      tmsb = (temperature >> 8) & 0xff;
    uint16_t    tlsb = temperature & 0xff;
    int32_t     temp;

    tlsb = (tlsb >> 4) * 625;
    temp = (int32_t)((tmsb * 1000000) + (tlsb * 100));

    return temp;
}

/** \breif 中断回调函数 */
am_local void __mpl3115a2_alarm_callback (void *p_arg)
{
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_arg;

    uint8_t reg_data[3];
    uint8_t measure_type = 0;
    int32_t tem_data   = 0;

    /** \brief 获取气压数据寄存器值 */
    __mpl3115a2_read(p_this, __MPL3115A2_REG_OUT_P_MSB, reg_data, 3);

    tem_data = __MPL3115A2_UINT8_TO_UINT32(reg_data);

    /** \brief 获取MPL3115A2当前模式 */
    __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG1, &measure_type, 1);
    if (__MPL3115A2_GET_MODE(measure_type)) {
        /** \brief 海拔计模式 */
        p_this->data[0].val = __mpl3115a2_get_altitude_value(tem_data); 
        p_this->data[0].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为-6:10^(-6)*/
    } else {
        /** \brief 气压计模式 */
        p_this->data[0].val = __mpl3115a2_get_press_value(tem_data); 
        p_this->data[0].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为-6:10^(-6)*/
    }

    /** \brief 获取温度 */
    __mpl3115a2_read(p_this, __MPL3115A2_REG_OUT_T_MSB, reg_data, 2);

    tem_data = __MPL3115A2_UINT8_TO_UINT16(reg_data);

    /** \brief 温度 */
    p_this->data[1].val = __mpl3115a2_get_tem_value(tem_data); 
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
am_local am_err_t __reg_attr_set (am_sensor_mpl3115a2_dev_t *p_this, uint8_t rate)
{
    am_err_t ret = AM_OK;

    uint8_t rate_cfg = 0;

    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG2, &rate_cfg, 1);
    if (ret != AM_OK) {
        am_kprintf("read error! \r\n");
        return ret;
    }

    rate_cfg = __MPL3115A2_RATE_SET(rate_cfg, rate);
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG2, &rate_cfg, 1);
    if (ret != AM_OK) {
        return ret;
    }
    return ret;
}

/**
 * \brief 中断引脚设置
 *
 * \param[in] p_this    : 指向MPL3115A2设备的指针
 * \param[in] int_num   : 事件中断信号输出引脚
                          (__MPL3115A2_INT1：中断连接到INT1)
                          (__MPL3115A2_INT2：中断连接到INT2)
 * \param[in] int_pp_od : 中断引脚结构选择控制字   
                          (__MPL3115A2_PP_MODE：中断推挽结构，
                           __MPL3115A2_OD_MODE：中断开漏结构)
 * \param[in] int_ipol  : 中断引脚有效电平控制字    
                          (__MPL3115A2_IPOL_L：中断低电平有效，
                           __MPL3115A2_IPOL_H：中断高电平有效)
 *
 * \return  AM_OK : 中断引脚设置完成
 */
am_local am_err_t __mpl3115a2_pin_int_set (am_sensor_mpl3115a2_dev_t *p_this,
                                           uint8_t int_num,
                                           uint8_t int_pp_od,
                                           uint8_t int_ipol)
{
    am_err_t ret = AM_OK;

    uint8_t ctrl_reg3 = 0;
    
    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG3, &ctrl_reg3, 1);
    if ( ret != AM_OK) {
        return ret;
    }

    switch (int_num) {
    case __MPL3115A2_INT1:
        ctrl_reg3 = ((int_pp_od << 4)| (int_ipol << 5)) | 
                      __MPL3115A2_INT_CLR_MASK(ctrl_reg3, 4);
        break;
    case __MPL3115A2_INT2:
        ctrl_reg3 = ((int_pp_od << 0)| (int_ipol << 1)) | 
                      __MPL3115A2_INT_CLR_MASK(ctrl_reg3, 0);
        break;
    default:
        break;
    }

    return __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG3, &ctrl_reg3, 1);
}

/**
 * \brief 中断连接输出引脚以及使能
 *
 * \param[in] p_this    : 指向MPL3115A2设备的指针
 * \param[in] int_event : 需要配置的中断事件，MPL3115有八种中断事件（如数据就绪事件：INT_CFG_DRDY）
 * \param[in] int_num   : 事件中断信号输出引脚
                          (__MPL3115A2_INT1：中断连接到INT1)
                          (__MPL3115A2_INT2：中断连接到INT2)
 * \param[in] is_enable : 事件中断使能与禁能
                          (AM_TRUE： 事件中断使能)
                          (AM_FALSE：事件中断禁能)
 *
 * \return  AM_OK : 中断相关设置完成
 */
am_local am_err_t __mpl3115a2_event_int_set (am_sensor_mpl3115a2_dev_t *p_this,
                                             uint8_t int_event,
                                             uint8_t int_num,
                                             uint8_t is_enable)
{
    am_err_t ret = AM_OK;

    uint8_t ctrl_reg4 = 0;
    uint8_t ctrl_reg5 = 0;

    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG5, &ctrl_reg5, 1);
    if ( ret != AM_OK) {
        return ret;
    }

    ctrl_reg5 = (int_num << int_event) | __MPL3115A2_INT_CLR_BIT(ctrl_reg5, int_event);
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG5, &ctrl_reg5, 1);
    if ( ret != AM_OK) {
        return ret;
    }

    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG4, &ctrl_reg4, 1);
    if ( ret != AM_OK) {
        return ret;
    }

    ctrl_reg4 = (is_enable << int_event) | __MPL3115A2_INT_CLR_BIT(ctrl_reg4, int_event);
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG4, &ctrl_reg4, 1);
    if ( ret != AM_OK) {
        return ret;
    }

    return ret;
}

/**
 * \brief 设置温度报警目标值与窗口值
 *
 * \param[in] p_this    : 指向MPL3115A2设备的指针
 * \param[in] int_num   : 事件中断信号输出引脚
                          (__MPL3115A2_INT1：中断连接到INT1)
                          (__MPL3115A2_INT2：中断连接到INT2)
 * \param[in] t_tgt_data: 温度报警目标值
 * \param[in] t_wnd_data: 温度报警窗口值
 *
 * \return  AM_OK : 设置温度报警目标值与窗口值完成
 */
am_local am_err_t __mpl3115a2_t_alarm_set (am_sensor_mpl3115a2_dev_t *p_this,
                                             uint8_t int_num,
                                             int8_t t_tgt_data,
                                             uint8_t t_wnd_data)
{
    am_err_t ret = AM_OK;

    /* 设置温度报警目标值 */
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_T_TGT, (uint8_t *)&t_tgt_data, 1);
    if ( ret != AM_OK) {
        return ret;
    }

    /* 设置温度报警窗口值 */
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_T_WND, &t_wnd_data, 1);
    if ( ret != AM_OK) {
        return ret;
    }

    if (t_wnd_data != 0) {
        __mpl3115a2_event_int_set(p_this, __MPL3115A2_INT_CFG_TW, int_num, AM_TRUE);
    } else {
        __mpl3115a2_event_int_set(p_this, __MPL3115A2_INT_CFG_TTH, int_num, AM_TRUE);
    }

    return ret;
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_MPL3115A2_CHAN_1) {
        return AM_SENSOR_TYPE_PRESS;
    } else if (id == AM_MPL3115A2_CHAN_2) {
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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;
    
    am_err_t ret        = AM_OK;
    uint8_t status_val  = 0;
    uint8_t reg_data[3] = {0};
    uint8_t measure_type = 0;
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
        ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_DR_STATUS, &status_val, 1);
        if (ret != AM_OK) {
            return ret;
        }
    } while (__MPL3115A2_GET_PRESS_STATUS(status_val) != 0x1 ||
             __MPL3115A2_GET_TEM_STATUS(status_val) != 0x1);

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            /** \brief 获取气压数据寄存器值 */
            ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_OUT_P_MSB, reg_data, 3);

            tem_data = __MPL3115A2_UINT8_TO_UINT32(reg_data);

            /** \brief 获取MPL3115A2当前模式 */
            __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG1, &measure_type, 1);
            if (__MPL3115A2_GET_MODE(measure_type)) {
                /** \brief 海拔计模式 */
                p_buf[i].val = __mpl3115a2_get_altitude_value(tem_data); 
                p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为-6:10^(-6)*/
            } else {
                /** \brief 气压计模式 */
                p_buf[i].val = __mpl3115a2_get_press_value(tem_data); 
                p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为-6:10^(-6)*/
            }

        } else if (cur_id == 1) {

            /** \brief 获取温度 */
            ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_OUT_T_MSB, reg_data, 2);

            tem_data = __MPL3115A2_UINT8_TO_UINT16(reg_data);
           
            /** \brief 温度 */
            p_buf[i].val = __mpl3115a2_get_tem_value(tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO; /*< \brief 单位默认为-6:10^(-6)*/

        }else {
            return -AM_ENODEV;  /*< \brief 若此次通道不属于该传感器，直接返回 */
        }
    }
    
    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG1, &status_val, 1);
    if (ret != AM_OK) {
        return ret;
    }

    /* OST先清零，再置位，以启动下一个测量 */
    status_val &= __MPL3115A2_OST_CLEAR_BIT;
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG1, &status_val, 1);
    if (ret != AM_OK) {
        return ret;
    }
    status_val |= __MPL3115A2_OST_SET_BIT;
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG1, &status_val, 1);
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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    uint8_t open_one_shot = 0;
    uint8_t ctrl_reg1  = 0;
    uint8_t pt_data_cfg = 0;

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

    /* 首先获取控制寄存器1的值 */
    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1); 
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    /* 配置过采样率值 、模式设置*/
    ctrl_reg1 = __MPL3115A2_OS_SET(ctrl_reg1, __MPL3115A2_OS_VAL);
    ctrl_reg1 = __MPL3115A2_MODE_SET(ctrl_reg1, __MPL3115A2_MODE);
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1); 
    if (ret != AM_OK) {
        curent_ret = ret;
    }

    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_PT_DATA_CFG, &pt_data_cfg, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    /* 气压/海拔 温度数据事件标志使能 */
    pt_data_cfg |= __MPL3115A2_PDEFE;
    pt_data_cfg |= __MPL3115A2_TDEFE;
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_PT_DATA_CFG, &pt_data_cfg, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }

    ret = __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG1, &open_one_shot, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    }
    /* 处于ACTIVE模式，根据ST位的值进行测量 */
    open_one_shot |= __MPL3115A2_ONE_SHOT_START;
    /* 默认轮询状态，OST置1 ，立即启动测量 */
    open_one_shot |= __MPL3115A2_OST_SET_BIT;
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG1, &open_one_shot, 1);
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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;

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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    am_sensor_val_t sensor_val = {0, 0};

    am_sensor_val_t __sampling_rate = {0, 0};

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if ((id != AM_MPL3115A2_CHAN_1) && (id != AM_MPL3115A2_CHAN_2)) {
        return -AM_ENODEV;
    }

    switch (attr) {

    /* 修改频率 */
    case AM_SENSOR_ATTR_SAMPLING_RATE:

        if (p_val->val < 0 || p_val->val > 15) {
            return -AM_ENOTSUP;
        }

        sensor_val.unit = p_val->unit;
        sensor_val.val  = p_val->val;
        am_sensor_val_unit_convert(&sensor_val, 1, AM_SENSOR_UNIT_BASE);

        ret = __reg_attr_set(p_this, sensor_val.val);

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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;

    am_err_t ret = AM_OK;

    if ((id != AM_MPL3115A2_CHAN_1) && (id != AM_MPL3115A2_CHAN_2)) {
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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;

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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t reg_clear[5];
    uint8_t ctrl_reg1 = 0;

    if (id != 0 && id != 1) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x4) {
        return AM_OK;
    }

    /* 中断引脚设置 */
    ret = __mpl3115a2_pin_int_set(p_this, 
                                  __MPL3115A2_INT1,
                                  __MPL3115A2_PP_MODE,
                                  __MPL3115A2_IPOL_L);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* 设置温度报警目标值与窗口值 */
    ret = __mpl3115a2_t_alarm_set(p_this,
                                  __MPL3115A2_INT1,
                                  __MPL3115A2_T_TGT_VALUE,
                                  __MPL3115A2_T_WND_VALUE);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* 中断连接至INT1, 并使能数据就绪中断 */
    ret = __mpl3115a2_event_int_set(p_this, 
                                    __MPL3115A2_INT_CFG_DRDY,
                                    __MPL3115A2_INT1,
                                    AM_TRUE);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* 中断状态，OST位清零 */
    __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1);
    ctrl_reg1 &= __MPL3115A2_OST_CLEAR_BIT;
    ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }

    /* 配置触发引脚 */
    if (p_this->dev_info->trigger_pin != -1) {    
        __mpl3115a2_read(p_this, __MPL3115A2_REG_OUT_P_MSB, reg_clear, 5);
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
    am_sensor_mpl3115a2_dev_t* p_this = (am_sensor_mpl3115a2_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    am_sensor_val_t p_val = {0, 0};

    uint8_t ctrl_reg1  = 0;

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

        /* 禁能数据就绪中断 */
        __mpl3115a2_event_int_set(p_this, __MPL3115A2_INT_CFG_DRDY, __MPL3115A2_INT1, AM_FALSE);

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

        /* 恢复轮询状态，OST置1 */
        __mpl3115a2_read(p_this, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1);
        ctrl_reg1 |= __MPL3115A2_OST_SET_BIT;
        ret = __mpl3115a2_write(p_this, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1);
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
 * \brief 传感器 MPL3115A2 初始化
 */
am_sensor_handle_t am_sensor_mpl3115a2_init (
        am_sensor_mpl3115a2_dev_t           *p_dev,
        const am_sensor_mpl3115a2_devinfo_t *p_devinfo,
        am_i2c_handle_t                      handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t ctrl_reg1  = 0;
    uint8_t mpl3115a2_id = 0;

    am_i2c_handle_t i2c_handle = NULL;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->mpl3115a2_dev.p_drv   = p_dev;
    p_dev->mpl3115a2_dev.p_funcs = &__g_sensor_mpl3115a2_funcs;
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
                                __mpl3115a2_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL); /* 下降沿触发 */
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);

    /* 检测ID */
    ret = __mpl3115a2_read(p_dev, __MPL3115A2_REG_WHO_AM_I, &mpl3115a2_id, 1);
    if (ret != AM_OK || mpl3115a2_id != __MPL3115A2_MY_ID) {
        cur_ret = ret;
        am_kprintf("Device ID:%x \r\n", mpl3115a2_id);
    }

    /* 首先获取控制寄存器1的值 */
    ret = __mpl3115a2_read(p_dev, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1);
    if (ret != AM_OK) {
        cur_ret = ret;
    }
    /* 复位MPL3115A2 */
    ctrl_reg1 |= __MPL3115A2_CMD_RESET; 
    ret = __mpl3115a2_write(p_dev, __MPL3115A2_REG_CTRL_REG1, &ctrl_reg1, 1);
    am_mdelay(10);
    if (ret != AM_OK) {
        cur_ret = ret;
    }

    /* 重置I2C */
    p_dev->dev_info->pfn_i2c_inst_deinit(handle); /* 先释放I2C资源 */

    i2c_handle = p_dev->dev_info->pfn_i2c_inst_init(); /* 重新初始化I2C */

    am_i2c_mkdev(&p_dev->i2c_dev,
                 i2c_handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);


    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor MPL3115A2 Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->mpl3115a2_dev);
}

/**
 * \brief 传感器 MPL3115A2 解初始化
 */
am_err_t am_sensor_mpl3115a2_deinit (am_sensor_handle_t handle)
{
    am_sensor_mpl3115a2_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->mpl3115a2_dev.p_drv   = NULL;
    p_dev->mpl3115a2_dev.p_funcs = NULL;
    p_dev->dev_info              = NULL;

    return AM_OK;
}



/* end of file */
