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
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_i2c_master_poll.c src_lpc_hw_i2c_master_poll
 *
 * \internal
 * \par History
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_i2c_master_poll
 * \copydoc demo_lpc_hw_i2c_master_poll.c
 */

/** [src_lpc_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
//#include "am_lpc82x.h"
#include "hw/amhw_lpc_i2c.h"
//#include "hw/amhw_lpc82x_clk.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __I2C_M_7BIT    0x0000u          /**< \brief 7-bits 设备地址 */
#define __I2C_M_10BIT   0x0001u          /**< \brief 10-bit 设备地址 */
#define __I2C_M_WR      0x0000u          /**< \brief 写操作 */
#define __I2C_M_RD      0x0002u          /**< \brief 读操作 */
#define __I2C_M_NOSTART 0x0010u          /**< \brief 无需重新启动标识 */

#define __TEST_LENTH    8                /**< \brief 测试读写的字节数(不能大于页大小) */

/**
 * \brief I2C 传输结构体定义，主要用于 I2C 轮询模式下
 */
typedef struct i2c_transfer {

    /** \brief I2C 设备地址 */
    volatile uint16_t addr;

    /** \brief I2C 设备特性以及 transfer 需要的特殊标识 */
    volatile uint16_t flags;

    /** \brief I2C 传输数据缓存指针 */
    volatile void    *p_buf;

    /** \brief I2C 数据缓存长度 */
    volatile uint16_t length;
} i2c_transfer_t;

/** \brief I2C 传输结构体 */
am_local i2c_transfer_t __g_i2c0_transfer;

/**
 * \brief I2C 传输结构体参数设置函数
 *
 * \param[in] p_trans 指向 I2C 传输结构体的指针
 * \param[in] addr    设备地址参数
 * \param[in] flags   传输控制标识位参数
 * \param[in] p_buf   指向发送或者接收数据缓存的指针
 * \param[in] length  缓存的字节长度参数
 *
 * \retval AM_OK 传输结构体设置完成
 */
am_local int __i2c_mktrans (i2c_transfer_t *p_trans,
                            uint16_t        addr,
                            uint16_t        flags,
                            void           *p_buf,
                            uint16_t        length)
{
    if(p_trans == NULL) {
        return -AM_EINVAL;
    }

    p_trans->addr   = addr;
    p_trans->flags  = flags;
    p_trans->p_buf  = p_buf;
    p_trans->length = length;

    return AM_OK;
}

/**
 * \brief I2C 主机启动函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] p_trans  指向 I2C 传输结构体的指针
 *
 * \return 无
 */
am_local void __i2c_mst_start (amhw_lpc_i2c_t *p_hw_i2c,
                               i2c_transfer_t *p_trans)
{
    uint16_t dev_addr = (p_trans->addr) << 1;

    if(!(p_trans->flags & __I2C_M_NOSTART)) {

        if (p_trans->flags & __I2C_M_RD) {
            dev_addr |= 0x1;            /* 设置 bit0 为 1，表明当前为读操作 */
        } else {
            dev_addr |= 0x0;            /* 设置 bit0 为 0，表明当前为写操作 */
        }

        /* 发送设备地址 */
        amhw_lpc_i2c_mstdat_write(p_hw_i2c, dev_addr);
        amhw_lpc_i2c_mst_start(p_hw_i2c);       /* I2C 控制器发送启动信号 */
    }
}

/**
 * \brief I2C 主机发送地址/数据
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] p_trans  指向 I2C 传输结构体的指针
 *
 * \retval AM_OK 发送完成
 */
am_local int __i2c_mst_send (amhw_lpc_i2c_t *p_hw_i2c,
                             i2c_transfer_t *p_trans)

{
    uint8_t i = 0;

    for (i = 0; i < p_trans->length; i++) {

        while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_MSTPEND));
        while ((p_hw_i2c->stat & AMHW_LPC_I2C_MASTER_STATE_MASK) !=
               AMHW_LPC_I2C_STAT_MSTTX);

        amhw_lpc_i2c_mstdat_write(p_hw_i2c, ((uint8_t *)(p_trans->p_buf))[i]);
        amhw_lpc_i2c_mst_continue(p_hw_i2c);
    }
    while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_MSTPEND));
    while ((p_hw_i2c->stat & AMHW_LPC_I2C_MASTER_STATE_MASK) !=
           AMHW_LPC_I2C_STAT_MSTTX);

    return AM_OK;
}

/**
 * \brief I2C主机接收地址/数据
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] p_trans  指向 I2C 传输结构体的指针
 *
 * \retval AM_OK 接收完成
 */
am_local int __i2c_mst_recv (amhw_lpc_i2c_t *p_hw_i2c,
                             i2c_transfer_t *p_trans)
{
    uint8_t i;

    for (i = 0; i < p_trans->length; i++) {
        while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_MSTPEND));

        while (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_MSTPEND));
        while ((p_hw_i2c->stat & AMHW_LPC_I2C_MASTER_STATE_MASK) !=
                    AMHW_LPC_I2C_STAT_MSTRX);

        /* 接收数据 */
        ((uint8_t *)(p_trans->p_buf))[i] = amhw_lpc_i2c_mstdat_read(p_hw_i2c);

        if (i != (p_trans->length - 1)) {
            amhw_lpc_i2c_mst_continue(p_hw_i2c);
        }
    }
    return AM_OK;
}


/**
 * \brief I2C 主机初始化配置
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] speed    主机速度参数
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

    /* 参数配置完成，启动 I2C 控制器 */
    amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_MASTER_MODE);

    return AM_OK;
}

void demo_lpc_hw_i2c_master_poll_entey(amhw_lpc_i2c_t *p_hw_i2c,
                                       uint32_t        clkdiv,
                                       uint8_t         addr)
{
    uint8_t         i                        = 0;
    uint8_t         eeprom_buf[__TEST_LENTH] = {0};
    uint8_t         test_addr[2]             = {0x00};
    i2c_transfer_t *p_trans                  = &__g_i2c0_transfer;
    
    /* I2C 主机初始化配置 */
    __i2c_mst_init(p_hw_i2c, clkdiv);

     /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(p_trans,
                  addr,
                  (__I2C_M_7BIT | __I2C_M_WR),
                  (uint8_t *)test_addr,
                  1);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_send(p_hw_i2c, p_trans);

    for (i = 0;i < __TEST_LENTH; i++) {
        eeprom_buf[i] = i + 0xA;
    }

     /* 设置 transfer 结构体参数，写入数据 */
    __i2c_mktrans(p_trans,
                  addr,
                  (__I2C_M_7BIT | __I2C_M_NOSTART | __I2C_M_WR),
                  (uint8_t *)eeprom_buf,
                  __TEST_LENTH);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_send(p_hw_i2c, p_trans);

    /* 结束当前传输 */
    amhw_lpc_i2c_mst_stop(p_hw_i2c);

    am_mdelay(10);

    /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(p_trans,
                  addr,
                  (__I2C_M_7BIT | __I2C_M_WR),
                  (uint8_t *)test_addr,
                  1);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_send(p_hw_i2c, p_trans);


   /* 设置 transfer 结构体参数，读取数据 */
    for (i = 0; i < __TEST_LENTH; i++) {
        eeprom_buf[i] = 0;
    }
    __i2c_mktrans(p_trans,
                  addr,
                  (__I2C_M_7BIT | __I2C_M_RD),
                  (uint8_t *)eeprom_buf,
                  __TEST_LENTH);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_recv(p_hw_i2c, p_trans);

    /* 结束当前传输 */
    amhw_lpc_i2c_mst_stop(p_hw_i2c);

    am_mdelay(10);

    /* 校验写入和读取的数据是否一致 */
    for (i = 0;i < __TEST_LENTH; i++) {
        AM_DBG_INFO("Read EEPROM the %2dth data is 0x%02x\r\n",
                    i,
                    eeprom_buf[i]);

        /* 校验失败 */
        if(eeprom_buf[i] != (i + 0xA)) {
            AM_DBG_INFO("verify failed!\r\n");
            break;
        }
    }

    if (__TEST_LENTH == i) {
        AM_DBG_INFO("verify success!\r\n");
    }

    /* 校验成功，LED0 以 200ms 时间间隔闪烁 */
    AM_FOREVER {
        am_led_on(LED0);
        am_mdelay(200);
        am_led_off(LED0);
        am_mdelay(200);
    }
}
/** [src_lpc_hw_i2c_master_poll] */

/* end of file */
