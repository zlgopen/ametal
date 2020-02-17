
#include "ametal.h"
#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/**
 ******************************************************************************
 ** \brief USART mode
 **
 ******************************************************************************/
typedef enum en_usart_mode
{
    UsartUartMode      = 0u,                ///< UART mode
    UsartClkSyncMode   = 1u,                ///< Clock sync mode
    UsartSmartCardMode = 2u,                ///< Smart card mode
} en_usart_mode_t;

/**
 *******************************************************************************
 ** \brief USART clock prescale enumeration
 **
 ******************************************************************************/
typedef enum en_usart_clk_div
{
    UsartClkDiv_1  = 0u,                    ///< PCLK/1
    UsartClkDiv_4  = 1u,                    ///< PCLK/4
    UsartClkDiv_16 = 2u,                    ///< PCLK/16
    UsartClkDiv_64 = 3u,                    ///< PCLK/64
} en_usart_clk_div_t;

/**
 ******************************************************************************
 ** \brief USART data direction
 **
 ******************************************************************************/
typedef enum en_usart_data_dir
{
    UsartDataLsbFirst = 0u,                 ///< LSB first
    UsartDataMsbFirst = 1u,                 ///< MSB first
} en_usart_data_dir_t;

/**
 ******************************************************************************
 ** \brief USART sample mode enumeration
 **
 ******************************************************************************/
typedef enum en_usart_sample_mode
{
    UsartSamleBit16 = 0u,                   ///< 16 Bit
    UsartSamleBit8  = 1u,                   ///< 8 Bit
} en_usart_sample_mode_t;

/**
 ******************************************************************************
 ** \brief USART data length enumeration
 **
 ******************************************************************************/
typedef enum en_usart_data_len
{
    UsartDataBits8 = 0u,                    ///< 8 Bit
    UsartDataBits9 = 1u,                    ///< 9 Bit
} en_usart_data_len_t;

/**
 ******************************************************************************
 ** \brief USART parity format enumeration
 **
 ******************************************************************************/
typedef enum en_usart_parity
{
    UsartParityNone = 0u,                   ///< No parity bit is used.
    UsartParityEven = 1u,                   ///< Even parity bit is used.
    UsartParityOdd  = 2u,                   ///< Odd parity bit is used.
} en_usart_parity_t;

/**
 ******************************************************************************
 ** \brief USART functions enumeration
 **
 ******************************************************************************/
typedef enum en_usart_func
{
    UsartRx              = 0u,              ///< UART RX function
    UsartRxInt           = 1u,              ///< USART RX interrupt function
    UsartTx              = 2u,              ///< UART TX function
    UsartTxEmptyInt      = 3u,              ///< USART TX empty interrupt function
    UsartTimeOut         = 4u,              ///< UART RX timeout function
    UsartTimeOutInt      = 5u,              ///< UART RX timeout interrupt function
    UsartSilentMode      = 6u,              ///< USART silent function
    UsartTxCmpltInt      = 7u,              ///< USART TX complete interrupt function
    UsartTxAndTxEmptyInt = 8u,              ///< USART TX function and USART TX empty interrupt function
    UsartParityCheck     = 9u,              ///< USART Parity check function
    UsartNoiseFilter     = 10u,             ///< USART noise filter function
    UsartFracBaudrate    = 11u,             ///< USART fractional baudrate function
    UsartMulProcessor    = 12u,             ///< USART multiple processor function
    UsartSmartCard       = 13u,             ///< USART smart card mode function
    UsartCts             = 14u,             ///< USART CTS function
} en_usart_func_t;

/**
 *******************************************************************************
 ** \brief USART status type enumeration
 **
 ******************************************************************************/
typedef enum en_usart_status
{
    UsartParityErr  = (1u << 0),            ///< USART parity error
    UsartFrameErr   = (1u << 1),            ///< USART receive frame error
    UsartOverrunErr = (1u << 3),            ///< USART receive over-run error
    UsartRxNoEmpty  = (1u << 5),            ///< USART data receive register is not empty
    UsartTxComplete = (1u << 6),            ///< USART transfer completely
    UsartTxEmpty    = (1u << 7),            ///< USART data transfer register is empty
    UsartRxTimeOut  = (1u << 8),            ///< USART data receive timeout
    UsartRxMpb      = (1u << 16),           ///< USART multiple processor id or normal data, 0: receive date; 1: received ID
} en_usart_status_t;

/**
 *******************************************************************************
 ** \brief USART Stop bit length select enumeration
 **
 ******************************************************************************/
typedef enum en_usart_stop_bit
{
    UsartOneStopBit = 0u,                   ///< 1 Stop Bit
    UsartTwoStopBit = 1u,                   ///< 2 Stop Bit
} en_usart_stop_bit_t;

/**
 *******************************************************************************
 ** \brief USART start bit detect mode enumeration
 **
 ******************************************************************************/
typedef enum en_usart_sb_detect_mode
{
    UsartStartBitLowLvl   = 0u,             ///< Start bit: RD pin low level
    UsartStartBitFallEdge = 1u,             ///< Start bit: RD pin falling edge
} en_usart_sb_detect_mode_t;

/**
 *******************************************************************************
 ** \brief USART clock mode selection enumeration
 **
 ******************************************************************************/
typedef enum en_usart_clk_mode
{
    UsartIntClkCkNoOutput = 0u,             ///< Select internal clock source and don't output clock.
    UsartIntClkCkOutput   = 1u,             ///< Select internal clock source and output clock.
    UsartExtClk           = 2u,             ///< Select external clock source.
} en_usart_clk_mode_t;

/**
 *******************************************************************************
 ** \brief USART smart-card mode selection enumeration
 **
 ******************************************************************************/
typedef enum en_usart_hw_flow_ctrl
{
    UsartRtsEnable = 0u,                    ///< Enable RTS function.
    UsartCtsEnable = 1u,                    ///< Enable CTS function.
} en_usart_hw_flow_ctrl_t;

/**
 ******************************************************************************
 ** \brief USART etu clocks of smart card enumeration
 **
 ******************************************************************************/
typedef enum en_usart_sc_etu_clk
{
    UsartScEtuClk32  = 0u,                  ///< 1 etu = 32/f
    UsartScEtuClk64  = 1u,                  ///< 1 etu = 64/f
    UsartScEtuClk128 = 3u,                  ///< 1 etu = 128/f
    UsartScEtuClk256 = 5u,                  ///< 1 etu = 256/f
    UsartScEtuClk372 = 6u,                  ///< 1 etu = 372/f
} en_usart_sc_etu_clk_t;

/**
 *******************************************************************************
 ** \brief Uart mode initialization configuration
 **
 ******************************************************************************/
typedef struct stc_usart_uart_init
{
    en_usart_clk_mode_t       enClkMode;        ///< Clock mode and this parameter can be a value of @ref en_usart_clk_mode_t

    en_usart_clk_div_t        enClkDiv;         ///< USART divide PCLK1, and this parameter can be a value of @ref en_usart_clk_div_t

    en_usart_data_len_t       enDataLength;     ///< 8/9 Bit character length and this parameter can be a value of @ref en_usart_data_len_t

    en_usart_data_dir_t       enDirection;      ///< UART data direction and this parameter can be a value of @ref en_usart_data_dir_t

    en_usart_stop_bit_t       enStopBit;        ///< Stop bit and this parameter can be a value of @ref en_usart_stop_bit_t

    en_usart_parity_t         enParity;         ///< Parity format and this parameter can be a value of @ref en_usart_parity_t

    en_usart_sample_mode_t    enSampleMode;     ///< USART sample mode, and this parameter can be a value of @ref en_usart_sample_mode_t

    en_usart_sb_detect_mode_t enDetectMode;     ///< USART start bit detect mode and this parameter can be a value of @ref en_usart_sb_detect_mode_t

    en_usart_hw_flow_ctrl_t   enHwFlow;         ///< Hardware flow control and this parameter can be a value of @ref en_usart_hw_flow_ctrl_t
} stc_usart_uart_init_t;

/**
 *******************************************************************************
 ** \brief generic error codes
 ******************************************************************************/
typedef enum en_result
{
    Ok                       = 0u,   ///< No error
    Error                    = 1u,   ///< Non-specific error code
    ErrorAddressAlignment    = 2u,   ///< Address alignment does not match
    ErrorAccessRights        = 3u,   ///< Wrong mode (e.g. user/system) mode is set
    ErrorInvalidParameter    = 4u,   ///< Provided parameter is not valid
    ErrorOperationInProgress = 5u,   ///< A conflicting or requested operation is still in progress
    ErrorInvalidMode         = 6u,   ///< Operation not allowed in current mode
    ErrorUninitialized       = 7u,   ///< Module (or part of it) was not initialized properly
    ErrorBufferFull          = 8u,   ///< Circular buffer can not be written because the buffer is full
    ErrorTimeout             = 9u,   ///< Time Out error occurred (e.g. I2C arbitration lost, Flash time-out, etc.)
    ErrorNotReady            = 10u,  ///< A requested final state is not reached
    OperationInProgress      = 11u,  ///< Indicator for operation in progress (e.g. ADC conversion not finished, DMA channel used, etc.)
} en_result_t;

typedef struct
{
    __IO uint32_t PE                        : 1;
    __IO uint32_t FE                        : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t ORE                       : 1;
    uint32_t RESERVED4                      : 1;
    __IO uint32_t RXNE                      : 1;
    __IO uint32_t TC                        : 1;
    __IO uint32_t TXE                       : 1;
    __IO uint32_t RTOF                      : 1;
    uint32_t RESERVED9                      : 7;
    __IO uint32_t MPB                       : 1;
    uint32_t RESERVED17                     :15;
} stc_usart_sr_field_t;

typedef struct
{
    __IO uint32_t TDR                       : 9;
    __IO uint32_t MPID                      : 1;
    uint32_t RESERVED10                     : 6;
    __IO uint32_t RDR                       : 9;
    uint32_t RESERVED25                     : 7;
} stc_usart_dr_field_t;

typedef struct
{
    __IO uint32_t DIV_FRACTION              : 7;
    uint32_t RESERVED7                      : 1;
    __IO uint32_t DIV_INTEGER               : 8;
    uint32_t RESERVED16                     :16;
} stc_usart_brr_field_t;

typedef struct
{
    __IO uint32_t RTOE                      : 1;
    __IO uint32_t RTOIE                     : 1;
    __IO uint32_t RE                        : 1;
    __IO uint32_t TE                        : 1;
    __IO uint32_t SLME                      : 1;
    __IO uint32_t RIE                       : 1;
    __IO uint32_t TCIE                      : 1;
    __IO uint32_t TXEIE                     : 1;
    uint32_t RESERVED8                      : 1;
    __IO uint32_t PS                        : 1;
    __IO uint32_t PCE                       : 1;
    uint32_t RESERVED11                     : 1;
    __IO uint32_t M                         : 1;
    uint32_t RESERVED13                     : 2;
    __IO uint32_t OVER8                     : 1;
    __IO uint32_t CPE                       : 1;
    __IO uint32_t CFE                       : 1;
    uint32_t RESERVED18                     : 1;
    __IO uint32_t CORE                      : 1;
    __IO uint32_t CRTOF                     : 1;
    uint32_t RESERVED21                     : 3;
    __IO uint32_t MS                        : 1;
    uint32_t RESERVED25                     : 3;
    __IO uint32_t ML                        : 1;
    __IO uint32_t FBME                      : 1;
    __IO uint32_t NFE                       : 1;
    __IO uint32_t SBS                       : 1;
} stc_usart_cr1_field_t;

typedef struct
{
    __IO uint32_t MPE                       : 1;
    uint32_t RESERVED1                      :10;
    __IO uint32_t CLKC                      : 2;
    __IO uint32_t STOP                      : 1;
    uint32_t RESERVED14                     :18;
} stc_usart_cr2_field_t;

typedef struct
{
    uint32_t RESERVED0                      : 5;
    __IO uint32_t SCEN                      : 1;
    uint32_t RESERVED6                      : 3;
    __IO uint32_t CTSE                      : 1;
    uint32_t RESERVED10                     :11;
    __IO uint32_t BCN                       : 3;
    uint32_t RESERVED24                     : 8;
} stc_usart_cr3_field_t;

typedef struct
{
    __IO uint32_t PSC                       : 2;
    uint32_t RESERVED2                      :30;
} stc_usart_pr_field_t;

typedef struct
{
    union
    {
        __IO uint32_t SR;
        stc_usart_sr_field_t SR_f;
    };
    union
    {
        __IO uint32_t DR;
        stc_usart_dr_field_t DR_f;
    };
    union
    {
        __IO uint32_t BRR;
        stc_usart_brr_field_t BRR_f;
    };
    union
    {
        __IO uint32_t CR1;
        stc_usart_cr1_field_t CR1_f;
    };
    union
    {
        __IO uint32_t CR2;
        stc_usart_cr2_field_t CR2_f;
    };
    union
    {
        __IO uint32_t CR3;
        stc_usart_cr3_field_t CR3_f;
    };
    union
    {
        __IO uint32_t PR;
        stc_usart_pr_field_t PR_f;
    };
}M4_USART_TypeDef;

/*!< Parameter valid check for USART Instances. */
#define IS_VALID_USART(__USARTx__)                                             \
(   (M4_USART1 == (__USARTx__))             ||                                 \
    (M4_USART2 == (__USARTx__))             ||                                 \
    (M4_USART3 == (__USARTx__))             ||                                 \
    (M4_USART4 == (__USARTx__)))

/*!< Parameter valid check for USART clock prescale. */
#define IS_VALID_USART_CLK_DIV(x)                                              \
(   (UsartClkDiv_1  == (x))                 ||                                 \
    (UsartClkDiv_4  == (x))                 ||                                 \
    (UsartClkDiv_16 == (x))                 ||                                 \
    (UsartClkDiv_64 == (x)))

/*!< Parameter valid check for USART function type. */
#define IS_VALID_USART_FUNC(x)                                                 \
(   (UsartRx              == (x))           ||                                 \
    (UsartTx              == (x))           ||                                 \
    (UsartCts             == (x))           ||                                 \
    (UsartRxInt           == (x))           ||                                 \
    (UsartTimeOut         == (x))           ||                                 \
    (UsartSmartCard       == (x))           ||                                 \
    (UsartSilentMode      == (x))           ||                                 \
    (UsartTxEmptyInt      == (x))           ||                                 \
    (UsartTimeOutInt      == (x))           ||                                 \
    (UsartTxCmpltInt      == (x))           ||                                 \
    (UsartParityCheck     == (x))           ||                                 \
    (UsartNoiseFilter     == (x))           ||                                 \
    (UsartFracBaudrate    == (x))           ||                                 \
    (UsartMulProcessor    == (x))           ||                                 \
    (UsartTxAndTxEmptyInt == (x)))

/*!< Parameter valid check for USART function type. */
#define IS_VALID_USART_STATUS(x)                                               \
(   (UsartRxMpb      == (x))                ||                                 \
    (UsartTxEmpty    == (x))                ||                                 \
    (UsartFrameErr   == (x))                ||                                 \
    (UsartRxNoEmpty  == (x))                ||                                 \
    (UsartRxTimeOut  == (x))                ||                                 \
    (UsartParityErr  == (x))                ||                                 \
    (UsartOverrunErr == (x))                ||                                 \
    (UsartTxComplete == (x)))

/*!< Parameter valid check for USART clock mode. */
#define IS_VALID_USART_CLK_MODE(x)                                             \
(   (UsartExtClk           == (x))          ||                                 \
    (UsartIntClkCkOutput   == (x))          ||                                 \
    (UsartIntClkCkNoOutput == (x)))

/*!< Parameter valid check for USART stop bit. */
#define IS_VALID_USART_STOP_BIT(x)                                             \
(   (UsartOneStopBit == (x))                ||                                 \
    (UsartTwoStopBit == (x)))

/*!< Parameter valid check for USART parity bit. */
#define IS_VALID_USART_PARITY_BIT(x)                                           \
(   (UsartParityOdd  == (x))                ||                                 \
    (UsartParityEven == (x))                ||                                 \
    (UsartParityNone == (x)))

/*!< Parameter valid check for USART data length. */
#define IS_VALID_USART_DATA_LEN(x)                                             \
(   (UsartDataBits8 == (x))                 ||                                 \
    (UsartDataBits9 == (x)))

/*!< Parameter valid check for USART data direction. */
#define IS_VALID_USART_DATA_DIR(x)                                             \
(   (UsartDataLsbFirst == (x))              ||                                 \
    (UsartDataMsbFirst == (x)))

/*!< Parameter valid check for USART sample mode. */
#define IS_VALID_USART_SAMPLE_MODE(x)                                          \
(   (UsartSamleBit8  == (x))                ||                                 \
    (UsartSamleBit16 == (x)))

/*!< Parameter valid check for USART sample mode. */
#define IS_VALID_USART_HW_FLOW_MODE(x)                                         \
(   (UsartRtsEnable == (x))                 ||                                 \
    (UsartCtsEnable == (x)))

/*!< Parameter valid check for USART detect mode. */
#define IS_VALID_USART_SB_DETECT_MODE(x)                                       \
(   (UsartStartBitLowLvl   == (x))          ||                                 \
    (UsartStartBitFallEdge == (x)))

/*!< Parameter valid check for USART mode. */
#define IS_VALID_USART_MODE(x)                                                 \
(   (UsartUartMode     == (x))              ||                                 \
    (UsartClkSyncMode  == (x))              ||                                 \
    (UsartSmartCardMode == (x)))

/*!< Parameter valid check for USART ETU clocks number. */
#define IS_VALID_USART_ETU_CLK(x)                                              \
(   (UsartScEtuClk32  == (x))               ||                                 \
    (UsartScEtuClk64  == (x))               ||                                 \
    (UsartScEtuClk128 == (x))               ||                                 \
    (UsartScEtuClk256 == (x))               ||                                 \
    (UsartScEtuClk372 == (x)))


#define M4_USART1                               ((M4_USART_TypeDef *)0x4001D000UL)
#define M4_USART2                               ((M4_USART_TypeDef *)0x4001D400UL)
#define M4_USART3                               ((M4_USART_TypeDef *)0x40021000UL)
#define M4_USART4                               ((M4_USART_TypeDef *)0x40021400UL)

#define PWC_FCG1_PERIPH_USART1      ((uint32_t)0x01000000)
#define PWC_FCG1_PERIPH_USART2      ((uint32_t)0x02000000)
#define PWC_FCG1_PERIPH_USART3      ((uint32_t)0x04000000)
#define PWC_FCG1_PERIPH_USART4      ((uint32_t)0x08000000)

/**
 *******************************************************************************
 ** \brief GPIO Port Index enumeration
 ******************************************************************************/
typedef enum en_port
{
    PortA = 0,                          ///< port group A
    PortB = 1,                          ///< port group B
    PortC = 2,                          ///< port group C
    PortD = 3,                          ///< port group D
    PortE = 4,                          ///< port group E
    PortH = 5,                          ///< port group H
}en_port_t;
/**
 *******************************************************************************
 ** \brief GPIO Pin Index enumeration
 ******************************************************************************/
typedef enum en_pin
{
    Pin00 = (1 <<  0),                  ///< Pin index 00 of each port group
    Pin01 = (1 <<  1),                  ///< Pin index 01 of each port group
    Pin02 = (1 <<  2),                  ///< Pin index 02 of each port group
    Pin03 = (1 <<  3),                  ///< Pin index 03 of each port group
    Pin04 = (1 <<  4),                  ///< Pin index 04 of each port group
    Pin05 = (1 <<  5),                  ///< Pin index 05 of each port group
    Pin06 = (1 <<  6),                  ///< Pin index 06 of each port group
    Pin07 = (1 <<  7),                  ///< Pin index 07 of each port group
    Pin08 = (1 <<  8),                  ///< Pin index 08 of each port group
    Pin09 = (1 <<  9),                  ///< Pin index 09 of each port group
    Pin10 = (1 << 10),                  ///< Pin index 10 of each port group
    Pin11 = (1 << 11),                  ///< Pin index 11 of each port group
    Pin12 = (1 << 12),                  ///< Pin index 12 of each port group
    Pin13 = (1 << 13),                  ///< Pin index 13 of each port group
    Pin14 = (1 << 14),                  ///< Pin index 14 of each port group
    Pin15 = (1 << 15),                  ///< Pin index 15 of each port group
    PinAll= 0xFFFF,                     ///< All pins selected
}en_pin_t;
/**
 *******************************************************************************
 ** \brief GPIO Function enumeration
 ******************************************************************************/
typedef enum en_port_func
{
    Func_Gpio       = 0u,               ///< function set to gpio
    Func_Fcmref     = 1u,               ///< function set to fcm reference
    Func_Rtcout     = 1u,               ///< function set to rtc output
    Func_Vcout      = 1u,               ///< function set to vc output
    Func_Adtrg      = 1u,               ///< function set to adc trigger
    Func_Mclkout    = 1u,               ///< function set to mclk output
    Func_Tim4       = 2u,               ///< function set to timer4
    Func_Tim6       = 3u,               ///< function set to timer6
    Func_Tima0      = 4u,               ///< function set to timerA
    Func_Tima1      = 5u,               ///< function set to timerA
    Func_Tima2      = 6u,               ///< function set to timerA
    Func_Emb        = 6u,               ///< function set to emb
    Func_Usart_Ck   = 7u,               ///< function set to usart clk
    Func_Spi_Nss    = 7u,               ///< function set to spi nss
    Func_Qspi       = 7u,               ///< function set to qspi
    Func_Key        = 8u,               ///< function set to key
    Func_Sdio       = 9u,               ///< function set to sdio
    Func_I2s        = 10u,              ///< function set to i2s
    Func_UsbF       = 10u,              ///< function set to usb full speed
    Func_Evnpt      = 14u,              ///< function set to event port
    Func_Eventout   = 15u,              ///< function set to event out
    Func_Usart1_Tx  = 32u,              ///< function set to usart tx of ch.1
    Func_Usart3_Tx  = 32u,              ///< function set to usart tx of ch.3
    Func_Usart1_Rx  = 33u,              ///< function set to usart rx of ch.1
    Func_Usart3_Rx  = 33u,              ///< function set to usart rx of ch.3
    Func_Usart1_Rts = 34u,              ///< function set to usart rts of ch.1
    Func_Usart3_Rts = 34u,              ///< function set to usart rts of ch.3
    Func_Usart1_Cts = 35u,              ///< function set to usart cts of ch.1
    Func_Usart3_Cts = 35u,              ///< function set to usart cts of ch.3
    Func_Usart2_Tx  = 36u,              ///< function set to usart tx of ch.2
    Func_Usart4_Tx  = 36u,              ///< function set to usart tx of ch.4
    Func_Usart2_Rx  = 37u,              ///< function set to usart rx of ch.2
    Func_Usart4_Rx  = 37u,              ///< function set to usart rx of ch.4
    Func_Usart2_Rts = 38u,              ///< function set to usart rts of ch.2
    Func_Usart4_Rts = 38u,              ///< function set to usart rts of ch.4
    Func_Usart2_Cts = 39u,              ///< function set to usart cts of ch.2
    Func_Usart4_Cts = 39u,              ///< function set to usart cts of ch.4
    Func_Spi1_Mosi  = 40u,              ///< function set to spi mosi of ch.1
    Func_Spi3_Mosi  = 40u,              ///< function set to spi mosi of ch.3
    Func_Spi1_Miso  = 41u,              ///< function set to spi miso of ch.1
    Func_Spi3_Miso  = 41u,              ///< function set to spi miso of ch.3
    Func_Spi1_Nss0  = 42u,              ///< function set to spi nss0 of ch.1
    Func_Spi3_Nss0  = 42u,              ///< function set to spi nss0 of ch.3
    Func_Spi1_Sck   = 43u,              ///< function set to spi sck of ch.1
    Func_Spi3_Sck   = 43u,              ///< function set to spi sck of ch.3
    Func_Spi2_Mosi  = 44u,              ///< function set to spi mosi of ch.2
    Func_Spi4_Mosi  = 44u,              ///< function set to spi mosi of ch.2
    Func_Spi2_Miso  = 45u,              ///< function set to spi miso of ch.4
    Func_Spi4_Miso  = 45u,              ///< function set to spi miso of ch.4
    Func_Spi2_Nss0  = 46u,              ///< function set to spi nss0 of ch.2
    Func_Spi4_Nss0  = 46u,              ///< function set to spi nss0 of ch.4
    Func_Spi2_Sck   = 47u,              ///< function set to spi sck of ch.2
    Func_Spi4_Sck   = 47u,              ///< function set to spi sck of ch.4
    Func_I2c1_Sda   = 48u,              ///< function set to i2c sda of ch.1
    Func_I2c3_Sda   = 48u,              ///< function set to i2c sda of ch.3
    Func_I2c1_Scl   = 49u,              ///< function set to i2c scl of ch.1
    Func_I2c3_Scl   = 49u,              ///< function set to i2c scl of ch.3
    Func_I2c2_Sda   = 50u,              ///< function set to i2c sda of ch.2
    Func_Can1_Tx    = 50u,              ///< function set to can tx of ch.1
    Func_I2c2_Scl   = 51u,              ///< function set to i2c scl of ch.2
    Func_Can1_Rx    = 51u,              ///< function set to can rx of ch.1
    Func_I2s1_Sd    = 52u,              ///< function set to i2s sd of ch.1
    Func_I2s3_Sd    = 52u,              ///< function set to i2s sd of ch.3
    Func_I2s1_Sdin  = 53u,              ///< function set to i2s sdin of ch.1
    Func_I2s3_Sdin  = 53u,              ///< function set to i2s sdin of ch.3
    Func_I2s1_Ws    = 54u,              ///< function set to i2s ws of ch.1
    Func_I2s3_Ws    = 54u,              ///< function set to i2s ws of ch.3
    Func_I2s1_Ck    = 55u,              ///< function set to i2s ck of ch.1
    Func_I2s3_Ck    = 55u,              ///< function set to i2s ck of ch.3
    Func_I2s2_Sd    = 56u,              ///< function set to i2s sd of ch.2
    Func_I2s4_Sd    = 56u,              ///< function set to i2s sd of ch.4
    Func_I2s2_Sdin  = 57u,              ///< function set to i2s sdin of ch.2
    Func_I2s4_Sdin  = 57u,              ///< function set to i2s sdin of ch.4
    Func_I2s2_Ws    = 58u,              ///< function set to i2s ws of ch.2
    Func_I2s4_Ws    = 58u,              ///< function set to i2s ws of ch.4
    Func_I2s2_Ck    = 59u,              ///< function set to i2s ck of ch.2
    Func_I2s4_Ck    = 59u,              ///< function set to i2s ck of ch.4
}en_port_func_t;

/**
 *******************************************************************************
 ** \brief functional state
 ******************************************************************************/
typedef enum en_functional_state
{
    Disable = 0u,
    Enable  = 1u,
} en_functional_state_t;
/**
 *******************************************************************************
 ** \brief flag status
 ******************************************************************************/
typedef enum en_flag_status
{
    Reset = 0u,
    Set   = 1u,
} en_flag_status_t, en_int_status_t;

en_result_t USART_UART_Init(M4_USART_TypeDef *USARTx,
                                const stc_usart_uart_init_t *pstcInitCfg);

en_result_t USART_SendData(M4_USART_TypeDef *USARTx, uint16_t u16Data);
