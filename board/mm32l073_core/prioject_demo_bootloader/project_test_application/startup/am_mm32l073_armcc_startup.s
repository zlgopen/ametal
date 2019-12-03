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
; * \brief MM32L073 startup code for ARMCC compiler
; * 
; * \internal
; * \par Modification History
; * - 1.00 17-10-26  pea, first implementation.
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

__Vectors       DCD     __initial_sp                   ; Top of Stack
                DCD     Reset_Handler                  ; Reset Handler
                DCD     NMI_Handler                    ; NMI Handler
                DCD     HardFault_Handler              ; Hard Fault Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     SVC_Handler                    ; SVCall Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     PendSV_Handler                 ; PendSV Handler
                DCD     SysTick_Handler                ; SysTick Handler

                ; External Interrupts
                DCD     am_exc_eint_handler            ; Window Watchdog
                DCD     am_exc_eint_handler            ; PVD through EXTI Line detect
                DCD     am_exc_eint_handler            ; RTC through EXTI Line & Tamper
                DCD     am_exc_eint_handler            ; FLASH
                DCD     am_exc_eint_handler            ; RCC & CRS
                DCD     am_exc_eint_handler            ; EXTI Line 0 and 1
                DCD     am_exc_eint_handler            ; EXTI Line 2 and 3
                DCD     am_exc_eint_handler            ; EXTI Line 4 to 15
                DCD     0                              ; Reserved
                DCD     am_exc_eint_handler            ; DMA1 Channel 1
                DCD     am_exc_eint_handler            ; DMA1 Channel 2 and Channel 3
                DCD     am_exc_eint_handler            ; DMA1 Channel 4 and Channel 5
                DCD     am_exc_eint_handler            ; ADC1 & COMP
                DCD     am_exc_eint_handler            ; TIM1 Break, Update, Trigger and Commutation
                DCD     am_exc_eint_handler            ; TIM1 Capture Compare
                DCD     am_exc_eint_handler            ; TIM2
                DCD     am_exc_eint_handler            ; TIM3
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     am_exc_eint_handler            ; TIM14
                DCD     0                              ; Reserved
                DCD     am_exc_eint_handler            ; TIM16
                DCD     am_exc_eint_handler            ; TIM17
                DCD     am_exc_eint_handler            ; I2C1
                DCD     0                              ; Reserved
                DCD     am_exc_eint_handler            ; SPI1
                DCD     am_exc_eint_handler            ; SPI2
                DCD     am_exc_eint_handler            ; UART1
                DCD     am_exc_eint_handler            ; UART2
                DCD     am_exc_eint_handler            ; AES
                DCD     am_exc_eint_handler            ; CAN
                DCD     am_exc_eint_handler            ; USB

                AREA    |.text|, CODE, READONLY

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
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
