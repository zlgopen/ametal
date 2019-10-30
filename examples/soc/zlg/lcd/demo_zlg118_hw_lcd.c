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
 * \brief LCD 控制器显示例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   在LCD屏上循环显示数字"0000~9999"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_lcd.c src_zlg118_hw_lcd
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-10  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_lcd
 * \copydoc demo_zlg118_hw_lcd.c
 */

/** [src_zlg118_hw_lcd] */
#include "ametal.h"
#include "am_zlg118.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_zlg118_lcd.h"
#include "hw/amhw_zlg118_lcd.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define LCD_CLK_SRC   AMHW_ZLG118_LCD_CLK_SRC_RCL   /**< \brief 时钟源 */
#define LCD_BIAS_SRC  AMHW_ZLG118_LCD_BIAS_CAP_DIV  /**< \brief 偏置电压源 */
#define LCD_DUTY      AMHW_ZLG118_DUTY_1_4          /**< \brief 占空比 */
#define LCD_BIAS      AMHW_ZLG118_BIAS_1_3          /**< \brief 偏置电压 */
#define LCD_CPCLK     AMHW_ZLG118_CPCLK_2K_HZ       /**< \brief 电压泵频率 */
#define LCD_SCAN_CLK  AMHW_ZLG118_LCDCLK_128_HZ     /**< \brief 扫描频率 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zlg118_lcd_t       *gp_hw_lcd   = NULL;  /**< \brief lcd 外设 */

/**
 * \brief LCD控制器端口初始化
 */
static void lcd_port_init ()
{
    /* 设置 SEG0-SEG7 */
    amhw_zlg118_seg0_31_ctr (gp_hw_lcd, 0x000000ff);

    /* 关闭SEG32-51 Com0-8 */
    amhw_zlg118_seg32_51_com0_8_ctr (gp_hw_lcd,
                                     AMHW_ZLG118_LCD_ALL_CLOSE,
                                     0xffffffff);

    /* 设置Com0-3 */
    amhw_zlg118_seg32_51_com0_8_ctr (gp_hw_lcd,
                                     AMHW_ZLG118_LCD_COM0_3_CTR,
                                     0xf);

    /* SEG32-35端口功能选择 */
    amhw_zlg118_seg32_51_com0_8_ctr (gp_hw_lcd,
                                     AMHW_ZLG118_LCD_MUX_CTR,
                                     0x1);

    /* 设置端口SEG32-SEG35 */
    amhw_zlg118_seg32_51_com0_8_ctr (gp_hw_lcd,
                                     AMHW_ZLG118_LCD_SEG32_35_CTR,
                                     0xf);
    /* 设置端口seg40-51 */
    amhw_zlg118_seg32_51_com0_8_ctr (gp_hw_lcd,
                                     AMHW_ZLG118_LCD_SEG40_51_CTR,
                                     0x10);

    /* 设置SEG36-39 COM4-7 */
    amhw_zlg118_seg32_51_com0_8_ctr (gp_hw_lcd,
                                     AMHW_ZLG118_LCD_COMSEG_CTR,
                                     0x0);
}

/**
 * \brief LCD控制器初始化
 */
static void lcd_init ()
{
	int peri = 0;

    /* 时钟源选择 */
    amhw_zlg118_lcd_clk_rsc_sel (gp_hw_lcd, LCD_CLK_SRC);

    /* 设置RCL时钟频率校准值 （32.768KHz）*/
    amhw_zlg118_rcc_rcl_trim_set(*((uint16_t*)0x00100C22));

    amhw_zlg118_rcc_rcl_waittime_set(AMHW_ZLG118_RCL_WAITTIME_256);

    amhw_zlg118_rcc_set_start(0x5A5A);
    amhw_zlg118_rcc_set_start(0xA5A5);

    amhw_zlg118_rcc_rcl_enable();

    /* 等待稳定*/
    while(amhw_zlg118_rcc_rcl_state_get() == AM_FALSE);

    /* 开启LCD时钟 */
    peri = CLK_LCD & 0xff;

    amhw_zlg118_rcc_peripheral_enable((amhw_zlg118_peripheral)peri);

    /* 选择偏置电压源 */
    amhw_zlg118_lcd_bias_rsc_sel (gp_hw_lcd, LCD_BIAS_SRC);

    /* 设置占空比 */
    amhw_zlg118_lcd_duty_sel (gp_hw_lcd, LCD_DUTY);

    /* 设置偏置电压 */
    amhw_zlg118_lcd_bias_sel (gp_hw_lcd, LCD_BIAS);

    /* 电压泵时钟频率选择 */
    amhw_zlg118_lcd_cpclk_sel (gp_hw_lcd, LCD_CPCLK);

    /* 扫描频率选择 */
    amhw_zlg118_lcd_cldclk_sel (gp_hw_lcd, LCD_SCAN_CLK);

    /* 中断禁能 */
    amhw_zlg118_lcd_int_disable (gp_hw_lcd);

    /* 禁能DMA */
    amhw_zlg118_lcd_dma_disable (gp_hw_lcd);
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_lcd_entry (void                    *p_hw_lcd,
                               uint32_t                *p_data)
{
    int i = 0;

    gp_hw_lcd  = (amhw_zlg118_lcd_t *)p_hw_lcd;

    /* LCD控制器初始化 */
    lcd_init ();

    /* LCD控制器端口初始化 */
    lcd_port_init ();

    /* LCD 模式选择 */
    amhw_zlg118_lcd_mode_sel (gp_hw_lcd, AMHW_ZLG118_LCD_MODE_0);

    /* 使能LCD */
    amhw_zlg118_lcd_enable (gp_hw_lcd);

    /* 清除显存数据 */
    for (i = 0; i < 2; i++){

        amhw_zlg118_lcd_ram_write (gp_hw_lcd, 0, i);
    }

    while (1)
    {
        for (i = 0; i < 10; i++){

            /* 写数据到显存 */
            amhw_zlg118_lcd_ram_write (gp_hw_lcd, p_data[i], 0);
            /* 写数据到显存 */
            amhw_zlg118_lcd_ram_write (gp_hw_lcd, p_data[i], 1);

            am_mdelay (1000);
        }

        i = 0;
    }
}

/* end of file */
