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
 * \brief I2S硬件层操作接口
 *
 * 1. 支持1个发送数据通道的发送器具有独立的位时钟和帧同步；
 * 2. 支持1个接收数据通道的接收器具有独立的位时钟和帧同步；
 * 3. 最大帧大小支持2个字；
 * 4. 字的位长度可以配置在8bits到32bits之间；
 * 5. 帧的第一个字和余下的字可以分开配置；
 * 6. 发送和接收通道都有一个32bit的FIFO；
 * 7. FIFO错误后支持重新启动。
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 2016-09-27  mkr, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_I2S_H
#define __AMHW_FSL_I2S_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup amhw_fsl_if_i2s
 * \copydoc amhw_fsl_i2s.h
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

 /**
 * \name I2S 发送控制寄存器TCSR相应的位掩码定义
 * @{
 */

#define AMHW_FSL_I2S_TCSR_TE_MASK       (1UL << 31)  /**< \brief 发送使能            */
#define AMHW_FSL_I2S_TCSR_STOPE_MASK    (1UL << 30)  /**< \brief 停止模式下发送使能 */
#define AMHW_FSL_I2S_TCSR_DBGE_MASK     (1UL << 29)  /**< \brief 调试模式下发送使能 */
#define AMHW_FSL_I2S_TCSR_BCE_MASK      (1UL << 28)  /**< \brief 位时钟使能          */
#define AMHW_FSL_I2S_TCSR_FR_MASK       (1UL << 25)  /**< \brief FIFO 复位，只写     */
#define AMHW_FSL_I2S_TCSR_SR_MASK       (1UL << 24)  /**< \brief 软件复位            */
#define AMHW_FSL_I2S_TCSR_WSF_MASK      (1UL << 20)  /**< \brief 字开始标志，写1清除 */
#define AMHW_FSL_I2S_TCSR_SEF_MASK      (1UL << 19)  /**< \brief 帧同步错误标志，写1清除 */
#define AMHW_FSL_I2S_TCSR_FWF_MASK      (1UL << 17)  /**< \brief FIFO 空标志，只读   */
#define AMHW_FSL_I2S_TCSR_FEF_MASK      (1UL << 18)  /**< \brief FIFO 溢出标志，写1清除 */
#define AMHW_FSL_I2S_TCSR_WSIE_MASK     (1UL << 12)  /**< \brief 字开始中断使能      */
#define AMHW_FSL_I2S_TCSR_SEIE_MASK     (1UL << 11)  /**< \brief 同步错误中断使能    */
#define AMHW_FSL_I2S_TCSR_FEIE_MASK     (1UL << 10)  /**< \brief FIFO 溢出中断使能   */
#define AMHW_FSL_I2S_TCSR_FWIE_MASK     (1UL << 9)   /**< \brief FIFO 警告中断使能   */
#define AMHW_FSL_I2S_TCSR_FWDE_MASK     (1UL << 1)   /**< \brief FIFO 警告时DMA请求  */
/** @} */

/**
 * \name I2S 发送配置寄存器TCR2相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_TCR2_SYNC_MASK        (3UL << 30)   /**< \brief 发送同步模式掩码 */
#define AMHW_FSL_I2S_TCR2_SYNC_DISABLE     (0UL << 30)   /**< \brief 发送异步模式     */
#define AMHW_FSL_I2S_TCR2_SYNC_SELF_RX     (1UL << 30)   /**< \brief 与自身的RX同步   */
#define AMHW_FSL_I2S_TCR2_SYNC_ANOTHER_TX  (2UL << 30)   /**< \brief 与另一个TX同步   */
#define AMHW_FSL_I2S_TCR2_SYNC_ANOTHER_RX  (3UL << 30)   /**< \brief 与另一个RX同步   */
#define AMHW_FSL_I2S_TCR2_BCS_MASK         (1UL << 29)   /**< \brief 发送位时钟交换   */
#define AMHW_FSL_I2S_TCR2_BCI_MASK         (1UL << 28)   /**< \brief 发送位时钟输入   */
#define AMHW_FSL_I2S_TCR2_MSEL_MASK        (3UL << 26)   /**< \brief 发送位时钟源     */
#define AMHW_FSL_I2S_TCR2_BCP_MASK         (1UL << 25)   /**< \brief 发送位时钟极性   */
#define AMHW_FSL_I2S_TCR2_BCD_MASK         (1UL << 24)   /**< \brief 发送位时钟方向   */
#define AMHW_FSL_I2S_TCR2_DIV_MASK         (0xffUL)      /**< \brief 发送位时钟分频数 */
/** @} */

/**
 * \name I2S 发送控制寄存器TCR3相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_TCR3_TCE_MASK      (1UL << 16)  /**< \brief 发送通道使能       */
#define AMHW_FSL_I2S_TCR3_WDFL_MASK     (1UL << 0)   /**< \brief 字标志配置         */
/** @} */

/**
 * \name I2S 发送控制寄存器TCR4相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_TCR4_FRSZ_MASK    (1UL << 16)    /**< \brief 发送帧大小      */
#define AMHW_FSL_I2S_TCR4_SYWD_MASK    (0x1fUL << 8)  /**< \brief 发送帧同步长度  */
#define AMHW_FSL_I2S_TCR4_MF_MASK      (1UL << 4)     /**< \brief 发送数据MSB在前 */
#define AMHW_FSL_I2S_TCR4_FSE_MASK     (1UL << 3)     /**< \brief 帧同步提前      */
#define AMHW_FSL_I2S_TCR4_FSP_MASK     (1UL << 1)     /**< \brief 帧同步极性      */
#define AMHW_FSL_I2S_TCR4_FSD_MASK     (1UL << 0)     /**< \brief 帧同步方向      */
/** @} */

/**
 * \name I2S 发送控制寄存器TCR5相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_TCR5_WNW_MASK    (0x1fUL << 24)  /**< \brief 发送字N位宽  */
#define AMHW_FSL_I2S_TCR5_W0W_MASK    (0x1fUL << 16)  /**< \brief 发送字0位宽  */
#define AMHW_FSL_I2S_TCR5_FBT_MASK    (0x1fUL << 8)   /**< \brief 发送首位移位 */
/** @} */

/**
 * \name I2S 发送掩码寄存器TMR相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_TMR_TWM_MASK    (0x03UL << 0)  /**< \brief 发送字掩码 */
/** @} */

/**
 * \name I2S 接收控制寄存器RCSR相应的位掩码定义
 * @{
 */

#define AMHW_FSL_I2S_RCSR_RE_MASK   (1UL << 31)  /**< \brief 接收使能                     */
#define AMHW_FSL_I2S_RCSR_STOPE     (1UL << 30)  /**< \brief 停止模式下接收使能  */
#define AMHW_FSL_I2S_RCSR_DBGE_MASK (1UL << 29)  /**< \brief 调试模式下接收使能  */
#define AMHW_FSL_I2S_RCSR_BCE_MASK  (1UL << 28)  /**< \brief 位时钟使能                 */
#define AMHW_FSL_I2S_RCSR_FR_MASK   (1UL << 25)  /**< \brief FIFO 复位，只写    */
#define AMHW_FSL_I2S_RCSR_SR_MASK   (1UL << 24)  /**< \brief 软件复位                      */
#define AMHW_FSL_I2S_RCSR_WSF_MASK  (1UL << 20)  /**< \brief 字开始标志，写1清除 */
#define AMHW_FSL_I2S_RCSR_SEF_MASK  (1UL << 19)  /**< \brief 帧同步错误标志，写1清除     */
#define AMHW_FSL_I2S_RCSR_FEF_MASK  (1UL << 18)  /**< \brief FIFO 溢出标志，写1清除  */
#define AMHW_FSL_I2S_RCSR_FWF_MASK  (1UL << 17)  /**< \brief FIFO 满标志，只读 */
#define AMHW_FSL_I2S_RCSR_WSIE_MASK (1UL << 12)  /**< \brief 字开始中断使能            */
#define AMHW_FSL_I2S_RCSR_SEIE_MASK (1UL << 11)  /**< \brief 同步错误中断使能         */
#define AMHW_FSL_I2S_RCSR_FEIE_MASK (1UL << 10)  /**< \brief FIFO 溢出中断使能   */
#define AMHW_FSL_I2S_RCSR_FWIE_MASK (1UL << 9)   /**< \brief FIFO 警告中断使能   */
#define AMHW_FSL_I2S_RCSR_FWDE_MASK (1UL << 1)   /**< \brief FIFO 警告时DMA请求  */
/** @} */

/**
 * \name I2S 接收控制寄存器RCR2相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_RCR2_SYNC_MASK        (3UL << 30)  /**< \brief 接收同步模式   */
#define AMHW_FSL_I2S_RCR2_SYNC_DISABLE     (0UL << 30)  /**< \brief 接收异步模式   */
#define AMHW_FSL_I2S_RCR2_SYNC_SELF_TX     (1UL << 30)  /**< \brief 接收身的RX同步 */
#define AMHW_FSL_I2S_RCR2_SYNC_ANOTHER_RX  (2UL << 30)  /**< \brief 接收一个TX同步 */
#define AMHW_FSL_I2S_RCR2_SYNC_ANOTHER_TX  (3UL << 30)  /**< \brief 接收一个RX同步 */
#define AMHW_FSL_I2S_RCR2_BCS_MASK         (1UL << 29)  /**< \brief 接收位时钟交换 */
#define AMHW_FSL_I2S_RCR2_BCI_MASK         (1UL << 28)  /**< \brief 接收位时钟输入 */
#define AMHW_FSL_I2S_RCR2_MSEL_MASK        (1UL << 26)  /**< \brief 接收位时钟源   */
#define AMHW_FSL_I2S_RCR2_BCP_MASK         (1UL << 25)  /**< \brief 接收位时钟极性 */
#define AMHW_FSL_I2S_RCR2_BCD_MASK         (1UL << 24)  /**< \brief 接收位时钟方向 */
#define AMHW_FSL_I2S_RCR2_DIV_MASK         (0xffUL)     /**< \brief 接收位时钟分频系数 */
/** @} */

/**
 * \name I2S 接收控制寄存器RCR3相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_RCR3_RCE_MASK      (1UL << 16)  /**< \brief 接收通道使能  */
#define AMHW_FSL_I2S_RCR3_WDFL_MASK     (1UL << 0)   /**< \brief 字标志配置    */
/** @} */

/**
 * \name I2S 接收控制寄存器RCR4相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_RCR4_FRSZ_MASK    (1UL << 16)    /**< \brief 接收帧大小      */
#define AMHW_FSL_I2S_RCR4_SYWD_MASK    (0x1fUL << 8)  /**< \brief 接收帧同步长度  */
#define AMHW_FSL_I2S_RCR4_MF_MASK      (1UL << 4)     /**< \brief 接收数据MSB在前 */
#define AMHW_FSL_I2S_RCR4_FSE_MASK     (1UL << 3)     /**< \brief 帧同步提前      */
#define AMHW_FSL_I2S_RCR4_FSP_MASK     (1UL << 1)     /**< \brief 帧同步极性      */
#define AMHW_FSL_I2S_RCR4_FSD_MASK     (1UL << 0)     /**< \brief 帧同步方向      */
/** @} */

/**
 * \name I2S 发送控制寄存器RCR5相应的位掩码定义
 * @{
 */
#define AMHW_FSL_I2S_RCR5_WNW_MASK    (0x1fUL << 24)  /**< \brief 接收字N位宽  */
#define AMHW_FSL_I2S_RCR5_W0W_MASK    (0x1fUL << 16)  /**< \brief 接收字0位宽  */
#define AMHW_FSL_I2S_RCR5_FBT_MASK    (0x1fUL << 8)   /**< \brief 接收首位移位 */
/** @} */

/**
  * \brief I2S寄存器块结构体
  */
typedef struct amhw_fsl_i2s {
    __IO uint32_t tcsr;              /**< \brief SAI发送控制寄存器1  */
    __I  uint32_t reserved0[1];      /**< \brief 保留                              */
    __IO uint32_t tcr2;              /**< \brief SAI发送控制寄存器2  */
    __IO uint32_t tcr3;              /**< \brief SAI发送控制寄存器3  */
    __IO uint32_t tcr4;              /**< \brief SAI发送控制寄存器4  */
    __IO uint32_t tcr5;              /**< \brief SAI发送控制寄存器5  */
    __I  uint32_t reserved1[2];      /**< \brief 保留                              */
    __O  uint32_t tdr;               /**< \brief SAI发送数据寄存器   */
    __I  uint32_t reserved2[15];     /**< \brief 保留                              */
    __IO uint32_t tmr;               /**< \brief SAI发送掩码寄存器   */
    __I  uint32_t reserved3[7];      /**< \brief 保留                              */
    __IO uint32_t rcsr;              /**< \brief SAI接收控制寄存器1  */
    __I  uint32_t reserved4[1];      /**< \brief 保留                              */
    __IO uint32_t rcr2;              /**< \brief SAI接收控制寄存器2  */
    __IO uint32_t rcr3;              /**< \brief SAI接收控制寄存器3  */
    __IO uint32_t rcr4;              /**< \brief SAI接收控制寄存器4  */
    __IO uint32_t rcr5;              /**< \brief SAI接收控制寄存器5  */
    __I  uint32_t reserved5[2];      /**< \brief 保留                              */
    __I  uint32_t rdr;               /**< \brief SAI接收数据寄存器   */
    __I  uint32_t reserved6[15];     /**< \brief 保留                              */
    __IO uint32_t rmr;               /**< \brief SAI接收掩码寄存器   */
    __I  uint32_t reserved7[7];      /**< \brief 保留                              */
    __IO uint32_t mcr;               /**< \brief SAI MCLK控制寄存器   */
    __IO uint32_t mdr;               /**< \brief SAI MCLK分频寄存器   */
} amhw_fsl_i2s_t;

/**
 * \brief 发送使用的I2S协议
 */
typedef enum amhw_fsl_i2s_protocol {
    AMHW_FSL_I2S_PROTOCOL_LEFT      = 0, /**< \brief 左边齐 */
    AMHW_FSL_I2S_PROTOCOL_RIGHET    = 1, /**< \brief 右对齐 */
    AMHW_FSL_I2S_PROTOCOL_TYPE      = 2, /**< \brief 典型I2S */
    AMHW_FSL_I2S_PROTOCOL_PCMA      = 3, /**< \brief PCMA */
    AMHW_FSL_I2S_PROTOCOL_PCMB      = 4  /**< \brief PCMB */
} amhw_fsl_i2s_protocol_t;

/**
  * \brief 发送主从模式
  */
typedef enum amhw_fsl_i2s_mode {
    AMHW_FSL_I2S_MODE_MASTER = 0,   /**< \brief 主机模式 */
    AMHW_FSL_I2S_MODE_SLAVE  = 1    /**< \brief 从机机模式 */
} amhw_fsl_i2s_mode_t;

/**
  * \brief 发送通道模式
  */
typedef enum amhw_fsl_i2s_channel_mode {
    AMHW_FSL_I2S_CHANNEL_MODE_MONO   = 0,  /**< \brief 1个通道1帧 */
    AMHW_FSL_I2S_CHANNEL_MODE_STEREO = 1   /**< \brief 2个通道1帧 */
} amhw_fsl_i2s_channel_mode_t;

/**
  * \brief 发送同步模式
  */
typedef enum amhw_fsl_i2s_sync_mode {
    AMHW_FSL_I2S_SYNC_DISABLED   = 0,  /**< \brief  异步模式           */
    AMHW_FSL_I2S_SYNC_SELF       = 1,  /**< \brief  与自身发送/接收器同步   */
    AMHW_FSL_I2S_SYNC_ANOTHER_TX = 2,  /**< \brief  与另一个SAI发送器同步 */
    AMHW_FSL_I2S_SYNC_ANOTHER_RX = 3   /**< \brief  与另一个SAI接收器同步 */
} amhw_fsl_i2s_sync_mode_t;

/**
  * \brief MCLK主时钟时钟源
  */
typedef enum amhw_fsl_i2s_mclk_src {
    AMHW_FSL_I2S_MCLK_SRC_SYSCLK  = 0, /**< \brief system clock            */
    AMHW_FSL_I2S_MCLK_SRC_SELECT1 = 1, /**< \brief OSCERCLK                 */
    AMHW_FSL_I2S_MCLK_SRC_SELECT2 = 2, /**< \brief Not Supported            */
    AMHW_FSL_I2S_MCLK_SRC_SELECT3 = 3  /**< \brief MCGPLLCLK/2 or MCGFLLCLK */
} amhw_fsl_i2s_mclk_src_t;

/**
  * \brief 发送位时钟源
  */
typedef enum amhw_fsl_i2s_bclk_src {
    AMHW_FSL_I2S_BCLK_SRC_BUS    = 0,  /**< \brief  bus_clk总线时钟    */
    AMHW_FSL_I2S_BCLK_SRC_MCLK   = 1,  /**< \brief  mclk主时钟                */
    AMHW_FSL_I2S_BCLK_SRC_OTHER0 = 2,  /**< \brief  Not Supported */
    AMHW_FSL_I2S_BCLK_SRC_OTHER1 = 3   /**< \brief  Not Supported */
} amhw_fsl_i2s_bclk_src_t;

/**
  * \brief I2S中断请求使能位
  */
typedef enum amhw_fsl_i2s_int_request {
    AMHW_FSL_I2S_INT_REQ_WORD_START = 1UL    << 12, /**< \brief  字起始中断            */
    AMHW_FSL_I2S_INT_REQ_SYNC_ERROR = 1UL    << 11, /**< \brief  同步错误中断         */
    AMHW_FSL_I2S_INT_REQ_FIFO_ERROR = 1UL    << 10, /**< \brief  FIFO错误中断     */
    AMHW_FSL_I2S_INT_REQ_FIFO_WARN  = 1UL    << 9,  /**< \brief  FIFO警告中断     */
    AMHW_FSL_I2S_INT_REQ_ALL        = 0x0FUL << 9,  /**< \brief  所有类型中断请求 */
} amhw_fsl_i2s_int_request_t;

/**
  * \brief I2S状态标志
  */
typedef enum amhw_fsl_i2s_state_flag {
    AMHW_FSL_I2S_STATE_FLAG_WORD_START   = 1UL << 20,  /**< \brief 字起始标志       */
    AMHW_FSL_I2S_STATE_FLAG_SYNC_ERROR   = 1UL << 19,  /**< \brief 同步错误标志  */
    AMHW_FSL_I2S_STATE_FLAG_FIFO_ERROR   = 1UL << 18,  /**< \brief FIFO错误标志 */
    AMHW_FSL_I2S_STATE_FLAG_FIFO_WARNING = 1UL << 17,  /**< \brief FIFO警告标志 */
    AMHW_FSL_I2S_STATE_FLAG_SOFT_RESET   = 1UL << 24,  /**< \brief 软件复位标志  */
    AMHW_FSL_I2S_STATE_FLAG_ALL          = 0x11E0000UL /**< \brief 所有复位标志  */
} amhw_fsl_i2s_state_flag_t;

/**
  * \brief I2S复位类型
  */
typedef enum amhw_fsl_i2s_reset_type {
    AMHW_FSL_I2S_RESET_TYPE_SORFWARE = 1UL << 24, /**< \brief  软件复位      */
    AMHW_FSL_I2S_RESET_TYPE_FIFO     = 1UL << 25, /**< \brief  FIFO复位     */
    AMHW_FSL_I2S_RESET_ALL           = 3UL << 24, /**< \brief  软件和FIFO都复位      */
} amhw_fsl_i2s_reset_type_t;

/**
  * \brief I2S运行模式
  */
typedef enum amhw_fsl_i2s_run_mode {
    AMHW_FSL_I2S_RUN_MODE_DEBUG = 0, /**< \brief  调试模式下运行  */
    AMHW_FSL_I2S_RUN_MODE_STOP  = 1  /**< \brief  停止模式下运行  */
} amhw_fsl_i2s_run_mode_t;

/**
  * \brief I2S时钟配置
  */
typedef struct amhw_fsl_i2s_clock_cfg {
    amhw_fsl_i2s_mclk_src_t mclk_src; /**< \brief  主时钟源            */
    amhw_fsl_i2s_bclk_src_t bclk_src; /**< \brief  位时钟源            */
    uint32_t mclk_src_freq;            /**< \brief  主时钟源频率  */
    uint32_t mclk;                     /**< \brief  主时钟频率       */
    uint32_t bclk;                     /**< \brief  位时钟频率       */
    uint32_t bclk_src_freq;            /**< \brief  位时钟源频率  */
} amhw_fsl_i2s_clock_cfg_t;

/*******************************************************************************
 * 发送器寄存器设置
 ******************************************************************************/

/**
 * \brief 设置I2S 发送控制/状态寄存器TCSR相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCSR_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcsr_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcsr |= flags;
}

/**
 * \brief 清除I2S 发送控制/状态寄存器TCSR相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCSR_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcsr_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcsr &= ~flags;
}

/**
 * \brief 获取I2S 发送控制/状态寄存器TCSR的值
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return 发送控制寄存器TCSR的值
 */
am_static_inline
uint32_t amhw_fsl_i2s_tcsr_get (amhw_fsl_i2s_t *p_hw_i2s)
{
    return (p_hw_i2s->tcsr);
}

/**
 * \brief 设置I2S 发送配置寄存器TCR2相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCR2_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcr2_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcr2 |= flags;
}

/**
 * \brief 清除I2S 发送配置寄存器TCR2相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCR2_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcr2_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcr2 &= ~flags;
}

/**
 * \brief 设置I2S 发送配置寄存器TCR3相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCR3_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcr3_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcr3 |= flags;
}

/**
 * \brief 清除I2S 发送配置寄存器TCR3相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCR3_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcr3_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcr3 &= ~flags;
}

/**
 * \brief 设置I2S 发送配置寄存器TCR4相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCR4_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcr4_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcr4 |= flags;
}

/**
 * \brief 清除I2S 发送配置寄存器TCR4相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCR4_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_tcr4_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tcr4 &= ~flags;
}

/**
 * \brief 设置I2S 发送位时钟分频系数
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] div      : MCLK时钟分频系数
 *
 * \note 实际分频数： (DIV + 1) * 2
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_bclk_div_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t div)
{
    p_hw_i2s->tcr2 = (p_hw_i2s->tcr2 & (~0xff)) | div;
}

/**
 * \brief 设置I2S 发送位时钟源
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] src      : I2S发送位时钟源
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_bclk_src_set (amhw_fsl_i2s_t          *p_hw_i2s,
                                    amhw_fsl_i2s_bclk_src_t  src)
{
    p_hw_i2s->tcr2 = (p_hw_i2s->tcr2 & (~(3UL << 26))) | (src << 26);
}

/**
 * \brief 设置I2S 发送帧大小
 *
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] size     : 帧字节大小，0:一个字大小，1:两个字大小
 *
 * \note 发送帧允许的最大帧大小为2个字
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_frame_size_set (amhw_fsl_i2s_t *p_hw_i2s,
                                      uint32_t         size)
{
    p_hw_i2s->tcr4 = (p_hw_i2s->tcr4 & (~(1UL << 16))) | (size << 16);
}

/**
 * \brief 设置I2S 发送帧同步位时钟长度
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] width    : 帧同步位时钟宽度
 *                       值为0时表示同步长度为1个位时钟
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_sywd_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t width)
{
    p_hw_i2s->tcr4 = ((p_hw_i2s->tcr4 & (~(0x1f << 8))) |
                     ((width & 0x1f) << 8));
}

/**
 * \brief 设置I2S 发送帧中除第一个字外的字的位大小
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] width    : 字的位大小
 *
 * \note set= width +1
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_wnw_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t width)
{
    p_hw_i2s->tcr5 = (p_hw_i2s->tcr5 & (~(0x1f << 24))) |
                     ((width & 0x1f) << 24);
}

/**
 * \brief 设置I2S 发送帧中第一个字位大小
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] width   : 字的位大小
 *
 * \note 不允许字的长度小于8bits，字的实际位长度= width + 1
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_w0w_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t width)
{
    p_hw_i2s->tcr5 = (p_hw_i2s->tcr5 & (~(0x1f << 16))) |
                     ((width & 0x1f) << 16);
}

/**
 * \brief 设置I2S 发送帧中字的位平移
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] shift    : 平移位数
 *
 * \note 与发送字的MSB设置相关，当发送MSB时，位移值 = 发送字位长度-1
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_fbt_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t shift)
{
    p_hw_i2s->tcr5 = (p_hw_i2s->tcr5 & (~(0x1f << 8))) |
                     ((shift & 0x1f) << 8);
}

/**
 * \brief I2S发送一个数据
 * \param[in] p_hw_i2s : set
 * \param[in] data     : 发送的数据
 *
 * \note 发送数据会进行写FIFO操作，发送要保证发送通道使能和FIFO空闲和无错
 * \return 无
 */
am_static_inline
void amhw_fsl_i2s_txdata_write (amhw_fsl_i2s_t *p_hw_i2s, uint32_t data)
{
    p_hw_i2s->tdr = data;
}

/**
 * \brief 设置I2S 发送数据掩码寄存器
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_FSL_I2S_TCR3_*宏值
 *
 * \note 帧中对应的字掩码使能后，使发送引脚高阻态，不会读取FIFO数据
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_mask_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->tmr = flags;
}

/*******************************************************************************
 * 接收器寄存器设置
 ******************************************************************************/

/**
 * \brief 设置I2S 接收控制/状态寄存器RCSR相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCSR_*宏值
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcsr_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcsr |= flags;
}

/**
 * \brief 清除I2S 接收控制/状态寄存器RCSR相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCSR_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcsr_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcsr &= ~flags;
}

/**
 * \brief 获取I2S 接收控制/状态寄存器RCSR的值
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return 接收控制寄存器RCSR的值
 */
am_static_inline
uint32_t amhw_fsl_i2s_rcsr_get (amhw_fsl_i2s_t *p_hw_i2s)
{
    return (p_hw_i2s->rcsr);
}

/**
 * \brief 设置I2S 接收配置寄存器RCR2相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCR2_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcr2_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcr2 |= flags;
}

/**
 * \brief 清除I2S 接收配置寄存器RCR2相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCR2_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcr2_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcr2 &= ~flags;
}

/**
 * \brief 设置I2S 接收配置寄存器RCR3相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCR3_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcr3_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcr3 |= flags;
}

/**
 * \brief 清除I2S 接收配置寄存器RCR3相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCR3_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcr3_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcr3 &= ~flags;
}

/**
 * \brief 设置I2S 接收配置寄存器RCR4相应的位为1
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCR4_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcr4_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcr4 |= flags;
}

/**
 * \brief 清除I2S 接收配置寄存器RCR4相应的位为0
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCR4_*宏值
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_rcr4_clr (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rcr4 &= ~flags;
}

/**
 * \brief 设置I2S 接收位时钟分频系数
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] div      : MCLK时钟分频系数
 *
 * \note 实际分频数： (DIV + 1) * 2
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_bclk_div_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t div)
{
    p_hw_i2s->rcr2 = ((p_hw_i2s->rcr2 & (~0xff)) | div);
}

/**
 * \brief 设置I2S 接收位时钟源
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] src      : I2S接收位时钟源
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_bclk_src_set (amhw_fsl_i2s_t          *p_hw_i2s,
                                    amhw_fsl_i2s_bclk_src_t  src)
{
    p_hw_i2s->rcr2 = ((p_hw_i2s->rcr2 & (~(3UL << 26))) | (src << 26));
}

/**
 * \brief 设置I2S 接收帧大小
 *
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] size     : 帧字节大小，0：一个字大小；1:两个字大小
 *
 * \note 接收帧允许的最大帧大小为2个字
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_frame_size_set (amhw_fsl_i2s_t *p_hw_i2s,
                                      uint32_t         size)
{
    p_hw_i2s->rcr4 = ((p_hw_i2s->rcr4 & (~(1UL << 16))) | (size << 16));
}

/**
 * \brief 设置I2S 接收帧同步位时钟长度
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] width    : 帧同步位时钟宽度
 *                       值为0时表示同步长度为1个位时钟
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_sywd_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t width)
{
    p_hw_i2s->rcr4 = ((p_hw_i2s->rcr4 & (~(0x1f << 8))) |
                     ((width & 0x1f) << 8));
}

/**
 * \brief 设置I2S 接收帧中除第一个字外的字的位大小
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] width    : 字的位大小
 *
 * \note set = width +1
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_wnw_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t width)
{
    p_hw_i2s->rcr5 = ((p_hw_i2s->rcr5 & (~(0x1f << 24))) |
                     ((width & 0x1f) << 24));
}

/**
 * \brief 设置I2S 接收帧中第一个字位大小
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] width    : 字的位大小
 *
 * \note 不允许字的长度小于8bits，字的实际位长度 = width +1
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_w0w_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t width)
{
    p_hw_i2s->rcr5 = ((p_hw_i2s->rcr5 & (~(0x1f << 16))) |
                     ((width & 0x1f) << 16));
}

/**
 * \brief 设置I2S 接收帧中字的位平移
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] shift    : 平移位数
 *
 * \note 与接收字的MSB设置相关，当接收MSB时，位移值 = 接收字位长度-1
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_fbt_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t shift)
{
    p_hw_i2s->rcr5 = ((p_hw_i2s->rcr5 & (~(0x1f << 8))) |
                     ((shift & 0x1f) << 8));
}

/**
 * \brief I2S接收一个数据
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return 接收的数据
 */
am_static_inline
uint32_t amhw_fsl_i2s_rxdata_read (amhw_fsl_i2s_t *p_hw_i2s)
{
    return (p_hw_i2s->rdr);
}

/**
 * \brief 设置I2S 接收数据掩码寄存器
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flags    : AMHW_I2S_RCR3_*宏值
 *
 * \note 帧中对应的字掩码使能后，使接收引脚高阻态，不会读取FIFO数据
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_mask_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t flags)
{
    p_hw_i2s->rmr = flags;
}

/*******************************************************************************
* 主时钟设置
*******************************************************************************/

/**
 * \brief MCLK分频时钟输入源选择
 * \param[in] p_hw_i2s : 指向i2s寄存器块的指针
 * \param[in] src      : MCLK分频时钟输入源
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_mclk_src_set (amhw_fsl_i2s_t          *p_hw_i2s,
                                 amhw_fsl_i2s_mclk_src_t  src)
{
    p_hw_i2s->mcr = (p_hw_i2s->mcr & (~(3UL << 24))) | (src << 24);
}

/**
 * \brief MCLK时钟输出使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_mclk_enable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->mcr |= 1 << 30;
}

/**
 * \brief MCLK时钟输出禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \note MCLK时钟输出禁能时，分频器关闭，引脚作为MCLK输入
 * \return    无
 */
am_static_inline
void amhw_fsl_i2s_mclk_disable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->mcr &= ~(1 << 30);
}

/**
 * \brief 获取I2S MCLK分频器更新状态标志
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \note 仅当MCLK分频器状态为0时，可以设置分频值
 * \retval 0: MCLK 分频频率未更新
 * \retval 1: MCLK 分频频率已经更新
 */
am_static_inline
am_bool_t amhw_fsl_i2s_mclk_duf_get (amhw_fsl_i2s_t *p_hw_i2s)
{
    return ((am_bool_t)((p_hw_i2s->mcr >> 31) & 0x01));
}

/**
 * \brief 设置I2S MCLK时钟分频的分子值
 *        MCLK output = MCLK input * ( (FRACT + 1) / (DIVIDE + 1) ).
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] fract    : MCLK时钟分频的分子值0~127
 *
 * \note MCLK时钟分频的FRACT值必须小于或等于DIV
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_mclk_fract_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t fract)
{
    p_hw_i2s->mdr = (p_hw_i2s->mdr & (~(0xff << 12))) | ((fract & 0xff) << 12);
}

/**
 * \brief 设置I2S MCLK时钟小数分频的分母值
 *        MCLK output = MCLK input * ( (FRACT + 1) / (DIVIDE + 1) ).
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] div      : MCLK时钟分频的分母值
 *
 * \note MCLK时钟分频的FRACT值必须小于或等于DIV
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_mclk_div_set (amhw_fsl_i2s_t *p_hw_i2s, uint32_t div)
{
    p_hw_i2s->mdr = (p_hw_i2s->mdr & (~0xfff)) | (div & 0xfff);
}

/*******************************************************************************
*  API
*******************************************************************************/

/**
 * \brief I2S主时钟分频配置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] mclk     : i2s主时钟频率
 * \param[in] src_clk  : 主时钟使用的时钟源频率
 *
 * \return  无
 */
void amhw_fsl_i2s_mclk_divider_cfg (amhw_fsl_i2s_t *p_hw_i2s,
                                     uint32_t         mclk,
                                     uint32_t         src_clk);

/**
 * \brief I2S发送寄存器初始化
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_init (amhw_fsl_i2s_t *p_hw_i2s);

/**
 * \brief I2S接收寄存器初始化
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
void amhw_i2s_rx_init (amhw_fsl_i2s_t *p_hw_i2s);

/**
 * \brief I2S发协议设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_protocol_set (amhw_fsl_i2s_t          *p_hw_i2s,
                                    amhw_fsl_i2s_protocol_t  protocol);

/**
 * \brief I2S接收协议设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_protocol_set (amhw_fsl_i2s_t          *p_hw_i2s,
                                    amhw_fsl_i2s_protocol_t  protocol);

/**
 * \brief I2S发送时钟配置
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] clk_config : i2s发送时钟配置参数
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_clock_setup (amhw_fsl_i2s_t           *p_hw_i2s,
                                   amhw_fsl_i2s_clock_cfg_t *clk_config);

/**
 * \brief I2S接收时钟配置
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] clk_config : i2s发送时钟配置参数
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_clock_setup (amhw_fsl_i2s_t           *p_hw_i2s,
                                   amhw_fsl_i2s_clock_cfg_t *clk_config);

/**
 * \brief I2S发送字长度配置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 * \param[in] bits : 位长度
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_word_width_cfg (amhw_fsl_i2s_t         *p_hw_i2s,
                                      amhw_fsl_i2s_protocol_t protocol,
                                      uint32_t                 bits);

/**
 * \brief I2S接收字长度配置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] protocol : 使用的I2S协议
 * \param[in] bits     : 位长度
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_word_width_cfg (amhw_fsl_i2s_t          *p_hw_i2s,
                                      amhw_fsl_i2s_protocol_t  protocol,
                                      uint32_t                  bits);

/**
 * \brief I2S发送通道使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_channel_enable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->tcr3 |= AMHW_FSL_I2S_TCR3_TCE_MASK;
}

/**
 * \brief I2S接收通道使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_channel_enable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->rcr3 |= AMHW_FSL_I2S_RCR3_RCE_MASK;
}

/**
 * \brief I2S发送通道模式配置（单声道或立体声）
 * \param[in] p_hw_i2s     : 指向I2S寄存器块的指针
 * \param[in] channel_mode : 发送通道模式
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_channel_mode_set (amhw_fsl_i2s_t              *p_hw_i2s,
                                        amhw_fsl_i2s_channel_mode_t  channel_mode);

/**
 * \brief I2S接收通道模式配置（单声道或立体声）
 * \param[in] p_hw_i2s     : 指向I2S寄存器块的指针
 * \param[in] channel_mode : 发送通道模式
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_channel_mode_set (amhw_fsl_i2s_t              *p_hw_i2s,
                                        amhw_fsl_i2s_channel_mode_t  channel_mode);

/**
 * \brief I2S发送主从模式设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] tx_mode  : 发送模式
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_mode_set (amhw_fsl_i2s_t      *p_hw_i2s,
                                amhw_fsl_i2s_mode_t  tx_mode);

/**
 * \brief I2S接收主从模式设置
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] tx_mode  : 发送模式
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_mode_set (amhw_fsl_i2s_t      *p_hw_i2s,
                                amhw_fsl_i2s_mode_t  tx_mode);

/**
 * \brief I2S发送同步的模式配置
 * \param[in] p_hw_i2s  : 指向I2S寄存器块的指针
 * \param[in] sync_mode : 发送同步的模式
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_sync_mode_set (amhw_fsl_i2s_t           *p_hw_i2s,
                                     amhw_fsl_i2s_sync_mode_t  sync_mode);

/**
 * \brief I2S接收同步的模式配置
 * \param[in] p_hw_i2s  : 指向I2S寄存器块的指针
 * \param[in] sync_mode : 发送同步的模式
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_sync_mode_set (amhw_fsl_i2s_t           *p_hw_i2s,
                                     amhw_fsl_i2s_sync_mode_t  sync_mode);

/**
 * \brief I2S 发送中断请求使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] int_req  : 中断请求的类型
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_int_enable (amhw_fsl_i2s_t             *p_hw_i2s,
                                  amhw_fsl_i2s_int_request_t  int_req)
{
    p_hw_i2s->tcsr |= int_req;
}

/**
 * \brief I2S 接收中断请求使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] int_req  : 中断请求的类型
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_int_enable (amhw_fsl_i2s_t *p_hw_i2s,
                                  amhw_fsl_i2s_int_request_t int_req)
{
    p_hw_i2s->rcsr |= int_req;
}

/**
 * \brief I2S 发送中断请求禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] int_req : 中断请求的类型
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_int_disable (amhw_fsl_i2s_t *p_hw_i2s,
                                   amhw_fsl_i2s_int_request_t int_req)
{
    p_hw_i2s->tcsr &= ~int_req;
}

/**
 * \brief I2S 接收中断请求禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] int_req  : 中断请求的类型
 *
 * \return  无
 */
am_static_inline
void amhw_i2s_rx_int_disable (amhw_fsl_i2s_t             *p_hw_i2s,
                              amhw_fsl_i2s_int_request_t  int_req)
{
    p_hw_i2s->rcsr &= ~int_req;
}

/**
 * \brief I2S 发送DMA请求使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_dma_request_enable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->tcsr |= AMHW_FSL_I2S_TCSR_FWDE_MASK;
}

/**
 * \brief I2S 接收DMA请求使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_dma_request_enable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->rcsr |= AMHW_FSL_I2S_TCSR_FWDE_MASK;
}

/**
 * \brief I2S 发送DMA请求禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_dma_request_disable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->tcsr &= ~AMHW_FSL_I2S_TCSR_FWDE_MASK;
}

/**
 * \brief I2S 接收DMA请求禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_dma_request_disable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->tcsr &= ~AMHW_FSL_I2S_TCSR_FWDE_MASK;
}

/**
 * \brief I2S 发送清除状态标志
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] flag_mask : 状态的标志
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_state_flag_clr (amhw_fsl_i2s_t            *p_hw_i2s,
                                      amhw_fsl_i2s_state_flag_t  flag_mask);

/**
 * \brief I2S 接收清除状态标志
 * \param[in] p_hw_i2s  : 指向I2S寄存器块的指针
 * \param[in] flag_mask : 状态的标志
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_state_flag_clr (amhw_fsl_i2s_t            *p_hw_i2s,
                                      amhw_fsl_i2s_state_flag_t  flag_mask);

/**
 * \brief I2S发送器复位
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] reset_mask : 复位的类型
 *
 * \return  无
 */
void amhw_fsl_i2s_tx_reset_set (amhw_fsl_i2s_t            *p_hw_i2s,
                                 amhw_fsl_i2s_reset_type_t  reset_mask);

/**
 * \brief I2S接收器复位
 * \param[in] p_hw_i2s   : 指向I2S寄存器块的指针
 * \param[in] reset_mask : 复位的类型
 *
 * \return  无
 */
void amhw_fsl_i2s_rx_reset_set (amhw_fsl_i2s_t            *p_hw_i2s,
                                 amhw_fsl_i2s_reset_type_t  reset_mask);

/**
 * \brief I2S 发送器使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_enable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->tcsr |= AMHW_FSL_I2S_TCSR_BCE_MASK;
    p_hw_i2s->tcsr |= AMHW_FSL_I2S_TCSR_TE_MASK;
}

/**
 * \brief I2S 接收器使能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_enable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->rcsr |= AMHW_FSL_I2S_RCSR_BCE_MASK;
    p_hw_i2s->rcsr |= AMHW_FSL_I2S_RCSR_RE_MASK;
}

/**
 * \brief I2S 发送器禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_tx_disable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->tcsr &= ~AMHW_FSL_I2S_TCSR_BCE_MASK;
    p_hw_i2s->tcsr &= ~AMHW_FSL_I2S_TCSR_TE_MASK;
}

/**
 * \brief I2S 接收器禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2s_rx_disable (amhw_fsl_i2s_t *p_hw_i2s)
{
    p_hw_i2s->rcsr &= ~AMHW_FSL_I2S_RCSR_BCE_MASK;
    p_hw_i2s->rcsr &= ~AMHW_FSL_I2S_RCSR_RE_MASK;
}

/**
 * \brief I2S发送数据（查询模式）
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] txBuff   : 指向要发送缓冲区的指针
 * \param[in] size     : 要发送数据的长度
 *
 * \return  无
 */
void amhw_fsl_i2s_senddata_polling (amhw_fsl_i2s_t *p_hw_i2s,
                                     uint32_t        *txBuff,
                                     uint32_t         size);

/**
 * \brief I2S接收数据（查询模式）
 * \param[in] p_hw_i2s : 指向I2S寄存器块的指针
 * \param[in] rxBuff   : 指向要接收缓冲区的指针
 * \param[in] size     : 要接收数据的长度
 *
 * \return  无
 */
void amhw_fsl_i2s_receivedata_polling (amhw_fsl_i2s_t *p_hw_i2s,
                                        uint8_t         *rxBuff,
                                        uint32_t         size);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_I2S_H */

/* end of file */
