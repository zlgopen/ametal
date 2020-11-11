/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_11 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOA_12 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    当前 I2C 的 SCL 引脚使用的是 PIOA_11，SDA 引脚使用的是 PIOA_12，
 *    可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_i2c_slave_poll.c src_zsl42x_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-16  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_i2c_slave_poll
 * \copydoc demo_zsl42x_hw_i2c_slave_poll.c
 */

/** [src_zsl42x_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_zsl42x_i2c.h"
#include "am_int.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** 主机操作从机地址定义 */
#define OPERATE_ADDR      0x50

#define EEPROM_BUFFER_LEN (0x20)

/*******************************************************************************
  全局变量
*******************************************************************************/

static uint8_t   sub_addr;
static uint8_t   eeprom[EEPROM_BUFFER_LEN] = {0};   /**< 缓冲区 */

/**
 * \brief 获取发送一个字节回调
 */
static int __txbyte_get (void *p_arg, uint8_t *p_byte)
{
    *p_byte = eeprom[sub_addr];
    sub_addr++;
    return AM_OK;
}

/**
 * \brief 接受一个字节回调
 */
static int __rxbyte_put(void *p_arg, uint8_t byte)
{
    eeprom[sub_addr] = byte;
    sub_addr++;
    return AM_OK;
}

/**
 * \brief 停止传输回调
 */
static void  __tran_stop(void *p_arg)
{
    sub_addr = 0;
}

/**
 * \brief I2C 从机初始化配置函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] dev_addr 主机操作从机的地址参数
 *
 * \retval AM_OK 初始化配置完成
 */
static int __i2c_slave_init (amhw_zsl42x_i2c_t *p_hw_i2c, uint8_t dev_addr)
{
    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /* 清空I2C控制寄存器 */
    p_hw_i2c->i2c_cr = 0;

    /* 从机模式，开启应答 */
    amhw_zsl42x_i2c_cr_set(p_hw_i2c, AMHW_ZSL42x_I2C_REPLY_ENABLE);

    /* 禁用波特率计数器 */
    amhw_zsl42x_i2c_tm_disable(p_hw_i2c);

    /* 设置从机地址 */
    amhw_zsl42x_i2c_slave_addr_set (p_hw_i2c, dev_addr);

    amhw_zsl42x_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/**
 * \brief I2C 状态机函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 *
 * \return 无
 */
static void __i2c_slave_event (void *p_i2c_slv)
{
    static uint8_t           rx_len       = 0;
    amhw_zsl42x_i2c_t         *p_hw_i2c_slv = p_i2c_slv;
    uint8_t                  state;
    uint8_t                  rx_data;
    uint8_t                  tx_data;

    state = amhw_zsl42x_i2c_status_get(p_hw_i2c_slv);

    switch(state) {

    case 0xa0:  /* 接收到停止条件或重复起始条件 */
        __tran_stop(NULL);
        rx_len = 0;
        amhw_zsl42x_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSL42x_I2C_REPLY_ENABLE);
        break;

    case 0x60:  /* 已接收到(与自身匹配的)SLA+W；已接收ACK */
    case 0x70:  /* 已接收通用调用地址（0x00）；已接收ACK */
        rx_len = 0;
        amhw_zsl42x_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSL42x_I2C_REPLY_ENABLE);
        break;

    case 0x68:  /* 主控时在SLA+读写丢失仲裁；已接收自身的SLA+W；已返回ACK */
    case 0x78:  /* 主控时在SLA+读写中丢失仲裁；已接收通用调用地址；已返回ACK */
    case 0x88:  /* 前一次寻址使用自身从地址；已接收数据字节；已返回非ACK */
        amhw_zsl42x_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSL42x_I2C_REPLY_ENABLE);
        break;

    case 0x80:  /* 前一次寻址使用自身从地址；已接收数据字节；已返回ACK */
    case 0x98:  /* 前一次寻址使用通用调用地址；已接收数据；已返回非ACK */
    case 0x90:  /* 前一次寻址使用通用调用地址；已接收数据；已返回ACK */
        amhw_zsl42x_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSL42x_I2C_REPLY_ENABLE);
        rx_data = amhw_zsl42x_i2c_dat_read(p_hw_i2c_slv);/* 接收数据 */
        if(rx_len) {    /* 忽略接收的第一个数据(从机设备子地址) */
            __rxbyte_put(NULL, rx_data);    /* 接受回调 */
        }
        rx_len++;
        break;

    case 0xa8:  /* 已接收自身的SLA+R；已返回ACK,将要发出数据并将收到ACK位 */
    case 0xb0:  /* 当主控时在SLA+读写中丢失仲裁；已接收自身SLA+R；已返回ACK */
    case 0xc8:  /* 装入的数据字节已被发送；已接收ACK */
    case 0xb8:  /* 已发送数据；已接收ACK */
        __txbyte_get(NULL, &tx_data);
        amhw_zsl42x_i2c_dat_write(p_hw_i2c_slv,tx_data);
        break;

    case 0xc0:  /* 发送数据，接收非ACKn */
        __tran_stop(NULL);
        break;

    default:
        break;
    }

    /* 清除中断标志 */
    amhw_zsl42x_i2c_cr_clear(p_hw_i2c_slv, AMHW_ZSL42x_I2C_INT_FLAG);
}

/**
 * \brief 例程入口
 */
void demo_zsl42x_hw_i2c_slave_int_entry (void    *p_hw_i2c,
                                       int      inum_num)
{
    __i2c_slave_init(p_hw_i2c, OPERATE_ADDR); /* I2C 从机初始化配置 */

    /* 连接中断 */
    am_int_connect(inum_num, __i2c_slave_event, (void *)p_hw_i2c);
    am_int_enable(inum_num);

    while(1) {
        ;
    }
}
/** [src_zsl42x_hw_i2c_slave_poll] */

/* end of file */
