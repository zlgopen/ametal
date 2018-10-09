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
 * \brief DMA 硬件操作接口。
 *
 * 1. DMA有18 个通道, 每个通道连接固定的外设，可接受外设请求，
 *    外设分别是USART,SPI和I2C等；
 * 2. 片内或片外事件均能触发DMA运行，每个DMA通道均可选择9个硬件输入触发源中的一个，
 *    也可以软件触发DMA运行；
 * 3. 每个通道均可设定优先级，具有优先级仲裁；
 * 4. 每个通道可通过传输描述符连接，由硬件自动传输多个描述符内容；
 * 5. 单次传输最多可传1024字，传输地址可自增。
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-27  mem,modified.
 * - 1.00 15-07-07  win, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_DMA_H
#define __AMHW_LPC82X_DMA_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_dma
 * \copydoc amhw_lpc82x_dma.h
 * @{
 */

/** \brief DMA 通道数量 */
#define AMHW_LPC82X_DMA_CHAN_CNT     18
    
/**
 * \brief DMA 通道寄存器结构体
 */
typedef struct amhw_lpc82x_dma_chan {
    __IO  uint32_t  cfg;            /**< \brief DMA 通道配置寄存器            */
    __I   uint32_t  ctlstat;        /**< \brief DMA 通道控制和状态寄存器      */
    __IO  uint32_t  xfercfg;        /**< \brief DMA 传输配置寄存器            */
    __I   uint32_t  reserved;       /**< \brief 保留                          */
} amhw_lpc82x_dma_chan_t;

/**
 * \brief DMA 控制器寄存器块结构体 
 */
typedef struct amhw_lpc82x_dma {
    __IO  uint32_t  ctrl;           /**< \brief DMA 控制寄存器                */
    __I   uint32_t  intstat;        /**< \brief DMA 中断状态寄存器            */
    __IO  uint32_t  srambase;       /**< \brief DMA 通道配置列表的SRAM地址    */
    __IO  uint32_t  reserved0[5];   /**< \brief 保留                          */
    __IO  uint32_t  enableset0;     /**< \brief 读和设置DMA通道使能寄存器     */
    __IO  uint32_t  reserved1;      /**< \brief 保留                          */
    __IO  uint32_t  enableclr0;     /**< \brief 清除DMA通道使能寄存器         */
    __IO  uint32_t  reserved2;      /**< \brief 保留                          */
    __I   uint32_t  active0;        /**< \brief DMA通道有效状态寄存器         */
    __IO  uint32_t  reserved3;      /**< \brief 保留                          */
    __I   uint32_t  busy0;          /**< \brief DMA通道busy状态寄存器         */
    __I   uint32_t  reserved4;      /**< \brief 保留                          */
    __IO  uint32_t  errint0;        /**< \brief DMA 通道错误中断状态寄存器    */
    __I   uint32_t  reserved5;      /**< \brief 保留                          */
    __IO  uint32_t  intenset0;      /**< \brief 读和设置DMA通道中断使能寄存器 */
    __I   uint32_t  reserved6;      /**< \brief 保留                          */
    __IO  uint32_t  intenclr0;      /**< \brief 清除DMA通道中断使能寄存器     */
    __I   uint32_t  reserved7;      /**< \brief 保留                          */
    __IO  uint32_t  inta0;          /**< \brief DMA A中断标志寄存器           */
    __I   uint32_t  reserved8;      /**< \brief 保留                          */
    __IO  uint32_t  intb0;          /**< \brief DMA B中断标志寄存器           */
    __I   uint32_t  reserved9;      /**< \brief 保留                          */
    __IO  uint32_t  setvalid0;      /**< \brief DMA通道有效等待控制位寄存器   */
    __I   uint32_t  reserved10;     /**< \brief 保留                          */
    __IO  uint32_t  settrig0;       /**< \brief DMA通道触发控制位寄存器       */
    __I   uint32_t  reserved11;     /**< \brief 保留                          */
    __IO  uint32_t  abort0;         /**< \brief DMA通道冻结控制位寄存器       */
    
    __I   uint32_t  reserved12[225]; /**< \brief 保留                         */

    /** \brief DMA通道寄存器    */
    amhw_lpc82x_dma_chan_t chan[AMHW_LPC82X_DMA_CHAN_CNT];
    
} amhw_lpc82x_dma_t;

/**
 * \name 中断状态
 * \anchor grp_amhw_lpc82x_dma_intstat
 * @{
 */

/** \brief 至少一个使能中断挂起 */
#define AMHW_LPC82X_DMA_INTSTAT_ACTIVEINT     AM_BIT(1)

/** \brief 至少一个错误中断挂起 */
#define AMHW_LPC82X_DMA_INTSTAT_ACTIVEERRINT  AM_BIT(2)

/** @}*/

/**
 * \name 通道配置参数
 * \anchor grp_amhw_lpc82x_dma_chan_flags
 * @{
 */

/** \brief 使能外设DMA请求 */
#define AMHW_LPC82X_DMA_CHAN_PERIPH_REQ_EN    AM_BIT(0)

/** \brief 使能硬件触发 */
#define AMHW_LPC82X_DMA_CHAN_HWTRIG_EN        AM_BIT(1)

/** \brief 硬件触发：下降沿敏感 */
#define AMHW_LPC82X_DMA_CHAN_HWTRIG_FALL      AM_BIT(1)

/** \brief 硬件触发：上升沿敏感 */
#define AMHW_LPC82X_DMA_CHAN_HWTRIG_RISE     (AM_BIT(1) | AM_BIT(4))

/** \brief 硬件触发：低电平敏感 */
#define AMHW_LPC82X_DMA_CHAN_HWTRIG_LOW      (AM_BIT(1) | AM_BIT(5))

/** \brief 硬件触发：高电平敏感 */
#define AMHW_LPC82X_DMA_CHAN_HWTRIG_HIGH     (AM_BIT(1) | AM_BIT(4) | AM_BIT(5))

/** \brief 使能突发模式 */
#define AMHW_LPC82X_DMA_CHAN_BURST_EN         AM_BIT(6)

/** \brief 每次边沿突发传输大小为 1    */
#define AMHW_LPC82X_DMA_CHAN_BURST_1          AM_SBF(0,  8)

/** \brief 每次边沿突发传输大小为 2    */
#define AMHW_LPC82X_DMA_CHAN_BURST_2          AM_SBF(1,  8)

/** \brief 每次边沿突发传输大小为 4    */
#define AMHW_LPC82X_DMA_CHAN_BURST_4          AM_SBF(2,  8)

/** \brief 每次边沿突发传输大小为 8    */
#define AMHW_LPC82X_DMA_CHAN_BURST_8          AM_SBF(3,  8)

/** \brief 每次边沿突发传输大小为 16   */
#define AMHW_LPC82X_DMA_CHAN_BURST_16         AM_SBF(4,  8)

/** \brief 每次边沿突发传输大小为 32   */
#define AMHW_LPC82X_DMA_CHAN_BURST_32         AM_SBF(5,  8)

/** \brief 每次边沿突发传输大小为 64   */
#define AMHW_LPC82X_DMA_CHAN_BURST_64         AM_SBF(6,  8)

/** \brief 每次边沿突发传输大小为 128  */
#define AMHW_LPC82X_DMA_CHAN_BURST_128        AM_SBF(7,  8)

/** \brief 每次边沿突发传输大小为 256  */
#define AMHW_LPC82X_DMA_CHAN_BURST_256        AM_SBF(8,  8)

/** \brief 每次边沿突发传输大小为 512  */
#define AMHW_LPC82X_DMA_CHAN_BURST_512        AM_SBF(9,  8)

/** \brief 每次边沿突发传输大小为 1024 */
#define AMHW_LPC82X_DMA_CHAN_BURST_1024       AM_SBF(10, 8)

/** \brief 源地址突发环绕     */
#define AMHW_LPC82X_DMA_CHAN_SRC_BURST_WRAP   AM_BIT(14)

/** \brief 目标地址突发环绕   */
#define AMHW_LPC82X_DMA_CHAN_DST_BURST_WRAP   AM_BIT(15)

/** \brief 通道优先级 0 (最高) */
#define AMHW_LPC82X_DMA_CHAN_PRIO_0           AM_SBF(0, 16)

/** \brief 通道优先级 1  */
#define AMHW_LPC82X_DMA_CHAN_PRIO_1           AM_SBF(1, 16)

/** \brief 通道优先级 2  */
#define AMHW_LPC82X_DMA_CHAN_PRIO_2           AM_SBF(2, 16)

/** \brief 通道优先级 3  */
#define AMHW_LPC82X_DMA_CHAN_PRIO_3           AM_SBF(3, 16)

/** @} */

/**
 * \name 传输描述符配置参数
 * \anchor grp_amhw_lpc82x_dma_xfercfg_flags
 * @{
 */
 
/** \brief 传输描述符有效 */
#define AMHW_LPC82X_DMA_XFER_VALID           AM_BIT(0)

/** \brief 描述符传输完成时，自动重载新的通道描述符 */
#define AMHW_LPC82X_DMA_XFER_RELOAD           AM_BIT(1)

/** \brief 立即触发DMA传输启动 */
#define AMHW_LPC82X_DMA_XFER_SWTRIG           AM_BIT(2)

/** \brief 描述符传输完成时，清除触发标志 */
#define AMHW_LPC82X_DMA_XFER_CLRTRIG          AM_BIT(3)

/** \brief 描述符传输完成时，产生A中断 */
#define AMHW_LPC82X_DMA_XFER_SETINTA          AM_BIT(4)

/** \brief 描述符传输完成时，产生B中断 */
#define AMHW_LPC82X_DMA_XFER_SETINTB          AM_BIT(5)

/** \brief 源地址不增加 */
#define AMHW_LPC82X_DMA_XFER_WIDTH_8BIT       AM_SBF(0, 8)

/** \brief 传输数据宽度为16 bit */
#define AMHW_LPC82X_DMA_XFER_WIDTH_16BIT      AM_SBF(1, 8)

/** \brief 传输数据宽度为32 bit */
#define AMHW_LPC82X_DMA_XFER_WIDTH_32BIT      AM_SBF(2, 8)

/** \brief 每次传输源地址不增加 */
#define AMHW_LPC82X_DMA_XFER_SRCINC_NOINC     AM_SBF(0, 12)

/** \brief 源地址增量为1个数据宽度 */
#define AMHW_LPC82X_DMA_XFER_SRCINC_1X        AM_SBF(1, 12)

/** \brief 源地址增量为2个数据宽度 */
#define AMHW_LPC82X_DMA_XFER_SRCINC_2X        AM_SBF(2, 12)

/** \brief 源地址增量为4个数据宽度 */
#define AMHW_LPC82X_DMA_XFER_SRCINC_4X        AM_SBF(3, 12)

/** \brief 目标地址不变 */
#define AMHW_LPC82X_DMA_XFER_DSTINC_NOINC     AM_SBF(0, 14)

/** \brief 目标地址增量为1个数据宽度 */
#define AMHW_LPC82X_DMA_XFER_DSTINC_1X        AM_SBF(1, 14)

/** \brief 目标地址增量为2个数据宽度 */
#define AMHW_LPC82X_DMA_XFER_DSTINC_2X        AM_SBF(2, 14)

/** \brief 目标地址增量为3个数据宽度 */
#define AMHW_LPC82X_DMA_XFER_DSTINC_4X        AM_SBF(3, 14)

/** \brief 传输总次数，最少为1次  */
#define AMHW_LPC82X_DMA_XFERCOUNT(n)          AM_SBF(n, 16)

/** @} */

/**
 * \brief 使能DMA控制器
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_enable (amhw_lpc82x_dma_t *p_hw_dma)
{
    p_hw_dma->ctrl = 1UL;
}

/**
 * \brief 禁能DMA控制器
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_disable (amhw_lpc82x_dma_t *p_hw_dma)
{
    p_hw_dma->ctrl = 0UL;
}

/**
 * \brief 获取DMA中断状态
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 *
 * \return 中断状态
 */
am_static_inline
uint32_t amhw_lpc82x_dma_int_stat_get (amhw_lpc82x_dma_t *p_hw_dma)
{
    return (p_hw_dma->intstat & 0x06);
}

/**
 * \brief 配置SRAM基地址
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] addr     : 通道描述符列表的起始地址
 *
 * \return 无
 *
 * \note 列表的起始地址必须是512字节对齐
 */
am_static_inline
void amhw_lpc82x_dma_sram_addr_set (amhw_lpc82x_dma_t *p_hw_dma, 
                                    uint32_t           addr)
{
    p_hw_dma->srambase = AM_BIT_CLR_MASK(addr, 0x1FF);
}

/**
 * \brief 获取SRAM基地址
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 *
 * \return 通道描述符列表的起始地址，该地址512字节对齐
 */
am_static_inline
uint32_t amhw_lpc82x_dma_sram_addr_get (amhw_lpc82x_dma_t *p_hw_dma)
{
    return p_hw_dma->srambase;
}

/**
 * \brief 获取通道描述符地址
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 通道描述符地址
 */
am_static_inline
uint32_t amhw_lpc82x_dma_chan_desc_addr_get (amhw_lpc82x_dma_t *p_hw_dma,
                                             uint8_t            chan)
{
    return (amhw_lpc82x_dma_sram_addr_get(p_hw_dma) + (chan << 4));
}

/**
 * \brief 使能DMA通道
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_enable (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    p_hw_dma->enableset0 = AM_BIT(chan);
}

/**
 * \brief 禁能DMA通道
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_disable (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    p_hw_dma->enableclr0 = AM_BIT(chan);
}

/**
 * \brief 判断指定通道是否处于活动状态
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * 
 * \retval AM_TRUE  : 通道处于活动状态
 * \retval AM_FALSE : 通道处于非活动状态
 *
 * \note 启动DMA后，通道处于活跃状态；DMA全部传输完成，通道处于非活跃状态
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_isactive (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    return ((p_hw_dma->active0 & AM_BIT(chan)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 判断指定通道是否busy
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * 
 * \retval AM_TRUE  : 通道 busy
 * \retval AM_FALSE : 通道 unbusy
 *
 * \note DMA通道在传输过程中被软件禁能，调用此函数判断是否可以abort终止
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_isbusy (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    return ((p_hw_dma->busy0 & AM_BIT(chan)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 获取全部错误标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 *
 * \return 错误中断标志, bit[17:0] 是错误标志位域，Bit n 对应通道 n
 */
am_static_inline
uint32_t amhw_lpc82x_dma_error_flags_get (amhw_lpc82x_dma_t *p_hw_dma)
{
    return (p_hw_dma->errint0 & 0x3FFFF);
}

/**
 * \brief 通过掩码清除错误中断标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] mask     : bit[17:0] 对应DMA通道 n， 
 *                       该清除位必须与 ERRINT 相对应
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_error_flags_clr (amhw_lpc82x_dma_t *p_hw_dma, 
                                      uint32_t           mask)
{
    p_hw_dma->errint0 = mask;
}

/**
 * \brief 判断指定通道的错误标志是否置位
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针。
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \retval AM_TRUE  : 通道发生错误
 * \retval AM_FALSE : 通道没有发生错误
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_iserror (amhw_lpc82x_dma_t *p_hw_dma,
                                        uint8_t            chan)
{
    return ((p_hw_dma->errint0 & AM_BIT(chan)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 清除指定通道的错误标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_error_clr (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    p_hw_dma->errint0 = AM_BIT(chan);
}

/**
 * \brief 使能DMA通道中断
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_int_enable (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    p_hw_dma->intenset0 = AM_BIT(chan);
}

/**
 * \brief 禁能DMA通道中断
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_int_disable (amhw_lpc82x_dma_t *p_hw_dma, 
                                       uint8_t            chan)
{
    p_hw_dma->intenclr0 = AM_BIT(chan);
}

/**
 * \brief 获取所有通道A中断标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 *
 * \return A中断标志，bit[17:0] 是DMA通道的A中断标志，Bit n 对应DMA通道 n 
 */
am_static_inline
uint32_t amhw_lpc82x_dma_inta_flags_get (amhw_lpc82x_dma_t *p_hw_dma)
{
    return (p_hw_dma->inta0 & 0x3FFFF);
}

/**
 * \brief 通过掩码清除A中断标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] mask     : bit[17:0] 对应DMA通道 n, 该被清除为必须与 INTA 相对应
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_inta_flags_clr (amhw_lpc82x_dma_t *p_hw_dma, uint32_t mask)
{
    p_hw_dma->inta0 = mask;
}

/**
 * \brief 判断指定通道中断A标志是否置位
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * 
 * \retval AM_TRUE  : 通道的A中断标志置位
 * \retval AM_FALSE : 通道的A中断标志没有被置位
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_inta_isactive (amhw_lpc82x_dma_t *p_hw_dma,
                                              uint8_t            chan)
{
    return ((p_hw_dma->inta0 & AM_BIT(chan)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 清除指定通道的A中断标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_inta_clr (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    p_hw_dma->inta0 = AM_BIT(chan);
}

/**
 * \brief 获取全部通道的B中断标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 *
 * \return B中断标志，bit[17:0] 是DMA通道的B中断标志，Bit n 对应DMA通道 n
 */
am_static_inline
uint32_t amhw_lpc82x_dma_intb_flags_get (amhw_lpc82x_dma_t *p_hw_dma)
{
    return (p_hw_dma->intb0 & 0x3FFFF);
}

/**
 * \brief 通过掩码清除B中断标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] mask     : bit[17:0] 对应DMA通道 n,该被清除为必须与 INTB 相对应
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_intb_flags_clr (amhw_lpc82x_dma_t *p_hw_dma, uint32_t mask)
{
    p_hw_dma->intb0 = mask;
}

/**
 * \brief 判断指定通道的B中断标志是否置位
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \retval AM_TRUE  : 通道的B中断标志置位
 * \retval AM_FALSE : 通道的B中断标志没有被置位
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_intb_isactive (amhw_lpc82x_dma_t *p_hw_dma,
                                              uint8_t            chan)
{
    return ((p_hw_dma->intb0 & AM_BIT(chan)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 清除指定通道的B中断标志
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_intb_clr (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    p_hw_dma->intb0 = AM_BIT(chan);
}

/**
 * \brief 设置指定通道传输描述符有效
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_xferdesc_valid_set (amhw_lpc82x_dma_t *p_hw_dma,
                                              uint8_t            chan)
{
    p_hw_dma->setvalid0 = AM_BIT(chan);
}

/**
 * \brief 设置指定通道传输描述符触发有效
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_xferdesc_trig_set (amhw_lpc82x_dma_t *p_hw_dma,
                                             uint8_t            chan)
{
    p_hw_dma->settrig0 = AM_BIT(chan);
}

/**
 * \brief 终止DMA通道
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 *
 * \note 终止DMA通道前，应先禁能通道，然后等待通道un busy，最后终止通道
 */
am_static_inline
void amhw_lpc82x_dma_chan_abort (amhw_lpc82x_dma_t *p_hw_dma, uint8_t chan)
{
    p_hw_dma->abort0 = AM_BIT(chan);
}

/**
 * \brief DMA 通道参数配置, 与LPC5410X的am_lpc5410x_dma_chan_setup效果一致
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * \param[in] flags    : 配置参数
 *                       \ref grp_amhw_lpc82x_dma_chan_flags
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_cfg_set (amhw_lpc82x_dma_t *p_hw_dma,
                                   uint8_t            chan,
                                   uint32_t           flags)
{
    p_hw_dma->chan[chan].cfg = flags;
}

/**
 * \brief DMA 通道外设请求使能 
 * 
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * 
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_periph_enable (amhw_lpc82x_dma_t *p_hw_dma,
                                         uint8_t            chan)
{
    AM_BIT_SET(p_hw_dma->chan[chan].cfg, 0);
}


/**
 * \brief 查询对应DMA通道的外设是否请求使能
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return AM_TRUE: 请求使能; AM_FALSE: 请求不使能
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_periph_is_enable (amhw_lpc82x_dma_t *p_hw_dma,
                                                 uint8_t            chan)
{
    return ((p_hw_dma->chan[chan].cfg & 1ul)? AM_TRUE : AM_FALSE);

}


/**
 * \brief DMA 通道外设请求禁能 
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_periph_disable (amhw_lpc82x_dma_t *p_hw_dma,
                                          uint8_t            chan)
{
    AM_BIT_CLR(p_hw_dma->chan[chan].cfg, 0);
}

/**
 * \brief 硬件触发方式设置
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * \param[in] flags    : 硬件触发方式，AMHW_LPC82X_DMA_CHAN_HWTRIG_*宏值
 *                       (#AMHW_LPC82X_DMA_CHAN_HWTRIG_FALL)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_hwtrig_set (amhw_lpc82x_dma_t *p_hw_dma,
                                      uint8_t            chan,
                                      uint32_t           flags)
{
    AM_BIT_CLR_MASK(p_hw_dma->chan[chan].cfg, (AM_BIT(1) | AM_SBF(0x3, 4)));
    AM_BIT_SET_MASK(p_hw_dma->chan[chan].cfg, flags);
}

/**
 * \brief 设置硬件触发的传输模式(单次传输或突发传输)
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * \param[in] flags    : 突发传输的设置值，AMHW_LPC82X_DMA_CHAN_BURST_*宏值
 *                       (#AMHW_LPC82X_DMA_CHAN_BURST_EN)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_burst_set (amhw_lpc82x_dma_t *p_hw_dma,
                                     uint8_t            chan,
                                     uint32_t           flags)
{
    AM_BIT_CLR_MASK(p_hw_dma->chan[chan].cfg, (AM_BIT(6) | AM_SBF(0xf, 8)));
    AM_BIT_SET_MASK(p_hw_dma->chan[chan].cfg, flags);
}

/**
 * \brief 使能源端突发环绕
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_src_burst_wrap_enable (amhw_lpc82x_dma_t *p_hw_dma,
                                                 uint8_t            chan)
{
    AM_BIT_SET(p_hw_dma->chan[chan].cfg, 14);
}

/**
 * \brief 禁能源端突发环绕
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_src_burst_wrap_disable (amhw_lpc82x_dma_t *p_hw_dma,
                                                  uint8_t            chan)
{
    AM_BIT_CLR(p_hw_dma->chan[chan].cfg, 14);
}

/**
 * \brief 使能目标端突发环绕
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_dst_burst_wrap_enable (amhw_lpc82x_dma_t *p_hw_dma,
                                                 uint8_t            chan)
{
    AM_BIT_SET(p_hw_dma->chan[chan].cfg, 15);
}

/**
 * \brief 禁能目标端突发环绕
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_dst_burst_wrap_disable (amhw_lpc82x_dma_t *p_hw_dma,
                                                  uint8_t            chan)
{
    AM_BIT_CLR(p_hw_dma->chan[chan].cfg, 15);
}

/**
 * \brief 设置通道优先级
 *
 * \param[in] p_hw_dma  : 指向DMA控制器寄存器块的指针
 * \param[in] chan      : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * \param[in] prio_flag : 通道优先级，AMHW_LPC82X_DMA_CHAN_PRIO_*宏
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_priority_set (amhw_lpc82x_dma_t *p_hw_dma,
                                        uint8_t            chan,
                                        uint32_t           prio_flag)
{
    AM_BIT_CLR_MASK(p_hw_dma->chan[chan].cfg, AM_SBF(7, 16));
    AM_BIT_SET_MASK(p_hw_dma->chan[chan].cfg, prio_flag);
}

/**
 * \brief 判断指定通道传输描述符有效标志是否置位
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \retval AM_TRUE  : 传输描述符有效标志置位
 * \retval AM_FALSE : 传输描述符有效标志没有置位
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_xferdesc_isvalid (amhw_lpc82x_dma_t *p_hw_dma,
                                                 uint8_t            chan)
{
    return ((p_hw_dma->chan[chan].ctlstat & AM_BIT(0)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 判断指定通道传输描述符触发标志是否被置位
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \retval AM_TRUE  : 当前通道触发被置位
 * \retval  AM_FALSE : 当前通道触发未被置位
 */
am_static_inline
am_bool_t amhw_lpc82x_dma_chan_xferdesc_istriggered (amhw_lpc82x_dma_t *p_hw_dma,
                                                     uint8_t            chan)
{
    return ((p_hw_dma->chan[chan].ctlstat & AM_BIT(2)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 获取指定通道传输描述符
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 *
 * \return DMA传输配置参数
 */
am_static_inline
uint32_t amhw_lpc82x_dma_chan_xfercfg_get (amhw_lpc82x_dma_t *p_hw_dma,
                                            uint8_t            chan)
{
    return (uint32_t)(p_hw_dma->chan[chan].xfercfg);
}

/**
 * \brief 设置指定通道的传输描述符
 *
 * \param[in] p_hw_dma : 指向DMA控制器寄存器块的指针
 * \param[in] chan     : DMA通道编号，DMA_CHAN_*宏值 (#DMA_CHAN_0)
 * \param[in] flags    : 传输配置参数，
 *                       \ref grp_amhw_lpc82x_dma_xfercfg_flags
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_dma_chan_xfercfg_set (amhw_lpc82x_dma_t *p_hw_dma,
                                       uint8_t            chan,
                                       uint32_t           flags)
{
    p_hw_dma->chan[chan].xfercfg = flags;
}

/**
 * @} amhw_lpc82x_if_dma
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_LPC82X_DMA_H */

/* end of file */
