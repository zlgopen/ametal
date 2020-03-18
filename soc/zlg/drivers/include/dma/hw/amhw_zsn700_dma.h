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
 * \brief DMA hardware operation interface.
 *
 * 1. 2 independent programmable DMA channel.
 * 2. 8,16,32 bits data to transfer.
 * 3. support programmable source address,destination address and data size。
 *
 * 存在一个 硬件配置项： AMHW_ZSN700_DMA_CHAN_CNT。用以配置 DMA通道数目，使用本文件时，
 * 应在 soc_cfg.h 文件中定义该宏。
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-18  yrz, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZSN700_DMA_H
#define __AMHW_ZSN700_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"
#include "soc_cfg.h"
#include "am_vdebug.h"
/**
 * \addtogroup amhw_zsn700_if_dma
 * \copydoc amhw_zsn700_dma.h
 * @{
 */

/** \brief DMA 通道数目  */
#define AMHW_ZSN700_DMA_CHAN_CNT  SOC_CFG_ZLG_DMA_CHAN_CNT

/**
 * \brief structure of DMA controller register
 */
typedef struct amhw_zsn700_dma_chan {
    __IO uint32_t  dma_conf;           /**< \brief 所有通道配置寄存器 */
         uint32_t  reserved[3];        /**< \brief Reserved */
    struct {                        
        __IO uint32_t  dma_confa;      /**< \brief 通道配置A寄存器 */
        __IO uint32_t  dma_confb;      /**< \brief 通道配置B寄存器 */
        __IO uint32_t  dma_srcadr;     /**< \brief 通道传输源地址寄存器 */
        __IO uint32_t  dma_dstadr;     /**< \brief 通道传输目的地址寄存器 */
    } chcfg[AMHW_ZSN700_DMA_CHAN_CNT]; /**< \brief DMA通道结构体 */

} amhw_zsn700_dma_t;

/**
 * \brief DMA transfer mode select
 */
typedef enum {
    AMHW_ZSN700_DMA_PER_TO_MER,     /**< \brief DMA peripheral to memory mode */
    AMHW_ZSN700_DMA_MER_TO_PER,     /**< \brief DMA memory to peripheral mode */
    AMHW_ZSN700_DMA_MER_TO_MER,     /**< \brief DMA memory to memory mode */
    AMHW_ZSN700_DMA_PER_TO_PER      /**< \brief DMA peripheral to peripheral mode */
} amhw_zsn700_dma_transfer_type_t;

/**
 * \brief DMA descriptor
 */
typedef struct amhw_zsn700_dma_xfer_desc {
    volatile uint32_t          xfercfg;        /**< \brief transfer configuration */
    volatile uint32_t          src_addr;       /**< \brief source address */
    volatile uint32_t          dst_addr;       /**< \brief destination address */
    volatile uint32_t          nbytes;         /**< \brief the bytes of data to transfer */
} amhw_zsn700_dma_xfer_desc_t;

/**
 * \brief DMA 通道总使能控制位     使能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_enable(amhw_zsn700_dma_t *p_hw_dma)
{
    p_hw_dma->dma_conf |= (0x1ul << 31);
}

/**
 * \brief DMA 通道总使能控制位     禁能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_disable(amhw_zsn700_dma_t *p_hw_dma)
{
    p_hw_dma->dma_conf &= ~(0x1ul << 31);
}

/**
 * \brief DMA 所有通道的传输状态   获取
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 *
 * \note  如果下列任何情况在传输中发生， DMAC 将该位置”1”。
 *            1.当"0"写入 CONF:EN 位， 并且所有通道传输结束。
 *            2.当除"0000"外的值写入 CONF:HALT 位， 并且所有通道传输暂停。
 *
 *        当 CONF:EN=1和 CONF:HALT=0000有效，并且所有的通道使能操作， 本位清零。
 *
 * \return none
 */
am_static_inline
am_bool_t amhw_zsn700_dma_transfer_check (amhw_zsn700_dma_t  *p_hw_dma)
{
    return ((p_hw_dma->dma_conf >> 30) & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief DMA 所有通道的传输状态   设置
 *
 * \param[in] p_hw_dma : 指向DMA寄存器结构体的指针
 * \param[in] set      : 待设值
 *
 * \note  尽管本位的属性是 RW,CPU写"1"不影响 DMA的操作。然而如果
 *        CONF寄存器需要更新而又不想改变当前位， 应该首先读出这个位，然后重新写入同样的值。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_transfer_set (amhw_zsn700_dma_t *p_hw_dma, uint8_t set)
{
    p_hw_dma->dma_conf = (p_hw_dma->dma_conf & (~(0x1ul << 30))) |
                         ((set & 0x1ul) << 30);
}

/**< \brief DMA 各通道优先级固定。 CH0 优先级 > CH1 优先级 */
#define  AMHW_ZSN700_DMA_CHAN_PRIORITY_CH0_BEST   (0ul << 28)
/**< \brief DMA 各通道优先级采用轮询方式决定 */
#define  AMHW_ZSN700_DMA_CHAN_PRIORITY_POLL       (1ul << 28)
/**
 * \brief DMA 通道优先级设置
 *
 * \param[in] p_hw_dma : 指向DMA寄存器结构体的指针
 * \param[in] set      : 优先级宏定义
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_priority_set (amhw_zsn700_dma_t *p_hw_dma, uint32_t set)
{
    p_hw_dma->dma_conf = (p_hw_dma->dma_conf & (~(1ul << 28))) | set;
}

/**
 * \brief DMA  HALT位控制所有通道的传输暂停或取消。
 *
 * \param[in] p_hw_dma : 指向DMA寄存器结构体的指针
 * \param[in] set      : 待设值
 *
 * \note  当这些位设置为"0000"以外的值时,所有正在传输的通道进入暂停状态。
 *        当设置为"0000"， 所有传输继续。
 *
 *        即便来自外部或者外设的传输请求有效，暂停状态的通道忽略传输请求。如果是块传输和突发传输，
 *        即便暂停清除， 相关的通道也不启动传输。为了完成传输过程中暂停的传输，需要在暂停取消后发
 *        动另外的传输请求。
 *
 *        这些位可以用于暂停传输而不复位该通道的配置寄存器。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_halt_set (amhw_zsn700_dma_t *p_hw_dma, uint8_t set)
{
    p_hw_dma->dma_conf = (p_hw_dma->dma_conf & (~(0xful << 24))) |
                         ((set & 0xful) << 24);
}

/**
 * \brief DMA 某一通道的使能控制位    使能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note 当该位置"1", 相关通道使能，等待触发，开始传输功能(CONF:EN 必须置 "1")。
 *       如果 MSK 位 (CONFB[0]) 位清 "0"， DMA在完成传输后清除本位，即"0"。
 *       当该位为"0", 相关通道禁止，不进行数据传输。同时， 如果正好在传输过程中，那么停止传输。
 *       该位可用于强制停止正在传输操作的相关通道，同时复位配置寄存器。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_enable(amhw_zsn700_dma_t *p_hw_dma,
                                 uint8_t            channel)
{
    p_hw_dma->chcfg[channel].dma_confa |= (0x1ul << 31);
}

/**
 * \brief DMA 某一通道的使能控制位    禁能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note 当该位置"1", 相关通道使能，等待触发，开始传输功能(CONF:EN 必须置 "1")。
 *       如果 MSK 位 (CONFB[0]) 位清 "0"， DMA在完成传输后清除本位，即"0"。
 *       当该位为"0", 相关通道禁止，不进行数据传输。同时， 如果正好在传输过程中，那么停止传输。
 *       该位可用于强制停止正在传输操作的相关通道，同时复位配置寄存器。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_disable(amhw_zsn700_dma_t *p_hw_dma,
                                  uint8_t            channel)
{
    p_hw_dma->chcfg[channel].dma_confa &= ~(0x1ul << 31);
}

/**
 * \brief DMA PAS位控制各自通道的传输暂停或取消        暂停
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note 当该位置 "1" ，相关正在传输的通道进入暂停状态。清除该位到 0，从新开始传输。
 *       该位清除为"0" 当通道的传输结束后。即便外部或外设传输请求有效，暂停状态通道
 *       忽略传输请求。如果是块传输和突发传输，相关通道不会开始传输，即使是清除了暂停位。
 *       为了完成暂停的传输，在清除暂停后发送另外的传输请求。
 *
 *       该位可用于暂停传输而不复位相关通道的配置寄存器。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_stop(amhw_zsn700_dma_t *p_hw_dma,
                                 uint8_t          channel)
{
    p_hw_dma->chcfg[channel].dma_confa |= (0x1ul << 30);
}

/**
 * \brief DMA PAS位控制各自通道的传输暂停或取消        开始
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note 当该位置 "1" ，相关正在传输的通道进入暂停状态。清除该位到 0，从新开始传输。
 *       该位清除为"0" 当通道的传输结束后。即便外部或外设传输请求有效，暂停状态通道
 *       忽略传输请求。如果是块传输和突发传输，相关通道不会开始传输，即使是清除了暂停位。
 *       为了完成暂停的传输，在清除暂停后发送另外的传输请求。
 *
 *       该位可用于暂停传输而不复位相关通道的配置寄存器。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_start(amhw_zsn700_dma_t *p_hw_dma,
                                  uint8_t          channel)
{
    p_hw_dma->chcfg[channel].dma_confa &= ~(0x1ul << 30);
}

/**
 * \brief DMA ST 位用于产生单独通道的软件传输请求
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] stat       : 待设值
 *
 * \note 当该位置"1", 软件传输触发产生，相关通道开始传输。传输完成后， DMA清除该位至"0"。
 *       当该位在传输中置"0"， 传输停止。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_soft_transfer_ask(amhw_zsn700_dma_t *p_hw_dma,
                                            uint8_t            channel,
                                            uint8_t            stat)
{
    p_hw_dma->chcfg[channel].dma_confa =
        (p_hw_dma->chcfg[channel].dma_confa & (~(0x1ul << 29))) |
        ((stat & 0x1ul) << 29) ;
}

/**
 * \brief DMA TRI_SEL[6:0]位选择传输请求的触发源
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] src        : 传输请求设定（请查看zsn700_dma_chan.h宏定义）
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_trigger_src_set(amhw_zsn700_dma_t *p_hw_dma,
                                          uint8_t            channel,
                                          uint8_t            src)
{
    p_hw_dma->chcfg[channel].dma_confa =
        (p_hw_dma->chcfg[channel].dma_confa & (~(0x7ful << 22))) |
        ((src & 0x7ful) << 22) ;
}

/**
 * \brief DMA BC[3:0]位指定块/突发传输的个数
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] num        : 待设值
 *
 * \note 设置为请求传输时，设置"0000"。
 *       设置"BC[3:0]=块数量 - 1"。最大允许传输的块数为 16。
 *
 *       这几位在传输的工程中可以去读。通常情况，当一个源地址访问，或者一个目的地址的访问成功完成，
 *       BC[3:0] 减去 1。
 *
 *       在 CONFB:RC=1 的情况下：传输开始时设置的值，在传输完成后重载。
 *       在 CONFB:RC=0 的情况下：成功完成传输后，该位清零，同时在传输暂停引起的传输非成功完成时，该值保持不变。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_bc_set(amhw_zsn700_dma_t *p_hw_dma,
                                 uint8_t            channel,
                                 uint8_t            num)
{
    p_hw_dma->chcfg[channel].dma_confa =
        (p_hw_dma->chcfg[channel].dma_confa & (~(0xful << 16))) |
        ((num & 0xful) << 16);
}

/**
 * \brief DMA TC[15:0] 位用于指定块/突发/请求传输的传输次数。
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] num        : 待设值
 *
 * \note 设定值"TC = 传输数量 - 1". 最大传输数为 65536。
 *
 *       传输时，这些值是可以读的。正常情况，当一次块传输完成， TC 减去 1
 *
 *       在 CONFB:RC=1 的情况下：传输开始时设置的值，在传输完成后重载。
 *       在 CONFB:RC=0 的情况下：成功完成传输后，该位清零，同时在传输暂停引起的传输非成功完成时，该值保持不变。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_tran_data_num_set (amhw_zsn700_dma_t *p_hw_dma,
                                             uint8_t            channel,
                                             uint16_t           num)
{
    p_hw_dma->chcfg[channel].dma_confa =
        (p_hw_dma->chcfg[channel].dma_confa & (~(0xfffful << 0))) |
        ((num & 0xfffful) << 0);
}

/**
 * \brief DMA TC[15:0] 位用于指定块/突发/请求传输的传输次数。     获取
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note 设定值"TC = 传输数量 - 1". 最大传输数为 65536。
 *
 *       传输时，这些值是可以读的。正常情况，当一次块传输完成， TC 减去 1
 *
 *       在 CONFB:RC=1 的情况下：传输开始时设置的值，在传输完成后重载。
 *       在 CONFB:RC=0 的情况下：成功完成传输后，该位清零，同时在传输暂停引起的传输非成功完成时，该值保持不变。
 *
 * \return 当前传输剩余的字节数
 */
am_static_inline
uint16_t amhw_zsn700_dma_chan_tran_data_num_get (amhw_zsn700_dma_t *p_hw_dma,
                                                 uint8_t            channel)
{
    return ( p_hw_dma->chcfg[channel].dma_confa & 0xfffful);
}


#define  AMHW_ZSN700_DMA_CHAN_TRANSFER_MODE_BLOCK    (0UL << 28) /**< \brief Block传输模式 */
#define  AMHW_ZSN700_DMA_CHAN_TRANSFER_MODE_BURST    (1UL << 28) /**< \brief Burst传输模式 */

#define  AMHW_ZSN700_DMA_CHAN_SIZE_8BIT              (0UL << 26) /**< \brief 数据宽度 8bit */
#define  AMHW_ZSN700_DMA_CHAN_SIZE_16BIT             (1UL << 26) /**< \brief 数据宽度 16bit */
#define  AMHW_ZSN700_DMA_CHAN_SIZE_32BIT             (2UL << 26) /**< \brief 数据宽度 32bit */

#define  AMHW_ZSN700_DMA_CHAN_SRC_ADD_INC_DISABLE    (1UL << 25) /**< \brief 源地址自增禁能 */
#define  AMHW_ZSN700_DMA_CHAN_SRC_ADD_INC_ENABLE     (0UL << 25) /**< \brief 源地址自增使能 */

#define  AMHW_ZSN700_DMA_CHAN_DST_ADD_INC_DISABLE    (1UL << 24) /**< \brief 目标地址自增禁能  */
#define  AMHW_ZSN700_DMA_CHAN_DST_ADD_INC_ENABLE     (0UL << 24) /**< \brief 目标地址自增使能  */

#define  AMHW_ZSN700_DMA_CHAN_RELOAD_COUNTER_DISABLE (0UL << 23) /**< \brief 传输数据量重载禁能  */
#define  AMHW_ZSN700_DMA_CHAN_RELOAD_COUNTER_ENABLE  (1UL << 23) /**< \brief 传输数据量重载使能  */

#define  AMHW_ZSN700_DMA_CHAN_RELOAD_SRC_ADD_DISABLE (0UL << 22) /**< \brief 源地址重载禁能  */
#define  AMHW_ZSN700_DMA_CHAN_RELOAD_SRC_ADD_ENABLE  (1UL << 22) /**< \brief 源地址重载使能  */

#define  AMHW_ZSN700_DMA_CHAN_RELOAD_DST_ADD_DISABLE (0UL << 21) /**< \brief 目标地址重载禁能  */
#define  AMHW_ZSN700_DMA_CHAN_RELOAD_DST_ADD_ENABLE  (1UL << 21) /**< \brief 目标地址重载使能  */

#define  AMHW_ZSN700_DMA_CHAN_INT_ERR_DISABLE        (0UL << 20) /**< \brief 传输错误中断禁能  */
#define  AMHW_ZSN700_DMA_CHAN_INT_ERR_ENABLE         (1UL << 20) /**< \brief 传输错误中断使能  */

#define  AMHW_ZSN700_DMA_CHAN_INT_TX_CMP_DISABLE     (0UL << 19) /**< \brief 完成中断禁能 */
#define  AMHW_ZSN700_DMA_CHAN_INT_TX_CMP_ENABLE      (1UL << 19) /**< \brief 完成中断使能 */

#define  AMHW_ZSN700_DMA_CHAN_CIRCULAR_MODE_DISABLE  (0UL << 0)  /**< \brief 循环模式禁能 */
#define  AMHW_ZSN700_DMA_CHAN_CIRCULAR_MODE_ENABLE   (1UL << 0)  /**< \brief 循环模式使能 */

/**
 * \brief DMA 通道设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] num        : 设置标志
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_config_set(amhw_zsn700_dma_t *p_hw_dma,
                                     uint8_t            channel,
                                     uint32_t           flag)

{
    p_hw_dma->chcfg[channel].dma_confb =
        (p_hw_dma->chcfg[channel].dma_confb & (~0x3ff8fffful)) |
        flag;
}


#define  AMHW_ZSN700_DMA_CHAN_INT_ERR      (1UL << 20) /**< \brief 未完成中断  */
#define  AMHW_ZSN700_DMA_CHAN_INT_TX_CMP   (1UL << 19) /**< \brief 完成中断 */
/**
 * \brief DMA 某一通道中断使能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] num        : 设置标志
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_int_enable(amhw_zsn700_dma_t *p_hw_dma,
                                     uint8_t            channel,
                                     uint32_t           flag)
{
    p_hw_dma->chcfg[channel].dma_confb |= flag;
}

/**
 * \brief DMA 某一通道中断禁能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] num        : 设置标志
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_int_disable(amhw_zsn700_dma_t *p_hw_dma,
                                      uint8_t            channel,
                                      uint32_t           flag)
{
    p_hw_dma->chcfg[channel].dma_confb &= ~flag;
}

/**
 * \brief DMA 状态标志
 *@{
 */
typedef enum {
    AMHW_ZSN700_DMA_FLAG_DEFAULT = 0,               /* 初始默认值 */
    AMHW_ZSN700_DMA_FLAG_ERR_ADDR_OVERFLOW,         /* 地址溢出传输错误 */
    AMHW_ZSN700_DMA_FLAG_STOP_ASK,                  /* 传输停止请求引起中止 */
    AMHW_ZSN700_DMA_FLAG_ERR_SRCADDR,               /* 传输源地址访问错误 */
    AMHW_ZSN700_DMA_FLAG_ERR_DSTADDR,               /* 传输目的地地址访问错误 */
    AMHW_ZSN700_DMA_FLAG_TX_COMPLETE,               /* 成功传输完成 */
    AMHW_ZSN700_DMA_FLAG_RESERVE,                   /* 保留 */
    AMHW_ZSN700_DMA_FLAG_TX_STOP,                   /* 传输暂停 */
}amhw_zsn700_dma_flag_t;


/**
 * \brief 状态标志获取
 *
 * \param[in] p_hw_dma : 指向DMA寄存器结构体的指针
 * \param[in] channel  : 通道号
 * \param[in] flag     : 状态标志
 *
 * \return AM_TRUE or AM_FALSE
 */
am_static_inline
am_bool_t amhw_zsn700_dma_chan_stat_check (amhw_zsn700_dma_t     *p_hw_dma,
                                           uint8_t                channel,
                                           amhw_zsn700_dma_flag_t flag)
{
    return ((((p_hw_dma->chcfg[channel].dma_confb >> 16) & 0x7ul) == flag) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 状态标志清除
 *
 * \param[in] p_hw_dma    : 指向DMA寄存器结构体的指针
 * \param[in] channel     : 通道号
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_flag_clear (amhw_zsn700_dma_t *p_hw_dma,
                                      uint8_t            channel)
{
    p_hw_dma->chcfg[channel].dma_confb &= ~(0x7ul << 16);
}

/**
 * \brief 源地址设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] address    : 地址
 *
 * \note  如果 CONFB:FS=“1”，传输源地址设定为固定值，并且不会改变。
 *
 *        如果 CONFB:FS=“0”， 且 CONFB:RS=“0”，传输时源地址根据 CONFB:WIDTH[1:0]的数据位宽自增。
 *        如果传输成功完成，这个值为传输完成地址的下一地址。如果传输未成功完成，这个值为暂停时所设定的值
 *
 *        如果 CONFB:FS=“0”且 CONFB:RS=“1”，传输时源地址根据 CONFB:WIDTH[1:0]的数据位宽自增。
 *        传输完成后，传输开始时设定的源地址重新加载到本寄存器位中。
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_src_address_set(amhw_zsn700_dma_t *p_hw_dma,
                                          uint8_t            channel,
                                          uint32_t           address)
{
    p_hw_dma->chcfg[channel].dma_srcadr = address;
}

/**
 * \brief 目标地址设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] address    : 地址
 *
 * \note  如果 CONFB:FD=“1”， 传输目的地址设置为固定地址，并且不改变。
 *
 *        如果 CONFB:FD=“0”， 且 CONFB:RD=“0”, 传输时目的地址根据 CONFB:WIDTH[1:0]的数据位宽
 *        自增。如果传输成功完成，这个值为传输完成地址的下一地址。如果传输未成功完成，这个值为暂停时所设定的值。
 *
 *        如果 CONFB:FD=“0” 且 CONFB:RD=“1”, 传输时目的地址根据 CONFB:WIDTH[1:0]的数据位宽
 *        自增。传输完成后，传输开始时设定的目的地址重新加载到本寄存器位中。

 * \return none
 */
am_static_inline
void amhw_zsn700_dma_chan_dst_address_set(amhw_zsn700_dma_t *p_hw_dma,
                                          uint8_t            channel,
                                          uint32_t           address)
{
    p_hw_dma->chcfg[channel].dma_dstadr = address;
}

/**
 * @} amhw_if_zsn700_dma
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_ZSN700_DMA_H */

/* end of file */
