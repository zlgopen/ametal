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
 * \brief HC32 kboot KinetisFlashTool 用户配置文件
 *
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup am_if_src_bootconf_hc32_kft
 * \copydoc am_bootconf_hc32_kft.c
 * @{
 */

#include <am_bootconf_usrcfg/am_bootconf_hc32f03x.h>
#include "am_hc32.h"
#include "am_hc32f03x_inst_init.h"
#include "am_hc32_boot_flash.h"
#include "am_arm_boot.h"
#include "am_boot_flash.h"
#include "am_boot_mem_flash.h"
#include "am_hc32_flash.h"
#include "hc32_regbase.h"
#include "am_boot_firmware_flash.h"
#include "am_int.h"
#include "am_hc32_clk.h"
#include "hw/amhw_hc32_rcc.h"
#include "am_arm_nvic.h"


/*******************************************************************************
 * flash配置
 ******************************************************************************/
/**
 * < \brief bootloader flash 设备信息
 */
static am_hc32_boot_flash_devinfo_t __g_flash_devinfo = {

    {
        /** \brief flash的起始地址 */
        0x00000000,
        /** \brief flash的总的大小 */
        64 * 1024,
        /** \brief flash扇区大小 */
        128,
        /** \brief flash页大小 */
        512,
    },

    /** \brief flash寄存器的基地址 */
    HC32_FLASH_BASE,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台初解始化函数 */
    NULL,
};

/**
 * < \brief bootloader flash 设备实例
 */
static am_hc32_boot_flash_dev_t __g_flash_dev;

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
static am_boot_flash_handle_t am_hc32_boot_single_flash_inst_init(void)
{
    return am_hc32_boot_flash_init(&__g_flash_dev, &__g_flash_devinfo);
}

/*******************************************************************************
 * 固件以flash的方式存储配置
 ******************************************************************************/
static am_boot_firmware_flash_devinfo_t __g_firmware_flash_devinfo = {
    /** \brief 固件在内存中存放的起始地址 */
    0x00006800,
    /**< \brief 应用代码区的总大小 */
    38 * 1024,
    /** \brief 驱动存放固件缓冲区大小 ,值为内存写入最小大小的整数倍 */
    4,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    NULL,
};

static am_boot_firmware_flash_dev_t __g_firmware_flash_dev;

am_boot_firmware_handle_t am_hc32_boot_single_firmware_flash_inst_init(void)
{
    am_boot_flash_handle_t   flash_handle;
    flash_handle = am_hc32_boot_single_flash_inst_init();
    return am_boot_firmware_flash_init (&__g_firmware_flash_dev,
                                        &__g_firmware_flash_devinfo,
                                         flash_handle);
}

/*******************************************************************************
 * 标准bootloader接口配置
 ******************************************************************************/

/* 在整个bootloader中用到的资源都要接初始化并释放，常见的如定时器，串口等 */
static void __hc32_boot_single_plfm_deinit()
{
    volatile uint32_t i = 1000000;
    /* 串口解初始化 ，解初始化前需要小段的延时，保证寄存器任务已经完成*/
    while(i--){
    }

    am_hc32_clk_inst_deinit();

    am_int_disable(INUM_UART0_2);
    am_clk_disable(CLK_UART0);

    am_hc32_clk_reset(CLK_UART0);
    am_hc32_clk_reset(CLK_GPIO);

    am_int_disable(INUM_TIM2);
    am_clk_disable(CLK_TIM012);
    am_hc32_clk_reset(CLK_TIM012);

    HC32_RCCCTRL->peri_clken[0] = 0x80800000;
    HC32_RCCCTRL->peri_clken[1] = 0;

    am_arm_nvic_deinit();

    __disable_irq();
}
/**
 * < \brief bootloader标准设备信息
 */
static am_arm_boot_devinfo_t __g_hc32_boot_single_devinfo = {
    /**< \brief 应用代码区起始地址*/
    0x00006800,
    /** \brief ram起始地址 */
    0x20000000,
    /** \brief ram大小 */
    8 * 1024,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    __hc32_boot_single_plfm_deinit,
};

int am_hc32_boot_single_inst_init()
{
    return am_arm_boot_init(&__g_hc32_boot_single_devinfo);
}


/** @} */

/* end of file */
