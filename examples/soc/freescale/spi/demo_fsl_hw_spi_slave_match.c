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
 * \brief SPI从机匹配中断演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI从机接口和主机接口对应连接起来(PIOC_4 == PIOD_4, PIOC_5 == PIOD_5,
 *      PIOC_6 == PIOD_6, PIOC_7 == PIOD_7)；；
 *
 * - 实验现象:
 *   1. SPI0从机接收SPI1主机数据，如果数据被匹配，则发送从机特征码给主机；
 *   2. SPI1主机检验收到的特征码是否正确，如果正确点亮LED，否则LED闪烁报警。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_spi_slave_match.c src_fsl_hw_spi_slave_match
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_fsl_if_hw_spi_slave_match
 * \copydoc demo_fsl_hw_spi_slave_match.c
 */
 
/** [src_fsl_hw_spi_slave_match] */
#include "ametal.h"
#include "am_int.h"
#include "hw/amhw_fsl_spi.h"
#include "am_board.h"
#include "am_arm_nvic.h"

#define __SLAVE_MATCH_DATA    0x55            /**< \brief 匹配数据      */
#define __DATA_LENGTH         0x5             /**< \brief 特征码长度    */

/** \brief 从机特征码    */
const uint8_t __g_slv_data[__DATA_LENGTH] = {
   1, 2, 1, 3, 5
};

/**
 * \brief SPI中断处理函数
 */
void __spi_irq_handle (void *p_arg)
{
    static volatile uint8_t pos = 0;               /* 数据位置          */
    static volatile am_bool_t  match_irq  = AM_TRUE;     /* 匹配中断打开标志   */
    static volatile am_bool_t  send_irq   = AM_FALSE;    /* 发送中断打开标志   */

    amhw_fsl_spi_t *p_hw_spi = (amhw_fsl_spi_t*)p_arg;

    /* 匹配中断 */
    if (match_irq && ((amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_MATCH) != 0 )) {
        
        /* 开始发送特征码配置 */
        p_hw_spi->s = 0x40;
        pos = 0;
        
        amhw_fsl_spi_int_disable(p_hw_spi, AMHW_FSL_SPI_IRQ_MATCH); /* 关闭匹配中断 */
        match_irq = AM_FALSE;
        
        amhw_fsl_spi_int_enable(p_hw_spi, AMHW_FSL_SPI_IRQ_T);      /* 开启发送中断 */
        send_irq  = AM_TRUE;
    }

    /* 发送中断 */
    if (send_irq && (amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_T_EMPTY) != 0 ) {
        
        /* 发送数据 */
        if (pos < __DATA_LENGTH) {
            amhw_fsl_spi_data8_read(p_hw_spi);
            amhw_fsl_spi_data8_wirte(p_hw_spi, __g_slv_data[pos++]);
            
        /* 回到最初状态 */
        } else {
            p_hw_spi->s = 0x40;
            amhw_fsl_spi_matchdata16_wirte(p_hw_spi, 0x55);
            
            amhw_fsl_spi_int_enable(p_hw_spi, AMHW_FSL_SPI_IRQ_MATCH);
            match_irq = AM_TRUE;
            
            amhw_fsl_spi_int_disable(p_hw_spi, AMHW_FSL_SPI_IRQ_T);
            send_irq  = AM_FALSE;
        }
    }
}

static void __spi_speed_cfg (amhw_fsl_spi_t *p_hw_spi,
                             uint32_t        target_speed,
                             uint32_t        spi_freq)
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
    /* 配置SPI为从机 */
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
void demo_fsl_hw_spi_slave_match_entry (amhw_fsl_spi_t *p_hw_spi0,
                                        amhw_fsl_spi_t *p_hw_spi1,
                                        int             slave_inum,
                                        uint32_t        spi_freq)
{
    uint8_t i;
    /** \brief 主机发送缓冲区 */
    uint8_t master_tx_buf[6] = {__SLAVE_MATCH_DATA, 0, 0, 0, 0, 0};
    /** \brief 主机接收缓冲区 */
    uint8_t master_rx_buf[6] = {0};
    am_bool_t error_ocur = AM_FALSE;

    /* SPI 从机初始化 */
    __spi_slve_init(p_hw_spi0);
    __spi_master_init(p_hw_spi1, spi_freq);

    /* 配置匹配数据(如果配置了8bit数据则，只有低8位有效 ) */
    amhw_fsl_spi_matchdata16_wirte(p_hw_spi0, __SLAVE_MATCH_DATA);

    /* 清除匹配标志 */
    p_hw_spi0->s = 0x40;

    /* 开启匹配中断 */
    amhw_fsl_spi_int_enable(p_hw_spi0, AMHW_FSL_SPI_IRQ_MATCH);
    am_int_connect(slave_inum, __spi_irq_handle, (void*)p_hw_spi0);
    am_int_enable(slave_inum);

    /* 主机发送请求码，并接收特征码 */
    for (i = 0; i < (__DATA_LENGTH + 1); i++) {
        while((amhw_fsl_spi_stat_get(p_hw_spi1) & AMHW_FSL_SPI_STAT_T_EMPTY) == 0);
        amhw_fsl_spi_data8_wirte(p_hw_spi1,master_tx_buf[i]);
        
        while((amhw_fsl_spi_stat_get(p_hw_spi1) & AMHW_FSL_SPI_STAT_R_FULL) == 0);
        master_rx_buf[i] = amhw_fsl_spi_data8_read(p_hw_spi1);
    }
    
    am_mdelay(1000);
    
    /* 检验收到的特征码 */
    for (i = 0; i < __DATA_LENGTH ; i++) {
        if (master_rx_buf[i+1] != __g_slv_data[i]) {
            error_ocur = AM_TRUE;
            break;
        }
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

/** [src_fsl_hw_spi_slave_match] */

/* end of file */
