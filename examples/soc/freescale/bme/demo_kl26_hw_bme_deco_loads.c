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
 * \brief  BME封装式存储指令实现例子
 *
 * - 操作：
 *   1.PIOA_1 引脚连接PC串口的TXD;
 *   2.PIOA_2 引脚连接PC串口的RXD;
 *   3.配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 * -实现现象：串口依次打印出以下字符串，用于C语言实现与BME指令执行效率上的差别，
 *         可以看出BME指令实现的执行效率更高效，程序最后点亮led灯

 *         systick end value: 0xff9d63
 *
 *         systick current value read overhead: 0x7
 *
 *         extracted data with las1 normal C is 1
 *
 *         systick start value: 0xff9d5c
 *
 *         systick end value: 0xff9d30
 *
 *         actual execution cycle for las1 operation with normal C code: 0x25
 *
 *         extracted data with las1 BME macro is 1
 *
 *         systick start value: 0xff9d6a
 *
 *         systick end value: 0xff9d58
 *
 *         actual execution cycle for las1 operation with BME macros: 0xb
 *
 *         extracted data with ubfx normal C is 17
 *
 *         systick start value: 0xff9d56
 *
 *         systick end value: 0xff9d2d
 *
 *         actual execution cycle for ubfx operation with normal C: 0x22
 *
 *         extracted data with ubfx BME macro is 17
 *
 *         systick start value: 0xff9d6a
 *
 *         systick end value: 0xff9d59
 *
 *         actual execution cycle for ubfx operation with BME macro: 0xa
 *
 * \note BME的 读回写1清零命令AMHW_KL26_BME_LAS1_*不在使用在有许多的w1c清零位的寄存器中，如外设的状态寄存器.
 *
 * \par 源代码
 * \snippet demo_kl26_hw_bme_deco_loads.c src_kl26_hw_bme_deco_loads
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-18  nwt, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_kl26_if_hw_bme_deco_loads
 * \copydoc demo_kl26_hw_bme_deco_loads.c
 */
 
/** [src_kl26_hw_bme_deco_loads] */

#include "hw/amhw_fsl_bme.h"
#include "ametal.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "hw/amhw_kl26_gpio.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "hw/amhw_arm_systick.h"
#include "am_board.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define  GPIO_PODR_SHIFT    4      /** <\brief GPIO数据输出寄存器偏移定义  */

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
 * \brief 计算用BME LAS1指令设置GPIO时的滴答时钟计数值
 */
static void __gpio_las1_op_with_bme_macros(void)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* 实际执行操作所需的时间周期 */
    uint32_t execution_cycle;

    uint32_t rdata;

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    rdata =  AMHW_KL26_BME_LAS1_W(&KL26_GPIO->gpio[2].pdor, 4);

    cnt_end_value   = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("extracted data with las1 BME macro is %d\n\r", rdata);
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for las1 operation with BME macros: 0x%x\n\r", execution_cycle);
#endif
}

/**
 * \brief 计算用C语言实现 LAS1指令来实现设置GPIO时的滴答时钟计数值
 */
static void __gpio_las1_op_with_normalc(uint32_t volatile *p_addr, uint8_t bitpos)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* 实际执行操作所需的时间周期 */
    uint32_t execution_cycle;

    uint32_t mask;

    uint8_t load_bit;


    cnt_start_value = AMHW_ARM_SYSTICK->val;

    /* C语言实现读回置1操作  */
    mask= 1UL << bitpos;
    load_bit = (*p_addr & mask)>>bitpos;
    *p_addr |= mask;;

    cnt_end_value   = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("extracted data with las1 normal C is %d\n\r", load_bit);
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for las1 operation with normal C code: 0x%x\n\r", execution_cycle);
#endif

}

/**
 * \brief 计算用BME UBFX指令来读回GPIO管脚数据输出寄存器的某些位段的内容滴答时钟计数值
 */
static void __gpio_ubfx_op_with_bme_macros(void)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* 实际执行操作所需的时间周期 */
    uint32_t execution_cycle;

    uint32_t rdata;

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    rdata = AMHW_KL26_BME_UBFX_W(&KL26_GPIO_ALIASED->gpio[2].pdor,
                                 GPIO_PODR_SHIFT, /* 从第几个位开始读回. */
                                 5);              /* 读回位的长度.  */

    cnt_end_value = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("extracted data with ubfx BME macro is %d\n\r", rdata);
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for ubfx operation with BME macro: 0x%x\n\r", execution_cycle);
#endif

}


/**
 * \brief 计算用C语言实现BME UBFX指令来读回GPIO管脚数据输出寄存器的某些位段的内容滴答时钟计数值
 */
static void __gpio_ubfx_op_with_normalc(volatile uint32_t  *p_addr,
                                        uint32_t *p_rdata,
                                        uint8_t bitpos,
                                        uint8_t fieldwidth)
{
    uint32_t cnt_start_value;
    uint32_t cnt_end_value;

    /* actual execution cycle */
    uint32_t execution_cycle;

    uint32_t reg_val;
    uint32_t mask;

    cnt_start_value = AMHW_ARM_SYSTICK->val;

    reg_val = *p_addr;

    /* generate bit mask */
    mask  = ((1UL << (fieldwidth)) - 1) << bitpos;

    /* read data returned to core */
    *p_rdata = (reg_val &  mask) >> bitpos;

    cnt_end_value = AMHW_ARM_SYSTICK->val;

    execution_cycle = cnt_start_value - cnt_end_value - __overhead;

#ifdef AM_VDEBUG
    am_kprintf("extracted data with ubfx normal C is %d\n\r", *p_rdata);
    am_kprintf("systick start value: 0x%x\n\r", cnt_start_value);
    am_kprintf("systick end value: 0x%x\n\r", cnt_end_value);
    am_kprintf("actual execution cycle for ubfx operation with normal C: 0x%x\n\r", execution_cycle);
#endif
}

/**
 * \brief 例程入口
 */
void demo_kl26_hw_bme_deco_loads_entry (void)
{

    uint32_t rdata = 0;

    /* 计算无任何中间代码时滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数  */
    __cal_systick_read_overhead();                  /* 两次普通计数 */
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数  */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用C语言实现读回置1操作设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */

    /* C语言读回置1操作 */
    __gpio_las1_op_with_normalc(&KL26_GPIO->gpio[2].pdor, 4);

    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用BME LAS1指令设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */
    __gpio_las1_op_with_bme_macros();               /* BME指令LAS1操作 */
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用C语言实现UBFX操作设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */

    /* C语言实现UBFX操作 */
    __gpio_ubfx_op_with_normalc(&KL26_GPIO->gpio[2].pdor,
                                &rdata,
                                GPIO_PODR_SHIFT,
                                5);

    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 计算使用BME UBFX指令设置时的滴答时钟计数值 */
    amhw_arm_systick_enable(AMHW_ARM_SYSTICK);      /* 使能向下计数   */
    __gpio_ubfx_op_with_bme_macros();               /* BME指令UBFX操作 */
    amhw_arm_systick_disable(AMHW_ARM_SYSTICK);     /* 禁能向下计数   */
    amhw_arm_systick_val_set(AMHW_ARM_SYSTICK, 0);  /* 清零计数值   */

    /* 使用BME LAC1指令读回清零指定位点亮LED */
    rdata =  AMHW_KL26_BME_LAC1_W(&KL26_GPIO->gpio[2].pdor, 8);
    rdata =  AMHW_KL26_BME_LAC1_W(&KL26_GPIO->gpio[2].pdor, 4);

    while (1) {
        ;
    }
}

/** [src_kl26_hw_bme_deco_loads] */
 
/* end of file */
