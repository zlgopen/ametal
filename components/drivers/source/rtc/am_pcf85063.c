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
 * \brief PCF85063软件包实现
 *
 * \internal
 * \par Modification history
 * - 1.10 17-08-16  vir, add standard alarm_clk
 * - 1.00 16-08-05  sky, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_pcf85063.h"
#include "am_gpio.h"

/*****************************************************************************
 * 宏定义
 ****************************************************************************/

/** \brief PCF85063设备地址 */
#define __PCF85063_ADDR               0x51

/**
 * \name PCF85063 寄存器地址
 * \anchor grp_pcf85063_reg_addr
 * @{
 */


#define __PCF85063_REG_CTRL_STATUS1   0x00  /**< \brief 控制状态寄存器1 */
#define __PCF85063_REG_CTRL_STATUS2   0x01  /**< \brief 控制状态寄存器2 */

#define __PCF85063_REG_OFFSET         0x02  /**< \brief 修正寄存器 */
#define __PCF85063_REG_RAM_BYTE       0x03  /**< \brief RAM字节寄存器 */

#define __PCF85063_REG_VL_SECONDS     0x04  /**< \brief 时间-秒寄存器(SECONDS:0~59) */
#define __PCF85063_REG_MINUTES        0x05  /**< \brief 时间-分寄存器(MINUTES:0~59) */
#define __PCF85063_REG_HOURS          0x06  /**< \brief 时间-时寄存器(HOURS:0~23) */
#define __PCF85063_REG_DAYS           0x07  /**< \brief 日期-天寄存器(DAYS:1~31) */
#define __PCF85063_REG_WEEKDAYS       0x08  /**< \brief 日期-周几寄存器(WEEKDAYS:0~6) */
#define __PCF85063_REG_CENTURY_MONTHS 0x09  /**< \brief 日期-月寄存器(MONTHS:1~12) */
#define __PCF85063_REG_YEAERS         0x0A  /**< \brief 日期-年寄存器(YEARS:0~99) */

#define __PCF85063_REG_SECOND_ALARM   0x0B  /**< \brief 秒闹钟寄存器 */
#define __PCF85063_REG_MINUTE_ALARM   0x0C  /**< \brief 分钟闹钟寄存器 */
#define __PCF85063_REG_HOUR_ALARM     0x0D  /**< \brief 小时闹钟寄存器 */
#define __PCF85063_REG_DAY_ALARM      0x0E  /**< \brief 天闹钟寄存器 */
#define __PCF85063_REG_WEEKDAY_ALARM  0x0F  /**< \brief 周几闹钟寄存器 */

#define __PCF85063_REG_TIMER_VALUE    0x10  /**< \brief 定时器寄存器 */
#define __PCF85063_REG_TIMER_MODE     0x11  /**< \brief 定时器模式寄存器 */

/**
 * @}
 */

/**
 * \name PCF85063 寄存器位域描述
 * \anchor grp_pcf85063_bitfield_descriptions
 * @{
 */

#define __PCF85063_BF_STOP        0x20     /**< \brief RTC停止 */
#define __PCF85063_BF_12_24       0x02     /**< \brief 12/24小时制(0:24 1:12) */

#define __PCF85063_BF_AIE         0x80     /**< \brief 闹钟中断使能-AIE */
#define __PCF85063_BF_AF          0x40     /**< \brief 闹钟中断标志 */
#define __PCF85063_BF_MI          0x20     /**< \brief 分钟中断闹钟 */
#define __PCF85063_BF_HMI         0x10     /**< \brief 半分钟中断闹钟 */
#define __PCF85063_BF_TF          0x08     /**< \brief 定时器中断标志 */
#define __PCF85063_BF_COF         0x07     /**< \brief 时钟输出掩码 */

#define __PCF85063_BF_ALARM_DIS   0x80     /**< \brief 闹钟禁能 */

#define __PCF85063_BF_TIMER_TCF   0x18     /**< \brief 定时器时钟源位域掩码-TCF[1:0] */
#define __PCF85063_BF_TE          0x04     /**< \brief 定时器使能-TIE */
#define __PCF85063_BF_TIE         0x02     /**< \brief 定时器中断掩码-TIE */
#define __PCF85063_BF_TI_TP       0x01     /**< \brief 定时器中断TTL(0) or PULSE(1) */

/**
 * @}
 */

/**
 * \name PCF85063 I2C当前状态
 * \anchor grp_pcf85063_i2c_operation
 * @{
 */

#define __PCF85063_I2C_RD_C2_STATUS        0x00    /**< \brief 读取控制寄存器状态 */
#define __PCF85063_I2C_WR_C2_STATUS        0x01    /**< \brief 清除控制寄存器相应的状态位 */
#define __PCF85063_I2C_RD_WEEKDAYS         0x02    /**< \brief 读取周几寄存器的值 */
#define __PCF85063_I2C_WR_WEEKDAYS         0x03    /**< \brief 写入周几寄存器的值*/
#define __PCF85063_I2C_DATA_PROCESSING     0x04    /**< \brief 数据处理阶段 */

/**
 * @}
 */

/**
 * \name PCF85063 软件复位码
 * \note 需要使用软件复位时，直接将该复位码写入CS1寄存器
 * @{
 */

#define __PCF85063_SOFTWARE_RESET_CODE  0x58    /**< \brief 软件复位码 */

/**
 * @}
 */
/**
 * \name PCF85063 闹钟模式
 * @{
 */

#define __PCF85063_ALARM_MODE_STANDARD     0x01    /**< \brief 用户调用的是标准的闹钟接口 */
#define __PCF85063_ALARM_MODE_NONSTANDARD  0x02    /**< \brief 用户调用的是非标准的闹钟接口 */
/**
 * @}
 */

/*****************************************************************************
 * 静态声明
 ****************************************************************************/

/* RTC driver function implementation */
static int __pcf85063_time_set (void *p_drv, am_tm_t *p_tm);
static int __pcf85063_time_get (void *p_drv, am_tm_t *p_tm);

/**
 * \brief RTC driver functions definition
 */
static struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
    __pcf85063_time_set,
    __pcf85063_time_get
};

/* 函数声明 */
static void __pcf85063_eint_isr (void *p_arg);

/* ALARM_CLK driver function implementation */
static int __pcf85063_alarm_clk_time_set (void *p_drv, am_alarm_clk_tm_t *p_tm);
static int __pcf85063_alarm_clk_callback_set (void         *p_drv,
                                              am_pfnvoid_t  pfn_alarm_callback,
                                              void         *p_arg);
static int __pcf85063_alarm_clk_on (void *p_drv);
static int __pcf85063_alarm_clk_off (void *p_drv);

/**
 * \brief ALARM_CLK driver functions definition
 */
static struct am_alarm_clk_drv_funcs __g_alarm_clk_drv_funcs = {
    __pcf85063_alarm_clk_time_set,
    __pcf85063_alarm_clk_callback_set,
    __pcf85063_alarm_clk_on,
    __pcf85063_alarm_clk_off,
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
 * \brief 偏移补偿的基准
 */
static const float __offset_lsb_ppm[2] = {
    4.34,       /* 正常模式下的ppm基准 */
    4.069       /* 快速模式下的ppm基准 */
};

/*****************************************************************************
 * 内部函数实现
 ****************************************************************************/

/**
 * \brief 判断年份是否为闰年
 *
 * \param[in] year : 年份
 *
 * \retval 1 : 闰年
 * \retval 0 : 平年
 */
static uint8_t __pcf85063_leap_year_check (uint16_t year)
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
static uint8_t __pcf85063_day_of_year (uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t i;

    if (__pcf85063_leap_year_check(year)) {
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
int __pcf85063_time_validator (am_tm_t *p_tm)
{
    if (p_tm->tm_year > 199 || p_tm->tm_year < 0 ||
        p_tm->tm_mon > 11   || p_tm->tm_mon < 0 ||
        p_tm->tm_mday >
        __mdays[__pcf85063_leap_year_check(1900 + p_tm->tm_year)][p_tm->tm_mon] ||
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
 * \brief PCF85063设备初始化
 * \param[in] p_dev : 指向PCF85063设备结构体
 * \return AM_OK表示初始化成功, 其他失败
 */
static int __pcf85063_init (am_pcf85063_dev_t *p_dev)
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
                       __PCF85063_REG_CTRL_STATUS2,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
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
static int __pcf85063_time_set (void *p_drv, am_tm_t *p_tm)
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
    if ((AM_OK != __pcf85063_time_validator(p_tm)) ||
        (p_tm->tm_hour > 23) || (p_tm->tm_hour < 0)) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_pcf85063_dev_t *)p_drv)->i2c_dev);

    /* 将时间日期信息填入buf中 */
    buf[__PCF85063_REG_VL_SECONDS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_sec);
    buf[__PCF85063_REG_MINUTES - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_min);
    buf[__PCF85063_REG_HOURS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_hour);
    buf[__PCF85063_REG_DAYS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_mday);
    buf[__PCF85063_REG_WEEKDAYS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_wday);
    buf[__PCF85063_REG_CENTURY_MONTHS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_mon + 1);
    buf[__PCF85063_REG_YEAERS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_year);

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_VL_SECONDS,
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
static int __pcf85063_time_get (void *p_drv, am_tm_t *p_tm)
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

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_pcf85063_dev_t *)p_drv)->i2c_dev);

    /* 通过I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_VL_SECONDS,
                      buf,
                      sizeof(buf));

    if (AM_OK != ret) {
        return -AM_EINVAL;
    }

    /** 从buf中获取时间日期信息 */
    p_tm->tm_sec  =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_VL_SECONDS - __PCF85063_REG_VL_SECONDS] & 0x7F);
    p_tm->tm_min  =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_MINUTES - __PCF85063_REG_VL_SECONDS] & 0x7F);
    p_tm->tm_hour =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_HOURS - __PCF85063_REG_VL_SECONDS] & 0x3F);
    p_tm->tm_mday =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_DAYS - __PCF85063_REG_VL_SECONDS] & 0x3F);
    p_tm->tm_wday =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_WEEKDAYS - __PCF85063_REG_VL_SECONDS] & 0x07);
    p_tm->tm_mon  =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_CENTURY_MONTHS - __PCF85063_REG_VL_SECONDS] & 0x1F) - 1;
    p_tm->tm_year =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_YEAERS - __PCF85063_REG_VL_SECONDS]);
    p_tm->tm_yday = __pcf85063_day_of_year(p_tm->tm_year, p_tm->tm_mon, p_tm->tm_mday);
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
static int __pcf85063_alarm_clk_time_set (void *p_drv, am_alarm_clk_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    uint8_t temp;      /* 临时存放闹钟时间wday的值 */
    uint8_t count;     /* 用来存放今天或未来最近的星期几开启了闹钟 */
    uint8_t flag = 1;

    /* 数据缓冲区 */
    uint8_t buf[4] = {0};
    am_tm_t current_tm;
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 验证时间信息的有效性 */
    if ((p_tm->min  > 59) || (p_tm->min  < 0) ||
        (p_tm->hour > 23) || (p_tm->hour < 0) ||
        (( p_tm->wdays & 0x7F) == 0 )) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(((am_pcf85063_dev_t *)p_drv)->i2c_dev);

    /* 将闹钟的wdays数据存放起来 */
    ((am_pcf85063_dev_t *)p_drv)->alarm_clk_wdays = p_tm->wdays;

    /* 标记用户调用的是非标准的闹钟接口 */
    ((am_pcf85063_dev_t *)p_drv)->alarm_mode = __PCF85063_ALARM_MODE_NONSTANDARD;

    /* 获取当前时间 */
    __pcf85063_time_get(p_drv, &current_tm);

    /* 将时间日期信息填入buf中  ，最高位为0表示使能该寄存器 */
    buf[0] = AM_HEX_TO_BCD(p_tm->min);
    buf[1] = AM_HEX_TO_BCD(p_tm->hour);
    buf[2] = 0X80;                           /* 关闭天闹钟 */

    /*
     * \brief 判断今天的时间是否超过闹钟的时间
     * \note flag =1 过了闹钟时间，今天闹钟无效，直接从明天开始判断
     *       flag =0 还没有过闹钟时间，今天闹钟有效，从今天开始判断
     */
    flag = 1;
    if ( p_tm->hour > current_tm.tm_hour ) {
       flag = 0;    /* 今天的闹钟有效 */
    } else if ( p_tm->hour == current_tm.tm_hour ) {
                if (  p_tm->min > current_tm.tm_min ) {
                     flag = 0;   /* 今天的闹钟有效 */
                }
    }
    temp = p_tm->wdays;                   /* 闹钟星期值存放到临时变量中运算  */
    count = current_tm.tm_wday + flag;    /* 将今天或者明天的星期值存放起来 */
    temp = temp >> count;                 /* 将闹钟时间wday右移使得最低位代表今天或明天的星期几 */

    while (! (temp & 0x1) ) {
        temp = temp >> 1;                /* 若今天或者明天没有设置闹钟，那继续查看明天或后天 */
        count++;
        if( 7 == count) {                /* 判断到星期六还没有设置闹钟的星期值，又从星期天开始判断 */
            count = 0;
            temp  = p_tm->wdays;
        }
    }

    buf[3] = count;  /* 最近要闹钟的星期天数 */



    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_MINUTE_ALARM,
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
static int __pcf85063_alarm_clk_callback_set (void         *p_drv,
                                              am_pfnvoid_t  pfn_alarm_callback,
                                              void         *p_arg)
{
	/* 函数调用返回值 */
    int ret = AM_OK;

    /* PCF85063 设备指针 */
    am_pcf85063_handle_t  pcf85063_handle = NULL;

    /* 设备信息指针 */
    const am_pcf85063_devinfo_t *p_devinfo = NULL;

    /* 检验参数的有效性 */
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    pcf85063_handle = (am_pcf85063_handle_t)p_drv;
    p_devinfo       = pcf85063_handle->p_devinfo;

    if (-1 == p_devinfo->int_pin) {
        return -AM_ENOTSUP;
    }

    /* conn_stat用于记录当前中断连接状态,避免重复调用GPIO连接函数 */
    if (!(pcf85063_handle->conn_stat)) {

        /* 连接引脚中断回调函数 */
        ret = am_gpio_trigger_connect(p_devinfo->int_pin,
        		                      __pcf85063_eint_isr,
									  pcf85063_handle);
        if (AM_OK != ret) {
            return ret;
        }

        /* 设置为下降沿触发 */
        am_gpio_trigger_cfg(p_devinfo->int_pin, AM_GPIO_TRIGGER_FALL);
        am_gpio_trigger_on(p_devinfo->int_pin);

        /* 更新conn_stat_inta状态为TRUE */
        pcf85063_handle->conn_stat = AM_TRUE;
    }

    /* 连接中断回调函数 */
    pcf85063_handle->triginfo[1].pfn_callback = pfn_alarm_callback;
    pcf85063_handle->triginfo[1].p_arg        = p_arg;

    /* 如果所有的回调函数都为空 */
    if ((pcf85063_handle->triginfo[0].pfn_callback == NULL) &&
    	(pcf85063_handle->triginfo[1].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (pcf85063_handle->conn_stat) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->int_pin,
                                             __pcf85063_eint_isr,
                                             pcf85063_handle);

            if (AM_OK != ret) {
                return ret;
            }

            /* 关闭引脚触发 */
            am_gpio_trigger_off(p_devinfo->int_pin);

            /* 更新conn_stat状态为FALSE */
            pcf85063_handle->conn_stat = AM_FALSE;
        }
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
static int __pcf85063_alarm_clk_on (void *p_drv)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1]     = {0};
    uint8_t buf_cs2[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    /* 从p_drv中获取i2c设备指针 */
    p_i2c_dev = &(((am_pcf85063_dev_t *)p_drv)->i2c_dev);

    buf[0] = __PCF85063_BF_ALARM_DIS;
     /* 关闭秒闹钟 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_SECOND_ALARM,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 关闭天闹钟 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_DAY_ALARM,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 读取控制状态寄存器2(用来控制闹钟中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS2,
                      buf_cs2,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断使能 */
    buf_cs2[0] |= __PCF85063_BF_AIE;

    /* 清除AF标志 */
    buf_cs2[0] &= ~__PCF85063_BF_AF;

    /* 将闹钟中断信息写入控制状态寄存器2 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS2,
                       buf_cs2,
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
static int __pcf85063_alarm_clk_off (void *p_drv)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[5]     = {0};
    uint8_t buf_cs2[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    /* 从p_drv中获取i2c设备指针 */
    p_i2c_dev = &(((am_pcf85063_dev_t *)p_drv)->i2c_dev);

    /* 关闭秒、分、小时、天、星期闹钟 */
    buf[0] = __PCF85063_BF_ALARM_DIS;
    buf[1] = __PCF85063_BF_ALARM_DIS;
    buf[2] = __PCF85063_BF_ALARM_DIS;
    buf[3] = __PCF85063_BF_ALARM_DIS;
    buf[4] = __PCF85063_BF_ALARM_DIS;

    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_DAY_ALARM,
                       buf,
                       sizeof (buf));

    if (AM_OK != ret) {
        return ret;
    }

    /* 读取控制状态寄存器2(用来控制闹钟中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS2,
                      buf_cs2,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断失能 */
    buf_cs2[0] &= ~__PCF85063_BF_AIE;

    /* 清除AF标志 */
    buf_cs2[0] &= ~__PCF85063_BF_AF;

    /* 将闹钟中断信息写入控制状态寄存器2 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS2,
                       buf_cs2,
                       1);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/**
 * \brief 根据中断标志来执行用户中断回调函数
 *
 * \param[in] p_dev     : 指向PCF85063设备的指针
 * \param[in] intr_flag : 中断标志
 *
 * \return 无
 */
static void __pcf85063_callback_exec (void    *p_drv,
                                      uint8_t  intr_flag)
{
    am_pcf85063_dev_t *p_dev        = NULL;
    am_pfnvoid_t       pfn_callback = NULL;
    void              *p_arg        = NULL;

    /* 检验参数有效性 */
    if (0 == intr_flag || NULL == p_drv) {
        return;
    }

    p_dev = (am_pcf85063_dev_t *)p_drv;

    /* 如果是TIMER中断，则执行对应用户中断回调函数 */
    if (intr_flag & __PCF85063_BF_TF) {
        pfn_callback = p_dev->triginfo[0].pfn_callback;
        p_arg        = p_dev->triginfo[0].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }

    /* 如果是ALARM中断，则执行对应用户中断回调函数 */
    if (intr_flag & __PCF85063_BF_AF) {
        pfn_callback = p_dev->triginfo[1].pfn_callback;
        p_arg        = p_dev->triginfo[1].p_arg;
        if (NULL != pfn_callback) {
            pfn_callback(p_arg);
        }
    }
}

/**
 * \brief 在中断中读取数据后的处理函数,判断中断类型
 *
 * \param[in,out] buf         : 数据缓冲区
 * \param[in]     p_intr_flag : 中断标志  参见 \ref grp_pcf85063_intr_flag
 *
 * \return 无
 *
 * \note 函数中buf 对应 CTRL_STATUS2寄存器 //shauew buf寄存器是否对应
 */
static void __pcf85063_read_write_data_processing (void    *p_arg,
                                                   uint8_t *p_intr_flag)
{
    am_pcf85063_dev_t *p_dev = NULL;

    if (NULL == p_arg) {
        return;
    }

    p_dev = (am_pcf85063_dev_t *)p_arg;

    *p_intr_flag = 0;

    /* 判断是否为TF标志置位 */
    if (p_dev->buf[0] & __PCF85063_BF_TF) {

        /* 记录TIMER中断标志 */
        *p_intr_flag |= __PCF85063_BF_TF;

        /* 清除TF标志 */
        p_dev->buf[0] &= ~__PCF85063_BF_TF;
    }

    /* 判断是否为AF标志置位 */
    if (p_dev->buf[0] & __PCF85063_BF_AF) {
        if (p_dev->buf[0] & __PCF85063_BF_AIE) {
            /* 记录闹钟中断标志,并清除AF */
            *p_intr_flag |= __PCF85063_BF_AF;
            p_dev->buf[0] &= ~__PCF85063_BF_AF;
        }
    }
}


static void __pcf85063_get_next_wday_processing (void *p_arg)
{
    am_pcf85063_dev_t *p_dev = NULL;
    
    uint8_t            temp;      /* 临时存放闹钟时间wday的值  */
    uint8_t            count;     /* 用来存放今天或未来最近的星期几开启了闹钟 */
    
    if (NULL == p_arg) {
        return;
    }

    p_dev = (am_pcf85063_dev_t *)p_arg;
    if ( p_dev->alarm_mode == __PCF85063_ALARM_MODE_NONSTANDARD) {
        return;
    }
 
    temp = p_dev->alarm_clk_wdays;   /* 闹钟星期值存放到临时变量中运算  */
    count = p_dev->buf[0] + 1;    /* 将明天的星期值存放起来 */
    temp = temp >> count;                    /* 将闹钟时间wday右移使得最低位代表明天的星期几 */

    while (! (temp & 0x1) ) {
        temp = temp >> 1;                /* 若明天没有设置闹钟，那继续查看后天 */
        count++;
        if( 7 == count) {                /* 判断到星期六还没有设置闹钟的星期值，又从星期天开始判断 */
            count = 0;
            temp  =  p_dev->alarm_clk_wdays;
        }
    }
    p_dev->buf[0] = count;
}
/**
 * \brief I2C读写过程中的状态机
 * \param[in] p_arg : 参数
 * \return 无
 * \note 状态变换顺序   [1]处理开始 --> [2]读取subaddr[0]的值 --> [3]处理数据，并写入subaddr[0]
 *     --> [4] 读取subaddr[1]的值--> [5]处理数据，并写入subaddr[1]--> [6]处理数据--> [7]处理完成
 */
static void __pcf85063_i2c_read_write_fsm (void *p_arg)
{
    /* I2C设备 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* PCF85063设备  */
    am_pcf85063_dev_t *p_pcf85063_dev = NULL;

    /* 判断参数的有效性 */
    if (NULL == p_arg) {
        return;
    }

    p_pcf85063_dev = (am_pcf85063_dev_t *)p_arg;

    /*获取i2c_device */
    p_i2c_dev = &(p_pcf85063_dev->i2c_dev);

    switch (p_pcf85063_dev->status) {

    /* 获取控制寄存器2的值 */
    case __PCF85063_I2C_RD_C2_STATUS:

        /* 加载下一状态 */
        p_pcf85063_dev->status = __PCF85063_I2C_WR_C2_STATUS;

        am_i2c_mktrans(&p_pcf85063_dev->trans[0],
                       p_i2c_dev->dev_addr,
                      (p_i2c_dev->dev_flags | AM_I2C_M_WR),
                      (uint8_t *)&p_pcf85063_dev->sub_addr[0],
                       1);

        am_i2c_mktrans(&p_pcf85063_dev->trans[1],
                       p_i2c_dev->dev_addr,
                      (p_i2c_dev->dev_flags | AM_I2C_M_RD),
                      (uint8_t *)p_pcf85063_dev->buf,
                      p_pcf85063_dev->nbytes);

        am_i2c_mkmsg(&p_pcf85063_dev->msg,
                     p_pcf85063_dev->trans,
                     2,
                     (am_pfnvoid_t)__pcf85063_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_i2c_dev->handle, &p_pcf85063_dev->msg);
        break;

    /* 清除控制寄存器2相应的状态 */
    case __PCF85063_I2C_WR_C2_STATUS:

        /* 加载下一状态 */
        p_pcf85063_dev->status = __PCF85063_I2C_RD_WEEKDAYS;

        /* 处理数据，获取中断状态 */
        __pcf85063_read_write_data_processing(p_arg, &p_pcf85063_dev->intr_flag);

        am_i2c_mktrans(&p_pcf85063_dev->trans[0],
                       p_i2c_dev->dev_addr,
                      (p_i2c_dev->dev_flags | AM_I2C_M_WR),
                      (uint8_t *)&p_pcf85063_dev->sub_addr[0],
                       1);

        am_i2c_mktrans(&p_pcf85063_dev->trans[1],
                        p_i2c_dev->dev_addr,
                       (p_i2c_dev->dev_flags | AM_I2C_M_NOSTART | AM_I2C_M_WR),
                       (uint8_t *)p_pcf85063_dev->buf,
                       p_pcf85063_dev->nbytes);

        am_i2c_mkmsg(&p_pcf85063_dev->msg,
                     p_pcf85063_dev->trans,
                     2,
                     (am_pfnvoid_t)__pcf85063_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_i2c_dev->handle, &p_pcf85063_dev->msg);
        break;

     /* 获取星期寄存器的值  */
    case __PCF85063_I2C_RD_WEEKDAYS:

        /* 加载下一状态 */
        p_pcf85063_dev->status = __PCF85063_I2C_WR_WEEKDAYS;

        am_i2c_mktrans(&p_pcf85063_dev->trans[0],
                       p_i2c_dev->dev_addr,
                      (p_i2c_dev->dev_flags | AM_I2C_M_WR),
                      (uint8_t *)&p_pcf85063_dev->sub_addr[1],
                       1);

        am_i2c_mktrans(&p_pcf85063_dev->trans[1],
                       p_i2c_dev->dev_addr,
                      (p_i2c_dev->dev_flags | AM_I2C_M_RD),
                      (uint8_t *)p_pcf85063_dev->buf,
                      p_pcf85063_dev->nbytes);

        am_i2c_mkmsg(&p_pcf85063_dev->msg,
                     p_pcf85063_dev->trans,
                     2,
                     (am_pfnvoid_t)__pcf85063_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_i2c_dev->handle, &p_pcf85063_dev->msg);
        break;

    /* 设置星期寄存器  */
    case __PCF85063_I2C_WR_WEEKDAYS:

        /* 加载下一状态 */
        p_pcf85063_dev->status = __PCF85063_I2C_DATA_PROCESSING;

        /* 获取下一个要闹钟的星期几*/
        __pcf85063_get_next_wday_processing(p_arg);

        am_i2c_mktrans(&p_pcf85063_dev->trans[0],
                       p_i2c_dev->dev_addr,
                      (p_i2c_dev->dev_flags | AM_I2C_M_WR),
                      (uint8_t *)&p_pcf85063_dev->sub_addr[1],
                       1);

        am_i2c_mktrans(&p_pcf85063_dev->trans[1],
                        p_i2c_dev->dev_addr,
                       (p_i2c_dev->dev_flags | AM_I2C_M_NOSTART | AM_I2C_M_WR),
                       (uint8_t *)p_pcf85063_dev->buf,
                       p_pcf85063_dev->nbytes);

        am_i2c_mkmsg(&p_pcf85063_dev->msg,
                     p_pcf85063_dev->trans,
                     2,
                     (am_pfnvoid_t)__pcf85063_i2c_read_write_fsm,
                     p_arg);

        am_i2c_msg_start(p_i2c_dev->handle, &p_pcf85063_dev->msg);
        break;

        /* 数据处理 */
    case __PCF85063_I2C_DATA_PROCESSING:

        /* 加载下一状态 */
        p_pcf85063_dev->status = __PCF85063_I2C_RD_C2_STATUS;

        /* 执行用户中断中断回调函数 */
        __pcf85063_callback_exec(p_arg, p_pcf85063_dev->intr_flag);

        /* 开启GPIO中断 */
        am_gpio_trigger_on(p_pcf85063_dev->p_devinfo->int_pin);
        break;

    default:
        break;
    }
}

/**
 * \brief PCF85063中断回调函数
 * \param[in] p_arg : 中断回调函数参数
 * \return 无
 */
static void __pcf85063_eint_isr (void *p_arg)
{
    am_pcf85063_dev_t        *p_dev        = (am_pcf85063_dev_t *)p_arg;

    if (NULL == p_arg) {
        return;
    }

    /* 关闭GPIO中断 */
    am_gpio_trigger_off(p_dev->p_devinfo->int_pin);

    if (p_dev->status == __PCF85063_I2C_RD_C2_STATUS) {

        /* 启动读写状态机 */
        __pcf85063_i2c_read_write_fsm(p_arg);
    }
}

/*****************************************************************************
 * 外部函数实现
 ****************************************************************************/

/**
 * \brief PCF85063 设备初始化
 */
am_pcf85063_handle_t am_pcf85063_init (am_pcf85063_dev_t           *p_dev,
                                       const am_pcf85063_devinfo_t *p_devinfo,
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
                 __PCF85063_ADDR,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 初始化设备信息 */
    p_dev->conn_stat = AM_FALSE;
    p_dev->p_devinfo = p_devinfo;

    for (i = 0; i < 2; i++) {
        p_dev->triginfo[i].pfn_callback = NULL;
        p_dev->triginfo[i].p_arg        = NULL;
    }
    p_dev->status      = __PCF85063_I2C_RD_C2_STATUS;   /* 初始状态 */
    p_dev->sub_addr[0] = __PCF85063_REG_CTRL_STATUS2;   /* 待操作的控制寄存器地址 */
    p_dev->sub_addr[1] = __PCF85063_REG_WEEKDAYS;       /* 待操作的星期寄存器地址 */
    p_dev->nbytes   = 1;                                /* 需要读取的数据个数 */

    /* 初始化PCF85063设备 */
    ret = __pcf85063_init(p_dev);
    if (AM_OK != ret) {
        return NULL;
    }
    /* 初始化clk_en_pin 的GPIO口 */
    if (p_dev->p_devinfo->clk_en_pin != -1) {
         am_gpio_pin_cfg(p_dev->p_devinfo->clk_en_pin, AM_GPIO_PUSH_PULL);  /* 配置为推挽输出 */
         am_gpio_set(p_dev->p_devinfo->clk_en_pin, AM_GPIO_LEVEL_LOW);
    }
    return p_dev;
}



/**
 * \brief PCF85063 设备解初始化
 */
void am_pcf85063_deinit (am_pcf85063_handle_t handle)
{

}

/**
 * \brief 获取PCF85063 RTC标准服务
 */
am_rtc_handle_t am_pcf85063_rtc_init (am_pcf85063_handle_t  handle,
                                      am_rtc_serv_t        *p_rtc)
{
    p_rtc->p_drv   = handle;
    p_rtc->p_funcs = &__g_rtc_drv_funcs;
    return p_rtc;
}

/**
 * \brief 获取PCF85063 ALARM_CLK标准服务
 */
am_alarm_clk_handle_t am_pcf85063_alarm_clk_init (am_pcf85063_handle_t   handle,
		                                         am_alarm_clk_serv_t    *p_alarm_clk)
{
	p_alarm_clk->p_drv   = handle;
	p_alarm_clk->p_funcs = &__g_alarm_clk_drv_funcs;
    return p_alarm_clk;
}

/**
 * \brief 软件复位PCF85063
 */
int am_pcf85063_software_reset (am_pcf85063_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {__PCF85063_SOFTWARE_RESET_CODE};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 中止PCF85063
 */
int am_pcf85063_stop (am_pcf85063_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 读取CS1 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS1,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 置STOP位 */
    buf[0] |= __PCF85063_BF_STOP;

    /* 写STOP到CS1 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 启动PCF85063(复位时PCF85063已启动，该接口主要是针对 am_pcf85063_stop())
 */
int am_pcf85063_start (am_pcf85063_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 读取CS1 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS1,
                      buf,
                      1);

    /* 清STOP位 */
    buf[0] &= ~__PCF85063_BF_STOP;

    /* 清除CS1中的STOP */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS1,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 写 PCF85063 RAM_Byte 寄存器
 */
int am_pcf85063_ram_write (am_pcf85063_handle_t handle, uint8_t data)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_RAM_BYTE,
                       &data,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 读 PCF85063 RAM_Byte 寄存器
 */
int am_pcf85063_ram_read (am_pcf85063_handle_t handle, uint8_t *p_data)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 利用I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_RAM_BYTE,
                      p_data,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief PCF85063时间日期设置函数
 */
int am_pcf85063_time_set (am_pcf85063_handle_t handle, am_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[7]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 验证时间信息的有效性 */
    if (AM_OK != __pcf85063_time_validator(p_tm)) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 将时间日期信息填入buf中 */
    buf[__PCF85063_REG_VL_SECONDS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_sec);
    buf[__PCF85063_REG_MINUTES - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_min);
    buf[__PCF85063_REG_HOURS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_hour);
    buf[__PCF85063_REG_DAYS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_mday);
    buf[__PCF85063_REG_WEEKDAYS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_wday);
    buf[__PCF85063_REG_CENTURY_MONTHS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_mon + 1);
    buf[__PCF85063_REG_YEAERS - __PCF85063_REG_VL_SECONDS] =
                                             AM_HEX_TO_BCD(p_tm->tm_year);

    /* 利用I2C传输时间以及日期数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_VL_SECONDS,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return -ret;
    }

    return AM_OK;
}

/**
 * \brief PCF85063时间日期获取函数
 */
int am_pcf85063_time_get (am_pcf85063_handle_t handle, am_tm_t *p_tm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[7]     = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle || NULL == p_tm) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 通过I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_VL_SECONDS,
                      buf,
                      sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    /* 从buf中获取时间日期信息 */
    p_tm->tm_sec  =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_VL_SECONDS - __PCF85063_REG_VL_SECONDS] & 0x7F);
    p_tm->tm_min  =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_MINUTES - __PCF85063_REG_VL_SECONDS] & 0x7F);
    p_tm->tm_hour =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_HOURS - __PCF85063_REG_VL_SECONDS] & 0x3F);
    p_tm->tm_mday =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_DAYS - __PCF85063_REG_VL_SECONDS] & 0x3F);
    p_tm->tm_wday =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_WEEKDAYS - __PCF85063_REG_VL_SECONDS] & 0x07);
    p_tm->tm_mon  =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_CENTURY_MONTHS - __PCF85063_REG_VL_SECONDS] & 0x1F) - 1;
    p_tm->tm_year =
    AM_BCD_TO_HEX(buf[__PCF85063_REG_YEAERS - __PCF85063_REG_VL_SECONDS]);
    p_tm->tm_yday = __pcf85063_day_of_year(p_tm->tm_year, p_tm->tm_mon, p_tm->tm_mday);

    return AM_OK;
}

/**
 * \brief PCF85063时钟源补偿设置
 */
int am_pcf85063_offset_set (am_pcf85063_handle_t handle, uint8_t mode, float ppm)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {0};

    /* 偏移量 */
    float  offset_val = 0.0;
    int8_t result     = 0;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 检验参数的有效性 */
    if (NULL == handle || ((mode != AM_PCF85063_OFFSET_MODE_NORMAL)
                       && (mode != AM_PCF85063_OFFSET_MODE_COURSE))) {
        return -AM_EINVAL;
    }

    if ((mode == AM_PCF85063_OFFSET_MODE_NORMAL)
              && ((ppm > 273.420f) || (ppm < -277.760f))) {
        return -AM_EINVAL;
    }

    if ((mode == AM_PCF85063_OFFSET_MODE_COURSE)
              && ((ppm > 256.347f) || (ppm < -260.416f))) {
        return -AM_EINVAL;
    }

    /* 从设备结构体中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 对偏移量进行计算 */
    offset_val = ppm / __offset_lsb_ppm[mode];

    /* 正负数处理 */
    if (offset_val > 0) {

        /* 对偏移量进行四舍五入 */
        result = offset_val + 0.5f;

    } else {

        /* 对偏移量进行四舍五入 */
        result = offset_val - 0.5f;

        /* 符号位为第7位 */
        result = (result & 0x3F) | 0x40;
    }

    /* 将计算结果填入buf */
    buf[0] = mode << 7 | result;

    /* 通过I2C读取数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_OFFSET,
                       buf,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 使能PCF85063
 */
int am_pcf85063_alarm_enable (am_pcf85063_handle_t      handle,
                              am_pcf85063_alarm_info_t *p_alarm_info)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[5]     = {0};
    uint8_t buf_cs2[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle || NULL == p_alarm_info) {
        return -AM_EINVAL;
    }

    /* 验证闹钟信息设置值的有效性 */
    if (p_alarm_info->mday > 31 || p_alarm_info->mday < 0 ||
        p_alarm_info->wday > 6  || p_alarm_info->wday < 0 ||
        p_alarm_info->hour > 23 || p_alarm_info->hour < 0 ||
        p_alarm_info->min  > 59 || p_alarm_info->min < 0 ||
        p_alarm_info->sec  > 59 || p_alarm_info->sec < 0 ) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 标记用户调用的是非标准的闹钟接口 */
    handle->alarm_mode = __PCF85063_ALARM_MODE_NONSTANDARD;

    /* 填入ALARM_SECOND信息 */
    if (p_alarm_info->enable & AM_PCF85063_ALARM_SECOND_ENABLE) {

        /* 使能并设置闹钟值 */
        buf[__PCF85063_REG_SECOND_ALARM - __PCF85063_REG_SECOND_ALARM] &=
                                                 ~__PCF85063_BF_ALARM_DIS;
        buf[__PCF85063_REG_SECOND_ALARM - __PCF85063_REG_SECOND_ALARM] |=
         (AM_HEX_TO_BCD(p_alarm_info->sec) & 0x7F);

    } else {

        /* 禁能闹钟 */
        buf[__PCF85063_REG_SECOND_ALARM - __PCF85063_REG_SECOND_ALARM] |=
                                                    __PCF85063_BF_ALARM_DIS;
    }

    /* 填入ALARM_MINUTE信息 */
    if (p_alarm_info->enable & AM_PCF85063_ALARM_MINUTE_ENABLE) {

        /* 使能并设置闹钟值 */
        buf[__PCF85063_REG_MINUTE_ALARM - __PCF85063_REG_SECOND_ALARM] &=
                                                 ~__PCF85063_BF_ALARM_DIS;
        buf[__PCF85063_REG_MINUTE_ALARM - __PCF85063_REG_SECOND_ALARM] |=
         (AM_HEX_TO_BCD(p_alarm_info->min) & 0x7F);

    } else {

        /* 禁能闹钟 */
        buf[__PCF85063_REG_MINUTE_ALARM - __PCF85063_REG_SECOND_ALARM] |=
                                                    __PCF85063_BF_ALARM_DIS;
    }

    /* 填入ALARM_HOUR信息 */
    if (p_alarm_info->enable & AM_PCF85063_ALARM_HOUR_ENABLE) {

        /* 使能并设置闹钟值 */
        buf[__PCF85063_REG_HOUR_ALARM - __PCF85063_REG_SECOND_ALARM] &=
                                                 ~__PCF85063_BF_ALARM_DIS;
        buf[__PCF85063_REG_HOUR_ALARM - __PCF85063_REG_SECOND_ALARM] |=
         (AM_HEX_TO_BCD(p_alarm_info->hour) & 0x3F);

    } else {

        /* 禁能闹钟 */
        buf[__PCF85063_REG_HOUR_ALARM - __PCF85063_REG_SECOND_ALARM] |=
                                                    __PCF85063_BF_ALARM_DIS;
    }

    /* 填入ALARM_DAY信息 */
    if (p_alarm_info->enable & AM_PCF85063_ALARM_DAY_ENABLE) {

        /* 使能并设置闹钟值 */
        buf[__PCF85063_REG_DAY_ALARM - __PCF85063_REG_SECOND_ALARM] &=
                                                 ~__PCF85063_BF_ALARM_DIS;
        buf[__PCF85063_REG_DAY_ALARM - __PCF85063_REG_SECOND_ALARM] |=
         (AM_HEX_TO_BCD(p_alarm_info->mday) & 0x3F);

    } else {

        /* 禁能闹钟 */
        buf[__PCF85063_REG_DAY_ALARM - __PCF85063_REG_SECOND_ALARM] |=
                                                    __PCF85063_BF_ALARM_DIS;
    }

    /* 填入ALARM_WEEKDAY信息 */
    if (p_alarm_info->enable & AM_PCF85063_ALARM_WEEKDAY_ENABLE) {

        /* 使能并设置闹钟值 */
        buf[__PCF85063_REG_WEEKDAY_ALARM - __PCF85063_REG_SECOND_ALARM] &=
                                                 ~__PCF85063_BF_ALARM_DIS;
        buf[__PCF85063_REG_WEEKDAY_ALARM - __PCF85063_REG_SECOND_ALARM] |=
         (AM_HEX_TO_BCD(p_alarm_info->wday) & 0x07);

    } else {

        /* 禁能闹钟 */
        buf[__PCF85063_REG_WEEKDAY_ALARM - __PCF85063_REG_SECOND_ALARM] |=
                                                    __PCF85063_BF_ALARM_DIS;
    }

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_SECOND_ALARM,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    /* 读取控制状态寄存器2(用来控制闹钟中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS2,
                      buf_cs2,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断使能 */
    buf_cs2[0] |= __PCF85063_BF_AIE;

    /* 清除AF标志 */
    buf_cs2[0] &= ~__PCF85063_BF_AF;

    /* 将闹钟中断信息写入控制状态寄存器2 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS2,
                       buf_cs2,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 禁能PCF85063闹钟
 */
int am_pcf85063_alarm_disable (am_pcf85063_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[5]     = {0};
    uint8_t buf_cs2[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 禁能闹钟 */
    buf[__PCF85063_REG_SECOND_ALARM - __PCF85063_REG_SECOND_ALARM]  |=
                                                        __PCF85063_BF_ALARM_DIS;
    buf[__PCF85063_REG_MINUTE_ALARM - __PCF85063_REG_SECOND_ALARM]  |=
                                                        __PCF85063_BF_ALARM_DIS;
    buf[__PCF85063_REG_HOUR_ALARM - __PCF85063_REG_SECOND_ALARM]    |=
                                                        __PCF85063_BF_ALARM_DIS;
    buf[__PCF85063_REG_DAY_ALARM - __PCF85063_REG_SECOND_ALARM]     |=
                                                        __PCF85063_BF_ALARM_DIS;
    buf[__PCF85063_REG_WEEKDAY_ALARM - __PCF85063_REG_SECOND_ALARM] |=
                                                        __PCF85063_BF_ALARM_DIS;

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_SECOND_ALARM,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    /* 读取控制状态寄存器2(用来控制闹钟中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS2,
                      buf_cs2,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 闹钟中断禁能 */
    buf[0] &= ~__PCF85063_BF_AIE;

    /* 清除AF标志 */
    buf_cs2[0] &= ~__PCF85063_BF_AF;

    /* 将闹钟中断信息写入控制状态寄存器2 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS2,
                       buf_cs2,
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 设置PCF85063时钟输出
 */
int am_pcf85063_clkout_set (am_pcf85063_handle_t handle, uint8_t freq)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle || freq > AM_PCA85063A_CLKOUT_FREQ_DISABLE) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取当前状态寄存器2的值 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS2,
                      buf,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    buf[0] &= ~__PCF85063_BF_COF;           /* 清除clockout段 */
    buf[0] |= __PCF85063_BF_COF & freq;     /* 填入新的clock */

    /* 写入状态寄存器2 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS2,
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
int am_pcf85063_clkout_close(am_pcf85063_handle_t handle)
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

/**
 * \brief 使能PCF85063定时器
 */
int am_pcf85063_timer_enable (am_pcf85063_handle_t handle,
                              uint8_t              clk_freq,
                              uint8_t              max_timer_count)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[2] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 利用I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_TIMER_VALUE,
                      buf,
                      sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    /* 装载计数值 */
    buf[__PCF85063_REG_TIMER_VALUE - __PCF85063_REG_TIMER_VALUE] = max_timer_count;

    /* 选择时钟频率以及使能定时器 */
    buf[__PCF85063_REG_TIMER_MODE - __PCF85063_REG_TIMER_VALUE] &=
                                               ~__PCF85063_BF_TIMER_TCF;
    buf[__PCF85063_REG_TIMER_MODE - __PCF85063_REG_TIMER_VALUE] |=
                                    (clk_freq & __PCF85063_BF_TIMER_TCF) |
                                                __PCF85063_BF_TI_TP |
                                                __PCF85063_BF_TE;

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_TIMER_VALUE,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 禁能PCF85063定时器
 */
int am_pcf85063_timer_disable (am_pcf85063_handle_t handle)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 */
    uint8_t buf[1] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 利用I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_TIMER_MODE,
                      buf,
                      sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    /* 清除定时器使能位 */
    buf[0] &= ~__PCF85063_BF_TE;

    /* 利用I2C传输数据 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_TIMER_MODE,
                       buf,
                       sizeof(buf));

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

int am_pcf85063_timer_count_get (am_pcf85063_handle_t  handle,
                                 uint8_t              *p_count)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 利用I2C读取数据 */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_TIMER_VALUE,
                      p_count,
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 使能PCF85063分钟/定时器中断
 */
int am_pcf85063_minute_timer_int_enable (am_pcf85063_handle_t handle,
                                         uint8_t              flags)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 buf存储寄存器值 */
    uint8_t buf[2] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制状态寄存器2(用来清除中断标志以及分钟类中断的使能) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS2,
                      &buf[0],
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 读取定时器模式寄存器(用来控制定时器中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_TIMER_MODE,
                      &buf[1],
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 根据不同的中断类型来处理 */

    if (flags & AM_PCF85063_INT_TIMER) {
        /* 定时器中断使能,TF生成脉冲 */
        buf[1] |= __PCF85063_BF_TIE;
    }

    if (flags & AM_PCF85063_INT_HMINUTE) {
        /* 半分钟中断使能 */
        buf[0] |= __PCF85063_BF_HMI;
    }

    if (flags & AM_PCF85063_INT_MINUTE) {
        /*  一分钟中断使能 */
        buf[0] |= __PCF85063_BF_MI;
    }

    /* 清除TF标志 */
    buf[0] &= ~__PCF85063_BF_TF;

    /* 将TF标志与分钟类中断信息写入控制状态寄存器2 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS2,
                       &buf[0],
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 将定时器信息写入定时器模式寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_TIMER_MODE,
                       &buf[1],
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief 禁能PCF85063分钟/定时器中断
 */
int am_pcf85063_minute_timer_int_disable (am_pcf85063_handle_t handle,
                                          uint8_t              flags)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 数据缓冲区 buf存储寄存器值 */
    uint8_t buf[2] = {0};

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 读取控制状态寄存器2(用来清除中断标志以及分钟类中断的使能) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_CTRL_STATUS2,
                      &buf[0],
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 读取定时器模式寄存器(用来控制定时器中断) */
    ret = am_i2c_read(p_i2c_dev,
                      __PCF85063_REG_TIMER_MODE,
                      &buf[1],
                      1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 根据不同的中断类型来处理 */

    if (flags & AM_PCF85063_INT_TIMER) {
        /* 定时器中断禁能 */
        buf[1] &= ~__PCF85063_BF_TIE;
    }

    if (flags & AM_PCF85063_INT_HMINUTE) {
        /* 半分钟中断禁能 */
        buf[0] &= ~__PCF85063_BF_HMI;
    }

    if (flags & AM_PCF85063_INT_MINUTE) {
        /*  一分钟中断禁能 */
        buf[0] &= ~__PCF85063_BF_MI;
    }

    /* 清除TF标志 */
    buf[0] &= ~__PCF85063_BF_TF;

    /* 将TF标志与分钟类中断信息写入控制状态寄存器2 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_CTRL_STATUS2,
                       &buf[0],
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    /* 将定时器信息写入定时器模式寄存器 */
    ret = am_i2c_write(p_i2c_dev,
                       __PCF85063_REG_TIMER_MODE,
                       &buf[1],
                       1);

    if (AM_OK != ret) {
        return ret;
    }

    return AM_OK;
}

/**
 * \brief PCF85063 分钟/定时器回调函数设置
 */
int am_pcf85063_minute_timer_cb_set (am_pcf85063_handle_t  handle,
                                     am_pfnvoid_t          pfn_minute_timer_callback,
                                     void                 *p_minute_timer_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 设备信息指针 */
    const am_pcf85063_devinfo_t *p_devinfo = NULL;

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
                                      __pcf85063_eint_isr,
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
    handle->triginfo[0].pfn_callback = pfn_minute_timer_callback;
    handle->triginfo[0].p_arg        = p_minute_timer_arg;

    /* 如果所有的回调函数都为空 */
    if ((handle->triginfo[0].pfn_callback == NULL) &&
        (handle->triginfo[1].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (handle->conn_stat) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->int_pin,
                                             __pcf85063_eint_isr,
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
 * \brief PCF85063 闹钟回调函数设置
 */
int am_pcf85063_alarm_cb_set (am_pcf85063_handle_t  handle,
                              am_pfnvoid_t          pfn_alarm_callback,
                              void                 *p_alarm_arg)
{
    /* 函数调用返回值 */
    int ret = AM_OK;

    /* 设备信息指针 */
    const am_pcf85063_devinfo_t *p_devinfo = NULL;

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
                                      __pcf85063_eint_isr,
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
    handle->triginfo[1].pfn_callback = pfn_alarm_callback;
    handle->triginfo[1].p_arg        = p_alarm_arg;

    /* 如果所有的回调函数都为空 */
    if ((handle->triginfo[0].pfn_callback == NULL) &&
        (handle->triginfo[1].pfn_callback == NULL)) {

        /* 如果conn_stat为TRUE则调用GPIO解除连接函数 */
        if (handle->conn_stat) {

            /* 删除引脚中断回调函数 */
            ret = am_gpio_trigger_disconnect(p_devinfo->int_pin,
                                             __pcf85063_eint_isr,
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

/* end of file */
