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
*****************************************************************************
*  History:
*
*  09.04.11  mifi   First Version
****************************************************************************/


/*
 * In this linker script there is no heap available.
 * The stack start at the end of the ram segment.
 */
STACK_SIZE = 0x2000;             /*  stack size config   8k        */

/*
 * Take a look in the "The GNU linker" manual, here you get
 * the following information about the "MEMORY":
 *
 * "The MEMORY command describes the location and size of 
 * blocks of memory in the target."
 */
MEMORY
{
   FLASH_INT     (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00000100
   FLASH_CONFIG  (rx)  : ORIGIN = 0x00000400, LENGTH = 0x00000010
   FLASH_TEXT    (rx)  : ORIGIN = 0x00000410, LENGTH = 0x0001F7F0
   RAM           (rwx) : ORIGIN = 0x1FFFF000, LENGTH = 16K
}

ENTRY(ResetHandler)
/*
 * And the "SECTION" is used for:
 *
 * "The SECTIONS command tells the linker how to map input
 * sections into output sections, and how to place the output
 * sections in memory.
 */
SECTIONS
{
   /* The startup code goes first into internal flash */
    .interrupts :
    {
      __VECTOR_TABLE = .;
      . = ALIGN(4);
      KEEP(*(.vectors))         /* Startup code */
      . = ALIGN(4);
    } > FLASH_INT

    .flash_config :
    {
      . = ALIGN(4);
      KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
      . = ALIGN(4);
    } > FLASH_CONFIG

     .text :
    {
        _stext = .;           /* Provide the name for the start of this section */
  
        *(.text)
        *(.text.*)              /*  cpp namespace function      */
        *(.romrun)              /*  rom中必须的函数             */
        
        . = ALIGN(4);           /* Align the start of the rodata part */
        *(.rodata)              /*  read-only data (constants)  */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
    } > FLASH_TEXT

    /* section information for simple shell symbols */
    .text :
    {
        . = ALIGN(4);
        __shellsym_tab_start = .;
        KEEP(*(.shellsymbol))
        __shellsym_tab_end = .;
    } >FLASH_TEXT

    /* .ARM.exidx is sorted, so has to go in its own output section */
    . = ALIGN(4);
     __exidx_start = .;
     PROVIDE(__exidx_start = __exidx_start);
    .ARM.exidx :
    {
        /* __exidx_start = .; */
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        /* __exidx_end = .;   */
    } > FLASH_TEXT
    . = ALIGN(4);
     __exidx_end = .;
     PROVIDE(__exidx_end = __exidx_end);

    /*
     * C++ 全局对象构造与析构函数表
     * 这里放在 .text 和 .ARM.exidx 之后, .data 之前,
     * 这里的  LMA 和 VMA 相同, 如果放在 .data 之后, LMA 与 VMA 不同,
     * 则需要启动程序从装载区搬运到运行区
     */

    . = ALIGN(4);
    .ctors :
    {
        KEEP (*cppRtBegin*.o(.ctors))
        KEEP (*(.preinit_array))
        KEEP (*(.init_array))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        KEEP (*cppRtEnd*.o(.ctors))
    }

    .dtors :
    {
        KEEP (*cppRtBegin*.o(.dtors))
        KEEP (*(.fini_array))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        KEEP (*cppRtEnd*.o(.dtors))
    }

    /* .data 段数据初始化内容放在这里 */
    . = ALIGN(16);
    _etext = . ;
    PROVIDE (etext = .);
    
   /*
    * The ".data" section is used for initialized data
    * and for functions (.fastrun) which should be copied 
    * from flash to ram. This functions will later be
    * executed from ram instead of flash.
    */
   .data : AT (_etext)
   {
      . = ALIGN(4);        /* Align the start of the section */
      _sdata = .;          /* Provide the name for the start of this section */
      
      *(.data)
      *(.data.*)
      
      . = ALIGN(4);        /* Align the start of the fastrun part */
      *(.fastrun)
      *(.fastrun.*)
      
      . = ALIGN(4);        /* Align the end of the section */
   } > RAM
   _edata = .;             /* Provide the name for the end of this section */
   
   USB_RAM_GAP = DEFINED(__usb_ram_size__) ? __usb_ram_size__ : 0x800;
   /*
    * The ".bss" section is used for uninitialized data.
    * This section will be cleared by the startup code.
    */
   .bss :
   {
      . = ALIGN(4);        /* Align the start of the section */
      _sbss = .;           /* Provide the name for the start of this section */
      
      *(.bss)
      *(.bss.*)
      . = ALIGN(512);
      USB_RAM_START = .;
    . += USB_RAM_GAP;
      
      . = ALIGN(4);        /* Align the end of the section */
   } > RAM
   _ebss = .;              /* Provide the name for the end of this section */
   
    /* 系统堆 */
    . = ALIGN(4);
    PROVIDE (__heap_start__ = .);
    .heap (NOLOAD) :
    {

    } > RAM
    . = ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE;
    . = ALIGN(4);
    PROVIDE (__heap_end__ = .);
   
   /* 
    * The ".stack" section is our stack.
    * Here this section starts at the end of the ram segment.
    */
   _estack = ORIGIN(RAM) + LENGTH(RAM);

   m_usb_bdt USB_RAM_START (NOLOAD) :
   {
     *(m_usb_bdt)
     USB_RAM_BDT_END = .;
   }

   m_usb_global USB_RAM_BDT_END (NOLOAD) :
   {
     *(m_usb_global)
   }
}

/*** EOF **/

