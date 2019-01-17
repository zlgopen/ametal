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
 * \brief I2C 从机例程（中断方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_10 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIO0_11 引脚连接 I2C 主机的 SDA 引脚。
 *
 * - 实验现象：
 *   1. 从机接收主机写地址和数据到从机，并通过串口打印出来；
 *   2. 从机发送来自主机的数据，并通过串口打印处理；
 *   3. LED0 以 200ms 时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_i2c_slave_int.c src_lpc_hw_i2c_slave_int
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_i2c_slave_int
 * \copydoc demo_lpc_hw_i2c_slave_int.c
 */

/** [src_lpc_hw_i2c_slave_int] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x.h"
#include "hw/amhw_lpc_i2c.h"
#include "hw/amhw_lpc84x_clk.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __OPERATE_ADDR    0x50             /**< \brief 主机操作从机地址定义 */

#define __TEST_LENTH      8                /**< \brief 测试读写的字节数 */

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/
volatile uint8_t __g_i2c_slv_rxrdy_flg  = 0;  /**< \brief 从机接收准备就绪标识 */
volatile uint8_t __g_i2c_slv_txrdy_flg  = 0;  /**< \brief 从机发送准备就绪标识 */

/**
 * \brief I2C 从机初始化配置函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] dev_addr 主机操作从机的地址参数
 *
 * \retval AM_OK 初始化配置完成
 */
am_local int __i2c_slave_init (amhw_lpc_i2c_t *p_hw_i2c,
                               uint8_t         dev_addr)
{
    amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_SLAVE_MODE);
    amhw_lpc_i2c_slv_addr_set(p_hw_i2c, AMHW_LPC_I2C_SLVADR_0, dev_addr);
    amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_SLAVE_MODE);

    return AM_OK;
}

/**
 * \brief I2C 从机发送数据
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] p_txbuf  发送数据/地址缓存
 * \param[in] nbytes   发送缓存的长度
 *
 * \retval AM_OK 发送完成
 */
am_local int __i2c_slv_send (amhw_lpc_i2c_t   *p_hw_i2c,
                             am_const uint8_t *p_txbuf,
                             uint32_t          nbytes)
{
    uint8_t i = 0;

    amhw_lpc_i2c_int_enable(p_hw_i2c, AMHW_LPC_I2C_INTENSET_SLVPEND);

    for (i = 0; i < nbytes; i++) {
        while (!__g_i2c_slv_txrdy_flg);
        __g_i2c_slv_txrdy_flg = 0;

        /* 从机写入数据 */
        amhw_lpc_i2c_slvdat_write(p_hw_i2c, *p_txbuf++);

        amhw_lpc_i2c_slv_continue(p_hw_i2c);
        amhw_lpc_i2c_int_enable(p_hw_i2c, AMHW_LPC_I2C_INTENSET_SLVPEND);
    }

    return AM_OK;
}

/**
 * \brief I2C 从机接收数据
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] p_rxbuf  接收数据/地址缓存
 * \param[in] nbytes   接收缓存的长度
 *
 * \retval AM_OK 接收完成
 */
am_local int __i2c_slv_recv (amhw_lpc_i2c_t *p_hw_i2c,
                             uint8_t        *p_rxbuf,
                             uint32_t        nbytes)
{
    uint8_t i = 0;

    amhw_lpc_i2c_int_enable(p_hw_i2c, AMHW_LPC_I2C_INTENSET_SLVPEND);
    am_kprintf("2.\n");
    for (i = 0; i < nbytes; i++) {

        while (!__g_i2c_slv_rxrdy_flg);
        __g_i2c_slv_rxrdy_flg = 0;
    am_kprintf("3.\n");
        /* 从机接收数据 */
        *p_rxbuf++ = amhw_lpc_i2c_slvdat_read(p_hw_i2c);

        amhw_lpc_i2c_slv_continue(p_hw_i2c);
        amhw_lpc_i2c_int_enable(p_hw_i2c, AMHW_LPC_I2C_INTENSET_SLVPEND);

    }

    return AM_OK;
}

/**
 * \brief I2C 从机错误状态函数
 *
 * \param[in] p_hw_i2c   指向 I2C 寄存器块的指针
 * \param[in] i2c_status 当前 I2C 控制器的状态参数
 *
 * \return 无
 */
am_local void __i2c_slv_errorstat (amhw_lpc_i2c_t *p_hw_i2c,
                                   uint32_t        i2c_status)
{
    uint32_t slverr_stat = i2c_status;

    if(slverr_stat & AMHW_LPC_I2C_STAT_SLVNOTSTR) {
        p_hw_i2c->intenclr = AMHW_LPC_I2C_STAT_SLVNOTSTR;

    } else if (slverr_stat & AMHW_LPC_I2C_STAT_SLVDESEL) {
        p_hw_i2c->stat = AMHW_LPC_I2C_STAT_SLVDESEL;
    }
}

/**
 * \brief I2C 从机状态机函数
 *
 * \param[in] p_hw_i2c   指向 I2C 寄存器块的指针
 * \param[in] i2c_status 当前 I2C 控制器的状态参数
 *
 * \return 无
 */
am_local void __i2c_slv_sm_event (amhw_lpc_i2c_t *p_hw_i2c,
                                  uint32_t        i2c_status)
{
    uint32_t i2c_slv_stat = (p_hw_i2c->stat) & AMHW_LPC_I2C_SLAVE_STATE_MASK;

    switch (i2c_slv_stat) {

    case AMHW_LPC_I2C_STAT_SLVADDR:
        amhw_lpc_i2c_slv_continue(p_hw_i2c);
        amhw_lpc_i2c_int_enable(p_hw_i2c, AMHW_LPC_I2C_INTENSET_SLVPEND);
        break;

    case AMHW_LPC_I2C_STAT_SLVRX:
        __g_i2c_slv_rxrdy_flg = 1;
        break;

    case AMHW_LPC_I2C_STAT_SLVTX:
        __g_i2c_slv_txrdy_flg = 1;
        break;

    default:
        break;
    }
}

/**
 * \brief I2C 从机中断服务函数
 *
 * \param[in] p_arg 中断函数的入口参数
 *
 * \return 无
 */
am_local void __i2c_slave_isr (void *p_arg)
{
    amhw_lpc_i2c_t *p_hw_i2c   = (amhw_lpc_i2c_t *)p_arg;
    uint32_t        i2c_status = p_hw_i2c->intstat;

    if (i2c_status == 0) {
        return;
    }

    __i2c_slv_errorstat(p_hw_i2c, i2c_status);

    if (i2c_status & AMHW_LPC_I2C_STAT_SLVPEND) {
        p_hw_i2c->intenclr = AMHW_LPC_I2C_INTENCLR_SLVPEND;
        __i2c_slv_sm_event(p_hw_i2c, i2c_status);
    }
}

void demo_lpc845_hw_i2c_slave_int_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                         int             inum)
{
    uint8_t  i2c_rxdata_buf[__TEST_LENTH] = {0};
    uint8_t  i2c_rxaddr_buf[__TEST_LENTH] = {0};
    uint16_t i                            = 0;

    /* I2C 从机初始化配置 */
    __i2c_slave_init(p_hw_i2c, __OPERATE_ADDR);

    /* 使能 slave 相关中断 */
    amhw_lpc_i2c_int_enable(p_hw_i2c, AMHW_LPC_I2C_INTENSET_SLVPEND |
                                      AMHW_LPC_I2C_INTENSET_SLVNOTSTR |
                                      AMHW_LPC_I2C_INTENSET_SLVDESEL);

    am_int_connect(inum, __i2c_slave_isr, p_hw_i2c);
    am_int_enable(inum);

    am_kprintf("1.\n");
    
    /* 接收主机发送给从机的地址 */
    __i2c_slv_recv(p_hw_i2c,
                   (uint8_t *)i2c_rxaddr_buf,
                   1);
    am_kprintf("10.\n");
    /* 接收主机发送给从机的数据 */
    __i2c_slv_recv(p_hw_i2c,
                   (uint8_t *)i2c_rxdata_buf,
                   __TEST_LENTH);

    /* 接收主机发送给从机的地址 */
    __i2c_slv_recv(p_hw_i2c,
                   (uint8_t *)i2c_rxaddr_buf,
                   1);


    /* 发送从主机接收到的数据 */
    __i2c_slv_send(p_hw_i2c,
                   (uint8_t *)i2c_rxdata_buf,
                   __TEST_LENTH);

    AM_DBG_INFO("slave receive address is 0x%02x \r\n", i2c_rxaddr_buf[0]);

    for (i = 0;i < __TEST_LENTH; i++) {
        AM_DBG_INFO("slave transmit %2dst data is 0x%02x \r\n",
                    i,
                    i2c_rxdata_buf[i]);
    }
}
/** [src_lpc_hw_i2c_slave_int] */

/* end of file */
