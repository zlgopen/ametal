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
 * \brief GPIO 硬件操作接口
 *
 * 1. 理论上，GPIO模块功能运行速率可达24M，
 *    FGPIO模块功能运行速率可达48M
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-15  nwt, make some changes about function name.
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */

#ifndef __KL26_GPIO_H
#define __KL26_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"
#include "../../../../kl26/kl26_periph_map.h"
#include "../../../../kl26/kl26_inum.h"

/**
 * \addtogroup amhw_kl26_if_gpio
 * \copydoc amhw_kl26_gpio.h
 * @{
 */

/**
 * \brief GPIO 端口寄存器结构体
 */
typedef struct gpio_reg {
    __IO uint32_t pdor;         /**< \brief 端口数据输出寄存器 */
    __O  uint32_t psor;         /**< \brief 端口设置输出寄存器 */
    __O  uint32_t pcor;         /**< \brief 端口清零输出寄存器 */
    __O  uint32_t ptor;         /**< \brief 端口翻转输出寄存器 */
    __I  uint32_t pdir;         /**< \brief 端口数据输入寄存器 */
    __IO uint32_t pddr;         /**< \brief 端口数据方向寄存器 */
    __IO uint32_t reserve[10];  /**< \brief 保留 */
} gpio_reg_t;

/**
 * \brief GPIO 寄存器块结构体
 */
typedef struct amhw_kl26_gpio {
    gpio_reg_t    gpio[5];      /**< \brief GPIO 端口 */
} amhw_kl26_gpio_t;

/**
 * \brief GPIO 寄存器块结构体
 */
typedef struct amhw_kl26_fgpio {
    gpio_reg_t    fgpio[5];     /**< \brief FGPIO 端口 */
} amhw_kl26_fgpio_t;

/**
 * \brief 设置GPIO引脚初始电平为低
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_gpio_pin_init_out_low (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 5].pdor &= ~(1ul << (pin & 0x1F));
}

/**
 * \brief 设置GPIO引脚初始电平为高
 *
 * \param[in] p_hw_gpio :指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_gpio_pin_init_out_high (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 5].pdor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 读GPIO引脚初始电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 低电平
 * \retval 1 : 高电平
 */
am_static_inline
int amhw_kl26_gpio_pin_init_get (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    return (((p_hw_gpio->gpio[pin >> 5].pdor &
            (1ul << (pin & 0x1F))) !=0) ? 1 : 0);
}

/**
 * \brief 设置GPIO引脚电平为高
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \note  向PSOR寄存器写1，写PDOR寄存器，使管脚输出电平为高
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_gpio_pin_out_high (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 5].psor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 设置GPIO引脚电平为低
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \note   向PCOR寄存器写1，清PDOR寄存器，使管脚输出电平为低
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_gpio_pin_out_low (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 5].pcor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 翻转GPIO引脚电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \note   向PTOR寄存器写1，翻转PDOR寄存器的值，使管脚输出电平翻转
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_gpio_pin_out_toggle (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 5].ptor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 读GPIO引脚输入电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 低电平
 * \retval 1 : 高电平
 */
am_static_inline
int amhw_kl26_gpio_pin_input_get (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    return ((((p_hw_gpio->gpio[pin >> 5].pdir) &
            (1ul << (pin & 0x1F))) !=0) ? 1 : 0);
}

/**
 * \brief 设置GPIO引脚为输入方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_gpio_pin_dir_input (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 5].pddr &= ~(1ul << (pin & 0x1F));
}

/**
 * \brief 设置GPIO引脚为输出方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_gpio_pin_dir_output (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 5].pddr |= (1ul << (pin & 0x1F));
}

/**
 * \brief 读GPIO引脚输入\输出方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 输入
 * \retval 1 : 输出
 */
am_static_inline
int amhw_kl26_gpio_pin_dir_get (amhw_kl26_gpio_t *p_hw_gpio, int pin)
{
    return (((p_hw_gpio->gpio[pin >> 5].pddr &
            (1ul << (pin & 0x1F))) != 0) ? 1 : 0);
}


/*******************************************************************************
                    FGPIO驱动函数
********************************************************************************/

/**
 * \brief 设置FGPIO引脚初始电平为低
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_fgpio_pin_init_out_low (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    p_hw_fgpio->fgpio[pin >> 5].pdor &= ~(1ul << (pin & 0x1F));
}

/**
 * \brief 设置FGPIO引脚初始电平为高
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_fgpio_pin_init_out_high (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    p_hw_fgpio->fgpio[pin >> 5].pdor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 读FGPIO引脚初始电平
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 低电平
 * \retval 1 : 高电平
 */
am_static_inline
int amhw_kl26_fgpio_pin_init_get (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    return (((p_hw_fgpio->fgpio[pin >> 5].pdor &
            (1ul << (pin & 0x1F))) !=0) ? 1 : 0);
}

/**
 * \brief 设置FGPIO引脚电平为高
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \note   向PSOR寄存器写1，写PDOR寄存器，使管脚输出电平为高
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_fgpio_pin_out_high (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    p_hw_fgpio->fgpio[pin >> 5].psor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 设置FGPIO引脚电平为低
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \note   向PCOR寄存器写1，清PDOR寄存器，使管脚输出电平为低
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_fgpio_pin_out_low (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    p_hw_fgpio->fgpio[pin >> 5].pcor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 翻转FGPIO引脚电平
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \note   向PTOR寄存器写1，翻转PDOR寄存器的值，使管脚输出电平翻转
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_fgpio_pin_out_toggle (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    p_hw_fgpio->fgpio[pin >> 5].ptor |= (1ul << (pin & 0x1F));
}

/**
 * \brief 读FGPIO引脚输入电平
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 低电平
 * \retval 1 : 高电平
 */
am_static_inline
int amhw_kl26_fgpio_pin_input_get (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    return (((p_hw_fgpio->fgpio[pin >> 5].pdir &
            (1ul << (pin & 0x1F))) !=0) ? 1 : 0);
}

/**
 * \brief 设置FGPIO引脚为输入方向
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_fgpio_pin_dir_input (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    p_hw_fgpio->fgpio[pin >> 5].pddr &= ~(1ul << (pin & 0x1F));
}

/**
 * \brief 设置FGPIO引脚为输出方向
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_fgpio_pin_dir_output (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    p_hw_fgpio->fgpio[pin >> 5].pddr |= (1ul << (pin & 0x1F));
}

/**
 * \brief 读FGPIO引脚输入\输出方向
 *
 * \param[in] p_hw_fgpio : 指向FGPIO寄存器块的指针
 * \param[in] pin        : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 输入
 * \retval 1 : 输出
 */
am_static_inline
int amhw_kl26_fgpio_pin_dir_get (amhw_kl26_fgpio_t *p_hw_fgpio, int pin)
{
    return (((p_hw_fgpio->fgpio[pin >> 5].pddr &
            (1ul << (pin & 0x1F))) != 0) ? 1 : 0);
}

/**
 * @} amhw_kl26_if_gpio
 */

#ifdef __cplusplus
}
#endif

#endif /* __KL26_GPIO_H */

/* end of file */

