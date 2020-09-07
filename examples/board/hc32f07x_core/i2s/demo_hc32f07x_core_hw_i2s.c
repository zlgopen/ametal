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
 * \brief  I2S硬件层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-27  mkr, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_clk.h"
#include "am_int.h"
#include "hc32f07x_pin.h"
#include "hc32f07x_clk.h"
#include "am_hc32f07x_i2s.h"
#include "amhw_hc32f07x_i2s.h"
#include "am_hc32f07x.h"
#include "am_hc32f07x_inst_init.h"

am_hc32f07x_i2s_dev_t  i2s0_dev = {
    AMHW_HC32F07X_I2S_PCLK ,           /* I2S 的输入时钟 */
    48000000,                          /* I2S 的输入时钟频率 */
    8000,                              /* I2S 采样频率 */
    AM_FALSE,                          /* I2S 主时钟输出使能 */
    AMHW_HC32F07X_I2S_MODE_MASTER_TX,  /* I2S 模式  */
    AMHW_HC32F07X_I2S_DATA_LEN16,      /* I2S 通道长度  */
    AMHW_HC32F07X_I2S_PHI              /* I2S 标准  */
};

uint16_t dat[] = {0x00F9,0x0045,0x0584,0x01DA,0x02CF,0x022E,0xFDA7,0x0090,
                  0x0349,0x01C9,0x01F4,0x01DF,0xFFAA,0x0037,0xFD3F,0xFF11};
static int i = 0;

static void __i2s_isr_handler (void *p_var)
{
    /** \brief 要发送的数据   */

    amhw_hc32f07x_i2s_t *p_hw_i2s = (amhw_hc32f07x_i2s_t *)(p_var);

    if(amhw_hc32f07x_i2s_flag_check (p_hw_i2s,
                                     AMHW_HC32F07X_I2S_FLAG_LEFT_TX_EMPTY)) {
        amhw_hc32f07x_i2s_data_left_write (p_hw_i2s,
                                           dat[i]);
        p_hw_i2s->sr &= ~(1<<1);
    }
    if(amhw_hc32f07x_i2s_flag_check (p_hw_i2s,
                                     AMHW_HC32F07X_I2S_FLAG_RIGHT_TX_EMPTY)){
        amhw_hc32f07x_i2s_data_right_write (p_hw_i2s,
                                            dat[i]);
        p_hw_i2s->sr &= ~(1<<15);
    }

    i++;
    if(i == 15) {
        i = 0;
    }
    /* 接收中断处理 */

}

void __gpio_init(int clk_i2s){

    am_clk_enable(CLK_GPIO);

    am_clk_enable(clk_i2s);

    /** \brief 配置I2S 功能引脚 */
    am_gpio_pin_cfg (PIOD_8,  PIOD_8_I2S0_SCK|PIOD_8_OUT_PP);     
    am_gpio_pin_cfg (PIOD_10, PIOD_10_I2S0_SD|PIOD_10_OUT_PP);    
    am_gpio_pin_cfg (PIOD_11, PIOD_11_I2S0_WS|PIOD_11_OUT_PP);  
}

void demo_hc32f07x_hw_i2s_int_entry (void)
{
    amhw_hc32f07x_i2s_t *p_hw_i2s = (amhw_hc32f07x_i2s_t *)HC32F07X_I2S0_BASE;

    extern int am_int_connect (int inum, am_pfnvoid_t pfn_isr, void *p_arg);
    extern int am_int_enable (int inum);

    AM_DBG_INFO("demo hc32f07x_core i2s!\r\n");

    am_int_connect(INUM_SPI0_I2S0, __i2s_isr_handler, p_hw_i2s);
    am_int_enable(INUM_SPI0_I2S0);

    __gpio_init(CLK_I2S0);

    am_hc32f07x_i2s_init (p_hw_i2s, &i2s0_dev);

    while(1) {
        am_kprintf("The program is running...\r\n");

        am_mdelay(200);
    }
}
