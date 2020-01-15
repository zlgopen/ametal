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
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 连接 EEPROM 的 I2C 引脚到相应引脚。
 *
 * - 实验现象：
 *   1. 写数据到 EEPROM；
 *   2. 读出刚才写入的数据；
 *   3. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_mm32l073_hw_i2c_master_poll.c src_mm32l073_hw_i2c_master_poll
 *
 * \internal
 * \par History
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_hw_i2c_master_poll
 * \copydoc demo_mm32l073_hw_i2c_master_poll.c
 */

/** [src_mm32l073_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_mm32_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define I2C_M_7BIT    0x0000u          /**< \brief 7-bits 设备地址 */
#define I2C_M_10BIT   0x0001u          /**< \brief 10-bit 设备地址 */
#define I2C_M_WR      0x0000u          /**< \brief 写操作 */
#define I2C_M_RD      0x0002u          /**< \brief 读操作 */
#define I2C_M_NOSTART 0x0010u          /**< \brief 无需重新启动标识 */

#define I2C_SPEED     400000            /**< \brief I2C 控制器速度参数定义 */
#define EEPROM_ADDR   (0xa0 >> 1)       /**< \brief EEPROM 设备地址定义 */
#define TEST_LEN      0X08                 /**< \brief 操作 EEPROM 的页大小 */

/**
 * \brief I2C 传输结构体定义，主要用于 I2C 轮询模式下
 */
typedef struct i2c_transfer {

    /** \brief I2C设备地址 */
    volatile uint16_t   addr;

    /** \brief I2C 设备特性以及transfer需要的特殊标识 */
    volatile uint16_t   flags;

    /** \brief I2C 传输数据缓存指针 */
    volatile void      *p_buf;

    /** \brief I2C 数据缓存长度 */
    volatile uint16_t   length;
} i2c_transfer_t;

/** \brief I2C 传输结构体 */
static i2c_transfer_t __g_i2c1_transfer;

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
static int __i2c_mktrans (i2c_transfer_t *p_trans,
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
static void __i2c_mst_start (amhw_mm32_i2c_t *p_hw_i2c,
                             i2c_transfer_t *p_trans)
{
    if(!(p_trans->flags & I2C_M_NOSTART)) {

        amhw_mm32_i2c_disable(p_hw_i2c);

        /* 发送设备地址 */
        amhw_mm32_i2c_tar_set(p_hw_i2c, p_trans->addr);

        amhw_mm32_i2c_con_clear (p_hw_i2c, AMHW_MM32_I2C_7BITADDR_MASTER);

        amhw_mm32_i2c_enable(p_hw_i2c);
		}

    if (p_trans->flags & I2C_M_RD) {
        amhw_mm32_i2c_dat_write(p_hw_i2c, 0x0100);
    }
}

/**
 * \brief I2C 主机发送地址/数据
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 * \param[in] p_trans  指向I2C传输结构体的指针
 * \param[in] stop     是否发送停止信号
 *                         AM_TRUE  : 是
 *                         AM_FALSE : 否
 *
 * \retval AM_OK 发送完成
 */
static int __i2c_mst_send (amhw_mm32_i2c_t *p_hw_i2c,
                           i2c_transfer_t *p_trans,
                           am_bool_t       stop)

{
    uint8_t i;

    for (i = 0; i < p_trans->length; i++) {

        while (!(p_hw_i2c->ic_status & AMHW_MM32_STATUS_FLAG_TFNF));
        if (i == (p_trans->length - 1) && stop) {
            amhw_mm32_i2c_dat_write(p_hw_i2c,
                                   0x200 | ((uint8_t *)(p_trans->p_buf))[i]);
        } else {
            amhw_mm32_i2c_dat_write(p_hw_i2c,
                                  ((uint8_t *)(p_trans->p_buf))[i]);
        }
    }

    while (!(p_hw_i2c->ic_status & AMHW_MM32_STATUS_FLAG_MST_ACTIVITY));

    return AM_OK;
}

/**
 * \brief I2C 主机接收地址/数据
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 * \param[in] p_trans  指向I2C传输结构体的指针
 * \param[in] stop     是否发送停止信号
 *                         AM_TRUE  : 是
 *                         AM_FALSE : 否
 *
 * \retval AM_OK 接收完成
 */
static int __i2c_mst_recv (amhw_mm32_i2c_t *p_hw_i2c,
                           i2c_transfer_t *p_trans,
                           am_bool_t       stop)
{
    uint8_t i;

    for (i = 0; i < p_trans->length; i++) {
        while (!(p_hw_i2c->ic_status & AMHW_MM32_STATUS_FLAG_RFNE));

        /* 接收数据 */
        ((uint8_t *)(p_trans->p_buf))[i] = amhw_mm32_i2c_dat_read(p_hw_i2c);

        if (i == (p_trans->length - 2) && stop) {
            amhw_mm32_i2c_dat_write(p_hw_i2c, 0x0300);
        } else if (i <= (p_trans->length - 2)){
            amhw_mm32_i2c_dat_write(p_hw_i2c, 0x0100);
        }
    }
    return AM_OK;
}

/**
 * \brief I2C 主机初始化配置
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 * \param[in] speed    主机速度参数
 *
 * \retval  AM_OK     配置完成
 * \retval -AM_EINVAL 参数无效
 */
static int __i2c_mst_init (amhw_mm32_i2c_t *p_hw_i2c,
                           uint32_t        speed,
                           uint32_t        clk_rate)
{
    uint16_t mode  = AMHW_MM32_I2C_NORMAL_SPEED;
    uint16_t count = (clk_rate / speed) / 2;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /* 关闭 I2C 控制器，配置参数 */
    amhw_mm32_i2c_disable (p_hw_i2c);

    /* 设置速率 */
    if (speed <= 100000) {
        amhw_mm32_i2c_ss_scl_low_set(p_hw_i2c, count);
        amhw_mm32_i2c_ss_scl_high_set(p_hw_i2c, count);
        mode = AMHW_MM32_I2C_NORMAL_SPEED;
    } else {

        amhw_mm32_i2c_fs_scl_low_set(p_hw_i2c, count);
        amhw_mm32_i2c_fs_scl_high_set(p_hw_i2c, count);
        mode = AMHW_MM32_I2C_HIGH_SPEED;
    }

    p_hw_i2c->ic_con = (mode |
                        AMHW_MM32_I2C_MASTER |
                        AMHW_MM32_I2C_RESTART_ENABLE);

    amhw_mm32_i2c_rx_tl_set(p_hw_i2c, 0);
    amhw_mm32_i2c_tx_tl_set(p_hw_i2c, 1);

    /* 清除中断 */
    amhw_mm32_i2c_intr_mask_clear (p_hw_i2c, 0xfff);

    return AM_OK;
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_i2c_master_poll_entry (amhw_mm32_i2c_t *p_hw_i2c,
                                        uint32_t        clk_rate)
{
    i2c_transfer_t *p_trans              = &__g_i2c1_transfer;
    uint8_t         eeprom_buf[16]       = {0};
    uint8_t         test_addr[2]         = {0x00};
    uint8_t         i;

    __i2c_mst_init(p_hw_i2c, I2C_SPEED, clk_rate);   /* I2C 主机初始化配置 */

     /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_WR),
                  (uint8_t *)test_addr,
                  1);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_send (p_hw_i2c, p_trans, AM_FALSE);

    for (i = 0;i < TEST_LEN; i++) {
        eeprom_buf[i] = i + 0xA;
    }

     /* 设置 transfer 结构体参数，写入数据 */
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_NOSTART | I2C_M_WR),
                  (uint8_t *)eeprom_buf,
                  TEST_LEN);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_send(p_hw_i2c, p_trans, AM_TRUE);

    am_mdelay(10);

    /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_WR | I2C_M_NOSTART),
                  (uint8_t *)test_addr,
                  1);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_send (p_hw_i2c, p_trans, AM_FALSE);


   /* 设置 transfer 结构体参数，读取数据 */
    for (i = 0; i < TEST_LEN; i++) {
        eeprom_buf[i] = 0;
    }
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_RD),
                  (uint8_t *)eeprom_buf,
                  TEST_LEN);

    __i2c_mst_start(p_hw_i2c, p_trans);
    __i2c_mst_recv(p_hw_i2c, p_trans, AM_TRUE);

    am_mdelay(10);

    /* 校验写入和读取的数据是否一致 */
    for (i = 0;i < TEST_LEN; i++) {
        am_kprintf("Read EEPROM the %2dth data is  : 0x%02x\r\n", i, eeprom_buf[i]);

        /* 校验失败 */
        if(eeprom_buf[i] != (i + 0xA)) {
            am_kprintf("verify failed!\r\n");
            break;
        }
    }

    if (TEST_LEN == i) {
        am_kprintf("verify success!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_mm32l073_hw_i2c_master_poll] */

/* end of file */
