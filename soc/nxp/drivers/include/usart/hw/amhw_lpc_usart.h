/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 串口（USART）操作接口
 *
 * 1. 支持7、8、9三种数据宽度，支持1位或2位停止位；
 * 2. 主机模式和从机模式均支持同步模式传输，包括数据相位选择和连续时钟选项；
 * 3. 多主机模式(9位数据位)支持软件地址匹配；
 * 4. 支持RS-485 收发器输出使能；
 * 5. 奇偶校验的产生和检查：支持奇校验、偶校验和无校验；
 * 6. 在异步模式下，软件可编程过采样时钟数，支持5 ~ 16时钟采样；
 * 7. 一个发送数据缓冲器和一个接收数据缓冲器；
 * 8. 用于自动流控制的 RTS/CTS 硬件信号；通过增量 CTS 检测、发送禁用控制以及作为
      RTS 输出的任意 GPIO 可进行软件流控制；
 * 9. 接收的数据和状态可以选择性地从单个寄存器中读取；
 * 10. 中止产生及检测；
 * 11. 接收数据采用2~3次采样，一个样品不同状态置位；
 * 12. 内建波特率产生器，支持自动波特率特性；
 * 13. 一个小数分频器用于所有的串口，用于产生更高精度的波特率；
 * 14. 中断(Interrupt)支持: 接收准备就绪， 发送准备就绪，接收空闲，接收断点检
 *     测到变化，帧错误，奇偶校验错误，溢出，欠载，增量 CTS 检测，接收采样噪声
 *     检测；
 * 15. 回路模式，用于测试数据和流控制；
 * 16. USART 发送和接收可以使用DMA控制器。
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-03  bob, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_USART_H
#define __AMHW_LPC_USART_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc_if_usart
 * \copydoc amhw_lpc_usart.h
 * @{
 */

/**
 * \brief USART 寄存器块结构体
 */
typedef struct amhw_lpc_usart {
    __IO uint32_t cfg;         /**< \brief USART 配置寄存器             */
    __IO uint32_t ctl;         /**< \brief USART 控制寄存器             */
    __IO uint32_t stat;        /**< \brief USART 状态寄存器             */
    __IO uint32_t intenset;    /**< \brief 中断使能寄存器               */
    __O  uint32_t intenclr;    /**< \brief 中断使能位清除寄存器         */
    __I  uint32_t rxdat;       /**< \brief 接收数据寄存器.              */
    __I  uint32_t rxdatstat;   /**< \brief 接收数据和状态寄存器         */
    __IO uint32_t txdat;       /**< \brief 发送数据寄存器               */
    __IO uint32_t brg;         /**< \brief 波特率产生寄存器             */
    __I  uint32_t intstat;     /**< \brief 中断状态寄存器               */
    __IO uint32_t osr;         /**< \brief 过采样选择寄存器             */
    __IO uint32_t addr;        /**< \brief 地址寄存器                   */
} amhw_lpc_usart_t;


/**
 * \name USART 基本配置寄存器位定义
 * \anchor grp_amhw_lpc_usart_cfg
 * @{
 */

/** \brief USART使能      */
#define AMHW_LPC_USART_CFG_ENABLE       AM_BIT(0)      

/** \brief 7位数据长度    */
#define AMHW_LPC_USART_CFG_7BIT         AM_SBF(0UL, 2) 

/** \brief 8位数据长度    */
#define AMHW_LPC_USART_CFG_8BIT         AM_SBF(1UL, 2) 

/** \brief 9位数据长度    */
#define AMHW_LPC_USART_CFG_9BIT         AM_SBF(2UL, 2) 

/** \brief 不使用奇偶校验 */
#define AMHW_LPC_USART_CFG_PARITY_NO    AM_SBF(0UL, 4) 

/** \brief 偶校验         */
#define AMHW_LPC_USART_CFG_PARITY_EVEN  AM_SBF(2UL, 4) 

 /** \brief 奇校验         */
#define AMHW_LPC_USART_CFG_PARITY_ODD   AM_SBF(3UL, 4)

/** \brief 1位停止位      */
#define AMHW_LPC_USART_CFG_STOP_1       AM_SBF(0UL, 6) 

/** \brief 2位停止位      */
#define AMHW_LPC_USART_CFG_STOP_2       AM_SBF(1UL, 6) 

/** \brief Use 32KHz RTC clock, the asynchronous clock source is RTC clock */
#define AMHW_LPC_USART_CFG_MODE32K      AM_BIT(7)

/** \brief 使能硬件流控制 */
#define AMHW_LPC_USART_CFG_HWFLOW       AM_BIT(9)   
  
/** \brief 使能异步模式   */
#define AMHW_LPC_USART_CFG_AYNMODE      AM_SBF(0UL, 11) 

/** \brief 使能同步模式   */
#define AMHW_LPC_USART_CFG_SYNMODE      AM_SBF(1UL, 11) 

/**
 * \brief 用于同步模式，数据采样在SCLK的下降沿 
 *        (必须与 #AMHW_LPC_USART_CFG_SYNMODE 宏一起使用，不可单独使用)
 */
#define AMHW_LPC_USART_CFG_FALLING      AM_SBF(0UL, 12)

/**
 * \brief 用于同步模式，数据采样在SCLK的上升沿 
 *        (必须与 #AMHW_LPC_USART_CFG_SYNMODE 宏一起使用，不可单独使用)
 */
#define AMHW_LPC_USART_CFG_RISING       AM_SBF(1UL, 12)

/**
 * \brief 用于同步模式，串口工作在主机模式
 *        (必须与 #AMHW_LPC_USART_CFG_SYNMODE 宏一起使用，不可单独使用)
 */
#define AMHW_LPC_USART_CFG_MASTER       AM_SBF(1UL, 14)

/**
 * \brief 用于同步模式，串口工作在从机模式        
 *        (必须与 #AMHW_LPC_USART_CFG_SYNMODE 宏一起使用，不可单独使用)
 */
#define AMHW_LPC_USART_CFG_SLAVE        AM_SBF(0UL, 14)

/** \brief 串口将工作在回路模式(LoopBack mode) */
#define AMHW_LPC_USART_CFG_LOOPBACK     AM_SBF(1UL, 15)

/**
 * \brief 用于RS-485模式: 输出使能失效(De-assertion)信号将会在延迟一个字符时间，
 *        需要与 #AMHW_LPC_USART_CFG_OESEL 宏一起使用
 */
#define AMHW_LPC_USART_CFG_OETA         AM_SBF(1UL, 18)

/**
 * \brief 自动地址匹配使能，当地址匹配模式被使能后（#AMHW_LPC_USART_CTL_ADDRDET）。
 *        地址匹配通过硬件完成，使用地址寄存器中的值完成地址匹配
 */
#define AMHW_LPC_USART_CFG_AUTOADDR     AM_SBF(1UL, 19)

/**
 * \brief 用于RS-485模式: RTS 用于控制RS-485收发器的输出使能
 */
#define AMHW_LPC_USART_CFG_OESEL        AM_SBF(1UL, 20)

/**
 * \brief 用于RS-485模式: 输出使能信号高电平有效，否则，默认情况下是低电平有效
 */
#define AMHW_LPC_USART_CFG_OEPOLHIGH    AM_SBF(1UL, 21)

/**
 * \brief 反转接收数据管脚的电平极性
 */
#define AMHW_LPC_USART_CFG_RXPOL        AM_SBF(1UL, 22)

/**
 * \brief 反转发送数据管脚的电平极性
 */
#define AMHW_LPC_USART_CFG_TXPOL        AM_SBF(1UL, 23)

/** @} */


/**
 * \name USART控制标志，这些标志用于函数 #amhw_lpc_usart_control()
 * \anchor grp_amhw_lpc_usart_ctl
 * @{
 */

/** \brief 持续break模式        */
#define AMHW_LPC_USART_CTL_TXBRKEN    AM_BIT(1)

/** \brief 使能地址匹配模式     */
#define AMHW_LPC_USART_CTL_ADDRDET    AM_BIT(2)

/** \brief 发送禁能             */
#define AMHW_LPC_USART_CTL_TXDIS      AM_BIT(6)

/** \brief 持续时钟产生         */
#define AMHW_LPC_USART_CTL_CC         AM_BIT(8)

/** \brief 收到一个字符清除CC位 */
#define AMHW_LPC_USART_CTL_CLRCCONRX  AM_BIT(9)

/** \brief 自动波特率使能       */
#define AMHW_LPC_USART_CTL_AUTOBAUD   AM_BIT(16)

/** @} */

/**
 * \name USART状态标志，可通过amhw_lpc_usart_stat_get函数获取USART状态
 * \anchor grp_amhw_lpc_usart_stat
 * @{
 */

/** \brief 接收准备就绪        */
#define AMHW_LPC_USART_STAT_RXRDY      AM_BIT(0)

/** \brief 接收空闲            */
#define AMHW_LPC_USART_STAT_RXIDLE     AM_BIT(1)

/** \brief 发送准备就绪        */
#define AMHW_LPC_USART_STAT_TXRDY      AM_BIT(2)

/** \brief 发送空闲            */
#define AMHW_LPC_USART_STAT_TXIDLE     AM_BIT(3)

/** \brief 实时CTS信号状态     */
#define AMHW_LPC_USART_STAT_CTS        AM_BIT(4)

/** \brief CTS发生变化,清零    */
#define AMHW_LPC_USART_STAT_DELTACTS   AM_BIT(5)

/** \brief 发送禁能状态标志    */
#define AMHW_LPC_USART_STAT_TXDIS      AM_BIT(6)

/** \brief 溢出错误标志        */
#define AMHW_LPC_USART_STAT_OVR        AM_BIT(8)

/** \brief 接收中断标志        */
#define AMHW_LPC_USART_STAT_RXBRK      AM_BIT(10)

/** \brief 接收器中止状态变化 */
#define AMHW_LPC_USART_STAT_DELTARXBRK AM_BIT(11)

/** \brief 接收检测到起始位   */
#define AMHW_LPC_USART_STAT_START      AM_BIT(12)

/** \brief 帧错误标志         */
#define AMHW_LPC_USART_STAT_FRMERR     AM_BIT(13)

/** \brief 奇偶校验错误标志   */
#define AMHW_LPC_USART_STAT_PARERR     AM_BIT(14)

/** \brief 接收噪声标志       */
#define AMHW_LPC_USART_STAT_RXNOISE    AM_BIT(15)

/** \brief 自动波特率错误标志 */
#define AMHW_LPC_USART_STAT_ABAUDERR   AM_BIT(16)

/** @} */

/**
 * \name 串口中断源定义,可通过amhw_lpc_usart_int_enable，
 *       amhw_lpc_usart_int_disable使能或禁能
 * \anchor grp_amhw_lpc_usart_int
 * @{
 */

/** \brief 接收准备就绪        */
#define AMHW_LPC_USART_INT_RXRDY      AM_BIT(0)

/** \brief 发送准备就绪        */  
#define AMHW_LPC_USART_INT_TXRDY      AM_BIT(2)  

/** \brief 发送空闲            */
#define AMHW_LPC_USART_INT_TXIDLE     AM_BIT(3)  

/** \brief CTS信号发生变化     */
#define AMHW_LPC_USART_INT_DELTACTS   AM_BIT(5) 

/** \brief 发送器禁止中断      */
#define AMHW_LPC_USART_INT_TXDIS      AM_BIT(6)  

/** \brief 溢出中断            */
#define AMHW_LPC_USART_INT_OVR        AM_BIT(8) 

/** \brief 接收break状态变化   */ 
#define AMHW_LPC_USART_INT_DELTARXBRK AM_BIT(11) 

/** \brief 接收器检测起始位    */
#define AMHW_LPC_USART_INT_START      AM_BIT(12) 

/** \brief 帧错误中断          */
#define AMHW_LPC_USART_INT_FRMERR     AM_BIT(13) 

/** \brief 奇偶校验错误中断    */
#define AMHW_LPC_USART_INT_PARERR     AM_BIT(14)
 
/** \brief 接收噪声中断        */
#define AMHW_LPC_USART_INT_RXNOISE    AM_BIT(15) 

/** \brief 自动波特率错误中断  */
#define AMHW_LPC_USART_INT_ABAUDERR   AM_BIT(16) 

/** \brief 所有中断            */
#define AMHW_LPC_USART_INT_ALL        (0x1F96D) 

/** @} */

/**
 * \name USART串口中断状态标志，可以通过函数amhw_lpc_usart_intstat_get()
 *       获取中断状态
 * \anchor grp_amhw_lpc_usart_intstat
 * @{
 */
 
/** \brief 接收准备就绪       */
#define AMHW_LPC_USART_INTSTAT_RXRDY      AM_BIT(0)

/** \brief 发送准备就绪       */
#define AMHW_LPC_USART_INTSTAT_TXRDY      AM_BIT(2)

/** \brief 发送空闲           */
#define AMHW_LPC_USART_INTSTAT_TXIDLE     AM_BIT(3)

/** \brief CTS信号发生变化    */
#define AMHW_LPC_USART_INTSTAT_DELTACTS   AM_BIT(5)

/** \brief 发送禁能中断标志   */
#define AMHW_LPC_USART_INTSTAT_TXDIS      AM_BIT(6)

/** \brief 溢出中断标志       */
#define AMHW_LPC_USART_INTSTAT_OVR        AM_BIT(8)

/** \brief 接收break状态变化  */
#define AMHW_LPC_USART_INTSTAT_DELTARXBRK AM_BIT(11)

/** \brief 接收器检测到起始位 */
#define AMHW_LPC_USART_INTSTAT_START      AM_BIT(12)

/** \brief 帧错误中断         */
#define AMHW_LPC_USART_INTSTAT_FRMERR     AM_BIT(13)

/** \brief 奇偶校验错误中断   */
#define AMHW_LPC_USART_INTSTAT_PARERR     AM_BIT(14)

/** \brief 接收噪声中断       */
#define AMHW_LPC_USART_INTSTAT_RXNOISE    AM_BIT(15)

/** \brief 自动波特率错误中断 */
#define AMHW_LPC_USART_INTSTAT_ABAUDERR   AM_BIT(16)

/** @} interrupt state */

/**
 * \name USART串口接收器寄存器（RXDATASTAT）标志位定义
 * \anchor grp_amhw_lpc_usart_rxstat
 * @{
 */
 
/** \brief 成帧错误标志     */
#define AMHW_LPC_USART_RXSTAT_FRAMERR    AM_BIT(13) 

/** \brief 奇偶校验错误标志 */
#define AMHW_LPC_USART_RXSTAT_PARITYERR  AM_BIT(14) 

/** \brief 接收噪声标志     */
#define AMHW_LPC_USART_RXSTAT_RXNOISE    AM_BIT(15) 

/** @} rx register state */


/**
 * \brief 配置串口
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] cfg_flags  : AMHW_LPC_USART_CFG_*宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_CFG_HWFLOW)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_config (amhw_lpc_usart_t *p_hw_usart, 
                            uint32_t          cfg_flags)
{
    p_hw_usart->cfg = cfg_flags;
}

/**
 * \brief 获取串口配置
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
uint32_t amhw_lpc_usart_config_get (amhw_lpc_usart_t *p_hw_usart)
{
    return p_hw_usart->cfg;
}

/**
 * \brief 串口配置寄存器位设置
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] cfg_flags  : AMHW_LPC_USART_CFG_*宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_CFG_HWFLOW)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_cfg_set (amhw_lpc_usart_t *p_hw_usart,
                             uint32_t          cfg_flags)
{
    AM_BIT_SET_MASK(p_hw_usart->cfg, cfg_flags);
}

/**
 * \brief 串口配置寄存器位清除
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] cfg_flags  : AMHW_LPC_USART_CFG_*宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_CFG_HWFLOW)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_cfg_clr (amhw_lpc_usart_t *p_hw_usart,
                             uint32_t          cfg_flags)
{
    AM_BIT_CLR_MASK(p_hw_usart->cfg, cfg_flags);
}

/**
 * \brief 设置串口控制寄存器值
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] ctl_flags  :  AMHW_LPC_USART_CTL_*宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_CTL_ADDRDET)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_control (amhw_lpc_usart_t *p_hw_usart, 
                             uint32_t          ctl_flags)
{
    p_hw_usart->ctl = ctl_flags;
}

/**
 * \brief 获取串口控制寄存器值
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *                         (#AMHW_LPC_USART_CTL_ADDRDET)
 *
 * \return 串口控制寄存器值
 */
am_static_inline
uint32_t amhw_lpc_usart_ctl_get (amhw_lpc_usart_t *p_hw_usart)
{
    return p_hw_usart->ctl;
}

/**
 * \brief 设置控制寄存器置位
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] ctl_flags  : AMHW_LPC_USART_CTL_*宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_CTL_ADDRDET),未设置的位不受影响。
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_ctl_set (amhw_lpc_usart_t *p_hw_usart, 
                             uint32_t          ctl_flags)
{
    AM_BIT_SET_MASK(p_hw_usart->ctl, ctl_flags);
}

/**
 * \brief 清除控制寄存器位
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] ctl_flags  : AMHW_LPC_USART_CTL_*宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_CTL_ADDRDET),未清除的位不受影响。
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_ctl_clr (amhw_lpc_usart_t *p_hw_usart, 
                             uint32_t          ctl_flags)
{
    AM_BIT_CLR_MASK(p_hw_usart->ctl, ctl_flags);
}

/**
 * \brief 获取串口状态
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return AMHW_LPC_USART_INTSTAT_*宏值或多个宏的或值
 *         (#AMHW_LPC_USART_INTSTAT_RXRDY)
 */
am_static_inline
uint32_t amhw_lpc_usart_stat_get (amhw_lpc_usart_t *p_hw_usart)
{
    return p_hw_usart->stat;
}

/**
 * \brief 清除串口状态标志
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] stat_flags : AMHW_LPC_USART_STAT_*宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_STAT_START)
 *
 * \return 无
 *
 * \note 大多数标志是自动清除.少部分标志才需要软件清除:
 *       #AMHW_LPC_USART_STAT_DELTACTS, #AMHW_LPC_USART_STAT_OVR,
 *       #AMHW_LPC_USART_STAT_DELTARXBRK, #AMHW_LPC_USART_STAT_START,
 *       #AMHW_LPC_USART_STAT_FRMERR, #AMHW_LPC_USART_STAT_PARERR
 *       #AMHW_LPC_USART_STAT_RXNOISE, #AMHW_LPC_USART_STAT_ABAUDERR
 */
am_static_inline
void amhw_lpc_usart_stat_clr (amhw_lpc_usart_t *p_hw_usart, 
                              uint32_t          stat_flags)
{
    p_hw_usart->stat = stat_flags;
}

/**
 * \brief 得到串口中断状态
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return AMHW_LPC_USART_INTSTAT_*宏值或多个宏的或值
 *         (#AMHW_LPC_USART_INTSTAT_RXRDY)
 */
am_static_inline
uint32_t amhw_lpc_usart_intstat_get (amhw_lpc_usart_t *p_hw_usart)
{
    return p_hw_usart->intstat;
}

/**
 * \brief 读取串口接收数据，同时包含接收相关状态
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return 读取到的值
 *          - bit0 ~ bit8 : 接收到的数据
 *          - bit13       : 帧错误状态
 *          - bit14       : 奇偶校验错误状态
 *          - bit15       : 接收噪声
 */
am_static_inline
uint32_t amhw_lpc_usart_rxdata_stat_read (amhw_lpc_usart_t *p_hw_usart)
{
     return p_hw_usart->rxdatstat;
}

/**
 * \brief 使能指定的串口中断
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] int_enable : AMHW_LPC_USART_INT_* 宏值或多个宏的或值
 *                         (#AMHW_LPC_USART_INT_RXRDY)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_int_enable (amhw_lpc_usart_t *p_hw_usart, 
                                uint32_t          int_enable)
{
    AM_BIT_SET_MASK(p_hw_usart->intenset, int_enable);
}

/**
 * \brief 禁能指定的串口中断
 *
 * \param[in] p_hw_usart  : 指向串口寄存器块的指针
 * \param[in] int_disable : AMHW_LPC_USART_INT_* 宏值或多个宏的或值
 *                          (#AMHW_LPC_USART_INT_RXRDY)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_int_disable (amhw_lpc_usart_t *p_hw_usart, 
                                 uint32_t          int_disable)
{
     p_hw_usart->intenclr = int_disable;
}

/**
 * \brief 写串口发送寄存器值（发送一个数据）
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] data       : 发送的数据
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_txdata_write (amhw_lpc_usart_t *p_hw_usart,
                                  uint32_t          data)
{
     p_hw_usart->txdat = data;
}

/**
 * \brief 读取串口接收数据
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return 串口接收到的数据
 */
am_static_inline
uint32_t amhw_lpc_usart_rxdata_read (amhw_lpc_usart_t *p_hw_usart)
{
     return p_hw_usart->rxdat;
}

/**
 * \brief 读取串口接收数据（含接受标志位）
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return 串口接收到的数据及接受标志信息
 */
am_static_inline
uint32_t amhw_lpc_usart_rxdatstat_read (amhw_lpc_usart_t *p_hw_usart)
{
     return p_hw_usart->rxdatstat;
}

/**
 * \brief 设置串口波特率产生寄存器值
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] brg_val    : 分频值，串口时钟将被brg_val分频(0~0xffff)
 *
 * \return 无
 * 
 * \note 串口波特率分频值，寄存器值0～0xFFFF,实际值1~65536
 */
am_static_inline
void amhw_lpc_usart_brg_set (amhw_lpc_usart_t *p_hw_usart, 
                             uint32_t          brg_val)
{
    p_hw_usart->brg = brg_val & 0xffff;
}

/**
 * \brief 设置过采样值
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] osr_val    : 发送和接收每位使用osr_val个时钟进行采样
 *                         
 *
 * \return 无
 *
 * \note 异步模式下的过采样值，默认0xF,寄存器值0x3～0xF,实际值4～16
 */
am_static_inline
void amhw_lpc_usart_osr_set (amhw_lpc_usart_t *p_hw_usart, 
                             uint8_t           osr_val)
{
    p_hw_usart->osr = osr_val;
}

/**
 * \brief 设置地址寄存器的值
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] addr_val   : 8位的地址值
 *
 * \return 无
 *
 * \note 在地址匹配模式中，如果使能了地址自动匹配模式，则该地址寄存器保存着的地
 *       址值用于硬件地址匹配。
 */
am_static_inline
void amhw_lpc_usart_addr_set (amhw_lpc_usart_t *p_hw_usart, 
                              uint8_t           addr_val)
{
    p_hw_usart->addr = addr_val;
}

/**
 * \brief 获取地址寄存器的值
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] addr_val   : 8位的地址值
 *
 * \return 8位地址
 *
 * \note 在地址匹配模式中，如果使能了地址自动匹配模式，则该地址寄存器保存着的地
 *       址值用于硬件地址匹配。
 */
am_static_inline
uint8_t amhw_lpc_usart_addr_get (amhw_lpc_usart_t *p_hw_usart, 
                                 uint8_t              addr_val)
{
    return (0xff & p_hw_usart->addr);
}

/**
 * \brief 使能串口
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_enable (amhw_lpc_usart_t *p_hw_usart)
{
    AM_BIT_SET_MASK(p_hw_usart->cfg, AMHW_LPC_USART_CFG_ENABLE);
}

/**
 * \brief 禁能串口
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_disable (amhw_lpc_usart_t *p_hw_usart)
{
    AM_BIT_CLR_MASK(p_hw_usart->cfg, AMHW_LPC_USART_CFG_ENABLE);
}

/**
 * \brief 读取CTS状态（用于硬件流控发送判断）
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return cts引脚状态（0表示低电平，非0表示高电平）
 */
am_static_inline
uint32_t amhw_lpc_usart_cts_stat_read (amhw_lpc_usart_t *p_hw_usart)
{
    return (p_hw_usart->stat & AMHW_LPC_USART_STAT_CTS);
}

/**
 * \brief 等待串口发送空闲
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_usart_wait_idle (amhw_lpc_usart_t *p_hw_usart)
{
    while ((p_hw_usart->stat & AMHW_LPC_USART_STAT_TXIDLE) == 0);
}

/**
 * \brief 串口波特率获取
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] u_clk      : 串口基础时钟
 *
 * \return 串口波特率
 */
uint32_t amhw_lpc_usart_baudrate_get (amhw_lpc_usart_t *p_hw_usart,
                                      uint32_t          u_clk);

/**
 * \brief 串口波特率设置
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] u_clk      : 串口的输入时钟频率
 * \param[in] baudrate   : 期望设置的串口波特率
 *
 * \return 实际的波特率（小于0表明设置失败）
 */
int amhw_lpc_usart_baudrate_set (amhw_lpc_usart_t *p_hw_usart,
                                 uint32_t          u_clk,
                                 uint32_t          baudrate);

/**
 * \brief USART数据发送(查询模式)
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] p_txbuf    : 发送数据缓冲区
 * \param[in] nbytes     : 发送数据个数
 *
 * \return 成功发送的数据个数
 */
int amhw_lpc_usart_poll_send (amhw_lpc_usart_t  *p_hw_usart,
                              const uint8_t   *p_txbuf,
                              uint32_t           nbytes);

/**
 * \brief USART数据接收(查询模式)
 *
 * \param[in] p_hw_usart : 指向串口寄存器块的指针
 * \param[in] p_rxbuf    : 接收数据缓冲区
 * \param[in] nbytes     : 接收数据个数
 *
 * \return 成功接收的数据个数
 */
int amhw_lpc_usart_poll_receive (amhw_lpc_usart_t *p_hw_usart,
                                 uint8_t          *p_rxbuf,
                                 uint32_t          nbytes);

/**
 * @} amhw_lpc_if_usart
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_USART_H */

/* end of file */
