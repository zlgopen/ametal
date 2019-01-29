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
 * \brief I2C 读写 EEPROM 例程（轮询方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_10 引脚连接 EEPROM 的 SCL 引脚；
 *   2. PIO0_11 引脚连接 EEPROM 的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 向 EEPROM 设备写入 __TEST_LENTH 字节数据；
 *   2. 读取 EEPROM 中的数据通过串口打印出来；
 *   3. 若写入数据和读取的数据一致，校验通过，LED0 以 200ms 间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_i2c_master_poll.c src_am845_core_hw_i2c_master_poll
 *
 * \internal
 * \par History
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_i2c_master_poll
 * \copydoc demo_am845_core_hw_i2c_master_poll.c
 */

/** [src_am845_core_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __I2C_MASTER    LPC84X_I2C0      /**< \brief I2C 主机控制器通道号定义 */
#define __I2C_SPEED     400000           /**< \brief I2C 控制器速度参数定义 */
#define __I2C_CLKDIV    6                /**< \brief I2C 控制器时钟分频参数定义 */
#define __EEPROM_ADDR   0x50             /**< \brief EEPROM 设备地址定义 */


/**
 * \brief I2C 主机速率设置
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] speed    主机速度参数
 *
 * \retval AM_OK 设置完成
 */
am_local int __i2c_speed_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t speed)
{
    uint32_t scl   = 0;
    uint32_t scl_h = 0;
    uint32_t scl_l = 0;

    scl = amhw_lpc84x_clk_periph_freq_get(p_hw_i2c) /
          ((amhw_lpc_i2c_clkdiv_get(p_hw_i2c)+1) * speed);

    scl_h = scl >> 1;
    scl_l = (scl - scl_h);
    if (scl_h < 2) {
        scl_h = 2;

    } else if (scl_h > 9) {
        scl_h = 9;
    }

    if (scl_l < 2) {
        scl_l = 2;

    } else if (scl_l > 9) {
        scl_l = 9;
    }
    amhw_lpc_i2c_msttime_set(p_hw_i2c, scl_h, scl_l);

    return AM_OK;
}



  
/**
 * \brief 例程入口
 */
void demo_am845_core_hw_i2c_master_poll_entry (void)
{
    AM_DBG_INFO("demo am845 I2C master poll !\r\n");

    am_gpio_pin_cfg(PIO0_10, PIO0_10_I2C0_SCL);
    am_gpio_pin_cfg(PIO0_11, PIO0_11_I2C0_SDA);

    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_I2C0);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_I2C0);
    amhw_lpc84x_clk_i2c0_clk_sel_set (AMHW_LPC84X_CLK_DEVICE_FRG0CLK);

    /* 设置速率 */
    __i2c_speed_set(__I2C_MASTER, __I2C_SPEED);

    demo_lpc_hw_i2c_master_poll_entey(__I2C_MASTER, __I2C_CLKDIV, __EEPROM_ADDR);
}
/** [src_am845_core_hw_i2c_master_poll] */

/* end of file */
