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
 * \brief RX8025SA软件包实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-07  vir, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_rx8025sa.h"
#include "am_gpio.h"
/*****************************************************************************
 * 宏定义
 ****************************************************************************/
 
 /** \brief RX8025SA设备地址  (七位)*/
#define __RX8025SA_ADDR               0x32

#define __RX8025SA_12H_FORMAT  0   /** \brief 12小时制 */
#define __RX8025SA_24H_FORMAT  1   /** \brief 24小时制 */
/**
 * \name RX8025SA 寄存器地址 + 传送模式 （ 默认为0）
 *
 * \note 高四位为：寄存器地址
 *       低四位为：传送模式        0h 标准的read模式
 *                      4h 缩短顺序read模式 （不推荐）
 */
 
#define __RX8025SA_REG_SECONDS          0x00    /**< \brief 秒寄存器地址 + 标准模式 */
#define __RX8025SA_REG_MINUTES          0x10    /**< \brief 分寄存器地址 + 标准模式 */
#define __RX8025SA_REG_HOURS            0x20    /**< \brief 时寄存器地址 + 标准模式 */
#define __RX8025SA_REG_WEEKDAYS         0x30    /**< \brief 星期寄存器地址 + 标准模式 */
#define __RX8025SA_REG_DAYS             0x40    /**< \brief 日寄存器地址 + 标准模式 */
#define __RX8025SA_REG_MONTHS           0x50    /**< \brief 月寄存器地址 + 标准模式 */
#define __RX8025SA_REG_YEAERS           0x60    /**< \brief 年寄存器地址 + 标准模式 */

#define __RX8025SA_REG_DIGITAL_OFFSET   0x70    /**< \brief 时钟偏移寄存器地址 + 标准模式 */

#define __RX8025SA_REG_MINUTES_ALARM_W  0x80    /**< \brief 闹钟W分钟寄存器地址 + 标准模式 */
#define __RX8025SA_REG_HOURS_ALARM_W    0x90    /**< \brief 闹钟W小时寄存器地址  + 标准模式*/
#define __RX8025SA_REG_WEEKDAYS_ALARM_W 0xa0    /**< \brief 闹钟W星期寄存器地址 + 标准模式*/

#define __RX8025SA_REG_MINUTES_ALARM_D  0xb0    /**< \brief 闹钟D分钟寄存器地址 + 标准模式 */
#define __RX8025SA_REG_HOURS_ALARM_D    0xc0    /**< \brief 闹钟D小时寄存器地址  + 标准模式*/
 /* 0x0d is reserved */
#define __RX8025SA_REG_CONTROL1         0xe0    /**< \brief 控制状态寄存器1地址 + 标准模式 */
#define __RX8025SA_REG_CONTROL2         0xf0    /**< \brief 控制状态寄存器2地址 + 标准模式 */

/**
 * \name RX8025SA 寄存器位域描述
 * \anchor grp_rx8025sa_bitfield_descriptions
 * @{
 */


#define __RX8025SA_C1_ALARM_W_EN   0x80   /**< \brief 使能ALARM_W闹钟 */
#define __RX8025SA_C1_ALARM_D_EN   0x40   /**< \brief 使能ALARM_D闹钟 */

#define __RX8025SA_C2_DAFG_ALARM_D        (uint8_t)(1<<0)    /**< \brief DAFG位*/
#define __RX8025SA_C2_WAFG_ALARM_W        (uint8_t)(1<<1)    /**< \brief WAFG位*/
#define __RX8025SA_C2_CTFG_INT            (uint8_t)(1<<2)    /**< \brief CTFG位*/

#define __RX8025SA_C1_24H_FORMAT   0x20  /**< \brief 24小时制 */

#define __RX8025SA_C2_VDSL_1_3_SET   0x80  /**< \brief 低电压检测功能的标准电压值设定为1.3v*/



/**
 * \name RX8025SA I2C当前状态
 * \anchor grp_rx8025sa_i2c_operation
 * @{
 */

#define __RX8025SA_I2C_RD_STATUS        0x00    /**< \brief 读取状态 */
#define __RX8025SA_I2C_WR_STATUS        0x01    /**< \brief 清除状态 */
#define __RX8025SA_I2C_DATA_PROCESSING  0x02    /**< \brief 数据处理阶段 */

/* RTC driver function implementation */
static int __rx8025sa_time_set (void *p_drv, am_tm_t *p_tm);
static int __rx8025sa_time_get (void *p_drv, am_tm_t *p_tm);

/**
 * \brief RTC driver functions definition
 */
static struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
    __rx8025sa_time_set,
    __rx8025sa_time_get,
};

/* 函数声明 */
static void __rx8025sa_intb_isr (void *p_arg);

/* ALARM_CLK driver function implementation */
static int __rx8025sa_alarm_clk_time_set (void *p_drv, am_alarm_clk_tm_t *p_tm);
static int __rx8025sa_alarm_clk_callback_set (void          *p_drv,
                                              am_pfnvoid_t   pfn_alarm_callback,
                                              void          *p_arg);
static int __rx8025sa_alarm_clk_on (void *p_drv);
static int __rx8025sa_alarm_clk_off (void *p_drv);

/**
 * \brief ALARM_CLK driver functions definition
 */
static struct am_alarm_clk_drv_funcs __g_alarm_clk_drv_funcs = {
    __rx8025sa_alarm_clk_time_set,
    __rx8025sa_alarm_clk_callback_set,
    __rx8025sa_alarm_clk_on,
    __rx8025sa_alarm_clk_off,
};
/**
 * \brief 闰年和平年每个月对应的天数
 */
static const uint8_t __mdays[2][12] = {
    /* 平年 */
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    /* 闰年 */
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/**
 * \brief 判断年份是否为闰年
 *
 * \param[in] year : 年份
 *
 * \retval 1 : 闰年
 * \retval 0 : 平年
 */
static uint8_t __rx8025sa_leap_year_check (uint16_t year)
{
    return (((!(year % 4) && (year % 100)) || !(year % 400)) ? 1 : 0);
}

/**
 * \brief 计算这是一年中的第几天
 *
 * \param[in] year  : 年份
 * \param[in] month : 月份
 * \param[in] day   : 天
 *
 * \retval 1 : 闰年
 * \retval 0 : 平年
 */
static uint8_t __rx8025sa_day_of_year (uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t i;

    if (__rx8025sa_leap_year_check(year)) {
        for (i = 0; i < month; i++) {
            day += __mdays[1][i];
        }
    } else {
        for (i = 0; i < month; i++) {
            day += __mdays[0][i];
        }
    }

    return (day);
}
/**
 * \brief 检验时间信息的有效性
 *
 * \param[in] p_tm : 指向时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
int __rx8025sa_time_validator (am_tm_t *p_tm)
{
    if (p_tm->tm_year > 199 || p_tm->tm_year < 0 ||
        p_tm->tm_mon > 11   || p_tm->tm_mon < 0 ||
        p_tm->tm_mday >
        __mdays[__rx8025sa_leap_year_check(1900 + p_tm->tm_year)][p_tm->tm_mon] ||
        p_tm->tm_mday < 0 ||
        p_tm->tm_wday > 6 || p_tm->tm_wday < 0 ||
        p_tm->tm_hour > 23 || p_tm->tm_hour < 0 ||
        p_tm->tm_min > 59 || p_tm->tm_min < 0 ||
        p_tm->tm_sec > 59 || p_tm->tm_sec < 0 ) {
        return -AM_EINVAL;
    } else {
        return AM_OK;
    }
}
/**
 * \brief RTC标准操作函数-时间设置驱动函数
 *
 * \param[in] p_drv : 指向设备结构体的指针
 * \param[in] p_tm  : 指向时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
static int __rx8025sa_time_set (void *p_drv, am_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    uint8_t temp;

    /* 数据缓冲区 */
    uint8_t buf[7] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 验证时间信息的有效性 */
    if ((AM_OK != __rx8025sa_time_validator(p_tm)) ||
        (p_tm->tm_hour > 23) || (p_tm->tm_hour < 0)) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_rx8025sa_dev_t *)p_drv)->i2c_dev);

    /* 将时间日期信息填入buf中 */
    buf[__RX8025SA_REG_SECONDS >> 4 ] = AM_HEX_TO_BCD(p_tm->tm_sec);
    buf[__RX8025SA_REG_MINUTES >> 4 ] = AM_HEX_TO_BCD(p_tm->tm_min);

    if (((am_rx8025sa_dev_t *)p_drv)->time_format == __RX8025SA_24H_FORMAT){
        buf[__RX8025SA_REG_HOURS >> 4] = AM_HEX_TO_BCD(p_tm->tm_hour);         /* 24小时制的小时 */
    } else {
        temp = AM_HEX_TO_BCD((p_tm->tm_hour % 12));
        if (temp == 0) {
            temp = 12;
        }
        buf[__RX8025SA_REG_HOURS >> 4] = temp | ((p_tm->tm_hour / 12) << 5);   /* 12小时制的小时 */
    }

    buf[__RX8025SA_REG_WEEKDAYS >> 4] = AM_HEX_TO_BCD(p_tm-> tm_wday);
    buf[__RX8025SA_REG_DAYS >> 4]     = AM_HEX_TO_BCD(p_tm->tm_mday);
    buf[__RX8025SA_REG_MONTHS >> 4]   = AM_HEX_TO_BCD(p_tm->tm_mon + 1);
    buf[__RX8025SA_REG_YEAERS >> 4]   = AM_HEX_TO_BCD(p_tm->tm_year);

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_SECONDS,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }

    return AM_OK;

}

/**
 * \brief RTC标准操作函数-时间获取驱动函数
 *
 * \param[in] p_drv : 指向设备结构体的指针
 * \param[in] p_tm  : 指向时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
static int __rx8025sa_time_get (void *p_drv, am_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    uint8_t  temp;

    /* 数据缓冲区 */
    uint8_t buf[7] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_rx8025sa_dev_t *)p_drv)->i2c_dev);

    /* 通过I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_SECONDS,
                      buf,
                      sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }

    /** 从buf中获取时间日期信息 */
    p_tm->tm_sec  = AM_BCD_TO_HEX(buf[__RX8025SA_REG_SECONDS >> 4] & 0x7F);
    p_tm->tm_min  = AM_BCD_TO_HEX(buf[__RX8025SA_REG_MINUTES >> 4] & 0x7F);

    if (((am_rx8025sa_dev_t *)p_drv)->time_format == __RX8025SA_24H_FORMAT){
        p_tm->tm_hour = AM_BCD_TO_HEX(buf[__RX8025SA_REG_HOURS >> 4] & 0x3F);        /* 24小时制的小时 */
    } else {
        temp = AM_BCD_TO_HEX(buf[__RX8025SA_REG_HOURS >> 4]& 0x1F);
        if (temp == 12) {
            temp =0;
        }
        p_tm->tm_hour = temp + ((buf[__RX8025SA_REG_HOURS >> 4] >> 5) & 0x01) * 12;  /* 12小时制的小时 */
    }

    p_tm->tm_wday = AM_BCD_TO_HEX(buf[__RX8025SA_REG_WEEKDAYS >> 4] & 0x07);
    p_tm->tm_mday = AM_BCD_TO_HEX(buf[__RX8025SA_REG_DAYS >> 4] & 0x3F );
    p_tm->tm_mon  = AM_BCD_TO_HEX(buf[__RX8025SA_REG_MONTHS >> 4] & 0x1F) - 1;
    p_tm->tm_year = AM_BCD_TO_HEX(buf[__RX8025SA_REG_YEAERS >> 4]);
    p_tm->tm_yday = __rx8025sa_day_of_year(p_tm->tm_year, p_tm->tm_mon, p_tm->tm_mday);
    return AM_OK;

}

/**
 * \brief ALARM_CLK 标准操作函数-闹钟时间设置函数
 *
 * \param[in] p_drv : 指向设备结构体的指针
 * \param[in] p_tm  : 指向闹钟时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
static int __rx8025sa_alarm_clk_time_set (void *p_drv, am_alarm_clk_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[3] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 验证时间信息的有效性 */
    if ((p_tm->min  > 59) || (p_tm->min  < 0) ||
        (p_tm->hour > 23) || (p_tm->hour < 0)) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_rx8025sa_dev_t *)p_drv)->i2c_dev);

    /* 将时间日期信息填入buf中 */
    buf[0] = AM_HEX_TO_BCD(p_tm->min);
    buf[1] = AM_HEX_TO_BCD(p_tm->hour);
    /* 星期数据无需转成BCD码 */
    buf[2] = p_tm->wdays;

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_MINUTES_ALARM_W,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }

    return AM_OK;
}

/**
 * \brief ALARM_CLK 标准操作函数-闹钟回调函数的设置函数
 *
 * \param[in] p_drv         : 指向设备结构体的指针
 * \param[in] pfn_callback  : 指向回调函数的指针
 * \param[in] p_arg         : 指向回调函数参数的指针
 *
 * \retval AM_OK  : 设置成功
 * \retval 其他             : 查看错误码
 */
static int __rx8025sa_alarm_clk_callback_set (void         *p_drv,
                                              am_pfnvoid_t  pfn_alarm_callback,
                                              void          *p_arg)
{
	/* 函数调用返回值 */
    int ret = AM_OK;

    /* RX8025SA 设备指针 */
    am_rx8025sa_handle_t  rx8025sa_handle = NULL;

    /* 设备信息指针 */
   const am_rx8025sa_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    rx8025sa_handle =  (am_rx8025sa_handle_t)p_drv;
    p_devinfo      = rx8025sa_handle->p_devinfo;

    /* 连接引脚中断回调函数 */
    ret = am_gpio_trigger_connect(p_devinfo->intb_pin,
                                  __rx8025sa_intb_isr,
                                  rx8025sa_handle);
    if (AM_OK != ret) {
        return ret;
    }

    /* 设置为下降沿触发 */
    am_gpio_trigger_cfg(p_devinfo->intb_pin, AM_GPIO_TRIGGER_FALL);
    am_gpio_trigger_on(p_devinfo->intb_pin);

    /* 连接中断回调函数 */
    rx8025sa_handle->triginfo[2].pfn_callback = pfn_alarm_callback;
    rx8025sa_handle->triginfo[2].p_arg        = p_arg;

    /* 如果所有的回调函数都为空 */
    if (rx8025sa_handle->triginfo[2].pfn_callback == NULL) {

        /* 删除引脚中断回调函数 */
        ret = am_gpio_trigger_disconnect(p_devinfo->intb_pin,
                                         __rx8025sa_intb_isr,
                                         rx8025sa_handle);

        if (AM_OK != ret) {
            return ret;
        }

        /* 关闭引脚触发 */
        am_gpio_trigger_off(p_devinfo->intb_pin);
    }

    return AM_OK;
}

/**
 * \brief ALARM_CLK 标准操作函数-开启闹钟函数
 *
 * \param[in] p_drv : 指向设备结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
static int __rx8025sa_alarm_clk_on (void *p_drv)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]  = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    /* 从p_drv中获取i2c设备指针 */
    p_i2c_dev = &(((am_rx8025sa_dev_t *)p_drv)->i2c_dev);

    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断使能 */
    buf[0] |= __RX8025SA_C1_ALARM_W_EN;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/**
 * \brief ALARM_CLK 标准操作函数-关闭闹钟函数
 *
 * \param[in] p_drv : 指向设备结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
static int __rx8025sa_alarm_clk_off (void *p_drv)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]  = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    /* 从p_drv中获取i2c设备指针 */
    p_i2c_dev = &(((am_rx8025sa_dev_t *)p_drv)->i2c_dev);

    /* 读取控制寄存器 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 关闭闹钟中断 */
    buf[0] &= ~__RX8025SA_C1_ALARM_W_EN;

    /* 将闹钟中断信息写入控制状态寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/**
 * \brief 根据中断标志来执行用户中断回调函数
 *
 * \param[in] p_dev     : 指向RX8025SA设备的指针
 * \param[in] intr_flag : 中断标志
 *
 * \return 无
 */
static void __rx8025sa_callback_exec (void    *p_drv,
                                      uint8_t  intr_flag)
{
    am_rx8025sa_dev_t *p_dev        = NULL;
    am_pfnvoid_t       pfn_callback = NULL;
    void              *p_arg        = NULL;

    /* 检验参数有效性 */
    if (0 == intr_flag || NULL == p_drv) {
        return;
    }

    p_dev = (am_rx8025sa_dev_t *)p_drv;

    /* 如果是周期中断，则执行对应用户中断回调函数 */
    if (intr_flag & __RX8025SA_C2_CTFG_INT) {
        pfn_callback = p_dev->triginfo[0].pfn_callback;
        p_arg        = p_dev->triginfo[0].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }

    /* 如果是ALARM_D中断，则执行对应用户中断回调函数 */
    if (intr_flag & __RX8025SA_C2_DAFG_ALARM_D) {
        pfn_callback = p_dev->triginfo[1].pfn_callback;
        p_arg        = p_dev->triginfo[1].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }

    /* 如果是ALARM_W中断，则执行对应用户中断回调函数 */
    if (intr_flag & __RX8025SA_C2_WAFG_ALARM_W) {
        pfn_callback = p_dev->triginfo[2].pfn_callback;
        p_arg        = p_dev->triginfo[2].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }
}
/**
 * \brief rx8025sa设备初始化
 * \param[in] p_dev : 指向rx8025sa设备结构体
 * \return AM_OK表示初始化成功, 其他失败
 */
static int __rx8025sa_init (am_rx8025sa_dev_t *p_dev)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(p_dev->i2c_dev);

    /* 控制状态寄存器2时钟输出禁能,清除中断标志 */
    buf[0] = 0;

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
    		           __RX8025SA_REG_CONTROL1,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 在中断中读取数据后的处理函数,判断中断类型
 *
 * \param[in,out] buf         : 数据缓冲区
 * \param[in]     p_intr_flag : 中断标志  参见 \ref grp_rx8025sa_intr_flag
 *
 * \return 无
 *
 * \note 函数中buf 对应 CONTROL2寄存器
 */
static void __rx8025sa_read_write_data_processing (void    *p_arg,
                                                   uint8_t *p_intr_flag)
{
    am_rx8025sa_dev_t *p_dev = NULL;

    if (NULL == p_arg) {
        return;
    }

    p_dev = (am_rx8025sa_dev_t *)p_arg;

    *p_intr_flag = 0;

    /* 判断是否为DAFG标志置位 */
    if (p_dev->buf[0] & __RX8025SA_C2_DAFG_ALARM_D) {

        /* 记录ALARM_D中断标志 */
        *p_intr_flag |= __RX8025SA_C2_DAFG_ALARM_D;

        /* 清除DAFG标志 */
        p_dev->buf[0] &= ~__RX8025SA_C2_DAFG_ALARM_D;
    }

    /* 判断是否为CTFG标志置位 */
    if (p_dev->buf[0] & __RX8025SA_C2_CTFG_INT) {
          /* 记录闹钟中断标志,并清除*/
        *p_intr_flag |= __RX8025SA_C2_CTFG_INT;

          /* 清除CTFG标志 */
        p_dev->buf[0] &= ~__RX8025SA_C2_CTFG_INT;
        }

    /* 判断是否为WAFG标志置位 */
    if (p_dev->buf[0] & __RX8025SA_C2_WAFG_ALARM_W) {
        /* 记录闹钟中断标志,并清除 */
        *p_intr_flag |= __RX8025SA_C2_WAFG_ALARM_W;

          /* 清除WAFG标志 */
        p_dev->buf[0] &= ~__RX8025SA_C2_WAFG_ALARM_W;
        }
}
/**
 * \brief I2C读写过程中的状态机
 * \param[in] p_arg : 参数
 * \return 无
 * \note 状态变换顺序   [1]处理开始 --> [2]写subaddr,读取状态 --> [3]处理数据
 *                   --> [5]写subaddr --> [6]写入数据 --> [7]处理完成
 */
static void __rx8025sa_i2c_read_write_fsm (void *p_arg)
{
    /* I2C设备 */
    am_i2c_device_t *p_dev_i2c = NULL;

    /* rx8025sa设备 */
    am_rx8025sa_dev_t *p_dev_rx8025sa = NULL;

    /* 判断参数的有效性 */
    if (NULL == p_arg) {
        return;
    }

    /* 获取i2c_status */
    p_dev_rx8025sa = (am_rx8025sa_dev_t *)p_arg;

    /*获取i2c_device */
    p_dev_i2c = &(p_dev_rx8025sa->i2c_dev);

    switch (p_dev_rx8025sa->status) {

    /* 获取状态 */
    case __RX8025SA_I2C_RD_STATUS:

        /* 加载下一状态 */
        p_dev_rx8025sa->status = __RX8025SA_I2C_WR_STATUS;

        am_i2c_mktrans(&p_dev_rx8025sa->trans[0],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_WR),
                       (uint8_t *)&p_dev_rx8025sa->sub_addr,
                       1);

        am_i2c_mktrans(&p_dev_rx8025sa->trans[1],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_RD),
                       (uint8_t *)p_dev_rx8025sa->buf,
                       p_dev_rx8025sa->nbytes);

        am_i2c_mkmsg(&p_dev_rx8025sa->msg,
                     p_dev_rx8025sa->trans,
                     2,
                     (am_pfnvoid_t)__rx8025sa_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_dev_i2c->handle, &p_dev_rx8025sa->msg);
        break;

    /* 清除状态 */
    case __RX8025SA_I2C_WR_STATUS:

        /* 加载下一状态 */
        p_dev_rx8025sa->status = __RX8025SA_I2C_DATA_PROCESSING;

        /* 处理数据，获取中断状态 */
        __rx8025sa_read_write_data_processing(p_arg, &p_dev_rx8025sa->intr_flag);

        am_i2c_mktrans(&p_dev_rx8025sa->trans[0],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_WR),
                       (uint8_t *)&p_dev_rx8025sa->sub_addr,
                       1);

        am_i2c_mktrans(&p_dev_rx8025sa->trans[1],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_NOSTART | AM_I2C_M_WR),
                       (uint8_t *)p_dev_rx8025sa->buf,
                       p_dev_rx8025sa->nbytes);

        am_i2c_mkmsg(&p_dev_rx8025sa->msg,
                     p_dev_rx8025sa->trans,
                     2,
                     (am_pfnvoid_t)__rx8025sa_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_dev_i2c->handle, &p_dev_rx8025sa->msg);
        break;

    /* 数据处理 */
    case __RX8025SA_I2C_DATA_PROCESSING:

        /* 加载下一状态 */
        p_dev_rx8025sa->status = __RX8025SA_I2C_RD_STATUS;

        /* 执行用户中断中断回调函数 */
        __rx8025sa_callback_exec(p_arg, p_dev_rx8025sa->intr_flag);

        /* 开启GPIO中断 */
        if ( (p_dev_rx8025sa->intr_flag & __RX8025SA_C2_DAFG_ALARM_D ) ||
             (p_dev_rx8025sa->intr_flag & __RX8025SA_C2_CTFG_INT )){
             am_gpio_trigger_on(p_dev_rx8025sa->p_devinfo->inta_pin);
        }
        if ( p_dev_rx8025sa->intr_flag & __RX8025SA_C2_WAFG_ALARM_W) {
             am_gpio_trigger_on(p_dev_rx8025sa->p_devinfo->intb_pin);
        }

        break;

    default:
        break;
    }
}

/**
 * \brief RX8025SA 上网INTA中断回调函数
 * \param[in] p_arg : 中断回调函数参数
 * \return 无
 */
static void __rx8025sa_inta_isr (void *p_arg)
{
    am_rx8025sa_dev_t        *p_dev        = (am_rx8025sa_dev_t *)p_arg;

    if (NULL == p_arg) {
        return;
    }

    /* 关闭GPIO中断 */
    am_gpio_trigger_off(p_dev->p_devinfo->inta_pin);

    if (p_dev->status == __RX8025SA_I2C_RD_STATUS) {

        /* 启动读写状态机 */
        __rx8025sa_i2c_read_write_fsm(p_arg);
    }
}

/**
 * \brief RX8025SA 上网INTB中断回调函数
 * \param[in] p_arg : 中断回调函数参数
 * \return 无
 */
static void __rx8025sa_intb_isr (void *p_arg)
{
    am_rx8025sa_dev_t        *p_dev        = (am_rx8025sa_dev_t *)p_arg;

    if (NULL == p_arg) {
        return;
    }

    /* 关闭GPIO中断 */
    am_gpio_trigger_off(p_dev->p_devinfo->intb_pin);

    if (p_dev->status == __RX8025SA_I2C_RD_STATUS) {

        /* 启动读写状态机 */
        __rx8025sa_i2c_read_write_fsm(p_arg);
    }
}

/**
 * \brief RX8025SA 时间格式设置
 */
static int __rx8025sa_time_format_set (am_rx8025sa_handle_t handle, uint8_t format)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]  = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    handle->time_format = format;
    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    if ( format == __RX8025SA_24H_FORMAT ){
        buf[0] |= __RX8025SA_C1_24H_FORMAT;
    } else {
        buf[0] &= ~ __RX8025SA_C1_24H_FORMAT;
    }

    /* 写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;

}

/*****************************************************************************
 * 外部函数实现
 ****************************************************************************/

/**
 * \brief RX8025SA 设备初始化
 */
am_rx8025sa_handle_t am_rx8025sa_init (am_rx8025sa_dev_t           *p_dev,
                                       const am_rx8025sa_devinfo_t *p_devinfo,
                                       am_i2c_handle_t              i2c_handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;
    int i;

    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == i2c_handle) {
        return NULL;
    }

    /* 构造i2c设备 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 __RX8025SA_ADDR,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 初始化设备信息 */
    p_dev->conn_stat_inta = AM_FALSE;
    p_dev->p_devinfo      = p_devinfo;

    p_dev->time_format    = __RX8025SA_24H_FORMAT;        /* 默认24小时制计时 */

    for (i = 0; i < 3; i++) {
        p_dev->triginfo[i].pfn_callback = NULL;
        p_dev->triginfo[i].p_arg        = NULL;
    }
    p_dev->status   = __RX8025SA_I2C_RD_STATUS;      /* 初始状态 */
    p_dev->sub_addr =__RX8025SA_REG_CONTROL2;        /* 待操作的寄存器地址 */
    p_dev->nbytes   = 1;                             /* 需要读取的数据个数 */

    /* 初始化RX8025SA设备 */
    ret = __rx8025sa_init(p_dev);
    if (AM_OK != ret) {
        return NULL;
    }

    /* 初始化时间格式 */
    ret = __rx8025sa_time_format_set( p_dev, p_dev->time_format);

    if (AM_OK != ret) {
        return NULL;
    }

    /* 初始化clk_en_pin 的GPIO口 */
    if (p_dev->p_devinfo->clk_en_pin != -1) {
         am_gpio_pin_cfg(p_dev->p_devinfo->clk_en_pin, AM_GPIO_OUTPUT_INIT_LOW | AM_GPIO_PUSH_PULL);  /* 配置为推挽输出 */
    }

    return p_dev;
}


/**
 * \brief RX8025SA 设备解初始化
 */
void am_rx8025sa_deinit (am_rx8025sa_handle_t handle)
{

}

/**
 * \brief 获取RX8025SA RTC标准服务
 */
am_rtc_handle_t am_rx8025sa_rtc_init (am_rx8025sa_handle_t  handle,
                                      am_rtc_serv_t        *p_rtc)
{
    p_rtc->p_drv   = handle;
    p_rtc->p_funcs = &__g_rtc_drv_funcs;
    return p_rtc;
}

/**
 * \brief 获取RX8025SA ALARM_CLK标准服务
 */
am_alarm_clk_handle_t am_rx8025sa_alarm_clk_init (am_rx8025sa_handle_t   handle,
		                                         am_alarm_clk_serv_t    *p_alarm_clk)
{
	p_alarm_clk->p_drv   = handle;
	p_alarm_clk->p_funcs = &__g_alarm_clk_drv_funcs;
    return p_alarm_clk;
}

/**
 * \brief 使能rx8025的 alarm_d 闹钟
 */
int am_rx8025sa_alarm_d_enable (am_rx8025sa_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断使能 */
    buf[0] |= __RX8025SA_C1_ALARM_D_EN;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}
/**
 * \brief 使能rx8025的 alarm_w 闹钟
 */
int am_rx8025sa_alarm_w_enable (am_rx8025sa_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断禁能 */
    buf[0] |= __RX8025SA_C1_ALARM_W_EN;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}



/**
 * \brief 禁能 RX8025SA 的 ALARM_D 闹钟
 */
int am_rx8025sa_alarm_d_disable (am_rx8025sa_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[2]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制寄存器1,2(用来控制闹钟中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      2);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断禁能 */
    buf[0] &= ~__RX8025SA_C1_ALARM_D_EN;

    /* 清除AF标志 */
    buf[1] &= ~__RX8025SA_C2_DAFG_ALARM_D;

    /* 将闹钟中断信息写入控制状态寄存器1,2 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       2);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 禁能 RX8025SA 的 ALARM_W 闹钟
 */
int am_rx8025sa_alarm_w_disable (am_rx8025sa_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[2]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制寄存器1,2(用来控制闹钟中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      2);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断禁能 */
    buf[0] &= ~__RX8025SA_C1_ALARM_W_EN;

    /* 清除AF标志 */
    buf[1] &= ~__RX8025SA_C2_WAFG_ALARM_W;

    /* 将闹钟中断信息写入控制状态寄存器1,2 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       2);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}



/**
 * \brief RX8025SA 周期中断函数设置
 */
int am_rx8025sa_periodic_int_set (am_rx8025sa_handle_t  handle,
                                   uint8_t              int_mode,
                                  am_pfnvoid_t          pfn_periodic_int_callback,
                                  void                 *p_periodic_int_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 设备信息指针 */
    const am_rx8025sa_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_devinfo = handle->p_devinfo;
    p_i2c_dev = &(handle->i2c_dev);

    /* conn_stat_inta用于记录当前中断连接状态,避免重复调用GPIO连接函数 */
    if (!(handle->conn_stat_inta)) {

        /* 连接引脚中断回调函数 */
        ret = am_gpio_trigger_connect(p_devinfo->inta_pin,
                                      __rx8025sa_inta_isr,
                                      handle);
        if (AM_OK != ret) {
            return ret;
        }

        /* 设置为下降沿触发 */
        am_gpio_trigger_cfg(p_devinfo->inta_pin, AM_GPIO_TRIGGER_FALL);
        am_gpio_trigger_on(p_devinfo->inta_pin);

        /* 更新conn_stat状态为TRUE */
        handle->conn_stat_inta = AM_TRUE;

    }

    /* 连接中断回调函数 */
    handle->triginfo[0].pfn_callback = pfn_periodic_int_callback;
    handle->triginfo[0].p_arg        = p_periodic_int_arg;

    /* 如果所有的回调函数都为空 */
    if ((handle->triginfo[0].pfn_callback == NULL) &&
        (handle->triginfo[1].pfn_callback == NULL)) {

        /* 如果conn_stat_inta为TRUE则调用GPIO解除连接函数 */
        if (handle->conn_stat_inta) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->inta_pin,
                                             __rx8025sa_inta_isr,
                                             handle);

            if (AM_OK != ret) {
                return ret;
            }

            /* 关闭引脚触发 */
            am_gpio_trigger_off(p_devinfo->inta_pin);

            /* 更新conn_stat状态为FALSE */
            handle->conn_stat_inta = AM_FALSE;
        }
    }

    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }
    buf[0] |= int_mode;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/**
 * \brief RX8025SA 闹钟D回调函数和闹钟时间设置
 */
int am_rx8025sa_alarm_d_set(am_rx8025sa_handle_t      handle,
                            am_rx8025sa_alarm_info_t *p_alarm_info,
                            am_pfnvoid_t              pfn_alarm_callback,
                            void                     *p_alarm_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[2] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 设备信息指针 */
    const am_rx8025sa_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_devinfo = handle->p_devinfo;
    p_i2c_dev = &(handle->i2c_dev);

    /* conn_stat_inta用于记录当前中断连接状态,避免重复调用GPIO连接函数 */
    if (!(handle->conn_stat_inta)) {

        /* 连接引脚中断回调函数 */
        ret = am_gpio_trigger_connect(p_devinfo->inta_pin,
                                      __rx8025sa_inta_isr,
                                       handle);
        if (AM_OK != ret) {
            return ret;
        }

        /* 设置为下降沿触发 */
        am_gpio_trigger_cfg(p_devinfo->inta_pin, AM_GPIO_TRIGGER_FALL);
        am_gpio_trigger_on(p_devinfo->inta_pin);

        /* 更新conn_stat_inta状态为TRUE */
        handle->conn_stat_inta = AM_TRUE;

    }
    /* 连接中断回调函数 */
    handle->triginfo[1].pfn_callback = pfn_alarm_callback;
    handle->triginfo[1].p_arg        = p_alarm_arg;

    /* 如果所有的回调函数都为空 */
    if ((handle->triginfo[0].pfn_callback == NULL) &&
        (handle->triginfo[1].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (handle->conn_stat_inta) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->inta_pin,
                                             __rx8025sa_inta_isr,
                                             handle);

            if (AM_OK != ret) {
                return ret;
            }

            /* 关闭引脚触发 */
            am_gpio_trigger_off(p_devinfo->inta_pin);

            /* 更新conn_stat状态为FALSE */
            handle->conn_stat_inta = AM_FALSE;
        }
    }

    /* 验证闹钟信息设置值的有效性 */
    if (p_alarm_info->hour > 23 || p_alarm_info->hour < 0 ||
        p_alarm_info->min  > 59 || p_alarm_info->min < 0) {
        return -AM_EINVAL;
    }

    buf[0] = AM_HEX_TO_BCD(p_alarm_info->min);
    buf[1] = AM_HEX_TO_BCD(p_alarm_info->hour);
    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_MINUTES_ALARM_D,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }
    /* 使能ALARM_D闹钟*/
    am_rx8025sa_alarm_d_enable(handle);

    return AM_OK;
}


/**
 * \brief RX8025SA 闹钟W回调函数和闹钟时间设置
 */
int am_rx8025sa_alarm_w_set(am_rx8025sa_handle_t      handle,
                            am_rx8025sa_alarm_info_t *p_alarm_info,
                            am_pfnvoid_t              pfn_alarm_callback,
                            void                     *p_alarm_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[3] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 设备信息指针 */
    const am_rx8025sa_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_devinfo = handle->p_devinfo;
    p_i2c_dev = &(handle->i2c_dev);

     /* 连接引脚中断回调函数 */
    ret = am_gpio_trigger_connect(p_devinfo->intb_pin,
                                  __rx8025sa_intb_isr,
                                  handle);
    if (AM_OK != ret) {
        return ret;
    }

    /* 设置为下降沿触发 */
    am_gpio_trigger_cfg(p_devinfo->intb_pin, AM_GPIO_TRIGGER_LOW);
    am_gpio_trigger_on(p_devinfo->intb_pin);

    /* 连接中断回调函数 */
    handle->triginfo[2].pfn_callback = pfn_alarm_callback;
    handle->triginfo[2].p_arg        = p_alarm_arg;

    /* 如果所有的回调函数都为空 */
    if ( handle->triginfo[2].pfn_callback == NULL) {

        /* 删除引脚中断回调函数 */
        ret = am_gpio_trigger_disconnect(p_devinfo->intb_pin,
                                         __rx8025sa_intb_isr,
                                         handle);

        if (AM_OK != ret) {
            return ret;
        }

        /* 关闭引脚触发 */
        am_gpio_trigger_off(p_devinfo->inta_pin);

         /* 更新conn_stat状态为FALSE */
        handle->conn_stat_inta = AM_FALSE;
    }

    /* 验证闹钟信息设置值的有效性 */
    if (p_alarm_info->alarm_week > 127  || p_alarm_info->alarm_week < 0 ||
        p_alarm_info->hour > 23 || p_alarm_info->hour < 0 ||
        p_alarm_info->min  > 59 || p_alarm_info->min < 0) {
        return -AM_EINVAL;
    }

    buf[0] = AM_HEX_TO_BCD(p_alarm_info->min);
    buf[1] = AM_HEX_TO_BCD(p_alarm_info->hour);
    buf[2] = p_alarm_info->alarm_week;
    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
    		           __RX8025SA_REG_MINUTES_ALARM_W,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }
    /* 使能ALARM_W闹钟*/
    am_rx8025sa_alarm_w_enable(handle);

    return AM_OK;

}
/*
 * 电源电压震荡检测
 */
int am_rx8025sa_power_check(am_rx8025sa_handle_t  handle)
{
    /* 函数调用返回值 */
	int ret;

    /* 数据缓冲区 */
    uint8_t buf;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 通过I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
    		          __RX8025SA_REG_CONTROL2,
                      &buf,
                      1);

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }
    buf = (buf>>4) & 0x07;

    return buf;
}

/**
 * 低电压检测功能的标准电压值设定
 */
int am_rx8025sa_check_voltage_set (am_rx8025sa_handle_t handle, uint8_t check_v_value)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
    		          __RX8025SA_REG_CONTROL2,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    if ( check_v_value == RX8025SA_CHECK_VOLTAGE_1_3V ){
        buf[0] |= __RX8025SA_C2_VDSL_1_3_SET;
    } else {
    	buf[0] &= ~ __RX8025SA_C2_VDSL_1_3_SET;
    }

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
    		           __RX8025SA_REG_CONTROL2,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;

}

/*
 * 计时频率进行调快或调慢
 */
int am_rx8025sa_clk_adjust ( am_rx8025sa_handle_t handle, float clk )
{
    /* 函数调用返回值 */
    int   ret = AM_OK;

    double  clk_off;
    int     temp;

    /* 数据缓冲区 */
    uint8_t buf[1]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    clk_off = ( clk - 32768)/32768;
    if ( clk_off >= 0 ) {
        temp = (int)(clk_off / 0.00000305 +0.5);
        temp += 1;
    } else {
        temp = (int)(clk_off / 0.00000305 -0.5);
    	 temp +=128;
    }

    buf[0] = temp;

    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_DIGITAL_OFFSET,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}
/**
 * 设置频率输出
 */
int am_rx8025sa_clkout_set(am_rx8025sa_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]  = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025SA_REG_CONTROL1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    buf[0] &= 0xEF;  /* 将控制寄存器1中CLEN2为清零 */

    /* 写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025SA_REG_CONTROL1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    /* 将 FOE引脚置高，使能CLK输出 */
    /* 将 FOE引脚置高，使能CLK输出 */
    if( handle->p_devinfo->clk_en_pin != -1) {
        ret = am_gpio_set(handle->p_devinfo->clk_en_pin, AM_GPIO_LEVEL_HIGH);
    }

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * 关掉频率输出
 */
int am_rx8025sa_clkout_close(am_rx8025sa_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 将 FOE引脚置低，关闭CLK输出 */
    if( handle->p_devinfo->clk_en_pin != -1) {
        ret = am_gpio_set(handle->p_devinfo->clk_en_pin, AM_GPIO_LEVEL_LOW);
    }

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}
/* end of file */
