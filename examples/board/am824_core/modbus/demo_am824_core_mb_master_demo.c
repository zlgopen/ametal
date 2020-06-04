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
 * \brief Modbus 主站RTU测试demo
 *
 * - 使用注意：
 *
 *   1. Modbus 主机例程默认使用软件定时器，使用软件定时器作为Modbus的定时时，
 *      请开启软件定时器的配置宏 am_prj_config.h 文件的 AM_CFG_SOFTIMER_ENABLE 宏
 *
 *   2. 使用硬件定时器时，配置硬件定时器的用户配置文件选择32位定时器长度模式
 *
 * \par 源代码
 * \snippet am_mb_master_modbus_demo.c src_mb_master_modbus_demo
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-07  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup grp_am_if_mb_mst
 * \copydoc am_mb_master_demo.c
 */

/** [src_mb_master_modbus_demo] */

#include "am_mb_master.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_lpc82x_inst_init.h"

#include "string.h"

/******************************************************************************/
/* 读参数构造函数 */
am_static_inline
void _mb_rd_param_construct (am_mb_master_rw_param_t *p_rw_param,
                             uint16_t                 rd_addr,
                             uint16_t                 rd_num,
                             uint16_t                *p_rd_buf)
{
    p_rw_param->rd_addr  = rd_addr;
    p_rw_param->rd_num   = rd_num;
    p_rw_param->p_rd_buf = p_rd_buf;
}

/* 写参数构造函数 */
am_static_inline
void _mb_wr_param_construct (am_mb_master_rw_param_t *p_rw_param,
                                uint16_t                 wr_addr,
                                uint16_t                 wr_num,
                                uint16_t                *p_wr_buf)
{
    p_rw_param->wr_addr  = wr_addr;
    p_rw_param->wr_num   = wr_num;
    p_rw_param->p_wr_buf = p_wr_buf;
}

/* Common Function Code Test */
am_local
am_mb_err_t app_mb_mst_comm_funcode_test (am_mb_master_handle mb_mst_handle,
                                          uint8_t             slave_addr)
{
    uint16_t                     rd_buf[10] = {0};
    uint16_t                     wr_buf[10] = {0};
    am_mb_err_t                  err;
    am_mb_master_rw_param_t      rw_param;

    memset(&rw_param, 0, sizeof(am_mb_master_rw_param_t));

    /* Construct the rw parameter */
    _mb_rd_param_construct(&rw_param, 1000, 10, rd_buf);

    /* 1: Make a 'Read Coils' request */
    err = am_mb_master_request(mb_mst_handle,         slave_addr,
                               AM_MB_FUNC_READ_COILS, &rw_param);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The coils value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    } else {
        AM_DBG_INFO("Read Coils Error\n");
    }
    memset((void *)rd_buf, 0, 10 * sizeof(uint16_t));

    /* 2: Make a 'Read Discrete Inputs' request */
    err = am_mb_master_request(mb_mst_handle,                   slave_addr,
                               AM_MB_FUNC_READ_DISCRETE_INPUTS, &rw_param);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The Discrete Inputs value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    } else {
        AM_DBG_INFO("Read Discrete Inputs Error\n");
    }
    memset((void *)rd_buf, 0, 10 * sizeof(uint16_t));

    /* 3: Make a 'Read Holding Register' request */
    err = am_mb_master_request(mb_mst_handle,                    slave_addr,
                               AM_MB_FUNC_READ_HOLDING_REGISTER, &rw_param);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The Holding Register value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    } else {
        AM_DBG_INFO("Read Holding Register Error\n");
    }
    memset((void *)rd_buf, 0, 10 * sizeof(uint16_t));

    /* 4: Make a 'Read Input Register' request */
    err = am_mb_master_request(mb_mst_handle,                  slave_addr,
                               AM_MB_FUNC_READ_INPUT_REGISTER, &rw_param);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The Input Register value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    } else {
        AM_DBG_INFO("Read Input Register Error\n");
    }

    /* 5: Make a 'Write Single Coil' request */
    wr_buf[0] = 0x0000; /* Write Coils On */
    _mb_wr_param_construct(&rw_param, 1000, 1, wr_buf);
    err = am_mb_master_request(mb_mst_handle,                slave_addr,
                               AM_MB_FUNC_WRITE_SINGLE_COIL, &rw_param);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Write Single Coil Error!!\n");
    }

    /* 6: Make a 'Write Register' request */
    wr_buf[0] = 1234;
    _mb_wr_param_construct(&rw_param, 1000, 1, wr_buf);
    err = am_mb_master_request(mb_mst_handle,             slave_addr,
                               AM_MB_FUNC_WRITE_REGISTER, &rw_param);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Write Register Error!!\n");
    }

    /* 15: Make a 'Write Multiple Coil' request */
    wr_buf[0] = 0x0355;
    _mb_wr_param_construct(&rw_param, 1002, 10, wr_buf);
    err = am_mb_master_request(mb_mst_handle,                   slave_addr,
                               AM_MB_FUNC_WRITE_MULTIPLE_COILS, &rw_param);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Multiple Coil Write Error!!\n");
    }

    /* 16: Make a 'Write Multiple Registers' request */
    wr_buf[0] = 0x01;
    wr_buf[1] = 0x01;
    wr_buf[2] = 0x02;
    wr_buf[3] = 0x02;
    wr_buf[4] = 0x03;
    wr_buf[5] = 0x03;
    wr_buf[6] = 0x04;
    wr_buf[7] = 0x07;
    wr_buf[8] = 0x08;
    wr_buf[9] = 0x09;
    _mb_wr_param_construct(&rw_param, 1000, 10, wr_buf);
    err = am_mb_master_request(mb_mst_handle,                       slave_addr,
                               AM_MB_FUNC_WRITE_MULTIPLE_REGISTERS, &rw_param);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Multiple Registers Write Error!!\n");
    }

    /* 16: Make a 'RW Multiple Registers' request */
    memset(rd_buf, 0, 10 * sizeof(uint16_t));

    wr_buf[0] = 0x09;
    wr_buf[1] = 0x08;
    wr_buf[2] = 0x07;
    wr_buf[3] = 0x06;
    wr_buf[4] = 0x05;
    wr_buf[5] = 0x04;
    wr_buf[6] = 0x03;
    wr_buf[7] = 0x02;
    wr_buf[8] = 0x01;
    wr_buf[9] = 0x00;
    _mb_wr_param_construct(&rw_param, 1011, 9,  wr_buf);
    _mb_rd_param_construct(&rw_param, 1000, 10, rd_buf);
    err = am_mb_master_request(mb_mst_handle,
                               slave_addr,
                               AM_MB_FUNC_READWRITE_MULTIPLE_REGISTERS,
                               &rw_param);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Multiple Registers Write Error!!\n");
    } else {
        AM_DBG_INFO("RW Multiple Registers value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    }

    /* 16: Make a 'RW Multiple Registers' request in other slave device */
    memset(rd_buf, 0, 10 * sizeof(uint16_t));

    wr_buf[0] = 0x09;
    wr_buf[1] = 0x08;
    wr_buf[2] = 0x07;
    wr_buf[3] = 0x06;
    wr_buf[4] = 0x05;
    wr_buf[5] = 0x04;
    wr_buf[6] = 0x03;
    wr_buf[7] = 0x02;
    wr_buf[8] = 0x01;
    wr_buf[9] = 0x00;
    _mb_wr_param_construct(&rw_param, 1010, 10, wr_buf);
    _mb_rd_param_construct(&rw_param, 1000, 10, rd_buf);
    err = am_mb_master_request(mb_mst_handle,
                               slave_addr,
                               AM_MB_FUNC_READWRITE_MULTIPLE_REGISTERS,
                               &rw_param);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Multiple Registers Write Error!!\n");
    } else {
        AM_DBG_INFO("Another slave RW Multiple Registers value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    }

    /* Broadcast test
     * Make a 'Write Register' request
     */
    wr_buf[0] = 4321;
    _mb_wr_param_construct(&rw_param, 1002, 1, wr_buf);
    err = am_mb_master_request(mb_mst_handle,
                               0,
                               AM_MB_FUNC_WRITE_REGISTER,
                               &rw_param);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Broadcast Write Register Error!!\n");
    }

    return err;
}

/******************************************************************************/
#define AM_MB_FUNC_MASK_WRITE  (0x16u)   /**< \brief Mask Write Function code */

typedef struct _mb_mst_mask_wr {
    uint16_t addr;     /**< \brief Address of register */
    uint16_t and_mask; /**< \brief and mask value */
    uint16_t or_mask;  /**< \brief or mask value */
} mb_mst_mask_wr_t;

/*
 * User definition function
 */
am_local am_mb_err_t _mb_mst_mask_wr_snd (void    *p_params,
                                          uint8_t *p_pdudata,
                                          uint8_t *p_pdudata_len)
{
    mb_mst_mask_wr_t *p_mask_wr = (mb_mst_mask_wr_t *)p_params;

    p_pdudata[0] = (p_mask_wr->addr >> 8) & 0xFF;
    p_pdudata[1] = p_mask_wr->addr & 0xFF;

    p_pdudata[2] = (p_mask_wr->and_mask >> 8) & 0xFF;
    p_pdudata[3] = p_mask_wr->and_mask & 0xFF;

    p_pdudata[4] = (p_mask_wr->or_mask >> 8) & 0xFF;
    p_pdudata[5] = p_mask_wr->or_mask & 0xFF;

    *p_pdudata_len = 6;

    return AM_MB_ERR_NOERR;
}

am_local
am_mb_err_t app_mb_mst_user_def_request (am_mb_master_handle mb_mst_handle,
                                         uint8_t             slave_addr)
{
    uint16_t                rd_buf[10] = {0};
    am_mb_err_t             err;
    am_mb_master_rw_param_t rw_param;

    _mb_rd_param_construct(&rw_param, 1000, 10, rd_buf);

    /* 3: Make a 'Read Holding Register' request */
    err = am_mb_master_request(mb_mst_handle,                    slave_addr,
                               AM_MB_FUNC_READ_HOLDING_REGISTER, &rw_param);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The Holding Register value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    }

    mb_mst_mask_wr_t mask_wr;
    mask_wr.addr     = 1000;
    mask_wr.and_mask = 0x00;
    mask_wr.or_mask  = 0xFF;

    /* Make 'Mask Write Register' Request */
    err = am_mb_master_funcode_register(mb_mst_handle,       AM_MB_FUNC_MASK_WRITE,
                                        _mb_mst_mask_wr_snd, NULL);
    err = am_mb_master_request(mb_mst_handle,         slave_addr,
                               AM_MB_FUNC_MASK_WRITE, &mask_wr);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Mask Write Register Error! Error Code = %d\n", err);
    }

    memset((void *)rd_buf, 0, 10 * sizeof(uint16_t));
    /* 3: resume make a 'Read Holding Register' request,
     *    the register 0 value was changed to 0xFF.
     */
    err = am_mb_master_request(mb_mst_handle,                    slave_addr,
                               AM_MB_FUNC_READ_HOLDING_REGISTER, &rw_param);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The Holding Register value are as follow:\n");
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("rd_buf[%d] = 0x%x\n", i, rd_buf[i]);
        }
    }

    return err;
}
/******************************************************************************/
am_local
am_mb_err_t _mb_mst_com_funcode_interfaces_test(am_mb_master_handle mb_mst_handle,
                                                uint8_t             slv_addr)
{
    uint8_t     coils          = 0;
    uint8_t     wr_coils[2]    = {0xF0, 0x03};
    uint16_t    hold_reg[10]   = {0};
    uint16_t    intput_reg[10] = {0};

    am_mb_master_rw_param_t rw_param;
    am_mb_err_t err            = AM_MB_ERR_NOERR;


    /* 读线圈 */
    err = am_mb_coils_read(mb_mst_handle, slv_addr, 1002, 7, &coils);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The coils value are = 0x%x\n", coils);
    } else {
        AM_DBG_INFO("read coild error, error code = %d\n", err);
    }

    /* 读离散量 */
    err = am_mb_discrete_inputs_read(mb_mst_handle, slv_addr, 1004, 5, &coils);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("The discrete input value are = 0x%x\n", coils);
    } else {
        AM_DBG_INFO("Read discrete input error, error code = %d\n", err);
    }

    /* 读保持寄存器 */
    err = am_mb_holding_reg_read(mb_mst_handle, slv_addr, 1000, 5, &hold_reg[0]);
    if (AM_MB_ERR_NOERR == err) {
        for (uint8_t i = 0; i < 5; i++) {
            AM_DBG_INFO("The holding reg %d = 0x%x\n", i, hold_reg[i]);
        }
    } else {
        AM_DBG_INFO("Read discrete input error, error code = %d\n", err);
    }

    /* 读输入寄存器 */
    err = am_mb_input_reg_read(mb_mst_handle, slv_addr, 1000, 10, &intput_reg[0]);
    if (AM_MB_ERR_NOERR == err) {
        for (uint8_t i = 0; i < 10; i++) {
            AM_DBG_INFO("The input reg %d = 0x%x\n", i, intput_reg[i]);
        }
    } else {
        AM_DBG_INFO("Read discrete input error, error code = %d\n", err);
    }

    /* 写单个线圈 */
    err = am_mb_single_coil_write(mb_mst_handle, slv_addr, 1000, 0x0000);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("Write single coil success\n");
    } else {
        AM_DBG_INFO("Write single coil error, error code = %d\n", err);
    }

    /* 写单个寄存器 */
    err = am_mb_single_reg_write(mb_mst_handle, slv_addr, 1009, 12345);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("Write single reg success\n");
    } else {
        AM_DBG_INFO("Write single reg error, error code = %d\n", err);
    }

    /* 写多个线圈 */
    err = am_mb_multiple_coils_write(mb_mst_handle, slv_addr, 1002,
                                     10,            &wr_coils[0]);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("Write multiple coils success\n");
    } else {
        AM_DBG_INFO("Write multiple coils error, error code = %d\n", err);
    }

    /* 写多个寄存器 */
    err = am_mb_multiple_regs_write(mb_mst_handle, slv_addr, 1000,
                                    10,            &intput_reg[0]);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("Write multiple regs success\n");
    } else {
        AM_DBG_INFO("Write multiple regs error, error code = %d\n", err);
    }

    /* 写多个寄存器 */
    _mb_wr_param_construct(&rw_param, 1000, 5, &hold_reg[0]);
    _mb_rd_param_construct(&rw_param, 1005, 5, &hold_reg[5]);
    err = am_mb_multiple_regs_read_write(mb_mst_handle, slv_addr, &rw_param);
    if (AM_MB_ERR_NOERR == err) {
        AM_DBG_INFO("Write Read multiple regs success\n");
        for(uint8_t i = 0; i < 5; i++) {
            AM_DBG_INFO("Read reg %d = 0x%x\n", i + 5, hold_reg[5 + i]);
        }
    } else {
        AM_DBG_INFO("Write Read multiple regs error, error code = %d\n", err);
    }

    return err;
}

/******************************************************************************/
void am_mb_master_demo_entry (void)
{
    uint8_t             slave_addr  = 0x01;
    uint32_t            ack_timeout = 3000;
    am_mb_err_t         err;
    am_mb_master_dev_t  mb_master_dev;      /* 主站设备 */
    am_mb_master_handle mb_master_handle;   /* 主站句柄 */
    am_mb_master_info_t mb_master_info;     /* 主站设备信息 */

    am_mb_m_funcode_handler_t funcode_buf[4]; /* 分配用户注册功能码内存 */


    mb_master_info.mode      = AM_MB_ASCII;      /* 主站RTU模式 */
    /* 串口设备信息初始化 */
    mb_master_info.serial.uart      = am_lpc82x_usart1_inst_init();
    mb_master_info.serial.baudrate  = 9600;         /* 波特率9600 */
    mb_master_info.serial.parity    = AM_MB_PAR_EVEN; /* 偶校验 */
    mb_master_info.serial.rs485_en  = AM_FALSE;       /* 不使能485功能 */
    /* 定时器设备信息初始化 */
    mb_master_info.timer.timer      = am_lpc82x_sct0_timing_inst_init();
    mb_master_info.timer.chan       = 0;

    /* Create a master station device */
    mb_master_handle = am_mb_master_create(&mb_master_dev);

    /* configure master station device */
    err = am_mb_master_config(mb_master_handle, &mb_master_info);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Modbus Master Device configure Error, Error code = %d\n", err);
        return;
    }

    /* 使用硬件定时器 */
//    am_mb_master_ioctl(mb_master_handle, AM_MBM_TYPE_USE_HW_TIMER, (void *)AM_TRUE);

    /* 分配用户自定义功能码内存 */
    am_mb_master_user_funcode_alloc(mb_master_handle, funcode_buf,
                                    AM_NELEMENTS(funcode_buf));

    /* Setting The Master ack timeout value - 3000ms */
    am_mb_master_ioctl(mb_master_handle,
                       AM_MBM_TYPE_ACK_TIMEOUT_SET,
                       (void *)ack_timeout);

    /* Start Master Station Device */
    am_mb_master_start(mb_master_handle);

    /* Common function code test */
    AM_DBG_INFO("\nCommon Function Code Test Begin:\n");
    err = app_mb_mst_comm_funcode_test(mb_master_handle, slave_addr);
    if (AM_MB_ERR_NOERR != err) {
       AM_DBG_INFO("Common Function Code Test Error! Error Code = %d\n", err);
    }

    /* 使用软件定时器 */
//    am_mb_master_ioctl(mb_master_handle, AM_MBM_TYPE_USE_HW_TIMER, (void *)AM_FALSE);

    /* User-defined function code */
    AM_DBG_INFO("\nUser-defined function code Test Begin:\n");
    app_mb_mst_user_def_request(mb_master_handle, slave_addr);

    /* Common Function's Code Interface test */
    _mb_mst_com_funcode_interfaces_test(mb_master_handle, slave_addr + 1);

    /* Stop the master */
    AM_DBG_INFO("\nStop Modbus Master Station\n");
    am_mb_master_stop(mb_master_handle);

    /* Delete Master Station */
    err = am_mb_master_delete(mb_master_handle);
    if (AM_MB_ERR_NOERR != err) {
        AM_DBG_INFO("Modbus Master Station Delete Error! Error Code = %d\n", err);
    }




    while(1);
}

/** [src_mb_master_modbus] */

/* end of file */


