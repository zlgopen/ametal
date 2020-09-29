;/*******************************************************************************
;*                                 AMetal
;*                       ----------------------------
;*                       innovating embedded platform
;*
;* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
;* All rights reserved.
;*
;* Contact information:
;* web site:    http://www.zlg.cn/
;*******************************************************************************/

;/**
; * \file
; * \brief LPC8xx startup code for ARMCC compiler
; *
; * \internal
; * \par Modification History
; * - 1.00 15-06-25  jon, first implementation
; * \endinternal
; */

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp         ; Top of Stack
                DCD     Reset_Handler        ; Reset Handler
                DCD     NMI_Handler          ; NMI Handler
                DCD     HardFault_Handler    ; Hard Fault Handler
                DCD     0                    ; Reserved
                DCD     0                    ; Reserved
                DCD     0                    ; Reserved
                DCD     0                    ; Reserved
                DCD     0                    ; Reserved
                DCD     0                    ; Reserved
                DCD     0                    ; Reserved
                DCD     SVC_Handler          ; SVCall Handler
                DCD     0                    ; Reserved
                DCD     0                    ; Reserved
                DCD     PendSV_Handler       ; PendSV Handler
                DCD     SysTick_Handler      ; SysTick Handler

                ; External Interrupts
                DCD     am_exc_eint_handler  ; PORTA_IRQHandler
                DCD     am_exc_eint_handler  ; PORTB_IRQHandler
                DCD     am_exc_eint_handler  ; PORTC_E_IRQHandler
                DCD     am_exc_eint_handler  ; PORTD_F_IRQHandler
                DCD     am_exc_eint_handler  ; DMAC_IRQHandler
                DCD     am_exc_eint_handler  ; TIM3_IRQHandler
                DCD     am_exc_eint_handler  ; UART0_2_IRQHandler
                DCD     am_exc_eint_handler  ; UART1_3_IRQHandler
                DCD     am_exc_eint_handler  ; LPUART0_IRQHandler
                DCD     am_exc_eint_handler  ; LPUART1_IRQHandler
                DCD     am_exc_eint_handler  ; SPI0_IRQHandler
                DCD     am_exc_eint_handler  ; SPI1_IRQHandler
                DCD     am_exc_eint_handler  ; I2C0_IRQHandler
                DCD     am_exc_eint_handler  ; I2C1_IRQHandler
                DCD     am_exc_eint_handler  ; TIM0_IRQHandler
                DCD     am_exc_eint_handler  ; TIM1_IRQHandler
                DCD     am_exc_eint_handler  ; TIM2_IRQHandler
                DCD     am_exc_eint_handler  ; LPTIM0_1_IRQHandler
                DCD     am_exc_eint_handler  ; TIM4_IRQHandler
                DCD     am_exc_eint_handler  ; TIM5_IRQHandler
                DCD     am_exc_eint_handler  ; TIM6_IRQHandler
                DCD     am_exc_eint_handler  ; PCA_IRQHandler
                DCD     am_exc_eint_handler  ; WDT_IRQHandler
                DCD     am_exc_eint_handler  ; RTC_IRQHandler
                DCD     am_exc_eint_handler  ; ADC_DAC_IRQHandler
                DCD     am_exc_eint_handler  ; PCNT_IRQHandler
                DCD     am_exc_eint_handler  ; VC0_IRQHandler
                DCD     am_exc_eint_handler  ; VC1_2_IRQHandler
                DCD     am_exc_eint_handler  ; LVD_IRQHandler
                DCD     am_exc_eint_handler  ; LCD_IRQHandler
                DCD     am_exc_eint_handler  ; FLASH_RAM_IRQHandler
                DCD     am_exc_eint_handler  ; CLKTRIM_IRQHandler

                AREA    |.text|, CODE, READONLY

; Reset Handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
					
				CPSID   I				
				LDR     R0, =0xE000ED08	;
                LDR     R1, =__Vectors
                STR     R1, [R0]
                LDR     R2, [R1]
                MSR     MSP, R2
                CPSIE   i
				
                LDR     R0, =__main
                BX      R0
                ENDP

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP

HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  am_exc_eint_handler       [WEAK]

am_exc_eint_handler
                B       .

                ENDP

                ALIGN

; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, =(Heap_Mem  + Heap_Size )
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END
