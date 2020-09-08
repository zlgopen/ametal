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
 * 1. 5 independent programmable DMA channel.
 * 2. 8,16,32 bits data to transfer.
 * 3. support programmable source address,destination address and data size。
 *
 * 存在一个 硬件配置项： AMHW_STM32F103RBT6_DMA_CHAN_CNT。用以配置 DMA通道数目，使用本文件时，
 * 应在 soc_cfg.h 文件中定义该宏。
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-10  ari, first implementation
 * \endinternal
 */

#ifndef __AMHW_STM32F103RBT6_DMA_H
#define __AMHW_STM32F103RBT6_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"
#include "soc_cfg.h"
#include "am_vdebug.h"
/**
 * \addtogroup amhw_stm32f103rbt6_if_dma
 * \copydoc amhw_stm32f103rbt6_dma.h
 * @{
 */

/** \brief DMA 通道数目  */
#define AMHW_STM32F103RBT6_DMA_CHAN_CNT  SOC_CFG_STM32F103RBT6_DMA_CHAN_CNT

/**
 * \brief structure of DMA controller register
 */
typedef struct amhw_stm32f103rbt6_dma_chan {
    __IO uint32_t  dma_isr;           /**< \brief DMA interrupt status register */
    __IO uint32_t  dma_ifcr;          /**< \brief DMA interrupt flag clear register */
    struct {                        
        __IO uint32_t  dma_ccr;       /**< \brief DMA channel configuration register */
        __IO uint32_t  dma_cndtr;     /**< \brief DMA channel Number of data to transfer */
        __IO uint32_t  dma_cpar;      /**< \brief DMA channel peripheral address */
        __IO uint32_t  dma_cmar;      /**< \brief DMA channel memory address */
        uint32_t       reserved[1];   /**< \brief Reserved */
    } chcfg[AMHW_STM32F103RBT6_DMA_CHAN_CNT];   /**< \brief DMA Channel Struct */

} amhw_stm32f103rbt6_dma_t;

/**
 * \brief DMA transfer mode select
 */
typedef enum amhw_stm32f103rbt6_dma_transfer_type {
    AMHW_STM32F103RBT6_DMA_PER_TO_MER,     /**< \brief DMA peripheral to memory mode */
    AMHW_STM32F103RBT6_DMA_MER_TO_PER,     /**< \brief DMA memory to peripheral mode */
    AMHW_STM32F103RBT6_DMA_MER_TO_MER,     /**< \brief DMA memory to memory mode */
    AMHW_STM32F103RBT6_DMA_PER_TO_PER      /**< \brief DMA peripheral to peripheral mode */
} amhw_stm32f103rbt6_dma_transfer_type_t;

/**
 * \brief DMA configuration
 *@{
 */
#define  AMHW_STM32F103RBT6_DMA_CHAN_MEM_TO_MEM_DISABLE    (0UL << 14) /**< \brief DMA memory to memory mode disable */
#define  AMHW_STM32F103RBT6_DMA_CHAN_MEM_TO_MEM_ENABLE     (1UL << 14) /**< \brief DMA memory to memory mode enable */

#define  AMHW_STM32F103RBT6_DMA_CHAN_PRIORITY_LOW          (0UL << 12) /**< \brief DMA channel priority low */
#define  AMHW_STM32F103RBT6_DMA_CHAN_PRIORITY_MIDDLE       (1UL << 12) /**< \brief DMA channel priority middle */
#define  AMHW_STM32F103RBT6_DMA_CHAN_PRIORITY_HIGH         (2UL << 12) /**< \brief DMA channel priority high */
#define  AMHW_STM32F103RBT6_DMA_CHAN_PRIORITY_HIGHEST      (3UL << 12) /**< \brief DMA channel priority highest */

#define  AMHW_STM32F103RBT6_DMA_CHAN_MEM_SIZE_8BIT         (0UL << 10) /**< \brief memory size 8 bit */
#define  AMHW_STM32F103RBT6_DMA_CHAN_MEM_SIZE_16BIT        (1UL << 10) /**< \brief memory size 16 bit */
#define  AMHW_STM32F103RBT6_DMA_CHAN_MEM_SIZE_32BIT        (2UL << 10) /**< \brief memory size 32 bit */

#define  AMHW_STM32F103RBT6_DMA_CHAN_PER_SIZE_8BIT         (0UL << 8) /**< \brief peripheral size 8 bit */
#define  AMHW_STM32F103RBT6_DMA_CHAN_PER_SIZE_16BIT        (1UL << 8) /**< \brief peripheral size 16 bit */
#define  AMHW_STM32F103RBT6_DMA_CHAN_PER_SIZE_32BIT        (2UL << 8) /**< \brief peripheral size 32 bit */

#define  AMHW_STM32F103RBT6_DMA_CHAN_MEM_ADD_INC_DISABLE   (0UL << 7) /**< \brief memory address increment disable */
#define  AMHW_STM32F103RBT6_DMA_CHAN_MEM_ADD_INC_ENABLE    (1UL << 7) /**< \brief memory address increment enable */

#define  AMHW_STM32F103RBT6_DMA_CHAN_PER_ADD_INC_DISABLE   (0UL << 6) /**< \brief peripheral address increment disable */
#define  AMHW_STM32F103RBT6_DMA_CHAN_PER_ADD_INC_ENABLE    (1UL << 6) /**< \brief peripheral address increment enable */

#define  AMHW_STM32F103RBT6_DMA_CHAN_CIRCULAR_MODE_DISABLE (0UL << 5) /**< \brief DMA circular mode disable */
#define  AMHW_STM32F103RBT6_DMA_CHAN_CIRCULAR_MODE_ENABLE  (1UL << 5) /**< \brief DMA circular mode enable */

#define  AMHW_STM32F103RBT6_DMA_CHAN_DIR_FROM_PER          (0UL << 4) /**< \brief DMA Read form peripheral */
#define  AMHW_STM32F103RBT6_DMA_CHAN_DIR_FROM_MEM          (1UL << 4) /**< \brief DMA Read form memory */

#define  AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_ERR_DISABLE     (0UL << 3) /**< \brief DMA transfer error interrupt disable */
#define  AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_ERR_ENABLE      (1UL << 3) /**< \brief DMA transfer error interrupt enable */

#define  AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_HALF_DISABLE    (0UL << 2) /**< \brief DMA half transfer interrupt disable */
#define  AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_HALF_ENABLE     (1UL << 2) /**< \brief DMA half transfer interrupt enable */

#define  AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_CMP_DISABLE     (0UL << 1) /**< \brief DMA transfer complete interrupt disable */
#define  AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_CMP_ENABLE      (1UL << 1) /**< \brief DMA transfer complete interrupt enable */

/**@}*/

/**
 * \brief DMA descriptor
 */
typedef struct amhw_stm32f103rbt6_dma_xfer_desc {
    volatile uint32_t          xfercfg;        /**< \brief transfer configuration */
    volatile uint32_t          src_addr;       /**< \brief source address */
    volatile uint32_t          dst_addr;       /**< \brief destination address */
    volatile uint32_t          nbytes;         /**< \brief the bytes of data to transfer */
} amhw_stm32f103rbt6_dma_xfer_desc_t;

/**
 * \brief DMA interrupt status
 *@{
 */
/** \brief Channel x transfer error flag */
#define  AMHW_STM32F103RBT6_DMA_CHAN_TX_ERR_FLAG(n)        (1u << ((n) * 4 + 3))

/** \brief Channel x  half transfer flag */
#define  AMHW_STM32F103RBT6_DMA_CHAN_TX_HALF_FLAG(n)       (1u << ((n) * 4 + 2))

/** \brief Channel x transfer complete flag */
#define  AMHW_STM32F103RBT6_DMA_CHAN_TX_COMP_FLAG(n)       (1u << ((n) * 4 + 1))

/** \brief Channel x global interrupt flag */
#define  AMHW_STM32F103RBT6_DMA_CHAN_GLOBAL_INT_FLAG(n)    (1u << ((n) * 4 + 0))
/**@}*/




/**
 * \brief Interrupt flag check
 *
 * \param[in] p_hw_dma    : The pointer to the structure of DMA register
 * \param[in] flag        : see the filed of DMA interrupt status
 *
 * \return AM_TRUE or AM_FALSE
 */
am_static_inline
am_bool_t amhw_stm32f103rbt6_dma_chan_stat_check (amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                        uint32_t        flag)
{
    return ((p_hw_dma->dma_isr & flag) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief Interrupt flag clear
 *
 * \param[in] p_hw_dma    : The pointer to the structure of DMA register
 * \param[in] flag        : see the filed of DMA interrupt status
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_flag_clear (amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                   uint32_t        flag)
{
    p_hw_dma->dma_ifcr |= flag;
}

/**
 * \brief DMA channel configuration
 *
 * \param[in] p_hw_dma   : The pointer to the structure of DMA register
 * \param[in] flag       : see the filed of DMA configuration
 * \param[in] channel    : DMA channel
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_config_set(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                  uint32_t        flag,
                                  uint8_t         channel)
{
    p_hw_dma->chcfg[channel].dma_ccr = (p_hw_dma->chcfg[channel].dma_ccr & (~0x7ffe)) | flag;
}

/**
 * \brief DMA channel Interrupt flag
 * \@{
 */

#define AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_ERR_MASK   (1u << 3)  /**< \brief transfer error interrupt */
#define AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_HALF_MASK  (1u << 2)  /**< \brief half transfer interrupt */
#define AMHW_STM32F103RBT6_DMA_CHAN_INT_TX_CMP_MASK   (1u << 1)  /**< \brief transfer complete  interrupt */
/** }@ */
/**
 * \brief DMA channel Interrupt enable
 *
 * \param[in] p_hw_dma   : The pointer to the structure of DMA register
 * \param[in] flag       : see the filed of DMA channel Interrupt flag
 * \param[in] channel    : DMA channel
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_int_enable(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                  uint32_t        flag,
                                  uint8_t         channel)
{
    p_hw_dma->chcfg[channel].dma_ccr |= flag;
}

/**
 * \brief DMA channel Interrupt disable
 *
 * \param[in] p_hw_dma   : The pointer to the structure of DMA register
 * \param[in] flag       : see the filed of DMA channel Interrupt flag
 * \param[in] channel    : DMA channel
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_int_disable(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                   uint32_t        flag,
                                   uint8_t         channel)
{
    p_hw_dma->chcfg[channel].dma_ccr &= ~flag;
}

/**
 * \brief DMA channel  enable
 *
 * \param[in] p_hw_dma   : The pointer to the structure of DMA register
 * \param[in] channel    : DMA channel
 * \param[in] flag       : AM_FALSE or AM_TRUE
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_enable(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                              uint8_t         channel,
                              am_bool_t       flag)
{
    p_hw_dma->chcfg[channel].dma_ccr = (p_hw_dma->chcfg[channel].dma_ccr & (~(1u << 0))) |
                                       flag << 0;
}
/**
 * \brief The number of data to transfer
 *
 * \param[in] p_hw_dma   : The pointer to the structure of DMA register
 * \param[in] size       : 0~65535
 * \param[in] channel    : DMA channel
 *
 * \note This register can be modified when DMA channel is disable(DMA_CCRx[EN] = 0)
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_tran_data_size(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                      uint32_t        size,
                                      uint8_t         channel)
{
    p_hw_dma->chcfg[channel].dma_cndtr = size;
}

/**
 * \brief The number of data to transfer
 *
 * \param[in] p_hw_dma The pointer to the structure of DMA register
 * \param[in] channel  DMA channel
 *
 * \return size
 *
 * \note This register can be modified when DMA channel is disable(DMA_CCRx[EN] = 0)
 */
am_static_inline
uint16_t amhw_stm32f103rbt6_dma_chan_tran_data_get(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                         uint8_t         channel)
{
    return p_hw_dma->chcfg[channel].dma_cndtr;
}

/**
 * \brief Peripheral address set
 *
 * \param[in] p_hw_dma   : The pointer to the structure of DMA register
 * \param[in] address    : peripheral address
 * \param[in] channel    : DMA channel
 *
 * \note This register can be modified when DMA channel is disable(DMA_CCRx[EN] = 0)
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_per_address_set(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                       uint32_t        address,
                                       uint8_t         channel)
{
    p_hw_dma->chcfg[channel].dma_cpar = address;
}

/**
 * \brief Memory address set
 *
 * \param[in] p_hw_dma   : The pointer to the structure of DMA register
 * \param[in] address    : Memory address
 * \param[in] channel    : DMA channel
 *
 * \note This register can be modified when DMA channel is disable(DMA_CCRx[EN] = 0)
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_dma_chan_mem_address_set(amhw_stm32f103rbt6_dma_t *p_hw_dma,
                                       uint32_t        address,
                                       uint8_t         channel)
{
    p_hw_dma->chcfg[channel].dma_cmar = address;
}

/**
 * @} amhw_if_stm32f103rbt6_dma
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_STM32F103RBT6_DMA_H */

/* end of file */
