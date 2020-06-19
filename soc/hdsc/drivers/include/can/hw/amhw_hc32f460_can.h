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
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-05  zcb, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_CAN_H
#define __AMHW_HC32F460_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_hc32f460_if_can
 * \copydoc amhw_hc32f460_can.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

/* 默认使能匿名联合体 */
#elif defined(__TMS470__)

/* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

typedef struct
{
    __IO uint8_t BUSOFF                     : 1;
    __IO uint8_t TACTIVE                    : 1;
    __IO uint8_t RACTIVE                    : 1;
    __IO uint8_t TSSS                       : 1;
    __IO uint8_t TPSS                       : 1;
    __IO uint8_t LBMI                       : 1;
    __IO uint8_t LBME                       : 1;
    __IO uint8_t RESET                      : 1;
} stc_can_cfg_stat_field_t;

typedef struct
{
    __IO uint8_t TSA                        : 1;
    __IO uint8_t TSALL                      : 1;
    __IO uint8_t TSONE                      : 1;
    __IO uint8_t TPA                        : 1;
    __IO uint8_t TPE                        : 1;
    __IO uint8_t STBY                       : 1;
    __IO uint8_t LOM                        : 1;
    __IO uint8_t TBSEL                      : 1;
} stc_can_tcmd_field_t;

typedef struct
{
    __IO uint8_t TSSTAT                     : 2;
    uint8_t RESERVED2                       : 2;
    __IO uint8_t TTBM                       : 1;
    __IO uint8_t TSMODE                     : 1;
    __IO uint8_t TSNEXT                     : 1;
    uint8_t RESERVED7                       : 1;
} stc_can_tctrl_field_t;

typedef struct
{
    __IO uint8_t RSSTAT                     : 2;
    uint8_t RESERVED2                       : 1;
    __IO uint8_t RBALL                      : 1;
    __IO uint8_t RREL                       : 1;
    __IO uint8_t ROV                        : 1;
    __IO uint8_t ROM                        : 1;
    __IO uint8_t SACK                       : 1;
} stc_can_rctrl_field_t;

typedef struct
{
    __IO uint8_t TSFF                       : 1;
    __IO uint8_t EIE                        : 1;
    __IO uint8_t TSIE                       : 1;
    __IO uint8_t TPIE                       : 1;
    __IO uint8_t RAFIE                      : 1;
    __IO uint8_t RFIE                       : 1;
    __IO uint8_t ROIE                       : 1;
    __IO uint8_t RIE                        : 1;
} stc_can_rtie_field_t;

typedef struct
{
    __IO uint8_t AIF                        : 1;
    __IO uint8_t EIF                        : 1;
    __IO uint8_t TSIF                       : 1;
    __IO uint8_t TPIF                       : 1;
    __IO uint8_t RAFIF                      : 1;
    __IO uint8_t RFIF                       : 1;
    __IO uint8_t ROIF                       : 1;
    __IO uint8_t RIF                        : 1;
} stc_can_rtif_field_t;

typedef struct
{
    __IO uint8_t BEIF                       : 1;
    __IO uint8_t BEIE                       : 1;
    __IO uint8_t ALIF                       : 1;
    __IO uint8_t ALIE                       : 1;
    __IO uint8_t EPIF                       : 1;
    __IO uint8_t EPIE                       : 1;
    __IO uint8_t EPASS                      : 1;
    __IO uint8_t EWARN                      : 1;
} stc_can_errint_field_t;

typedef struct
{
    __IO uint8_t EWL                        : 4;
    __IO uint8_t AFWL                       : 4;
} stc_can_limit_field_t;

typedef struct
{
    __IO uint32_t SEG_1                     : 8;
    __IO uint32_t SEG_2                     : 7;
    uint32_t RESERVED15                     : 1;
    __IO uint32_t SJW                       : 7;
    uint32_t RESERVED23                     : 1;
    __IO uint32_t PRESC                     : 8;
} stc_can_bt_field_t;

typedef struct
{
    __IO uint8_t ALC                        : 5;
    __IO uint8_t KOER                       : 3;
} stc_can_ealcap_field_t;

typedef struct
{
    __IO uint8_t ACFADR                     : 4;
    uint8_t RESERVED4                       : 1;
    __IO uint8_t SELMASK                    : 1;
    uint8_t RESERVED6                       : 2;
} stc_can_acfctrl_field_t;

typedef struct
{
    __IO uint8_t AE_1                       : 1;
    __IO uint8_t AE_2                       : 1;
    __IO uint8_t AE_3                       : 1;
    __IO uint8_t AE_4                       : 1;
    __IO uint8_t AE_5                       : 1;
    __IO uint8_t AE_6                       : 1;
    __IO uint8_t AE_7                       : 1;
    __IO uint8_t AE_8                       : 1;
} stc_can_acfen_field_t;

typedef struct
{
    __IO uint32_t ACODEORAMASK              :29;
    __IO uint32_t AIDE                      : 1;
    __IO uint32_t AIDEE                     : 1;
    uint32_t RESERVED31                     : 1;
} stc_can_acf_field_t;

typedef struct
{
    __IO uint8_t TBPTR                      : 6;
    __IO uint8_t TBF                        : 1;
    __IO uint8_t TBE                        : 1;
} stc_can_tbslot_field_t;

typedef struct
{
    __IO uint8_t TTEN                       : 1;
    __IO uint8_t T_PRESC                    : 2;
    __IO uint8_t TTIF                       : 1;
    __IO uint8_t TTIE                       : 1;
    __IO uint8_t TEIF                       : 1;
    __IO uint8_t WTIF                       : 1;
    __IO uint8_t WTIE                       : 1;
} stc_can_ttcfg_field_t;

typedef struct
{
    __IO uint32_t REF_ID                    :29;
    uint32_t RESERVED29                     : 2;
    __IO uint32_t REF_IDE                   : 1;
} stc_can_ref_msg_field_t;

typedef struct
{
    __IO uint16_t TTPTR                     : 6;
    uint16_t RESERVED6                      : 2;
    __IO uint16_t TTYPE                     : 3;
    uint16_t RESERVED11                     : 1;
    __IO uint16_t TEW                       : 4;
} stc_can_trg_cfg_field_t;

/**
 * \brief HC32F460 CAN寄存器块结构体
 */
typedef struct amhw_hc32f460_can {
    __IO uint32_t RBUF0;
    __IO uint32_t RBUF1;
    __IO uint32_t RBUF2;
    __IO uint32_t RBUF3;
    uint8_t RESERVED4[64];
    __IO uint32_t TBUF0;
    __IO uint32_t TBUF1;
    __IO uint32_t TBUF2;
    __IO uint32_t TBUF3;
    uint8_t RESERVED8[64];
    union
    {
        __IO uint8_t CFG_STAT;
        stc_can_cfg_stat_field_t CFG_STAT_f;
    };
    union
    {
        __IO uint8_t TCMD;
        stc_can_tcmd_field_t TCMD_f;
    };
    union
    {
        __IO uint8_t TCTRL;
        stc_can_tctrl_field_t TCTRL_f;
    };
    union
    {
        __IO uint8_t RCTRL;
        stc_can_rctrl_field_t RCTRL_f;
    };
    union
    {
        __IO uint8_t RTIE;
        stc_can_rtie_field_t RTIE_f;
    };
    union
    {
        __IO uint8_t RTIF;
        stc_can_rtif_field_t RTIF_f;
    };
    union
    {
        __IO uint8_t ERRINT;
        stc_can_errint_field_t ERRINT_f;
    };
    union
    {
        __IO uint8_t LIMIT;
        stc_can_limit_field_t LIMIT_f;
    };
    union
    {
        __IO uint32_t BT;
        stc_can_bt_field_t BT_f;
    };
    uint8_t RESERVED17[4];
    union
    {
        __IO uint8_t EALCAP;
        stc_can_ealcap_field_t EALCAP_f;
    };
    uint8_t RESERVED18[1];
    __IO uint8_t RECNT;
    __IO uint8_t TECNT;
    union
    {
        __IO uint8_t ACFCTRL;
        stc_can_acfctrl_field_t ACFCTRL_f;
    };
    uint8_t RESERVED21[1];
    union
    {
        __IO uint8_t ACFEN;
        stc_can_acfen_field_t ACFEN_f;
    };
    uint8_t RESERVED22[1];
    union
    {
        __IO uint32_t ACF;
        stc_can_acf_field_t ACF_f;
    };
    uint8_t RESERVED23[2];
    union
    {
        __IO uint8_t TBSLOT;
        stc_can_tbslot_field_t TBSLOT_f;
    };
    union
    {
        __IO uint8_t TTCFG;
        stc_can_ttcfg_field_t TTCFG_f;
    };
    union
    {
        __IO uint32_t REF_MSG;
        stc_can_ref_msg_field_t REF_MSG_f;
    };
    union
    {
        __IO uint16_t TRG_CFG;
        stc_can_trg_cfg_field_t TRG_CFG_f;
    };
    __IO uint16_t TT_TRIG;
    __IO uint16_t TT_WTRIG;
}amhw_hc32f460_can_t;

/**
 * \brief CAN模式定义
 */
typedef enum amhw_hc32f460_can_type {
    AMHW_HC32F460_CAN_BASIC_CAN = 0,        /**< \brief basic can */
    AMHW_HC32F460_CAN_PELI_CAN,             /**< \brief peli can */
}amhw_hc32f460_can_type_t;

/**
 * \brief CAN功能定义
 *
 * \note basic can只知有复位模式、工作模式、睡眠模式
 *
 */
typedef enum amhw_hc32f460_can_mode_func {
    AMHW_HC32F460_CAN_MODE_RESET = 0,       /**< \brief 复位模式 */
    AMHW_HC32F460_CAN_MODE_ONLY_LISTEN,     /**< \brief 只听模式 */
    AMHW_HC32F460_CAN_MODE_SELF_CHECK,      /**< \brief 自检模式 */
    AMHW_HC32F460_CAN_MODE_SLEEP = 4,       /**< \brief 睡眠模式 */
    AMHW_HC32F460_CAN_MODE_RUN,             /**< \brief 工作模式 */
}amhw_hc32f460_can_mode_t;

/**
 * \brief CAN数据接收方式
 */
typedef enum amhw_hc32f460_can_recv_mode {
    AMHW_HC32F460_CAN_RECV_POLL = 0,        /**< \brief 查询接收模式 */
    AMHW_HC32F460_CAN_RECV_INT,             /**< \brief 中断接收模式 */
}amhw_hc32f460_can_recv_mode_t;

/**
 * \brief peli CAN滤波方式
 */
typedef enum amhw_hc32f460_can_peli_filter_mode {
    AMHW_HC32F460_CAN_PELI_SINGLE_FILTER,   /**< \brief peli can 单滤波器模式 */
    AMHW_HC32F460_CAN_PELI_DOUBLE_FLITER,   /**< \brief peli can 双滤波器模式 */
}amhw_hc32f460_can_peli_filter_mode_t;

/******************************************************************************/

/**
 * \brief CAN中断
 *
 * \note 
 *
 * @{
 */

#define AMHW_HC32F460_CAN_INT_RECV             (0x1 << 7ul)  /**< \brief 接收中断 */
#define AMHW_HC32F460_CAN_INT_OVER             (0x1 << 6ul)  /**< \brief 数据溢出中断 */
#define AMHW_HC32F460_CAN_INT_PTB_TRAN         (0x1 << 3ul)  /**< \brief PTB发送中断 */
#define AMHW_HC32F460_CAN_INT_STB_TRAN         (0x1 << 2ul)  /**< \brief STB发送中断 */
#define AMHW_HC32F460_CAN_INT_ERR              (0x1 << 1ul)  /**< \brief 错误中断 */


typedef enum
{
    CanExternalLoopBackMode  = 0x40,        ///< Loop back mode, external
    CanInternalLoopBackMode  = 0x20,        ///< Loop back mode, internal
    CanTxSignalPrimaryMode   = 0x10,        ///< Transmission primary single shot mode for PTB
    CanTxSignalSecondaryMode = 0x08,        ///< Transmission secondary single shot mode for STB
    CanListenOnlyMode        = 0xFF,        ///< Listen only mode
}en_can_mode_t;

typedef enum en_can_functional_state
{
    CanDisable = 0u,
    CanEnable  = 1u,
} en_can_functional_state_t;

typedef enum
{
    CanRxNormal = 0,        ///< Normal operation
    CanRxAll    = 1,        ///< RB stores correct data frames as well as data frames with error
}en_can_rx_buf_all_t;

typedef enum
{
    CanRxBufOverwritten = 0,        ///< The oldest message will be overwritten
    CanRxBufNotStored   = 1,        ///< The new message will not be stored
}en_can_rx_buf_mode_en_t;

typedef enum
{
    CanSelfAckEnable  = 1,        ///< Self-ACK when LBME=1
    CanSelfAckDisable = 0,        ///< no self-ACK
}en_can_self_ack_en_t;

typedef enum
{
    CanSTBFifoMode    = 0,        ///< FIFO mode
    CanSTBPrimaryMode = 1,        ///< Priority decision mode
}en_can_stb_mode_t;

typedef struct stc_can_bt
{
    uint8_t SEG_1;      ///< Bit timing segment 1(Tseg_1 = (SEG_1 + 2)*TQ)
    uint8_t SEG_2;      ///< Bit timing segment 2(Tseg_2 = (SEG_2 + 1)*TQ)
    uint8_t SJW;        ///< Synchronization jump width(Tsjw = (SJW + 1)*TQ)
    uint8_t PRESC;      ///< The Prescaler divides the system clock to get the time quanta clock tq_clk(TQ)
}stc_can_bt_t;

typedef struct stc_can_warning_limit
{
    uint8_t CanWarningLimitVal;         ///< Receive buffer almost full warning limit
    uint8_t CanErrorWarningLimitVal;    ///< Programmable error warning limit
}stc_can_warning_limit_t;

typedef struct stc_can_init_config
{
    en_can_rx_buf_all_t     enCanRxBufAll;      ///< @ref en_can_rx_buf_all_t
    en_can_rx_buf_mode_en_t enCanRxBufMode;     ///< @ref en_can_rx_buf_mode_en_t
    en_can_self_ack_en_t    enCanSAck;          ///< @ref en_can_self_ack_en_t
    en_can_stb_mode_t       enCanSTBMode;       ///< @ref en_can_stb_mode_t
    stc_can_bt_t            stcCanBt;           ///< @ref stc_can_bt_t
    stc_can_warning_limit_t stcWarningLimit;    ///< @ref stc_can_warning_limit_t
}stc_can_init_config_t;

typedef struct
{
    uint32_t DLC                     : 4;        ///< Data length code
    uint32_t RESERVED0               : 2;        ///< Ignore
    uint32_t RTR                     : 1;        ///< Remote transmission request
    uint32_t IDE                     : 1;        ///< IDentifier extension
    uint32_t RESERVED1               : 24;       ///< Ignore
}stc_can_txcontrol_t;

typedef enum
{
    CanPTBSel       = 0U,        ///< high-priority buffer
    CanSTBSel       = 1U,        ///< secondary buffer
}en_can_buffer_sel_t;

typedef struct stc_can_txframe
{
    union
    {
        uint32_t TBUF32_0;                  ///< Ignore
        uint32_t StdID;                     ///< Standard ID
        uint32_t ExtID;                     ///< Extended ID
    };
    union
    {
        uint32_t TBUF32_1;                  ///< Ignore
        stc_can_txcontrol_t Control_f;      ///< CAN Tx Control
    };
    union
    {
        uint32_t TBUF32_2[2];               ///< Ignore
        uint8_t  Data[8];                   ///< CAN data
    };
    en_can_buffer_sel_t     enBufferSel;    ///< CAN Tx buffer select

}stc_can_txframe_t;

typedef enum
{
    CanTxBufEmpty        = 0,       ///< TTEN=0 or  TTTBM=0: STB is empty
                                    ///< TTEN=1 and TTTBM=1: PTB and STB are empty
    CanTxBufnotHalfFull  = 1,       ///< TTEN=0 or  TTTBM=0: STB is less than or equal to half full
                                    ///< TTEN=1 and TTTBM=1: PTB and STB are not empty and not full
    CanTxBufHalfFull     = 2,       ///< TTEN=0 or  TTTBM=0: STB is more than half full
                                    ///< TTEN=1 and TTTBM=1: None
    CanTxBufFull         = 3,       ///< TTEN=0 or  TTTBM=0: STB is full
                                    ///< TTEN=1 and TTTBM=1: PTB and STB are full
}en_can_tx_buf_status_t;

typedef enum
{
    CanPTBTxCmd      = 0x10,        ///< Transmit primary for PTB
    CanPTBTxAbortCmd = 0x08,        ///< Transmit primary abort for PTB
    CanSTBTxOneCmd   = 0x04,        ///< Transmit secondary one frame for STB
    CanSTBTxAllCmd   = 0x02,        ///< Transmit secondary all frames for STB
    CanSTBTxAbortCmd = 0x01,        ///< Transmit secondary abort for STB
}en_can_tx_cmd_t;

typedef enum
{
    CanRxBufEmpty          = 0,        ///< Empty
    CanRxBufnotAlmostFull  = 1,        ///< >empty and <almost full
    CanRxBufAlmostFull     = 2,        ///< >=almost full, but not full and no overflow
    CanRxBufFull           = 3,        ///< full
}en_can_rx_buf_status_t;

typedef struct
{
    uint8_t DLC          : 4;       ///< Data length code
    uint8_t RESERVED0    : 2;       ///< Ignore
    uint8_t RTR          : 1;       ///< Remote transmission request
    uint8_t IDE          : 1;       ///< IDentifier extension
}stc_can_rxcontrol_t;

typedef struct
{
    uint8_t RESERVED0    : 4;       ///< Ignore
    uint8_t TX           : 1;       ///< TX is set to 1 if the loop back mode is activated
    uint8_t KOER         : 3;       ///< Kind of error
}stc_can_status_t;

typedef struct
{
    stc_can_rxcontrol_t Control_f;      ///< @ref stc_can_rxcontrol_t
    stc_can_status_t    Status_f;       ///< @ref stc_can_status_t
    uint16_t            CycleTime;      ///< TTCAN cycletime
}stc_can_cst_t;

typedef struct stc_can_rxframe
{
    union
    {
        uint32_t RBUF32_0;              ///< Ignore
        uint32_t StdID;                 ///< Standard ID
        uint32_t ExtID;                 ///< Extended ID
    };
    union
    {
        uint32_t        RBUF32_1;       ///< Ignore
        stc_can_cst_t   Cst;            ///< @ref stc_can_cst_t
    };
    union
    {
        uint32_t RBUF32_2[2];           ///< Ignore
        uint8_t  Data[8];               ///< CAN data
    };

}stc_can_rxframe_t;


typedef enum
{
    //<<Can Tx or Tx Irq Flg
    CanTxBufFullIrqFlg          = 0x00000001,   ///<
    CanRxIrqFlg                 = 0x00008000,   ///< Receive interrupt flag
    CanRxOverIrqFlg             = 0x00004000,   ///< RB overrun interrupt flag
    CanRxBufFullIrqFlg          = 0x00002000,   ///< RB full interrupt flag
    CanRxBufAlmostFullIrqFlg    = 0x00001000,   ///< RB almost full interrupt flag
    CanTxPrimaryIrqFlg          = 0x00000800,   ///< Transmission primary interrupt flag
    CanTxSecondaryIrqFlg        = 0x00000400,   ///< Transmission secondary interrupt flag
    CanErrorIrqFlg              = 0x00000200,   ///< Error interrupt flag
    CanAbortIrqFlg              = 0x00000100,   ///< Abort interrupt flag

    //<< Can Error Irq Flg
    CanErrorWarningIrqFlg       = 0x00800000,   ///< Error warning limit reached flag
    CanErrorPassivenodeIrqFlg   = 0x00400000,   ///< Error passive mode active flag
    CanErrorPassiveIrqFlg       = 0x00100000,   ///< Error passive interrupt flag
    CanArbiLostIrqFlg           = 0x00040000,   ///< Arbitration lost interrupt flag
    CanBusErrorIrqFlg           = 0x00010000,   ///< Bus error interrupt flag
}en_can_irq_flag_type_t;

typedef enum
{
    //<<Can Rx or Tx Irq En
    CanRxIrqEn              = 0x00000080,   ///< Receive interrupt enable
    CanRxOverIrqEn          = 0x00000040,   ///< RB overrun interrupt enable
    CanRxBufFullIrqEn       = 0x00000020,   ///< RB full interrupt enable
    CanRxBufAlmostFullIrqEn = 0x00000010,   ///< RB almost full interrupt enable
    CanTxPrimaryIrqEn       = 0x00000008,   ///< Transmission primary interrupt enable
    CanTxSecondaryIrqEn     = 0x00000004,   ///< Transmission secondary enable
    CanErrorIrqEn           = 0x00000002,   ///< Error interrupt enable

    //<<Can Error Irq En
    CanErrorPassiveIrqEn    = 0x00200000,   ///< Error passive mode active enable
    CanArbiLostIrqEn        = 0x00080000,   ///< Arbitration lost interrupt enable
    CanBusErrorIrqEn        = 0x00020000,   ///< Bus error interrupt enable

}en_can_irq_type_t;

typedef enum en_can_filter_sel
{
    CanFilterSel1        = 0u,   ///< The Acceptance Filter 1 Enable
    CanFilterSel2        = 1u,   ///< The Acceptance Filter 2 Enable
    CanFilterSel3        = 2u,   ///< The Acceptance Filter 3 Enable
    CanFilterSel4        = 3u,   ///< The Acceptance Filter 4 Enable
    CanFilterSel5        = 4u,   ///< The Acceptance Filter 5 Enable
    CanFilterSel6        = 5u,   ///< The Acceptance Filter 6 Enable
    CanFilterSel7        = 6u,   ///< The Acceptance Filter 7 Enable
    CanFilterSel8        = 7u,   ///< The Acceptance Filter 8 Enable
}en_can_filter_sel_t;

typedef enum en_can_acf_format_en
{
    CanStdFrames        = 0x02u,   ///< Accepts only Standard frames
    CanExtFrames        = 0x03u,   ///< Accepts only Extended frames
    CanAllFrames        = 0x00u,   ///< Accepts both standard or extended frames
}en_can_acf_format_en_t;

typedef struct stc_can_filter
{
    uint32_t                u32CODE;        ///< Acceptance CODE
    uint32_t                u32MASK;        ///< Acceptance MASK
    en_can_filter_sel_t     enFilterSel;    ///< The Acceptance Filters Enable
    en_can_acf_format_en_t  enAcfFormat;    ///< The Acceptance Filters Frame Format Check.
}stc_can_filter_t;

typedef enum
{
    NO_ERROR        = 0U,
    BIT_ERROR       = 1U,
    FORM_ERROR      = 2U,
    STUFF_ERROR     = 3U,
    ACK_ERROR       = 4U,
    CRC_ERROR       = 5U,
    UNKOWN_ERROR    = 6U,
}en_can_error_t;

typedef enum
{
    CanRxActive = 0x04,        ///< Reception active
    CanTxActive = 0x02,        ///< Transmission active
    CanBusoff   = 0x01,        ///< Bus off
}en_can_status_t;

am_static_inline
void amhw_hc32f460_can_mode_config(amhw_hc32f460_can_t *p_hw_can, en_can_mode_t enMode, en_can_functional_state_t enNewState)
{
    if(CanListenOnlyMode == enMode)
    {
        p_hw_can->TCMD_f.LOM = enNewState;
    }else
    {
        if(CanEnable == enNewState)
        {
            p_hw_can->CFG_STAT |= enMode;
        }else
        {
            p_hw_can->CFG_STAT &= ~enMode;
        }
    }
}

#define CAN_RESET_ENABLE(p_hw_can)       (p_hw_can->CFG_STAT_f.RESET = 1u)
#define CAN_RESET_DISABLE(p_hw_can)                                                     \
do{                                                                             \
    do{                                                                         \
        p_hw_can->CFG_STAT_f.RESET = 0u;                                          \
    }while(p_hw_can->CFG_STAT_f.RESET);                                               \
}while(0)

am_static_inline
void amhw_hc32f460_can_lowlevel_set(amhw_hc32f460_can_t *p_hw_can, stc_can_init_config_t *pstcCanInitCfg)
{
    if (NULL != pstcCanInitCfg)
    {
        p_hw_can->RCTRL_f.SACK   = pstcCanInitCfg->enCanSAck;
        p_hw_can->TCTRL_f.TSMODE = pstcCanInitCfg->enCanSTBMode;
        p_hw_can->RCTRL_f.RBALL  = pstcCanInitCfg->enCanRxBufAll;
        p_hw_can->RCTRL_f.ROM    = pstcCanInitCfg->enCanRxBufMode;

        p_hw_can->RTIE = 0x00u;

        CAN_RESET_ENABLE(p_hw_can);

        p_hw_can->BT_f.PRESC = pstcCanInitCfg->stcCanBt.PRESC;
        p_hw_can->BT_f.SEG_1 = pstcCanInitCfg->stcCanBt.SEG_1;
        p_hw_can->BT_f.SEG_2 = pstcCanInitCfg->stcCanBt.SEG_2;
        p_hw_can->BT_f.SJW   = pstcCanInitCfg->stcCanBt.SJW;

        p_hw_can->LIMIT_f.AFWL = pstcCanInitCfg->stcWarningLimit.CanWarningLimitVal;
        p_hw_can->LIMIT_f.EWL  = pstcCanInitCfg->stcWarningLimit.CanErrorWarningLimitVal;

        CAN_RESET_DISABLE(p_hw_can);
    }
}

am_static_inline
void amhw_hc32f460_can_lowlevel_get(amhw_hc32f460_can_t *p_hw_can, stc_can_init_config_t *pstcCanInitCfg)
{
    if (NULL != pstcCanInitCfg)
    {
        pstcCanInitCfg->enCanSAck      = (en_can_self_ack_en_t)p_hw_can->RCTRL_f.SACK;
        pstcCanInitCfg->enCanSTBMode   = (en_can_stb_mode_t)p_hw_can->TCTRL_f.TSMODE;
        pstcCanInitCfg->enCanRxBufAll  = (en_can_rx_buf_all_t)p_hw_can->RCTRL_f.RBALL;
        pstcCanInitCfg->enCanRxBufMode = (en_can_rx_buf_mode_en_t)p_hw_can->RCTRL_f.ROM;

        p_hw_can->RTIE = 0x00u;

        CAN_RESET_ENABLE(p_hw_can);

        pstcCanInitCfg->stcCanBt.PRESC = p_hw_can->BT_f.PRESC;
        pstcCanInitCfg->stcCanBt.SEG_1 = p_hw_can->BT_f.SEG_1;
        pstcCanInitCfg->stcCanBt.SEG_2 = p_hw_can->BT_f.SEG_2;
        pstcCanInitCfg->stcCanBt.SJW   = p_hw_can->BT_f.SJW;

        pstcCanInitCfg->stcWarningLimit.CanWarningLimitVal       = p_hw_can->LIMIT_f.AFWL;
        pstcCanInitCfg->stcWarningLimit.CanErrorWarningLimitVal  = p_hw_can->LIMIT_f.EWL;

        CAN_RESET_DISABLE(p_hw_can);
    }
}


am_static_inline
uint8_t amhw_hc32f460_can_rx_error_cnt_get(amhw_hc32f460_can_t *p_hw_can)
{
    return p_hw_can->RECNT;
}

am_static_inline
void amhw_hc32f460_can_rx_error_cnt_clr(amhw_hc32f460_can_t *p_hw_can)
{
    p_hw_can->RECNT = 0;
}

am_static_inline
uint8_t amhw_hc32f460_can_tx_error_cnt_get(amhw_hc32f460_can_t *p_hw_can)
{
    return p_hw_can->TECNT;
}

am_static_inline
void amhw_hc32f460_can_tx_error_cnt_clr(amhw_hc32f460_can_t *p_hw_can)
{
    p_hw_can->TECNT = 0;
}

am_static_inline
void amhw_hc32f460_can_set_frame(amhw_hc32f460_can_t *p_hw_can, stc_can_txframe_t *pstcTxFrame)
{
    if(NULL != pstcTxFrame)
    {
        p_hw_can->TCMD_f.TBSEL = pstcTxFrame->enBufferSel;
        p_hw_can->TBUF0 = pstcTxFrame->TBUF32_0;
        p_hw_can->TBUF1 = pstcTxFrame->TBUF32_1;
        p_hw_can->TBUF2 = pstcTxFrame->TBUF32_2[0];
        p_hw_can->TBUF3 = pstcTxFrame->TBUF32_2[1];

        if(CanSTBSel == pstcTxFrame->enBufferSel)
        {
            p_hw_can->TCTRL_f.TSNEXT = CanEnable;
        }
    }
}

am_static_inline
en_can_tx_buf_status_t amhw_hc32f460_can_transmit_cmd(amhw_hc32f460_can_t *p_hw_can, en_can_tx_cmd_t enTxCmd)
{
    p_hw_can->TCMD |= enTxCmd;

    return (en_can_tx_buf_status_t)p_hw_can->TCTRL_f.TSSTAT;

}

am_static_inline
en_can_rx_buf_status_t amhw_hc32f460_can_receive(amhw_hc32f460_can_t *p_hw_can, stc_can_rxframe_t *pstcRxFrame)
{
    if(NULL != pstcRxFrame)
    {
        pstcRxFrame->RBUF32_0    = p_hw_can->RBUF0;
        pstcRxFrame->RBUF32_1    = p_hw_can->RBUF1;
        pstcRxFrame->RBUF32_2[0] = p_hw_can->RBUF2;
        pstcRxFrame->RBUF32_2[1] = p_hw_can->RBUF3;

        p_hw_can->RCTRL_f.RREL = 1u;
    }
    return (en_can_rx_buf_status_t)p_hw_can->RCTRL_f.RSSTAT;
}

am_static_inline
am_bool_t amhw_hc32f460_can_irq_flag_get(amhw_hc32f460_can_t *p_hw_can, en_can_irq_flag_type_t enCanIrqFlgType)
{
    volatile uint32_t *u32pIE = NULL;
    am_bool_t bRet = AM_FALSE;

    u32pIE = (volatile uint32_t*)(&p_hw_can->RTIE);
    if( *u32pIE & enCanIrqFlgType)
    {
        bRet = AM_TRUE;
    }
    return bRet;
}

am_static_inline
uint8_t amhw_hc32f460_can_int_status_get(amhw_hc32f460_can_t *p_hw_can)
{    
   return p_hw_can->RTIF;
}

am_static_inline
uint8_t  amhw_hc32f460_can_status_get(amhw_hc32f460_can_t *p_hw_can, en_can_status_t enCanStatus)
{
    am_bool_t bRet = AM_FALSE;

    if(p_hw_can->CFG_STAT & enCanStatus)
    {
        bRet = AM_TRUE;
    }
    return bRet;
}


am_static_inline
void amhw_hc32f460_can_irq_flag_clr(amhw_hc32f460_can_t *p_hw_can, en_can_irq_flag_type_t enCanIrqFlgType)
{
    volatile uint32_t *u32pIE = NULL;

    u32pIE = (volatile uint32_t*)(&p_hw_can->RTIE);

    *u32pIE |= enCanIrqFlgType;
}

am_static_inline
void amhw_hc32f460_can_irq_cmd(amhw_hc32f460_can_t *p_hw_can, en_can_irq_type_t enCanIrqType, en_can_functional_state_t enNewState)
{
    volatile uint32_t *u32pIE;

    u32pIE = (volatile uint32_t*)(&p_hw_can->RTIE);

    if(CanEnable == enNewState)
    {
        *u32pIE |= enCanIrqType;
    }else
    {
        *u32pIE &= ~((uint32_t)enCanIrqType);
    }

}

#define CAN_ACF_ID_REG_SEL                  ((uint8_t)0x00u)
#define CAN_ACF_MASK_REG_SEL                ((uint8_t)0x01u)

am_static_inline
void amhw_hc32f460_can_filter_config(amhw_hc32f460_can_t *p_hw_can, const stc_can_filter_t *pstcFilter, en_can_functional_state_t enNewState)
{
    if(NULL != pstcFilter)
    {
        CAN_RESET_ENABLE(p_hw_can);

        //<<Acceptance filter address
        p_hw_can->ACFCTRL_f.ACFADR  = pstcFilter->enFilterSel;

        //<<ID config
        p_hw_can->ACFCTRL_f.SELMASK = CAN_ACF_ID_REG_SEL;
        p_hw_can->ACF               = pstcFilter->u32CODE;

        //<<MASK config
        p_hw_can->ACFCTRL_f.SELMASK = CAN_ACF_MASK_REG_SEL;
        p_hw_can->ACF               = pstcFilter->u32MASK;

        //<<Frame format config
        p_hw_can->ACF_f.AIDEE = ((pstcFilter->enAcfFormat >> 1ul) & 0x01u);
        p_hw_can->ACF_f.AIDE  = (pstcFilter->enAcfFormat & 0x01ul);

        if(CanEnable == enNewState)
        {
            p_hw_can->ACFEN |= (uint8_t)(1ul << pstcFilter->enFilterSel);
        }else
        {
            p_hw_can->ACFEN &= (uint8_t)(~(1ul << (pstcFilter->enFilterSel)));
        }

        CAN_RESET_DISABLE(p_hw_can);
    }
}

am_static_inline
en_can_error_t amhw_hc32f460_can_error_status_get(amhw_hc32f460_can_t *p_hw_can)
{
    en_can_error_t enRet = UNKOWN_ERROR;

    if(6u > p_hw_can->EALCAP_f.KOER)
    {
        enRet = (en_can_error_t)p_hw_can->EALCAP_f.KOER;
    }
    return enRet;

}

/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

/* 允许匿名联合体使能 */
#elif defined(__GNUC__)

/* 默认使用匿名联合体 */
#elif defined(__TMS470__)

/* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

/**
 * @} amhw_hc32f460_if_can
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_CAN_H */

/* end of file */
