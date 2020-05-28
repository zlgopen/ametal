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
 * \brief HC32F460 DMA 通道编号定义
 *
 * \internal
 * \par Modification History
 *
 * - 1.00 2020-03-02  cds, first implementation
 *
 * \endinternal
 */

#ifndef __HC32F460_DMA_CHAN_H
#define __HC32F460_DMA_CHAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup hc32f460_if_dma_chan
 * \copydoc hc32f460_dma_chan.h
 * @{
 */

/**
 * \name hc32f460 DMA 通道编号
 * @{
 */
#define DMA_CHAN_1                0               /**< \brief DMA通道1 */
#define DMA_CHAN_2                1               /**< \brief DMA通道2 */
#define DMA_CHAN_3                2               /**< \brief DMA通道3 */
#define DMA_CHAN_4                3               /**< \brief DMA通道4 */

/** @} */

/**
 * \name 启动对应通道进行传输的事件编号
 * \anchor dma_request_src
 * @{
 */
/**
 *******************************************************************************
 ** \brief Event number enumeration
 ******************************************************************************/
typedef enum en_event_src
{
    EVT_SWI_IRQ0            = 0u,
    EVT_SWI_IRQ1            = 1u,
    EVT_SWI_IRQ2            = 2u,
    EVT_SWI_IRQ3            = 3u,
    EVT_SWI_IRQ4            = 4u,
    EVT_SWI_IRQ5            = 5u,
    EVT_SWI_IRQ6            = 6u,
    EVT_SWI_IRQ7            = 7u,
    EVT_SWI_IRQ8            = 8u,
    EVT_SWI_IRQ9            = 9u,
    EVT_SWI_IRQ10           = 10u,
    EVT_SWI_IRQ11           = 11u,
    EVT_SWI_IRQ12           = 12u,
    EVT_SWI_IRQ13           = 13u,
    EVT_SWI_IRQ14           = 14u,
    EVT_SWI_IRQ15           = 15u,
    EVT_SWI_IRQ16           = 16u,
    EVT_SWI_IRQ17           = 17u,
    EVT_SWI_IRQ18           = 18u,
    EVT_SWI_IRQ19           = 19u,
    EVT_SWI_IRQ20           = 20u,
    EVT_SWI_IRQ21           = 21u,
    EVT_SWI_IRQ22           = 22u,
    EVT_SWI_IRQ23           = 23u,
    EVT_SWI_IRQ24           = 24u,
    EVT_SWI_IRQ25           = 25u,
    EVT_SWI_IRQ26           = 26u,
    EVT_SWI_IRQ27           = 27u,
    EVT_SWI_IRQ28           = 28u,
    EVT_SWI_IRQ29           = 29u,
    EVT_SWI_IRQ30           = 30u,
    EVT_SWI_IRQ31           = 31u,

    /* External Interrupt. */
    EVT_PORT_EIRQ0          = 0u,
    EVT_PORT_EIRQ1          = 1u,
    EVT_PORT_EIRQ2          = 2u,
    EVT_PORT_EIRQ3          = 3u,
    EVT_PORT_EIRQ4          = 4u,
    EVT_PORT_EIRQ5          = 5u,
    EVT_PORT_EIRQ6          = 6u,
    EVT_PORT_EIRQ7          = 7u,
    EVT_PORT_EIRQ8          = 8u,
    EVT_PORT_EIRQ9          = 9u,
    EVT_PORT_EIRQ10         = 10u,
    EVT_PORT_EIRQ11         = 11u,
    EVT_PORT_EIRQ12         = 12u,
    EVT_PORT_EIRQ13         = 13u,
    EVT_PORT_EIRQ14         = 14u,
    EVT_PORT_EIRQ15         = 15u,

    /* DMAC */
    EVT_DMA1_TC0            = 32u,
    EVT_DMA1_TC1            = 33u,
    EVT_DMA1_TC2            = 34u,
    EVT_DMA1_TC3            = 35u,
    EVT_DMA2_TC0            = 36u,
    EVT_DMA2_TC1            = 37u,
    EVT_DMA2_TC2            = 38u,
    EVT_DMA2_TC3            = 39u,
    EVT_DMA1_BTC0           = 40u,
    EVT_DMA1_BTC1           = 41u,
    EVT_DMA1_BTC2           = 42u,
    EVT_DMA1_BTC3           = 43u,
    EVT_DMA2_BTC0           = 44u,
    EVT_DMA2_BTC1           = 45u,
    EVT_DMA2_BTC2           = 46u,
    EVT_DMA2_BTC3           = 47u,

    /* EFM */
    EVT_EFM_OPTEND          = 52u,

    /* USB SOF */
    EVT_USBFS_SOF           = 53u,

    /* DCU */
    EVT_DCU1                = 55u,
    EVT_DCU2                = 56u,
    EVT_DCU3                = 57u,
    EVT_DCU4                = 58u,

    /* TIMER 0 */
    EVT_TMR01_GCMA          = 64u,
    EVT_TMR01_GCMB          = 65u,
    EVT_TMR02_GCMA          = 66u,
    EVT_TMR02_GCMB          = 67u,

    /* RTC */
    EVT_RTC_ALM             = 81u,
    EVT_RTC_PRD             = 82u,

    /* TIMER 6 */
    EVT_TMR61_GCMA          = 96u,
    EVT_TMR61_GCMB          = 97u,
    EVT_TMR61_GCMC          = 98u,
    EVT_TMR61_GCMD          = 99u,
    EVT_TMR61_GCME          = 100u,
    EVT_TMR61_GCMF          = 101u,
    EVT_TMR61_GOVF          = 102u,
    EVT_TMR61_GUDF          = 103u,
    EVT_TMR61_SCMA          = 107u,
    EVT_TMR61_SCMB          = 108u,
    EVT_TMR62_GCMA          = 112u,
    EVT_TMR62_GCMB          = 113u,
    EVT_TMR62_GCMC          = 114u,
    EVT_TMR62_GCMD          = 115u,
    EVT_TMR62_GCME          = 116u,
    EVT_TMR62_GCMF          = 117u,
    EVT_TMR62_GOVF          = 118u,
    EVT_TMR62_GUDF          = 119u,
    EVT_TMR62_SCMA          = 123u,
    EVT_TMR62_SCMB          = 124u,
    EVT_TMR63_GCMA          = 128u,
    EVT_TMR63_GCMB          = 129u,
    EVT_TMR63_GCMC          = 130u,
    EVT_TMR63_GCMD          = 131u,
    EVT_TMR63_GCME          = 132u,
    EVT_TMR63_GCMF          = 133u,
    EVT_TMR63_GOVF          = 134u,
    EVT_TMR63_GUDF          = 135u,
    EVT_TMR63_SCMA          = 139u,
    EVT_TMR63_SCMB          = 140u,

    /* TIMER A */
    EVT_TMRA1_OVF           = 256u,
    EVT_TMRA1_UDF           = 257u,
    EVT_TMRA1_CMP           = 258u,
    EVT_TMRA2_OVF           = 259u,
    EVT_TMRA2_UDF           = 260u,
    EVT_TMRA2_CMP           = 261u,
    EVT_TMRA3_OVF           = 262u,
    EVT_TMRA3_UDF           = 263u,
    EVT_TMRA3_CMP           = 264u,
    EVT_TMRA4_OVF           = 265u,
    EVT_TMRA4_UDF           = 266u,
    EVT_TMRA4_CMP           = 267u,
    EVT_TMRA5_OVF           = 268u,
    EVT_TMRA5_UDF           = 269u,
    EVT_TMRA5_CMP           = 270u,
    EVT_TMRA6_OVF           = 272u,
    EVT_TMRA6_UDF           = 273u,
    EVT_TMRA6_CMP           = 274u,

    /* USART */
    EVT_USART1_EI           = 278u,
    EVT_USART1_RI           = 279u,
    EVT_USART1_TI           = 280u,
    EVT_USART1_TCI          = 281u,
    EVT_USART1_RTO          = 282u,
    EVT_USART2_EI           = 283u,
    EVT_USART2_RI           = 284u,
    EVT_USART2_TI           = 285u,
    EVT_USART2_TCI          = 286u,
    EVT_USART2_RTO          = 287u,
    EVT_USART3_EI           = 288u,
    EVT_USART3_RI           = 289u,
    EVT_USART3_TI           = 290u,
    EVT_USART3_TCI          = 291u,
    EVT_USART3_RTO          = 292u,
    EVT_USART4_EI           = 293u,
    EVT_USART4_RI           = 294u,
    EVT_USART4_TI           = 295u,
    EVT_USART4_TCI          = 296u,
    EVT_USART4_RTO          = 297u,

    /* SPI */
    EVT_SPI1_SRRI           = 299u,
    EVT_SPI1_SRTI           = 300u,
    EVT_SPI1_SPII           = 301u,
    EVT_SPI1_SPEI           = 302u,
    EVT_SPI1_SPTEND         = 303u,
    EVT_SPI2_SRRI           = 304u,
    EVT_SPI2_SRTI           = 305u,
    EVT_SPI2_SPII           = 306u,
    EVT_SPI2_SPEI           = 307u,
    EVT_SPI2_SPTEND         = 308u,
    EVT_SPI3_SRRI           = 309u,
    EVT_SPI3_SRTI           = 310u,
    EVT_SPI3_SPII           = 311u,
    EVT_SPI3_SPEI           = 312u,
    EVT_SPI3_SPTEND         = 313u,
    EVT_SPI4_SRRI           = 314u,
    EVT_SPI4_SRTI           = 315u,
    EVT_SPI4_SPII           = 316u,
    EVT_SPI4_SPEI           = 317u,
    EVT_SPI4_SPTEND         = 318u,

    /* AOS */
    EVT_AOS_STRG            = 319u,

    /* TIMER 4 */
    EVT_TMR41_SCMUH         = 368u,
    EVT_TMR41_SCMUL         = 369u,
    EVT_TMR41_SCMVH         = 370u,
    EVT_TMR41_SCMVL         = 371u,
    EVT_TMR41_SCMWH         = 372u,
    EVT_TMR41_SCMWL         = 373u,
    EVT_TMR42_SCMUH         = 374u,
    EVT_TMR42_SCMUL         = 375u,
    EVT_TMR42_SCMVH         = 376u,
    EVT_TMR42_SCMVL         = 377u,
    EVT_TMR42_SCMWH         = 378u,
    EVT_TMR42_SCMWL         = 379u,
    EVT_TMR43_SCMUH         = 384u,
    EVT_TMR43_SCMUL         = 385u,
    EVT_TMR43_SCMVH         = 386u,
    EVT_TMR43_SCMVL         = 387u,
    EVT_TMR43_SCMWH         = 388u,
    EVT_TMR43_SCMWL         = 389u,

    /* EVENT PORT */
    EVT_EVENT_PORT1         = 394u,
    EVT_EVENT_PORT2         = 395u,
    EVT_EVENT_PORT3         = 396u,
    EVT_EVENT_PORT4         = 397u,

    /* I2S */
    EVT_I2S1_TXIRQOUT       = 400u,
    EVT_I2S1_RXIRQOUT       = 401u,
    EVT_I2S2_TXIRQOUT       = 403u,
    EVT_I2S2_RXIRQOUT       = 404u,
    EVT_I2S3_TXIRQOUT       = 406u,
    EVT_I2S3_RXIRQOUT       = 407u,
    EVT_I2S4_TXIRQOUT       = 409u,
    EVT_I2S4_RXIRQOUT       = 410u,

    /* COMPARATOR */
    EVT_ACMP1               = 416u,
    EVT_ACMP2               = 417u,
    EVT_ACMP3               = 418u,

    /* I2C */
    EVT_I2C1_RXI            = 420u,
    EVT_I2C1_TXI            = 421u,
    EVT_I2C1_TEI            = 422u,
    EVT_I2C1_EE1            = 423u,
    EVT_I2C2_RXI            = 424u,
    EVT_I2C2_TXI            = 425u,
    EVT_I2C2_TEI            = 426u,
    EVT_I2C2_EE1            = 427u,
    EVT_I2C3_RXI            = 428u,
    EVT_I2C3_TXI            = 429u,
    EVT_I2C3_TEI            = 430u,
    EVT_I2C3_EE1            = 431u,

    /* PVD */
    EVT_PVD_PVD1            = 433u,
    EVT_PVD_PVD2            = 434u,

    /* OTS */
    EVT_OTS                 = 435u,

    /* WDT */
    EVT_WDT_REFUDF          = 439u,

    /* ADC */
    EVT_ADC1_EOCA           = 448u,
    EVT_ADC1_EOCB           = 449u,
    EVT_ADC1_CHCMP          = 450u,
    EVT_ADC1_SEQCMP         = 451u,
    EVT_ADC2_EOCA           = 452u,
    EVT_ADC2_EOCB           = 453u,
    EVT_ADC2_CHCMP          = 454u,
    EVT_ADC2_SEQCMP         = 455u,

    /* TRNG */
    EVT_TRNG_END            = 456u,

    /* SDIO */
    EVT_SDIOC1_DMAR         = 480u,
    EVT_SDIOC1_DMAW         = 481u,
    EVT_SDIOC2_DMAR         = 483u,
    EVT_SDIOC2_DMAW         = 484u,
    EVT_MAX                 = 511u,
}en_event_src_t;

/** @} */

/**
 * @} hc32f460_if_dma_chan
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F460_DMA_CHAN_H */

/* end of file */
