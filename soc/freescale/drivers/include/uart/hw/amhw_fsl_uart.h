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
 * \brief 串口（UART）操作接口
 *
 * 1. 全双工操作，标准不归零(NRZ)格式
 * 2. 具有独立使能的双缓冲可分离的发送接收器和接收器
 * 3. 波特率可编程（ 13 位模数分频器）
 * 4. 中断驱动或轮询操作
 * 1. 发送数据寄存器空并且发送完成
 * 2. 接收数据寄存器满
 * 3. 接收过载、奇偶检验错误、帧错误和噪声错误
 * 4. 空闲接收器检测
 * 5. 接收引脚上有活动沿
 * 6. 支持 LIN 的中断检测
 * 5. 硬件奇偶校验生成和检测
 * 6. 可编程的 8 位或专 9 位字符长度
 * 7. 可编程的 1 位或 2 位停止位
 * 8. 空闲线或地址标志唤醒接收器：
 * 9. 可选的 13 位中止符生成/11 位中止符检测
 * 10. 可选的发送器输出极性
 * 11. 5 通道 DMA 接口
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-14  nwt, make some changes about Macro definition.
 * - 1.00 15-07-03  xiongyanming, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_UART_H
#define __AMHW_FSL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
//#include "amhw_kl26_periph_map.h"
//#include "kl26_inum.h"
/**
 * \addtogroup amhw_fsl_if_uart
 * \copydoc amhw_fsl_uart.h
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

/**
 * \brief uart 寄存器块结构体
 */
typedef struct amhw_fsl_uart {
     __IO uint8_t bdh;                    /**< \brief UART 波特率高位寄存器  */
     __IO uint8_t bdl;                    /**< \brief UART 波特率底位寄存器  */
     __IO uint8_t c1;                     /**< \brief UART 控制寄存器        */
     __IO uint8_t c2;                     /**< \brief UART 控制寄存器        */
     __IO uint8_t stat1;                  /**< \brief UART 状态寄存器1       */
     __IO uint8_t stat2;                  /**< \brief UART 状态寄存器2       */
     __IO uint8_t c3;                     /**< \brief UART 控制寄存器        */
     __IO uint8_t data;                   /**< \brief UART 数据寄存器        */
     union {                                                                 
          struct {                                                           
              __IO uint8_t uart0_ma1;     /**< \brief UART0 匹配地址寄存器   */
              __IO uint8_t uart0_ma2;     /**< \brief UART0 匹配地址寄存器   */
              __IO uint8_t uart0_c4;      /**< \brief UART0 控制寄存器       */
              __IO uint8_t uart0_c5;      /**< \brief UART0 控制寄存器       */
          };                                                                 
          __IO uint8_t uart1_2_c4;        /**< \brief UART1/2 控制寄存器     */
      };
} amhw_fsl_uart_t;


/**
 * \name 串口数据寄存器地址定义，(可能由DMA用到)
 * @{
 */
/** \brief UART0 接收数据寄存器地址  */
#define AMHW_FSL_UART0_DAT_ADDR  (&(AMHW_FSL_UART0->data))
/** \brief UART1 接收数据寄存器地址  */
#define AMHW_FSL_UART1_DAT_ADDR  (&(AMHW_FSL_UART1->data))
/** \brief UART2 接收数据寄存器地址  */
#define AMHW_FSL_UART2_DAT_ADDR  (&(AMHW_FSL_UART2->data))

/** @} */

/**
 * \name uart 波特率分频因子设置位宏定义
 * @{
 */
/** \brief 波特率分频因子高位设置                                            */
#define AMHW_FSL_UART_BDH_SBR_MASK     0x1Fu
#define AMHW_FSL_UART_BDH_SBR(x)       (((uint8_t)(((uint8_t)(x))<<0))&0x1Fu)
/** \brief 波特率分频因子底位设置                                            */
#define AMHW_FSL_UART_BDL_SBR_MASK     0xFFu
#define AMHW_FSL_UART_BDL_SBR(x)       (((uint8_t)(((uint8_t)(x))<<0))&0xFFu)

/** @} */

/**
 * \name uart 接收器/发送器使能位宏定义
 * @{
 */
#define AMHW_FSL_UART_C2_TE_EN         ((uint8_t)(1<<3)) /**< \brief 发送器使能  */
#define AMHW_FSL_UART_C2_RE_EN         ((uint8_t)(1<<2)) /**< \brief 接收器使能  */

/** @} */

/**
 * \name uart 设置OSR值宏定义
 * @{
 */
#define AMHW_FSL_UART_C4_OSR_MASK     ((uint8_t)0x1Fu)
#define AMHW_FSL_UART_C4_OSR(x)       (((uint8_t)(((uint8_t)(x))<<0))&0x1Fu)

/** @} */

/**
 * \name uart 状态寄存器1掩码位宏定义
 * @{
 */
/** \brief 发送数据空时，表明可以继续发送                                    */
#define AMHW_FSL_UART_S1_TDRE_MASK     0x80u

/** \brief 接发送完成标志掩码位                                                      */
#define AMHW_FSL_UART_S1_TC_MASK       0x40u

/** \brief 接收数据为满时，表明可以读取数据                                  */
#define AMHW_FSL_UART_S1_RDRF_MASK     0x20u

/** \brief 校验错误位域掩码 */
#define AMHW_FSL_UART_S1_PF_MASK       0x01u

/** @} */

/**
 * \brief 使能双边沿采样位宏定义
 */
#define AMHW_FSL_UART0_C5_BOTHEDGE_EN  ((uint8_t)(1<<1))

/**
 * \name uart C1 控制寄存器1
 * @{
 */

/** \brief 回环模式或单线模式 */
#define AMHW_FSL_UART_C1_LOOPS          ((uint8_t)(1 << 7))

/** \brief 接收源选择，[LOOPS = 1]该位域为0时LOOP模式，为1时Single-Wire模式 */
#define AMHW_FSL_UART_C1_RSRC           ((uint8_t)(1 << 5))

#define AMHW_FSL_UART_C1_M_8BIT         ((uint8_t)(0 << 4)) /**< \brief 8位数据长度*/
#define AMHW_FSL_UART_C1_M_9BIT         ((uint8_t)(1 << 4)) /**< \brief 9位数据长度*/
#define AMHW_FSL_UART_C1_M_GET(c1)      ((c1 & (1 << 4)))   /**< \brief 获取数据长度*/

#define AMHW_FSL_UART_C1_PARITY_NO      ((uint8_t)(0 << 1)) /**< \brief 不使用校验 */
#define AMHW_FSL_UART_C1_PARITY_EVEN    ((uint8_t)(2 << 0)) /**< \brief 偶校验     */
#define AMHW_FSL_UART_C1_PARITY_ODD     ((uint8_t)(3 << 0)) /**< \brief 奇校验     */
#define AMHW_FSL_UART_C1_PARITY_GET(c1) (c1 & 0x3)          /**< \brief 获取校验设置  */

/** @} */

/**
 * \name uart C1 停止位位宏定义
 * @{
 */
#define AMHW_FSL_UART_BDH_SBNS_STOP_1   (0UL << 5)         /**< \brief 1位停止位  */
#define AMHW_FSL_UART_BDH_SBNS_STOP_2   (1UL << 5)         /**< \brief 2位停止位  */

/** @} */

/**
 * \name uart C3 控制寄存器3
 * @{
 */

/** \brief 配置单线模式下IO引脚的输入输出方向，该位域0表示输入，该位域1表示输出 */
#define AMHW_FSL_UART_C3_TXDIR          (1U << 5)

/** \brief 单线模式下Tx引脚为输入方向，由 #AMHW_FSL_UART_C3_TXDIR 位域控制 */
#define AMHW_FSL_UART_SINGLE_WIRE_INPUT  0

/** \brief 单线模式下Tx引脚为输出方向，由 #AMHW_FSL_UART_C3_TXDIR 位域控制 */
#define AMHW_FSL_UART_SINGLE_WIRE_OUTPUT 1

/** @} */

/**
 * \name 串口中断源定义,可通过amhw_fsl_uart_int_enable使能或amhw_fsl_uart_int_disable禁能
 * @{
 */
#define AMHW_FSL_UART_INT_BDH_LBKDIE  (1UL << 9) /**< \brief LIN断点检测中断      */
#define AMHW_FSL_UART_INT_BDH_RXEDGIE (1UL << 8) /**< \brief RxD输入有效边沿中断  */
#define AMHW_FSL_UART_INT_C2_TIE      (1UL << 7) /**< \brief TDRE发送准备就绪中断 */
#define AMHW_FSL_UART_INT_C2_TCIE     (1UL << 6) /**< \brief TC发送完成中断       */
#define AMHW_FSL_UART_INT_C2_IRIE     (1UL << 5) /**< \brief RDRF接收准备就绪中断 */
#define AMHW_FSL_UART_INT_C2_IDIE     (1UL << 4) /**< \brief IDLE空闲线路中断使能 */
#define AMHW_FSL_UART_INT_C3_OVR      (1UL << 3) /**< \brief 溢出中断             */
#define AMHW_FSL_UART_INT_C3_NEIE     (1UL << 2) /**< \brief 噪声错误中断         */
#define AMHW_FSL_UART_INT_C3_IFRMERR  (1UL << 1) /**< \brief 帧错误中断           */
#define AMHW_FSL_UART_INT_C3_PARERR   (1UL << 0) /**< \brief 奇偶校验错误中断     */
#define AMHW_FSL_UART_INT_ALL         (0x3FF)    /**< \brief 所有中断             */

/** @} */

/**
 * \name UART串口中断状态标志，可以通过函数amhw_fsl_uart_intstat_get()获取中断状态
 * @{
 */
#define AMHW_FSL_UART_INTSTAT_S2_LBKDIF  (1UL << 15) /**< \brief LIN断点检测标志   */
#define AMHW_FSL_UART_INTSTAT_S2_RXEDGIF (1UL << 14) /**< \brief RxD输入有效沿标志 */
#define AMHW_FSL_UART_INTSTAT_S1_TDRE    (1UL << 7)  /**< \brief TDRE发送就绪标志  */
#define AMHW_FSL_UART_INTSTAT_S1_TC      (1UL << 6)  /**< \brief TC发送完成标志    */
#define AMHW_FSL_UART_INTSTAT_S1_RDRF    (1UL << 5)  /**< \brief RDRF接收就绪标志  */
#define AMHW_FSL_UART_INTSTAT_S1_IDLE    (1UL << 4)  /**< \brief IDLE空闲标志      */
#define AMHW_FSL_UART_INTSTAT_S1_OVR     (1UL << 3)  /**< \brief 溢出标志          */
#define AMHW_FSL_UART_INTSTAT_S1_NF      (1UL << 2)  /**< \brief 噪声错误标志      */
#define AMHW_FSL_UART_INTSTAT_S1_FE      (1UL << 1)  /**< \brief 帧错误标志        */
#define AMHW_FSL_UART_INTSTAT_S1_PF      (1UL << 0)  /**< \brief 奇偶校验错误标志  */

/** @} interrupt state */


/**
 * \name uart 接收器/发送器DMA使能位宏定义
 * @{
 */

#define AMHW_FSL_UART_DMA_RX_EN         ((uint8_t)(1<<5)) /**< \brief 接收器DMA使能  */
#define AMHW_FSL_UART_DMA_TX_EN         ((uint8_t)(1<<7)) /**< \brief 发送器DMA使能  */

/** @} */


/**
 * \name 串口特殊工作模式
 * \anchor amhw_fsl_uart_spec_mode
 * @{
 */

/** \brief 回环模式 */
#define AMHW_FSL_UART_SPEC_MODE_LOOP            0

/** \brief 单线模式 */
#define AMHW_FSL_UART_SPEC_MODE_SINGLE_WIRE     1

/** @} */


/**
 * \brief 串口波特率分频因子高位设置函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] sbr_val   : 波特率分频因子高位值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_bdh_sbr_set (amhw_fsl_uart_t *p_hw_uart, uint8_t sbr_val)
{
    p_hw_uart->bdh &= ~AMHW_FSL_UART_BDH_SBR_MASK;
    p_hw_uart->bdh |= AMHW_FSL_UART_BDH_SBR(sbr_val);
}
/**
 * \brief 串口波特率分频因子底位设置函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] sbr_val   : 波特率分频因子底位值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_bdl_sbr_set (amhw_fsl_uart_t *p_hw_uart, uint8_t sbr_val)
{
    p_hw_uart->bdl &= ~AMHW_FSL_UART_BDL_SBR_MASK;
    p_hw_uart->bdl |= AMHW_FSL_UART_BDL_SBR(sbr_val);

}

/**
 * \brief 发送器使能函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_c2_tx_enable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->c2 |= AMHW_FSL_UART_C2_TE_EN;
}
/**
 * \brief 发送器禁能函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_c2_tx_disable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->c2 &= ~AMHW_FSL_UART_C2_TE_EN;
}

/**
 * \brief 接收器使能函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_c2_rx_enable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->c2 |= AMHW_FSL_UART_C2_RE_EN;
}

/**
 * \brief 接收器禁能函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_c2_rx_disable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->c2 &= ~AMHW_FSL_UART_C2_RE_EN;
}
/**
 * \brief 使能串口
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_enable (amhw_fsl_uart_t *p_hw_uart)
{
    amhw_fsl_uart_c2_rx_enable(p_hw_uart);
    amhw_fsl_uart_c2_tx_enable(p_hw_uart);
}

/**
 * \brief 禁能串口
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_disable (amhw_fsl_uart_t *p_hw_uart)
{
    amhw_fsl_uart_c2_rx_disable(p_hw_uart);
    amhw_fsl_uart_c2_tx_disable(p_hw_uart);
}

/**
 * \brief 过采样率OSR值设置函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] osr_val   : 波特率分频因子高位值
 *
 * \note 过采样率只用于串口0
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_c4_osr_set (amhw_fsl_uart_t *p_hw_uart, uint8_t osr_val)
{
    uint8_t temp=0;
    temp = p_hw_uart->uart0_c4 ;
    temp &= ~AMHW_FSL_UART_C4_OSR_MASK;
    temp|= AMHW_FSL_UART_C4_OSR(osr_val);
    p_hw_uart->uart0_c4 =temp;
}

/**
 * \brief 写串口发送寄存器值（发送一个数据）
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] data      : 发送的数据
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_txdata_write (amhw_fsl_uart_t *p_hw_uart, uint8_t data)
{
    p_hw_uart->data = data;
}

/**
 * \brief 读取串口接收数据
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \return 串口接收到的数据
 */
am_static_inline
uint8_t amhw_fsl_uart_rxdata_read (amhw_fsl_uart_t *p_hw_uart)
{
     return p_hw_uart->data;
}


/**
 * \brief 得到串口发送状态
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return ：S1_TDRE位值，1时表明可以继续发送数据
 */
am_static_inline
uint8_t amhw_fsl_uart_stat1_tdre_get (amhw_fsl_uart_t *p_hw_uart)
{

   return (((uint8_t)(p_hw_uart->stat1 & AMHW_FSL_UART_S1_TDRE_MASK)) >> 7);
}

/**
 * \brief 得到串口发送完成标志
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return ：S1_TC位值，1时表明传输完成，0表示正在传输
 */
am_static_inline
uint8_t amhw_fsl_uart_stat1_tc_get (amhw_fsl_uart_t *p_hw_uart)
{
   return (((uint8_t)(p_hw_uart->stat1 & AMHW_FSL_UART_S1_TC_MASK)) >> 6);
}

/**
 * \brief 得到串口接收状态
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return ：S1_RDRF位值，1时表明可以继续接收数据
 */
am_static_inline
uint8_t amhw_fsl_uart_stat1_rdre_get (amhw_fsl_uart_t *p_hw_uart)
{
   return (((uint8_t)(p_hw_uart->stat1 & AMHW_FSL_UART_S1_RDRF_MASK)) >> 5);
}

/**
 * \brief 得到校验状态
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 *
 * \return PF位域值，1表示当前接收数据出现校验错误
 */
am_static_inline
uint8_t amhw_fsl_uart_stat1_pf_get (amhw_fsl_uart_t *p_hw_uart)
{
    return (p_hw_uart->stat1 & AMHW_FSL_UART_S1_PF_MASK);
}


/**
 * \brief UART数据模式位设置
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] datamode  : 数据模式
                          AMHW_FSL_UART_C1_M_*宏值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_data_mode_set (amhw_fsl_uart_t *p_hw_uart, uint8_t datamode)
{
    p_hw_uart->c1 &= ~(1<<4);
    p_hw_uart->c1|= datamode;
}

/**
 * \brief UART奇偶校验设置
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] parity    : 数据模式
                          AMHW_FSL_UART_C1_PARITY_*宏值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_parity_set (amhw_fsl_uart_t *p_hw_uart, uint8_t parity)
{

    p_hw_uart->c1 &= ~(3<<0);
    p_hw_uart->c1 |= parity;

}

/**
 * \brief UART停止位设置函数
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] stop_bit  : 停止模式位宏
                          AMHW_FSL_UART_BDH_SBNS_STOP_*宏值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_stop_bit_set (amhw_fsl_uart_t *p_hw_uart, uint8_t stop_bit)
{
    p_hw_uart->bdh &= ~(1<<5);
    p_hw_uart->bdh |= stop_bit;
}

/**
 * \brief 使能指定的串口中断
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 * \param[in] int_enable : AMHW_FSL_UART_INT_* 宏值或多个AMHW_FSL_UART_INT_*宏的或值
 *                         (#AMHW_FSL_UART_INT_BDH_LBKDIE)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_int_enable (amhw_fsl_uart_t *p_hw_uart, uint32_t int_enable)
{
     p_hw_uart->bdh |= ((uint8_t)((int_enable&0x300)>>2));
     p_hw_uart->c2  |= ((uint8_t)(int_enable&0xf0));
     p_hw_uart->c3  |= ((uint8_t)(int_enable&0x0f));
}

/**
 * \brief 禁能指定的串口中断
 *
 * \param[in] p_hw_uart   : 指向串口寄存器块的指针
 * \param[in] int_disable : AMHW_FSL_UART_INT_* 宏值或多个AMHW_FSL_UART_INT_*宏的或值
 *                          (#AMHW_FSL_UART_INT_BDH_LBKDIE)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_int_disable (amhw_fsl_uart_t *p_hw_uart, uint32_t int_disable)
{
    p_hw_uart->bdh &= ~((uint8_t)((int_disable&0x300)>>2));
    p_hw_uart->c2  &= ~((uint8_t)(int_disable&0xf0));
    p_hw_uart->c3  &= ~((uint8_t)(int_disable&0x0f));
}

/**
 * \brief 得到ver0 串口中断状态
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \return AMHW_FSL_UART_INTSTAT_*宏值或多个AMHW_FSL_UART_INTSTAT_*宏的或值
 *         (#AMHW_FSL_UART_INTSTAT_S2_LBKDIF)
 */
am_static_inline
uint32_t amhw_fsl_uart_ver0_intstat_get (amhw_fsl_uart_t *p_hw_uart)
{
    uint32_t intstat = 0;
    intstat = ((p_hw_uart->stat2 & 0xc0) << 8)|
               (p_hw_uart->stat1 & 0xff);
               
    /* if UART0, w1c s1 */

    p_hw_uart->stat1 |= 0x1F;

    return intstat;
}
/**
 * \brief 得到ver1 串口中断状态
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \return AMHW_FSL_UART_INTSTAT_*宏值或多个AMHW_FSL_UART_INTSTAT_*宏的或值
 *         (#AMHW_FSL_UART_INTSTAT_S2_LBKDIF)
 */
am_static_inline
uint32_t amhw_fsl_uart_ver1_intstat_get (amhw_fsl_uart_t *p_hw_uart)
{
    uint32_t intstat = 0;
    intstat = ((p_hw_uart->stat2 & 0xc0) << 8)|
               (p_hw_uart->stat1 & 0xff);
    
    return intstat;
}

/**
 * \brief 单线模式下Tx引脚的输入输出方向配置
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] dir       : 输入输出方向 #AMHW_FSL_UART_SINGLE_WIRE_INPUT 或者
 *                        #AMHW_FSL_UART_SINGLE_WIRE_OUTPUT
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_single_wire_dir_set (amhw_fsl_uart_t *p_hw_uart, uint8_t dir)
{
    p_hw_uart->c3 = ((p_hw_uart->c3 & (~AMHW_FSL_UART_C3_TXDIR)) | (dir << 5));
}


/**
 * \brief 使能指定的ver0 串口MDA接收传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver0_dma_rx_enable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->uart0_c5 |= AMHW_FSL_UART_DMA_RX_EN;
}
/**
 * \brief 使能指定的ver0 串口MDA接收传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver1_dma_rx_enable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->uart1_2_c4 |= AMHW_FSL_UART_DMA_RX_EN;
}

/**
 * \brief 禁能指定的ver0 串口MDA接收传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver0_dma_rx_disable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->uart0_c5 &= ~AMHW_FSL_UART_DMA_RX_EN;
}

/**
 * \brief 禁能指定的ver1 串口MDA接收传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver1_dma_rx_disable (amhw_fsl_uart_t *p_hw_uart)
{
    p_hw_uart->uart1_2_c4 &= ~AMHW_FSL_UART_DMA_RX_EN;
}

/**
 * \brief 使能指定的ver0 串口DMA发送传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver0_dma_tx_enable (amhw_fsl_uart_t *p_hw_uart)
{

    p_hw_uart->uart0_c5 |= AMHW_FSL_UART_DMA_TX_EN;

}

/**
 * \brief 使能指定的ver1 串口DMA发送传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver1_dma_tx_enable (amhw_fsl_uart_t *p_hw_uart)
{

    p_hw_uart->uart1_2_c4 |= AMHW_FSL_UART_DMA_TX_EN;

}
/**
 * \brief 禁能指定的ver0 串口MDA发送传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver0_dma_tx_disable (amhw_fsl_uart_t *p_hw_uart)
{

    p_hw_uart->uart0_c5 &= ~AMHW_FSL_UART_DMA_TX_EN;

}

/**
 * \brief 禁能指定的ver1 串口MDA发送传输
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_uart_ver1_dma_tx_disable (amhw_fsl_uart_t *p_hw_uart)
{

    p_hw_uart->uart1_2_c4 &= ~AMHW_FSL_UART_DMA_TX_EN;

}


/**
 * \brief UART特殊功能模式设置(回环模式、 单线模式)
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] mode      : 工作模式
 *
 * \return 无
 *
 * \note 普通的串口模式不需要使用到该函数进行设置
 */
void amhw_fsl_uart_special_mode_set (amhw_fsl_uart_t *p_hw_uart, uint8_t mode);


/**
 * \brief UART数据发送(查询模式)
 *
 * \param[in] p_hw_uart : 指向串口寄存器块的指针
 * \param[in] p_txbuf    : 发送数据缓冲区
 * \param[in] nbytes     : 发送数据个数
 *
 * \return 成功发送的数据个数
 */
uint32_t amhw_fsl_uart_poll_send (amhw_fsl_uart_t  *p_hw_uart,
                                   const uint8_t *p_txbuf,
                                   uint32_t       nbytes);

/**
 * \brief UART数据接收(查询模式)
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 * \param[in] p_rxbuf    : 接收数据缓冲区
 * \param[in] nbytes     : 接收数据个数
 *
 * \return 成功接收的数据个数
 */
uint32_t amhw_fsl_uart_poll_receive (amhw_fsl_uart_t *p_hw_uart,
                                      uint8_t      *p_rxbuf,
                                      uint32_t      nbytes);

/**
 * \brief VER0 串口波特率设置
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 * \param[in] sysclk     : 串口的输入时钟频率
 * \param[in] baud       : 期望设置的串口波特率
 *
 * \return 大于0表示实际的波特率，小于0表明设置失败。
 */
int amhw_fsl_uart_ver0_baudrate_set (amhw_fsl_uart_t *p_hw_uart, uint32_t sysclk, int baud);

/**
 * \brief VER1 串口波特率设置
 *
 * \param[in] p_hw_uart  : 指向串口寄存器块的指针
 * \param[in] sysclk     : 串口的输入时钟频率
 * \param[in] baud       : 期望设置的串口波特率
 *
 * \return 大于0表示实际的波特率，小于0表明设置失败。
 */
int amhw_fsl_uart_ver1_baudrate_set (amhw_fsl_uart_t *p_hw_uart, uint32_t sysclk, int baud);

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
 * @} amhw_fsl_if_uart
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_UART_H */

/* end of file */
