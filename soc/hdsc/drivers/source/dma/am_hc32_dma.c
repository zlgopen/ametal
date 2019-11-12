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
 * - 1.00 19-07-17  zp, first implementation
 * \endinternal
 */

#include "am_hc32_dma.h"
#include "am_int.h"
#include "am_vdebug.h"

/*******************************************************************************
  私有定义
*******************************************************************************/


/** \brief 中断未连接标识 */
#define __INT_NOT_CONNECTED      0xFF

/** \brief 通道未连接标识 */
#define __CHAN_NOT_CONNECTED     0xFF

/** \brief 定义指向DMA设备信息的指针 */
#define __DMA_DEVINFO_DECL(p_dma_devinfo, p_dev)  \
        const am_hc32_dma_devinfo_t *p_dma_devinfo = p_dev->p_devinfo

/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief 指向DMA设备的指针 */
static am_hc32_dma_dev_t *__gp_dma_dev;

/** \brief DMA中断回调函数信息数组 */
static struct am_hc32_dma_int_info __dma_int_info[AMHW_HC32_DMA_CHAN_CNT];

/** \brief DMA中断回调函数信息映射 */
static uint8_t __dma_int_map[AMHW_HC32_DMA_CHAN_CNT];


/*******************************************************************************
  公共函数
*******************************************************************************/

/* 建立传输描述符 */
int am_hc32_dma_xfer_desc_build (amhw_hc32_dma_xfer_desc_t *p_desc,
                                   uint32_t                     src_addr,
                                   uint32_t                     dst_addr,
                                   uint32_t                     nbytes,
                                   uint32_t                     flags)
{
    if (p_desc == NULL) {
        return AM_ERROR;
    }

    if (nbytes > 0xFFFF) {
        return AM_ERROR;
    }

    p_desc->xfercfg  = flags;
    p_desc->src_addr = src_addr;
    p_desc->dst_addr = dst_addr;
    p_desc->nbytes   = nbytes;

    return AM_OK;
}

/* 开始DMA传输 */
int am_hc32_dma_xfer_desc_chan_cfg (amhw_hc32_dma_xfer_desc_t    *p_desc,
                                      amhw_hc32_dma_transfer_type_t type,
                                      uint8_t                         chan)
{

    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)
                                  p_dma_devinfo->dma_reg_base;

    if (p_desc == NULL) {
        return AM_ERROR;
    }

    if (chan > AMHW_HC32_DMA_CHAN_CNT) {
        return AM_ERROR;
    }

    switch(type) {

    case AMHW_HC32_DMA_PER_TO_MER:
    case AMHW_HC32_DMA_MER_TO_PER:
    case AMHW_HC32_DMA_MER_TO_MER:
    case AMHW_HC32_DMA_PER_TO_PER:

        amhw_hc32_dma_chan_src_address_set(p_hw_dma,
                                             chan,
                                             p_desc->src_addr);

        amhw_hc32_dma_chan_dst_address_set(p_hw_dma,
                                             chan,
                                             p_desc->dst_addr);
        break;

    default:
        break;
    }

    amhw_hc32_dma_chan_tran_data_num_set(p_hw_dma,
                                           chan,
                                           p_desc->nbytes - 1);

    amhw_hc32_dma_chan_config_set(p_hw_dma,
                                    chan,
                                    p_desc->xfercfg);

    return AM_OK;
}

/* 通道优先级设定，通道0优先级最高 */
void am_hc32_dma_priority_ch0_highest (void)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_priority_set(p_hw_dma, AMHW_HC32_DMA_CHAN_PRIORITY_CH0_BEST);
}

/* 通道优先级设定，通道优先级相同（轮询） */
void am_hc32_dma_priority_same (void)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_priority_set(p_hw_dma, AMHW_HC32_DMA_CHAN_PRIORITY_POLL);
}

/* 设定通道请求的触发源（请查看hc32_dma_chan.h宏定义）*/
void am_hc32_dma_chan_src_set (int chan, uint8_t src)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_chan_trigger_src_set(p_hw_dma, chan, src);
}

/* 开始通道传输 */
int am_hc32_dma_chan_start (int chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_enable(p_hw_dma);

    amhw_hc32_dma_chan_enable(p_hw_dma, chan);

    return AM_OK;
}

/* 停止通道传输 */
int am_hc32_dma_chan_stop (int chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_chan_int_disable(p_hw_dma,
                                     chan,
                                     AMHW_HC32_DMA_CHAN_INT_TX_CMP);

    amhw_hc32_dma_disable(p_hw_dma);

    amhw_hc32_dma_chan_disable(p_hw_dma, chan);

    return AM_OK;
}

/* 触发源为软件是，软件请求产生 */
/* 软件传输触发产生，相关通道开始传输。传输完成后， DMA清除该位。当该位在传输中置"0"， 传输停止。*/
void am_hc32_dma_chan_soft_ask_start (int chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_chan_soft_transfer_ask(p_hw_dma, chan, 1);
}

/* 触发源为软件是，软件请求停止 */
/* 软件传输触发产生，相关通道开始传输。传输完成后， DMA清除该位。当该位在传输中置"0"， 传输停止。*/
void am_hc32_dma_chan_soft_ask_stop (int chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);

    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_chan_soft_transfer_ask(p_hw_dma, chan, 0);
}

/* DMA中断处理函数 */
static void __dma_int_handler (void *p_arg)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    int      i;
    int      chan = 0xFF;
    uint8_t  flag = 0xFF;

    am_hc32_pfn_dma_isr_t  pfn_isr;
    void                    *p_isr_arg;

    for (i = 0 ; i < AMHW_HC32_DMA_CHAN_CNT; i++) {
        if (amhw_hc32_dma_chan_stat_check(
                p_hw_dma, i, AMHW_HC32_DMA_FLAG_TX_COMPLETE) == AM_TRUE){

            chan = i;
            flag = AM_HC32_DMA_INT_COMPLETE;
            amhw_hc32_dma_chan_flag_clear(p_hw_dma, i);

        } else if (amhw_hc32_dma_chan_stat_check(
                p_hw_dma, i, AMHW_HC32_DMA_FLAG_ERR_ADDR_OVERFLOW) == AM_TRUE){

            chan = i;
            flag = AM_HC32_DMA_INT_ERR_ADDR_OVERFLOW;
            amhw_hc32_dma_chan_flag_clear(p_hw_dma, i);

        } else if (amhw_hc32_dma_chan_stat_check(
                p_hw_dma, i, AMHW_HC32_DMA_FLAG_STOP_ASK) == AM_TRUE){

            chan = i;
            flag = AM_HC32_DMA_INT_ERR_STOPASK;
            amhw_hc32_dma_chan_flag_clear(p_hw_dma, i);

        } else if (amhw_hc32_dma_chan_stat_check(
                p_hw_dma, i, AMHW_HC32_DMA_FLAG_ERR_SRCADDR) == AM_TRUE){

            chan = i;
            flag = AM_HC32_DMA_INT_ERR_SRCADDR;
            amhw_hc32_dma_chan_flag_clear(p_hw_dma, i);

        } else if (amhw_hc32_dma_chan_stat_check(
                p_hw_dma, i, AMHW_HC32_DMA_FLAG_ERR_DSTADDR) == AM_TRUE) {

            chan = i;
            flag = AM_HC32_DMA_INT_ERR_DSTADDR;
            amhw_hc32_dma_chan_flag_clear(p_hw_dma, i);

        } else if (amhw_hc32_dma_chan_stat_check(
                p_hw_dma, i, AMHW_HC32_DMA_FLAG_TX_STOP) == AM_TRUE) {

            chan = i;
            flag = AM_HC32_DMA_INT_ERR_STOP;
            amhw_hc32_dma_chan_flag_clear(p_hw_dma, i);
        }

        if (0xFF != chan) {

            pfn_isr   = __dma_int_info[chan].pfn_isr;
            p_isr_arg = __dma_int_info[chan].p_arg;

            if (pfn_isr) {
                pfn_isr(p_isr_arg , flag);
            }
        }
    }
}

/* 连接DMA中断服务函数 */
int am_hc32_dma_isr_connect (int                        chan,
                               am_hc32_pfn_dma_isr_t    pfn_isr,
                                void                     *p_arg)
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
int am_hc32_dma_isr_disconnect (int                     chan,
                                  am_hc32_pfn_dma_isr_t pfn_isr,
                                  void                   *p_arg)
{
    if (__dma_int_map[chan] == chan) {
        __dma_int_info[chan].pfn_isr = NULL;
        __dma_int_map[chan]          = __INT_NOT_CONNECTED;
    } else {
        return -AM_EPERM;
    }

    return AM_OK;
}

/**
 * \breif 获取某通道当前传输剩余的字节数
 */
uint16_t am_hc32_dma_tran_data_get (int chan)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    return amhw_hc32_dma_chan_tran_data_num_get(p_hw_dma, chan) + 1;
}

/**
 * \brief 设置传输的字节数
 */
void am_hc32_dma_tran_data_size (int chan, uint32_t trans_data_byte)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_chan_tran_data_num_set(p_hw_dma,
                                           chan,
                                           trans_data_byte - 1);
}

/**
 * \brief 块数据数量
 */
void am_hc32_dma_block_data_size (int chan, uint8_t block_data_num)
{
    __DMA_DEVINFO_DECL(p_dma_devinfo, __gp_dma_dev);
    amhw_hc32_dma_t *p_hw_dma = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    amhw_hc32_dma_chan_bc_set(p_hw_dma,
                                chan,
                                block_data_num - 1);
}

/* DMA初始化 */
int am_hc32_dma_init (am_hc32_dma_dev_t           *p_dev,
                        const am_hc32_dma_devinfo_t *p_devinfo)
{
    uint32_t i = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return AM_ERROR;
    }

    p_dev->p_devinfo = p_devinfo;
    __gp_dma_dev     = p_dev;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    for (i = 0; i < AMHW_HC32_DMA_CHAN_CNT; i++) {
        __dma_int_info[i].pfn_isr = NULL;
        __dma_int_map[i]          = __INT_NOT_CONNECTED;
    }

    am_int_connect(p_devinfo->inum, __dma_int_handler, (void *)0);
    am_int_enable(p_devinfo->inum);

    return AM_OK;
}

/* DMA去初始化 */
void am_hc32_dma_deinit (void)
{
    am_hc32_dma_devinfo_t *p_dma_devinfo = NULL;
    amhw_hc32_dma_t       *p_hw_dma;
    
    int i = 0;

    if (NULL == __gp_dma_dev) {
        return;
    }

    p_dma_devinfo = (am_hc32_dma_devinfo_t *)__gp_dma_dev->p_devinfo;
    p_hw_dma      = (amhw_hc32_dma_t *)p_dma_devinfo->dma_reg_base;

    for (i = 0; i < AMHW_HC32_DMA_CHAN_CNT; i++) {
        __dma_int_info[i].pfn_isr = NULL;
        __dma_int_map[i]          = 0;
        amhw_hc32_dma_chan_disable(p_hw_dma , i);
    }

    am_int_disconnect(p_dma_devinfo->inum, __dma_int_handler, (void *)0);

    am_int_disable(p_dma_devinfo->inum);


    if (p_dma_devinfo->pfn_plfm_deinit) {
        p_dma_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
