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
 * \brief  DS1302 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 17-8-11  fra, first implementation.
 * \endinternal
 */

#ifndef __AM_DS1302_H
#define __AM_DS1302_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_ds1302
 * @copydoc am_ds1302.h
 * @{
 */
 
#include "ametal.h"
#include "am_time.h"
#include "am_rtc.h"
#include "am_spi.h"
 
/**
 * \brief DS1302spi模式设备信息结构体
 *
 * 三线制SPI模式，针对MOSI与MISO接到一起的芯片
 * 需要驱动底层支持SPI三线制传输,若驱动底层不支持
 * 三线制传输，则会报错，那么请选择gpio模式操作
 */
typedef struct am_ds1302_spi_devinfo {

    /**< \brief 片选引脚  */
    uint16_t          ce_pin;

} am_ds1302_spi_devinfo_t;

/**
 * \brief DS1302gpio模式设备信息结构体
 */
typedef struct am_ds1302_gpio_devinfo {

    uint16_t          sclk_pin;              /**< \brief SCLK引脚对应的IO管脚号 */
    uint16_t          ce_pin;                /**< \brief ce引脚对应的IO管脚号 */
    uint16_t          io_pin;                /**< \brief IO引脚对应的IO管脚号 */

} am_ds1302_gpio_devinfo_t;

/**
 * \brief DS1302设备结构体
 */
typedef struct am_ds1302_dev {

    void              *pfn;          /**< \brief DS1302驱动函数 */
    void              *p_drv;        /**< \brief 驱动函数的第一个入口参数 */

} am_ds1302_dev_t;

/** \brief DS1302操作句柄定义  */
typedef am_ds1302_dev_t  *am_ds1302_handle_t;

/**
 * \brief DS1302spi模式下设备结构体
 */
typedef struct am_ds1302_spi_dev {

    /** \brief spi服务设备 */
    am_spi_device_t                     spi_dev;

    /** \brief DS1302spi模式设备信息 */
    const am_ds1302_spi_devinfo_t      *p_spi_devinfo;

    /** \brief DS1302标准服务 */
    am_ds1302_dev_t                     common;

} am_ds1302_spi_dev_t;

/**
 * \brief DS1302gpio模式下设备结构体
 */
typedef struct am_ds1302_gpio_dev {

    /** \brief DS1302gpio模式设备信息 */
    const am_ds1302_gpio_devinfo_t      *p_gpio_devinfo;

    /** \brief DS1302标准服务 */
    am_ds1302_dev_t                      common;

} am_ds1302_gpio_dev_t;

/**
 * \brief DS1302 设备初始化，spi驱动方式
 *
 * \param[in] p_dev          : 指向DS1302设备结构体的指针
 * \param[in] p_devinfo      : 指向DS1302设备信息结构体的指针
 * \param[in] spi_handle     : spi操作句柄
 *
 * \return DS1302服务操作句柄,如果为 NULL，表明初始化失败
 */
am_ds1302_handle_t am_ds1302_spi_init (am_ds1302_spi_dev_t            *p_dev,
                                       const am_ds1302_spi_devinfo_t  *p_devinfo,
                                       am_spi_handle_t                 spi_handle);

/**
 * \brief DS1302 设备初始化，gpio驱动方式
 *
 * \param[in] p_dev          : 指向DS1302设备结构体的指针
 * \param[in] p_devinfo      : 指向DS1302设备信息结构体的指针
 *
 * \return DS1302服务操作句柄,如果为 NULL，表明初始化失败
 */
am_ds1302_handle_t am_ds1302_gpio_init (am_ds1302_gpio_dev_t            *p_dev,
                                        const am_ds1302_gpio_devinfo_t  *p_devinfo);

/**
 * \brief DS1302 设备解初始化
 *
 * \param[in] handle : ds1302操作句柄
 *
 * \return 无
 */
void am_ds1302_deinit (am_ds1302_handle_t handle);

/**
 * \brief 获取RTC标准服务句柄
 *
 * \param[in] handle     : DS1302操作句柄
 * \param[in] p_rtc      : 指向RTC标准服务
 *
 * return RTC标准服务操作句柄
 */
am_rtc_handle_t am_ds1302_rtc_init (am_ds1302_handle_t    handle,
                                    am_rtc_serv_t        *p_rtc);

/**
 * \brief DS1302时间日期设置函数
 *
 * \param[in] handle : DS1302服务操作句柄
 * \param[in] p_tm   : 指向时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_ds1302_time_set (am_ds1302_handle_t handle, am_tm_t *p_tm);

/**
 * \brief DS1302时间日期获取函数
 *
 * \param[in]  handle : ds1302服务操作句柄
 * \param[out] p_tm   : 指向时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_ds1302_time_get (am_ds1302_handle_t handle, am_tm_t *p_tm);

/**
 * \brief DS1302终止函数
 *
 * \param[in]  handle : ds1302服务操作句柄
 *
 * \retval AM_OK      : 操作成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_ds1302_stop (am_ds1302_handle_t handle);

/**
 * \brief DS1302写RAM函数
 *
 * \param[in]  handle  : ds1302服务操作句柄
 * \param[in]  p_data  : 指向写入RAM的数据缓冲区的指针,数据的大小不超过31字节
 * \param[in]  data_len: 传入数据的长度
 * \param[in]  position: 写入RAM中的起始位置(0~30)
 *
 * \retval AM_OK       : 操作成功
 * \retval -AM_EINVAL  : 参数错误
 */
int am_ds1302_ram_write (am_ds1302_handle_t handle,
                         uint8_t           *p_data,
                         uint8_t            data_len,
                         uint8_t            position);

/**
 * \brief DS1302读RAM函数
 *
 * \param[in]  handle  : ds1302服务操作句柄
 * \param[out] p_data  : 读出的RAM中的数据
 * \param[in]  data_len: 传入数据的长度
 * \param[in]  position: 读RAM的起始位置(0~30)
 *
 * \retval AM_OK       : 操作成功
 * \retval -AM_EINVAL  : 参数错误
 */
int am_ds1302_ram_read (am_ds1302_handle_t handle,
                        uint8_t           *p_data,
                        uint8_t            data_len,
                        uint8_t            position);


/**
 * \name二极管数目与电阻阻值
 * @{
 */
#define   AM_DS1302_TRICKLE_1D_2K      0xa5  /**< \brief 一个二极管，电阻2K */
#define   AM_DS1302_TRICKLE_1D_4K      0xa6  /**< \brief 一个二极管，电阻4K */
#define   AM_DS1302_TRICKLE_1D_8K      0xa7  /**< \brief 一个二极管，电阻8K */
#define   AM_DS1302_TRICKLE_2D_2K      0xa9  /**< \brief 两个二极管，电阻2K */
#define   AM_DS1302_TRICKLE_2D_4K      0xaa  /**< \brief 两个二极管，电阻4K */
#define   AM_DS1302_TRICKLE_2D_8K      0xab  /**< \brief 两个二极管，电阻8K */
/** @}*/

/**
 * \brief DS1302充电操作
 *
 * \param[in]  handle  : ds1302服务操作句柄
 * \param[in]  set_val : 二极管数目与电阻阻值的组合，可选以下的宏
 *                       -#AM_DS1302_TRICKLE_1D_2K
 *                       -#AM_DS1302_TRICKLE_1D_4K
 *                       -#AM_DS1302_TRICKLE_1D_8K
 *                       -#AM_DS1302_TRICKLE_2D_2K
 *                       -#AM_DS1302_TRICKLE_2D_4K
 *                       -#AM_DS1302_TRICKLE_2D_8K
 *
 *                       -xD表示二极管数目，yK表示电阻阻值
 *                       -充电电流的计算可以如下:
 *                       IMAX = (VCC - 二极管压差)/R, 若3.3V系统电压加在VCC上，
 *                       选择了一个二极管，电阻选择2K
 *                       那么IMAX = (3.3- 0.7) / 2k = 1.3mA，最大充电电流
 *                       不能超过所选充电电池所允许的最大充电电流
 *                       用户可根据所选电池的充电电流的大小来选择相应的宏
 *
 * \retval AM_OK      : 操作成功
 * \retval -AM_EINVAL : 操作失败
 */
int am_ds1302_trickle_enable(am_ds1302_handle_t handle, uint8_t set_val);

/**
 * \brief DS1302禁能充电函数
 *
 * \param[in]  handle : ds1302服务操作句柄
 *
 * \retval AM_OK      : 操作成功
 * \retval -AM_EINVAL : 操作失败
 */
int am_ds1302_trickle_disable(am_ds1302_handle_t handle);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* __AM_DS1302_H */

/* end of file */
