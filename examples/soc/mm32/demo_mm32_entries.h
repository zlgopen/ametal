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
 * \brief ZLG系列外设驱动的所有例程函数入口声明
 * \sa    demo_mm32_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-02  tee, first implementation
 * \endinternal
 */

#ifndef __DEMO_MM32_ENTRIES_H
#define __DEMO_MM32_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"

#include "hw/amhw_mm32_adc.h"
#include "hw/amhw_mm32_exti.h"
#include "hw/amhw_mm32_flash.h"
#include "hw/amhw_mm32_gpio.h"
#include "hw/amhw_mm32_i2c.h"
#include "hw/amhw_mm32_pwr.h"
//#include "hw/amhw_mm32l073_rcc.h"
#include "hw/amhw_mm32_spi.h"
#include "hw/amhw_mm32_dma.h"
#include "hw/amhw_mm32_syscfg.h"
#include "hw/amhw_mm32_tim.h"
#include "hw/amhw_mm32_uart.h"
#include "hw/amhw_mm32_iwdg.h"
#include "hw/amhw_mm32_wwdg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] chan      : 引脚对应的 ADC 通道号
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_mm32_hw_adc_int_entry (amhw_mm32_adc_t *p_hw_adc,
                                int             int_num,
                                int             chan,
                                uint32_t        vref_mv);

/**
 * \brief ADC 硬件层（DMA）例程入口
 *
 */
void demo_mm32_hw_adc_dma_entry (amhw_mm32_adc_t *p_hw_adc,
                                int            *p_adc_chan,
                                int             adc_chan_num,
                                uint8_t         dma_chan);

/**
 * \brief CLK 例程，通过 HW 层接口实现
 *
 * \param[in] p_clk_id_buf 保存时钟号的缓冲区
 * \param[in] buf_lenth    时钟号缓冲区大小
 *
 * \return 无
 */
void demo_mm32_hw_clk_entry (am_clk_id_t *p_clk_id_buf, uint8_t buf_lenth);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * \param[in] dma_chan DMA 通道号
 *
 * \return 无
 */
void demo_mm32_drv_dma_m2m_entry (uint32_t dma_chan);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 *
 * \return 无
 */
void demo_mm32_drv_flash_entry (amhw_mm32_flash_t *p_hw_flash, uint8_t sector);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio  指向 GPIO 外设寄存器块的指针
 * \param[in] input_pin  输入引脚号
 * \param[in] output_pin 输出引脚号
 *
 * \return 无
 */
void demo_mm32_hw_gpio_entry (amhw_mm32_gpio_t *p_hw_gpio,
                             int32_t          input_pin,
                             int32_t          output_pin);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio     指向 GPIO 外设寄存器块的指针
 * \param[in] p_hw_syscfg   指向 SYSCFG 外设寄存器块的指针
 * \param[in] p_hw_exti     指向 EXTI 外设寄存器块的指针
 * \param[in] pin           中断引脚号
 * \param[in] portsource    中断引脚端口
 * \param[in] pinsource     中断引脚源
 * \param[in] exti_line_num 外部中断线号
 *
 * \return 无
 */
void demo_mm32_hw_gpio_trigger_entry (amhw_mm32_gpio_t         *p_hw_gpio,
                                     amhw_mm32_syscfg_t       *p_hw_syscfg,
                                     amhw_mm32_exti_t         *p_hw_exti,
                                     int32_t                  pin,
                                     uint8_t                  portsource,
                                     uint8_t                  pinsource,
                                     amhw_mm32_exti_line_num_t exti_line_num);

/**
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 * \param[in] clk_rate I2C 时钟源频率
 *
 * \return 无
 */
void demo_mm32_hw_i2c_master_poll_entry (amhw_mm32_i2c_t *p_hw_i2c,
                                        uint32_t        clk_rate);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 *
 * \return 无
 */
void demo_mm32_hw_i2c_slave_poll_entry (amhw_mm32_i2c_t *p_hw_i2c);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_spi 指向 SPI 外设寄存器块的指针
 * \param[in] cs_pin   片选引脚号
 * \param[in] clk_rate SPI 时钟源频率
 *
 * \return 无
 */
void demo_mm32_hw_spi_master_entry (amhw_mm32_spi_t *p_hw_spi,
                                   int32_t         cs_pin,
                                   uint32_t        clk_rate);

/**
 * \brief 定时器捕获例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] chan     定时器通道
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] inum     中断号
 *
 * \return 无
 */
void demo_mm32_hw_tim_cap_entry (amhw_mm32_tim_t      *p_hw_tim,
                                amhw_mm32_tim_type_t  type,
                                uint32_t             chan,
                                uint32_t             clk_rate,
                                int32_t              inum);

/**
 * \brief 定时器通道比较匹配引脚翻转例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] chan     定时器通道
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] inum     中断号
 *
 * \return 无
 */
void demo_mm32_hw_tim_cmp_toggle_entry (amhw_mm32_tim_t      *p_hw_tim,
                                       amhw_mm32_tim_type_t  type,
                                       uint32_t             chan,
                                       uint32_t             clk_rate,
                                       uint8_t              inum);

/**
 * \brief 定时器 PWM 输出例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] chan     定时器通道
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] inum     中断号
 *
 * \return 无
 */
void demo_mm32_hw_tim_pwm_entry (amhw_mm32_tim_t     *p_hw_tim,
                                amhw_mm32_tim_type_t type,
                                uint32_t            chan,
                                uint32_t            clk_rate,
                                int32_t             inum);

/**
 * \brief 定时器带死区时间的互补 PWM 输出例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] chan     定时器通道
 * \param[in] clk_rate 定时器时钟源频率
 *
 * \return 无
 */
void demo_mm32_hw_tim_pwm_dead_entry (amhw_mm32_tim_t     *p_hw_tim,
                                     amhw_mm32_tim_type_t type,
                                     uint32_t            chan,
                                     uint32_t            clk_rate);

/**
 * \brief 定时器定时例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim 指向定时器外设寄存器块的指针
 * \param[in] type     定时器类型
 * \param[in] clk_rate 定时器时钟源频率
 * \param[in] inum     中断号
 *
 * \return 无
 */
void demo_mm32_hw_tim_timing_entry (amhw_mm32_tim_t     *p_hw_tim,
                                   amhw_mm32_tim_type_t type,
                                   uint32_t            clk_rate,
                                   int32_t             int_num);

/**
 * \brief UART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] pfn_init  指向 UART 引脚初始化函数的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] uart_base UART 基地址
 * \param[in] inum_uart UART 中断号
 *
 * \return 无
 */
void demo_mm32_hw_uart_int_entry (amhw_mm32_uart_t *p_hw_uart,
                                 void (* pfn_init)(void),
                                 uint32_t         clk_rate,
                                 unsigned long    uart_base,
                                 unsigned char    inum_uart);

/**
 * \brief UART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \return 无
 */
void demo_mm32_hw_uart_polling_entry (amhw_mm32_uart_t *p_hw_uart,
                                     uint32_t         clk_rate);

/**
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 *
 * \return 无
 */
void demo_mm32_hw_uart_rx_dma_entry (amhw_mm32_uart_t *p_hw_uart,
                                    uint32_t         clk_rate,
                                    int32_t          dma_chan);

/**
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 *
 * \return 无
 */
void demo_mm32_hw_uart_tx_dma_entry (amhw_mm32_uart_t *p_hw_uart,
                                    uint32_t         clk_rate,
                                    int32_t          dma_chan);
                                    
/**
 * \brief 睡眠模式例程，使用定时器周期唤醒，通过驱动层接口实现
 *
 * \param[in] timer_handle   定时器标准服务句柄
 * \param[in] timer_clk_rate 在系统时钟切换为 LSI 时，定时器的时钟频率
 *
 * \return 无
 */
void demo_mm32l073_drv_sleepmode_timer_wake_up_entry (am_timer_handle_t timer_handle,
                                                    uint32_t          timer_clk_rate);

/**
 * \brief 睡眠模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_mm32l073_drv_sleepmode_wake_up_entry (void);

/**
 * \brief 待机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_mm32l073_drv_standbymode_wake_up_entry (void);

/**
 * \brief 停机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_mm32l073_drv_stopmode_wake_up_entry (void);

/**
 * \brief IWDG 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_iwdg    指向 IWDG 外设寄存器块的指针
 * \param[in] time_out_ms  看门狗超时时间
 * \param[in] feed_time_ms 喂狗时间
 *
 * \return 无
 */
void demo_mm32_hw_iwdg_entry (amhw_mm32_iwdg_t *p_hw_iwdg,
                             uint32_t         time_out_ms,
                             uint32_t         feed_time_ms);

/**
 * \brief WWDG 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_wwdg    指向 WWDG 外设寄存器块的指针
 * \param[in] clk_rate     WWDG 时钟源频率
 * \param[in] time_out_ms  看门狗超时时间
 * \param[in] feed_time_ms 喂狗时间
 *
 * \return 无
 */
void demo_mm32_hw_wwdg_entry (amhw_mm32_wwdg_t *p_hw_wwdg,
                             uint32_t         clk_rate,
                             uint32_t         time_out_ms,
                             uint32_t         feed_time_ms);


/**
 * \brief ZML166 ADC pt100测温例程
 * \param[in] handle      ZML166 ADC 服务句柄
 * \param[in] p_para      PT100电阻校准参数
 * \return 无
 */
void demo_zml166_adc_pt100_measure_entry(void                   *p_handle,
                                         float                  *p_para);


/**
 * \brief ZML166 ADC 测量固定电压例程
 * \param[in] handle      ZML166 ADC 服务句柄
 * \param[in] p_para      flash 电压校准参数
 * \param[in] gpa_index   ZML166 ADC增益倍数设置
 * \return 无
 */
void dome_zml166_adc_vol_measure_entry(void                   *p_handle,
                                       float                  *p_para,
                                       uint8_t                 gpa_index);

/**
 * \brief ZML166 ADC 热电偶测温例程
 * \param[in] handle      ZML166 ADC 服务句柄
 * \param[in] p_para      校准参数
 * \param[in] type        热电偶类型
 * \return 无
 */
void dome_zml166_adc_thermocouple_measure_entry(void              *p_handle,
                                                float             *p_para,
                                                uint8_t            type);

/**
 * \brief ZML166 ADC 电压校准程序
 * \param[in] handle      ZML166 ADC 服务句柄
 * \param[in] uart_handle 串口句柄
 * \param[out] p_para      校准参数输出
 * \return 无
 */
void demo_zml166_adc_vol_para_adjuet_entry(void                   *p_handle,
                                           am_uart_handle_t        uart_handle,
                                           float                  *p_para);
#ifdef __cplusplus
}
#endif

#endif /* __DEMO_MM32_ENTRIES_H */

/* end of file */
