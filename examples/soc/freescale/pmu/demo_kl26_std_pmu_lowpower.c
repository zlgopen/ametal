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
 * \brief KL26系统模式配置演示例程，通过PMU驱动层实现
 *
 * - 操作步骤:
 *   1. 根据串口输出内容提示进行操作；
 *   2. 波特率：115200。
 *
 * - 注意:
 *   1. 进入lls, vlls之前，lptmr也会定时唤醒MCU，
 *   2. 其中vlls模式唤醒会复位
 *
 * \par 源代码
 * \snippet demo_kl26_std_pmu_lowpower.c src_kl26_std_pmu_lowpower
 *
 * \internal
 * \par History
 * - 1.00 16-09-24  ari, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_kl26_std_pmu_lowpower
 * \copydoc demo_kl26_std_pmu_lowpower.c
 */

/** [src_kl26_std_pmu_lowpower] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_prj_config.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_kl26_pmu.h"
#include "hw/amhw_kl26_sim.h"
#include "hw/amhw_fsl_uart.h"
#include "am_kl26_inst_init.h"
#include "hw/amhw_kl26_mcg.h"
#include "am_kl26_clk.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/kl26_clk.h"
#include "../../../../soc/freescale/kl26/kl26_inum.h"
#include "../../../../soc/freescale/kl26/kl26_periph_map.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"

#define UART_BAUDRATE  115200


/* 获取一个字符 */
#define GET_CHAR(c) amhw_fsl_uart_poll_receive(KL26_UART0, &(c), 1)

void current_mode_print (void);
void system_mode_sel_print (void);
void uart0_baud_config (void);


am_uart_handle_t __g_handle;
/**
 * \brief 按键中断回调函数
 */
void gpio_irq_handle (void *p_arg)
{

    am_kprintf("\r\n GPIO Interrupt Occur!\r\n");
}

/**
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
static void __lptmr_timing_callback (void *p_arg)
{
   am_led_toggle(LED0);
}

/**
 * \brief 主函数
 */
void demo_kl26_std_pmu_lowpower_entry (void)
{
    uint8_t  recv;

    uint32_t clk_frequency;

    am_timer_handle_t lptmr_timing_handle = NULL;

    /*LLWU唤醒源信息 */
    am_kl26_pmu_llwu_src_info_t llwu_src_info[2];


    /* 配置UART0 波特率 */
    uart0_baud_config();
    
    am_kprintf("\r\nChip rest...\r\n");
    
    am_led_on(0);
    am_mdelay(1000);
    am_led_off(0);
    am_mdelay(1000);
    am_led_on(0);
    am_mdelay(1000);
    am_led_off(0);

    /* 配置唤醒引脚 PTB0 下降沿LLWU 唤醒 */
    am_kl26_pmu_llwu_src_pin_mkinfo(&llwu_src_info[0],
                                    AMHW_FSL_LLWU_PIN_5_PTB0,
                                    AMHW_FSL_LLWU_PIN_ARG_FALLING);
    am_kl26_pmu_llwu_src_cfg(&llwu_src_info[0]);
    am_gpio_pin_cfg(PIOB_0, AM_GPIO_INPUT);
    


    /* 配置唤醒的内部模块为LPTMR 定时唤醒 */
    am_kl26_pmu_llwu_src_module_mkinfo(&llwu_src_info[1],
                                       AMHW_FSL_LLWU_MODULE_0_LPTMR0,
                                       AMHW_FSL_LLWU_MODULE_ARG_ENABLE);

    am_kl26_pmu_llwu_src_cfg(&llwu_src_info[1]);


    lptmr_timing_handle =  am_kl26_lptmr_timing_inst_init();

    am_timer_clkin_freq_get(lptmr_timing_handle, &clk_frequency);

    am_timer_callback_set(lptmr_timing_handle, 0, __lptmr_timing_callback, NULL);

    /* 设定中断频率 1Hz
     */
    am_timer_enable(lptmr_timing_handle, 0, clk_frequency);

    /* 配置一个普通按键中断，等待和停止模式唤醒 */
    am_gpio_pin_cfg(PIOC_0, AM_GPIO_INPUT);
    am_gpio_trigger_connect(PIOC_0, gpio_irq_handle, (void*)0);
    am_gpio_trigger_cfg(PIOC_0, AM_GPIO_TRIGGER_FALL);
    am_gpio_trigger_on(PIOC_0);


    while (1) {

        system_mode_sel_print();
        GET_CHAR(recv);

        switch (recv) {

        case 'a':   /* RUN */
            am_kprintf("Enter RUN_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_RUN) != AM_OK) {
                am_kprintf("Enter RUN_MODE Failed !\r\n");
            } else {
                uart0_baud_config();
            }
            current_mode_print();
            break;

        case 'b':   /* VLPR */
            am_kprintf("Enter VLPR_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLPR) != AM_OK) {
                am_kprintf("Enter VLPR_MODE Failed !\r\n");
            } else {
                amhw_kl26_sim_uart0_src_set(KL26_SIM_UART0SRC_MCGIRCLK);
                amhw_fsl_uart_ver0_baudrate_set(KL26_UART0,
                                           am_kl26_clk_periph_rate_get((void *)(KL26_UART0)),
                                           UART_BAUDRATE);
            }
            current_mode_print();
            break;

        case 'c':   /* WAIT */
            am_kprintf("Enter WAIT_MODE...\r\n");

            am_mdelay(100); 
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_WAIT) != AM_OK) {
                am_kprintf("Enter WAIT_MODE Failed !\r\n");
            } else {
                am_kprintf("Wake Up from WAIT_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'd':   /* VLPW */
            am_kprintf("Enter VLPW_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLPW) != AM_OK) {
                am_kprintf("Enter VLPW_MODE Failed !\r\n");
            } else {
                am_kprintf("Wake Up from VLPW_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'e':   /* STOP2 */
            am_kprintf("Enter STOP2_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_STOP2) != AM_OK) {
                am_kprintf("Enter STOP2_MODE Failed !\r\n");
            } else {
                am_kprintf("Wake Up from STOP2_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'f':   /* STOP1 */
            am_kprintf("Enter STOP1_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_STOP1) != AM_OK) {
                am_kprintf("Enter STOP1_MODE Failed !\r\n");
            } else {
                am_kprintf("Wake Up from STOP1_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'g':   /* STOP */
            am_kprintf("Enter STOP_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_STOP) != AM_OK) {
                am_kprintf("Enter STOP_MODE Failed !\r\n");
            } else {

                am_kprintf("Wake Up from STOP_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'h':   /* VLPS */
            am_kprintf("Enter VLPS_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLPS) != AM_OK) {
                am_kprintf("Enter VLPS_MODE Failed !\r\n");
            } else {

                am_kprintf("Wake Up from VLPS_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'i':   /* LLS */
            am_kprintf("Enter LLS_MODE...\r\n");
            am_mdelay(100);
            /* 开启 LLWU 中断*/
             am_int_connect(INUM_LLWU, (am_pfnvoid_t)am_kl26_pmu_llwu_irq_handle ,(void *)0);
             am_int_enable(INUM_LLWU);

            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_LLS) != AM_OK) {
                am_kprintf("Enter LLS_MODE Failed !\r\n");
            } else {

                am_kprintf("Wake Up from LLS_MODE !\r\n");
            }

            /* 开启 LLWU 中断*/
            am_int_connect(INUM_LLWU, (am_pfnvoid_t)am_kl26_pmu_llwu_irq_handle ,(void *)0);
            am_int_enable(INUM_LLWU);

            current_mode_print();
            break;

        case 'j':   /* VLSS3 */
            am_kprintf("Enter VLSS3_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLLS3) != AM_OK) {
                am_kprintf("Enter VLSS3_MODE Failed !\r\n");
            }
            current_mode_print();
            break;

        case 'k':   /* VLLS1 */
            am_kprintf("Enter VLLS1_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLLS1) != AM_OK) {
                am_kprintf("Enter VLLS1_MODE Failed !\r\n");
            }
            current_mode_print();
            break;

        case 'l':   /* VLLS0 */
            am_kprintf("Enter VLLS0_MODE...\r\n");
            am_mdelay(100);
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLLS0) != AM_OK) {
                am_kprintf("Enter VLLS0_MODE Failed !\r\n");
            }
            am_board_init();
            break;

        default:
            am_kprintf("\r\n Enter a wrong word!\r\n");
            break;
        }
    }
}
/*
 * 打印当前模式
 */
void current_mode_print ()
{
    uint8_t rev;

    am_kprintf("\r\n\r\n");
    if(am_kl26_pmu_mode_get() == AM_KL26_PMU_MODE_RUN) {
        am_kprintf("Current mode is RUN_MODE!\r\n");
    } else {
        am_kprintf("Current mode is VLPR_MODE!\r\n");
    }
    
    am_kprintf("Enter any key to select system mode!");

    GET_CHAR(rev);
}

/*
 * 打印系统模式选择信息
 */
void system_mode_sel_print ()
{
    am_kprintf("\r\n\r\n");
    am_kprintf("------------------------------------------------------------------------------------\r\n");
    am_kprintf("PTC0`s falling edge is used as wake up source from WAIT、VLPW、VLPS and STOPx mode！\r\n");
    am_kprintf("PTB0`s falling edge is used as wake up source from LLS、VLLS3、VLLS1和VLSS0 mode！\r\n");
    am_kprintf("\r\n---------System-Mode-Select------------\r\n");
    am_kprintf("Enter a  into RUN_MODE   \r\n");
    am_kprintf("      b  into VLPR_MODE  \r\n");
    am_kprintf("      c  into WAIT_MODE  \r\n");
    am_kprintf("      d  into VLPW_MODE  \r\n");
    am_kprintf("      e  into STOP2_MODE \r\n");
    am_kprintf("      f  into STOP1_MODE \r\n");
    am_kprintf("      g  into STOP_MODE  \r\n");
    am_kprintf("      h  into VLPS_MODE  \r\n");
    am_kprintf("      i  into LLS_MODE   \r\n");
    am_kprintf("      j  into VLLS3_MODE \r\n");
    am_kprintf("      k  into VLLS1_MODE \r\n");
    am_kprintf("      l  into VLLS0_MODE \r\n");
    am_kprintf("->");
}

void uart0_baud_config (void)
{
    /* 串口0使用内部参考时钟
     * 在am_kl26_hwconfig_uart.c配置uart0的时钟源为KL26_SIM_UART0SRC_MCGIRCLK
     */
    amhw_kl26_mcg_irc_disable();
    amhw_kl26_mcg_irc_src_set(KL26_TPM2_IRC_SRC_FAST);
    amhw_kl26_mcg_irc_enable();

    /* 波特率设置 */
    __g_handle = am_kl26_uart0_inst_init();
    am_debug_init(__g_handle,UART_BAUDRATE);

}

/** [src_kl26_std_pmu_lowpower] */

/* end of file */
