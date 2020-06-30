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
 * \brief SPI 硬件操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-23  cds, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_SPI_H
#define __AMHW_HC32F460_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_hc32f460_if_spi
 * \copydoc amhw_hc32f460_spi.h
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
 * \brief SPI 控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SPIMDS                    : 1;  /**< \brief SPI模式选择 */
    __IO uint32_t TXMDS                     : 1;  /**< \brief 通信模式选择 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t MSTR                      : 1;  /**< \brief SPI主从模式选择 */
    __IO uint32_t SPLPBK                    : 1;  /**< \brief SPI回环1 */
    __IO uint32_t SPLPBK2                   : 1;  /**< \brief SPI回环2 */
    __IO uint32_t SPE                       : 1;  /**< \brief SPI功能允许 */
    __IO uint32_t CSUSPE                    : 1;  /**< \brief 通讯自动挂起功能允许 */
    __IO uint32_t EIE                       : 1;  /**< \brief SPI错误中断允许 */
    __IO uint32_t TXIE                      : 1;  /**< \brief SPI发送中断允许 */
    __IO uint32_t RXIE                      : 1;  /**< \brief SPI接收中断允许 */
    __IO uint32_t IDIE                      : 1;  /**< \brief SPI空闲中断允许 */
    __IO uint32_t MODFE                     : 1;  /**< \brief 模式故障错误检测允许 */
    __IO uint32_t PATE                      : 1;  /**< \brief 奇偶校验自诊断 */
    __IO uint32_t PAOE                      : 1;  /**< \brief 奇偶校验模式选择 */
    __IO uint32_t PAE                       : 1;  /**< \brief 奇偶校验允许 */
    uint32_t RESERVED16                     :16;  /**< \brief 保留 */
} stc_spi_cr1_field_t;

/**
 * \brief SPI 通信配置寄存器1位域结构体
 */
typedef struct
{
    __IO uint32_t FTHLV                     : 2;  /**< \brief 帧数设定位 */
    uint32_t RESERVED2                      : 4;  /**< \brief 保留 */
    __IO uint32_t SPRDTD                    : 1;  /**< \brief 数据寄存器读取对象选择*/
    uint32_t RESERVED7                      : 1;  /**< \brief 保留 */
    __IO uint32_t SS0PV                     : 1;  /**< \brief SS0信号极性设定 */
    __IO uint32_t SS1PV                     : 1;  /**< \brief SS1信号极性设定 */
    __IO uint32_t SS2PV                     : 1;  /**< \brief SS2信号极性设定 */
    __IO uint32_t SS3PV                     : 1;  /**< \brief SS3信号极性设定 */
    uint32_t RESERVED12                     : 8;  /**< \brief 保留 */
    __IO uint32_t MSSI                      : 3;  /**< \brief SCK延迟设定位 */
    uint32_t RESERVED23                     : 1;  /**< \brief 保留 */
    __IO uint32_t MSSDL                     : 3;  /**< \brief SS无效延迟设定位 */
    uint32_t RESERVED27                     : 1;  /**< \brief 保留 */
    __IO uint32_t MIDI                      : 3;  /**< \brief SPI下次存取延迟设定位 */
    uint32_t RESERVED31                     : 1;  /**< \brief 保留 */
} stc_spi_cfg1_field_t;

/**
 * \brief SPI 状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t OVRERF                    : 1;  /**< \brief 过载错误标志 */
    __IO uint32_t IDLNF                     : 1;  /**< \brief SPI空闲标志 */
    __IO uint32_t MODFERF                   : 1;  /**< \brief 模式故障错误标志 */
    __IO uint32_t PERF                      : 1;  /**< \brief 奇偶校验错误标志 */
    __IO uint32_t UDRERF                    : 1;  /**< \brief 欠载错误标志 */
    __IO uint32_t TDEF                      : 1;  /**< \brief 发送缓冲器空标志 */
    uint32_t RESERVED6                      : 1;  /**< \brief 保留 */
    __IO uint32_t RDFF                      : 1;  /**< \brief 接收缓冲器满标志 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_spi_sr_field_t;

/**
 * \brief SPI 通信配置寄存器2位域结构体
 */
typedef struct
{
    __IO uint32_t CPHA                      : 1;  /**< \brief SCK相位设定位 */
    __IO uint32_t CPOL                      : 1;  /**< \brief SCK极性设定位 */
    __IO uint32_t MBR                       : 3;  /**< \brief 位速率分频设定位 */
    __IO uint32_t SSA                       : 3;  /**< \brief SS信号有效设定位 */
    __IO uint32_t DSIZE                     : 4;  /**< \brief SPI数据长度设定位 */
    __IO uint32_t LSBF                      : 1;  /**< \brief SPI LSB first位 */
    __IO uint32_t NXTDLE                    : 1;  /**< \brief SPI下次存取延迟允许 */
    __IO uint32_t SSDLE                     : 1;  /**< \brief SS无效延迟允许 */
    __IO uint32_t SCKDLE                    : 1;  /**< \brief SCK延迟允许 */
    uint32_t RESERVED16                     :16;  /**< \brief 保留 */
} stc_spi_cfg2_field_t;

typedef struct amhw_hc32f460_spi_t {
    __IO uint32_t DR;               /**< \brief SPI数据寄存器*/
    union
    {
        __IO uint32_t CR1;          /**< \brief SPI控制寄存器*/
        stc_spi_cr1_field_t CR1_f;
    };
    uint8_t RESERVED2[4];           /**< \brief 保留*/
    union
    {
        __IO uint32_t CFG1;         /**< \brief SPI通信配置寄存器1*/
        stc_spi_cfg1_field_t CFG1_f;
    };
    uint8_t RESERVED3[4];           /**< \brief 保留*/
    union
    {
        __IO uint32_t SR;           /**< \brief SPI状态寄存器*/
        stc_spi_sr_field_t SR_f;
    };
    union
    {
        __IO uint32_t CFG2;         /**< \brief SPI通信配置寄存器2*/
        stc_spi_cfg2_field_t CFG2_f;
    };
}amhw_hc32f460_spi_t;


/**
 * \brief  8bit数据写
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] data     : 待发送数据
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_tx_data8_put (amhw_hc32f460_spi_t *p_hw_spi,
                                     uint8_t              data)
{
    p_hw_spi->DR = data;
}

/**
 * \brief  16bit数据写
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] data     : 待发送数据
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_tx_data16_put (amhw_hc32f460_spi_t *p_hw_spi,
                                     uint16_t              data)
{
    p_hw_spi->DR = data;
}

/**
 * \brief  32bit数据写
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] data     : 待发送数据
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_tx_data32_put (amhw_hc32f460_spi_t *p_hw_spi,
                                      uint32_t             data)
{
    p_hw_spi->DR = data;
}

/**
 * \brief  8bit数据写
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] data     : 待发送数据
 *
 * \return none
 */
am_static_inline
uint8_t amhw_hc32f460_spi_rx_data8_get (amhw_hc32f460_spi_t *p_hw_spi)
{
    return p_hw_spi->DR;
}

/**
 * \brief  16bit数据写
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] data     : 待发送数据
 *
 * \return none
 */
am_static_inline
uint16_t amhw_hc32f460_spi_rx_data16_get (amhw_hc32f460_spi_t *p_hw_spi)
{
    return p_hw_spi->DR;
}

/**
 * \brief  32bit数据写
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] data     : 待发送数据
 *
 * \return none
 */
am_static_inline
uint32_t amhw_hc32f460_spi_rx_data32_get (amhw_hc32f460_spi_t *p_hw_spi)
{
    return p_hw_spi->DR;
}

/**
 * \brief  奇偶校验使能/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : 使能、禁能标志
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_parity_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                     am_bool_t            flag)
{
    p_hw_spi->CR1_f.PAE = flag;
}



/**
 * \brief SPI parity enumeration
 */
typedef enum amhw_hc32f460_spi_parity_mode
{
    SpiParityEven = 0u,       /**< \brief Select even parity send and receive */
    SpiParityOdd  = 1u,       /**< \brief Select odd parity send and receive */
} amhw_hc32f460_spi_parity_mode_t;

/**
 * \brief  奇偶校验模式选择
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] mode     : 校验模式(amhw_hc32f460_spi_parity_mode_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_parity_mode_sel(amhw_hc32f460_spi_t *p_hw_spi,
                                       uint32_t             mode)
{
    p_hw_spi->CR1_f.PAOE = mode;
}



/**
 * \brief  奇偶校验自诊断使能/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_parity_selfdetect_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                                uint32_t             flag)
{
    p_hw_spi->CR1_f.PATE = flag;
}



/**
 * \brief  模式故障错误检测允许
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_modefault_error_detect_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                                     uint32_t             flag)
{
    p_hw_spi->CR1_f.MODFE = flag;
}

#define AMHW_HC32F460_SPI_IDLE_INT_ENABLE_MASK    (1 << 11)
#define AMHW_HC32F460_SPI_RX_INT_ENABLE_MASK      (1 << 10)
#define AMHW_HC32F460_SPI_TX_INT_ENABLE_MASK      (1 << 9)
#define AMHW_HC32F460_SPI_ERROR_INT_ENABLE_MASK   (1 << 8)
#define AMHW_HC32F460_SPI_ALL_INT_ENABLE_MASK    (AMHW_HC32F460_SPI_IDLE_INT_ENABLE_MASK |\
                                                  AMHW_HC32F460_SPI_RX_INT_ENABLE_MASK   |\
                                                  AMHW_HC32F460_SPI_TX_INT_ENABLE_MASK   |\
                                                  AMHW_HC32F460_SPI_ERROR_INT_ENABLE_MASK)


/**
 * \brief  SPI空闲中断允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_idle_int_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                       uint32_t             flag)
{
    p_hw_spi->CR1_f.IDIE = flag;
}

/**
 * \brief  SPI接收中断允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_rx_int_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                     uint32_t             flag)
{
    p_hw_spi->CR1_f.RXIE = flag;
}

/**
 * \brief  SPI发送中断允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_tx_int_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                     uint32_t             flag)
{
    p_hw_spi->CR1_f.TXIE = flag;
}

/**
 * \brief  SPI错误中断允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_error_int_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                        uint32_t             flag)
{
    p_hw_spi->CR1_f.EIE = flag;
}

/**
 * \brief  SPI所有中断允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_all_int_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                      uint32_t             flag)
{
    if (flag) {
        p_hw_spi->CR1 |= AMHW_HC32F460_SPI_ALL_INT_ENABLE_MASK;
    } else {
        p_hw_spi->CR1 &= (~AMHW_HC32F460_SPI_ALL_INT_ENABLE_MASK);
    }
}


/**
 * \brief  通讯自动挂起功能允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_auto_suspend_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                           uint32_t             flag)
{
    p_hw_spi->CR1_f.CSUSPE = flag;
}

/**
 * \brief  SPI功能允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_enable(amhw_hc32f460_spi_t *p_hw_spi,
                              uint32_t             flag)
{
    p_hw_spi->CR1_f.SPE = flag;
}

/**
 * \brief  SPI回还（同相）模式允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_loopback_samephase_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                                  uint32_t             flag)
{
    p_hw_spi->CR1_f.SPLPBK2 = flag;
}

/**
 * \brief  SPI回还（反相）模式允许/禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_loopback_antiphase_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                                  uint32_t             flag)
{
    p_hw_spi->CR1_f.SPLPBK = flag;
}


/**
 * \brief SPI master/slave mode enumeration
 */
typedef enum amhw_hc32f460_spi_master_slave_mode
{
    AMHW_HC32F460_SPI_MODE_SLAVE  = 0u,        /**< \brief Spi slave mode */
    AMHW_HC32F460_SPI_MODE_MASTER = 1u,        /**< \brief Spi master mode */
} amhw_hc32f460_spi_master_slave_mode_t;


/**
 * \brief  SPI主从模式选择
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] mode     : 主/从模式(amhw_hc32f460_spi_master_slave_mode_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_mode_sel(amhw_hc32f460_spi_t *p_hw_spi,
                                uint32_t             flag)
{
    p_hw_spi->CR1_f.MSTR = flag;
}

/**
 * \brief SPI master/slave mode enumeration
 */
typedef enum amhw_hc32f460_spi_trans_mode
{
    AMHW_HC32F460_SPI_TRANS_MODE_FULL_DUPLEX = 0u,     /**< \brief Spi slave mode */
    AMHW_HC32F460_SPI_TRANS_MODE_ONLY_SEND   = 1u,     /**< \brief Spi master mode */
} amhw_hc32f460_spi_trans_mode_t;

/**
 * \brief  通信模式选择
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] mode     : 通信模式(amhw_hc32f460_spi_trans_mode_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_trans_mode_sel(amhw_hc32f460_spi_t *p_hw_spi,
                                      uint32_t             flag)
{
    p_hw_spi->CR1_f.TXMDS = flag;
}

/**
 * \brief SPI work mode enumeration
 */
typedef enum amhw_hc32f460_spi_work_mode
{
    AMHW_HC32F460_SPI_WORK_MODE_4LINE  = 0u,  /**< \brief 4 lines spi work mode */
    AMHW_HC32F460_SPI_WORK_MODE_3LINE  = 1u,  /**< \brief 3 lines spi work mode */
} amhw_hc32f460_spi_work_mode_t;

/**
 * \brief  SPI工作模式选择
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] mode     : 工作模式(amhw_hc32f460_spi_work_mode_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_work_mode_sel(amhw_hc32f460_spi_t *p_hw_spi,
                                      uint32_t             flag)
{
    p_hw_spi->CR1_f.SPIMDS = flag;
}



/**
 * \brief SPI SS next interval time enumeration
 */
typedef enum amhw_hc32f460_spi_ss_interval_time
{
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK1PLUSPCK2 = 0u,  /**< \brief Spi SS interval time 1 SCK plus 2 PCLK1 */
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK2PLUSPCK2 = 1u,  /**< \brief Spi SS interval time 2 SCK plus 2 PCLK1 */
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK3PLUSPCK2 = 2u,  /**< \brief Spi SS interval time 3 SCK plus 2 PCLK1 */
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK4PLUSPCK2 = 3u,  /**< \brief Spi SS interval time 4 SCK plus 2 PCLK1 */
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK5PLUSPCK2 = 4u,  /**< \brief Spi SS interval time 5 SCK plus 2 PCLK1 */
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK6PLUSPCK2 = 5u,  /**< \brief Spi SS interval time 6 SCK plus 2 PCLK1 */
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK7PLUSPCK2 = 6u,  /**< \brief Spi SS interval time 7 SCK plus 2 PCLK1 */
    AMHW_HC32F460_SPI_SS_INTERVAL_SCK8PLUSPCK2 = 7u,  /**< \brief Spi SS interval time 8 SCK plus 2 PCLK1 */
} amhw_hc32f460_spi_ss_interval_time_t;

/**
 * \brief  SPI下次存取延迟设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] time     : SPI 片选信号到下次存取的间隔时间(amhw_hc32f460_spi_ss_interval_time_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_ss_interval_time_set(amhw_hc32f460_spi_t *p_hw_spi,
                                            uint32_t             time)
{
    p_hw_spi->CFG1_f.MIDI = time;
}


/**
 * \brief SPI SS invalid delay time enumeration
 */
typedef enum amhw_hc32f460_spi_ss_invalid_delay
{
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK1 = 0u,  /**< \brief Spi SS invalid delay time 1 SCK*/
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK2 = 1u,  /**< \brief Spi SS invalid delay time 2 SCK*/
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK3 = 2u,  /**< \brief Spi SS invalid delay time 3 SCK*/
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK4 = 3u,  /**< \brief Spi SS invalid delay time 4 SCK*/
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK5 = 4u,  /**< \brief Spi SS invalid delay time 5 SCK*/
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK6 = 5u,  /**< \brief Spi SS invalid delay time 6 SCK*/
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK7 = 6u,  /**< \brief Spi SS invalid delay time 7 SCK*/
    AMHW_HC32F460_SPI_SS_INVALID_DELAY_SCK8 = 7u,  /**< \brief Spi SS invalid delay time 8 SCK*/
} amhw_hc32f460_spi_ss_invalid_delay_t;

/**
 * \brief  SS无效延迟设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] time     : SPI 片选信号到下次存取的间隔时间(amhw_hc32f460_spi_ss_invalid_delay_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_ss_invalid_delay_set(amhw_hc32f460_spi_t *p_hw_spi,
                                            uint32_t             time)
{
    p_hw_spi->CFG1_f.MSSDL = time;
}


/**
 * \brief SPI sck delay time enumeration
 */
typedef enum amhw_hc32f460_spi_ss_sck_delay
{
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK1 = 0u,  /**< \brief Spi SS invalid delay time 1 SCK*/
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK2 = 1u,  /**< \brief Spi SS invalid delay time 2 SCK*/
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK3 = 2u,  /**< \brief Spi SS invalid delay time 3 SCK*/
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK4 = 3u,  /**< \brief Spi SS invalid delay time 4 SCK*/
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK5 = 4u,  /**< \brief Spi SS invalid delay time 5 SCK*/
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK6 = 5u,  /**< \brief Spi SS invalid delay time 6 SCK*/
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK7 = 6u,  /**< \brief Spi SS invalid delay time 7 SCK*/
    AMHW_HC32F460_SPI_SS_SCK_DELAY_SCK8 = 7u,  /**< \brief Spi SS invalid delay time 8 SCK*/
} amhw_hc32f460_spi_ss_sck_delay_t;

/**
 * \brief  SS无效延迟设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] time     : SPI 片选信号到下次存取的间隔时间(amhw_hc32f460_spi_ss_sck_delay_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_ss_sck_delay_set(amhw_hc32f460_spi_t *p_hw_spi,
                                        uint32_t             time)
{
    p_hw_spi->CFG1_f.MSSI = time;
}



/**
 * \brief SPI SS polarity enumeration
 */
typedef enum amhw_hc32f460_spi_ss_polarity
{
    AMHW_HC32F460_SPI_SS_LOW_VALID  = 0u,  /**< \brief SS0~3 signal low level valid */
    AMHW_HC32F460_SPI_SS_HIGH_VALID = 1u,  /**< \brief SS0~3 signal high level valid */
} amhw_hc32f460_spi_ss_polarity_t;


/**
 * \brief  SS3信号极性设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] level    : SPI 片选有效信号极性(amhw_hc32f460_spi_ss_polarity_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_ss3_valid_level_set(amhw_hc32f460_spi_t *p_hw_spi,
                                          uint32_t             level)
{
    p_hw_spi->CFG1_f.SS3PV = level;
}

/**
 * \brief  SS2信号极性设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] level    : SPI 片选有效信号极性(amhw_hc32f460_spi_ss_polarity_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_ss2_valid_level_set(amhw_hc32f460_spi_t *p_hw_spi,
                                          uint32_t             level)
{
    p_hw_spi->CFG1_f.SS2PV = level;
}

/**
 * \brief  SS1信号极性设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] level    : SPI 片选有效信号极性(amhw_hc32f460_spi_ss_polarity_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_ss1_valid_level_set(amhw_hc32f460_spi_t *p_hw_spi,
                                           uint32_t             level)
{
    p_hw_spi->CFG1_f.SS1PV = level;
}

/**
 * \brief  SS0信号极性设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] level    : SPI 片选有效信号极性(amhw_hc32f460_spi_ss_polarity_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_ss0_valid_level_set(amhw_hc32f460_spi_t *p_hw_spi,
                                           uint32_t             level)
{
    p_hw_spi->CFG1_f.SS0PV = level;
}


/**
 * \brief SPI read object enumeration
 */
typedef enum amhw_hc32f460_spi_read_object_t
{
    AMHW_HC32F460_SPI_READ_RX_BUFFER  = 0u,  /**< \brief Read receive buffer */
    AMHW_HC32F460_SPI_READ_TX_BUFFER  = 1u,  /**< \brief Read send buffer (must be read TDEF = 1) */
} amhw_hc32f460_spi_read_object_t;

/**
 * \brief  数据寄存器读取对象选择
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] 读取对象        : SPI_DR 读接收缓冲器/读发送缓冲器(amhw_hc32f460_spi_read_object_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_read_object_set(amhw_hc32f460_spi_t *p_hw_spi,
                                       uint32_t             flag)
{
    p_hw_spi->CFG1_f.SPRDTD = flag;
}


/**
 * \brief SPI frame number enumeration
 */
typedef enum amhw_hc32f460_spi_frame_number
{
    AMHW_HC32F460_SPI_FRAME_NUMBER1  = 0u,  /**< \brief 1 frame data */
    AMHW_HC32F460_SPI_FRAME_NUMBER2  = 1u,  /**< \brief 2 frame data */
    AMHW_HC32F460_SPI_FRAME_NUMBER3  = 2u,  /**< \brief 3 frame data */
    AMHW_HC32F460_SPI_FRAME_NUMBER4  = 3u,  /**< \brief 4 frame data */
} amhw_hc32f460_spi_read_frame_number_t;

/**
 * \brief  帧数设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : SPI_DR 读接收缓冲器/读发送缓冲器(amhw_hc32f460_spi_read_frame_number_t)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_frame_number_set(amhw_hc32f460_spi_t *p_hw_spi,
                                        uint32_t             flag)
{
    p_hw_spi->CFG1_f.FTHLV = flag;
}


/**
 * \brief SPI status flag
 */

#define AMHW_HC32F460_SPI_STATUS_RXBUFFER_FULL   (1 << 7)  /**< \brief rxbuffer full status */
#define AMHW_HC32F460_SPI_STATUS_TXBUFFER_EMPTY  (1 << 5)  /**< \brief txbuffer empty status */
#define AMHW_HC32F460_SPI_STATUS_UNDERLOAD_ERROR (1 << 4)  /**< \brief underload error status */
#define AMHW_HC32F460_SPI_STATUS_PARITY_ERROR    (1 << 3)  /**< \brief parity error status */
#define AMHW_HC32F460_SPI_STATUS_MODEFAULT_ERROR (1 << 2)  /**< \brief modefault error status */
#define AMHW_HC32F460_SPI_STATUS_STATUS_IDLE     (1 << 1)  /**< \brief spi idle status */
#define AMHW_HC32F460_SPI_STATUS_OVERLOAD_ERROR  (1 << 0)  /**< \brief overload error status */

/**
 * \brief 标志有效性判断
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : 标志宏定义
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f460_spi_flag_check (amhw_hc32f460_spi_t *p_hw_spi,
                                        uint32_t             flag)
{
    return ((p_hw_spi->SR & flag)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除中断标志
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : 标志宏定义(SPI空闲标志除外)
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_spi_flag_clr (amhw_hc32f460_spi_t *p_hw_spi,
                                 uint32_t             flag)
{

    p_hw_spi->SR &= (~flag);
}


/**
 * \brief  SCK延迟允许
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE  : SCK延迟为SCKDL的值
 *                       AM_FALSE ：SCK延迟为1个SCK
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_sck_delay_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                        am_bool_t            flag)
{
    p_hw_spi->CFG2_f.SCKDLE = flag;
}

/**
 * \brief  SS无效延迟允许
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_ss_invalid_delay_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                               am_bool_t            flag)
{
    p_hw_spi->CFG2_f.SSDLE = flag;
}

/**
 * \brief  SPI下次存取延迟允许
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE/AM_FALSE
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_next_interval_delay_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                                  am_bool_t            flag)
{
    p_hw_spi->CFG2_f.NXTDLE = flag;
}


/**
 * \brief SPI first bit position enumeration
 */
typedef enum amhw_hc32f460_spi_first_bit_position
{
    AMHW_HC32F460_SPI_MSB_FIRST  = 0u,  /**< \brief msb_first */
    AMHW_HC32F460_SPI_LSB_FIRST  = 1u,  /**< \brief lsb_first */
} amhw_hc32f460_spi_first_bit_position_t;


/**
 * \brief  SPI数据传输顺序
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : MSB first/ LSB first(amhw_hc32f460_spi_first_bit_position_t)
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_first_bit_position_set(amhw_hc32f460_spi_t *p_hw_spi,
                                              am_bool_t            flag)
{
    p_hw_spi->CFG2_f.LSBF = flag;
}



/**
 * \brief SPI data length
 */
typedef enum amhw_hc32f460_spi_data_length
{
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT4   = 0u,  /**< \brief 4 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT5   = 1u,  /**< \brief 5 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT6   = 2u,  /**< \brief 6 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT7   = 3u,  /**< \brief 7 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT8   = 4u,  /**< \brief 8 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT9   = 5u,  /**< \brief 9 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT10  = 6u,  /**< \brief 10 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT11  = 7u,  /**< \brief 11 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT12  = 8u,  /**< \brief 12 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT13  = 9u,  /**< \brief 13 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT14  = 10u, /**< \brief 14 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT15  = 11u, /**< \brief 15 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT16  = 12u, /**< \brief 16 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT20  = 13u, /**< \brief 20 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT24  = 14u, /**< \brief 24 bits */
    AMHW_HC32F460_SPI_DATA_LENGTH_BIT32  = 15u, /**< \brief 32 bits */
} amhw_hc32f460_spi_data_length_t;

/**
 * \brief  SPI数据长度设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] length   : 数据长度（amhw_hc32f460_spi_data_length_t）
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_data_length_set(amhw_hc32f460_spi_t *p_hw_spi,
                                       am_bool_t            length)
{
    p_hw_spi->CFG2_f.DSIZE = length;
}



/**
 * \brief SPI first bit position enumeration
 */
typedef enum amhw_hc32f460_spi_ss_enable
{
    AMHW_HC32F460_SPI_SS0_VALID  = 0u,  /**< \brief msb_first */
    AMHW_HC32F460_SPI_SS1_VALID  = 1u,  /**< \brief msb_first */
    AMHW_HC32F460_SPI_SS2_VALID  = 2u,  /**< \brief msb_first */
    AMHW_HC32F460_SPI_SS3_VALID  = 3u,  /**< \brief msb_first */
} amhw_hc32f460_spi_ss_enable_t;


/**
 * \brief  SS信号有效设定位
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : SS信号标志（amhw_hc32f460_spi_ss_enable_t）
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_ss_enable(amhw_hc32f460_spi_t *p_hw_spi,
                                 uint32_t             flag)
{
    p_hw_spi->CFG2_f.SSA = flag;
}



/**
 * \brief SPI clock division enumeration
 */
typedef enum amhw_hc32f460_spi_clk_div
{
    AMHW_HC32F460_SPI_CLK_DIV_2    = 0u,  /**< \brief spi pckl1 division 2 */
    AMHW_HC32F460_SPI_CLK_DIV_4    = 1u,  /**< \brief spi pckl1 division 4 */
    AMHW_HC32F460_SPI_CLK_DIV_8    = 2u,  /**< \brief spi pckl1 division 8 */
    AMHW_HC32F460_SPI_CLK_DIV_16   = 3u,  /**< \brief spi pckl1 division 16 */
    AMHW_HC32F460_SPI_CLK_DIV_32   = 4u,  /**< \brief spi pckl1 division 32 */
    AMHW_HC32F460_SPI_CLK_DIV_64   = 5u,  /**< \brief spi pckl1 division 64 */
    AMHW_HC32F460_SPI_CLK_DIV_128  = 6u,  /**< \brief spi pckl1 division 128 */
    AMHW_HC32F460_SPI_CLK_DIV_256  = 7u,  /**< \brief spi pckl1 division 256 */
} amhw_hc32f460_spi_clk_div_t;

/**
 * \brief  位速率分频设定
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : clk分频（amhw_hc32f460_spi_clk_div_t）
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_clk_div_set(amhw_hc32f460_spi_t *p_hw_spi,
                                   uint32_t             flag)
{
    p_hw_spi->CFG2_f.MBR = flag;
}



/**
 * \brief SPI SCK polarity enumeration
 */
typedef enum amhw_hc32f460_spi_sck_polarity
{
    AMHW_HC32F460_SPI_SCK_IDLE_LEVEL_LOW   = 0u,  /**< \brief SCK is low level when SCK idle */
    AMHW_HC32F460_SPI_SCK_IDLE_LEVEL_HIGH  = 1u,  /**< \brief SCK is high level when SCK idle */
} amhw_hc32f460_spi_sck_polarity_t;

/**
 * \brief  设置SCK空闲时的电平极性
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : SCK空闲电平（amhw_hc32f460_spi_sck_polarity_t）
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_sck_polarity_set(amhw_hc32f460_spi_t *p_hw_spi,
                                        uint32_t             flag)
{
    p_hw_spi->CFG2_f.CPOL = flag;
}

/**
 * \brief SPI SCK phase enumeration
 */
typedef enum amhw_hc32f460_spi_sck_phase
{
    AMHW_HC32F460_SPI_SCK_ODDSAMPLE_EVENCHANGE   = 0u,  /**< \brief SCK Odd edge data sample,even edge data change */
	AMHW_HC32F460_SPI_SCK_EVENSAMPLE_ODDCHANGE   = 1u,  /**< \brief SCK Odd edge data change,even edge data sample */
} amhw_hc32f460_spi_sck_phase_t;

/**
 * \brief  设置SCK空闲时的电平极性
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : SCK相位（amhw_hc32f460_spi_sck_phase_t）
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_sck_phase_set(amhw_hc32f460_spi_t *p_hw_spi,
                                     uint32_t             flag)
{
    p_hw_spi->CFG2_f.CPHA = flag;
}

/**
 * \brief  设置SCK模式（极性和相位）
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AMHW_ZLG_SPI_CLK_MODE_0(空闲时低电平，奇数边沿采样，偶数边沿数据变化)
 *                       AMHW_ZLG_SPI_CLK_MODE_1(空闲时低电平，偶数边沿采样，奇数边沿数据变化)
 *                       AMHW_ZLG_SPI_CLK_MODE_2(空闲时高电平，奇数边沿采样，偶数边沿数据变化)
 *                       AMHW_ZLG_SPI_CLK_MODE_3(空闲时高电平，偶数边沿采样，奇数边沿数据变化)
 *
 * \return status
 */
am_static_inline
void amhw_hc32f460_spi_clk_mode_set(amhw_hc32f460_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->CFG2 = (p_hw_spi->CFG2 & (~(3u << 0))) | (flag << 0);
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
 * @} amhw_hc32f460_if_spi
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_SPI_H */

/* end of file */
