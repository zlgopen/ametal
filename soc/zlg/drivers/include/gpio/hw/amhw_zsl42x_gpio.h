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
 * \brief GPIO hardware operation interface
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-06  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_GPIO_H
#define __AMHW_ZSL42x_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"

/**
 * \addtogroup amhw_zsl42x_if_gpio
 * \copydoc amhw_zsl42x_gpio.h
 * @{
 */

#define AMHW_ZSL42x_GPIO_PORT_MAX 6  /**< \brief GPIO 定义GPIO端口数量 */

/**
 * \brief GPIO 引脚寄存器块结构体
 */
typedef struct {

    /**
     * \brief 端口功能配置器(64个，每个对应一个引脚)
     *        PA0~PA15 、
     *        PB0~PB15 、
     *        PC0~PC15 、
     *        PD0~PD15
     */
    __IO  uint32_t pin_x_sel_I[64];

} amhw_zsl42x_gpio_pin_t;

/**
 * \brief GPIO 端口寄存器结构体
 */
typedef struct {
    __IO  uint32_t dir;         /**< \brief 输入输出配置寄存器 */
    __IO  uint32_t in;          /**< \brief 输入值寄存器*/
    __IO  uint32_t out;         /**< \brief 输出值配置寄存器*/
    __IO  uint32_t ads;         /**< \brief 数模配置寄存器*/
    __IO  uint32_t bset;        /**< \brief 位置位寄存器*/
    __IO  uint32_t bclr;        /**< \brief 位清零寄存器*/
    __IO  uint32_t bsetclr;     /**< \brief 位置位清零寄存器*/
    __IO  uint32_t dr;          /**< \brief 驱动能力配置寄存器*/
    __IO  uint32_t pu;          /**< \brief 上拉使能配置寄存器*/
    __IO  uint32_t pd;          /**< \brief 下拉使能配置寄存器*/
    __IO  uint32_t reserve;     /**< \brief 保留位 */
    __IO  uint32_t od;          /**< \brief 开漏输出配置寄存器*/
    __IO  uint32_t hie;         /**< \brief 高电平中断使能配置寄存器*/
    __IO  uint32_t lie;         /**< \brief 低电平中断使能配置寄存器*/
    __IO  uint32_t rie;         /**< \brief 上升沿中断使能配置寄存器*/
    __IO  uint32_t fie;         /**< \brief 下降沿中断使能配置寄存器*/

}amhw_zsl42x_gpio_port_t;

/**
 * \brief GPIO 端口中断寄存器结构体
 */
typedef struct {
    __IO  uint32_t stat;          /**< \brief 中断状态寄存器*/
          uint32_t reserve0[3];   /**< \brief 保留位 */
    __IO  uint32_t iclr;          /**< \brief 中断清除寄存器*/
          uint32_t reserve1[11];  /**< \brief 保留位 */
}amhw_zsl42x_gpio_port_int_t;

/**
 * \brief GPIO 端口辅助寄存器结构体
 */
typedef struct {
    __IO  uint32_t ctrl0;         /**< \brief 端口辅助功能配置寄存器0 */
    __IO  uint32_t ctrl1;         /**< \brief 端口辅助功能配置寄存器1 */
    __IO  uint32_t ctrl2;         /**< \brief 端口辅助功能配置寄存器2 */
    __IO  uint32_t timgs;         /**< \brief 端口辅助功能定时器门控选择 */
    __IO  uint32_t times;         /**< \brief 端口辅助功能定时器ETR选择 */
    __IO  uint32_t timcps;        /**< \brief 端口辅助功能定时器捕获输入选择 */
    __IO  uint32_t pcas;          /**< \brief 端口辅助功能PCA捕获选择 */
    __IO  uint32_t pcnt;          /**< \brief 端口辅助功能PCNT输入选择 */
    __I   uint32_t reserve[824];  /**< \brief 保留位 */
}amhw_zsl42x_gpio_port_support_t;

/**
 * \brief 一组结构体（四个端口为一组，例如PA-PD）
 */
typedef struct {
    amhw_zsl42x_gpio_pin_t          gpio_pin_x;
    amhw_zsl42x_gpio_port_t         gpio_port[4];
    amhw_zsl42x_gpio_port_int_t     gpio_int_reg[4];
    amhw_zsl42x_gpio_port_support_t gpio_sup_reg;
} amhw_zsl42x_gpio_group_t;

/**
 * \brief GPIO寄存器块结构体
 */
typedef struct amhw_zsl42x_gpio {
    amhw_zsl42x_gpio_group_t    gpio[(AMHW_ZSL42x_GPIO_PORT_MAX/4) + 1];
} amhw_zsl42x_gpio_t;


/**
 * \brief GPIO引脚复用设置
 * \note 每个引脚不一样，唯一可以确定的是普通GPIO（不复用）对应的值为0
 */
typedef enum {
    AMHW_ZSL42x_AFIO_NO = 0,
    AMHW_ZSL42x_AFIO_1,
    AMHW_ZSL42x_AFIO_2,
    AMHW_ZSL42x_AFIO_3,
    AMHW_ZSL42x_AFIO_4,
    AMHW_ZSL42x_AFIO_5,
    AMHW_ZSL42x_AFIO_6,
    AMHW_ZSL42x_AFIO_7,
}amhw_zsl42x_gpio_afio_t;

/**
 * \brief 引脚复用设置
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] afio_choice: 引脚复用选择
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_afio_set (amhw_zsl42x_gpio_t      *p_hw_gpio,
                                amhw_zsl42x_gpio_afio_t  afio_choice,
                                int                      pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_pin_x.pin_x_sel_I[pin % 64] = afio_choice;
}

/**
 * \brief 引脚复用情况获取
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 引脚当前复用设置的具体数值
 */
am_static_inline
amhw_zsl42x_gpio_afio_t amhw_zsl42x_gpio_afio_get (amhw_zsl42x_gpio_t *p_hw_gpio,
                                                   int                 pin)
{
    return (amhw_zsl42x_gpio_afio_t)p_hw_gpio->gpio[pin / 64].gpio_pin_x.pin_x_sel_I[pin % 64];
}

/**
 * \brief 设置GPIO引脚方向为输出
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_dir_output (amhw_zsl42x_gpio_t *p_hw_gpio,
                                      int                 pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].dir &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 设置GPIO引脚方向为输入
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_dir_input (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].dir |= (0x1ul << (pin % 16));
}

/**
 * \brief 获取GPIO引脚输入输出方向
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 1 ： 输入
 *         0 ： 输出
 */
am_static_inline
int amhw_zsl42x_gpio_pin_dir_get (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    return (p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].dir & (0x1ul << (pin % 16))) ? 1 : 0;
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
int amhw_zsl42x_gpio_pin_input_get(amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    return (p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].in & (0x1ul << (pin % 16))) ? 1 : 0;
}

/**
 * \brief 设置GPIO引脚电平为高
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 *
 * \note 如果配置成开漏输出，则需外部上拉电阻来拉高。
 */
am_static_inline
void amhw_zsl42x_gpio_pin_out_high (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].out |= (0x1ul << (pin % 16));
}

/**
 * \brief 设置GPIO引脚电平为低
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 *
 */
am_static_inline
void amhw_zsl42x_gpio_pin_out_low (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].out &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 读GPIO引脚输出电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 低电平
 * \retval 1 : 高电平
 */
am_static_inline
int amhw_zsl42x_gpio_pin_output_get (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    return (p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].out & (0x1ul << (pin % 16))) ? 1 : 0;
}

/**
 * \brief 设置GPIO引脚为模拟端口
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_analog_set (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].ads |= (0x1ul << (pin % 16));
}

/**
 * \brief 设置GPIO引脚为数字端口
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_digital_set (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].ads &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 获取当前端口数字端口/模拟端口
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 数字端口
 * \retval 1 : 模拟端口
 */
am_static_inline
int amhw_zsl42x_gpio_pin_analog_digital_get (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    return (p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].ads & (0x1ul << (pin % 16))) ? 1 : 0;
}

/**
 * \brief 位置位寄存器     置位
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_bit_set (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bset |= (0x1ul << (pin % 16));
}

/**
 * \brief 位清零寄存器    清零
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_bit_clr (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bclr |= (0x1ul << (pin % 16));
}

/**
 * \brief 位清零寄存器   保持
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_bit_hold (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bset &= ~(0x1ul << (pin % 16));
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bclr &= ~(0x1ul << (pin % 16));
}


/**
 * \brief 位置位清零寄存器   置位
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 *
 * \note 该寄存器    高16bit -- 置位  （对应位写1）
 *              低16bit -- 清零  （对应位写1）
 *
 *       高16bit和低16bit 相同位同时写1时，
 *       清零具有高优先级。即该端口被清零。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_bitsetclr_set (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bsetclr |= (0x1ul << ((pin % 16) + 16));
}

/**
 * \brief 位置位清零寄存器    清零
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \note 该寄存器    高16bit -- 置位  （对应位写1）
 *              低16bit -- 清零  （对应位写1）
 *
 *       高16bit和低16bit 相同位同时写1时，
 *       清零具有高优先级。即该端口被清零。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_bitsetclr_clr (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bsetclr |= (0x1ul << (pin % 16));
}


/**
 * \brief 位置位清零寄存器    保持
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_bitsetclr_hold (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bsetclr &= ~(0x1ul << (pin % 16));
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].bsetclr &= ~(0x1ul << ((pin % 16) + 16));
}

/**
 * \brief 上拉使能配置寄存器    使能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_pu_enable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].pu |= (0x1ul << (pin % 16));
}

/**
 * \brief 上拉使能配置寄存器   禁能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_pu_disable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].pu &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 下拉使能配置寄存器    使能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_pd_enable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].pd |= (0x1ul << (pin % 16));
}

/**
 * \brief 下拉使能配置寄存器   禁能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_pd_disable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].pd &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 开漏输出配置寄存器    使能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_od_enable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].od |= (0x1ul << (pin % 16));
}

/**
 * \brief 开漏输出配置寄存器   禁能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_od_disable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].od &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 驱动能力配置寄存器    低驱动能力
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_driver_low (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].dr |= (0x1ul << (pin % 16));
}

/**
 * \brief 驱动能力配置寄存器  高驱动能力
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_driver_high (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].dr &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 高电平中断使能配置寄存器    使能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_high_int_enable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].hie |= (0x1ul << (pin % 16));
}

/**
 * \brief 高电平中断使能配置寄存器   禁能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_high_int_disable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].hie &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 低电平中断使能配置寄存器    使能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_low_int_enable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].lie |= (0x1ul << (pin % 16));
}

/**
 * \brief 低电平中断使能配置寄存器   禁能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_low_int_disable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].lie &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 上升沿中断使能配置寄存器    使能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_raising_int_enable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].rie |= (0x1ul << (pin % 16));
}

/**
 * \brief 上升沿中断使能配置寄存器   禁能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_raising_int_disable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].rie &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 下降沿中断使能配置寄存器    使能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_falling_int_enable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].fie |= (0x1ul << (pin % 16));
}

/**
 * \brief 下降沿中断使能配置寄存器   禁能
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_pin_falling_int_disable (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_port[(pin % 64) / 16].fie &= ~(0x1ul << (pin % 16));
}

/**
 * \brief 中断状态寄存器  获取是否有中断触发
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 1 : 有中断触发
 * \retval 0 : 无中断触发
 */
am_static_inline
int amhw_zsl42x_gpio_pin_int_flag_get (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    return (p_hw_gpio->gpio[pin / 64].gpio_int_reg[(pin % 64) / 16].stat & (0x1ul << (pin % 16))) ? 1 : 0;
}

/**
 * \brief 中断清除寄存器  清除标志
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval none
 */
am_static_inline
void amhw_zsl42x_gpio_pin_int_flag_clr (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_int_reg[(pin % 64) / 16].iclr |= (0x1ul << (pin % 16));
    p_hw_gpio->gpio[pin / 64].gpio_int_reg[(pin % 64) / 16].iclr &= ~(0x1ul << (pin % 16));
    p_hw_gpio->gpio[pin / 64].gpio_int_reg[(pin % 64) / 16].iclr |= (0x1ul << (pin % 16));
}

/**
 * \brief 中断清除寄存器  保留不改变
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval none
 */
am_static_inline
void amhw_zsl42x_gpio_pin_int_flag_hold (amhw_zsl42x_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin / 64].gpio_int_reg[(pin % 64) / 16].iclr |= (0x1ul << (pin % 16));
}

/**
 * \brief 端口辅助功能配置寄存器0  DEEPSLEEP模式
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 当系统处于 ACTIVE/SLEEP模式下时，系统时钟不会被关闭，可以将 IESEL设置为 0，
 *       此时触发端口中断的外部信号源将经过系统时钟同步之后产生中断信号，可以滤除外部信号源
 *       的毛刺。
 *       当系统即将进入 DEEPSLEEP模式下时，系统时钟将被关闭，可以将 IESEL设置为 1，此
 *       时触发端口中断的外部信号源直接产生中断信号，不能滤除外部信号源的毛刺。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_int_mode_deepsleep (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl0 |= (0x1ul);
}

/**
 * \brief 端口辅助功能配置寄存器0  ACTIVE/SLEEP模式
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 当系统处于 ACTIVE/SLEEP模式下时，系统时钟不会被关闭，可以将 IESEL设置为 0，
 *       此时触发端口中断的外部信号源将经过系统时钟同步之后产生中断信号，可以滤除外部信号源
 *       的毛刺。
 *       当系统即将进入 DEEPSLEEP模式下时，系统时钟将被关闭，可以将 IESEL设置为 1，此
 *       时触发端口中断的外部信号源直接产生中断信号，不能滤除外部信号源的毛刺。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_int_mode_active_or_sleep (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl0 &= ~(0x1ul);
}

/**
 * \brief IR输出极性选择  正向输出
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_irout_polarity_forward (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 &= ~((0x1ul << 14));
}

/**
 * \brief IR输出极性选择  反向输出
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_irout_polarity_reverse (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 |= ((0x1ul << 14));
}

/**
 * \brief IR输出极性获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \retval 0 : 正向输出
 * \retval 1 : 反向输出
 */
am_static_inline
int amhw_zsl42x_gpio_sup_irout_polarity_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 & (0x1ul << 14)) ? 1 : 0;
}

/**
 * \brief hclk输出门控   门控
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_hclk_gate (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 &= ~((0x1ul << 13));
}

/**
 * \brief hclk输出门控    输出
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_hclk_output (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 |= ((0x1ul << 13));
}

/**
 * \brief pclk输出门控   门控
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_pclk_gate (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 &= ~((0x1ul << 12));
}

/**
 * \brief pclk输出门控    输出
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_pclk_output (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 |= ((0x1ul << 12));
}

/**
 * \brief hclk输出分频选择 、
 *        pclk输出分频选择 、
 *        tclk输出分频选择
 *        枚举量
 */
typedef enum {
    AMHW_ZSL42x_GPIO_SUP_CLK_DIV_NO = 0ul,
    AMHW_ZSL42x_GPIO_SUP_CLK_DIV_2,
    AMHW_ZSL42x_GPIO_SUP_CLK_DIV_4,
    AMHW_ZSL42x_GPIO_SUP_CLK_DIV_8,
}amhw_zsl42x_gpio_sup_clk_div_t;

/**
 * \brief hclk输出分频选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] div_value : 分频系数    amhw_zsl42x_gpio_sup_clk_div_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_hclk_div_set (amhw_zsl42x_gpio_t             *p_hw_gpio,
                                        amhw_zsl42x_gpio_sup_clk_div_t  div_value)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 = (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 & (~(0x3ul << 10))) |
                                            ((div_value & 0x3ul) << 10);
}

/**
 * \brief hclk输出分频系数获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 当前分频系数 amhw_zsl42x_gpio_sup_clk_div_t枚举量中的一种
 */
am_static_inline
amhw_zsl42x_gpio_sup_clk_div_t amhw_zsl42x_gpio_sup_hclk_div_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_clk_div_t)((p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 >> 10) & 0x3ul);
}

/**
 * \brief pclk输出分频选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] div_value : 分频系数    amhw_zsl42x_gpio_sup_clk_div_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_pclk_div_set (amhw_zsl42x_gpio_t             *p_hw_gpio,
                                        amhw_zsl42x_gpio_sup_clk_div_t  div_value)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 = (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 & (~(0x3ul << 8))) |
                                            ((div_value & 0x3ul) << 8);
}

/**
 * \brief pclk输出分频系数获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 当前分频系数 amhw_zsl42x_gpio_sup_clk_div_t枚举量中的一种
 */
am_static_inline
amhw_zsl42x_gpio_sup_clk_div_t amhw_zsl42x_gpio_sup_pclk_div_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_clk_div_t)((p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 >> 8) & 0x3ul);
}

/**
 * \brief 外部时钟信号来源选择  、
 *        SPI0—SSN信号来源选择 、
 *        SPI1—SSN信号来源选择
 *        枚举量
 */
typedef enum {
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_HIGH = 0,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PA3,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PA4,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PA6,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PA8,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PA9,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PA12,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PA15,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB1,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB2,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB5,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB6,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB9,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB10,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB12,
    AMHW_ZSL42x_GPIO_SUP_EXTCLK_SSN01_PB14,
}amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t;

/**
 * \brief SPI0 SSN信号来源选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] ssn0_src  : 信号来源    amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_spi0_ssn_set (amhw_zsl42x_gpio_t                         *p_hw_gpio,
                                        amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t  ssn0_src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 = (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 & (~(0xful << 4))) |
                                            ((ssn0_src & 0xful) << 4);
}

/**
 * \brief SPI0 SSN信号来源获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 当前分频系数 amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t枚举量中的一种
 */
am_static_inline
amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t amhw_zsl42x_gpio_sup_spi0_ssn_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t)((p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 >> 4) & 0xful);
}

/**
 * \brief SPI1 SSN信号来源选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] ssn1_src  : 信号来源    amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_spi1_ssn_set (amhw_zsl42x_gpio_t                         *p_hw_gpio,
                                        amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t  ssn1_src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 = (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 & (~(0xful << 0))) |
                                            ((ssn1_src & 0xful) << 0);
}

/**
 * \brief SPI1 SSN信号来源获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 当前分频系数 amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t枚举量中的一种
 */
am_static_inline
amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t amhw_zsl42x_gpio_sup_spi1_ssn_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t)((p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 >> 0) & 0xful);
}

/**
 * \brief 外部时钟信号来源选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] ssn1_src  : 信号来源    amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_ext_clk_set (amhw_zsl42x_gpio_t                         *p_hw_gpio,
                                       amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t  extclk_src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 = (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 & (~(0xful << 0))) |
                                            ((extclk_src & 0xful) << 0);
}

/**
 * \brief 外部时钟信号来源获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 当前分频系数 amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t枚举量中的一种
 */
am_static_inline
amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t amhw_zsl42x_gpio_sup_ext_clk_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_extclk_or_ssn01_src_t)((p_hw_gpio->gpio[0].gpio_sup_reg.ctrl1 >> 0) & 0xful);
}

/**
 * \brief AHB总线访问模式   使用FASTIO总线访问PxIN,PxOUT寄存器
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_inout_fastio_bus_mode (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 &= ~(0x3ul << 15);
}

/**
 * \brief AHB总线访问模式   使用AHB总线访问PxIN,PxOUT寄存器
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_inout_ahb_bus_mode (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 |= (0x3ul << 15);
}

/**
 * \brief AHB总线访问模式获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return   0   使用FASTIO总线访问PxIN,PxOUT寄存器
 *           1   使用AHB总线访问PxIN,PxOUT寄存器
 */
am_static_inline
int amhw_zsl42x_gpio_sup_inout_bus_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 & (0x3ul << 15)) ? 1 : 0;
}

/**
 * \brief tclk输出信号源选择
 *        枚举量
 */
typedef enum {
    AMHW_ZSL42x_GPIO_SUP_TCLK_NO = 0,
    AMHW_ZSL42x_GPIO_SUP_TCLK_RCH,
    AMHW_ZSL42x_GPIO_SUP_TCLK_XTH,
    AMHW_ZSL42x_GPIO_SUP_TCLK_PLL,
}amhw_zsl42x_gpio_sup_tclk_src_t;

/**
 * \brief tclk输出分频选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] div_value : 分频系数    amhw_zsl42x_gpio_sup_clk_div_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tclk_div_set (amhw_zsl42x_gpio_t             *p_hw_gpio,
                                        amhw_zsl42x_gpio_sup_clk_div_t  div_value)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 = (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 & (~(0x3ul << 6))) |
                                            ((div_value & 0x3ul) << 6);
}

/**
 * \brief tclk输出分频系数获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 当前分频系数 amhw_zsl42x_gpio_sup_clk_div_t枚举量中的一种
 */
am_static_inline
amhw_zsl42x_gpio_sup_clk_div_t amhw_zsl42x_gpio_sup_tclk_div_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_clk_div_t)((p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 >> 6) & 0x3ul);
}

/**
 * \brief tclk输出信号源选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] tclk_src  : 分频系数    amhw_zsl42x_gpio_sup_tclk_src_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tclk_src_set (amhw_zsl42x_gpio_t             *p_hw_gpio,
                                        amhw_zsl42x_gpio_sup_clk_div_t  tclk_src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 = (p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 & (~(0x3ul << 4))) |
                                            ((tclk_src & 0x3ul) << 4);
}

/**
 * \brief ttclk输出信号源获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return 当前分频系数 amhw_zsl42x_gpio_sup_tclk_src_t枚举量中的一种
 */
am_static_inline
amhw_zsl42x_gpio_sup_tclk_src_t amhw_zsl42x_gpio_sup_tclk_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_tclk_src_t)((p_hw_gpio->gpio[0].gpio_sup_reg.ctrl2 >> 4) & 0x3ul);
}

/**
 * \brief LPTimer1定时器GATE输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_lptim1_gate_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                               uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.pcas = (p_hw_gpio->gpio[0].gpio_sup_reg.pcas & (~(0x7ul << 6))) |
                                           ((src & 0x7ul) << 6);
}

/**
 * \brief LPTimer1定时器GATE输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_lptim1_gate_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.pcas >> 6) & 0x7ul);
}

/**
 * \brief LPTimer0定时器GATE输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_lptim0_gate_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                               uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timgs = (p_hw_gpio->gpio[0].gpio_sup_reg.timgs & (~(0x7ul << 12))) |
                                            ((src & 0x7ul) << 12);
}

/**
 * \brief LPTimer0定时器GATE输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_lptim0_gate_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timgs >> 12) & 0x7ul);
}

/**
 * \brief Timer3定时器GATE输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim3_gate_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timgs = (p_hw_gpio->gpio[0].gpio_sup_reg.timgs & (~(0x7ul << 9))) |
                                            ((src & 0x7ul) << 9);
}

/**
 * \brief Timer3定时器GATE输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim3_gate_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timgs >> 9) & 0x7ul);
}

/**
 * \brief Timer2定时器GATE输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim2_gate_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timgs = (p_hw_gpio->gpio[0].gpio_sup_reg.timgs & (~(0x7ul << 6))) |
                                            ((src & 0x7ul) << 6);
}

/**
 * \brief Timer3定时器GATE输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim2_gate_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timgs >> 6) & 0x7ul);
}

/**
 * \brief Timer1定时器GATE输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim1_gate_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timgs = (p_hw_gpio->gpio[0].gpio_sup_reg.timgs & (~(0x7ul << 3))) |
                                            ((src & 0x7ul) << 3);
}

/**
 * \brief Timer1定时器GATE输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim1_gate_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timgs >> 3) & 0x7ul);
}

/**
 * \brief Timer0定时器GATE输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim0_gate_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timgs = (p_hw_gpio->gpio[0].gpio_sup_reg.timgs & (~(0x7ul))) |
                                            (src & 0x7ul);
}

/**
 * \brief Timer0定时器GATE输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim0_gate_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (p_hw_gpio->gpio[0].gpio_sup_reg.timgs & 0x7ul);
}

/**
 * \brief LPTimer1定时器ETR输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_lptim1_etr_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                              uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.pcas = (p_hw_gpio->gpio[0].gpio_sup_reg.pcas & (~(0x7ul << 9))) |
                                            ((src & 0x7ul) << 9);
}

/**
 * \brief LPTimer1定时器ETR输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_lptim1_etr_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.pcas >> 9) & 0x7ul);
}

/**
 * \brief LPTimer0定时器ETR输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_lptim0_etr_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                              uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.times = (p_hw_gpio->gpio[0].gpio_sup_reg.times & (~(0x7ul << 12))) |
                                            ((src & 0x7ul) << 12);
}

/**
 * \brief LPTimer0定时器ETR输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器 ETR选择(GPIO_TIMES)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_lptim0_etr_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.times >> 12) & 0x7ul);
}

/**
 * \brief Timer3定时器ETR输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim3_etr_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                            uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.times = (p_hw_gpio->gpio[0].gpio_sup_reg.times & (~(0x7ul << 9))) |
                                            ((src & 0x7ul) << 9);
}

/**
 * \brief Timer3定时器ETR输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器 ETR选择(GPIO_TIMES)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim3_etr_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.times >> 9) & 0x7ul);
}

/**
 * \brief Timer2定时器ETR输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim2_etr_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                            uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.times = (p_hw_gpio->gpio[0].gpio_sup_reg.times & (~(0x7ul << 6))) |
                                            ((src & 0x7ul) << 6);
}

/**
 * \brief Timer2定时器ETR输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器 ETR选择(GPIO_TIMES)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim2_etr_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.times >> 6) & 0x7ul);
}

/**
 * \brief Timer1定时器ETR输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim1_etr_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                            uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.times = (p_hw_gpio->gpio[0].gpio_sup_reg.times & (~(0x7ul << 3))) |
                                            ((src & 0x7ul) << 3);
}

/**
 * \brief Timer1定时器ETR输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器 ETR选择(GPIO_TIMES)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim1_etr_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.times >> 3) & 0x7ul);
}

/**
 * \brief Timer0定时器ETR输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器门控选择(GPIO_TIMGS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim0_etr_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                            uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.times = (p_hw_gpio->gpio[0].gpio_sup_reg.times & (~(0x7ul << 0))) |
                                            ((src & 0x7ul) << 0);
}

/**
 * \brief Timer0定时器ETR输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器 ETR选择(GPIO_TIMES)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim0_etr_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.times >> 0) & 0x7ul);
}

/**
 * \brief Timer3定时器CH0B输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim3_ch0b_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timcps = (p_hw_gpio->gpio[0].gpio_sup_reg.timcps & (~(0x7ul << 12))) |
                                             ((src & 0x7ul) << 12);
}

/**
 * \brief Timer3定时器CH0B输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim3_ch0b_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timcps >> 12) & 0x7ul);
}

/**
 * \brief Timer3定时器CH0A输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim3_ch0a_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timcps = (p_hw_gpio->gpio[0].gpio_sup_reg.timcps & (~(0x7ul << 9))) |
                                             ((src & 0x7ul) << 9);
}

/**
 * \brief Timer3定时器CH0A输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim3_ch0a_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timcps >> 9) & 0x7ul);
}

/**
 * \brief Timer2定时器CHA输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim2_cha_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timcps = (p_hw_gpio->gpio[0].gpio_sup_reg.timcps & (~(0x7ul << 6))) |
                                             ((src & 0x7ul) << 6);
}

/**
 * \brief Timer2定时器CHA输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim2_cha_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timcps >> 6) & 0x7ul);
}

/**
 * \brief Timer1定时器CHA输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim1_cha_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timcps = (p_hw_gpio->gpio[0].gpio_sup_reg.timcps & (~(0x7ul << 3))) |
                                             ((src & 0x7ul) << 3);
}

/**
 * \brief Timer1定时器CHA输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim1_cha_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timcps >> 3) & 0x7ul);
}

/**
 * \brief Timer0定时器CHA输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_tim0_cha_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                             uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.timcps = (p_hw_gpio->gpio[0].gpio_sup_reg.timcps & (~(0x7ul << 0))) |
                                             ((src & 0x7ul) << 0);
}

/**
 * \brief Timer0定时器CHA输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能定时器捕获输入选择(GPIO_TIMCPS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_tim0_cha_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.timcps >> 0) & 0x7ul);
}

/**
 * \brief PCA ECI时钟输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_pca_eci_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                           uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.pcas = (p_hw_gpio->gpio[0].gpio_sup_reg.pcas & (~(0x7ul << 3))) |
                                           ((src & 0x7ul) << 3);
}

/**
 * \brief PCA ECI时钟输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_pca_eci_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.pcas >> 3) & 0x7ul);
}

/**
 * \brief PCA CH0 捕获口输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *         --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_pca_ch0_src_set (amhw_zsl42x_gpio_t *p_hw_gpio,
                                           uint8_t             src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.pcas = (p_hw_gpio->gpio[0].gpio_sup_reg.pcas & (~(0x7ul << 0))) |
                                           ((src & 0x7ul) << 0);
}

/**
 * \brief PCA CH0 捕获口输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \note 具体选择数值，请参考用户手册GPIO章节的
 *       --端口辅助功能 PCA 捕获选择(GPIO_PCAS)
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_gpio_sup_pca_ch0_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return ((p_hw_gpio->gpio[0].gpio_sup_reg.pcas >> 0) & 0x7ul);
}

/**
 * \brief PCNT脉冲输入选择
 *        枚举量
 */
typedef enum {
    AMHW_ZSL42x_GPIO_SUP_PCNT_NO = 0,
    AMHW_ZSL42x_GPIO_SUP_PCNT_VC0_OUT,
    AMHW_ZSL42x_GPIO_SUP_PCNT_VC1_OUT,
    AMHW_ZSL42x_GPIO_SUP_PCNT_VC2_OUT,
}amhw_zsl42x_gpio_sup_pcnt_in_src_t;

/**
 * \brief PCNT_S1输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值   amhw_zsl42x_gpio_sup_pcnt_in_src_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_pcnt_s1_src_set (amhw_zsl42x_gpio_t                *p_hw_gpio,
                                           amhw_zsl42x_gpio_sup_pcnt_in_src_t src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.pcnt = (p_hw_gpio->gpio[0].gpio_sup_reg.pcnt & (~(0x3ul << 2))) |
                                           ((src & 0x3ul) << 2);
}

/**
 * \brief PCNT_S1输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return amhw_zsl42x_gpio_sup_pcnt_in_src_t枚举量
 */
am_static_inline
amhw_zsl42x_gpio_sup_pcnt_in_src_t amhw_zsl42x_gpio_sup_pcnt_s1_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_pcnt_in_src_t)((p_hw_gpio->gpio[0].gpio_sup_reg.pcnt >> 2) & 0x3ul);
}

/**
 * \brief PCNT_S0输入选择
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] src       : 设定数值   amhw_zsl42x_gpio_sup_pcnt_in_src_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_gpio_sup_pcnt_s0_src_set (amhw_zsl42x_gpio_t                *p_hw_gpio,
                                           amhw_zsl42x_gpio_sup_pcnt_in_src_t src)
{
    p_hw_gpio->gpio[0].gpio_sup_reg.pcnt = (p_hw_gpio->gpio[0].gpio_sup_reg.pcnt & (~(0x3ul << 0))) |
                                           ((src & 0x3ul) << 0);
}

/**
 * \brief PCNT_S0输入获取
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 *
 * \return amhw_zsl42x_gpio_sup_pcnt_in_src_t枚举量
 */
am_static_inline
amhw_zsl42x_gpio_sup_pcnt_in_src_t amhw_zsl42x_gpio_sup_pcnt_s0_src_get (amhw_zsl42x_gpio_t *p_hw_gpio)
{
    return (amhw_zsl42x_gpio_sup_pcnt_in_src_t)((p_hw_gpio->gpio[0].gpio_sup_reg.pcnt >> 0) & 0x3ul);
}

typedef enum amhw_zsl42x_gpiomode
{
    AMHW_ZSL42x_GPIO_MODE_IPU = 0,         /**< \brief 上拉输入模式  */
    AMHW_ZSL42x_GPIO_MODE_IPD = 1,         /**< \brief 下拉输入模式 */
    AMHW_ZSL42x_GPIO_MODE_AIN = 2,         /**< \brief 模拟输入模式 */
    AMHW_ZSL42x_GPIO_MODE_IN_FLOATING = 3, /**< \brief 浮空输入模式 */
    AMHW_ZSL42x_GPIO_MODE_OUT_PP = 4,      /**< \brief 推免输出模式 */
    AMHW_ZSL42x_GPIO_MODE_OUT_OD = 5,      /**< \brief 开漏输出模式 */
    AMHW_ZSL42x_GPIO_MODE_AOUT = 6         /**< \brief 模拟输出模式 */
}amhw_zsl42x_gpiomode_t;

/**
 * \brief 设置GPIO引脚的模式
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] gpio_mode : 引脚的模式，值为 amhw_zlg217_gpiomode_t 这个枚举类型
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
void amhw_zsl42x_gpio_pin_mode_set (amhw_zsl42x_gpio_t     *p_hw_gpio,
                                    amhw_zsl42x_gpiomode_t  gpio_mode,
                                    int                     pin);


/**
 * @} amhw_zsl42x_if_gpio
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSL42x_GPIO_H */

/* end of file */
