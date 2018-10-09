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
 * \brief DMA 驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-23  mkr, first implementation.
 * \endinternal
 */

#include "am_kl26_dma.h"
#include "am_int.h"

/*******************************************************************************
  私有定义
*******************************************************************************/

/** \brief 中断未连接标识 */
#define __INT_NOT_CONNECTED      0xFF

/** \brief 通道未连接标识 */
#define __CHAN_NOT_CONNECTED     0xFF

/** \brief 定义指向DMA设备信息的指针 */
#define __DMA_DEVINFO_DECL(p_dma_devinfo, p_dev)  \
        const am_kl26_dma_devinfo_t *p_dma_devinfo = p_dev->p_devinfo


/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief 指向DMA设备的指针 */
am_kl26_dma_dev_t *__gp_dma_dev;

/** \brief DMA中断回调函数信息数组 */
static struct am_kl26_dma_int_info __dma_int_info[KL26_DMA_CHAN_CNT];

/** \brief DMA中断回调函数信息映射 */
static uint8_t __dma_int_map[KL26_DMA_CHAN_CNT];

/** \brief DMA通道信息映射 */
static uint8_t __dma_chan_map[KL26_DMA_CHAN_CNT];

/*******************************************************************************
  公共函数
*******************************************************************************/

/* 配置DMA传输通道以及请求源 */
int am_kl26_dma_chan_cfg (int chan , uint8_t flags)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    if (__dma_chan_map[chan] == __CHAN_NOT_CONNECTED) {
        amhw_kl26_dma_chan_source_set(p_dma_devinfo->p_hw_dmamux,
                                      flags & (~KL26_DMA_TRIGGER_ENABLE),
                                      chan);
        if (flags & KL26_DMA_TRIGGER_ENABLE) {
            amhw_kl26_dma_trig_enable(p_dma_devinfo->p_hw_dmamux, chan);
        } else {
            amhw_kl26_dma_trig_disable(p_dma_devinfo->p_hw_dmamux, chan);
        }

        __dma_chan_map[chan] = chan;
        return AM_OK;
    } else {
        return AM_ERROR;
    }
}

/* 建立传输描述符 */
int am_kl26_dma_xfer_desc_build (amhw_kl26_dma_xfer_desc_t *p_desc,
                                 volatile uint32_t          src_addr,
                                 uint32_t                   dst_addr,
                                 uint32_t                   nbytes,
                                 uint32_t                   flags)
{
    if (p_desc == NULL) {
        return AM_ERROR;
    }

    if (((src_addr >> 20) != 0x000) && ((src_addr >> 20) != 0x1ff) &&
        ((src_addr >> 20) != 0x200) && ((src_addr >> 20) != 0x400) &&
        ((dst_addr >> 20) != 0x000) && ((dst_addr >> 20) != 0x1ff) &&
        ((dst_addr >> 20) != 0x200) && ((dst_addr >> 20) != 0x400)) {
        return AM_ERROR;
    }

    if (nbytes >= 0xFFFFFF) {
        return AM_ERROR;
    }

    p_desc->xfercfg  = flags;
    p_desc->src_addr = src_addr;
    p_desc->dst_addr = dst_addr;
    p_desc->nbytes   = nbytes;
    
    return AM_OK;
}

/* 设置源地址缓冲区 */
void am_kl26_dma_xfer_source_buffer_set (amhw_kl26_dma_buffer_size_set_t size,
                                         uint8_t                         chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    amhw_kl26_dma_source_buffer_set (p_dma_devinfo->p_hw_dma,
                                     size,
                                     chan);
}

/* 设置目的地址缓冲区 */
void am_kl26_dma_xfer_desc_buffer_set (amhw_kl26_dma_buffer_size_set_t size,
                                       uint8_t                         chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    amhw_kl26_dma_destination_buffer_set (p_dma_devinfo->p_hw_dma,
                                          size,
                                          chan);
}

/* 开始DMA传输 */
int am_kl26_dma_chan_start (amhw_kl26_dma_xfer_desc_t     *p_desc,
                            amhw_kl26_dma_transfer_type_t  type,
                            uint8_t                        chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    int i;

    if (p_desc == NULL) {
        return AM_ERROR;
    }

    if (chan > KL26_DMA_CHAN_CNT) {
        return AM_ERROR;
    }

    switch(type) {

    case KL26_DMA_PER_TO_MER:
        p_desc->xfercfg |= (KL26_DMA_DCR_SOURCE_INCR_DISABLE |
                            KL26_DMA_DCR_DESTINATION_INCR_ENABLE);
        break;

    case KL26_DMA_MER_TO_PER:
        p_desc->xfercfg |= (KL26_DMA_DCR_SOURCE_INCR_ENABLE |
                            KL26_DMA_DCR_DESTINATION_INCR_DISABLE);
        break;

    case KL26_DMA_MER_TO_MER:
        p_desc->xfercfg |= (KL26_DMA_DCR_SOURCE_INCR_ENABLE |
                            KL26_DMA_DCR_DESTINATION_INCR_ENABLE);
        break;

    case KL26_DMA_PER_TO_PER:
        p_desc->xfercfg |= (KL26_DMA_DCR_SOURCE_INCR_DISABLE |
                            KL26_DMA_DCR_DESTINATION_INCR_DISABLE);
        break;

    default:
        break;
    }

    while (amhw_kl26_dma_busy_status_get(p_dma_devinfo->p_hw_dma, chan)) {
        for (i = 0 ; i < 100 ; i++) {
            if(!(amhw_kl26_dma_busy_status_get(p_dma_devinfo->p_hw_dma, chan))) {
                break;
            }
        }
        return AM_ERROR;
    }

//    amhw_kl26_dma_chan_enable(p_dma_devinfo->p_hw_dmamux, chan);
    amhw_kl26_dma_bcr_set(p_dma_devinfo->p_hw_dma,
                          p_desc->nbytes,
                          chan);
    amhw_kl26_dma_source_address_set(p_dma_devinfo->p_hw_dma,
                                     (uint32_t)(p_desc->src_addr),
                                     chan);
    amhw_kl26_dma_destination_address_set(p_dma_devinfo->p_hw_dma,
                                          (uint32_t)(p_desc->dst_addr),
                                          chan);
    amhw_kl26_dma_transfer_mode_set(p_dma_devinfo->p_hw_dma,
                                    p_desc->xfercfg,
                                    chan);

    amhw_kl26_dma_chan_enable(p_dma_devinfo->p_hw_dmamux, chan);

    return AM_OK;
}

/* 停止通道传输 */
int am_kl26_dma_chan_stop (int chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    while (amhw_kl26_dma_busy_status_get(p_dma_devinfo->p_hw_dma , chan));

    amhw_kl26_dma_chan_disable(p_dma_devinfo->p_hw_dmamux, chan);
    amhw_kl26_dma_status_clear(p_dma_devinfo->p_hw_dma, chan);

    return AM_OK;
}

/* DMA中断处理函数 */
static void __dma_int_handler (void *p_arg)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    amhw_kl26_dma_t *p_hw_dma = p_dma_devinfo->p_hw_dma;

    int      i;
    int      chan = 0xFF;
    uint8_t  flag = 0xFF;

    am_kl26_pfn_dma_isr_t  pfn_isr;
    void                  *p_isr_arg;

    for (i = 0 ; i < KL26_DMA_CHAN_CNT ; i++) {
        if (amhw_kl26_dma_complete_ststus_get(p_hw_dma , i)) {
            chan = i;
            if(amhw_kl26_dma_configuration_error_get(p_hw_dma , i))  {
                flag = AM_KL26_DMA_INT_ERROR ;
            } else if (amhw_kl26_dma_source_bus_error_get(p_hw_dma , i))  {
                flag = AM_KL26_DMA_INT_ERROR ;
            } else if (amhw_kl26_dma_destination_bus_error_get(p_hw_dma , i)) {
                flag = AM_KL26_DMA_INT_ERROR ;
            } else {
                flag = AM_KL26_DMA_INT_NORMAL;
            }

            break;
        }
    }
    
    if (0xFF != chan) {
        amhw_kl26_dma_status_clear(p_hw_dma, chan);

        pfn_isr   = __dma_int_info[chan].pfn_isr;
        p_isr_arg = __dma_int_info[chan].p_arg;

        if (pfn_isr) {
            pfn_isr(p_isr_arg , flag);
        }
    }
}

/* 连接DMA中断服务函数 */
int am_kl26_dma_isr_connect (int                    chan,
                             am_kl26_pfn_dma_isr_t  pfn_isr,
                             void                  *p_arg)
{
    if (__dma_int_map[chan] == __INT_NOT_CONNECTED) {
        __dma_int_info[chan].pfn_isr = pfn_isr;
        __dma_int_info[chan].p_arg   = p_arg;
        __dma_int_map[chan]          = chan;
    } else {
        return -AM_EPERM;
    }

    return AM_OK;
}

/* 删除DMA中断服务函数连接 */
int am_kl26_dma_isr_disconnect (int                    chan,
                                am_kl26_pfn_dma_isr_t  pfn_isr,
                                void                  *p_arg)
{
    if (__dma_int_map[chan] == chan) {
        __dma_int_info[chan].pfn_isr = NULL;
        __dma_int_map[chan]          = __INT_NOT_CONNECTED;
    } else {
        return -AM_EPERM;
    }

    return AM_OK;
}

/* DMA初始化 */
int am_kl26_dma_init (am_kl26_dma_dev_t           *p_dev,
                      const am_kl26_dma_devinfo_t *p_devinfo)
{
    uint32_t i;

    if (p_dev == NULL || p_devinfo == NULL) {
        return AM_ERROR;
    }

    p_dev->p_devinfo = p_devinfo;
    __gp_dma_dev     = p_dev;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    for (i = 0; i < KL26_DMA_CHAN_CNT; i++) {
        __dma_int_info[i].pfn_isr = NULL;
        __dma_int_map[i]          = __INT_NOT_CONNECTED;
        __dma_chan_map[i]         = __CHAN_NOT_CONNECTED;
    }

    am_int_connect(p_devinfo->inum,
                   __dma_int_handler,
                  (void *)0);

    am_int_enable(p_devinfo->inum);

    return AM_OK;
}

/* DMA去初始化 */
void am_kl26_dma_deinit (void)
{
    am_kl26_dma_devinfo_t *p_dma_devinfo = NULL;
    int i;

    if (NULL == __gp_dma_dev) {
        return;
    }

    p_dma_devinfo = (am_kl26_dma_devinfo_t *)__gp_dma_dev->p_devinfo;

    for (i = 0; i < KL26_DMA_CHAN_CNT; i++) {
        __dma_int_info[i].pfn_isr = NULL;
        __dma_int_map[i]          = 0;
        amhw_kl26_dma_chan_disable(p_dma_devinfo->p_hw_dmamux , i);
    }

    am_int_disable(p_dma_devinfo->inum);
    am_int_disconnect(p_dma_devinfo->inum,
                      __dma_int_handler,
                     (void *)0);

    if (p_dma_devinfo->pfn_plfm_deinit) {
        p_dma_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
