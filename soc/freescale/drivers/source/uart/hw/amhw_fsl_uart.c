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
 * \brief 串口（UART）操作接口
 *
 * 1. 全双工操作，标准不归零(NRZ)格式
 * 2. 具有独立使能的双缓冲可分离的发送接收器和接收器
 * 3. 波特率可编程（ 13 位模数分频器）
 * 4. 中断驱动或轮询操作
 * 1. 发送数据寄存器空并且发送完成
 * 2. 接收数据寄存器满
 * 3. 接收过载、奇偶检验错误、帧错误和噪声错误
 * 4. 空闲接收器检测
 * 5. 接收引脚上有活动沿
 * 6. 支持 LIN 的中断检测
 * 5. 硬件奇偶校验生成和检测
 * 6. 可编程的 8 位或专 9 位字符长度
 * 7. 可编程的 1 位或 2 位停止位
 * 8. 空闲线或地址标志唤醒接收器：
 * 9. 可选的 13 位中止符生成/11 位中止符检测
 * 10. 可选的发送器输出极性
 * 11. 5 通道 DMA 接口
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-15  nwt, make some changes about function name
 * - 1.00 15-07-03  xiongyanming, first implementation.
 * \endinternal
 */
#include "hw/amhw_fsl_uart.h"
/**
 * \brief 比较误差大小
 */
static int __err_cal (uint32_t  uart0clk,
                      uint32_t  baud,
                      uint32_t *p_osr_val)
{
    uint8_t  i               = 0;
    uint32_t calculated_baud = 0;
    uint32_t baud_diff       = 0;
    uint32_t osr_val         = 0;
    uint32_t sbr_val         = 0;

    uint32_t temp            = 0;

    /* 用最小的过采样率OSR_VAL来计算第一个baud_diff*/
    osr_val      = 4;
    /* 得到理论计算的波特率 */
    sbr_val = (uint32_t)(uart0clk/(baud * osr_val));
    calculated_baud = (uart0clk / (osr_val * sbr_val));

    /* 计算的波特率偏差 */
    if (calculated_baud > baud) {
        baud_diff = calculated_baud - baud;
    }  else {
        baud_diff = baud - calculated_baud;
    }

    /* 计算最合适的过采样率osr_val */
    for (i = 5; i <= 32; i++) {
        sbr_val = (uint32_t)(uart0clk/(baud * i));
        calculated_baud = (uart0clk / (i * sbr_val));

        if (calculated_baud > baud){
             temp = calculated_baud - baud;
        }else{
            temp = baud - calculated_baud;
        }
        if (temp <= baud_diff) {
            baud_diff = temp;
            osr_val = i;
        }
    }

    *p_osr_val = osr_val;

    /* 波特率误差控制在%4以下 */
    if (baud_diff < ((baud / 100) * 3)) {
           return AM_OK;
    }else {
        return AM_ERROR;
    }

}

/**
 * \brief USART数据发送(查询模式)
 */
uint32_t amhw_fsl_uart_poll_send (amhw_fsl_uart_t  *p_hw_uart,
                                   const uint8_t *p_txbuf,
                                   uint32_t       nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

         while((p_hw_uart->stat1 & AMHW_FSL_UART_S1_TDRE_MASK) == 0);
         amhw_fsl_uart_txdata_write(p_hw_uart, (uint8_t)(*p_txbuf++));
    }

    return nbytes;
}


/**
 * \brief USART数据接收(查询模式)
 */
uint32_t amhw_fsl_uart_poll_receive (amhw_fsl_uart_t *p_hw_uart,
                                      uint8_t      *p_rxbuf,
                                      uint32_t      nbytes)
{
    uint32_t len = nbytes;

    while (len--) {
        while(!amhw_fsl_uart_stat1_rdre_get(p_hw_uart));
        *p_rxbuf++ = amhw_fsl_uart_rxdata_read(p_hw_uart);
    }

    return nbytes;
}

/**
 * \brief VER0 串口波特率设置
 */
int amhw_fsl_uart_ver0_baudrate_set (amhw_fsl_uart_t *p_hw_uart, uint32_t sysclk, int baud)
{
    uint32_t osr_val  = 0;
    uint32_t sbr_val  = 0;
    uint32_t uart0clk = 0;
    /* 确保有效的 clock value*/
    if ((sysclk > 50000000) || (sysclk < 32000)) {
        sysclk = 0;

        /* 初始化异常，由于期望的系统时钟值无效*/
        return AM_ERROR;
    }

    /* uart0 时钟单位为 HZ */
    uart0clk  = sysclk;
    /* 在改变UART1/2寄存器值前 接收发送禁能 */
    amhw_fsl_uart_disable(p_hw_uart);


	if (__err_cal(uart0clk,baud,&osr_val) == AM_OK) {
	   /* 如果OSR的值在4x - 8x之间，使能双边沿采样*/
		if ((osr_val >3) && (osr_val < 9)){
			p_hw_uart->uart0_c5 |= AMHW_FSL_UART0_C5_BOTHEDGE_EN;
		}
		sbr_val = (uint32_t)((uart0clk)/(baud * (osr_val)));
		amhw_fsl_uart_c4_osr_set(p_hw_uart,(osr_val-1));
	}else {
		return AM_ERROR;
	}

    amhw_fsl_uart_bdh_sbr_set(p_hw_uart,((sbr_val&0x1F00)>>8));
    amhw_fsl_uart_bdl_sbr_set(p_hw_uart,sbr_val & AMHW_FSL_UART_BDL_SBR_MASK);

    /* 使能接收和传送 */
    amhw_fsl_uart_enable(p_hw_uart);

    return baud;
}

/**
 * \brief VER1 串口波特率设置
 */
int amhw_fsl_uart_ver1_baudrate_set (amhw_fsl_uart_t *p_hw_uart, uint32_t sysclk, int baud)
{

    uint32_t sbr_val  = 0;
    /* 确保有效的 clock value*/
    if ((sysclk > 50000000) || (sysclk < 32000)) {
        sysclk = 0;

        /* 初始化异常，由于期望的系统时钟值无效*/
        return AM_ERROR;
    }


    /* 在改变UART1/2寄存器值前 接收发送禁能 */
    amhw_fsl_uart_disable(p_hw_uart);

	sbr_val = (uint16_t)((sysclk)/(baud * 16));


    amhw_fsl_uart_bdh_sbr_set(p_hw_uart,((sbr_val&0x1F00)>>8));
    amhw_fsl_uart_bdl_sbr_set(p_hw_uart,sbr_val & AMHW_FSL_UART_BDL_SBR_MASK);

    /* 使能接收和传送 */
    amhw_fsl_uart_enable(p_hw_uart);

    return baud;
}

/**
 * \brief UART特殊功能模式设置(回环模式、 单线模式)
 */
void amhw_fsl_uart_special_mode_set (amhw_fsl_uart_t *p_hw_uart, uint8_t mode)
{
    switch (mode) {

    case AMHW_FSL_UART_SPEC_MODE_LOOP:
        p_hw_uart->c1 |= AMHW_FSL_UART_C1_LOOPS;
        p_hw_uart->c1 &= ~AMHW_FSL_UART_C1_RSRC;
        break;

    case AMHW_FSL_UART_SPEC_MODE_SINGLE_WIRE:
        p_hw_uart->c1 |= AMHW_FSL_UART_C1_LOOPS;
        p_hw_uart->c1 |= AMHW_FSL_UART_C1_RSRC;
        break;

    default:
        break;

    }
}


/* end of file */

