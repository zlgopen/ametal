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
 * \brief HC32系列外设驱动的所有例程函数入口声明
 * \sa    demo_hc32_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-02  tee, first implementation
 * \endinternal
 */

#ifndef __DEMO_HC32_ENTRIES_H
#define __DEMO_HC32_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"
#include "am_can.h"

#include "hw/amhw_hc32_adc.h"
#include "hw/amhw_hc32_flash.h"
#include "hw/amhw_hc32_gpio.h"
#include "hw/amhw_hc32_i2c.h"
#include "hw/amhw_hc32_spi.h"
#include "hw/amhw_hc32_dma.h"
#include "hw/amhw_hc32_tim.h"
#include "hw/amhw_hc32_uart.h"
#include "hw/amhw_hc32_wdt.h"
#include "hw/amhw_hc32_lvd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] chan      : 引脚对应的 ADC通道号
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 *
 * \note  一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 *        配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \retval 无
 */
void demo_hc32_hw_adc_int_entry (void    *p_hw_adc,
                                 int      int_num,
                                 uint8_t  chan,
                                 uint32_t vref_mv);

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc   : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num    : 中断号
 * \param[in] vref_mv    : ADC参考电压，用以将ADC采样结果转换为电压值显示
 * \param[in] p_adc_chan : ADC通道数组指针
 * \param[in] sqr_num    : 规则通道个数
 * \param[in] jqr_num    : 插入通道个数
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 *       配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \retval 无
 */
void demo_hc32_hw_multichan_adc_entry (void    *p_hw_adc,
                                       int      int_num,
                                       uint32_t vref_mv,
                                       int     *p_adc_chan,
                                       uint8_t  sqr_num,
                                       uint8_t  jqr_num);

/**
 * \brief ADC 硬件层（DMA）例程入口
 *
 * \param[in] p_hw_adc   : 指向 ADC 外设寄存器块的指针
 * \param[in] p_adc_chan : ADC通道数组指针
 * \param[in] chan_num   : ADC通道个数
 * \param[in] vref_mv    : ADC参考电压，用以将ADC采样结果转换为电压值显示
 * \param[in] dma_chan   : DMA通道号
 * \param[in] dma_src    : DMA传输触发源
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC模块相应的时钟，
 *       配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \retval 无
 */
void demo_hc32_hw_adc_dma_entry (void    *p_hw_adc,
                                 int     *p_adc_chan,
                                 int      chan_num,
                                 uint32_t vref_mv,
                                 int      dma_chan,
                                 uint8_t  dma_src);

/**
 * \brief CLK 例程，通过 HW 层接口实现
 *
 * \param[in] p_clk_id_buf 保存时钟号的缓冲区
 * \param[in] buf_lenth    时钟号缓冲区大小
 *
 * \retval 无
 */
void demo_hc32_hw_clk_entry (am_clk_id_t *p_clk_id_buf, uint8_t buf_lenth);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * \param[in] dma_chan  DMA 通道号
 *
 * \retval 无
 */
void demo_hc32_drv_dma_m2m_entry (uint32_t dma_chan);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 * \param[in] sector     使用 FLASH 扇区号
 *
 * \retval 无
 */
void demo_hc32_drv_flash_entry (void *p_hw_flash, uint16_t sector);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio  指向 GPIO外设寄存器块的指针
 * \param[in] input_pin  输入引脚号
 * \param[in] output_pin 输出引脚号
 *
 * \retval 无
 */
void demo_hc32_hw_gpio_entry (void    *p_hw_gpio,
                              int32_t  input_pin,
                              int32_t  output_pin);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio     指向 GPIO 外设寄存器块的指针
 * \param[in] pin           中断引脚号
 *
 * \retval 无
 */
void demo_hc32_hw_gpio_trigger_entry (void *p_hw_gpio, int32_t pin);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_spi 指向 SPI 外设寄存器块的指针
 * \param[in] cs_pin   片选引脚号
 *
 * \retval 无
 */
void demo_hc32_hw_spi_master_entry (void    *p_hw_spi,
                                    int32_t  cs_pin);

/**
 * \brief 定时器捕获例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] chan     定时器通道
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] inum     中断号
 *
 * \retval 无
 */
void demo_hc32_hw_tim_cap_entry (void     *p_hw_tim,
                                 uint32_t  chan,
                                 uint32_t  clk_rate,
                                 int32_t   inum);

/**
 * \brief 定时器通道比较匹配引脚翻转例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] chan     定时器通道
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] clk_div  定时器时钟源分频系数
 * \param[in] inum     中断号
 *
 * \retval 无
 */
void demo_hc32_hw_tim_cmp_toggle_entry (void    *p_hw_tim,
                                        uint8_t  type,
                                        uint32_t chan,
                                        uint32_t clk_rate,
                                        uint8_t  clk_div,
                                        uint8_t  inum);

/**
 * \brief 定时器通道带死区的互补PWM输出例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim  指向定时器外设寄存器块的指针
 * \param[in] type      定时器类型
 * \param[in] chan_a    定时器互补PWM通道A
 * \param[in] chan_b    定时器互补PWM通道B
 * \param[in] duty_us   脉宽时间（us）
 * \param[in] period_us 周期时间（us）
 * \param[in] dead_us   死区时间设置（us）
 *
 * \retval 无
 */
void demo_hc32_hw_tim_pwm_dead_entry (void    *p_hw_tim,
                                      uint8_t  type,
                                      uint8_t  chan_a,
                                      uint8_t  chan_b,
                                      uint32_t duty_us,
                                      uint32_t period_us,
                                      uint16_t dead_us);

/**
 * \brief 高级定时器软件软件同步开始、同步停止、同步清零例程，通过 HW层接口实现
 *
 * \param[in] p_tim_cfg 指向软件同步配置结构体的指针（amhw_hc32_adtim_sw_sync_t）
 *
 * \retval 无
 */
void demo_hc32_hw_adtim_swsync_entry (void *p_tim_cfg);

/**
 * \brief 高级定时器端口、软件刹车例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adtim 指向定时器外设寄存器块的指针
 * \param[in] chan       定时器通道
 * \param[in] duty_ns    脉宽时间
 * \param[in] period_ns  周期时间
 * \param[in] output_pin 输出引脚
 * \param[in] src        ADT端口触发控制 - 触发源选择
 *
 * \retval 无
 */
void demo_hc32_hw_adtim_port_sw_brake_entry (void          *p_hw_adtim,
                                             uint8_t        chan,
                                             unsigned long  duty_ns,
                                             unsigned long  period_ns,
                                             int            output_pin,
                                             uint8_t        src);

/**
 * \brief 高级定时器VC刹车例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adtim 指向定时器外设寄存器块的指针
 * \param[in] chan       定时器通道
 * \param[in] duty_ns    脉宽时间
 * \param[in] period_ns  周期时间
 * \param[in] p_hw_vc    指向VC外设寄存器块的指针
 * \param[in] vc0_n      VC0 电压比较器 “-”端输入选择
 * \param[in] vc0_p      VC0 电压比较器 “+”端输入选择
 *
 * \retval 无
 */
void demo_hc32_hw_adtim_vc_brake_entry (void         *p_hw_adtim,
                                        uint8_t       chan,
                                        unsigned long duty_ns,
                                        unsigned long period_ns,
                                        void         *p_hw_vc,
                                        uint8_t       vc0_n,
                                        uint8_t       vc0_p);

/**
 * \brief 高级定时器定时触发ADC转换例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adtim  指向定时器外设寄存器块的指针
 * \param[in] period_us   周期时间
 * \param[in] p_hw_adc    指向ADC外设寄存器块的指针
 * \param[in] adc_int_num ADC中断号
 * \param[in] p_adc_chan  ADC通道编号数组
 * \param[in] sqr_num     ADC顺序通道数量
 *
 * \retval 无
 */
void demo_hc32_hw_adtim_trig_adc_sqr_entry (void    *p_hw_adtim,
                                            uint32_t period_us,
                                            void    *p_hw_adc,
                                            int      adc_int_num,
                                            int     *p_adc_chan,
                                            uint8_t  sqr_num);

/**
 * \brief 通用定时器端口刹车例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim   指向定时器外设寄存器块的指针
 * \param[in] type       定时器类型
 * \param[in] chan       定时器通道
 * \param[in] inum       定时器中断号
 * \param[in] duty_ns    脉宽时间
 * \param[in] period_ns  周期时间
 *
 * \retval 无
 */
void demo_hc32_hw_tim_port_brake_entry (void    *p_hw_tim,
                                        uint8_t  type,
                                        uint8_t  chan,
                                        int      inum,
                                        uint32_t duty_ns,
                                        uint32_t period_ns);

/**
 * \brief 通用定时器VC刹车例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adtim 指向定时器外设寄存器块的指针
 * \param[in] chan       定时器通道
 * \param[in] duty_ns    脉宽时间
 * \param[in] period_ns  周期时间
 * \param[in] p_hw_vc    指向VC外设寄存器块的指针
 * \param[in] vc0_n      VC0 电压比较器 “-”端输入选择
 * \param[in] vc0_p      VC0 电压比较器 “+”端输入选择
 *
 * \retval 无
 */
void demo_hc32_hw_tim_vc_brake_entry (void         *p_hw_tim,
                                      uint8_t       chan,
                                      unsigned long duty_ns,
                                      unsigned long period_ns,
                                      void         *p_hw_vc,
                                      uint8_t       vc0_n,
                                      uint8_t       vc0_p);

/**
 * \brief 定时器 PWM 输出例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] chan     定时器通道
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] clk_div  定时器时钟源分频系数
 * \param[in] inum     中断号
 *
 * \retval 无
 */
void demo_hc32_hw_tim_pwm_entry (void    *p_hw_tim,
                                 uint8_t  type,
                                 uint32_t chan,
                                 uint32_t clk_rate,
                                 uint8_t  clk_div,
                                 int32_t  inum);

/**
 * \brief 定时器定时例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] int_num  中断号
 *
 * \retval 无
 */
void demo_hc32_hw_tim_timing_entry (void    *p_hw_tim,
                                    uint8_t  type,
                                    uint32_t clk_rate,
                                    int32_t  int_num);

/**
 * \brief 通用定时器定时触发ADC转换例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim    指向定时器外设寄存器块的指针
 * \param[in] type        定时器类型
 * \param[in] chan        定时器通道
 * \param[in] duty_us     脉宽时间
 * \param[in] period_us   周期时间
 * \param[in] p_hw_adc    指向ADC外设寄存器块的指针
 * \param[in] adc_int_num ADC中断号
 * \param[in] p_adc_chan  ADC通道编号数组
 * \param[in] sqr_num     ADC顺序通道数量
 *
 * \retval 无
 */
void demo_hc32_hw_tim_trig_adc_sqr_entry (void    *p_hw_tim,
                                          uint8_t  type,
                                          uint8_t  chan,
                                          uint32_t duty_us,
                                          uint32_t period_us,
                                          void    *p_hw_adc,
                                          int      adc_int_num,
                                          int     *p_adc_chan,
                                          uint8_t  sqr_num);

/**
 * \brief UART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] pfn_init  指向 UART 引脚初始化函数的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] uart_base UART 基地址
 * \param[in] inum_uart UART 中断号
 *
 * \retval 无
 */
void demo_hc32_hw_uart_int_entry (void             *p_hw_uart,
                                  void            (*pfn_init) (void),
                                  uint32_t          clk_rate,
                                  unsigned long     uart_base,
                                  unsigned char     inum_uart);

/**
 * \brief UART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \retval 无
 */
void demo_hc32_hw_uart_polling_entry (void     *p_hw_uart,
                                      uint32_t  clk_rate);

/**
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \retval 无
 */
void demo_hc32_hw_uart_rx_dma_entry (void    *p_hw_uart,
                                     uint32_t clk_rate,
                                     int32_t  dma_chan,
                                     int32_t  dma_src);

/**
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \retval 无
 */
void demo_hc32_hw_uart_tx_dma_entry (void    *p_hw_uart,
                                     uint32_t clk_rate,
                                     int32_t  dma_chan,
                                     int32_t  dma_src);

/**
 * \brief LPUART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_lpuart   指向LPUART 外设寄存器块的指针
 * \param[in] pfn_init      指向 LPUART 引脚初始化函数的指针
 * \param[in] sclk_src      LPUART 时钟源选择
 * \param[in] clk_rate      LPUART 时钟源频率
 * \param[in] baudrate      LPUART 波特率
 * \param[in] inum_lpuart   LPUART 中断号
 *
 * \retval 无
 */
void demo_hc32_hw_lpuart_int_entry (void             *p_hw_lpuart,
                                    void            (*pfn_init) (void),
                                    uint8_t           sclk_src,
                                    uint32_t          clk_rate,
                                    uint32_t          baudrate,
                                    unsigned char     inum_lpuart);

/**
 * \brief LPUART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_lpuart 指向 LPUART 外设寄存器块的指针
 * \param[in] clk_src     LPUART 时钟源选择
 * \param[in] clk_rate    LPUART 时钟源频率
 * \param[in] baudrate    LPUART 波特率
 *
 * \retval 无
 */
void demo_hc32_hw_lpuart_polling_entry (void     *p_hw_lpuart,
                                        uint8_t   clk_src,
                                        uint32_t  clk_rate,
                                        uint32_t  baudrate);

/**
 * \brief LPUART DMA 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_lpuart 指向 LPUART 外设寄存器块的指针
 * \param[in] clk_src     LPUART 时钟源选择
 * \param[in] clk_rate    LPUART 时钟源频率
 * \param[in] baudrate    LPUART 波特率
 * \param[in] dma_chan    DMA 通道号
 * \param[in] dma_src     DMA 触发源选择
 *
 * \retval 无
 */
void demo_hc32_hw_lpuart_rx_dma_entry (void    *p_hw_lpuart,
                                       uint8_t  clk_src,
                                       uint32_t clk_rate,
                                       uint32_t baudrate,
                                       int32_t  dma_chan,
                                       int32_t  dma_src);

/**
 * \brief LPUART DMA 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_lpuart 指向 LPUART 外设寄存器块的指针
 * \param[in] clk_src     LPUART 时钟源选择
 * \param[in] clk_rate    LPUART 时钟源频率
 * \param[in] baudrate    LPUART 波特率
 * \param[in] dma_chan    DMA 通道号
 * \param[in] dma_src     DMA 触发源选择
 *
 * \retval 无
 */
void demo_hc32_hw_lpuart_tx_dma_entry (void    *p_hw_lpuart,
                                       uint8_t  clk_src,
                                       uint32_t clk_rate,
                                       uint32_t baudrate,
                                       int32_t  dma_chan,
                                       int32_t  dma_src);

/**
 * \brief HC32 I2C 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c     指向 I2C 外设寄存器块的指针
 * \param[in] clk_rate     I2c工作频率
 *
 * \return 无
 */
void demo_hc32_hw_i2c_master_poll_entry (void      *p_hw_i2c,
                                         uint32_t   clk_rate);

/**
 * \brief HC32 I2C 从机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c     指向 I2C 外设寄存器块的指针
 * \param[in] inum_num     I2c中断号
 *
 * \return 无
 */
void demo_hc32_hw_i2c_slave_int_entry (void    *p_hw_i2c,
                                       int      inum_num);

/**
 * \brief HC32 WDT 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_wdt     指向 WDT 外设寄存器块的指针
 * \param[in] time_out_ms  看门狗超时时间
 * \param[in] feed_time_ms 喂狗时间
 *
 * \retval 无
 */
void demo_hc32_hw_wdt_entry (void     *p_hw_wdt,
                             uint32_t  time_out_ms,
                             uint32_t  feed_time_ms);

/**
 * \brief DAC输出电压例程，hw接口层实现
 *
 * \param[in] p_hw_dac  指向DAC寄存器指针
 * \param[in] dma_src   DMA请求源
 * \param[in] dma_chan  DMA通道
 * \param[in] p_vol_val 电压值
 *
 * \retval 无
 */
void demo_hc32_hw_dac_dma_entry(void     *p_hw_dac,
                                int32_t   dma_src,
                                int32_t   dma_chan,
                                uint16_t *p_vol_val);

/**
 * \brief VC中断触发例程，hw接口层实现
 *
 * \param[in] p_hw_vc   指向VC寄存器指针
 * \param[in] pfn_cb_t  中断触发回调函数
 * \param[in] p_arg     中断触发回调函数参数
 * \param[in] p_hw_dac  指向ADC寄存器指针
 * \param[in] mv_val    DAC 输出电压
 *
 */
void demo_hc32_hw_vc_int_entry (void      *p_hw_vc,
                                void     (*pfn_cb_t) (void *),
                                void      *p_arg,
                                void      *p_hw_dac,
                                uint16_t   mv_val);

/**
 * \brief LVD系统复位例程，hw接口层实现
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器指针
 * \param[in] lvd_vids : LVD 阈值电压，amhw_hc32_lvd_vids_t
 * \param[in] lvd_src  : LVD 监测来源选择，宏定义 AMHW_HC32_LVD_SRC_
 *
 * \retval 无
 */
void demo_hc32_hw_lvd_system_reset_entry (amhw_hc32_lvd_t     *p_hw_base,
                                          amhw_hc32_lvd_vids_t lvd_vids,
                                          uint32_t             lvd_src);

/**
 * \brief LVD系统复位例程，hw接口层实现
 *
 * \param[in] p_hw_lvd   : 指向LVD寄存器指针
 * \param[in] lvd_vids   : LVD 阈值电压，amhw_hc32_lvd_vids_t
 * \param[in] lvd_src    : LVD 监测来源选择，宏定义 AMHW_HC32_LVD_SRC_
 * \param[in] pfn_lvd_cb : 用户回调函数
 * \param[in] p_arg      : 用户回调函数参数
 *
 * \retval 无
 */
void demo_hc32_hw_lvd_int_entry (amhw_hc32_lvd_t      *p_hw_base,
                                 amhw_hc32_lvd_vids_t  lvd_vids,
                                 uint32_t              lvd_src,
                                 am_pfnvoid_t          pfn_lvd_cb,
                                 void                 *p_arg);

/**
 * \brief OPA 通用运算放大器模式，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa :指向OPA寄存器指针
 *
 * \retval 无
 */
void demo_hc32_hw_opa_entry (void *p_hw_opa);

/**
 * \brief OPA DAC电压跟随例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa  : 指向OPA寄存器指针
 * \param[in] p_hw_dac  : 指向DAC寄存器指针
 * \param[in] mode      : OPA工作模式
 * \param[in] p_vol_val : DAC设置电压值数组
 * \param[in] number    : DAC设置电压值数组长度
 *
 * \retval 无
 */
void demo_hc32_hw_opa_dac_entry (void      *p_hw_opa,
                                 void      *p_hw_dac,
                                 uint8_t    mode,
                                 uint16_t  *p_vol_val,
                                 uint32_t   number);

/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_aes    : 指向AES寄存器指针
 * \param[in] key_len     : 密钥长度
 * \param[in] p_aes_data  : AES加密明文
 * \param[in] p_key_data  : AES密钥
 *
 * \retval 无
 */
void demo_hc32_hw_aes_ency_decy_entry (void     *p_hw_aes,
                                       uint8_t   key_len,
                                       uint32_t *p_aes_data,
                                       uint32_t *p_key_data);

/**
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_trng   : 指向TRNG寄存器指针
 *
 * \retval 无
 */
void demo_hc32_hw_trng_generate_entry (void *p_hw_trng);

/**
 * \brief LCD 显示例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_lcd   : 指向lcd寄存器指针
 * \param[in] p_data     : LCD显示数据
 *
 * \retval 无
 */
void demo_hc32_hw_lcd_entry (void *p_hw_lcd, uint32_t *p_data);


/******************************************************************************/

/**
 * \brief hc32x3x OPA例程，通过异步标准接口实现
 *
 * \param[in] handle      OPA 标准服务句柄
 * \param[in] ch          OPA通道
 *
 * \return 无
 */
void demo_hc32x3x_drv_opa_entry (void *p_handle, uint8_t ch);

/**
 * \brief hc32x3x OPA 电压跟随例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa :指向OPA寄存器指针
 * \param[in] mode     :OPA工作模式
 * \param[in] ch       :OPA通道
 *
 * \retval :无
 */
void demo_hc32x3x_hw_opa_entry (void *p_hw_opa, uint8_t mode, uint8_t ch);

/**
 * \brief RTC 实时时钟例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_rtc    : 指向RTC寄存器指针
 *
 * \retval 无
 */
void demo_hc32_hw_rtc_entry (void *p_hw_rtc);

/**
 * \brief FLASH 写保护例程
 *
 * \param[in] p_hw_flash 指向flash寄存器块的指针
 * \param[in] sector   扇区编号
 *
 * \return 无
 */
void demo_hc32_drv_flash_write_protect_entry (amhw_hc32_flash_t *p_hw_flash, uint16_t sector);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_HC32_ENTRIES_H */

/* end of file */
