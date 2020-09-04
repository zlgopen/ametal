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
 * \brief MicroPort 用户配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-13  pea, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_MICROPORT_H
#define __AM_HWCONF_MICROPORT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "ametal.h"
#include "am_time.h"
#include "am_alarm_clk.h"
#include "am_ds1302.h"
#include "am_pcf85063.h"
#include "am_rx8025t.h"
#include "am_mtd.h"
#include "am_ftl.h"
#include "am_mx25xx.h"
#include "am_ep24cxx.h"

/*******************************************************************************
  MicroPort DS1302 实例初始化
*******************************************************************************/

/**
 * \brief MicroPort DS1302 实例初始化（使用芯片特殊功能）
 *
 * \param 无
 *
 * \return DS1302 标准服务句柄，若为NULL，表明初始化失败
 */
am_ds1302_handle_t am_microport_ds1302_inst_init (void);

/**
 * \brief MicroPort DS1302 实例初始化（使用通用的 RTC 功能）
 *
 * \param 无
 *
 * \return RTC 标准服务句柄，若为NULL，表明初始化失败
 */
am_rtc_handle_t am_microport_ds1302_rtc_inst_init (void);

/**
 * \brief MicroPort DS1302 实例初始化（将 DS1302 用作系统时间）
 *
 * \param 无
 *
 * \retval AM_OK       成功
 * \retval -AM_EINVAL  参数错误
 */
int am_microport_ds1302_time_inst_init (void);

/*******************************************************************************
  MicroPort RTC(PCF85063) 实例初始化
*******************************************************************************/

/**
 * \brief MicroPort RTC(PCF85063) 实例初始化（使用芯片特殊功能）
 *
 * \param 无
 *
 * \return PCF85063 标准服务句柄，若为NULL，表明初始化失败
 */
am_pcf85063_handle_t am_microport_rtc_inst_init (void);

/**
 * \brief MicroPort RTC(PCF85063) 实例初始化（使用通用的 RTC 功能）
 * \param 无
 * \return RTC 标准服务句柄，若为NULL，表明初始化失败
 */
am_rtc_handle_t am_microport_std_rtc_inst_init (void);

/**
 * \brief MicroPort RTC(PCF85063) 实例初始化（使用通用的闹钟功能）
 *
 * \param 无
 *
 * \return ALARM_CLK 标准服务句柄，若为NULL，表明初始化失败
 */
am_alarm_clk_handle_t am_microport_rtc_alarm_clk_inst_init (void);

/**
 * \brief MicroPort RTC(PCF85063) 实例初始化（将 PCF85063 用作系统时间）
 *
 * \param 无
 *
 * \retval AM_OK       成功
 * \retval -AM_EINVAL  参数错误
 */
int am_microport_rtc_time_inst_init (void);

/*******************************************************************************
  MicroPort RX8025T 实例初始化
*******************************************************************************/

/**
 * \brief MicroPort RX8025T 实例初始化（使用芯片特殊功能）
 *
 * \param 无
 *
 * \return RX8025T 标准服务句柄，若为NULL，表明初始化失败
 */
am_rx8025t_handle_t am_microport_rx8025t_inst_init (void);

/**
 * \brief MicroPort RX8025T 实例初始化（使用通用的 RTC 功能）
 *
 * \param 无
 *
 * \return RTC 标准服务句柄，若为NULL，表明初始化失败
 */
am_rtc_handle_t am_microport_rx8025t_rtc_inst_init (void);

/**
 * \brief MicroPort RX8025T 实例初始化（使用通用的闹钟功能）
 *
 * \param 无
 *
 * \return ALARM_CLK 标准服务句柄，若为NULL，表明初始化失败
 */
am_alarm_clk_handle_t  am_microport_rx8025t_alarm_clk_inst_init (void);

/**
 * \brief MicroPort RX8025T 实例初始化（将 RX8025T 用作系统时间）
 *
 * \param 无
 *
 * \retval AM_OK       成功
 * \retval -AM_EINVAL  参数错误
 */
int am_microport_rx8025t_time_inst_init (void);

/*******************************************************************************
  MicroPort FLASH(MX25xx) 实例初始化
*******************************************************************************/

/**
 * \brief MicroPort FLASH(MX25xx) 实例初始化
 *
 * \param 无
 *
 * \return MX25XX 标准服务句柄，若为NULL，表明初始化失败
 */
am_mx25xx_handle_t am_microport_flash_inst_init (void);

/**
 * \brief MicroPort FLASH(MX25xx) 实例初始化(使用 MTD 标准接口)
 *
 * \param 无
 *
 * \return MTD 标准服务句柄，若为NULL，表明初始化失败
 */
am_mtd_handle_t am_microport_flash_mtd_inst_init (void);

/**
 * \brief MicroPort FLASH(MX25xx) 实例初始化(使用 FTL 标准接口)
 *
 * \param 无
 *
 * \return FTL 标准服务句柄，若为NULL，表明初始化失败
 */
am_ftl_handle_t am_microport_flash_ftl_inst_init (void);

/*******************************************************************************
  MicroPort EEPROM 实例初始化
*******************************************************************************/

/**
 * \brief MicroPort EEPROM 实例初始化（使用芯片功能）
 *
 * \param 无
 *
 * \return EP24CXX 标准服务句柄，若为NULL，表明初始化失败
 */
am_ep24cxx_handle_t am_microport_eeprom_inst_init (void);

/**
 * \brief MicroPort EEPROM 实例初始化（将 FM24C02 用作标准的 NVRAM 设备）
 *
 * \param 无
 *
 * \return AM_OK, 提供NVRAM服务成功；其它值，提供NVRAM服务失败。
 */
int am_microport_eeprom_nvram_inst_init (void);

/*******************************************************************************
  MicroPort RS485 实例初始化
*******************************************************************************/

/**
 * \brief MicroPort RS485 实例初始化
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为NULL，表明初始化失败
 */
am_uart_handle_t am_microport_rs485_inst_init (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AM_HWCONF_MICROPORT_H */

/* end of file */
