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
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 使对应的中断引脚上产生下降沿。
 *
 * - 实验现象：
 *   1. 中断产生时调试串口输出 "the gpio interrupt happen!"。
 *
 * \par 源代码
 * \snippet demo_mm32_hw_gpio_trigger.c src_mm32_hw_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32_hw_gpio_trigger
 * \copydoc demo_mm32_hw_gpio_trigger.c
 */

/** [src_mm32_hw_gpio_trigger] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "hw/amhw_mm32_exti.h"
#include "hw/amhw_mm32_gpio.h"
#include "hw/amhw_mm32_syscfg.h"

/**
 * \brief 引脚中断服务函数
 */
static void __gpio_isr (void *p_arg)
{
    AM_DBG_INFO("the gpio interrupt happen!\r\n");
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_gpio_trigger_entry (amhw_mm32_gpio_t         *p_hw_gpio,
                                     amhw_mm32_syscfg_t       *p_hw_syscfg,
                                     amhw_mm32_exti_t         *p_hw_exti,
                                     int32_t                  pin,
                                     uint8_t                  portsource,
                                     uint8_t                  pinsource,
                                     amhw_mm32_exti_line_num_t exti_line_num)
{

    /* 取消引脚复用功能 */
    amhw_mm32_gpio_pin_afr_set(p_hw_gpio, AMHW_MM32_GPIO_AF_DEFAULT, pin);

    /* 设置引脚方向为输入及设置引脚的模式 */
    amhw_mm32_gpio_pin_set(p_hw_gpio, AMHW_MM32_GPIO_MODE_IPU << 2 | 0x00, pin);

    /* 使能引脚上拉电阻 */
    amhw_mm32_gpio_pin_out_high(p_hw_gpio, pin);

    /*
     * 中断线 0 只能连一个端口编号为 0 的 GPIO 引脚，比如说，PIOA_0 连接到了中断线 0，
     * PIOB_0、PIOC_0、PIOD_0 就不能连到中断线 0, 但此时 PIOA_1 可以连接到中断线 1。
     * 对应关系: PIOA_8 对应于外部中断的引脚源为 AMHW_MM32_EXTI_PINSOURCE8
     */
    amhw_mm32_syscfg_exti_line_config(p_hw_syscfg, portsource, pinsource);

    /* 清除中断线配置 */
    amhw_mm32_exti_imr_clear(p_hw_exti, exti_line_num);
    amhw_mm32_exti_emr_clear(p_hw_exti, exti_line_num);

    /* 清除中断线触发方式配置 */
    amhw_mm32_exti_rtsr_clear(p_hw_exti, exti_line_num);
    amhw_mm32_exti_ftsr_clear(p_hw_exti, exti_line_num);

    /* 下降沿触发 */
    amhw_mm32_exti_ftsr_set(p_hw_exti, exti_line_num);

    /* 清引脚中断线标志位 */
    amhw_mm32_exti_pending_clear(p_hw_exti, exti_line_num);

    /* 连接用户注册的中断回调函数 */
    am_gpio_trigger_connect(pin, __gpio_isr, NULL);

    /* 设置开放中断线请求 */
    amhw_mm32_exti_imr_set(p_hw_exti,  exti_line_num);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_mm32_hw_gpio_trigger] */

/* end of file */
