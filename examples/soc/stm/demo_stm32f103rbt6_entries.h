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
 * \brief STM32F103RBT6系列外设驱动的所有例程函数入口声明
 * \sa    demo_stm32f103rbt6_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-02  tee, first implementation
 * \endinternal
 */

#ifndef __DEMO_STM32F103RBT6_ENTRIES_H
#define __DEMO_STM32F103RBT6_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"
#include "am_can.h"

#include "hw/amhw_stm32f103rbt6_adc.h"
#include "hw/amhw_stm32f103rbt6_exti.h"
#include "hw/amhw_stm32f103rbt6_flash.h"
#include "hw/amhw_stm32f103rbt6_gpio.h"
#include "hw/amhw_stm32f103rbt6_i2c.h"
#include "hw/amhw_stm32f103rbt6_pwr.h"
#include "hw/amhw_stm32f103rbt6_spi.h"
#include "hw/amhw_stm32f103rbt6_dma.h"
#include "hw/amhw_stm32f103rbt6_syscfg.h"
#include "hw/amhw_stm32f103rbt6_tim.h"
#include "hw/amhw_stm32f103rbt6_usart.h"
#include "hw/amhw_stm32f103rbt6_iwdg.h"
#include "hw/amhw_stm32f103rbt6_wwdg.h"
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
void demo_stm32f103rbt6_hw_adc_int_entry (amhw_stm32f103rbt6_adc_t *p_hw_adc,
                                int             int_num,
                                int             chan,
                                uint32_t        vref_mv);

/**
 * \brief ADC 硬件层（DMA）规则通道例程入口
 *
 */
void demo_stm32f103rbt6_hw_adc_dma_entry (amhw_stm32f103rbt6_adc_t *p_hw_adc,
                                int            *p_adc_chan,
                                int             adc_chan_num,
                                uint8_t         dma_chan);

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] chan      : 引脚对应的 ADC通道号
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_adc_int_entry (void    *p_hw_adc,
                                   int      int_num,
                                   uint8_t  chan,
                                   uint32_t vref_mv);

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 * \param[in] p_adc_chan: ADC通道数组指针
 * \param[in] sqr_num   : 规则通道个数
 * \param[in] jqr_num   : 插入通道个数
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_multichan_adc_entry (void    *p_hw_adc,
                                         int      int_num,
                                         uint32_t vref_mv,
                                         int     *p_adc_chan,
                                         uint8_t  sqr_num,
                                         uint8_t  jqr_num);

/**
 * \brief ADC 硬件层（DMA）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] p_adc_chan: ADC通道数组指针
 * \param[in] chan_num  : ADC通道个数
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 * \param[in] dma_chan  : DMA通道号
 * \param[in] dma_src   : DMA传输触发源
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_adc_dma_entry (void    *p_hw_adc,
                                   int     *p_adc_chan,
                                   int      chan_num,
                                   uint32_t vref_mv,
                                   int      dma_chan,
                                   uint8_t  dma_src);

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] chan      : 引脚对应的 ADC通道号
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_zsn700_hw_adc_int_entry (void    *p_hw_adc,
                                   int      int_num,
                                   uint8_t  chan,
                                   uint32_t vref_mv);

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 * \param[in] p_adc_chan: ADC通道数组指针
 * \param[in] sqr_num   : 规则通道个数
 * \param[in] jqr_num   : 插入通道个数
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_zsn700_hw_multichan_adc_entry (void    *p_hw_adc,
                                         int      int_num,
                                         uint32_t vref_mv,
                                         int     *p_adc_chan,
                                         uint8_t  sqr_num,
                                         uint8_t  jqr_num);

/**
 * \brief ADC 硬件层（DMA）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] p_adc_chan: ADC通道数组指针
 * \param[in] chan_num  : ADC通道个数
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 * \param[in] dma_chan  : DMA通道号
 * \param[in] dma_src   : DMA传输触发源
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_zsn700_hw_adc_dma_entry (void    *p_hw_adc,
                                   int     *p_adc_chan,
                                   int      chan_num,
                                   uint32_t vref_mv,
                                   int      dma_chan,
                                   uint8_t  dma_src);

/**
 * \brief stm32f103rbt6 ADC 硬件层（使用了中断）规则通道例程入口
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
void demo_stm32f103rbt6_hw_adc_int_entry (amhw_stm32f103rbt6_adc_t *p_hw_adc,
                                   int                int_num,
                                   int                chan,
                                   uint32_t           vref_mv);

/**
 * \brief stm32f103rbt6 DC 硬件层（DMA）规则通道例程入口
 */
void demo_stm32f103rbt6_hw_adc_dma_entry (amhw_stm32f103rbt6_adc_t *p_hw_adc,
                                   int               *p_adc_chan,
                                   int                adc_chan_num,
                                   uint8_t            dma_chan);


/**
 * \brief stm32f103rbt6 ADC 硬件层（使用了中断） 注入通道例程入口
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
void demo_stm32f103rbt6_hw_adc_injected_int_entry (void              *p_hw_adc,
                                            int                int_num,
                                            uint32_t           vref_mv,
                                            int               *p_adc_chan,
                                            int                adc_chan_num);

/**
 * \brief stm32f103rbt6 ADC 硬件层（使用了中断）规则通道、注入通道例程入口
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
void demo_stm32f103rbt6_hw_adc_dma_double_entry (void              *p_hw_adc1,
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
void demo_stm32f103rbt6_hw_clk_entry (am_clk_id_t *p_clk_id_buf, uint8_t buf_lenth);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * \param[in] dma_chan DMA 通道号
 *
 * \return 无
 */
void demo_stm32f103rbt6_drv_dma_m2m_entry (uint32_t dma_chan);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * \param[in] dma_chan DMA 通道号
 *
 * \return 无
 */
void demo_stm32f103rbt6118_drv_dma_m2m_entry (uint32_t dma_chan);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * \param[in] dma_chan DMA 通道号
 *
 * \return 无
 */
void demo_zsn700_drv_dma_m2m_entry (uint32_t dma_chan);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 *
 * \return 无
 */
void demo_stm32f103rbt6_drv_flash_entry (amhw_stm32f103rbt6_flash_t *p_hw_flash, uint8_t sector);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 *
 * \return 无
 */
void demo_stm32f103rbt6118_drv_flash_entry (void *p_hw_flash, uint16_t sector);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 *
 * \return 无
 */
void demo_zsn700_drv_flash_entry (void *p_hw_flash, uint16_t sector);

/**
 * \brief stm32f103rbt6 FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 *
 * \return 无
 */
void demo_stm32f103rbt6_drv_flash_entry (amhw_stm32f103rbt6_flash_t *p_hw_flash, uint8_t page_num);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio  指向 GPIO 外设寄存器块的指针
 * \param[in] input_pin  输入引脚号
 * \param[in] output_pin 输出引脚号
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_gpio_entry (amhw_stm32f103rbt6_gpio_t *p_hw_gpio,
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
void demo_stm32f103rbt6_hw_gpio_trigger_entry (amhw_stm32f103rbt6_gpio_t         *p_hw_gpio,
                                     amhw_stm32f103rbt6_syscfg_t       *p_hw_syscfg,
                                     amhw_stm32f103rbt6_exti_t         *p_hw_exti,
                                     int32_t                  pin,
                                     uint8_t                  portsource,
                                     uint8_t                  pinsource,
                                     amhw_stm32f103rbt6_line_num_t exti_line_num);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio  指向 GPIO外设寄存器块的指针
 * \param[in] input_pin  输入引脚号
 * \param[in] output_pin 输出引脚号
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_gpio_entry (void    *p_hw_gpio,
                                int32_t  input_pin,
                                int32_t  output_pin);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio  指向 GPIO外设寄存器块的指针
 * \param[in] input_pin  输入引脚号
 * \param[in] output_pin 输出引脚号
 *
 * \return 无
 */
void demo_zsn700_hw_gpio_entry (void    *p_hw_gpio,
                                int32_t  input_pin,
                                int32_t  output_pin);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio     指向 GPIO 外设寄存器块的指针
 * \param[in] pin           中断引脚号
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_gpio_trigger_entry (void *p_hw_gpio, int32_t pin);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio     指向 GPIO 外设寄存器块的指针
 * \param[in] pin           中断引脚号
 *
 * \return 无
 */
void demo_zsn700_hw_gpio_trigger_entry (void *p_hw_gpio, int32_t pin);

/**
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 * \param[in] clk_rate I2C 时钟源频率
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_i2c_master_poll_entry (amhw_stm32f103rbt6_i2c_t *p_hw_i2c,
                                        uint32_t        clk_rate);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_i2c_slave_poll_entry (amhw_stm32f103rbt6_i2c_t *p_hw_i2c);

/**
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 * \param[in] clk_rate I2C 时钟源频率
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_i2c_master_poll_entry (void        *p_hw_i2c,
                                           uint32_t     clk_rate);

/**
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 * \param[in] clk_rate I2C 时钟源频率
 *
 * \return 无
 */
void demo_zsn700_hw_i2c_master_poll_entry (void        *p_hw_i2c,
                                           uint32_t     clk_rate);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 * \param[in] inum_num I2C中断号
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_i2c_slave_int_entry (void  *p_hw_i2c,
                                         int    inum_num);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * \param[in] p_hw_i2c 指向 I2C 外设寄存器块的指针
 * \param[in] inum_num I2C中断号
 *
 * \return 无
 */
void demo_zsn700_hw_i2c_slave_int_entry (void  *p_hw_i2c,
                                         int    inum_num);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_spi 指向 SPI 外设寄存器块的指针
 * \param[in] cs_pin   片选引脚号
 * \param[in] clk_rate SPI 时钟源频率
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_spi_master_entry (amhw_stm32f103rbt6_spi_t *p_hw_spi,
                                   int32_t         cs_pin,
                                   uint32_t        clk_rate);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_spi 指向 SPI 外设寄存器块的指针
 * \param[in] cs_pin   片选引脚号
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_spi_master_entry (void    *p_hw_spi,
                                      int32_t  cs_pin);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_spi 指向 SPI 外设寄存器块的指针
 * \param[in] cs_pin   片选引脚号
 *
 * \return 无
 */
void demo_zsn700_hw_spi_master_entry (void    *p_hw_spi,
                                      int32_t  cs_pin);

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
void demo_stm32f103rbt6_hw_tim_cap_entry (amhw_stm32f103rbt6_tim_t      *p_hw_tim,
                                amhw_stm32f103rbt6_tim_type_t  type,
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
void demo_stm32f103rbt6_hw_tim_cmp_toggle_entry (amhw_stm32f103rbt6_tim_t      *p_hw_tim,
                                       amhw_stm32f103rbt6_tim_type_t  type,
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
void demo_stm32f103rbt6_hw_tim_pwm_entry (amhw_stm32f103rbt6_tim_t     *p_hw_tim,
                                amhw_stm32f103rbt6_tim_type_t type,
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
void demo_stm32f103rbt6_hw_tim_pwm_dead_entry (amhw_stm32f103rbt6_tim_t     *p_hw_tim,
                                     amhw_stm32f103rbt6_tim_type_t type,
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
void demo_stm32f103rbt6_hw_tim_timing_entry (amhw_stm32f103rbt6_tim_t     *p_hw_tim,
                                   amhw_stm32f103rbt6_tim_type_t type,
                                   uint32_t            clk_rate,
                                   int32_t             int_num);

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
void demo_stm32f103rbt6118_hw_tim_cap_entry (void     *p_hw_tim,
                                   uint8_t   type,
                                   uint32_t  chan,
                                   uint32_t  clk_rate,
                                   int32_t   inum);

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
void demo_zsn700_hw_tim_cap_entry (void     *p_hw_tim,
                                   uint8_t   type,
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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_tim_cmp_toggle_entry (void    *p_hw_tim,
                                          uint8_t  type,
                                          uint32_t chan,
                                          uint32_t clk_rate,
                                          uint8_t  clk_div,
                                          uint8_t  inum);

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
 * \return 无
 */
void demo_zsn700_hw_tim_cmp_toggle_entry (void    *p_hw_tim,
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
 * \param[in] duty_ns   脉宽时间（us）
 * \param[in] period_ns 周期时间（us）
 * \param[in] dead_us   死区时间设置（us）
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_tim_pwm_dead_entry (void    *p_hw_tim,
                                        uint8_t  type,
                                        uint8_t  chan_a,
                                        uint8_t  chan_b,
                                        uint32_t duty_us,
                                        uint32_t period_us,
                                        uint16_t dead_us);

/**
 * \brief 定时器通道带死区的互补PWM输出例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_tim  指向定时器外设寄存器块的指针
 * \param[in] type      定时器类型
 * \param[in] chan_a    定时器互补PWM通道A
 * \param[in] chan_b    定时器互补PWM通道B
 * \param[in] duty_ns   脉宽时间（us）
 * \param[in] period_ns 周期时间（us）
 * \param[in] dead_us   死区时间设置（us）
 *
 * \return 无
 */
void demo_zsn700_hw_tim_pwm_dead_entry (void    *p_hw_tim,
                                        uint8_t  type,
                                        uint8_t  chan_a,
                                        uint8_t  chan_b,
                                        uint32_t duty_us,
                                        uint32_t period_us,
                                        uint16_t dead_us);

/**
 * \brief 高级定时器软件软件同步开始、同步停止、同步清零例程，通过 HW层接口实现
 *
 * \param[in] p_tim_cfg 指向软件同步配置结构体的指针（amhw_stm32f103rbt6118_adtim_sw_sync_t）
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_adtim_swsync_entry (void *p_tim_cfg);

/**
 * \brief 高级定时器软件软件同步开始、同步停止、同步清零例程，通过 HW层接口实现
 *
 * \param[in] p_tim_cfg 指向软件同步配置结构体的指针（amhw_stm32f103rbt6118_adtim_sw_sync_t）
 *
 * \return 无
 */
void demo_zsn700_hw_adtim_swsync_entry (void *p_tim_cfg);

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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_adtim_port_sw_brake_entry (void          *p_hw_adtim,
                                               uint8_t        chan,
                                               unsigned long  duty_ns,
                                               unsigned long  period_ns,
                                               int            output_pin,
                                               uint8_t        src);

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
 * \return 无
 */
void demo_zsn700_hw_adtim_port_sw_brake_entry (void          *p_hw_adtim,
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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_adtim_vc_brake_entry (void         *p_hw_adtim,
                                          uint8_t       chan,
                                          unsigned long duty_ns,
                                          unsigned long period_ns,
                                          void         *p_hw_vc,
                                          uint8_t       vc0_n,
                                          uint8_t       vc0_p);

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
 * \return 无
 */
void demo_zsn700_hw_adtim_vc_brake_entry (void         *p_hw_adtim,
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
 * \param[in] period_ns   周期时间
 * \param[in] p_hw_adc    指向ADC外设寄存器块的指针
 * \param[in] adc_int_num ADC中断号
 * \param[in] p_adc_chan  ADC通道编号数组
 * \param[in] sqr_num     ADC顺序通道数量
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_adtim_trig_adc_sqr_entry (void    *p_hw_adtim,
                                              uint32_t period_ns,
                                              void    *p_hw_adc,
                                              int      adc_int_num,
                                              int     *p_adc_chan,
                                              uint8_t  sqr_num);

/**
 * \brief 高级定时器定时触发ADC转换例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adtim  指向定时器外设寄存器块的指针
 * \param[in] period_ns   周期时间
 * \param[in] p_hw_adc    指向ADC外设寄存器块的指针
 * \param[in] adc_int_num ADC中断号
 * \param[in] p_adc_chan  ADC通道编号数组
 * \param[in] sqr_num     ADC顺序通道数量
 *
 * \return 无
 */
void demo_zsn700_hw_adtim_trig_adc_sqr_entry (void    *p_hw_adtim,
                                              uint32_t period_ns,
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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_tim_port_brake_entry (void    *p_hw_tim,
                                          uint8_t  type,
                                          uint8_t  chan,
                                          int      inum,
                                          uint32_t duty_ns,
                                          uint32_t period_us);

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
 * \return 无
 */
void demo_zsn700_hw_tim_port_brake_entry (void    *p_hw_tim,
                                          uint8_t  type,
                                          uint8_t  chan,
                                          int      inum,
                                          uint32_t duty_ns,
                                          uint32_t period_us);

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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_tim_vc_brake_entry (void         *p_hw_tim,
                                        uint8_t       chan,
                                        unsigned long duty_ns,
                                        unsigned long period_ns,
                                        void         *p_hw_vc,
                                        uint8_t       vc0_n,
                                        uint8_t       vc0_p);

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
 * \return 无
 */
void demo_zsn700_hw_tim_vc_brake_entry (void         *p_hw_tim,
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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_tim_pwm_entry (void    *p_hw_tim,
                                   uint8_t  type,
                                   uint32_t chan,
                                   uint32_t clk_rate,
                                   uint8_t  clk_div,
                                   int32_t  inum);

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
 * \return 无
 */
void demo_zsn700_hw_tim_pwm_entry (void    *p_hw_tim,
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
 * \param[in] inum     中断号
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_tim_timing_entry (void    *p_hw_tim,
                                      uint8_t  type,
                                      uint32_t clk_rate,
                                      int32_t  int_num);

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
void demo_zsn700_hw_tim_timing_entry (void    *p_hw_tim,
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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_tim_trig_adc_sqr_entry (void    *p_hw_tim,
                                            uint8_t  type,
                                            uint8_t  chan,
                                            uint32_t duty_us,
                                            uint32_t period_us,
                                            void    *p_hw_adc,
                                            int      adc_int_num,
                                            int     *p_adc_chan,
                                            uint8_t  sqr_num);

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
 * \return 无
 */
void demo_zsn700_hw_tim_trig_adc_sqr_entry (void    *p_hw_tim,
                                            uint8_t  type,
                                            uint8_t  chan,
                                            uint32_t duty_us,
                                            uint32_t period_us,
                                            void    *p_hw_adc,
                                            int      adc_int_num,
                                            int     *p_adc_chan,
                                            uint8_t  sqr_num);

/**
 * \brief STM32F103RBT6 UART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] pfn_init  指向 UART 引脚初始化函数的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] uart_base UART 基地址
 * \param[in] inum_uart UART 中断号
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_uart_int_entry (amhw_stm32f103rbt6_usart_t *p_hw_uart,
                                 void (* pfn_init)(void),
                                 uint32_t         clk_rate,
                                 unsigned long    uart_base,
                                 unsigned char    inum_uart);

/**
 * \brief STM32F103RBT6 UART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_uart_polling_entry (amhw_stm32f103rbt6_usart_t *p_hw_uart,
                                     uint32_t         clk_rate);

/**
 * \brief STM32F103RBT6 UART 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_uart_rx_dma_entry (amhw_stm32f103rbt6_usart_t *p_hw_uart,
                                    uint32_t         clk_rate,
                                    int32_t          dma_chan);

/**
 * \brief STM32F103RBT6 UART 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_uart_tx_dma_entry (amhw_stm32f103rbt6_usart_t *p_hw_uart,
                                    uint32_t         clk_rate,
                                    int32_t          dma_chan);

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
void demo_stm32f103rbt6118_hw_uart_int_entry (void             *p_hw_uart,
                                    void (* pfn_init)(void),
                                    uint32_t         clk_rate,
                                    unsigned long    uart_base,
                                    unsigned char    inum_uart);

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
void demo_zsn700_hw_uart_int_entry (void             *p_hw_uart,
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
void demo_stm32f103rbt6118_hw_uart_polling_entry (void     *p_hw_uart,
                                        uint32_t  clk_rate);

/**
 * \brief UART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \return 无
 */
void demo_zsn700_hw_uart_polling_entry (void     *p_hw_uart,
                                        uint32_t  clk_rate);

/**
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_uart_rx_dma_entry (void    *p_hw_uart,
                                       uint32_t clk_rate,
                                       int32_t  dma_chan,
                                       int32_t  dma_src);

/**
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \return 无
 */
void demo_zsn700_hw_uart_rx_dma_entry (void    *p_hw_uart,
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
 * \return 无
 */
void demo_stm32f103rbt6118_hw_uart_tx_dma_entry (void    *p_hw_uart,
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
 * \return 无
 */
void demo_zsn700_hw_uart_tx_dma_entry (void    *p_hw_uart,
                                       uint32_t clk_rate,
                                       int32_t  dma_chan,
                                       int32_t  dma_src);

/**
 * \brief LPUART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向LPUART 外设寄存器块的指针
 * \param[in] pfn_init  指向 LPUART 引脚初始化函数的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 * \param[in] uart_base LPUART 基地址
 * \param[in] inum_uart LPUART 中断号
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_lpuart_int_entry (void             *p_hw_uart,
                                      void (* pfn_init)(void),
                                      uint8_t           sclk_src,
                                      uint32_t          clk_rate,
                                      uint32_t          baudrate,
                                      unsigned long     uart_base,
                                      unsigned char     inum_uart);

/**
 * \brief LPUART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向LPUART 外设寄存器块的指针
 * \param[in] pfn_init  指向 LPUART 引脚初始化函数的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 * \param[in] uart_base LPUART 基地址
 * \param[in] inum_uart LPUART 中断号
 *
 * \return 无
 */
void demo_zsn700_hw_lpuart_int_entry (void             *p_hw_uart,
                                      void (* pfn_init)(void),
                                      uint8_t           sclk_src,
                                      uint32_t          clk_rate,
                                      uint32_t          baudrate,
                                      unsigned long     uart_base,
                                      unsigned char     inum_uart);

/**
 * \brief LPUART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 LPUART 外设寄存器块的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_lpuart_polling_entry (void     *p_hw_uart,
                                          uint8_t   clk_src,
                                          uint32_t  clk_rate,
                                          uint32_t  baudrate);

/**
 * \brief LPUART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 LPUART 外设寄存器块的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 *
 * \return 无
 */
void demo_zsn700_hw_lpuart_polling_entry (void     *p_hw_uart,
                                          uint8_t   clk_src,
                                          uint32_t  clk_rate,
                                          uint32_t  baudrate);

/**
 * \brief LPUART DMA 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 LPUART 外设寄存器块的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_lpuart_rx_dma_entry (void    *p_hw_uart,
                                         uint8_t  clk_src,
                                         uint32_t clk_rate,
                                         uint32_t baudrate,
                                         int32_t  dma_chan,
                                         int32_t  dma_src);

/**
 * \brief LPUART DMA 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 LPUART 外设寄存器块的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \return 无
 */
void demo_zsn700_hw_lpuart_rx_dma_entry (void    *p_hw_uart,
                                         uint8_t  clk_src,
                                         uint32_t clk_rate,
                                         uint32_t baudrate,
                                         int32_t  dma_chan,
                                         int32_t  dma_src);

/**
 * \brief LPUART DMA 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 LPUART 外设寄存器块的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_lpuart_tx_dma_entry (void    *p_hw_uart,
                                         uint8_t  clk_src,
                                         uint32_t clk_rate,
                                         uint32_t baudrate,
                                         int32_t  dma_chan,
                                         int32_t  dma_src);

/**
 * \brief LPUART DMA 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 LPUART 外设寄存器块的指针
 * \param[in] sclk_src  LPUART 时钟源选择
 * \param[in] clk_rate  LPUART 时钟源频率
 * \param[in] baudrate  LPUART 波特率
 * \param[in] dma_chan  DMA 通道号
 * \param[in] dma_src   DMA 触发源选择
 *
 * \return 无
 */
void demo_zsn700_hw_lpuart_tx_dma_entry (void    *p_hw_uart,
                                         uint8_t  clk_src,
                                         uint32_t clk_rate,
                                         uint32_t baudrate,
                                         int32_t  dma_chan,
                                         int32_t  dma_src);

/**
 * \brief STM32F103RBT6 USART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] pfn_init  指向 USART 引脚初始化函数的指针
 * \param[in] clk_rate  USART 时钟源频率
 * \param[in] uart_base USART 基地址
 * \param[in] inum_uart USART 中断号
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_usart_int_entry (void                *p_hw_usart,
                                     void (* pfn_init)(void),
                                     uint32_t             clk_rate,
                                     unsigned long        usart_base,
                                     unsigned char        inum_usart);

/**
 * \brief STM32F103RBT6 USART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] clk_rate   USART 时钟源频率
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_usart_polling_entry (void                *p_hw_usart,
                                         uint32_t             clk_rate);

/**
 * \brief STM32F103RBT6 USART 接收例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] clk_rate   USART 时钟源频率
 * \param[in] dma_chan   DMA 通道号
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_usart_rx_dma_entry (void                *p_hw_usart,
                                        uint32_t             clk_rate,
                                        int32_t              dma_chan);

/**
 * \brief STM32F103RBT6 USART 发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_usart 指向 USART 外设寄存器块的指针
 * \param[in] clk_rate   USART 时钟源频率
 * \param[in] dma_chan   DMA 通道号
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_usart_tx_dma_entry (void                *p_hw_usart,
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
void demo_stm32f103rbt6116_drv_sleepmode_timer_wake_up_entry (am_timer_handle_t timer_handle,
                                                    uint32_t          timer_clk_rate);

/**
 * \brief 睡眠模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_stm32f103rbt6116_drv_sleepmode_wake_up_entry (void);

/**
 * \brief 待机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_stm32f103rbt6116_drv_standbymode_wake_up_entry (void);

/**
 * \brief 停机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_stm32f103rbt6116_drv_stopmode_wake_up_entry (void);

/**
 * \brief 睡眠模式例程，使用定时器周期唤醒，通过驱动层接口实现
 *
 * \param[in] timer_handle   定时器标准服务句柄
 * \param[in] timer_clk_rate 在系统时钟切换为 LSI 时，定时器的时钟频率
 *
 * \return 无
 */
void demo_zml165_drv_sleepmode_timer_wake_up_entry (am_timer_handle_t timer_handle,
                                                    uint32_t          timer_clk_rate);

/**
 * \brief 睡眠模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_zml165_drv_sleepmode_wake_up_entry (void);

/**
 * \brief 待机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_zml165_drv_standbymode_wake_up_entry (void);

/**
 * \brief 停机模式例程，通过驱动层接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_zml165_drv_stopmode_wake_up_entry (void);

/**
 * \brief IWDG 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_iwdg    指向 IWDG 外设寄存器块的指针
 * \param[in] time_out_ms  看门狗超时时间
 * \param[in] feed_time_ms 喂狗时间
 *
 * \return 无
 */
void demo_stm32f103rbt6_hw_iwdg_entry (amhw_stm32f103rbt6_iwdg_t *p_hw_iwdg,
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
void demo_stm32f103rbt6_hw_wwdg_entry (amhw_stm32f103rbt6_wwdg_t *p_hw_wwdg,
                             uint32_t         clk_rate,
                             uint32_t         time_out_ms,
                             uint32_t         feed_time_ms);

/**
 * \brief ZSN700 WDT 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_wdt     指向 WDT 外设寄存器块的指针
 * \param[in] time_out_ms  看门狗超时时间
 * \param[in] feed_time_ms 喂狗时间
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_wdt_entry (void     *p_hw_wdt,
                               uint32_t  time_out_ms,
                               uint32_t  feed_time_ms);

/**
 * \brief ZSN700 WDT 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_wdt     指向 WDT 外设寄存器块的指针
 * \param[in] time_out_ms  看门狗超时时间
 * \param[in] feed_time_ms 喂狗时间
 *
 * \return 无
 */
void demo_zsn700_hw_wdt_entry (void     *p_hw_wdt,
                               uint32_t  time_out_ms,
                               uint32_t  feed_time_ms);

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
void demo_zml166_adc_vol_measure_entry(void                   *p_handle,
                                       float                  *p_para,
                                       uint8_t                 gpa_index);

/**
 * \brief ZML166 ADC 热电偶测温例程
 * \param[in] handle      ZML166 ADC 服务句柄
 * \param[in] p_para      校准参数
 * \param[in] type        热电偶类型
 * \return 无
 */
void dome_zml166_adc_thermocouple_measure_entry(void     *p_handle,
                                                float    *p_para,
                                                void     *p_ther_handle);

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

/**
 * \brief ZML165 ADC 电压测量程序
 * \param[in] handle      ZML165 ADC 服务句柄
 * \return 无
 */
void demo_zml165_adc_vol_measure_entry(void  *p_handle);

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
void demo_stm32f103rbt6_can_int_entry (am_can_handle_t     can_handle,
                                am_can_bps_param_t *can_btr_baud,
                                am_can_int_type_t   int_type,
                                am_can_filter_t    *p_filterbuff,
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
void demo_stm32f103rbt6_can_entry (am_can_handle_t     can_handle,
                            am_can_bps_param_t *can_btr_baud);

/**
 * \brief USB打印机例程   该例程带打印时间以及打印数据量显示
 * \param[in] handle      USB打印机服务句柄
 * \return 无
 */
void demo_usbd_printer_counter_entry (void* p_handle);

/**
 * \brief USB打印机例程   该例程仅打印相关信息
 * \param[in] handle      USB打印机服务句柄
 * \return 无
 */
void demo_usbd_printer_entry (void* p_handle);

/**
 * \brief USB模拟串口例程
 * \param[in] handle      USB模拟串口服务句柄
 * \return 无
 */
void demo_usbd_vcom_entry (void* p_handle);

/**
 * \brief USB模拟按键例程
 * \param[in] handle      USB模拟键盘服务句柄
 * \return 无
 */
void demo_usbd_keyboard_entry (void* p_handle);


/**
 * \brief USB模拟U盘例程
 * \param[in] handle      USB模拟键盘服务句柄
 * \return 无
 */
void demo_usbd_msc_entry (void* p_handle);

/**
 * \brief DAC输出电压例程，hw接口层实现
 *
 * \param[in] p_hw_dac 指向DAC寄存器指针
 * \param[in] dma_src  DMA请求源
 * \param[in] dma_chan DMA通道
 * \param[in] vol_val  电压值
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_dac_dma_entry(void     *p_hw_dac,
                                  int32_t  dma_src,
                                  int32_t  dma_chan,
                                  uint16_t *vol_val);

/**
 * \brief DAC输出电压例程，hw接口层实现
 *
 * \param[in] p_hw_dac 指向DAC寄存器指针
 * \param[in] dma_src  DMA请求源
 * \param[in] dma_chan DMA通道
 * \param[in] vol_val  电压值
 *
 * \return 无
 */
void demo_zsn700_hw_dac_dma_entry(void     *p_hw_dac,
                                  int32_t  dma_src,
                                  int32_t  dma_chan,
                                  uint16_t *vol_val);

/**
 * \brief VC中断触发例程，hw接口层实现
 *
 * \param[in] p_hw_vc   指向VC寄存器指针
 * \param[in] vc_init_t vc信息初始化结构体
 * \param[in] pfn_cb_t  中断触发回调函数
 *
 */
void demo_stm32f103rbt6118_hw_vc_int_entry (void    *p_hw_vc,
                                  void   (*pfn_cb_t) (void *),
                                  void    *p_hw_dac,
                                  uint16_t vol_val);

/**
 * \brief VC中断触发例程，hw接口层实现
 *
 * \param[in] p_hw_vc   指向VC寄存器指针
 * \param[in] vc_init_t vc信息初始化结构体
 * \param[in] pfn_cb_t  中断触发回调函数
 *
 */
void demo_zsn700_hw_vc_int_entry (void    *p_hw_vc,
                                  void   (*pfn_cb_t) (void *),
                                  void    *p_hw_dac,
                                  uint16_t vol_val);

/**
 * \brief LVD系统复位例程，hw接口层实现
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器指针
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_lvd_system_reset_entry (void *p_hw_lvd);

/**
 * \brief LVD系统复位例程，hw接口层实现
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器指针
 *
 * \return 无
 */
void demo_zsn700_hw_lvd_system_reset_entry (void *p_hw_lvd);

/**
 * \brief LVD系统复位例程，hw接口层实现
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器指针
 * \param[in] p_lvd_cb : 用户回调函数
 *
 * \return 无
 */
void demo_stm32f103rbt6118_hw_lvd_int_entry (void *p_hw_lvd,
                                   void (*p_lvd_cb)(void *));

/**
 * \brief LVD系统复位例程，hw接口层实现
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器指针
 * \param[in] p_lvd_cb : 用户回调函数
 *
 * \return 无
 */
void demo_zsn700_hw_lvd_int_entry (void *p_hw_lvd,
                                   void (*p_lvd_cb)(void *));

/**
 * \brief OPA 电压跟随例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa :指向OPA寄存器指针
 * \param[in] mode     :OPA工作模式
 *
 * \retval :无
 */
void demo_stm32f103rbt6118_hw_opa_one_entry (void *p_hw_opa, uint8_t mode);

/**
 * \brief OPA 电压跟随例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa :指向OPA寄存器指针
 * \param[in] mode     :OPA工作模式
 *
 * \retval :无
 */
void demo_zsn700_hw_opa_one_entry (void *p_hw_opa, uint8_t mode);

/**
 * \brief OPA DAC电压跟随例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa :指向OPA寄存器指针
 * \param[in] p_hw_dac :指向DAC寄存器指针
 * \param[in] mode     :OPA工作模式
 * \param[in] vol_val  :DAC设置电压值
 *
 * \retval :无
 */
void demo_stm32f103rbt6118_hw_opa_dac_entry (void     *p_hw_opa,
                                   void     *p_hw_dac,
                                   uint8_t   mode,
                                   uint16_t *vol_val);

/**
 * \brief OPA DAC电压跟随例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa :指向OPA寄存器指针
 * \param[in] p_hw_dac :指向DAC寄存器指针
 * \param[in] mode     :OPA工作模式
 * \param[in] vol_val  :DAC设置电压值
 *
 * \retval :无
 */
void demo_zsn700_hw_opa_dac_entry (void     *p_hw_opa,
                                   void     *p_hw_dac,
                                   uint8_t   mode,
                                   uint16_t *vol_val);

/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_aes   :指向AES寄存器指针
 * \param[in] key_len    :密钥长度
 * \param[in] p_aes_data :AES加密明文
 * \param[in] p_key_data  :AES密钥
 *
 * \retval :无
 */
void demo_stm32f103rbt6118_hw_aes_ency_decy_entry (void     *p_hw_aes,
                                         uint8_t   key_len,
                                         uint32_t *p_aes_data,
                                         uint32_t *p_key_data);

/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_aes   :指向AES寄存器指针
 * \param[in] key_len    :密钥长度
 * \param[in] p_aes_data :AES加密明文
 * \param[in] p_key_data  :AES密钥
 *
 * \retval :无
 */
void demo_zsn700_hw_aes_ency_decy_entry (void     *p_hw_aes,
                                         uint8_t   key_len,
                                         uint32_t *p_aes_data,
                                         uint32_t *p_key_data);

/**
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_trng   :指向TRNG寄存器指针
 *
 * \retval :无
 */
void demo_stm32f103rbt6118_hw_trng_generate_entry (void *p_hw_trng);

/**
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_trng   :指向TRNG寄存器指针
 *
 * \retval :无
 */
void demo_zsn700_hw_trng_generate_entry (void *p_hw_trng);

/**
 * \brief LCD 显示例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_lcd   :指向lcd寄存器指针
 * \param[in] p_data     :LCD显示数据
 *
 * \retval :无
 */
void demo_stm32f103rbt6118_hw_lcd_entry (void                    *p_hw_lcd,
                               uint32_t                *p_data);

/**
 * \brief LCD 显示例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_lcd   :指向lcd寄存器指针
 * \param[in] p_data     :LCD显示数据
 *
 * \retval :无
 */
void demo_zsn700_hw_lcd_entry (void                    *p_hw_lcd,
                               uint32_t                *p_data);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_STM32F103RBT6_ENTRIES_H */

/* end of file */
