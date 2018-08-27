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
 * \brief MicroPort RS485 用户配置文件
 * \sa am_hwconf_microport_rs485.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-16  pea, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_gpio.h"

#include "am_gpio.h"
#include "am_fsl_uart.h"
#include "hw/amhw_fsl_uart.h"
#include "hw/amhw_kl26_sim.h"

#include "../../../../../soc/freescale/kl26/am_kl26.h"
#include "../../../../../soc/freescale/kl26/kl26_clk.h"
#include "../../../../../soc/freescale/kl26/kl26_inum.h"
#include "../../../../../soc/freescale/kl26/kl26_periph_map.h"
#include "../../../../../soc/freescale/kl26/kl26_pin.h"

/**
 * \addtogroup am_if_src_hwconf_microport_rs485
 * \copydoc am_hwconf_microport_rs485.c
 * @{
 */

/** \brief RS485方向控制引脚 */
#define __MICROPORT_RS485_DIR_PIN    PIOA_12

/**
 * \brief RS485 方向控制函数
 *
 * \param[in] is_txmode AM_TRUE: 发送模式， AM_FALSE: 接收模式
 */
void __microport_rs485_dir (am_bool_t is_txmode)
{
	am_gpio_set(__MICROPORT_RS485_DIR_PIN, (int)is_txmode);
}

/** \brief MicroPort RS485平台初始化 */
am_local void __microport_rs485_plfm_init (void)
{
    /* 引脚初始化      PIOA_1_UART0_RX  PIOA_2_UART0_TX   */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_UART0_RX);
    am_gpio_pin_cfg(PIOA_2, PIOA_2_UART0_TX);

    /* UART0时钟源选择                  */
    amhw_kl26_sim_uart0_src_set(  KL26_SIM_UART0SRC_PLLFLLCLK );
    /* 开启UART0时钟                  */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_UART0);

    /* 485 发送/接收方向控制引脚 */
    am_gpio_pin_cfg(__MICROPORT_RS485_DIR_PIN, AM_GPIO_OUTPUT_INIT_LOW | AM_GPIO_PULLDOWN);
    __microport_rs485_dir(AM_FALSE);
}

/** \brief 解除 MicroPort RS485 平台初始化 */
am_local void __microport_rs485_plfm_deinit (void)
{
    /* 关闭UART0时钟                  */
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_UART0);

    am_gpio_pin_cfg(PIOA_1, PIOA_1_GPIO);
    am_gpio_pin_cfg(PIOA_2, PIOA_2_GPIO);
}

/** \brief 串口0设备信息 */
static const am_fsl_uart_devinfo_t __g_microport_rs485_devinfo = {

    KL26_UART0,                      /**< \brief 串口0           */
    INUM_UART0,                      /**< \brief 串口0的中断编号.    */
	AM_FSL_UART_VER0,                /**< \brief 串口驱动的版本号.     */
	CLK_UART0,                       /**< \brief 串口0的时钟号.     */
    AMHW_FSL_UART_C1_M_8BIT      |   /**< \brief 8位数据.          */
    AMHW_FSL_UART_C1_PARITY_NO   |   /**< \brief 无极性.           */
    AMHW_FSL_UART_BDH_SBNS_STOP_1,   /**< \brief 1个停止位.         */

    115200,                          /**< \brief 设置的波特率.       */

    0,                               /**< \brief 无其他中断.         */

	__microport_rs485_dir,           /**< \brief USART0使用RS485   */
	__microport_rs485_plfm_init,     /**< \brief USART0的平台初始化.  */
	__microport_rs485_plfm_deinit,   /**< \brief USART0的平台去初始化. */
};

/** \brief 定义MicroPort RS485 设备 */
static am_fsl_uart_dev_t  __g_microport_rs485_dev;   /**< \brief 定义串口0 设备.*/

/** \brief MicroPort RS485 实例初始化，获得 USART0 标准服务句柄 */
am_uart_handle_t am_microport_rs485_inst_init (void)
{
	return am_fsl_uart_init(&__g_microport_rs485_dev, &__g_microport_rs485_devinfo);
}

/** \brief MicroPort RS485 实例解初始化 */
void am_microport_rs485_inst_deinit (am_uart_handle_t handle)
{
	am_fsl_uart_deinit((am_fsl_uart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
