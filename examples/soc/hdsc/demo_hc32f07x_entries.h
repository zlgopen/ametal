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

#ifndef __DEMO_HC32F07X_ENTRIES_H
#define __DEMO_HC32F07X_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"
#include "am_can.h"

#include "hw/amhw_hc32_adc.h"
#include "hw/amhw_hc32f07x_flash.h"
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
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * \param[in] p_hw_flash 指向 FLASH 外设寄存器块的指针
 * \param[in] sector     使用 FLASH 扇区号
 *
 * \retval 无
 */
void demo_hc32f07x_drv_flash_entry (void *p_hw_flash, uint16_t sector);


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
void demo_hc32f07x_hw_dac_dma_entry(void     *p_hw_dac,
                                int32_t   dma_src,
                                int32_t   dma_chan,
                                uint16_t *p_vol_val);

/**
 * \brief OPA 通用运算放大器模式，通过 HW 层接口实现
 *
 * \param[in] p_hw_opa :指向OPA寄存器指针
 * \param[in] mode     :OPA工作模式
 *
 * \retval 无
 */
void demo_hc32f07x_hw_opa_one_entry (void *p_hw_opa, uint8_t mode);

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
void demo_hc32f07x_hw_opa_dac_entry (void      *p_hw_opa,
                                 void      *p_hw_dac,
                                 uint8_t    mode,
                                 uint16_t  *p_vol_val,
                                 uint32_t   number);


#ifdef __cplusplus
}
#endif

#endif /* __DEMO_HC32_ENTRIES_H */

/* end of file */
