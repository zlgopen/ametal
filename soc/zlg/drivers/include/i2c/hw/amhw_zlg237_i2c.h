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
 * \brief I2C操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-24  zc, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG237_I2C_H
#define __AMHW_ZLG237_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "amhw_zlg237_rcc.h"
/*
 * \brief 匿名结构体段的开始
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_zlg237_if_i2c
 * \copydoc amhw_zlg237_i2c.h
 * @{
 */

/**
 * \brief I2C寄存器块结构体
 */
typedef struct amhw_zlg237_i2c {

    __IO uint16_t  i2c_cr1;                /**< \brief I2C控制寄存器 1 */
         uint16_t  reserve0;               /**< \brief reserve 保留位 */
    __IO uint16_t  i2c_cr2;                /**< \brief I2C控制寄存器 2 */
         uint16_t  reserve1;
    __IO uint16_t  i2c_oar1;               /**< \brief I2C自身地址寄存器 1 */
         uint16_t  reserve2;
    __IO uint16_t  i2c_oar2;               /**< \brief I2C自身地址寄存器 2 */
         uint16_t  reserve3;
    __IO uint16_t  i2c_dr;                 /**< \brief I2C数据寄存器  */
         uint16_t  reserve4;
    __IO uint16_t  i2c_sr1;                /**< \brief I2C状态寄存器 1 */
         uint16_t  reserve5;
    __IO uint16_t  i2c_sr2;                /**< \brief I2C状态寄存器 2 */
         uint16_t  reserve6;
    __IO uint16_t  i2c_ccr;                /**< \brief I2C时钟控制寄存器 */
         uint16_t  reserve7;
    __IO uint16_t  i2c_trise;              /**< \brief I2C TRISE 寄存器  */
         uint16_t  reserve8;

} amhw_zlg237_i2c_t;

/** \brief IIC 模式  */
typedef enum {

    /** \brief IIC 模式 */
    AMHW_ZLG237_I2C_MODE_I2C         = (uint16_t)0x0000,
    /** \brief SMBUS 设备 */
    AMHW_ZLG237_I2C_MODE_SMBUSDEVICE = (uint16_t)0x0002,
    /** \brief SMBUS 主机 */
    AMHW_ZLG237_I2C_MODE_SMBUSHOST   = (uint16_t)0x000A,

}amhw_zlg237_i2c_mode_t;

/** \brief I2C_acknowledegment */
typedef enum {
    AMHW_ZLG237_I2C_ACK_ENABLE  = (uint16_t)0x0400,    /** \brief ACK 使能 */
    AMHW_ZLG237_I2C_ACK_DISABLE = (uint16_t)0x0000,    /** \brief ACK 禁能 */
}amhw_zlg237_i2c_ack_t;

/** \brief I2C_acknowledged_address */
typedef enum {

    /** \brief 7位地址模式*/
    AMHW_ZLG237_I2C_ACK_ADDRESS_7_BIT  = (uint16_t)0x4000,

    /** \brief 10 位地址模式*/
    AMHW_ZLG237_I2C_ACK_ADDRESS_10_BIT = (uint16_t)0xC000,
}amhw_zlg237_i2c_ack_address_t;

/** \brief I2C_fast_mode_duty */
typedef enum {

    /** \brief fast mode Tlow/Thigh = 16/9 */
    AMHW_ZLG237_I2C_DUTY_CYCLE_16_9 = (uint16_t)0x4000,

    /** \brief fast mode Tlow/Thigh = 2 */
    AMHW_ZLG237_I2C_DUTY_CYCLE_2    = (uint16_t)0xBFFF,
}amhw_zlg237_i2c_duty_t;

/** \brief I2C模块使能 */
#define AMHW_ZLG237_I2C_CR1_PE                  (0)

/** \brief SMBUS模式选择 */
#define AMHW_ZLG237_I2C_CR1_SMBUS               (1ul)
typedef enum {
    AMHW_ZLG237_I2C_MODE = 0,
    AMHW_ZLG237_SMBUS_MODE,
} amhw_zlg237_i2c_cr1_smbus_t;

/** \brief SMBus类型 */
#define AMHW_ZLG237_I2C_CR1_SMBTYPE             (3ul)
typedef enum {
    AMHW_ZLG237_SMBUS_SLV = 0,         /** <brief SMBus 设备 */
    AMHW_ZLG237_SMBUS_MASTER,          /** <brief SMBus 主机 */
} amhw_zlg237_i2c_cr1_smbtype_t;

/** \brief arp使能
 *  \note : 如果SMBTYPE = 0 ,使能SMBus设备的默认地址
 *          如果SMBTYPE = 1 ,使能SMBus设备的主地址
 */
#define AMHW_ZLG237_I2C_CR1_ENARP               (4ul)

/** \brief pec使能 */
#define AMHW_ZLG237_I2C_CR1_ENPEC               (5ul)

/** \brief engc 广播呼叫使能 */
#define AMHW_ZLG237_I2C_CR1_ENGC                (6ul)

/** \brief NOSTRETCH 禁止时钟延长（从模式）
 *  \note: 该位用于当ADDR或BTF标志被置位，在从模式下禁止时钟延长，直到它被软件复位
 */
#define AMHW_ZLG237_I2C_CR1_NOSTRETCH           (7ul)

/** \brief start 起始条件产生
 *  \note: 软件可设置，或当起始条件发出后或PE = 0 时，由硬件清除
 */
#define AMHW_ZLG237_I2C_CR1_START               (8ul)

/** \brief stop 停止条件产生
 *  \note: 软件可设置，或当检测到停止条件，由硬件清除，当检测到超时错误时，硬件置位
 */
#define AMHW_ZLG237_I2C_CR1_STOP                (9ul)

/** \brief ack 应答使能
 *  \note: 软件可设置该位，或当PE = 0 时，由硬件清除
 */
#define AMHW_ZLG237_I2C_CR1_ACK                 (10ul)

/** \brief pos 应答/PEC位置（用于数据接受）
 *  \note: 软件可设置该位，或当PE = 0 时，由硬件清除
 */
#define AMHW_ZLG237_I2C_CR1_POS                 (11ul)

/** \brief pec 数据包出错检测
 *  \note: 软件可设置该位；当传送PEC后，或起始或停止条件时，或当PE=0 时硬件将其清除
 *         仲裁丢失时，PEC计算失效
 */
#define AMHW_ZLG237_I2C_CR1_PEC                 (12ul)

/** \brief alert SMBus 提醒
 *  \note: 软件可设置该位；当 PE = 0 时，由硬件清除
 */
#define AMHW_ZLG237_I2C_CR1_ALERT               (13ul)

/** \brief swrst 软件复位
 *  \note: 当被置位时，I2C处于复位状态.在复位该位前确信I2C引脚被释放，总线是空的
 *         该位可以用于BUSY位为‘1’，在总线上有没有检测到停止条件时
 */
#define AMHW_ZLG237_I2C_CR1_SWRST               (15ul)

/** \brief freq[5:0] 模块时钟频率 */
#define AMHW_ZLG237_I2C_CR2_FREQ                (0ul)

/** \brief iterren 出错中断使能 */
#define AMHW_ZLG237_I2C_CR2_ITERREN             (8ul)

/** \brief itevten 事件中断使能 */
#define AMHW_ZLG237_I2C_CR2_ITEVTEN             (9ul)

/** \brief itbufen 缓冲器中断使能 */
#define AMHW_ZLG237_I2C_CR2_ITBUFEN             (10ul)

typedef enum {
    I2C_CR2_ITERREN = 0x01,    /* \brief 出错中断*/
    I2C_CR2_ITEVTEN = 0x02,    /* \brief 事件中断*/
    I2C_CR2_ITBUFEN = 0x04,    /* \brief 缓冲器中断*/
} amhw_zlg237_i2c_iten_t;

/** \brief dmaen DMA请求使能 */
#define AMHW_ZLG237_I2C_CR2_DMAEN               (11ul)

typedef enum {
    /** <brief 禁止DMA请求 */
    AMHW_ZLG237_I2C_DMAEN_DISABLE = 0,

    /** <brief 当TxE = 1或RxNE = 1时，允许DMA请求 */
    AMHW_ZLG237_I2C_DMAEN_ENABLE,
} amhw_zlg237_i2c_cr2_dmaen_t;

/** \brief last DMA最后一次传输
 *  \note: 主接收模式使用，使得在最后一次接收数据时可以产生一个NACK
 */
#define AMHW_ZLG237_I2C_CR2_LAST                (12ul)

/** \brief add0 接口地址
 *  \note：  7  位地址模式不用关心
 *         10 位地址模式为地址第0位
 */
#define AMHW_ZLG237_I2C_OAR1_ADD0               (0ul)

/** \brief add[7:1] 接口地址 地址的7~1位 */
#define AMHW_ZLG237_I2C_OAR1_ADD7_1             (1ul)

/** \brief add[9:8]
 *  \note：  7  位地址模式不用关心
 *         10 位地址模式为地址的9~8位
 * */
#define AMHW_ZLG237_I2C_OAR1_ADD9_8             (8ul)

/** \brief reserve[13:10] 始终由软件位保持为 1 */
#define AMHW_ZLG237_I2C_OAR1_RESERVE            (10ul)


/** \brief addmode 寻址模式（从模式）
 *  \note： 0： 7位从地址  不响应10位地址
 *         1：10位从地址 不响应7位地址
 */
#define AMHW_ZLG237_I2C_OAR1_ADDMODE            (15ul)

/** \brief endual 双地址模式使能位 */
#define AMHW_ZLG237_I2C_OAR2_ENDUAL             (0ul)

/** \brief add2[7:1] 接口地址 双地址模式下地址的 7~1 位 */
#define AMHW_ZLG237_I2C_OAR2_ADD2               (1ul)

/** \brief sr PEC[7:0] 存放内部PEC的值*/
#define AMHW_ZLG237_I2C_SR2_PEC7_1              (8ul)

/** \brief duty 快速模式下的占空比 */
#define AMHW_ZLG237_I2C_CCR_DUTY                (14ul)

/******************************************************************************
                             IIC MASTER Events
 *****************************************************************************/
/** \brief EV5  BUSY MSL SB*/
#define I2C_EVENT_MASTER_MODE_SELECT                 ((uint32_t)0x00030001)

/** \brief EV6 BUSY,MSL.ADDR,TXE,TRA */
#define IC2_EVENT_MASTER_TRANSMITTER_MODE_SELECTED   ((uint32_t)0x00070082)
/** \brief EV6 BUSTY,MSL,ADDR */
#define I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED      ((uint32_t)0x00030002)


/** \brief EV9 BUSY,MSL,ADD10 */
#define IC2_EVENT_MASTER_MODE_ADDRESS10              ((uint32_t)0x00030008)

/** \brief EV7 BUSY,MSL,RXNE */
#define I2C_EVENT_MASTER_BYTE_RECEIVED               ((uint32_t)0x00030040)

/** \brief EV8 TRA,BUSY,MSL,TXE */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTNG            ((uint32_t)0x00070080)

/** \brief TRA,BUSY,MSL,TXE,BTF */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTED            ((uint32_t)0x00070084)


/******************************************************************************
                             IIC SLAVE Events
 *****************************************************************************/

/** \brief EV1 BUSY,ADDR */
#define I2C_EVENT_SLAVE_RECEIER_ADDRES_MATCHED       ((uint32_t)0x00020002)
/** \brief EV1 TRA,BUSY,TXE,ADDR */
#define I2C_EVENT_SLAVE_TRAMSMITTER_ADDRESS_MATCHED  ((uint32_t)0x00060082)
/** \brief EV1 DUALF,BUSY */
#define I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED    ((uint32_t)0x00820000)

/** \brief EV1 DUALF,TRA,BUSY,TXE */
#define I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED ((uint32_t)0x00860080)

/** \brief EV1 GENCALL,BUSY */
#define I2C_EVENT_SLAVE_GENRALCALLADDRESS_MATCHED         ((uint32_t)0x00120000)

/** \brief EV2 BUSY，RXNE */
#define I2C_EVENT_SLAVE_BYTE_RECEIVED                     ((uint32_t)0x00020040)

/** \brief EV4 STOPF */
#define I2C_EVENT_SLAVE_STOP_DETECTED                     ((uint32_t)0x00000010)

/** \brief EV3 TRA,BUSY,TXE,BTF */
#define I2C_EVENT_SLAVE_BYTE_TRANSMITTED                  ((uint32_t)0x00060084)


/** \brief EV3 TRA,BUSY,TXE */
#define I2C_EVENT_SLAVE_BYTE_TRANSMITTING                 ((uint32_t)0x00060080)
/** \brief EV3_2 AF */
#define I2C_EVENT_SLAVE_ACK_FAILURE                       ((uint32_t)0x00000400)



/* \brief sr1 sr2 寄存器 状态检测宏*/
typedef enum {
    AMHW_ZLG_I2C_FLAG_DUALF      = ((uint32_t)0x00800000),
    AMHW_ZLG_I2C_FLAG_SMBHOST    = ((uint32_t)0x00400000),
    AMHW_ZLG_I2C_FLAG_SMBDEFAULT = ((uint32_t)0x00200000),
    AMHW_ZLG_I2C_FLAG_GENCALL    = ((uint32_t)0x00100000),
    AMHW_ZLG_I2C_FLAG_TRA        = ((uint32_t)0x00040000),
    AMHW_ZLG_I2C_FLAG_BUSY       = ((uint32_t)0x00020000),
    AMHW_ZLG_I2C_FLAG_MSL        = ((uint32_t)0x00010000),
    AMHW_ZLG_I2C_FLAG_SB         = ((uint32_t)0x10000001),
    AMHW_ZLG_I2C_FLAG_ADDR       = ((uint32_t)0x10000002),
    AMHW_ZLG_I2C_FLAG_BTF        = ((uint32_t)0x10000004),
    AMHW_ZLG_I2C_FLAG_ADD10      = ((uint32_t)0x10000008),
    AMHW_ZLG_I2C_FLAG_STOPF      = ((uint32_t)0x10000010),
    AMHW_ZLG_I2C_FLAG_RXNE       = ((uint32_t)0x10000040),
    AMHW_ZLG_I2C_FLAG_TXE        = ((uint32_t)0x10000080),
    AMHW_ZLG_I2C_FLAG_BERR       = ((uint32_t)0x10000100),
    AMHW_ZLG_I2C_FLAG_ARLO       = ((uint32_t)0x10000200),
    AMHW_ZLG_I2C_FLAG_AF         = ((uint32_t)0x10000400),
    AMHW_ZLG_I2C_FLAG_OVR        = ((uint32_t)0x10000800),
    AMHW_ZLG_I2C_FLAG_PECERR     = ((uint32_t)0x10001000),
    AMHW_ZLG_I2C_FLAG_TIMEOUT    = ((uint32_t)0x10004000),
    AMHW_ZLG_I2C_FLAG_SMBALERT   = ((uint32_t)0x10008000),

}amhw_zlg237_flag_t;



/**
 * \brief I2C中断标志
 */
typedef enum {
    AMHW_ZLG_INT_FLAG__SB       = ((uint32_t)0x02000001), /* <brief 起始位（主模式） */
    AMHW_ZLG_INT_FLAG__ADDR     = ((uint32_t)0x02000002), /* <brief 地址发送/匹配 */
    AMHW_ZLG_INT_FLAG__BTF      = ((uint32_t)0x02000004), /* <brief 字节发送结束 */
    AMHW_ZLG_INT_FLAG__ADD10    = ((uint32_t)0x02000008), /* <brief 10位头序列已发送 */
    AMHW_ZLG_INT_FLAG__STOPF    = ((uint32_t)0x02000010), /* <brief 停止条件检测位（从） */
    AMHW_ZLG_INT_FLAG__RXNE     = ((uint32_t)0x06000040), /* <brief 数据寄存器非空 （收）*/
    AMHW_ZLG_INT_FLAG__TXE      = ((uint32_t)0x06000080), /* <brief 数据寄存器空 （发） */
    AMHW_ZLG_INT_FLAG__BERR     = ((uint32_t)0x01000100), /* <brief 总线错误 */
    AMHW_ZLG_INT_FLAG__ARLO     = ((uint32_t)0x01000200), /* <brief 仲裁丢失（主） */
    AMHW_ZLG_INT_FLAG__AF       = ((uint32_t)0x01000400), /* <brief 应答失败 */
    AMHW_ZLG_INT_FLAG__OVR      = ((uint32_t)0x01000800), /* <brief 过载/欠载 */
    AMHW_ZLG_INT_FLAG__PECERR   = ((uint32_t)0x01001000), /* <brief 接收时发生PEC错误 */
    AMHW_ZLG_INT_FLAG__TIMEOUT  = ((uint32_t)0x01004000), /* <brief 超时/tlow错误 */
    AMHW_ZLG_INT_FLAG__SMBALERT = ((uint32_t)0x01008000), /* <brief SMBUS提醒 */
}amhw_zlg237_int_flag_t;

typedef enum {
    DISABLE = 0,
    ENABLE,
}state_t;

/**
 *  \brief 使能 I2C外设
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_enable (amhw_zlg237_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_cr1 |= (1u) << AMHW_ZLG237_I2C_CR1_PE;
}

/**
 *  \brief 禁能I2C外设
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_disable (amhw_zlg237_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_cr1 &= ~((1u) << AMHW_ZLG237_I2C_CR1_PE);
}



/**
 *  \brief 使能或禁能 I2C外设
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state:  0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_dma_en (amhw_zlg237_i2c_t *p_hw_i2c,
                             state_t           state)
{
    if (state) {

        /** <brief 当TxE = 1 或RxNE = 1时，允许DMA请求*/
        p_hw_i2c->i2c_cr2 |= 1 << AMHW_ZLG237_I2C_CR2_DMAEN;
    } else {

        /** <brief 禁止DMA请求 */
        p_hw_i2c->i2c_cr2 &= ~(1 << AMHW_ZLG237_I2C_CR2_DMAEN);
    }
}


/**
 *  \brief 最后一次传输DMA
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state:  0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_dma_last_en (amhw_zlg237_i2c_t *p_hw_i2c,
                                   state_t           state)
{
    if (state) {

        /** <brief 下一次的DMA的EOT是最后的传输 */
        p_hw_i2c->i2c_cr2 |= 1 << AMHW_ZLG237_I2C_CR2_LAST;
    } else {

        /** <brief 下一次DMA的EOT不是最后的传输 */
        p_hw_i2c->i2c_cr2 &= ~(1 << AMHW_ZLG237_I2C_CR2_LAST);
    }
}


/**
 *  \brief start 起始条件产生
 *
 *  \note: 软件可设置，或当起始条件发出后或PE = 0 时，由硬件清除
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state:  0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_genstrat (amhw_zlg237_i2c_t *p_hw_i2c,
                               state_t            state)
{
    if (state) {

        /** <brief 在当前字节传输或在当前起始条件发出后产生停止条件（主模式）
         *  在当前字节传输或释放SCL和SDA线（从模式）*/
        p_hw_i2c->i2c_cr1 |= (uint16_t)(1 << AMHW_ZLG237_I2C_CR1_START);
    } else {

        /** <brief 无起始条件产生（主从） */
        p_hw_i2c->i2c_cr1 &= ~(uint16_t)(1 << AMHW_ZLG237_I2C_CR1_START);
    }
}

/* \<brief 读取指定寄存器值 */
am_static_inline
uint16_t amhw_zlg237_i2c_read_reg(uint16_t  reg)
{
    return (uint16_t)reg;
}

/**
 *  \brief stop 停止条件产生
 *
 *  \note: 软件可设置，或当起始条件发出后或PE = 0 时，由硬件清除
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state:  0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_genstop (amhw_zlg237_i2c_t    *p_hw_i2c,
                              state_t               state)
{
    if (state) {

        /** <brief 重复产生起始条件（主模式） 当总线空闲时，产生起始条件（从模式）*/
        p_hw_i2c->i2c_cr1 |= (uint16_t)(1 << AMHW_ZLG237_I2C_CR1_STOP);
    } else {

        /** <brief 无起始条件产生（主从） */
        p_hw_i2c->i2c_cr1 &= ~(uint16_t)(1 << AMHW_ZLG237_I2C_CR1_STOP);
    }
}

/**
 *  \brief Ack 应答使能
 *
 *  \note: 软件可设置，或当起始条件发出后或PE = 0 时，由硬件清除
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state:  0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_ack_en (amhw_zlg237_i2c_t *p_hw_i2c,
                             state_t            state)
{
    if (state) {

        /** <brief 在收到一个字节后返回一个应答（匹配的地址或数据）  */
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_ACK;
    } else {

        /** <brief 无应答返回 */
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_ACK);
    }
}


/** \brief 配置自身地址寄存器 2 oar2
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] address:   指向uint8_t 类型的地址
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_oar2_config (amhw_zlg237_i2c_t *p_hw_i2c,
                                  uint8_t            address)
{

    /** <brief 初始化 add2[7:1] */
    p_hw_i2c->i2c_oar2 &= ~(0x7f << AMHW_ZLG237_I2C_OAR2_ADD2);

    /** <brief 设置 7位地址 */
    p_hw_i2c->i2c_oar2 |= (uint16_t)((uint16_t)address & (0x7f <<AMHW_ZLG237_I2C_OAR2_ADD2));
}


/** \brief  endual 双地址模式使能位
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state:   0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_endual (amhw_zlg237_i2c_t    *p_hw_i2c,
                             state_t               state)
{
    if (state) {

        /** <brief 在7位地址模式下，oar1和oar2都能被识别*/
        p_hw_i2c->i2c_oar2 |= 1 << AMHW_ZLG237_I2C_OAR2_ENDUAL;
    } else {

        /** <brief 在7位地址模式下，只有oar1能被识别*/
        p_hw_i2c->i2c_oar2 &= ~(1 << AMHW_ZLG237_I2C_OAR2_ENDUAL);
    }
}


/** \brief  engc 广播呼叫使能位
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state:   0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_engc (amhw_zlg237_i2c_t    *p_hw_i2c,
                           state_t               state)
{
    if (state) {

        /** <brief ENGC使能 允许广播呼叫 以应答响应地址 00h  */
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_ENGC;
    } else {

        /** <brief ENGC禁能 禁止广播呼叫 以非应答响应地址 00h */
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_ENGC);
    }
}


/** \brief  中断模式使能设置
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] mode: 指向amhw_zlg237_i2c_iten_tl 的enum成员
 *  \param[in] state:   0 禁能  1 使能
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_iten_mode_set (amhw_zlg237_i2c_t      *p_hw_i2c,
                                    uint32_t                mode,
                                    state_t                 state)
{
    if (mode & I2C_CR2_ITERREN) {
        if (state) {
            p_hw_i2c->i2c_cr2 |= 1 << AMHW_ZLG237_I2C_CR2_ITERREN;
        } else {
            p_hw_i2c->i2c_cr2 &= ~(1 << AMHW_ZLG237_I2C_CR2_ITERREN);
        }
    }

    if (mode & I2C_CR2_ITEVTEN) {
        if (state) {
            p_hw_i2c->i2c_cr2 |= 1 << AMHW_ZLG237_I2C_CR2_ITEVTEN;
        } else {
            p_hw_i2c->i2c_cr2 &= ~(1 << AMHW_ZLG237_I2C_CR2_ITEVTEN);
        }
    }

    if (mode & I2C_CR2_ITBUFEN) {
        if (state) {
            p_hw_i2c->i2c_cr2 |= 1 << AMHW_ZLG237_I2C_CR2_ITBUFEN;
        } else {
            p_hw_i2c->i2c_cr2 &= ~(1 << AMHW_ZLG237_I2C_CR2_ITBUFEN);
        }
    }
}

/** \brief  发送数据
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] data:   数据
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_send_data (amhw_zlg237_i2c_t *p_hw_i2c,
                                uint8_t            data)
{
    p_hw_i2c->i2c_dr = data;
}

/** \brief  接收数据
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *
 *  \return uint8_t类型的数据
 */
am_static_inline
uint8_t amhw_zlg237_i2c_recv_data (amhw_zlg237_i2c_t *p_hw_i2c)
{
    return (uint8_t)p_hw_i2c->i2c_dr;
}

/** \brief  发送地址 以选择从设备
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] address: 被发送的从设备地址  uint8_t
 *  \param[in] slv_mode: 设定从设备的 模式  ：  0 发送    1 接收
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_send7bit_address (amhw_zlg237_i2c_t   *p_hw_i2c,
                                       uint8_t              address,
                                       uint8_t              slv_mode)
{
    /** <brief read */
    p_hw_i2c->i2c_dr  = (address << 1) | (slv_mode & 0x01);
}

/** \brief  软件复位
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0 不处于复位状态    1 处于复位状态
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_swrst (amhw_zlg237_i2c_t      *p_hw_i2c,
                            state_t                 state)
{
    if (state) {

        /** <brief  I2C模块处于复位状态 */
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_SWRST;
    } else {

        /** <brief  I2C模块不处于复位状态 */
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_SWRST);
    }
}

/** \brief  pos ack/pec 位置 （用于数据接收）
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0 当前NACK    1 下一个字节的NACK
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_pos (amhw_zlg237_i2c_t      *p_hw_i2c,
                          state_t                 state)
{
    if (state) {

        /** <brief ack 位控制当前移位寄存器内接收的下一个字节的(N)ACK. PEC位表明当前移位寄存器内接收的下一个字节是PEC*/
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_POS;
    } else {

        /** <brief ack 位控制当前移位寄存器内正在接收的字节的(N)ACK. PEC位表明当前移位寄存器内的字节是PEC*/
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_POS);
    }
}

/** \brief  alert SMBus提醒
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0 释放SMBAlert 引脚使其变高。提醒响应地址头紧跟在NACK信号后面
 *                    1 驱动SMBAlert 引脚使其变低。提醒响应地址头紧跟在ACK信号后面
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_alert (amhw_zlg237_i2c_t      *p_hw_i2c,
                            state_t                 state)
{
    if (state) {
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_ALERT;
    } else {
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_ALERT);
    }
}

/** \brief  pec 数据包出错检测
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0  无PEC传输
 *                    1  PEC传输（在发送或接收模式）
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_pec (amhw_zlg237_i2c_t      *p_hw_i2c,
                          state_t                 state)
{
    if (state) {
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_PEC;
    } else {
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_PEC);
    }
}

/** \brief  enpec pec使能
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0  禁止PEC计算
 *                    1  开启PEC计算
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_enpec (amhw_zlg237_i2c_t      *p_hw_i2c,
                            state_t                 state)
{
    if (state) {
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_ENPEC;
    } else {
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_ENPEC);
    }
}

/** \brief  pec[7:0]  获取存放在PEC[7:0] 中的PEC的值
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \return uint8_t类型的 PEC的值
 */
am_static_inline
uint8_t amhw_zlg237_i2c_getpec (amhw_zlg237_i2c_t      *p_hw_i2c)
{
    return (p_hw_i2c->i2c_sr2 >> AMHW_ZLG237_I2C_SR2_PEC7_1);
}

/** \brief  enarp arp使能
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0  禁止ARP使能
 *                    1  使能ARP
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_enarp (amhw_zlg237_i2c_t      *p_hw_i2c,
                            state_t                 state)
{
    if (state) {
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_ENARP;
    } else {
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_ENARP);
    }
}

/** \brief  nostretch 禁止时钟延长（从模式）
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0  允许时钟延长
 *                    1  禁止时钟延长
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_nostretch (amhw_zlg237_i2c_t      *p_hw_i2c,
                                state_t                 state)
{
    if (state) {
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CR1_NOSTRETCH;
    } else {
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CR1_NOSTRETCH);
    }
}

/** \brief  duty 快速模式时的占空比
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] state: 0  快速模式下： Tlow/Thigh = 2
 *                    1  快速模式下： Tlow/Thigh = 16/9
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_fastmodeduty (amhw_zlg237_i2c_t      *p_hw_i2c,
                                   state_t                 state)
{
    if (state) {
        p_hw_i2c->i2c_cr1 |= 1 << AMHW_ZLG237_I2C_CCR_DUTY;
    } else {
        p_hw_i2c->i2c_cr1 &= ~(1 << AMHW_ZLG237_I2C_CCR_DUTY);
    }
}

/** \brief  检测上一次事件是否是已知事件
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] event：     事件宏定义
 *
 *  \return 是否是 已定义的宏事件    0 不是  1 是
 */
am_static_inline
uint8_t amhw_zlg237_i2c_checkevent(amhw_zlg237_i2c_t      *p_hw_i2c,
                                   uint32_t                event)
{
    static uint32_t lastevent = 0;

    /** <brief 从I2C状态寄存器中获取 上一个事件的的值 */
    lastevent = (uint32_t)(((uint32_t)p_hw_i2c->i2c_sr2 << 16) | (uint32_t)p_hw_i2c->i2c_sr1) & 0x00FFFFFF;

    if ((lastevent & event) == event) {

        /** <brief 上一个事件是已发生的宏定义事件 */
        return 1;
    } else {

        /** <brief 上一个事件与定义的宏事件不同 */
        return 0;
    }
}

/** \brief  获取上一次事件值
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] event：     事件宏定义
 *
 *  \return uint32_t类型的事件的值
 */
am_static_inline
uint32_t amhw_zlg237_i2c_getlastevent(amhw_zlg237_i2c_t      *p_hw_i2c)
{

    /** <brief 从I2C状态寄存器中获取 上一个事件的的值 */
    return (((uint32_t)p_hw_i2c->i2c_sr2 << 16) | (uint32_t)p_hw_i2c->i2c_sr1) & 0x00FFFFFF;
}

/** \brief  检测I2C状态寄存器的 各个状态位是否设置
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] flag：     各个位状态寄存器掩码   指向amhw_zlg237_flag_t
 *
 *  \return   置位为  0   置位为 1
 */
am_static_inline
int amhw_zlg237_i2c_checkflagstaus(amhw_zlg237_i2c_t      *p_hw_i2c,
                                   amhw_zlg237_flag_t      i2c_flag)
{
    __IO uint32_t i2c_sr_num = 0,i2c_sr_value = 0;

    /** <brief 获取 状态寄存器 编号    0 1  */
    i2c_sr_num = i2c_flag >> 28;

    /** <brief 获得 [23:0] 的寄存器状态 */
    i2c_flag &= 0x00FFFFFF;

    if (i2c_sr_num != 0) {

        /** \brief 读取sr1的寄存器值         */
        i2c_sr_value = p_hw_i2c->i2c_sr1;
    } else {

        /** \brief 读取sr2的寄存器值         */
        i2c_flag = (amhw_zlg237_flag_t)(i2c_flag >> 16);
        i2c_sr_value = p_hw_i2c->i2c_sr2;
    }

    if ((i2c_sr_value & i2c_flag) != (uint32_t)0) {

        /** \brief 该位已被置 1 */
        return 1;
    } else {

        /** \brief 该位已被置0 */
        return 0;
    }
}

/** \brief  清零SR1寄存器指定位上的值
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] flag： amhw_zlg237_flag_t
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_clearflag (amhw_zlg237_i2c_t      *p_hw_i2c,
                                amhw_zlg237_flag_t      i2c_flag)
{
    p_hw_i2c->i2c_sr1 = (uint16_t)~(i2c_flag & 0x00FFFFFF);
}

/** \brief  获取指定中断的的状态
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] i2c_it：          指向amhw_zlg237_int_flag_t
 *
 *  \return  1 中断已设置    0 中断未设置
 */
am_static_inline
int amhw_zlg237_i2c_get_itflagstatus (amhw_zlg237_i2c_t      *p_hw_i2c,
                                      amhw_zlg237_int_flag_t  i2c_it)
{
    uint32_t enablestatus = 0;

    /** <brief 检测指定IIC中断源是否使能 */
    enablestatus = (uint32_t)(((i2c_it & 0x07000000) >> 16) & p_hw_i2c->i2c_cr2);

    /** <brief 获得 bit[23：0]的值 */
    i2c_it &= 0x00FFFFFF;

    if (((p_hw_i2c->i2c_sr1 & i2c_it) != (uint32_t)(0)) && enablestatus) {

        /** <brief 中断已设置 */
        return 1;
    } else {

        /** <brief 中断未设置 */
        return 0;
    }
}

/** \brief  获取中断的的状态
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] i2c_it：          指向amhw_zlg237_int_flag_t
 *
 *  \return  1 中断已设置    0 中断未设置
 */
am_static_inline
uint16_t amhw_zlg237_i2c_getitstatus (amhw_zlg237_i2c_t *p_hw_i2c)
{
    return (uint16_t)(p_hw_i2c->i2c_sr1 & 0xdfdf);
}

/** \brief  清零指定的中断请求
 *
 *  \param[in] p_hw_i2c : 指向I2C寄存器结构体的指针
 *  \param[in] i2c_it：                 I2C_IT_BERR
 *                          I2C_IT_ARLO
 *                          I2C_IT_AF
 *                          I2C_IT_OVR
 *                          I2C_IT_PECERR
 *                          I2C_IT_TIMEOUT
 *                          I2C_IT_SMBALERT
 *
 *  \return 无
 */
am_static_inline
void amhw_zlg237_i2c_clearit(amhw_zlg237_i2c_t      *p_hw_i2c,
                             amhw_zlg237_int_flag_t  i2c_it)
{
    p_hw_i2c->i2c_sr1 = (uint16_t)~(i2c_it & 0x00FFFFFF);
}

am_static_inline
void amhw_zlg237_i2c_clearallit(amhw_zlg237_i2c_t      *p_hw_i2c)
{

    /** SB  read sr1 write dr */

    /** ADDR read sr1 read sr2 */

    /** BRF read sr1 read or write dr */

    /** ADD10 read sr1 write cr1 */

    /** STOPF read sr1 write cr1 */

    /** RXNE read or write dr */

    /** TXE write dr */
    amhw_zlg237_i2c_read_reg(p_hw_i2c->i2c_sr1);
    amhw_zlg237_i2c_read_reg(p_hw_i2c->i2c_sr2);

    p_hw_i2c->i2c_cr1 &= 0xffff;
    p_hw_i2c->i2c_dr  &= 0xffff;

    p_hw_i2c->i2c_sr1 &= (uint16_t)~(0x00FFFFFF);
}


/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_ZLG237_I2C_H */

#endif

/* end of file */
