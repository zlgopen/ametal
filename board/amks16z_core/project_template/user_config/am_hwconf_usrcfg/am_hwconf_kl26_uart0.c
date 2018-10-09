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
 * \brief kl26 UART0 用户配置文件
 * \sa am_kl26_hwconfig_uart0.c
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-15  nwt, make some changes.
 * - 1.00 15-10-21  xym, first implementation.
 * \endinternal
 */

#include "kl26_periph_map.h"
#include "kl26_pin.h"
#include "am_gpio.h"
#include "am_fsl_uart.h"
#include "hw/amhw_fsl_uart.h"
#include "hw/amhw_kl26_sim.h"
#include "kl26_inum.h"
#include "kl26_clk.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_uart0
 * \copydoc am_kl26_hwconfig_uart0.c
 * @{
 */

/**< \brief 定义RS485用来控制方向的引脚（使能485方向控制函数后有效） */
#define AM_KL26_UART0_485_DIR_PIN   PIOB_18

/**< \brief 变量声明 */
static const am_fsl_uart_devinfo_t __g_uart0_devinfo;

/** \brief uart0 485 发送/接收方向控制 */
static void __uart0_int_485_send_cfg (am_bool_t flag)
{
    if (flag) {
        am_gpio_set(AM_KL26_UART0_485_DIR_PIN, 1);
    } else {
        am_gpio_set(AM_KL26_UART0_485_DIR_PIN, 0);
    }
}

/** \brief 串口0平台初始化 */
static void __kl26_plfm_uart0_init (void)
{
    /* 引脚初始化      PIOA_1_UART0_RX  PIOA_2_UART0_TX   */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_UART0_RX);
    am_gpio_pin_cfg(PIOA_2, PIOA_2_UART0_TX);

    /* UART0时钟源选择                  */
    amhw_kl26_sim_uart0_src_set(  KL26_SIM_UART0SRC_PLLFLLCLK );
    /* 开启UART0时钟                  */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_UART0);
    
    if (__g_uart0_devinfo.uart_int_485_send != NULL) {
        /* 485 发送/接收方向控制引脚 */
        am_gpio_pin_cfg(AM_KL26_UART0_485_DIR_PIN, AM_GPIO_OUTPUT_INIT_LOW | AM_GPIO_PULLDOWN);
        __uart0_int_485_send_cfg(AM_FALSE);
    }
}

/** \brief 解除串口0平台初始化 */
static void __kl26_plfm_uart0_deinit (void)
{
    /* 关闭UART0时钟                  */
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_UART0);

    am_gpio_pin_cfg(PIOA_1, PIOA_1_GPIO);
    am_gpio_pin_cfg(PIOA_2, PIOA_2_GPIO);
}

/** \brief 串口0设备信息 */
static const am_fsl_uart_devinfo_t __g_uart0_devinfo = {

    KL26_UART0,                 /**< \brief 串口0           */
    INUM_UART0,                      /**< \brief 串口0的中断编号.    */
	AM_FSL_UART_VER0,                /**< \brief 串口驱动的版本号.   */
	CLK_UART0,                       /**< \brief 串口0的时钟号.     */
    AMHW_FSL_UART_C1_M_8BIT      |   /**< \brief 8位数据.          */
    AMHW_FSL_UART_C1_PARITY_NO   |   /**< \brief 无极性.           */
    AMHW_FSL_UART_BDH_SBNS_STOP_1,   /**< \brief 1个停止位.         */

    115200,                          /**< \brief 设置的波特率.       */

    0,                               /**< \brief 无其他中断.         */

    NULL,                            /**< \brief USART0不使用RS485   */
    __kl26_plfm_uart0_init,          /**< \brief USART0的平台初始化.  */
    __kl26_plfm_uart0_deinit,        /**< \brief USART0的平台去初始化. */

};

static am_fsl_uart_dev_t  __g_uart0_dev;   /**< \brief 定义串口0 设备.*/

/** \brief UART0实例初始化，获得uart0标准服务句柄 */
am_uart_handle_t am_kl26_uart0_inst_init (void)
{
    return am_fsl_uart_init(&__g_uart0_dev, &__g_uart0_devinfo);
}

/** \brief UART0实例解初始化 */
void am_kl26_uart0_inst_deinit (am_uart_handle_t handle)
{
    am_fsl_uart_deinit((am_fsl_uart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */



