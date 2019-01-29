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
 * \brief 内存到内存的 DMA 传输例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. 如果 DMA 传输成功，LED0 长亮；
 *   2. 如果 DMA 传输失败，LED0 闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_lpc824_drv_dma_m2m.c src_lpc824_drv_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-30  mem,modified
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_drv_dma_m2m
 * \copydoc demo_lpc824_drv_dma_m2m.c
 */

/** [src_lpc824_drv_dma_m2m] */
#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x.h"
#include "am_lpc84x_dma.h"

/**
 * \name 内存拷贝状态标志,用于设置 m2m_result_t 中的 result 成员
 * \anchor grp_dam_m2m_result
 * @{
 */
#define __TRANS_OK      0
#define __TRANS_INIT    1
#define __TRANS_ERROR  -1
/**
 * @}
 */

/** \brief 内存拷贝结果 */
typedef struct m2m_result {
    am_lpc84x_dma_controller_t *p_ctr;  /**< \brief 通道控制器 */
    volatile int                result; /**< \brief 传输结果 */
} m2m_result_t;

/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg 用户自定义参数
 * \param[in] stat  DMA 传输完成状态标志，该参数的可能取值：
 *                      AM_LPC84X_DMA_STAT_INTA    A 中断发生
 *                      AM_LPC84X_DMA_STAT_INTB    B 中断发生
 *                      AM_LPC84X_DMA_STAT_INTERR  错误发生
 *
 * \return 无
 */
am_local void __dma_m2m_isr (void *p_arg, int stat)
{
    m2m_result_t *p_reslut = (m2m_result_t*)p_arg;

    /* DAM 传输完成 */
    if (stat & AM_LPC84X_DMA_STAT_INTERR) {
        p_reslut->result = __TRANS_ERROR;
    } else if (stat == AM_LPC84X_DMA_STAT_INTA) {
        p_reslut->result = __TRANS_OK;
    }

    /* 释放控制器 */
    am_lpc84x_dma_controller_release(p_reslut->p_ctr);
}

/**
 * \brief DMA 内存拷贝
 */
am_local int __dma_memory_copy (uint32_t      dst,
                                uint32_t      src,
                                uint16_t      nbytes,
                                m2m_result_t *p_result)
{
    uint8_t                     dma_chan = 0;
    am_lpc84x_dma_controller_t *p_ctr    = NULL;
    am_lpc84x_dma_transfer_t    transfer;

    /* 获取通道控制器 */
    do {
        p_ctr = am_lpc84x_dma_controller_get(dma_chan++, DMA_CHAN_OPT_PRIO_1);
    } while (p_ctr == NULL && dma_chan <= 17);

    if (p_ctr == NULL) {
        return AM_ERROR;
    }

    /* 构造传输结构体 */
    am_lpc84x_dma_transfer_build(
       &transfer,                           /* 传输结构 */
        src,                                /* 源地址 */
        dst,                                /* 目标地址 */
        nbytes,                             /* 传输大小 */
        AM_LPC84X_DMA_TRANS_WIDTH_8BIT  |   /* 传输宽度 */
        AM_LPC84X_DMA_TRANS_SRCINC_1X   |   /* 源地址增量 */
        AM_LPC84X_DMA_TRANS_DSTINC_1X);     /* 目标地址增量 */

    /* 传输数据 */
    p_result->p_ctr  = p_ctr;
    p_result->result = __TRANS_INIT;

    am_lpc84x_dma_transfer(p_ctr, &transfer, __dma_m2m_isr, (void*)p_result);

    return AM_OK;
}

void demo_lpc845_drv_dma_m2m_entry(uint8_t *p_src, int len)
{
    int i = 0;
    m2m_result_t trans_result;
  
    /* 申请空间 */
    uint8_t *p_dst = calloc(len, sizeof(uint8_t));

    
    
    /* 内存拷贝 */
    __dma_memory_copy((uint32_t)p_dst,
                      (uint32_t)p_src,
                      len,
                     &trans_result);

    /* 等待传输完成 */
    while (trans_result.result == __TRANS_INIT) {};

    /* 数据校验 */
    for (i = 0; i < len; i++) {

        /* 传输失败 */
        if (p_src[i] != p_dst[i]) {
            AM_FOREVER {
                am_led_on(LED0);
                am_mdelay(200);
                am_led_off(LED0);
                am_mdelay(200);
            }
        }
    }

    am_led_on(LED0);
    
    /* 释放内存空间 */
    free(p_dst);
    AM_FOREVER {
        ; /* VOID */
    }
}

/** [src_lpc824_drv_dma_m2m] */

/* end of file */
