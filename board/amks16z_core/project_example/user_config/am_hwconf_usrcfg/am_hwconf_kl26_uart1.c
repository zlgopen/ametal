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
 * \brief kl26 UART1 用户配置文件
 * \sa am_kl26_hwconfig_uart1.c
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-15  nwt, make some changes.
 * - 1.00 15-10-21  xym, first implementation.
 * \endinternal
 */

#include "am_gpio.h"
#include "am_fsl_uart.h"
#include "hw/amhw_fsl_uart.h"
#include "hw/amhw_kl26_sim.h"

#include "../../../../../soc/freescale/kl26/kl26_clk.h"
#include "../../../../../soc/freescale/kl26/kl26_inum.h"
#include "../../../../../soc/freescale/kl26/kl26_periph_map.h"
#include "../../../../../soc/freescale/kl26/kl26_pin.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_uart1
 * \copydoc am_kl26_hwconfig_uart1.c
 * @{
 */

/** \brief 串口1平台初始化 */
static void __kl26_plfm_uart1_init (void)
{
    /* 引脚初始化      PIOC_3_UART1_RX  PIOC_4_UART1_TX   */
    am_gpio_pin_cfg(PIOC_3, PIOC_3_UART1_RX);
    am_gpio_pin_cfg(PIOC_4, PIOC_4_UART1_TX);

    /* 开启UART1时钟                  */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_UART1);
}

/** \brief 解除串口1平台初始化 */
static void __kl26_plfm_uart1_deinit (void)
{
     /* 关闭UART1时钟                  */
     amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_UART1);

     am_gpio_pin_cfg(PIOC_3, PIOC_3_GPIO);
     am_gpio_pin_cfg(PIOC_4, PIOC_4_GPIO);
}

/** \brief 串口1设备信息 */
static const am_fsl_uart_devinfo_t __g_uart1_devinfo = {

    KL26_UART1,                      /**< \brief 串口1            */
    INUM_UART1,                      /**< \brief 串口1的中断编号.    */
	AM_FSL_UART_VER1,                /**< \brief 串口驱动的版本号.     */
	CLK_UART1,                       /**< \brief 串口1的时钟号.     */
    AMHW_FSL_UART_C1_M_8BIT      |  /**< \brief 8位数据.          */
	AMHW_FSL_UART_C1_PARITY_NO   |  /**< \brief 无极性.           */
    AMHW_FSL_UART_BDH_SBNS_STOP_1,  /**< \brief 1个停止位.         */

    115200,                          /**< \brief 设置的波特率.       */

    0,                               /**< \brief 无其他中断.         */

    NULL,                            /**< \brief USART1不使用RS485   */
    __kl26_plfm_uart1_init,          /**< \brief USART1的平台初始化.  */
    __kl26_plfm_uart1_deinit,        /**< \brief USART1的平台去初始化. */
};

static am_fsl_uart_dev_t  __g_uart1_dev;   /**< \brief 定义串口1 设备.*/

/** \brief UART1实例初始化，获得uart1标准服务句柄 */
am_uart_handle_t am_kl26_uart1_inst_init (void)
{
    return am_fsl_uart_init(&__g_uart1_dev, &__g_uart1_devinfo);
}

/** \brief UART1实例解初始化 */
void am_kl26_uart1_inst_deinit (am_uart_handle_t handle)
{
    am_fsl_uart_deinit((am_fsl_uart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */



