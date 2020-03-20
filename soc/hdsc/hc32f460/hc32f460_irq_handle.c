//#include "hc32f460_intctrl.h"
#include "am_hc32f460.h"
#include "am_hc32f460_gpio.h"
#include "am_hc32f460_uart.h"
#include "am_hc32f460_tim_timing.h"
#include "am_hc32f460_timea_timing.h"
#include "am_hc32f460_tim4_timing.h"
#include "am_hc32f460_adtim_timing.h"
#include "am_hc32f460_timea_cap.h"
#include "am_hc32f460_adtim_cap.h"

#define AM_HC32F460_INT_EXTI0_MASK    (1 << 0)
#define AM_HC32F460_INT_EXTI1_MASK    (1 << 1)
#define AM_HC32F460_INT_EXTI2_MASK    (1 << 2)
#define AM_HC32F460_INT_EXTI3_MASK    (1 << 3)
#define AM_HC32F460_INT_EXTI4_MASK    (1 << 4)
#define AM_HC32F460_INT_EXTI5_MASK    (1 << 5)
#define AM_HC32F460_INT_EXTI6_MASK    (1 << 6)
#define AM_HC32F460_INT_EXTI7_MASK    (1 << 7)
#define AM_HC32F460_INT_EXTI8_MASK    (1 << 8)
#define AM_HC32F460_INT_EXTI9_MASK    (1 << 9)
#define AM_HC32F460_INT_EXTI10_MASK   (1 << 10)
#define AM_HC32F460_INT_EXTI11_MASK   (1 << 11)
#define AM_HC32F460_INT_EXTI12_MASK   (1 << 12)
#define AM_HC32F460_INT_EXTI13_MASK   (1 << 13)
#define AM_HC32F460_INT_EXTI14_MASK   (1 << 14)
#define AM_HC32F460_INT_EXTI15_MASK   (1 << 15)

#define AM_HC32F460_INT_EXTI0_ENABLE_MASK    (1 << 0)
#define AM_HC32F460_INT_EXTI1_ENABLE_MASK    (1 << 1)
#define AM_HC32F460_INT_EXTI2_ENABLE_MASK    (1 << 2)
#define AM_HC32F460_INT_EXTI3_ENABLE_MASK    (1 << 3)
#define AM_HC32F460_INT_EXTI4_ENABLE_MASK    (1 << 4)
#define AM_HC32F460_INT_EXTI5_ENABLE_MASK    (1 << 5)
#define AM_HC32F460_INT_EXTI6_ENABLE_MASK    (1 << 6)
#define AM_HC32F460_INT_EXTI7_ENABLE_MASK    (1 << 7)
#define AM_HC32F460_INT_EXTI8_ENABLE_MASK    (1 << 8)
#define AM_HC32F460_INT_EXTI9_ENABLE_MASK    (1 << 9)
#define AM_HC32F460_INT_EXTI10_ENABLE_MASK   (1 << 10)
#define AM_HC32F460_INT_EXTI11_ENABLE_MASK   (1 << 11)
#define AM_HC32F460_INT_EXTI12_ENABLE_MASK   (1 << 12)
#define AM_HC32F460_INT_EXTI13_ENABLE_MASK   (1 << 13)
#define AM_HC32F460_INT_EXTI14_ENABLE_MASK   (1 << 14)
#define AM_HC32F460_INT_EXTI15_ENABLE_MASK   (1 << 15)

/* DMA 共享中断使能宏定义 */
#define AM_HC32F460_INT_VSSEL_DMA1_TC0_MASK    (1 << 0)
#define AM_HC32F460_INT_VSSEL_DMA1_TC1_MASK    (1 << 1)
#define AM_HC32F460_INT_VSSEL_DMA1_TC2_MASK    (1 << 2)
#define AM_HC32F460_INT_VSSEL_DMA1_TC3_MASK    (1 << 3)
#define AM_HC32F460_INT_VSSEL_DMA2_TC0_MASK    (1 << 4)
#define AM_HC32F460_INT_VSSEL_DMA2_TC1_MASK    (1 << 5)
#define AM_HC32F460_INT_VSSEL_DMA2_TC2_MASK    (1 << 6)
#define AM_HC32F460_INT_VSSEL_DMA2_TC3_MASK    (1 << 7)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC0_MASK   (1 << 8)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC1_MASK   (1 << 9)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC2_MASK   (1 << 10)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC3_MASK   (1 << 11)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC0_MASK   (1 << 12)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC1_MASK   (1 << 13)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC2_MASK   (1 << 14)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC3_MASK   (1 << 15)
#define AM_HC32F460_INT_VSSEL_DMA1_ERR_MASK    (1 << 16)
#define AM_HC32F460_INT_VSSEL_DMA2_ERR_MASK    (1 << 17)



/**
 *******************************************************************************
 ** \brief Int No.128 share IRQ handler
 **
 ******************************************************************************/
void IRQ128_Handler(void)
{
    uint32_t int_vssel = HC32F460_INTC.VSSEL[128 - 128];
    uint32_t int_flag = HC32F460_INTC.EIFR;

    /* external interrupt 00 */
    if ((int_flag == AM_HC32F460_INT_EXTI0_MASK) && (int_vssel & AM_HC32F460_INT_EXTI0_ENABLE_MASK))
    {
        Extint00_IrqHandler();
    }
    /* external interrupt 01 */
    if ((int_flag == AM_HC32F460_INT_EXTI1_MASK) && (int_vssel & AM_HC32F460_INT_EXTI1_ENABLE_MASK))
    {
        Extint01_IrqHandler();
    }
    /* external interrupt 02 */
    if ((int_flag == AM_HC32F460_INT_EXTI2_MASK) && (int_vssel & AM_HC32F460_INT_EXTI2_ENABLE_MASK))
    {
        Extint02_IrqHandler();
    }
    /* external interrupt 03 */
    if ((int_flag == AM_HC32F460_INT_EXTI3_MASK) && (int_vssel & AM_HC32F460_INT_EXTI3_ENABLE_MASK))
    {
        Extint03_IrqHandler();
    }
    /* external interrupt 04 */
    if ((int_flag == AM_HC32F460_INT_EXTI4_MASK) && (int_vssel & AM_HC32F460_INT_EXTI4_ENABLE_MASK))
    {
        Extint04_IrqHandler();
    }
    /* external interrupt 05 */
    if ((int_flag == AM_HC32F460_INT_EXTI5_MASK) && (int_vssel & AM_HC32F460_INT_EXTI5_ENABLE_MASK))
    {
        Extint05_IrqHandler();
    }
    /* external interrupt 06 */
    if ((int_flag == AM_HC32F460_INT_EXTI6_MASK) && (int_vssel & AM_HC32F460_INT_EXTI6_ENABLE_MASK))
    {
        Extint06_IrqHandler();
    }
    /* external interrupt 07 */
    if ((int_flag == AM_HC32F460_INT_EXTI7_MASK) && (int_vssel & AM_HC32F460_INT_EXTI7_ENABLE_MASK))
    {
        Extint07_IrqHandler();
    }
    /* external interrupt 08 */
    if ((int_flag == AM_HC32F460_INT_EXTI8_MASK) && (int_vssel & AM_HC32F460_INT_EXTI8_ENABLE_MASK))
    {
        Extint08_IrqHandler();
    }
    /* external interrupt 09 */
    if ((int_flag == AM_HC32F460_INT_EXTI9_MASK) && (int_vssel & AM_HC32F460_INT_EXTI9_ENABLE_MASK))
    {
        Extint09_IrqHandler();
    }
    /* external interrupt 10 */
    if ((int_flag == AM_HC32F460_INT_EXTI10_MASK) && (int_vssel & AM_HC32F460_INT_EXTI10_ENABLE_MASK))
    {
        Extint10_IrqHandler();
    }
    /* external interrupt 11 */
    if ((int_flag == AM_HC32F460_INT_EXTI11_MASK) && (int_vssel & AM_HC32F460_INT_EXTI11_ENABLE_MASK))
    {
        Extint11_IrqHandler();
    }
    /* external interrupt 12 */
    if ((int_flag == AM_HC32F460_INT_EXTI12_MASK) && (int_vssel & AM_HC32F460_INT_EXTI12_ENABLE_MASK))
    {
        Extint12_IrqHandler();
    }
    /* external interrupt 13 */
    if ((int_flag == AM_HC32F460_INT_EXTI13_MASK) && (int_vssel & AM_HC32F460_INT_EXTI13_ENABLE_MASK))
    {
        Extint13_IrqHandler();
    }
    /* external interrupt 14 */
    if ((int_flag == AM_HC32F460_INT_EXTI14_MASK) && (int_vssel & AM_HC32F460_INT_EXTI14_ENABLE_MASK))
    {
        Extint14_IrqHandler();
    }
    /* external interrupt 15 */
    if ((int_flag == AM_HC32F460_INT_EXTI15_MASK) && (int_vssel & AM_HC32F460_INT_EXTI15_ENABLE_MASK))
    {
        Extint15_IrqHandler();
    }
}

#define __HC32F460_UART_CR1_RIE_MASK    (1 << 5)    /**< \brief 接收中断使能 */
#define __HC32F460_UART_SR_PE_MASK      (1 << 0)    /**< \brief 接收数据错误标志 */
#define __HC32F460_UART_SR_FE_MASK      (1 << 1)    /**< \brief 接收帧错误标志 */
#define __HC32F460_UART_SR_ORE_MASK     (1 << 2)    /**< \brief 接收上溢错误标志 */
#define __HC32F460_UART_SR_RX_ERROR_MASK (__HC32F460_UART_SR_PE_MASK | \
                                          __HC32F460_UART_SR_FE_MASK | \
                                          __HC32F460_UART_SR_ORE_MASK)
#define __HC32F460_INTC_VSSEL_USART1_EI_MASK  (1 << 22)
#define __HC32F460_INTC_VSSEL_USART1_RI_MASK  (1 << 23)
#define __HC32F460_INTC_VSSEL_USART1_TI_MASK  (1 << 24)
#define __HC32F460_INTC_VSSEL_USART1_TCI_MASK (1 << 25)
#define __HC32F460_INTC_VSSEL_USART1_RTO_MASK (1 << 26)

#define __HC32F460_INTC_VSSEL_USART2_EI_MASK  (1 << 27)
#define __HC32F460_INTC_VSSEL_USART2_RI_MASK  (1 << 28)
#define __HC32F460_INTC_VSSEL_USART2_TI_MASK  (1 << 29)
#define __HC32F460_INTC_VSSEL_USART2_TCI_MASK (1 << 30)
#define __HC32F460_INTC_VSSEL_USART2_RTO_MASK (1 << 31)

#define __HC32F460_INTC_VSSEL_USART3_EI_MASK  (1 << 0)
#define __HC32F460_INTC_VSSEL_USART3_RI_MASK  (1 << 1)
#define __HC32F460_INTC_VSSEL_USART3_TI_MASK  (1 << 2)
#define __HC32F460_INTC_VSSEL_USART3_TCI_MASK (1 << 3)
#define __HC32F460_INTC_VSSEL_USART3_RTO_MASK (1 << 4)

#define __HC32F460_INTC_VSSEL_USART4_EI_MASK  (1 << 5)
#define __HC32F460_INTC_VSSEL_USART4_RI_MASK  (1 << 6)
#define __HC32F460_INTC_VSSEL_USART4_TI_MASK  (1 << 7)
#define __HC32F460_INTC_VSSEL_USART4_TCI_MASK (1 << 8)
#define __HC32F460_INTC_VSSEL_USART4_RTO_MASK (1 << 9)

#define __HC32F460_UART_CR1_TXEIE_MASK    (1 << 7)    /**< \brief 发送中断使能 */
#define __HC32F460_UART_CR1_TCIE_MASK     (1 << 6)    /**< \brief 发送完成中断使能 */
#define __HC32F460_UART_SR_TXE_MASK       (1 << 7)    /**< \brief 发送空中断标志 */
#define __HC32F460_UART_SR_TC_MASK        (1 << 6)    /**< \brief 发送完成中断标志 */

#define __HC32F460_UART_CR1_RTOIE_MASK    (1 << 1)    /**< \brief 串口超时中断使能 */
#define __HC32F460_UART_SR_RTOF_MASK      (1 << 8)    /**< \brief 超时中断标志 */


#include "amhw_arm_systick.h"
/**
 *******************************************************************************
 ** \brief Int No.136 share IRQ handler
 **
 ******************************************************************************/

#if 0

extern am_hc32f460_uart_dev_t __g_uart1_dev;
extern am_hc32f460_uart_dev_t __g_uart2_dev;
extern am_hc32f460_uart_dev_t __g_uart3_dev;
extern am_hc32f460_uart_dev_t __g_uart4_dev;

void IRQ136_Handler(void *parg)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;

    uint32_t int_vssel136 = HC32F460_INTC.VSSEL[136 - 128];

    u32Tmp1 = HC32F460_UART1->SR;
    u32Tmp2 = HC32F460_UART1->CR1;

    /* USART Ch.1 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_EI_MASK))
    {
    	UsartRxErr_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RI_MASK))
    {
    	UsartRxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TI_MASK))
    {
    	UsartTxEmpty_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TCI_MASK))
    {
    	UsartTxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RTO_MASK))
    {
    	UsartRxTO_IrqHandler(&__g_uart1_dev);
    }

    u32Tmp1 = HC32F460_UART2->SR;
    u32Tmp2 = HC32F460_UART2->CR1;

    /* USART Ch.2 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart2_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.137 share IRQ handler
 **
 ******************************************************************************/
void IRQ137_Handler(void *parg)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t int_vssel137 = HC32F460_INTC.VSSEL[137 - 128];


    u32Tmp1 = HC32F460_UART3->SR;
    u32Tmp2 = HC32F460_UART3->CR1;

    /* USART Ch.3 Receive error */
//    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_EI_MASK))
//    {
//        UsartRxErr_IrqHandler(&__g_uart3_dev);
//        return;
//    }
    /* USART Ch.3 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & (1 << 5)) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart3_dev);
    }
    /* USART Ch.3 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart3_dev);
        return;
    }
    /* USART Ch.3 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart3_dev);
        return;
    }
    /* USART Ch.3 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart3_dev);
        return;
    }

    u32Tmp1 = HC32F460_UART4->SR;
    u32Tmp2 = HC32F460_UART4->CR1;

    /* USART Ch.4 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart4_dev);
    }
}

#endif

extern am_hc32f460_tim_timing_dev_t __g_tim01_timing_dev;
extern am_hc32f460_tim_timing_dev_t __g_tim02_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea1_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea2_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea3_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea4_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea5_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea6_timing_dev;
extern am_hc32f460_tim4_timing_dev_t __g_tim41_timing_dev;
extern am_hc32f460_tim4_timing_dev_t __g_tim42_timing_dev;
extern am_hc32f460_tim4_timing_dev_t __g_tim43_timing_dev;

extern am_hc32f460_adtim_timing_dev_t __g_adtim61_timing_dev;
extern am_hc32f460_adtim_timing_dev_t __g_adtim62_timing_dev;
extern am_hc32f460_adtim_timing_dev_t __g_adtim63_timing_dev;


extern am_hc32f460_timea_cap_dev_t __g_timea1_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea2_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea3_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea4_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea5_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea6_cap_dev;

extern am_hc32f460_adtim_cap_dev_t  __g_adtim61_cap_dev;
extern am_hc32f460_adtim_cap_dev_t  __g_adtim62_cap_dev;
extern am_hc32f460_adtim_cap_dev_t  __g_adtim63_cap_dev;

/**
 *******************************************************************************
 ** \brief flag status
 ******************************************************************************/
typedef enum en_flag_status
{
    Reset = 0u,
    Set   = 1u,
} en_flag_status_t, en_int_status_t;

/*! Bit mask definition*/
#define     BIT_MASK_00                 (1ul << 0)
#define     BIT_MASK_01                 (1ul << 1)
#define     BIT_MASK_02                 (1ul << 2)
#define     BIT_MASK_03                 (1ul << 3)
#define     BIT_MASK_04                 (1ul << 4)
#define     BIT_MASK_05                 (1ul << 5)
#define     BIT_MASK_06                 (1ul << 6)
#define     BIT_MASK_07                 (1ul << 7)
#define     BIT_MASK_08                 (1ul << 8)
#define     BIT_MASK_09                 (1ul << 9)
#define     BIT_MASK_10                 (1ul << 10)
#define     BIT_MASK_11                 (1ul << 11)
#define     BIT_MASK_12                 (1ul << 12)
#define     BIT_MASK_13                 (1ul << 13)
#define     BIT_MASK_14                 (1ul << 14)
#define     BIT_MASK_15                 (1ul << 15)
#define     BIT_MASK_16                 (1ul << 16)
#define     BIT_MASK_17                 (1ul << 17)
#define     BIT_MASK_18                 (1ul << 18)
#define     BIT_MASK_19                 (1ul << 19)
#define     BIT_MASK_20                 (1ul << 20)
#define     BIT_MASK_21                 (1ul << 21)
#define     BIT_MASK_22                 (1ul << 22)
#define     BIT_MASK_23                 (1ul << 23)
#define     BIT_MASK_24                 (1ul << 24)
#define     BIT_MASK_25                 (1ul << 25)
#define     BIT_MASK_26                 (1ul << 26)
#define     BIT_MASK_27                 (1ul << 27)
#define     BIT_MASK_28                 (1ul << 28)
#define     BIT_MASK_29                 (1ul << 29)
#define     BIT_MASK_30                 (1ul << 30)
#define     BIT_MASK_31                 (1ul << 31)

#define bM4_TMR01_BCONR_INTENA                    (*((volatile unsigned int*)(0x42480208UL)))
#define bM4_TMR01_STFLR_CMAF                      (*((volatile unsigned int*)(0x42480280UL)))
#define bM4_TMR01_BCONR_INTENB                    (*((volatile unsigned int*)(0x42480248UL)))
#define bM4_TMR01_STFLR_CMBF                      (*((volatile unsigned int*)(0x424802C0UL)))
#define bM4_TMR02_BCONR_INTENA                    (*((volatile unsigned int*)(0x42488208UL)))
#define bM4_TMR02_STFLR_CMAF                      (*((volatile unsigned int*)(0x42488280UL)))
#define bM4_TMR02_BCONR_INTENB                    (*((volatile unsigned int*)(0x42488248UL)))
#define bM4_TMR02_STFLR_CMBF                      (*((volatile unsigned int*)(0x424882C0UL)))

/**
 *******************************************************************************
 ** \brief Int No.130 share IRQ handler
 **
 ******************************************************************************/
void IRQ130_Handler(void)
{
    uint32_t VSSEL130 = HC32F460_INTC.VSSEL[130 - 128];

    /* Timer0 Ch. 1 A compare match */
    if (Set == bM4_TMR01_BCONR_INTENA)
    {
        if ((Set == bM4_TMR01_STFLR_CMAF) && (VSSEL130 & BIT_MASK_00))
        {
            Timer01GCMA_IrqHandler(&__g_tim01_timing_dev);
        }
    }
    /* Timer0 Ch. 1 B compare match */
    if (Set == bM4_TMR01_BCONR_INTENB)
    {
        if ((Set == bM4_TMR01_STFLR_CMBF) && (VSSEL130 & BIT_MASK_01))
        {
            Timer01GCMB_IrqHandler(&__g_tim01_timing_dev);
        }
    }
    /* Timer0 Ch. 2 A compare match */
    if (Set == bM4_TMR02_BCONR_INTENA)
    {
        if ((Set == bM4_TMR02_STFLR_CMAF) && (VSSEL130 & BIT_MASK_02))
        {
            Timer02GCMA_IrqHandler(&__g_tim02_timing_dev);
        }
    }
    /* Timer0 Ch. 2 B compare match */
    if (Set == bM4_TMR02_BCONR_INTENB)
    {
        if ((Set == bM4_TMR02_STFLR_CMBF) && (VSSEL130 & BIT_MASK_03))
        {
            Timer02GCMB_IrqHandler(&__g_tim02_timing_dev);
        }
    }
    
//    /* Main-OSC stop */
//    if (Set == bM4_SYSREG_CMU_XTALSTDCR_XTALSTDIE)
//    {
//        if ((Set == bM4_SYSREG_CMU_XTALSTDSR_XTALSTDF) && (VSSEL130 & BIT_MASK_21))
//        {
//            MainOscStop_IrqHandler();
//        }
//    }
//    /* Wakeup timer */
//    if ((Set == bM4_WKTM_CR_WKOVF) && (VSSEL130 & BIT_MASK_22))
//    {
//        WakeupTimer_IrqHandler();
//    }
//    /* SWDT */
//    if ((M4_SWDT->SR & (BIT_MASK_16 | BIT_MASK_17)) && (VSSEL130 & BIT_MASK_23))
//    {
//        Swdt_IrqHandler();
//    }
}
#define bM4_TMR61_ICONR_INTENA                    (*((volatile unsigned int*)(0x42300A80UL)))
#define bM4_TMR61_ICONR_INTENB                    (*((volatile unsigned int*)(0x42300A84UL)))
#define bM4_TMR61_ICONR_INTENC                    (*((volatile unsigned int*)(0x42300A88UL)))
#define bM4_TMR61_ICONR_INTEND                    (*((volatile unsigned int*)(0x42300A8CUL)))
#define bM4_TMR61_ICONR_INTENE                    (*((volatile unsigned int*)(0x42300A90UL)))
#define bM4_TMR61_ICONR_INTENF                    (*((volatile unsigned int*)(0x42300A94UL)))
#define bM4_TMR61_ICONR_INTENOVF                  (*((volatile unsigned int*)(0x42300A98UL)))
#define bM4_TMR61_ICONR_INTENUDF                  (*((volatile unsigned int*)(0x42300A9CUL)))
#define bM4_TMR61_ICONR_INTENDTE                  (*((volatile unsigned int*)(0x42300AA0UL)))

#define bM4_TMR61_STFLR_CMAF                      (*((volatile unsigned int*)(0x42300E00UL)))
#define bM4_TMR61_STFLR_CMBF                      (*((volatile unsigned int*)(0x42300E04UL)))
#define bM4_TMR61_STFLR_CMCF                      (*((volatile unsigned int*)(0x42300E08UL)))
#define bM4_TMR61_STFLR_CMDF                      (*((volatile unsigned int*)(0x42300E0CUL)))
#define bM4_TMR61_STFLR_CMEF                      (*((volatile unsigned int*)(0x42300E10UL)))
#define bM4_TMR61_STFLR_CMFF                      (*((volatile unsigned int*)(0x42300E14UL)))
#define bM4_TMR61_STFLR_OVFF                      (*((volatile unsigned int*)(0x42300E18UL)))
#define bM4_TMR61_STFLR_UDFF                      (*((volatile unsigned int*)(0x42300E1CUL)))
#define bM4_TMR61_STFLR_DTEF                      (*((volatile unsigned int*)(0x42300E20UL)))

#define bM4_TMR62_ICONR_INTENA                    (*((volatile unsigned int*)(0x42308A80UL)))
#define bM4_TMR62_ICONR_INTENB                    (*((volatile unsigned int*)(0x42308A84UL)))
#define bM4_TMR62_ICONR_INTENC                    (*((volatile unsigned int*)(0x42308A88UL)))
#define bM4_TMR62_ICONR_INTEND                    (*((volatile unsigned int*)(0x42308A8CUL)))
#define bM4_TMR62_ICONR_INTENE                    (*((volatile unsigned int*)(0x42308A90UL)))
#define bM4_TMR62_ICONR_INTENF                    (*((volatile unsigned int*)(0x42308A94UL)))
#define bM4_TMR62_ICONR_INTENOVF                  (*((volatile unsigned int*)(0x42308A98UL)))
#define bM4_TMR62_ICONR_INTENUDF                  (*((volatile unsigned int*)(0x42308A9CUL)))
#define bM4_TMR62_ICONR_INTENDTE                  (*((volatile unsigned int*)(0x42308AA0UL)))


#define bM4_TMR62_STFLR_CMAF                      (*((volatile unsigned int*)(0x42308E00UL)))
#define bM4_TMR62_STFLR_CMBF                      (*((volatile unsigned int*)(0x42308E04UL)))
#define bM4_TMR62_STFLR_CMCF                      (*((volatile unsigned int*)(0x42308E08UL)))
#define bM4_TMR62_STFLR_CMDF                      (*((volatile unsigned int*)(0x42308E0CUL)))
#define bM4_TMR62_STFLR_CMEF                      (*((volatile unsigned int*)(0x42308E10UL)))
#define bM4_TMR62_STFLR_CMFF                      (*((volatile unsigned int*)(0x42308E14UL)))
#define bM4_TMR62_STFLR_OVFF                      (*((volatile unsigned int*)(0x42308E18UL)))
#define bM4_TMR62_STFLR_UDFF                      (*((volatile unsigned int*)(0x42308E1CUL)))
#define bM4_TMR62_STFLR_DTEF                      (*((volatile unsigned int*)(0x42308E20UL)))

#define bM4_TMR63_ICONR_INTENA                    (*((volatile unsigned int*)(0x42310A80UL)))
#define bM4_TMR63_ICONR_INTENB                    (*((volatile unsigned int*)(0x42310A84UL)))
#define bM4_TMR63_ICONR_INTENC                    (*((volatile unsigned int*)(0x42310A88UL)))
#define bM4_TMR63_ICONR_INTEND                    (*((volatile unsigned int*)(0x42310A8CUL)))
#define bM4_TMR63_ICONR_INTENE                    (*((volatile unsigned int*)(0x42310A90UL)))
#define bM4_TMR63_ICONR_INTENF                    (*((volatile unsigned int*)(0x42310A94UL)))
#define bM4_TMR63_ICONR_INTENOVF                  (*((volatile unsigned int*)(0x42310A98UL)))
#define bM4_TMR63_ICONR_INTENUDF                  (*((volatile unsigned int*)(0x42310A9CUL)))
#define bM4_TMR63_ICONR_INTENDTE                  (*((volatile unsigned int*)(0x42310AA0UL)))

#define bM4_TMR63_STFLR_CMAF                      (*((volatile unsigned int*)(0x42310E00UL)))
#define bM4_TMR63_STFLR_CMBF                      (*((volatile unsigned int*)(0x42310E04UL)))
#define bM4_TMR63_STFLR_CMCF                      (*((volatile unsigned int*)(0x42310E08UL)))
#define bM4_TMR63_STFLR_CMDF                      (*((volatile unsigned int*)(0x42310E0CUL)))
#define bM4_TMR63_STFLR_CMEF                      (*((volatile unsigned int*)(0x42310E10UL)))
#define bM4_TMR63_STFLR_CMFF                      (*((volatile unsigned int*)(0x42310E14UL)))
#define bM4_TMR63_STFLR_OVFF                      (*((volatile unsigned int*)(0x42310E18UL)))
#define bM4_TMR63_STFLR_UDFF                      (*((volatile unsigned int*)(0x42310E1CUL)))
#define bM4_TMR63_STFLR_DTEF                      (*((volatile unsigned int*)(0x42310E20UL)))

/**
 *******************************************************************************
 ** \brief Int No.131 share IRQ handler
 **
 ******************************************************************************/
void IRQ131_Handler(void)
{
    uint32_t VSSEL131 = HC32F460_INTC.VSSEL[131 - 128];
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    /* Timer6 Ch.1 A compare match */
    if (Set == bM4_TMR61_ICONR_INTENA)
    {
        if ((Set == bM4_TMR61_STFLR_CMAF) && (VSSEL131 & BIT_MASK_00))
        {
            Timer61GCMA_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /* Timer6 Ch.1 B compare match */
    if (Set == bM4_TMR61_ICONR_INTENB)
    {
        if ((Set == bM4_TMR61_STFLR_CMBF) && (VSSEL131 & BIT_MASK_01))
        {
            Timer61GCMB_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /* Timer6 Ch.1 C compare match */
    if (Set == bM4_TMR61_ICONR_INTENC)
    {
        if ((Set == bM4_TMR61_STFLR_CMCF) && (VSSEL131 & BIT_MASK_02))
        {
            Timer61GCMC_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /* Timer6 Ch.1 D compare match */
    if (Set == bM4_TMR61_ICONR_INTEND)
    {
        if ((Set == bM4_TMR61_STFLR_CMDF) && (VSSEL131 & BIT_MASK_03))
        {
            Timer61GCMD_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /* Timer6 Ch.1 E compare match */
    if (Set == bM4_TMR61_ICONR_INTENE)
    {
        if ((Set == bM4_TMR61_STFLR_CMEF) && (VSSEL131 & BIT_MASK_04))
        {
            Timer61GCME_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /* Timer6 Ch.1 F compare match */
    if (Set == bM4_TMR61_ICONR_INTENF)
    {
        if ((Set == bM4_TMR61_STFLR_CMFF) && (VSSEL131 & BIT_MASK_05))
        {
            Timer61GCMF_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /* Timer6 Ch.1 overflow */
    if (Set == bM4_TMR61_ICONR_INTENOVF)
    {
        if ((Set == bM4_TMR61_STFLR_OVFF) && (VSSEL131 & BIT_MASK_06))
        {
            Timer61GOV_IrqHandler(&__g_adtim61_timing_dev);
        }
    }
    /* Timer6 Ch.1 underflow */
    if (Set == bM4_TMR61_ICONR_INTENUDF)
    {
        if ((Set == bM4_TMR61_STFLR_UDFF) && (VSSEL131 & BIT_MASK_07))
        {
            Timer61GUD_IrqHandler(&__g_adtim61_timing_dev);
        }
    }
    /* Timer6 Ch.1 dead time */
    if (Set == bM4_TMR61_ICONR_INTENDTE)
    {
        if (((Set == bM4_TMR61_STFLR_DTEF)) && (VSSEL131 & BIT_MASK_08))
        {
            Timer61GDT_IrqHandler(&__g_adtim61_timing_dev);
        }
    }
    /* Timer6 Ch.1 A up-down compare match */
    u32Tmp1 = (HC32F460_TMR61->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
    u32Tmp2 = HC32F460_TMR61->STFLR & (BIT_MASK_09 | BIT_MASK_10);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_11))
    {
        Timer61SCMA_IrqHandler(&__g_adtim61_timing_dev);
    }
    /* Timer6 Ch.1 B up-down compare match */
    u32Tmp1 = (HC32F460_TMR61->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
    u32Tmp2 = HC32F460_TMR61->STFLR & (BIT_MASK_11 | BIT_MASK_12);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_12))
    {
        Timer61SCMB_IrqHandler(&__g_adtim61_timing_dev);
    }
    /* Timer6 Ch.2 A compare match */
    if (Set == bM4_TMR62_ICONR_INTENA)
    {
        if ((Set == bM4_TMR62_STFLR_CMAF) && (VSSEL131 & BIT_MASK_16))
        {
            Timer62GCMA_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /* Timer6 Ch.2 B compare match */
    if (Set == bM4_TMR62_ICONR_INTENB)
    {
        if ((Set == bM4_TMR62_STFLR_CMBF) && (VSSEL131 & BIT_MASK_17))
        {
            Timer62GCMB_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /* Timer6 Ch.2 C compare match */
    if (Set == bM4_TMR62_ICONR_INTENC)
    {
        if ((Set == bM4_TMR62_STFLR_CMCF) && (VSSEL131 & BIT_MASK_18))
        {
            Timer62GCMC_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /* Timer6 Ch.2 D compare match */
    if (Set == bM4_TMR62_ICONR_INTEND)
    {
        if ((Set == bM4_TMR62_STFLR_CMDF) && (VSSEL131 & BIT_MASK_19))
        {
            Timer62GCMD_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /* Timer6 Ch.2 E compare match */
    if (Set == bM4_TMR62_ICONR_INTENE)
    {
        if ((Set == bM4_TMR62_STFLR_CMEF) && (VSSEL131 & BIT_MASK_20))
        {
            Timer62GCME_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /* Timer6 Ch.2 F compare match */
    if (Set == bM4_TMR62_ICONR_INTENF)
    {
        if ((Set == bM4_TMR62_STFLR_CMFF) && (VSSEL131 & BIT_MASK_21))
        {
            Timer62GCMF_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /* Timer6 Ch.2 overflow */
    if (Set == bM4_TMR62_ICONR_INTENOVF)
    {
        if ((Set == bM4_TMR62_STFLR_OVFF) && (VSSEL131 & BIT_MASK_22))
        {
            Timer62GOV_IrqHandler(&__g_adtim62_timing_dev);
        }
    }
    /* Timer6 Ch.2 underflow */
    if (Set == bM4_TMR62_ICONR_INTENUDF)
    {
        if ((Set == bM4_TMR62_STFLR_UDFF) && (VSSEL131 & BIT_MASK_23))
        {
            Timer62GUD_IrqHandler(&__g_adtim62_timing_dev);
        }
    }
    /* Timer6 Ch.2 dead time */
    if (Set == bM4_TMR62_ICONR_INTENDTE)
    {
        if (((Set == bM4_TMR62_STFLR_DTEF)) && (VSSEL131 & BIT_MASK_24))
        {
            Timer62GDT_IrqHandler(&__g_adtim62_timing_dev);
        }
    }
    /* Timer6 Ch.2 A up-down compare match */
    u32Tmp1 = (HC32F460_TMR62->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
    u32Tmp2 = HC32F460_TMR62->STFLR & (BIT_MASK_09 | BIT_MASK_10);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_27))
    {
        Timer62SCMA_IrqHandler(&__g_adtim62_timing_dev);
    }
    /* Timer6 Ch.2 B up-down compare match */
    u32Tmp1 = (HC32F460_TMR62->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
    u32Tmp2 = HC32F460_TMR62->STFLR & (BIT_MASK_11 | BIT_MASK_12);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_28))
    {
        Timer62SCMB_IrqHandler(&__g_adtim62_timing_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.132 share IRQ handler
 **
 ******************************************************************************/
void IRQ132_Handler(void)
{
    uint32_t VSSEL132 = HC32F460_INTC.VSSEL[132 - 128];
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    /* Timer6 Ch.3 A compare match */
    if (Set == bM4_TMR63_ICONR_INTENA)
    {
        if ((Set == bM4_TMR63_STFLR_CMAF) && (VSSEL132 & BIT_MASK_00))
        {
            Timer63GCMA_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /* Timer6 Ch.3 B compare match */
    if (Set == bM4_TMR63_ICONR_INTENB)
    {
        if ((Set == bM4_TMR63_STFLR_CMBF) && (VSSEL132 & BIT_MASK_01))
        {
            Timer63GCMB_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /* Timer6 Ch.3 C compare match */
    if (Set == bM4_TMR63_ICONR_INTENC)
    {
        if ((Set == bM4_TMR63_STFLR_CMCF) && (VSSEL132 & BIT_MASK_02))
        {
            Timer63GCMC_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /* Timer6 Ch.3 D compare match */
    if (Set == bM4_TMR63_ICONR_INTEND)
    {
        if ((Set == bM4_TMR63_STFLR_CMDF) && (VSSEL132 & BIT_MASK_03))
        {
            Timer63GCMD_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /* Timer6 Ch.3 E compare match */
    if (Set == bM4_TMR63_ICONR_INTENE)
    {
        if ((Set == bM4_TMR63_STFLR_CMEF) && (VSSEL132 & BIT_MASK_04))
        {
            Timer63GCME_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /* Timer6 Ch.3 F compare match */
    if (Set == bM4_TMR63_ICONR_INTENF)
    {
        if ((Set == bM4_TMR63_STFLR_CMFF) && (VSSEL132 & BIT_MASK_05))
        {
            Timer63GCMF_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /* Timer6 Ch.3 overflow */
    if (Set == bM4_TMR63_ICONR_INTENOVF)
    {
        if ((Set == bM4_TMR63_STFLR_OVFF) && (VSSEL132 & BIT_MASK_06))
        {
            Timer63GOV_IrqHandler(&__g_adtim63_timing_dev);
        }
    }
    /* Timer6 Ch.3 underflow */
    if (Set == bM4_TMR63_ICONR_INTENUDF)
    {
        if ((Set == bM4_TMR63_STFLR_UDFF) && (VSSEL132 & BIT_MASK_07))
        {
            Timer63GUD_IrqHandler(&__g_adtim63_timing_dev);
        }
    }
    /* Timer6 Ch.3 dead time */
    if (Set == bM4_TMR63_ICONR_INTENDTE)
    {
        if (((Set == bM4_TMR63_STFLR_DTEF)) && (VSSEL132 & BIT_MASK_08))
        {
            Timer63GDT_IrqHandler(&__g_adtim63_timing_dev);
        }
    }
    /* Timer6 Ch.3 A up-down compare match */
    u32Tmp1 = (HC32F460_TMR63->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
    u32Tmp2 = HC32F460_TMR63->STFLR & (BIT_MASK_09 | BIT_MASK_10);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL132 & BIT_MASK_11))
    {
        Timer63SCMA_IrqHandler(&__g_adtim63_timing_dev);
    }
    /* Timer6 Ch.3 B up-down compare match */
    u32Tmp1 = (HC32F460_TMR63->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
    u32Tmp2 = HC32F460_TMR63->STFLR & (BIT_MASK_11 | BIT_MASK_12);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL132 & BIT_MASK_12))
    {
        Timer63SCMB_IrqHandler(&__g_adtim63_timing_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.136 share IRQ handler
 **
 ******************************************************************************/
void IRQ136_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;

    uint32_t int_vssel136 = HC32F460_INTC.VSSEL[136 - 128];
    
#if 0
    u32Tmp1 = HC32F460_UART1->SR;
    u32Tmp2 = HC32F460_UART1->CR1;
 
    /* USART Ch.1 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart1_dev);
    }

    u32Tmp1 = HC32F460_UART2->SR;
    u32Tmp2 = HC32F460_UART2->CR1;

    /* USART Ch.1 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart2_dev);
    }
#endif

    u32Tmp1 = HC32F460_TMRA1->BCSTR;
    /* TimerA Ch.1 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_00))
    {
        TimerA1OV_IrqHandler(&__g_timea1_timing_dev);
    }
    /* TimerA Ch.1 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_01))
    {
        TimerA1UD_IrqHandler(&__g_timea1_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA1->ICONR;
    u32Tmp2 = HC32F460_TMRA1->STFLR;
    /* TimerA Ch.1 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_02))
    {
        TimerA1CMP_IrqHandler(&__g_timea1_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA2->BCSTR;
    /* TimerA Ch.2 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_03))
    {
        TimerA2OV_IrqHandler(&__g_timea2_timing_dev);
    }
    /* TimerA Ch.2 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_04))
    {
        TimerA2UD_IrqHandler(&__g_timea2_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA2->ICONR;
    u32Tmp2 = HC32F460_TMRA2->STFLR;
    /* TimerA Ch.2 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_05))
    {
        TimerA2CMP_IrqHandler(&__g_timea2_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA3->BCSTR;
    /* TimerA Ch.3 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_06))
    {
        TimerA3OV_IrqHandler(&__g_timea3_timing_dev);
    }
    /* TimerA Ch.3 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_07))
    {
        TimerA3UD_IrqHandler(&__g_timea3_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA3->ICONR;
    u32Tmp2 = HC32F460_TMRA3->STFLR;
    /* TimerA Ch.3 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_08))
    {
        TimerA3CMP_IrqHandler(&__g_timea3_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA4->BCSTR;
    /* TimerA Ch.4 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_09))
    {
        TimerA4OV_IrqHandler(&__g_timea4_timing_dev);
    }
    /* TimerA Ch.4 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_10))
    {
        TimerA4UD_IrqHandler(&__g_timea4_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA4->ICONR;
    u32Tmp2 = HC32F460_TMRA4->STFLR;
    /* TimerA Ch.4 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_11))
    {
        TimerA4CMP_IrqHandler(&__g_timea4_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA5->BCSTR;
    /* TimerA Ch.5 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_12))
    {
        TimerA5OV_IrqHandler(&__g_timea5_timing_dev);
    }
    /* TimerA Ch.5 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_13))
    {
        TimerA5UD_IrqHandler(&__g_timea5_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA5->ICONR;
    u32Tmp2 = HC32F460_TMRA5->STFLR;
    /* TimerA Ch.5 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_14))
    {
        TimerA5CMP_IrqHandler(&__g_timea5_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA6->BCSTR;
    /* TimerA Ch.6 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_16))
    {
        TimerA6OV_IrqHandler(&__g_timea6_timing_dev);
    }
    /* TimerA Ch.6 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_17))
    {
        TimerA6UD_IrqHandler(&__g_timea6_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA6->ICONR;
    u32Tmp2 = HC32F460_TMRA6->STFLR;
    /* TimerA Ch.6 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_18))
    {
        TimerA6CMP_IrqHandler(&__g_timea6_cap_dev);
    }    
}

/**
 *******************************************************************************
 ** \brief Int No.137 share IRQ handler
 **
 ******************************************************************************/
void IRQ137_Handler(void)
{
#if 0
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;

    uint32_t int_vssel136 = HC32F460_INTC.VSSEL[136 - 128];
    uint32_t int_vssel137 = HC32F460_INTC.VSSEL[137 - 128];
    uint32_t int_vssel141 = HC32F460_INTC.VSSEL[141 - 128];
    uint32_t int_flag_uart1 = HC32F460_UART1->SR;
    uint32_t int_flag_uart2 = HC32F460_UART2->SR;
    uint32_t int_flag_uart3 = HC32F460_UART3->SR;
    uint32_t int_flag_uart4 = HC32F460_UART4->SR;

    u32Tmp1 = HC32F460_UART3->SR;
    u32Tmp2 = HC32F460_UART3->CR1;

    /* USART Ch.3 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart3_dev);
    }
    /* USART Ch.3 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart3_dev);
    }
    /* USART Ch.3 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart3_dev);
    }
    /* USART Ch.3 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart3_dev);
    }
    /* USART Ch.3 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart3_dev);
    }

    u32Tmp1 = HC32F460_UART4->SR;
    u32Tmp2 = HC32F460_UART4->CR1;

    /* USART Ch.1 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.1 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.1 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.1 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.1 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart4_dev);
    }
#endif    
}

/**
 *******************************************************************************
 ** \brief Int No.138 share IRQ handler
 **
 ******************************************************************************/
void IRQ138_Handler(void)
{
    uint32_t u32Tmp1 = 0u;
    uint32_t VSSEL138 = HC32F460_INTC.VSSEL[138 - 128];

    u32Tmp1 = HC32F460_TMR41->OCSRU;
    /* Timer4 Ch.1 U phase higher compare match */
    if ((VSSEL138 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer41GCMUH_IrqHandler(&__g_tim41_timing_dev);
    }
    /* Timer4 Ch.1 U phase lower compare match */
    if ((VSSEL138 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer41GCMUL_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->OCSRV;
    /* Timer4 Ch.1 V phase higher compare match */
    if ((VSSEL138 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer41GCMVH_IrqHandler(&__g_tim41_timing_dev);
    }
    /* Timer4 Ch.1 V phase lower compare match */
    if ((VSSEL138 & BIT_MASK_03) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer41GCMVL_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->OCSRW;
    /* Timer4 Ch.1 W phase higher compare match */
    if ((VSSEL138 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer41GCMWH_IrqHandler(&__g_tim41_timing_dev);
    }
    /* Timer4 Ch.1 W phase lower compare match */
    if ((VSSEL138 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer41GCMWL_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->CCSR;
    /* Timer4 Ch.1 overflow */
    if ((VSSEL138 & BIT_MASK_06) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
    {
        Timer41GOV_IrqHandler(&__g_tim41_timing_dev);
    }
    /* Timer4 Ch.1 underflow */
    if ((VSSEL138 & BIT_MASK_07) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
    {
        Timer41GUD_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->RCSR;
    /* Timer4 Ch.1 U phase reload */
    if ((VSSEL138 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04))
    {
        Timer41ReloadU_IrqHandler(&__g_tim41_timing_dev);
    }
    /* Timer4 Ch.1 V phase reload */
    if ((VSSEL138 & BIT_MASK_09) && (u32Tmp1 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08))
    {
        Timer41ReloadV_IrqHandler(&__g_tim41_timing_dev);
    }
    /* Timer4 Ch.1 W phase reload */
    if ((VSSEL138 & BIT_MASK_10) && (u32Tmp1 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_12))
    {
        Timer41ReloadW_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->OCSRU;
    /* Timer4 Ch.2 U phase higher compare match */
    if ((VSSEL138 & BIT_MASK_16) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer42GCMUH_IrqHandler(&__g_tim42_timing_dev);
    }
    /* Timer4 Ch.2 U phase lower compare match */
    if ((VSSEL138 & BIT_MASK_17) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer42GCMUL_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->OCSRV;
    /* Timer4 Ch.2 V phase higher compare match */
    if ((VSSEL138 & BIT_MASK_18) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer42GCMVH_IrqHandler(&__g_tim42_timing_dev);
    }
    /* Timer4 Ch.2 V phase lower compare match */
    if ((VSSEL138 & BIT_MASK_19) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer42GCMVL_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->OCSRW;
    /* Timer4 Ch.2 W phase higher compare match */
    if ((VSSEL138 & BIT_MASK_20) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer42GCMWH_IrqHandler(&__g_tim42_timing_dev);
    }
    /* Timer4 Ch.2 W phase lower compare match */
    if ((VSSEL138 & BIT_MASK_21) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer42GCMWL_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->CCSR;
    /* Timer4 Ch.2 overflow */
    if ((VSSEL138 & BIT_MASK_22) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
    {
        Timer42GOV_IrqHandler(&__g_tim42_timing_dev);
    }
    /* Timer4 Ch.2 underflow */
    if ((VSSEL138 & BIT_MASK_23) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
    {
        Timer42GUD_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->RCSR;
    /* Timer4 Ch.2 U phase reload */
    if ((VSSEL138 & BIT_MASK_24) && (u32Tmp1 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04))
    {
        Timer42ReloadU_IrqHandler(&__g_tim42_timing_dev);
    }
    /* Timer4 Ch.2 V phase reload */
    if ((VSSEL138 & BIT_MASK_25) && (u32Tmp1 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08))
    {
        Timer42ReloadV_IrqHandler(&__g_tim42_timing_dev);
    }
    /* Timer4 Ch.2 W phase reload */
    if ((VSSEL138 & BIT_MASK_26) && (u32Tmp1 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_12))
    {
        Timer42ReloadW_IrqHandler(&__g_tim42_timing_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.139 share IRQ handler
 **
 ******************************************************************************/
void IRQ139_Handler(void)
{
    uint32_t u32Tmp1 = 0u;
    uint32_t VSSEL139 = HC32F460_INTC.VSSEL[139 - 128];;

    u32Tmp1 = HC32F460_TMR43->OCSRU;
    /* Timer4 Ch.3 U phase higher compare match */
    if ((VSSEL139 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer43GCMUH_IrqHandler(&__g_tim43_timing_dev);
    }
    /* Timer4 Ch.3 U phase lower compare match */
    if ((VSSEL139 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer43GCMUL_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->OCSRV;
    /* Timer4 Ch.3 V phase higher compare match */
    if ((VSSEL139 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer43GCMVH_IrqHandler(&__g_tim43_timing_dev);
    }
    /* Timer4 Ch.3 V phase lower compare match */
    if ((VSSEL139 & BIT_MASK_03) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer43GCMVL_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->OCSRW;
    /* Timer4 Ch.3 W phase higher compare match */
    if ((VSSEL139 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer43GCMWH_IrqHandler(&__g_tim43_timing_dev);
    }
    /* Timer4 Ch.3 W phase lower compare match */
    if ((VSSEL139 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer43GCMWL_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->CCSR;
    /* Timer4 Ch.3 overflow */
    if ((VSSEL139 & BIT_MASK_06) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
    {
        Timer43GOV_IrqHandler(&__g_tim43_timing_dev);
    }
    /* Timer4 Ch.3 underflow */
    if ((VSSEL139 & BIT_MASK_07) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
    {
        Timer43GUD_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->RCSR;
    /* Timer4 Ch.3 U phase reload */
    if ((VSSEL139 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04))
    {
        Timer41ReloadU_IrqHandler(&__g_tim43_timing_dev);
    }
    /* Timer4 Ch.3 V phase reload */
    if ((VSSEL139 & BIT_MASK_09) && (u32Tmp1 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08))
    {
        Timer43ReloadV_IrqHandler(&__g_tim43_timing_dev);
    }
    /* Timer4 Ch.3 W phase reload */
    if ((VSSEL139 & BIT_MASK_10) && (u32Tmp1 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_12))
    {
        Timer43ReloadW_IrqHandler(&__g_tim43_timing_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.137 share IRQ handler
 **
 ******************************************************************************/
void IRQ141_Handler(void *parg)
{

}


