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
 * \brief I2C 从机例程（DMA 方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_10 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIO0_11 引脚连接 I2C 主机的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 接收主机发送过来的数据，通过串口发送给上位机；
 *   2. 传输失败则 LED0 灯长亮。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. DMA 初始化工作已经在 am_prj_config.c 文件内部完成。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_i2c_slave_dma.c src_lpc824_hw_i2c_slave_dma
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-04  cyl, modified the using of dma interface
 * - 1.00 15-07-16  win, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_lpc824_hw_i2c_slave_dma
 * \copydoc demo_lpc824_hw_i2c_slave_dma.c
 */

/** [src_lpc824_hw_i2c_slave_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x.h"
#include "am_lpc84x_dma.h"
#include "hw/amhw_lpc_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __SLAVE_ADDR      0x50              /**< \brief 主机操作从机地址定义 */

#define __TEST_LENTH      4                 /**< \brief 测试读写的字节数 */

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief DMA 重载描述符 */
am_local __attribute__((aligned(16)))  am_lpc84x_dma_xfer_desc_t __g_desc[2];

am_local uint8_t __g_buf_dsta[__TEST_LENTH];   /**< \brief A 目标端数据缓冲区 */
am_local uint8_t __g_buf_dstb[__TEST_LENTH];   /**< \brief B 目标端数据缓冲区 */

am_local volatile am_bool_t __g_a_trans_done;  /**< \brief A 目标端传输完成标志 */
am_local volatile am_bool_t __g_b_trans_done;  /**< \brief B 目标端传输完成标志 */

/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg 用户自定义参数，通过 am_lpc82x_dma_xfer_desc_startup() 函数传递
 * \param[in] stat  DMA 中断状态，由底层驱动传入，该参数的可能取值：
 *                      AM_LPC84X_DMA_STAT_INTA
 *                      AM_LPC84X_DMA_STAT_INTB
 *                      AM_LPC84X_DMA_STAT_INTERR
 *
 * \return 无
 */
am_local void __i2c_dma_isr (void *p_arg, int stat)
{
    if (stat & AM_LPC84X_DMA_STAT_INTA) {
        __g_a_trans_done = AM_TRUE;
        am_led_off(LED0);
    } else if (stat & AM_LPC84X_DMA_STAT_INTB) {
        __g_b_trans_done = AM_TRUE;
        am_led_off(LED0);
    } else if (stat & AM_LPC84X_DMA_STAT_INTERR) {

        /* 用户自定义执行代码 */
    }
}

/**
 * \brief I2C 从机初始化配置函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] dev_addr 主机操作从机的地址参数
 *
 * \retval AM_OK 初始化配置完成
 */
am_local int __i2c_slave_init (amhw_lpc_i2c_t *p_hw_i2c, uint8_t dev_addr)
{
    amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_SLAVE_MODE);
    amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_TIMEOUT_MODE);

    amhw_lpc_i2c_clkdiv_set(p_hw_i2c, 7);
    amhw_lpc_i2c_slv_addr_set(p_hw_i2c, AMHW_LPC_I2C_SLVADR_0, dev_addr);
    amhw_lpc_i2c_slvqual_set(p_hw_i2c, 0);

    amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_SLAVE_MODE);

    return AM_OK;
}

/**
 * \brief I2C 从机启动函数
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 *
 * \return 无
 *
 * \note 本函数仅仅针对主机读从机操作
 */
am_local void __i2c_slave_start (amhw_lpc_i2c_t *p_hw_i2c)
{
    uint8_t addr     = 0;
    uint8_t sub_addr = 0;

    while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_SLVPEND));
    while ((p_hw_i2c->stat & AMHW_LPC_I2C_SLAVE_STATE_MASK) !=
           AMHW_LPC_I2C_STAT_SLVADDR);

    /* 匹配到从机地址，启动从机 DMA 请求 */
    if (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_SLVIDX_MASK)) {

        /* 接收主机发送过来的从机地址, 地址最低位 1 或 0, 代表读或写操作 */
        addr = amhw_lpc_i2c_slvdat_read(p_hw_i2c);
        amhw_lpc_i2c_slv_continue(p_hw_i2c);

        while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_SLVPEND));
        while ((p_hw_i2c->stat & AMHW_LPC_I2C_SLAVE_STATE_MASK) !=
               AMHW_LPC_I2C_STAT_SLVRX);

        /* 接收主机发送过来的从机子地址 */
        sub_addr = amhw_lpc_i2c_slvdat_read(p_hw_i2c);
        amhw_lpc_i2c_slv_continue(p_hw_i2c);

        amhw_lpc_i2c_slv_dma_enable(p_hw_i2c);

        AM_DBG_INFO("Slave Address is %x\r\n", addr);
        AM_DBG_INFO("Slave Sub Address is %x\r\n", sub_addr);
    }
}

/**
 * \brief I2C 从机 DMA 接收数据，采用 ping-pong 操作
 */
am_local void __dma_i2c_test (amhw_lpc_i2c_t *p_hw_i2c, uint8_t chan)
{
    uint32_t                    flags1 = 0;
    uint32_t                    flags2 = 0;
    am_lpc84x_dma_controller_t *p_ctr  = NULL;

    p_ctr = am_lpc84x_dma_controller_get(chan,
                                         DMA_CHAN_OPT_PRIO_3 |
                                         DMA_CHAN_OPT_PERIPH_REQ_EN);

    /* DMA 传输配置 */
    flags1 =
        AM_LPC84X_DMA_XFER_VALID           | /* 当前配置描述符有效 */
        AM_LPC84X_DMA_XFER_RELOAD          | /* 传输完成重载通道描述符 */
        AM_LPC84X_DMA_XFER_SWTRIG          | /* 软件触发 */
        AM_LPC84X_DMA_XFER_WIDTH_8BIT      | /* 传输数据宽度：8 bit */
        AM_LPC84X_DMA_XFER_SRCINC_NOINC    | /* 源端数据地址不递增 */
        AM_LPC84X_DMA_XFER_DSTINC_1X       | /* 目标端数据地址按 1 个数据宽度递增 */
        AM_LPC84X_DMA_XFER_SETINTA;          /* 使能中断 A */

    /* DMA 传输配置 */
    flags2 =
        AM_LPC84X_DMA_XFER_VALID           | /* 当前配置描述符有效 */
        AM_LPC84X_DMA_XFER_RELOAD          | /* 传输完成重载通道描述符 */
        AM_LPC84X_DMA_XFER_SWTRIG          | /* 软件触发 */
        AM_LPC84X_DMA_XFER_WIDTH_8BIT      | /* 传输数据宽度：8 bit */
        AM_LPC84X_DMA_XFER_SRCINC_NOINC    | /* 源端数据地址不递增 */
        AM_LPC84X_DMA_XFER_DSTINC_1X       | /* 目标端数据地址按 1 个数据宽度递增 */
        AM_LPC84X_DMA_XFER_SETINTB;          /* 使能中断 B */

    /* 建立通道描述符 A */
    am_lpc84x_dma_xfer_desc_build(
       &__g_desc[0],                           /* A 的重载描述符 */
        (uint32_t)&(LPC84X_I2C0->slvdat), /* 源端数据缓冲区 */
        (uint32_t)__g_buf_dsta,                /* A 的目标端缓冲区地址 */
        __TEST_LENTH,                          /* 传输字节数 */
        flags1);                               /* 传输配置 */

    /* 建立通道描述符 B */
    am_lpc84x_dma_xfer_desc_build(
       &__g_desc[1],                           /* B 的重载描述符 */
        (uint32_t)&(LPC84X_I2C0->slvdat), /* 源端数据缓冲区 */
        (uint32_t)__g_buf_dstb,                /* B 的目标端缓冲区地址 */
        __TEST_LENTH,                          /* 传输字节数 */
        flags2);                               /* 传输配置 */

    /* 连接两个通道描述符 */
    am_lpc84x_dma_xfer_desc_link(&__g_desc[1], &__g_desc[0]);
    am_lpc84x_dma_xfer_desc_link(&__g_desc[0], &__g_desc[1]);

    /* 启动 DMA 传输，马上开始传输 */
    am_lpc84x_dma_xfer_desc_startup(p_ctr,
                                    &__g_desc[0],
                                    __i2c_dma_isr,
                                    NULL);

    __i2c_slave_start(p_hw_i2c);
}

void demo_lpc845_hw_i2c_slave_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                         uint8_t         chan)
{
    uint8_t i = 0;

    /* I2C 主机初始化配置 */
    __i2c_slave_init(p_hw_i2c, __SLAVE_ADDR);

    /* 点亮 LED0 */
    am_led_on(LED0);

    /* I2C 测试函数 */
    __dma_i2c_test(p_hw_i2c, chan);

    AM_FOREVER {
        if (__g_a_trans_done) {
            __g_a_trans_done = AM_FALSE;
            for (i = 0; i < __TEST_LENTH; i++) {
                AM_DBG_INFO("A_buf %2dst is 0x%02x\r\n", i, __g_buf_dsta[i]);
            }
        }

        if (__g_b_trans_done) {
            __g_b_trans_done = AM_FALSE;
            for (i = 0; i < __TEST_LENTH; i++) {
                AM_DBG_INFO("B_buf %2dst is 0x%02x\r\n", i, __g_buf_dstb[i]);
            }
        }
    }
}
/** [src_lpc824_hw_i2c_slave_dma] */

/* end of file */
