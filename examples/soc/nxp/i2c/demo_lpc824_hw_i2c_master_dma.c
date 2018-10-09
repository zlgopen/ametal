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
 * \snippet demo_lpc824_hw_i2c_master_dma.c src_lpc824_hw_i2c_master_dma
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-04  cyl, modified the using of dma interface
 * - 1.00 15-07-16  win, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_lpc824_hw_i2c_master_dma
 * \copydoc demo_lpc824_hw_i2c_master_dma.c
 */

/** [src_lpc824_hw_i2c_master_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x.h"
#include "am_lpc82x_dma.h"
#include "hw/amhw_lpc_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __I2C_MASTER  LPC82X_I2C0      /**< \brief I2C 主机控制器通道号定义 */
#define __I2C_CLKDIV  15               /**< \brief I2C 控制器时钟分频参数定义 */

#define __AM_I2C_RD   1                /**< \brief I2C 读操作 */
#define __AM_I2C_WR   0                /**< \brief I2C 写操作 */

#define __TX_DAM_ARG  0                /**< \brief I2C DMA 发送完成回调函数参数 */
#define __RX_DAM_ARG  1                /**< \brief I2C DMA 接收完成回调函数参数 */

#define __TEST_LENTH  8                /**< \brief 测试读写的字节数(不能大于页大小) */


/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

am_local uint8_t __g_buf_srca[__TEST_LENTH];    /**< \brief A 源端数据缓冲区 */
am_local uint8_t __g_buf_dstb[__TEST_LENTH];    /**< \brief B 目标端数据缓冲区 */

am_local volatile am_bool_t __g_trans_done_tx;  /**< \brief 传输完成标志 */
am_local volatile am_bool_t __g_trans_done_rx;  /**< \brief 接收完成标志 */

  
/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg 用户自定义参数，通过 am_lpc82x_dma_xfer_desc_startup() 函数传递
 * \param[in] stat  DMA 中断状态，由底层驱动传入，该参数的可能取值：
 *                      AM_LPC82X_DMA_STAT_INTA
 *                      AM_LPC82X_DMA_STAT_INTB
 *                      AM_LPC82X_DMA_STAT_INTERR
 *
 * \return 无
 */
am_local void __i2c_dma_isr (void *p_arg, volatile int stat)
{
    if ((stat & AM_LPC82X_DMA_STAT_INTA) && ((int)p_arg == __TX_DAM_ARG)) {
        while ((__I2C_MASTER->stat & AMHW_LPC_I2C_MASTER_STATE_MASK) !=
                AMHW_LPC_I2C_STAT_MSTTX);

        __g_trans_done_tx = AM_TRUE;
        amhw_lpc_i2c_mst_dma_disable(LPC82X_I2C0);
        amhw_lpc_i2c_mst_stop(LPC82X_I2C0);
    }

    if ((stat & AM_LPC82X_DMA_STAT_INTA) && ((int)p_arg == __RX_DAM_ARG)) {
        while ((__I2C_MASTER->stat & AMHW_LPC_I2C_MASTER_STATE_MASK) !=
                AMHW_LPC_I2C_STAT_MSTRX);

        __g_trans_done_rx = AM_TRUE;
        amhw_lpc_i2c_mst_dma_disable(LPC82X_I2C0);
        amhw_lpc_i2c_mst_stop(LPC82X_I2C0);
    }

    if (stat & AM_LPC82X_DMA_STAT_INTERR) {

        /* 用户自定义执行代码 */
    }
}

/**
 * \brief I2C 主机初始化配置
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] clkdiv   I2C 主机时钟分频参数
 *
 * \retval  AM_OK     配置完成
 * \retval -AM_EINVAL 参数无效
 */
am_local int __i2c_mst_init (amhw_lpc_i2c_t *p_hw_i2c,
                             uint32_t        clkdiv)
{
    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /* 关闭 I2C 控制器，配置参数 */
    amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_MASTER_MODE);

    /* 设置时钟分频参数 */
    amhw_lpc_i2c_clkdiv_set(p_hw_i2c, clkdiv);

    amhw_lpc_i2c_msttime_set(p_hw_i2c, 9, 9);

    /* 参数配置完成，启动 I2C 控制器 */
    amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_MASTER_MODE);

    return AM_OK;
}

/**
 * \brief 启动 I2C, 发送从机地址和从机子地址
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 * \param[in] dev_addr 从机地址
 * \param[in] sub_addr 子地址
 * \param[in] flags    值位 __AM_I2C_RD 或 __AM_I2C_WR
 *
 * \return 无
 */
am_local void __i2c_mst_start (amhw_lpc_i2c_t *p_hw_i2c,
                               uint32_t        dev_addr,
                               uint32_t        sub_addr,
                               uint32_t        flags)
{
    dev_addr = dev_addr << 1;

    /* 发送设备地址 */
    amhw_lpc_i2c_mstdat_write(p_hw_i2c, dev_addr);
    amhw_lpc_i2c_mst_start(p_hw_i2c);       /* I2C 控制器发送启动信号 */

    while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_MSTPEND));
    while ((__I2C_MASTER->stat & AMHW_LPC_I2C_MASTER_STATE_MASK) !=
            AMHW_LPC_I2C_STAT_MSTTX);

    amhw_lpc_i2c_mstdat_write(p_hw_i2c, sub_addr);
    amhw_lpc_i2c_mst_continue(p_hw_i2c);

    if (flags == __AM_I2C_RD) {
        dev_addr = dev_addr | 0x01;

        while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_MSTPEND));
        while ((__I2C_MASTER->stat & AMHW_LPC_I2C_MASTER_STATE_MASK) !=
                AMHW_LPC_I2C_STAT_MSTTX);

        /* 发送设备地址 */
        amhw_lpc_i2c_mstdat_write(p_hw_i2c, dev_addr);
        amhw_lpc_i2c_mst_start(p_hw_i2c);       /* I2C 控制器发送启动信号 */
    }

    while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_MSTPEND)) ;

    /* 使能 I2C DMA 请求 */
    amhw_lpc_i2c_mst_dma_enable(p_hw_i2c);
}

/**
 * \brief DMA I2C 测试函数
 */
am_local void __i2c_dma_test (uint8_t addr, uint8_t sub_addr)
{
    int                         i     = 0;
    am_lpc82x_dma_controller_t *p_ctr = NULL;
    am_lpc82x_dma_transfer_t    transfer;

    p_ctr = am_lpc82x_dma_controller_get(DMA_CHAN_I2C0_MST_REQ,
                                         DMA_CHAN_OPT_PRIO_3 |
                                         DMA_CHAN_OPT_PERIPH_REQ_EN);

    for (i = 0; i < __TEST_LENTH; i++) {
        __g_buf_srca[i] = i + 8;
    }

    am_lpc82x_dma_transfer_build(&transfer,
                                  (uint32_t)__g_buf_srca,
                                  (uint32_t)&(LPC82X_I2C0->mstdat),
                                  __TEST_LENTH,
                                  AM_LPC82X_DMA_TRANS_WIDTH_8BIT |
                                  AM_LPC82X_DMA_TRANS_SRCINC_1X |
                                  AM_LPC82X_DMA_TRANS_DSTINC_NOINC);

    am_lpc82x_dma_transfer(p_ctr,
                          &transfer,
                           __i2c_dma_isr,
                           (void*)__TX_DAM_ARG);

    __i2c_mst_start(LPC82X_I2C0, addr, sub_addr, __AM_I2C_WR);

    am_mdelay(10);

    am_lpc82x_dma_transfer_build(&transfer,
                                  (uint32_t)&(LPC82X_I2C0->mstdat),
                                  (uint32_t)__g_buf_dstb,
                                  __TEST_LENTH,
                                  AM_LPC82X_DMA_TRANS_WIDTH_8BIT |
                                  AM_LPC82X_DMA_TRANS_SRCINC_NOINC |
                                  AM_LPC82X_DMA_TRANS_DSTINC_1X);

    am_lpc82x_dma_transfer(p_ctr,
                          &transfer,
                           __i2c_dma_isr,
                           (void*)__RX_DAM_ARG);

    __i2c_mst_start(LPC82X_I2C0, addr, sub_addr, __AM_I2C_RD);

    am_mdelay(10);

    for (i = 0; i < __TEST_LENTH; i++) {
        if (__g_buf_dstb[i] != __g_buf_srca[i]) {
            AM_DBG_INFO("verify failed at index %d\r\n", i);
        } else {
            AM_DBG_INFO("Read EEPROM the %2dth data is 0x%02x\r\n",
                        i,
                        __g_buf_dstb[i]);
        }
    }

    if (__TEST_LENTH == i) {
        AM_DBG_INFO("i2c dma transfer done!\r\n");
    }
}


void demo_lpc824_hw_i2c_master_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                          uint32_t        clkdiv,
                                          uint8_t         addr,
                                          uint8_t         sub_addr)
{
    /* I2C 主机初始化配置 */
    __i2c_mst_init(p_hw_i2c, clkdiv);

    __i2c_dma_test(addr, sub_addr);

    AM_FOREVER {
        if (__g_trans_done_tx && __g_trans_done_rx) {
            am_led_toggle(LED0);
            am_mdelay(200);
        }
    }
}



/** [src_lpc824_hw_i2c_master_dma] */

/* end of file */
