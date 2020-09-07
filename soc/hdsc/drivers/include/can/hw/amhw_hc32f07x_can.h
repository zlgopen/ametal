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
 * \brief CAN 硬件操作接口
 */

#ifndef __AMHW_HC32F07X_CAN_H
#define __AMHW_HC32F07X_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#pragma anon_unions
/**
 * \addtogroup amhw_hc32f07x_if_can
 * \copydoc amhw_hc32f07x_can.h
 * @{
 */

/**
 * \brief can structure of register
 */
/**
 * \brief can structure of register
 */
/**< \brief 接收buf控制位 */	
typedef struct amhw_hc32f07x_can_cst {
    uint8_t dlc          : 4;    /**< \brief 接收数据长度 */   
    uint8_t reserved0    : 2;    /**< \brief 保留 */   
    uint8_t rtr          : 1;    /**< \brief 帧类型 */   
    uint8_t ide          : 1;    /**< \brief 帧格式 */   
    uint8_t reserved1    : 4;    /**< \brief 保留 */  
    uint8_t tx           : 1;    /**< \brief 回环模式自接收标志 */   
    uint8_t koer         : 3;    /**< \brief 错误类别 */   
    uint16_t    cycletime;          
}amhw_hc32f07x_can_cst_t;

/**< \brief 发送buf控制位 */
typedef struct amhw_hc32f07x_can_txctrl {
    uint32_t dlc          : 4;        /**< \brief 发送数据长度 */
    uint32_t reserved0    : 2;        /**< \brief 保留 */
    uint32_t rtr          : 1;        /**< \brief 帧类型 */
    uint32_t ide          : 1;        /**< \brief 帧格式 */
    uint32_t reserved1    : 24;       /**< \brief 保留 */
}amhw_hc32f07x_can_txctrl_t;

/**< \brief 发送buf */
typedef struct amhw_hc32f07x_can_tbuf {
    __IO uint32_t tbuf0;

    union{
        __IO uint32_t tbuf1;
        __IO  amhw_hc32f07x_can_txctrl_t    txctrl;
    };

    __IO uint32_t tbuf2;
    __IO uint32_t tbuf3;
}amhw_hc32f07x_can_tbuf_t;

/**< \brief 接收buf */
typedef struct amhw_hc32f07x_can_rbuf {
    __IO uint32_t rbuf0;

    union{
        __IO uint32_t rbuf1;
        __IO amhw_hc32f07x_can_cst_t cst;
    };

    __IO uint32_t rbuf2;
    __IO uint32_t rbuf3;
}amhw_hc32f07x_can_rbuf_t;

/**
 * \brief HC32F07X CAN寄存器结构体
 */	
typedef struct amhw_hc32f07x_can {

    __IO amhw_hc32f07x_can_rbuf_t    rbuf;
    __IO uint8_t   reserve0[64];           /**< \brief 保留 */
    __IO amhw_hc32f07x_can_tbuf_t    tbuf;
    __IO uint8_t   reserve1[64];           /**< \brief 保留 */
    __IO uint8_t   cfgstat;    /**< \brief 配置和状态寄存器 */ 
    __IO uint8_t   tcmd;       /**< \brief 命令寄存器 */
    __IO uint8_t   tctrl;      /**< \brief 发送控制寄存器 */
    __IO uint8_t   rctrl;      /**< \brief 接收控制寄存器 */
    __IO uint8_t   rtie;       /**< \brief 接收和发送中断使能寄存器 */
    __IO uint8_t   rtif;       /**< \brief 接收和发送中断标志寄存器 */
    __IO uint8_t   errint;     /**< \brief 错误中断使能和标志寄存器 */
    __IO uint8_t   limit;      /**< \brief 警告限定寄存器*/
    __IO uint32_t  bt;         /**< \brief 位时序寄存器 */
    __IO uint8_t  reserve2[4]; /**< \brief 保留 */	
    __IO uint8_t  ealcap;      /**< \brief 错误和仲裁失败捕捉寄存器 */	
    __IO uint8_t  reserve3[1]; /**< \brief 保留 */
    __IO uint8_t  recnt;       /**< \brief 接收错误计数寄存器 */
    __IO uint8_t  tecnt;       /**< \brief 发送错误计数器寄存器 */	
    __IO uint8_t  acfctrl;     /**< \brief 筛选器组控制寄存器*/
    __IO uint8_t  reserve4[1]; /**< \brief 保留 */
    __IO uint8_t  acfen;       /**< \brief 筛选器组使能寄存器 */	
    __IO uint8_t  reserve5[1]; /**< \brief 保留 */	
    __IO uint32_t acf;         /**< \brief 筛选器组code和mask寄存器 */	
    __IO uint8_t  reserve6[2]; /**< \brief 保留 */
    __IO uint8_t  tbslot;      /**< \brief tb slot指针寄存器 */
    __IO uint8_t  ttcfg;       /**< \brief 时间触发配置寄存器 */	
    __IO uint32_t  refmsg;     /**< \brief 参考消息寄存器 */	
    __IO uint16_t  trgcfg;     /**< \brief 触发配置寄存器 */	
    __IO uint16_t  trgtrig;    /**< \brief 触发时间寄存器 */
    __IO uint16_t  trgwtrig;   /**< \brief 触发看门时间寄存器 */			
} amhw_hc32f07x_can_t;

/**
 * \brief CAN IDE格式定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_IDE_STANDARD = 0, /* 标准格式 */
    AMHW_HC32F07X_CAN_IDE_EXTENDED = 1, /* 扩展格式 */
}amhw_hc32f07x_can_ide_t;

/**
 * \brief CAN RTR帧定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_RTR_DATA   = 0, /* 数据帧 */
    AMHW_HC32F07X_CAN_RTR_REMOTE = 1, /* 远程帧 */
}amhw_hc32f07x_can_rtr_t;

/**
 * \brief CAN 发送BUF选择定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_TXBUF_PTB = 0, /* PTB*/
    AMHW_HC32F07X_CAN_TXBUF_STB = 1, /* STB */
}amhw_hc32f07x_can_txbuf_t;

/**
 * \brief CAN 数据长度定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_DATA_LEN0 = 0, /* 0BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN1 = 1, /* 1BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN2 = 2, /* 2BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN3 = 3, /* 3BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN4 = 4, /* 4BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN5 = 5, /* 5BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN6 = 6, /* 6BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN7 = 7, /* 7BYTE */
    AMHW_HC32F07X_CAN_DATA_LEN8 = 8, /* 8BYTE */	
}amhw_hc32f07x_can_data_t;

/**
 * \brief CAN 模式
 */
typedef enum {
    AMHW_HC32F07X_CAN_EXT_LOOPBACK_MODE   = 0X40, /* 外部回环模式 */
    AMHW_HC32F07X_CAN_INT_LOOPBACK_MODE   = 0X20, /* 内部回环模式 */
    AMHW_HC32F07X_CAN_SIG_PRIARY_MODE     = 0X10, /* PTB单次传输模式 */
    AMHW_HC32F07X_CAN_SIG_SECONDARY_MODE  = 0X08, /* STB单次传输模式 */
    AMHW_HC32F07X_CAN_LISTEN_ONLY_MODE    = 0XFF, /* 静默模式 */
}amhw_hc32f07x_can_mode_t;

/**
 * \brief CAN 模式
 */
typedef enum amhw_zmf159_can_type {
    AMHW_HC32F07X_CAN_PTB = 0,        /**< \brief ptb can */
    AMHW_HC32F07X_CAN_STB,             /**< \brief stb can */
}amhw_hc32f07x_can_type_t;

/**
 * \brief CAN 复位请求
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_reset_set (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->cfgstat |= (1u<<7);
}

/**
 * \brief CAN 复位请求清除
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_reset_clr (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->cfgstat &= ~(1u<<7);
}

/**
 * \brief CAN 模式设置使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_mode_enable (amhw_hc32f07x_can_t *p_hw_can,
                                    amhw_hc32f07x_can_mode_t mode )
{
    if(mode == AMHW_HC32F07X_CAN_LISTEN_ONLY_MODE){
        p_hw_can->tcmd |= (1u<<6);
    }else {
	      p_hw_can->cfgstat |= mode;
	}
}

/**
 * \brief CAN 模式禁能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_mode_disable (amhw_hc32f07x_can_t *p_hw_can,
                                    amhw_hc32f07x_can_mode_t mode )
{
    if(mode == AMHW_HC32F07X_CAN_LISTEN_ONLY_MODE){
        p_hw_can->tcmd &= ~(1u<<6);
    }else {
        p_hw_can->cfgstat &= ~mode;
    }
}

/**
 * \brief CAN状态标志
 *  @{
 */
#define AMHW_HC32F07X_CAN_FLAG_RX_ACTIVE        (1U << 2) /* 接收中状态信号 */
#define AMHW_HC32F07X_CAN_FLAG_TX_ACTIVE        (1U << 1) /* 发送中状态信号 */
#define AMHW_HC32F07X_CAN_FLAG_BUSOFF_ACTIVE    (1U << 0) /* 总线关闭状态信号 */

/**
 * \brief CAN 状态获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_can_status_get (amhw_hc32f07x_can_t *p_hw_can, uint8_t flag)
{
  return  ((p_hw_can->cfgstat  & flag) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief CAN TECNT和RECNT寄存器清零
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_cnt_clr (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->cfgstat |= (1u<<0);
}

/**
 * \brief CAN 发送BUF选择位
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_txbuf_sel (amhw_hc32f07x_can_t       *p_hw_can,
                                  amhw_hc32f07x_can_txbuf_t  flag)
{
    p_hw_can->tcmd  = (p_hw_can->tcmd & (~(1u << 7)))|(flag << 7);
}



/**
 * \brief CAN STBY设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stby_sel (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd |= (1u<<5);
}

/**
 * \brief CAN STBY清除
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stby_clr (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd &= ~(1u<<5);
}

/**
 * \brief CAN PTB发送使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ptb_send_enable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd |= (1u<<4);
}

/**
 * \brief CAN PTB发送禁能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ptb_send_disable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd &= ~(1u<<4);
}

/**
 * \brief CAN PTB发送取消
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ptb_abort_enable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd |= (1u<<3);
}

/**
 * \brief CAN STB发送一帧数据使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stb_sendone_enable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd |= (1u<<2);
}

/**
 * \brief CAN STB发送一帧数据禁止
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stb_sendone_disable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd &= ~(1u<<2);
}

/**
 * \brief CAN STB发送所有数据使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stb_sendall_enable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd |= (1u<<1);
}

/**
 * \brief CAN STB发送所有数据禁止
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stb_sendall_disable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd &= ~(1u<<1);
}

/**
 * \brief CAN STB发送取消
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stb_abort_enable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tcmd |= (1u<<0);
}

/**
 * \brief CAN 下一个STB SLOT
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stb_slot_next (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tctrl &= (1u<<6);
}

/**
 * \brief CAN STB发送模式定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_STB_MODE_FIFO = 0, /* FIFO模式 */
    AMHW_HC32F07X_CAN_STB_MODE_PRIM = 1, /* 优先级模式 */
}amhw_hc32f07x_can_stb_mode_t;

/**
 * \brief CAN STB发送模式
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_stb_mode_set (amhw_hc32f07x_can_t *p_hw_can,
	                                 amhw_hc32f07x_can_stb_mode_t flag)
{
    p_hw_can->tctrl = (p_hw_can->tctrl & (~(1u << 5)))|(flag << 5);
}

/**
 * \brief CAN TTCAN BUF模式定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_TT_BUF_TS   = 0, /* TSMODE设定 */
    AMHW_HC32F07X_CAN_TT_BUF_TR = 1, /* 通过TBPTR和TTPTR设定 */
}amhw_hc32f07x_can_tt_buf_t;

/**
 * \brief CAN TTCAN BUF模式
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ttcan_buf_set (amhw_hc32f07x_can_t *p_hw_can,
	                                   amhw_hc32f07x_can_tt_buf_t flag)
{
    p_hw_can->tctrl = (p_hw_can->tctrl & (~(1u << 4)))|(flag << 4);
}

/**
 * \brief CAN STB状态定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_TB_EMPTY        = 0, /* 空 */
    AMHW_HC32F07X_CAN_TB_NOT_HALFFULL = 1, /* 小于等于半满*/
    AMHW_HC32F07X_CAN_TB_HALFFULL     = 2, /* 大于半满 */
    AMHW_HC32F07X_CAN_TB_FULL         = 3, /* 满 */
}amhw_hc32f07x_can_tb_status_t;

/**
 * \brief CAN TTCAN BUF模式
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
amhw_hc32f07x_can_tb_status_t amhw_hc32f07x_can_get_tb_status (amhw_hc32f07x_can_t *p_hw_can)
{
   return (amhw_hc32f07x_can_tb_status_t) (p_hw_can->tctrl & 0x03 );
}

/**
 * \brief CAN 自应答使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_sack_enable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->rctrl |= 0x80 ;
}

/**
 * \brief CAN 自应答禁止
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_sack_disable (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->rctrl &= ~(1u << 7) ;
}

/**
 * \brief CAN 接收BUF上溢模式定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_RXBUF_OVERWRITTEN = 0, /* 最早接收到的数据被覆盖 */
    AMHW_HC32F07X_CAN_RXBUF_NOTSTORED   = 1, /* 最新接收到的数据不被存储*/
}amhw_hc32f07x_can_rxbuf_mode_t;

/**
 * \brief CAN 接收BUF上溢模式设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_rxbuf_over_mode_set (amhw_hc32f07x_can_t *p_hw_can,
	                                        amhw_hc32f07x_can_rxbuf_mode_t flag)
{
    p_hw_can->rctrl = (p_hw_can->rctrl & (~(1u << 6)))|(flag << 6);
}

/**
 * \brief CAN 接收BUF上溢标志
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_can_rxbuf_over_get (amhw_hc32f07x_can_t *p_hw_can)
{
      return ((p_hw_can->cfgstat & (1u << 5)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief CAN 接收BUF释放
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_rxbuf_release (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->rctrl |= (1u << 4);
}

/**
 * \brief CAN 接收BUF数据存储模式定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_RXBUF_NORMAL = 0, /* 正常模式 */
    AMHW_HC32F07X_CAN_RXBUF_ALL    = 1, /* 存储所有(存储所有)数据*/
}amhw_hc32f07x_can_rxbuf_all_t;

/**
 * \brief CAN 接收BUF数据存储设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_rxbuf_stores_all_set (amhw_hc32f07x_can_t *p_hw_can,
                                             amhw_hc32f07x_can_rxbuf_all_t flag)
{
    p_hw_can->rctrl = (p_hw_can->rctrl & (~(1u << 3)))|(flag << 3);
}

/**
 * \brief CAN 接收BUF状态
 */
typedef enum {
    AMHW_HC32F07X_CAN_RXBUF_EMPTY        = 0, /* 空 */
    AMHW_HC32F07X_CAN_RXBUF_NOT_ALMOSTFULL = 1, /* 非空但小于限定值 */
    AMHW_HC32F07X_CAN_RXBUF_ALMOSTFULL     = 2, /* 大于限定值但未满 */
    AMHW_HC32F07X_CAN_RXBUF_FULL         = 3, /* 满 */
}amhw_hc32f07x_can_rxbuf_status_t;

/**
 * \brief CAN TTCAN BUF模式
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
amhw_hc32f07x_can_rxbuf_status_t 
amhw_hc32f07x_can_get_rxbuf_status (amhw_hc32f07x_can_t *p_hw_can)
{
   return (amhw_hc32f07x_can_rxbuf_status_t) (p_hw_can->rctrl & 0x03 );
}

/**
 * \brief CAN 收发中断
 * @{
 */
#define AMHW_HC32F07X_CAN_INT_RECV          (0x1 << 7ul)  /**< \brief 接收中断 */
#define AMHW_HC32F07X_CAN_INT_RX_OVER       (0x1 << 6ul)  /**< \brief 接收上溢中断 */
#define AMHW_HC32F07X_CAN_INT_RX_FULL       (0x1 << 5ul)  /**< \brief 接收满中断 */
#define AMHW_HC32F07X_CAN_INT_RX_ALMOSTFULL (0x1 << 4ul)  /**< \brief 接收将满中断 */
#define AMHW_HC32F07X_CAN_INT_TX_PTB        (0x1 << 3ul)  /**< \brief PTB发送中断 */
#define AMHW_HC32F07X_CAN_INT_TX_STB        (0x1 << 2ul)  /**< \brief STB发送中断 */
#define AMHW_HC32F07X_CAN_INT_ERROR         (0x1 << 1ul)  /**< \brief 错误中断 */
#define AMHW_HC32F07X_CAN_INT_PASSIVE       (0x1 << 5ul)  /**< \brief 错误被动中断 */
#define AMHW_HC32F07X_CAN_INT_ARBILOST      (0x1 << 3ul)  /**< \brief 仲裁失败中断 */
#define AMHW_HC32F07X_CAN_INT_BUS_ERR       (0x1 << 1ul)  /**< \brief 总线错误中断 */
/**
 * @}
 */

/**
 * \brief CAN 收发中断使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_int_enable (amhw_hc32f07x_can_t *p_hw_can,
                                   uint8_t              flag)
{
    if(flag == AMHW_HC32F07X_CAN_INT_PASSIVE ||
       flag == AMHW_HC32F07X_CAN_INT_ARBILOST||
       flag == AMHW_HC32F07X_CAN_INT_BUS_ERR) {

        p_hw_can->errint |= (flag);
    }else {
        p_hw_can->rtie |= (flag);
    }
}

/**
 * \brief CAN 收发中断禁能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断源，参考宏定义AMHW_HC32F07X_CAN_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_int_disable (amhw_hc32f07x_can_t *p_hw_can,
                                    uint8_t              flag)
{
    if(flag == AMHW_HC32F07X_CAN_INT_PASSIVE ||
       flag == AMHW_HC32F07X_CAN_INT_ARBILOST||
       flag == AMHW_HC32F07X_CAN_INT_BUS_ERR) {

        p_hw_can->errint &= ~(flag);
    }else {
        p_hw_can->rtie &= ~(flag);
    }
}

/**
 * \brief CAN 禁能所有收发中断
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断源，参考宏定义AMHW_HC32F07X_CAN_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_int_all_disable (amhw_hc32f07x_can_t *p_hw_can)
{
        p_hw_can->rtie = 0x00;
}

/**
 * \brief CAN 收发中断标志
 * @{
 */
#define AMHW_HC32F07X_CAN_FLAG_RECV          (0x1 << 7ul)  /**< \brief 接收中断标志 */
#define AMHW_HC32F07X_CAN_FLAG_RX_OVER       (0x1 << 6ul)  /**< \brief 接收上溢中断标志 */
#define AMHW_HC32F07X_CAN_FLAG_RX_FULL       (0x1 << 5ul)  /**< \brief 接收满中断标志 */
#define AMHW_HC32F07X_CAN_FLAG_RX_ALMOSTFULL (0x1 << 4ul)  /**< \brief 接收将满中断标志 */
#define AMHW_HC32F07X_CAN_FLAG_TX_PTB        (0x1 << 3ul)  /**< \brief PTB发送中断标志 */
#define AMHW_HC32F07X_CAN_FLAG_TX_STB        (0x1 << 2ul)  /**< \brief STB发送中断标志*/
#define AMHW_HC32F07X_CAN_FLAG_ERROR         (0x1 << 1ul)  /**< \brief 错误中断标志 */
#define AMHW_HC32F07X_CAN_FLAG_TX_ABORT      (0x1 << 0ul)  /**< \brief 取消发送中断标志 */
/**
 * @}
 */

/**
 * \brief CAN 收发中断标志获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_FLAG_
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_can_get_irq_status (amhw_hc32f07x_can_t *p_hw_can,
                                            uint8_t              flag)
{
   return (uint8_t) ((p_hw_can->rtif & flag ) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief CAN 收发中断标志清除
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_FLAG_
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_irq_status_clr (amhw_hc32f07x_can_t *p_hw_can,
                                       uint8_t              flag)
{
     p_hw_can->rtif |= (flag);
}

/**
 * \brief CAN 错误中断标志
 * @{
 */
#define AMHW_HC32F07X_CAN_ERR_FLAG_EWARN     (0x1 << 7ul)  /**< \brief 错误警告中断标志 */
#define AMHW_HC32F07X_CAN_ERR_FLAG_PASSIVE   (0x1 << 4ul)  /**< \brief 错误被动中断标志 */
#define AMHW_HC32F07X_CAN_ERR_FLAG_ARBILOST  (0x1 << 2ul)  /**< \brief 仲裁失败中断标志 */
#define AMHW_HC32F07X_CAN_ERR_FLAG_BUS_ERR   (0x1 << 0ul)  /**< \brief 总线错误中断标志 */
/**
 * @}
 */

/**
 * \brief CAN 错误中断标志获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_ERR_FLAG_
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_can_get_err_status (amhw_hc32f07x_can_t *p_hw_can,
                                              uint8_t            flag)
{
   return (uint8_t) ((p_hw_can->errint  & flag ) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief CAN 错误中断标志清除
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_FLAG_
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_err_status_clr (amhw_hc32f07x_can_t *p_hw_can,
                                       uint8_t              flag)
{
     p_hw_can->errint |= (flag);
}

/**
 * \brief CAN 被动错误标志获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_FLAG_
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_can_err_active_get (amhw_hc32f07x_can_t *p_hw_can)
{
    return (uint8_t) ((p_hw_can->errint  & (1 << 6) ) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief CAN 波特率预分频设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] presc     : 波特率预分频值，8bit
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_brp_set (amhw_hc32f07x_can_t *p_hw_can,
                                uint8_t              presc)
{
     p_hw_can->bt = (p_hw_can->bt & (~(0xfful << 24))) | ((presc & 0xfful) << 24);
}

/**
 * \brief CAN 波特率预分频获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 波特率预分频值，8bit
 */
am_static_inline
uint8_t amhw_hc32f07x_can_brp_get (amhw_hc32f07x_can_t *p_hw_can)
{
     return (uint8_t)((p_hw_can->bt >> 24) & 0xfful);
}

/**
 * \brief CAN 同步补偿宽度时间设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] sjw       : 同步补偿宽度时间，7bit
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_sjw_set (amhw_hc32f07x_can_t *p_hw_can,
                                uint8_t              sjw)
{
     p_hw_can->bt = (p_hw_can->bt & (~(0x7f << 16))) | ((sjw & 0x7f) << 16);
}

/**
 * \brief CAN 同步补偿宽度时间获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 同步补偿宽度时间，7bit
 */
am_static_inline
uint8_t amhw_hc32f07x_can_sjw_get (amhw_hc32f07x_can_t *p_hw_can)
{
     return (uint8_t)((p_hw_can->bt >> 16) & 0x7f);
}

/**
 * \brief CAN 位段2时间设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] seg2     : 位段2时间，7bit
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tseg2_set (amhw_hc32f07x_can_t *p_hw_can,
                                  uint8_t              seg2  )
{
     p_hw_can->bt = (p_hw_can->bt & (~(0x7f << 8))) | ((seg2 & 0x7f) << 8);
}

/**
 * \brief CAN 位段2时间设定获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 位段2时间，7bit
 */
am_static_inline
uint8_t amhw_hc32f07x_can_tseg2_get (amhw_hc32f07x_can_t *p_hw_can )
{
     return (uint8_t)((p_hw_can->bt >> 8) & 0x7f);
}

/**
 * \brief CAN 位段1时间设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] seg2     : 位段1时间，8bit
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tseg1_set (amhw_hc32f07x_can_t *p_hw_can,
                                  uint8_t              seg1  )
{
     p_hw_can->bt = (p_hw_can->bt & (~0xff )) | (seg1 & 0xff);
}

/**
 * \brief CAN 位段1时间设定获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 位段1时间，8bit
 */
am_static_inline
uint8_t amhw_hc32f07x_can_tseg1_get (amhw_hc32f07x_can_t *p_hw_can )
{
     return (uint8_t)(p_hw_can->bt & 0xff);
}

/**
 * \brief CAN 错误状态定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_ERROR_NO     = 0u, /* 无错误 */
    AMHW_HC32F07X_CAN_ERROR_BIT    = 1u, /* 位错误 */
    AMHW_HC32F07X_CAN_ERROR_FORM   = 2u, /* 形式错误 */
    AMHW_HC32F07X_CAN_ERROR_STUFF  = 3u, /* 填充错误 */
    AMHW_HC32F07X_CAN_ERROR_ACK    = 4u, /* 应答错误 */
    AMHW_HC32F07X_CAN_ERROR_CRC    = 5u, /* CRC错误 */
    AMHW_HC32F07X_CAN_ERROR_UNKOWN = 6u, /* 未知错误 */
}amhw_hc32f07x_can_error_t;

/**
 * \brief CAN 错误状态获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 错误状态，参考amhw_hc32f07x_can_error_t
 */
am_static_inline
amhw_hc32f07x_can_error_t 
amhw_hc32f07x_can_error_status_get (amhw_hc32f07x_can_t *p_hw_can )
{
     return (amhw_hc32f07x_can_error_t)((p_hw_can->ealcap >> 5) & 0x07);
}

/**
 * \brief CAN 仲裁失败捕获
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 仲裁失败位置
 */
am_static_inline
uint8_t amhw_hc32f07x_can_arbitration_lost_get (amhw_hc32f07x_can_t *p_hw_can)
{
     return (uint8_t)(p_hw_can->ealcap & 0x1f);
}

/**
 * \brief CAN 接收BUF警告限定设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] val       : 接收BUF警告限定值
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_rxbuf_warning_set (amhw_hc32f07x_can_t *p_hw_can,
                                          uint8_t              val  )
{
     p_hw_can->limit = (p_hw_can->limit & (~(0x0f << 4))) | ((val & 0x0f) << 4);
}

/**
 * \brief CAN 错误警告限定设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] val       : 错误警告限定值
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_err_warning_set (amhw_hc32f07x_can_t *p_hw_can,
                                        uint8_t              val  )
{
     p_hw_can->limit = (p_hw_can->limit & (~0x0f)) | (val & 0x0f);
}

/**
 * \brief CAN 发送错误计数器
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 错误计数值
 */
am_static_inline
uint8_t amhw_hc32f07x_can_get_transerrorcount (amhw_hc32f07x_can_t *p_hw_can)
{
     return (uint8_t)(p_hw_can->tecnt);
}

/**
 * \brief CAN 接收错误计数器
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return 错误计数值
 */
am_static_inline
uint8_t amhw_hc32f07x_can_get_recverrorcount (amhw_hc32f07x_can_t *p_hw_can)
{
     return (uint8_t)(p_hw_can->recnt);
}

/**
 * \brief CAN 选择筛选器定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_ACF_ID     = 0u, /* ACF指向筛选器ID寄存器 */
    AMHW_HC32F07X_CAN_ACF_MASK   = 1u, /* ACF指向筛选器MASK寄存器 */
}amhw_hc32f07x_can_acf_t;

/**
 * \brief CAN 选择筛选器设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 参考枚举定义amhw_hc32f07x_can_acf_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_acf_select_set (amhw_hc32f07x_can_t      *p_hw_can,
                                       amhw_hc32f07x_can_acf_t   flag)
{
     p_hw_can->acfctrl = (p_hw_can->acfctrl & (~(0x01 << 5))) | (flag << 5);
}

/**
 * \brief CAN 筛选器组使能
 */
typedef enum {
    AMHW_HC32F07X_CAN_FILTER1     = 0u, /* 筛选器组1使能 */
    AMHW_HC32F07X_CAN_FILTER2     = 1u, /* 筛选器组2使能 */
    AMHW_HC32F07X_CAN_FILTER3     = 2u, /* 筛选器组3使能 */
    AMHW_HC32F07X_CAN_FILTER4     = 3u, /* 筛选器组4使能 */
    AMHW_HC32F07X_CAN_FILTER5     = 4u, /* 筛选器组5使能 */
    AMHW_HC32F07X_CAN_FILTER6     = 5u, /* 筛选器组6使能 */
    AMHW_HC32F07X_CAN_FILTER7     = 6u, /* 筛选器组7使能 */
    AMHW_HC32F07X_CAN_FILTER8     = 7u, /* 筛选器组8使能 */
}amhw_hc32f07x_can_filter_t;

/**
 * \brief CAN 筛选器地址
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] filter    : 参考枚举定义amhw_hc32f07x_can_filter_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_filter_addr (amhw_hc32f07x_can_t        *p_hw_can,
                                    amhw_hc32f07x_can_filter_t  filter)
{
     p_hw_can->acfctrl = (filter & 0x0f);
}

/**
 * \brief CAN 筛选器组使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] filter    : 参考枚举定义amhw_hc32f07x_can_filter_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_filter_enable (amhw_hc32f07x_can_t        *p_hw_can,
                                      amhw_hc32f07x_can_filter_t  filter)
{
     p_hw_can->acfen |= (1u << filter);
}

/**
 * \brief CAN 筛选器组禁能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] filter    : 参考枚举定义amhw_hc32f07x_can_filter_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_filter_disable (amhw_hc32f07x_can_t        *p_hw_can,
                                       amhw_hc32f07x_can_filter_t  filter)
{
    p_hw_can->acfen &= ~(1u << filter);
}

/**
 * \brief CAN 筛选器组使能
 */
typedef enum {
    AMHW_HC32F07X_CAN_AIDE_STD  = 2u, /* 仅接收标准帧 */
    AMHW_HC32F07X_CAN_AIDE_EXT  = 3u, /* 仅接收扩展帧 */
    AMHW_HC32F07X_CAN_AIDE_ALL  = 0U, /* 接收标准帧和扩展帧 */
}amhw_hc32f07x_can_aide_t;

/**
 * \brief CAN 筛选器接收类型设定
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ide_set (amhw_hc32f07x_can_t     *p_hw_can,
                                amhw_hc32f07x_can_aide_t flag)
{
     p_hw_can->acf = (p_hw_can->acf & (~(0x03 << 29))) | (flag << 29);
}

/**
 * \brief CAN 筛选器CODE/MASK设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] val       : 设置值
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_filter_code_set (amhw_hc32f07x_can_t *p_hw_can,
                                        uint32_t             val)
{
     p_hw_can->acf = (p_hw_can->acf & (~(0x1fffffff << 0))) | (val & 0x1fffffff);
}

/**
 * \brief CAN TTCAN设置TB为空
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_tbslot_empty_set (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tbslot |= (1u << 7);
}

/**
 * \brief CAN TTCAN设置TB为填充
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_tbslot_filled_set (amhw_hc32f07x_can_t *p_hw_can)
{
    p_hw_can->tbslot |= (1u << 6);
}

/**
 * \brief CAN 筛选器组使能
 */
typedef enum {
    AMHW_HC32F07X_CAN_TBPTR_PTB   = 0u, /* 指向PTB */
    AMHW_HC32F07X_CAN_TBPTR_STB1  = 1u, /* 指向STB SLOT1 */
    AMHW_HC32F07X_CAN_TBPTR_STB2  = 2u, /* 指向STB SLOT2 */
    AMHW_HC32F07X_CAN_TBPTR_STB3  = 3u, /* 指向STB SLOT1 */
    AMHW_HC32F07X_CAN_TBPTR_STB4  = 4u, /* 指向STB SLOT2 */
}amhw_hc32f07x_can_tbptr_t;

/**
 * \brief CAN 筛选器CODE/MASK设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 参考枚举量amhw_hc32f07x_can_tbptr_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tbptr_set (amhw_hc32f07x_can_t       *p_hw_can,
                                  amhw_hc32f07x_can_tbptr_t  flag)
{
     p_hw_can->tbslot = (p_hw_can->tbslot & (~(0x07<< 0))) | (flag & 0x07);
}

/**
 * \brief CAN TTCAN中断
 * @{
 */
#define AMHW_HC32F07X_CAN_TT_INT_WT  (0x1 << 7ul)    /**< \brief 看门中断 */
#define AMHW_HC32F07X_CAN_TT_INT_IF  (0x1 << 4ul)    /**< \brief 时间触发中断 */
/**
 * @}
 */

/**
 * \brief CAN TTCAN中断使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 参考枚举量AMHW_HC32F07X_CAN_TT_INT_	
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_int_enable (amhw_hc32f07x_can_t *p_hw_can,
                                      uint8_t              flag)
{
     p_hw_can->ttcfg |= flag;
}

/**
 * \brief CAN TTCAN中断禁能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 参考枚举量AMHW_HC32F07X_CAN_TT_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_int_disable (amhw_hc32f07x_can_t *p_hw_can,
                                       uint8_t              flag)
{
     p_hw_can->ttcfg &= ~flag;
}

/**
 * \brief CAN TTCAN中断标志
 * @{
 */
#define AMHW_HC32F07X_CAN_TT_FLAG_WT  (0x1 << 6ul)    /**< \brief 看门中断标志 */
#define AMHW_HC32F07X_CAN_TT_FLAG_ERR (0x1 << 5ul)   /**< \brief 看门中断标志 */
#define AMHW_HC32F07X_CAN_TT_FLAG_IF  (0x1 << 3ul)    /**< \brief 时间触发中断标志 */
/**
 * @}
 */

/**
 * \brief CAN TTCAN中断标志获取
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_TT_FLAG_
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_can_tt_status_get (amhw_hc32f07x_can_t *p_hw_can,
                                           uint8_t              flag)
{
    return (uint8_t) ((p_hw_can->ttcfg & flag ) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief CAN TTCAN中断标志清零
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 中断标志，参考宏定义AMHW_HC32F07X_CAN_TT_FLAG_
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
void amhw_hc32f07x_can_tt_status_clr (amhw_hc32f07x_can_t *p_hw_can,
                                      uint8_t              flag)
{
    p_hw_can->ttcfg |= flag;
}

/**
 * \brief CAN TTCAN计数器预分频
 */
typedef enum {
    AMHW_HC32F07X_CAN_TT_PRESC_DIV1  = 0u, /* DIV1 */
    AMHW_HC32F07X_CAN_TT_PRESC_DIV2  = 1u, /* DIV2 */
    AMHW_HC32F07X_CAN_TT_PRESC_DIV3  = 2u, /* DIV3 */
    AMHW_HC32F07X_CAN_TT_PRESC_DIV4  = 3u, /* DIV4 */
}amhw_hc32f07x_can_tt_presc_t;

/**
 * \brief CAN TTCAN计数器预分频设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : TTCAN计数器预分频值，参考枚举量amhw_hc32f07x_can_tt_presc_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_prescdiv_set (amhw_hc32f07x_can_t *p_hw_can,
                                        uint8_t              flag)
{
    p_hw_can->ttcfg = (p_hw_can->ttcfg & (~(0x03<< 1))) | ((flag & 0x03) << 1);
}

/**
 * \brief CAN TTCAN使能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_enable (amhw_hc32f07x_can_t *p_hw_can)
{
     p_hw_can->ttcfg |= 0x01;
}

/**
 * \brief CAN TTCAN禁能
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_disable (amhw_hc32f07x_can_t *p_hw_can)
{
     p_hw_can->ttcfg &= ~(0x01);
}

/**
 * \brief CAN TTCAN参考消息IDE定义
 */
typedef enum {
    AMHW_HC32F07X_CAN_TT_IDE_STD  = 0u, /* 标准格式 */
    AMHW_HC32F07X_CAN_TT_IDE_EXT  = 1u, /* 扩展格式 */
}amhw_hc32f07x_can_tt_ide_t;

/**
 * \brief CAN TTCAN参考消息IDE设置
 * \param[in] p_hw_can : 指向CAN寄存器结构体的指针
 * \param[in] flag     : IDE设置值，参考枚举量amhw_hc32f07x_can_tt_ide_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_refide_format_set (amhw_hc32f07x_can_t *p_hw_can,
                                             uint8_t              flag)
{
    p_hw_can->refmsg = (p_hw_can->refmsg & (~(1U << 31))) | ((flag & 0x01) << 31);;
}

/**
 * \brief CAN TTCAN参考消息IDE设置
 * \param[in] p_hw_can : 指向CAN寄存器结构体的指针
 * \param[in] val      : IDE设置值，参考枚举量amhw_hc32f07x_can_tt_ide_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_refid_set (amhw_hc32f07x_can_t *p_hw_can,
                                     uint32_t             val)
{
    p_hw_can->refmsg = (p_hw_can->refmsg & (1U << 31)) | (val & (~(1U << 31)));
}

/**
 * \brief CAN TTCAN触发类型
 */
typedef enum {
    AMHW_HC32F07X_CAN_TT_IMMEDIATE = 0u, /* 立即触发 */
    AMHW_HC32F07X_CAN_TT_TIME      = 1u, /* 时间触发 */
    AMHW_HC32F07X_CAN_TT_SINGLE    = 2u, /* 单次发送触发 */
    AMHW_HC32F07X_CAN_TT_START     = 3u, /* 发送开始触发 */
    AMHW_HC32F07X_CAN_TT_STOP      = 4u, /* 发送停止触发 */
}amhw_hc32f07x_can_tt_trigger_type_t;

/**
 * \brief CAN TTCAN参考消息IDE设置
 * \param[in] p_hw_can : 指向CAN寄存器结构体的指针
 * \param[in] flag     : 触发类型，参考枚举量amhw_hc32f07x_can_tt_trigger_type_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_trigger_type_set (amhw_hc32f07x_can_t                 *p_hw_can,
                                            amhw_hc32f07x_can_tt_trigger_type_t  flag)
{
    p_hw_can->refmsg  = (p_hw_can->refmsg & (~(0x03 << 8))) | 
                       ((flag & 0x03) << 8);
}

/**
 * \brief CAN TTCAN发送使能窗口设置
 * \param[in] p_hw_can : 指向CAN寄存器结构体的指针
 * \param[in] flag     : 发送使能窗口数，为flag+1
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_tt_tew_set (amhw_hc32f07x_can_t *p_hw_can,
                                   uint8_t              flag)
{
    p_hw_can->trgcfg = (p_hw_can->trgcfg & (~(0x0f << 12))) | 
                       ((flag & 0x0f) << 12   );
}

/**
 * \brief CAN TTCAN发送触发器指针
 */
typedef enum {
    AMHW_HC32F07X_CAN_TTPTR_PTB   = 0u, /* 指向PTB */
    AMHW_HC32F07X_CAN_TTPTR_STB1  = 1u, /* 指向STB SLOT1 */
    AMHW_HC32F07X_CAN_TTPTR_STB2  = 2u, /* 指向STB SLOT2 */
    AMHW_HC32F07X_CAN_TTPTR_STB3  = 3u, /* 指向STB SLOT1 */
    AMHW_HC32F07X_CAN_TTPTR_STB4  = 4u, /* 指向STB SLOT2 */
}amhw_hc32f07x_can_ttptr_t;

/**
 * \brief CAN 发送触发器TB slot指针设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 参考枚举量amhw_hc32f07x_can_ttptr_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ttcan_ttptr_set (amhw_hc32f07x_can_t       *p_hw_can,
                                        amhw_hc32f07x_can_ttptr_t  flag)
{
     p_hw_can->trgcfg = (p_hw_can->trgcfg & (~(0x07<< 0))) | (flag & 0x07);
}

/**
 * \brief CAN 触发时间设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 触发时间
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ttcan_trig_set (amhw_hc32f07x_can_t *p_hw_can,
                                       uint16_t             flag)
{
     p_hw_can->trgtrig  = (p_hw_can->trgtrig & (~0xffff)) | flag;
}

/**
 * \brief CAN 触发看门时间设置
 * \param[in] p_hw_can  : 指向CAN寄存器结构体的指针
 * \param[in] flag      : 看门时间
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_can_ttcan_wtrig_set (amhw_hc32f07x_can_t *p_hw_can,
                                        uint16_t             flag)
{
     p_hw_can->trgwtrig = (p_hw_can->trgwtrig & (~0xffff)) | flag;
}


/**
 * @} amhw_hc32f07x_if_can
 */
#pragma no_anon_unions
#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F07X_CAN_H */

/* end of file */
