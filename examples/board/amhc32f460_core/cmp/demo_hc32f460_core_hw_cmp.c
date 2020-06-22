/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/
/**
 * \file
 * \brief CMP例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *    1.J25通过跳冒连接，断开J24跳冒并将J24-VRO与J6-29连接，J8-52与示波器连接；
 *    2.打开工程编译并全速运行；
 *    3.调节可调电阻R138；
 *    4.观察LED1(绿色，D26)与示波器：
 *       a.J8-52电平为高时，LED1点亮，表示INP电压高于INM；
 *       b.J8-52电平为低时，LED1熄灭，表示INP电压低于INM；
 *
 * \note
 *    更改CMP单元时，需重新配置对应的引脚、INP、INM等
 *    
 * \par 源代码
 * \snippet demo_hc32f460_core_hw_cmp_entry.c
 *          src_demo_hc32f460_core_hw_cmp_entry
 *
 * \internal
 * \par Modification history
 * - 1.00 
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_core_hw_cmp_entry
 * \copydoc demo_hc32f460_core_hw_cmp_entry.c
 */

/** [src_hc32f460_core_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_clk.h"
#include "hw/amhw_hc32f460_i2c.h"
#include "demo_hc32f460_entries.h"
#include "demo_hc32f460_core_entries.h"
#include "hc32f460_regbase.h"


/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_cmp_entry (void)
{
    am_kprintf("demo hc32f460_core hw cmp!\r\n");

    /* Set PA4 as CMP2_INP1 input */
    am_gpio_pin_cfg(PIOA_4, GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) | GPIO_AFIO(AMHW_HC32F460_AFIO_USBF));
    
    /* Set PB13 as Vcout output */
    am_gpio_pin_cfg(PIOB_13, GPIO_AFIO(AMHW_HC32F460_AFIO_VCOUT));

    /* Enable peripheral clock */
    am_clk_enable(CLK_CMP);
    am_clk_enable(CLK_DAC);

    demo_hc32f460_hw_cmp_entry(HC32F460_CMP2);
}
/** [src_hc32f460_core_hw_i2c_slave_poll] */

/* end of file */
