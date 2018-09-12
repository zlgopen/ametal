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
 * \brief LPC82X PIN INT(引脚中断) 硬件操作接口
 *
 * 1. 最多支持8个引脚中断， 
 * 2. 每个中断可从引脚0-28中任意选择，
 *    可配置为上升沿、下降沿、双边沿、高电平、低电平触发；
 * 3. 中断通道可配置为模式匹配引擎输入，对多个输入进行布尔运算，
 *    运算结果用于触发中断或唤醒内核。
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  mem, modified.
 * - 1.01 15-09-18  zxl, modified.
 * - 1.00 15-07-06  zxl, first implementation.
 * \endinternal
 */
#ifndef __AMHW_LPC82X_PINT_H
#define __AMHW_LPC82X_PINT_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \addtogroup amhw_lpc82x_if_pint
 * \copydoc amhw_lpc82x_pint.h
 * @{
 */

/**
 * @brief LPC82x 引脚中断寄存器块结构体
 */
typedef struct amhw_lpc82x_pint{
    __IO uint32_t isel;     /**< \brief 引脚中断模式寄存           */
    __IO uint32_t ienr;     /**< \brief 电平或上升沿中断使能寄存器 */
    __O  uint32_t sienr;    /**< \brief 电平或上升沿中断设置寄存器 */
    __O  uint32_t cienr;    /**< \brief 电平或上升沿中断清零寄存器 */
    __IO uint32_t ienf;     /**< \brief 电平或下降沿中断使能寄存器 */
    __O  uint32_t sienf;    /**< \brief 电平或下降沿中断设置寄存器 */
    __O  uint32_t cienf;    /**< \brief 电平或下降沿中断清零寄存器 */
    __IO uint32_t rise;     /**< \brief 引脚中断上升沿寄存器       */
    __IO uint32_t fall;     /**< \brief 引脚中断下降沿寄存器       */
    __IO uint32_t ist;      /**< \brief 引脚中断状态寄存器         */
    __IO uint32_t pmctrl;   /**< \brief 模式匹配中断控制寄存器     */
    __IO uint32_t pmsrc;    /**< \brief 模式匹配中断位片源寄存器   */
    __IO uint32_t pmcfg;    /**< \brief 模式匹配中断位片配置寄存器 */
} amhw_lpc82x_pint_t;

/** \brief 中断通道数量 */
#define AMHW_LPC82X_PINT_CHAN_NUM     8

/**
 * \name 中断通道编号
 * \anchor grp_amhw_lpc82x_pint_chan
 * @{
 */

#define AMHW_LPC82X_PINT_CHAN_0    0  /**< \brief 中断通道 0       */
#define AMHW_LPC82X_PINT_CHAN_1    1  /**< \brief 中断通道 1       */
#define AMHW_LPC82X_PINT_CHAN_2    2  /**< \brief 中断通道 2       */
#define AMHW_LPC82X_PINT_CHAN_3    3  /**< \brief 中断通道 3       */
#define AMHW_LPC82X_PINT_CHAN_4    4  /**< \brief 中断通道 4       */
#define AMHW_LPC82X_PINT_CHAN_5    5  /**< \brief 中断通道 5       */
#define AMHW_LPC82X_PINT_CHAN_6    6  /**< \brief 中断通道 6       */
#define AMHW_LPC82X_PINT_CHAN_7    7  /**< \brief 中断通道 7       */

/** @} */

/**
 * \brief 触发模式定义
 */
typedef enum amhw_lpc82x_pint_trigger { 
    AMHW_LPC82X_PINT_TRIGGER_HIGH = 0,   /**< \brief 高电平触发模式 */
    AMHW_LPC82X_PINT_TRIGGER_LOW,        /**< \brief 低电平触发模式 */
    AMHW_LPC82X_PINT_TRIGGER_RISE,       /**< \brief 上升沿触发模式 */
    AMHW_LPC82X_PINT_TRIGGER_FALL,       /**< \brief 下降沿触发模式 */
    AMHW_LPC82X_PINT_TRIGGER_BOTH_EDGES, /**< \brief 双边沿触发模式 */
    AMHW_LPC82X_PINT_TRIGGER_OFF         /**< \brief 关闭触发模式   */
} amhw_lpc82x_pint_trigger_t;

/**
 * \brief 中断位片编号
 */
typedef enum amhw_lpc82x_pint_bitslice {
    AMHW_LPC82X_PINT_BITSLICE_0 = 0,          /**< \brief 位片 0         */
    AMHW_LPC82X_PINT_BITSLICE_1,              /**< \brief 位片 1         */
    AMHW_LPC82X_PINT_BITSLICE_2,              /**< \brief 位片 2         */
    AMHW_LPC82X_PINT_BITSLICE_3,              /**< \brief 位片 3         */
    AMHW_LPC82X_PINT_BITSLICE_4,              /**< \brief 位片 4         */
    AMHW_LPC82X_PINT_BITSLICE_5,              /**< \brief 位片 5         */
    AMHW_LPC82X_PINT_BITSLICE_6,              /**< \brief 位片 6         */
    AMHW_LPC82X_PINT_BITSLICE_7               /**< \brief 位片 7         */
} amhw_lpc82x_pint_bitslice_t;

/**
 * \brief 中断位片引脚匹配参数
 */
typedef enum amhw_lpc82x_pint_slice_arg {
    AMHW_LPC82X_PINT_SLICE_ARG_ALWAYS_VALID = 0, /**< \brief 一直有效           */ 
    AMHW_LPC82X_PINT_SLICE_ARG_STICKY_RISE,      /**< \brief 滞留上升沿         */ 
    AMHW_LPC82X_PINT_SLICE_ARG_STICKY_FALL,      /**< \brief 滞留下降沿         */ 
    AMHW_LPC82X_PINT_SLICE_ARG_STICKY_RISE_FALL, /**< \brief 滞留上升沿或下降沿 */ 
    AMHW_LPC82X_PINT_SLICE_ARG_HIGH_LEVEL,       /**< \brief 高电平有效         */ 
    AMHW_LPC82X_PINT_SLICE_ARG_LOW_LEVEL,        /**< \brief 低电平有效         */ 
    AMHW_LPC82X_PINT_SLICE_ARG_ALWAYS_INVALID,   /**< \brief 一直无效           */ 
    AMHW_LPC82X_PINT_SLICE_ARG_RISE_FALL         /**< \brief 上升沿或下降沿事件 */ 
} amhw_lpc82x_pint_slice_arg_t;

/**
 * \brief 设置中断触发模式
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 * \param[in] mode      : 触发模式
 *
 * \return 无
 *
 * \note 边沿检测状态只能软件清除；\n
 *       配置为中断边沿触发模式前，应调用 #amhw_lpc82x_pint_rise_dec_clr() 函数 
 *       清除之前的边沿检测状态。
 */
void amhw_lpc82x_pint_trigger_set (amhw_lpc82x_pint_t         *p_hw_pint, 
                                   uint8_t                     chan,
                                   amhw_lpc82x_pint_trigger_t  mode);

/**
 * \brief 获取通道的当前触发模式
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \return 通道触发模式
 */
amhw_lpc82x_pint_trigger_t 
amhw_lpc82x_pint_trigger_get (amhw_lpc82x_pint_t *p_hw_pint, 
                              uint8_t             chan);

/**
 * \brief 获取上升沿检测的状态或最后写入的状态
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \retval AM_TRUE  ：检测到上升沿
 * \retval AM_FALSE ：没有检测到上升沿或者上升沿被清除
 */
am_static_inline 
am_bool_t amhw_lpc82x_pint_rise_dec_get (amhw_lpc82x_pint_t *p_hw_pint,
                                         uint8_t             chan)
{
    return (p_hw_pint->rise & AM_BIT(chan)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除对应通道的上升沿检测
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_pint_rise_dec_clr (amhw_lpc82x_pint_t *p_hw_pint, 
                                    uint8_t             chan)
{
    p_hw_pint->rise = AM_BIT(chan);
}

/**
 * \brief 获取下降沿检测的状态或最后写入的状态
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \retval AM_TRUE  ：检测到下降沿
 * \retval AM_FALSE ：没有检测到下降沿或者下降沿被清除
 */
am_static_inline 
am_bool_t amhw_lpc82x_pint_fall_dec_get (amhw_lpc82x_pint_t *p_hw_pint,
                                         uint8_t             chan)
{
    return ((p_hw_pint->fall & AM_BIT(chan)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 清除对应通道的下降沿检测
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_pint_fall_dec_clr (amhw_lpc82x_pint_t *p_hw_pint, 
                                    uint8_t             chan)
{
    p_hw_pint->fall = AM_BIT(chan);
}

/**
 * \brief 获取通道中断状态
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \retval AM_TRUE  ：通道中断中断正在请求
 * \retval AM_FALSE ：没有中断请求
 */
am_static_inline 
am_bool_t amhw_lpc82x_pint_int_stat_get (amhw_lpc82x_pint_t *p_hw_pint,
                                         uint8_t             chan)
{
    return (p_hw_pint->ist & AM_BIT(chan)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 边沿触发中断发生时，调用此函数清除边沿检测
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \return 无
 *
 * \note 中断发生时，如果模式为边沿触发，需要调用该函数清除中断标志
 */
am_static_inline 
void amhw_lpc82x_pint_edge_dec_clr (amhw_lpc82x_pint_t *p_hw_pint, 
                                    uint8_t             chan)
{
    p_hw_pint->ist = AM_BIT(chan);
}


/**
 * \brief 电平触发中断发生时，调用此函数反转引脚的中断触发电平
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] chan      : 中断通道编号
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_pint_level_int_tog (amhw_lpc82x_pint_t *p_hw_pint, 
                                     uint8_t             chan)
{
    p_hw_pint->ist = AM_BIT(chan);
}


/**
 * \brief 设置中断模式为模式匹配引擎
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 *
 * \return 无
 *
 * \note 设置后引脚中断不能使用
 */
am_static_inline 
void amhw_lpc82x_pint_pmatch_mode_set (amhw_lpc82x_pint_t *p_hw_pint)
{
    AM_BIT_SET(p_hw_pint->pmctrl, 0);
}

/**
 * \brief 设置中断模式为引脚中断模式
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_pint_pin_int_mode_set (amhw_lpc82x_pint_t *p_hw_pint)
{
    AM_BIT_CLR(p_hw_pint->pmctrl, 0);
}

/**
 * \brief 使能模式匹配引擎"接收事件"输出
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_pint_pmatch_rxev_enable (amhw_lpc82x_pint_t *p_hw_pint)
{
    AM_BIT_SET(p_hw_pint->pmctrl, 1);
}

/**
 * \brief 禁能模式匹配引擎"接收事件"输出
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_pint_pmatch_rxev_disable (amhw_lpc82x_pint_t *p_hw_pint)
{
    AM_BIT_CLR(p_hw_pint->pmctrl, 1);
}

/**
 * \brief 获取位片输出状态
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] slice     : 位片编号
 *
 * \retval AM_TRUE  : 位片输出有效
 * \retval AM_FALSE : 位片输出无效
 */
am_static_inline 
am_bool_t amhw_lpc82x_pint_pmatch_get (amhw_lpc82x_pint_t          *p_hw_pint,
                                       amhw_lpc82x_pint_bitslice_t  slice)
{
    return (p_hw_pint->pmctrl & AM_BIT(slice + 24)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 配置位片参数
 *
 * \param[in] p_hw_pint : 指向引脚中断寄存器块的指针
 * \param[in] slice     : 位片编号
 * \param[in] chan      : 中断通道
 * \param[in] arg       : 位片参数
 * \param[in] is_end    : 如果该值为AM_TURE，当前位片即为最终构成
 *
 * \return 无
 */
void amhw_lpc82x_pint_pmatch_slice_cfg(amhw_lpc82x_pint_t           *p_hw_pint, 
                                       amhw_lpc82x_pint_bitslice_t   slice,
                                       uint8_t                       chan,
                                       amhw_lpc82x_pint_slice_arg_t  arg,
                     am_bool_t                     is_end);
/**
 * @}
 */
 
#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_PINT_H */

/* end of file */
