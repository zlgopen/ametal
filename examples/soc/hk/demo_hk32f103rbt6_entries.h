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
 * \brief HK32F103系列外设驱动的所有例程函数入口声明
 * \sa    demo_hk32f103rbt6_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-02  tee, first implementation
 * \endinternal
 */

#ifndef __DEMO_HK32F103RBT6_ENTRIES_H
#define __DEMO_HK32F103RBT6_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"
#include "am_can.h"
#include "hw/amhw_hk32f103rbt6_adc.h"
#include "hw/amhw_hk32f103rbt6_exti.h"
#include "hw/amhw_hk32f103rbt6_flash.h"
#include "hw/amhw_hk32f103rbt6_gpio.h"
#include "hw/amhw_hk32f103rbt6_i2c.h"
#include "hw/amhw_hk32f103rbt6_pwr.h"
#include "hw/amhw_hk32f103rbt6_spi.h"
#include "hw/amhw_hk32f103rbt6_dma.h"
#include "hw/amhw_hk32f103rbt6_syscfg.h"
#include "hw/amhw_hk32f103rbt6_tim.h"
#include "hw/amhw_hk32f103rbt6_usart.h"
#include "hw/amhw_hk32f103rbt6_iwdg.h"
#include "hw/amhw_hk32f103rbt6_wwdg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ADC 硬件层（使用了中断）规则通道例程入口
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
void demo_hk32f103rbt6_hw_adc_int_entry (amhw_hk32f103rbt6_adc_t *p_hw_adc,
                                int             int_num,
                                int             chan,
                                uint32_t        vref_mv);

/**
 * \brief ADC 硬件层（DMA）规则通道例程入口
 *
 */
void demo_hk32f103rbt6_hw_adc_dma_entry (amhw_hk32f103rbt6_adc_t *p_hw_adc,
                                int            *p_adc_chan,
                                int             adc_chan_num,
                                uint8_t         dma_chan);

/**
 * \brief hk32f103rbt6 ADC 硬件层（使用了中断） 注入通道例程入口
 *
 * \param[in] p_hw_adc     : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num      : 中断号
 * \param[in] vref_mv      : 参考电压（mv）
 * \param[in] p_adc_chan   : 指向ADC通道数组的指针
 * \param[in] adc_chan_num : ADC通道个数
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_adc_injected_int_entry (void              *p_hw_adc,
                                            int                int_num,
                                            uint32_t           vref_mv,
                                            int               *p_adc_chan,
                                            int                adc_chan_num);

/**
 * \brief hk32f103rbt6 ADC 硬件层（使用了中断）规则通道、注入通道例程入口
 *
 * \param[in] p_hw_adc     : 指向 ADC1外设寄存器块的指针
 * \param[in] p_adc_chan   : 指向ADC1通道数组的指针
 * \param[in] adc_chan_num : ADC1通道个数
 * \param[in] dma_chan     : DMA通道
 * \param[in] p_hw_adc     : 指向 ADC2外设寄存器块的指针
 * \param[in] p_adc_chan   : 指向ADC2通道数组的指针
 * \param[in] adc_chan_num : ADC2通道个数
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_adc_dma_double_entry (void              *p_hw_adc1,
                                          int               *p_adc1_chan,
                                          int                adc1_chan_num,
                                          uint8_t            dma_chan,
                                          void              *p_hw_adc2,
                                          int               *p_adc2_chan,
                                          int                adc2_chan_num);

/**
 * \brief CLK 例程，通过 HW 层接口实现
 *
 * \param[in] p_clk_id_buf 保存时钟号的缓冲区
 * \param[in] buf_lenth    时钟号缓冲区大小
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_clk_entry (am_clk_id_t *p_clk_id_buf, uint8_t buf_lenth);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * \param[in] dma_chan DMA 通道号
 *
 * \return 无
 */
void demo_hk32f103rbt6_drv_dma_m2m_entry (uint32_t dma_chan);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 *
 * \return 无
 */
void demo_hk32f103rbt6_drv_flash_entry (amhw_hk32f103rbt6_flash_t *p_hw_flash, uint8_t sector);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio  指向 GPIO 外设寄存器块的指针
 * \param[in] input_pin  输入引脚号
 * \param[in] output_pin 输出引脚号
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_gpio_entry (amhw_hk32f103rbt6_gpio_t *p_hw_gpio,
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
void demo_hk32f103rbt6_hw_gpio_trigger_entry (amhw_hk32f103rbt6_gpio_t         *p_hw_gpio,
                                     amhw_hk32f103rbt6_syscfg_t       *p_hw_syscfg,
                                     amhw_hk32f103rbt6_exti_t         *p_hw_exti,
                                     int32_t                  pin,
                                     uint8_t                  portsource,
                                     uint8_t                  pinsource,
                                     amhw_hk32f103rbt6_exti_line_num_t exti_line_num);

/**
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 * \param[in] clk_rate I2C 时钟源频率
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_i2c_master_poll_entry (amhw_hk32f103rbt6_i2c_t *p_hw_i2c,
                                        uint32_t        clk_rate);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_i2c_slave_poll_entry (amhw_hk32f103rbt6_i2c_t *p_hw_i2c);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_spi 指向 SPI 外设寄存器块的指针
 * \param[in] cs_pin   片选引脚号
 * \param[in] clk_rate SPI 时钟源频率
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_spi_master_entry (amhw_hk32f103rbt6_spi_t *p_hw_spi,
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
void demo_hk32f103rbt6_hw_tim_cap_entry (amhw_hk32f103rbt6_tim_t      *p_hw_tim,
                                amhw_hk32f103rbt6_tim_type_t  type,
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
void demo_hk32f103rbt6_hw_tim_cmp_toggle_entry (amhw_hk32f103rbt6_tim_t      *p_hw_tim,
                                       amhw_hk32f103rbt6_tim_type_t  type,
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
void demo_hk32f103rbt6_hw_tim_pwm_entry (amhw_hk32f103rbt6_tim_t     *p_hw_tim,
                                amhw_hk32f103rbt6_tim_type_t type,
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
void demo_hk32f103rbt6_hw_tim_pwm_dead_entry (amhw_hk32f103rbt6_tim_t     *p_hw_tim,
                                     amhw_hk32f103rbt6_tim_type_t type,
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
void demo_hk32f103rbt6_hw_tim_timing_entry (amhw_hk32f103rbt6_tim_t     *p_hw_tim,
                                   amhw_hk32f103rbt6_tim_type_t type,
                                   uint32_t            clk_rate,
                                   int32_t             int_num);

/**
 * \brief HK32F103 UART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] pfn_init  指向 UART 引脚初始化函数的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] uart_base UART 基地址
 * \param[in] inum_uart UART 中断号
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_uart_int_entry (amhw_hk32f103rbt6_usart_t *p_hw_uart,
                                 void (* pfn_init)(void),
                                 uint32_t         clk_rate,
                                 unsigned long    uart_base,
                                 unsigned char    inum_uart);

/**
 * \brief HK32F103 UART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_uart_polling_entry (amhw_hk32f103rbt6_usart_t *p_hw_uart,
                                     uint32_t         clk_rate);

/**
 * \brief HK32F103 UART 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_uart_rx_dma_entry (amhw_hk32f103rbt6_usart_t *p_hw_uart,
                                    uint32_t         clk_rate,
                                    int32_t          dma_chan);

/**
 * \brief HK32F103 UART 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_uart_tx_dma_entry (amhw_hk32f103rbt6_usart_t *p_hw_uart,
                                    uint32_t         clk_rate,
                                    int32_t          dma_chan);

/**
 * \brief HK32F103RBT6 USART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] pfn_init  指向 USART 引脚初始化函数的指针
 * \param[in] clk_rate  USART 时钟源频率
 * \param[in] uart_base USART 基地址
 * \param[in] inum_uart USART 中断号
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_usart_int_entry (void                *p_hw_usart,
                                     void (* pfn_init)(void),
                                     uint32_t             clk_rate,
                                     unsigned long        usart_base,
                                     unsigned char        inum_usart);

/**
 * \brief HK32F103RBT6 USART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] clk_rate   USART 时钟源频率
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_usart_polling_entry (void                *p_hw_usart,
                                         uint32_t             clk_rate);

/**
 * \brief HK32F103RBT6 USART 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] clk_rate   USART 时钟源频率
 * \param[in] dma_chan   DMA 通道号
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_usart_rx_dma_entry (void                *p_hw_usart,
                                        uint32_t             clk_rate,
                                        int32_t              dma_chan);

/**
 * \brief HK32F103RBT6 USART 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] clk_rate   USART 时钟源频率
 * \param[in] dma_chan   DMA 通道号
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_usart_tx_dma_entry (void                *p_hw_usart,
                                        uint32_t             clk_rate,
                                        int32_t              dma_chan);

/**
 * \brief 睡眠模式例程，使用定时器周期唤醒，通过驱动层接口实现
 *
 * \param[in] timer_handle   定时器标准服务句柄
 * \param[in] timer_clk_rate 在系统时钟切换为 LSI 时，定时器的时钟频率
 *
 * \return 无
 */
void demo_hk32f103rbt6_drv_sleepmode_timer_wake_up_entry (am_timer_handle_t timer_handle,
                                                    uint32_t          timer_clk_rate);

/**
 * \brief 睡眠模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_hk32f103rbt6_drv_sleepmode_wake_up_entry (void);

/**
 * \brief 待机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_hk32f103rbt6_drv_standbymode_wake_up_entry (void);

/**
 * \brief 停机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_hk32f103rbt6_drv_stopmode_wake_up_entry (void);

/**
 * \brief IWDG 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_iwdg    指向 IWDG 外设寄存器块的指针
 * \param[in] time_out_ms  看门狗超时时间
 * \param[in] feed_time_ms 喂狗时间
 *
 * \return 无
 */
void demo_hk32f103rbt6_hw_iwdg_entry (amhw_hk32f103rbt6_iwdg_t *p_hw_iwdg,
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
void demo_hk32f103rbt6_hw_wwdg_entry (amhw_hk32f103rbt6_wwdg_t *p_hw_wwdg,
                             uint32_t         clk_rate,
                             uint32_t         time_out_ms,
                             uint32_t         feed_time_ms);

/**
 * \brief CAN 中断例程入口
 *
 * \param[in] am_can_handle_t     can服务句柄
 * \param[in] am_can_bps_param_t  指向波特率参数结构体的指针
 * \param[in] am_can_int_type_t   中断使能类型
 * \param[in] uint8_t             指向过滤table的指针
 * \param[in] size_t              过滤table的长度
 * \return 无
 */
void demo_hk32f103rbt6_can_int_entry (am_can_handle_t     can_handle,
                                am_can_bps_param_t *can_btr_baud,
                                am_can_int_type_t   int_type,
                                uint8_t            *p_filterbuff,
                                size_t              lenth);
/**
 * \brief 例程入口
 */

/**
 * \brief CAN 例程入口
 *
 * \param[in] am_can_handle_t     can服务句柄
 * \param[in] am_can_bps_param_t  指向波特率参数结构体的指针
 * \param[in] am_can_int_type_t   中断使能类型
 *
 * \return 无
 */
void demo_hk32f103rbt6_can_entry (am_can_handle_t     can_handle,
                            am_can_bps_param_t *can_btr_baud);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_HK32F103RBT6_ENTRIES_H */

/* end of file */
