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
 * \snippet demo_zlg118_hw_i2c_master_poll.c src_zlg118_hw_i2c_master_poll
 *
 * \internal
 * \par History
 * - 1.00 19-12-16  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_i2c_master_poll
 * \copydoc demo_zlg118_hw_i2c_master_poll.c
 */

/** [src_zlg118_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_errno.h"
#include "hw/amhw_zlg118_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define I2C_M_7BIT          0x0000u     /**< \brief 7-bits 设备地址 */
#define I2C_M_10BIT         0x0001u     /**< \brief 10-bit 设备地址 */
#define I2C_M_WR            0x0000u     /**< \brief 写操作 */
#define I2C_M_RD            0x0002u     /**< \brief 读操作 */
#define I2C_M_NOSTART       0x0004u     /**< \brief 无需重新启动标识 */

#define I2C_SUBADDR_NONE    0x0000u     /**< \brief 无子地址(默认)  */
#define I2C_SUBADDR_1BYTE   0x1000u     /**< \brief 子地址宽度1字节 */
#define I2C_SUBADDR_2BYTE   0x2000u     /**< \brief 子地址宽度2字节 */

#define I2C_SPEED           100000      /**< \brief I2C 控制器速度参数定义 */
#define EEPROM_ADDR         0x50        /**< \brief EEPROM 设备地址定义 */
#define TEST_LEN            0x08        /**< \brief 操作 EEPROM 的页大小 */

#define __I2C_ST_IDLE       (0x10u)     /* 空闲状态 */
#define __I2C_ST_MSG_START  (0x11u)     /* 消息传输开始状态 */

/**
 * \brief I2C 传输结构体定义，主要用于 I2C 轮询模式下
 */
typedef struct i2c_transfer {

    /** \brief I2C设备地址 */
    volatile uint16_t   addr;

    /** \brief I2C 设备特性以及transfer需要的特殊标识 */
    uint16_t            flags;

    /** \brief I2C 传输数据缓存指针 */
    uint8_t            *p_buf;

    /** \brief I2C 数据缓存长度 */
    uint16_t            length;
} i2c_transfer_t;

typedef struct i2c_message {
    i2c_transfer_t     *p_transfer;
    uint16_t            trans_num;      /**< \brief 请求处理的传输个数          */
    uint16_t            done_num;       /**< \brief 成功处理的传输个数          */
    int                 status;         /**< \brief 消息的状态                         */
} i2c_message_t;

/** \brief I2C 传输结构体 */
static i2c_transfer_t __g_i2c1_transfer[4];

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

static void __i2c_mkmsg (i2c_message_t  *p_msg,
                         i2c_transfer_t *p_transfer,
                         uint16_t        trans_num)
{
    p_msg->p_transfer   = p_transfer;
    p_msg->trans_num    = trans_num;
    p_msg->done_num     = 0;
    p_msg->status       = -AM_ENOTCONN;
}

/**
 * \brief I2C 主机启动函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] p_trans  指向 I2C 传输结构体的指针
 *
 * \return 无
 */
static am_err_t __i2c_msg_start (amhw_zlg118_i2c_t  *p_hw_i2c,
                                 i2c_message_t      *p_msg)
{
    int     state;
    uint8_t data_ptr = 0;

    i2c_transfer_t *p_cur_trans = p_msg->p_transfer;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /* 发送起始条件 */
    amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_START_ENABLE);

    while(1) {
        /* 获取中断标志 */
        while(0 == amhw_zlg118_i2c_irq_get(p_hw_i2c));
        p_msg->status = __I2C_ST_MSG_START;

        /* 获取I2C状态 */
        state = amhw_zlg118_i2c_status_get(p_hw_i2c);

        if(p_cur_trans[p_msg->done_num].flags & I2C_M_RD) {  /* 读操作 */
            switch (state) {

            case 0x08: /* 已发送起始条件 */
            case 0x10: /* 已发送重复起始条件 */

                /* 清除起始条件 */
                amhw_zlg118_i2c_cr_clear(p_hw_i2c,
                                         AMHW_ZLG118_I2C_START_ENABLE);

                /* 传输从机地址和读命令 */
                amhw_zlg118_i2c_dat_write(p_hw_i2c,
                                          ((p_cur_trans->addr << 1) | 0x1));
                break;

            case 0x18: /* 已发送SLA+W，已接收ACK */

                /* 传输目标地址 */
                amhw_zlg118_i2c_dat_write(p_hw_i2c,
                        p_cur_trans[p_msg->done_num - 1].p_buf[data_ptr]);
                break;

            case 0x28: /* 已发送数据，已接收ACK */

                /* 发送重复起始条件 */
                amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_START_ENABLE);
                break;

            case 0x40: /* 已发送SLA +R,已接收ACK */

                /* 使能应答 */
                if(p_cur_trans[p_msg->done_num].length > 1) {
                    amhw_zlg118_i2c_cr_set(p_hw_i2c,
                                           AMHW_ZLG118_I2C_REPLY_ENABLE);
                }
                break;

            case 0x48: /* 已发送SLA +R,已接收非ACK */
                amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_STOP_ENABLE);
                amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_START_ENABLE);
                break;

            case 0x50:/* 已接收数据字节，ACK已返回 */

                /* 接收数据 */
                p_cur_trans[p_msg->done_num].p_buf[data_ptr++] = \
                    amhw_zlg118_i2c_dat_read(p_hw_i2c);

                if(data_ptr == p_cur_trans[p_msg->done_num].length - 1) {

                    /* 禁能应答 */
                    amhw_zlg118_i2c_cr_clear(p_hw_i2c,
                                             AMHW_ZLG118_I2C_REPLY_ENABLE);
                }
                break;

            case 0x58:/* 已接收数据字节，非ACK已返回 */

                /* 接收最后一个数据 */
                p_cur_trans[p_msg->done_num].p_buf[data_ptr++] = \
                    amhw_zlg118_i2c_dat_read(p_hw_i2c);
                p_msg->status = __I2C_ST_IDLE;
                break;

            case 0x38: /* 在SLA+ 读写或写数据字节时丢失仲裁 */
            default:
                p_msg->status = __I2C_ST_IDLE;
                break;
            }
        } else {    /* 写操作 */
            switch (state) {

            case 0x08: /* 已发送起始条件 */

                /* 清除起始条件 */
                amhw_zlg118_i2c_cr_clear(p_hw_i2c,
                                         AMHW_ZLG118_I2C_START_ENABLE);

                /* 传输从机地址和写命令 */
                amhw_zlg118_i2c_dat_write(p_hw_i2c, (p_cur_trans->addr << 1));
                break;

            case 0x18: /* 已发送SLA+W，已接收ACK */
            case 0x28: /* 已发送数据，已接收ACK */
                if(p_msg->done_num <= p_msg->trans_num) {

                    /* 传输数据 */
                    amhw_zlg118_i2c_dat_write(p_hw_i2c,
                                              p_cur_trans[p_msg->done_num].\
                                              p_buf[data_ptr++]);
                }
                break;

            case 0x20: /* 已发送SLA+W，已接收非ACK */
            case 0x30: /* 已发送数据 */
            case 0x38: /* 在SLA+ 读写或写数据字节时丢失仲裁 */
            default:
                p_msg->status = __I2C_ST_IDLE;
                break;
            }
        }

        /* 判断消息是否传输完成 */
        if(data_ptr >= p_cur_trans[p_msg->done_num].length) {
            p_msg->done_num++;
            data_ptr = 0;
        }

        if(p_msg->status == __I2C_ST_IDLE ||
           p_msg->done_num > p_msg->trans_num) {

            /* 设置停止停止标志 */
            amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_STOP_ENABLE);

            /* 清除中断标志位 */
            amhw_zlg118_i2c_cr_clear(p_hw_i2c, AMHW_ZLG118_I2C_INT_FLAG);
            break;
        } else if(p_msg->done_num == p_msg->trans_num) {
            p_msg->done_num++;
        }

        /* 清除中断标志位 */
        amhw_zlg118_i2c_cr_clear(p_hw_i2c, AMHW_ZLG118_I2C_INT_FLAG);
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
static int __i2c_mst_init (amhw_zlg118_i2c_t *p_hw_i2c,
                           uint32_t        speed,
                           uint32_t        clk_rate)
{
    uint16_t count = ((clk_rate / speed) >> 3) - 1;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c->i2c_cr = 0;

    /* 设置速率 */
    if (speed < 100000) {
        amhw_zlg118_i2c_cr_clear(p_hw_i2c, AMHW_ZLG118_I2C_HIGH_SPEED);
    } else {
        amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_HIGH_SPEED);
    }
    amhw_zlg118_i2c_tm_set(p_hw_i2c, count);
    amhw_zlg118_i2c_tm_enable(p_hw_i2c);

    amhw_zlg118_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_i2c_master_poll_entry (void        *p_hw_i2c,
                                           uint32_t     clk_rate)
{
    i2c_message_t   msg;
    i2c_transfer_t *p_trans              = __g_i2c1_transfer;
    uint8_t         eeprom_buf[16]       = {0};
    uint8_t         test_addr[2]         = {0x02};
    uint8_t         i;

    __i2c_mst_init(p_hw_i2c, I2C_SPEED, clk_rate);   /* I2C 主机初始化配置 */

     /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(&p_trans[0],
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_SUBADDR_1BYTE | I2C_M_WR),
                  (uint8_t *)test_addr,
                  1);

    for (i = 0;i < TEST_LEN; i++) {
        eeprom_buf[i] = i + 0xA;
    }

     /* 设置 transfer 结构体参数，写入数据 */
    __i2c_mktrans(&p_trans[1],
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_SUBADDR_1BYTE |
                   I2C_M_NOSTART | I2C_M_WR),
                  (uint8_t *)eeprom_buf,
                  TEST_LEN);

    __i2c_mkmsg(&msg, p_trans, 2);
    __i2c_msg_start(p_hw_i2c, &msg);

    am_mdelay(10);

    /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(&p_trans[0],
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_WR | I2C_SUBADDR_1BYTE),
                  (uint8_t *)test_addr,
                  1);

   /* 设置 transfer 结构体参数，读取数据 */
    for (i = 0; i < TEST_LEN; i++) {
        eeprom_buf[i] = 0;
    }
    __i2c_mktrans(&p_trans[1],
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_RD | I2C_SUBADDR_1BYTE),
                  (uint8_t *)eeprom_buf,
                  TEST_LEN);

    __i2c_mkmsg(&msg, p_trans, 2);
    __i2c_msg_start(p_hw_i2c, &msg);

    am_mdelay(10);

    /* 校验写入和读取的数据是否一致 */
    for (i = 0;i < TEST_LEN; i++) {
        am_kprintf("Read EEPROM the %2dth data is : 0x%02x\r\n",
                   i,
                   eeprom_buf[i]);

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
/** [src_zlg118_hw_i2c_master_poll] */

/* end of file */
