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
 * \brief BME封装式存储指令实现例子
 *
 * - 操作：
 *   1.PIOA_1 引脚连接PC串口的TXD;
 *   2.PIOA_2 引脚连接PC串口的RXD;
 *   3.配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 * -实现现象：串口依次打印出以下字符串，用于C语言实现与BME指令执行效率上的差别，
 *         可以看出BME指令实现的执行效率更高效
 *
 *         systick start value: 0xff9d6a
 *
 *         systick end value: 0xff9d63
 *
 *         systick current value read overhead: 0x7
 *
 *         systick start value: 0xff9cb3
 *
 *         systick end value: 0xff9c9b
 *
 *         actual execution cycle for xor operation with normal C code: 0x11
 *
 *         systick start value: 0xff9cb0
 *
 *         systick end value: 0xff9c9f
 *
 *         actual execution cycle for xor operation with BME macros: 0xa
 *
 *         systick start value: 0xff9d57
 *
 *         systick end value: 0xff9d22
 *
 *         actual execution cycle for BFI operation with normal C: 0x2e
 *
 *         systick start value: 0xff9d19
 *
 *         systick end value: 0xff9d03
 *
 *         actual execution cycle for BFI operation with BME macro: 0xf
 *
 * \note BME 指令操作GPIO模块的，应该使用KL26_GPIO_BASE的别名地址为 0x4000_F000
 *
 *
 * \par 源代码
 * \snippet demo_kl26_hw_bme_deco_stores.c src_kl26_hw_bme_deco_stores
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-18  nwt, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_kl26_if_hw_bme_deco_stores
 * \copydoc demo_kl26_hw_bme_deco_stores.c
 */
 
/** [src_kl26_hw_bme_deco_stores] */
#include "ametal.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "hw/amhw_kl26_gpio.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "hw/amhw_arm_systick.h"
#include "hw/amhw_fsl_bme.h"
#include "am_board.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define  GPIO_PORT_MUX_SHIFT    8      /** <\brief GPIO管脚复用宏定义  */

/** \brief GPIO模块别名地址(仅限BME的BFI与UBFX指令使用) */
#define KL26_GPIO_ALIASED  ((amhw_kl26_gpio_t    *)0x4000F000UL)

/*******************************************************************************
  全局变量
*******************************************************************************/
static volatile uint32_t __overhead = 0;     /** <\brief 用于计算滴答时钟计数  */


/**
 * \brief 计算无任何中间代码时滴答时钟计数值
 */
static void __cal_systick_read_overhead(void)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    cnt_end_value   = AMHW_ARM_SYSTICK->val;

    __overhead = cnt_start_value - cnt_end_value;

#ifdef AM_VDEBUG
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("systick current value read overhead: 0x%x\n\r", __overhead);
#endif
}

/**
 * \brief 计算用BME XOR指令设置GPIO时的滴答时钟计数值
 */
static void __gpio_xor_op_with_bme_macros(void)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* 实际执行操作所需的时间周期 */
    uint32_t execution_cycle;


    /* 设置 PORTC4号管脚为GPIO功能 */
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_4, 0x1);

    /* 设置GPIOC 4号管脚为输出方向 */
    amhw_kl26_gpio_pin_dir_output(KL26_GPIO, PIOC_4);

    /* 设置GPIOC 4号管脚初始电平 */
    amhw_kl26_gpio_pin_init_out_low(KL26_GPIO, PIOC_4);

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    AMHW_KL26_BME_XOR_W(&KL26_GPIO->gpio[2].pdor, 0x10);

    cnt_end_value   = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for xor operation with BME macros: 0x%x\n\r", execution_cycle);
#endif
}

/**
 * \brief 计算用C语言xor代码设置GPIO时的滴答时钟计数值
 */
static void __gpio_xor_op_with_normalc(void)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* 实际执行操作所需的时间周期 */
    uint32_t execution_cycle;


    /* 设置 PORTC4号管脚为GPIO功能 */
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_4, 0x1);

    /* 设置GPIOC4号管脚为输出方向 */
    amhw_kl26_gpio_pin_dir_output(KL26_GPIO, PIOC_4);

    /* 设置GPIOC4号管脚初始电平 */
    amhw_kl26_gpio_pin_init_out_low(KL26_GPIO, PIOC_4);

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    KL26_GPIO->gpio[2].pdor ^= 0x10;

    cnt_end_value   = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for xor operation with normal C code: 0x%x\n\r", execution_cycle);
#endif
}

/**
 * \brief 计算用BME bfi指令设置管脚复用功能为GPIO时的滴答时钟计数值
 */
static void __gpio_bfi_op_with_bme_macros(void)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* 实际执行操作所需的时间周期 */
    uint32_t execution_cycle;

    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_4, 0x0);

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    /* 数据的位置与位偏移的位置对齐 */
    AMHW_KL26_BME_BFI_W(&KL26_PORT->port[2].port_pcr[4], /* 外设地址为PTC4管脚控制寄存器 */
                        1 << GPIO_PORT_MUX_SHIFT,             /* 数据 */
                        GPIO_PORT_MUX_SHIFT,                  /* 从第几个位开始替换 */
                        3);                                   /* 被替换位的长度 */

    cnt_end_value = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for BFI operation with BME macro: 0x%x\n\r", execution_cycle);
#endif

}

/**
 * \brief 计算用用C语言来实现BFI指令设置管脚复用功能为GPIO时的滴答时钟计数值
 */
static void __gpio_bfi_op_with_normalc(volatile uint32_t  *p_addr,
                                       uint32_t wdata,
                                       uint8_t bitpos,
                                       uint8_t fieldwidth)
{
    uint32_t reg_val;
    uint32_t mask;

    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* 实际执行操作所需的时间周期 */
    uint32_t execution_cycle;

    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_4, 0x0);

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    /* C语言实现BFI指令的相同功能 */
    reg_val = *p_addr;
    mask    = ((1 << (fieldwidth)) - 1) << bitpos;
    reg_val = (reg_val & ~mask) | ((wdata) & mask);
    *p_addr = reg_val;

    cnt_end_value = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for BFI operation with normal C: 0x%x\n\r", execution_cycle);
#endif
}


/**
 * \brief 例程入口
 */
void demo_kl26_hw_bme_deco_stores_entry (void)
{
    /* 计算无任何中间代码时滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数  */
    __cal_systick_read_overhead();                  /* 两次普通计数 */
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数  */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用C语言实现XOR操作设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */
    __gpio_xor_op_with_normalc();                   /* C语言XOR操作 */
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用BME XOR指令设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */
    __gpio_xor_op_with_bme_macros();                /* BME指令XOR操作 */
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用C语言实现BFI操作设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */

    /* C语言实现BFI操作 */
    __gpio_bfi_op_with_normalc(&KL26_PORT->port[2].port_pcr[4],
                               1 << GPIO_PORT_MUX_SHIFT,
                               GPIO_PORT_MUX_SHIFT,
                               3);
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用BME BFI指令设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */
    __gpio_bfi_op_with_bme_macros();                /* BME指令BFI操作 */
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    while (1) {
        ;
    }
}

/** [src_kl26_hw_bme_deco_stores] */
 
/* end of file */
