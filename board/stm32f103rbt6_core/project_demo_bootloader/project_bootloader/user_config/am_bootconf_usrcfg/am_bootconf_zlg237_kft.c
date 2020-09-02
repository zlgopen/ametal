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
 * \brief ZLG237 kboot KinetisFlashTool 用户配置文件
 *
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */


/**
 * \addtogroup am_if_src_bootconf_zlg237_kft
 * \copydoc am_bootconf_zlg237_kft.c
 * @{
 */

#include "am_arm_boot.h"
#include "am_boot_kft.h"
#include "ametal.h"
#include "am_gpio.h"
#include "am_zlg237.h"
#include "am_zlg237_inst_init.h"
#include "am_boot_serial_uart.h"
#include "am_zlg237_boot_flash.h"
#include "am_boot.h"
#include "am_boot_flash.h"
#include "am_zlg_tim_cap.h"
#include "am_zlg_flash.h"
#include "zlg237_regbase.h"
#include "am_int.h"
#include "am_zlg237_clk.h"
#include "amhw_zlg237_rcc.h"
#include "am_arm_nvic.h"


/*******************************************************************************
 * flash配置
 ******************************************************************************/
/**
 *  \brief bootloader flash 设备信息
 */
static am_zlg237_boot_flash_devinfo_t __g_flash_devinfo = {

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
    ZLG237_FLASH_BASE,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台初解始化函数 */
    NULL,
};

/**
 *  \brief bootloader flash 设备实例
 */
static am_zlg237_boot_flash_dev_t __g_flash_dev;

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
am_boot_flash_handle_t am_zlg237_boot_kft_flash_inst_init(void)
{
    return am_zlg237_boot_flash_init(&__g_flash_dev, &__g_flash_devinfo);
}

/*******************************************************************************
 * 标准bootloader接口配置
 ******************************************************************************/

static void __zlg237_boot_kft_plfm_deinit()
{
    volatile uint32_t i = 1000000;
    /* 串口解初始化 ，解初始化前需要小段的延时，保证寄存器任务已经完成*/
    while(i--){

    }
    am_clk_disable(CLK_USART1);
    am_gpio_pin_cfg(PIOA_9, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_10,AM_GPIO_INPUT);

    amhw_zlg237_usart_disable((amhw_zlg237_usart_t *)ZLG237_USART1_BASE);
    am_int_disable(INUM_USART1);

    /* 定时器解初始化  */
    amhw_zlg_tim_int_disable(ZLG237_TIM4, AMHW_ZLG_TIM_UIE);
    amhw_zlg_tim_disable(ZLG237_TIM4);
    am_int_disable(INUM_TIM4);
    am_zlg237_clk_reset(CLK_TIM4);
    am_clk_disable(CLK_TIM4);

    ZLG237_RCC->ahbenr = 0;
    ZLG237_RCC->apb1enr = 0;
    ZLG237_RCC->apb2enr = 0;

    amhw_zlg237_rcc_sys_clk_set(AMHW_ZLG237_SYSCLK_HSE);

    am_arm_nvic_deinit();
    __disable_irq();
}

/**
 *  \brief bootloader标准设备信息
 */
static am_arm_boot_devinfo_t __g_zlg237_boot_devinfo = {
    /**< \brief 应用代码区起始地址*/
    0x0800AC00,
    /** \brief ram起始地址 */
    0x20000000,
    /** \brief ram结束地址 */
    20 * 1024,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    __zlg237_boot_kft_plfm_deinit,
};

/**
 *  \brief bootloader标准设备实例
 */
int am_zlg237_boot_kft_inst_init()
{
    return am_arm_boot_init(&__g_zlg237_boot_devinfo);
}

/** @} */

/* end of file */
