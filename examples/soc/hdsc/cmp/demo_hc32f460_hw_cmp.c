/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief CMP例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *    1.J25通过跳冒连接，断开J24跳冒并将J24-VRO与J6-29连接，J8-52与示波器连接；
 *    2.打开工程编译并全速运行；
 *    3.调节可调电阻R138；
 *    4.观察LED1(绿色，D26)与示波器：
 *       a.J8-52电平为高时，LED1点亮，表示INP电压高于INM；
 *       b.J8-52电平为低时，LED1熄灭，表示INP电压低于INM；

 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_cmp.c src_hc32f460_hw_cmp
 *
 * \internal
 * \par Modification history
 * - 1.00 
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_cmp
 * \copydoc demo_hc32f460_hw_cmp.c
 */

/** [src_hc32f460_hw_cmp] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_hc32f460_cmp.h"
#include "am_int.h"
#include "hc32f460_clk.h"
#include "am_hc32f460_clk.h"
#include "am_board.h"
#include "string.h"
#include "hc32f460_irq_handle.h"
#include "am_int.h"


#define     INT_ACMP1               416u
#define     INT_ACMP2               417u
#define     INT_ACMP3               418u

/*******************************************************************************
  宏定义
*******************************************************************************/
static void cmp_callback_function(void)
{
    am_led_toggle(LED0);
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_cmp_entry (amhw_hc32f460_cmp_t *p_hw_cmp)
{
    stc_cmp_dac_init_t stc_dac_init_cfg;    
    stc_cmp_init_t stc_cmp_config;    
    stc_cmp_input_sel_t stc_cmp_input;    
    stc_intc_sel_field_t *stc_int_sel;
    memset(&stc_dac_init_cfg, 0, sizeof(stc_dac_init_cfg));
    memset(&stc_cmp_config, 0, sizeof(stc_cmp_config));    
    memset(&stc_cmp_config, 0, sizeof(stc_cmp_input));
    
    /* Set DAC */
    stc_dac_init_cfg.u8DacData = 0x80u;
    stc_dac_init_cfg.enCmpDacEN = CMP_Enable;
    CMP_DAC_Init((amhw_hc32f460_cmp_cr_t *)HC32F460_CMP_CR_BASE, CmpDac1, &stc_dac_init_cfg);
    CMP_DAC_Init((amhw_hc32f460_cmp_cr_t *)HC32F460_CMP_CR_BASE, CmpDac2, &stc_dac_init_cfg);

    /* Set CMP mode */
    stc_cmp_config.enCmpIntEN = CMP_Enable;         /* Interrupt enable */
    stc_cmp_config.enCmpInvEn = CMP_Disable;
    stc_cmp_config.enCmpOutputEn =  CMP_Enable;
    stc_cmp_config.enCmpVcoutOutputEn =  CMP_Enable; /* Out enable */
    stc_cmp_config.enEdgeSel = CmpBothEdge;
    stc_cmp_config.enFltClkDiv = CmpFltPclk3Div64;
    CMP_Init(p_hw_cmp, &stc_cmp_config);

    /* Set CMP input */
    stc_cmp_input.enInpSel = CmpInp1;
    stc_cmp_input.enInmSel = CmpInm3;
    CMP_InputSel(p_hw_cmp, &stc_cmp_input);    

    /* Registration IRQ : CMP */        
    stc_int_sel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * 2));
    stc_int_sel->INTSEL = INT_ACMP2;
    am_int_connect(2, (am_pfnvoid_t)cmp_callback_function, NULL);
    am_int_enable(2);

    /* Enable CMP */
    CMP_Cmd(p_hw_cmp, CMP_Enable);    
    
    while(1)
    {
    }    
}
/** [src_hc32f460_hw_cmp] */

/* end of file */
