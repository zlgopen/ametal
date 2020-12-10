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
 * \brief LVD系统中断例程，hw接口层实现
 *
 * \param[in] p_hw_lvd   : 指向LVD寄存器指针
 * \param[in] lvd_vids   : LVD 阈值电压，amhw_hc32_lvd_vids_t
 * \param[in] lvd_src    : LVD 监测来源选择，宏定义 AMHW_HC32_LVD_SRC_
 * \param[in] pfn_lvd_cb : 用户回调函数
 * \param[in] p_arg      : 用户回调函数参数
 *
 * \retval 无
 */
void demo_hc32f07x_hw_lvd_int_entry (amhw_hc32_lvd_t        *p_hw_base,
                                     amhw_hc32_lvd_vids_t    lvd_vids,
                                     uint32_t                lvd_src,
                                     am_pfnvoid_t            pfn_lvd_cb,
                                     void                   *p_arg);

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

/**
 * \brief USB模拟U盘例程
 * \param[in] handle      USB模拟键盘服务句柄
 * \return 无
 */
void demo_usbd_msc_entry (void* p_handle);	

/**
 * \brief USB模拟按键例程
 * \param[in] handle      USB模拟键盘服务句柄
 * \return 无
 */
void demo_usbd_keyboard_entry (void* p_handle);

/**
 * \brief USB打印机例程   该例程仅打印相关信息
 * \param[in] handle      USB打印机服务句柄
 * \return 无
 */
void demo_usbd_printer_entry (void* p_handle);

/**
 * \brief USB打印机例程   该例程带打印时间以及打印数据量显示
 * \param[in] handle      USB打印机服务句柄
 * \return 无
 */
void demo_usbd_printer_counter_entry (void* p_handle);

/**
 * \brief USB模拟串口例程
 * \param[in] handle      USB模拟串口服务句柄
 * \return 无
 */
void demo_usbd_vcom_entry (void* p_handle);

/**
 * \brief 例程入口
 */
void demo_hc32f07x_drv_flash_write_protect_entry(void *p_flash, uint16_t sector);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_HC32_ENTRIES_H */

/* end of file */
