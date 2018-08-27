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
 * \brief TSI 硬件操作接口
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-26  nwt, make some changes.
 * - 1.00 15-10-23  wxj, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_TSI_H
#define __AMHW_FSL_TSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_fsl_if_tsi
 * \copydoc amhw_fsl_tsi.h
 * @{
 */

/**
 * \brief TSI 寄存器块结构体
 */
typedef struct amhw_fsl_tsi {
    __IO uint32_t    gencs;         /**< \brief 控制、状态寄存器 */
    __IO uint32_t    data;          /**< \brief 数据寄存器 */
    __IO uint32_t    tshd;          /**< \brief 阀值寄存器*/
} amhw_fsl_tsi_t;

/**
 * \anchor TSI INT MODE
 * \brief  TSI 中断模式定义
 */
typedef enum amhw_fsl_tsi_irq_mode {
    AMHW_FSL_TSI_OUT_RANGE = 0,         /**< \brief 超出阀值中断使能 */
    AMHW_FSL_TSI_END_SCAN,              /**< \brief 扫描结束中断使能 */
}amhw_fsl_tsi_irq_mode_t;

/**
 * \anchor TSI MODE
 * \brief  TSI 模式定义
 */
typedef enum amhw_fsl_tsi_mode {
    AMHW_FSL_TSI_CAP_SENSING       = 0, /**< \brief 电容感应模式，无噪声检测 */
    AMHW_FSL_TSI_SINGLE_THR_FRQDIS = 4, /**< \brief 单阀值噪声检测模式，频率限制电路禁能*/
    AMHW_FSL_TSI_SINGLE_THR_FRQEN  = 8, /**< \brief 单阀值噪声检测模式，频率限制电路使能*/
    AMHW_FSL_TSI_AUTOMATIC_NOISE   = 12, /**< \brief 自动噪声检测模式 */
}amhw_fsl_tsi_mode_t;

/**
 * \anchor TSI REFCHRG
 * \brief  TSI 参考振荡器恒定电流源定义
 */
typedef enum amhw_fsl_tsi_refchrg {
    AMHW_FSL_TSI_REFCHRG_500NA     = 0, /**< \brief 500nA */
    AMHW_FSL_TSI_REFCHRG_1UA,           /**< \brief 1uA */
    AMHW_FSL_TSI_REFCHRG_2UA,           /**< \brief 2uA */
    AMHW_FSL_TSI_REFCHRG_4UA,           /**< \brief 4uA */
    AMHW_FSL_TSI_REFCHRG_8UA,           /**< \brief 8uA */
    AMHW_FSL_TSI_REFCHRG_16UA,          /**< \brief 16uA */
    AMHW_FSL_TSI_REFCHRG_32UA,          /**< \brief 32uA */
    AMHW_FSL_TSI_REFCHRG_64UA,          /**< \brief 64uA */
}amhw_fsl_tsi_refchrg_t;

/**
 * \anchor TSI EXTCHRG
 * \brief  TSI 电极振荡器恒定电流源定义
 */
typedef enum amhw_fsl_tsi_extchrg {
    AMHW_FSL_TSI_EXTCHRG_500NA     = 0, /**< \brief 500nA */
    AMHW_FSL_TSI_EXTCHRG_1UA,           /**< \brief 1uA */
    AMHW_FSL_TSI_EXTCHRG_2UA,           /**< \brief 2uA */
    AMHW_FSL_TSI_EXTCHRG_4UA,           /**< \brief 4uA */
    AMHW_FSL_TSI_EXTCHRG_8UA,           /**< \brief 8uA */
    AMHW_FSL_TSI_EXTCHRG_16UA,          /**< \brief 16uA */
    AMHW_FSL_TSI_EXTCHRG_32UA,          /**< \brief 32uA */
    AMHW_FSL_TSI_EXTCHRG_64UA,          /**< \brief 64uA */
}amhw_fsl_tsi_extchrg_t;

/**
 * \anchor TSI DVOLT
 * \brief  TSI 振荡器电压定义
 */
typedef enum amhw_fsl_tsi_dvolt {
    AMHW_FSL_TSI_DV103             = 0, /**< \brief DV = 1.03V */
    AMHW_FSL_TSI_DV073,                 /**< \brief DV = 0.73V */
    AMHW_FSL_TSI_DV043,                 /**< \brief DV = 0.43V */
    AMHW_FSL_TSI_DV029,                 /**< \brief DV = 0.29V */
}amhw_fsl_tsi_dvolt_t;

/**
 * \anchor TSI TSICH
 * \brief  TSI 电流检测通道
 */
typedef enum amhw_fsl_tsi_channel {
    AMHW_FSL_TSI_CHAN0             = 0, /**< \brief 通道0 */
    AMHW_FSL_TSI_CHAN1,                 /**< \brief 通道1 */
    AMHW_FSL_TSI_CHAN2,                 /**< \brief 通道2 */
    AMHW_FSL_TSI_CHAN3,                 /**< \brief 通道3 */
    AMHW_FSL_TSI_CHAN4,                 /**< \brief 通道4 */
    AMHW_FSL_TSI_CHAN5,                 /**< \brief 通道5 */
    AMHW_FSL_TSI_CHAN6,                 /**< \brief 通道6 */
    AMHW_FSL_TSI_CHAN7,                 /**< \brief 通道7 */
    AMHW_FSL_TSI_CHAN8,                 /**< \brief 通道8 */
    AMHW_FSL_TSI_CHAN9,                 /**< \brief 通道9 */
    AMHW_FSL_TSI_CHAN10,                /**< \brief 通道10 */
    AMHW_FSL_TSI_CHAN11,                /**< \brief 通道11 */
    AMHW_FSL_TSI_CHAN12,                /**< \brief 通道12 */
    AMHW_FSL_TSI_CHAN13,                /**< \brief 通道13 */
    AMHW_FSL_TSI_CHAN14,                /**< \brief 通道14 */
    AMHW_FSL_TSI_CHAN15,                /**< \brief 通道15 */
}amhw_fsl_tsi_channel_t;

/**
 * \anchor TSI FREQ_DIV
 * \brief  TSI 电极振荡器分频器定义
 */
typedef enum amhw_fsl_tsi_oscfreq_div {
    AMHW_FSL_TSI_OSCFRQ_DIV_1      = 0, /**< \brief 1分频 */
    AMHW_FSL_TSI_OSCFRQ_DIV_2,          /**< \brief 2分频 */
    AMHW_FSL_TSI_OSCFRQ_DIV_4,          /**< \brief 4分频 */
    AMHW_FSL_TSI_OSCFRQ_DIV_8,          /**< \brief 8分频 */
    AMHW_FSL_TSI_OSCFRQ_DIV_16,         /**< \brief 16分频 */
    AMHW_FSL_TSI_OSCFRQ_DIV_32,         /**< \brief 32分频 */
    AMHW_FSL_TSI_OSCFRQ_DIV_64,         /**< \brief 64分频 */
    AMHW_FSL_TSI_OSCFRQ_DIV_128         /**< \brief 128分频 */
}amhw_fsl_tsi_oscfreq_div_t;

/**
 * \anchor TSI scan tirgger mode
 * \brief  TSI 扫描触发模式定义
 */
typedef enum amhw_fsl_tsi_trig_mode {
    AMHW_FSL_TSI_TRIG_SOFTWARE     = 0, /**< \brief 软件触发 */
    AMHW_FSL_TSI_TRIG_HAEDWARE,         /**< \brief 硬件触发 */
}amhw_fsl_tsi_trig_mode_t;

/**
 * \name  TSI 寄存器位定义
 * @{
 */

#define AMHW_FSL_TSI_OUTRGF       (0x1UL << 31) /** \brief 数据超出阀值标志位 */
#define AMHW_FSL_TSI_ESOR         (0x1UL << 28) /** \brief 中断模式选择 */
#define AMHW_FSL_TSI_TSIEN        (0x1UL << 7)  /** \brief TSI使能 */
#define AMHW_FSL_TSI_TSIIEN       (0x1UL << 6)  /** \brief 中断使能 */
#define AMHW_FSL_TSI_STPE         (0x1UL << 5)  /** \brief 低功耗模式下TSI使能 */
#define AMHW_FSL_TSI_STM          (0x1UL << 4)  /** \brief 扫描触发方式选择 */
#define AMHW_FSL_TSI_SCNIP        (0x1UL << 3)  /** \brief 进程中扫描 */
#define AMHW_FSL_TSI_EOSF         (0x1UL << 2)  /** \brief 扫描结束标志 */
#define AMHW_FSL_TSI_CURSW        (0x1UL << 1)  /** \brief 电流对交换 */
#define AMHW_FSL_TSI_DMAEN        (0x1UL << 23) /** \brief DMA传输使能 */
#define AMHW_FSL_TSI_SWTS         (0x1UL << 22) /** \brief 通道软件触发启动 */

/** @} */

/**
 * \brief 读取 TSI 数据超出阀值标志
 *
 * \param[in] p_hw_tsi   : 指向TSI 外设寄存器块的指针
 *
 * \retval 0 : 数据没有超出范围
 * \retval 1 : 数据超出范围
 */
am_static_inline
int amhw_fsl_tsi_outrgf_get (amhw_fsl_tsi_t *p_hw_tsi)
{
    return (((p_hw_tsi->gencs & (1ul << 31)) !=0) ? 1 : 0);
}

/**
 * \brief 清零 TSI 数据超出阀值标志
 *
 * \param[in] p_hw_tsi   : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_outrgf_clr (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs |= AMHW_FSL_TSI_OUTRGF;
}

/**
 * \brief TSI 中断模式设置
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] irq       : 中断模式
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_int_mode_set (amhw_fsl_tsi_t *p_hw_tsi, amhw_fsl_tsi_irq_mode_t irq)
{
    AM_BITS_SET(p_hw_tsi->gencs, 28, 1, irq);
}

/**
 * \brief TSI 工作模式设置
 *
 * \param[in] p_hw_tsi  : 指向 TSI 外设寄存器块的指针
 * \param[in] mode      : 要设置的模式
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_mode_set (amhw_fsl_tsi_t *p_hw_tsi, amhw_fsl_tsi_mode_t mode)
{
    AM_BITS_SET(p_hw_tsi->gencs, 24, 4, mode);
}

/**
 * \brief TSI 参考振荡器恒流源设置
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] value     : 设置的电流值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_refchrg_val_set (amhw_fsl_tsi_t *p_hw_tsi,
                               amhw_fsl_tsi_refchrg_t value)
{
    AM_BITS_SET(p_hw_tsi->gencs, 21, 3, value);
}

/**
 * \brief 振荡器电压设置
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] value     : 设置的电压值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_dvolt_set (amhw_fsl_tsi_t *p_hw_tsi,
                         amhw_fsl_tsi_dvolt_t value)
{
    AM_BITS_SET(p_hw_tsi->gencs, 19, 2, value);
}

/**
 * \brief TSI 电极振荡器恒流源设置
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] value     : 设置的电流值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_extchrg_val_set (amhw_fsl_tsi_t *p_hw_tsi,
                               amhw_fsl_tsi_extchrg_t value)
{
    AM_BITS_SET(p_hw_tsi->gencs, 16, 3, value);
}

/**
 * \brief TSI 电极振荡器分频设置
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] div_val   : 设置的分频值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_oscfreq_div_set (amhw_fsl_tsi_t *p_hw_tsi,
                               amhw_fsl_tsi_oscfreq_div_t div_val)
{
    AM_BITS_SET(p_hw_tsi->gencs, 13, 3, div_val);
}

/**
 * \brief TSI 电极扫描次数设置
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] num       : 设置的扫描次数
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_scan_num_set (amhw_fsl_tsi_t *p_hw_tsi, uint32_t num)
{
    AM_BITS_SET(p_hw_tsi->gencs, 8, 5, num);
}

/**
 * \brief TSI 模块使能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_mode_enable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs |= AMHW_FSL_TSI_TSIEN;
}

/**
 * \brief TSI 模块禁能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_mode_disable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs &= (~AMHW_FSL_TSI_TSIEN);
}

/**
 * \brief TSI 中断使能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_int_enable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs |= AMHW_FSL_TSI_TSIIEN;
}

/**
 * \brief TSI 中断禁能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_int_disable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs &= (~AMHW_FSL_TSI_TSIIEN);
}

/**
 * \brief TSI 低功耗模式下工作使能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_lowpower_enable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs |= AMHW_FSL_TSI_STPE;
}

/**
 * \brief TSI 低功耗模式下工作禁能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_lowpower_disable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs &= (~AMHW_FSL_TSI_STPE);
}

/**
 * \brief TSI 扫描触发模式设置
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] mode      : 需要设置的扫描模式
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_scan_trig_set (amhw_fsl_tsi_t *p_hw_tsi,
                             amhw_fsl_tsi_trig_mode_t mode)
{
    AM_BITS_SET(p_hw_tsi->gencs, 4, 1, mode);
}

/**
 * \brief TSI 读扫描触发模式
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \retval 0 : 软件触发
 * \retval 1 : 硬件触发
 */
am_static_inline
int amhw_fsl_tsi_scan_trig_get (amhw_fsl_tsi_t *p_hw_tsi)
{
    return (((p_hw_tsi->gencs & AMHW_FSL_TSI_STM) !=0) ? 1 : 0);
}

/**
 * \brief TSI 扫描状态获取
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \retval 0 : 进程中没有扫描
 * \retval 1 : 进程中有扫描
 */
am_static_inline
int amhw_fsl_tsi_scnip_get (amhw_fsl_tsi_t *p_hw_tsi)
{
    return (((p_hw_tsi->gencs & AMHW_FSL_TSI_SCNIP) != 0) ? 1 : 0);
}

/**
 * \brief TSI 读扫描结束标志
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \retval 0 : 扫描没有结束
 * \retval 1 : 扫描结束
 */
am_static_inline
int amhw_fsl_tsi_eosf_get (amhw_fsl_tsi_t *p_hw_tsi)
{
    return (((p_hw_tsi->gencs & AMHW_FSL_TSI_EOSF) !=0) ? 1 : 0);
}

/**
 * \brief TSI 清扫描结束标志
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_eosf_clr (amhw_fsl_tsi_t *p_hw_tsi)
{
   p_hw_tsi->gencs |= AMHW_FSL_TSI_EOSF;
}

/**
 * \brief TSI 电极振荡器电流与参考振荡器电流交换使能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_cursw_enable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs |= AMHW_FSL_TSI_CURSW;
}

/**
 * \brief TSI 电极振荡器电流与参考振荡器电流交换禁能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_cursw_disable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->gencs &= (~AMHW_FSL_TSI_CURSW);
}

/**
 * \brief TSI 电流检测通道选择
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] chan      : 需要选择的通道
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_chan_set (amhw_fsl_tsi_t *p_hw_tsi, amhw_fsl_tsi_channel_t chan)
{
    AM_BITS_SET(p_hw_tsi->data, 28, 4, chan);
}

/**
 * \brief TSI DMA传输请求使能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_dma_enable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->data |= AMHW_FSL_TSI_DMAEN;
}

/**
 * \brief TSI DMA传输请求禁能
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_dma_disable (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->data &= (~AMHW_FSL_TSI_DMAEN);
}

/**
 * \brief TSI 通道软件触发启动
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_soft_tirg_start (amhw_fsl_tsi_t *p_hw_tsi)
{
    p_hw_tsi->data |= AMHW_FSL_TSI_SWTS;
}

/**
 * \brief TSI 读取转换结果值
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 转换后的值
 */
am_static_inline
uint16_t amhw_fsl_tsi_convert_value_get (amhw_fsl_tsi_t *p_hw_tsi)
{
    return AM_BITS_GET(p_hw_tsi->data, 0, 16);
}

/**
 * \brief TSI 写通道阀值上限
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] value     : 阀值上限
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_wake_chan_thresholdh_write (amhw_fsl_tsi_t *p_hw_tsi, uint32_t value)
{
    AM_BITS_SET(p_hw_tsi->tshd, 16, 16, value);
}

/**
 * \brief TSI 读通道阀值上限
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 阀值上限
 */
am_static_inline
uint16_t amhw_fsl_tsi_wake_chan_thresholdh_read (amhw_fsl_tsi_t *p_hw_tsi)
{
    return AM_BITS_GET(p_hw_tsi->tshd, 16, 16);
}

/**
 * \brief TSI 写通道下限阀值
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 * \param[in] value     : 阀值下限
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tsi_wake_chan_thresholdl_write (amhw_fsl_tsi_t *p_hw_tsi, uint32_t value)
{
    AM_BITS_SET(p_hw_tsi->tshd, 0, 16, value);
}

/**
 * \brief TSI 读通道阀值下限
 *
 * \param[in] p_hw_tsi  : 指向TSI 外设寄存器块的指针
 *
 * \return 下限阀值
 */
am_static_inline
uint16_t amhw_fsl_tsi_wake_chan_thresholdl_read (amhw_fsl_tsi_t *p_hw_tsi)
{
    return AM_BITS_GET(p_hw_tsi->tshd, 0, 16);
}

/**
 * @} amhw_fsl_if_tsi
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_TSI_H */

/* end of file */

