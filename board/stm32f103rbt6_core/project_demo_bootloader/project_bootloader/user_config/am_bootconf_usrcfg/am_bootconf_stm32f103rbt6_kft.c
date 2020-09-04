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
 * \brief STM32F103RBT6 kboot KinetisFlashTool 用户配置文件
 *
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */


/**
 * \addtogroup am_if_src_bootconf_stm32f103rbt6_kft
 * \copydoc am_bootconf_stm32f103rbt6_kft.c
 * @{
 */

#include "am_arm_boot.h"
#include "am_boot_kft.h"
#include "ametal.h"
#include "am_gpio.h"
#include "am_stm32f103rbt6.h"
#include "am_stm32f103rbt6_inst_init.h"
#include "am_boot_serial_uart.h"
#include "am_stm32f103rbt6_boot_flash.h"
#include "am_boot.h"
#include "am_boot_flash.h"
#include "am_stm32f103rbt6_tim_cap.h"
#include "am_stm32f103rbt6_flash.h"
#include "stm32f103rbt6_regbase.h"
#include "am_int.h"
#include "am_stm32f103rbt6_clk.h"
#include "amhw_stm32f103rbt6_rcc.h"
#include "am_arm_nvic.h"


/*******************************************************************************
 * flash配置
 ******************************************************************************/
/**
 *  \brief bootloader flash 设备信息
 */
static am_stm32f103rbt6_boot_flash_devinfo_t __g_flash_devinfo = {

    {
        /** \brief flash的起始地址 */
        0x08000000,
        /** \brief flash的总的大小 */
        128 * 1024,
        /** \brief flash扇区大小 */
        1024 * 4,
        /** \brief flash页大小 */
        1024,
    },
    /** \brief flash寄存器的基地址 */
    STM32F103RBT6_FLASH_BASE,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台初解始化函数 */
    NULL,
};

/**
 *  \brief bootloader flash 设备实例
 */
static am_stm32f103rbt6_boot_flash_dev_t __g_flash_dev;

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
am_boot_flash_handle_t am_stm32f103rbt6_boot_kft_flash_inst_init(void)
{
    return am_stm32f103rbt6_boot_flash_init(&__g_flash_dev, &__g_flash_devinfo);
}

/*******************************************************************************
 * 标准bootloader接口配置
 ******************************************************************************/

static void __stm32f103rbt6_boot_kft_plfm_deinit()
{
    volatile uint32_t i = 1000000;
    /* 串口解初始化 ，解初始化前需要小段的延时，保证寄存器任务已经完成*/
    while(i--){

    }
    am_clk_disable(CLK_USART1);
    am_gpio_pin_cfg(PIOA_9, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_10,AM_GPIO_INPUT);

    amhw_stm32f103rbt6_usart_disable((amhw_stm32f103rbt6_usart_t *)STM32F103RBT6_USART1_BASE);
    am_int_disable(INUM_USART1);

    /* 定时器解初始化  */
    amhw_stm32f103rbt6_tim_int_disable(STM32F103RBT6_TIM4, AMHW_STM32F103RBT6_TIM_UIE);
    amhw_stm32f103rbt6_tim_disable(STM32F103RBT6_TIM4);
    am_int_disable(INUM_TIM4);
    am_stm32f103rbt6_clk_reset(CLK_TIM4);
    am_clk_disable(CLK_TIM4);

    STM32F103RBT6_RCC->ahbenr = 0;
    STM32F103RBT6_RCC->apb1enr = 0;
    STM32F103RBT6_RCC->apb2enr = 0;

    amhw_stm32f103rbt6_rcc_sys_clk_set(AMHW_STM32F103RBT6_SYSCLK_HSE);

    am_arm_nvic_deinit();
    __disable_irq();
}

/**
 *  \brief bootloader标准设备信息
 */
static am_arm_boot_devinfo_t __g_stm32f103rbt6_boot_devinfo = {
    /**< \brief 应用代码区起始地址*/
    0x0800AC00,
    /** \brief ram起始地址 */
    0x20000000,
    /** \brief ram结束地址 */
    20 * 1024,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    __stm32f103rbt6_boot_kft_plfm_deinit,
};

/**
 *  \brief bootloader标准设备实例
 */
int am_stm32f103rbt6_boot_kft_inst_init()
{
    return am_arm_boot_init(&__g_stm32f103rbt6_boot_devinfo);
}

/** @} */

/* end of file */
