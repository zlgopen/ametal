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
 * \brief MicroPort �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-13  pea, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_MICROPORT_H
#define __AM_HWCONF_MICROPORT_H

#include "ametal.h"
#include "am_time.h"
#include "am_alarm_clk.h"
#include "am_ds1302.h"
#include "am_pcf85063.h"
#include "am_rx8025t.h"
#include "am_mtd.h"
#include "am_ftl.h"
#include "am_mx25xx.h"
#include "am_is25xx.h"
#include "am_ep24cxx.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
  MicroPort DS1302 ʵ����ʼ��
*******************************************************************************/

/**
 * \brief MicroPort DS1302 ʵ����ʼ����ʹ��оƬ���⹦�ܣ�
 *
 * \param ��
 *
 * \return DS1302 ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_ds1302_handle_t am_microport_ds1302_inst_init (void);

/**
 * \brief MicroPort DS1302 ʵ����ʼ����ʹ��ͨ�õ� RTC ���ܣ�
 *
 * \param ��
 *
 * \return RTC ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_rtc_handle_t am_microport_ds1302_rtc_inst_init (void);

/**
 * \brief MicroPort DS1302 ʵ����ʼ������ DS1302 ����ϵͳʱ�䣩
 *
 * \param ��
 *
 * \retval AM_OK       �ɹ�
 * \retval -AM_EINVAL  ��������
 */
int am_microport_ds1302_time_inst_init (void);

/*******************************************************************************
  MicroPort RTC(PCF85063) ʵ����ʼ��
*******************************************************************************/

/**
 * \brief MicroPort RTC(PCF85063) ʵ����ʼ����ʹ��оƬ���⹦�ܣ�
 * \param ��
 * \return PCF85063 ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_pcf85063_handle_t am_microport_rtc_inst_init (void);

/**
 * \brief MicroPort RTC(PCF85063) ʵ����ʼ����ʹ��ͨ�õ� RTC ���ܣ�
 * \param ��
 * \return RTC ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_rtc_handle_t am_microport_rtc_std_inst_init (void);

#if 0
/**
 * \brief MicroPort RTC(PCF85063) ʵ����ʼ����ʹ��ͨ�õ����ӹ��ܣ�
 *
 * \param ��
 *
 * \return ALARM_CLK ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_alarm_clk_handle_t am_microport_rtc_alarm_clk_inst_init (void);
#endif

/**
 * \brief MicroPort RTC(PCF85063) ʵ����ʼ������ PCF85063 ����ϵͳʱ�䣩
 *
 * \param ��
 *
 * \retval AM_OK       �ɹ�
 * \retval -AM_EINVAL  ��������
 */
int am_microport_rtc_time_inst_init (void);

/*******************************************************************************
  MicroPort RX8025T ʵ����ʼ��
*******************************************************************************/

/**
 * \brief MicroPort RX8025T ʵ����ʼ����ʹ��оƬ���⹦�ܣ�
 *
 * \param ��
 *
 * \return RX8025T ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_rx8025t_handle_t am_microport_rx8025t_inst_init (void);

/**
 * \brief MicroPort RX8025T ʵ����ʼ����ʹ��ͨ�õ� RTC ���ܣ�
 *
 * \param ��
 *
 * \return RTC ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_rtc_handle_t am_microport_rx8025t_rtc_inst_init (void);

#if 0
/**
 * \brief MicroPort RX8025T ʵ����ʼ����ʹ��ͨ�õ����ӹ��ܣ�
 *
 * \param ��
 *
 * \return ALARM_CLK ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_alarm_clk_handle_t  am_microport_rx8025t_alarm_clk_inst_init (void);
#endif

/**
 * \brief MicroPort RX8025T ʵ����ʼ������ RX8025T ����ϵͳʱ�䣩
 *
 * \param ��
 *
 * \retval AM_OK       �ɹ�
 * \retval -AM_EINVAL  ��������
 */
int am_microport_rx8025t_time_inst_init (void);

/*******************************************************************************
  MicroPort FLASH(MX25xx) ʵ����ʼ��
*******************************************************************************/

/**
 * \brief MicroPort FLASH(MX25xx) ʵ����ʼ��
 *
 * \param ��
 *
 * \return MX25XX ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_mx25xx_handle_t am_microport_flash_inst_init (void);

/**
 * \brief MicroPort FLASH(IS25xx) ʵ����ʼ��
 *
 * \param ��
 *
 * \return IS25XX ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_is25xx_handle_t am_microport_flash_is25xx_inst_init (void);

/**
 * \brief MicroPort FLASH(MX25xx) ʵ����ʼ��(ʹ�� MTD ��׼�ӿ�)
 *
 * \param ��
 *
 * \return MTD ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_mtd_handle_t am_microport_flash_mtd_inst_init (void);

/**
 * \brief MicroPort FLASH(MX25xx) ʵ����ʼ��(ʹ�� FTL ��׼�ӿ�)
 *
 * \param ��
 *
 * \return FTL ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_ftl_handle_t am_microport_flash_ftl_inst_init (void);

/*******************************************************************************
  MicroPort EEPROM ʵ����ʼ��
*******************************************************************************/

/**
 * \brief MicroPort EEPROM ʵ����ʼ����ʹ��оƬ���ܣ�
 *
 * \param ��
 *
 * \return EP24CXX ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_ep24cxx_handle_t am_microport_eeprom_inst_init (void);

/**
 * \brief MicroPort EEPROM ʵ����ʼ������ FM24C02 ������׼�� NVRAM �豸��
 *
 * \param ��
 *
 * \return AM_OK, �ṩNVRAM����ɹ�������ֵ���ṩNVRAM����ʧ�ܡ�
 */
int am_microport_eeprom_nvram_inst_init (void);

/*******************************************************************************
  MicroPort RS485 ʵ����ʼ��
*******************************************************************************/

/**
 * \brief MicroPort RS485 ʵ����ʼ��
 *
 * \param ��
 *
 * \return UART ��׼����������ΪNULL��������ʼ��ʧ��
 */
am_uart_handle_t am_microport_rs485_inst_init (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AM_HWCONF_MICROPORT_H */

/* end of file */
