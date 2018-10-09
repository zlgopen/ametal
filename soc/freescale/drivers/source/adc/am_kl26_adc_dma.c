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
 * \brief ADC_DMA implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-13  ari, first implementation.
 * \endinternal
 */
#include "am_kl26_dma.h"
#include "am_fsl_adc.h"
#include "am_adc.h"

/******************************************************************************/
static void __am_fsl_adc_dma_isr (void *p_arg , uint8_t flag);

/**
 * \brief DMA传输配置
 */
int am_fsl_adc_dma_tran_cfg (am_fsl_adc_dev_t *p_dev,
                             am_adc_buf_desc_t *p_desc)
{

    uint32_t flags  = 0;
    uint32_t length = 0;
    amhw_kl26_dma_xfer_desc_t    dma_desc;

    if( p_desc->p_buf == NULL) {
        return -AM_EINVAL;
    }

    /* DMA传输配置 */
    flags = KL26_DMA_DCR_PER_REQUEST_ENABLE      |  /* 外设请求源使能    */
            KL26_DMA_DCR_SINGLE_TRANSFERS        |  /* 单次传输          */
            KL26_DMA_DCR_AUTO_ALIGN_DISABLE      |  /* 自动对齐禁能      */
            KL26_DMA_DCR_REQUEST_AFFECTED        |  /* 请求有影响        */
            KL26_DMA_DCR_NO_LINKING              |  /* 无通道连接        */
            KL26_DMA_DCR_INTERRUTP_ENABLE        |  /* DMA中断使能       */
            KL26_DMA_DCR_START_DISABLE    ;         /* DMA开始传输禁能   */

    /* 连接DMA中断服务函数 */
    am_kl26_dma_isr_connect(p_dev->p_devinfo->dma_chan, __am_fsl_adc_dma_isr, p_dev);

    am_kl26_dma_chan_cfg(p_dev->p_devinfo->dma_chan,
                         KL26_DMA_TRIGGER_DISABLE | /**< \brief DMA正常模式 */
                         DMA_REQUEST_MUX0_ADC0);         /**< \brief 请求源      */

    if ( p_dev->p_devinfo->is_diff == AM_TRUE ) {
        /* 16位、13位、11位、9位差分输入存放到  int16_t 类型的buf里 */
        flags |= KL26_DMA_DCR_SOURCE_SIZE_16_BIT |
                KL26_DMA_DCR_DESTINATION_SIZE_16_BIT;
        length = (p_desc->length) << 1;

    } else if ( p_dev->p_devinfo->bits == 8 ) {
        /* 8位单端输入 存放到uint8_t类型的buf里 */
        flags |= KL26_DMA_DCR_SOURCE_SIZE_8_BIT |
                KL26_DMA_DCR_DESTINATION_SIZE_8_BIT;
        length = (p_desc->length) << 0;
    } else {
        /* 16位、12位、10位 单端输入 存放到 uint16_t 类型的buf里 */
        flags |= KL26_DMA_DCR_SOURCE_SIZE_16_BIT |
                KL26_DMA_DCR_DESTINATION_SIZE_16_BIT;
        length = (p_desc->length) << 1;
    }


    /* 建立通道描述符 */
    am_kl26_dma_xfer_desc_build(&dma_desc,                      /* 通道描述符   */
                                (uint32_t)&(p_dev->p_devinfo->p_hw_adc->dat[p_dev->p_devinfo->seq]), /* 源端数据缓冲 */
                                (uint32_t)(p_desc->p_buf),         /* 目标数据缓冲 */
                                (uint32_t)(length),                /* 传输字节数   */
                                flags);                            /* 传输配置     */

    /* 启动DMA传输，马上开始传输 */
    if (am_kl26_dma_chan_start(&dma_desc,
                               KL26_DMA_PER_TO_MER,   /* 外设到 内存   */
                               (uint8_t)p_dev->p_devinfo->dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
}



/**
 * \brief ADC DMA传输中断服务函数
 */
static void __am_fsl_adc_dma_isr (void *p_arg , uint8_t flag)
{

    am_fsl_adc_dev_t *p_dev = (am_fsl_adc_dev_t *)p_arg;
    uint32_t cfg = 0;

    if (flag == AM_KL26_DMA_INT_NORMAL) { /* 通信正常 */

        if (NULL != p_dev->p_desc[p_dev->desc_pos].pfn_complete) {
            p_dev->p_desc[p_dev->desc_pos].pfn_complete(p_dev->p_desc[p_dev->desc_pos].p_arg, AM_OK);
        }

        p_dev->desc_pos++;
        if (p_dev->desc_pos == p_dev->desc_num) { /* 已经接收完成 */
            p_dev->desc_pos = 0;

            if (NULL != p_dev->pfn_callback) {
                p_dev->pfn_callback(p_dev->p_arg, AM_OK);
            }
            p_dev->count_pos++; /* 转换序列标准 */
            if (p_dev->count != 0 && p_dev->count_pos == p_dev->count) {
                p_dev->count_pos = 0;
                am_adc_stop((am_adc_handle_t)&(p_dev->adc_serv), p_dev->chan);  /* 关闭模块 */
                amhw_fsl_adc_dma_disable(p_dev->p_devinfo->p_hw_adc); /* 关闭DMA传输*/
                am_kl26_dma_chan_stop(p_dev->p_devinfo->dma_chan);
                return ; /* 返回 */
            }
        }

        /* 否则启动下一个传输 */
        am_adc_stop((am_adc_handle_t)&(p_dev->adc_serv), p_dev->chan);                /* 关闭ADC模块 */
        am_kl26_dma_chan_stop(p_dev->p_devinfo->dma_chan);   /* 停止通道传输  */
        am_fsl_adc_dma_tran_cfg(p_dev, &(p_dev->p_desc[p_dev->desc_pos]));
        amhw_fsl_adc_dma_enable(p_dev->p_devinfo->p_hw_adc);  /* 使能DMA传输*/

        /* 获取ADC配置状态控制寄存器1的值 */
        cfg = amhw_fsl_adc_sc1_get(p_dev->p_devinfo->p_hw_adc,
                                    p_dev->p_devinfo->seq);

        cfg &= ~AMHW_FSL_ADC_SC1_CHAN_MASK;
        cfg |= AMHW_FSL_ADC_SC1_CHAN_SEL_CFG(p_dev->chan);

        amhw_fsl_adc_sc1_cfg(p_dev->p_devinfo->p_hw_adc,      /* 开启ADC通道传输*/
                              p_dev->p_devinfo->seq,
                              cfg);
    } else { /* 通信错误 */

    }

}

