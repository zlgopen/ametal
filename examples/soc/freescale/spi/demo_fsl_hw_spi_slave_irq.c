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
 * \brief SPI从机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI0从机接口和SPI1主机接口对应连接起来(PIOC_4 == PIOD_4, PIOC_5 == PIOD_5,
 *   PIOC6 == PIOD_6, PIOC_7 == PIOD_7)；
 *   2. 将程序下载到demo板，重新上电或者复位。
 *
 * - 实验现象:
 *   1. 主机SPI1和从机SPI0互发数据(SPI0使用中断方式收发数据)；
 *   2. 比较SPI1和SPI0数据，如果相同则LED0一直点亮，否则LED0以200ms时间间隔闪烁。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_spi_slave_irq.c src_fsl_hw_spi_slave_irq
 *
 * \internal
 * \par History
 * - 1.00 16-09-27  sdy, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_fsl_if_hw_spi_slave_irq
 * \copydoc demo_fsl_hw_spi_slave_irq.c
 */
 
/** [src_fsl_hw_spi_slave_irq] */
#include "ametal.h"
#include "am_int.h"
#include "hw/amhw_fsl_spi.h"
#include "am_board.h"
#include "am_vdebug.h"


#define __NUM_COUNT   32                            /**< \brief 缓冲区大小     */

static uint8_t __g_spi0_slve_txbuf[__NUM_COUNT]   = {0};     /**< \brief 从机发送缓冲区 */
static uint8_t __g_spi0_slve_rxbuf[__NUM_COUNT]   = {0};     /**< \brief 从机接收缓冲区 */
static uint8_t __g_spi1_master_txbuf[__NUM_COUNT] = {0};     /**< \brief 主机发送缓冲区 */
static uint8_t __g_spi1_master_rxbuf[__NUM_COUNT] = {0};     /**< \brief 主机接收缓冲区 */


/**
 * \brief SPI0从机中断处理函数
 */
static void __spi0_irq(void *p_arg)
{
    amhw_fsl_spi_t *p_hw_spi = (amhw_fsl_spi_t*)p_arg;
    
    static uint8_t send_pos = 0;
    static uint8_t recv_pos = 0;
    
    /* 接收数据 */
    if (amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_R_FULL) {
        __g_spi0_slve_rxbuf[recv_pos++] = amhw_fsl_spi_data8_read(p_hw_spi);
    }
    
    /* 发送数据 */
    if (amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_T_EMPTY) {
        amhw_fsl_spi_data8_wirte(p_hw_spi,__g_spi0_slve_txbuf[send_pos++]);
    }
}



static void __spi_speed_cfg (amhw_fsl_spi_t *p_hw_spi,
                             uint32_t target_speed,
                             uint32_t spi_freq)
{

    uint32_t real_speed;              /* 计算出的速度    */
    uint32_t pdiv, best_pdiv;         /* 预分频值        */
    uint32_t div, best_div;           /* 分配值          */
    uint32_t diff, min_diff;          /* 速度差值        */

   min_diff = 0xFFFFFFFFU;

   /* 设置为最大的分频值,速度为最小 */
   best_pdiv  = 7;
   best_div   = 8;

   /* 查找最合适的速度 */
   for (pdiv = 0; (pdiv <= 7) && min_diff; pdiv++)
   {
       for (div = 0; (div <= 8) && min_diff; div++)
       {
           /* all_div = (pdiv+1) * 2^(div+1) */
           real_speed = (spi_freq / ((pdiv + 1) << (div+1)));

           /* 实际速度应小于目标速度 */
           if (target_speed >= real_speed)
           {
               diff = target_speed-real_speed;

               if (min_diff > diff)
               {
                   /* 最合适的速度 */
                   min_diff   = diff;
                   best_pdiv  = pdiv;
                   best_div   = div;
               }
           }
       }
   }

   amhw_fsl_spi_div_cfg(p_hw_spi, best_pdiv, best_div);
}

/**
 * \brief SPI0从机初始化（8bit数据宽度 SPI_MODE1）
 */
static void __spi_slve_init (amhw_fsl_spi_t *p_hw_spi)
{
    /* 配置SPI为从机 */
    amhw_fsl_spi_workmode_cfg(p_hw_spi, AMHW_FSL_SPI_WMODE_SLAVE);

    /* 配置8bit数据宽度 */
    amhw_fsl_spi_feature_cfg(p_hw_spi, AMHW_FSL_SPI_CFG_8BIT_WIDTH);

    /* 配置时钟相位和极性 */
    amhw_fsl_spi_mode_cfg(p_hw_spi, AMHW_FSL_SPI_MODE_1);

    /* 使能SPI模块 */
    amhw_fsl_spi_enable(p_hw_spi);
}

/**
 * \brief SPI1从机初始化（8bit数据宽度 SPI_MODE1）
 */
static void __spi_master_init (amhw_fsl_spi_t *p_hw_spi, uint32_t spi_freq)
{
    /* 配置SPI为主机 */
    amhw_fsl_spi_workmode_cfg(p_hw_spi, AMHW_FSL_SPI_WMODE_MASTER);

    /* 配置8bit数据宽度 */
    amhw_fsl_spi_feature_cfg(p_hw_spi, AMHW_FSL_SPI_CFG_8BIT_WIDTH);

    /* 配置时钟相位和极性 */
    amhw_fsl_spi_mode_cfg(p_hw_spi, AMHW_FSL_SPI_MODE_1);
    
    /* 配置CS引脚为片选输出 */
    amhw_fsl_spi_cs_cfg(p_hw_spi, AMHW_FSL_SPI_CS_SPI_OUT);

    /* 配置传输速度 */
    __spi_speed_cfg(p_hw_spi, 3000000, spi_freq);

    /* 使能SPI模块 */
    amhw_fsl_spi_enable(p_hw_spi);
}


/**
 * \brief 例程入口
 */
void demo_fsl_hw_spi_slave_irq_entry (amhw_fsl_spi_t *p_hw_spi0,
                                      amhw_fsl_spi_t *p_hw_spi1,
                                      int             slave_inum,
                                      uint32_t        spi_freq)
{
    uint8_t i;
    
    am_bool_t error_ocur = AM_FALSE;
    
    /* SPI初始化，SPI0为从机，SPI1为主机 */
    __spi_slve_init(p_hw_spi0);
    __spi_master_init(p_hw_spi1, spi_freq);


    /* 构造发送的数据 */
    for (i = 0; i < __NUM_COUNT; i++) {
        __g_spi0_slve_txbuf[i]   = i * 3;
        __g_spi1_master_txbuf[i] = i * 2;
    }
    
    /* SPI0从机使用中断接收和发送数据 */
    amhw_fsl_spi_int_enable(p_hw_spi0, AMHW_FSL_SPI_IRQ_R_MODDEF);
    amhw_fsl_spi_int_enable(p_hw_spi0, AMHW_FSL_SPI_IRQ_T);

    am_int_connect(slave_inum, __spi0_irq, (void*)p_hw_spi0);
    am_int_enable(slave_inum);
    
    /* SPI主机收发数据 */
    for (i = 0; i < __NUM_COUNT; i++) {
        while((amhw_fsl_spi_stat_get(p_hw_spi1) & AMHW_FSL_SPI_STAT_T_EMPTY) == 0);
        amhw_fsl_spi_data8_wirte(p_hw_spi1,__g_spi1_master_txbuf[i]);
        
        while((amhw_fsl_spi_stat_get(p_hw_spi1) & AMHW_FSL_SPI_STAT_R_FULL) == 0);
        __g_spi1_master_rxbuf[i] = amhw_fsl_spi_data8_read(p_hw_spi1);
    }
   
    /* 等待最后的数据发送完成 */
    am_mdelay(1000);
    
    /* 数据检验 */
   for (i = 0; i < __NUM_COUNT; i++) {
        if (__g_spi0_slve_txbuf[i]   != __g_spi1_master_rxbuf[i] ||
            __g_spi1_master_txbuf[i] != __g_spi0_slve_rxbuf[i]) {
                
            error_ocur  = AM_TRUE;
            break;   
        }
        AM_DBG_INFO("spi0_tx = %d spi1_rx = %d\r\n",
                    __g_spi0_slve_txbuf[i],
                    __g_spi1_master_rxbuf[i]);
        AM_DBG_INFO("spi1_tx = %d spi0_rx = %d\r\n",
                    __g_spi1_master_txbuf[i],
                    __g_spi0_slve_rxbuf[i]);
    } 

    while (1) {
        
        /* 数据校验错误，LED闪烁提醒 */
        if(error_ocur) {
            am_led_on(LED1);
            am_mdelay(200);
            am_led_off(LED1);
            am_mdelay(200);
        
        /* LED灯常亮 */
        } else {
            am_led_on(LED1);
        }

    }
}

/** [src_fsl_hw_spi_slave_irq] */

/* end of file */
