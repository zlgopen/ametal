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
; * \brief ZLG217 startup code for ARMCC compiler
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

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     am_exc_eint_handler       ;0 - Window Watchdog
                DCD     am_exc_eint_handler       ;1 - PVD through EXTI Line detect
                DCD     am_exc_eint_handler       ;2 - Tamper
                DCD     am_exc_eint_handler       ;3 - RTC
                DCD     am_exc_eint_handler       ;4 - Flash
                DCD     am_exc_eint_handler       ;5 - RCC
                DCD     am_exc_eint_handler       ;6 - EXTI Line 0
                DCD     am_exc_eint_handler       ;7 - EXTI Line 1
                DCD     am_exc_eint_handler       ;8 - EXTI Line 2
                DCD     am_exc_eint_handler       ;9 - EXTI Line 3
                DCD     am_exc_eint_handler       ;10 - EXTI Line 4
                DCD     am_exc_eint_handler       ;11 - DMA1 Channel 1
                DCD     am_exc_eint_handler       ;12 - DMA1 Channel 2
                DCD     am_exc_eint_handler       ;13 - DMA1 Channel 3
                DCD     am_exc_eint_handler       ;14 - DMA1 Channel 4
                DCD     am_exc_eint_handler       ;15 - DMA1 Channel 5
                DCD     am_exc_eint_handler       ;16 - DMA1 Channel 6
                DCD     am_exc_eint_handler       ;17 - DMA1 Channel 7
                DCD     am_exc_eint_handler       ;18 - ADC1_2
                DCD     am_exc_eint_handler       ;19 - USB
                DCD     0                         ;20 - Reserve
                DCD     am_exc_eint_handler       ;21 - CAN1 RX1
                DCD     0                         ;22 - Reserve
                DCD     am_exc_eint_handler       ;23 - EXTI Line 9..5
                DCD     am_exc_eint_handler       ;24 - TIM1 Break
                DCD     am_exc_eint_handler       ;25 - TIM1 Update
                DCD     am_exc_eint_handler       ;26 - TIM1 Trigger and Commutation
                DCD     am_exc_eint_handler       ;27 - TIM1 Capture Compare
                DCD     am_exc_eint_handler       ;28 - TIM2
                DCD     am_exc_eint_handler       ;29 - TIM3
                DCD     am_exc_eint_handler       ;30 - TIM4
                DCD     am_exc_eint_handler       ;31 - I2C1 Event
                DCD     0                         ;32 - Reserve
                DCD     am_exc_eint_handler       ;33 - I2C2 Event
                DCD     0                         ;34 - Reserve
                DCD     am_exc_eint_handler       ;35 - SPI1
                DCD     am_exc_eint_handler       ;36 - SPI2
                DCD     am_exc_eint_handler       ;37 - UART1
                DCD     am_exc_eint_handler       ;38 - UART2
                DCD     am_exc_eint_handler       ;39 - UART3
                DCD     am_exc_eint_handler       ;40 - EXTI Line 15..10
                DCD     am_exc_eint_handler       ;41 - RTC Alarm through EXTI Line 17
                DCD     am_exc_eint_handler       ;42 - USB Wakeup from suspend
                DCD     0                         ;43 - Reserve
                DCD     0                         ;44 - Reserve
                DCD     am_exc_eint_handler       ;45 - AES
 
                AREA    |.text|, CODE, READONLY

; Reset Handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                CPSID   I                   ; 禁能全局中断
                LDR     R0, =0xE000ED08	;
                LDR     R1, =__Vectors
                STR     R1, [R0]
                LDR     R2, [R1]
                MSR     MSP, R2
                CPSIE   I                   ; 打开全局中断
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
