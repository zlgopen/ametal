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
 * \brief I2C 硬件操作接口
 *
 * 1. 2个I2C接口；
 * 2. 可编程选择64个串行时钟频率；
 * 3. 支持多个主机和具有从机的多主机；
 * 4. 支持中断驱动的逐字节数据传输；
 * 5. 通过软件配置，可支持7bits和10bits地址的设备；
 * 6. 支持系统管理总线；
 * 7. 在无片上时钟请求时接收数据和从机地址匹配，可以将设备从掉电模式中唤醒；
 * 8. 支持DMA；
 * 9. 可编程电子脉冲滤波器
 *
 * \internal
 * Modification History
 * - 1.00 16-09-19  ari, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_I2C_H
#define __AMHW_FSL_I2C_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
/**
 * @addtogroup amhw_fsl_if_i2c
 * @copydoc amhw_fsl_i2c.h
 * @{
 */

/**
 * \brief I2C 寄存器块结构体
 */
typedef struct amhw_fsl_i2c {
    __IO uint8_t  address1;         /**< \brief I2C 地址寄存器1  */
    __IO uint8_t  clkdiv;           /**< \brief I2C 时钟分频寄存器  */
    __IO uint8_t  control1;         /**< \brief I2C 控制寄存器1 */
    __IO uint8_t  status ;          /**< \brief I2C 状态寄存器     */
    __IO uint8_t  data;             /**< \brief I2C 数据I/O寄存器     */
    __IO uint8_t  control2;         /**< \brief I2C 控制寄存器2 */
    __IO uint8_t  glitch_filter;    /**< \brief I2C 可编程脉冲滤波寄存器   */
    __IO uint8_t  range_address;    /**< \brief I2C 范围地址寄存器  */
    __IO uint8_t  smbus;            /**< \brief I2C SMBUS控制与状态寄存器   */
    __IO uint8_t  address2;         /**< \brief I2C 地址寄存器2 */
    __IO uint8_t  timeout_high;     /**< \brief I2C 低电平超时高位寄存器   */
    __IO uint8_t  timeout_low;      /**< \brief I2C 低电平超时低位寄存器   */
} amhw_fsl_i2c_t;

/**
 * \name I2C传输模式
 * \anchor grep_amhw_fsl_i2c_transmode
 * @{
 */

#define AMHW_FSL_I2C_TRANSMODE_RECV  0          /**< \brief I2C 接收模式         */
#define AMHW_FSL_I2C_TRANSMODE_SEND  AM_BIT(4)  /**< \brief I2C 发送模式         */

/** @} */

/**
 * \name I2C 工作模式
 * \anchor grep_amhw_fsl_i2c_workmode
 * @{
 */

#define AMHW_FSL_I2C_WORKMDOE_SLAVE      0        /**< \brief I2C 从机模式       */
#define AMHW_FSL_I2C_WORKMDOE_MASTER  AM_BIT(5)  /**< \brief I2C 主机模式        */

/** @} */

/**
 * \name I2C 状态标志
 * \anchor grep_amhw_fsl_i2c_stat
 * @{
 */

#define AMHW_FSL_I2C_STAT_TCF    AM_BIT(7)     /**< \brief I2C 传输完成标志      */
#define AMHW_FSL_I2C_STAT_IAAS   AM_BIT(6)     /**< \brief I2C 作为从机被寻址    */
#define AMHW_FSL_I2C_STAT_BUSY   AM_BIT(5)     /**< \brief I2C 总线忙标志        */
#define AMHW_FSL_I2C_STAT_ARBL   AM_BIT(4)     /**< \brief I2C 总线仲裁丢失标志  */
#define AMHW_FSL_I2C_STAT_RAM    AM_BIT(3)     /**< \brief I2C 地址范围寄存器匹配  */
#define AMHW_FSL_I2C_STAT_SRW    AM_BIT(2)     /**< \brief I2C 从设备发送状态    */
#define AMHW_FSL_I2C_STAT_IICIF  AM_BIT(1)     /**< \brief I2C 中断标志          */
#define AMHW_FSL_I2C_STAT_RXAK   AM_BIT(0)     /**< \brief I2C 无回应信号        */

/** @} */

/**
 * \name I2C 地址模式
 * \anchor grep_amhw_fsl_i2c_addrmode
 * @{
 */

#define AMHW_FSL_I2C_ADDRMODE_BIT7        0        /**< \brief I2C 7位地址模式   */
#define AMHW_FSL_I2C_ADDRMODE_BIT10    AM_BIT(6)   /**< \brief I2C 10位地址模式  */

/** @} */

/**
 * \name I2C 驱动模式
 * \anchor grep_amhw_fsl_i2c_drivemode
 * @{
 */

#define AMHW_FSL_I2C_DRIVEMODE_NORMAL  0         /**< \brief I2C 正常驱动模式   */
#define AMHW_FSL_I2C_DRIVEMODE_HIGH   AM_BIT(5)  /**< \brief I2C 高速驱动模式   */

/** @} */

/**
 * \name I2C 从机时钟模式
 * \anchor grep_amhw_fsl_i2c_s_clkmode
 * @{
 */

#define AMHW_FSL_I2C_S_CLKMODE_FALLOW_M    0      /**< \brief I2C 从机跟随主机   */
#define AMHW_FSL_I2C_S_CLKMODE_ALONE   AM_BIT(4)  /**< \brief I2C 从机波特率独立 */

/** @} */

/**
 * \name I2C 超时标志
 * \anchor grep_amhw_fsl_i2c_tmout_flags
 * @{
 */

#define AMHW_FSL_I2C_TMOUT_FLAG_SCL_LOW    AM_BIT(3) /**< \brief SCL低超时标志  */
#define AMHW_FSL_I2C_TMOUT_FLAG_IDEL       AM_BIT(2) /**< \brief 空闲超时       */
#define AMHW_FSL_I2C_TMOUT_FLAG_SDA_LOW    AM_BIT(1) /**< \brief SCL高超时标志2 */

/** @} */

/**
 * \name I2C中断选项
 * \anchor grep_amhw_fsl_i2c_int_option
 * \note 只有使能了全局中断，其他中断才有效
 *          - 字节传输完成中断  ：IICIE(TCF)
 *          - 地址匹配中断       : IICIE(IAAS)
 *          - 仲裁丢失中断      ：IICIE(ARBL)
 *          - 停止信号或起始信号检测中断  ：IICIE & SSIE（STOPF | STARTF（I2Cx_FLT第4位））
 *          - SCL低电平超时中断 ：IICIE（SLTF）
 *          - SDA低电平超时中断 ：IICIE & SHTF2IE（SHTF2）
 *          - 低功耗模式唤醒中断：IICIE & WUEN（IAAS）
 *
 * @{
 */

#define AMHW_FSL_I2C_INT_IICIE   AM_BIT(7)      /**< \brief I2C全局中断       */
#define AMHW_FSL_I2C_INT_STOPIE  AM_BIT(6)      /**< \brief 停止信号检测中断  */
/**< \brief 实际上I2Cx_FLT寄存器第6位是起始信号和停止信号中断，手册上没有写，在KL03的手册中写了 */
#define AMHW_FSL_I2C_INT_SSIE    AM_BIT(6)      /**< \brief I2C Bus Stop or Start Interrupt Enable */
#define AMHW_FSL_I2C_INT_SHTF2IE AM_BIT(5)      /**< \brief SDA低电平超时中断 */
#define AMHW_FSL_I2C_INT_WUEN    AM_BIT(4)      /**< \brief 唤醒中断          */

/** @} */

/** \brief 超时时间输入时钟选择 */
typedef enum amhw_fsl_i2c_timeout_clk_sel {
    AMHW_FSL_I2C_MODULE_CLOCK_64 = 0,          /**< \brief I2C模块时钟64分频  */
    AMHW_FSL_I2C_MODULE_CLOCK_1  = AM_BIT(4)   /**< \brief I2C模块时钟        */
} amhw_fsl_i2c_timeout_clk_sel_t;

/**
 * \brief 使能I2C模块
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 |=  AM_BIT(7);
}

/**
 * \brief 禁能I2C模块
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 &= ~AM_BIT(7);
}

/**
 * \brief 发送一个起始信号
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_start_signal_send (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 |=  AM_BIT(5);
}

/**
 * \brief 发送一个停止信号
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_stop_signal_send (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 &= ~AM_BIT(5);
}

/**
 * \brief 发送一个重复起始信号
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_restart_signal_send (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 |= AM_BIT(2);
}

/**
 * \brief 设置传输模式
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] mode     : 传输模式，参见 grep_amhw_fsl_i2c_transmode
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_transmode_set (amhw_fsl_i2c_t *p_hw_i2c , uint8_t mode)
{
    if (AMHW_FSL_I2C_TRANSMODE_SEND == mode) {
        p_hw_i2c->control1 |=  mode;
    } else if (AMHW_FSL_I2C_TRANSMODE_RECV == mode) {
        p_hw_i2c->control1 &= ~AM_BIT(4);
    }
}

/**
 * \brief 设置从机1地址
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] address  : 从机1的地址
 * \param[in] mode     : 地址模式
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addr1_set (amhw_fsl_i2c_t *p_hw_i2c,
                                     uint16_t    address,
                                     uint8_t     mode)
{

    if (mode == AMHW_FSL_I2C_ADDRMODE_BIT10) {
        p_hw_i2c->control2 |= AM_BIT(6);
        p_hw_i2c->control2 &= ~AM_SBF(0x07, 0);
        p_hw_i2c->control2 |= (address >> 8) & 0x7;
    } else {
        p_hw_i2c->control2 &= ~AM_BIT(6);
        p_hw_i2c->address1 = (address << 1);
    }
}

/**
 * \brief 获取从机1地址(低7位)
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
uint8_t amhw_fsl_i2c_addr1_get (amhw_fsl_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->address1 >> 1);
}


/**
 * \brief 设置分频值
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] div      : 写入的分频因子系数
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_clk_div_set (amhw_fsl_i2c_t *p_hw_i2c, uint8_t div)
{
    p_hw_i2c->clkdiv = div;
}

/**
 * \brief 使能I2C中断
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] irq_flag : 中断选项,参见 grep_amhw_fsl_i2c_int_option
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_int_enable (amhw_fsl_i2c_t *p_hw_i2c, uint8_t irq_flag)
{
    if (irq_flag & AMHW_FSL_I2C_INT_IICIE) {
        p_hw_i2c->control1 |= AM_BIT(6);
    }
    if(irq_flag & AMHW_FSL_I2C_INT_SSIE) {
        p_hw_i2c->glitch_filter |= AM_BIT(5);
    }
    if (irq_flag & AMHW_FSL_I2C_INT_SHTF2IE) {
        p_hw_i2c->smbus |= AM_BIT(0);
    }
    if(irq_flag & AMHW_FSL_I2C_INT_WUEN) {
        p_hw_i2c->control1 |= AM_BIT(1);
    }
}

/**
 * \brief 禁能I2C中断
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] irq_flag : 中断选项,参见 grep_amhw_fsl_i2c_int_option
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_int_disable (amhw_fsl_i2c_t *p_hw_i2c, uint8_t irq_flag)
{
    if (irq_flag & AMHW_FSL_I2C_INT_IICIE) {
        p_hw_i2c->control1 &= ~AM_BIT(6);
    }
    if(irq_flag & AMHW_FSL_I2C_INT_SSIE) {
        p_hw_i2c->glitch_filter &= ~AM_BIT(5);
    }
    if (irq_flag & AMHW_FSL_I2C_INT_SHTF2IE) {
        p_hw_i2c->smbus &= ~AM_BIT(0);
    }
    if(irq_flag & AMHW_FSL_I2C_INT_WUEN) {
        p_hw_i2c->control1 &= ~AM_BIT(1);
    }
}

/**
 * \brief I2C ACK应答
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_ack_respond (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 &= ~AM_BIT(3);
}

/**
 * \brief I2C 不应答（NAK）
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_nak_respond (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 |= AM_BIT(3);
}

/**
 * \brief I2C 重发
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 *
 * \note  出现仲裁丢失的时候用
 */
am_static_inline
void amhw_fsl_i2c_rsta (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 |= AM_BIT(2);
}

/**
 * \brief I2C DMA模式使能
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_dma_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 |= AM_BIT(0);
}

/**
 * \brief I2C DMA模式禁能
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_dma_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control1 &= ~AM_BIT(0);
}

/**
 * \brief 返回状态寄存器的值
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  状态标志，参见 grep_amhw_fsl_i2c_stat
 */
am_static_inline
uint8_t amhw_fsl_i2c_stat_get (amhw_fsl_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->status);
}

/**
 * \brief 清除状态寄存器标志
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] flags    : 状态标志,参见 grep_amhw_fsl_i2c_stat
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_stat_clr (amhw_fsl_i2c_t *p_hw_i2c , uint8_t flags)
{
    p_hw_i2c->status = flags;
}

/**
 * \brief 写数据到数据寄存器
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] data     : 写入的数据
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_data_write (amhw_fsl_i2c_t *p_hw_i2c , uint8_t data)
{
    p_hw_i2c->data = data;
}

/**
 * \brief 读取数据寄存器的值
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
uint8_t amhw_fsl_i2c_data_read (amhw_fsl_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->data);
}

/**
 * \brief 使能常用地址匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_general_call_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control2 |= AM_BIT(7);
}

/**
 * \brief 禁能常用地址匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_general_call_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control2 &= ~AM_BIT(7);
}

/**
 * \brief 设置I2C地址模式
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] mode     : 地址模式，参见 grep_amhw_fsl_i2c_addrmode
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addrmode_set (amhw_fsl_i2c_t *p_hw_i2c , uint8_t mode)
{
    p_hw_i2c->control2 &= ~AM_BIT(6);
    p_hw_i2c->control2 |=  mode;
}

/**
 * \brief 设置驱动模式
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] mode     : 驱动模式，参见 grep_amhw_fsl_i2c_drivemode
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_drivemode_set (amhw_fsl_i2c_t *p_hw_i2c , uint8_t mode)
{
    p_hw_i2c->control2 &= ~AM_BIT(5);
    p_hw_i2c->control2 |=  mode;
}

/**
 * \brief I2C 从机波特率来源设置
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] mode     : 从机时钟模式，参见 grep_amhw_fsl_i2c_s_clkmode
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_s_clkmode_set (amhw_fsl_i2c_t *p_hw_i2c , uint8_t mode)
{
    p_hw_i2c->control2 &= ~AM_BIT(4);
    p_hw_i2c->control2 |=  (mode);
}

/**
 * \brief 使能地址范围匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addr_range_match_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control2 |= AM_BIT(3);
}

/**
 * \brief 禁能地址范围匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addr_range_match_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->control2 &= ~AM_BIT(3);
}

/**
 * \brief 设置I2C范围寄存器的地址
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] addr     :写入的地址
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addr_range_set (amhw_fsl_i2c_t *p_hw_i2c, uint8_t addr)
{
    p_hw_i2c->range_address = (addr << 1);
}

/**
 * \brief 获取I2C范围寄存器的地址
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  地址
 */
am_static_inline
uint8_t amhw_fsl_i2c_addr_range_get (amhw_fsl_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->range_address >> 1);
}

/**
 * \brief 使能停止模式进入延迟
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 *
 * \note 使能后，数据传输完成后才会产生停止信号
 */
am_static_inline
void amhw_fsl_i2c_stop_entry_delay_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->glitch_filter |= AM_BIT(7);
}

/**
 * \brief 禁能停止模式进入延迟
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_stop_entry_delay_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->glitch_filter &= ~AM_BIT(7);
}

/**
 * \brief I2C 获取总线停止标志
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
am_bool_t amhw_fsl_i2c_stop_is_dec (amhw_fsl_i2c_t *p_hw_i2c)
{
    return AM_BIT_ISSET(p_hw_i2c->glitch_filter, 6) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除停止信号检测标志
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_stop_dec_clr (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->glitch_filter |= AM_BIT(6);
}

/**
 * \brief I2C 获取总线开始标志
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 *
 * \note FLT寄存器中第4位为总线开始标识，但是kl16和kl26的手册上都没写
 */
am_static_inline
am_bool_t amhw_fsl_i2c_start_is_dec (amhw_fsl_i2c_t *p_hw_i2c)
{
    return AM_BIT_ISSET(p_hw_i2c->glitch_filter, 4) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除开始信号检测标志
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_start_dec_clr (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->glitch_filter |= AM_BIT(4);
}

/**
 * \brief 设置滤波因子
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_flt_fact (amhw_fsl_i2c_t *p_hw_i2c, uint8_t fact)
{
    if (fact > 15)
        fact = 15;
    
    p_hw_i2c->glitch_filter &= ~0x0f;
    p_hw_i2c->glitch_filter |= fact;
}

/**
 * \brief 使能快速ACK
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 *
 * \note 使能快速ACK后，应答设置对当前接收的数据有效
 */
am_static_inline
void amhw_fsl_i2c_fast_ack_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->smbus |= AM_BIT(7);
}

/**
 * \brief 禁能快速ACK
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_fast_ack_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->smbus &= ~AM_BIT(7);
}

/**
 * \brief 使能警报地址匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_alertaddr_match_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->smbus |= AM_BIT(6);
}

/**
 * \brief 禁能警报地址匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_alertaddr_match_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->smbus &= ~AM_BIT(6);
}

/**
 * \brief 设置addr2的地址
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] addr     : 写入的地址
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addr2_set (amhw_fsl_i2c_t *p_hw_i2c, uint8_t addr)
{
    p_hw_i2c->address2 = (addr << 1);
}

/**
 * \brief 获取addr2的地址
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  addr2的地址
 */
am_static_inline
uint8_t amhw_fsl_i2c_addr2_get (amhw_fsl_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->address2 >> 1);
}

/**
 * \brief 使能第二地址匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addr2_match_enable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->smbus |= AM_BIT(5);
}

/**
 * \brief 禁能第二地址匹配
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_addr2_match_disable (amhw_fsl_i2c_t *p_hw_i2c)
{
    p_hw_i2c->smbus &= ~AM_BIT(5);
}

/**
 * \brief 选择超时计数器时钟
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] clk      : 时钟选择
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_timeout_clk_sel (amhw_fsl_i2c_t                *p_hw_i2c,
                                  amhw_fsl_i2c_timeout_clk_sel_t clk)
{
    p_hw_i2c->smbus &= ~AM_BIT(4);
    p_hw_i2c->smbus |= clk;
}

/**
 * \brief 获取I2C超时标志
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 *
 * \return 超时标志,参见 grep_amhw_fsl_i2c_tmout_flags
 */
am_static_inline
uint8_t amhw_fsl_i2c_tmout_flag_get (amhw_fsl_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->smbus & 0x0E);
}

/**
 * \brief I2C SCL超时标志清除
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指针
 * \param[in] flags    : 超时标志， 参见 grep_amhw_fsl_i2c_tmout_flags
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_tmout_flag_clr (amhw_fsl_i2c_t *p_hw_i2c , uint8_t flags)
{
    p_hw_i2c->smbus |= flags;
}

/**
 * \brief 设置I2C的SCL低电平超时值
 *
 * \param[in] p_hw_i2c : 指向i2c寄存器块的指
 * \param[in] value    : 从设备地址数据
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_i2c_scl_timeout_val_set (amhw_fsl_i2c_t *p_hw_i2c , uint16_t value)
{
    p_hw_i2c->timeout_high = value >> 8;
    p_hw_i2c->timeout_low  = value;
}

/**
 * @} amhw_fsl_if_i2c
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_I2C_H */


/* end of file */
