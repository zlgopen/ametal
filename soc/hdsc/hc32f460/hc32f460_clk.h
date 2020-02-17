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
 * \brief ZLG217 时钟ID定义
 *
 * \internal
 * \par Modification History
 * - 1.00 19-12-23  cds, first implementation
 * \endinternal
 */

#ifndef __HD32F460_CLK_H
#define __HD32F460_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup h32f460_if_clockid
 * \copydoc hc32f460_clk.h
 * @{
 */

/**
 * \name 系统或外设的时钟ID
 * \anchor grp_clk_id
 * @{
 */
/* 时钟源ID */
#define CLK_XTAL         (0ul)  /**< \brief 外部高速振荡器 */
#define CLK_XTAL32       (1ul)  /**< \brief 外部低速振荡器 */
#define CLK_MPLL         (2ul)  /**< \brief MPLL时钟 */
#define CLK_MPLLQ        (3ul)  /**< \brief MPLLQ时钟 */
#define CLK_MPLLP        (4ul)  /**< \brief MPLLP时钟 */
#define CLK_MPLLR        (5ul)  /**< \brief MPLLR时钟 */
#define CLK_UPLL         (6ul)  /**< \brief UPLL时钟 */
#define CLK_UPLLQ        (7ul)  /**< \brief UPLLP时钟 */
#define CLK_UPLLP        (8ul)  /**< \brief UPLLQ时钟 */
#define CLK_UPLLR        (9ul)  /**< \brief UPLLR时钟 */
#define CLK_HRC          (10ul) /**< \brief 内部高速振荡器 */
#define CLK_MRC          (11ul) /**< \brief 内部中速振荡器 */
#define CLK_LRC          (12ul) /**< \brief 内部低速振荡器 */
#define CLK_SWDTRC       (13ul) /**< \brief SWTD专用内部低速振荡器 */
#define CLK_SYSCLK_DIV1  (14ul) /**< \brief 系统时钟1分频 */
#define CLK_SYSCLK_DIV2  (15ul) /**< \brief 系统时钟2分频 */
#define CLK_SYSCLK_DIV3  (16ul) /**< \brief 系统时钟2分频 */
#define CLK_SYSCLK_DIV4  (17ul) /**< \brief 系统时钟4分频 */
#define CLK_SYSCLK_DIV8  (18ul) /**< \brief 系统时钟8分频 */
#define CLK_SYSCLK_DIV16 (19ul) /**< \brief 系统时钟16分频 */
#define CLK_SYSCLK_DIV32 (20ul) /**< \brief 系统时钟32分频 */
#define CLK_SYSCLK_DIV64 (21ul) /**< \brief 系统时钟64分频 */

/* 工作时钟 ID */
#define CLK_HCLK         ((1 << (31ul)) | 0ul)
#define CLK_PCLK0        ((1 << (31ul)) | 1ul)
#define CLK_PCLK1        ((1 << (31ul)) | 2ul)
#define CLK_PCLK2        ((1 << (31ul)) | 3ul)
#define CLK_PCLK3        ((1 << (31ul)) | 4ul)
#define CLK_PCLK4        ((1 << (31ul)) | 5ul)
#define CLK_EXCLK        ((1 << (31ul)) | 6ul)
#define CLK_UCLK         ((1 << (31ul)) | 7ul)
#define CLK_CANCLK       ((1 << (31ul)) | 8ul)
#define CLK_STICCLK      ((1 << (31ul)) | 9ul)
#define CLK_SWDTCLK      ((1 << (31ul)) | 10ul)
#define CLK_TCK          ((1 << (31ul)) | 11ul)
#define CLK_TPIUCLK      ((1 << (31ul)) | 12ul)
#define CLK_I2S1CLK      ((1 << (31ul)) | 13ul)
#define CLK_I2S2CLK      ((1 << (31ul)) | 14ul)
#define CLK_I2S3CLK      ((1 << (31ul)) | 15ul)
#define CLK_I2S4CLK      ((1 << (31ul)) | 16ul)

/* 外设时钟时钟源标志 */
#define WORK_CLK_FLAG (1 << 31)
#define FROM_HCLK     (1 << 30ul)
#define FROM_PCLK0    (1 << 29ul)
#define FROM_PCLK1    (1 << 28ul)
#define FROM_PCLK2    (1 << 27ul)
#define FROM_PCLK3    (1 << 26ul)
#define FROM_PCLK4    (1 << 25ul)
#define FROM_EXCLK    (1 << 24ul)
#define FROM_UCLK     (1 << 23ul)
#define FROM_CANCLK   (1 << 22ul)
#define FROM_STICCLK  (1 << 21ul)
#define FROM_SWDTCLK  (1 << 20ul)
#define FROM_TCK      (1 << 19ul)
#define FROM_TPIUCLK  (1 << 18ul)
#define FROM_I2S1CLK  (1 << 17ul)
#define FROM_I2S2CLK  (1 << 16ul)
#define FROM_I2S3CLK  (1 << 15ul)
#define FROM_I2S4CLK  (1 << 14ul)

/* 外设时钟 */
#define CLK_RAMHS  (FROM_HCLK | (0ul))
#define CLK_RAM0   (FROM_HCLK | (4ul))
#define CLK_ECCRAM (FROM_HCLK | (8ul))
#define CLK_RETRAM (FROM_HCLK | (10ul))
#define CLK_DMA1   (FROM_HCLK | (14ul))
#define CLK_DMA2   (FROM_HCLK | (15ul))
#define CLK_FCM    (FROM_HCLK | (16ul)) /* FCM没有单独挂载在某一工作时钟下 */
#define CLK_PTDIS  (FROM_HCLK | (17ul)) /* 没有找到PTDIS的时钟来源 */


#define CLK_AES    (FROM_PCLK1 | (20ul))
#define CLK_HASH   (FROM_PCLK1 | (21ul))

#define CLK_TRNG   (FROM_PCLK4 | (22ul))
#define CLK_CRC    (FROM_PCLK1 | (23ul))
#define CLK_DCU1   (FROM_HCLK | (24ul))
#define CLK_DCU2   (FROM_HCLK | (25ul))
#define CLK_DCU3   (FROM_HCLK | (26ul))
#define CLK_DCU4   (FROM_HCLK | (27ul))
#define CLK_KEY    (FROM_HCLK | (31ul))/* 没有找到KEY的时钟来源 */

#define CLK_CAN    (FROM_EXCLK | (32ul))
#define CLK_QSPI   (FROM_PCLK1 | (35ul))
#define CLK_IIC1   (FROM_PCLK3 | (36ul))
#define CLK_IIC2   (FROM_PCLK3 | (37ul))
#define CLK_IIC3   (FROM_PCLK3 | (38ul))
#define CLK_USBFS  (FROM_PCLK1 | (40ul))
#define CLK_SDIOC1 (FROM_EXCLK | (42ul))
#define CLK_SDIOC2 (FROM_EXCLK | (43ul))
#define CLK_I2S1   (FROM_I2S1CLK | (44ul))
#define CLK_I2S2   (FROM_I2S2CLK | (45ul))
#define CLK_I2S3   (FROM_I2S3CLK | (46ul))
#define CLK_I2S4   (FROM_I2S4CLK | (47ul))
#define CLK_SPI1   (FROM_PCLK1 | (48ul))
#define CLK_SPI2   (FROM_PCLK1 | (49ul))
#define CLK_SPI3   (FROM_PCLK1 | (50ul))
#define CLK_SPI4   (FROM_PCLK1 | (51ul))
#define CLK_USART1 (FROM_PCLK1 | (56ul))
#define CLK_USART2 (FROM_PCLK1 | (57ul))
#define CLK_USART3 (FROM_PCLK1 | (58ul))
#define CLK_USART4 (FROM_PCLK1 | (59ul))

#define CLK_TIMER0_1 (FROM_PCLK1 | (64ul))
#define CLK_TIMER0_2 (FROM_PCLK1 | (65ul))
#define CLK_TIMERA_1 (FROM_PCLK1 | (66ul))
#define CLK_TIMERA_2 (FROM_PCLK1 | (67ul))
#define CLK_TIMERA_3 (FROM_PCLK1 | (68ul))
#define CLK_TIMERA_4 (FROM_PCLK1 | (69ul))
#define CLK_TIMERA_5 (FROM_PCLK1 | (70ul))
#define CLK_TIMERA_6 (FROM_PCLK1 | (71ul))
#define CLK_TIMER4_1 (FROM_PCLK1 | (72ul))
#define CLK_TIMER4_2 (FROM_PCLK1 | (73ul))
#define CLK_TIMER4_3 (FROM_PCLK1 | (74ul))
#define CLK_EMB      (FROM_PCLK1 | (79ul))
#define CLK_TIMER6_1 (FROM_PCLK1 | (80ul))
#define CLK_TIMER6_2 (FROM_PCLK1 | (81ul))
#define CLK_TIMER6_3 (FROM_PCLK1 | (82ul))

#define CLK_ADC1     (FROM_PCLK1 | (96ul))
#define CLK_ADC2     (FROM_PCLK1 | (97ul))
#define CLK_DAC      (FROM_PCLK1 | (100ul))/* 没有找到DAC的时钟来源 */
#define CLK_CMP      (FROM_PCLK3 | (104ul))
#define CLK_OTS      (FROM_PCLK1 | (108ul))/* 没有找到OTS的时钟来源 */





/**
 * @}
 */

/**
 * @} hc32f460_if_clk
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F460_CLK_H */

/* end of file */
