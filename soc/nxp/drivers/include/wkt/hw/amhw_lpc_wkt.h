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
 * \brief 82X WKT 硬件操作接口描述
 * 
 * 1. 32位可加载减法计数器。加载计数值时计数器会自动启动。超时事件会生成一个
 *    中断/唤醒请求；
 * 2. WKT位于一个独立且始终上电的电源域中；
 * 3. WKT支持两个时钟源，一个时钟源来源于始终上电的电源域；
 * 4. WKT可用于将器件从任何低功耗模式(包括深度掉电模式)中唤醒，也可用于通用定时。
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-02  sky, modified.
 * - 1.00 15-07-10  zxl, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_WKT_H
#define __AMHW_LPC_WKT_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

 /** 
 * \addtogroup amhw_lpc_if_wkt
 * \copydoc amhw_lpc_wkt.h
 * @{
 */

/** \brief 最大计时值，32位的最大值 */
#define AMHW_LPC_WKT_INVAL_MAX              (0xFFFFFFFFUL)

/** \brief IRC时钟 750kHz */
#define AMHW_LPC_WKT_IRC_CLOCK_FREQ          750000

/** \brief 低功耗时钟 10kHz */
#define AMHW_LPC_WKT_LOW_POWER_CLOCK_FREQ    10000

/** \brief 外部时钟由用户自定义 */
#define AMHW_LPC_WKT_EXT_CLOCK_FREQ          0

/**
  * \brief 82x WKT 寄存器块结构体
 */
typedef struct amhw_lpc_wkt {
    __IO uint32_t ctrl;          /**< \brief 自唤醒定时器控制寄存器 (R/W) */
    __IO uint32_t reserved0[2];  /**< \brief 保留 */
    __IO uint32_t count;         /**< \brief 计数器寄存器 (R/W) */
} amhw_lpc_wkt_t;

/** \brief WKT 选择时钟源 */
typedef enum amhw_lpc_wkt_clksel_str {
    AMHW_LPC_WKT_IRC_CLOCK = 0,        /**< \brief 分频IRC时钟 750kHz */
    AMHW_LPC_WKT_LOW_POWER_CLOCK ,     /**< \brief 低功耗时钟 标称10kHz */
    AMHW_LPC_WKT_EXT_CLOCK = AM_BIT(3) /**< \brief 外部时钟 */
} amhw_lpc_wkt_clksel_str_t;


/**
 * \brief WKT CLKSEL 选择自唤醒定时器时钟源
 *
 * \param[in] p_hw_wkt : 指向WKT寄存器块的指针
 * \param[in] value    : 配置参数 值为 0 或者 1 (amhw_lpc_wkt_clksel_str_t)
 *                       取AMHW_LPC_WKT_IRC_CLOCK       (分频IRC时钟 750 kHz)
*                        取AMHW_LPC_WKT_LOW_POWER_CLOCK (低功耗时钟 10 kHz)
 *                       取AMHW_LPC_WKT_EXT_CLOCK       (外部时钟 频率自定义)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_wkt_clksel_cfg (amhw_lpc_wkt_t            *p_hw_wkt,
                              amhw_lpc_wkt_clksel_str_t  value)
{
     AM_BIT_CLR_MASK(p_hw_wkt->ctrl, AM_BIT(3) | AM_BIT(0));
     AM_BIT_SET_MASK(p_hw_wkt->ctrl, value);
}

/**
 * \brief WKT ALARMFLAG 获取 自唤醒定时器时钟源
 * \param[in] p_hw_wkt : 指向WKT寄存器块的指针
 * \retval =0 : IRC时钟
 * \retval =1 : 低功耗时钟
 * \retval >1 : 外部时钟
 */
am_static_inline
int amhw_lpc_wkt_clksel_get (amhw_lpc_wkt_t *p_hw_wkt)
{
    return (p_hw_wkt->ctrl & (AM_BIT(3) | AM_BIT(0)));
}

/**
 * \brief WKT CLEARCTR 清零自唤醒定时器
 * \param[in] p_hw_wkt : 指向WKT寄存器块的指针
 * \return 无
 * \note 清零计数器。计数在载入新的计数值之前不会停止
 */
am_static_inline
void amhw_lpc_wkt_clearctr_clr (amhw_lpc_wkt_t *p_hw_wkt)
{
     AM_BIT_SET(p_hw_wkt->ctrl, 2);
}

/**
 * \brief WKT ALARMFLAG 获取唤醒或警报定时器状态
 * \param[in] p_hw_wkt : 指向WKT寄存器块的指针
 * \retval 0 : 未超时
 * \retval 1 : 超时
 */
am_static_inline
int amhw_lpc_wkt_alarmflag_get (amhw_lpc_wkt_t *p_hw_wkt)
{
    return (((p_hw_wkt->ctrl & (1UL << 1)) != 0) ? 1 : 0);
}

/**
 * \brief WKT ALARMFLAG 写1清除唤醒或警报定时器状态
 * \param[in] p_hw_wkt : 指向WKT寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_lpc_wkt_alarmflag_clear (amhw_lpc_wkt_t *p_hw_wkt)
{
    AM_BIT_SET(p_hw_wkt->ctrl, 1);
}

/**
 * \brief 设置当前定时器的计数值
 *
 * \param[in] p_hw_wkt   : 指向WKT寄存器块的指针
 * \param[in] value      : 要设置定时器的计数值
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_wkt_count_set (amhw_lpc_wkt_t *p_hw_wkt, uint32_t value)
{
    p_hw_wkt->count = value;
}


/**
 * \brief 得到当前定时器的计数值
 * \param[in] p_hw_wkt : 指向WKT寄存器块的指针
 * \return 当前定时器的计数值
 */
am_static_inline
uint32_t amhw_lpc_wkt_count_get (amhw_lpc_wkt_t *p_hw_wkt)
{
    return p_hw_wkt->count;
}

/**
 * @} amhw_lpc_if_wkt
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_WKT_H */

/* end of file */
