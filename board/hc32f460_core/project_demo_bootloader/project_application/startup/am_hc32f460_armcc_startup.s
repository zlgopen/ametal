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
; * \brief ZLG startup code for ARMCC compiler
; * 
; * \internal
; * \par Modification History
; * - 1.00 15-06-25  jon, first implementation
; * \endinternal
; */

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
Vector_App EQU  0x00000000
Stack_Size      EQU     0x00000600

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000300

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
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     am_exc_eint_handler        ; IRQ000_Handler
                DCD     am_exc_eint_handler        ; IRQ001_Handler
                DCD     am_exc_eint_handler        ; IRQ002_Handler
                DCD     am_exc_eint_handler        ; IRQ003_Handler
                DCD     am_exc_eint_handler        ; IRQ004_Handler
                DCD     am_exc_eint_handler        ; IRQ005_Handler
                DCD     am_exc_eint_handler        ; IRQ006_Handler
                DCD     am_exc_eint_handler        ; IRQ007_Handler
                DCD     am_exc_eint_handler        ; IRQ008_Handler
                DCD     am_exc_eint_handler        ; IRQ009_Handler
                DCD     am_exc_eint_handler        ; IRQ010_Handler
                DCD     am_exc_eint_handler        ; IRQ011_Handler
                DCD     am_exc_eint_handler        ; IRQ012_Handler
                DCD     am_exc_eint_handler        ; IRQ013_Handler
                DCD     am_exc_eint_handler        ; IRQ014_Handler
                DCD     am_exc_eint_handler        ; IRQ015_Handler
                DCD     am_exc_eint_handler        ; IRQ016_Handler
                DCD     am_exc_eint_handler        ; IRQ017_Handler
                DCD     am_exc_eint_handler        ; IRQ018_Handler
                DCD     am_exc_eint_handler        ; IRQ019_Handler
                DCD     am_exc_eint_handler        ; IRQ020_Handler
                DCD     am_exc_eint_handler        ; IRQ021_Handler
                DCD     am_exc_eint_handler        ; IRQ022_Handler
                DCD     am_exc_eint_handler        ; IRQ023_Handler
                DCD     am_exc_eint_handler        ; IRQ024_Handler
                DCD     am_exc_eint_handler        ; IRQ025_Handler
                DCD     am_exc_eint_handler        ; IRQ026_Handler
                DCD     am_exc_eint_handler        ; IRQ027_Handler
                DCD     am_exc_eint_handler        ; IRQ028_Handler
                DCD     am_exc_eint_handler        ; IRQ029_Handler
                DCD     am_exc_eint_handler        ; IRQ030_Handler
                DCD     am_exc_eint_handler        ; IRQ031_Handler
                DCD     am_exc_eint_handler        ; IRQ032_Handler
                DCD     am_exc_eint_handler        ; IRQ033_Handler
                DCD     am_exc_eint_handler        ; IRQ034_Handler
                DCD     am_exc_eint_handler        ; IRQ035_Handler
                DCD     am_exc_eint_handler        ; IRQ036_Handler
                DCD     am_exc_eint_handler        ; IRQ037_Handler
                DCD     am_exc_eint_handler        ; IRQ038_Handler
                DCD     am_exc_eint_handler        ; IRQ039_Handler
                DCD     am_exc_eint_handler        ; IRQ040_Handler
                DCD     am_exc_eint_handler        ; IRQ041_Handler
                DCD     am_exc_eint_handler        ; IRQ042_Handler
                DCD     am_exc_eint_handler        ; IRQ043_Handler
                DCD     am_exc_eint_handler        ; IRQ044_Handler
                DCD     am_exc_eint_handler        ; IRQ045_Handler
                DCD     am_exc_eint_handler        ; IRQ046_Handler
                DCD     am_exc_eint_handler        ; IRQ047_Handler
                DCD     am_exc_eint_handler        ; IRQ048_Handler
                DCD     am_exc_eint_handler        ; IRQ049_Handler
                DCD     am_exc_eint_handler        ; IRQ050_Handler
                DCD     am_exc_eint_handler        ; IRQ051_Handler
                DCD     am_exc_eint_handler        ; IRQ052_Handler
                DCD     am_exc_eint_handler        ; IRQ053_Handler
                DCD     am_exc_eint_handler        ; IRQ054_Handler
                DCD     am_exc_eint_handler        ; IRQ055_Handler
                DCD     am_exc_eint_handler        ; IRQ056_Handler
                DCD     am_exc_eint_handler        ; IRQ057_Handler
                DCD     am_exc_eint_handler        ; IRQ058_Handler
                DCD     am_exc_eint_handler        ; IRQ059_Handler
                DCD     am_exc_eint_handler        ; IRQ060_Handler
                DCD     am_exc_eint_handler        ; IRQ061_Handler
                DCD     am_exc_eint_handler        ; IRQ062_Handler
                DCD     am_exc_eint_handler        ; IRQ063_Handler
                DCD     am_exc_eint_handler        ; IRQ064_Handler
                DCD     am_exc_eint_handler        ; IRQ065_Handler
                DCD     am_exc_eint_handler        ; IRQ066_Handler
                DCD     am_exc_eint_handler        ; IRQ067_Handler
                DCD     am_exc_eint_handler        ; IRQ068_Handler
                DCD     am_exc_eint_handler        ; IRQ069_Handler
                DCD     am_exc_eint_handler        ; IRQ070_Handler
                DCD     am_exc_eint_handler        ; IRQ071_Handler
                DCD     am_exc_eint_handler        ; IRQ072_Handler
                DCD     am_exc_eint_handler        ; IRQ073_Handler
                DCD     am_exc_eint_handler        ; IRQ074_Handler
                DCD     am_exc_eint_handler        ; IRQ075_Handler
                DCD     am_exc_eint_handler        ; IRQ076_Handler
                DCD     am_exc_eint_handler        ; IRQ077_Handler
                DCD     am_exc_eint_handler        ; IRQ078_Handler
                DCD     am_exc_eint_handler        ; IRQ079_Handler
                DCD     am_exc_eint_handler        ; IRQ080_Handler
                DCD     am_exc_eint_handler        ; IRQ081_Handler
                DCD     am_exc_eint_handler        ; IRQ082_Handler
                DCD     am_exc_eint_handler        ; IRQ083_Handler
                DCD     am_exc_eint_handler        ; IRQ084_Handler
                DCD     am_exc_eint_handler        ; IRQ085_Handler
                DCD     am_exc_eint_handler        ; IRQ086_Handler
                DCD     am_exc_eint_handler        ; IRQ087_Handler
                DCD     am_exc_eint_handler        ; IRQ088_Handler
                DCD     am_exc_eint_handler        ; IRQ089_Handler
                DCD     am_exc_eint_handler        ; IRQ090_Handler
                DCD     am_exc_eint_handler        ; IRQ091_Handler
                DCD     am_exc_eint_handler        ; IRQ092_Handler
                DCD     am_exc_eint_handler        ; IRQ093_Handler
                DCD     am_exc_eint_handler        ; IRQ094_Handler
                DCD     am_exc_eint_handler        ; IRQ095_Handler
                DCD     am_exc_eint_handler        ; IRQ096_Handler
                DCD     am_exc_eint_handler        ; IRQ097_Handler
                DCD     am_exc_eint_handler        ; IRQ098_Handler
                DCD     am_exc_eint_handler        ; IRQ099_Handler
                DCD     am_exc_eint_handler        ; IRQ100_Handler
                DCD     am_exc_eint_handler        ; IRQ101_Handler
                DCD     am_exc_eint_handler        ; IRQ102_Handler
                DCD     am_exc_eint_handler        ; IRQ103_Handler
                DCD     am_exc_eint_handler        ; IRQ104_Handler
                DCD     am_exc_eint_handler        ; IRQ105_Handler
                DCD     am_exc_eint_handler        ; IRQ106_Handler
                DCD     am_exc_eint_handler        ; IRQ107_Handler
                DCD     am_exc_eint_handler        ; IRQ108_Handler
                DCD     am_exc_eint_handler        ; IRQ109_Handler
                DCD     am_exc_eint_handler        ; IRQ110_Handler
                DCD     am_exc_eint_handler        ; IRQ111_Handler
                DCD     am_exc_eint_handler        ; IRQ112_Handler
                DCD     am_exc_eint_handler        ; IRQ113_Handler
                DCD     am_exc_eint_handler        ; IRQ114_Handler
                DCD     am_exc_eint_handler        ; IRQ115_Handler
                DCD     am_exc_eint_handler        ; IRQ116_Handler
                DCD     am_exc_eint_handler        ; IRQ117_Handler
                DCD     am_exc_eint_handler        ; IRQ118_Handler
                DCD     am_exc_eint_handler        ; IRQ119_Handler
                DCD     am_exc_eint_handler        ; IRQ120_Handler
                DCD     am_exc_eint_handler        ; IRQ121_Handler
                DCD     am_exc_eint_handler        ; IRQ122_Handler
                DCD     am_exc_eint_handler        ; IRQ123_Handler
                DCD     am_exc_eint_handler        ; IRQ124_Handler
                DCD     am_exc_eint_handler        ; IRQ125_Handler
                DCD     am_exc_eint_handler        ; IRQ126_Handler
                DCD     am_exc_eint_handler        ; IRQ127_Handler
                DCD     am_exc_eint_handler        ; IRQ128_Handler
                DCD     am_exc_eint_handler        ; IRQ129_Handler
                DCD     am_exc_eint_handler        ; IRQ130_Handler
                DCD     am_exc_eint_handler        ; IRQ131_Handler
                DCD     am_exc_eint_handler        ; IRQ132_Handler
                DCD     am_exc_eint_handler        ; IRQ133_Handler
                DCD     am_exc_eint_handler        ; IRQ134_Handler
                DCD     am_exc_eint_handler        ; IRQ135_Handler
                DCD     am_exc_eint_handler        ; IRQ136_Handler
                DCD     am_exc_eint_handler        ; IRQ137_Handler
                DCD     am_exc_eint_handler        ; IRQ138_Handler
                DCD     am_exc_eint_handler        ; IRQ139_Handler
                DCD     am_exc_eint_handler        ; IRQ140_Handler
                DCD     am_exc_eint_handler        ; IRQ141_Handler
                DCD     am_exc_eint_handler        ; IRQ142_Handler
                DCD     am_exc_eint_handler        ; IRQ143_Handler
                    
                    
                AREA    |.text|, CODE, READONLY

; Reset Handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                    
                
                
SET_SRAM3_WAIT
                LDR     R0, =0x40050804
                MOV     R1, #0x77
                STR     R1, [R0]

                LDR     R0, =0x4005080C
                MOV     R1, #0x77
                STR     R1, [R0]

                LDR     R0, =0x40050800
                MOV     R1, #0x1100
                STR     R1, [R0]

                LDR     R0, =0x40050804
                MOV     R1, #0x76
                STR     R1, [R0]

                LDR     R0, =0x4005080C
                MOV     R1, #0x76
                STR     R1, [R0]
				
				LDR     R0, =0xE000ED88
                MOV     R1, #0x00F00000
                STR     R1, [R0]
                
                
                
                
                ;reset NVIC if in rom debug
;                LDR     R0, =0x20000000
;                LDR     R2, =Vector_App
;                MOVS    R1, #0                 ; for warning,
;                ADD     R1, PC,#0              ; for A1609W,
;                CMP     R1, R0
;                BLS     RAMCODE

              ; ram code base address.
                ADD     R2, R0,R2
                CPSID   I
                LDR     R0, =0xE000ED08	;
                LDR     R1, =__Vectors
                STR     R1, [R0]
                LDR     R2, [R1]
                MSR     MSP, R2
                CPSIE   I
;RAMCODE
;                LDR     R0, =0xE000ED08
;                STR     R2, [R0]
                LDR     R0, =SystemInit
                BLX     R0

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
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
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
