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
 * \brief DMA 硬件操作接口。
 *
 * 1. 4个独立可编程的DMA控制通道。
 * 2. 双地址传输通过32位主机连接到系统总线上。
 * 3. 8、16、32位的快数据传输。
 * 4. 支持可编程的源地址、目的地址以及传输字节数。
 * 5. 可选的单/双通道连接。
 * 6. 可选的访问源地址和目标地址的矩阵。
 * 7. 在传输边界使用固定优先级方案的通道仲裁。
 * 8. 支持源地址和目的地址的自动更新
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-16  mkr, first implementation.
 * \endinternal
 */

#ifndef __KL26_DMA_H
#define __KL26_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"

/**
 * \addtogroup amhw_kl26_if_dma
 * \copydoc amhw_kl26_dma.h
 * @{
 */

#define KL26_DMA_CHAN_CNT       4     /**< \brief DMA 通道数量 */

/******************************************************************************/
/**
 * \brief DMA 通道寄存器结构体
 */
typedef struct amhw_kl26_dmamux {
    __IO  uint8_t  chcfg[4];            /**< \brief DMA 通道配置寄存器           */
} amhw_kl26_dmamux_t;

/**
 * \brief DMA 控制器寄存器块结构体
 */
typedef struct amhw_kl26_dma_chan {
    __IO uint32_t  dma_sar;             /**< \brief DMA 源地址配置寄存器       */
    __IO uint32_t  dma_dar;             /**< \brief DMA 目的地址配置寄存器  */
    __IO uint32_t  dma_dsr_bcr;         /**< \brief DMA 状态寄存器                      */
    __IO uint32_t  dma_dcr;             /**< \brief DMA 控制寄存器                      */
} amhw_kl26_dma_chan_t;

#define  KL26_DMA_TRIGGER_ENABLE           (1UL << 6)       /**< \brief DMA 触发使能                  */
#define  KL26_DMA_TRIGGER_DISABLE          (0UL << 6)       /**< \brief DMA 触发禁能                  */

#define  KL26_DMA_ADDRESS_VALUE_ALLOWED_1  (0x000UL << 20)  /**< \brief DMA 有效地址模式1  */
#define  KL26_DMA_ADDRESS_VALUE_ALLOWED_2  (0x1FFUL << 20)  /**< \brief DMA 有效地址模式2  */
#define  KL26_DMA_ADDRESS_VALUE_ALLOWED_3  (0x200UL << 20)  /**< \brief DMA 有效地址模式3  */
#define  KL26_DMA_ADDRESS_VALUE_ALLOWED_4  (0x400UL << 20)  /**< \brief DMA 有效地址模式4  */

/**
 * \brief DMA 通道选择结构体
 */
typedef struct amhw_kl26_dma {
    amhw_kl26_dma_chan_t    dma_chan[4];     /**< \brief DMA 通道配置                            */
} amhw_kl26_dma_t;

/**
 * \brief DMA 模式选择
 */
typedef enum amhw_kl26_dma_transfer_type {
    KL26_DMA_PER_TO_MER,     /**< \brief DMA 外设到内存                            */
    KL26_DMA_MER_TO_PER,     /**< \brief DMA 内存到外设                            */
    KL26_DMA_MER_TO_MER,     /**< \brief DMA 内存到内存                            */
    KL26_DMA_PER_TO_PER      /**< \brief DMA 外设到外设                            */
} amhw_kl26_dma_transfer_type_t;

#define  KL26_DMA_DCR_INTERRUTP_DISABLE         (0UL << 31)   /**< \brief DMA中断禁能              */
#define  KL26_DMA_DCR_INTERRUTP_ENABLE          (1UL << 31)   /**< \brief DMA中断使能              */

#define  KL26_DMA_DCR_PER_REQUEST_DISABLE       (0UL << 30)   /**< \brief DMA外设请求使能    */
#define  KL26_DMA_DCR_PER_REQUEST_ENABLE        (1UL << 30)   /**< \brief DMA外设请求禁能    */

#define  KL26_DMA_DCR_CYCLE_TRANSFERS           (0UL << 29)   /**< \brief DMA循环传输              */
#define  KL26_DMA_DCR_SINGLE_TRANSFERS          (1UL << 29)   /**< \brief DMA单次传输              */

#define  KL26_DMA_DCR_AUTO_ALIGN_DISABLE        (0UL << 28)   /**< \brief DMA自动对齐禁能    */
#define  KL26_DMA_DCR_AUTO_ALIGN_ENABLE         (1UL << 28)   /**< \brief DMA自动对齐使能    */

#define  KL26_DMA_DCR_SOURCE_INCR_DISABLE       (0UL << 22)   /**< \brief 源地址自增禁能        */
#define  KL26_DMA_DCR_SOURCE_INCR_ENABLE        (1UL << 22)   /**< \brief 源地址自增使能        */

#define  KL26_DMA_DCR_SOURCE_SIZE_32_BIT        (0x0UL << 20) /**< \brief 32位传输                       */
#define  KL26_DMA_DCR_SOURCE_SIZE_8_BIT         (0x1UL << 20) /**< \brief 8 位传输                       */
#define  KL26_DMA_DCR_SOURCE_SIZE_16_BIT        (0x2UL << 20) /**< \brief 16位传输                       */

#define  KL26_DMA_DCR_DESTINATION_INCR_DISABLE  (0UL << 19)   /**< \brief 目的地址自增使能    */
#define  KL26_DMA_DCR_DESTINATION_INCR_ENABLE   (1UL << 19)   /**< \brief 目的地址自增使能    */

#define  KL26_DMA_DCR_DESTINATION_SIZE_32_BIT   (0x0UL << 17) /**< \brief 32位传输                        */
#define  KL26_DMA_DCR_DESTINATION_SIZE_8_BIT    (0x1UL << 17) /**< \brief 8 位传输                        */
#define  KL26_DMA_DCR_DESTINATION_SIZE_16_BIT   (0x2UL << 17) /**< \brief 16位传输                        */

#define  KL26_DMA_DCR_START_DISABLE             (0x0UL << 16) /**< \brief 软件启动禁能              */
#define  KL26_DMA_DCR_START_ENABLE              (0x1UL << 16) /**< \brief 软件启动使能              */

#define KL26_DMA_DCR_REQUEST_NO_AFFECTED        (0x0UL << 7)  /**< \brief 请求没影响                   */
#define KL26_DMA_DCR_REQUEST_AFFECTED           (0x1UL << 7)  /**< \brief 请求有影响                   */

/** \brief 没有通道连接      */
#define  KL26_DMA_DCR_NO_LINKING                (0x0UL << 4)

/** \brief 每个周期后连接到LCH1，当BCR到0，连接到LCH2 */
#define  KL26_DMA_DCR_LCH1_TO_LCH2              (0x1UL << 4)

/** \brief 每个周期结束连接到LCH1  */
#define  KL26_DMA_DCR_EACH_CYCLE_TO_LCH1        (0x2UL << 4)

/** \brief BCR到0连接到LCH1  */
#define  KL26_DMA_DCR_BCR_ZERO_TO_LCH1          (0x3UL << 4)

#define  KL26_DMA_DCR_LCH1_CHAN0                (0x0UL << 2)  /**< \brief LCH1映射到通道0 */
#define  KL26_DMA_DCR_LCH1_CHAN1                (0x1UL << 2)  /**< \brief LCH1映射到通道1 */
#define  KL26_DMA_DCR_LCH1_CHAN2                (0x2UL << 2)  /**< \brief LCH1映射到通道2 */
#define  KL26_DMA_DCR_LCH1_CHAN3                (0x3UL << 2)  /**< \brief LCH1映射到通道3 */

#define  KL26_DMA_DCR_LCH2_CHAN0                (0x0UL)       /**< \brief LCH2映射到通道0 */
#define  KL26_DMA_DCR_LCH2_CHAN1                (0x1UL)       /**< \brief LCH2映射到通道1 */
#define  KL26_DMA_DCR_LCH2_CHAN2                (0x2UL)       /**< \brief LCH2映射到通道2 */
#define  KL26_DMA_DCR_LCH2_CHAN3                (0x3UL)       /**< \brief LCH2映射到通道3 */

/**
 * \brief 缓冲区大小设置枚举体
 */
typedef enum amhw_kl26_dma_buffer_size_set {
    KL26_DMA_BUFFER_DISABLE = 0,    /**< \brief 无缓冲区                  */
    KL26_DMA_BUFFER_16_BYTES,       /**< \brief 16字节缓冲区        */
    KL26_DMA_BUFFER_32_BYTES,       /**< \brief 32字节缓冲区        */
    KL26_DMA_BUFFER_64_BYTES,       /**< \brief 64字节缓冲区        */
    KL26_DMA_BUFFER_128_BYTES,      /**< \brief 128字节缓冲区      */
    KL26_DMA_BUFFER_256_BYTES,      /**< \brief 256字节缓冲区      */
    KL26_DMA_BUFFER_512_BYTES,      /**< \brief 512字节缓冲区      */
    KL26_DMA_BUFFER_1_KB,           /**< \brief 1KB缓冲区                */
    KL26_DMA_BUFFER_2_KB,           /**< \brief 2KB缓冲区                */
    KL26_DMA_BUFFER_4_KB,           /**< \brief 4KB缓冲区                */
    KL26_DMA_BUFFER_8_KB,           /**< \brief 8KB缓冲区                 */
    KL26_DMA_BUFFER_16_KB,          /**< \brief 16KB缓冲区              */
    KL26_DMA_BUFFER_32_KB,          /**< \brief 32KB缓冲区              */
    KL26_DMA_BUFFER_64_KB,          /**< \brief 64KB缓冲区              */
    KL26_DMA_BUFFER_128_KB,         /**< \brief 128KB缓冲区            */
    KL26_DMA_BUFFER_256_KB,         /**< \brief 256KB缓冲区            */
} amhw_kl26_dma_buffer_size_set_t;


/**
 * \brief DMA 重载描述符
 */
typedef struct amhw_kl26_dma_xfer_desc {
    volatile uint32_t          xfercfg;        /**< \brief 传输配置                     */
    volatile uint32_t          src_addr;       /**< \brief 源数据地址                */
    volatile uint32_t          dst_addr;       /**< \brief 目标数据地址           */
    volatile uint32_t          nbytes;         /**< \brief 传输数据字节数      */
} amhw_kl26_dma_xfer_desc_t;

/**
 * \brief 使能DMA通道
 *
 * \param[in] p_hw_dmamux : 指向DMA通道控制器寄存器块的指针
 * \param[in] chan        : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_chan_enable (amhw_kl26_dmamux_t *p_hw_dmamux,
                                uint8_t             chan)
{
    p_hw_dmamux->chcfg[chan] |=(1UL<<7);
}

/**
 * \brief 禁能DMA通道
 *
 * \param[in] p_hw_dmamux : 指向DMA通道控制器寄存器块的指针
 * \param[in] chan        : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_chan_disable (amhw_kl26_dmamux_t *p_hw_dmamux,
                                 uint8_t             chan)
{
    p_hw_dmamux->chcfg[chan] &= ~(1UL<<7);
}

/**
 * \brief 使能DMA通道触发器
 *
 * \param[in] p_hw_dmamux : 指向DMA通道控制器寄存器块的指针
 * \param[in] chan        : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_trig_enable (amhw_kl26_dmamux_t *p_hw_dmamux,
                                uint8_t             chan)
{
    p_hw_dmamux->chcfg[chan] &= ~(1UL << 6);
    p_hw_dmamux->chcfg[chan] |=  (1UL << 6);
}

/**
 * \brief 禁能DMA通道触发器
 *
 * \param[in] p_hw_dmamux : 指向DMA通道控制器寄存器块的指针
 * \param[in] chan        : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_trig_disable (amhw_kl26_dmamux_t *p_hw_dmamux,
                                 uint8_t             chan)
{
    p_hw_dmamux->chcfg[chan] &= ~(1UL << 6);
}

/**
 * \brief DMA 通道源选择
 *
 * \param[in] p_hw_dmamux : 指向DMA通道控制器寄存器块的指针
 * \param[in] source      : DMA请求源设置，在其他文件有相关定义
 * \param[in] chan        : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_chan_source_set (amhw_kl26_dmamux_t *p_hw_dmamux,
                                    uint8_t             source,
                                    uint8_t             chan)
{
    p_hw_dmamux->chcfg[chan] &= ~(0x3f);
    p_hw_dmamux->chcfg[chan] |= source ;
}

/**
 * \brief DMA 源地址设置
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] address   : 写入对应的源地址
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_source_address_set (amhw_kl26_dma_t *p_hw_dma,
                                       uint32_t         address,
                                       uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_sar = address;
}

/**
 * \brief DMA 目的地址设置
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] address   : 写入对应的目的地址
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_destination_address_set (amhw_kl26_dma_t *p_hw_dma,
                                            uint32_t         address,
                                            uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dar = address;
}

/**
 * \brief DMA 配置错误返回
 *
 * \param[in] p_hw_dma : 指向DMA控制寄存器块的指针
 * \param[in] chan     : 对应的通道
 *
 * \retval  AM_TRUE       : 配置错误发生
 * \retval  AM_FALSE      : 没有配置错误发生
 */
am_static_inline
am_bool_t amhw_kl26_dma_configuration_error_get (amhw_kl26_dma_t *p_hw_dma,
                                              uint8_t          chan)
{
    return (am_bool_t)(p_hw_dma->dma_chan[chan].dma_dsr_bcr & (0x1UL << 30));
}

/**
 * \brief DMA 源总线错误返回
 *
 * \param[in] p_hw_dma : 指向DMA控制寄存器块的指针
 * \param[in] chan     : 对应的通道
 *
 * \retval  AM_TRUE       : 源总线错误发生
 * \retval  AM_FALSE      : 没有源总线错误发生
 */
am_static_inline
am_bool_t amhw_kl26_dma_source_bus_error_get (amhw_kl26_dma_t *p_hw_dma,
                                           uint8_t          chan)
{
    return (am_bool_t)(p_hw_dma->dma_chan[chan].dma_dsr_bcr & (0x1UL << 29));
}

/**
 * \brief DMA 目的总线错误返回
 *
 * \param[in] p_hw_dma : 指向DMA控制寄存器块的指针
 * \param[in] chan     : 对应的通道
 *
 * \retval  AM_TRUE       : 目的总线错误发生
 * \retval  AM_FALSE      : 没有目的总线错误发生
 */
am_static_inline
am_bool_t amhw_kl26_dma_destination_bus_error_get (amhw_kl26_dma_t *p_hw_dma,
                                                uint8_t          chan)
{
    return (am_bool_t)(p_hw_dma->dma_chan[chan].dma_dsr_bcr & (0x1UL << 28));
}

/**
 * \brief DMA 通道请求返回
 *
 * \param[in] p_hw_dma : 指向DMA控制寄存器块的指针
 * \param[in] chan     : 对应的通道
 *
 * \retval  AM_TRUE       : 通道正在传输数据
 * \retval  AM_FALSE      : 没有通道请求
 */
am_static_inline
am_bool_t amhw_kl26_dma_request_status_get (amhw_kl26_dma_t *p_hw_dma,
                                         uint8_t          chan)
{
    return (am_bool_t)(p_hw_dma->dma_chan[chan].dma_dsr_bcr & (0x1UL << 26));
}

/**
 * \brief DMA 通道状态返回
 *
 * \param[in] p_hw_dma : 指向DMA控制寄存器块的指针
 * \param[in] chan     : 对应的通道
 *
 * \retval  AM_TRUE       : 通道初始化后使能
 * \retval  AM_FALSE      : 通道不活跃
 */
am_static_inline
am_bool_t amhw_kl26_dma_busy_status_get (amhw_kl26_dma_t *p_hw_dma,
                                      uint8_t          chan)
{
    return (am_bool_t)(p_hw_dma->dma_chan[chan].dma_dsr_bcr & (0x1UL << 25));
}

/**
 * \brief DMA 传输完成返回
 *
 * \param[in] p_hw_dma : 指向DMA控制寄存器块的指针
 * \param[in] chan     : 对应的通道
 *
 * \retval  AM_TRUE       : 传输完成
 * \retval  AM_FALSE      : 传输没有完成
 */
am_static_inline
am_bool_t amhw_kl26_dma_complete_ststus_get (amhw_kl26_dma_t *p_hw_dma,
                                          uint8_t          chan)
{
    return (p_hw_dma->dma_chan[chan].dma_dsr_bcr & (0x1UL << 24)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief DMA 状态清除
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_status_clear (amhw_kl26_dma_t *p_hw_dma,
                                 uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dsr_bcr |=  (1UL << 24);
}

/**
 * \brief DMA 块字节数写入
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] num       : 传输的字节数
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_bcr_set (amhw_kl26_dma_t *p_hw_dma,
                            uint32_t         num,
                            uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dsr_bcr &= ~(0xffffff);
    p_hw_dma->dma_chan[chan].dma_dsr_bcr |=  (num);
}

/**
 * \brief DMA 块字节数读取
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
uint32_t amhw_kl26_dma_bcr_get (amhw_kl26_dma_t *p_hw_dma,
                                uint8_t          chan)
{
    return (p_hw_dma->dma_chan[chan].dma_dsr_bcr & 0xffffff);
}

/**
 * \brief DMA 完成中断使能
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_intterupt_enable (amhw_kl26_dma_t *p_hw_dma,
                                     uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr |=  (1UL<<31);
}

/**
 * \brief DMA 完成中断禁能
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_intterupt_disable (amhw_kl26_dma_t *p_hw_dma,
                                      uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(1UL<<31);
}

/**
 * \brief DMA 外部设备请求使能
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_per_resquest_enable (amhw_kl26_dma_t *p_hw_dma,
                                        uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr |= (1UL<<30);
}

/**
 * \brief DMA 外部设备请求禁能
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_per_resquest_disable (amhw_kl26_dma_t *p_hw_dma,
                                         uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(1UL<<30);
}

/**
 * \brief DMA 异步请求使能
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_asy_request_enable (amhw_kl26_dma_t *p_hw_dma,
                                       uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(1UL<<23);
    p_hw_dma->dma_chan[chan].dma_dcr |=  (1UL<<23);
}

/**
 * \brief DMA 异步请求禁能
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_asy_resquest_disable (amhw_kl26_dma_t *p_hw_dma,
                                         uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(1UL<<23);
}

/**
 * \brief DMA 开始传输使能
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_transfer_start (amhw_kl26_dma_t *p_hw_dma,
                                   uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(1UL << 16);
    p_hw_dma->dma_chan[chan].dma_dcr |=  (1UL << 16);
}

/**
 * \brief DMA 传输停止
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_transfer_stop (amhw_kl26_dma_t *p_hw_dma ,
                                  uint8_t     chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(1UL << 16);
}

/**
 * \brief DMA 传输模式设置
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] mode      : DMA模式设置，有对应的宏定义
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_transfer_mode_set (amhw_kl26_dma_t *p_hw_dma,
                                      uint32_t         mode,
                                      uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(0xf0ffffbf);
    p_hw_dma->dma_chan[chan].dma_dcr |= (mode);
}

/**
 * \brief DMA 源地址缓冲区大小设置
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] mode      : 缓冲区大小设置枚举体
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_source_buffer_set (amhw_kl26_dma_t                 *p_hw_dma,
                                      amhw_kl26_dma_buffer_size_set_t  mode,
                                      uint8_t                          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(0xf << 12);
    p_hw_dma->dma_chan[chan].dma_dcr |=  (mode << 12);
}

/**
 * \brief DMA 目的地址缓冲区大小设置
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] mode      : 缓冲区大小设置枚举体
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_destination_buffer_set (amhw_kl26_dma_t                 *p_hw_dma,
                                           amhw_kl26_dma_buffer_size_set_t  mode,
                                           uint8_t                          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(0xf << 8);
    p_hw_dma->dma_chan[chan].dma_dcr |=  (mode << 8);
}

/**
 * \brief DMA 连接通道模式设置
 *
 * \param[in] p_hw_dma  : 指向DMA控制寄存器块的指针
 * \param[in] mode      : DMA模式设置，有对应的宏定义
 * \param[in] chan      : 对应的通道
 *
 * \return    无
 */
am_static_inline
void amhw_kl26_dma_link_mode_set (amhw_kl26_dma_t *p_hw_dma,
                                  uint32_t         mode,
                                  uint8_t          chan)
{
    p_hw_dma->dma_chan[chan].dma_dcr &= ~(0x3f);
    p_hw_dma->dma_chan[chan].dma_dcr |=  (mode);
}

/**
 * @} amhw_if_kl26_dma
 */

#ifdef __cplusplus
}
#endif

#endif  /* __KL26_DMA_H */

/* end of file */
