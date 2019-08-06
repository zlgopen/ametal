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
 * \brief 中断向量表
 *
 * \internal
 * \par modification history
 * - 1.01 17-12-11  pea, amend annotation
 * - 1.00 11-04-09  mifi, first implementation
 * \endinternal
 */

/***************************************************************************
  外部变量声明
***************************************************************************/
extern unsigned long _estack;

/***************************************************************************
  外部函数声明
***************************************************************************/
extern void am_exc_eint_handler (void);
extern void ResetHandler(void);
/***************************************************************************
  本地函数声明
***************************************************************************/

void NMI_Handler (void) __attribute__((weak));
void HardFault_Handler (void) __attribute__((weak));
void MemManage_Handler (void) __attribute__((weak));
void BusFault_Handler (void) __attribute__((weak));
void UsageFault_Handler (void) __attribute__((weak));
void SVC_Handler (void) __attribute__((weak));
void DebugMon_Handler (void) __attribute__((weak));
void PendSV_Handler (void) __attribute__((weak));
void SysTick_Handler (void) __attribute__((weak));
void am_exc_eint_handler (void) __attribute__((weak));

/***************************************************************************
  本地全局变量定义
***************************************************************************/

/** \brief 中断向量表 */
__attribute__ ((section(".vectors"), used))
void (* const gVectors[])(void) =
{
    (void (*)(void))((unsigned long)&_estack),
    ResetHandler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    0, 0,
    PendSV_Handler,
    SysTick_Handler,

    am_exc_eint_handler,         /* 0 - Window Watchdog */
    am_exc_eint_handler,         /* 1 - PVD through EXTI Line detect */
    am_exc_eint_handler,         /* 2 - Tamper */
    am_exc_eint_handler,         /* 3 - RTC */
    am_exc_eint_handler,         /* 4 - Flash */
    am_exc_eint_handler,         /* 5 - RCC */
    am_exc_eint_handler,         /* 6 - EXTI Line 0 */
    am_exc_eint_handler,         /* 7 - EXTI Line 1 */
    am_exc_eint_handler,         /* 8 - EXTI Line 2 */
    am_exc_eint_handler,         /* 9 - EXTI Line 3 */
    am_exc_eint_handler,         /* 10 - EXTI Line 4 */
    am_exc_eint_handler,         /* 11 - DMA1 Channel 1 */
    am_exc_eint_handler,         /* 12 - DMA1 Channel 2 */
    am_exc_eint_handler,         /* 13 - DMA1 Channel 3 */
    am_exc_eint_handler,         /* 14 - DMA1 Channel 4 */
    am_exc_eint_handler,         /* 15 - DMA1 Channel 5 */
    am_exc_eint_handler,         /* 16 - DMA1 Channel 6 */
    am_exc_eint_handler,         /* 17 - DMA1 Channel 7 */
    am_exc_eint_handler,         /* 18 - ADC1_2 */
    am_exc_eint_handler,         /* 19 - USB prioty high or CAN1 TX*/
    am_exc_eint_handler,         /* 20 - USB prioty high or CAN1 RX0*/
    am_exc_eint_handler,         /* 21 - CAN1 RX1 */
    am_exc_eint_handler,         /* 22 - CAN1 SEC */
    am_exc_eint_handler,         /* 23 - EXTI Line 9..5 */
    am_exc_eint_handler,         /* 24 - TIM1 Break */
    am_exc_eint_handler,         /* 25 - TIM1 Update */
    am_exc_eint_handler,         /* 26 - TIM1 Trigger and Commutation */
    am_exc_eint_handler,         /* 27 - TIM1 Capture Compare */
    am_exc_eint_handler,         /* 28 - TIM2 */
    am_exc_eint_handler,         /* 29 - TIM3 */
    am_exc_eint_handler,         /* 30 - TIM4 */
    am_exc_eint_handler,         /* 31 - I2C1 Event */
    0,                           /* 32 - Reserve */
    am_exc_eint_handler,         /* 33 - I2C2 Event */
    am_exc_eint_handler,         /* 34 - I2C2 ER */
    am_exc_eint_handler,         /* 35 - SPI1 */
    am_exc_eint_handler,         /* 36 - SPI2 */
    am_exc_eint_handler,         /* 37 - UART1 */
    am_exc_eint_handler,         /* 38 - UART2 */
    am_exc_eint_handler,         /* 39 - UART3 */
    am_exc_eint_handler,         /* 40 - EXTI Line 15..10 */
    am_exc_eint_handler,         /* 41 - RTC Alarm through EXTI Line 17 */
    am_exc_eint_handler,         /* 42 - USB Wakeup from suspend */
};

/***************************************************************************
  本地函数定义
***************************************************************************/
void NMI_Handler (void) { while(1); }
void HardFault_Handler (void) { while(1); }
void MemManage_Handler (void) { while(1); }
void BusFault_Handler (void) { while(1); }
void UsageFault_Handler (void) { while(1); }
void SVC_Handler (void) { while(1); }
void DebugMon_Handler (void) { while(1); }
void PendSV_Handler (void) { while(1); }
void SysTick_Handler (void) { while(1); }

void am_exc_eint_handler (void) { while(1); }

/* end of file */
