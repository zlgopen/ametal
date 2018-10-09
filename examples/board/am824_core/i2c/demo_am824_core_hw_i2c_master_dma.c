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
 * \brief I2C 读写 EEPROM 例程（DMA 方式），通过 HW 层接口实现
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
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. DMA 初始化工作已经在 am_prj_config.c 文件内部完成。
 *
 * \par 源代码
 * \snippet demo_am824_hw_i2c_master_dma.c src_am824_hw_i2c_master_dma
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-04  cyl, modified the using of dma interface
 * - 1.00 15-07-16  win, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_am824_hw_i2c_master_dma
 * \copydoc demo_am824_hw_i2c_master_dma.c
 */

/** [src_am824_hw_i2c_master_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __I2C_MASTER  LPC82X_I2C0      /**< \brief I2C 主机控制器通道号定义 */
#define __I2C_CLKDIV  15               /**< \brief I2C 控制器时钟分频参数定义 */

#define __EEPROM_ADDR 0x50             /**< \brief EEPROM 器件地址 */
#define __SUB_ADDR    0x00             /**< \brief 从机子地址 */



/*******************************************************************************
  本地全局变量定义
*******************************************************************************/


/**
 * \brief 例程入口
 */
void demo_am824_core_hw_i2c_master_dma_entry (void)
{
    AM_DBG_INFO("demo am824 master dma!\r\n");

    am_gpio_pin_cfg(PIO0_10, PIO0_10_I2C0_SCL);
    am_gpio_pin_cfg(PIO0_11, PIO0_11_I2C0_SDA);

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_I2C0);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_I2C0);

    demo_lpc824_hw_i2c_master_dma_entry(__I2C_MASTER, 
                                        __I2C_CLKDIV, 
                                        __EEPROM_ADDR, 
                                        __SUB_ADDR);
    
}
/** [src_am824_hw_i2c_master_dma] */

/* end of file */
