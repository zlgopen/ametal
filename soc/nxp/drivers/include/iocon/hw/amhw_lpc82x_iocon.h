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
 * \brief LPC82X IOCON (IO配置) 硬件操作接口
 * 
 *  1. 设置模式（消极，下拉，上拉，中继模式）；
 *  2. 使能和禁能引脚迟滞；
 *  3. 使能和禁能引脚输入反转；
 *  4. 使能和禁能开漏模式；
 *  5. 设置引脚滤波周期；
 *  6. 设置滤波器分频；
 *  7. 设置I2C引脚总线速度模式( PIO0_10 和 PIO0_11)。
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  mem, modified.
 * - 1.01 15-09-18  zxl, modified.
 * - 1.00 15-07-03  zxl, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_IOCON_H
#define __AMHW_LPC82X_IOCON_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_iocon
 * \copydoc amhw_lpc82x_iocon.h
 * @{
 */


/**
 * \brief LPC82x IOCON 寄存器块结构体
 */
typedef struct amhw_lpc82x_iocon {
    __IO uint32_t  pio[31];       /**< \brief 引脚功能和电气特性控制寄存器 */
} amhw_lpc82x_iocon_t;


/**
 * \brief LPC82x (PIO0_0~PIO0_28) 引脚配置寄存器偏移表
 *
 *  offset = amhw_lpc82x_iocon_offset_pin[PIO_15] : PIO_15引脚寄存器偏移 \n
 *  pio[offset]                                   : 配置PIO_15的寄存器
 */
extern const uint8_t amhw_lpc82x_iocon_offset_pin[];

/**
 * \brief PINT 引脚模式
 */
typedef enum amhw_lpc82x_iocon_mode {
    AMHW_LPC82X_IOCON_MODE_INACTIVE = 0,  /**< \brief 消极模式(无上拉和下拉) */
    AMHW_LPC82X_IOCON_MODE_PULLDOWN,      /**< \brief 下拉模式               */
    AMHW_LPC82X_IOCON_MODE_PULLUP,        /**< \brief 上拉模式               */
    AMHW_LPC82X_IOCON_MODE_REPEATER       /**< \brief 中继模式               */
} amhw_lpc82x_iocon_mode_t;

/**
 *  \brief PINT 引脚滤波周期
 */
typedef enum amhw_lpc82x_iocon_filt_cycle {
    AMHW_LPC82X_IOCON_FILT_CYCLE_0 = 0,   /**< \brief 不使用滤波功能         */
    AMHW_LPC82X_IOCON_FILT_CYCLE_1,       /**< \brief 过滤小于一个周期信号   */
    AMHW_LPC82X_IOCON_FILT_CYCLE_2,       /**< \brief 过滤小于两个周期信号   */
    AMHW_LPC82X_IOCON_FILT_CYCLE_3        /**< \brief 过滤小于三个周期信号   */
} amhw_lpc82x_iocon_filt_cycle_t;

/**
 * \brief PINT 引脚滤波采样时钟分频
 */
typedef enum amhw_lpc82x_iocon_filt_clkdiv {
    AMHW_LPC82X_IOCON_FILT_CLKDIV_0 = 0,  /**< \brief 滤波器采样时钟分频0    */
    AMHW_LPC82X_IOCON_FILT_CLKDIV_1,      /**< \brief 滤波器采样时钟分频1    */
    AMHW_LPC82X_IOCON_FILT_CLKDIV_2,      /**< \brief 滤波器采样时钟分频2    */
    AMHW_LPC82X_IOCON_FILT_CLKDIV_3,      /**< \brief 滤波器采样时钟分频3    */
    AMHW_LPC82X_IOCON_FILT_CLKDIV_4,      /**< \brief 滤波器采样时钟分频4    */
    AMHW_LPC82X_IOCON_FILT_CLKDIV_5,      /**< \brief 滤波器采样时钟分频5    */
    AMHW_LPC82X_IOCON_FILT_CLKDIV_6       /**< \brief 滤波器采样时钟分频6    */
} amhw_lpc82x_iocon_filt_clkdiv_t;


/**
 * \brief PINT 引脚I2C模式（仅用于PIO0_10和PIO0_11）
 */
typedef enum amhw_lpc82x_iocon_i2c_mode {
    AMHW_LPC82X_IOCON_I2C_MODE_STD  = 0,  /**< \brief I2C标准模式/快速模式   */
    AMHW_LPC82X_IOCON_I2C_MODE_IO,        /**< \brief 普通IO模式             */
    AMHW_LPC82X_IOCON_I2C_MODE_PLUS       /**< \brief 超快速模式             */
} amhw_lpc82x_iocon_i2c_mode_t;


/**
 * \brief 配置引脚模式
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] mode       : 引脚模式
 *
 * \return 无
 *
 * \note 对PIO0_10和PIO0_11无效，内部开漏
 */
am_static_inline
void amhw_lpc82x_iocon_mode_set (amhw_lpc82x_iocon_t      *p_hw_iocon,
                                 int                       pin,
                                 amhw_lpc82x_iocon_mode_t  mode)
{
    uint32_t reg = p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]];

    AM_BIT_CLR_MASK(reg, AM_SBF(0x3, 3));
    AM_BIT_SET_MASK(reg, AM_SBF(mode, 3));

    p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]] = reg;
}


/**
 * \brief 使能引脚迟滞
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 对PIO0_10和PIO0_11无效
 */
am_static_inline
void amhw_lpc82x_iocon_hys_enable (amhw_lpc82x_iocon_t *p_hw_iocon, int pin)
{
    AM_BIT_SET_MASK(p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]], 
                    AM_BIT(5));
}


/**
 * \brief 禁能引脚迟滞
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 对PIO0_10和PIO0_11无效
 */
am_static_inline
void amhw_lpc82x_iocon_hys_disable (amhw_lpc82x_iocon_t *p_hw_iocon, int pin)
{
    AM_BIT_CLR_MASK(p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]],
                    AM_BIT(5));
}


/**
 * \brief 使能引脚输入反转
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_iocon_inv_enable (amhw_lpc82x_iocon_t *p_hw_iocon, int pin)
{
    AM_BIT_SET_MASK(p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]], 
                    AM_BIT(6));
}


/**
 * \brief 禁能引脚输入反转
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_iocon_inv_disable (amhw_lpc82x_iocon_t *p_hw_iocon, int pin)
{
    AM_BIT_CLR_MASK(p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]], 
                    AM_BIT(6));
}


/**
 * \brief 使能引脚开漏
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 对PIO0_10和PIO0_11无效，内部开漏
 */
am_static_inline
void amhw_lpc82x_iocon_od_enable (amhw_lpc82x_iocon_t *p_hw_iocon, int pin)
{
     AM_BIT_SET_MASK(p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]], 
                     AM_BIT(10));
}


/**
 * \brief 禁能引脚开漏
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 对PIO0_10和PIO0_11无效，内部开漏
 */
am_static_inline
void amhw_lpc82x_iocon_od_disable (amhw_lpc82x_iocon_t *p_hw_iocon, int pin)
{
   AM_BIT_CLR_MASK(p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]], 
                   AM_BIT(10));
}


/**
 * \brief 配置滤波引脚周期
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] cycle      : 引脚滤波周期
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_iocon_filt_cycle_set (amhw_lpc82x_iocon_t          *p_hw_iocon,
                                       int                           pin,
                                      amhw_lpc82x_iocon_filt_cycle_t cycle)
{
    uint32_t reg = p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]];

    AM_BIT_CLR_MASK(reg, AM_SBF(0x3, 11));
    AM_BIT_SET_MASK(reg, AM_SBF(cycle, 11));

    p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]] = reg;
}


/**
 * \brief 设置采样滤波器分频
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] div        : 引脚采样滤波器分频参数
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_iocon_filt_clkdiv_set (amhw_lpc82x_iocon_t         *p_hw_iocon,
                                        uint8_t                      pin,
                                     amhw_lpc82x_iocon_filt_clkdiv_t div)
{
    uint32_t reg = p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]];

    AM_BIT_CLR_MASK(reg, AM_SBF(0x7, 13));
    AM_BIT_SET_MASK(reg, AM_SBF(div, 13));

    p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]] = reg;
}


/**
 * \brief 设置I2C模式
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 #PIO0_10 或 #PIO0_11
 * \param[in] mode       : I2C模式
 *
 * \return 无
 *
 * \note 只对PIO0_10和PIO0_11有效
 */
am_static_inline
void amhw_lpc82x_iocon_i2c_mode_set (amhw_lpc82x_iocon_t         *p_hw_iocon,
                                     int                          pin,
                                     amhw_lpc82x_iocon_i2c_mode_t mode)
{
    uint32_t reg = p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]];

    AM_BIT_CLR_MASK(reg, AM_SBF(0x3, 8));
    AM_BIT_SET_MASK(reg, AM_SBF(mode, 8));

    p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]] = reg;

}


/**
 * \brief 配置PIO寄存器
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] data       : 写入到寄存器的数据
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_iocon_pio_cfg (amhw_lpc82x_iocon_t *p_hw_iocon, 
                                int                  pin, 
                                uint32_t             data)
{
    p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]] = data;
}


/**
 * \brief 获取PIO寄存器的数据 
 *
 * \param[in] p_hw_iocon : 指向IOCON寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return : 引脚PIO寄存器的值
 */
am_static_inline
uint32_t amhw_lpc82x_iocon_pio_get (amhw_lpc82x_iocon_t *p_hw_iocon, int pin)
{
    return p_hw_iocon->pio[amhw_lpc82x_iocon_offset_pin[pin]];
}

/**
 * @} amhw_lpc82x_if_iocon
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_IOCON_H */

/* end of file */
