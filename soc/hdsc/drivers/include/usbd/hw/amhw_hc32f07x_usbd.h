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
 * \brief USBFS操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-15
 * \endinternal
 */

#ifndef __AMHW_HC32_USBFS_H
#define __AMHW_HC32_USBFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <am_types.h>

/*
 * \brief 匿名结构体段的开始
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__GNUC__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_hc32_if_usbfs
 * \copydoc amhw_hc32_usbfs.h
 * @{
 */

/**
 * \brief USBFS寄存器块结构体
 */
typedef struct amhw_hc32_usbfs {
    __IO uint32_t reserve0[2];     /**< \brief 保留 */
    __IO uint32_t gahbcfg;         /**< \brief 0x08 AHB控制寄存器 */
    __IO uint32_t gusbcfg;         /**< \brief 0x0c USB配置寄存器 */
    __IO uint32_t grstctl;         /**< \brief 0x10 复位寄存器 */
    __IO uint32_t gintsts;         /**< \brief 0x14 中断寄存器 */
    __IO uint32_t gintmsk;         /**< \brief 0x18 中断屏蔽寄存器 */
    __IO uint32_t grxstsr;         /**< \brief 0x1c 接收状态调试读取寄存器 */
    __IO uint32_t grxstsp;         /**< \brief 0x20 接收状态读取和出栈寄存器 */
    __IO uint32_t grxfsiz;         /**< \brief 0x24 接收FIFO大小寄存器 */
    __IO uint32_t gnptxfsiz;       /**< \brief 0x28 非周期发送FIFO大小寄存器 */
    __I  uint32_t gnptxsts;        /**< \brief 0x2c 非周期发送FIFO状态寄存器 */
    __IO uint32_t reserve1[3];     /**< \brief 保留 */
    __IO uint32_t cid;             /**< \brief 0x3c ID寄存器 */
    __IO uint32_t reserve2[49];    /**< \brief 保留 */	
    __IO uint32_t dptxfsiz1;       /**< \brief 0x104 设备周期IN端点发送FIFO大小寄存器*/
    __IO uint32_t dptxfsiz2;       /**< \brief 0x108 设备周期IN端点发送FIFO大小寄存器*/
    __IO uint32_t dptxfsiz3;       /**< \brief 0x10c 设备周期IN端点发送FIFO大小寄存器*/
    __IO uint32_t dptxfsiz4;       /**< \brief 0x110 设备周期IN端点发送FIFO大小寄存器*/
    __IO uint32_t reserve3[443];   /**< \brief 保留 */	
    __IO uint32_t dcfg;            /**< \brief 0x800 设备配置寄存器 */
    __IO uint32_t dctl;            /**< \brief 0x804 设备控制寄存器 */
    __I  uint32_t dsts;            /**< \brief 0x808 设备状态寄存器 */
    __IO uint32_t reserve4;        /**< \brief 保留 */
    __IO uint32_t diepmsk;         /**< \brief 0x810 设备IN端点通用中断屏蔽寄存器 */
    __IO uint32_t doepmsk;         /**< \brief 0x814 设备OUT端点通用中断屏蔽寄存器 */
    __IO  uint32_t daint;           /**< \brief 0x818 设备全体端点中断寄存器 */	
    __IO uint32_t daintmsk;        /**< \brief 0x81c 设备全体端点中断屏蔽寄存器 */
    __IO uint32_t reserve5[5];     /**< \brief 保留 */
    __IO uint32_t diepempmsk;      /**< \brief 0x834 设备IN端点FIFO空中断屏蔽寄存器 */	
    __IO uint32_t reserve6[50];    /**< \brief 保留 */
    __IO uint32_t diepctl0;        /**< \brief 0x900 设备IN端点0控制寄存器 */
    __IO uint32_t reserve7;        /**< \brief 保留 */
    __IO uint32_t diepint0;        /**< \brief 0x908 设备IN端点0中断寄存器 */
    __IO uint32_t reserve8;        /**< \brief 保留 */
    __IO uint32_t diepsiz0;        /**< \brief 0x910 设备IN端点0传输大小寄存器 */

    __IO uint32_t diepdma0;        /**< \brief 0x914 设备IN端点0DMA地址寄存器 */
    __IO uint32_t dtxfsts0;        /**< \brief 0x920 设备IN端点0控制寄存器 */
    __IO uint32_t reserve9;        /**< \brief 保留 */

    __IO uint32_t diepctl1;        /**< \brief 0x920 设备IN端点1控制寄存器 */
    __IO uint32_t reserve10;        /**< \brief 保留 */
    __IO uint32_t diepint1;        /**< \brief 0x928 设备IN端点1中断寄存器 */
    __IO uint32_t reserve11;       /**< \brief 保留 */
    __IO uint32_t diepsiz1;        /**< \brief 0x930 设备IN端点1传输大小寄存器 */
    __IO uint32_t diepdma1;        /**< \brief 0x934 设备IN端点1DMA地址寄存器 */
    __IO uint32_t dtxfsts1;        /**< \brief 0x938 设备IN端点1控制寄存器 */
    __IO uint32_t reserve12[9];    /**< \brief 0x93c保留 */

    __IO uint32_t diepctl2;        /**< \brief 0x960 设备IN端点2控制寄存器 */
    __IO uint32_t reserve13;       /**< \brief 保留 */		
    __IO uint32_t diepint2;        /**< \brief 0x968 设备IN端点2中断寄存器 */	
    __IO uint32_t reserve14;       /**< \brief 保留 */
    __IO uint32_t diepsiz2;        /**< \brief 0x970 设备IN端点2传输大小寄存器 */
    __IO uint32_t diepdma2;        /**< \brief 0x974 设备IN端点2DMA地址寄存器 */
    __IO uint32_t dtxfsts2;        /**< \brief 0x920 设备IN端点2控制寄存器 */
    __IO uint32_t reserve15[9];   /**< \brief 保留 */

    __IO uint32_t diepctl3;        /**< \brief 0x960 设备IN端点3控制寄存器 */
    __IO uint32_t reserve16;       /**< \brief 保留 */		
    __IO uint32_t diepint3;        /**< \brief 0x968 设备IN端点3中断寄存器 */	
    __IO uint32_t reserve17;       /**< \brief 保留 */
    __IO uint32_t diepsiz3;        /**< \brief 0x970 设备IN端点3传输大小寄存器 */
    __IO uint32_t diepdma3;        /**< \brief 0x974 设备IN端点3DMA地址寄存器 */
    __IO uint32_t dtxfsts3;        /**< \brief 0x920 设备IN端点3控制寄存器 */
    __IO uint32_t reserve18[9];   /**< \brief 保留 */

    __IO uint32_t diepctl4;        /**< \brief 0x9e0 设备IN端点4控制寄存器 */
    __IO uint32_t reserve19;       /**< \brief 保留 */		
    __IO uint32_t diepint4;        /**< \brief 0x9e8 设备IN端点4中断寄存器 */	
    __IO uint32_t reserve20;       /**< \brief 保留 */
    __IO uint32_t diepsiz4;        /**< \brief 0x9f0 设备IN端点4传输大小寄存器 */
    __IO uint32_t diepdma4;        /**< \brief 0x9f4 设备IN端点4DMA地址寄存器 */
    __IO uint32_t dtxfsts4;        /**< \brief 0x9f8 设备IN端点4控制寄存器 */
    __IO uint32_t reserve21[65];   /**< \brief 0x9fc保留 */

    __IO uint32_t doepctl0;        /**< \brief 0xb00 设备OUT端点0控制寄存器 */
    __IO uint32_t reserve22;       /**< \brief 保留 */		
    __IO uint32_t doepint0;        /**< \brief 0xb08 设备OUT端点0中断寄存器 */
    __IO uint32_t reserve23;       /**< \brief 保留 */
    __IO uint32_t doepsiz0;        /**< \brief 0xb10 设备OUT端点0传输大小寄存器 */
    __IO uint32_t doepdma0;        /**< \brief 0xb14 设备OUT端点0DMA地址寄存器 */
    __IO uint32_t reserve24[10];   /**< \brief 0xb18保留 */

    __IO uint32_t doepctl1;        /**< \brief 0xb40 设备OUT端点1控制寄存器 */
    __IO uint32_t reserve25;       /**< \brief 保留 */		
    __IO uint32_t doepint1;        /**< \brief 0xb08 设备OUT端点1中断寄存器 */
    __IO uint32_t reserve26;       /**< \brief 保留 */
    __IO uint32_t doepsiz1;        /**< \brief 0xb10 设备OUT端点1传输大小寄存器 */
    __IO uint32_t doepdma1;        /**< \brief 0x974 设备OUT端点1DMA地址寄存器 */
    __IO uint32_t reserve27[10];   /**< \brief 保留 */	

    __IO uint32_t doepctl2;        /**< \brief 0xb40 设备OUT端点2控制寄存器 */
    __IO uint32_t reserve28;       /**< \brief 保留 */
    __IO uint32_t doepint2;        /**< \brief 0xb48 设备OUT端点2中断寄存器 */
    __IO uint32_t reserve29;       /**< \brief 保留 */
    __IO uint32_t doepsiz2;        /**< \brief 0xb50 设备OUT端点2传输大小寄存器 */
    __IO uint32_t doepdma2;        /**< \brief 0xb54 设备OUT端点2DMA地址寄存器 */
    __IO uint32_t reserve30[10];   /**< \brief 保留 */

    __IO uint32_t doepctl3;        /**< \brief 0xb80 设备OUT端点3控制寄存器 */
    __IO uint32_t reserve31;       /**< \brief 保留 */
    __IO uint32_t doepint3;        /**< \brief 0xb88 设备OUT端点3中断寄存器 */
    __IO uint32_t reserve32;       /**< \brief 保留 */
    __IO uint32_t doepsiz3;        /**< \brief 0xb90 设备OUT端点3传输大小寄存器 */
    __IO uint32_t doepdma3;        /**< \brief 0xb94 设备OUT端点3DMA地址寄存器 */
    __IO uint32_t reserve33[10];   /**< \brief 保留 */

    __IO uint32_t doepctl4;        /**< \brief 0xC00 设备OUT端点4控制寄存器 */
    __IO uint32_t reserve34;       /**< \brief 保留 */
    __IO uint32_t doepint4;        /**< \brief 0xc08 设备OUT端点4中断寄存器 */
    __IO uint32_t reserve35;       /**< \brief 保留 */
    __IO uint32_t doepsiz4;        /**< \brief 0xb10 设备OUT端点4传输大小寄存器 */
    __IO uint32_t doepdma4;        /**< \brief 0xc14 设备OUT端点4DMA地址寄存器 */
    __IO uint32_t reserve36[122];   /**< \brief 保留 */

    __IO uint32_t pcgcctl;         /**< \brief 0xe00 电源和门控时钟控制急促器 */		
} amhw_hc32_usbfs_t;
 
typedef enum {
    AMHW_HC32_USBFS_SINGLE = 0u,                // 单次
    AMHW_HC32_USBFS_INCR   = 1u,                // INCR
    AMHW_HC32_USBFS_INCR4  = 3u,                // INCR4
    AMHW_HC32_USBFS_INCR8  = 5u,                // INCR8
    AMHW_HC32_USBFS_INCR16 = 7u,                // INCR16
}amhw_hc32_usbfs_hbstlen_type_t;

typedef enum {
    AMHW_HC32_USBFS_TXFIFO0   = 0u,                // TXFIFO0编号
    AMHW_HC32_USBFS_TXFIFO1   = 1u,                // TXFIFO1编号
    AMHW_HC32_USBFS_TXFIFO2   = 2u,                // TXFIFO2编号
    AMHW_HC32_USBFS_TXFIFO3   = 3u,                // TXFIFO3编号
    AMHW_HC32_USBFS_TXFIFO4   = 4u,                // TXFIFO4编号
    AMHW_HC32_USBFS_TXFIFO5   = 5u,                // TXFIFO5编号
    AMHW_HC32_USBFS_TXFIFO6   = 6u,                // TXFIFO6编号
    AMHW_HC32_USBFS_TXFIFO7   = 7u,                // TXFIFO7编号
    AMHW_HC32_USBFS_TXFIFO8   = 8u,                // TXFIFO8编号
    AMHW_HC32_USBFS_TXFIFO9   = 9u,                // TXFIFO9编号
    AMHW_HC32_USBFS_TXFIFO10  = 10u,               // TXFIFO10编号
    AMHW_HC32_USBFS_TXFIFO11  = 11u,               // TXFIFO11编号
    AMHW_HC32_USBFS_TXFIFO12  = 12u,               // TXFIFO12编号
    AMHW_HC32_USBFS_TXFIFO13  = 13u,               // TXFIFO13编号
    AMHW_HC32_USBFS_TXFIFO14  = 14u,               // TXFIFO14编号
    AMHW_HC32_USBFS_TXFIFO15  = 15u,               // TXFIFO15编号
    AMHW_HC32_USBFS_TXFIFOALL = 16U,               // 刷新所有的发送FIFO
}amhw_hc32_usbfs_txfifonum_type_t;

/**
 * \brief  DMA使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dma_enable (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->gahbcfg |= (0x1ul << 5);
}

/**
 * \brief  DMA禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dma_disable (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->gahbcfg &= ~(0x1ul << 5);
} 

/**
 * \brief  HBSTLEN 批量长度/类型设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] hbstlen    :批量长度/类型(amhw_hc32_usbfs_hbstlen_type_t枚举量)
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_hbstlen_set (amhw_hc32_usbfs_t *p_hw_usbfs,
                                  amhw_hc32_usbfs_hbstlen_type_t hbstlen)
{
    p_hw_usbfs->gahbcfg |= ((hbstlen & 0xful) << 1);
} 

/**
 * \brief  GINTMSK 全局中断使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_gintmsk_enable (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->gahbcfg |= (0x1ul << 0);
}

/**
 * \brief  GINTMSK 全局中断禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_gintmsk_disable (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->gahbcfg &= ~(0x1ul << 0);
} 

/**
 * \brief  USB周转时间设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_trdt_set (amhw_hc32_usbfs_t *p_hw_usbfs, uint8_t tim)
{
    p_hw_usbfs->gusbcfg = (p_hw_usbfs->gusbcfg & (~(0xf << 10))) | 
                          ((tim & 0xf) << 10);
}

/**
 * \brief  PHYSEL 全速系列收发器选择
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_physel_set (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->gusbcfg |= (0x1ul << 6);
}

/**
 * \brief  AHBIDL AHB主器件空闲指示
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return AM_TRUE  :  主器件处于空闲
 *         AM_FALSE :  主器件不处于空闲
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_ahbidl_read (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return ((p_hw_usbfs->grstctl >> 31) & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief  DMAREQ DMA请求进行指示
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return AM_TRUE  :  有DMA请求正在进行
 *         AM_FALSE :  无正在进行的DMA请求
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_dmareq_read(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return ((p_hw_usbfs->grstctl >> 30) & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief  TXFNUM TxFIFO编号设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] txfifo     : 进行刷新的FIFO编号(amhw_hc32_usbfs_txfifonum_type_t枚举量)
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_txfnum_write (amhw_hc32_usbfs_t *p_hw_usbfs, 
                                   amhw_hc32_usbfs_txfifonum_type_t txfifo)
{
    p_hw_usbfs->grstctl |= ((txfifo & 0x1ful) << 6);
}

/**
 * \brief  TXFFLSH TxFIFO刷新使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_txfflsh_enable (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->grstctl |= (0x1ul << 5);
}

/**
 * \brief  TXFFLSH TxFIFO刷新禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_txfflsh_disable (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->grstctl &= ~(0x1ul << 5);
}

/**
 * \brief  RXFFLSH RxFIFO刷新使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_rxfflsh_enable (amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->grstctl |=(0x1ul << 4);
}

/**
 * \brief  RXFFLSH RxFIFO刷新禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_rxfflsh_disable(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->grstctl &= ~(0x1ul << 4);
}

/**
 * \brief  HSRST HCLK域逻辑软复位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_hsrst_set(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->grstctl |= (0x1ul << 1);
}

///**
// * \brief  HSRST HCLK域逻辑软复位
// *
// * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
// *
// * \return none
// */
//am_static_inline
//void amhw_hc32_usbfs_hsrst_clr(amhw_hc32_usbfs_t *p_hw_usbfs)
//{
//    p_hw_usbfs->grstctl &= ~(0x1ul << 1);
//}

/**
 * \brief  CSRST 模块软复位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_csrst_enable(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->grstctl |=(0x1ul << 0);
}

/**
 * \brief  CSRST 模块软复位值获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return AM_TRUE  :  复位状态
 *         AM_FALSE :  未进行复位
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_csrst_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return ((p_hw_usbfs->grstctl & (0x1ul << 0)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 全局中断状态定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_INT_WKUOINT   (1U << 31) /* 恢复、远程唤醒中断 */
#define AMHW_HC32F07X_USBFS_INT_VBUS      (1U << 30) /* VBUSy中断 */
#define AMHW_HC32F07X_USBFS_INT_DATAFSUSP (1U << 22) /* 数据获取挂起 */
#define AMHW_HC32F07X_USBFS_INT_INISOOUT  (1U << 21) /* 未完成OUT同步传输 */
#define AMHW_HC32F07X_USBFS_INT_IISOIXFR  (1U << 20) /* 未完成IN同步传输 */
#define AMHW_HC32F07X_USBFS_INT_OEPINT    (1U << 19) /* OUT端点中断 */
#define AMHW_HC32F07X_USBFS_INT_IEPINT    (1U << 18) /* IN端点中断 */
#define AMHW_HC32F07X_USBFS_INT_EOPF      (1U << 15) /* 周期性帧结束中断 */
#define AMHW_HC32F07X_USBFS_INT_ISOODRP   (1U << 14) /* 丢弃同步数据包中断 */
#define AMHW_HC32F07X_USBFS_INT_ENUMDNE   (1U << 13) /* 枚举完成中断 */
#define AMHW_HC32F07X_USBFS_INT_USBRST    (1U << 12) /* USB复位中断 */
#define AMHW_HC32F07X_USBFS_INT_USBSUSP   (1U << 11) /* USB挂起中断 */
#define AMHW_HC32F07X_USBFS_INT_ESUSP     (1U << 10) /* USB早期挂起中断 */
#define AMHW_HC32F07X_USBFS_INT_GONAKEFF  (1U << 7) /* 全局OUTNAK有效中断 */
#define AMHW_HC32F07X_USBFS_INT_GINAKEFF  (1U << 6) /* 全局非周期性INNAK有效中断 */
#define AMHW_HC32F07X_USBFS_INT_RXFNE     (1U << 4) /* RXFIFO非空中断 */
#define AMHW_HC32F07X_USBFS_INT_SOF       (1U << 3) /* 帧起始中断 */
#define AMHW_HC32F07X_USBFS_INT_ALL       (0XFFFFFFFF) /* 所有全局中断*/
/**
 * \brief  全局中断状态获取
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return  参考宏定义AMHW_HC32F07X_USBFS_INT_
 */
am_static_inline
uint32_t amhw_hc32_usbfs_gintsts_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
  return  (uint32_t)(p_hw_usbfs->gintsts );
}

/**
 * \brief  全局中断状态清除
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 中断标志，参考宏定义AMHW_HC32F07X_USBFS_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_gintsts_clr(amhw_hc32_usbfs_t *p_hw_usbfs,
                                 uint32_t           flag)
{
    p_hw_usbfs->gintsts |= flag;
}

/**
 * \brief  全局中断使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 中断标志，参考宏定义AMHW_HC32F07X_USBFS_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_int_enable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint32_t           flag)
{
    p_hw_usbfs->gintmsk |= flag;
}

/**
 * \brief  对应全局中断禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 中断标志，参考宏定义AMHW_HC32F07X_USBFS_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_int_disable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint32_t           flag)
{
    p_hw_usbfs->gintmsk &= ~flag;
}

/**
 * \brief USBFS数据包状态
 */
typedef enum {
    AMHW_HC32F07X_USBFS_STS_GOUT_NAK    = 1, /* 全局OUTNAK（触发中断） */
    AMHW_HC32F07X_USBFS_STS_DATA_UPDT   = 2, /* 接收到OUT数据包 */
    AMHW_HC32F07X_USBFS_STS_XFER_COMP   = 3, /* OUT传输完成（触发中断） */
    AMHW_HC32F07X_USBFS_STS_SETUP_COMP  = 4, /* SETUP事务完成（触发中断） */
    AMHW_HC32F07X_USBFS_STS_SETUP_UPDT  = 6, /* 接收到SETUP数据包 */
}amhw_hc32f07x_usbfs_sts_t;

/**
 * \brief  获取数据包状态
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return 参考枚举量amhw_hc32f07x_usbfs_sts_t
 */
am_static_inline
amhw_hc32f07x_usbfs_sts_t 
amhw_hc32_usbfs_pktsts_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (amhw_hc32f07x_usbfs_sts_t)((p_hw_usbfs->grxstsr >> 17) & 0x0f);
}

/**
 * \brief USBFS数据PID
 */
typedef enum {
    AMHW_HC32F07X_USBFS_PID_DATA0   = 0, /* DATA0 */
    AMHW_HC32F07X_USBFS_PID_DATA1   = 2, /* DATA1 */
    AMHW_HC32F07X_USBFS_PID_DATA2   = 1, /* DATA2 */
    AMHW_HC32F07X_USBFS_PID_SETUP   = 3, /* MDATA */
}amhw_hc32f07x_usbfs_pid_t;

/**
 * \brief  获取数据PID
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return 参考枚举量amhw_hc32f07x_usbfs_pid_t
 */
am_static_inline
amhw_hc32f07x_usbfs_pid_t 
amhw_hc32_usbfs_pid_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (amhw_hc32f07x_usbfs_pid_t)((p_hw_usbfs->grxstsr >> 15) & 0x03);
}

/**
 * \brief  获取接收的数据包的字节数
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return 字节数
 */
am_static_inline
uint16_t amhw_hc32_usbfs_bcnt_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (uint16_t)((p_hw_usbfs->grxstsr >> 4) & 0x3ff);
}

/**
 * \brief  获取接收的数据包的端点编号
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return 端点编号
 */
am_static_inline
uint8_t amhw_hc32_usbfs_epnum_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (uint8_t)(p_hw_usbfs->grxstsr & 0x0f);
}

/**
 * \brief  设置RxFIFO深度
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_rxfd_set(amhw_hc32_usbfs_t *p_hw_usbfs,uint16_t depth)
{
    p_hw_usbfs->grxfsiz = depth;
}

/**
 * \brief  获取RxFIFO深度
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return RxFIFO深度
 */
am_static_inline
uint16_t amhw_hc32_usbfs_rxfd_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (uint16_t)(p_hw_usbfs->grxfsiz & 0x3ff);
}

/**
 * \brief  设置非周期发送FIFO大小
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_nptfd_set(amhw_hc32_usbfs_t *p_hw_usbfs,uint16_t depth)
{
    p_hw_usbfs->gnptxfsiz = (depth << 16);
}

/**
 * \brief  获取非周期发送FIFO大小
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return 非周期发送FIFO大小
 */
am_static_inline
uint16_t amhw_hc32_usbfs_nptfd_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (uint16_t)((p_hw_usbfs->gnptxfsiz >> 16) & 0x3ff);
}

/**
 * \brief  设置产品ID
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_cid_set(amhw_hc32_usbfs_t *p_hw_usbfs,uint32_t id)
{
    p_hw_usbfs->cid = id;
}

/**
 * \brief  设置IN端点TxFIFO深度
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] depth      : FIFO深度值
 * \param[in] n          : 对应FIFO寄存器编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_ineptxfd_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint16_t           depth,
                                  uint8_t            n)
{
    switch(n)
    {
        case 1: 
            p_hw_usbfs->dptxfsiz1 =(p_hw_usbfs->dptxfsiz1 &(~(0xfffful<<16))) |
                                   (depth << 16);
            break;
        case 3:
            p_hw_usbfs->dptxfsiz2 =(p_hw_usbfs->dptxfsiz2 &(~(0xfffful<<16))) |
                                   (depth << 16);
            break;
        case 5:
            p_hw_usbfs->dptxfsiz3 =(p_hw_usbfs->dptxfsiz3 &(~(0xfffful<<16))) |
                                   (depth << 16);
            break;
        case 7:
            p_hw_usbfs->dptxfsiz4 =(p_hw_usbfs->dptxfsiz4 &(~(0xfffful<<16))) |
                                   (depth << 16);
            break;
        default: break;
    }
}

/**
 * \brief  设置IN端点TxFIFORAM起始地址
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] address    : IN端点TxFIFORAM起始地址
 * \param[in] n          : 对应FIFO寄存器编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_ineptxsa_set(amhw_hc32_usbfs_t *p_hw_usbfs,
	                                uint16_t           address,
                                  uint8_t            n)
{
    switch(n)
    {
        case 1: p_hw_usbfs->dptxfsiz1 =(p_hw_usbfs->dptxfsiz1 &(~0xffff)) | 
                                       address;
                break;
        case 3: p_hw_usbfs->dptxfsiz2 =(p_hw_usbfs->dptxfsiz2 &(~0xffff)) |
                                       address;
                break;
        case 5: p_hw_usbfs->dptxfsiz3 =(p_hw_usbfs->dptxfsiz3 &(~0xffff)) |
                                       address;
                break;
        case 7: p_hw_usbfs->dptxfsiz4 =(p_hw_usbfs->dptxfsiz4 &(~0xffff)) |
                                       address;
                break;
        default: break;
    }
}

/**
 * \brief USBFS帧间隔定义
 */
typedef enum {
    AMHW_HC32F07X_USBFS_PFIVL80   = 0, /* 80%帧间隔 */
    AMHW_HC32F07X_USBFS_PFIVL85   = 2, /* 85%帧间隔 */
    AMHW_HC32F07X_USBFS_PFIVL90   = 1, /* 90%帧间隔 */
    AMHW_HC32F07X_USBFS_PFIVL95   = 3, /* 95%帧间隔 */
}amhw_hc32f07x_usbfs_pfivl_t;

/**
 * \brief  设置周期性帧间隔
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考枚举量amhw_hc32f07x_usbfs_pfivl_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dfivl_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                               amhw_hc32f07x_usbfs_pfivl_t flag)
{
    p_hw_usbfs->dcfg =(p_hw_usbfs->dcfg & (~(0x03 << 11))) | (flag << 11);
}

/**
 * \brief  设置设备地址
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] address    : 设备地址
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dad_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                             uint8_t            address)
{
    p_hw_usbfs->dcfg =(p_hw_usbfs->dcfg & (~(0x7f << 4))) | (address << 4);
}

/**
 * \brief  非零长度状态OUT握手信号
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 0,收到数据发给应用程序，根据NAK和STALL回复握手信号
 *                         1,不将数据发给应用程序，恢复STALL握手信号
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_nzlsohsk_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint8_t            flag)
{
    p_hw_usbfs->dcfg =(p_hw_usbfs->dcfg & (~(0x01 << 2))) | (flag << 2);
}

/**
 * \brief  设备速度设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dspd_set(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    p_hw_usbfs->dcfg |= 0x03;
}



/**
 * \brief 设备控制定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_WRITE_POPRGDNE  (1U << 11) /* 上电编程完成 */
#define AMHW_HC32F07X_USBFS_WRITE_CGONAK    (1U << 10) /* 清零全局OUT NAK */
#define AMHW_HC32F07X_USBFS_WRITE_SGONAK    (1U << 9)  /* 置位全局OUT NAK */
#define AMHW_HC32F07X_USBFS_WRITE_CGINAK    (1U << 8)  /* 清零全局IN NAK */
#define AMHW_HC32F07X_USBFS_WRITE_SGINAK    (1U << 7)  /* 置位全局IN NAK */

/**
 * \brief  设备控制
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_WRITE_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dctl_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                              uint16_t           flag)
{
    p_hw_usbfs->dctl |= flag;
}

/**
 * \brief 设备控制定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_DCTL_SDIS      (1U << 1)  /* 软断连 */
#define AMHW_HC32F07X_USBFS_DCTL_RWUSIG    (1U << 0)  /* 远程唤醒 */

/**
 * \brief  设备控制使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_CTL_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dctl_enable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                 uint16_t           flag)
{
    p_hw_usbfs->dctl |= flag;
}

/**
 * \brief  设备控制禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_CTL_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_dctl_disable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint16_t           flag)
{
    p_hw_usbfs->dctl &= ~flag;
}

/**
 * \brief 设备控制状态定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_READ_POPRGDNE  (1U << 11) /* 上电编程完成 */
#define AMHW_HC32F07X_USBFS_READ_GONSTS    (1U << 3)  /* 全局OUT NAK状态 */
#define AMHW_HC32F07X_USBFS_READ_GINSTS    (1U << 2)  /* 全局IN NAK状态 */

/**
 * \brief  设备控制状态读取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_READ_
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_dctl_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                   uint16_t           flag)
{
    return ((p_hw_usbfs->dctl & flag) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief  接收SOF的帧编号
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return SOF帧编号
 */
am_static_inline
uint16_t amhw_hc32_usbfs_fnsof_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (uint16_t)((p_hw_usbfs->dsts >> 8) & 0x3fff);
}

/**
 * \brief  获取不定错误标志
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_eerr_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return  (((p_hw_usbfs->dsts >> 3) & 0x01) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief  获取枚举速度
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return SOF帧编号
 */
am_static_inline
uint8_t amhw_hc32_usbfs_enumspd_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return (uint8_t)((p_hw_usbfs->dsts >> 1) & 0x3);
}

/**
 * \brief  获取挂起状态
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_suspsts_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{
    return  (((p_hw_usbfs->dsts >> 0) & 0x01) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 设备IN端点通用中断定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_EP_IN_INEPNEM    (1U << 6) /* IN端点NAK有效中断 */
#define AMHW_HC32F07X_USBFS_EP_IN_INEPNNM    (1U << 5) /* EP不匹配时接收到IN令牌中断 */
#define AMHW_HC32F07X_USBFS_EP_IN_ITTXFEMSK  (1U << 4) /* TxFIFO为空接收到IN令牌中断 */
#define AMHW_HC32F07X_USBFS_EP_IN_TOM        (1U << 3) /* 超时中断*/
#define AMHW_HC32F07X_USBFS_EP_IN_EPDM       (1U << 1) /* 端点禁止中断 */
#define AMHW_HC32F07X_USBFS_EP_IN_XFRCM      (1U << 0) /* 传输完成中断 */
#define AMHW_HC32F07X_USBFS_EP_IN_ALL        (0x0b) /* 所有IN端点中断 */

/**
 * \brief  设备IN端点通用中断使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_IN_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_diepmsk_enable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint16_t           flag)
{
    p_hw_usbfs->diepmsk |= flag;
}

/**
 * \brief  设备IN端点通用中断禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_IN_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_diepmsk_disable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                     uint16_t           flag)
{
    p_hw_usbfs->diepmsk &= ~flag;
}

/**
 * \brief 设备OUT端点通用中断定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_EP_OUT_OTEPDM (1U << 4) /* 端点禁止时接收到OUT令牌中断 */
#define AMHW_HC32F07X_USBFS_EP_OUT_STUPM  (1U << 3) /* SETUP阶段完成中断 */
#define AMHW_HC32F07X_USBFS_EP_OUT_EPDM   (1U << 1) /* 端点禁止中断 */
#define AMHW_HC32F07X_USBFS_EP_OUT_XFRCM  (1U << 0) /* 传输完成中断 */
#define AMHW_HC32F07X_USBFS_EP_OUT_ALL    (0X2b)    /* 所有OUT端点中断 */

/**
 * \brief  设备OUT端点通用中断使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_OUT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_doepmsk_enable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint16_t           flag)
{
    p_hw_usbfs->doepmsk |= flag;
}

/**
 * \brief  设备OUT端点通用中断禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_IN_
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_doepmsk_disable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                     uint16_t           flag)
{
    p_hw_usbfs->doepmsk &= ~flag;
}

/**
 * \brief 设置OUT端点中断位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应OUT端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_oepint_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                uint16_t           num)
{       
    p_hw_usbfs->daint |= (0x1 << (16+num)); 
} 

/**
 * \brief 设置IN端点中断位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应IN端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_iepint_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                uint16_t           num)
{       
    p_hw_usbfs->daint |= (0x1 << num); 
}


/**
 * \brief 获取OUT端点中断位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应OUT端点编号
 *
 * \return none
 */
am_static_inline
uint16_t amhw_hc32_usbfs_oepint_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{       
    return (uint16_t)(p_hw_usbfs->daint >> 16);
}

/**
 * \brief 获取IN端点中断位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应IN端点编号
 *
 * \return none
 */
am_static_inline
uint16_t amhw_hc32_usbfs_iepint_get(amhw_hc32_usbfs_t *p_hw_usbfs)
{       
    return (uint16_t)(p_hw_usbfs->daint);
}

/**
 * \brief  设置OUT端点中断使能位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应OUT端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_oepintm_enable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                      uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->daintmsk |= (1u << 16); 
                break;
        case 2: p_hw_usbfs->daintmsk |= (1u << 18); 
                break;
        case 4: p_hw_usbfs->daintmsk |= (1u << 20);
                break;
        case 6: p_hw_usbfs->daintmsk |= (1u << 22);
                break;
        case 8: p_hw_usbfs->daintmsk |= (1u << 24);
                break;
        default: break;
    }
}

/**
 * \brief  设置OUT端点中断禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应OUT端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_oepintm_disable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                      uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->daintmsk &= ~(1u << 16); 
                break;
        case 2: p_hw_usbfs->daintmsk &= ~(1u << 17); 
                break;
        case 4: p_hw_usbfs->daintmsk &= ~(1u << 18);
                break;
        case 6: p_hw_usbfs->daintmsk &= ~(1u << 19);
                break;
        case 8: p_hw_usbfs->daintmsk &= ~(1u << 20);
                break;
        default: break;
    }
}

/**
 * \brief  设置IN端点中断使能位
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应IN端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_iepintm_enable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                      uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->daintmsk |= (1u << 0); 
                break;
        case 1: p_hw_usbfs->daintmsk |= (1u << 1); 
                break;
        case 3: p_hw_usbfs->daintmsk |= (1u << 3);
                break;
        case 5: p_hw_usbfs->daintmsk |= (1u << 5);
                break;
        case 7: p_hw_usbfs->daintmsk |= (1u << 7);
                break;
        default: break;
    }
}

/**
 * \brief  设置IN端点中断禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应IN端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_iepintm_disable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                      uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->daintmsk &= ~(1u << 0); 
                break;
        case 1: p_hw_usbfs->daintmsk &= ~(1u << 1); 
                break;
        case 3: p_hw_usbfs->daintmsk &= ~(1u << 2);
                break;
        case 5: p_hw_usbfs->daintmsk &= ~(1u << 3);
                break;
        case 7: p_hw_usbfs->daintmsk &= ~(1u << 4);
                break;
        default: break;
    }
}

/**
 * \brief  设置IN端点FIFO空中断使能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应IN端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_neptxfem_enable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                      uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepempmsk |= (1u << 0); 
                break;
        case 1: p_hw_usbfs->diepempmsk |= (1u << 1); 
                break;
        case 3: p_hw_usbfs->diepempmsk |= (1u << 2);
                break;
        case 5: p_hw_usbfs->diepempmsk |= (1u << 3);
                break;
        case 7: p_hw_usbfs->diepempmsk |= (1u << 4);
                break;
        default: break;
    }
}

/**
 * \brief  设置IN端点FIFO空中断禁能
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 对应IN端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_neptxfem_disable(amhw_hc32_usbfs_t *p_hw_usbfs,
                                      uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepempmsk &= ~(1u << 0); 
                break;
        case 1: p_hw_usbfs->diepempmsk &= ~(1u << 1); 
                break;
        case 3: p_hw_usbfs->diepempmsk &= ~(1u << 2);
                break;
        case 5: p_hw_usbfs->diepempmsk &= ~(1u << 3);
                break;
        case 7: p_hw_usbfs->diepempmsk &= ~(1u << 4);
                break;
        default: break;
    }
}

/**
 * \brief 设备端点控制位定义
 *  @{
 */

#define AMHW_HC32F07X_USBFS_CTL_EPENA  (1U << 31) /* 端点使能 */
#define AMHW_HC32F07X_USBFS_CTL_EPDIS  (1U << 30) /* 端点禁止 (OUT端点0除外)*/
#define AMHW_HC32F07X_USBFS_CTL_SNAK   (1U << 27) /* 置位NAK */
#define AMHW_HC32F07X_USBFS_CTL_CNAK   (1U << 26) /* 清零NAK */
#define AMHW_HC32F07X_USBFS_CTL_STALL  (1U << 21) /* STALL握手 */

/* 端点0不可对以下位设置 */
#define AMHW_HC32F07X_USBFS_CTL_SODDFRM (1U << 29) /* 设置奇数帧 */
#define AMHW_HC32F07X_USBFS_CTL_SD0PID  (1U << 28) /* 将端点数据PID设为DATA0 */
#define AMHW_HC32F07X_USBFS_CTL_SEVNFRM (1U << 28) /* 设置偶数帧*/

/* 只可对OUT端点进行以下位设置 */
#define AMHW_HC32F07X_USBFS_CTL_SNPM    (1U << 20) /* 监听模式 */
#define AMHW_HC32F07X_USBFS_CTL_SNPMCLR (~(1U << 20)) /* 监听模式清除 */

/**
 * \brief  设备IN端点控制位设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_CTL_
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_diepctl_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint32_t           flag,
                                  uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepctl0 |= flag;
                break;
        case 1: p_hw_usbfs->diepctl1 |= flag; 
                break;
        case 3: p_hw_usbfs->diepctl2 |= flag;
                break;
        case 5: p_hw_usbfs->diepctl3 |= flag;
                break;
        case 7: p_hw_usbfs->diepctl4 |= flag;
                break;
        default: break;
    }
}

/**
 * \brief  设备IN端点控制位清除
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_CTL_
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_diepctl_clr(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint32_t           flag,
                                  uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepctl0 &= ~flag;
                break;
        case 1: p_hw_usbfs->diepctl1 &= ~flag; 
                break;
        case 3: p_hw_usbfs->diepctl2 &= ~flag;
                break;
        case 5: p_hw_usbfs->diepctl3 &= ~flag;
                break;
        case 7: p_hw_usbfs->diepctl4 &= ~flag;
                break;
        default: break;
    }
}

/**
 * \brief  
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_nextep_set(amhw_hc32_usbfs_t *p_hw_usbfs,
	                               uint8_t           ep_num,
                                  uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepctl0 = (p_hw_usbfs->diepctl0 & ~(0xf << 11)) | 
					                              (ep_num <<11);
                break;
        case 1: p_hw_usbfs->diepctl1 = (p_hw_usbfs->diepctl1 & ~(0xf << 11)) | 
					                              (ep_num <<11); 
                break;
        case 3: p_hw_usbfs->diepctl2 = (p_hw_usbfs->diepctl2 & ~(0xf << 11)) | 
					                              (ep_num <<11);
                break;
        case 5: p_hw_usbfs->diepctl3 = (p_hw_usbfs->diepctl3 & ~(0xf << 11)) | 
					                              (ep_num <<11);
                break;
        case 7: p_hw_usbfs->diepctl4 = (p_hw_usbfs->diepctl4 & ~(0xf << 11)) | 
					                              (ep_num <<11);
                break;
        default: break;
    }
}

/**
 * \brief  设备OUT端点控制位设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_CTL_
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_doepctl_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint32_t           flag,
                                  uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepctl0 |= flag;
                break;
        case 2: p_hw_usbfs->doepctl1 |= flag; 
                break;
        case 4: p_hw_usbfs->doepctl2 |= flag;
                break;
        case 6: p_hw_usbfs->doepctl3 |= flag;
                break;
        case 8: p_hw_usbfs->doepctl4 |= flag;
                break;
        default: break;
    }
}

/**
 * \brief  设备OUT端点控制位清除
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_CTL_
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_doepctl_clr(amhw_hc32_usbfs_t *p_hw_usbfs,
	                                uint32_t           flag,
                                  uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepctl0 &= ~flag;		
                break;
        case 2: p_hw_usbfs->doepctl1 &= ~flag;	
                break;
        case 4: p_hw_usbfs->doepctl2 &= ~flag;
                break;
        case 6: p_hw_usbfs->doepctl3 &= ~flag;
                break;
        case 8: p_hw_usbfs->doepctl4 &= ~flag;
                break;
        default: break;
    }
}

/**
 * \brief  TxFIFO编号设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] tfifo_num  : TxFIFO编号
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_in_txfnum_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint8_t           tfifo_num,
                                  uint8_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepctl0 = (p_hw_usbfs->diepctl0 & (~(0xf<<22))) | 
                                       ((tfifo_num & 0xf) << 22);
                break;
        case 1: p_hw_usbfs->diepctl1 = (p_hw_usbfs->diepctl1 & (~(0xf<<22))) | 
                                       ((tfifo_num & 0xf) << 22);
                break;
        case 3: p_hw_usbfs->diepctl2 = (p_hw_usbfs->diepctl2 & (~(0xf<<22))) |
                                       ((tfifo_num & 0xf) << 22);
                break;
        case 5: p_hw_usbfs->diepctl3 = (p_hw_usbfs->diepctl3 & (~(0xf<<22))) |
                                       ((tfifo_num & 0xf) << 22);
                break;
        case 7: p_hw_usbfs->diepctl4 = (p_hw_usbfs->diepctl4 & (~(0xf<<22))) |
                                        ((tfifo_num & 0xf) << 22);
                break;
        default: break;
    }    
}

/**
 * \brief USBFS端点类型定义
 */
typedef enum {
    AMHW_HC32F07X_USBFS_EPTYPE_CTRL  = 0, /* 控制 */
    AMHW_HC32F07X_USBFS_EPTYPE_ISOC  = 1, /* 同步 */
    AMHW_HC32F07X_USBFS_EPTYPE_BULK  = 2, /* 批量 */
    AMHW_HC32F07X_USBFS_EPTYPE_INTR  = 3, /* 中断 */
}amhw_hc32f07x_usbfs_eptype_t;

/**
 * \brief  设备IN端点类型获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return 设备端点类型
 */
am_static_inline
void amhw_hc32_usbfs_in_eptyp_set(amhw_hc32_usbfs_t *p_hw_usbfs, 
                                  amhw_hc32f07x_usbfs_eptype_t type,
                                  uint8_t num)
{

    switch(num)
    {
       case 0: p_hw_usbfs->diepctl0 = 
                               ((p_hw_usbfs->diepctl0 & (~(0x03 << 18))) |
                               type << 18);
               break;
        case 1: p_hw_usbfs->diepctl1 = 
                               ((p_hw_usbfs->diepctl1 & (~(0x03 << 18))) |
                                type << 18);
                break;
        case 3: p_hw_usbfs->diepctl2 =
                               ((p_hw_usbfs->diepctl2 & (~(0x03 << 18))) |
                               type << 18);
                break;
        case 5: p_hw_usbfs->diepctl3 =
                               ((p_hw_usbfs->diepctl3 & (~(0x03 << 18))) |
                               type << 18);
                break;
        case 7: p_hw_usbfs->diepctl4 =
                               ((p_hw_usbfs->diepctl4 & (~(0x03 << 18))) |
                               type << 18);
                break;
        default: break;
    }
}

/**
 * \brief  设备OUT端点类型获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return 设备端点类型
 */
am_static_inline
void amhw_hc32_usbfs_out_eptyp_set(amhw_hc32_usbfs_t *p_hw_usbfs, 
                                   amhw_hc32f07x_usbfs_eptype_t type,
                                   uint8_t num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepctl0 = 
                                ((p_hw_usbfs->doepctl0 & (~(0x03 << 18))) |
                                type << 18);
                break;
        case 2: p_hw_usbfs->doepctl1 = 
                                ((p_hw_usbfs->doepctl1 & (~(0x03 << 18))) |
                                type << 18);
                break;
        case 4: p_hw_usbfs->doepctl2 =
                                ((p_hw_usbfs->doepctl2 & (~(0x03 << 18))) |
                                type << 18);
                break;
        case 6: p_hw_usbfs->doepctl3 =
                                ((p_hw_usbfs->doepctl3 & (~(0x03 << 18))) |
                                type << 18);
                break;
        case 8: p_hw_usbfs->doepctl4 =
                                ((p_hw_usbfs->doepctl4 & (~(0x03 << 18))) |
                                type << 18);
                break;
        default: break;
    }
}

/**
 * \brief  设备IN端点NAK状态获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return AM_TRUE :回复NAK握手
 *         AM_FALSE:根据FIFO状态回复非NAK握手
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_in_naksts_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                        uint8_t            num)
{
    am_bool_t naksts = AM_FALSE;
    switch(num)
    {
        case 0: naksts = (((p_hw_usbfs->diepctl0 >> 17) & 
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 1: naksts = (((p_hw_usbfs->diepctl1 >> 17) & 
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 3: naksts = (((p_hw_usbfs->diepctl2 >> 17) &
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 5: naksts = (((p_hw_usbfs->diepctl3 >> 17) &
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 7: naksts = (((p_hw_usbfs->diepctl4 >> 17) &
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        default: break;
    }
    return naksts;
}

/**
 * \brief  设备OUT端点NAK状态获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 *
 * \return AM_TRUE :回复NAK握手
 *         AM_FALSE:根据FIFO状态回复非NAK握手
 */
am_static_inline
am_bool_t amhw_hc32_usbfs_out_naksts_get(amhw_hc32_usbfs_t *p_hw_usbfs, uint8_t num)
{
	  am_bool_t naksts = AM_FALSE;
    switch(num)
    {
        case 0: naksts = (((p_hw_usbfs->doepctl0 >> 17) & 
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 2: naksts = (((p_hw_usbfs->doepctl1 >> 17) & 
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 4: naksts = (((p_hw_usbfs->doepctl2 >> 17) &
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 6: naksts = (((p_hw_usbfs->doepctl3 >> 17) &
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        case 8: naksts = (((p_hw_usbfs->doepctl4 >> 17) &
                         0x01) ? AM_TRUE : AM_FALSE);
                break;
        default: break;
    }
    return naksts;
}

/**
 * \brief  设备IN活动端点设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_in_usbaep_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                   uint16_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepctl0 |= 1u << 15;
                break;
        case 1: p_hw_usbfs->diepctl1 |= 1u << 15;
                break;
        case 3: p_hw_usbfs->diepctl2 |= 1u << 15;
                break;
        case 5: p_hw_usbfs->diepctl3 |= 1u << 15;
                break;
        case 7: p_hw_usbfs->diepctl4 |= 1u << 15;
                break;
        default: break;
    }
}

/**
 * \brief  设备OUT活动端点设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_out_usbaep_set(amhw_hc32_usbfs_t *p_hw_usbfs,
	                                   uint16_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepctl0 |= 1u << 15;
                break;
        case 2: p_hw_usbfs->doepctl1 |= 1u << 15;
                break;
        case 4: p_hw_usbfs->doepctl2 |= 1u << 15;
                break;
        case 6: p_hw_usbfs->doepctl3 |= 1u << 15;
                break;
        case 8: p_hw_usbfs->doepctl4 |= 1u << 15;
                break;
        default: break;
    }
}


/**
 * \brief  IN端点最大数据包大小设置
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] size       : 参考枚举量amhw_hc32f07x_usbfs_mpsiz_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_in_mpsiz_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                  uint8_t            size,
                                  uint8_t            num)
{
    if(num==0){
        if(size >= 64){
            size = 0;
        }
        else if(size >= 32){
            size = 1;
        }
        else if(size >= 16){
            size = 2;
        }
        else {
            size = 3;
        }
    }
    switch(num)
    {
        case 0: p_hw_usbfs->diepctl0 = (p_hw_usbfs->diepctl0 & (~0x03)) | 
                                       (size & 0x03);
                break;
        case 1: p_hw_usbfs->diepctl1 = (p_hw_usbfs->diepctl1 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        case 3: p_hw_usbfs->diepctl2 = (p_hw_usbfs->diepctl2 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        case 5: p_hw_usbfs->diepctl3 = (p_hw_usbfs->diepctl3 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        case 7: p_hw_usbfs->diepctl4 = (p_hw_usbfs->diepctl4 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        default: break;
    }
}

/**
 * \brief  OUT端点最大数据包大小设置
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] size       : 参考枚举量amhw_hc32f07x_usbfs_mpsiz_t
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_out_mpsiz_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                   uint8_t            size,
                                   uint8_t            num)
{
    if(num==0){
        if(size >= 64){
            size = 0;
        }
        else if(size >= 32){
            size = 1;
        }
        else if(size >= 16){
            size = 2;
        }
        else {
            size = 3;
        }
    }
    switch(num)
    {
        case 0: p_hw_usbfs->doepctl0 = (p_hw_usbfs->doepctl0 & (~0x03)) | 
                                       (size & 0x03);
                break;
        case 2: p_hw_usbfs->doepctl1 = (p_hw_usbfs->doepctl1 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        case 4: p_hw_usbfs->doepctl2 = (p_hw_usbfs->doepctl2 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        case 6: p_hw_usbfs->doepctl3 = (p_hw_usbfs->doepctl3 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        case 8: p_hw_usbfs->doepctl4 = (p_hw_usbfs->doepctl4 & (~0x7ff)) |
                                       (size & 0x7ff);
                break;
        default: break;
    }
}

/**
 * \brief  IN端点中断类型获取
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : IN端点编号
 *
 * \return IN端点中断值
 */
am_static_inline
uint8_t amhw_hc32_usbfs_int_instat_get(amhw_hc32_usbfs_t *p_hw_usbfs, uint8_t num)
{
	  uint8_t stat = 0;
    switch(num)
    {
        case 0: stat = (p_hw_usbfs->diepint0);
                break;
        case 1: stat = (p_hw_usbfs->diepint1);
                break;
        case 3: stat = (p_hw_usbfs->diepint2);
                break;
        case 5: stat = (p_hw_usbfs->diepint3);
                break;
        case 7: stat = (p_hw_usbfs->diepint4);
                break;
        default: break;
    }
    return stat;
}

/**
 * \brief 设备IN端点中断定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_IN_INEPNE (1U << 6) /* IN端点NAK有效 */
#define AMHW_HC32F07X_USBFS_IN_ITTXFE (1U << 4) /* TxFIFO为空接收到IN令牌 */
#define AMHW_HC32F07X_USBFS_IN_TO     (1U << 3) /* 超时 */
#define AMHW_HC32F07X_USBFS_IN_EPDISD (1U << 1) /* 端点禁止中断 */
#define AMHW_HC32F07X_USBFS_IN_XFRC   (1U << 0) /* 传输完成中断 */
#define AMHW_HC32F07X_USBFS_IN_ALL    (0XFF) /* 传输完成中断 */

/**
 * \brief  设备IN端点中断清零
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_IN
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_diepint_clr(amhw_hc32_usbfs_t *p_hw_usbfs,
	                               uint8_t           flag,
	                               uint8_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepint0 |= flag;
                break;
        case 1: p_hw_usbfs->diepint1 |= flag;
                break;
        case 3: p_hw_usbfs->diepint2 |= flag;
                break;
        case 5: p_hw_usbfs->diepint3 |= flag;
                break;
        case 7: p_hw_usbfs->diepint4 |= flag;
                break;
        default: break;
    }
}

/**
 * \brief  设备IN端点数据包计数值设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] cnt        : 计数值
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_in_pktcnt_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                   uint16_t          cnt,
                                   uint8_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepsiz0 = (p_hw_usbfs->diepsiz0 & (~(0x03 << 19))) |
                                       ((cnt & 0x03)<< 19);
                break;
        case 1: p_hw_usbfs->diepsiz1 = (p_hw_usbfs->diepsiz1 & (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        case 3: p_hw_usbfs->diepsiz2 = (p_hw_usbfs->diepsiz2 & (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        case 5: p_hw_usbfs->diepsiz3 = (p_hw_usbfs->diepsiz3 & (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        case 7: p_hw_usbfs->diepsiz4 = (p_hw_usbfs->diepsiz4 & (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        default: break;
    }
}

/**
 * \brief  设备IN端点数据包计数值获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return 计数值
 */
am_static_inline
uint16_t amhw_hc32_usbfs_in_pktcnt_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                     uint8_t           num)
{
    uint16_t cnt = 0;
    switch(num)
    {
        case 0: cnt = ((p_hw_usbfs->diepsiz0 >> 19) & 0x03);
                break;
        case 1: cnt = ((p_hw_usbfs->diepsiz1 >> 19) & 0x3ff);
                break;
        case 3: cnt = ((p_hw_usbfs->diepsiz2 >> 19) & 0x3ff);
                break;
        case 5: cnt = ((p_hw_usbfs->diepsiz3 >> 19) & 0x3ff);
                break;
        case 7: cnt = ((p_hw_usbfs->diepsiz4 >> 19) & 0x3ff);
                break;
        default: break;
    }
    return cnt;
}

/**
 * \brief  设备IN端点传输大小设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] size       : 传输大小
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_in_xfrsiz_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                   uint32_t          size,
                                   uint8_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepsiz0 = (p_hw_usbfs->diepsiz0 & (~0x7f)) |
                                       ((size & 0x7f)<< 0);
                break;
        case 1: p_hw_usbfs->diepsiz1 = (p_hw_usbfs->diepsiz1 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        case 3: p_hw_usbfs->diepsiz2 = (p_hw_usbfs->diepsiz2 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        case 5: p_hw_usbfs->diepsiz3 = (p_hw_usbfs->diepsiz3 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        case 7: p_hw_usbfs->diepsiz4 = (p_hw_usbfs->diepsiz4 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        default: break;
    }
}

/**
 * \brief  设备IN端点传输大小获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return 传输大小值
 */
am_static_inline
uint32_t amhw_hc32_usbfs_in_xfrsiz_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                       uint8_t           num)
{
    uint32_t size = 0;
    switch(num)
    {
        case 0: size = (p_hw_usbfs->diepsiz0  & 0x7f);
                break;
        case 1: size = (p_hw_usbfs->diepsiz1  & 0x7ffff);
                break;
        case 3: size = (p_hw_usbfs->diepsiz2  & 0x7ffff);
                break;
        case 5: size = (p_hw_usbfs->diepsiz3  & 0x7ffff);
                break;
        case 7: size = (p_hw_usbfs->diepsiz4  & 0x7ffff);
                break;
        default: break;
    }
    return size;
}

/**
 * \brief  设备IN端点DMA地址设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] adderess   : DMA地址
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_in_dmaaddr_set(amhw_hc32_usbfs_t *p_hw_usbfs,
	                               uint32_t           adderess,
	                               uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->diepdma0 = adderess;
                break;
        case 1: p_hw_usbfs->diepdma1 = adderess;
                break;
        case 3: p_hw_usbfs->diepdma2 = adderess;
                break;
        case 5: p_hw_usbfs->diepdma3 = adderess;
                break;
        case 7: p_hw_usbfs->diepdma4 = adderess;
                break;
        default: break;
    }
}

/**
 * \brief  设备IN端点FIFO可用空间大小获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return 32位字为单位的可用空间
 */
am_static_inline
uint16_t amhw_hc32_usbfs_ineptfsize_set(amhw_hc32_usbfs_t *p_hw_usbfs,
	                                 uint8_t            num)
{
    uint16_t size = 0;
    switch(num)
    {
        case 0: size = p_hw_usbfs->dtxfsts0 & 0xffff;
                break;
        case 1: size = p_hw_usbfs->dtxfsts1 & 0xffff;
                break;
        case 3: size = p_hw_usbfs->dtxfsts2 & 0xffff;
                break;
        case 5: size = p_hw_usbfs->dtxfsts3 & 0xffff;
                break;
        case 7: size = p_hw_usbfs->dtxfsts4 & 0xffff;
                break;
        default: break;
    }
    return size;
}

/**
 * \brief 设备OUT端点中断定义
 *  @{
 */
#define AMHW_HC32F07X_USBFS_OUT_RECSTUP (1U << 6) /* 收到连续的setup数据包*/
#define AMHW_HC32F07X_USBFS_OUT_OTEPDIS (1U << 4) /* 端点禁止时接收到IN令牌 */
#define AMHW_HC32F07X_USBFS_OUT_STUP    (1U << 3) /* STUP阶段完成 */
#define AMHW_HC32F07X_USBFS_OUT_EPDISD  (1U << 1) /* 端点禁止中断 */
#define AMHW_HC32F07X_USBFS_OUT_XFRC    (1U << 0) /* 传输完成中断 */

/**
 * \brief  设备IN端点中断清零
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] flag       : 参考宏定义AMHW_HC32F07X_USBFS_OUT
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_doepint_clr(amhw_hc32_usbfs_t *p_hw_usbfs,
	                               uint8_t           flag,
	                               uint8_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepint0 |= flag;
                break;
        case 2: p_hw_usbfs->doepint1 |= flag;
                break;
        case 4: p_hw_usbfs->doepint2 |= flag;
                break;
        case 6: p_hw_usbfs->doepint3 |= flag;
                break;
        case 8: p_hw_usbfs->doepint4 |= flag;
                break;
        default: break;
    }
}

/**
 * \brief  OUT端点中断类型获取
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : OUT端点编号
 *
 * \return OUT端点中断值
 */
am_static_inline
uint8_t amhw_hc32_usbfs_int_outstat_get(amhw_hc32_usbfs_t *p_hw_usbfs,
		                                uint8_t num)
{
    uint8_t stat = 0;
    switch(num)
    {
        case 0: stat = (p_hw_usbfs->doepint0);
                break;
        case 2: stat = (p_hw_usbfs->doepint1);
                break;
        case 4: stat = (p_hw_usbfs->doepint2);
                break;
        case 6: stat = (p_hw_usbfs->doepint3);
                break;
        case 8: stat = (p_hw_usbfs->doepint4);
                break;
        default: break;
    }
    return stat;
}

/**
 * \brief  设备OUT端点数据包计数值设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] cnt        : 计数值
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_out_pktcnt_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint16_t          cnt,
                                    uint8_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepsiz0 = (p_hw_usbfs->doepsiz0 &
                                       (~(0x01 << 19))) |
                                       ((cnt & 0x01)<< 19);
                break;
        case 2: p_hw_usbfs->doepsiz1 = (p_hw_usbfs->doepsiz1 &
                                       (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        case 4: p_hw_usbfs->doepsiz2 = (p_hw_usbfs->doepsiz2 &
                                       (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        case 6: p_hw_usbfs->doepsiz3 = (p_hw_usbfs->doepsiz3 &
                                       (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        case 8: p_hw_usbfs->doepsiz4 = (p_hw_usbfs->doepsiz4 &
                                       (~(0x3ff << 19))) |
                                       ((cnt & 0x3ff)<< 19);
                break;
        default: break;
    }
}

/**
 * \brief  设备OUT端点数据包计数值获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return 计数值
 */
am_static_inline
uint16_t amhw_hc32_usbfs_out_pktcnt_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                        uint8_t           num)
{
    uint16_t cnt = 0;
    switch(num)
    {
        case 0: cnt = ((p_hw_usbfs->doepsiz0 >> 19) & 0x01);
                break;
        case 2: cnt = ((p_hw_usbfs->doepsiz1 >> 19) & 0x3ff);
                break;
        case 4: cnt = ((p_hw_usbfs->doepsiz2 >> 19) & 0x3ff);
                break;
        case 6: cnt = ((p_hw_usbfs->doepsiz3 >> 19) & 0x3ff);
                break;
        case 8: cnt = ((p_hw_usbfs->doepsiz4 >> 19) & 0x3ff);
                break;
        default: break;
    }
    return cnt;
}

/**
 * \brief  设备OUT端点传输大小设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] size       : 传输大小
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
uint32_t amhw_hc32_usbfs_out_xfrsiz_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint8_t           num)
{
	  uint32_t cnt = 0;
    switch(num)
    {
        case 0: cnt = (p_hw_usbfs->doepsiz0 & (0x7f));
                break;
        case 2: cnt = (p_hw_usbfs->doepsiz1 & (0x7ffff));
                break;
        case 4: cnt = (p_hw_usbfs->doepsiz2 & (0x7ffff));
                break;
        case 6: cnt = (p_hw_usbfs->doepsiz3 & (0x7ffff));
                break;
        case 8: cnt = (p_hw_usbfs->doepsiz4 & (0x7ffff));
                break;
        default: break;
    }
		return cnt;
}

/**
 * \brief  设备OUT端点传输大小设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] size       : 传输大小
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_out_xfrsiz_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint32_t          size,
                                    uint8_t           num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepsiz0 = (p_hw_usbfs->doepsiz0 & (~0x7f)) |
                                       ((size & 0x7f)<< 0);
                break;
        case 2: p_hw_usbfs->doepsiz1 = (p_hw_usbfs->doepsiz1 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        case 4: p_hw_usbfs->doepsiz2 = (p_hw_usbfs->doepsiz2 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        case 6: p_hw_usbfs->doepsiz3 = (p_hw_usbfs->doepsiz3 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        case 8: p_hw_usbfs->doepsiz4 = (p_hw_usbfs->doepsiz4 & (~0x7ffff)) |
                                       ((size & 0x7ffff)<< 0);
                break;
        default: break;
    }
}

/**
 * \brief  设备IN端点传输大小获取
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return 传输大小值
 */
am_static_inline
uint32_t amhw_hc32_usbfs_xfrsiz_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                    uint8_t           num)
{
    uint32_t size = 0;
    switch(num)
    {
        case 0: size = (p_hw_usbfs->diepsiz0  & 0x7f);
                break;
        case 1: size = (p_hw_usbfs->diepsiz1  & 0x7ffff);
                break;
        case 3: size = (p_hw_usbfs->diepsiz2  & 0x7ffff);
                break;
        case 5: size = (p_hw_usbfs->diepsiz3  & 0x7ffff);
                break;
        case 7: size = (p_hw_usbfs->diepsiz4  & 0x7ffff);
                break;
        default: break;
    }
    return size;
}

/**
 * \brief USBFS SETUP数据包计数
 */
typedef enum {
    AMHW_HC32F07X_USBFS_SETUP_CNT1  = 1, /* 1个数据包 */
    AMHW_HC32F07X_USBFS_SETUP_CNT2  = 2, /* 1个数据包 */
    AMHW_HC32F07X_USBFS_SETUP_CNT3  = 3, /* 1个数据包 */
}amhw_hc32f07x_usbfs_setup_t;

/**
 * \brief  设备OUT端点0SETUP计数包设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_out0_stuocnt_get(amhw_hc32_usbfs_t *p_hw_usbfs,
                                      amhw_hc32f07x_usbfs_setup_t cnt)
{
    p_hw_usbfs->doepsiz0 = (p_hw_usbfs->doepsiz0 & (~(0x03 << 29))) |
                           ((cnt & 0x03)<< 29);
}

/**
 * \brief  设备OUT端点DMA地址设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] adderess   : DMA地址
 * \param[in] num        : 端点编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_out_dmaaddr_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                     uint32_t           adderess,
                                     uint8_t            num)
{
    switch(num)
    {
        case 0: p_hw_usbfs->doepdma0 = adderess;
                break;
        case 2: p_hw_usbfs->doepdma1 = adderess;
                break;
        case 4: p_hw_usbfs->doepdma2 = adderess;
                break;
        case 6: p_hw_usbfs->doepdma3 = adderess;
                break;
        case 8: p_hw_usbfs->doepdma4 = adderess;
                break;
        default: break;
    }
}

/**
 * \brief  设备门控时钟设置
 *
 * \param[in] p_hw_usbfs : 指向usbfs寄存器的指针
 * \param[in] en         : AM_TRUE :启动HCLK和PHY时钟
 *                         AM_FALSE:停止HCLK和PHY时钟
 *
 * \return none
 */
am_static_inline
void amhw_hc32_usbfs_gatclk_set(amhw_hc32_usbfs_t *p_hw_usbfs,
                                am_bool_t          en)
{
    if(en == AM_TRUE){
        p_hw_usbfs->pcgcctl &= ~0x03;
    }else {
        p_hw_usbfs->pcgcctl |= 0x03;
    }
}


 /**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__GNUC__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_HC32_USBFS_H */

#endif

/* end of file */
