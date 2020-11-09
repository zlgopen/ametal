/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief Modbus 从站测试demo
 *
 * - 使用注意：
 *
 *   1. Modbus 主机例程默认使用软件定时器，使用软件定时器作为Modbus的定时时，
 *      请开启软件定时器的配置宏 am_prj_config.h 文件的 AM_CFG_SOFTIMER_ENABLE 宏
 *
 *   2. 使用硬件定时器时，配置硬件定时器的用户配置文件选择32位定时器长度模式
 *
 * \par 源代码
 * \snippet am_mb_slave_modbus_demo.c src_mb_slave_modbus_demo
 *
 * \internal
 * \par Modification history
 * - 1.00 17-03-31  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup grp_am_if_mb_slv
 * \copydoc am_mb_slave_modbus_demo.c
 */

/** [src_mb_slave_modbus_demo] */
#include "am_system.h"
#include "am_mb_slave.h"
#include "am_mb_ascii_comm.h"
#include "am_mb_utils.h"
#include "am_zsl42x_inst_init.h"
#include "demo_am42x_core_entries.h"
/*******************************************************************************
 * Macro Definition
 ******************************************************************************/
#define _AM_MB_MASK_WR_REG_FUNCODE   (22)  /* 写寄存器掩码功能码 */

#define _AM_MB_COIL_START_ADDR       (1000) /**< \brief 线圈起始地址为0   */
#define _AM_MB_COIL_END_ADDR         (1160) /**< \brief 线圈结束地址为160 */
#define _AM_MB_COIL_NUM              (20)   /**< \brief 支持的线圈数为20 * 8个 */

#define _AM_MB_HOLD_REGS_START_ADDR  (1000) /**< \brief 保持寄存器起始地址为0   */
#define _AM_MB_HOLD_REGS_END_ADDR    (1010) /**< \brief 保持寄存器结束地址为10 */
#define _AM_MB_HODL_REGS_NUM         (10)  /**< \brief 支持的保持寄存器为10个 */

#define _AM_MB_INPUT_REGS_START_ADDR (1000)   /**< \brief 输入寄存器起始地址为0   */
#define _AM_MB_INPUT_REGS_END_ADDR   (1010)  /**< \brief 输入寄存器结束地址为10 */
#define _AM_MB_INPUT_REGS_NUM        (10)  /**< \brief 支持的保持寄存器为10个 */

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/* 分配线圈内存并设置初值 */
am_local uint8_t  _g_coils_buf[_AM_MB_COIL_NUM] = {
    0xFF, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
};

/* 分配保持寄存器和输入寄存器内存并设置初值 */
am_local uint16_t _g_hold_regs[_AM_MB_HODL_REGS_NUM]   = {0,1,2,3,4,5,6,7,8,9};
am_local uint16_t _g_input_regs[_AM_MB_INPUT_REGS_NUM] = {10,11,12,13,14,15,16,17,18,19};

/******************************************************************************/
/** \brief 内存提取16位无符号数(大端模式) */
am_static_inline uint16_t _mb_to_u16 (const uint8_t *buf)
{
    return ((uint16_t)buf[0] << 8) + buf[1];
}

/******************************************************************************/
/** \brief 读线圈回调函数 */
am_local
am_mb_exception_t _mb_slv_read_coils_callback (am_mb_slave_handle slave,
                                               uint8_t           *p_buf,
                                               uint16_t           addr,
                                               uint16_t           num)
{
    if ((addr < _AM_MB_COIL_START_ADDR) || (addr > _AM_MB_COIL_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_ADDRESS;
    }

    if (addr + num > _AM_MB_COIL_END_ADDR) {
        return AM_MB_EXP_ILLEGAL_DATA_VALUE;
    }

    /* 读线圈值到buf中*/
    am_mb_bits_cp_to_buf(p_buf, _g_coils_buf, addr - _AM_MB_COIL_START_ADDR, num);

    return AM_MB_EXP_NONE;
}
/******************************************************************************/
/** \brief 写线圈回调函数 */
am_local
am_mb_exception_t _mb_slv_write_coils_callback (am_mb_slave_handle slave,
                                                uint8_t           *p_buf,
                                                uint16_t           addr,
                                                uint16_t           num)
{
    if ((addr < _AM_MB_COIL_START_ADDR) || (addr > _AM_MB_COIL_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_ADDRESS;
    }

    if (addr + num > _AM_MB_COIL_END_ADDR) {
        return AM_MB_EXP_ILLEGAL_DATA_VALUE;
    }

    /* 将buf缓冲区值写到指定线圈*/
    am_mb_bits_set_from_buf(_g_coils_buf, p_buf, addr - _AM_MB_COIL_START_ADDR, num);

    return AM_MB_EXP_NONE;
}

/******************************************************************************/
/** \brief 读离散量输入回调函数 */
am_local
am_mb_exception_t _mb_slv_read_discrete_inputs_callback (am_mb_slave_handle slave,
                                                         uint8_t           *p_buf,
                                                         uint16_t           addr,
                                                         uint16_t           num)
{
    return _mb_slv_read_coils_callback(slave, p_buf, addr, num);
}
/******************************************************************************/
/** \brief 读保持寄存器回调函数 */
am_local
am_mb_exception_t _mb_slv_read_hold_regs_callback (am_mb_slave_handle slave,
                                                   uint8_t           *p_buf,
                                                   uint16_t           addr,
                                                   uint16_t           num)
{
    uint16_t index;

    if ((addr < _AM_MB_HOLD_REGS_START_ADDR) || (addr > _AM_MB_HOLD_REGS_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_ADDRESS;
    }

    if ((addr + num > _AM_MB_HOLD_REGS_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_VALUE;
    }

    index = addr - _AM_MB_HOLD_REGS_START_ADDR;

    /* 读取保持寄存器数据到p_buf中*/
    am_mb_regcpy(p_buf, &_g_hold_regs[index], num);

    return AM_MB_EXP_NONE;
}

/******************************************************************************/
/** \brief 写寄存器回调函数 */
am_local
am_mb_exception_t _mb_slv_write_hold_regs_callback (am_mb_slave_handle slave,
                                                    uint8_t           *p_buf,
                                                    uint16_t           addr,
                                                    uint16_t           num)
{
    uint16_t index;

    if ((addr < _AM_MB_HOLD_REGS_START_ADDR) ||
        (addr > _AM_MB_HOLD_REGS_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_ADDRESS;
    }

    if ((addr + num > _AM_MB_HOLD_REGS_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_VALUE;
    }

    index = addr - _AM_MB_HOLD_REGS_START_ADDR;

    /* 将p_buf数据写到保持寄存器数据 */
    am_mb_regcpy(&_g_hold_regs[index], p_buf, num);

    return AM_MB_EXP_NONE;
}
/******************************************************************************/
/** \brief 读输入寄存器回调函数 */
am_local
am_mb_exception_t _mb_slv_read_input_regs_callback (am_mb_slave_handle slave,
                                                    uint8_t           *p_buf,
                                                    uint16_t           addr,
                                                    uint16_t           num)
{
    uint16_t index;

    if ((addr < _AM_MB_INPUT_REGS_START_ADDR) ||
        (addr > _AM_MB_INPUT_REGS_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_ADDRESS;
    }

    if ((addr + num > _AM_MB_INPUT_REGS_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_VALUE;
    }

    index = addr - _AM_MB_INPUT_REGS_START_ADDR;

    /* 读取输入寄存器数据到p_buf中*/
    am_mb_regcpy(p_buf, &_g_input_regs[index], num);

    return AM_MB_EXP_NONE;
}

/** \brief 扩展写寄存器掩码回调函数 */
am_local
am_mb_exception_t _mb_slv_reg_wr_mask_callback (am_mb_slave_handle slave,
                                                uint8_t           *p_pdu,
                                                uint16_t          *p_len)
{
    uint16_t addr, and, or;

    if (*p_len != 7) {
        return AM_MB_EXP_ILLEGAL_DATA_VALUE;
    }

    addr = _mb_to_u16(p_pdu + 1);
    and  = _mb_to_u16(p_pdu + 3);
    or   = _mb_to_u16(p_pdu + 5);

    if ((addr < _AM_MB_HOLD_REGS_START_ADDR) ||
        (addr > _AM_MB_HOLD_REGS_END_ADDR)) {
        return AM_MB_EXP_ILLEGAL_DATA_ADDRESS;
    }

    _g_hold_regs[addr - _AM_MB_HOLD_REGS_START_ADDR] &= and;
    _g_hold_regs[addr - _AM_MB_HOLD_REGS_START_ADDR] |= or;

    return AM_MB_EXP_NONE;

}

/******************************************************************************/
/* 通用功能码回调函数注册 */
am_local
am_mb_err_t app_mb_slave_comm_callback_register (am_mb_slave_handle slave)
{
    /* 注册读线圈回调函数 */
    am_mb_slave_register_callback(slave,
                                  AM_MB_FUNC_COILS_CALLBACK,
                                  AM_MB_FUNC_CALLBACK_READ,
                                  _mb_slv_read_coils_callback);

    /* 注册写线圈回调函数 */
    am_mb_slave_register_callback(slave,
                                  AM_MB_FUNC_COILS_CALLBACK,
                                  AM_MB_FUNC_CALLBACK_WRITE,
                                  _mb_slv_write_coils_callback);

    /* 注册读离散量输入回调函数 */
    am_mb_slave_register_callback(slave,
                                  AM_MB_FUNC_DISCRETEINPUTS_CALLBACK,
                                  AM_MB_FUNC_CALLBACK_READ,
                                  _mb_slv_read_discrete_inputs_callback);

    /* 注册读保持寄存器回调函数 */
    am_mb_slave_register_callback(slave,
                                  AM_MB_FUNC_HOLDREGISTERS_CALLBACK,
                                  AM_MB_FUNC_CALLBACK_READ,
                                  _mb_slv_read_hold_regs_callback);

    /* 注册写保持寄存器回调函数 */
    am_mb_slave_register_callback(slave,
                                  AM_MB_FUNC_HOLDREGISTERS_CALLBACK,
                                  AM_MB_FUNC_CALLBACK_WRITE,
                                  _mb_slv_write_hold_regs_callback);

    /* 注册读输入寄存器回调函数 */
    am_mb_slave_register_callback(slave,
                                  AM_MB_FUNC_INPUTREGISTERS_CALLBACK,
                                  AM_MB_FUNC_CALLBACK_READ,
                                  _mb_slv_read_input_regs_callback);

    /* 注册扩展写寄存器掩码回调函数 */
    am_mb_slave_register_handler(slave,
                                 _AM_MB_MASK_WR_REG_FUNCODE,
                                 _mb_slv_reg_wr_mask_callback);

    return AM_MB_ERR_NOERR;
}

/******************************************************************************/
/** \brief Modbus 从站测试示例 */
void demo_zsl42x_core_mb_slave_test_entry (void)
{
    uint8_t     i       = 0;
    uint32_t    cur_ms;
    uint32_t    pre_ms;
    am_mb_err_t err;

    am_mb_slave_dev_t   mb_slave_dev;    /* 从站设备 */
    am_mb_slave_handle  mb_slave_handle; /* 从站句柄 */

    am_funcode_handle_t funcode_buf[4];  /* 分配用户注册功能码内存 */
    am_mb_slave_info_t  mb_slave_info;   /* 从站设备信息 */


    /* 从站参数初始化 */
    mb_slave_info.mode                  = AM_MB_ASCII;    /* RTU模式 */
    mb_slave_info.slave_addr            = 0x08;           /* 从站地址-0x08  */
    /* 串口设备信息初始化 */
    mb_slave_info.serial_info.uart      = am_zsl42x_uart1_inst_init();
    mb_slave_info.serial_info.baudrate  = 115200;         /* 从站波特率-115200 */
    mb_slave_info.serial_info.parity    = AM_MB_PAR_NONE; /* 检验-偶校验            */
    /* 硬件定时器设备信息初始化 */
    mb_slave_info.timer_info.timer      = am_zsl42x_tim0_timing_inst_init ();
    mb_slave_info.timer_info.chan       = 0;

    /* 初始化从站设备并获取从站句柄 */
    mb_slave_handle = am_mb_slave_init(&mb_slave_dev, &mb_slave_info, &err);
    if (err != AM_MB_ERR_NOERR) {
        return;
    }

    /* 分配用户注册功能码内存 */
    am_mbs_user_funcode_alloc(mb_slave_handle, funcode_buf,
                              AM_NELEMENTS(funcode_buf));

    /* 注册通用功能码回调函数 */
    app_mb_slave_comm_callback_register(mb_slave_handle);

    am_mb_slave_start(mb_slave_handle);

    pre_ms = am_ticks_to_ms(am_sys_tick_get());
    AM_FOREVER {
        am_mb_slave_poll(mb_slave_handle);

        cur_ms = am_ticks_to_ms(am_sys_tick_get());
        if (cur_ms - pre_ms > 1000) {
            pre_ms = cur_ms;
            i++;
            if (i == 20) {
                i = 0;
                break;
            }
        }
    }

    /* 修改从站地址 */
    am_mb_slave_stop(mb_slave_handle);
    am_mb_slave_set_addr(mb_slave_handle, 0x01);
    am_mb_slave_start(mb_slave_handle);

    pre_ms = am_ticks_to_ms(am_sys_tick_get());
    AM_FOREVER {
        am_mb_slave_poll(mb_slave_handle);

        cur_ms = am_ticks_to_ms(am_sys_tick_get());
        if (cur_ms - pre_ms > 1000) {
            pre_ms = cur_ms;
            i++;
            if (i == 10) {
                i = 0;
                break;
            }
        }
    }

    /* 修改为硬件定时器 */
    am_mb_slave_stop(mb_slave_handle);
    am_mb_slave_ioctl(mb_slave_handle, AM_MBS_TYPE_USED_HW_TMR, (void *)AM_TRUE);
    am_mb_slave_start(mb_slave_handle);

    AM_FOREVER {
        am_mb_slave_poll(mb_slave_handle);

        cur_ms = am_ticks_to_ms(am_sys_tick_get());
        if (cur_ms - pre_ms > 1000) {
            pre_ms = cur_ms;
            i++;
            if (i == 20) {
                i = 0;
                break;
            }
        }
    }

    /* 修改软件定时器 */
    am_mb_slave_stop(mb_slave_handle);
    am_mb_slave_ioctl(mb_slave_handle, AM_MBS_TYPE_USED_HW_TMR, (void *)AM_FALSE);
    am_mb_slave_start(mb_slave_handle);

    pre_ms = am_ticks_to_ms(am_sys_tick_get());
    AM_FOREVER {
        am_mb_slave_poll(mb_slave_handle);
    }
}

/** [src_mb_slave_modbus_demo] */

/* end of file */

