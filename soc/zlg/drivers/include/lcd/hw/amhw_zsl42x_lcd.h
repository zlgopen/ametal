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
 * \brief LCD 底层操作接口
 * \internal
 * \par Modification History
 * - 1.00 19-10-14
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_LCD_H
#define __AMHW_ZSL42x_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zsl42x_if_lcd
 * \copydoc amhw_zsl42x_lcd.h
 * @{
 */

/**
 * \brief LCD - 寄存器组
 */
typedef struct amhw_zsl42x_lcd {
    __IO uint32_t lcdcr0;        /**< \brief LCD配置寄存器0 */
    __IO uint32_t lcdcr1;        /**< \brief LCD配置寄存器1 */
    __IO uint32_t lcdintclr;     /**< \brief LCD中断清除寄存器 */
    __IO uint32_t lcdpoen0;      /**< \brief LCD输出配置寄存器0 */
    __IO uint32_t lcdpoen1;      /**< \brief LCD输出配置寄存器1 */
         uint32_t res[11];       /**< \brief 保留 */
    __IO uint32_t lcdram0_f[16]; /**< \brief LCD RAM0~f */
} amhw_zsl42x_lcd_t;

/**
 * \brief LCD对比度设置,contrast值一共16个等级。
 */
typedef enum amhw_zsl42x_lcd_ctr_val {
    AHHW_ZSL42x_CTR_1 = 0,
    AHHW_ZSL42x_CTR_2,
    AHHW_ZSL42x_CTR_3,
    AHHW_ZSL42x_CTR_4,
    AHHW_ZSL42x_CTR_5,
    AHHW_ZSL42x_CTR_6,
    AHHW_ZSL42x_CTR_7,
    AHHW_ZSL42x_CTR_8,
    AHHW_ZSL42x_CTR_9,
    AHHW_ZSL42x_CTR_10,
    AHHW_ZSL42x_CTR_11,
    AHHW_ZSL42x_CTR_12,
    AHHW_ZSL42x_CTR_13,
    AHHW_ZSL42x_CTR_14,
    AHHW_ZSL42x_CTR_15,
    AHHW_ZSL42x_CTR_16
}amhw_zsl42x_lcd_ctr_val_t;

/**
 * \brief LCD对比度调整
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : contrast值，参考枚举定义：LCD对比度设置
 *
 * \note : 仅当Bias电压来源选择内部电阻分压时有效
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lcd_ctr_set (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr0 = (p_hw_lcd->lcdcr0 & (~(0xf << 12))) | (flag << 12);
}

/**
 * \brief Bias电压来源选择掩码
 */
#define AMHW_ZSL42x_LCD_BIAS_EXT_RES  (0U)   /**< \brief 外部电阻模式，需要外部电路配合 */
#define AMHW_ZSL42x_LCD_BIAS_CAP_DIV  (1U)   /**< \brief 电容分压模式，需要外部电路配合*/

/**< \brief 内部电阻分压，中功耗模式*/
#define AMHW_ZSL42x_LCD_BIAS_INT_RES_DIV_POWER_MID   (2U)
/**< \brief 内部电阻分压，小功耗模式*/
#define AMHW_ZSL42x_LCD_BIAS_INT_RES_DIV_POWER_LOW   (4U)
/**< \brief 内部电阻分压，大功耗模式*/
#define AMHW_ZSL42x_LCD_BIAS_INT_RES_DIV_POWER_HIGH  (6U)

/**
 * \brief Bias电压来源选择
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : 偏置电压，参考枚宏定义：Bias电压来源选择掩码
 *
 */
am_static_inline
void amhw_zsl42x_lcd_bias_rsc_sel (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr0 = (p_hw_lcd->lcdcr0 & (~(0x7 << 9))) | (flag << 9);
}

/**
 * \brief LCD duty配置
 */
typedef enum amhw_zsl42x_lcd_duty {
    AMHW_ZSL42x_DUTY_STATIC = 0,
    AMHW_ZSL42x_DUTY_1_2,
    AMHW_ZSL42x_DUTY_1_3,
    AMHW_ZSL42x_DUTY_1_4,
    AMHW_ZSL42x_DUTY_1_6 = 5,
    AMHW_ZSL42x_DUTY_1_8 = 7,
}amhw_zsl42x_lcd_duty_t;

/**
 * \brief LCD duty配置
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : 偏置电压，参考枚举定义：LCD duty配置
 *
 * \retval : 无
 *
 */
am_static_inline
void amhw_zsl42x_lcd_duty_sel (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr0 = (p_hw_lcd->lcdcr0 & (~(0x7 << 6))) | (flag << 6);
}

/**
 * \biref 偏置电压配置掩码
 */
#define AMHW_ZSL42x_BIAS_1_3  (0U)  /**< \brief 1/3 bias */
#define AMHW_ZSL42x_BIAS_1_2  (1U)  /**< \brief 1/2 bias */

/**
 * \brief LCD duty配置
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : 偏置电压，参考宏定义：偏置电压配置掩码
 *
 * \retval : 无
 *
 */
am_static_inline
void amhw_zsl42x_lcd_bias_sel (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr0 |= (p_hw_lcd->lcdcr0 & ~(0x1 << 5)) | (flag << 5);
}

/**
 * \biref 电压泵时钟频率选择掩码
 */
#define AMHW_ZSL42x_CPCLK_2K_HZ  (0U)  /**< \brief 2k Hz */
#define AMHW_ZSL42x_CPCLK_4K_HZ  (1U)  /**< \brief 4k Hz */
#define AMHW_ZSL42x_CPCLK_8K_HZ  (2U)  /**< \brief 8k Hz */
#define AMHW_ZSL42x_CPCLK_16K_HZ (3U)  /**< \brief 16k Hz */

/**
 * \brief 电压泵时钟频率选择
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : 偏置电压，参考宏定义：电压泵时钟频率选择
 *
 * \retval : 无
 *
 */
am_static_inline
void amhw_zsl42x_lcd_cpclk_sel (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr0 = (p_hw_lcd->lcdcr0 & (~(0x3 << 3))) | (flag << 3);
}

/**
 * \biref LCD扫描频率选择掩码
 */
#define AMHW_ZSL42x_LCDCLK_64_HZ   (0U)  /**< \brief 64 Hz */
#define AMHW_ZSL42x_LCDCLK_128_HZ  (1U)  /**< \brief 128 Hz */
#define AMHW_ZSL42x_LCDCLK_256_HZ  (2U)  /**< \brief 256 Hz */
#define AMHW_ZSL42x_LCDCLK_512_HZ  (3U)  /**< \brief 512 Hz */

/**
 * \brief LCD扫描频率选择
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : 偏置电压，参考宏定义：LCD扫描频率选择
 *
 * \note : LCD帧频率 = LCD扫描频率×Duty
 *
 * \retval : 无
 *
 */
am_static_inline
void amhw_zsl42x_lcd_cldclk_sel (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr0 = (p_hw_lcd->lcdcr0 & (~(0x3 << 1))) | (flag << 1);
}

/**
 * \brief LCD使能
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 * \retval : 无
 *
 */
am_static_inline
void amhw_zsl42x_lcd_enable (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    p_hw_lcd->lcdcr0 |= (1 << 0);
}

/**
 * \brief LCD禁能
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 * \retval : 无
 *
 */
am_static_inline
void amhw_zsl42x_lcd_disable (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    p_hw_lcd->lcdcr0 &= ~(1 << 0);
}

/**
 * \brief LCD中断状态获取
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 * \retval : 1 中断  0 无中断
 *
 */
am_static_inline
int amhw_zsl42x_lcd_int_stu_check (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    return (p_hw_lcd->lcdcr0 >> 11);
}

/**
 * \brief 中断使能
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 */
am_static_inline
void amhw_zsl42x_lcd_int_enable (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    p_hw_lcd->lcdcr1 |= (1 << 9);
}

/**
 * \brief 中断禁能
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 */
am_static_inline
void amhw_zsl42x_lcd_int_disable (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    p_hw_lcd->lcdcr1 &= ~(1 << 9);
}

/**
 * \brief 中断标志清除
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 */
am_static_inline
void amhw_zsl42x_lcd_int_flag_clr (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    p_hw_lcd->lcdintclr &= ~(1 << 10);
}

/**
 * \brief DMA硬件触发使能
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 */
am_static_inline
void amhw_zsl42x_lcd_dma_enable (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    p_hw_lcd->lcdcr1 |= (1 << 10);
}

/**
 * \brief DMA硬件触发禁能
 *
 * \param[in] p_hw_lcd: 指向LCD寄存器组的指针
 *
 */
am_static_inline
void amhw_zsl42x_lcd_dma_disable (amhw_zsl42x_lcd_t *p_hw_lcd)
{
    p_hw_lcd->lcdcr1 &= ~(1 << 10);
}

/**
 * \brief LCD RAM显示模式选择掩码
 */
#define AMHW_ZSL42x_LCD_MODE_0  (0U)
#define AMHW_ZSL42x_LCD_MODE_1  (1U)

/**
 * \brief LCD RAM显示模式选择
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : 模式，参考宏定义：LCD RAM显示模式选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lcd_mode_sel (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr1 = (p_hw_lcd->lcdcr1 & ~(1 << 8)) | (flag << 8);
}

/**
 * \brief LCD时钟源选择掩码
 */
#define AMHW_ZSL42x_LCD_CLK_SRC_XTL  (1U)
#define AMHW_ZSL42x_LCD_CLK_SRC_RCL  (0U)

/**
 * \brief LCD时钟源选择选择
 *
 * \param[in] p_hw_lcd  : 指向LCD寄存器组的指针
 * \param[in] flag      : 时钟源，参考宏定义：LCD时钟源选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lcd_clk_rsc_sel (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{
    p_hw_lcd->lcdcr1 = (p_hw_lcd->lcdcr1 & ~(1 << 7)) | (flag << 7);
}

/**
 * \brief 闪屏频率与LCD中断间隔设置
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] val      : Blinkcnt值
 *
 * \note : 注：LCD闪烁频率为 = LCD帧频率 / (BlinkCnt+1)
 *            LCD中断间隔= (BlinkCnt+1)*(1/LCD 帧频率)
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_blink_cnt_set (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t val)
{
    p_hw_lcd->lcdcr1 = (p_hw_lcd->lcdcr1 & (~(0x3f))) | (val << 0);
}

/**
 * \brief Seg0_31输出控制
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : 需要控制开启或关闭的Seg位
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_seg0_31_ctr (amhw_zsl42x_lcd_t *p_hw_lcd, uint32_t flag)
{

    p_hw_lcd->lcdpoen0 &= ~flag;
}

/**
 * \brief Seg32_51_Com0_8控制掩码
 */
#define AMHW_ZSL42x_LCD_SEG32_35_CTR  0    /**< \brief seg32-35 */
#define AMHW_ZSL42x_LCD_COM0_3_CTR    1    /**< \brief COM0-3 */
#define AMHW_ZSL42x_LCD_COMSEG_CTR    2    /**< \brief COM4-7 seg36-39 */
#define AMHW_ZSL42x_LCD_MUX_CTR       3    /**< \brief SEG32~SEG35端口功能选择 */
#define AMHW_ZSL42x_LCD_SEG40_51_CTR  4    /**< \brief seg40-51 */
#define AMHW_ZSL42x_LCD_ALL_CLOSE     5    /**< \brief 所有控制输出关闭*/

/**
 * \brief Seg32_51_Com0_8输出控制
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] flag     : Seg32_51_Com0_8控制码
 * \param[in] data     : 控制输出数据(不同控制数据格式不同，例如：AMHW_ZSL42x_LCD_MUX_CTR开启
 *                       data = 1，AMHW_ZSL42x_LCD_COM0_3_CTR全部关闭，data = 0x0)
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_seg32_51_com0_8_ctr(amhw_zsl42x_lcd_t *p_hw_lcd,
                                     uint32_t           flag,
                                     uint32_t           data)
{



    switch (flag){

        case AMHW_ZSL42x_LCD_ALL_CLOSE:
            p_hw_lcd->lcdpoen1 = (p_hw_lcd->lcdpoen1 | 0xffffffff) & data;
            break;
        case AMHW_ZSL42x_LCD_SEG40_51_CTR :
            p_hw_lcd->lcdpoen1 = (p_hw_lcd->lcdpoen1) & (~(data << 8));
            break;
        case AMHW_ZSL42x_LCD_MUX_CTR :
            if (data == 0){
                p_hw_lcd->lcdpoen1 &= ~(1 << 12);
            }else{
                p_hw_lcd->lcdpoen1 |=  (1 << 12);
            }
            break;
        case AMHW_ZSL42x_LCD_COMSEG_CTR :
            p_hw_lcd->lcdpoen1 = (p_hw_lcd->lcdpoen1)  & (~(data << 4));
            break;
        case AMHW_ZSL42x_LCD_COM0_3_CTR :
            p_hw_lcd->lcdpoen1 =
                         (p_hw_lcd->lcdpoen1 ) & (~(data << 21));
            break;
        case AMHW_ZSL42x_LCD_SEG32_35_CTR :
            p_hw_lcd->lcdpoen1 =
                         (p_hw_lcd->lcdpoen1) & ~data;
            break;
        default : return ;
    }
}

/**
 * \brief LCD-RAM_0_f 输出控制
 *
 * \param[in] p_hw_lcd : 指向LCD寄存器组的指针
 * \param[in] data     : ram写入数据
 * \param[in] addr     : 地址索引
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lcd_ram_write (amhw_zsl42x_lcd_t *p_hw_lcd ,
                                uint32_t           data,
                                uint8_t            addr)
{
    p_hw_lcd->lcdram0_f[addr] =
                           (p_hw_lcd->lcdram0_f[addr] & (~(0xffffffff))) | data;
}


/**
 * @} amhw_if_zsl42x_lcd
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSL42x_lcd_H */

/* end of file */
