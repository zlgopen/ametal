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
 * - 1.00 19-12-18  cds, first implementation
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
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    am_exc_eint_handler,  /* IRQ000_Handler */
    am_exc_eint_handler,  /* IRQ001_Handler */
    am_exc_eint_handler,  /* IRQ002_Handler */
    am_exc_eint_handler,  /* IRQ003_Handler */
    am_exc_eint_handler,  /* IRQ004_Handler */
    am_exc_eint_handler,  /* IRQ005_Handler */
    am_exc_eint_handler,  /* IRQ006_Handler */
    am_exc_eint_handler,  /* IRQ007_Handler */
    am_exc_eint_handler,  /* IRQ008_Handler */
    am_exc_eint_handler,  /* IRQ009_Handler */
    am_exc_eint_handler,  /* IRQ010_Handler */
    am_exc_eint_handler,  /* IRQ011_Handler */
    am_exc_eint_handler,  /* IRQ012_Handler */
    am_exc_eint_handler,  /* IRQ013_Handler */
    am_exc_eint_handler,  /* IRQ014_Handler */
    am_exc_eint_handler,  /* IRQ015_Handler */
    am_exc_eint_handler,  /* IRQ016_Handler */
    am_exc_eint_handler,  /* IRQ017_Handler */
    am_exc_eint_handler,  /* IRQ018_Handler */
    am_exc_eint_handler,  /* IRQ019_Handler */
    am_exc_eint_handler,  /* IRQ020_Handler */
    am_exc_eint_handler,  /* IRQ021_Handler */
    am_exc_eint_handler,  /* IRQ022_Handler */
    am_exc_eint_handler,  /* IRQ023_Handler */
    am_exc_eint_handler,  /* IRQ024_Handler */
    am_exc_eint_handler,  /* IRQ025_Handler */
    am_exc_eint_handler,  /* IRQ026_Handler */
    am_exc_eint_handler,  /* IRQ027_Handler */
    am_exc_eint_handler,  /* IRQ028_Handler */
    am_exc_eint_handler,  /* IRQ029_Handler */
    am_exc_eint_handler,  /* IRQ030_Handler */
    am_exc_eint_handler,  /* IRQ031_Handler */
    am_exc_eint_handler,  /* IRQ032_Handler */
    am_exc_eint_handler,  /* IRQ033_Handler */

    am_exc_eint_handler,  /* IRQ034_Handler */
    am_exc_eint_handler,  /* IRQ035_Handler */
    am_exc_eint_handler,  /* IRQ036_Handler */
    am_exc_eint_handler,  /* IRQ037_Handler */
    am_exc_eint_handler,  /* IRQ038_Handler */
    am_exc_eint_handler,  /* IRQ039_Handler */
    am_exc_eint_handler,  /* IRQ040_Handler */
    am_exc_eint_handler,  /* IRQ041_Handler */
    am_exc_eint_handler,  /* IRQ042_Handler */
    am_exc_eint_handler,  /* IRQ043_Handler */

    am_exc_eint_handler,  /* IRQ044_Handler */
    am_exc_eint_handler,  /* IRQ045_Handler */
    am_exc_eint_handler,  /* IRQ046_Handler */
    am_exc_eint_handler,  /* IRQ047_Handler */
    am_exc_eint_handler,  /* IRQ048_Handler */
    am_exc_eint_handler,  /* IRQ049_Handler */
    am_exc_eint_handler,  /* IRQ050_Handler */
    am_exc_eint_handler,  /* IRQ051_Handler */
    am_exc_eint_handler,  /* IRQ052_Handler */
    am_exc_eint_handler,  /* IRQ053_Handler */

    am_exc_eint_handler,  /* IRQ054_Handler */
    am_exc_eint_handler,  /* IRQ055_Handler */
    am_exc_eint_handler,  /* IRQ056_Handler */
    am_exc_eint_handler,  /* IRQ057_Handler */
    am_exc_eint_handler,  /* IRQ058_Handler */
    am_exc_eint_handler,  /* IRQ059_Handler */
    am_exc_eint_handler,  /* IRQ060_Handler */
    am_exc_eint_handler,  /* IRQ061_Handler */
    am_exc_eint_handler,  /* IRQ062_Handler */
    am_exc_eint_handler,  /* IRQ063_Handler */

    am_exc_eint_handler,  /* IRQ064_Handler */
    am_exc_eint_handler,  /* IRQ065_Handler */
    am_exc_eint_handler,  /* IRQ066_Handler */
    am_exc_eint_handler,  /* IRQ067_Handler */
    am_exc_eint_handler,  /* IRQ068_Handler */
    am_exc_eint_handler,  /* IRQ069_Handler */
    am_exc_eint_handler,  /* IRQ070_Handler */
    am_exc_eint_handler,  /* IRQ071_Handler */
    am_exc_eint_handler,  /* IRQ072_Handler */
    am_exc_eint_handler,  /* IRQ073_Handler */

    am_exc_eint_handler,  /* IRQ074_Handler */
    am_exc_eint_handler,  /* IRQ075_Handler */
    am_exc_eint_handler,  /* IRQ076_Handler */
    am_exc_eint_handler,  /* IRQ077_Handler */
    am_exc_eint_handler,  /* IRQ078_Handler */
    am_exc_eint_handler,  /* IRQ079_Handler */
    am_exc_eint_handler,  /* IRQ080_Handler */
    am_exc_eint_handler,  /* IRQ081_Handler */
    am_exc_eint_handler,  /* IRQ082_Handler */
    am_exc_eint_handler,  /* IRQ083_Handler */

    am_exc_eint_handler,  /* IRQ084_Handler */
    am_exc_eint_handler,  /* IRQ085_Handler */
    am_exc_eint_handler,  /* IRQ086_Handler */
    am_exc_eint_handler,  /* IRQ087_Handler */
    am_exc_eint_handler,  /* IRQ088_Handler */
    am_exc_eint_handler,  /* IRQ089_Handler */
    am_exc_eint_handler,  /* IRQ090_Handler */
    am_exc_eint_handler,  /* IRQ091_Handler */
    am_exc_eint_handler,  /* IRQ092_Handler */
    am_exc_eint_handler,  /* IRQ093_Handler */

    am_exc_eint_handler,  /* IRQ094_Handler */
    am_exc_eint_handler,  /* IRQ095_Handler */
    am_exc_eint_handler,  /* IRQ096_Handler */
    am_exc_eint_handler,  /* IRQ097_Handler */
    am_exc_eint_handler,  /* IRQ098_Handler */
    am_exc_eint_handler,  /* IRQ099_Handler */
    am_exc_eint_handler,  /* IRQ100_Handler */
    am_exc_eint_handler,  /* IRQ101_Handler */
    am_exc_eint_handler,  /* IRQ102_Handler */
    am_exc_eint_handler,  /* IRQ103_Handler */

    am_exc_eint_handler,  /* IRQ104_Handler */
    am_exc_eint_handler,  /* IRQ105_Handler */
    am_exc_eint_handler,  /* IRQ106_Handler */
    am_exc_eint_handler,  /* IRQ107_Handler */
    am_exc_eint_handler,  /* IRQ108_Handler */
    am_exc_eint_handler,  /* IRQ109_Handler */
    am_exc_eint_handler,  /* IRQ110_Handler */
    am_exc_eint_handler,  /* IRQ111_Handler */
    am_exc_eint_handler,  /* IRQ112_Handler */
    am_exc_eint_handler,  /* IRQ113_Handler */

    am_exc_eint_handler,  /* IRQ114_Handler */
    am_exc_eint_handler,  /* IRQ115_Handler */
    am_exc_eint_handler,  /* IRQ116_Handler */
    am_exc_eint_handler,  /* IRQ117_Handler */
    am_exc_eint_handler,  /* IRQ118_Handler */
    am_exc_eint_handler,  /* IRQ119_Handler */
    am_exc_eint_handler,  /* IRQ120_Handler */
    am_exc_eint_handler,  /* IRQ121_Handler */
    am_exc_eint_handler,  /* IRQ122_Handler */
    am_exc_eint_handler,  /* IRQ123_Handler */

    am_exc_eint_handler,  /* IRQ124_Handler */
    am_exc_eint_handler,  /* IRQ125_Handler */
    am_exc_eint_handler,  /* IRQ126_Handler */
    am_exc_eint_handler,  /* IRQ127_Handler */
    am_exc_eint_handler,  /* IRQ128_Handler */
    am_exc_eint_handler,  /* IRQ129_Handler */
    am_exc_eint_handler,  /* IRQ130_Handler */
    am_exc_eint_handler,  /* IRQ131_Handler */
    am_exc_eint_handler,  /* IRQ132_Handler */
    am_exc_eint_handler,  /* IRQ133_Handler */

    am_exc_eint_handler,  /* IRQ134_Handler */
    am_exc_eint_handler,  /* IRQ135_Handler */
    am_exc_eint_handler,  /* IRQ136_Handler */
    am_exc_eint_handler,  /* IRQ137_Handler */
    am_exc_eint_handler,  /* IRQ138_Handler */
    am_exc_eint_handler,  /* IRQ139_Handler */
    am_exc_eint_handler,  /* IRQ140_Handler */
    am_exc_eint_handler,  /* IRQ141_Handler */
    am_exc_eint_handler,  /* IRQ142_Handler */
    am_exc_eint_handler,  /* IRQ143_Handler */


};

/***************************************************************************
  本地函数定义
***************************************************************************/
void NMI_Handler (void) { while (1); }
void HardFault_Handler (void) { while (1); }
void MemManage_Handler (void) { while (1); }
void BusFault_Handler (void) { while (1); }
void UsageFault_Handler (void) { while (1); }
void SVC_Handler (void) { while (1); }
void DebugMon_Handler (void) { while (1); }
void PendSV_Handler (void) { while (1); }
void SysTick_Handler (void) { while (1); }

void am_exc_eint_handler (void) { while (1); }

/* end of file */
