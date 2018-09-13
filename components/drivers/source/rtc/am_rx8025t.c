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
 * \brief RX8025T软件包实现
 *
 * \internal
 * \par Modification history
 * - 1.01 18-01-08  sdq, make some changes.
 * - 1.00 17-08-07  vir, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_rx8025t.h"
#include "am_gpio.h"
/*****************************************************************************
 * 宏定义
 ****************************************************************************/
 
 /** \brief RX8025T设备地址  (七位)*/
#define __RX8025T_ADDR               0x32

#define __RX8025T_YEAR_OFFS          100
/**
 * \name RX8025T寄存器地址
 */
 
#define __RX8025T_REG_SECONDS          0x00    /**< \brief 秒寄存器地址  */
#define __RX8025T_REG_MINUTES          0x01    /**< \brief 分寄存器地址  */
#define __RX8025T_REG_HOURS            0x02    /**< \brief 时寄存器地址   */
#define __RX8025T_REG_WEEKDAYS         0x03    /**< \brief 星期寄存器地址 */
#define __RX8025T_REG_DAYS             0x04    /**< \brief 日寄存器地址  */
#define __RX8025T_REG_MONTHS           0x05    /**< \brief 月寄存器地址  */
#define __RX8025T_REG_YEAERS           0x06    /**< \brief 年寄存器地址   */

#define __RX8025T_REG_RAM              0x07    /**< \brief RAM寄存器地址   */

#define __RX8025T_REG_MINUTES_ALARM    0x08    /**< \brief 闹钟分寄存器地址   */
#define __RX8025T_REG_HOURS_ALARM      0x09    /**< \brief 闹钟时寄存器地址   */
#define __RX8025T_REG_WEEK_DAY_ALARM   0x0a    /**< \brief 闹钟星期或天寄存器地址   */

#define __RX8025T_REG_TIMER_COUNTER0   0x0b    /**< \brief 定时控制寄存器1地址   */
#define __RX8025T_REG_TIMER_COUNTER1   0x0c    /**< \brief 定时控制寄存器2地址   */

#define __RX8025T_REG_EXTENSION        0X0d    /**< \brief 扩展寄存器地址   */
#define __RX8025T_REG_FLAG             0x0e    /**< \brief 标志寄存器地址   */
#define __RX8025T_REG_CONTROL          0x0f    /**< \brief 控制寄存器地址   */

/**
 * \name RX8025T 寄存器位域描述
 * \anchor grp_rx8025t_bitfield_descriptions
 * @{
 */

#define __RX8025T_TE_TIMER_ENABLE      0x10    /**< \brief 使能定时器 */
#define __RX8025T_USEL_MIN_UPDATE      0x20    /**< \brief 更新的每分钟触发中断 */
#define __RX8025T_WADA_ALARM_DAY       0x40    /**< \brief 闹钟功能选择天为目标 */

#define __RX8025T_FLAG_AF   0x08    /**< \brief 闹钟中断标志置位 */
#define __RX8025T_FLAG_TF   0x10    /**< \brief 定时器中断标志置位 */
#define __RX8025T_FLAG_UF   0x20    /**< \brief 时间更新中断标志置位 */

#define __RX8025T_ALARM_INT_ENABLE   0x08  /**< \brief 闹钟中断使能 */
/**
 * \name RX8025T I2C当前状态
 * \anchor grp_rx8025t_i2c_operation
 * @{
 */

#define __RX8025T_I2C_RD_STATUS        0x00    /**< \brief 读取状态 */
#define __RX8025T_I2C_WR_STATUS        0x01    /**< \brief 清除状态 */
#define __RX8025T_I2C_DATA_PROCESSING  0x02    /**< \brief 数据处理阶段 */

/* RTC driver function implementation */
static int __rx8025t_time_set (void *p_drv, am_tm_t *p_tm);
static int __rx8025t_time_get (void *p_drv, am_tm_t *p_tm);

/**
 * \brief RTC driver functions definition
 */
static struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
    __rx8025t_time_set,
    __rx8025t_time_get,
};

/* 函数声明 */
static void __rx8025t_eint_isr (void *p_arg);

/* ALARM_CLK driver function implementation */
static int __rx8025t_alarm_clk_time_set (void *p_drv, am_alarm_clk_tm_t *p_tm);
static int __rx8025t_alarm_clk_callback_set (void         *p_drv,
                                             am_pfnvoid_t  pfn_alarm_callback,
                                             void         *p_arg);
static int __rx8025t_alarm_clk_on (void *p_drv);
static int __rx8025t_alarm_clk_off (void *p_drv);

/**
 * \brief ALARM_CLK driver functions definition
 */
static struct am_alarm_clk_drv_funcs __g_alarm_clk_drv_funcs = {
    __rx8025t_alarm_clk_time_set,
    __rx8025t_alarm_clk_callback_set,
    __rx8025t_alarm_clk_on,
    __rx8025t_alarm_clk_off,
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
static uint8_t __rx8025t_leap_year_check (uint16_t year)
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
static uint8_t __rx8025t_day_of_year (uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t i;

    if (__rx8025t_leap_year_check(year)) {
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
int __rx8025t_time_validator (am_tm_t *p_tm)
{
    if (p_tm->tm_year > 199 || p_tm->tm_year < 0 ||
        p_tm->tm_mon > 11   || p_tm->tm_mon < 0 ||
        p_tm->tm_mday >
        __mdays[__rx8025t_leap_year_check(1900 + p_tm->tm_year)][p_tm->tm_mon] ||
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
static int __rx8025t_time_set (void *p_drv, am_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[7] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 验证时间信息的有效性 */
    if ((AM_OK != __rx8025t_time_validator(p_tm)) ||
        (p_tm->tm_hour > 23) || (p_tm->tm_hour < 0)) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_rx8025t_dev_t *)p_drv)->i2c_dev);

    /* 将时间日期信息填入buf中 */
    buf[__RX8025T_REG_SECONDS ] = AM_HEX_TO_BCD(p_tm->tm_sec);
    buf[__RX8025T_REG_MINUTES ] = AM_HEX_TO_BCD(p_tm->tm_min);
    buf[__RX8025T_REG_HOURS]    = AM_HEX_TO_BCD(p_tm->tm_hour);         /* 24小时制的小时 */

    buf[__RX8025T_REG_WEEKDAYS] = 1 << p_tm->tm_wday;
    buf[__RX8025T_REG_DAYS]     = AM_HEX_TO_BCD(p_tm->tm_mday);
    buf[__RX8025T_REG_MONTHS]   = AM_HEX_TO_BCD(p_tm->tm_mon + 1);
    buf[__RX8025T_REG_YEAERS]   = AM_HEX_TO_BCD((p_tm->tm_year - __RX8025T_YEAR_OFFS) % 100);

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_SECONDS,
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
static int __rx8025t_time_get (void *p_drv, am_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    uint8_t  temp = 0;

    /* 数据缓冲区 */
    uint8_t buf[7] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_rx8025t_dev_t *)p_drv)->i2c_dev);

    /* 通过I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_SECONDS,
                      buf,
                      sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }

    /** 从buf中获取时间日期信息 */
    p_tm->tm_sec  = AM_BCD_TO_HEX(buf[__RX8025T_REG_SECONDS] & 0x7F);
    p_tm->tm_min  = AM_BCD_TO_HEX(buf[__RX8025T_REG_MINUTES] & 0x7F);
    p_tm->tm_hour = AM_BCD_TO_HEX(buf[__RX8025T_REG_HOURS] & 0x3F);

    while((buf[__RX8025T_REG_WEEKDAYS] >>= 1)) {
        temp++;
    }
    p_tm->tm_wday = temp;
    p_tm->tm_mday = AM_BCD_TO_HEX(buf[__RX8025T_REG_DAYS] & 0x3F );
    p_tm->tm_mon  = AM_BCD_TO_HEX(buf[__RX8025T_REG_MONTHS] & 0x1F) - 1;
    p_tm->tm_year = AM_BCD_TO_HEX(buf[__RX8025T_REG_YEAERS]) + __RX8025T_YEAR_OFFS;
    p_tm->tm_yday = __rx8025t_day_of_year(p_tm->tm_year, p_tm->tm_mon, p_tm->tm_mday);
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
static int __rx8025t_alarm_clk_time_set (void *p_drv, am_alarm_clk_tm_t *p_tm)
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
    p_i2c_dev = &(((am_rx8025t_dev_t *)p_drv)->i2c_dev);

    /* 将时间日期信息填入buf中 */
    buf[__RX8025T_REG_MINUTES_ALARM - __RX8025T_REG_MINUTES_ALARM] =
    		                              AM_HEX_TO_BCD(p_tm->min);
    buf[__RX8025T_REG_HOURS_ALARM - __RX8025T_REG_MINUTES_ALARM] =
    		                              AM_HEX_TO_BCD(p_tm->hour);
    /* 星期数据无需转成BCD码 */
    buf[__RX8025T_REG_WEEK_DAY_ALARM - __RX8025T_REG_MINUTES_ALARM] = p_tm->wdays;

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
    	               __RX8025T_REG_MINUTES_ALARM,
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
static int __rx8025t_alarm_clk_callback_set (void         *p_drv,
                                             am_pfnvoid_t  pfn_alarm_callback,
                                             void         *p_arg)
{
	/* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t wada_buf[1] = {0};

    /* RX8025T 设备指针 */
    am_rx8025t_handle_t  rx8025t_handle = NULL;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 设备信息指针 */
    const am_rx8025t_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    rx8025t_handle = (am_rx8025t_handle_t)p_drv;
    p_devinfo      = rx8025t_handle->p_devinfo;
    p_i2c_dev      = &(rx8025t_handle->i2c_dev);

    if (-1 == p_devinfo->int_pin) {
        return -AM_ENOTSUP;
    }

    /* conn_stat用于记录当前中断连接状态,避免重复调用GPIO连接函数 */
    if (!(rx8025t_handle->conn_stat)) {

        /* 连接引脚中断回调函数 */
        ret = am_gpio_trigger_connect(p_devinfo->int_pin,
        		                      __rx8025t_eint_isr,
									  rx8025t_handle);
        if (AM_OK != ret) {
            return ret;
        }

        /* 设置为下降沿触发 */
        am_gpio_trigger_cfg(p_devinfo->int_pin, AM_GPIO_TRIGGER_FALL);
        am_gpio_trigger_on(p_devinfo->int_pin);

        /* 更新conn_stat_inta状态为TRUE */
        rx8025t_handle->conn_stat = AM_TRUE;

    }

    /* 连接中断回调函数 */
    rx8025t_handle->triginfo[2].pfn_callback = pfn_alarm_callback;
    rx8025t_handle->triginfo[2].p_arg        = p_arg;

    /* 如果所有的回调函数都为空 */
    if ((rx8025t_handle->triginfo[0].pfn_callback == NULL) &&
    	(rx8025t_handle->triginfo[1].pfn_callback == NULL) &&
        (rx8025t_handle->triginfo[2].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (rx8025t_handle->conn_stat) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->int_pin,
                                             __rx8025t_eint_isr,
                                             rx8025t_handle);

            if (AM_OK != ret) {
                return ret;
            }

            /* 关闭引脚触发 */
            am_gpio_trigger_off(p_devinfo->int_pin);

            /* 更新conn_stat状态为FALSE */
            rx8025t_handle->conn_stat = AM_FALSE;
        }
    }

    /* 读取寄存器 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_EXTENSION,
                      wada_buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

   /* 标准层的闹钟功能只有选择星期作为闹钟时间的功能 */
   wada_buf[0] &= ~__RX8025T_WADA_ALARM_DAY;

    /* 将信息写入态寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_EXTENSION,
                       wada_buf,
                       1);

    if (AM_OK != ret) {
        return ret;
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
static int __rx8025t_alarm_clk_on (void *p_drv)
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
    p_i2c_dev = &(((am_rx8025t_dev_t *)p_drv)->i2c_dev);

    /* 读取控制寄存器1 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_CONTROL,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断使能 */
    buf[0] |= __RX8025T_ALARM_INT_ENABLE;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_CONTROL,
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
static int __rx8025t_alarm_clk_off (void *p_drv)
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
    p_i2c_dev = &(((am_rx8025t_dev_t *)p_drv)->i2c_dev);

    /* 读取控制寄存器 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_CONTROL,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 关闭闹钟中断 */
    buf[0] &= ~__RX8025T_ALARM_INT_ENABLE;

    /* 将闹钟中断信息写入控制状态寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_CONTROL,
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
 * \param[in] p_dev     : 指向RX8025T设备的指针
 * \param[in] intr_flag : 中断标志
 *
 * \return 无
 */
static void __rx8025t_callback_exec (void    *p_drv,
                                      uint8_t  intr_flag)
{
    am_rx8025t_dev_t *p_dev        = NULL;
    am_pfnvoid_t       pfn_callback = NULL;
    void              *p_arg        = NULL;

    /* 检验参数有效性 */
    if (0 == intr_flag || NULL == p_drv) {
        return;
    }

    p_dev = (am_rx8025t_dev_t *)p_drv;

    /* 如果是周期中断，则执行对应用户中断回调函数 */
    if (intr_flag & __RX8025T_FLAG_UF) {
        pfn_callback = p_dev->triginfo[0].pfn_callback;
        p_arg        = p_dev->triginfo[0].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }

    /* 如果是ALARM_D中断，则执行对应用户中断回调函数 */
    if (intr_flag & __RX8025T_FLAG_TF) {
        pfn_callback = p_dev->triginfo[1].pfn_callback;
        p_arg        = p_dev->triginfo[1].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }

    /* 如果是ALARM_W中断，则执行对应用户中断回调函数 */
    if (intr_flag & __RX8025T_FLAG_AF) {
        pfn_callback = p_dev->triginfo[2].pfn_callback;
        p_arg        = p_dev->triginfo[2].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }
}

/**
 * \brief 在中断中读取数据后的处理函数,判断中断类型
 *
 * \param[in,out] buf         : 数据缓冲区
 * \param[in]     p_intr_flag : 中断标志  参见 \ref grp_rx8025t_intr_flag
 *
 * \return 无
 *
 * \note 函数中buf 对应 CONTROL2寄存器
 */
static void __rx8025t_read_write_data_processing (void    *p_arg,
                                                  uint8_t *p_intr_flag)
{
    am_rx8025t_dev_t *p_dev = NULL;

    if (NULL == p_arg) {
        return;
    }

    p_dev = (am_rx8025t_dev_t *)p_arg;

    *p_intr_flag = 0;

    /* 判断是否为UF标志置位 */
    if (p_dev->buf[0] & __RX8025T_FLAG_UF) {

        /* 记录时间更新中断标志 */
        *p_intr_flag |= __RX8025T_FLAG_UF;

        /* 清除UF标志 */
        p_dev->buf[0] &= ~__RX8025T_FLAG_UF;
    } else {
        p_dev->buf[0] |= __RX8025T_FLAG_UF;
    }

    /* 判断是否为TF标志置位 */
    if (p_dev->buf[0] & __RX8025T_FLAG_TF) {
         /* 记录定时器中断标志,并清除*/
        *p_intr_flag |= __RX8025T_FLAG_TF;

         /* 清除TF标志 */
        p_dev->buf[0] &= ~__RX8025T_FLAG_TF;
    } else {
        p_dev->buf[0] |= __RX8025T_FLAG_TF;
    }

    /* 判断是否为AF标志置位 */
    if (p_dev->buf[0] & __RX8025T_FLAG_AF) {
        /* 记录闹钟中断标志,并清除 */
        *p_intr_flag |= __RX8025T_FLAG_AF;

          /* 清除AF标志 */
        p_dev->buf[0] &= ~__RX8025T_FLAG_AF;
    } else {
        p_dev->buf[0] |= __RX8025T_FLAG_AF;
    }

}
/**
 * \brief I2C读写过程中的状态机
 * \param[in] p_arg : 参数
 * \return 无
 * \note 状态变换顺序   [1]处理开始 --> [2]写subaddr,读取状态 --> [3]处理数据
 *                   --> [5]写subaddr --> [6]写入数据 --> [7]处理完成
 *
 *       如果时间更新中断和定时器中断在同一秒到来，经过测试发现定时器中断会比时间更新中断先到，读取中断
 *       状态只能读取到定时器中断，等到清除中断标识的时候（读取中断约0.5ms之后），时间更新中断也到来
 *       了，如果这时清除掉所有中断那么时间更新中断将得不到响应，如果只清除定时器中断，那么本次产生的时
 *       间更新中断会被推迟到下一次任何一个中断来临时一并处理，推迟的时间取决于下一次中断到来的时间。为
 *       了解决这个问题，采用的方法是第一次读取中断之后只清除处理过的中断，之后再读取一次中断标识，再进
 *       行处理。这种方法能保证中断发生后能及时处理，但是如果由于某种原因（程序的执行速度加快或SPI中断
 *       到来的更快）两次状态机的时间间隔缩短，就可能导致第二次读取状态的时候仍然没有读取到时间更新中
 *       断，而在第二次清状态的时候时间更新中断到来了。为了避免这种问题，再加入一个软件定时器，进行延时
 *       处理，发生中断2ms之后第三次读取中断状态并进行处理。
 */
static void __rx8025t_i2c_read_write_fsm (void *p_arg)
{
    am_i2c_device_t  *p_dev_i2c     = NULL; /* I2C设备 */
    am_rx8025t_dev_t *p_dev_rx8025t = NULL; /* rx8025t设备 */

    /* 判断参数的有效性 */
    if (NULL == p_arg) {
        return;
    }

    p_dev_rx8025t = (am_rx8025t_dev_t *)p_arg;

    /*获取i2c_device */
    p_dev_i2c = &(p_dev_rx8025t->i2c_dev);

    switch (p_dev_rx8025t->status) {

    /* 获取状态 */
    case __RX8025T_I2C_RD_STATUS:

        p_dev_rx8025t->read_int_time_status++;

        /* 加载下一状态 */
        p_dev_rx8025t->status = __RX8025T_I2C_WR_STATUS;

        am_i2c_mktrans(&p_dev_rx8025t->trans[0],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_WR),
                       (uint8_t *)&p_dev_rx8025t->sub_addr,
                       1);

        am_i2c_mktrans(&p_dev_rx8025t->trans[1],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_RD),
                       (uint8_t *)p_dev_rx8025t->buf,
                       p_dev_rx8025t->nbytes);

        am_i2c_mkmsg(&p_dev_rx8025t->msg,
                     p_dev_rx8025t->trans,
                     2,
                     (am_pfnvoid_t)__rx8025t_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_dev_i2c->handle, &p_dev_rx8025t->msg);
        break;

    /* 清除状态 */
    case __RX8025T_I2C_WR_STATUS:

        /* 加载下一状态 */
        p_dev_rx8025t->status = __RX8025T_I2C_DATA_PROCESSING;

        /* 处理数据，获取中断状态 */
        __rx8025t_read_write_data_processing(p_arg, &p_dev_rx8025t->intr_flag);

        am_i2c_mktrans(&p_dev_rx8025t->trans[0],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_WR),
                       (uint8_t *)&p_dev_rx8025t->sub_addr,
                       1);

        am_i2c_mktrans(&p_dev_rx8025t->trans[1],
                       p_dev_i2c->dev_addr,
                       (p_dev_i2c->dev_flags | AM_I2C_M_NOSTART | AM_I2C_M_WR),
                       (uint8_t *)p_dev_rx8025t->buf,
                       p_dev_rx8025t->nbytes);

        am_i2c_mkmsg(&p_dev_rx8025t->msg,
                     p_dev_rx8025t->trans,
                     2,
                     (am_pfnvoid_t)__rx8025t_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_dev_i2c->handle, &p_dev_rx8025t->msg);
        break;

    /* 数据处理 */
    case __RX8025T_I2C_DATA_PROCESSING:

        /* 加载下一状态 */
        p_dev_rx8025t->status = __RX8025T_I2C_RD_STATUS;

        /* 执行用户中断中断回调函数 */
        __rx8025t_callback_exec(p_arg, p_dev_rx8025t->intr_flag);

        if (p_dev_rx8025t->read_int_time_status == 1) {
            am_i2c_msg_start(p_dev_i2c->handle, &p_dev_rx8025t->msg);
        } else if (p_dev_rx8025t->read_int_time_status == 2){
            /* 启动软件定时器 */
            am_softimer_start(&p_dev_rx8025t->timer, 2);
        } else if (p_dev_rx8025t->read_int_time_status == 3){
            p_dev_rx8025t->read_int_time_status = 0;
            /* 开启GPIO中断 */
            am_gpio_trigger_on(p_dev_rx8025t->p_devinfo->int_pin);
        }
        break;

    default:
        break;
    }
}

/**
 * \brief RX8025T中断状态读取延时处理回调函数
 */
static void __softimer_cb (void *p_arg)
{
    am_rx8025t_dev_t *p_dev = (am_rx8025t_dev_t *)p_arg;

    am_softimer_stop(&p_dev->timer);
    __rx8025t_i2c_read_write_fsm(p_arg);
}


/**
 * \brief RX8025T 上网INTA中断回调函数
 * \param[in] p_arg : 中断回调函数参数
 * \return 无
 */
static void __rx8025t_eint_isr (void *p_arg)
{
    am_rx8025t_dev_t *p_dev = (am_rx8025t_dev_t *)p_arg;

    if (NULL == p_arg) {
        return;
    }

    /* 关闭GPIO中断 */
    am_gpio_trigger_off(p_dev->p_devinfo->int_pin);

    if (p_dev->status == __RX8025T_I2C_RD_STATUS) {
        /* 启动读写状态机 */
         __rx8025t_i2c_read_write_fsm(p_arg);
    }
}

/*****************************************************************************
 * 外部函数实现
 ****************************************************************************/

/*
 *将数据写入RAM中
 */
int am_rx8025t_ram_write (am_rx8025t_handle_t handle, uint8_t data)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf = data;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
    		           __RX8025T_REG_RAM,
                       &buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/*
 * 将RAM的数据读出来
 */
int am_rx8025t_ram_read (am_rx8025t_handle_t handle, uint8_t *pbuf)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 利用I2C传输数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_RAM,
                      pbuf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 软件复位RX8025T
 */
int am_rx8025t_software_reset (am_rx8025t_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {0x01};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_CONTROL,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief RX8025T 设备初始化
 */
am_rx8025t_handle_t am_rx8025t_init (am_rx8025t_dev_t           *p_dev,
                                     const am_rx8025t_devinfo_t *p_devinfo,
                                     am_i2c_handle_t             i2c_handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;
    int i;

    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == i2c_handle || NULL == p_devinfo) {
        return NULL;
    }

    /* 构造i2c设备 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 __RX8025T_ADDR,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 初始化设备信息 */
    p_dev->conn_stat = AM_FALSE;
    p_dev->p_devinfo = p_devinfo;

    for (i = 0; i < 3; i++) {
        p_dev->triginfo[i].pfn_callback = NULL;
        p_dev->triginfo[i].p_arg        = NULL;
    }
    p_dev->status   = __RX8025T_I2C_RD_STATUS;      /* 初始状态 */
    p_dev->sub_addr = __RX8025T_REG_FLAG;       /* 待操作的寄存器地址 */
    p_dev->nbytes   = 1;                             /* 需要读取的数据个数 */

    /* 初始化RX8025T设备 */
    ret = am_rx8025t_software_reset(p_dev);

    /* 初始化软件定时器 */
    am_softimer_init(&p_dev->timer, __softimer_cb, (void *)p_dev);

    p_dev->read_int_time_status = 0;

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
 * \brief RX8025T 设备解初始化
 */
void am_rx8025t_deinit (am_rx8025t_handle_t handle)
{

}

/**
 * \brief 获取RX8025T RTC标准服务
 */
am_rtc_handle_t am_rx8025t_rtc_init (am_rx8025t_handle_t  handle,
                                     am_rtc_serv_t       *p_rtc)
{
    p_rtc->p_drv   = handle;
    p_rtc->p_funcs = &__g_rtc_drv_funcs;
    return p_rtc;
}

/**
 * \brief 获取RX8025T ALARM_CLK标准服务
 */
am_alarm_clk_handle_t am_rx8025t_alarm_clk_init (am_rx8025t_handle_t   handle,
		                                         am_alarm_clk_serv_t  *p_alarm_clk)
{
	p_alarm_clk->p_drv   = handle;
	p_alarm_clk->p_funcs = &__g_alarm_clk_drv_funcs;
    return p_alarm_clk;
}

/**
 * \brief 对  更新中断、定时中断、闹钟中断 使能
 */
int am_rx8025t_int_enable (am_rx8025t_handle_t handle, uint8_t int_select)
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
                      __RX8025T_REG_CONTROL,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 中断使能 */
    buf[0] |= int_select;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
    		           __RX8025T_REG_CONTROL,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/**
 * \brief 对  更新中断、定时中断、闹钟中断 失能
 */
int am_rx8025t_int_disable (am_rx8025t_handle_t handle, uint8_t int_select)
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
                      __RX8025T_REG_CONTROL,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 中断失能 */
    buf[0] &= ~int_select;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_CONTROL,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}
/*
 *  使能定时器
 */
int am_rx8025t_timer_enable(am_rx8025t_handle_t  handle)
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
                      __RX8025T_REG_EXTENSION,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断使能 */
    buf[0] |= __RX8025T_TE_TIMER_ENABLE;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_EXTENSION,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/*
 *  失能定时器
 */
int am_rx8025t_timer_disable(am_rx8025t_handle_t  handle)
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
                      __RX8025T_REG_EXTENSION,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断使能 */
    buf[0] &= ~__RX8025T_TE_TIMER_ENABLE;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_EXTENSION,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/*
 * 设置定时器
 */
int am_rx8025t_timer_set (am_rx8025t_handle_t  handle, uint16_t count, uint8_t timer_clk)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t timer_buf[2]     = {0};
    uint8_t tsel_buf[1]      = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }
    if (count > 4095) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 设置count的值 */
    timer_buf[0] = (uint8_t)count;
    timer_buf[1] = (uint8_t)(count >> 8);
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_TIMER_COUNTER0,
                       timer_buf,
                       2);
    if (AM_OK != ret) {
        return ret;
    }

    /* 设置定时器的时钟 timer_clk */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_EXTENSION,
                      tsel_buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    tsel_buf[0] &= 0xfc;         /* 将低两位清零 */
    tsel_buf[0] |= timer_clk;

    /* 将闹钟中断信息写入控制状态寄存器1 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_EXTENSION,
                       tsel_buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 使能定时器 */
    ret = am_rx8025t_timer_enable(handle);
    if (AM_OK != ret) {
        return ret;
    }

    /* 开启定时器中断 */
    ret = am_rx8025t_int_enable(handle,AM_RX8025T_BF_TIE);
    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}
/*
 * 定时器回调函数设定
 */
int am_rx8025t_timer_cb_set( am_rx8025t_handle_t  handle,
                             am_pfnvoid_t         pfn_timer_callback,
                             void                *p_timer_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 设备信息指针 */
    const am_rx8025t_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取设备信息 */
    p_devinfo = handle->p_devinfo;

    if (-1 == p_devinfo->int_pin) {
        return -AM_ENOTSUP;
    }

    /* conn_stat用于记录当前中断连接状态,避免重复调用GPIO连接函数 */
    if (!(handle->conn_stat)) {

        /* 连接引脚中断回调函数 */
        ret = am_gpio_trigger_connect(p_devinfo->int_pin,
                                      __rx8025t_eint_isr,
                                      handle);
        if (AM_OK != ret) {
            return ret;
        }

        /* 设置为下降沿触发 */
        am_gpio_trigger_cfg(p_devinfo->int_pin, AM_GPIO_TRIGGER_FALL);
        am_gpio_trigger_on(p_devinfo->int_pin);

        /* 更新conn_stat状态为TRUE */
        handle->conn_stat = AM_TRUE;

    }

    /* 连接中断回调函数 */
    handle->triginfo[1].pfn_callback = pfn_timer_callback;
    handle->triginfo[1].p_arg        = p_timer_arg;

    /* 如果所有的回调函数都为空 */
    if ((handle->triginfo[0].pfn_callback == NULL) &&
        (handle->triginfo[1].pfn_callback == NULL) &&
        (handle->triginfo[2].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (handle->conn_stat) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->int_pin,
                                             __rx8025t_eint_isr,
                                             handle);

            if (AM_OK != ret) {
                return ret;
            }

            /* 关闭引脚触发 */
            am_gpio_trigger_off(p_devinfo->int_pin);

            /* 更新conn_stat状态为FALSE */
            handle->conn_stat = AM_FALSE;
        }
    }

    return AM_OK;
}

/*
 * 时间更新触发中断功能设置
 */
int am_rx8025t_time_update_set( am_rx8025t_handle_t  handle, uint8_t mode)
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

    /* 读取寄存器 */
    ret = am_i2c_read(p_i2c_dev,
    		          __RX8025T_REG_EXTENSION,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 更新中断选择 */
    if ( mode == AM_RX8025T_USEL_PER_MIN) {
        buf[0] |= __RX8025T_USEL_MIN_UPDATE;
    } else {
        buf[0] &= ~__RX8025T_USEL_MIN_UPDATE;
    }


    /* 将信息写入态寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_EXTENSION,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 使能时间更新中断 */
    ret = am_rx8025t_int_enable(handle,AM_RX8025T_BF_UIE);
    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}
/*
 * 更新中断回调函数设置
 */
int am_rx8025t_update_int_cb_set( am_rx8025t_handle_t  handle,
                                  am_pfnvoid_t         pfn_update_int_callback,
                                  void                *p_update_int_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 设备信息指针 */
    const am_rx8025t_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取设备信息 */
    p_devinfo = handle->p_devinfo;

    if (-1 == p_devinfo->int_pin) {
        return -AM_ENOTSUP;
    }

    /* conn_stat用于记录当前中断连接状态,避免重复调用GPIO连接函数 */
    if (!(handle->conn_stat)) {

        /* 连接引脚中断回调函数 */
        ret = am_gpio_trigger_connect(p_devinfo->int_pin,
                                      __rx8025t_eint_isr,
                                      handle);
        if (AM_OK != ret) {
            return ret;
        }

        /* 设置为下降沿触发 */
        am_gpio_trigger_cfg(p_devinfo->int_pin, AM_GPIO_TRIGGER_LOW);
        am_gpio_trigger_on(p_devinfo->int_pin);

        /* 更新conn_stat状态为TRUE */
        handle->conn_stat = AM_TRUE;

    }

    /* 连接中断回调函数 */
    handle->triginfo[0].pfn_callback = pfn_update_int_callback;
    handle->triginfo[0].p_arg        = p_update_int_arg;

    /* 如果所有的回调函数都为空 */
    if ((handle->triginfo[0].pfn_callback == NULL) &&
        (handle->triginfo[0].pfn_callback == NULL) &&
        (handle->triginfo[1].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (handle->conn_stat) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->int_pin,
                                             __rx8025t_eint_isr,
                                             handle);

            if (AM_OK != ret) {
                return ret;
            }

            /* 关闭引脚触发 */
            am_gpio_trigger_off(p_devinfo->int_pin);

            /* 更新conn_stat状态为FALSE */
            handle->conn_stat = AM_FALSE;
        }
    }

    return AM_OK;

}

/**
 * \brief RX8025T 闹钟回调函数和闹钟时间设置
 */
int am_rx8025t_alarm_set(am_rx8025t_handle_t      handle,
                         am_rx8025t_alarm_info_t *p_alarm_info,
                         am_pfnvoid_t             pfn_alarm_callback,
                         void                    *p_alarm_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[3]      = {0};
    uint8_t wada_buf[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 设备信息指针 */
    const am_rx8025t_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_devinfo = handle->p_devinfo;
    p_i2c_dev = &(handle->i2c_dev);

    if (-1 == p_devinfo->int_pin) {
        return -AM_ENOTSUP;
    }

    /* conn_stat用于记录当前中断连接状态,避免重复调用GPIO连接函数 */
    if (!(handle->conn_stat)) {

        /* 连接引脚中断回调函数 */
        ret = am_gpio_trigger_connect(p_devinfo->int_pin,
                                      __rx8025t_eint_isr,
                                      handle);
        if (AM_OK != ret) {
            return ret;
        }

        /* 设置为下降沿触发 */
        am_gpio_trigger_cfg(p_devinfo->int_pin, AM_GPIO_TRIGGER_FALL);
        am_gpio_trigger_on(p_devinfo->int_pin);

        /* 更新conn_stat_inta状态为TRUE */
        handle->conn_stat = AM_TRUE;

    }

    /* 连接中断回调函数 */
    handle->triginfo[2].pfn_callback = pfn_alarm_callback;
    handle->triginfo[2].p_arg        = p_alarm_arg;

    /* 如果所有的回调函数都为空 */
    if ((handle->triginfo[0].pfn_callback == NULL) &&
    	(handle->triginfo[1].pfn_callback == NULL) &&
        (handle->triginfo[2].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (handle->conn_stat) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->int_pin,
            	                        	 __rx8025t_eint_isr,
                                             handle);

            if (AM_OK != ret) {
                return ret;
            }

            /* 关闭引脚触发 */
            am_gpio_trigger_off(p_devinfo->int_pin);

            /* 更新conn_stat状态为FALSE */
            handle->conn_stat = AM_FALSE;
        }
    }

    /* 读取寄存器 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_EXTENSION,
                      wada_buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 更新中断选择 */
    if ( p_alarm_info->alarm_mode == AM_RX8025T_ALARM_DAY) {
    	wada_buf[0] |= __RX8025T_WADA_ALARM_DAY;
    } else {
    	wada_buf[0] &= ~__RX8025T_WADA_ALARM_DAY;
    }

    /* 将信息写入态寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_EXTENSION,
                       wada_buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    buf[__RX8025T_REG_MINUTES_ALARM  - __RX8025T_REG_MINUTES_ALARM] = AM_HEX_TO_BCD(p_alarm_info->min);
    buf[__RX8025T_REG_HOURS_ALARM    - __RX8025T_REG_MINUTES_ALARM] = AM_HEX_TO_BCD(p_alarm_info->hour);
    if( p_alarm_info->alarm_mode == AM_RX8025T_ALARM_DAY) {
        buf[__RX8025T_REG_WEEK_DAY_ALARM - __RX8025T_REG_MINUTES_ALARM] = AM_HEX_TO_BCD(p_alarm_info->week_day);
    } else {
        buf[__RX8025T_REG_WEEK_DAY_ALARM - __RX8025T_REG_MINUTES_ALARM] = p_alarm_info->week_day;
    }

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_MINUTES_ALARM,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }
    /* 使能ALARM闹钟*/
    ret = am_rx8025t_int_enable( handle, AM_RX8025T_BF_AIE);
    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 温度补偿时间间距
 */
int am_rx8025t_temperature_offset(am_rx8025t_handle_t handle,uint8_t offset_mode)
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

    /* 读取寄存器 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_EXTENSION,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    buf[0] &= 0x3f;         /* 将高两位清零 */
    buf[0] |= offset_mode << 6;

    /* 将信息写入态寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_CONTROL,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 *  \brief clkout 输出时钟频率的设置
 */
int am_rx8025t_clkout_set(am_rx8025t_handle_t handle, uint8_t frequency)
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

    /* 读取寄存器 */
    ret = am_i2c_read(p_i2c_dev,
                      __RX8025T_REG_EXTENSION,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    buf[0] &= 0xf3;         /* 将 bit3 和 bit2 两位清零 */
    buf[0] |= frequency << 2;

    /* 将信息写入态寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __RX8025T_REG_EXTENSION,
                       buf,
                       1);

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
 * \brief 关闭频率的输出
 */
int am_rx8025t_clkout_close(am_rx8025t_handle_t handle)
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

