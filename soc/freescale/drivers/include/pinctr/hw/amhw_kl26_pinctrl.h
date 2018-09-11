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
 * \brief PORT 硬件操作接口
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-15  nwt,  make some changes about function name
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */

#ifndef AMHW_KL26_PINCTRL_H_
#define AMHW_KL26_PINCTRL_H_

#include "../../../../kl26/kl26_inum.h"
#include "../../../../kl26/kl26_periph_map.h"
#include "am_types.h"
#include "ametal.h"

/** 
 * \addtogroup amhw_kl26_if_pinctrl
 * \copydoc amhw_kl26_pinctrl.h
 * @{
 */
 
/**
 * \brief PORT 端口寄存器结构体
 */
typedef struct amhw_kl26_port_reg {
    __IO uint32_t port_pcr[32];         /**< \brief 端口管脚配置寄存器 */
    __O  uint32_t port_gpclr;           /**< \brief 端口控制低字节寄存器 */
    __O  uint32_t port_gpchr;           /**< \brief 端口控制高字节寄存器 */
    __IO uint32_t reserve[6];           /**< \brief 端口保留位*/
    __IO uint32_t port_isfr;            /**< \brief 端口中状态标志寄存器 */
    __IO uint32_t reserve1[983];        /**< \brief 端口保留位*/
}amhw_kl26_port_reg_t;

/**
 * \brief PORT 寄存器块结构体
 */
typedef struct amhw_kl26_port {

    amhw_kl26_port_reg_t port[5];            /**< \brief 5个端口 */
    
} amhw_kl26_port_t;

/**
 * \name  PORT_PCR寄存器位定义
 * @{
 */
 
/** \brief  引脚中断状态标志位 */
#define KL26_PORT_PCR_ISF               (0x1UL << 24)

/** \brief  DMA上升沿中断请求 */
#define AMHW_KL26_INT_DMA_RISING             (0x1UL)

/** \brief  DMA下降沿中断请求 */
#define AMHW_KL26_INT_DMA_FALLING            (0x2UL)

/** \brief  DMA上升沿或者下降沿中断请求 */
#define AMHW_KL26_INT_DMA_EITHER             (0x3UL)

/** \brief  引脚低电平触发中断 */
#define AMHW_KL26_INT_LOW                    (0x8UL)

/** \brief  引脚上升沿触发中断 */
#define AMHW_KL26_INT_RISING                 (0x9UL)

/** \brief  引脚下降沿触发中断 */
#define AMHW_KL26_INT_FALLING                (0xAUL)

/** \brief  引脚上升沿或者下降沿触发中断 */
#define AMHW_KL26_PINT_EITHER                (0xBUL)

/** \brief  引脚高电平触发中断 */
#define AMHW_KL26_INT_HIGH                   (0xCUL)

/** \brief  引脚强驱动能力 */
#define KL26_PORT_DRIVE_HIGH            (0x1UL << 6)

/** \brief  引脚输入滤波 */
#define KL26_PORT_FILTER                (0x1UL << 4)

/** \brief  引脚转换速率 */
#define KL26_PORT_SLEW_SLOW             (0x1UL << 2)

/** \brief  引脚上/下拉使能 */
#define KL26_PORT_PULL_EN               (0x1UL << 1)

/** \brief  引脚上拉选择 */
#define KL26_PORT_PULL_SEL              (0x1UL << 0)

/** \brief  默认GPIO功能 */
#define KL26_PORT_GPIO                  (0x1UL)

/** @} */

/**
 * \brief 读引脚中断状态标志位
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 没有触发中断
 * \retval 1 : 触发了中断
 */
am_static_inline
int amhw_kl26_port_pin_isf_get (amhw_kl26_port_t *p_hw_port, int pin)
{
    return ((((p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)])
            & KL26_PORT_PCR_ISF) !=0) ? 1 : 0);
}

/**
 * \brief 清零引脚中断状态标志位
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_isf_clr (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= KL26_PORT_PCR_ISF;
}

/**
 * \brief 设置引脚中断触发方式
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 * \param[in] irq       : 需要设置的引脚触发方式
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_irq_cfg (amhw_kl26_port_t *p_hw_port, int pin, uint32_t irq)
{
    AM_BITS_SET(p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)], 16, 4, irq);
}

/**
 * \brief 禁能引脚中断
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_irq_disable (amhw_kl26_port_t *p_hw_port, int pin)
{
    AM_BITS_SET(p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)], 16, 4, 0UL);
}

/**
 * \brief 设置引脚复用功能
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 * \param[in] func      : 要设置的引脚功能
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_func_cfg (amhw_kl26_port_t *p_hw_port, int pin, uint32_t func)
{
    AM_BITS_SET(p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)], 8, 3, func);
}

/**
 * \brief 设置引脚驱动能力强
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_drive_high (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= KL26_PORT_DRIVE_HIGH;
}

/**
 * \brief 设置引脚驱动能力弱
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_drive_low (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~KL26_PORT_DRIVE_HIGH);
}

/**
 * \brief 设置引脚输入滤波使能
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_filter_enable (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= KL26_PORT_FILTER;
}

/**
 * \brief 设置引脚输入滤波禁能
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_filter_disable (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~KL26_PORT_FILTER);
}

/**
 * \brief 设置引脚转换速率低
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_slew_slow (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= KL26_PORT_SLEW_SLOW;
}

/**
 * \brief 设置引脚转换速率快
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_slew_fast (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~KL26_PORT_SLEW_SLOW);
}

/**
 * \brief 设置引脚上/下拉使能
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_pull_enable (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= KL26_PORT_PULL_EN;
}


/**
 * \brief 设置引脚上/下拉禁能
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_pull_disable (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~KL26_PORT_PULL_EN);
}

/**
 * \brief 设置引脚上拉
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_pull_up (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= KL26_PORT_PULL_SEL;
}

/**
 * \brief 设置引脚下拉
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_pin_pull_down (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~KL26_PORT_PULL_SEL);
}

/**
 * \brief 低字节写端口管脚pin[15:0]
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 * \param[in] gpwe      : PORT端口管脚使能
 * \param[in] gpwd      : 要设置的PORT端口管脚值
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_low_word_write (amhw_kl26_port_t *p_hw_port,
                                    int          pin,
                                    uint32_t     gpwe,
                                    uint32_t     gpwd)
{
    uint32_t data=0;
    data = (gpwe << 16) | gpwd;
    p_hw_port->port[pin >> 5].port_gpclr = data;
}

/**
 * \brief 高字节写端口管脚pin[31:16]
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 * \param[in] gpwe      : PORT端口管脚使能
 * \param[in] gpwd      : 要设置的PORT端口管脚值
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_high_word_write (amhw_kl26_port_t *p_hw_port,
                                     int          pin,
                                     uint32_t     gpwe,
                                     uint32_t     gpwd)
{
    uint32_t data=0;
    data = (gpwe << 16) | gpwd;
    p_hw_port->port[pin >> 5].port_gpchr = data;
}

/**
 * \brief 读端口中断状态标志
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 端口中断状态标志值
 */
am_static_inline
int amhw_kl26_port_irq_flag_read (amhw_kl26_port_t *p_hw_port, int pin)
{
    return (p_hw_port->port[pin >> 5].port_isfr);
}

/**
 * \brief 清零端口中断状态标志
 *
 * \param[in] p_hw_port : 指向PORT寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_kl26_port_irq_flag_clr (amhw_kl26_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_isfr = 0xFFFFFFFF;
}

/**
 * @} amhw_kl26_if_pinctrl
 */

#ifdef __cplusplus
}
#endif

#endif /* AMHW_KL26_PINCTRL_H_ */
