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
 * \brief SC16IS740软件包实现
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-13  wk, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_spi.h"
#include "am_sc16is740.h"


/*****************************************************************************
 * 宏定义
 ****************************************************************************/

/*
 * \brief 寄存器地址
 */
#define __SC16IS740_REG_RHRTHR               0X00    /* 接收、发送保持寄存器 */
#define __SC16IS740_REG_IER                  0X01    /* 中断使能寄存器 */
#define __SC16IS740_REG_FCRIIR               0X02    /* FIFO控制、中断识别寄存器 */
#define __SC16IS740_REG_LCR                  0X03    /* 线路控制寄存器 */
#define __SC16IS740_REG_MCR                  0X04    /* 调制解调器控制寄存器 */
#define __SC16IS740_REG_LSR                  0X05    /* 线路状态寄存器 */
#define __SC16IS740_REG_MSR                  0X06    /* 调制解调器状态寄存器 */
#define __SC16IS740_REG_SPR                  0X07    /*  */

#define __SC16IS740_REG_TXLVL                0X08    /* 发送FIFO级别寄存器 */
#define __SC16IS740_REG_RXLVL                0X09    /* 接收FIFO级别寄存器 */
#define __SC16IS740_REG_IODir                0X0A    /* I/O引脚方向寄存器 */
#define __SC16IS740_REG_IOState              0X0B    /* I/O引脚状态寄存器 */
#define __SC16IS740_REG_IOIntEna             0X0C    /* I/O中断使能寄存器 */

#define __SC16IS740_REG_IOControl            0X0E    /* I/O引脚控制寄存器 */
#define __SC16IS740_REG_EFCR                 0X0F    /* 额外功能寄存器 */


#define __SC16IS740_REG_DLL                  0X00    /* 除数锁存器LSB */
#define __SC16IS740_REG_DLH                  0X01    /* 除数锁存器MSB */
#define __SC16IS740_REG_EFR                  0X02    /* 增强功能寄存器 */

#define __SC16IS740_REG_XON1                 0X04
#define __SC16IS740_REG_XON2                 0X05
#define __SC16IS740_REG_XOFF1                0X06
#define __SC16IS740_REG_XOFF2                0X07

#define __SC16IS740_REG_TCR                  0X06    /* 传输控制寄存器 */
#define __SC16IS740_REG_TLR                  0X07    /* 触发电平寄存器 */