/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief GPIO 硬件操作接口
 * 
 * 1. 可单独配置每个引脚的方向和输出的电平，包括置位，清零，反转；
 * 2. 可同时操作每个端口所有引脚的方向和输出的电平，包括置位，清零，反转；
 * 3. 可单独获取引脚电平或同时获取端口所有引脚电平；
 * 4. 可通过掩码设置和获取端口引脚电平。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-24  mem, modified.
 * - 1.00 15-07-03  zxl, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_GPIO_H
#define __AMHW_LPC82X_GPIO_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \addtogroup amhw_lpc82x_if_gpio 
 * \copydoc amhw_lpc82x_gpio.h
 * @{
 */
    
/**
 * \brief GPIO 寄存器块结构体
 */
typedef struct amhw_lpc82x_gpio {
    __IO uint8_t  b[128][32];       /**< \brief 引脚字节寄存器       */
    __IO uint32_t w[32][32];        /**< \brief 引脚字寄存器         */
    __IO uint32_t dir[32];          /**< \brief 方向寄存器           */
    __IO uint32_t mask[32];         /**< \brief 掩码寄存             */
    __IO uint32_t pin[32];          /**< \brief 端口引脚寄存器       */
    __IO uint32_t mpin[32];         /**< \brief 端口掩码寄存器       */
    __IO uint32_t set[32];          /**< \brief 输出的读或置位寄存器 */
    __O  uint32_t clr[32];          /**< \brief 端口清除寄存器       */
    __O  uint32_t not0[32];         /**< \brief 端口翻转寄存器       */
    __O  uint32_t dirset[32];       /**< \brief 端口方向置位寄存器   */
    __O  uint32_t dirclr[32];       /**< \brief 端口方向清零寄存器   */
    __O  uint32_t dirnot[32];       /**< \brief 端口方向翻转寄存器   */
} amhw_lpc82x_gpio_t;

/**
 * \name GPIO 引脚电平
 * \anchor grp_amhw_lpc82x_gpio_pin_level
 * @{
 */
 
#define AMHW_LPC82X_GPIO_LEVEL_LOW     0        /**< \brief 低电平 */
#define AMHW_LPC82X_GPIO_LEVEL_HIGH    1        /**< \brief 高电平 */

/** @} */

/**
 * \name GPIO 引脚方向
 * \anchor grp_amhw_lpc82x_gpio_pin_dir
 * @{
 */
 
#define AMHW_LPC82X_GPIO_DIR_INPUT     0        /**< \brief 方向为输入 */
#define AMHW_LPC82X_GPIO_DIR_OUTPUT    1        /**< \brief 方向为输出 */

/** @} */


/**
 * \brief 设置GPIO引脚的输出电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] level     : 电平状态
 *      \arg AMHW_LPC82X_GPIO_LEVEL_LOW  ： 低电平
 *      \arg AMHW_LPC82X_GPIO_LEVEL_HIGH ： 高电平
 *
 * \return 无
 *
 * \note 该功能仅限于已设置为输出的GPIO引脚
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_out_set (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                   int                 pin, 
                                   uint8_t             level)
{
    p_hw_gpio->b[0][pin] = level;
}

/**
 * \brief 设置GPIO引脚输出高电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 该功能仅限于已设置为输出的GPIO引脚
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_out_high (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->set[0] = AM_BIT(pin);
}

/**
 * \brief 设置GPIO引脚输出低电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 该功能仅限于已设置为输出的GPIO引脚
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_out_low (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->clr[0] = AM_BIT(pin);
}

/**
 * \brief 翻转GPIO引脚输出电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 该功能仅限于已设置为输出的GPIO引脚
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_out_tog (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->not0[0] = AM_BIT(pin);
}

/**
 * \brief 获取指定引脚的电平状态
 *
。
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \retval AMHW_LPC82X_GPIO_LEVEL_LOW  : 低电平
 * \retval AMHW_LPC82X_GPIO_LEVEL_HIGH : 高电平
 *
 * \note 除了该引脚配置为模拟I/O时，读总为0，其他条件下都可以获取引脚的电平状态
 */
am_static_inline 
int amhw_lpc82x_gpio_pin_level_get (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    return AM_BIT_GET(p_hw_gpio->pin[0], pin);
}

/**
 * \brief 设置GPIO引脚方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] dir       : 引脚方向
 *      \arg AMHW_LPC82X_GPIO_DIR_INPUT  ： 方向为输入
 *      \arg AMHW_LPC82X_GPIO_DIR_OUTPUT ： 方向为输出
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_dir_set (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                   int                 pin, 
                                   uint8_t             dir)
{
    uint32_t reg = p_hw_gpio->dir[0];
    
    reg &= ~AM_BIT(pin);
    reg |=  AM_SBF(dir, pin);
    p_hw_gpio->dir[0] = reg;
}


/**
 * \brief 设置GPIO引脚方向为输出
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_dir_output (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->dirset[0] = AM_BIT(pin);
}

/**
 * \brief 设置GPIO引脚方向为输入
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_dir_input (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->dirclr[0] = AM_BIT(pin);
}

/**
 * \brief 反转GPIO引脚方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_pin_dir_tog (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->dirnot[0] = AM_BIT(pin);
}

/**
 * \brief 获取引脚的方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \retval AMHW_LPC82X_GPIO_DIR_INPUT  : 方向为输入
 * \retval AMHW_LPC82X_GPIO_DIR_OUTPUT : 方向为输出
 */
am_static_inline
uint8_t amhw_lpc82x_gpio_pin_dir_get (amhw_lpc82x_gpio_t *p_hw_gpio, int pin)
{
    return AM_BIT_GET(p_hw_gpio->dir[0], pin);
}

/**
 * \brief 设置端口引脚的状态
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] value     : 引脚状态，引脚n对应bit_n,如bit_n为1,则置引脚电平为高，
 *                        反之置引脚为低电平
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_out_set (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                    uint32_t            value)
{
    p_hw_gpio->pin[0] = value;
}

/**
 * \brief 设置端口GPIO引脚输出高电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin_mask  : 引脚n对应bit_n,如bit_n为1,则置引脚电平为高，
 *                        如bit_n为0，不操作该引脚
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_out_high (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                     uint32_t            pin_mask)
{
    p_hw_gpio->set[0] = pin_mask;
}

/**
 * \brief 设置端口GPIO引脚输出低电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin_mask  : 引脚n对应bit_n,如bit_n为1,则置引脚电平为低，
 *                        如bit_n为0，不操作该引脚
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_out_low (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                    uint32_t            pin_mask)
{
    p_hw_gpio->clr[0] = pin_mask;
}

/**
 * \brief 反转端口GPIO引脚输出电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin_mask  : 引脚n对应bit_n,如bit_n为1,则反转引脚电平，
 *                        如bit_n为0，不操作该引脚
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_out_tog (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                    uint32_t            pin_mask)
{
    p_hw_gpio->not0[0] = pin_mask;
}

/**
 * \brief 获取端口GPIO引脚电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 端口的引脚状态，引脚n对应bit_n,如bit_n为1,则引脚n电平为高，
 *         反之引脚n为低电平
 *
 * \note 除了该引脚配置为模拟I/O时，读总为0，其他条件下都可以获取引脚的电平状态
 */
am_static_inline 
uint32_t amhw_lpc82x_gpio_port_levle_get (amhw_lpc82x_gpio_t *p_hw_gpio)
{
    return p_hw_gpio->pin[0];
}

/**
 * \brief 设置端口GPIO引脚的方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] value     : 引脚方向，引脚n对应bit_n,如bit_n为1,
 *                        则设置引脚方向为输出，反之置引脚方向为输入
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_dir_set (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                    uint32_t            value)
{
    p_hw_gpio->dir[0] = value;
}

/**
 * \brief 设置端口GPIO引脚方向为输出
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin_mask  : 引脚n对应bit_n,如bit_n为1,则设置引脚方向为输出，
 *                        如bit_n为0，不操作该引脚
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_dir_output (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                       uint32_t            pin_mask)
{
    p_hw_gpio->dirset[0] = pin_mask;
}

/**
 * \brief 设置端口GPIO引脚输出低电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin_mask  : 引脚n对应bit_n,如bit_n为1,则设置引脚方向为输入，
 *                        如bit_n为0，不操作该引脚
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_dir_input (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                      uint32_t            pin_mask)
{
    p_hw_gpio->dirclr[0] = pin_mask;
}

/**
 * \brief 反转端口GPIO引脚方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin_mask  : 引脚n对应bit_n,如bit_n为1,则反转引脚方向，
 *                        如bit_n为0，不操作该引脚
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_dir_tog (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                    uint32_t            pin_mask)
{
    p_hw_gpio->dirnot[0] = pin_mask;
}

/**
 * \brief 获取端口GPIO引脚方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 端口的引脚方向，引脚n对应bit_n,如bit_n为1,则引脚n方向为输出，
 *         反之引脚n方向为输入
 */
am_static_inline 
uint32_t amhw_lpc82x_gpio_port_dir_get (amhw_lpc82x_gpio_t *p_hw_gpio)
{
    return p_hw_gpio->dir[0];
}

/**
 * \brief 设置端口的GPIO引脚掩码，用于控制端口的读和写
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] mask      : 端口掩码值，引脚n对应bit_n
 * 
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_mask_set (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                uint32_t            mask)
{
    p_hw_gpio->mask[0] = mask;
}

/**
 * \brief 获取端口GPIO引脚掩码
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针 
 *
 * \return 端口的GPIO引脚掩码，引脚n对应bit_n
 */
am_static_inline 
uint32_t amhw_lpc82x_gpio_mask_get (amhw_lpc82x_gpio_t *p_hw_gpio)
{
    return p_hw_gpio->mask[0];
}

/**
 * \brief 根据掩码设置GPIO端口的引脚状态
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] value     : 引脚状态，引脚n对应bit_n，如bit_n为1，则置引脚
 *                        电平为高，反之置引脚为低电平，如果对应的MASK值为1，
 *                        该输出为将不受影响
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_gpio_port_masked_out_set (amhw_lpc82x_gpio_t *p_hw_gpio,  
                                           uint32_t            value)
{
    p_hw_gpio->mpin[0] = value;
}

/**
 * \brief 根据掩码获取端口GPIO引脚状态
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 掩码后的GPIO端口引脚状态
 *
 * \note 如果某一位对应的MASK寄存器值为0，该寄存器正常读取该引脚电平状态，
 *       对应的MASK寄存器值为1时，读总为0
 */
am_static_inline 
uint32_t amhw_lpc82x_gpio_port_masked_level_get (amhw_lpc82x_gpio_t *p_hw_gpio)
{
    return p_hw_gpio->mpin[0];
}


/**
 * @} amhw_lpc82x_if_gpio
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_GPIO_H */

/* end of file */
