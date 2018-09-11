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
 * \brief ADC 硬件操作接口
 *
 * 1. 16位逐次逼近型数模转换器；
 * 2. 高达4对差分输入，24个单端模拟输入；
 * 3. 单次或连续转换，转换完成自动范围空闲状态；
 * 4. 可配置采样时间，转换速度或功耗；
 * 5. 可选择高/低阈值比较和硬件平均；
 * 6. 转换完成或硬件平均完成中断请求；
 * 7. 四个可选时钟源；
 * 8. 自带温度传感器；
 * 9. 可选的参考电压源；
 * 10. 硬件校正模式;
 * 11. 2个独立触发的转换序列；
 * 12. 支持输出模式有：
 *     ---16位、13位、11位、9位差分输入模式
 *     ---16位、12位、10位、8位单端输入模式
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-14  mkr, first implementation.
 * \endinternal
 */
#ifndef  __AMHW_FSL_ADC_H
#define  __AMHW_FSL_ADC_H

#ifdef __cplusplus
extern "C" {
#endif
#include "am_types.h"
#include "am_clk.h"

/**
 * \addtogroup amhw_fsl_if_adc
 * \copydoc amhw_fsl_adc.h
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
typedef struct amhw_fsl_adc {
    union {
        __IO uint32_t sc1[2];      /**< \brief ADC 状态控制寄存器1      */
        struct {
            __IO uint32_t sc1_a;   /**< \brief ADC 序列A控制寄存器     */
            __IO uint32_t sc1_b;   /**< \brief ADC 序列B控制寄存器     */
        };
    };
    __IO uint32_t cfg1;           /**< \brief ADC 配置寄存器1     */
    __IO uint32_t cfg2;           /**< \brief ADC 配置寄存器2     */

    union {
        __I uint32_t dat[2];       /**< \brief ADC 数据寄存器      */
        struct {
            __I uint32_t dat_a;    /**< \brief ADC 序列A数据寄存器     */
            __I uint32_t dat_b;    /**< \brief ADC 序列B数据寄存器2     */
        };
    };
    union {
        __IO uint32_t cv[2];      /**< \brief ADC 阈值寄存器      */
        struct {
            __IO uint32_t cv_1;    /**< \brief ADC 阈值1寄存器     */
            __IO uint32_t cv_2;    /**< \brief ADC 阈值2寄存器     */
        };
    };
    __IO uint32_t sc2;      /**< \brief ADC 状态控制寄存器2      */
    __IO uint32_t sc3;      /**< \brief ADC 状态控制寄存器3      */

    __IO uint32_t ofs;      /**< \brief ADC 偏移矫正寄存器      */
    __IO uint32_t pg;      /**< \brief ADC Plus-Side增益寄存器      */
    __IO uint32_t mg;      /**< \brief ADC Minus-Side控制寄存器      */

    union {
        __IO uint32_t clp[7];      /**< \brief ADC Plus-Side矫正值寄存器  */
        struct {
            __IO uint32_t clp_d;    /**< \brief ADC Plus-Side矫正值寄存器D */
            __IO uint32_t clp_s;    /**< \brief ADC Plus-Side矫正值寄存器S */
            __IO uint32_t clp_4;    /**< \brief ADC Plus-Side矫正值寄存器4 */
            __IO uint32_t clp_3;    /**< \brief ADC Plus-Side矫正值寄存器3 */
            __IO uint32_t clp_2;    /**< \brief ADC Plus-Side矫正值寄存器2 */
            __IO uint32_t clp_1;    /**< \brief ADC Plus-Side矫正值寄存器1 */
            __IO uint32_t clp_0;    /**< \brief ADC Plus-Side矫正值寄存器0 */
        };
    };
    union {
        __IO uint32_t clm[7];      /**< \brief ADC Minus-Side矫正值寄存器  */
        struct {
            __IO uint32_t clm_d;    /**< \brief ADC Minus-Side矫正值寄存器D */
            __IO uint32_t clm_s;    /**< \brief ADC Minus-Side矫正值寄存器S */
            __IO uint32_t clm_4;    /**< \brief ADC Minus-Side矫正值寄存器4 */
            __IO uint32_t clm_3;    /**< \brief ADC Minus-Side矫正值寄存器3 */
            __IO uint32_t clm_2;    /**< \brief ADC Minus-Side矫正值寄存器2 */
            __IO uint32_t clm_1;    /**< \brief ADC Minus-Side矫正值寄存器1 */
            __IO uint32_t clm_0;    /**< \brief ADC Minus-Side矫正值寄存器0 */
        };
    };
} amhw_fsl_adc_t;


/**
 * \brief ADC 序列寄存器索引
 * \anchor grp_amhw_adc_chan
 */
typedef enum amhw_fsl_adc_chan {
    AMHW_FSL_ADC_CHAN_0       = 0 ,         /**< \brief ADC 通道 0   */
    AMHW_FSL_ADC_CHAN_1       = 1 ,         /**< \brief ADC 通道 1   */
    AMHW_FSL_ADC_CHAN_2       = 2 ,         /**< \brief ADC 通道 2   */
    AMHW_FSL_ADC_CHAN_3       = 3 ,         /**< \brief ADC 通道 3   */
    AMHW_FSL_ADC_CHAN_4       = 4 ,         /**< \brief ADC 通道 4   */
    AMHW_FSL_ADC_CHAN_5       = 5 ,         /**< \brief ADC 通道 5   */
    AMHW_FSL_ADC_CHAN_6       = 6 ,         /**< \brief ADC 通道 6   */
    AMHW_FSL_ADC_CHAN_7       = 7 ,         /**< \brief ADC 通道 7   */
    AMHW_FSL_ADC_CHAN_8       = 8 ,         /**< \brief ADC 通道 8   */
    AMHW_FSL_ADC_CHAN_9       = 9 ,         /**< \brief ADC 通道 9   */
    AMHW_FSL_ADC_CHAN_10      = 10,         /**< \brief ADC 通道 10  */
    AMHW_FSL_ADC_CHAN_11      = 11,         /**< \brief ADC 通道 11  */
    AMHW_FSL_ADC_CHAN_12      = 12,         /**< \brief ADC 通道 12  */
    AMHW_FSL_ADC_CHAN_13      = 13,         /**< \brief ADC 通道 13  */
    AMHW_FSL_ADC_CHAN_14      = 14,         /**< \brief ADC 通道 14  */
    AMHW_FSL_ADC_CHAN_15      = 15,         /**< \brief ADC 通道 15  */
    AMHW_FSL_ADC_CHAN_16      = 16,         /**< \brief ADC 通道 16  */
    AMHW_FSL_ADC_CHAN_17      = 17,         /**< \brief ADC 通道 17  */
    AMHW_FSL_ADC_CHAN_18      = 18,         /**< \brief ADC 通道 18  */
    AMHW_FSL_ADC_CHAN_19      = 19,         /**< \brief ADC 通道 19  */
    AMHW_FSL_ADC_CHAN_20      = 20,         /**< \brief ADC 通道 20  */
    AMHW_FSL_ADC_CHAN_21      = 21,         /**< \brief ADC 通道 21  */
    AMHW_FSL_ADC_CHAN_22      = 22,         /**< \brief ADC 通道 22  */
    AMHW_FSL_ADC_CHAN_23      = 23,         /**< \brief ADC 通道 23  */
    AMHW_FSL_ADC_CHAN_TEMP    = 26,         /**< \brief ADC 通道 11  */
    AMHW_FSL_ADC_CHAN_BANDGAP = 27,         /**< \brief ADC 通道数量 */
    AMHW_FSL_ADC_CHAN_VREFSH  = 29,         /**< \brief ADC 通道数量 */
    AMHW_FSL_ADC_CHAN_VREFSL  = 30,         /**< \brief ADC 通道数量 */
    AMHW_FSL_ADC_CHAN_CLOSE   = 31,         /**< \brief ADC 通道数量 */
} amhw_fsl_adc_chan_t;
/**
 * \brief ADC 序列寄存器索引
 */
typedef enum amhw_fsl_adc_seq_index {
    AMHW_FSL_ADC_SEQ_A = 0,                      /**< \brief 序列A索引    */
    AMHW_FSL_ADC_SEQ_B = 1,                      /**< \brief 序列B索引    */
} amhw_fsl_adc_seq_index_t;


/**
 * \name ADC 控制寄存器位域定义
 * \anchor grp_amhw_fsl_adc_sc1
 * @{
 */
#define AMHW_FSL_ADC_SC1_COCO_MASK   (1UL << 7)      /**< \brief 转换完成标志掩码 */
#define AMHW_FSL_ADC_SC1_INT_MASK    (1UL << 6)      /**< \brief 中断使能掩码 */
#define AMHW_FSL_ADC_SC1_DIFF_MASK   (1UL << 5)      /**< \brief 差分输入使能掩码 */
#define AMHW_FSL_ADC_SC1_CHAN_MASK   (0x1FUL)        /**< \brief 通道掩码 */

#define AMHW_FSL_ADC_SC1_INT_EN_CFG  (1UL << 6)      /**< \brief 中断使能配置 */
#define AMHW_FSL_ADC_SC1_DIFF_EN_CFG (1UL << 5)      /**< \brief 差分输入使能配置 */
/** \brief 通道选择配置 */
#define AMHW_FSL_ADC_SC1_CHAN_SEL_CFG(x) ((x) & AMHW_FSL_ADC_SC1_CHAN_MASK)
/** @} */

/**
 * \brief 使能ADC转换完成中断
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_int_enable (amhw_fsl_adc_t           *p_hw_adc,
                               amhw_fsl_adc_seq_index_t  index)
{
    p_hw_adc->sc1[index] |= AMHW_FSL_ADC_SC1_INT_EN_CFG;
}

/**
 * \brief 禁能ADC转换完成中断
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_int_disable (amhw_fsl_adc_t           *p_hw_adc,
                                amhw_fsl_adc_seq_index_t  index)
{
    p_hw_adc->sc1[index] &= ~AMHW_FSL_ADC_SC1_INT_EN_CFG;
}
/**
 * \brief 使能差分输入
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_diff_enable (amhw_fsl_adc_t           *p_hw_adc,
                                amhw_fsl_adc_seq_index_t  index)
{
    p_hw_adc->sc1[index] |= AMHW_FSL_ADC_SC1_DIFF_EN_CFG;
}

/**
 * \brief 禁能差分输入，使用单端输入
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_diff_disable (amhw_fsl_adc_t           *p_hw_adc,
                                 amhw_fsl_adc_seq_index_t  index)
{
    p_hw_adc->sc1[index] &= ~AMHW_FSL_ADC_SC1_DIFF_EN_CFG;
}

/**
 * \brief 设置通道
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \param[in] chan     : ADC通道 ， /ref grp_amhw_fsl_adc_chan
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_chan_set (amhw_fsl_adc_t          *p_hw_adc,
                             amhw_fsl_adc_seq_index_t index,
                             amhw_fsl_adc_chan_t      chan)
{
    p_hw_adc->sc1[index] &= ~AMHW_FSL_ADC_SC1_CHAN_MASK;
    p_hw_adc->sc1[index] |= chan;
}

/**
 * \brief 获取通道
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \return ADC通道 /ref grp_amhw_fsl_adc_chan
 */
am_static_inline
uint32_t amhw_fsl_adc_chan_get (amhw_fsl_adc_t          *p_hw_adc,
                                 amhw_fsl_adc_seq_index_t index)
{
    return p_hw_adc->sc1[index] & AMHW_FSL_ADC_SC1_CHAN_MASK;
}

/**
 * \brief 获取ADC转换完成标志位
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 *
 * \retval AM_TRUE  : 转换完成标志位置位
 * \retval AM_FALSE : 转换完成标志位未置位
 *
 */
am_static_inline
uint32_t amhw_fsl_adc_complete_is (amhw_fsl_adc_t          *p_hw_adc,
                                    amhw_fsl_adc_seq_index_t index)
{
    return (p_hw_adc->sc1[index] & AMHW_FSL_ADC_SC1_COCO_MASK) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 配置状态控制寄存器1
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \param[in] cfg      : 配置参数 ， /ref grp_amhw_fsl_adc_sc1
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_sc1_cfg (amhw_fsl_adc_t          *p_hw_adc,
                            amhw_fsl_adc_seq_index_t index,
                            uint32_t                  cfg)
{
    p_hw_adc->sc1[index] = cfg;
}

/**
 * \brief 获取配置状态控制寄存器1
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \return  配置状态控制寄存器1
 */
am_static_inline
uint32_t amhw_fsl_adc_sc1_get (amhw_fsl_adc_t          *p_hw_adc,
                                amhw_fsl_adc_seq_index_t index)
{
    return p_hw_adc->sc1[index] ;
}
/**
 * \brief ADC 时钟分频系数
 */
typedef enum amhw_fsl_adc_clk_div {
    AMHW_FSL_ADC_CLK_DIV_1 = 0,                      /**< \brief 分频系数为1    */
    AMHW_FSL_ADC_CLK_DIV_2 = 1,                      /**< \brief 分频系数为2    */
    AMHW_FSL_ADC_CLK_DIV_4 = 2,                      /**< \brief 分频系数为4    */
    AMHW_FSL_ADC_CLK_DIV_8 = 3,                      /**< \brief 分频系数为8    */
} amhw_fsl_adc_clk_div_t;

/**
 * \brief ADC 时钟源
 */
typedef enum amhw_fsl_adc_clk_src {
    AMHW_FSL_ADC_CLK_SRC_BUS     = 0,            /**< \brief bus总线时钟    */
    AMHW_FSL_ADC_CLK_SRC_ALTCLK2 = 1,            /**< \brief bus/2    */
    AMHW_FSL_ADC_CLK_SRC_ALTCLK  = 2,            /**< \brief 可选时钟    */
    AMHW_FSL_ADC_CLK_SRC_ADACK   = 3,            /**< \brief 异步时钟    */
} amhw_fsl_adc_clk_src_t;

/**
 * \name ADC 分辨率定义
 * \anchor grp_amhw_fsl_adc_resolution
 * @{
 */
#define AMHW_FSL_ADC_BITS_8    0                      /**< \brief 8位    */
#define AMHW_FSL_ADC_BITS_9    0                      /**< \brief 9位    */
#define AMHW_FSL_ADC_BITS_10   2                      /**< \brief 10位    */
#define AMHW_FSL_ADC_BITS_11   2                      /**< \brief 11位    */
#define AMHW_FSL_ADC_BITS_12   1                      /**< \brief 12位    */
#define AMHW_FSL_ADC_BITS_13   1                      /**< \brief 13位    */
#define AMHW_FSL_ADC_BITS_16   3                      /**< \brief 16位    */
/** @} */

/**
 * \name ADC 配置寄存器1位域定义
 * \anchor grp_amhw_fsl_adc_cfg1
 * @{
 */
#define AMHW_FSL_ADC_CFG1_LPC_MASK   (1UL << 7)      /**< \brief 低功耗使能掩码 */
#define AMHW_FSL_ADC_CFG1_DIV_MASK   (3UL << 5)      /**< \brief 时钟分频掩码 */
#define AMHW_FSL_ADC_CFG1_LSMP_MASK  (1UL << 4)      /**< \brief 长采样时间掩码 */
#define AMHW_FSL_ADC_CFG1_MODE_MASK  (3UL << 2)      /**< \brief 分辨率掩码 */
#define AMHW_FSL_ADC_CFG1_ICLK_MASK  (3UL << 0)      /**< \brief 输入时钟掩码 */

#define AMHW_FSL_ADC_CFG1_LPC_EN_CFG  (1UL << 7)      /**< \brief 低功耗使能配置 */
/** \brief 分频系数配置             */
#define AMHW_FSL_ADC_CFG1_DIV_CFG(x)  (((x) << 5) & AMHW_FSL_ADC_CFG1_DIV_MASK)
/** \brief 长采样时间使能配置 */
#define AMHW_FSL_ADC_CFG1_LSMP_EN_CFG (1UL << 4)
/** \brief 分辨率配置                 */
#define AMHW_FSL_ADC_CFG1_MODE_CFG(x) (((x) << 2) & AMHW_FSL_ADC_CFG1_MODE_MASK)
/** \brief 输入时钟配置              */
#define AMHW_FSL_ADC_CFG1_ICLK_CFG(x) (((x) << 0) & AMHW_FSL_ADC_CFG1_ICLK_MASK)
/** @} */

/**
 * \brief 使能低功耗模式
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_lowpower_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg1 |= AMHW_FSL_ADC_CFG1_LPC_EN_CFG;
}

/**
 * \brief 禁能低功耗模式，进入正常功耗模式
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_lowpower_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg1 &= ~AMHW_FSL_ADC_CFG1_LPC_EN_CFG;
}

/**
 * \brief 设置时钟分频系数
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] div      : 时钟分频系数
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_clkdiv_set (amhw_fsl_adc_t          *p_hw_adc,
                               amhw_fsl_adc_clk_div_t   div)
{
    p_hw_adc->cfg1 &= ~AMHW_FSL_ADC_CFG1_DIV_MASK;
    p_hw_adc->cfg1 |= AMHW_FSL_ADC_CFG1_DIV_CFG(div);
}

/**
 * \brief 获取时钟分频系数
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 时钟分频系数
 */
am_static_inline
uint32_t amhw_fsl_adc_clkdiv_get (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->cfg1 & AMHW_FSL_ADC_CFG1_DIV_MASK) >> 5);
}

/**
 * \brief  使能长采样时间
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_lsmp_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg1 |= AMHW_FSL_ADC_CFG1_LSMP_MASK;
}

/**
 * \brief 禁能长采样时间，进入短采样时间模式
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_lsmp_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg1 &= ~AMHW_FSL_ADC_CFG1_LSMP_MASK;
}

/**
 * \brief 设置分辨率模式
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] res      : 分辨率，/ref grp_amhw_fsl_adc_resolution
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_mode_set (amhw_fsl_adc_t *p_hw_adc, uint32_t res)
{
    p_hw_adc->cfg1 &= ~AMHW_FSL_ADC_CFG1_MODE_MASK;
    p_hw_adc->cfg1 |= AMHW_FSL_ADC_CFG1_MODE_CFG(res);
}

/**
 * \brief 获取分辨率模式
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 分辨率，/ref grp_amhw_fsl_adc_resolution
 */
am_static_inline
uint32_t amhw_fsl_adc_mode_get (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->cfg1 & AMHW_FSL_ADC_CFG1_MODE_MASK) >> 2);
}

/**
 * \brief 设置模块时钟源
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] src      : 时钟源
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_clksrc_set (amhw_fsl_adc_t         *p_hw_adc,
                               amhw_fsl_adc_clk_src_t  src)
{
    p_hw_adc->cfg1 &= ~AMHW_FSL_ADC_CFG1_ICLK_MASK;
    p_hw_adc->cfg1 |= AMHW_FSL_ADC_CFG1_ICLK_CFG(src);
}

/**
 * \brief 获取模块时钟源
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 时钟源
 */
am_static_inline
uint32_t amhw_fsl_adc_clksrc_get (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->cfg1 & AMHW_FSL_ADC_CFG1_ICLK_MASK));
}

/**
 * \brief 设置配置寄存器1
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] cfg      : 配置参数 ， /ref grp_amhw_fsl_adc_cfg1
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cfg1_cfg (amhw_fsl_adc_t *p_hw_adc, uint32_t cfg)
{
    p_hw_adc->cfg1 = cfg;
}

/**
 * \brief 获取配置寄存器1
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 配置寄存器1
 */
am_static_inline
uint32_t amhw_fsl_adc_cfg1_get (amhw_fsl_adc_t *p_hw_adc)
{
    return p_hw_adc->cfg1;
}

/**
 * \brief ADC 长采样时间选择
 */
typedef enum amhw_fsl_adc_lsts {
    AMHW_FSL_ADC_LSTS_20ADCK  = 0,            /**< \brief 增加20个ADCK时钟    */
    AMHW_FSL_ADC_LSTS_12ADCK  = 1,            /**< \brief 增加12个ADCK时钟    */
    AMHW_FSL_ADC_LSTS_6ADCK   = 2,            /**< \brief 增加6个ADCK时钟    */
    AMHW_FSL_ADC_LSTS_2ADCK   = 3,            /**< \brief 增加2个ADCK时钟    */
} amhw_fsl_adc_lsts_t;

/**
 * \brief 通道a/b选择
 */
typedef enum amhw_fsl_adc_muxsel {
    AMHW_FSL_ADC_MUXSEL_A = 0,               /**< \brief 选择ADxxa通道 */
    AMHW_FSL_ADC_MUXSEL_B = 1,               /**< \brief 选择ADxxb通道 */
} amhw_fsl_adc_muxsel_t;

/**
 * \name ADC 配置寄存器1位域定义
 * \anchor grp_amhw_fsl_adc_cfg2
 * @{
 */
#define AMHW_FSL_ADC_CFG2_MUXSEL_MASK  (1UL << 4)  /**< \brief 序列使能掩码 */
#define AMHW_FSL_ADC_CFG2_ACK_MASK     (1UL << 3)  /**< \brief 异步时钟输出使能掩码 */
#define AMHW_FSL_ADC_CFG2_HSC_MASK     (1UL << 2)  /**< \brief 高速转换配置掩码 */
#define AMHW_FSL_ADC_CFG2_LSTS_MASK    (3UL << 0)  /**< \brief 长采样时间配置掩码 */


#define AMHW_FSL_ADC_CFG2_MUXSEL_CFG(x)  ((x) << 4) /**< \brief 序列B使能配置 */
#define AMHW_FSL_ADC_CFG2_ACK_EN_CFG     (1UL << 3) /**< \brief 异步时钟输出使能配置 */
#define AMHW_FSL_ADC_CFG2_HSC_EN_CFG     (1UL << 2) /**< \brief 高速转换使能配置 */
/** \brief 长采样时间选择配置 */
#define AMHW_FSL_ADC_CFG2_LSTS_CFG(x)    ((x) & AMHW_FSL_ADC_CFG2_LSTS_MASK)
/** @} */

/**
 * \brief 设置选择通道a/b
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_seq_set (amhw_fsl_adc_t        *p_hw_adc,
                            amhw_fsl_adc_muxsel_t  index)
{
    p_hw_adc->cfg2 &= ~AMHW_FSL_ADC_CFG2_MUXSEL_MASK;
    p_hw_adc->cfg2 |= AMHW_FSL_ADC_CFG2_MUXSEL_CFG(index);
}

/**
 * \brief 获取生效的序列通道
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return  使用的通道号，a或者b
 */
am_static_inline
uint32_t amhw_fsl_adc_seq_get (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->cfg2 & AMHW_FSL_ADC_CFG2_MUXSEL_MASK) >> 4) ;
}

/**
 * \brief  使能异步时钟输出
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_ack_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg2 |= AMHW_FSL_ADC_CFG2_ACK_MASK;
}

/**
 * \brief 禁能异步时钟输出
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_ack_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg2 &= ~AMHW_FSL_ADC_CFG2_ACK_MASK;
}
/**
 * \brief  使能高速转换
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_hsc_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg2 |= AMHW_FSL_ADC_CFG2_HSC_MASK;
}

/**
 * \brief 禁能高速转换
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_hsc_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->cfg2 &= ~AMHW_FSL_ADC_CFG2_HSC_MASK;
}

/**
 * \brief 设置长采样时间
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] time     : 延时参数
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_lsts_set (amhw_fsl_adc_t *p_hw_adc, amhw_fsl_adc_lsts_t time)
{
    p_hw_adc->cfg2 &= ~AMHW_FSL_ADC_CFG2_LSTS_MASK;
    p_hw_adc->cfg2 |= AMHW_FSL_ADC_CFG2_LSTS_CFG(time);
}

/**
 * \brief 获取长采样时间
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return  序列索引
 */
am_static_inline
uint32_t amhw_fsl_adc_lsts_get (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->cfg2 & AMHW_FSL_ADC_CFG2_LSTS_MASK));
}

/**
 * \brief 配置配置寄存器2
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] cfg      : 配置参数 ， /ref grp_amhw_fsl_adc_cfg2
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cfg2_cfg (amhw_fsl_adc_t *p_hw_adc, uint32_t cfg)
{
    p_hw_adc->cfg2 = cfg;
}

/**
 * \brief 获取配置寄存器2
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 配置寄存器2
 */
am_static_inline
uint32_t amhw_fsl_adc_cfg2_get (amhw_fsl_adc_t *p_hw_adc)
{
    return p_hw_adc->cfg2;
}

/**
 * \brief 获取数据寄存器数值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 序列索引
 *
 * \return ADC转换后数据
 *
 * \note 非差分模式下数据为右对齐
 */
am_static_inline
uint16_t amhw_fsl_adc_dat_get (amhw_fsl_adc_t          *p_hw_adc,
                                amhw_fsl_adc_seq_index_t index)
{
    return (uint16_t)(p_hw_adc->dat[index] & 0xffff);
}

/**
 * \brief 设置阈值1寄存器内容
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] dat      : 配置内容
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cv1_set (amhw_fsl_adc_t *p_hw_adc, uint16_t dat)
{
    p_hw_adc->cv_1 = dat;
}

/**
 * \brief 获取阈值1寄存器内容
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
uint16_t amhw_fsl_adc_cv1_get (amhw_fsl_adc_t *p_hw_adc)
{
    return (uint16_t)(p_hw_adc->cv_1 & 0xffff);
}

/**
 * \brief 设置阈值2寄存器内容
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] dat      : 配置内容
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cv2_set (amhw_fsl_adc_t *p_hw_adc, uint16_t dat)
{
    p_hw_adc->cv_2 = dat;
}

/**
 * \brief 获取阈值2寄存器内容
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
uint16_t amhw_fsl_adc_cv2_get (amhw_fsl_adc_t *p_hw_adc)
{
    return (uint16_t)(p_hw_adc->cv_2 & 0xffff);
}


/**
 * \brief ADC 长采样时间选择
 */
typedef enum amhw_fsl_adc_ref {
    AMHW_FSL_ADC_REF_VREFHL = 0,     /**< \brief VREFH、VREFL做参考电压 */
    AMHW_FSL_ADC_REF_ALTHL  = 1,     /**< \brief VALTH、VALTL做参考电压    */
} amhw_fsl_adc_ref_t;

/**
 * \name ADC 状态控制寄存器2位域定义
 * \anchor grp_amhw_fsl_adc_sc2
 * @{
 */
#define AMHW_FSL_ADC_SC2_ACT_MASK      (1UL << 7)   /**< \brief 正在转换标志掩码 */
#define AMHW_FSL_ADC_SC2_HWTRG_MASK    (1UL << 6)   /**< \brief 触发方式标志掩码 */
#define AMHW_FSL_ADC_SC2_CMP_MASK      (1UL << 5)   /**< \brief 比较功能标志掩码 */
#define AMHW_FSL_ADC_SC2_CMPGT_MASK    (1UL << 4)   /**< \brief 比较功能大于掩码 */
#define AMHW_FSL_ADC_SC2_CMPR_MASK     (1UL << 3)   /**< \brief 比较功能范围掩码 */
#define AMHW_FSL_ADC_SC2_DMA_MASK      (1UL << 2)   /**< \brief DMA传输掩码        */
#define AMHW_FSL_ADC_SC2_REF_MASK      (3UL << 0)   /**< \brief 参考电压选择掩码 */

#define AMHW_FSL_ADC_SC2_HWTRG_EN_CFG  (1UL << 6)   /**< \brief 硬件触发使能配置 */
#define AMHW_FSL_ADC_SC2_CMP_EN_CFG    (1UL << 5)   /**< \brief 比较功能使能配置 */
#define AMHW_FSL_ADC_SC2_CMPGT_EN_CFG  (1UL << 4)   /**< \brief 比较功能大于配置 */
#define AMHW_FSL_ADC_SC2_CMPR_EN_CFG   (1UL << 3)   /**< \brief 比较功能范围配置 */
#define AMHW_FSL_ADC_SC2_DMA_EN_CFG    (1UL << 2)   /**< \brief 使能DMA传输配置 */
#define AMHW_FSL_ADC_SC2_REF_CFG(x) ((x) & AMHW_FSL_ADC_SC2_REF_MASK) /**< \brief 参考电压选择配置 */
/** @} */

/**
 * \brief 是否正在转换当中
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 *
 * \retval AM_TRUE  : 正在转换
 * \retval AM_FALSE : 空闲
 *
 */
am_static_inline
uint32_t amhw_fsl_adc_busy_is (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->sc2 & AMHW_FSL_ADC_SC2_ACT_MASK) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief  使能硬件触发
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_hwtrg_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 |= AMHW_FSL_ADC_SC2_HWTRG_MASK;
}

/**
 * \brief 禁能硬件触发，使用软件触发
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_hwtrg_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 &= ~AMHW_FSL_ADC_SC2_HWTRG_MASK;
}

/**
 * \brief  使能比较功能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cmp_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 |= AMHW_FSL_ADC_SC2_CMP_MASK;
}

/**
 * \brief 禁能比较功能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cmp_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 &= ~AMHW_FSL_ADC_SC2_CMP_MASK;
}

/**
 * \brief 使能大于比较功能
 * \note 跟CV1大于或等于比较，跟CV2小于比较（如果启动范围比较的话）
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cmpgt_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 |= AMHW_FSL_ADC_SC2_CMPGT_MASK;
}

/**
 * \brief 禁能大于比较功能
 * \note  跟CV1小于比较，跟CV2大于或等于比较（如果启动范围比较的话）
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cmpgt_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 &= ~AMHW_FSL_ADC_SC2_CMPGT_MASK;
}

/**
 * \brief  使能范围比较
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cmpr_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 |= AMHW_FSL_ADC_SC2_CMPR_MASK;
}

/**
 * \brief 禁能范围比较
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cmpr_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 &= ~AMHW_FSL_ADC_SC2_CMPR_MASK;
}

/**
 * \brief  使能DMA传输
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_dma_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 |= AMHW_FSL_ADC_SC2_DMA_MASK;
}

/**
 * \brief 禁能DMA传输
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_dma_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc2 &= ~AMHW_FSL_ADC_SC2_DMA_MASK;
}

/**
 * \brief 设置参考电压
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] ref      : 参考电压选择项
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_ref_set (amhw_fsl_adc_t *p_hw_adc, amhw_fsl_adc_ref_t ref)
{
    p_hw_adc->sc2 &= ~AMHW_FSL_ADC_SC2_REF_MASK;
    p_hw_adc->sc2 |= AMHW_FSL_ADC_SC2_REF_CFG(ref);
}

/**
 * \brief 获取参考电压
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 参考电压选择项
 */
am_static_inline
amhw_fsl_adc_ref_t amhw_fsl_adc_ref_get (amhw_fsl_adc_t *p_hw_adc)
{
    return (amhw_fsl_adc_ref_t)(p_hw_adc->sc2 & AMHW_FSL_ADC_SC2_REF_MASK);
}

/**
 * \brief 配置状态控制寄存器2
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] cfg      : 配置参数 ， /ref grp_amhw_fsl_adc_sc2
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_sc2_cfg (amhw_fsl_adc_t *p_hw_adc, uint32_t cfg)
{
    p_hw_adc->sc2 = cfg;
}

/**
 * \brief ADC 硬件平均个数
 */
typedef enum amhw_fsl_adc_avgs {
    AMHW_FSL_ADC_AVGS_4  = 0,     /**< \brief 4个采样值做平均 */
    AMHW_FSL_ADC_AVGS_8  = 1,     /**< \brief 8个采样值做平均    */
    AMHW_FSL_ADC_AVGS_16 = 2,     /**< \brief 16个采样值做平均    */
    AMHW_FSL_ADC_AVGS_32 = 3,     /**< \brief 32个采样值做平均    */
} amhw_fsl_adc_avgs_t;

/**
 * \name ADC 状态控制寄存器3位域定义
 * \anchor grp_amhw_fsl_adc_sc3
 * @{
 */
#define AMHW_FSL_ADC_SC3_CAL_MASK      (1UL << 7)    /**< \brief 矫正标志掩码 */
#define AMHW_FSL_ADC_SC3_CALF_MASK     (1UL << 6)    /**< \brief 矫正失败掩码 */
#define AMHW_FSL_ADC_SC3_CONT_MASK     (1UL << 3)    /**< \brief 连续转换标志掩码 */
#define AMHW_FSL_ADC_SC3_AVG_MASK      (1UL << 2)    /**< \brief 硬件平均掩码 */
#define AMHW_FSL_ADC_SC3_AVGS_MASK     (3UL << 0)    /**< \brief 硬件平均个数掩码 */

#define AMHW_FSL_ADC_SC3_CAL_EN_CFG    (1UL << 7)   /**< \brief 矫正使能配置 */
#define AMHW_FSL_ADC_SC3_CONT_EN_CFG   (1UL << 3)   /**< \brief 连续转换使能配置 */
#define AMHW_FSL_ADC_SC3_AVG_EN_CFG    (1UL << 2)   /**< \brief 硬件平均使能配置 */
#define AMHW_FSL_ADC_SC3_AVGS_CFG(x) ((x) & AMHW_FSL_ADC_SC3_AVGS_MASK) /**< \brief 硬件平均个数配置 */
/** @} */

/**
 * \brief  使能矫正（启动矫正）
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cal_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc3 |= AMHW_FSL_ADC_SC3_CAL_MASK;
}

/**
 * \brief 禁能矫正
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_cal_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc3 &= ~AMHW_FSL_ADC_SC3_CAL_MASK;
}

/**
 * \brief 是否在矫正中
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 *
 * \retval AM_TRUE  : 正在矫正
 * \retval AM_FALSE : 矫正完成
 *
 */
am_static_inline
uint32_t amhw_fsl_adc_cal_get (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->sc3 & AMHW_FSL_ADC_SC3_CAL_MASK) ? AM_TRUE : AM_FALSE) ;
}

/**
 * \brief 矫正是否失败
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 *
 * \retval AM_TRUE  : 矫正失败
 * \retval AM_FALSE : 矫正正常完成
 *
 */
am_static_inline
uint32_t amhw_fsl_adc_cal_fail_is (amhw_fsl_adc_t *p_hw_adc)
{
    return ((p_hw_adc->sc3 & AMHW_FSL_ADC_SC3_CALF_MASK) ? AM_TRUE : AM_FALSE) ;
}

/**
 * \brief  清除矫正失败标志位
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_calf_clr (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc3 |= AMHW_FSL_ADC_SC3_CALF_MASK;
}

/**
 * \brief  使能连续转换模式
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_continue_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc3 |= AMHW_FSL_ADC_SC3_CONT_MASK;
}

/**
 * \brief 禁能连续转换模式
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_continue_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc3 &= ~AMHW_FSL_ADC_SC3_CONT_MASK;
}

/**
 * \brief  使能硬件平均
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_avg_enable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc3 |= AMHW_FSL_ADC_SC3_AVG_MASK;
}

/**
 * \brief 禁能硬件平均
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_avg_disable (amhw_fsl_adc_t *p_hw_adc)
{
    p_hw_adc->sc3 &= ~AMHW_FSL_ADC_SC3_AVG_MASK;
}

/**
 * \brief 设置硬件平均的采样个数
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] avgs     : 平均的采样个数
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_avgs_set (amhw_fsl_adc_t *p_hw_adc, amhw_fsl_adc_avgs_t avgs)
{
    p_hw_adc->sc3 &= ~AMHW_FSL_ADC_SC3_AVGS_MASK;
    p_hw_adc->sc3 |= AMHW_FSL_ADC_SC3_AVGS_CFG(avgs);
}

/**
 * \brief 获取硬件平均的采样个数
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return 平均的采样个数
 */
am_static_inline
amhw_fsl_adc_avgs_t amhw_fsl_adc_avgs_get (amhw_fsl_adc_t *p_hw_adc)
{
    return (amhw_fsl_adc_avgs_t)(p_hw_adc->sc3 & AMHW_FSL_ADC_SC3_AVGS_MASK);
}

/**
 * \brief 配置状态控制寄存器3
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] cfg      : 配置参数 ， /ref grp_amhw_fsl_adc_sc3
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_sc3_cfg (amhw_fsl_adc_t *p_hw_adc, uint32_t cfg)
{
    p_hw_adc->sc3 = cfg;
}


/**
 * \brief 设置偏移误差矫正值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] val      : 偏移误差值
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_ofs_set (amhw_fsl_adc_t *p_hw_adc, uint16_t val)
{
    p_hw_adc->ofs = val;
}

/**
 * \brief 获取偏移误差矫正值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return  偏移误差值
 */
am_static_inline
uint16_t amhw_fsl_adc_ofs_get (amhw_fsl_adc_t *p_hw_adc)
{
    return (uint16_t)(p_hw_adc->ofs);
}

/**
 * \brief 设置Plus-Side增益寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] val      : 增益值
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_pg_set (amhw_fsl_adc_t *p_hw_adc, uint16_t val)
{
    p_hw_adc->pg = val;
}

/**
 * \brief 获取Plus-Side增益寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return  增益值
 */
am_static_inline
uint16_t amhw_fsl_adc_pg_get (amhw_fsl_adc_t *p_hw_adc)
{
    return (uint16_t)(p_hw_adc->pg);
}

/**
 * \brief 设置Minus-Side增益寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] val      : 增益值
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_mg_set (amhw_fsl_adc_t *p_hw_adc, uint16_t val)
{
    p_hw_adc->mg = val;
}

/**
 * \brief 获取Minus-Side增益寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \return  增益值
 */
am_static_inline
uint16_t amhw_fsl_adc_mg_get (amhw_fsl_adc_t *p_hw_adc)
{
    return (uint16_t)(p_hw_adc->mg);
}

/**
 * \brief 设置Plus-Side增益寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 矫正寄存器索引值
 * \param[in] val      : 矫正值
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_clp_set (amhw_fsl_adc_t *p_hw_adc, uint8_t index, uint16_t val)
{
    if (index < 7) {
        p_hw_adc->clp[index] = val;
    }
}

/**
 * \brief 获取Plus-Side增益寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 矫正寄存器索引值，值范围为[0,6]之间
 *
 * \return  矫正值
 */
am_static_inline
uint16_t amhw_fsl_adc_clp_get (amhw_fsl_adc_t *p_hw_adc, uint8_t index)
{
    return (uint16_t)(p_hw_adc->clp[index]);
}

/**
 * \brief 设置Minus-Side寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 矫正寄存器索引值，值范围为[0,6]之间
 * \param[in] val      : 矫正值
 * \return 无
 */
am_static_inline
void amhw_fsl_adc_clm_set (amhw_fsl_adc_t *p_hw_adc, uint8_t index, uint16_t val)
{
    if (7 > index) {
        p_hw_adc->clm[index] = val;
    }
}

/**
 * \brief 获取Minus-Side寄存器值
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] index    : 矫正寄存器索引值，值范围为[0,6]之间
 * \return  矫正值
 */
am_static_inline
uint16_t amhw_fsl_adc_clm_get (amhw_fsl_adc_t *p_hw_adc, uint8_t index)
{
    return (uint16_t)(p_hw_adc->clm[index]);
}


/**
 * \brief ADC 校验, 默认矫正时钟频率为2M
 *
 * \param[in] p_hw_adc : 指向ADC寄存器块的指针
 * \param[in] clk_id   : ADC所使用的时钟源
 * \retval AM_TRUE  : 矫正成功
 * \retval AM_FALSE : 矫正失败
 */
uint8_t amhw_fsl_adc_calibrate (amhw_fsl_adc_t *p_hw_adc, am_clk_id_t clk_id);

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

#endif /*__AMHW_FSL_ADC_H */

/* end of file */
