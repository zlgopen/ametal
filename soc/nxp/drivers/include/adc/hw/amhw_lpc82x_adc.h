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
 * \brief ADC 硬件操作接口描述
 *
 * 1. 12位逐次逼近型数模转换器；
 * 2. 模拟输入引脚多达12个；
 * 3. 2个独立触发的转换序列；
 * 4. 可选择高/低阈值比较和“零点穿越”监测；
 * 5. 支持掉电模式和低功耗模式；
 * 6. 电压测量范围从 Vrefn 到 Vrefp (典型值为 3V; 不可超出 VDD 的电压值)；
 * 7. 12位的转换速率高达 1.2 MHz采样速率；
 * 8. 突发转换模式支持单通道或多通道输入；
 * 9. 支持DMA模式；
 * 10. 硬件校正模式。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-25  hgo, modify macro definition.
 * - 1.00 15-07-15  bob, first implementation.
 * \endinternal
 */
#ifndef  __AMHW_LPC82X_ADC_H
#define  __AMHW_LPC82X_ADC_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_adc
 * \copydoc amhw_lpc82x_adc.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */
 
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
    
  /* 默认使能匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */


/**
 * \brief ADC 寄存器块结构体
 */
typedef struct amhw_lpc82x_adc {
    __IO uint32_t ctrl;                 /**< \brief ADC 控制寄存器          */
    __IO uint32_t reserved1;            /**< \brief 保留                    */
    union {
        __IO uint32_t seq_ctrl[2];      /**< \brief ADC 序列控制寄存器      */
        struct {
            __IO uint32_t seqa_ctrl;    /**< \brief ADC 序列A控制寄存器     */
            __IO uint32_t seqb_ctrl;    /**< \brief ADC 序列B控制寄存器     */
        };
    };
    union {
        __IO uint32_t seq_gdat[2];      /**< \brief ADC 序列全局数据寄存器  */
        struct {
            __IO uint32_t seqa_gdat;    /**< \brief ADC 序列A全局数据寄存器 */
            __IO uint32_t seqb_gdat;    /**< \brief ADC 序列B全局数据寄存器 */
        };
    };
    __IO uint32_t reserved2[2];         /**< \brief 保留                    */
    union {
        __I uint32_t dat[12];           /**< \brief ADC 通道数据寄存器      */
        struct {
            __I uint32_t dat0;          /**< \brief ADC 通道0数据寄存器     */
            __I uint32_t dat1;          /**< \brief ADC 通道1数据寄存器     */
            __I uint32_t dat2;          /**< \brief ADC 通道2数据寄存器     */
            __I uint32_t dat3;          /**< \brief ADC 通道3数据寄存器     */
            __I uint32_t dat4;          /**< \brief ADC 通道4数据寄存器     */
            __I uint32_t dat5;          /**< \brief ADC 通道5数据寄存器     */
            __I uint32_t dat6;          /**< \brief ADC 通道6数据寄存器     */
            __I uint32_t dat7;          /**< \brief ADC 通道7数据寄存器     */
            __I uint32_t dat8;          /**< \brief ADC 通道8数据寄存器     */
            __I uint32_t dat9;          /**< \brief ADC 通道9数据a寄存器    */
            __I uint32_t dat10;         /**< \brief ADC 通道10数据寄存器    */
            __I uint32_t dat11;         /**< \brief ADC 通道11数据寄存器    */
        };
    };
    union {
        __IO uint32_t thr_low[2];       /**< \brief ADC 低阈值寄存器        */
        struct {
            __IO uint32_t thr0_low;     /**< \brief ADC 低阈值寄存器 0      */
            __IO uint32_t thr1_low;     /**< \brief ADC 低阈值寄存器 1      */
        };
    };
    union {
        __IO uint32_t thr_high[2];      /**< \brief ADC 高阈值寄存器        */
        struct {
            __IO uint32_t thr0_high;    /**< \brief ADC 高阈值寄存器 0      */
            __IO uint32_t thr1_high;    /**< \brief ADC 高阈值寄存器 1      */
        };
    };
    __IO uint32_t chan_thrsel;          /**< \brief ADC 通道阈值选择寄存器  */
    __IO uint32_t inten;                /**< \brief ADC 中断使能寄存器      */
    __IO uint32_t flags;                /**< \brief ADC 中断请求标志寄存器  */
    __IO uint32_t trim;                 /**< \brief ADC 触发寄存器          */
} amhw_lpc82x_adc_t;


/**
 * \name ADC 通道编号
 * \anchor grp_amhw_lpc82x_adc_chan
 * @{
 */

#define AMHW_LPC82X_ADC_CHAN_0          0          /**< \brief ADC 通道 0   */
#define AMHW_LPC82X_ADC_CHAN_1          1          /**< \brief ADC 通道 1   */
#define AMHW_LPC82X_ADC_CHAN_2          2          /**< \brief ADC 通道 2   */
#define AMHW_LPC82X_ADC_CHAN_3          3          /**< \brief ADC 通道 3   */
#define AMHW_LPC82X_ADC_CHAN_4          4          /**< \brief ADC 通道 4   */
#define AMHW_LPC82X_ADC_CHAN_5          5          /**< \brief ADC 通道 5   */
#define AMHW_LPC82X_ADC_CHAN_6          6          /**< \brief ADC 通道 6   */
#define AMHW_LPC82X_ADC_CHAN_7          7          /**< \brief ADC 通道 7   */
#define AMHW_LPC82X_ADC_CHAN_8          8          /**< \brief ADC 通道 8   */
#define AMHW_LPC82X_ADC_CHAN_9          9          /**< \brief ADC 通道 9   */
#define AMHW_LPC82X_ADC_CHAN_10         10         /**< \brief ADC 通道 10  */
#define AMHW_LPC82X_ADC_CHAN_11         11         /**< \brief ADC 通道 11  */

/** @} */

#define AMHW_LPC82X_ADC_CHAN_CNT        12         /**< \brief ADC 通道数量 */

/**
 * \brief ADC 序列寄存器索引
 */
typedef enum amhw_lpc82x_adc_seq_index {
    AMHW_LPC82X_ADC_SEQ_A = 0,                     /**< \brief 序列A索引    */
    AMHW_LPC82X_ADC_SEQ_B = 1,                     /**< \brief 序列B索引    */
} amhw_lpc82x_adc_seq_index_t;


/**
 * \brief ADC 阈值比较索引
 */
typedef enum amhw_lpc82x_adc_comp_thre_index {
    AMHW_LPC82X_ADC_COMP_THRE_0 = 0,               /**< \brief 阈值比较0索引 */
    AMHW_LPC82X_ADC_COMP_THRE_1 = 1,               /**< \brief 阈值比较1索引 */
} amhw_lpc82x_adc_comp_thre_index_t;


/**
 * \name ADC 控制寄存器位域定义
 * \anchor grp_amhw_lpc82x_adc_ctrl
 * @{
 */
 
/** \brief 设置时钟分频值           */
#define AMHW_LPC82X_ADC_CTRL_CLK_DIV(x)  AM_SBF((x) & 0xFF, 0)

/** \brief 低功耗模式使能           */
#define AMHW_LPC82X_ADC_CTRL_LPWRMODE    AM_SBF(1UL, 10)

/** \brief 校正使能                 */
#define AMHW_LPC82X_ADC_CTRL_CALMODE     AM_SBF(1UL, 30)

/** @} */

/**
 * \name ADC 序列控制寄存器位域定义
 * \anchor grp_amhw_lpc82x_adc_seq_ctrl
 * @{
 */
 
/** \brief 触发选择为软件触发       */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_SW        AM_SBF(0UL, 12)

/** \brief 触发选择为ADC_PINTRG0    */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_PIN0      AM_SBF(1UL, 12)

/** \brief 触发选择为ADC_PINTRG1    */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_PIN1      AM_SBF(2UL, 12)

/** \brief 触发选择为SCT_OUT3       */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_SCT       AM_SBF(3UL, 12)

/** \brief 触发选择为模拟比较器输出 */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_ACMP      AM_SBF(4UL, 12)

/** \brief 触发选择为ARM_TXEV       */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_ARM_TXEV  AM_SBF(5UL, 12)

/** \brief 阴极触发                 */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_POL_NEG   AM_SBF(0UL, 18)

/** \brief 阳极触发                 */
#define AMHW_LPC82X_ADC_SEQ_CTRL_TRIG_POL_POS   AM_SBF(1UL, 18)

/** \brief 忽略同步触发             */
#define AMHW_LPC82X_ADC_SEQ_CTRL_BYPASS_SYNC    AM_SBF(1UL, 19)

/** \brief 使能突发转换模式         */
#define AMHW_LPC82X_ADC_SEQ_CTRL_MODE_BURST    (AM_SBF(1UL, 27) |\
                                                AM_SBF(0UL, 26))

/** \brief 使能单次转换模式         */
#define AMHW_LPC82X_ADC_SEQ_CTRL_MODE_SINGLE    AM_SBF(1UL, 28)

/** \brief 序列A高优先级            */
#define AMHW_LPC82X_ADC_SEQ_CTRL_SEQA_PIRO_HIGH AM_SBF(1UL, 29)

/** \brief 转换完成产生事件         */
#define AMHW_LPC82X_ADC_SEQ_CTRL_MODE_EOC       AM_SBF(0UL, 30)

/** \brief 序列转换完成产生事件     */
#define AMHW_LPC82X_ADC_SEQ_CTRL_MODE_EOS       AM_SBF(1UL, 30)

/** \brief 序列控制寄存器使能       */
#define AMHW_LPC82X_ADC_SEQ_CTRL_ENA            AM_SBF(1UL, 31)

/** \brief 使能ADC通道              */
#define AMHW_LPC82X_ADC_SEQ_CTRL_ENABLE_CH(ch)  AM_SBF(1UL, (ch) & 0xFF)

/** @} */


/**
 * \name 数据寄存器位域定义
 * \anchor grp_amhw_lpc82x_adc_data_reg
 * @{
 */

/** \brief 获取ADC转换值            */
#define AMHW_LPC82X_ADC_DR_RESULT(n)        AM_BITS_GET(n, 4, 12)

/** \brief 阈值穿越比较结果         */
#define AMHW_LPC82X_ADC_DR_THCMPRANGE(n)    AM_BITS_GET(n, 16, 2)

/** \brief 阈值范围比较结果         */
#define AMHW_LPC82X_ADC_DR_THCMPCROSS(n)    AM_BITS_GET(n, 18, 2)

/** \brief 当前转换的通道号         */
#define AMHW_LPC82X_ADC_DR_CHANNEL(n)       AM_BITS_GET(n, 26, 4)

/** \brief 溢出标志位               */
#define AMHW_LPC82X_ADC_DR_OVERRUN          AM_SBF(1UL, 30)

/** \brief 数据有效位               */
#define AMHW_LPC82X_ADC_DR_DATAVALID        AM_SBF(1UL, 31)

/** @} */


/**
 * \name ADC中断使能寄存器位域定义
 * \anchor grp_amhw_lpc82x_adc_inten
 * @{
 */

/** \brief 序列A中断使能位          */
#define AMHW_LPC82X_ADC_INTEN_SEQA_ENABLE       AM_SBF(1UL, 0)

/** \brief 序列B中断使能位          */
#define AMHW_LPC82X_ADC_INTEN_SEQB_ENABLE       AM_SBF(1UL, 1)

/** \brief 溢出中断使能位           */
#define AMHW_LPC82X_ADC_INTEN_OVRRUN_ENABLE     AM_SBF(1UL, 2)

/** \brief 禁能阈值比较中断         */
#define AMHW_LPC82X_ADC_INTEN_CMP_DISABLE(ch)   AM_SBF(0UL, AM_SBF(ch, 1) + 3)

/** \brief 使能外部阈值比较中断     */
#define AMHW_LPC82X_ADC_INTEN_CMP_OUTSIDETH(ch) AM_SBF(1UL, AM_SBF(ch, 1) + 3)

/** \brief 使能穿越阈值比较中断     */
#define AMHW_LPC82X_ADC_INTEN_CMP_CROSSTH(ch)   AM_SBF(2UL, AM_SBF(ch, 1) + 3)

/** \brief 禁能所有中断     */
#define AMHW_LPC82X_ADC_INTEN_ALL_DISABLE       (0)
/** @} */


/**
 * \name ADC 标志寄存器位域定义
 * \anchor grp_amhw_lpc82x_adc_flags
 * @{
 */

/** \brief 通道阈值比较标志         */
#define AMHW_LPC82X_ADC_FLAGS_THCMP_MASK(ch)    AM_SBF(1UL, ch)

/** \brief 通道溢出标志             */
#define AMHW_LPC82X_ADC_FLAGS_OVRRUN_MASK(ch)   AM_SBF(1UL, 12 + (ch))

/** \brief 序列A溢出标志            */
#define AMHW_LPC82X_ADC_FLAGS_SEQA_OVRRUN_MASK  AM_SBF(1UL, 24)

/** \brief 序列B溢出标志            */
#define AMHW_LPC82X_ADC_FLAGS_SEQB_OVRRUN_MASK  AM_SBF(1UL, 25)

/** \brief 序列A中断标志            */
#define AMHW_LPC82X_ADC_FLAGS_SEQA_INT_MASK     AM_SBF(1UL, 28)

/** \brief 序列B中断标志            */
#define AMHW_LPC82X_ADC_FLAGS_SEQB_INT_MASK     AM_SBF(1UL, 29)

/** \brief 阈值比较中断标志         */
#define AMHW_LPC82X_ADC_FLAGS_THCMP_INT_MASK    AM_SBF(1UL, 30)

/** \brief 溢出中断标志             */
#define AMHW_LPC82X_ADC_FLAGS_OVRRUN_INT_MASK   AM_SBF(1UL, 31)

/** @} */

/**
 * \name ADC 触发寄存器位域定义
 * \anchor grp_amhw_lpc82x_adc_trim
 * @{
 */

/** \brief 高电压触发（2.7~3.6V）   */
#define AMHW_LPC82X_ADC_TRIM_VRANGE_HIGHT   AM_SBF(0UL, 5)

/** \brief 低电压触发（2.4~2.7V）   */
#define AMHW_LPC82X_ADC_TRIM_VRANGE_LOW     AM_SBF(1UL, 5)

/** @} */


/**
 * \brief ADC 时钟分频设置
 *
 * \param[in] p_hw_adc       : 指向ADC寄存器块的指针
 * \param[in] clkdiv         : 分频系数(0~255)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_clkdiv_set (amhw_lpc82x_adc_t *p_hw_adc, uint8_t clkdiv)
{
    AM_BITS_SET(p_hw_adc->ctrl, 0, 8, clkdiv);
}

/**
 * \brief ADC 时钟分频获取
 *
 * \param[in] p_hw_adc       : 指向ADC寄存器块的指针
 *
 * \return ADC时钟分频系数
 */
am_static_inline
uint8_t amhw_lpc82x_adc_clkdiv_get (amhw_lpc82x_adc_t *p_hw_adc)
{
    return (p_hw_adc->ctrl & 0xFF);
}

/**
 * \brief ADC 校验
 *
 * \param[in] p_hw_adc       : 指向ADC寄存器块的指针
 * \param[in] sysclk_freq    : 系统时钟
 *
 * \return 无
 */
void amhw_lpc82x_adc_calibrate (amhw_lpc82x_adc_t *p_hw_adc,
                                uint32_t           sysclk_freq);

/**
 * \brief ADC 控制配置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] flags    : 功能配置参数，参见
 *                       \ref grp_amhw_lpc82x_adc_ctrl
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_config (amhw_lpc82x_adc_t *p_hw_adc, uint32_t flags)
{
    p_hw_adc->ctrl = flags;
}

/**
 * \brief 获取 ADC 控制寄存器状态 
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 *
 * \return 控制寄存器状态 
 */
am_static_inline
uint32_t amhw_lpc82x_adc_ctrl_get (amhw_lpc82x_adc_t *p_hw_adc)
{
    return p_hw_adc->ctrl;
}

/**
 * \brief ADC 转换序列配置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 * \param[in] flags    : 功能配置参数，参见
 *                       \ref grp_amhw_lpc82x_adc_seq_ctrl
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_seq_config (amhw_lpc82x_adc_t           *p_hw_adc,
                                 amhw_lpc82x_adc_seq_index_t  index,
                                 uint32_t                     flags)
{
    p_hw_adc->seq_ctrl[index] = flags;
}

/**
 * \brief 获取 ADC 序列转换配置状态
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 *
 * \return ADC 序列转换配置状态
 */
am_static_inline
uint32_t amhw_lpc82x_adc_seq_ctrl_get (amhw_lpc82x_adc_t           *p_hw_adc,
                                       amhw_lpc82x_adc_seq_index_t  index)
{
    return p_hw_adc->seq_ctrl[index];
}

/**
 * \brief 启动ADC序列转换
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_seq_start (amhw_lpc82x_adc_t           *p_hw_adc,
                                amhw_lpc82x_adc_seq_index_t  index)
{
    AM_BIT_SET(p_hw_adc->seq_ctrl[index], 26);
}

/**
 * \brief 停止ADC序列转换
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_seq_stop (amhw_lpc82x_adc_t           *p_hw_adc,
                               amhw_lpc82x_adc_seq_index_t  index)
{
    AM_BIT_CLR(p_hw_adc->seq_ctrl[index], 26);
}

/**
 * \brief 使能ADC转换序列
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_seq_enable (amhw_lpc82x_adc_t           *p_hw_adc,
                                 amhw_lpc82x_adc_seq_index_t  index)
{
    AM_BIT_SET(p_hw_adc->seq_ctrl[index], 31);
}

/**
 * \brief 禁能ADC转换序列
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_seq_disable (amhw_lpc82x_adc_t           *p_hw_adc,
                                  amhw_lpc82x_adc_seq_index_t  index)
{
    AM_BIT_CLR(p_hw_adc->seq_ctrl[index], 31);
}

/**
 * \brief ADC转换序列使能和启动
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 *
 * \return 无
 */
am_static_inline  
void amhw_lpc82x_adc_seq_start_and_enable (amhw_lpc82x_adc_t           *p_hw_adc,
                                           amhw_lpc82x_adc_seq_index_t  index)
{
    AM_BIT_SET_MASK(p_hw_adc->seq_ctrl[index] , AM_BIT(31) | AM_BIT(26));
}

/**
 * \brief 使能ADC转换序列的通道
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 * \param[in] chan     : ADC 通道, 参见
 *                       \ref grp_amhw_lpc82x_adc_chan
 * \return 无
 */
am_static_inline  
void amhw_lpc82x_adc_seq_chan_set (amhw_lpc82x_adc_t           *p_hw_adc,
                                   amhw_lpc82x_adc_seq_index_t  index,
                                   int                          chan)
{
    AM_BIT_CLR     (p_hw_adc->seq_ctrl[index], 31);
    AM_BIT_SET_MASK(p_hw_adc->seq_ctrl[index], AM_BIT(31) | AM_BIT(chan));
}

/**
 * \brief 从通道数据寄存器获取ADC转换值
 *
 * 返回值是 \ref grp_amhw_lpc82x_adc_data_reg 中的 'n'
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] chan     : ADC 通道, 参见
 *                       \ref grp_amhw_lpc82x_adc_chan
 *
 * \return ADC转换值
 */
am_static_inline
uint32_t amhw_lpc82x_adc_chan_data_get (amhw_lpc82x_adc_t *p_hw_adc, int chan)
{
    return p_hw_adc->dat[chan];
}

/**
 * \brief 从全局数据寄存器获取ADC转换值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC转换序列，
 *                       (#AMHW_LPC82X_ADC_SEQ_A) 或 (#AMHW_LPC82X_ADC_SEQ_B)
 *
 * \return ADC转换值
 */
am_static_inline
uint32_t amhw_lpc82x_adc_glo_data_get (amhw_lpc82x_adc_t           *p_hw_adc,
                                       amhw_lpc82x_adc_seq_index_t  index)
{
    return p_hw_adc->seq_gdat[index];
}

/**
 * \brief 设置ADC低比较阈值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC阈值比较，
 *                       (#AMHW_LPC82X_ADC_COMP_THRE_0) 或
 *                       (#AMHW_LPC82X_ADC_COMP_THRE_1)
 * \param[in] value    : 设置的低阈值
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_adc_thr_low_set (amhw_lpc82x_adc_t                 *p_hw_adc,
                                  amhw_lpc82x_adc_comp_thre_index_t  index,
                                  uint32_t                           value)
{
    p_hw_adc->thr_low[index] = AM_SBF(value & 0xFFF, 4); 
}

/**
 * \brief 设置ADC高比较阈值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 选择ADC阈值比较，
 *                       (#AMHW_LPC82X_ADC_COMP_THRE_0) 或
 *                       (#AMHW_LPC82X_ADC_COMP_THRE_1)
 * \param[in] value    : 设置的高阈值
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_adc_thr_high_set (amhw_lpc82x_adc_t                 *p_hw_adc,
                                   amhw_lpc82x_adc_comp_thre_index_t  index,
                                   uint32_t                           value)
{
    p_hw_adc->thr_high[index] = AM_SBF(value & 0xFFF, 4);
}

/**
 * \brief 阈值比较0选择通道号
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] chan     : ADC 通道, 参见
 *                       \ref grp_amhw_lpc82x_adc_chan
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_adc_thr0_sel (amhw_lpc82x_adc_t *p_hw_adc, int chan)
{ 
    AM_BIT_CLR(p_hw_adc->chan_thrsel, chan);
}

/**
 * \brief 阈值比较1选择通道号
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] chan     : ADC 通道, 参见
 *                       \ref grp_amhw_lpc82x_adc_chan
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_adc_thr1_sel (amhw_lpc82x_adc_t *p_hw_adc, int chan)
{
    AM_BIT_SET(p_hw_adc->chan_thrsel, chan);
}

/**
 * \brief ADC 中断使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] flags    : 中断配置参数，参见
 *                       \ref grp_amhw_lpc82x_adc_inten
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_adc_int_enable (amhw_lpc82x_adc_t *p_hw_adc, uint32_t flags)
{
    p_hw_adc->inten = flags;
}

/**
 * \brief ADC 中断禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] flags    : 中断配置参数，参见
 *                       \ref grp_amhw_lpc82x_adc_inten
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_adc_int_disable (amhw_lpc82x_adc_t *p_hw_adc, uint32_t flags)
{
    p_hw_adc->inten &= ~flags;
}

/**
 * \brief 获取中断标志
 * 
 * 和 \ref grp_amhw_lpc82x_adc_flags 的参数进行与操作获取具体状态
 *
 * \par 示例
 * \code 
 * if (amhw_lpc82x_adc_flags_get(p_hw_adc)&AMHW_LPC82X_ADC_FLAGS_SEQA_INT_MASK){
 * // do something.
 * }
 * \endcode
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 *
 * \return 中断标志.
 */
am_static_inline 
uint32_t amhw_lpc82x_adc_flags_get (amhw_lpc82x_adc_t *p_hw_adc)
{
    return p_hw_adc->flags;
}

/**
 * \brief 清除中断标志
 * 
 * 和 \ref grp_amhw_lpc82x_adc_flags 的参数进行与操作获取具体状态
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] flags    : 中断标志参数, 参见 \ref grp_amhw_lpc82x_adc_flags
 * 
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_adc_flags_clr (amhw_lpc82x_adc_t *p_hw_adc, uint32_t flags)
{
    p_hw_adc->flags = flags;
}

/**
 * \brief ADC 触发电压选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] flag     : 触发电压参数
 *                       \ref grp_amhw_lpc82x_adc_inten
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_adc_trim_voltage_set (amhw_lpc82x_adc_t *p_hw_adc,
                                       uint32_t           flag)
{
    p_hw_adc->trim = flag;
}


/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
 
  /* 允许匿名联合体使能 */
#elif defined(__GNUC__)

  /* 默认使用匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

/** @} */


/**
 *@} 
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMHW_LPC82X_ADC_H */

/* end of file */
