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
 * \brief CAN hardware operation interface
 *
 * -note:在对同一寄存器进行读写时其所对应的功能不同
 *       r_w1  r_w
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-31  zc, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG237_CAN_H
#define __AMHW_ZLG237_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_zlg237_if_can
 * \copydoc amhw_zlg237_can.h
 * @{
 */
/* \brief 发送邮箱寄存器  */
typedef struct amhw_zlg237_can_tx {
    __IO  uint32_t tir;      /**< \brief 0x180 发送邮箱标识符寄存器 */
    __IO  uint32_t tdtr;     /**< \brief 0x184 发送邮箱数据长度和时间戳寄存器*/
    __IO  uint32_t tdlr;     /**< \brief 0x188 发送邮箱低字节数据寄存器*/
    __IO  uint32_t tdhr;     /**< \brief 0x18c 发送邮箱高字节数据寄存器 */
}amhw_zlg237_can_tx_t;

/* \brief 接收FIFO邮箱寄存器  */
typedef struct amhw_zlg237_can_rx {
    __IO  uint32_t rir;      /**< \brief 0x1c0  接收FIFO邮箱标识符寄存器*/
    __IO  uint32_t rdtr;     /**< \brief 0x1c4 接收FIFO邮箱数据长度和时间戳寄存器 */
    __IO  uint32_t rdlr;     /**< \brief 0x1c8 接收FIFO邮箱低字节数据寄存器 */
    __IO  uint32_t rdhr;     /**< \brief 0x1cc 接收FIFO邮箱高字节数据寄存器*/
}amhw_zlg237_can_rx_t;

/* \brief 过滤器组寄存器  */
typedef struct amhw_zlg237_can_firx {
    __IO  uint32_t f_r1;     /**< \brief 过滤器组寄存器 1 */
    __IO  uint32_t f_r2;     /**< \brief 过滤器组寄存器 2 */
}amhw_zlg237_can_firx_t;

/**
 * \brief ZLG237 CAN寄存器块结构体
 */
typedef struct amhw_zlg237_can {

    __IO  uint32_t mcr;             /**< \brief 0x00 主控制寄存器 */
    __IO  uint32_t msr;             /**< \brief 0x04 主状态寄存器 */
    __IO  uint32_t tsr;             /**< \brief 0x08 发送状态寄存器 */
    __IO  uint32_t rf_0r;           /**< \brief 0x0c 接收FIFO 0 寄存器 */
    __IO  uint32_t rf_1r;           /**< \brief 0x10 接收FIFO 1 寄存器 */
    __IO  uint32_t ier;             /**< \brief 0x14 中断使能寄存器 */
    __IO  uint32_t esr;             /**< \brief 0x18 错误状态寄存器 */
    __IO  uint32_t btr;             /**< \brief 0x1c 位时序使能寄存器 */

          uint32_t reserve_0[88];   /**< \brief 保留位*/

    /**< \brief 0x180...0x1ac 发送邮箱寄存器 */
    __IO  amhw_zlg237_can_tx_t tx_mail[3];

    /**< \brief 0x1b0...0x1cc 接收FIFO邮箱寄存器 */
    __IO  amhw_zlg237_can_rx_t rx_mail[2];

          uint32_t reserve_1[12];   /**< \brief 保留位*/

    __IO  uint32_t fmr;             /**< \brief 0x200 过滤器主控寄存器 */
    __IO  uint32_t fm_1r;           /**< \brief 0x204 过滤器模式寄存器 */
          uint32_t reserve_2;       /**< \brief 保留位*/
    __IO  uint32_t fs_1r;           /**< \brief 0x20c 过滤器位宽寄存器 */
          uint32_t reserve_3;       /**< \brief 保留位*/
    __IO  uint32_t ffa_1r;          /**< \brief 0x214 过滤器FIFO关联寄存器 */
          uint32_t reserve_4;       /**< \brief 保留位*/
    __IO  uint32_t fa_1r;           /**< \brief 0x21c 过滤器激活寄存器 */
          uint32_t reserve_5[8];    /**< \brief 保留位*/

    /**< \brief 0x240...0x31c 过滤器组寄存器 */
    __IO  amhw_zlg237_can_firx_t fi_rx[14];


}amhw_zlg237_can_t;

/**<brief 传输信息 */
typedef struct {
    uint32_t std_id;    /**< \brief 标识符 */
    uint32_t ext_id;    /**< \brief 扩展标识符*/
    uint8_t  ide;       /**< \brief 标识符选择*/
    uint8_t  rtr;       /**< \brief 远程发送请求： 数据帧    远程帧*/
    uint8_t  dlc;       /**< \brief 发送数据长度*/
    uint8_t  data[8];   /**< \brief 待发送的数据*/
} amhw_zlg237_can_tx_msg_t;

/**<brief 接收信息 */
typedef struct {
    uint32_t std_id;    /**< \brief 标识符 */
    uint32_t ext_id;    /**< \brief 扩展标识符*/
    uint8_t  ide;       /**< \brief 标识符选择*/
    uint8_t  rtr;       /**< \brief 远程发送请求： 数据帧    远程帧*/
    uint8_t  dlc;       /**< \brief 发送数据长度*/
    uint8_t  data[8];   /**< \brief 待发送的数据*/
    uint8_t  fmi;       /**< \brief 过滤器匹配序号*/
} amhw_zlg237_can_rx_msg_t;

/**
 * \brief CAN运行模式定义
 */
typedef enum amhw_zlg237_can_operating_type {
    AMHW_ZLG237_CAN_OPERATING_MODE_INIT= 1,      /**< \brief 初始化模式 */
    AMHW_ZLG237_CAN_OPERATING_NORMAL,            /**< \brief 正常模式*/
    AMHW_ZLG237_CAN_OPERATING_SLEEP,             /**< \brief 睡眠模式*/
}amhw_zlg237_can_mode_t;

/**
 * \brief CAN自测模式定义
 */
typedef enum  {
    AMHW_ZLG237_CAN_MODE_NORMAL = 0,            /**< \brief 正常模式*/
    AMHW_ZLG237_CAN_MODE_LOOPBACK,            /**< \brief 环回模式*/
    AMHW_ZLG237_CAN_MODE_SILENT,              /**< \brief 静默模式*/
    AMHW_ZLG237_CAN_MODE_SIEENT_LOOPBACK,     /**< \brief 环回静默*/
}amhw_zlg237_can_test_mode_t;

/**\brief 中断源定义 */
typedef enum {
    AMHW_ZLG237_CAN_INT_TMEIE = 0,            /**< \brief 发送邮箱空中断使能 */
    AMHW_ZLG237_CAN_INT_FMPIE0,               /**< \brief FIFO 0 消息挂号中断使能 */
    AMHW_ZLG237_CAN_INT_FFIE0,                /**< \brief FIFO 0 满中断中断使能 */
    AMHW_ZLG237_CAN_INT_FOVIE0,               /**< \brief FIFO 0 溢出中断使能 */
    AMHW_ZLG237_CAN_INT_FMPIE1,               /**< \brief FIFO 1 消息挂号中断使能 */
    AMHW_ZLG237_CAN_INT_FFIE1,                /**< \brief FIFO 1 满中断中断使能 */
    AMHW_ZLG237_CAN_INT_FOVIE1,               /**< \brief FIFO 1 溢出中断使能 */
    AMHW_ZLG237_CAN_INT_EWGIE = 8,            /**< \brief 错误警告中断使能 */
    AMHW_ZLG237_CAN_INT_EPVIE,                /**< \brief 错误被动中断使能 */
    AMHW_ZLG237_CAN_INT_BOFIE,                /**< \brief 离线中断使能 */
    AMHW_ZLG237_CAN_INT_LECIE,                /**< \brief 上次错误号中断使能 */
    AMHW_ZLG237_CAN_INT_ERRIE = 15,           /**< \brief 错误中断使能 */
    AMHW_ZLG237_CAN_INT_WKUIE,                /**< \brief 唤醒中断使能 */
    AMHW_ZLG237_CAN_INT_SLKIE,                /**< \brief 睡眠中断使能 */
}amhw_zlg237_can_int_t;

#define AMHW_ZLG237_CAN_FLAG_TSR    ((uint32_t)0x08000000)
#define AMHW_ZLG237_CAN_FLAG_RF1R   ((uint32_t)0x04000000)
#define AMHW_ZLG237_CAN_FLAG_RF0R   ((uint32_t)0x02000000)
#define AMHW_ZLG237_CAN_FLAG_MSR    ((uint32_t)0x01000000)
#define AMHW_ZLG237_CAN_FLAG_ESR    ((uint32_t)0x00F00000)

/**\brief flag标志位定义 */
typedef enum {
    /* 发送*/
    AMHW_ZLG237_CAN_FLAG_RQCP0 = 0X38000001,  /**< \brief 邮箱0请求完成 */
    AMHW_ZLG237_CAN_FLAG_RQCP1 = 0X38000100,  /**< \brief 邮箱1请求完成 */
    AMHW_ZLG237_CAN_FLAG_RQCP2 = 0X38010000,  /**< \brief 邮箱2请求完成 */

    /* 接收 */
    AMHW_ZLG237_CAN_FLAG_FMP0  = 0X12000003,  /**< \brief FIFO 0 挂号*/
    AMHW_ZLG237_CAN_FLAG_FF0   = 0X32000008,  /**< \brief FIFO 0 满 */
    AMHW_ZLG237_CAN_FLAG_FOV0  = 0X32000010,  /**< \brief FIFO 0 溢出 */
    AMHW_ZLG237_CAN_FLAG_FMP1  = 0X14000003,  /**< \brief FIFO 1 挂号 */
    AMHW_ZLG237_CAN_FLAG_FF1   = 0X34000008,  /**< \brief FIFO 1 满  */
    AMHW_ZLG237_CAN_FLAG_FOV1  = 0X34000010,  /**< \brief FIFO 1 溢出 */

    /* 运行模式 */
    AMHW_ZLG237_CAN_FLAG_WKU   = 0X31000008,  /**< \brief 唤醒标识位 */
    AMHW_ZLG237_CAN_FLAG_SKAK  = 0X31000012,  /**< \brief 睡眠应答 */

    AMHW_ZLG237_CAN_FLAG_ERI   = 0x31000040,  /**< \brief 出错中断挂号 */

    /* 错误 */
    AMHW_ZLG237_CAN_FLAG_EWG   = 0X10f00001,  /**< \brief 错误警告 */
    AMHW_ZLG237_CAN_FLAG_EPV   = 0X10f00002,  /**< \brief 错误被动 */
    AMHW_ZLG237_CAN_FLAG_BOF   = 0X10f00004,  /**< \brief 离线 */
    AMHW_ZLG237_CAN_FLAG_LEC   = 0x30f00070,  /**< \brief 上次错误代码 */

}amhw_zlg237_can_flag_t;

/** \brief 中断标志 */
typedef enum {
    AMHW_ZLG237_CAN_INT_FLAG_TME  = ((uint32_t)0x00000001),
    AMHW_ZLG237_CAN_INT_FLAG_RQCP0 = AMHW_ZLG237_CAN_INT_FLAG_TME,
    AMHW_ZLG237_CAN_INT_FLAG_RQCP1 = AMHW_ZLG237_CAN_INT_FLAG_TME,
    AMHW_ZLG237_CAN_INT_FLAG_RQCP2 = AMHW_ZLG237_CAN_INT_FLAG_TME,
    AMHW_ZLG237_CAN_INT_FLAG_FMP0 = ((uint32_t)0x00000002),
    AMHW_ZLG237_CAN_INT_FLAG_FF0  = ((uint32_t)0x00000004),
    AMHW_ZLG237_CAN_INT_FLAG_FOV0 = ((uint32_t)0x00000008),
    AMHW_ZLG237_CAN_INT_FLAG_FMP1 = ((uint32_t)0x00000010),
    AMHW_ZLG237_CAN_INT_FLAG_FF1  = ((uint32_t)0x00000020),
    AMHW_ZLG237_CAN_INT_FLAG_FOV1 = ((uint32_t)0x00000040),

    AMHW_ZLG237_CAN_INT_FLAG_WKU = ((uint32_t)0x00010000),
    AMHW_ZLG237_CAN_INT_FLAG_SLK = ((uint32_t)0x00020000),
    AMHW_ZLG237_CAN_INT_FLAG_EWG = ((uint32_t)0x00000100),
    AMHW_ZLG237_CAN_INT_FLAG_EPV = ((uint32_t)0x00000200),
    AMHW_ZLG237_CAN_INT_FLAG_BOF = ((uint32_t)0x00000400),
    AMHW_ZLG237_CAN_INT_FLAG_LEC = ((uint32_t)0x00000800),
    AMHW_ZLG237_CAN_INT_FLAG_ERR = ((uint32_t)0x00008000),
}amhw_zlg237_can_int_flag_t;




typedef enum  {
    AMHW_ZLG237_CAN_BS1_1TQ = 0,
    AMHW_ZLG237_CAN_BS1_2TQ,
    AMHW_ZLG237_CAN_BS1_3TQ,
    AMHW_ZLG237_CAN_BS1_4TQ,
    AMHW_ZLG237_CAN_BS1_5TQ,
    AMHW_ZLG237_CAN_BS1_6TQ,
    AMHW_ZLG237_CAN_BS1_7TQ,
    AMHW_ZLG237_CAN_BS1_8TQ,
    AMHW_ZLG237_CAN_BS1_9TQ,
    AMHW_ZLG237_CAN_BS1_10TQ,
    AMHW_ZLG237_CAN_BS1_11TQ,
    AMHW_ZLG237_CAN_BS1_12TQ,
    AMHW_ZLG237_CAN_BS1_13TQ,
    AMHW_ZLG237_CAN_BS1_14TQ,
    AMHW_ZLG237_CAN_BS1_15TQ,
    AMHW_ZLG237_CAN_BS1_16TQ,
}amhw_zlg237_can_bs1_t;

typedef enum  {
    AMHW_ZLG237_CAN_BS2_1TQ = 0,
    AMHW_ZLG237_CAN_BS2_2TQ,
    AMHW_ZLG237_CAN_BS2_3TQ,
    AMHW_ZLG237_CAN_BS2_4TQ,
    AMHW_ZLG237_CAN_BS2_5TQ,
    AMHW_ZLG237_CAN_BS2_6TQ,
    AMHW_ZLG237_CAN_BS2_7TQ,
    AMHW_ZLG237_CAN_BS2_8TQ,
}amhw_zlg237_can_bs2_t;

#define CAN_DISABLE 0x00
#define CAN_ENABLE  0x01


#define AMHW_ZLG237_CAN_SJW_1TQ    ((uint8_t)0x00)      /**< \brief 1个时间单元 */
#define AMHW_ZLG237_CAN_SJW_2TQ    ((uint8_t)0x01)      /**< \brief 2个时间单元 */
#define AMHW_ZLG237_CAN_SJW_3TQ    ((uint8_t)0x02)      /**< \brief 3个时间单元 */
#define AMHW_ZLG237_CAN_SJW_4TQ    ((uint8_t)0x03)      /**< \brief 4个时间单元 */

/**< \brief 标准标识符 */
#define AMHW_ZLG237_CAN_ID_STANDARD    ((uint32_t)0x00000000)

/**< \brief 扩展标识符 */
#define AMHW_ZLG237_CAN_ID_EXTENDED    ((uint32_t)0x00000001)

#define CAN_IS_STD      AMHW_ZLG237_CAN_ID_STANDARD
#define CAN_ID_EXT      AMHW_ZLG237_CAN_ID_EXTENDED

/**< \brief 数据帧 */
#define AMHW_ZLG237_CAN_RTR_DATA       ((uint32_t)0x00000000)

/**< \brief 远程帧 */
#define AMHW_ZLG237_CAN_RTR_REMOTE     ((uint32_t)0x00000001)

#define CAN_RTR_DATA        AMHW_ZLG237_CAN_RTR_DATA
#define CAN_RTR_REMOTE      AMHW_ZLG237_CAN_RTR_REMOTE

/** \brief 滤波器位宽 */
#define AMHW_ZLG237_CAN_FILTER_SCALE_16BIT  0X00
#define AMHW_ZLG237_CAN_FILTER_SCALE_32BIT  0X01

/** \brief 滤波器模式 */
#define AMHW_ZLG237_CAN_FILTER_MODE_IDMASK  0X00    /**< \brief 屏蔽位模式 */
#define AMHW_ZLG237_CAN_FILTER_MODE_IDLIST  0X01    /**< \brief 标识符列表模式 */

/** \brief 滤波器FIFO 关联 */
#define AMHW_ZLG237_CAN_FILTER_FIFO0  0X00    /**< \brief FIFO 0  */
#define AMHW_ZLG237_CAN_FILTER_FIFO1  0X01    /**< \brief FIFO 1 */

#define AMHW_ZLG237_CAN_LEC_NOERR      0X00    /**< \brief 无错误  */
#define AMHW_ZLG237_CAN_LEC_STUFER     0X01    /**< \brief 位填充错误  */
#define AMHW_ZLG237_CAN_LEC_FORMER     0X02    /**< \brief 格式错误  */
#define AMHW_ZLG237_CAN_LEC_ACKER      0X03    /**< \brief 确认错误  */
#define AMHW_ZLG237_CAN_LEC_BRCER      0X04    /**< \brief 隐性错误  */
#define AMHW_ZLG237_CAN_LEC_BDERR      0X05    /**< \brief 显性错误 */
#define AMHW_ZLG237_CAN_LEC_CRCER      0X06    /**< \brief CRC错误 */
#define AMHW_ZLG237_CAN_LEC_SOFTER     0X07    /**< \brief 由软件设置  */


/**< \brief 调试冻结 */
#define AMHW_ZLG237_CAN_MCR_DBF        ((uint32_t)(1ul << 16))

/**< \brief 软件复位 */
#define AMHW_ZLG237_CAN_MCR_RESET      ((uint32_t)(1ul << 15))

/**< \brief 时间通信触发模式 */
#define AMHW_ZLG237_CAN_MCR_TTCM       ((uint32_t)(1ul << 7))

/**< \brief 自动离线管理 */
#define AMHW_ZLG237_CAN_MCR_ABOM       ((uint32_t)(1ul << 6))

/**< \brief 自动唤醒模式 */
#define AMHW_ZLG237_CAN_MCR_AWUM       ((uint32_t)(1ul << 5))

/**< \brief 禁止报文自动重传 */
#define AMHW_ZLG237_CAN_MCR_NART       ((uint32_t)(1ul << 4))

/**< \brief 接收FIFO锁定模式*/
#define AMHW_ZLG237_CAN_MCR_RFLM       ((uint32_t)(1ul << 3))

/**< \brief 发送FIFO优先级 */
#define AMHW_ZLG237_CAN_MCR_TXFP       ((uint32_t)(1ul << 2))

/**< \brief 睡眠模式请求 */
#define AMHW_ZLG237_CAN_MCR_SLEEP      ((uint32_t)(1ul << 1))

/**< \brief 初始化请求 */
#define AMHW_ZLG237_CAN_MCR_INRQ       ((uint32_t)(1ul << 0))


/**< \brief 初始化确认*/
#define AMHW_ZLG237_CAN_MSR_INAK       ((uint32_t)(1ul << 0))

/**< \brief 睡眠模式确认*/
#define AMHW_ZLG237_CAN_MSR_SLAK       ((uint32_t)(1ul << 1))

/**< \brief 出错中断挂号*/
#define AMHW_ZLG237_CAN_MSR_ERRI       ((uint32_t)(1ul << 2))

/**< \brief 唤醒中断挂号*/
#define AMHW_ZLG237_CAN_MSR_WKUI       ((uint32_t)(1ul << 3))

/**< \brief 睡眠模确认中断*/
#define AMHW_ZLG237_CAN_MSR_SLAKI      ((uint32_t)(1ul << 4))

/**< \brief FIFO 0 报文数目 */
#define AMHW_ZLG237_CAN_RF0R_FMP0      ((uint32_t)(3ul << 0))
/**< \brief FIFO 0 满*/
#define AMHW_ZLG237_CAN_RF0R_FULL0     ((uint32_t)(1ul << 3))
/**< \brief FIFO 0 溢出*/
#define AMHW_ZLG237_CAN_RF0R_FOVR0     ((uint32_t)(1ul << 4))
/**< \brief 释放接收FIFO 0输出邮箱*/
#define AMHW_ZLG237_CAN_RF0R_RFOM0     ((uint32_t)(1ul << 5))

/**< \brief FIFO 1 报文数目 */
#define AMHW_ZLG237_CAN_RF0R_FMP1      ((uint32_t)(3ul << 0))
/**< \brief FIFO 1 满*/
#define AMHW_ZLG237_CAN_RF0R_FULL1     ((uint32_t)(1ul << 3))
/**< \brief FIFO 1 溢出*/
#define AMHW_ZLG237_CAN_RF0R_FOVR1     ((uint32_t)(1ul << 4))
/**< \brief 释放接收FIFO 1输出邮箱*/
#define AMHW_ZLG237_CAN_RF1R_RFOM1     ((uint32_t)(1ul << 5))

/**< \brief 发送数据请求 */
#define AMHW_ZLG237_CAN_TIR_TXRQ       ((uint32_t)(1ul << 0))

/**< \brief 错误警告标志*/
#define AMHW_ZLG237_CAN_ESR_EWGF       ((uint32_t)(1ul << 0))

/**< \brief 错误被动标志*/
#define AMHW_ZLG237_CAN_ESR_EPVF       ((uint32_t)(1ul << 1))

/**< \brief 离线标志*/
#define AMHW_ZLG237_CAN_ESR_BOFF       ((uint32_t)(1ul << 2))

/**< \brief 上次错误代码 */
#define AMHW_ZLG237_CAN_ESR_LEC        ((uint32_t)(7ul << 4))

/**< \brief 接收错误计数器 */
#define AMHW_ZLG237_CAN_ESR_REC        ((uint32_t)(0XFFul << 24))

/**< \brief 接收错误计数器 */
#define AMHW_ZLG237_CAN_ESR_TEC        ((uint32_t)(0XFFul << 16))

/**< \brief 邮箱 0 请求完成*/
#define AMHW_ZLG237_CAN_TSR_RQCP0      ((uint32_t)(1ul << 0))

/**< \brief 邮箱 0 发送成功*/
#define AMHW_ZLG237_CAN_TSR_TXOK0      ((uint32_t)(1ul << 1))

/**< \brief 邮箱 0 中止发送*/
#define AMHW_ZLG237_CAN_TSR_ABRQ0      ((uint32_t)(1ul << 7))

/**< \brief 邮箱 1 请求完成*/
#define AMHW_ZLG237_CAN_TSR_RQCP1      ((uint32_t)(1ul << 8))

/**< \brief 邮箱 1 发送成功*/
#define AMHW_ZLG237_CAN_TSR_TXOK1      ((uint32_t)(1ul << 9))

/**< \brief 邮箱 1 中止发送*/
#define AMHW_ZLG237_CAN_TSR_ABRQ1      ((uint32_t)(1ul << 15))

/**< \brief 邮箱 2 请求完成*/
#define AMHW_ZLG237_CAN_TSR_RQCP2      ((uint32_t)(1ul << 16))

/**< \brief 邮箱 2 发送成功*/
#define AMHW_ZLG237_CAN_TSR_TXOK2      ((uint32_t)(1ul << 17))

/**< \brief 邮箱 2 中止发送*/
#define AMHW_ZLG237_CAN_TSR_ABRQ2      ((uint32_t)(1ul << 23))

/**< \brief 发送邮箱 0 为空 */
#define AMHW_ZLG237_CAN_TSR_TME0       ((uint32_t)(1ul << 26))

/**< \brief 发送邮箱 1 为空 */
#define AMHW_ZLG237_CAN_TSR_TME1       ((uint32_t)(1ul << 27))

/**< \brief 发送邮箱 2 为空 */
#define AMHW_ZLG237_CAN_TSR_TME2       ((uint32_t)(1ul << 28))

/**< \brief 发送时间戳 */
#define AMHW_ZLG237_CAN_TDTR_TGT       ((uint32_t)(1ul << 8))

typedef enum {
    ZLG237_CAN_TX_STATUS_FAILED = 0,    /**< \brief 传输失败 */
    ZLG237_CAN_TX_STATUS_OK,            /**< \brief 传输成功 */
    ZLG237_CAN_TX_STATUS_PENGDING,      /**< \brief 待传输 */
    ZLG237_CAN_TX_STATUS_NOMAILBOX,     /**< \brief 未提供空的邮箱 */
}amhw_zlg237_can_tx_status_t;

/**
 * \brief CAN 运行模式设置
 *
 * \param[in] p_hw_can          : 指向CAN寄存器块的指针
 * \param[in] operating_mode    : 指向amhw_zlg237_can_operating_type_t 类型
 *
 * \return  CAN的模式设置状态是否成功  0  1
 */
uint8_t amhw_zlg237_can_operating_mode_request (amhw_zlg237_can_t     *p_hw_can,
                                                amhw_zlg237_can_mode_t mode);

/**
 * \brief CAN 检测相应标志位是否被设置
 *
 * \param[in] p_hw_can        : 指向CAN寄存器块的指针
 * \param[in] can_it          : 指向设置 amhw_zlg237_can_flag_t
 * \param[in] state           : 状态   0  1
 * \return  无
 */
uint8_t amhw_zlg237_can_get_flagstatus(amhw_zlg237_can_t        *p_hw_can,
                                       amhw_zlg237_can_flag_t    can_flag);

/**
 * \brief 获取指定中断的状态
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 *
 * \return 无
 */
uint8_t amhw_zlg237_can_get_itstatus (amhw_zlg237_can_t *p_hw_can,
                                      uint32_t           can_it);

/**
 * \brief  清除指定中断的标志位
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 *
 * \return 无
 */
void amhw_zlg237_can_clear_itstatus (amhw_zlg237_can_t *p_hw_can,
                                      uint32_t           can_it);

/**
 * \brief CAN 调试冻结控制
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] state      : 指向uint8_t
 * \return 无
 */
am_static_inline
void amhw_zlg237_can_dbf_freeze (amhw_zlg237_can_t *p_hw_can,
                                 uint8_t            state)
{
    if (state != CAN_DISABLE) {

        /* 在Debug时， 冻结CAN的接收/发送   仍然可以正常读写和控制接收  FIFO */
        p_hw_can->mcr |= (uint32_t)AMHW_ZLG237_CAN_MCR_DBF;
    } else {

        /* 在调试时，CAN正常工作 */
        p_hw_can->mcr &= ~(uint32_t)AMHW_ZLG237_CAN_MCR_DBF;
    }
}

/**
 * \brief 控制CAN时间触发通信模式
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] state      : 指向uint8_t
 * \return 无
 */
am_static_inline
void amhw_zlg237_can_ttcom_mode_set (amhw_zlg237_can_t *p_hw_can,
                                     uint8_t            state)
{
    if (state != CAN_DISABLE) {

        /* 允许时间触发通信模式 */
        p_hw_can->mcr |= AMHW_ZLG237_CAN_MCR_TTCM;

        /* 设置发送时间戳  */
        p_hw_can->tx_mail[0].tdtr |= AMHW_ZLG237_CAN_TDTR_TGT;
        p_hw_can->tx_mail[1].tdtr |= AMHW_ZLG237_CAN_TDTR_TGT;
        p_hw_can->tx_mail[2].tdtr |= AMHW_ZLG237_CAN_TDTR_TGT;
    } else {

        /* 禁止时间触发通信模式 */
        p_hw_can->mcr &= ~(uint32_t)AMHW_ZLG237_CAN_MCR_TTCM;

        /* 不发送时间戳  */
        p_hw_can->tx_mail[0].tdtr &= ~AMHW_ZLG237_CAN_TDTR_TGT;
        p_hw_can->tx_mail[1].tdtr &= ~AMHW_ZLG237_CAN_TDTR_TGT;
        p_hw_can->tx_mail[2].tdtr &= ~AMHW_ZLG237_CAN_TDTR_TGT;
    }
}

/**
 * \brief 检测传输消息
 *
 * \param[in] p_hw_can         : 指向CAN寄存器块的指针
 * \param[in] transmit_mailbox : 传输所用邮箱的ID
 *
 * \return  当前传输消息邮箱的状态
 */
am_static_inline
uint8_t amhw_zlg237_can_transmit_status (amhw_zlg237_can_t    *p_hw_can,
                                         uint8_t               transmit_mailbox)
{
    uint32_t state = 0;

    /* 检查为几号邮箱 */
    switch (transmit_mailbox) {

    case 0:
        state = p_hw_can->tsr & (AMHW_ZLG237_CAN_TSR_RQCP0 |
                                 AMHW_ZLG237_CAN_TSR_TXOK0 |
                                 AMHW_ZLG237_CAN_TSR_TME0);
        break;

    case 1:
        state = p_hw_can->tsr & (AMHW_ZLG237_CAN_TSR_RQCP1 |
                                 AMHW_ZLG237_CAN_TSR_TXOK1 |
                                 AMHW_ZLG237_CAN_TSR_TME1);
        break;

    case 2:
        state = p_hw_can->tsr & (AMHW_ZLG237_CAN_TSR_RQCP2 |
                                 AMHW_ZLG237_CAN_TSR_TXOK2 |
                                 AMHW_ZLG237_CAN_TSR_TME2);
        break;

    default:
        state = ZLG237_CAN_TX_STATUS_FAILED;
        break;
    }
    /* 检查邮箱 状态*/
    switch (state) {

    case (0x00):
        state = ZLG237_CAN_TX_STATUS_PENGDING;
        break;

    case (AMHW_ZLG237_CAN_TSR_RQCP0 | AMHW_ZLG237_CAN_TSR_TME0):
        state = ZLG237_CAN_TX_STATUS_FAILED;
        break;

    case (AMHW_ZLG237_CAN_TSR_RQCP1 | AMHW_ZLG237_CAN_TSR_TME1):
        state = ZLG237_CAN_TX_STATUS_FAILED;
        break;

    case (AMHW_ZLG237_CAN_TSR_RQCP2 | AMHW_ZLG237_CAN_TSR_TME2):
        state = ZLG237_CAN_TX_STATUS_FAILED;
        break;

    case (AMHW_ZLG237_CAN_TSR_RQCP0 |
          AMHW_ZLG237_CAN_TSR_TME0  |
          AMHW_ZLG237_CAN_TSR_TXOK0):
        state = ZLG237_CAN_TX_STATUS_OK;
        break;

    case (AMHW_ZLG237_CAN_TSR_RQCP1 |
          AMHW_ZLG237_CAN_TSR_TME1  |
          AMHW_ZLG237_CAN_TSR_TXOK1):
        state = ZLG237_CAN_TX_STATUS_OK;
        break;

    case (AMHW_ZLG237_CAN_TSR_RQCP2 |
          AMHW_ZLG237_CAN_TSR_TME2  |
          AMHW_ZLG237_CAN_TSR_TXOK2):
        state = ZLG237_CAN_TX_STATUS_OK;
        break;

    default:
        state = ZLG237_CAN_TX_STATUS_FAILED;
        break;
    }

    return (uint8_t) state;
}

/**
 * \brief 释放指定FIFO
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] rx_message : 指向接收消息结构指针
 *
 * \return  无
 */
am_static_inline
void amhw_zlg237_can_fifo_release (amhw_zlg237_can_t    *p_hw_can,
                                   uint8_t               fifonum)
{
    if (fifonum == 0) {
        p_hw_can->rf_0r |= AMHW_ZLG237_CAN_RF0R_RFOM0;
    } else {
        p_hw_can->rf_1r |= AMHW_ZLG237_CAN_RF1R_RFOM1;
    }
}

/**
 * \brief 返回指定FIFO中存放的报文数目(处于挂号状态的报文)
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] fifonum    : FIFO 编号
 *
 * \return  fifo中的报文数目
 */
am_static_inline
uint8_t amhw_zlg237_can_message_pending (amhw_zlg237_can_t    *p_hw_can,
                                      uint8_t                  fifonum)
{
    uint8_t message_pending = 0;
    if (fifonum == 0) {
        message_pending = (uint8_t)(p_hw_can->rf_0r & (uint32_t)0x03);
    } else if (fifonum == 1 ){
        message_pending = (uint8_t)(p_hw_can->rf_1r & (uint32_t)0x03);
    } else {
        message_pending = 0;
    }

    return message_pending ;
}

/**
 * \brief 进入低功耗模式
 *
 * \param[in] p_hw_can          : 指向CAN寄存器块的指针
 *
 * \return  返回睡眠模式状态 0 1
 */
am_static_inline
uint8_t amhw_zlg237_can_sleep (amhw_zlg237_can_t *p_hw_can)
{
    uint8_t sleepstatus = CAN_DISABLE;

    /* 请求睡眠模式 */
    p_hw_can->mcr = (((p_hw_can->mcr) & (uint32_t)
                                      (~(uint32_t)AMHW_ZLG237_CAN_MCR_INRQ)) |
                       AMHW_ZLG237_CAN_MCR_SLEEP);
    /* 睡眠模式状态 */
    if ( (p_hw_can->msr & (AMHW_ZLG237_CAN_MSR_SLAK |
                           AMHW_ZLG237_CAN_MSR_INAK)) ==
           AMHW_ZLG237_CAN_MSR_SLAK) {
        sleepstatus = CAN_ENABLE;
    }

    return (uint8_t)sleepstatus;
}

/**
 * \brief 唤醒CAN
 *
 * \param[in] p_hw_can          : 指向CAN寄存器块的指针
 *
 * \return  是否成功唤醒  0  1
 */
am_static_inline
uint8_t amhw_zlg237_can_wakeup (amhw_zlg237_can_t *p_hw_can)
{
    uint32_t  wait_slak   = 0x0000ffff;
    uint8_t   wakeupstatus = CAN_DISABLE;

    /* 唤醒请求 */
    p_hw_can->mcr &= ~(uint32_t)AMHW_ZLG237_CAN_MCR_SLEEP;

    /* 睡眠模式状态  */
    while ( (p_hw_can->msr & AMHW_ZLG237_CAN_MSR_SLAK) ==
            AMHW_ZLG237_CAN_MSR_SLAK && (wait_slak != 0x00)) {
        wait_slak--;
    }

    if ((p_hw_can->msr & AMHW_ZLG237_CAN_MSR_SLAK) !=
            AMHW_ZLG237_CAN_MSR_SLAK) {
        /* 成功唤醒  睡眠模式退出 */
        wakeupstatus = CAN_ENABLE;
    }

    return (uint8_t)wakeupstatus;
}

/**
 * \brief 获取上次CAN总线上的错误代码
 *
 * \param[in] p_hw_can          : 指向CAN寄存器块的指针
 *
 * \return  错误码 [2:0]
 */
am_static_inline
uint8_t amhw_zlg237_can_get_lasterrorcode (amhw_zlg237_can_t *p_hw_can)
{
    uint8_t errorcode = 0;

    /* 获取错误码的值 */
    errorcode = (((uint8_t)p_hw_can->esr) & (uint8_t)AMHW_ZLG237_CAN_ESR_LEC);

    /* 返回错误吗 */
    return errorcode ;
}

/**
 * \brief 获取CAN 接受错误计数器的值 REC
 *
 * \param[in] p_hw_can          : 指向CAN寄存器块的指针
 *
 * \return  错误计数值
 */
am_static_inline
uint8_t amhw_zlg237_can_get_recverrorcount (amhw_zlg237_can_t *p_hw_can)
{
    uint8_t counter = 0;

    /* 获取错误计数值 */
    counter = (uint8_t)((p_hw_can->esr &
              (uint32_t)AMHW_ZLG237_CAN_ESR_REC) >> 24);

    /* 返回错误计数值 */
    return counter ;
}

/**
 * \brief 获取CAN 9位发送错误计数器的低8位 TEC
 *
 * \param[in] p_hw_can          : 指向CAN寄存器块的指针
 *
 * \return  错误计数值
 */
am_static_inline
uint8_t amhw_zlg237_can_get_transerrorcount (amhw_zlg237_can_t *p_hw_can)
{
    uint8_t counter = 0;

    /* 获取错误计数值 */
    counter = (uint8_t)((p_hw_can->esr &
              (uint32_t)AMHW_ZLG237_CAN_ESR_TEC) >> 16);

    /* 返回错误计数值 */
    return counter ;
}

/**
 * \brief CAN 中断使能设置
 *
 * \param[in] p_hw_can        : 指向CAN寄存器块的指针
 * \param[in] can_it          : 指向设置的中断源 amhw_zlg237_can_int_t
 * \param[in] state           : 状态   0  1
 * \return  无
 */
am_static_inline
void amhw_zlg237_can_itconfig(amhw_zlg237_can_t        *p_hw_can,
                              amhw_zlg237_can_int_t     can_it,
                              uint8_t                   state)
{
    if (state != CAN_DISABLE) {
        p_hw_can->ier |= (uint32_t)(1ul << can_it);
    } else {
        p_hw_can->ier &= ~(uint32_t)(1ul << can_it);
    }
}

/**
 * \brief CAN 清除挂号状态
 *
 * \param[in] p_hw_can        : 指向CAN寄存器块的指针
 * \param[in] can_it          : 指向设置 amhw_zlg237_can_flag_t
 * \note   不包含 AMHW_ZLG237_CAN_FLAG_FMP
 *
 *
 * \param[in] state           : 状态   0  1
 * \return  无
 */
am_static_inline
void amhw_zlg237_can_clearflag(amhw_zlg237_can_t        *p_hw_can,
                               amhw_zlg237_can_flag_t    can_flag)
{
    uint32_t flagtmp = 0;

    /* ESR register */
    if (can_flag == AMHW_ZLG237_CAN_FLAG_LEC) {
        p_hw_can->esr = (uint32_t)CAN_DISABLE;
    } else {    /* MSR or TSR or RF0R or RF1R */
        flagtmp = can_flag & 0x000fffff;

        if ((can_flag & AMHW_ZLG237_CAN_FLAG_RF0R) != (uint32_t)AM_FALSE) {

            /* 接收 */
            p_hw_can->rf_0r |= (uint32_t)flagtmp;
        } else if ((can_flag & AMHW_ZLG237_CAN_FLAG_RF1R) != (uint32_t)AM_FALSE) {
            /* 接收 */
            p_hw_can->rf_1r |= (uint32_t)flagtmp;
        } else if ((can_flag & AMHW_ZLG237_CAN_FLAG_TSR) != (uint32_t)AM_FALSE) {
            /* 接收 */
            p_hw_can->tsr |= (uint32_t)flagtmp;
        } else { /* msr */
            p_hw_can->msr |= (uint32_t)flagtmp;
        }
    }
}

/**
 * \brief 波特率预设值设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] brp        : 波特率预设值， 10bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_can_brp_set (amhw_zlg237_can_t *p_hw_can, uint16_t brp)
{
    p_hw_can->btr = (p_hw_can->btr & (~0x3ff)) | (brp & 0x3ff);
}

/**
 * \brief 获取波特率预设值
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 波特率预设值，10bit
 */
am_static_inline
uint8_t amhw_zlg237_can_brp_get (amhw_zlg237_can_t *p_hw_can)
{
    return (uint16_t)(p_hw_can->btr & 0x3ff);
}

/**
 * \brief 同步跳转宽度设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] sjw        : 同步跳转宽度值，2bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_can_sjw_set (amhw_zlg237_can_t *p_hw_can, uint8_t sjw)
{
    p_hw_can->btr = (p_hw_can->btr & (~(0x3 << 24))) | ((sjw & 0x3) << 24);
}

/**
 * \brief 获取同步跳转宽度
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 同步跳转宽度，2bit
 */
am_static_inline
uint8_t amhw_zlg237_can_sjw_get (amhw_zlg237_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->btr >> 24) & 0x3);
}

/**
 * \brief 时间段1设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] tseg1        : 采样模式值，4bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_can_tseg1_set (amhw_zlg237_can_t *p_hw_can, uint8_t tseg1)
{
    p_hw_can->btr = (p_hw_can->btr & (~(0xf << 16))) | ((tseg1 & 0xf) << 16);
}

/**
 * \brief 获取时间段1
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 时间段1，4bit
 */
am_static_inline
uint8_t amhw_zlg237_can_tseg1_get (amhw_zlg237_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->btr >> 16) & 0xf);
}

/**
 * \brief 时间段2设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] tseg2        : 采样模式值，3bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_can_tseg2_set (amhw_zlg237_can_t *p_hw_can, uint8_t tseg2)
{
    p_hw_can->btr = (p_hw_can->btr & (~(0x7 << 20))) | ((tseg2 & 0x7) << 20);
}

/**
 * \brief 时间段2设置
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zlg237_can_tseg2_get (amhw_zlg237_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->btr >> 20) & 0x7);
}

/**
 * \brief 获取指定中断的状态
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zlg237_can_check_itstatus (uint32_t  can_reg,
                                        uint32_t  can_it)
{
    uint8_t state = CAN_DISABLE;

    if (can_reg & can_it) {

        state = CAN_ENABLE;

    } else {
        state = CAN_DISABLE;
    }

    return state;
}


/**
 * @} amhw_zlg237_if_can
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG237_CAN_H */

/* end of file */
