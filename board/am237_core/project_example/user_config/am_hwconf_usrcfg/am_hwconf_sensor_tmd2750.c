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
 * \brief 传感器 TMD2750 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_tmd2750.h"
#include "am_common.h"
#include "zlg237_pin.h"
#include "am_zlg237_inst_init.h"

am_const am_local tmd2750_param_config_t __g_tmd2750_param_default = {
        AM_TMD2750_ATIME_50MS,                      /* ALS ADC的积分时间 */
        AM_TMD2750_WTIME_2_78MS_OR_33_4MS,          /* (与wlong_en配置相关)如果wlong_en为真，等待时间为后者，否则为前者 */
        0,                                          /* 连续故障次数进入Prox中断，该操作直接配置寄存器值，详细内容查阅数据手册Page25 */
        0,                                          /* 连续故障次数进入ALS中断，该操作直接配置寄存器值，详细内容查阅数据手册Page25 */
        AM_FALSE,                                   /* WLONG使能设置，使能后WTIME值将扩大12倍 */
        AM_FALSE,                                   /* Prox脉冲长度扩大16倍设置 */
        AM_TMD2750_PULSE_LEN_16US_OR_256US,         /* (与ppulse_len_16x_en配置相关)Prox脉冲长度，当 ppulse_len_16x_en为真，则为后者，否则为前者*/
        16,                                         /* (与apc_disable配置相关)一个Prox周期的最大脉冲数 */
        AM_TMD2750_PGAIN_4,                         /* Prox IR传感器增益 */
        AM_TMD2750_PLDRIVE_19MA,                    /* Prox IR VCSEL电流 */
        AM_TMD2750_AGAIN_256,                       /* ALS传感器增益 */
        AM_FALSE,                                   /* sleep_after_interrupt */
        AM_TMD2750_APC_DISABLE,                     /* Prox自动脉冲控制设置 */
        AM_TMD2750_ELECTRICAL_OPTICAL_CROSSTALK,    /* Prox校准类型选择 */
        AM_TMD2750_BINSRCH_TARGET_15,               /* Prox偏移校准目标 */
        AM_FALSE,                                   /* Prox自动偏移调节设置 */
        AM_TMD2750_SAMPLE_SIZE_DISABLE,             /* Prox平均数量 */
        2816,                                       /* Prox采样持续时间(us) */
};

/** \brief 传感器 TMD2750 设备信息实例 */
am_const am_local struct am_sensor_tmd2750_devinfo __g_tmd2750_info = {
        &__g_tmd2750_param_default,             /**< \brief 参数初始化 */
        PIOC_1,                                 /**< \brief 报警输出引脚 */
        0x49,                                   /**< \brief I2C 7位 设备地址 */
};

/** \breif 传感器 TMD2750 设备结构体定义 */
am_local struct am_sensor_tmd2750_dev __g_tmd2750_dev;

/** \brief 传感器 TMD2750 设备实例化 */
am_sensor_handle_t am_sensor_tmd2750_inst_init (void)
{
    return am_sensor_tmd2750_init(&__g_tmd2750_dev,
                                  &__g_tmd2750_info,
                                  am_zlg237_i2c1_inst_init());
}

/** \brief 传感器 TMD2750 实例解初始化 */
am_err_t am_sensor_tmd2750_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_tmd2750_deinit(handle);
}

/* end of file */
