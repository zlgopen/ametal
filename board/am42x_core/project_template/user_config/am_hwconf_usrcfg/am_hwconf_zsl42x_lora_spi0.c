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
 * \brief ZSL42X LORA SPI0 用户配置文件
 * \sa am_hwconf_zsl42x_lora_spi0.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-02-15  win, first implementation.
 * - 1.01 20-09-23  yg,  adaptation zsl42x.
 * \endinternal
 */

#include "am_gpio.h"
#include "am_clk.h"
#include "am_types.h"

#include "am_zsl42x_lora.h"
#include "am_zsl42x_lora_reg.h"
#include "am_zsl42x_lora_if_spi.h"
#include "am_hwconf_zsl42x_lora_spi0.h"
#include "am_hwconf_zsl42x_lpsoftimer.h"

#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_spi.h"

/** \brief 使能中断类型 */
#define __ZSL42X_LORA_IRQ_MASK  (AM_ZSL42X_LORA_IRQ_MASK_TX_DONE       | \
                                 AM_ZSL42X_LORA_IRQ_MASK_RX_DONE       | \
                                 AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID  | \
                                 AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR       | \
                                 AM_ZSL42X_LORA_IRQ_MASK_CAD_DONE      | \
                                 AM_ZSL42X_LORA_IRQ_MASK_CAD_DET       | \
                                 AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT)

/** \brief DIO1中断类型 */
#define __ZSL42X_LORA_DIO1_MASK   (__ZSL42X_LORA_IRQ_MASK)

/** \brief DIO2中断类型 */
#define __ZSL42X_LORA_DIO2_MASK   (0)

/** \brief DIO3中断类型 */
#define __ZSL42X_LORA_DIO3_MASK   (0)

/**
 * \addtogroup am_if_src_hwconf_zsl42x_lora_spi0
 * \copydoc am_hwconf_zsl42x_lora_spi0.c
 * @{
 */

/**
 * \brief ZSL42X LORA SPI0 平台初始化
 */
am_local void __zsl42x_lora_if_spi0_plfm_init (void)
{
    am_clk_enable(CLK_SPI0);

    am_gpio_pin_cfg(PIOA_15, PIOA_15_SPI0_CS  | PIOA_15_OUT_PP);
    am_gpio_pin_cfg(PIOA_5,  PIOA_5_SPI0_SCK  | PIOA_5_OUT_PP);
    am_gpio_pin_cfg(PIOA_6,  PIOA_6_SPI0_MISO | PIOA_6_INPUT_PU);
    am_gpio_pin_cfg(PIOA_7,  PIOA_7_SPI0_MOSI | PIOA_7_OUT_PP);
}

/**
 * \brief 解除 ZSL42X LORA SPI0 平台初始化
 */
am_local void __zsl42x_lora_if_spi0_plfm_deinit (void)
{
    am_gpio_pin_cfg(PIOA_15, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_5,  AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_6,  AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_7,  AM_GPIO_INPUT);

    am_clk_disable(CLK_SPI0);
}

//PIOA_15,    /**< \brief 片选引脚 */
//PIOC_10,    /**< \brief 复位引脚 */
//PIOC_11,    /**< \brief BUSY信号引脚 */
//
//PIOC_12,    /**< \brief DIO1引脚 */
//PIOF_6,     /**< \brief DIO2引脚 */
//PIOF_7,     /**< \brief DIO3引脚 */
//
//PIOF_5,     /**< \brief txen引脚 */
//PIOF_4,     /**< \brief rxen引脚 */

/**
 * \brief zsl42x lora 平台初始化
 */
am_local void __zsl42x_lora_plfm_init (void *p_arg)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_arg;

    /* 引脚初始化 */
    am_gpio_pin_cfg(PIOA_15, AM_GPIO_OUTPUT_INIT_HIGH);         // CS
    am_gpio_pin_cfg(PIOC_10, PIOC_10_GPIO | PIOC_10_OUT_PP);    // RST
    am_gpio_pin_cfg(PIOC_11, PIOC_11_GPIO | PIOC_11_INPUT_PD);  // BUSY
    am_gpio_pin_cfg(PIOC_12, PIOC_12_GPIO | PIOC_12_INPUT_PD);  // DIO1
    am_gpio_pin_cfg(PIOF_4,  PIOF_4_GPIO  | PIOF_4_OUT_PP);     // RX_EN
    am_gpio_pin_cfg(PIOF_5,  PIOF_5_GPIO  | PIOF_5_OUT_PP);     // TX_EN
    am_gpio_pin_cfg(PIOF_6,  PIOF_6_GPIO  | PIOF_6_INPUT_PU);   // DIO2
    am_gpio_pin_cfg(PIOF_7,  PIOF_7_GPIO  | PIOF_7_INPUT_PU);   // DIO3

    /* 复位和片选引脚输出高电平 */
    am_gpio_set(PIOC_10, 1);
    am_gpio_set(PIOA_15, 1);

    /* 上升沿触发 */
    am_gpio_trigger_cfg(p_dev->p_devinfo->dio1_pin, AM_GPIO_TRIGGER_RISE);

    /* 连接DIO1的中断函数 */
    am_gpio_trigger_connect(p_dev->p_devinfo->dio1_pin,
                            p_dev->p_dio_irq,
                            &p_dev->lora_dev);
}

/**
 * \brief zsl42x lora 解平台初始化
 */
am_local void __zsl42x_lora_plfm_deinit (void *p_arg)
{
    (void)p_arg;

    am_gpio_pin_cfg(PIOA_15, PIOA_15_GPIO | PIOA_15_INPUT_PU);
    am_gpio_pin_cfg(PIOC_10, PIOC_10_GPIO | PIOC_10_INPUT_PU);
    am_gpio_pin_cfg(PIOC_11, PIOC_11_GPIO | PIOC_11_INPUT_PU);
    am_gpio_pin_cfg(PIOC_12, PIOC_12_GPIO | PIOC_12_INPUT_PU);
    am_gpio_pin_cfg(PIOF_4,  PIOF_4_GPIO  | PIOF_4_INPUT_PU);
    am_gpio_pin_cfg(PIOF_5,  PIOF_5_GPIO  | PIOF_5_INPUT_PU);
    am_gpio_pin_cfg(PIOF_6,  PIOF_6_GPIO  | PIOF_6_INPUT_PU);
    am_gpio_pin_cfg(PIOF_7,  PIOF_7_GPIO  | PIOF_7_INPUT_PU);
}

/**
 * \brief zsl42x lora 适配中断使能
 */
am_local int __zsl42x_lora_dio_irq_enable (int pin)
{
    return am_gpio_trigger_on(pin);
}

/**
 * \brief zsl42x lora 适配中断禁能
 */
am_local int __zsl42x_lora_dio_irq_disable (int pin)
{
    return am_gpio_trigger_off(pin);
}

/**
 * \brief LORA SPI0 设备信息
 */
am_const struct am_zsl42x_lora_if_spi_devinfo __g_zsl42x_lora_spi0_devinfo = {

    {
        AM_ZSL42X_LORA_CORE_TYPE_SX1268, /**< \brief 芯片器件编号 */

        PIOA_15,                         /**< \brief 片选引脚 */
        PIOC_10,                         /**< \brief 复位引脚 */
        PIOC_11,                         /**< \brief BUSY信号引脚 */

        PIOC_12,                         /**< \brief DIO1引脚 */
        PIOF_6,                          /**< \brief DIO2引脚 */
        PIOF_7,                          /**< \brief DIO3引脚 */

        PIOF_5,                          /**< \brief txen引脚 */
        PIOF_4,                          /**< \brief rxen引脚 */

        32000000,                        /**< \brief ZSL42X_LORA使用的晶振频率 */

        /**
         * \brief 按照470 ~ 510频段校正
         *        校准过程将产生4ms左右的等待过程，并产生较大的电流峰值
         */
        AM_ZSL42X_LORA_CALIBRATE_IMAGE_470MHz_510MHz,

        __ZSL42X_LORA_IRQ_MASK,
        __ZSL42X_LORA_DIO1_MASK,
        __ZSL42X_LORA_DIO2_MASK,
        __ZSL42X_LORA_DIO3_MASK,

        __zsl42x_lora_plfm_init,         /**< \brief 相关引脚初始化函数 */
        __zsl42x_lora_plfm_deinit,       /**< \brief 相关引脚去初始化函数 */

        am_gpio_set,                     /**< \brief 引脚电平设置功能函数指针 */
        am_gpio_get,                     /**< \brief 引脚电平获取功能函数指针 */

        __zsl42x_lora_dio_irq_enable,    /**< \brief 引脚中断使能 */
        __zsl42x_lora_dio_irq_disable    /**< \brief 引脚中断禁能 */
    },

    ZSL42x_SPI0_BASE,                      /**< \brief SPI0 寄存器指针   */

    __zsl42x_lora_if_spi0_plfm_init,     /**< \brief ZSL42X LORA SPI0 平台初始化函数 */
    __zsl42x_lora_if_spi0_plfm_deinit    /**< \brief ZSL42X LORA SPI0 平台解初始化函数 */
};

/**
 * \brief ZSL42X LORA SPI0 设备实例
 */
am_local am_zsl42x_lora_if_spi_dev_t __g_zsl42x_lora_spi0_dev;

/**
 * \brief ZSL42X LORA SPI0 实例初始化
 */
am_zsl42x_lora_handle_t am_zsl42x_lora_spi0_inst_init (void)
{
    am_zsl42x_lpsoftimer_init();
    return am_zsl42x_lora_if_spi_init(&__g_zsl42x_lora_spi0_dev,
                                      &__g_zsl42x_lora_spi0_devinfo);
}

/**
 * \brief ZSL42X LORA SPI0 实例解初始化
 */
void am_zsl42x_lora_spi0_inst_deinit (am_zsl42x_lora_handle_t handle)
{
    am_zsl42x_lora_if_spi_deinit(handle);
}

/**
 * @}
 */

/* end of file */
