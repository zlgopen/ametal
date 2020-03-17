//#include "hc32f460_intctrl.h"
#include "am_hc32f460.h"
#include "am_hc32f460_gpio.h"
#include "am_hc32f460_uart.h"

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
/**
 *******************************************************************************
 ** \brief Int No.137 share IRQ handler
 **
 ******************************************************************************/
void IRQ141_Handler(void *parg)
{

}


