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
 * \brief DAC数模转换例程，通过HW接口实现，DMA传输数据
 *
 * - 实验现象：
 *   1. 采集数模转换的输出口PE30的电压值
 *
 *
 * \par 源代码
 * \snippet demo_kl26_hw_dac_dma_st.c src_kl26_hw_dac_dma_st
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-27  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_hw_dac_dma_st
 * \copydoc demo_kl26_hw_dac_dma_st.c
 */

 /** [src_kl26_hw_dac_dma_st] */
#include "ametal.h"
#include "am_dac.h"
#include "am_fsl_dac.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_kl26_inst_init.h"
#include "am_kl26_dma.h"
#include "hw/amhw_kl26_sim.h"
#include "hw/amhw_fsl_dac.h"
#include "am_int.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

#define   __BUFLEN  360

static volatile uint8_t  __g_signal = 0;

static const uint16_t __g_sin_wave[__BUFLEN] = { /* 正弦波数据 */
    0x0800, 0x0823, 0x0847, 0x086B, 0x088E, 0x08B2, 0x08D6, 0x08F9,
    0x091C, 0x0940, 0x0963, 0x0986, 0x09A9, 0x09CC, 0x09EF, 0x0A11,
    0x0A34, 0x0A56, 0x0A78, 0x0A9A, 0x0ABC, 0x0ADD, 0x0AFF, 0x0B20,
    0x0B40, 0x0B61, 0x0B81, 0x0BA1, 0x0BC1, 0x0BE0, 0x0BFF, 0x0C1E,
    0x0C3D, 0x0C5B, 0x0C78, 0x0C96, 0x0CB3, 0x0CD0, 0x0CEC, 0x0D08,
    0x0D24, 0x0D3F, 0x0D5A, 0x0D74, 0x0D8E, 0x0DA7, 0x0DC0, 0x0DD9,
    0x0DF1, 0x0E09, 0x0E20, 0x0E37, 0x0E4D, 0x0E63, 0x0E78, 0x0E8D,
    0x0EA1, 0x0EB5, 0x0EC8, 0x0EDB, 0x0EED, 0x0EFE, 0x0F0F, 0x0F20,
    0x0F30, 0x0F3F, 0x0F4E, 0x0F5C, 0x0F6A, 0x0F77, 0x0F84, 0x0F8F,
    0x0F9B, 0x0FA6, 0x0FB0, 0x0FB9, 0x0FC2, 0x0FCB, 0x0FD2, 0x0FD9,
    0x0FE0, 0x0FE6, 0x0FEB, 0x0FF0, 0x0FF4, 0x0FF7, 0x0FFA, 0x0FFC,
    0x0FFE, 0x0FFF, 0x0FFF, 0x0FFF, 0x0FFE, 0x0FFC, 0x0FFA, 0x0FF7,
    0x0FF4, 0x0FF0, 0x0FEB, 0x0FE6, 0x0FE0, 0x0FD9, 0x0FD2, 0x0FCB,
    0x0FC2, 0x0FB9, 0x0FB0, 0x0FA6, 0x0F9B, 0x0F8F, 0x0F84, 0x0F77,
    0x0F6A, 0x0F5C, 0x0F4E, 0x0F3F, 0x0F30, 0x0F20, 0x0F0F, 0x0EFE,
    0x0EED, 0x0EDB, 0x0EC8, 0x0EB5, 0x0EA1, 0x0E8D, 0x0E78, 0x0E63,
    0x0E4D, 0x0E37, 0x0E20, 0x0E09, 0x0DF1, 0x0DD9, 0x0DC0, 0x0DA7,
    0x0D8E, 0x0D74, 0x0D5A, 0x0D3F, 0x0D24, 0x0D08, 0x0CEC, 0x0CD0,
    0x0CB3, 0x0C96, 0x0C78, 0x0C5B, 0x0C3D, 0x0C1E, 0x0BFF, 0x0BE0,
    0x0BC1, 0x0BA1, 0x0B81, 0x0B61, 0x0B40, 0x0B20, 0x0AFF, 0x0ADD,
    0x0ABC, 0x0A9A, 0x0A78, 0x0A56, 0x0A34, 0x0A11, 0x09EF, 0x09CC,
    0x09A9, 0x0986, 0x0963, 0x0940, 0x091C, 0x08F9, 0x08D6, 0x08B2,
    0x088E, 0x086B, 0x0847, 0x0823, 0x0800, 0x07DC, 0x07B8, 0x0794,
    0x0771, 0x074D, 0x0729, 0x0706, 0x06E3, 0x06BF, 0x069C, 0x0679,
    0x0656, 0x0633, 0x0610, 0x05EE, 0x05CB, 0x05A9, 0x0587, 0x0565,
    0x0543, 0x0522, 0x0500, 0x04DF, 0x04BF, 0x049E, 0x047E, 0x045E,
    0x043E, 0x041F, 0x0400, 0x03E1, 0x03C2, 0x03A4, 0x0387, 0x0369,
    0x034C, 0x032F, 0x0313, 0x02F7, 0x02DB, 0x02C0, 0x02A5, 0x028B,
    0x0271, 0x0258, 0x023F, 0x0226, 0x020E, 0x01F6, 0x01DF, 0x01C8,
    0x01B2, 0x019C, 0x0187, 0x0172, 0x015E, 0x014A, 0x0137, 0x0124,
    0x0112, 0x0101, 0x00F0, 0x00DF, 0x00CF, 0x00C0, 0x00B1, 0x00A3,
    0x0095, 0x0088, 0x007B, 0x0070, 0x0064, 0x0059, 0x004F, 0x0046,
    0x003D, 0x0034, 0x002D, 0x0026, 0x001F, 0x0019, 0x0014, 0x000F,
    0x000B, 0x0008, 0x0005, 0x0003, 0x0001, 0x0000, 0x0000, 0x0000,
    0x0001, 0x0003, 0x0005, 0x0008, 0x000B, 0x000F, 0x0014, 0x0019,
    0x001F, 0x0026, 0x002D, 0x0034, 0x003D, 0x0046, 0x004F, 0x0059,
    0x0064, 0x0070, 0x007B, 0x0088, 0x0095, 0x00A3, 0x00B1, 0x00C0,
    0x00CF, 0x00DF, 0x00F0, 0x0101, 0x0112, 0x0124, 0x0137, 0x014A,
    0x015E, 0x0172, 0x0187, 0x019C, 0x01B2, 0x01C8, 0x01DF, 0x01F6,
    0x020E, 0x0226, 0x023F, 0x0258, 0x0271, 0x028B, 0x02A5, 0x02C0,
    0x02DB, 0x02F7, 0x0313, 0x032F, 0x034C, 0x0369, 0x0387, 0x03A4,
    0x03C2, 0x03E1, 0x0400, 0x041F, 0x043E, 0x045E, 0x047E, 0x049E,
    0x04BF, 0x04DF, 0x0500, 0x0522, 0x0543, 0x0565, 0x0587, 0x05A9,
    0x05CB, 0x05EE, 0x0610, 0x0633, 0x0656, 0x0679, 0x069C, 0x06BF,
    0x06E3, 0x0706, 0x0729, 0x074D, 0x0771, 0x0794, 0x07B8, 0x07DC,
};

/** \brief DAC_DMA传输完成回调函数 */
static void __am_kl26_dac_dma_isr (void *p_arg , uint8_t flag)
{
    if (flag == AM_KL26_DMA_INT_NORMAL) {
        __g_signal = 1;
    }
}

/** \brief DMA 初始化配置 */
static void _kl26_dac_dma_init (uint8_t   dma_chan, uint8_t *p_dest,
                          const uint16_t *p_src,    uint32_t  lenth)
{
    uint32_t flags;
    amhw_kl26_dma_xfer_desc_t dma_desc;

    /* DMA传输配置 */
    flags = KL26_DMA_DCR_PER_REQUEST_ENABLE      |  /* 外设请求源使能    */
            KL26_DMA_DCR_SINGLE_TRANSFERS        |  /* 单次传输          */
            KL26_DMA_DCR_AUTO_ALIGN_DISABLE      |  /* 自动对齐禁能      */
            KL26_DMA_DCR_SOURCE_SIZE_16_BIT      |  /* 源地址2字节读取   */
            KL26_DMA_DCR_SOURCE_SIZE_16_BIT      |  /* 目的地址2字节写入 */
            KL26_DMA_DCR_REQUEST_AFFECTED        |  /* 请求有影响        */
            KL26_DMA_DCR_NO_LINKING              |  /* 无通道连接        */
            KL26_DMA_DCR_INTERRUTP_ENABLE        |  /* DMA中断使能       */
            KL26_DMA_DCR_START_DISABLE    ;         /* DMA开始传输禁能   */

    /* 连接DMA中断服务函数 */
    am_kl26_dma_isr_connect(dma_chan, __am_kl26_dac_dma_isr, NULL);

    am_kl26_dma_chan_cfg(dma_chan,
                         KL26_DMA_TRIGGER_DISABLE | /**< \brief DMA正常模式 */
                         DMA_REQUEST_MUX0_DAC0);         /**< \brief DAC请求源      */

    /* 建立通道描述符 */
    am_kl26_dma_xfer_desc_build(&dma_desc,              /* 通道描述符   */
                                (uint32_t)(p_src),      /* 源端数据缓冲 */
                                (uint32_t)(p_dest),     /* 目标数据缓冲 */
                                (uint32_t)(lenth << 1), /* 传输字节数   */
                                flags);                 /* 传输配置     */

    /* 启动DMA传输，马上开始传输 */
    if (am_kl26_dma_chan_start(&dma_desc,
                               KL26_DMA_MER_TO_PER, /*  内存到外设  */
                               dma_chan) == AM_ERROR) {
        am_kprintf("DMA init error!\n");
    }

}

/** \brief DAC 初始化配置 */
static void _kl26_dac_init (void)
{
    /**
     * \brief DAC 缓存区功能配置结构体定义
     */
    amhw_fsl_dac_buffer_config_t dac_buf_cfg = {
        AM_TRUE,                         /* 使能BUF功能                   */
        AMHW_FSL_DAC_TRG_SOFT,           /* 软件触发                           */
        AM_TRUE,                         /* 缓冲读取底指针中断使能 */
        AM_FALSE,                        /* 缓冲读取顶指针中断禁能 */
        AM_TRUE,                         /* 使能DMA功能                    */
        AMHW_FSL_DAC_BUFMODE_NORMAL,     /* 缓存区模式为正常模式      */
        AM_TRUE,                         /* 缓存区上限使能                 */
    };

    /* 配置PIOE_30为DAC0_OUT功能                 */
    am_gpio_pin_cfg (PIOE_30, PIOE_30_DAC0_OUT);

    /* 开启DAC时钟                               */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_DAC0);

    /* DAC缓冲区传输使能 */
    amhw_fsl_dac_dat_buf_init(KL26_DAC0, &dac_buf_cfg);

    /* 使能DAC模块 */
    amhw_fsl_dac_enable(KL26_DAC0);
}

/** \brief 启动DAC转换 */
static void _kl26_dac_start (void)
{
    amhw_fsl_dac_soft_trg_enable(KL26_DAC0);
}

/**
 * \brief DAC输出电压，DMA传输数据，HW层接口实现
 * \return 无
 */
void demo_kl26_hw_dac_dma_st_entry (void)
{
    uint32_t key;

    am_kl26_dma_inst_init();       /* DMA实例初始化            */
    _kl26_dac_dma_init(DMA_CHAN_0, /* DAC DMA传输初始化 */
                       (uint8_t *)&(KL26_DAC0->dat),
                       __g_sin_wave,
                       __BUFLEN);
    _kl26_dac_init();              /* DAC 初始化 */

    _kl26_dac_start();             /* 启动DAC */

    am_kprintf("DAC hw dac dma test\n");

    while (1) {
        key = am_int_cpu_lock();
        if (1 == __g_signal) {
            __g_signal = 0;
            am_int_cpu_unlock(key);
            _kl26_dac_dma_init(DMA_CHAN_0,
                               (uint8_t *)&(KL26_DAC0->dat),
                               __g_sin_wave,
                               __BUFLEN);
        } else {
            am_int_cpu_unlock(key);
        }

        _kl26_dac_start();

        am_mdelay(20);
    }
}


 /** [src_kl26_hw_dac_dma_st] */

/* end of file */

