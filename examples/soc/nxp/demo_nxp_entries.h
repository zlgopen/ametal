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
 * \brief NXP系列外设驱动的所有例程函数入口声明
 * \sa    demo_nxp_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-08-28  adw, first implementation
 * \endinternal
 */

#ifndef __DEMO_NXP_ENTRIES_H
#define __DEMO_NXP_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"
#include "am_adc.h"
#include "am_lpc_sct.h"

#include "hw/amhw_lpc_wwdt.h"
#include "hw/amhw_lpc_crc.h"
#include "hw/amhw_lpc_i2c.h"
#include "hw/amhw_lpc_sct.h"
#include "hw/amhw_lpc_usart.h"
#include "hw/amhw_lpc_spi.h"
#include "hw/amhw_lpc_wkt.h"
#include "hw/amhw_lpc_mrt.h"

#include "hw/amhw_lpc82x_adc.h"
#include "hw/amhw_lpc82x_acmp.h"
#include "hw/amhw_lpc82x_fmc.h"
#include "hw/amhw_lpc82x_gpio.h"
#include "hw/amhw_lpc82x_pmu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief WWDT 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_wwdt : 指向 WWDT 外设寄存器块的指针
 * \param[in] inum      : 中断号
 * \param[in] wdt_freq  : WWDT频率
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 WWDT 模块相应的时钟，
 * 配置合适的WDT时钟频率。
 */
void demo_lpc_hw_wwdt_entry (amhw_lpc_wwdt_t *p_hw_wwdt,
                             int              inum,
                             uint32_t         wdt_freq);

/**
 * \brief acmp 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_acmp : 指向 acmp 外设寄存器块的指针
 * \param[in] flags     : acmp配置
 * \param[in] inum      : 中断号
 * \param[in] pin       : 比较输出引脚
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 WWDT 模块相应的时钟，
 * 配置合适的WDT时钟频率。
 */
void demo_lpc824_hw_acmp_int_entry (amhw_lpc82x_acmp_t *p_hw_acmp, 
                                    uint32_t            flags, 
                                    int                 inum, 
                                    int                 pin);

/**
 * \brief acmp 硬件层（电压阶梯）例程入口
 *
 * \param[in] p_hw_acmp : 指向 acmp 外设寄存器块的指针
 * \param[in] pin       : 输出比较引脚
 */
void demo_lpc824_hw_acmp_lad_entry (amhw_lpc82x_acmp_t *p_hw_acmp, 
                                    int                 pin);
                             
/**
 * \brief acmp 硬件层（轮询）例程入口
 *
 * \param[in] p_hw_acmp : 指向 acmp 外设寄存器块的指针
 * \param[in] p_acmp    : 模拟比较寄存器块
 */
void demo_lpc824_hw_acmp_poll_entry (amhw_lpc82x_acmp_t *p_hw_acmp,
                                     int                 pin);

/**
 * \brief adc 标准adc（dma方式）例程入口
 *
 * \param[in] adc_handle  : adc 寄存器块
 * \param[in] chan        : adc 通道
 * \param[in] vref_mv     : adc 参考电压
 */
void demo_lpc824_hw_adc_dma_entry (amhw_lpc82x_adc_t *p_hw_adc,
                                   int                ch,
                                   uint32_t           vref_mv);

/**
 * \brief adc 标准adc（dma方式）例程入口
 *
 * \param[in] adc_handle  : adc 寄存器块
 * \param[in] inum        : 中断号
 * \param[in] vref_mv     : adc 参考电压
 */
void demo_lpc824_hw_adc_thcmp_entry (amhw_lpc82x_adc_t  *p_hw_adc, 
                                     int                 inum,
                                     uint32_t            vref_mv);

/**
 * \brief acmp 标准adc（dma int方式）例程入口
 *
 * \param[in] adc_handle  : adc 句柄
 * \param[in] chan        : adc 通道
 * \param[in] rate        : adc 采样速率
 */
void demo_lpc_std_adc_dma_int_entry (am_adc_handle_t adc_handle,
                                     int             chan,
                                     uint32_t        rate);

/**
 * \brief acmp 标准adc（dma poll方式）例程入口
 *
 * \param[in] adc_handle  : adc句柄
 * \param[in] chan        : adc 通道
 * \param[in] rate        : adc 采样速率
 */
void demo_lpc_std_adc_dma_poll_entry (am_adc_handle_t adc_handle,
                                      int             chan,
                                      uint32_t        rate);

/**
 * \brief acmp 标准adc（int方式）例程入口
 *
 * \param[in] adc_handle  : adc句柄
 * \param[in] chan        : adc 通道
 * \param[in] rate        : adc 采样速率 
 */
void demo_lpc_std_adc_int_entry (am_adc_handle_t adc_handle,
                                 int             chan,
                                 uint32_t        rate);

/**
 * \brief acmp 标准adc（poll 方式）例程入口
 *
 * \param[in] adc_handle  : adc句柄
 * \param[in] ch          : 通道
 * \param[in] rate        : 速率
 */
void demo_lpc_std_adc_poll_entry (am_adc_handle_t handle, 
                                  int             ch, 
                                  uint32_t        rate);


/**
 * \brief acmp 硬件层 crc例程入口
 *
 * \param[in] p_hw_crc  : crc 句柄
 * \param[in] p_data    : 测试数据
 * \param[in] rate      : 测试数据长度
 */
void demo_lpc_hw_crc_entry (amhw_lpc_crc_t  *p_hw_crc,
                            am_const char   *p_data, 
                            uint32_t         nbytes);


/**
 * \brief dma 硬件触发例程入口
 *
 * \param[in] chan : dma 通道
 * \param[in] pin  : 触发引脚
 */
void demo_lpc824_drv_dma_hwtrigger_burst_entry (uint8_t  chan,
                                                int      pin);


/**
 * \brief dma 内存到内存例程入口
 *
 * \param[in] p_src : 测试数据源地址
 * \param[in] len   : 测试数据的长度
 */
void demo_lpc824_drv_dma_m2m_entry (uint8_t *p_src,
                                    int      len);



/**
 * \brief DMA ping-pong 例程入口
 *
 * \param[in] chan  : DMA 通道
 * \param[in] p_src : 测试数据源地址
 * \param[in] len   : 测试数据的长度
 */
void demo_lpc824_drv_dma_ping_pong_entry (uint8_t  chan, 
                                          uint8_t *p_src, 
                                          int      len);


/**
 * \brief flash 硬件层例程入口
 *
 * \param[in] chan  : DMA 通道
 * \param[in] p_src : 测试数据源地址
 * \param[in] len   : 测试数据的长度
 */
void demo_lpc824_hw_flash_ctrl_entry (amhw_lpc82x_fmc_time_t time);


/**
 * \brief gpio 硬件层例程入口
 *
 * \param[in] p_hw_gpio  : GPIO 寄存器块指针
 * \param[in] pin        : 引脚
 */
void demo_lpc824_hw_gpio_entry (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                int                 pin);

/**
 * \brief gpio 硬件层中断例程入口
 *
 * \param[in] key_pin  : 按键引脚
 * \param[in] led_pin  : led引脚
 * \param[in] pint_sel : 中断通道号
 * \param[in] inum     : 中断号
 */
void demo_lpc824_hw_gpio_int_entry (int                 key_pin,
                                    int                 led_pin,
                                    int                 pint_sel,
                                    int                 inum);

/**
 * \brief gpio 匹配模式例程入口
 *
 * \param[in] pin  : led引脚
 */
void demo_lpc824_hw_gpio_pmatch_entry (int pin);

/**
 * \brief gpio 硬件层（按键）例程入口
 *
 * \param[in] pin  : 按键引脚
 */
void demo_lpc_std_gpio_key_entry (int pin);

/**
 * \brief i2c master 硬件层（dma）例程入口
 *
 * \param[in] p_hw_i2c  : i2c寄存器块
 * \param[in] clkdiv    : 分频参数
 * \param[in] addr      : 器件地址
 * \param[in] sub_addr  : 从机地址
 */
void demo_lpc824_hw_i2c_master_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                          uint32_t        clkdiv,
                                          uint8_t         addr,
                                          uint8_t         sub_add);


/**
 * \brief i2c master 硬件层（dma）例程入口
 *
 * \param[in] p_hw_i2c  : i2c寄存器块
 * \param[in] clkdiv    : 分频参数
 * \param[in] addr      : 器件地址
 */
void demo_lpc_hw_i2c_master_poll_entey (amhw_lpc_i2c_t *p_hw_i2c,
                                        uint32_t        clkdiv,
                                        uint8_t         addr);

/**
 * \brief i2c master 硬件层（dma）例程入口
 *
 * \param[in] p_hw_i2c : i2c寄存器块
 * \param[in] chan     : dma通道号
 */
void demo_lpc824_hw_i2c_slave_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                         uint8_t         chan);

/**
 * \brief i2c slave 硬件层（int）例程入口
 *
 * \param[in] p_hw_i2c  : i2c寄存器块
 * \param[in] inum      : 中断号
 */
void demo_lpc_hw_i2c_slave_int_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                      int             inum);


/**
 * \brief i2c slave 硬件层（poll）例程入口
 *
 * \param[in] p_hw_i2c  : i2c寄存器块
 */
void demo_lpc_hw_i2c_slave_poll_entry (amhw_lpc_i2c_t *p_hw_i2c);
/**
 * \brief iap 硬件层例程入口
 */
void demo_lpc824_hw_iap_entry (void);

/**
 * \brief i2c master 硬件层（dma）例程入口
 *
 * \param[in] p_hw_mrt  : mrt寄存器块
 * \param[in] inum      : 中断号
 */
void demo_lpc824_hw_mrt_entry (amhw_lpc_mrt_t *p_hw_mrt, int inum);

/**
 * \brief pll 硬件层例程入口
 */
void demo_lpc824_hw_pll_entry(void);

/**
 * \brief pmu 硬件层（deeppowerdown）例程入口
 *
 * \param[in] p_hw_mrt  : pmu寄存器块
 */
void demo_lpc824_hw_pmu_deeppowerdown_entry (amhw_lpc82x_pmu_t  *p_hw_pmu);

/**
 * \brief pmu 硬件层（deepsleep）例程入口
 *
 * \param[in] p_hw_mrt  : pmu寄存器块
 * \param[in] pin       : 引脚
 */
void demo_lpc824_hw_pmu_deepsleep_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                         int                 pin);

/**
 * \brief pmu 硬件层（powerdown）例程入口
 *
 * \param[in] p_hw_mrt  : pmu寄存器块
 * \param[in] pin       : 引脚
 */
void demo_lpc824_hw_pmu_powerdown_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                         int                 pin);

/**
 * \brief pmu 硬件层（sleep）例程入口
 *
 * \param[in] p_hw_mrt  : pmu寄存器块
 * \param[in] pin       : 引脚
 */
void demo_lpc824_hw_pmu_sleep_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                     int                 pin);

/**
 * \brief sct 驱动层（pwm）例程入口
 *
 * \param[in] sct_handle  : sct服务操作句柄
 */
void demo_lpc_drv_sct_timing_pwm_entry (am_lpc_sct_handle_t sct_handle);

/**
 * \brief sct 硬件层（pwm）例程入口
 *
 * \param[in] p_hw_sct  : sct寄存器块
 * \param[in] frq       : 系统时钟频率
 */
void demo_lpc_hw_sct_1_32bit_pwm_entry (amhw_lpc_sct_t *p_hw_sct, 
                                           uint32_t        frq);

/**
 * \brief sct 硬件层（timing_32bit）例程入口
 *
 * \param[in] p_hw_sct  : sct寄存器块
 * \param[in] inum      : 中断号
 * \param[in] frq       : 系统时钟频率
 */
void demo_lpc_hw_sct_1_32bit_timing_entry (amhw_lpc_sct_t *p_hw_sct,
                                           int             inum,
                                           uint32_t        frq);


/**
 * \brief sct 硬件层（timing_16bit）例程入口
 *
 * \param[in] p_hw_sct  : sct寄存器块
 * \param[in] inum      : 中断号
 * \param[in] frq       : 系统时钟频率
 * \param[in] pin       : 蜂鸣器引脚
 */
void demo_lpc_hw_sct_2_16bit_timing_entry (amhw_lpc_sct_t *p_hw_sct, 
                                           int             inum,
                                           uint32_t        frq,
                                           int             pin);


/**
 * \brief sct 硬件层（pwm_16bit）例程入口
 *
 * \param[in] p_hw_sct  : sct寄存器块
 * \param[in] frq       : 系统时钟频率
 */
void demo_lpc_hw_sct_2_16bit_pwm_entry(amhw_lpc_sct_t *p_hw_sct,
                                       int             frq);

/**
 * \brief sct 硬件层（pwm_16bit）例程入口
 *
 * \param[in] p_hw_sct  : sct寄存器块
 * \param[in] inum      : 中断号
 * \param[in] frq       : 系统时钟频率
 */
void demo_lpc_hw_sct_cap_entry (amhw_lpc_sct_t *p_hw_sct, 
                                int             inum, 
                                uint32_t        frq);

/**
 * \brief sct 硬件层（pwm_16bit）例程入口
 *
 * \param[in] p_hw_sct  : sct寄存器块
 * \param[in] frq       : 系统时钟频率
 */
void demo_lpc_hw_sct_multi_states_entry(amhw_lpc_sct_t *p_hw_sct,
                                        uint32_t        frq);
/**
 * \brief bod 硬件层（掉电测试）例程入口
 */
void demo_lpc824_hw_bod_entry(void);


/**
 * \brief clk 硬件层（时钟频率输出）例程入口
 */
void demo_lpc824_hw_clkout_entry (void);

/**
 * \brief usart 硬件层（自动波特率）例程入口
 *
 * \param[in] p_hw_usart  : usart寄存器块
 * \param[in] u_clk       : 串口基本工作频率
 */
void demo_lpc824_hw_usart_autobaud_entry(amhw_lpc_usart_t *p_hw_usart,
                                         uint32_t          u_clk);

/**
 * \brief usart 硬件层（poll）例程入口
 *
 * \param[in] p_hw_usart  : usart寄存器块
 * \param[in] u_clk       : 串口基本工作频率
 * \param[in] baudrate    : 波特率
 */
void demo_lpc_hw_usart_poll_entry(amhw_lpc_usart_t *p_hw_usart,
                                  uint32_t          u_clk,
                                  uint32_t          baudrate);

/**
 * \brief usart 硬件层（rx dma）例程入口
 *
 * \param[in] p_hw_usart  : usart寄存器块
 * \param[in] u_clk       : 串口基本工作频率
 * \param[in] baudrate    : 波特率
 * \param[in] chan        : 外设dma请求通道
 */
void demo_lpc824_hw_usart_rx_dma_entry (amhw_lpc_usart_t *p_hw_usart,
                                        uint32_t          uclk,
                                        uint32_t          baudrate,
                                        int               chan);
/**
 * \brief usart 硬件层（tx dma）例程入口
 *
 * \param[in] p_hw_usart  : usart寄存器块
 * \param[in] u_clk       : 串口基本工作频率
 * \param[in] baudrate    : 波特率
 * \param[in] chan        : 外设dma请求通道
 */
void demo_lpc824_hw_usart_tx_dma_entry (amhw_lpc_usart_t *p_hw_usart,
                                        uint32_t          uclk,
                                        uint32_t          baudrate,
                                        int               chan);

/**
 * \brief usart 硬件层（poll）例程入口
 *
* \param[in] uart_handle  : 服务句柄
 */
void demo_lpc_std_usart_flowctrl_entry (am_uart_handle_t  uart_handle);

/**
 * \brief wkt 硬件层（deeppowerdown-wakeup）例程入口
 *
 * \param[in] p_hw_pmu  : pmu寄存器块
 * \param[in] p_hw_wkt  : wkt寄存器块
 * \param[in] inum      : 中断号
 * \param[in] delay_inms: 延时时间（唤醒）
 */
void demo_lpc824_hw_wkt_deeppowerdown_wakeup_entry(amhw_lpc82x_pmu_t *p_hw_pmu,
                                                   amhw_lpc_wkt_t    *p_hw_wkt,
                                                   int                inum,
                                                   uint32_t           delay_inms);
/**
 * \brief wkt 硬件层（deeppowerdown-wakeup）例程入口
 *
 * \param[in] p_hw_wkt   : wkt寄存器块
 * \param[in] wkt_mdelay : 延时时间
 */
void demo_lpc_hw_wkt_timer(amhw_lpc_wkt_t *p_hw_wkt,
                           uint32_t        wkt_mdelay);

/**
 * \brief wkt 标准层（timer）例程入口
 *
 * \param[in] timer_handle   : 操作句柄
 * \param[in] chan           : 通道
 * \param[in] clkin          : 定时器时钟
 */
void demo_lpc_std_wkt_timer_entry (am_timer_handle_t timer_handle,
                                   uint8_t           chan,
                                   uint32_t          clkin);

/**
 * \brief spi 硬件层（master）例程入口
 *
 * \param[in] p_hw_spi  : spi寄存器块  
 * \param[in] clk       : 时钟
 */
void demo_lpc_hw_spi_master_entry(amhw_lpc_spi_t *p_hw_spi,
                                  uint32_t        clk);
/**
 * \brief spi 硬件层（slave）例程入口
 *
 * \param[in] p_hw_spi  : spi寄存器块  
 * \param[in] clk       : 时钟
 */
void demo_lpc_hw_spi_slave_entry (amhw_lpc_spi_t *p_hw_spi,
                                  uint32_t        clk);

/**
 * \brief spi 硬件层（slave）例程入口
 *
 * \param[in] p_hw_spi  : spi寄存器块  
 * \param[in] chan      : dma通道
 */
void demo_lpc824_hw_spi_slave_dma_entry(amhw_lpc_spi_t *p_hw_spi, 
                                        uint8_t         chan);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_NXP_ENTRIES_H */

/* end of file */
