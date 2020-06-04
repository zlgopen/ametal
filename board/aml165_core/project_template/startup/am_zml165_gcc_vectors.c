/****************************************************************************
*  Copyright (c) 2011 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may
*     be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
*  SUCH DAMAGE.
*
****************************************************************************
*  History:
*
*  09.04.2011  mifi  First Version for the ZML165
****************************************************************************/
#define __VECTORS_ZML165_C__

/*=========================================================================*/
/*  DEFINE: All extern Data                                                */
/*=========================================================================*/
extern unsigned long _estack;

/*=========================================================================*/
/*  DEFINE: Prototypes                                                     */
/*=========================================================================*/

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

extern void am_exc_eint_handler (void);
extern void ResetHandler(void);
/*=========================================================================*/
/*  DEFINE: All code exported                                              */
/*=========================================================================*/
/*
 * This is our vector table.
 */
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
   0,
   0,
   PendSV_Handler,
   SysTick_Handler,

   am_exc_eint_handler,         /* 0 */
   am_exc_eint_handler,         /* 1 */
   0,                           /* 2 */
   am_exc_eint_handler,         /* 3 */
   am_exc_eint_handler,         /* 4 */
   am_exc_eint_handler,         /* 5 */
   am_exc_eint_handler,         /* 6 */
   am_exc_eint_handler,         /* 7 */
   0,                           /* 8 */
   am_exc_eint_handler,         /* 9 */
   am_exc_eint_handler,         /* 10 */
   am_exc_eint_handler,         /* 11 */
   am_exc_eint_handler,         /* 12 */
   am_exc_eint_handler,         /* 13 */
   am_exc_eint_handler,         /* 14 */
   am_exc_eint_handler,         /* 15 */
   am_exc_eint_handler,         /* 16 */
   0,                           /* 17 */
   0,                           /* 18 */
   am_exc_eint_handler,         /* 19 */
   0,                           /* 20 */
   am_exc_eint_handler,         /* 21 */
   am_exc_eint_handler,         /* 22 */
   am_exc_eint_handler,         /* 23 */
   0,                           /* 24 */
   am_exc_eint_handler,         /* 25 */
   am_exc_eint_handler,         /* 26 */
   am_exc_eint_handler,         /* 27 */
   am_exc_eint_handler,         /* 28 */
   am_exc_eint_handler,         /* 29 */
   am_exc_eint_handler,         /* 30 */
   am_exc_eint_handler          /* 31 */
}; /* gVectors */

/*
 * And here are the weak interrupt handlers.
 */
void am_exc_eint_handler (void) { while(1); }
void NMI_Handler (void) { while(1); }
void HardFault_Handler (void) { while(1); }
void MemManage_Handler (void) { while(1); }
void BusFault_Handler (void) { while(1); }
void UsageFault_Handler (void) { while(1); }
void SVC_Handler (void) { while(1); }
void DebugMon_Handler (void) { while(1); }
void PendSV_Handler (void) { while(1); }
void SysTick_Handler (void) { while(1); }

void WDT_IRQHandler (void) { while(1); }
void TIMER0_IRQHandler (void) { while(1); }
void TIMER1_IRQHandler (void) { while(1); }
void TIMER2_IRQHandler (void) { while(1); }
void TIMER3_IRQHandler (void) { while(1); }
void UART0_IRQHandler (void) { while(1); }
void UART1_IRQHandler (void) { while(1); }
void UART2_IRQHandler (void) { while(1); }
void UART3_IRQHandler (void) { while(1); }
void PWM_IRQHandler (void) { while(1); }
void I2C0_IRQHandler (void) { while(1); }
void I2C1_IRQHandler (void) { while(1); }
void I2C2_IRQHandler (void) { while(1); }
void SPI_IRQHandler (void) { while(1); }
void SSP0_IRQHandler (void) { while(1); }
void SSP1_IRQHandler (void) { while(1); }
void PLL0_IRQHandler (void) { while(1); }
void RTC_IRQHandler (void) { while(1); }
void EINT0_IRQHandler (void) { while(1); }
void EINT1_IRQHandler (void) { while(1); }
void EINT2_IRQHandler (void) { while(1); }
void EINT3_IRQHandler (void) { while(1); }
void ADC_IRQHandler (void) { while(1); }
void BOD_IRQHandler (void) { while(1); }
void USB_IRQHandler (void) { while(1); }
void CAN_IRQHandler (void) { while(1); }
void DMA_IRQHandler (void) { while(1); }
void I2S_IRQHandler (void) { while(1); }
void ENET_IRQHandler (void) { while(1); }
void RIT_IRQHandler (void) { while(1); }
void MCPWM_IRQHandler (void) { while(1); }
void QEI_IRQHandler (void) { while(1); }

/*** EOF ***/
