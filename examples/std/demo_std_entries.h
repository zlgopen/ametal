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
 * \brief STD閹碉拷閺堝绶ョ粙瀣毐閺佹澘鍙嗛崣锝咃紣閺勶拷
 * \sa    demo_std_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-02  sdy, first implementation
 * \endinternal
 */

#ifndef __DEMO_STD_ENTRIES_H
#define __DEMO_STD_ENTRIES_H

#include "am_cap.h"
#include "am_crc.h"
#include "am_dac.h"
#include "am_i2c.h"
#include "am_i2c_slv.h"
#include "am_pwm.h"
#include "am_wdt.h"
#include "am_spi.h"
#include "am_spi_slv.h"
#include "am_adc.h"
#include "am_uart.h"
#include "am_temp.h"
#include "am_timer.h"
#include "am_rtc.h"
#include "am_hc595.h"
#include "am_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ADC娓氬鈻奸敍灞藉焺閻€劍鐓＄拠銏℃煙瀵繗骞忛崣鏈匘C鏉烆剚宕茬紒鎾寸亯閿涘矂锟芥俺绻冮弽鍥у櫙閹恒儱褰涚�圭偟骞�
 *
 * \param[in] handle ADC 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] chan   ADC 闁岸浜鹃崣锟�
 *
 * \return 閺冿拷
 */
void demo_std_adc_entry (am_adc_handle_t handle, int chan);

/**
 * \brief ADC 閸掆晝鏁ら弶鍨摍閻ㄥ嫮鍎归弫蹇曟暩闂冿拷(NTC)閿涘本甯圭粻妤�鍤〒鈺佸娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] handle    ADC 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] chan      ADC 闁岸浜鹃崣鍑ょ礄鐠囥儵锟芥岸浜剧�电懓绨查惃鍕穿閼存岸娓舵潻鐐村复閼凤拷 NTC 缁旑垽绱�
 * \param[in] res_fixed 閸ュ搫鐣鹃崚鍡楀竾閻㈢敻妯嗛崐纭风礄閸楁洑缍呴敍姘儌婵棴绱氶敍灞肩瑢閸忚渹缍嬬涵顑挎閻絻鐭鹃惄绋垮彠
 *
 * \return 閺冿拷
 */
void demo_std_adc_ntc_entry (am_adc_handle_t handle,
                             int             chan,
                             uint32_t        res_fixed);

/**
 * \brief 閾氬倿绂忛崳銊ょ伐缁嬪绱濋柅姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param 閺冿拷
 *
 * \return 閺冿拷
 */
void demo_std_buzzer_entry (void);

/**
 * \brief 鐎规碍妞傞崳銊ョ杽閻滅増宕熼懢宄板閼虫枻绱濋柅姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] cap_handle 鏉堟挸鍙嗛幑鏇″箯閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] pwm_handle PWM 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] cap_chan   鐎规碍妞傞崳銊﹀礋閼剧兘锟芥岸浜鹃敍鍫滅閼割剟绮拋銈勫▏閻€劍宕熼懢鐑斤拷姘朵壕 0閿涳拷
 * \param[in] pwm_chan   鐎规碍妞傞崳銊ㄧ翻閸戞椽锟芥岸浜鹃敍鍫滅閼割剟绮拋銈勫▏閻拷 PWM 闁岸浜� 0閿涳拷
 *
 * \return 閺冿拷
 */
void demo_std_timer_cap_entry (am_cap_handle_t cap_handle, int cap_chan);

/**
 * \brief CRC 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] crc_handle CRC 閺嶅洤鍣張宥呭閸欍儲鐒�
 *
 * \return 閺冿拷
 */
void demo_std_crc_entry (am_crc_handle_t crc_handle);

/**
 * \brief GPIO 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] input_pin  GPIO 缁犅ゅ壖閺嶅洤褰� (# PIO*_0)
 * \param[in] output_pin GPIO 缁犅ゅ壖閺嶅洤褰� (# PIO*_0)
 *
 * \return 閺冿拷
 */
void demo_std_gpio_entry (int input_pin, int output_pin);

/**
 * \brief GPIO 瀵洝鍓兼稉顓熸焽娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] pin GPIO 缁犅ゅ壖閺嶅洤褰� (# PIO*_0)
 *
 * \return 閺冿拷
 */
void demo_std_gpio_trigger_entry (int pin);

/**
 * \brief I2C 娑撶粯婧�鐠囪褰囧〒鈺佸娴肩姵鍔呴崳锟� LM75 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] handle 濞撯晛瀹抽弽鍥у櫙閺堝秴濮熼崣銉︾労
 *
 * \return 閺冿拷
 */
void demo_std_i2c_lm75_entry (am_temp_handle_t handle);

/**
 * \brief I2C 娑撶粯婧�鐠佸潡妫� EEPROM 鐠佹儳顦笟瀣柤閿涘矂锟芥俺绻冨鍌涱劄閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] handle      I2C 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] eeprom_addr EEPROM 閸︽澘娼�
 * \param[in] test_len    濞村鐦�涙濡弫锟�
 *
 * \return 閺冿拷
 */
void demo_std_i2c_master_async_entry (am_i2c_handle_t handle,
                                      uint32_t        eeprom_addr,
                                      uint32_t        test_len);

/**
 * \brief I2C 娑撶粯婧�鐠佸潡妫� EEPROM 鐠佹儳顦笟瀣柤閿涘矂锟芥俺绻冮崥灞绢劄閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] handle      I2C 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] eeprom_addr EEPROM 閸︽澘娼�
 * \param[in] test_len    濞村鐦�涙濡弫锟�
 *
 * \return 閺冿拷
 */
void demo_std_i2c_master_sync_entry (am_i2c_handle_t handle,
                                     uint32_t        eeprom_addr,
                                     uint32_t        test_len);

/**
 * \brief I2C 娴犲孩婧�濡剝瀚� TMP100 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] handle I2C 閺嶅洤鍣張宥呭閸欍儲鐒�
 *
 * \return 閺冿拷
 */
void demo_std_i2c_slave_subaddr_entry (am_i2c_slv_handle_t handle);

/**
 * \brief I2C 娴犲孩婧�娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] handle   I2C 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] dev_addr 娴犲孩婧�閸︽澘娼�
 *
 * \return 閺冿拷
 */
void demo_std_i2c_slave_entry (am_i2c_slv_handle_t handle, uint16_t dev_addr);
 
/**
 * \brief 閹稿鏁笟瀣柤閿涳拷4娑擃亝瀵滈柨顕嗙礆
 * \param[in] led_id LED 缂傛牕褰�
 * \return 閺冿拷
 */
void demo_std_4key_entry (void);

/**
 * \brief 閹稿鏁笟瀣柤
 * \param[in] led_id LED 缂傛牕褰�
 * \return 閺冿拷
 */
void demo_std_key_entry (void);

/**
 * \brief 閹稿鏁笟瀣柤閿涘奔濞囬悽銊︽殶閻胶顓搁弰鍓с仛瑜版挸澧犻幐澶夌瑓閻ㄥ嫭瀵滈柨顕嗙礄閹稿鏁禍瀣╂閸︺劍瀵滈柨顔兼礀鐠嬪啩鑵戠粩瀣祮婢跺嫮鎮婇敍锟�
 * \param[in] id  閺佹壆鐖滅粻锛勭椽閸欙拷
 * \return 閺冿拷
 */
void demo_std_key_digitron_entry (int32_t id);

/**
 * \brief 閹稿鏁笟瀣柤閿涘奔濞囬悽銊︽殶閻胶顓搁弰鍓с仛瑜版挸澧犻幐澶夌瑓閻ㄥ嫭瀵滈柨顕嗙礄閹稿鏁禍瀣╂缂傛挸鐡ㄩ崚鎵箚瑜般垻绱﹂崘鎻掑隘娑擃叏绱�
 * \param[in] id  閺佹壆鐖滅粻锛勭椽閸欙拷
 * \return 閺冿拷
 */
void demo_std_key_digitron_rngbuf_entry (int32_t id);

/**
 * \brief LED 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 * \param[in] led_id LED 缂傛牕褰�
 * \return 閺冿拷
 */
void demo_std_led_entry (int led_id);

/**
 * \brief LED 濞翠焦鎸夐悘顖欑伐缁嬶拷
 *
 * \param[in] led_id_start  婢舵矮閲淟ED閻ㄥ嫯鎹ｆ慨瀣椽閸欙拷
 * \param[in] num           閻€劋绨ù浣规寜閻忣垯绶ョ粙瀣畱 LED 閺佹壆娲�
 *
 * \return 閺冿拷
 */
void demo_std_led_water_light_entry (int led_id_start, int num);

/**
 * \brief 鐎规碍妞傞崳锟� PWM 鏉堟挸鍤崝鐔诲厴閿涘矂锟芥俺绻冮弽鍥у櫙閹恒儱褰涚�圭偟骞�
 *
 * \param[in] pwm_handle   PWM 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] pwm_chan     闁岸浜鹃崣锟�
 *
 * \return 閺冿拷
 */
void demo_std_timer_pwm_entry (am_pwm_handle_t pwm_handle, int pwm_chan);

/**
 * \brief 鏉烆垯娆㈢�规碍妞傞崳銊ょ伐缁嬪绱濋柅姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 * \return 閺冿拷
 */
void demo_std_softimer_entry (void);

/**
 * \brief SPI 鐠囪鍟� FLASH(MX25L3206E) 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] spi_handle  SPI 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] cs_pin      閻楀洭锟藉绱╅懘姘娇
 * \param[in] test_addr   濞村鐦崷鏉挎絻
 * \param[in] test_length 濞村鐦�涙濡弫锟�
 *
 * \return 閺冿拷
 */
void demo_std_spi_flash_entry (am_spi_handle_t spi_handle,
                               int             cs_pin,
                               uint32_t        test_addr,
                               uint32_t        test_length);

/**
 * \brief SPI 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] spi_handle SPI 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] cs_pin     閻楀洭锟藉绱╅懘姘娇
 *
 * \return 閺冿拷
 */
void demo_std_spi_master_entry (am_spi_handle_t spi_handle,
                                int             cs_pin);

/**
 * \brief SPI 娴犲孩婧�娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] spi_handle SPI 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] cs_pin     閻楀洭锟藉绱╅懘姘娇
 *
 * \return 閺冿拷
 */
void demo_std_spi_slave_entry (am_spi_slv_handle_t handle);

/**
 * \brief 瀵よ埖妞傞弽鍥у櫙閹恒儱褰涙笟瀣柤閿涘矂锟芥俺绻冮弽鍥у櫙閹恒儱褰涚�圭偟骞�
 *
 * \param[in] led_id LED 缂傛牕褰�
 *
 * \return 閺冿拷
 */
void demo_std_delay_entry (int led_id);

/**
 * \brief Systick 娓氬鈻奸敍宀勶拷姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 *
 * \param[in] handle 鐎规碍妞傞崳銊︾垼閸戝棙婀囬崝鈥冲綖閺岋拷
 * \param[in] led_id LED 缂傛牕褰�
 *
 * \return 閺冿拷
 */
void demo_std_systick_timer_entry (am_timer_handle_t handle, int led_id);

/**
 * \brief 鐎规碍妞傞崳銊ョ杽閻滅増鐖ｉ崙鍡楃暰閺冭泛娅掓笟瀣柤閿涘矂锟芥俺绻冮弽鍥у櫙閹恒儱褰涚�圭偟骞�
 *
 * \param[in] handle      鐎规碍妞傞崳銊︾垼閸戝棙婀囬崝鈥冲綖閺岋拷
 * \param[in] timing_chan 鐎规碍妞傞崳銊╋拷姘朵壕閸欙拷
 *
 * \return 閺冿拷
 */
void demo_std_timer_timing_entry (am_timer_handle_t handle,
                                  int               timing_chan);

/**
 * \brief UART 閺屻儴顕楅弬鐟扮础娑撳甯撮弨璺哄絺闁焦鏆熼幑顔荤伐缁嬪绱濋柅姘崇箖閺嶅洤鍣幒銉ュ經鐎圭偟骞�
 * \param[in] handle UART 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \return 閺冿拷
 */
void demo_std_uart_polling_entry (am_uart_handle_t handle);

/**
 * \brief UART 閻滎垰鑸扮紓鎾冲暱閸栫儤鏌熷蹇庣瑓閹恒儲鏁归崣鎴︼拷浣规殶閹诡喕绶ョ粙瀣剁礉闁俺绻冮弽鍥у櫙閹恒儱褰涚�圭偟骞�
 * \param[in] uart_handle UART 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \return 閺冿拷
 */
void demo_std_uart_ringbuf_entry (am_uart_handle_t uart_handle);

/**
 * \brief UART RS485娓氬鈻�
 * \param[in] uart_handle UART 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \return 閺冿拷
 */
void demo_std_uart_rs485_entry (am_uart_handle_t uart_handle);

/**
 * \brief WDT 濠曟梻銇氭笟瀣柤閿涘矂锟芥俺绻冮弽鍥у櫙閹恒儱褰涚�圭偟骞�
 *
 * \param[in] handle       WDT 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \param[in] time_out_ms  閻妫悪妤勭Т閺冭埖妞傞梻锟�
 * \param[in] feed_time_ms 閸犲倻瀚嶉弮鍫曟？
 *
 * \return 閺冿拷
 */
void demo_std_wdt_entry (am_wdt_handle_t handle,
                         uint32_t        time_out_ms,
                         uint32_t        feed_time_ms);
                         
/**
 * \brief 閺佹壆鐖滅粻鈩冩▔缁�锟�60缁夋帟顓搁弮鏈电伐缁嬶拷
 * \param[in] id 閺佹壆鐖滅粻锛勭椽閸欙拷
 * \return 閺冿拷
 */          
void demo_std_digitron_60s_counting_entry (int32_t id);

/**
 * \brief 閺佹壆鐖滅粻鈥崇杽閺冭埖妯夌粈鐑樹刊鎼达缚绶ョ粙锟�
 *
 * \param[in] id          閺佹壆鐖滅粻锛勭椽閸欙拷
 * \param[in] temp_handle 濞撯晛瀹虫导鐘冲妳閸ｃ劍鐖ｉ崙鍡樻箛閸斺�冲綖閺岋拷
 *
 * \return 閺冿拷
 */
void demo_std_digitron_temp_entry (int32_t id, am_temp_handle_t temp_handle);
 
/**
 * \brief HC595 鏉堟挸鍤笟瀣柤閿涘奔璐熸笟澶哥艾鐟欏倸鐧傞悳鎷岃杽閿涘苯缂撶拋顔肩殺HC595閻拷8娑擃亣绶崙楦跨箾閹恒儱鍩孡ED閻忥拷
 * \param[in] hc595_handle  HC595 閺嶅洤鍣張宥呭閸欍儲鐒�
 * \return 閺冿拷
 */
void demo_std_hc595_led_entry (am_hc595_handle_t hc595_handle);

/**
 * \brief RTC娓氬鈻�
 * \param[in] rtc_handle  RTC鐠佹儳顦弽鍥у櫙閺堝秴濮熼崣銉︾労
 * \return 閺冿拷
 */
void demo_std_rtc_entry (am_rtc_handle_t rtc_handle);

/**
 * \brief NVRAM 濞村鐦笟瀣柤
 *
 * 濞村鐦担璺ㄦ暏閻ㄥ嫬鐡ㄩ崒銊︻唽闂囷拷閸︼拷 __g_nvram_segs[] 閸掓銆冩稉顓炵暰娑斿绱檃m_nvram_cfg.c 閺傚洣娆㈡稉顓ㄧ礆
 *
 * \param[in] p_nvram_name  鐎涙ê鍋嶅▓闈涙倳
 * \param[in] nvram_unit    鐎涙ê鍋嶅▓闈涘礋閸忓啫褰�
 * \param[in] test_lenth    濞村鐦拠璇插晸閺佺増宓侀惃鍕毐鎼达拷
 *
 * \return 閺冿拷
 */
void demo_std_nvram_entry (char *p_nvram_name, int32_t nvram_unit, int32_t test_lenth);

/**
 * \brief 濞撯晜绠嶆惔锔跨炊閹扮喎娅扝TS221娓氬鈻奸敍宀勶拷姘崇箖鐟欙箑褰傚Ο鈥崇础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_hts221_int_entry (am_sensor_handle_t handle);

/**
 * \brief 濞撯晜绠嶆惔锔跨炊閹扮喎娅扝TS221娓氬鈻奸敍宀勶拷姘崇箖閺屻儴顕楅弬鐟扮础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_hts221_entry (am_sensor_handle_t handle);

/**
 * \brief 閸樺濮忓〒鈺佸娴肩姵鍔呴崳藱MP280娓氬鈻奸敍宀勶拷姘崇箖閺屻儴顕楅弬鐟扮础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_bmp280_entry (am_sensor_handle_t handle);

/**
 * \brief 閸樺濮忓〒鈺傜畭鎼达缚绱堕幇鐔锋珤BME280娓氬鈻奸敍宀勶拷姘崇箖閺屻儴顕楅弬鐟扮础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_bme280_entry (am_sensor_handle_t handle);

/**
 * \brief 閸樺濮忓〒鈺佸娴肩姵鍔呴崳鈫橮S22HB娓氬鈻奸敍宀勶拷姘崇箖閺屻儴顕楅弬鐟扮础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_lps22hb_entry (am_sensor_handle_t handle);

/**
 * \brief 閸樺濮忓〒鈺佸娴肩姵鍔呴崳鈫橮S22HB娓氬鈻奸敍宀勶拷姘崇箖鐟欙箑褰傚Ο鈥崇础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_lps22hb_int_entry (am_sensor_handle_t handle);

/**
 * \brief 娑撳閰遍梽锟介摶杞板崕娴肩姵鍔呴崳藱MG160娓氬鈻奸敍宀勶拷姘崇箖閺屻儴顕楀Ο鈥崇础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_bmg160_entry (am_sensor_handle_t handle);

/**
 * \brief 娑撳閰辩壕浣风炊閹扮喎娅扢MC5883MA娓氬鈻奸敍宀勶拷姘崇箖閺屻儴顕楀Ο鈥崇础鐎圭偟骞�
 *
 * \return 閺冿拷
 */
void demo_std_mmc5883ma_entry (am_sensor_handle_t handle);

/**
 * \brief DAC杈撳嚭鐢靛帇锛屾爣鍑嗘帴鍙ｅ眰瀹炵幇
 * \return 鏃�
 */
void demo_lpc_std_dac_entry (am_dac_handle_t dac0_handle,
                             uint8_t         channel,
                             uint32_t        mv_out);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_STD_ENTRIES_H */

/* end of file */
