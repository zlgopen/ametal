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

Stack_Size      EQU     0x0000a00

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
                DCD     am_exc_eint_handler  ; SPI0 controller
                DCD     am_exc_eint_handler  ; SPI1 controller
                DCD     0                    ; Reserved
                DCD     am_exc_eint_handler  ; UART0
                DCD     am_exc_eint_handler  ; UART1
                DCD     am_exc_eint_handler  ; UART2
                DCD     0                    ; Reserved
                DCD     am_exc_eint_handler  ; I2C1 controller
                DCD     am_exc_eint_handler  ; I2C0 controller
                DCD     am_exc_eint_handler  ; Smart Counter Timer
                DCD     am_exc_eint_handler  ; Multi-Rate Timer
                DCD     am_exc_eint_handler  ; Comparator
                DCD     am_exc_eint_handler  ; watchdog warning interrupt
                DCD     am_exc_eint_handler  ; Brown Out Detect
                DCD     am_exc_eint_handler  ; Non-Volatile Memory Controller
                DCD     am_exc_eint_handler  ; Wakeup timer
                DCD     am_exc_eint_handler  ; ADC Sequence A Completion [Only on LPC82X]
                DCD     am_exc_eint_handler  ; ADC Sequence B Completion [Only on LPC82X]
                DCD     am_exc_eint_handler  ; ADC Threshold compare [Only on LPC82X]
                DCD     am_exc_eint_handler  ; ADC Overrun [Only on LPC82X]
                DCD     am_exc_eint_handler  ; DMA Controller [Only on LPC82X]
                DCD     am_exc_eint_handler  ; I2C2 Controller [Only on LPC82X]
                DCD     am_exc_eint_handler  ; I2C3 Controller [Only on LPC82X]
                DCD     0                    ; Reserved
                DCD     am_exc_eint_handler  ; PIO INT0
                DCD     am_exc_eint_handler  ; PIO INT1
                DCD     am_exc_eint_handler  ; PIO INT2
                DCD     am_exc_eint_handler  ; PIO INT3
                DCD     am_exc_eint_handler  ; PIO INT4
                DCD     am_exc_eint_handler  ; PIO INT5
                DCD     am_exc_eint_handler  ; PIO INT6
                DCD     am_exc_eint_handler  ; PIO INT7


;//   <h> Code Read Protection level (CRP)
;//     <o>    CRP_Level:
;//                     <0xFFFFFFFF=> Disabled
;//                     <0x4E697370=> NO_ISP
;//                     <0x12345678=> CRP1
;//                     <0x87654321=> CRP2
;//                     <0x43218765=> CRP3 (Are you sure?)
;//   </h>
CRP_Level       EQU     0xFFFFFFFF

                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF


                AREA    |.text|, CODE, READONLY

; Reset Handler
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
