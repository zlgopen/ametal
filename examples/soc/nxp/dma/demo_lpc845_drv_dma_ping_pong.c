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
 * \brief DMA 乒乓传输例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. 如果 DMA 传输成功，LED0 长亮；
 *   2. 如果 DMA 传输失败，LED0 闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_lpc824_drv_dma_ping_pong.c src_lpc824_drv_dma_ping_pong
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-30  mem,modified
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_drv_dma_ping_pong
 * \copydoc demo_lpc824_drv_dma_ping_pong.c
 */

/** [src_lpc824_drv_dma_ping_pong] */
#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x_dma.h"
#include "lpc84x_dma_chan.h"

/** \brief 重载描述符，使用乒乓传输时，描述符必须是16字节对齐 */
am_local __attribute__((aligned(16)))  am_lpc84x_dma_xfer_desc_t __g_desc[2];

am_local volatile am_bool_t __g_trans_a_done;  /**< \brief 传输完成标志 */
am_local volatile am_bool_t __g_trans_b_done;  /**< \brief 传输完成标志 */
am_local volatile am_bool_t __g_trans_err;     /**< \brief 传输错误标志 */

/**
 * \brief DMA 中断服务程序。
 *
 * \param[in] p_arg 用户自定义参数
 * \param[in] stat  DMA 传输完成状态标志，该参数的可能取值：
 *                      AM_LPC84X_DMA_STAT_INTA   A 中断发生
 *                      AM_LPC84X_DMA_STAT_INTB   B 中断发生
 *                      AM_LPC84X_DMA_STAT_INTERR 错误发生
 *
 * \return 无
 */
am_local void __dma_ping_pong_isr (void *p_arg, int stat)
{
    __g_trans_err = AM_FALSE;

    if (stat & AM_LPC84X_DMA_STAT_INTERR) {
        __g_trans_err = AM_TRUE;
    }

    if (stat & AM_LPC84X_DMA_STAT_INTA) {
        __g_trans_a_done = AM_TRUE;
    }
    if (stat & AM_LPC84X_DMA_STAT_INTB) {
        __g_trans_b_done = AM_TRUE;
    }
}

/**
 * \brief DMA 传输，乒乓模式
 */
void demo_lpc845_drv_dma_ping_pong_entry (uint8_t  chan,
                                          uint8_t *p_src, 
                                          int      len)
{
    int          i      = 0;
    uint32_t     flags1 = 0;
    uint32_t     flags2 = 0;
    
    /* 申请ping-pong数据缓冲区空间 */
    uint8_t *p_dsta = calloc(len, sizeof(uint8_t));
    uint8_t *p_dstb = calloc(len, sizeof(uint8_t));
    
    am_lpc84x_dma_controller_t *p_ctr  = NULL;

    p_ctr = am_lpc84x_dma_controller_get(chan, DMA_CHAN_OPT_PRIO_1);

    if (p_ctr == NULL) {
        AM_DBG_INFO("Geting a Controller Faild !\r\n");
        AM_FOREVER {};
    }

    /* DMA 传输配置 */
    flags1 = AM_LPC84X_DMA_XFER_VALID           | /* 当前通道描述符有效 */
             AM_LPC84X_DMA_XFER_RELOAD          | /* 传输完成重载通道描述符 */
             AM_LPC84X_DMA_XFER_SWTRIG          | /* 软件触发 */
             AM_LPC84X_DMA_XFER_WIDTH_8BIT      | /* 传输数据宽度：8 bit */
             AM_LPC84X_DMA_XFER_SRCINC_1X       | /* 源地址按 1 个数据宽度递增 */
             AM_LPC84X_DMA_XFER_DSTINC_1X       | /* 目标地址按 1 个数据宽度递增 */
             AM_LPC84X_DMA_XFER_SETINTA;          /* 使能中断 A */

    /* DMA 传输配置 */
    flags2 = AM_LPC84X_DMA_XFER_VALID           | /* 当前通道描述符有效 */
             AM_LPC84X_DMA_XFER_RELOAD          | /* 传输完成重载通道描述符 */
             AM_LPC84X_DMA_XFER_CLRTRIG         | /* 清除触发标志 */
             AM_LPC84X_DMA_XFER_WIDTH_8BIT      | /* 传输数据宽度：8 bit */
             AM_LPC84X_DMA_XFER_SRCINC_1X       | /* 源地址按 1 个数据宽度递增 */
             AM_LPC84X_DMA_XFER_DSTINC_1X       | /* 目标地址按 1 个数据宽度递增 */
             AM_LPC84X_DMA_XFER_SETINTB;          /* 使能中断 B */

    /* 建立通道描述符 A */
    am_lpc84x_dma_xfer_desc_build(
       &__g_desc[0],            /* A 的重载描述符 */
        (uint32_t)p_src,        /* 源端数据缓冲区 */
        (uint32_t)p_dsta,       /* A 的目标端缓冲区地址 */
        len,                    /* 传输字节数 */
        flags1);                /* 传输配置 */

    /* 建立通道描述符B */
    am_lpc84x_dma_xfer_desc_build(
       &__g_desc[1],            /* B 的重载描述符 */
        (uint32_t)p_src,        /* 源端数据缓冲区 */
        (uint32_t)p_dstb,       /* B 的目标端缓冲区地址 */
        len,                    /* 传输字节数 */
        flags2);                /* 传输配置 */

    /* 链接重载描述符 */
    am_lpc84x_dma_xfer_desc_link(&__g_desc[1], &__g_desc[0]);
    am_lpc84x_dma_xfer_desc_link(&__g_desc[0], &__g_desc[1]);

    /* 启动 DMA 传输，马上开始传输 */
    am_lpc84x_dma_xfer_desc_startup(p_ctr,
                                   &__g_desc[0],
                                    __dma_ping_pong_isr,
                                    (void*)0);

    am_mdelay(1);

    /* 等待传输完成 */
    while ((__g_trans_a_done == AM_FALSE) || (__g_trans_b_done == AM_FALSE));

    /* 终止控制器数据传输 */
    am_lpc84x_dma_controller_abort(p_ctr);

    /* 不再使用该控制器时，应释放控制器 */
    am_lpc84x_dma_controller_release(p_ctr);

    /* 核实传输数据 */
    for (i = 0; i < len; i++) {
        if (p_src[i] != p_dsta[i]) {
            __g_trans_err = AM_TRUE;
                      
            am_kprintf("%d", i);  
                    
            break;
        }
    }
    for (i = 0; i < len; i++) {
        if (p_src[i] != p_dstb[i]) {
            __g_trans_err = AM_TRUE;
            break;
        }
    }

    if (__g_trans_err == AM_TRUE) {

         /* 释放空间 */
        free(p_dsta);
        free(p_dstb);
        
        AM_FOREVER {
            am_led_on(LED0);
            am_mdelay(200);
            am_led_off(LED0);
            am_mdelay(200);
        }
    } else {
        am_led_on(LED0);
    }
        
        /* 释放空间 */
    free(p_dsta);
    free(p_dstb);
        
    while(1) {
        ;
    }
}

/** [src_lpc824_drv_dma_ping_pong] */

/* end of file */
