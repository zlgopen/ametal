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
 * \brief PCF8551驱动接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 20-02-25  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_PCF8551_H
#define __AM_PCF8551_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_i2c.h"

/**
 * @addtogroup am_if_pcf8551
 * @copydoc am_pcf8551.h
 * @{
 */
 
/**
 * \name PCF8551控制命令
 * 
 *     控制命令用于 am_pcf8551_ioctl() 函数的 cmd 参数。其p_arg参数的实际类型随着
 * 命令的不同而不同。
 *
 * @{
 */

/**
 * \brief 设置帧率，单位Hz
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示设置的LCD帧率，
 * 有效值有：32、64、96、128，默认值64
 */
#define  AM_PCF8551_FRAME_FREQ_SET     1

/**
 * \brief 设置驱动模式
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示设置的驱动模式，
 * 有效值有：4：使能COM0-COM3
 *           3：使能COM0-COM2
 *           2：使能COM0-COM1
 *           1：使能COM0
 */
#define  AM_PCF8551_DRIVE_MODE_SET     2

/**
 * \brief 闪烁控制
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示设置的闪烁频率
 * 有效值有：0：关闭闪烁
 *           1：闪烁频率为0.5Hz
 *           2：闪烁频率为1Hz
 *           3：闪烁频率为2Hz
 */
#define  AM_PCF8551_BLINK_FREQ_SET     3

/**
 * \brief PCF8551 设备信息结构体
 */
typedef struct am_pcf8551_devinfo {

    /** \brief I2C 7位 设备地址 */
    uint8_t i2c_addr;

} am_pcf8551_devinfo_t;    
    
/**
 * \breif PCF8551 设备结构体定义
 */
typedef struct am_pcf8551_dev {

    /** \brief i2c设备实例 */
    am_i2c_device_t i2c_dev;    

    /** \brief 设备信息 */
    const am_pcf8551_devinfo_t *dev_info;

} am_pcf8551_dev_t;

typedef am_pcf8551_dev_t *am_pcf8551_handle_t;

/**
 * \brief PCF8551 初始化
 *
 * \param[in] p_dev     : 指向PCF8551设备的指针
 * \param[in] p_devinfo : 指向PCF8551设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 标准错误号
 */
am_pcf8551_handle_t am_pcf8551_init (am_pcf8551_dev_t           *p_dev,
                                     const am_pcf8551_devinfo_t *p_devinfo,
                                     am_i2c_handle_t             handle);

/**
 * \brief PCF8551 解初始化
 *
 * \param[in] handle   : PCF8551 初始化获取的标准服务句柄
 *
 * \return 标准错误号
 */
am_err_t am_pcf8551_deinit (am_pcf8551_handle_t handle);

/**
 * \brief PCF8551 控制函数
 *
 * \param[in]     handle : PCF8551 初始化获取的标准服务句柄
 * \param[in]     cmd    : PCF8551 控制指令
 * \param[in,out] p_arg  : 命令对应的参数
 *
 * \return 标准错误号
 */
int am_pcf8551_ioctl (am_pcf8551_handle_t  handle,
                      int                  cmd,
                      void                *p_arg);

/**
 * \brief PCF8551显示
 *
 * \param[in] handle   : PCF8551标准服务句柄
 * \param[in] ram_addr : 显存地址，显存地址范围0x04-0x17
 * \param[in] p_data   : 显存数据
 * \param[in] len      : 数据长度
 *
 * \return 标准错误号
 */
int am_pcf8551_display (am_pcf8551_handle_t handle,
                        uint8_t             ram_addr,
                        uint8_t            *p_data,
                        uint8_t             len);

/**
 * \brief PCF8551开显示
 *
 * \param[in] handle : PCF8551标准服务句柄
 *
 * \return 标准错误号
 */
int am_pcf8551_display_on (am_pcf8551_handle_t handle);

/**
 * \brief PCF8551关显示
 *
 * \param[in] handle : PCF8551标准服务句柄
 *
 * \return 标准错误号
 */
int am_pcf8551_display_off (am_pcf8551_handle_t handle);

/**
 * \brief PCF8551软复位
 *
 * \param[in] handle : PCF8551标准服务句柄
 *
 * \return 标准错误号
 */
int am_pcf8551_software_rst (am_pcf8551_handle_t handle);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _AM_PCF8551_H */

/* end of file */    


