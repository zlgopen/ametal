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
 * \brief I2C 从机例程（轮询方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_10 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIO0_11 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    此例程可以用来模拟 EEPROM。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_i2c_slave_poll.c src_lpc_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-10  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_i2c_slave_poll
 * \copydoc demo_lpc_hw_i2c_slave_poll.c
 */

/** [src_lpc_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x.h"
#include "hw/amhw_lpc_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __OPERATE_ADDR      0x50         /**< \brief 主机操作从机地址定义 */

#define __EEPROM_BUFFER_LEN (0x20)       /**< \brief 模拟的 EEPROM 缓冲区大小 */

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/
am_local uint32_t  __g_operate;              /**< \brief I2C 从机操作位 */
am_local am_bool_t __g_ssub_addr = AM_FALSE; /**< \brief I2C 从机地址标志 */
am_local uint8_t   __g_sub_addr;             /**< \brief I2C 从机地址 */

am_local uint8_t   __g_eeprom_buf[__EEPROM_BUFFER_LEN]; /**< \brief 缓冲区 */

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

    amhw_lpc_i2c_slv_addr_set(p_hw_i2c, AMHW_LPC_I2C_SLVADR_0, dev_addr);

    amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_SLAVE_MODE);

    return AM_OK;
}

/**
 * \brief I2C 状态机函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 *
 * \return 无
 */
am_local void __i2c_slave_event (amhw_lpc_i2c_t *p_hw_i2c)
{
    if (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_SLVPEND)) {
        return ;
    }

    /* 从机未选中 */
    if (p_hw_i2c->stat & AMHW_LPC_I2C_STAT_SLVDESEL) {
        __g_operate = 0;
        p_hw_i2c->stat |= AMHW_LPC_I2C_STAT_SLVDESEL;
        return ;
    }

    switch (p_hw_i2c->stat & AMHW_LPC_I2C_SLAVE_STATE_MASK) {

    /* 主机发送从机地址 */
    case AMHW_LPC_I2C_STAT_SLVADDR:

        /* 从机地址匹配成功 */
        if (!(p_hw_i2c->stat & AMHW_LPC_I2C_STAT_SLVIDX_MASK)) {
            __g_operate = amhw_lpc_i2c_slvdat_read(p_hw_i2c) & 0x1;
            if (!__g_operate) {
                __g_ssub_addr = AM_TRUE;
            } else {
                __g_operate = 1;
            }
            amhw_lpc_i2c_slv_continue(p_hw_i2c);
        }
        break;

    /* 从机接收就绪 */
    case AMHW_LPC_I2C_STAT_SLVRX:
        if (__g_ssub_addr == AM_TRUE) {
            __g_sub_addr = amhw_lpc_i2c_slvdat_read(p_hw_i2c);
            amhw_lpc_i2c_slv_continue(p_hw_i2c);
            __g_ssub_addr =  AM_FALSE;
            break;
        }

        /* 未做 __g_sub_addr 合法性的判断 */
        __g_eeprom_buf[__g_sub_addr++] = amhw_lpc_i2c_slvdat_read(p_hw_i2c);
        amhw_lpc_i2c_slv_continue(p_hw_i2c);

        break;

    /* 从机发送就绪 */
    case AMHW_LPC_I2C_STAT_SLVTX:
        if (__g_operate) {
            amhw_lpc_i2c_slvdat_write(p_hw_i2c, __g_eeprom_buf[__g_sub_addr++]);
            amhw_lpc_i2c_slv_continue(p_hw_i2c);
        }
        break;

    default:
        break;
    }
}

void demo_lpc_hw_i2c_slave_poll_entry (amhw_lpc_i2c_t *p_hw_i2c)
{
    __i2c_slave_init(p_hw_i2c, __OPERATE_ADDR); /* I2C 从机初始化配置 */

    AM_FOREVER {
        __i2c_slave_event(p_hw_i2c);
    }
}
/** [src_lpc_hw_i2c_slave_poll] */

/* end of file */
