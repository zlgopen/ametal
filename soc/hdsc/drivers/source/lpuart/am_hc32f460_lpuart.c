#include "am_hc32f460_lpuart.h"

/**
 *******************************************************************************
 ** \brief Initialize UART mode of the specified USART.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] pstcInitCfg             Pointer to USART mode configure structure
 ** \arg This parameter detail refer @ref stc_usart_uart_init_t
 **
 ** \retval Ok                          USART is initialized normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t USART_UART_Init(M4_USART_TypeDef *USARTx,
                                const stc_usart_uart_init_t *pstcInitCfg)
{
    stc_usart_pr_field_t PR_f = {0};
    stc_usart_cr1_field_t CR1_f = {0};
    stc_usart_cr2_field_t CR2_f = {0};
    stc_usart_cr3_field_t CR3_f = {0};
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx && pstcInitCfg pointer */
//    if ((IS_VALID_USART(USARTx)) && (NULL != pstcInitCfg))
//    {
//        /* Check parameters */
//        DDL_ASSERT(IS_VALID_USART_CLK_DIV(pstcInitCfg->enClkDiv));
//        DDL_ASSERT(IS_VALID_USART_CLK_MODE(pstcInitCfg->enClkMode));
//        DDL_ASSERT(IS_VALID_USART_STOP_BIT(pstcInitCfg->enStopBit));
//        DDL_ASSERT(IS_VALID_USART_PARITY_BIT(pstcInitCfg->enParity));
//        DDL_ASSERT(IS_VALID_USART_DATA_DIR(pstcInitCfg->enDirection));
//        DDL_ASSERT(IS_VALID_USART_DATA_LEN(pstcInitCfg->enDataLength));
//        DDL_ASSERT(IS_VALID_USART_HW_FLOW_MODE(pstcInitCfg->enHwFlow));
//        DDL_ASSERT(IS_VALID_USART_SAMPLE_MODE(pstcInitCfg->enSampleMode));
//        DDL_ASSERT(IS_VALID_USART_SB_DETECT_MODE(pstcInitCfg->enDetectMode));

        /* Set default value */
        USARTx->CR1 = (uint32_t)0xFFFFFFF3ul;
        USARTx->CR1 = (uint32_t)0x80000000ul;
        USARTx->CR2 = (uint32_t)0x00000000ul;
        USARTx->CR3 = (uint32_t)0x00000000ul;
        USARTx->BRR = (uint32_t)0x0000FFFFul;
        USARTx->PR = (uint32_t)0x00000000ul;

        /* Set USART mode */
        CR3_f.SCEN = (uint32_t)0ul;
        CR1_f.MS = (uint32_t)0ul;

        PR_f.PSC = (uint32_t)(pstcInitCfg->enClkDiv);
        CR1_f.M = (uint32_t)(pstcInitCfg->enDataLength);
        CR1_f.ML = (uint32_t)(pstcInitCfg->enDirection);
        CR2_f.STOP = (uint32_t)(pstcInitCfg->enStopBit);
        CR2_f.CLKC = (uint32_t)(pstcInitCfg->enClkMode);

        switch(pstcInitCfg->enParity)
        {
            case UsartParityNone:
                CR1_f.PCE = (uint32_t)0ul;
                break;
            case UsartParityEven:
                CR1_f.PS = (uint32_t)0ul;
                CR1_f.PCE = (uint32_t)1ul;
                break;
            case UsartParityOdd:
                CR1_f.PS = (uint32_t)1ul;
                CR1_f.PCE = (uint32_t)1ul;
                break;
            default:
                break;
        }

        CR3_f.CTSE = (uint32_t)(pstcInitCfg->enHwFlow);
        CR1_f.SBS = (uint32_t)(pstcInitCfg->enDetectMode);
        CR1_f.OVER8 = (uint32_t)(pstcInitCfg->enSampleMode);

        USARTx->PR_f = PR_f;
        USARTx->CR1_f= CR1_f;
        USARTx->CR2_f= CR2_f;
        USARTx->CR3_f= CR3_f;
        enRet = Ok;
//    }

    return enRet;
}

/**
 ******************************************************************************
 ** \brief Write UART data buffer
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u16Data                 Send data
 **
 ** \retval Ok                          Data has been successfully sent
 **
 ******************************************************************************/
en_result_t USART_SendData(M4_USART_TypeDef *USARTx, uint16_t u16Data)
{
    /* Check USARTx pointer */
//    DDL_ASSERT(IS_VALID_USART(USARTx));

    USARTx->DR_f.TDR = (uint32_t)u16Data;

    return Ok;
}


extern uint32_t SystemCoreClock;



typedef struct
{
    __IO uint16_t PIN00                     : 1;
    __IO uint16_t PIN01                     : 1;
    __IO uint16_t PIN02                     : 1;
    __IO uint16_t PIN03                     : 1;
    __IO uint16_t PIN04                     : 1;
    __IO uint16_t PIN05                     : 1;
    __IO uint16_t PIN06                     : 1;
    __IO uint16_t PIN07                     : 1;
    __IO uint16_t PIN08                     : 1;
    __IO uint16_t PIN09                     : 1;
    __IO uint16_t PIN10                     : 1;
    __IO uint16_t PIN11                     : 1;
    __IO uint16_t PIN12                     : 1;
    __IO uint16_t PIN13                     : 1;
    __IO uint16_t PIN14                     : 1;
    __IO uint16_t PIN15                     : 1;
} stc_port_pidr_field_t;

typedef struct
{
    __IO uint16_t POUT00                    : 1;
    __IO uint16_t POUT01                    : 1;
    __IO uint16_t POUT02                    : 1;
    __IO uint16_t POUT03                    : 1;
    __IO uint16_t POUT04                    : 1;
    __IO uint16_t POUT05                    : 1;
    __IO uint16_t POUT06                    : 1;
    __IO uint16_t POUT07                    : 1;
    __IO uint16_t POUT08                    : 1;
    __IO uint16_t POUT09                    : 1;
    __IO uint16_t POUT10                    : 1;
    __IO uint16_t POUT11                    : 1;
    __IO uint16_t POUT12                    : 1;
    __IO uint16_t POUT13                    : 1;
    __IO uint16_t POUT14                    : 1;
    __IO uint16_t POUT15                    : 1;
} stc_port_podr_field_t;

typedef struct
{
    __IO uint16_t POUTE00                   : 1;
    __IO uint16_t POUTE01                   : 1;
    __IO uint16_t POUTE02                   : 1;
    __IO uint16_t POUTE03                   : 1;
    __IO uint16_t POUTE04                   : 1;
    __IO uint16_t POUTE05                   : 1;
    __IO uint16_t POUTE06                   : 1;
    __IO uint16_t POUTE07                   : 1;
    __IO uint16_t POUTE08                   : 1;
    __IO uint16_t POUTE09                   : 1;
    __IO uint16_t POUTE10                   : 1;
    __IO uint16_t POUTE11                   : 1;
    __IO uint16_t POUTE12                   : 1;
    __IO uint16_t POUTE13                   : 1;
    __IO uint16_t POUTE14                   : 1;
    __IO uint16_t POUTE15                   : 1;
} stc_port_poer_field_t;

typedef struct
{
    __IO uint16_t POS00                     : 1;
    __IO uint16_t POS01                     : 1;
    __IO uint16_t POS02                     : 1;
    __IO uint16_t POS03                     : 1;
    __IO uint16_t POS04                     : 1;
    __IO uint16_t POS05                     : 1;
    __IO uint16_t POS06                     : 1;
    __IO uint16_t POS07                     : 1;
    __IO uint16_t POS08                     : 1;
    __IO uint16_t POS09                     : 1;
    __IO uint16_t POS10                     : 1;
    __IO uint16_t POS11                     : 1;
    __IO uint16_t POS12                     : 1;
    __IO uint16_t POS13                     : 1;
    __IO uint16_t POS14                     : 1;
    __IO uint16_t POS15                     : 1;
} stc_port_posr_field_t;

typedef struct
{
    __IO uint16_t POR00                     : 1;
    __IO uint16_t POR01                     : 1;
    __IO uint16_t POR02                     : 1;
    __IO uint16_t POR03                     : 1;
    __IO uint16_t POR04                     : 1;
    __IO uint16_t POR05                     : 1;
    __IO uint16_t POR06                     : 1;
    __IO uint16_t POR07                     : 1;
    __IO uint16_t POR08                     : 1;
    __IO uint16_t POR09                     : 1;
    __IO uint16_t POR10                     : 1;
    __IO uint16_t POR11                     : 1;
    __IO uint16_t POR12                     : 1;
    __IO uint16_t POR13                     : 1;
    __IO uint16_t POR14                     : 1;
    __IO uint16_t POR15                     : 1;
} stc_port_porr_field_t;

typedef struct
{
    __IO uint16_t POT00                     : 1;
    __IO uint16_t POT01                     : 1;
    __IO uint16_t POT02                     : 1;
    __IO uint16_t POT03                     : 1;
    __IO uint16_t POT04                     : 1;
    __IO uint16_t POT05                     : 1;
    __IO uint16_t POT06                     : 1;
    __IO uint16_t POT07                     : 1;
    __IO uint16_t POT08                     : 1;
    __IO uint16_t POT09                     : 1;
    __IO uint16_t POT10                     : 1;
    __IO uint16_t POT11                     : 1;
    __IO uint16_t POT12                     : 1;
    __IO uint16_t POT13                     : 1;
    __IO uint16_t POT14                     : 1;
    __IO uint16_t POT15                     : 1;
} stc_port_potr_field_t;

typedef struct
{
    __IO uint16_t PIN00                     : 1;
    __IO uint16_t PIN01                     : 1;
    __IO uint16_t PIN02                     : 1;
    uint16_t RESERVED3                      :13;
} stc_port_pidrh_field_t;

typedef struct
{
    __IO uint16_t POUT00                    : 1;
    __IO uint16_t POUT01                    : 1;
    __IO uint16_t POUT02                    : 1;
    uint16_t RESERVED3                      :13;
} stc_port_podrh_field_t;

typedef struct
{
    __IO uint16_t POUTE00                   : 1;
    __IO uint16_t POUTE01                   : 1;
    __IO uint16_t POUTE02                   : 1;
    uint16_t RESERVED3                      :13;
} stc_port_poerh_field_t;

typedef struct
{
    __IO uint16_t POS00                     : 1;
    __IO uint16_t POS01                     : 1;
    __IO uint16_t POS02                     : 1;
    uint16_t RESERVED3                      :13;
} stc_port_posrh_field_t;

typedef struct
{
    __IO uint16_t POR00                     : 1;
    __IO uint16_t POR01                     : 1;
    __IO uint16_t POR02                     : 1;
    uint16_t RESERVED3                      :13;
} stc_port_porrh_field_t;

typedef struct
{
    __IO uint16_t POT00                     : 1;
    __IO uint16_t POT01                     : 1;
    __IO uint16_t POT02                     : 1;
    uint16_t RESERVED3                      :13;
} stc_port_potrh_field_t;

typedef struct
{
    __IO uint16_t SPFE                      : 5;
    uint16_t RESERVED5                      :11;
} stc_port_pspcr_field_t;

typedef struct
{
    __IO uint16_t BFSEL                     : 4;
    uint16_t RESERVED4                      :10;
    __IO uint16_t RDWT                      : 2;
} stc_port_pccr_field_t;

typedef struct
{
    __IO uint16_t PINAE                     : 6;
    uint16_t RESERVED6                      :10;
} stc_port_pinaer_field_t;

typedef struct
{
    __IO uint16_t WE                        : 1;
    uint16_t RESERVED1                      : 7;
    __IO uint16_t WP                        : 8;
} stc_port_pwpr_field_t;

typedef struct
{
    __IO uint16_t POUT                      : 1;
    __IO uint16_t POUTE                     : 1;
    __IO uint16_t NOD                       : 1;
    uint16_t RESERVED3                      : 1;
    __IO uint16_t DRV                       : 2;
    __IO uint16_t PUU                       : 1;
    uint16_t RESERVED7                      : 1;
    __IO uint16_t PIN                       : 1;
    __IO uint16_t INVE                      : 1;
    uint16_t RESERVED10                     : 2;
    __IO uint16_t INTE                      : 1;
    uint16_t RESERVED13                     : 1;
    __IO uint16_t LTE                       : 1;
    __IO uint16_t DDIS                      : 1;
} stc_port_pcr_field_t;

typedef struct
{
    __IO uint16_t FSEL                      : 6;
    uint16_t RESERVED6                      : 2;
    __IO uint16_t BFE                       : 1;
    uint16_t RESERVED9                      : 7;
} stc_port_pfsr_field_t;

typedef struct
{
    union
    {
        __IO uint16_t PIDRA;
        stc_port_pidr_field_t PIDRA_f;
    };
    uint8_t RESERVED1[2];
    union
    {
        __IO uint16_t PODRA;
        stc_port_podr_field_t PODRA_f;
    };
    union
    {
        __IO uint16_t POERA;
        stc_port_poer_field_t POERA_f;
    };
    union
    {
        __IO uint16_t POSRA;
        stc_port_posr_field_t POSRA_f;
    };
    union
    {
        __IO uint16_t PORRA;
        stc_port_porr_field_t PORRA_f;
    };
    union
    {
        __IO uint16_t POTRA;
        stc_port_potr_field_t POTRA_f;
    };
    uint8_t RESERVED6[2];
    union
    {
        __IO uint16_t PIDRB;
        stc_port_pidr_field_t PIDRB_f;
    };
    uint8_t RESERVED7[2];
    union
    {
        __IO uint16_t PODRB;
        stc_port_podr_field_t PODRB_f;
    };
    union
    {
        __IO uint16_t POERB;
        stc_port_poer_field_t POERB_f;
    };
    union
    {
        __IO uint16_t POSRB;
        stc_port_posr_field_t POSRB_f;
    };
    union
    {
        __IO uint16_t PORRB;
        stc_port_porr_field_t PORRB_f;
    };
    union
    {
        __IO uint16_t POTRB;
        stc_port_potr_field_t POTRB_f;
    };
    uint8_t RESERVED12[2];
    union
    {
        __IO uint16_t PIDRC;
        stc_port_pidr_field_t PIDRC_f;
    };
    uint8_t RESERVED13[2];
    union
    {
        __IO uint16_t PODRC;
        stc_port_podr_field_t PODRC_f;
    };
    union
    {
        __IO uint16_t POERC;
        stc_port_poer_field_t POERC_f;
    };
    union
    {
        __IO uint16_t POSRC;
        stc_port_posr_field_t POSRC_f;
    };
    union
    {
        __IO uint16_t PORRC;
        stc_port_porr_field_t PORRC_f;
    };
    union
    {
        __IO uint16_t POTRC;
        stc_port_potr_field_t POTRC_f;
    };
    uint8_t RESERVED18[2];
    union
    {
        __IO uint16_t PIDRD;
        stc_port_pidr_field_t PIDRD_f;
    };
    uint8_t RESERVED19[2];
    union
    {
        __IO uint16_t PODRD;
        stc_port_podr_field_t PODRD_f;
    };
    union
    {
        __IO uint16_t POERD;
        stc_port_poer_field_t POERD_f;
    };
    union
    {
        __IO uint16_t POSRD;
        stc_port_posr_field_t POSRD_f;
    };
    union
    {
        __IO uint16_t PORRD;
        stc_port_porr_field_t PORRD_f;
    };
    union
    {
        __IO uint16_t POTRD;
        stc_port_potr_field_t POTRD_f;
    };
    uint8_t RESERVED24[2];
    union
    {
        __IO uint16_t PIDRE;
        stc_port_pidr_field_t PIDRE_f;
    };
    uint8_t RESERVED25[2];
    union
    {
        __IO uint16_t PODRE;
        stc_port_podr_field_t PODRE_f;
    };
    union
    {
        __IO uint16_t POERE;
        stc_port_poer_field_t POERE_f;
    };
    union
    {
        __IO uint16_t POSRE;
        stc_port_posr_field_t POSRE_f;
    };
    union
    {
        __IO uint16_t PORRE;
        stc_port_porr_field_t PORRE_f;
    };
    union
    {
        __IO uint16_t POTRE;
        stc_port_potr_field_t POTRE_f;
    };
    uint8_t RESERVED30[2];
    union
    {
        __IO uint16_t PIDRH;
        stc_port_pidrh_field_t PIDRH_f;
    };
    uint8_t RESERVED31[2];
    union
    {
        __IO uint16_t PODRH;
        stc_port_podrh_field_t PODRH_f;
    };
    union
    {
        __IO uint16_t POERH;
        stc_port_poerh_field_t POERH_f;
    };
    union
    {
        __IO uint16_t POSRH;
        stc_port_posrh_field_t POSRH_f;
    };
    union
    {
        __IO uint16_t PORRH;
        stc_port_porrh_field_t PORRH_f;
    };
    union
    {
        __IO uint16_t POTRH;
        stc_port_potrh_field_t POTRH_f;
    };
    uint8_t RESERVED36[918];
    union
    {
        __IO uint16_t PSPCR;
        stc_port_pspcr_field_t PSPCR_f;
    };
    uint8_t RESERVED37[2];
    union
    {
        __IO uint16_t PCCR;
        stc_port_pccr_field_t PCCR_f;
    };
    union
    {
        __IO uint16_t PINAER;
        stc_port_pinaer_field_t PINAER_f;
    };
    union
    {
        __IO uint16_t PWPR;
        stc_port_pwpr_field_t PWPR_f;
    };
    uint8_t RESERVED40[2];
    union
    {
        __IO uint16_t PCRA0;
        stc_port_pcr_field_t PCRA0_f;
    };
    union
    {
        __IO uint16_t PFSRA0;
        stc_port_pfsr_field_t PFSRA0_f;
    };
    union
    {
        __IO uint16_t PCRA1;
        stc_port_pcr_field_t PCRA1_f;
    };
    union
    {
        __IO uint16_t PFSRA1;
        stc_port_pfsr_field_t PFSRA1_f;
    };
    union
    {
        __IO uint16_t PCRA2;
        stc_port_pcr_field_t PCRA2_f;
    };
    union
    {
        __IO uint16_t PFSRA2;
        stc_port_pfsr_field_t PFSRA2_f;
    };
    union
    {
        __IO uint16_t PCRA3;
        stc_port_pcr_field_t PCRA3_f;
    };
    union
    {
        __IO uint16_t PFSRA3;
        stc_port_pfsr_field_t PFSRA3_f;
    };
    union
    {
        __IO uint16_t PCRA4;
        stc_port_pcr_field_t PCRA4_f;
    };
    union
    {
        __IO uint16_t PFSRA4;
        stc_port_pfsr_field_t PFSRA4_f;
    };
    union
    {
        __IO uint16_t PCRA5;
        stc_port_pcr_field_t PCRA5_f;
    };
    union
    {
        __IO uint16_t PFSRA5;
        stc_port_pfsr_field_t PFSRA5_f;
    };
    union
    {
        __IO uint16_t PCRA6;
        stc_port_pcr_field_t PCRA6_f;
    };
    union
    {
        __IO uint16_t PFSRA6;
        stc_port_pfsr_field_t PFSRA6_f;
    };
    union
    {
        __IO uint16_t PCRA7;
        stc_port_pcr_field_t PCRA7_f;
    };
    union
    {
        __IO uint16_t PFSRA7;
        stc_port_pfsr_field_t PFSRA7_f;
    };
    union
    {
        __IO uint16_t PCRA8;
        stc_port_pcr_field_t PCRA8_f;
    };
    union
    {
        __IO uint16_t PFSRA8;
        stc_port_pfsr_field_t PFSRA8_f;
    };
    union
    {
        __IO uint16_t PCRA9;
        stc_port_pcr_field_t PCRA9_f;
    };
    union
    {
        __IO uint16_t PFSRA9;
        stc_port_pfsr_field_t PFSRA9_f;
    };
    union
    {
        __IO uint16_t PCRA10;
        stc_port_pcr_field_t PCRA10_f;
    };
    union
    {
        __IO uint16_t PFSRA10;
        stc_port_pfsr_field_t PFSRA10_f;
    };
    union
    {
        __IO uint16_t PCRA11;
        stc_port_pcr_field_t PCRA11_f;
    };
    union
    {
        __IO uint16_t PFSRA11;
        stc_port_pfsr_field_t PFSRA11_f;
    };
    union
    {
        __IO uint16_t PCRA12;
        stc_port_pcr_field_t PCRA12_f;
    };
    union
    {
        __IO uint16_t PFSRA12;
        stc_port_pfsr_field_t PFSRA12_f;
    };
    union
    {
        __IO uint16_t PCRA13;
        stc_port_pcr_field_t PCRA13_f;
    };
    union
    {
        __IO uint16_t PFSRA13;
        stc_port_pfsr_field_t PFSRA13_f;
    };
    union
    {
        __IO uint16_t PCRA14;
        stc_port_pcr_field_t PCRA14_f;
    };
    union
    {
        __IO uint16_t PFSRA14;
        stc_port_pfsr_field_t PFSRA14_f;
    };
    union
    {
        __IO uint16_t PCRA15;
        stc_port_pcr_field_t PCRA15_f;
    };
    union
    {
        __IO uint16_t PFSRA15;
        stc_port_pfsr_field_t PFSRA15_f;
    };
    union
    {
        __IO uint16_t PCRB0;
        stc_port_pcr_field_t PCRB0_f;
    };
    union
    {
        __IO uint16_t PFSRB0;
        stc_port_pfsr_field_t PFSRB0_f;
    };
    union
    {
        __IO uint16_t PCRB1;
        stc_port_pcr_field_t PCRB1_f;
    };
    union
    {
        __IO uint16_t PFSRB1;
        stc_port_pfsr_field_t PFSRB1_f;
    };
    union
    {
        __IO uint16_t PCRB2;
        stc_port_pcr_field_t PCRB2_f;
    };
    union
    {
        __IO uint16_t PFSRB2;
        stc_port_pfsr_field_t PFSRB2_f;
    };
    union
    {
        __IO uint16_t PCRB3;
        stc_port_pcr_field_t PCRB3_f;
    };
    union
    {
        __IO uint16_t PFSRB3;
        stc_port_pfsr_field_t PFSRB3_f;
    };
    union
    {
        __IO uint16_t PCRB4;
        stc_port_pcr_field_t PCRB4_f;
    };
    union
    {
        __IO uint16_t PFSRB4;
        stc_port_pfsr_field_t PFSRB4_f;
    };
    union
    {
        __IO uint16_t PCRB5;
        stc_port_pcr_field_t PCRB5_f;
    };
    union
    {
        __IO uint16_t PFSRB5;
        stc_port_pfsr_field_t PFSRB5_f;
    };
    union
    {
        __IO uint16_t PCRB6;
        stc_port_pcr_field_t PCRB6_f;
    };
    union
    {
        __IO uint16_t PFSRB6;
        stc_port_pfsr_field_t PFSRB6_f;
    };
    union
    {
        __IO uint16_t PCRB7;
        stc_port_pcr_field_t PCRB7_f;
    };
    union
    {
        __IO uint16_t PFSRB7;
        stc_port_pfsr_field_t PFSRB7_f;
    };
    union
    {
        __IO uint16_t PCRB8;
        stc_port_pcr_field_t PCRB8_f;
    };
    union
    {
        __IO uint16_t PFSRB8;
        stc_port_pfsr_field_t PFSRB8_f;
    };
    union
    {
        __IO uint16_t PCRB9;
        stc_port_pcr_field_t PCRB9_f;
    };
    union
    {
        __IO uint16_t PFSRB9;
        stc_port_pfsr_field_t PFSRB9_f;
    };
    union
    {
        __IO uint16_t PCRB10;
        stc_port_pcr_field_t PCRB10_f;
    };
    union
    {
        __IO uint16_t PFSRB10;
        stc_port_pfsr_field_t PFSRB10_f;
    };
    union
    {
        __IO uint16_t PCRB11;
        stc_port_pcr_field_t PCRB11_f;
    };
    union
    {
        __IO uint16_t PFSRB11;
        stc_port_pfsr_field_t PFSRB11_f;
    };
    union
    {
        __IO uint16_t PCRB12;
        stc_port_pcr_field_t PCRB12_f;
    };
    union
    {
        __IO uint16_t PFSRB12;
        stc_port_pfsr_field_t PFSRB12_f;
    };
    union
    {
        __IO uint16_t PCRB13;
        stc_port_pcr_field_t PCRB13_f;
    };
    union
    {
        __IO uint16_t PFSRB13;
        stc_port_pfsr_field_t PFSRB13_f;
    };
    union
    {
        __IO uint16_t PCRB14;
        stc_port_pcr_field_t PCRB14_f;
    };
    union
    {
        __IO uint16_t PFSRB14;
        stc_port_pfsr_field_t PFSRB14_f;
    };
    union
    {
        __IO uint16_t PCRB15;
        stc_port_pcr_field_t PCRB15_f;
    };
    union
    {
        __IO uint16_t PFSRB15;
        stc_port_pfsr_field_t PFSRB15_f;
    };
    union
    {
        __IO uint16_t PCRC0;
        stc_port_pcr_field_t PCRC0_f;
    };
    union
    {
        __IO uint16_t PFSRC0;
        stc_port_pfsr_field_t PFSRC0_f;
    };
    union
    {
        __IO uint16_t PCRC1;
        stc_port_pcr_field_t PCRC1_f;
    };
    union
    {
        __IO uint16_t PFSRC1;
        stc_port_pfsr_field_t PFSRC1_f;
    };
    union
    {
        __IO uint16_t PCRC2;
        stc_port_pcr_field_t PCRC2_f;
    };
    union
    {
        __IO uint16_t PFSRC2;
        stc_port_pfsr_field_t PFSRC2_f;
    };
    union
    {
        __IO uint16_t PCRC3;
        stc_port_pcr_field_t PCRC3_f;
    };
    union
    {
        __IO uint16_t PFSRC3;
        stc_port_pfsr_field_t PFSRC3_f;
    };
    union
    {
        __IO uint16_t PCRC4;
        stc_port_pcr_field_t PCRC4_f;
    };
    union
    {
        __IO uint16_t PFSRC4;
        stc_port_pfsr_field_t PFSRC4_f;
    };
    union
    {
        __IO uint16_t PCRC5;
        stc_port_pcr_field_t PCRC5_f;
    };
    union
    {
        __IO uint16_t PFSRC5;
        stc_port_pfsr_field_t PFSRC5_f;
    };
    union
    {
        __IO uint16_t PCRC6;
        stc_port_pcr_field_t PCRC6_f;
    };
    union
    {
        __IO uint16_t PFSRC6;
        stc_port_pfsr_field_t PFSRC6_f;
    };
    union
    {
        __IO uint16_t PCRC7;
        stc_port_pcr_field_t PCRC7_f;
    };
    union
    {
        __IO uint16_t PFSRC7;
        stc_port_pfsr_field_t PFSRC7_f;
    };
    union
    {
        __IO uint16_t PCRC8;
        stc_port_pcr_field_t PCRC8_f;
    };
    union
    {
        __IO uint16_t PFSRC8;
        stc_port_pfsr_field_t PFSRC8_f;
    };
    union
    {
        __IO uint16_t PCRC9;
        stc_port_pcr_field_t PCRC9_f;
    };
    union
    {
        __IO uint16_t PFSRC9;
        stc_port_pfsr_field_t PFSRC9_f;
    };
    union
    {
        __IO uint16_t PCRC10;
        stc_port_pcr_field_t PCRC10_f;
    };
    union
    {
        __IO uint16_t PFSRC10;
        stc_port_pfsr_field_t PFSRC10_f;
    };
    union
    {
        __IO uint16_t PCRC11;
        stc_port_pcr_field_t PCRC11_f;
    };
    union
    {
        __IO uint16_t PFSRC11;
        stc_port_pfsr_field_t PFSRC11_f;
    };
    union
    {
        __IO uint16_t PCRC12;
        stc_port_pcr_field_t PCRC12_f;
    };
    union
    {
        __IO uint16_t PFSRC12;
        stc_port_pfsr_field_t PFSRC12_f;
    };
    union
    {
        __IO uint16_t PCRC13;
        stc_port_pcr_field_t PCRC13_f;
    };
    union
    {
        __IO uint16_t PFSRC13;
        stc_port_pfsr_field_t PFSRC13_f;
    };
    union
    {
        __IO uint16_t PCRC14;
        stc_port_pcr_field_t PCRC14_f;
    };
    union
    {
        __IO uint16_t PFSRC14;
        stc_port_pfsr_field_t PFSRC14_f;
    };
    union
    {
        __IO uint16_t PCRC15;
        stc_port_pcr_field_t PCRC15_f;
    };
    union
    {
        __IO uint16_t PFSRC15;
        stc_port_pfsr_field_t PFSRC15_f;
    };
    union
    {
        __IO uint16_t PCRD0;
        stc_port_pcr_field_t PCRD0_f;
    };
    union
    {
        __IO uint16_t PFSRD0;
        stc_port_pfsr_field_t PFSRD0_f;
    };
    union
    {
        __IO uint16_t PCRD1;
        stc_port_pcr_field_t PCRD1_f;
    };
    union
    {
        __IO uint16_t PFSRD1;
        stc_port_pfsr_field_t PFSRD1_f;
    };
    union
    {
        __IO uint16_t PCRD2;
        stc_port_pcr_field_t PCRD2_f;
    };
    union
    {
        __IO uint16_t PFSRD2;
        stc_port_pfsr_field_t PFSRD2_f;
    };
    union
    {
        __IO uint16_t PCRD3;
        stc_port_pcr_field_t PCRD3_f;
    };
    union
    {
        __IO uint16_t PFSRD3;
        stc_port_pfsr_field_t PFSRD3_f;
    };
    union
    {
        __IO uint16_t PCRD4;
        stc_port_pcr_field_t PCRD4_f;
    };
    union
    {
        __IO uint16_t PFSRD4;
        stc_port_pfsr_field_t PFSRD4_f;
    };
    union
    {
        __IO uint16_t PCRD5;
        stc_port_pcr_field_t PCRD5_f;
    };
    union
    {
        __IO uint16_t PFSRD5;
        stc_port_pfsr_field_t PFSRD5_f;
    };
    union
    {
        __IO uint16_t PCRD6;
        stc_port_pcr_field_t PCRD6_f;
    };
    union
    {
        __IO uint16_t PFSRD6;
        stc_port_pfsr_field_t PFSRD6_f;
    };
    union
    {
        __IO uint16_t PCRD7;
        stc_port_pcr_field_t PCRD7_f;
    };
    union
    {
        __IO uint16_t PFSRD7;
        stc_port_pfsr_field_t PFSRD7_f;
    };
    union
    {
        __IO uint16_t PCRD8;
        stc_port_pcr_field_t PCRD8_f;
    };
    union
    {
        __IO uint16_t PFSRD8;
        stc_port_pfsr_field_t PFSRD8_f;
    };
    union
    {
        __IO uint16_t PCRD9;
        stc_port_pcr_field_t PCRD9_f;
    };
    union
    {
        __IO uint16_t PFSRD9;
        stc_port_pfsr_field_t PFSRD9_f;
    };
    union
    {
        __IO uint16_t PCRD10;
        stc_port_pcr_field_t PCRD10_f;
    };
    union
    {
        __IO uint16_t PFSRD10;
        stc_port_pfsr_field_t PFSRD10_f;
    };
    union
    {
        __IO uint16_t PCRD11;
        stc_port_pcr_field_t PCRD11_f;
    };
    union
    {
        __IO uint16_t PFSRD11;
        stc_port_pfsr_field_t PFSRD11_f;
    };
    union
    {
        __IO uint16_t PCRD12;
        stc_port_pcr_field_t PCRD12_f;
    };
    union
    {
        __IO uint16_t PFSRD12;
        stc_port_pfsr_field_t PFSRD12_f;
    };
    union
    {
        __IO uint16_t PCRD13;
        stc_port_pcr_field_t PCRD13_f;
    };
    union
    {
        __IO uint16_t PFSRD13;
        stc_port_pfsr_field_t PFSRD13_f;
    };
    union
    {
        __IO uint16_t PCRD14;
        stc_port_pcr_field_t PCRD14_f;
    };
    union
    {
        __IO uint16_t PFSRD14;
        stc_port_pfsr_field_t PFSRD14_f;
    };
    union
    {
        __IO uint16_t PCRD15;
        stc_port_pcr_field_t PCRD15_f;
    };
    union
    {
        __IO uint16_t PFSRD15;
        stc_port_pfsr_field_t PFSRD15_f;
    };
    union
    {
        __IO uint16_t PCRE0;
        stc_port_pcr_field_t PCRE0_f;
    };
    union
    {
        __IO uint16_t PFSRE0;
        stc_port_pfsr_field_t PFSRE0_f;
    };
    union
    {
        __IO uint16_t PCRE1;
        stc_port_pcr_field_t PCRE1_f;
    };
    union
    {
        __IO uint16_t PFSRE1;
        stc_port_pfsr_field_t PFSRE1_f;
    };
    union
    {
        __IO uint16_t PCRE2;
        stc_port_pcr_field_t PCRE2_f;
    };
    union
    {
        __IO uint16_t PFSRE2;
        stc_port_pfsr_field_t PFSRE2_f;
    };
    union
    {
        __IO uint16_t PCRE3;
        stc_port_pcr_field_t PCRE3_f;
    };
    union
    {
        __IO uint16_t PFSRE3;
        stc_port_pfsr_field_t PFSRE3_f;
    };
    union
    {
        __IO uint16_t PCRE4;
        stc_port_pcr_field_t PCRE4_f;
    };
    union
    {
        __IO uint16_t PFSRE4;
        stc_port_pfsr_field_t PFSRE4_f;
    };
    union
    {
        __IO uint16_t PCRE5;
        stc_port_pcr_field_t PCRE5_f;
    };
    union
    {
        __IO uint16_t PFSRE5;
        stc_port_pfsr_field_t PFSRE5_f;
    };
    union
    {
        __IO uint16_t PCRE6;
        stc_port_pcr_field_t PCRE6_f;
    };
    union
    {
        __IO uint16_t PFSRE6;
        stc_port_pfsr_field_t PFSRE6_f;
    };
    union
    {
        __IO uint16_t PCRE7;
        stc_port_pcr_field_t PCRE7_f;
    };
    union
    {
        __IO uint16_t PFSRE7;
        stc_port_pfsr_field_t PFSRE7_f;
    };
    union
    {
        __IO uint16_t PCRE8;
        stc_port_pcr_field_t PCRE8_f;
    };
    union
    {
        __IO uint16_t PFSRE8;
        stc_port_pfsr_field_t PFSRE8_f;
    };
    union
    {
        __IO uint16_t PCRE9;
        stc_port_pcr_field_t PCRE9_f;
    };
    union
    {
        __IO uint16_t PFSRE9;
        stc_port_pfsr_field_t PFSRE9_f;
    };
    union
    {
        __IO uint16_t PCRE10;
        stc_port_pcr_field_t PCRE10_f;
    };
    union
    {
        __IO uint16_t PFSRE10;
        stc_port_pfsr_field_t PFSRE10_f;
    };
    union
    {
        __IO uint16_t PCRE11;
        stc_port_pcr_field_t PCRE11_f;
    };
    union
    {
        __IO uint16_t PFSRE11;
        stc_port_pfsr_field_t PFSRE11_f;
    };
    union
    {
        __IO uint16_t PCRE12;
        stc_port_pcr_field_t PCRE12_f;
    };
    union
    {
        __IO uint16_t PFSRE12;
        stc_port_pfsr_field_t PFSRE12_f;
    };
    union
    {
        __IO uint16_t PCRE13;
        stc_port_pcr_field_t PCRE13_f;
    };
    union
    {
        __IO uint16_t PFSRE13;
        stc_port_pfsr_field_t PFSRE13_f;
    };
    union
    {
        __IO uint16_t PCRE14;
        stc_port_pcr_field_t PCRE14_f;
    };
    union
    {
        __IO uint16_t PFSRE14;
        stc_port_pfsr_field_t PFSRE14_f;
    };
    union
    {
        __IO uint16_t PCRE15;
        stc_port_pcr_field_t PCRE15_f;
    };
    union
    {
        __IO uint16_t PFSRE15;
        stc_port_pfsr_field_t PFSRE15_f;
    };
    union
    {
        __IO uint16_t PCRH0;
        stc_port_pcr_field_t PCRH0_f;
    };
    union
    {
        __IO uint16_t PFSRH0;
        stc_port_pfsr_field_t PFSRH0_f;
    };
    union
    {
        __IO uint16_t PCRH1;
        stc_port_pcr_field_t PCRH1_f;
    };
    union
    {
        __IO uint16_t PFSRH1;
        stc_port_pfsr_field_t PFSRH1_f;
    };
    union
    {
        __IO uint16_t PCRH2;
        stc_port_pcr_field_t PCRH2_f;
    };
    union
    {
        __IO uint16_t PFSRH2;
        stc_port_pfsr_field_t PFSRH2_f;
    };
}M4_PORT_TypeDef;

#define M4_PORT                                 ((M4_PORT_TypeDef *)0x40053800UL)

/**
 *******************************************************************************
 ** \brief   PSPCR, PCCR, PINAER, PCRxy, PFSRxy write enable
 **
 ** \param   None
 **
 ** \retval  None
 **
 ******************************************************************************/
void PORT_Unlock(void)
{
    M4_PORT->PWPR = 0xA501u;
}

/**
 *******************************************************************************
 ** \brief  SPCR, PCCR, PINAER, PCRxy, PFSRxy write disable
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void PORT_Lock(void)
{
    M4_PORT->PWPR = 0xA500u;
}

/**
 *******************************************************************************
 ** \brief   Set Port Pin function
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any value of @ref en_pin_t
 ** \param   [in]  enFuncSel            Function selection, This parameter can be
 **                                     any value of @ref en_port_func_t
 **
 ** \param   [in] enSubFunc             The new state of the gpio sub-function.
 ** \arg     Enable                     Enable.
 ** \arg     Disable                    Disable.
 **
 ** \retval  Ok                         Set successful to corresponding pins
 **
 ******************************************************************************/
en_result_t PORT_SetFunc(en_port_t enPort, uint16_t u16Pin, en_port_func_t enFuncSel,  \
                  en_functional_state_t enSubFunc)
{
    stc_port_pfsr_field_t *PFSRx;
    uint8_t u8PinPos = 0u;

    PORT_Unlock();

    for (u8PinPos = 0u; u8PinPos < 16u; u8PinPos ++)
    {
        if (u16Pin & (uint16_t)(1ul<<u8PinPos))
        {
            PFSRx = (stc_port_pfsr_field_t *)((uint32_t)(&M4_PORT->PFSRA0) \
                    + 0x40ul * enPort + 0x4ul * u8PinPos);

            /* main function setting */
            PFSRx->FSEL = enFuncSel;

            /* sub function enable setting */
            PFSRx->BFE = (Enable == enSubFunc ? Enable : Disable);
        }
    }

    PORT_Lock();
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get flag status
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enStatus                Choose need get status's flag
 ** \arg UsartParityError               Parity check error
 ** \arg UsartFrameError                Frame error
 ** \arg UsartOverRunError              Overrun error
 ** \arg UsartRxRegNoEmpty              Rx register is no empty
 ** \arg UsartTxComplete                Transfer completely
 ** \arg UsartTxRegNoEmpty              Tx register is no empty
 ** \arg UsartRxTimeOut                 Data receive timeout
 ** \arg UsartRxDataType                Data is multiple processor id or normal data.
 **
 ** \retval Set                         Flag is set.
 ** \retval Reset                       Flag is reset or enStatus is invalid.
 **
 ******************************************************************************/
en_flag_status_t USART_GetStatus(M4_USART_TypeDef *USARTx,
                                en_usart_status_t enStatus)
{
//    /* Check parameter */
//    DDL_ASSERT(IS_VALID_USART(USARTx));
//    DDL_ASSERT(IS_VALID_USART_STATUS(enStatus));

    return ((USARTx->SR & enStatus) ? Set : Reset);
}
/**
 *******************************************************************************
 ** \brief  The drive capability of xtal.
 **
 ******************************************************************************/
typedef enum en_clk_xtal_drv
{
    ClkXtalHighDrv                  = 0u,   ///< High drive capability.20MHz~24MHz.
    ClkXtalMidDrv                   = 1u,   ///< Middle drive capability.16MHz~20MHz.
    ClkXtalLowDrv                   = 2u,   ///< Low drive capability.8MHz~16MHz.
    ClkXtalTinyDrv                  = 3u,   ///< Tiny drive capability.8MHz.
}en_clk_xtal_drv_t;
/**
 *******************************************************************************
 ** \brief  The source of xtal.
 **
 ******************************************************************************/
typedef enum en_clk_xtal_mode
{
    ClkXtalModeOsc                  = 0u,   ///< Use external high speed osc as source.
    ClkXtalModeExtClk               = 1u,   ///< Use external clk as source.
}en_clk_xtal_mode_t;
/**
 *******************************************************************************
 ** \brief  Configuration structure of XTAL.
 **
 ** \note   Configures the XTAL if needed.
 **
 ******************************************************************************/
typedef struct stc_clk_xtal_cfg
{
    en_functional_state_t           enFastStartup;  ///< Enable fast start up or not.
    en_clk_xtal_mode_t              enMode;         ///< Select xtal mode.
    en_clk_xtal_drv_t               enDrv;          ///< Select xtal drive capability.
}stc_clk_xtal_cfg_t;
/**
 *******************************************************************************
 ** \brief  Configuration structure of PLL.
 **
 ** \note   Configures the PLL if needed.
 **
 ******************************************************************************/
typedef struct stc_clk_pll_cfg
{
    uint32_t         PllpDiv;            ///< Pllp clk, division factor of VCO out.
    uint32_t         PllqDiv;            ///< Pllq clk, division factor of VCO out.
    uint32_t         PllrDiv;            ///< Pllr clk, division factor of VCO out.
    uint32_t         plln;               ///< Multiplication factor of vco out, ensure between 240M~480M
    uint32_t         pllmDiv;            ///< Division factor of VCO in, ensure between 1M~12M.
}stc_clk_mpll_cfg_t, stc_clk_upll_cfg_t;
/**
 *******************************************************************************
 ** \brief  The system clock source.
 **
 ******************************************************************************/
 typedef enum en_clk_sys_source
 {
     ClkSysSrcHRC                   = 0u,   ///< The system clock source is HRC.
     ClkSysSrcMRC                   = 1u,   ///< The system clock source is MRC.
     ClkSysSrcLRC                   = 2u,   ///< The system clock source is LRC.
     ClkSysSrcXTAL                  = 3u,   ///< The system clock source is XTAL.
     ClkSysSrcXTAL32                = 4u,   ///< The system clock source is XTAL32.
     CLKSysSrcMPLL                  = 5u,   ///< The system clock source is MPLL.
 }en_clk_sys_source_t;
 /**
  *******************************************************************************
  ** \brief  The division factor of system clock.
  **
  ******************************************************************************/
 typedef enum en_clk_sysclk_div_factor
 {
     ClkSysclkDiv1                   = 0u,   ///< 1 division.
     ClkSysclkDiv2                   = 1u,   ///< 2 division.
     ClkSysclkDiv4                   = 2u,   ///< 4 division.
     ClkSysclkDiv8                   = 3u,   ///< 8 division.
     ClkSysclkDiv16                  = 4u,   ///< 16 division.
     ClkSysclkDiv32                  = 5u,   ///< 32 division.
     ClkSysclkDiv64                  = 6u,   ///< 64 division.
 }en_clk_sysclk_div_factor_t;
 /**
  *******************************************************************************
  ** \brief  Configuration structure of system clock.
  **
  ** \note   Configures the system clock if needed.
  **
  ******************************************************************************/
 typedef struct stc_clk_sysclk_cfg
 {
     en_clk_sysclk_div_factor_t      enHclkDiv;        ///< Division for hclk.
     en_clk_sysclk_div_factor_t      enExclkDiv;       ///< Division for exclk.
     en_clk_sysclk_div_factor_t      enPclk0Div;       ///< Division for pclk0.
     en_clk_sysclk_div_factor_t      enPclk1Div;       ///< Division for pclk1.
     en_clk_sysclk_div_factor_t      enPclk2Div;       ///< Division for pclk2.
     en_clk_sysclk_div_factor_t      enPclk3Div;       ///< Division for pclk3.
     en_clk_sysclk_div_factor_t      enPclk4Div;       ///< Division for pclk4.
 }stc_clk_sysclk_cfg_t;
 /**
  *******************************************************************************
  ** \brief  The pll clock source.
  **
  ******************************************************************************/
  typedef enum en_clk_pll_source
  {
      ClkPllSrcXTAL                  = 0u,   ///< The pll clock source is XTAL.
      ClkPllSrcHRC                   = 1u,   ///< The pll clock source is HRC.
  }en_clk_pll_source_t;

  typedef struct
  {
      __IO uint16_t FLNWT                     : 1;
      __IO uint16_t CKSMRC                    : 1;
      uint16_t RESERVED2                      :13;
      __IO uint16_t STOP                      : 1;
  } stc_sysreg_pwr_stpmcr_field_t;

  typedef struct
  {
      __IO uint16_t PERICKSEL                 : 4;
      uint16_t RESERVED4                      :12;
  } stc_sysreg_cmu_pericksel_field_t;

  typedef struct
  {
      __IO uint16_t I2S1CKSEL                 : 4;
      __IO uint16_t I2S2CKSEL                 : 4;
      __IO uint16_t I2S3CKSEL                 : 4;
      __IO uint16_t I2S4CKSEL                 : 4;
  } stc_sysreg_cmu_i2scksel_field_t;

  typedef struct
  {
      __IO uint32_t RAMPDC0                   : 1;
      __IO uint32_t RAMPDC1                   : 1;
      __IO uint32_t RAMPDC2                   : 1;
      __IO uint32_t RAMPDC3                   : 1;
      __IO uint32_t RAMPDC4                   : 1;
      __IO uint32_t RAMPDC5                   : 1;
      __IO uint32_t RAMPDC6                   : 1;
      __IO uint32_t RAMPDC7                   : 1;
      __IO uint32_t RAMPDC8                   : 1;
      uint32_t RESERVED9                      :23;
  } stc_sysreg_pwr_rampc0_field_t;

  typedef struct
  {
      __IO uint32_t RAMOPM                    :16;
      uint32_t RESERVED16                     :16;
  } stc_sysreg_pwr_ramopm_field_t;

  typedef struct
  {
      __IO uint32_t AESRDP                    : 1;
      __IO uint32_t AESWRP                    : 1;
      __IO uint32_t HASHRDP                   : 1;
      __IO uint32_t HASHWRP                   : 1;
      __IO uint32_t TRNGRDP                   : 1;
      __IO uint32_t TRNGWRP                   : 1;
      __IO uint32_t CRCRDP                    : 1;
      __IO uint32_t CRCWRP                    : 1;
      __IO uint32_t FMCRDP                    : 1;
      __IO uint32_t FMCWRP                    : 1;
      uint32_t RESERVED10                     : 2;
      __IO uint32_t WDTRDP                    : 1;
      __IO uint32_t WDTWRP                    : 1;
      __IO uint32_t SWDTRDP                   : 1;
      __IO uint32_t SWDTWRP                   : 1;
      __IO uint32_t BKSRAMRDP                 : 1;
      __IO uint32_t BKSRAMWRP                 : 1;
      __IO uint32_t RTCRDP                    : 1;
      __IO uint32_t RTCWRP                    : 1;
      __IO uint32_t DMPURDP                   : 1;
      __IO uint32_t DMPUWRP                   : 1;
      __IO uint32_t SRAMCRDP                  : 1;
      __IO uint32_t SRAMCWRP                  : 1;
      __IO uint32_t INTCRDP                   : 1;
      __IO uint32_t INTCWRP                   : 1;
      __IO uint32_t SYSCRDP                   : 1;
      __IO uint32_t SYSCWRP                   : 1;
      __IO uint32_t MSTPRDP                   : 1;
      __IO uint32_t MSTPWRP                   : 1;
      uint32_t RESERVED30                     : 1;
      __IO uint32_t BUSERRE                   : 1;
  } stc_sysreg_mpu_ippr_field_t;

  typedef struct
  {
      __IO uint32_t PCLK0S                    : 3;
      uint32_t RESERVED3                      : 1;
      __IO uint32_t PCLK1S                    : 3;
      uint32_t RESERVED7                      : 1;
      __IO uint32_t PCLK2S                    : 3;
      uint32_t RESERVED11                     : 1;
      __IO uint32_t PCLK3S                    : 3;
      uint32_t RESERVED15                     : 1;
      __IO uint32_t PCLK4S                    : 3;
      uint32_t RESERVED19                     : 1;
      __IO uint32_t EXCKS                     : 3;
      uint32_t RESERVED23                     : 1;
      __IO uint32_t HCLKS                     : 3;
      uint32_t RESERVED27                     : 5;
  } stc_sysreg_cmu_scfgr_field_t;

  typedef struct
  {
      uint8_t RESERVED0                       : 4;
      __IO uint8_t USBCKS                     : 4;
  } stc_sysreg_cmu_ufsckcfgr_field_t;

  typedef struct
  {
      __IO uint8_t CKSW                       : 3;
      uint8_t RESERVED3                       : 5;
  } stc_sysreg_cmu_ckswr_field_t;

  typedef struct
  {
      __IO uint8_t MPLLOFF                    : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_pllcr_field_t;

  typedef struct
  {
      __IO uint8_t UPLLOFF                    : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_upllcr_field_t;

  typedef struct
  {
      __IO uint8_t XTALSTP                    : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_xtalcr_field_t;

  typedef struct
  {
      __IO uint8_t HRCSTP                     : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_hrccr_field_t;

  typedef struct
  {
      __IO uint8_t MRCSTP                     : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_mrccr_field_t;

  typedef struct
  {
      __IO uint8_t HRCSTBF                    : 1;
      uint8_t RESERVED1                       : 2;
      __IO uint8_t XTALSTBF                   : 1;
      uint8_t RESERVED4                       : 1;
      __IO uint8_t MPLLSTBF                   : 1;
      __IO uint8_t UPLLSTBF                   : 1;
      uint8_t RESERVED7                       : 1;
  } stc_sysreg_cmu_oscstbsr_field_t;

  typedef struct
  {
      __IO uint8_t MCO1SEL                    : 4;
      __IO uint8_t MCO1DIV                    : 3;
      __IO uint8_t MCO1EN                     : 1;
  } stc_sysreg_cmu_mco1cfgr_field_t;

  typedef struct
  {
      __IO uint8_t MCO2SEL                    : 4;
      __IO uint8_t MCO2DIV                    : 3;
      __IO uint8_t MCO2EN                     : 1;
  } stc_sysreg_cmu_mco2cfgr_field_t;

  typedef struct
  {
      __IO uint8_t TPIUCKS                    : 2;
      uint8_t RESERVED2                       : 5;
      __IO uint8_t TPIUCKOE                   : 1;
  } stc_sysreg_cmu_tpiuckcfgr_field_t;

  typedef struct
  {
      __IO uint8_t XTALSTDIE                  : 1;
      __IO uint8_t XTALSTDRE                  : 1;
      __IO uint8_t XTALSTDRIS                 : 1;
      uint8_t RESERVED3                       : 4;
      __IO uint8_t XTALSTDE                   : 1;
  } stc_sysreg_cmu_xtalstdcr_field_t;

  typedef struct
  {
      __IO uint8_t XTALSTDF                   : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_xtalstdsr_field_t;

  typedef struct
  {
      __IO uint8_t XTALSTB                    : 4;
      uint8_t RESERVED4                       : 4;
  } stc_sysreg_cmu_xtalstbcr_field_t;

  typedef struct
  {
      __IO uint16_t PORF                      : 1;
      __IO uint16_t PINRF                     : 1;
      __IO uint16_t BORF                      : 1;
      __IO uint16_t PVD1RF                    : 1;
      __IO uint16_t PVD2RF                    : 1;
      __IO uint16_t WDRF                      : 1;
      __IO uint16_t SWDRF                     : 1;
      __IO uint16_t PDRF                      : 1;
      __IO uint16_t SWRF                      : 1;
      __IO uint16_t MPUERF                    : 1;
      __IO uint16_t RAPERF                    : 1;
      __IO uint16_t RAECRF                    : 1;
      __IO uint16_t CKFERF                    : 1;
      __IO uint16_t XTALERF                   : 1;
      __IO uint16_t MULTIRF                   : 1;
      __IO uint16_t CLRF                      : 1;
  } stc_sysreg_rmu_rstf0_field_t;

  typedef struct
  {
      __IO uint8_t PVD1NMIS                   : 1;
      uint8_t RESERVED1                       : 3;
      __IO uint8_t PVD2NMIS                   : 1;
      uint8_t RESERVED5                       : 3;
  } stc_sysreg_pwr_pvdicr_field_t;

  typedef struct
  {
      uint8_t RESERVED0                       : 1;
      __IO uint8_t PVD1DETFLG                 : 1;
      uint8_t RESERVED2                       : 3;
      __IO uint8_t PVD2DETFLG                 : 1;
      uint8_t RESERVED6                       : 2;
  } stc_sysreg_pwr_pvddsr_field_t;

  typedef struct
  {
      __IO uint32_t MPLLM                     : 5;
      uint32_t RESERVED5                      : 2;
      __IO uint32_t PLLSRC                    : 1;
      __IO uint32_t MPLLN                     : 9;
      uint32_t RESERVED17                     : 3;
      __IO uint32_t MPLLR                     : 4;
      __IO uint32_t MPLLQ                     : 4;
      __IO uint32_t MPLLP                     : 4;
  } stc_sysreg_cmu_pllcfgr_field_t;

  typedef struct
  {
      __IO uint32_t UPLLM                     : 5;
      uint32_t RESERVED5                      : 3;
      __IO uint32_t UPLLN                     : 9;
      uint32_t RESERVED17                     : 3;
      __IO uint32_t UPLLR                     : 4;
      __IO uint32_t UPLLQ                     : 4;
      __IO uint32_t UPLLP                     : 4;
  } stc_sysreg_cmu_upllcfgr_field_t;

  typedef struct
  {
      __IO uint16_t FPRCB0                    : 1;
      __IO uint16_t FPRCB1                    : 1;
      __IO uint16_t FPRCB2                    : 1;
      __IO uint16_t FPRCB3                    : 1;
      uint16_t RESERVED4                      : 4;
      __IO uint16_t FPRCWE                    : 8;
  } stc_sysreg_pwr_fprc_field_t;

  typedef struct
  {
      __IO uint8_t PDMDS                      : 2;
      __IO uint8_t VVDRSD                     : 1;
      __IO uint8_t RETRAMSD                   : 1;
      __IO uint8_t IORTN                      : 2;
      uint8_t RESERVED6                       : 1;
      __IO uint8_t PWDN                       : 1;
  } stc_sysreg_pwr_pwrc0_field_t;

  typedef struct
  {
      __IO uint8_t VPLLSD                     : 1;
      __IO uint8_t VHRCSD                     : 1;
      uint8_t RESERVED2                       : 4;
      __IO uint8_t STPDAS                     : 2;
  } stc_sysreg_pwr_pwrc1_field_t;

  typedef struct
  {
      __IO uint8_t DDAS                       : 4;
      __IO uint8_t DVS                        : 2;
      uint8_t RESERVED6                       : 2;
  } stc_sysreg_pwr_pwrc2_field_t;

  typedef struct
  {
      uint8_t RESERVED0                       : 2;
      __IO uint8_t PDTS                       : 1;
      uint8_t RESERVED3                       : 5;
  } stc_sysreg_pwr_pwrc3_field_t;

  typedef struct
  {
      __IO uint8_t WKE00                      : 1;
      __IO uint8_t WKE01                      : 1;
      __IO uint8_t WKE02                      : 1;
      __IO uint8_t WKE03                      : 1;
      __IO uint8_t WKE10                      : 1;
      __IO uint8_t WKE11                      : 1;
      __IO uint8_t WKE12                      : 1;
      __IO uint8_t WKE13                      : 1;
  } stc_sysreg_pwr_pdwke0_field_t;

  typedef struct
  {
      __IO uint8_t WKE20                      : 1;
      __IO uint8_t WKE21                      : 1;
      __IO uint8_t WKE22                      : 1;
      __IO uint8_t WKE23                      : 1;
      __IO uint8_t WKE30                      : 1;
      __IO uint8_t WKE31                      : 1;
      __IO uint8_t WKE32                      : 1;
      __IO uint8_t WKE33                      : 1;
  } stc_sysreg_pwr_pdwke1_field_t;

  typedef struct
  {
      __IO uint8_t VD1WKE                     : 1;
      __IO uint8_t VD2WKE                     : 1;
      __IO uint8_t NMIWKE                     : 1;
      uint8_t RESERVED3                       : 1;
      __IO uint8_t RTCPRDWKE                  : 1;
      __IO uint8_t RTCALMWKE                  : 1;
      __IO uint8_t XTAL32ERWKE                : 1;
      __IO uint8_t WKTMWKE                    : 1;
  } stc_sysreg_pwr_pdwke2_field_t;

  typedef struct
  {
      __IO uint8_t WK0EGS                     : 1;
      __IO uint8_t WK1EGS                     : 1;
      __IO uint8_t WK2EGS                     : 1;
      __IO uint8_t WK3EGS                     : 1;
      __IO uint8_t VD1EGS                     : 1;
      __IO uint8_t VD2EGS                     : 1;
      __IO uint8_t NMIEGS                     : 1;
      uint8_t RESERVED7                       : 1;
  } stc_sysreg_pwr_pdwkes_field_t;

  typedef struct
  {
      __IO uint8_t PTWK0F                     : 1;
      __IO uint8_t PTWK1F                     : 1;
      __IO uint8_t PTWK2F                     : 1;
      __IO uint8_t PTWK3F                     : 1;
      __IO uint8_t VD1WKF                     : 1;
      __IO uint8_t VD2WKF                     : 1;
      __IO uint8_t NMIWKF                     : 1;
      uint8_t RESERVED7                       : 1;
  } stc_sysreg_pwr_pdwkf0_field_t;

  typedef struct
  {
      uint8_t RESERVED0                       : 3;
      __IO uint8_t RXD0WKF                    : 1;
      __IO uint8_t RTCPRDWKF                  : 1;
      __IO uint8_t RTCALMWKF                  : 1;
      __IO uint8_t XTAL32ERWKF                : 1;
      __IO uint8_t WKTMWKF                    : 1;
  } stc_sysreg_pwr_pdwkf1_field_t;

  typedef struct
  {
      uint8_t RESERVED0                       : 7;
      __IO uint8_t ADBUFE                     : 1;
  } stc_sysreg_pwr_pwcmr_field_t;

  typedef struct
  {
      uint8_t RESERVED0                       : 4;
      __IO uint8_t XTALDRV                    : 2;
      __IO uint8_t XTALMS                     : 1;
      __IO uint8_t SUPDRV                     : 1;
  } stc_sysreg_cmu_xtalcfgr_field_t;

  typedef struct
  {
      __IO uint8_t EXVCCINEN                  : 1;
      uint8_t RESERVED1                       : 4;
      __IO uint8_t PVD1EN                     : 1;
      __IO uint8_t PVD2EN                     : 1;
      uint8_t RESERVED7                       : 1;
  } stc_sysreg_pwr_pvdcr0_field_t;

  typedef struct
  {
      __IO uint8_t PVD1IRE                    : 1;
      __IO uint8_t PVD1IRS                    : 1;
      __IO uint8_t PVD1CMPOE                  : 1;
      uint8_t RESERVED3                       : 1;
      __IO uint8_t PVD2IRE                    : 1;
      __IO uint8_t PVD2IRS                    : 1;
      __IO uint8_t PVD2CMPOE                  : 1;
      uint8_t RESERVED7                       : 1;
  } stc_sysreg_pwr_pvdcr1_field_t;

  typedef struct
  {
      __IO uint8_t PVD1NFDIS                  : 1;
      __IO uint8_t PVD1NFCKS                  : 2;
      uint8_t RESERVED3                       : 1;
      __IO uint8_t PVD2NFDIS                  : 1;
      __IO uint8_t PVD2NFCKS                  : 2;
      uint8_t RESERVED7                       : 1;
  } stc_sysreg_pwr_pvdfcr_field_t;

  typedef struct
  {
      __IO uint8_t PVD1LVL                    : 3;
      uint8_t RESERVED3                       : 1;
      __IO uint8_t PVD2LVL                    : 3;
      uint8_t RESERVED7                       : 1;
  } stc_sysreg_pwr_pvdlcr_field_t;

  typedef struct
  {
      __IO uint8_t XTAL32STP                  : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_xtal32cr_field_t;

  typedef struct
  {
      __IO uint8_t XTAL32DRV                  : 2;
      __IO uint8_t XTAL32SUPDRV               : 1;
      uint8_t RESERVED3                       : 5;
  } stc_sysreg_cmu_xtal32cfgr_field_t;

  typedef struct
  {
      __IO uint8_t XTAL32NF                   : 2;
      uint8_t RESERVED2                       : 6;
  } stc_sysreg_cmu_xtal32nfr_field_t;

  typedef struct
  {
      __IO uint8_t LRCSTP                     : 1;
      uint8_t RESERVED1                       : 7;
  } stc_sysreg_cmu_lrccr_field_t;

  typedef struct
  {
      uint8_t RESERVED0                       : 7;
      __IO uint8_t CSDIS                      : 1;
  } stc_sysreg_pwr_xtal32cs_field_t;


  typedef struct
  {
      uint8_t RESERVED0[12];
      union
      {
          __IO uint16_t PWR_STPMCR;
          stc_sysreg_pwr_stpmcr_field_t PWR_STPMCR_f;
      };
      uint8_t RESERVED1[2];
      union
      {
          __IO uint16_t CMU_PERICKSEL;
          stc_sysreg_cmu_pericksel_field_t CMU_PERICKSEL_f;
      };
      union
      {
          __IO uint16_t CMU_I2SCKSEL;
          stc_sysreg_cmu_i2scksel_field_t CMU_I2SCKSEL_f;
      };
      union
      {
          __IO uint32_t PWR_RAMPC0;
          stc_sysreg_pwr_rampc0_field_t PWR_RAMPC0_f;
      };
      union
      {
          __IO uint32_t PWR_RAMOPM;
          stc_sysreg_pwr_ramopm_field_t PWR_RAMOPM_f;
      };
      union
      {
          __IO uint32_t MPU_IPPR;
          stc_sysreg_mpu_ippr_field_t MPU_IPPR_f;
      };
      union
      {
          __IO uint32_t CMU_SCFGR;
          stc_sysreg_cmu_scfgr_field_t CMU_SCFGR_f;
      };
      union
      {
          __IO uint8_t CMU_UFSCKCFGR;
          stc_sysreg_cmu_ufsckcfgr_field_t CMU_UFSCKCFGR_f;
      };
      uint8_t RESERVED8[1];
      union
      {
          __IO uint8_t CMU_CKSWR;
          stc_sysreg_cmu_ckswr_field_t CMU_CKSWR_f;
      };
      uint8_t RESERVED9[3];
      union
      {
          __IO uint8_t CMU_PLLCR;
          stc_sysreg_cmu_pllcr_field_t CMU_PLLCR_f;
      };
      uint8_t RESERVED10[3];
      union
      {
          __IO uint8_t CMU_UPLLCR;
          stc_sysreg_cmu_upllcr_field_t CMU_UPLLCR_f;
      };
      uint8_t RESERVED11[3];
      union
      {
          __IO uint8_t CMU_XTALCR;
          stc_sysreg_cmu_xtalcr_field_t CMU_XTALCR_f;
      };
      uint8_t RESERVED12[3];
      union
      {
          __IO uint8_t CMU_HRCCR;
          stc_sysreg_cmu_hrccr_field_t CMU_HRCCR_f;
      };
      uint8_t RESERVED13[1];
      union
      {
          __IO uint8_t CMU_MRCCR;
          stc_sysreg_cmu_mrccr_field_t CMU_MRCCR_f;
      };
      uint8_t RESERVED14[3];
      union
      {
          __IO uint8_t CMU_OSCSTBSR;
          stc_sysreg_cmu_oscstbsr_field_t CMU_OSCSTBSR_f;
      };
      union
      {
          __IO uint8_t CMU_MCO1CFGR;
          stc_sysreg_cmu_mco1cfgr_field_t CMU_MCO1CFGR_f;
      };
      union
      {
          __IO uint8_t CMU_MCO2CFGR;
          stc_sysreg_cmu_mco2cfgr_field_t CMU_MCO2CFGR_f;
      };
      union
      {
          __IO uint8_t CMU_TPIUCKCFGR;
          stc_sysreg_cmu_tpiuckcfgr_field_t CMU_TPIUCKCFGR_f;
      };
      union
      {
          __IO uint8_t CMU_XTALSTDCR;
          stc_sysreg_cmu_xtalstdcr_field_t CMU_XTALSTDCR_f;
      };
      union
      {
          __IO uint8_t CMU_XTALSTDSR;
          stc_sysreg_cmu_xtalstdsr_field_t CMU_XTALSTDSR_f;
      };
      uint8_t RESERVED20[31];
      __IO uint8_t CMU_MRCTRM;
      __IO uint8_t CMU_HRCTRM;
      uint8_t RESERVED22[63];
      union
      {
          __IO uint8_t CMU_XTALSTBCR;
          stc_sysreg_cmu_xtalstbcr_field_t CMU_XTALSTBCR_f;
      };
      uint8_t RESERVED23[29];
      union
      {
          __IO uint16_t RMU_RSTF0;
          stc_sysreg_rmu_rstf0_field_t RMU_RSTF0_f;
      };
      uint8_t RESERVED24[30];
      union
      {
          __IO uint8_t PWR_PVDICR;
          stc_sysreg_pwr_pvdicr_field_t PWR_PVDICR_f;
      };
      union
      {
          __IO uint8_t PWR_PVDDSR;
          stc_sysreg_pwr_pvddsr_field_t PWR_PVDDSR_f;
      };
      uint8_t RESERVED26[30];
      union
      {
          __IO uint32_t CMU_PLLCFGR;
          stc_sysreg_cmu_pllcfgr_field_t CMU_PLLCFGR_f;
      };
      union
      {
          __IO uint32_t CMU_UPLLCFGR;
          stc_sysreg_cmu_upllcfgr_field_t CMU_UPLLCFGR_f;
      };
      uint8_t RESERVED28[758];
      union
      {
          __IO uint16_t PWR_FPRC;
          stc_sysreg_pwr_fprc_field_t PWR_FPRC_f;
      };
      union
      {
          __IO uint8_t PWR_PWRC0;
          stc_sysreg_pwr_pwrc0_field_t PWR_PWRC0_f;
      };
      union
      {
          __IO uint8_t PWR_PWRC1;
          stc_sysreg_pwr_pwrc1_field_t PWR_PWRC1_f;
      };
      union
      {
          __IO uint8_t PWR_PWRC2;
          stc_sysreg_pwr_pwrc2_field_t PWR_PWRC2_f;
      };
      union
      {
          __IO uint8_t PWR_PWRC3;
          stc_sysreg_pwr_pwrc3_field_t PWR_PWRC3_f;
      };
      union
      {
          __IO uint8_t PWR_PDWKE0;
          stc_sysreg_pwr_pdwke0_field_t PWR_PDWKE0_f;
      };
      union
      {
          __IO uint8_t PWR_PDWKE1;
          stc_sysreg_pwr_pdwke1_field_t PWR_PDWKE1_f;
      };
      union
      {
          __IO uint8_t PWR_PDWKE2;
          stc_sysreg_pwr_pdwke2_field_t PWR_PDWKE2_f;
      };
      union
      {
          __IO uint8_t PWR_PDWKES;
          stc_sysreg_pwr_pdwkes_field_t PWR_PDWKES_f;
      };
      union
      {
          __IO uint8_t PWR_PDWKF0;
          stc_sysreg_pwr_pdwkf0_field_t PWR_PDWKF0_f;
      };
      union
      {
          __IO uint8_t PWR_PDWKF1;
          stc_sysreg_pwr_pdwkf1_field_t PWR_PDWKF1_f;
      };
      union
      {
          __IO uint8_t PWR_PWCMR;
          stc_sysreg_pwr_pwcmr_field_t PWR_PWCMR_f;
      };
      uint8_t RESERVED40[4];
      __IO uint8_t PWR_MDSWCR;
      union
      {
          __IO uint8_t CMU_XTALCFGR;
          stc_sysreg_cmu_xtalcfgr_field_t CMU_XTALCFGR_f;
      };
      uint8_t RESERVED42[1];
      union
      {
          __IO uint8_t PWR_PVDCR0;
          stc_sysreg_pwr_pvdcr0_field_t PWR_PVDCR0_f;
      };
      union
      {
          __IO uint8_t PWR_PVDCR1;
          stc_sysreg_pwr_pvdcr1_field_t PWR_PVDCR1_f;
      };
      union
      {
          __IO uint8_t PWR_PVDFCR;
          stc_sysreg_pwr_pvdfcr_field_t PWR_PVDFCR_f;
      };
      union
      {
          __IO uint8_t PWR_PVDLCR;
          stc_sysreg_pwr_pvdlcr_field_t PWR_PVDLCR_f;
      };
      uint8_t RESERVED46[10];
      union
      {
          __IO uint8_t CMU_XTAL32CR;
          stc_sysreg_cmu_xtal32cr_field_t CMU_XTAL32CR_f;
      };
      union
      {
          __IO uint8_t CMU_XTAL32CFGR;
          stc_sysreg_cmu_xtal32cfgr_field_t CMU_XTAL32CFGR_f;
      };
      uint8_t RESERVED48[3];
      union
      {
          __IO uint8_t CMU_XTAL32NFR;
          stc_sysreg_cmu_xtal32nfr_field_t CMU_XTAL32NFR_f;
      };
      uint8_t RESERVED49[1];
      union
      {
          __IO uint8_t CMU_LRCCR;
          stc_sysreg_cmu_lrccr_field_t CMU_LRCCR_f;
      };
      uint8_t RESERVED50[1];
      __IO uint8_t CMU_LRCTRM;
      uint8_t RESERVED51[1];
      union
      {
          __IO uint8_t PWR_XTAL32CS;
          stc_sysreg_pwr_xtal32cs_field_t PWR_XTAL32CS_f;
      };
  }M4_SYSREG_TypeDef;
#define M4_SYSREG                               ((M4_SYSREG_TypeDef *)0x40054000UL)

#define ENABLE_CLOCK_REG_WRITE()            (M4_SYSREG->PWR_FPRC = 0xa501u)
#define DISABLE_CLOCK_REG_WRITE()           (M4_SYSREG->PWR_FPRC = 0xa500u)
 /**
  *******************************************************************************
  ** \brief  Select pll clock source.
  **
  ** \param  [in] enPllSrc               The pll clock source.
  ** \arg    ClkPllSrcXTAL               Select XTAL as pll clock source.
  ** \arg    ClkPllSrcHRC                Select HRC as pll clock source.
  **
  ** \retval None
  **
  ** \note   None
  **
  ******************************************************************************/
 void CLK_SetPllSource(en_clk_pll_source_t enPllSrc)
 {
//     DDL_ASSERT(IS_PLL_SOURCE(enPllSrc));

     ENABLE_CLOCK_REG_WRITE();

     M4_SYSREG->CMU_PLLCFGR_f.PLLSRC = enPllSrc;

     DISABLE_CLOCK_REG_WRITE();
 }

 typedef struct
 {
     __IO uint32_t FAPRT                     :16;
     uint32_t RESERVED16                     :16;
 } stc_efm_faprt_field_t;

 typedef struct
 {
     __IO uint32_t FSTP                      : 1;
     uint32_t RESERVED1                      :31;
 } stc_efm_fstp_field_t;

 typedef struct
 {
     __IO uint32_t SLPMD                     : 1;
     uint32_t RESERVED1                      : 3;
     __IO uint32_t FLWT                      : 4;
     __IO uint32_t LVM                       : 1;
     uint32_t RESERVED9                      : 7;
     __IO uint32_t CACHE                     : 1;
     uint32_t RESERVED17                     : 7;
     __IO uint32_t CRST                      : 1;
     uint32_t RESERVED25                     : 7;
 } stc_efm_frmc_field_t;

 typedef struct
 {
     __IO uint32_t PEMODE                    : 1;
     uint32_t RESERVED1                      : 3;
     __IO uint32_t PEMOD                     : 3;
     uint32_t RESERVED7                      : 1;
     __IO uint32_t BUSHLDCTL                 : 1;
     uint32_t RESERVED9                      :23;
 } stc_efm_fwmc_field_t;

 typedef struct
 {
     __IO uint32_t PEWERR                    : 1;
     __IO uint32_t PEPRTERR                  : 1;
     __IO uint32_t PGSZERR                   : 1;
     __IO uint32_t PGMISMTCH                 : 1;
     __IO uint32_t OPTEND                    : 1;
     __IO uint32_t RDCOLERR                  : 1;
     uint32_t RESERVED6                      : 2;
     __IO uint32_t RDY                       : 1;
     uint32_t RESERVED9                      :23;
 } stc_efm_fsr_field_t;

 typedef struct
 {
     __IO uint32_t PEWERRCLR                 : 1;
     __IO uint32_t PEPRTERRCLR               : 1;
     __IO uint32_t PGSZERRCLR                : 1;
     __IO uint32_t PGMISMTCHCLR              : 1;
     __IO uint32_t OPTENDCLR                 : 1;
     __IO uint32_t RDCOLERRCLR               : 1;
     uint32_t RESERVED6                      :26;
 } stc_efm_fsclr_field_t;

 typedef struct
 {
     __IO uint32_t PEERRITE                  : 1;
     __IO uint32_t OPTENDITE                 : 1;
     __IO uint32_t RDCOLERRITE               : 1;
     uint32_t RESERVED3                      :29;
 } stc_efm_fite_field_t;

 typedef struct
 {
     __IO uint32_t FSWP                      : 1;
     uint32_t RESERVED1                      :31;
 } stc_efm_fswp_field_t;

 typedef struct
 {
     __IO uint32_t FPMTSW                    :20;
     uint32_t RESERVED20                     :12;
 } stc_efm_fpmtsw_field_t;

 typedef struct
 {
     __IO uint32_t FPMTEW                    :20;
     uint32_t RESERVED20                     :12;
 } stc_efm_fpmtew_field_t;

 typedef struct
 {
     __IO uint32_t REMPRT                    :16;
     uint32_t RESERVED16                     :16;
 } stc_efm_mmf_remprt_field_t;

 typedef struct
 {
     __IO uint32_t RM0SIZE                   : 5;
     uint32_t RESERVED5                      : 7;
     __IO uint32_t RM0TADDR                  :17;
     uint32_t RESERVED29                     : 2;
     __IO uint32_t EN0                       : 1;
 } stc_efm_mmf_remcr0_field_t;

 typedef struct
 {
     __IO uint32_t RM1SIZE                   : 5;
     uint32_t RESERVED5                      : 7;
     __IO uint32_t RM1TADDR                  :17;
     uint32_t RESERVED29                     : 2;
     __IO uint32_t EN1                       : 1;
 } stc_efm_mmf_remcr1_field_t;

 typedef struct
 {
     __IO uint32_t PORTINEN                  : 1;
     __IO uint32_t CMPEN                     : 3;
     uint32_t RESERVED4                      : 1;
     __IO uint32_t OSCSTPEN                  : 1;
     __IO uint32_t PWMSEL                    : 3;
     uint32_t RESERVED9                      :19;
     __IO uint32_t NFSEL                     : 2;
     __IO uint32_t NFEN                      : 1;
     __IO uint32_t INVSEL                    : 1;
 } stc_emb_ctl_field_t;

 typedef struct
 {
     __IO uint32_t PWMLV                     : 3;
     uint32_t RESERVED3                      :29;
 } stc_emb_pwmlv_field_t;

 typedef struct
 {
     __IO uint32_t SOE                       : 1;
     uint32_t RESERVED1                      :31;
 } stc_emb_soe_field_t;

 typedef struct
 {
     __IO uint32_t PORTINF                   : 1;
     __IO uint32_t PWMSF                     : 1;
     __IO uint32_t CMPF                      : 1;
     __IO uint32_t OSF                       : 1;
     __IO uint32_t PORTINST                  : 1;
     __IO uint32_t PWMST                     : 1;
     uint32_t RESERVED6                      :26;
 } stc_emb_stat_field_t;

 typedef struct
 {
     __IO uint32_t PORTINFCLR                : 1;
     __IO uint32_t PWMSFCLR                  : 1;
     __IO uint32_t CMPFCLR                   : 1;
     __IO uint32_t OSFCLR                    : 1;
     uint32_t RESERVED4                      :28;
 } stc_emb_statclr_field_t;

 typedef struct
 {
     __IO uint32_t PORTINTEN                 : 1;
     __IO uint32_t PWMINTEN                  : 1;
     __IO uint32_t CMPINTEN                  : 1;
     __IO uint32_t OSINTEN                   : 1;
     uint32_t RESERVED4                      :28;
 } stc_emb_inten_field_t;
 typedef struct
 {
     union
     {
         __IO uint32_t FAPRT;
         stc_efm_faprt_field_t FAPRT_f;
     };
     union
     {
         __IO uint32_t FSTP;
         stc_efm_fstp_field_t FSTP_f;
     };
     union
     {
         __IO uint32_t FRMC;
         stc_efm_frmc_field_t FRMC_f;
     };
     union
     {
         __IO uint32_t FWMC;
         stc_efm_fwmc_field_t FWMC_f;
     };
     union
     {
         __IO uint32_t FSR;
         stc_efm_fsr_field_t FSR_f;
     };
     union
     {
         __IO uint32_t FSCLR;
         stc_efm_fsclr_field_t FSCLR_f;
     };
     union
     {
         __IO uint32_t FITE;
         stc_efm_fite_field_t FITE_f;
     };
     union
     {
         __IO uint32_t FSWP;
         stc_efm_fswp_field_t FSWP_f;
     };
     union
     {
         __IO uint32_t FPMTSW;
         stc_efm_fpmtsw_field_t FPMTSW_f;
     };
     union
     {
         __IO uint32_t FPMTEW;
         stc_efm_fpmtew_field_t FPMTEW_f;
     };
     uint8_t RESERVED10[40];
     __IO uint32_t UQID1;
     __IO uint32_t UQID2;
     __IO uint32_t UQID3;
     uint8_t RESERVED13[164];
     union
     {
         __IO uint32_t MMF_REMPRT;
         stc_efm_mmf_remprt_field_t MMF_REMPRT_f;
     };
     union
     {
         __IO uint32_t MMF_REMCR0;
         stc_efm_mmf_remcr0_field_t MMF_REMCR0_f;
     };
     union
     {
         __IO uint32_t MMF_REMCR1;
         stc_efm_mmf_remcr1_field_t MMF_REMCR1_f;
     };
 }M4_EFM_TypeDef;

#define M4_EFM                                  ((M4_EFM_TypeDef *)0x40010400UL)
 /**
  *******************************************************************************
  ** \brief Sets the code latency value..
  **
  ** \param  [in] u32Latency             specifies the FLASH Latency value.
  ** \arg    EFM_LATENCY_0               FLASH 0 Latency cycle
  ** \arg    EFM_LATENCY_1               FLASH 1 Latency cycle
  ** \arg    EFM_LATENCY_2               FLASH 2 Latency cycles
  ** \arg    EFM_LATENCY_3               FLASH 3 Latency cycles
  ** \arg    EFM_LATENCY_4               FLASH 4 Latency cycles
  ** \arg    EFM_LATENCY_5               FLASH 5 Latency cycles
  ** \arg    EFM_LATENCY_6               FLASH 6 Latency cycles
  ** \arg    EFM_LATENCY_7               FLASH 7 Latency cycles
  ** \arg    EFM_LATENCY_8               FLASH 8 Latency cycles
  ** \arg    EFM_LATENCY_9               FLASH 9 Latency cycles
  ** \arg    EFM_LATENCY_10              FLASH 10 Latency cycles
  ** \arg    EFM_LATENCY_11              FLASH 11 Latency cycles
  ** \arg    EFM_LATENCY_12              FLASH 12 Latency cycles
  ** \arg    EFM_LATENCY_13              FLASH 13 Latency cycles
  ** \arg    EFM_LATENCY_14              FLASH 14 Latency cycles
  ** \arg    EFM_LATENCY_15              FLASH 15 Latency cycles
  **
  ** \retval None
  **
  ** \note   None
  **
  ******************************************************************************/
 void EFM_SetLatency(uint32_t u32Latency)
 {
//     DDL_ASSERT(IS_VALID_FLASH_LATENCY(u32Latency));

     M4_EFM->FRMC_f.FLWT = u32Latency;
 }

#define EFM_LATENCY_5                   (5ul)

 /**
  *******************************************************************************
  ** \brief  The clock flag status.
  **
  ******************************************************************************/
 typedef enum en_clk_flag
 {
     ClkFlagHRCRdy                   = 0u,   ///< The clock flag is HRC ready.
     ClkFlagXTALRdy                  = 1u,   ///< The clock flag is XTAL ready.
     ClkFlagMPLLRdy                  = 2u,   ///< The clock flag is MPLL ready.
     ClkFlagUPLLRdy                  = 3u,   ///< The clock flag is UPLL ready.
     ClkFlagXTALStoppage             = 4u,   ///< The clock flag is XTAL stoppage.
 }en_clk_flag_t;

 /*! Memory clear */
 #define MEM_ZERO_STRUCT(x)              do {                                   \
                                         memset((void*)&(x), 0l, (sizeof(x)));  \
                                         }while(0)
 typedef struct
 {
     __IO uint32_t RAMHS                     : 1;
     uint32_t RESERVED1                      : 3;
     __IO uint32_t RAM0                      : 1;
     uint32_t RESERVED5                      : 3;
     __IO uint32_t ECCRAM                    : 1;
     uint32_t RESERVED9                      : 1;
     __IO uint32_t RETRAM                    : 1;
     uint32_t RESERVED11                     : 3;
     __IO uint32_t DMA1                      : 1;
     __IO uint32_t DMA2                      : 1;
     __IO uint32_t FCM                       : 1;
     __IO uint32_t AOS                       : 1;
     uint32_t RESERVED18                     : 2;
     __IO uint32_t AES                       : 1;
     __IO uint32_t HASH                      : 1;
     __IO uint32_t TRNG                      : 1;
     __IO uint32_t CRC                       : 1;
     __IO uint32_t DCU1                      : 1;
     __IO uint32_t DCU2                      : 1;
     __IO uint32_t DCU3                      : 1;
     __IO uint32_t DCU4                      : 1;
     uint32_t RESERVED28                     : 3;
     __IO uint32_t KEY                       : 1;
 } stc_mstp_fcg0_field_t;

 typedef struct
 {
     __IO uint32_t CAN                       : 1;
     uint32_t RESERVED1                      : 2;
     __IO uint32_t QSPI                      : 1;
     __IO uint32_t IIC1                      : 1;
     __IO uint32_t IIC2                      : 1;
     __IO uint32_t IIC3                      : 1;
     uint32_t RESERVED7                      : 1;
     __IO uint32_t USBFS                     : 1;
     uint32_t RESERVED9                      : 1;
     __IO uint32_t SDIOC1                    : 1;
     __IO uint32_t SDIOC2                    : 1;
     __IO uint32_t I2S1                      : 1;
     __IO uint32_t I2S2                      : 1;
     __IO uint32_t I2S3                      : 1;
     __IO uint32_t I2S4                      : 1;
     __IO uint32_t SPI1                      : 1;
     __IO uint32_t SPI2                      : 1;
     __IO uint32_t SPI3                      : 1;
     __IO uint32_t SPI4                      : 1;
     uint32_t RESERVED20                     : 4;
     __IO uint32_t USART1                    : 1;
     __IO uint32_t USART2                    : 1;
     __IO uint32_t USART3                    : 1;
     __IO uint32_t USART4                    : 1;
     uint32_t RESERVED28                     : 4;
 } stc_mstp_fcg1_field_t;

 typedef struct
 {
     __IO uint32_t TIMER0_1                  : 1;
     __IO uint32_t TIMER0_2                  : 1;
     __IO uint32_t TIMERA_1                  : 1;
     __IO uint32_t TIMERA_2                  : 1;
     __IO uint32_t TIMERA_3                  : 1;
     __IO uint32_t TIMERA_4                  : 1;
     __IO uint32_t TIMERA_5                  : 1;
     __IO uint32_t TIMERA_6                  : 1;
     __IO uint32_t TIMER4_1                  : 1;
     __IO uint32_t TIMER4_2                  : 1;
     __IO uint32_t TIMER4_3                  : 1;
     uint32_t RESERVED11                     : 4;
     __IO uint32_t EMB                       : 1;
     __IO uint32_t TIMER6_1                  : 1;
     __IO uint32_t TIMER6_2                  : 1;
     __IO uint32_t TIMER6_3                  : 1;
     uint32_t RESERVED19                     :13;
 } stc_mstp_fcg2_field_t;

 typedef struct
 {
     __IO uint32_t ADC1                      : 1;
     __IO uint32_t ADC2                      : 1;
     uint32_t RESERVED2                      : 2;
     __IO uint32_t DAC                       : 1;
     uint32_t RESERVED5                      : 3;
     __IO uint32_t CMP                       : 1;
     uint32_t RESERVED9                      : 3;
     __IO uint32_t OTS                       : 1;
     uint32_t RESERVED13                     :19;
 } stc_mstp_fcg3_field_t;

 typedef struct
 {
     __IO uint32_t PRT0                      : 1;
     uint32_t RESERVED1                      :15;
     __IO uint32_t FCG0PCWE                  :16;
 } stc_mstp_fcg0pc_field_t;

 typedef struct
 {
     union
     {
         __IO uint32_t FCG0;
         stc_mstp_fcg0_field_t FCG0_f;
     };
     union
     {
         __IO uint32_t FCG1;
         stc_mstp_fcg1_field_t FCG1_f;
     };
     union
     {
         __IO uint32_t FCG2;
         stc_mstp_fcg2_field_t FCG2_f;
     };
     union
     {
         __IO uint32_t FCG3;
         stc_mstp_fcg3_field_t FCG3_f;
     };
     union
     {
         __IO uint32_t FCG0PC;
         stc_mstp_fcg0pc_field_t FCG0PC_f;
     };
 }M4_MSTP_TypeDef;

#define M4_MSTP                                 ((M4_MSTP_TypeDef *)0x40048000UL)


 void CLK_XtalConfig(const stc_clk_xtal_cfg_t *pstcXtalCfg)
 {
     if(NULL != pstcXtalCfg)
     {
         ENABLE_CLOCK_REG_WRITE();

         M4_SYSREG->CMU_XTALCFGR_f.SUPDRV = pstcXtalCfg->enFastStartup;
         M4_SYSREG->CMU_XTALCFGR_f.XTALMS = pstcXtalCfg->enMode;
         M4_SYSREG->CMU_XTALCFGR_f.XTALDRV = pstcXtalCfg->enDrv;

         DISABLE_CLOCK_REG_WRITE();
     }
     else
     {
         /* code */
     }
 }
#define CLK_FCG_STABLE                      ((uint16_t)0xB0u)
#define CLK_SYSCLK_STABLE                   ((uint16_t)0xB0u)
#define DEFAULT_FCG0                        (0xFFFFFAEEul)
#define DEFAULT_FCG1                        (0xFFFFFFFFul)
#define DEFAULT_FCG2                        (0xFFFFFFFFul)
#define DEFAULT_FCG3                        (0xFFFFFFFFul)
 void CLK_SysClkConfig(const stc_clk_sysclk_cfg_t *pstcSysclkCfg)
 {
     __IO uint32_t timeout = 0ul;
     __IO uint32_t fcg0 = M4_MSTP->FCG0;
     __IO uint32_t fcg1 = M4_MSTP->FCG1;
     __IO uint32_t fcg2 = M4_MSTP->FCG2;
     __IO uint32_t fcg3 = M4_MSTP->FCG3;

     if(NULL != pstcSysclkCfg)
     {
//         DDL_ASSERT(IS_SYSCLK_CONFIG_VALID(pstcSysclkCfg));

         /* Only current system clock source is MPLL need to close fcg0~fcg3 and
         open fcg0~fcg3 during switch system clock division.
         We need to backup fcg0~fcg3 before close them. */
         if(CLKSysSrcMPLL == M4_SYSREG->CMU_CKSWR_f.CKSW)
         {
             /* Close fcg0~fcg3. */
             M4_MSTP->FCG0 = DEFAULT_FCG0;
             M4_MSTP->FCG1 = DEFAULT_FCG1;
             M4_MSTP->FCG2 = DEFAULT_FCG2;
             M4_MSTP->FCG3 = DEFAULT_FCG3;

             /* Wait stable after close fcg. */
             do
             {
                 timeout++;
             }while(timeout < CLK_FCG_STABLE);
         }

         /* Switch to target system clock division. */
         ENABLE_CLOCK_REG_WRITE();

 				uint32_t addr = (uint32_t)(&(M4_SYSREG->PWR_FPRC));

         M4_SYSREG->CMU_SCFGR = ( (uint32_t)pstcSysclkCfg->enPclk0Div           |
                                 ((uint32_t)pstcSysclkCfg->enPclk1Div << 4u)    |
                                 ((uint32_t)pstcSysclkCfg->enPclk2Div << 8u)    |
                                 ((uint32_t)pstcSysclkCfg->enPclk3Div << 12u)   |
                                 ((uint32_t)pstcSysclkCfg->enPclk4Div << 16u)   |
                                 ((uint32_t)pstcSysclkCfg->enExclkDiv << 20u)   |
                                 ((uint32_t)pstcSysclkCfg->enHclkDiv << 24u));

         DISABLE_CLOCK_REG_WRITE();

         timeout = 0ul;
         do
         {
             timeout++;
         }while(timeout < CLK_SYSCLK_STABLE);

         /* Open fcg0~fcg3. */
         M4_MSTP->FCG0 = fcg0;
         M4_MSTP->FCG1 = fcg1;
         M4_MSTP->FCG2 = fcg2;
         M4_MSTP->FCG3 = fcg3;

         /* Wait stable after open fcg. */
         timeout = 0ul;
         do
         {
             timeout++;
         }while(timeout < CLK_FCG_STABLE);
     }
     else
     {
         /* code */
     }
 }

#define CLK_XTAL_TIMEOUT                    ((uint16_t)0x1000u)
#define ENABLE_CLOCK_REG_WRITE()            (M4_SYSREG->PWR_FPRC = 0xa501u)
#define DISABLE_CLOCK_REG_WRITE()           (M4_SYSREG->PWR_FPRC = 0xa500u)

 en_flag_status_t CLK_GetFlagStatus(en_clk_flag_t enClkFlag)
 {
     en_flag_status_t status;

//     DDL_ASSERT(IS_CLK_FLAG(enClkFlag));

     switch(enClkFlag)
     {
         case ClkFlagHRCRdy:
             status = ((1u == M4_SYSREG->CMU_OSCSTBSR_f.HRCSTBF) ? Set : Reset);
             break;
         case ClkFlagXTALRdy:
             status = ((1u == M4_SYSREG->CMU_OSCSTBSR_f.XTALSTBF) ? Set : Reset);
             break;
         case ClkFlagMPLLRdy:
             status = ((1u == M4_SYSREG->CMU_OSCSTBSR_f.MPLLSTBF) ? Set : Reset);
             break;
         case ClkFlagUPLLRdy:
             status = ((1u == M4_SYSREG->CMU_OSCSTBSR_f.UPLLSTBF) ? Set : Reset);
             break;
         default:
             status = ((1u == M4_SYSREG->CMU_XTALSTDSR_f.XTALSTDF) ? Set : Reset);
             break;
     }

     return status;
 }
 en_result_t CLK_XtalCmd(en_functional_state_t enNewState)
 {
     __IO uint32_t timeout = 0u;
     en_flag_status_t status;
     en_result_t enRet = Ok;

//     DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

//     DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

     ENABLE_CLOCK_REG_WRITE();

     if(Disable == enNewState)
     {
         if(ClkSysSrcXTAL == M4_SYSREG->CMU_CKSWR_f.CKSW)
         {
             enRet = Error;
         }
         else if(ClkPllSrcXTAL == M4_SYSREG->CMU_PLLCFGR_f.PLLSRC)
         {
             if(0u == M4_SYSREG->CMU_PLLCR_f.MPLLOFF)
             {
                 enRet = Error;
             }
             else
             {
                 M4_SYSREG->CMU_XTALCR_f.XTALSTP = 1u;
             }
         }
         else
         {
             M4_SYSREG->CMU_XTALCR_f.XTALSTP = 1u;
         }
     }
     else
     {
          M4_SYSREG->CMU_XTALCR_f.XTALSTP = 0u;
          do
         {
             status = CLK_GetFlagStatus(ClkFlagXTALRdy);
             timeout++;
         }while((timeout < CLK_XTAL_TIMEOUT) && (status != Set));
     }

     DISABLE_CLOCK_REG_WRITE();

     return enRet;
 }
#if !defined (HRC_VALUE)
#define HRC_VALUE ((uint32_t)16000000)  /*!< Internal high speed RC freq. */
#endif
#if !defined (XTAL_VALUE)
#define XTAL_VALUE ((uint32_t)8000000)  /*!< External high speed OSC freq. */
#endif
 void CLK_MpllConfig(const stc_clk_mpll_cfg_t *pstcMpllCfg)
 {
     uint32_t vcoIn = 0ul;
     uint32_t vcoOut = 0ul;

     if(NULL != pstcMpllCfg)
     {
//         DDL_ASSERT(IS_PLL_DIV_VALID(pstcMpllCfg->PllpDiv));
//         DDL_ASSERT(IS_PLLQ_DIV_VALID(pstcMpllCfg->PllqDiv));
//         DDL_ASSERT(IS_PLL_DIV_VALID(pstcMpllCfg->PllrDiv));
//         DDL_ASSERT(IS_PLLN_VALID(pstcMpllCfg->plln));
//         DDL_ASSERT(IS_PLLM_VALID(pstcMpllCfg->pllmDiv));

         vcoIn = ((ClkPllSrcXTAL == M4_SYSREG->CMU_PLLCFGR_f.PLLSRC ?
                 XTAL_VALUE : HRC_VALUE) / pstcMpllCfg->pllmDiv);
         vcoOut = vcoIn * pstcMpllCfg->plln;

//         DDL_ASSERT(IS_PLL_VCO_IN_VALID(vcoIn));
//         DDL_ASSERT(IS_PLL_VCO_OUT_VALID(vcoOut));

         ENABLE_CLOCK_REG_WRITE();

         M4_SYSREG->CMU_PLLCFGR_f.MPLLP = pstcMpllCfg->PllpDiv - 1ul;
         M4_SYSREG->CMU_PLLCFGR_f.MPLLQ = pstcMpllCfg->PllqDiv - 1ul;
         M4_SYSREG->CMU_PLLCFGR_f.MPLLR = pstcMpllCfg->PllrDiv - 1ul;
         M4_SYSREG->CMU_PLLCFGR_f.MPLLN = pstcMpllCfg->plln - 1ul;
         M4_SYSREG->CMU_PLLCFGR_f.MPLLM = pstcMpllCfg->pllmDiv - 1ul;

         DISABLE_CLOCK_REG_WRITE();
     }
     else
     {
         /* code */
     }
 }
 /*******************************************************************************
  * Local pre-processor symbols/macros ('#define')
  ******************************************************************************/
 #define EFM_LOCK                            (0x00000000u)
 #define EFM_UNLOCK                          (0x00000001u)
 #define EFM_KEY1                            (0x0123ul)
 #define EFM_KEY2                            (0x3210ul)
 void EFM_Unlock(void)
 {
     M4_EFM->FAPRT = EFM_KEY1;
     M4_EFM->FAPRT = EFM_KEY2;
 }
 void EFM_Lock(void)
 {
     if(EFM_UNLOCK == M4_EFM->FAPRT)
     {
         M4_EFM->FAPRT = EFM_KEY2;
         M4_EFM->FAPRT = EFM_KEY2;
     }
 }
#define CLK_MPLL_TIMEOUT                    ((uint16_t)0x1000u)
 en_result_t CLK_MpllCmd(en_functional_state_t enNewState)
 {
     __IO uint32_t timeout = 0ul;
     en_flag_status_t status;
     en_result_t enRet = Ok;

//     DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

     ENABLE_CLOCK_REG_WRITE();

     if(Disable == enNewState)
     {
         if(CLKSysSrcMPLL == M4_SYSREG->CMU_CKSWR_f.CKSW)
         {
             enRet = Error;
         }
         else
         {
             M4_SYSREG->CMU_PLLCR_f.MPLLOFF = 1u;
         }
     }
     else
     {
         M4_SYSREG->CMU_PLLCR_f.MPLLOFF = 0u;
         do
         {
             status = CLK_GetFlagStatus(ClkFlagMPLLRdy);
             timeout++;
         }while((timeout < CLK_MPLL_TIMEOUT) && (status != Set));
     }

     DISABLE_CLOCK_REG_WRITE();

     return enRet;
 }
 void CLK_SetSysClkSource(en_clk_sys_source_t enTargetSysSrc)
 {
     __IO uint32_t timeout = 0ul;
     __IO uint32_t fcg0 = M4_MSTP->FCG0;
     __IO uint32_t fcg1 = M4_MSTP->FCG1;
     __IO uint32_t fcg2 = M4_MSTP->FCG2;
     __IO uint32_t fcg3 = M4_MSTP->FCG3;

//     DDL_ASSERT(IS_SYSCLK_SOURCE(enTargetSysSrc));

     /* Only current system clock source or target system clock source is MPLL
     need to close fcg0~fcg3 and open fcg0~fcg3 during switch system clock source.
     We need to backup fcg0~fcg3 before close them. */
     if((CLKSysSrcMPLL == M4_SYSREG->CMU_CKSWR_f.CKSW) ||
         (CLKSysSrcMPLL == enTargetSysSrc))
     {
         /* Close fcg0~fcg3. */
         M4_MSTP->FCG0 = DEFAULT_FCG0;
         M4_MSTP->FCG1 = DEFAULT_FCG1;
         M4_MSTP->FCG2 = DEFAULT_FCG2;
         M4_MSTP->FCG3 = DEFAULT_FCG3;

         /* Wait stable after close fcg. */
         do
         {
             timeout++;
         }while(timeout < CLK_FCG_STABLE);
     }

     /* Switch to target system clock source. */
     ENABLE_CLOCK_REG_WRITE();

     M4_SYSREG->CMU_CKSWR_f.CKSW = enTargetSysSrc;

     DISABLE_CLOCK_REG_WRITE();

     timeout = 0ul;
     do
     {
         timeout++;
     }while(timeout < CLK_SYSCLK_STABLE);

     /* Open fcg0~fcg3. */
     M4_MSTP->FCG0 = fcg0;
     M4_MSTP->FCG1 = fcg1;
     M4_MSTP->FCG2 = fcg2;
     M4_MSTP->FCG3 = fcg3;

     /* Wait stable after open fcg. */
     timeout = 0ul;
     do
     {
         timeout++;
     }while(timeout < CLK_FCG_STABLE);

     SystemCoreClockUpdate();
 }
/**
 *******************************************************************************
 ** \brief Initialize Clock.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void ClkInit(void)
{
    stc_clk_xtal_cfg_t   stcXtalCfg;
    stc_clk_mpll_cfg_t   stcMpllCfg;
    en_clk_sys_source_t  enSysClkSrc;
    stc_clk_sysclk_cfg_t stcSysClkCfg;

    MEM_ZERO_STRUCT(enSysClkSrc);
    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv  = ClkSysclkDiv1;  /* Max 168MHz */
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;  /* Max 84MHz */
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;  /* Max 168MHz */
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;  /* Max 84MHz */
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;  /* Max 60MHz */
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;  /* Max 42MHz */
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;  /* Max 84MHz */
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Switch system clock source to MPLL. */
    /* Use Xtal as MPLL source. */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* MPLL config. */
    stcMpllCfg.pllmDiv = 1ul;
    stcMpllCfg.plln = 50ul;
    stcMpllCfg.PllpDiv = 4ul;
    stcMpllCfg.PllqDiv = 4ul;
    stcMpllCfg.PllrDiv = 4ul;
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_5);
    EFM_Lock();

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);

    /* Wait MPLL ready. */
    while (Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
    }

    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);
}
void PWC_Fcg1PeriphClockCmd(uint32_t u32Fcg1Periph, en_functional_state_t enNewState)
{
//    DDL_ASSERT(IS_PWC_FCG1_PERIPH(u32Fcg1Periph));
//    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        M4_MSTP->FCG1 &= ~u32Fcg1Periph;
    }
    else
    {
        M4_MSTP->FCG1 |= u32Fcg1Periph;
    }
}

/**
 *******************************************************************************
 ** \brief Get USART clock
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval USART clock frequency
 **
 ******************************************************************************/
static uint32_t UsartGetClk(const M4_USART_TypeDef *USARTx)
{
    uint32_t u32PClk1 = 0ul;
    uint32_t u32UartClk = 0ul;

    /* Check USARTx pointer */
//    DDL_ASSERT(IS_VALID_USART(USARTx));

    u32PClk1 = SystemCoreClock / (1ul << M4_SYSREG->CMU_SCFGR_f.PCLK1S);
    u32UartClk = u32PClk1 / (1ul << (2ul * USARTx->PR_f.PSC));

    return u32UartClk;
}

/**
 *******************************************************************************
 ** \brief Set smart card mode baudrate
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u32Baudrate             Baudrate
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
static en_result_t SetScBaudrate(M4_USART_TypeDef *USARTx,
                                uint32_t u32Baudrate)
{
    uint32_t B = 0ul;
    uint32_t C = 0ul;
    uint32_t S = 0ul;
    float DIV = 0.0f;
    uint64_t u64Tmp = 0u;
    uint32_t DIV_Integer = 0ul;
    uint32_t DIV_Fraction = 0xFFFFFFFFul;
    const uint16_t au16EtuClkCnts[] = {32u, 64u, 93u, 128u, 186u, 256u, 372u, 512u};
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
//    if (IS_VALID_USART(USARTx))
//   {
        C = UsartGetClk(USARTx);

        if (C > 0ul)
        {
            B = u32Baudrate;
            S = au16EtuClkCnts[USARTx->CR3_f.BCN];

            /* FBME = 0 Calculation formula */
            /* B = C / (2 * S * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 2 * S)) - 1 */
            DIV = ((float)C / ((float)B * (float)S * 2.0f)) - 1.0f;
            DIV_Integer = (uint32_t)DIV;

            if (!((DIV < 0.0f) || (DIV_Integer > 0xFFul)))
            {
                enRet = Ok;
                if ((DIV - (float)DIV_Integer) > 0.00001f)
                {
                    /* FBME = 1 Calculation formula */
                    /* B = C * (128 + DIV_Fraction) / ((2 * S) * (DIV_Integer + 1) * 256) */
                    /* DIV_Fraction = ((2 * S) * (DIV_Integer + 1) * 256 * B / C) - 128 */
                    /* DIV_Fraction = ((DIV_Integer + 1) * B * S * 512 / C) - 128 */
                    u64Tmp = (uint64_t)(((uint64_t)DIV_Integer + 1ul) * B * S);
                    DIV_Fraction = (uint32_t)(512ul * u64Tmp / C - 128ul);
                    if (DIV_Fraction > 0x7Ful)
                    {
                        enRet = ErrorInvalidParameter;
                    }
                }

                if (Ok == enRet)
                {
                    USARTx->CR1_f.FBME = (DIV_Fraction > 0x7Ful) ? 0ul : 1ul;
                    USARTx->BRR_f.DIV_FRACTION = DIV_Fraction;
                    USARTx->BRR_f.DIV_INTEGER = DIV_Integer;
                }
            }
        }
//    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set synchronous clock mode baudrate
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u32Baudrate             Baudrate
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
static en_result_t SetClkSyncBaudrate(M4_USART_TypeDef *USARTx,
                                uint32_t u32Baudrate)
{
    uint32_t C = 0ul;
    uint32_t B = 0ul;
    uint64_t u64Tmp = 0u;
    float DIV = 0.0f;
    uint32_t DIV_Integer = 0ul;
    uint32_t DIV_Fraction = 0xFFFFFFFFul;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
//    if (IS_VALID_USART(USARTx))
//    {
        C = UsartGetClk(USARTx);
        if (C > 0ul)
        {
            B = u32Baudrate;

            /* FBME = 0 Calculation formula */
            /* B = C / (4 * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 4)) - 1 */
            DIV = ((float)C / ((float)B * 4.0f)) - 1.0f;
            DIV_Integer = (uint32_t)DIV;

            if (!((DIV < 0.0f) || (DIV_Integer > 0xFFul)))
            {
                enRet = Ok;
                if ((DIV - (float)DIV_Integer) > 0.00001f)
                {
                    /* FBME = 1 Calculation formula */
                    /* B = C * (128 + DIV_Fraction) / (4 * (DIV_Integer + 1) * 256) */
                    /* DIV_Fraction = (4 * (DIV_Integer + 1) * 256 * B / C) - 128 */
                    /* DIV_Fraction = ((DIV_Integer + 1) * B * 1024 / C) - 128 */
                    u64Tmp = (uint64_t)(((uint64_t)DIV_Integer + 1ul) * (uint64_t)B);
                    DIV_Fraction = (uint32_t)(1024ul * u64Tmp / C - 128ul);
                    if (DIV_Fraction > 0x7Ful)
                    {
                        enRet = ErrorInvalidParameter;
                    }
                }

                if (Ok == enRet)
                {
                    USARTx->CR1_f.FBME = (DIV_Fraction > 0x7Ful) ? 0ul : 1ul;
                    USARTx->BRR_f.DIV_FRACTION = DIV_Fraction;
                    USARTx->BRR_f.DIV_INTEGER = DIV_Integer;
                }
            }
        }
//    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set UART mode baudrate
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u32Baudrate             Baudrate
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
static en_result_t SetUartBaudrate(M4_USART_TypeDef *USARTx,
                                uint32_t u32Baudrate)
{
    uint32_t B = 0ul;
    uint32_t C = 0ul;
    uint32_t OVER8 = 0ul;
    float DIV = 0.0f;
    uint64_t u64Tmp = 0u;
    uint32_t DIV_Integer = 0ul;
    uint32_t DIV_Fraction = 0xFFFFFFFFul;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
//    if (IS_VALID_USART(USARTx))
//    {
        C = UsartGetClk(USARTx);

        if (C > 0ul)
        {
            B = u32Baudrate;
            OVER8 = USARTx->CR1_f.OVER8;
            /* FBME = 0 Calculation formula */
            /* B = C / (8 * (2 - OVER8) * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 8 * (2 - OVER8))) - 1 */
            DIV = ((float)C / ((float)B * 8.0f * (2.0f - (float)OVER8))) - 1.0f;
            DIV_Integer = (uint32_t)(DIV);

            if (!((DIV < 0.0f) || (DIV_Integer > 0xFFul)))
            {
                enRet = Ok;
                if ((DIV - (float)DIV_Integer) > 0.00001f)
                {
                    /* FBME = 1 Calculation formula */
                    /* B = C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256) */
                    /* DIV_Fraction = ((8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B) / C) - 128 */
                    /* E = (C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B)) - 1 */
                    /* DIV_Fraction = (((2 - OVER8) * (DIV_Integer + 1) * 2048 * B) / C) - 128 */
                    u64Tmp = (uint64_t)(((uint64_t)2ul - (uint64_t)OVER8) * ((uint64_t)DIV_Integer + 1ul) * (uint64_t)B);
                    DIV_Fraction = (uint32_t)(2048ul * u64Tmp / C - 128ul);
                    if (DIV_Fraction > 0x7Ful)
                    {
                        enRet = ErrorInvalidParameter;
                    }
                }

                if (Ok == enRet)
                {
                    USARTx->CR1_f.FBME = (DIV_Fraction > 0x7Ful) ? 0ul : 1ul;
                    USARTx->BRR_f.DIV_FRACTION = DIV_Fraction;
                    USARTx->BRR_f.DIV_INTEGER = DIV_Integer;
                }
            }
        }
//    }

    return enRet;
}

en_result_t USART_SetBaudrate(M4_USART_TypeDef *USARTx,
                                uint32_t u32Baudrate)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
//    if (IS_VALID_USART(USARTx))
//    {
        if(1ul == USARTx->CR3_f.SCEN)
        {
            enRet = SetScBaudrate(USARTx, u32Baudrate);
        }
        else if(1ul == USARTx->CR1_f.MS)
        {
            enRet = SetClkSyncBaudrate(USARTx, u32Baudrate);
        }
        else
        {
            enRet = SetUartBaudrate(USARTx, u32Baudrate);
        }
//    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Configure USART function.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enFunc                  USART function selection
 ** \arg UsartTimeOut                   UART RX timeout function
 ** \arg UsartTimeOutInt                UART RX timeout interrupt function
 ** \arg UsartRx                        UART RX function
 ** \arg UsartTx                        UART TX function
 ** \arg UsartSilentMode                USART silent function
 ** \arg UsartRxInt                     USART RX interrupt function
 ** \arg UsartTxCmpltInt                USART TX complete interrupt function
 ** \arg UsartTxEmptyInt                USART TX empty interrupt function
 ** \arg UsartParityCheck               USART Parity check function
 ** \arg UsartFracBaudrate              USART fractional baudrate function
 ** \arg UsartNoiseFilter               USART noise filter function
 ** \param [in] enCmd                   USART functional state
 ** \arg Enable                         Enable the specified USART function
 ** \arg Disable                        Disable the specified USART function
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx == NULL
 **
 ******************************************************************************/
en_result_t USART_FuncCmd(M4_USART_TypeDef *USARTx,
                                en_usart_func_t enFunc,
                                en_functional_state_t enCmd)
{
    uint32_t u32Addr;
    __IO stc_usart_cr1_field_t CR1_f;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
//    if(IS_VALID_USART(USARTx))
//    {
        enRet = Ok;
        switch(enFunc)
        {
            case UsartRx:
                USARTx->CR1_f.RE = (uint32_t)enCmd;
                break;
            case UsartRxInt:
                USARTx->CR1_f.RIE = (uint32_t)enCmd;
                break;
            case UsartTx:
                USARTx->CR1_f.TE = (uint32_t)enCmd;
                break;
            case UsartTxEmptyInt:
                USARTx->CR1_f.TXEIE = (uint32_t)enCmd;
                break;
            case UsartTimeOut:
                USARTx->CR1_f.RTOE = (uint32_t)enCmd;
                break;
            case UsartTimeOutInt:
                USARTx->CR1_f.RTOIE = (uint32_t)enCmd;
                break;
            case UsartSilentMode:
                USARTx->CR1_f.SLME = (uint32_t)enCmd;
                break;
            case UsartParityCheck:
                USARTx->CR1_f.PCE = (uint32_t)enCmd;
                break;
            case UsartNoiseFilter:
                USARTx->CR1_f.NFE = (uint32_t)enCmd;
                break;
            case UsartTxCmpltInt:
                USARTx->CR1_f.TCIE = (uint32_t)enCmd;
                break;
            case UsartTxAndTxEmptyInt:
                CR1_f = USARTx->CR1_f;
                CR1_f.TE = (uint32_t)enCmd;
                CR1_f.TXEIE = (uint32_t)enCmd;
                u32Addr = (uint32_t)&CR1_f;
                USARTx->CR1 = *(__IO uint32_t *)u32Addr;
                break;
            case UsartFracBaudrate:
                USARTx->CR1_f.FBME = (uint32_t)enCmd;
                break;
            case UsartMulProcessor:
                USARTx->CR2_f.MPE = (uint32_t)enCmd;
                break;
            case UsartSmartCard:
                USARTx->CR3_f.SCEN = (uint32_t)enCmd;
                break;
            case UsartCts:
                USARTx->CR3_f.CTSE = (uint32_t)enCmd;
                break;
            default:
                enRet = ErrorInvalidParameter;
                break;
        }
//    }

    return enRet;
}
