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
 * \brief DMA内存到内存例程，由DMA驱动层实现
 *
 * - 实验现象：
 *   1. 传输成功则串口打印"transfer success"，否则打印"transfer failure"。
 *
 * \par 源代码
 * \snippet demo_kl26_dr_dma_m2m.c src_kl26_dr_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-18 mkr , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_dma_m2m
 * \copydoc demo_kl26_dr_dma_m2m.c
 */

/** [src_kl26_dr_dma_m2m] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_kl26_dma.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

am_bool_t volatile __g_trans_done;           /**< \brief 传输完成标志     */

/**
 * \brief 非乒乓传输时，描述符的地址不要求16字节对齐。
 */
amhw_kl26_dma_xfer_desc_t g_desc;

/**
 * \brief DMA中断服务程序。
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_kl26_dma_isr_connect() 函数传递。
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (#AM_KL26_DMA_INT_ERROR) 或 (#AM_KL26_DMA_INT_NORMAL)。

 * \return    无。
 */
static void dma_isr (void *p_arg , uint8_t flag)
{
    int flag_chan  = (int)p_arg;
    if (flag == AM_KL26_DMA_INT_NORMAL) {

        /* DMA传输完成 */
        switch(flag_chan) {

        case DMA_CHAN_0:
            __g_trans_done = AM_TRUE;
            break;

        case DMA_CHAN_1:
            break;

        case DMA_CHAN_2:
            break;

        case DMA_CHAN_3:
            break;

        default:
            break;
        }
    } else  {
        // 用户自定义执行代码
        switch(flag_chan) {

        case DMA_CHAN_0:
            break;

        case DMA_CHAN_1:
            break;

        case DMA_CHAN_2:
            break;

        case DMA_CHAN_3:
            break;

        default:
            break;
        }
    }
}

/**
 * \brief DMA m2m模式测试程序。
 *
 * \retval  AM_OK     : 传输成功
 * \retval  AM_ERROR  : 传输失败
 */
static int dma_m2m_test (void)
{
    uint32_t flags;
    uint32_t i;
    uint8_t buf_src[50];          /**< \brief 源端数据缓冲区   */
    uint8_t buf_dst[50];          /**< \brief 目标端数据缓冲区 */

    am_kl26_dma_chan_cfg(DMA_CHAN_0,
                         KL26_DMA_TRIGGER_DISABLE |  /**< \brief DMA正常模式  */
                         DMA_REQUEST_MUX0_ALWAYSON0);     /**< \brief 请求源                */

    for (i = 0; i < 50; i++) {
        buf_src[i] = i;
        buf_dst[i] = 0;
    }

    /* 连接DMA中断服务函数 */
    am_kl26_dma_isr_connect(DMA_CHAN_0, dma_isr, (void *)0);

    /* DMA传输配置 */
    flags = KL26_DMA_DCR_PER_REQUEST_DISABLE    |  /* 外设请求源禁能   */
            KL26_DMA_DCR_CYCLE_TRANSFERS        |  /* 循环传输         */
            KL26_DMA_DCR_AUTO_ALIGN_DISABLE     |  /* 自动对齐禁能     */
            KL26_DMA_DCR_SOURCE_SIZE_8_BIT      |  /* 源地址1字节      */
            KL26_DMA_DCR_DESTINATION_SIZE_8_BIT |  /* 目的地址1字节写入*/
            KL26_DMA_DCR_REQUEST_AFFECTED       |  /* 请求有影响       */
            KL26_DMA_DCR_NO_LINKING             |  /* 无通道连接       */
            KL26_DMA_DCR_INTERRUTP_ENABLE       |  /* DMA中断使能      */
            KL26_DMA_DCR_START_ENABLE    ;         /* DMA开始传输使能  */

    /* 建立通道描述符 */
    am_kl26_dma_xfer_desc_build(&g_desc,               /* 通道描述符       */
                                (uint32_t)(buf_src),   /* 源端数据缓冲区   */
                                (uint32_t)(buf_dst),   /* 目标端数据缓冲区 */
                                50,                    /* 传输字节数       */
                                flags);                /* 传输配置         */

    /* 启动DMA传输，马上开始传输 */
    if (am_kl26_dma_chan_start(&g_desc,
                               KL26_DMA_MER_TO_MER,  /* 内存到内存   */
                               DMA_CHAN_0) == AM_ERROR) {
        return AM_ERROR;
    }

    while(__g_trans_done == AM_FALSE);                   /* 等待传输完成  */

    for (i = 0; i < 50; i++) {
        if (buf_src[i] != buf_dst[i]) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

/**
 * \brief DMA内存到内存
 * \return 无
 */
void demo_kl26_dr_dma_m2m_entry (void)
{

    if (dma_m2m_test() == AM_OK){
        am_kprintf("transfer success\n");
    } else {
        am_kprintf("transfer failure\n");
    }

    while (1){

    }
}

/** [src_kl26_dr_dma_m2m] */

/* end of file */

