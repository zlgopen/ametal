/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief am824zb 所有例程函数入口声明，本文只包含zigbee相关demo，
 *  其他demo请参照am824_core中的demo
 * \sa demo_am824zb_all_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation.
 * \endinternal
 */

/**
 * \brief ZM516X 模块自动波特率例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_zm516x_baudrate_entry(void);

/**
 * \brief ZM516X 模块 GPIO 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_zm516x_gpio_entry (void);

/**
 * \brief ZM516X 模块 KEY LED 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_zm516x_key_led_entry (void);

/**
 * \brief ZM516X 模块组网例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_zm516x_network_entry (void);

/**
 * \brief ZM516X 模块搜索例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_zm516x_search_entry(void);

/**
 * \brief ZM516X 模块演示例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_zm516x_entry (void);

/**
 * \brief ADC DMA 演示例程，通过HW接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_hw_adc_dma_entry (void);

/**
 * \brief ADC thcmp 演示例程，通过HW接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_hw_adc_thcmp_entry (void);

/**
 * \brief ADC DMA 中断演示例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_adc_dma_int_entry (void);

/**
 * \brief ADC DMA 轮询演示例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_adc_dma_poll_entry (void);

/**
 * \brief  ADC 中断演示例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_adc_int_entry (void);

/**
 * \brief  ADC 热敏电阻演示例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_adc_ntc_entry (void);

/**
 * \brief  ADC DMA 轮询演示例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am824zb_std_adc_poll_entry (void);

/* end of file */
