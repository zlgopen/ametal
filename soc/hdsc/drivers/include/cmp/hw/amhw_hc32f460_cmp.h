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
 * \brief CMP hardware operation interface
 *
 * -note:在对同一寄存器进行读写时其所对应的功能不同
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-05  zcb, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_CMP_H
#define __AMHW_HC32F460_CMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"
#include "am_bitops.h"
#include "hc32f460_regbase.h"
/**
 * \addtogroup amhw_hc32f460_if_cmp
 * \copydoc amhw_hc32f460_cmp.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

/* 默认使能匿名联合体 */
#elif defined(__TMS470__)

/* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

typedef struct
{
    __IO uint16_t FLTSL                     : 3;
    uint16_t RESERVED3                      : 2;
    __IO uint16_t EDGSL                     : 2;
    __IO uint16_t IEN                       : 1;
    __IO uint16_t CVSEN                     : 1;
    uint16_t RESERVED9                      : 3;
    __IO uint16_t OUTEN                     : 1;
    __IO uint16_t INV                       : 1;
    __IO uint16_t CMPOE                     : 1;
    __IO uint16_t CMPON                     : 1;
} stc_cmp_ctrl_field_t;

typedef struct
{
    __IO uint16_t RVSL                      : 4;
    uint16_t RESERVED4                      : 4;
    __IO uint16_t CVSL                      : 4;
    __IO uint16_t C4SL                      : 3;
    uint16_t RESERVED15                     : 1;
} stc_cmp_vltsel_field_t;

typedef struct
{
    __IO uint16_t OMON                      : 1;
    uint16_t RESERVED1                      : 7;
    __IO uint16_t CVST                      : 4;
    uint16_t RESERVED12                     : 4;
} stc_cmp_mon_field_t;

typedef struct
{
    __IO uint16_t STB                       : 4;
    uint16_t RESERVED4                      :12;
} stc_cmp_cvsstb_field_t;

typedef struct
{
    __IO uint16_t PRD                       : 8;
    uint16_t RESERVED8                      : 8;
} stc_cmp_cvsprd_field_t;

typedef struct
{
    __IO uint16_t DATA                      : 8;
    uint16_t RESERVED8                      : 8;
} stc_cmp_cr_dadr1_field_t;

typedef struct
{
    __IO uint16_t DATA                      : 8;
    uint16_t RESERVED8                      : 8;
} stc_cmp_cr_dadr2_field_t;

typedef struct
{
    __IO uint16_t DA1EN                     : 1;
    __IO uint16_t DA2EN                     : 1;
    uint16_t RESERVED2                      :14;
} stc_cmp_cr_dacr_field_t;

typedef struct
{
    __IO uint16_t DA1SW                     : 1;
    __IO uint16_t DA2SW                     : 1;
    uint16_t RESERVED2                      : 2;
    __IO uint16_t VREFSW                    : 1;
    uint16_t RESERVED5                      :11;
} stc_cmp_cr_rvadc_field_t;
 
typedef struct
{
    uint8_t RESERVED0[256];
    union
    {
        __IO uint16_t DADR1;
        stc_cmp_cr_dadr1_field_t DADR1_f;
    };
    union
    {
        __IO uint16_t DADR2;
        stc_cmp_cr_dadr2_field_t DADR2_f;
    };
    uint8_t RESERVED2[4];
    union
    {
        __IO uint16_t DACR;
        stc_cmp_cr_dacr_field_t DACR_f;
    };
    uint8_t RESERVED3[2];
    union
    {
        __IO uint16_t RVADC;
        stc_cmp_cr_rvadc_field_t RVADC_f;
    };
}amhw_hc32f460_cmp_cr_t;

typedef struct
{
    union
    {
        __IO uint16_t CTRL;
        stc_cmp_ctrl_field_t CTRL_f;
    };
    union
    {
        __IO uint16_t VLTSEL;
        stc_cmp_vltsel_field_t VLTSEL_f;
    };
    union
    {
        __IO uint16_t MON;
        stc_cmp_mon_field_t MON_f;
    };
    union
    {
        __IO uint16_t CVSSTB;
        stc_cmp_cvsstb_field_t CVSSTB_f;
    };
    union
    {
        __IO uint16_t CVSPRD;
        stc_cmp_cvsprd_field_t CVSPRD_f;
    };
}amhw_hc32f460_cmp_t;


typedef enum en_cmp_dac_ch
{
    CmpDac1 = 0u,                               ///< DAC1
    CmpDac2 = 1u,                               ///< DAC2
} en_cmp_dac_ch_t;

typedef enum en_cmp_functional_state
{
    CMP_Disable = 0u,
    CMP_Enable  = 1u,
} en_cmp_functional_state_t;

typedef struct stc_cmp_dac_init
{
    uint8_t               u8DacData;            ///< CMP DAC Data register value

    en_cmp_functional_state_t enCmpDacEN;           ///< CMP DAC enable
} stc_cmp_dac_init_t;

/**
 *******************************************************************************
 ** \brief CMP edge selection enumeration
 ******************************************************************************/
typedef enum en_cmp_edge_sel
{
    CmpNoneEdge     = 0u,                       ///< None edge detection
    CmpRisingEdge   = 1u,                       ///< Rising edge detection
    CmpFaillingEdge = 2u,                       ///< Falling edge detection
    CmpBothEdge     = 3u,                       ///< Falling or Rising edge detection
} en_cmp_edge_sel_t;

/**
 *******************************************************************************
 ** \brief CMP filter sample clock division enumeration
 ******************************************************************************/
typedef enum en_cmp_fltclk_div
{
    CmpNoneFlt       = 0u,                      ///< Unuse filter
    CmpFltPclk3Div1  = 1u,                      ///< PCLK3/1
    CmpFltPclk3Div2  = 2u,                      ///< PCLK3/2
    CmpFltPclk3Div4  = 3u,                      ///< PCLK3/4
    CmpFltPclk3Div8  = 4u,                      ///< PCLK3/8
    CmpFltPclk3Div16 = 5u,                      ///< PCLK3/16
    CmpFltPclk3Div32 = 6u,                      ///< PCLK3/32
    CmpFltPclk3Div64 = 7u,                      ///< PCLK3/64
} en_cmp_fltclk_div_t;


typedef struct stc_cmp_init
{
    en_cmp_edge_sel_t     enEdgeSel;            ///< CMP edge sel

    en_cmp_fltclk_div_t   enFltClkDiv;          ///< CMP FLTclock division

    en_cmp_functional_state_t enCmpOutputEn;        ///< CMP Output enable

    en_cmp_functional_state_t enCmpVcoutOutputEn;   ///< CMP output result enable

    en_cmp_functional_state_t enCmpInvEn;           ///< CMP INV sel for output

    en_cmp_functional_state_t enCmpIntEN;           ///< CMP interrupt enable
} stc_cmp_init_t;

typedef enum en_cmp_inm_sel
{
    CmpInmNone = 0u,                            ///< None input
    CmpInm1    = 1u,                            ///< INM1 input
    CmpInm2    = 2u,                            ///< INM2 input
    CmpInm3    = 4u,                            ///< INM3 input
    CmpInm4    = 8u,                            ///< INM4 input
} en_cmp_inm_sel_t;

typedef enum en_cmp_inp_sel
{
    CmpInpNone             = 0u,                ///< None input
    CmpInp1                = 1u,                ///< INP1 input
    CmpInp2                = 2u,                ///< INP2 input
    CmpInp1_Inp2           = 3u,                ///< INP1 INP2 input
    CmpInp3                = 4u,                ///< INP3 input
    CmpInp1_Inp3           = 5u,                ///< INP1 INP3 input
    CmpInp2_Inp3           = 6u,                ///< INP2 INP3 input
    CmpInp1_Inp2_Inp3      = 7u,                ///< INP1 INP2 INP3 input
    CmpInp4                = 8u,                ///< INP4 input
    CmpInp1_Inp4           = 9u,                ///< INP1 INP4 input
    CmpInp2_Inp4           = 10u,               ///< INP2 INP4 input
    CmpInp1_Inp2_Inp4      = 11u,               ///< INP1 INP2 INP4 input
    CmpInp3_Inp4           = 12u,               ///< INP3 INP4 input
    CmpInp1_Inp3_Inp4      = 13u,               ///< INP1 INP3 INP4 input
    CmpInp2_Inp3_Inp4      = 14u,               ///< INP2 INP3 INP4 input
    CmpInp1_Inp2_Inp3_Inp4 = 15u,               ///< INP1 INP2 INP3 INP4 input
} en_cmp_inp_sel_t;

typedef enum en_cmp_inp4_sel
{
    CmpInp4None      = 0u,                      ///< None input
    CmpInp4PGAO      = 1u,                      ///< PGAO output
    CmpInp4PGAO_BP   = 2u,                      ///< PGAO_BP output
    CmpInp4CMP1_INP4 = 4u,                      ///< CMP1_INP4
} en_cmp_inp4_sel_t;

typedef struct stc_cmp_input_sel
{
    en_cmp_inm_sel_t  enInmSel;                 ///< CMP INM sel

    en_cmp_inp_sel_t  enInpSel;                 ///< CMP INP sel

    en_cmp_inp4_sel_t enInp4Sel;                ///< CMP INP4 sel
} stc_cmp_input_sel_t;

am_static_inline
am_bool_t CMP_DAC_Init(amhw_hc32f460_cmp_cr_t *p_hw_cmp_cr, en_cmp_dac_ch_t enCh,
                            const stc_cmp_dac_init_t *pstcInitCfg)
{
    am_bool_t enRet = AM_FALSE;

    if (pstcInitCfg != NULL)
    {        
        p_hw_cmp_cr->DACR &= (uint16_t)(~(1ul << enCh));    /* Disable DAC */

        if (CmpDac1 == enCh) {
            *(__IO uint8_t *)(&p_hw_cmp_cr->DADR1) = pstcInitCfg->u8DacData;
        } else if (CmpDac2 == enCh) {
            *(__IO uint8_t *)(&p_hw_cmp_cr->DADR2) = pstcInitCfg->u8DacData;
        } else {
            ;
        }

//        *(__IO uint8_t *)CMP_CR_DADRx(p_hw_cmp_cr, enCh) = pstcInitCfg->u8DacData; /* Set DAC data */

        if (CMP_Enable == pstcInitCfg->enCmpDacEN)
        {
            p_hw_cmp_cr->DACR |= (uint16_t)(1ul << enCh); /* Enable DAC */
        }
        enRet = AM_OK;
    }

    return enRet;
}

am_static_inline
am_bool_t CMP_Init(amhw_hc32f460_cmp_t *p_hw_cmp, const stc_cmp_init_t *pstcInitCfg)
{
    am_bool_t enRet = AM_FALSE;

    /* Check CMPx && pstcInitCfg pointer */
    if (NULL != pstcInitCfg)
    {
        /* De-Initialize CMP */
        p_hw_cmp->CTRL = (uint16_t)0x0000u;
        p_hw_cmp->VLTSEL = (uint16_t)0x0000u;
        p_hw_cmp->CVSSTB = (uint16_t)0x0005u;
        p_hw_cmp->CVSPRD = (uint16_t)0x000Fu;

        p_hw_cmp->CTRL_f.IEN = (uint16_t)pstcInitCfg->enCmpIntEN;
        p_hw_cmp->CTRL_f.INV = (uint16_t)pstcInitCfg->enCmpInvEn;
        p_hw_cmp->CTRL_f.EDGSL = (uint16_t)pstcInitCfg->enEdgeSel;
        p_hw_cmp->CTRL_f.FLTSL = (uint16_t)pstcInitCfg->enFltClkDiv;
        p_hw_cmp->CTRL_f.CMPOE = (uint16_t)pstcInitCfg->enCmpOutputEn;
        p_hw_cmp->CTRL_f.OUTEN = (uint16_t)pstcInitCfg->enCmpVcoutOutputEn;
        enRet = AM_OK;
    }

    return enRet;
}

am_static_inline
am_bool_t CMP_InputSel(amhw_hc32f460_cmp_t *p_hw_cmp,
                            const stc_cmp_input_sel_t *pstcInputSel)
{
    am_bool_t enRet = AM_FALSE;

    /* Check CMPx && pstcInputSel pointer */
    if (NULL != pstcInputSel)
    {
        if ((CmpInp4PGAO == pstcInputSel->enInp4Sel) ||
            (CmpInp4PGAO_BP == pstcInputSel->enInp4Sel))
        {
            if ((amhw_hc32f460_cmp_t *)HC32F460_CMP3_BASE != p_hw_cmp)
            {
                enRet = AM_OK;
            }
        }
        else if (CmpInp4CMP1_INP4 == pstcInputSel->enInp4Sel)
        {
            if ((amhw_hc32f460_cmp_t *)HC32F460_CMP1_BASE == p_hw_cmp)
            {
                enRet = AM_OK;
            }
        }
        else
        {
            enRet = AM_OK;
        }

        if (enRet == AM_OK)
        {
            p_hw_cmp->VLTSEL_f.CVSL = (uint16_t)pstcInputSel->enInpSel;
            p_hw_cmp->VLTSEL_f.RVSL = (uint16_t)pstcInputSel->enInmSel;
            p_hw_cmp->VLTSEL_f.C4SL = (uint16_t)pstcInputSel->enInp4Sel;
        }
    }

    return enRet;
}

am_static_inline
am_bool_t CMP_Cmd(amhw_hc32f460_cmp_t *p_hw_cmp, en_cmp_functional_state_t enCmd)
{
    am_bool_t enRet = AM_FALSE;

    /* Check CMPx pointer */
    if (NULL != p_hw_cmp)
    {
        p_hw_cmp->CTRL_f.CMPON = (uint16_t)(enCmd);
        enRet = AM_OK;
    }

    return enRet;
}


/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

/* 允许匿名联合体使能 */
#elif defined(__GNUC__)

/* 默认使用匿名联合体 */
#elif defined(__TMS470__)

/* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

/**
 * @} amhw_hc32f460_if_cmp
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_CMP_H */

/* end of file */
