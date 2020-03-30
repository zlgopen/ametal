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
 * \brief DAC转换 DMA传输数据例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   1. 串口打印出 "DMA transfer done!"。
 *   2. 打印出当前DAC输出电压值。(输出电压类似为一个正弦波，波谷值0mv 、波峰值 3300 mv)
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_dac_dma.c src_zsn700_hw_dac_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-10  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_dac_dma
 * \copydoc demo_zsn700_hw_dac_dma.c
 */

/** [src_zsn700_hw_dac_dma] */

#include "ametal.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zsn700_dma.h"
#include "hw/amhw_zsn700_dac.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
static amhw_zsn700_dac_t      *gp_hw_dac = NULL;     /**< \brief dac 外设 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zsn700_dma_xfer_desc_t g_desc;             /**< \brief DMA 重载描述符 */

static volatile am_bool_t       g_trans_done = AM_FALSE; /**< \brief 传输完成标志 */

static int32_t                  g_dma_chan = 0;        /**< \brief DMA 通道号 */

static uint16_t                 vol_data = 0;          /**< \brief 目标端数据缓存 */

/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_zsn700_dma_isr_connect() 函数传递
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (#AM_ZLG116_DMA_INT_ERROR) 或 (#AM_ZLG116_DMA_INT_NORMAL)
 *
 * \return 无
 */
static void dac_dma_isr (void *p_arg , uint32_t flag)
{
    int flag_chan  = (int)p_arg;

    if (flag == AM_ZSN700_DMA_INT_COMPLETE) {
        if (flag_chan == g_dma_chan) {

            g_trans_done = AM_TRUE;
        }
    } else {
        /* 用户自定义执行代码 */
    }
}

/**
 * \brief DAC DMA传输配置
 */
static int dac_dma_tran_cfg (amhw_zsn700_dac_t *p_hw_dac,
                             int32_t           dma_chan)
{

    uint32_t flags;

    /* 连接DMA中断服务函数 */
    am_zsn700_dma_isr_connect(dma_chan, dac_dma_isr, (void *)dma_chan);

    /* DMA 传输配置 */
    flags = AMHW_ZSN700_DMA_CHAN_TRANSFER_MODE_BLOCK    | /* SPI只有块传输模式 */
            AMHW_ZSN700_DMA_CHAN_SIZE_16BIT             | /* 数据位宽 8bit */

            AMHW_ZSN700_DMA_CHAN_SRC_ADD_INC_DISABLE     | /* 源地址不自增 */
            AMHW_ZSN700_DMA_CHAN_RELOAD_SRC_ADD_ENABLE   | /* 源地址重载*/

            AMHW_ZSN700_DMA_CHAN_DST_ADD_INC_DISABLE   | /* 目标地址不自增 */
            AMHW_ZSN700_DMA_CHAN_RELOAD_DST_ADD_ENABLE | /* 目标地址重载 */

            AMHW_ZSN700_DMA_CHAN_RELOAD_COUNTER_ENABLE  | /* 传输数据量重载 */
            AMHW_ZSN700_DMA_CHAN_INT_ERR_DISABLE        | /* 传输错误中断禁能 */
            AMHW_ZSN700_DMA_CHAN_INT_TX_CMP_ENABLE      | /* 传输完成中断使能 */
            AMHW_ZSN700_DMA_CHAN_CIRCULAR_MODE_ENABLE ;   /* 开启循环模式 */

    /* 建立通道描述符 */
    am_zsn700_dma_xfer_desc_build(&g_desc,                      /* 通道描述符 */
                                   (uint32_t)(&vol_data),       /* 源端数据缓冲 */
                                   (uint32_t)(&(p_hw_dac->dacdhr12r0)), /* 目标数据缓冲 */
                                   1,                           /* 传输次数 */
                                   flags);                      /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_zsn700_dma_xfer_desc_chan_cfg(&g_desc,
                                          AMHW_ZSN700_DMA_PER_TO_MER, /* 外设到 内存 */
                                          (uint8_t)dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {
        /* 开启传输 */
    }

    return AM_OK;
}

/**
 * \brief DAC初始化
 */
static void dac_hw_init(amhw_zsn700_dac_t *p_hw_dac,
                        uint32_t           verf)
{

    if (p_hw_dac == NULL){
        return ;
    }

    /* 设置参考电压源 */
    amhw_zsn700_dac_chan_sref_sel(p_hw_dac, verf);

    /* 失能输出缓冲器*/
    amhw_zsn700_dac_chan_output_buf_disable(p_hw_dac);

    /* 触发使能 */
    amhw_zsn700_dac_chan_trg_enable(p_hw_dac);

    /* 触发方式选择  软件触发*/
    amhw_zsn700_dac_chan_trg_sel(p_hw_dac, AMHW_ZSN700_DAC_CHAN_SOFTWARE_TRG);

    /* 使能dac通道 */
    amhw_zsn700_dac_chan_enable(p_hw_dac);

}

/**
 * \brief DAC DMA初始化
 */
static void dac_hw_dma_init(amhw_zsn700_dac_t *p_hw_dac)
{
    if (p_hw_dac == NULL){
        return ;
    }

    /* 使能DAC DMA功能 */
    amhw_zsn700_dac_chan_dma_enbale (p_hw_dac);
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_dac_dma_entry(void     *p_hw_dac,
                                  int32_t  dma_src,
                                  int32_t  dma_chan,
                                  uint16_t *vol_val)
{

    int i = 0;

    vol_data = vol_val[0];

    g_dma_chan = dma_chan;

    gp_hw_dac = (amhw_zsn700_dac_t *)p_hw_dac;

    /* DAC 初始化 默认参考电压类型 外部参考电压源  PB01*/
    dac_hw_init(gp_hw_dac, AMHW_ZSN700_DAC_CHAN_MASK_EXTER_REF);

    /* DAC DMA使能 */
    dac_hw_dma_init(gp_hw_dac);

    /* 开始DMA传输 */
    dac_dma_tran_cfg(gp_hw_dac, dma_chan);

    /* 默认通道0和通道1优先级一样（轮询）*/
    am_zsn700_dma_priority_same();

    /* 设置传输请求信号来源 */
    am_zsn700_dma_chan_src_set (dma_chan, dma_src);

    /* 开始通道传输 */
    am_zsn700_dma_chan_start(dma_chan);

    AM_FOREVER {

        /* 软件触发 */
        amhw_zsn700_dac_chan_software_trg_enable(gp_hw_dac);

        /* 等待传输完成 */
        while (g_trans_done == AM_FALSE);

        g_trans_done = AM_FALSE;


        if (i == 128 )
        {
            vol_data = vol_val[0];
            i = 0;
        }else{
            i++;
            vol_data = vol_val[i];
        }

        am_kprintf("DMA transfer done!\r\n");
        am_kprintf("mv_out = %d mv\r\n", gp_hw_dac->dacdor0 * 3300 / 4096);

        am_mdelay(1000);
    }

}

