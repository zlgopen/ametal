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
 * \brief SDIO 硬件操作接口
 * \internal
 * \par Modification history
 * - 1.00 17-04-13  ari, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG_SDIO_H
#define __AMHW_ZLG_SDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_zlg_sdio
 * \copydoc amhw_zlg_sdio.h
 * @{
 */

/**
 * \brief sdio structure of register
 */
typedef struct amhw_zlg_sdio {
    __IO uint32_t  mmc_ctrl;        /**< \brief sdio mmc_ctrl register */
    __IO uint32_t  mmc_io;          /**< \brief sdio mmc_io register */
    __IO uint32_t  mmc_bytecntl;    /**< \brief sdio mmc_bytecntl register */
    __IO uint32_t  mmc_tr_blockcnt; /**< \brief sdio interrupt enable register */
    __IO uint32_t  mmc_crcctl;      /**< \brief sdio mmc_crcctl register */
    __IO uint32_t  cmd_crc;         /**< \brief sdio cmd_crc register*/
    __IO uint32_t  dat_crcl;        /**< \brief sdio dat_crcl control */
    __IO uint32_t  dat_crch;        /**< \brief sdio dat_crch register */
    __IO uint32_t  mmc_int_mask;    /**< \brief sdio mmc_int_mask number */
    __IO uint32_t  clr_mmc_int;     /**< \brief sdio clr_mmc_int register */
    __IO uint32_t  mmc_cardsel;     /**< \brief sdio mmc_cardsel control */
    __I  uint32_t  mmc_siq;         /**< \brief sdio mmc_siq control */
    __IO uint32_t  mmc_io_mbctl;    /**< \brief sdio mmc_io_mbctl control */
    __IO uint32_t  mmc_blockcnt;    /**< \brief sdio mmc_blockcnt control */
    __IO uint32_t  mmc_timeoutcnt;  /**< \brief sdio mmc_timeoutcnt control */
    __IO uint32_t  cmd_bufx[16];    /**< \brief sdio cmd_bufx control */
    __IO uint32_t  buf_ctll;        /**< \brief sdio buf_ctll control */
    __IO uint32_t  resiver[31];     /**< \brief sdio cmd_bufx control */
    __IO uint32_t  data_buf[128];   /**< \brief sdio data_buf control */
} amhw_zlg_sdio_t;

/**
 * \brief sdio的DAT线位宽
 */
typedef enum amhw_zlg_sdio_width {
    AMHW_ZLG_ADIO_WIDTH_1BIT      = 0x0,   /**< \brief 1位 */
	AMHW_ZLG_ADIO_WIDTH_4BIT      = 0x1,   /**< \brief 4位 */
} amhw_zlg_sdio_width_t;

/**
 * \brief sdio速度模式配置
 */
typedef enum amhw_zlg_sdio_speed_mode {
    AMHW_ZLG_ADIO_SPEED_MODE_LOW      = 0x0,   /**< \brief 低速模式 */
	AMHW_ZLG_ADIO_SPEED_MODE_HIGH     = 0x1,   /**< \brief 高速模式 */
} amhw_zlg_sdio_speed_mode_t;

/**
 * \brief sdio端口时钟配置
 */
typedef enum amhw_zlg_port_clk_mode {
    AMHW_ZLG_ADIO_PORT_CLK_MODE_1_2BASECLK      = 0x0,   /**< \brief 1/2倍基础时钟速模式 */
    AMHW_ZLG_ADIO_PORT_CLK_MODE_1_4BASECLK      = 0x1,   /**< \brief 1/4倍基础时钟速模式 */
    AMHW_ZLG_ADIO_PORT_CLK_MODE_1_6BASECLK      = 0x2,   /**< \brief 1/6倍基础时钟速模式 */
    AMHW_ZLG_ADIO_PORT_CLK_MODE_1_8BASECLK      = 0x3,   /**< \brief 1/8倍基础时钟速模式 */
	AMHW_ZLG_ADIO_PORT_CLK_MODE_1_10BASECLK     = 0x4,   /**< \brief 1/10倍基础时钟速模式 */
    AMHW_ZLG_ADIO_PORT_CLK_MODE_1_12BASECLK     = 0x5,   /**< \brief 1/12倍基础时钟速模式 */
    AMHW_ZLG_ADIO_PORT_CLK_MODE_1_14BASECLK     = 0x6,   /**< \brief 1/14倍基础时钟速模式 */
    AMHW_ZLG_ADIO_PORT_CLK_MODE_1_16BASECLK     = 0x7,   /**< \brief 1/16倍基础时钟速模式 */
} amhw_zlg_port_clk_mode_t;

/**
 * \brief sdio端口时钟配置
 */
typedef enum amhw_zlg_output_selection_mode {
    AMHW_ZLG_SDIO_OUTPUT_SELECTION_OD_MODE    = 0x1,   /**< \brief 输出使用*/
	AMHW_ZLG_SDIO_OUTPUT_SELECTION_PP_MODE    = 0x0,   /**< \brief 使用mmc_port模式 */
} amhw_zlg_output_selection_mode_t;

/**
 * \brief sdio端口时钟配置
 */
typedef enum amhw_zlg_signal_mode {
	AMHW_ZLG_SDIO_SIGNAL_MMC_PORT_MODE    = 0x0,   /**< \brief 使用mmc_port模式 */
    AMHW_ZLG_SDIO_SIGNAL_AUTO_MODE        = 0x1,   /**< \brief 自动传输模式*/
} amhw_zlg_signal_mode_t;

/**
 * \brief sdio端口操作模式选择
 */
typedef enum amhw_zlg_sdio_port_operation_mode {
	AMHW_ZLG_SDIO_PORT_OPER_SPI_MODE    = 0x0,   /**< \brief 使用SPI_模式 */
	AMHW_ZLG_SDIO_PORT_OPER_SDIO_MODE   = 0x1,   /**< \brief SDIO/SD/MMC模式*/
} amhw_zlg_sdio_port_operation_mode_t;

/**
 * \brief sdio端口操作模式选择
 */
typedef enum amhw_zlg_sdio_pclkg_mode {
	AMHW_ZLG_SDIO_PCLKG_SEL_REORSEND    = 0x0,   /**< \brief 端口产生8个空时钟 */
	AMHW_ZLG_SDIO_PCLKG_8FREE_CLK       = 0x1,   /**< \brief 选择接收响应/发送命令*/
} amhw_zlg_sdio_pclkg_t;

/**
 * \brief sdio端口操作模式选择
 */
typedef enum amhw_zlg_resive_or_send_mode {
	AMHW_ZLG_SDIO_RESIVE_MODE     = 0x0,   /**< \brief 使用接收模式 */
	AMHW_ZLG_SDIO_SEND_MODE       = 0x1,   /**< \brief 使用发送模式*/
} amhw_zlg_resive_or_send_mode_t;


/**
 * \brief sdio端口操作模式选择
 */
typedef enum amhw_zlg_transfer_mode {
	AMHW_ZLG_SDIO_WRITE_MODE      = 0x0,   /**< \brief 写模式 */
	AMHW_ZLG_SDIO_READ_MODE       = 0x1,   /**< \brief 读模式*/
} amhw_zlg_transfer_mode_t;

/**
 * \brief 读等待使能函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_read_wait_enable (amhw_zlg_sdio_t              *p_hw_sdio)
{
	p_hw_sdio->mmc_ctrl |= ( 1<<10 );
}

/**
* \brief 读等待禁能函数
*
* \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
*
* \return 无
*/
am_static_inline
void amhw_zlg_sdio_read_wait_disable (amhw_zlg_sdio_t             *p_hw_sdio)
{
	p_hw_sdio->mmc_ctrl &= ~(1 << 10);
}

/**
 * \brief 中断使能函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_int_enable (amhw_zlg_sdio_t              *p_hw_sdio)
{
	p_hw_sdio->mmc_ctrl |= (1 << 9);
}

/**
* \brief 中断禁能函数
*
* \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
*
* \return 无
*/
am_static_inline
void amhw_zlg_sdio_int_disable (amhw_zlg_sdio_t             *p_hw_sdio)
{
	p_hw_sdio->mmc_ctrl &= ~(1 << 9);
}

/**
 * \brief SDIO使能函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_mode_enable (amhw_zlg_sdio_t              *p_hw_sdio)
{
	p_hw_sdio->mmc_ctrl |= (1 << 8);
}

/**
* \brief SDIO禁能函数
*
* \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
*
* \return 无
*/
am_static_inline
void amhw_zlg_sdio_mode_disable (amhw_zlg_sdio_t             *p_hw_sdio)
{
	p_hw_sdio->mmc_ctrl &= ~(1 << 8);
}

/**
 * \brief 总线位宽设置能函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] width    p_hw_sdio 控制寄存器位宽枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_bus_width_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_sdio_width_t width)
{

	if (width) {
		p_hw_sdio->mmc_ctrl |= (1 << 7);
	} else {
		p_hw_sdio->mmc_ctrl &= ~(1 << 7);
	}
}

/**
 * \brief 速率选择传输函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] permit   SDIO 控制传输配置枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_speed_mode_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_sdio_speed_mode_t mode)
{

	if (mode) {
		p_hw_sdio->mmc_ctrl |= (1 << 6);
	} else {
		p_hw_sdio->mmc_ctrl &= ~(1 << 6);
	}
}

/**
 * \brief 速率选择配置函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] permit   SDIO 控制寄存器CLK速率选择枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_prot_clk_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_port_clk_mode_t speed)
{
	p_hw_sdio->mmc_ctrl |= (speed << 3);
}

/**
 * \brief 输出配置函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] mode   SDIO 控制寄存器输出配置枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_output_mode_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_output_selection_mode_t mode)
{
	if (mode) {
	    p_hw_sdio->mmc_ctrl |= (1 << 2);
	} else {
		p_hw_sdio->mmc_ctrl &= ~(1 << 2);
	}
}


/**
 * \brief 信号选择函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] permit   SDIO 控制寄存器高位中断允许枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_signal_mode_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_signal_mode_t mode)
{
	if (mode) {
	    p_hw_sdio->mmc_ctrl |= (1 << 1);
	} else {
		p_hw_sdio->mmc_ctrl &= ~(1 << 1);
	}
}

/**
 * \brief 端口模式选择位配置函数
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] permit   SDIO 控制寄存器高位端口模式枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_port_operation_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_sdio_port_operation_mode_t mode)
{
	if (mode) {
	    p_hw_sdio->mmc_ctrl |= (1 << 0);
	} else {
		p_hw_sdio->mmc_ctrl &= ~(1 << 0);
	}
}

/**
 * \brief
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] permit   SDIO 控制寄存器高位cmd终止标志
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_cmd12_io_flag_set (amhw_zlg_sdio_t *p_hw_sdio, am_bool_t cmd_stop)
{
	if (cmd_stop) {
	    p_hw_sdio->mmc_io |= (1 << 9);
	} else {
		p_hw_sdio->mmc_io &= ~(1 << 9);
	}
}

/**
 * \brief
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return AM_TRUE  是cmd12/IO终止命令
 *         AM_FALSE 不是cmd12/IO终止命令
 */
am_static_inline
am_bool_t amhw_zlg_sdio_cmd12_io_flag_get (amhw_zlg_sdio_t *p_hw_sdio)
{
    if ((p_hw_sdio->mmc_io) & (1 << 9)) {
    	return AM_TRUE;
    } else {
    	return AM_FALSE;
    }
}

/**
 * \brief 设置sdio的特征位
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] permit   当前指令后是否跟数据块
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_cmdch_flag_set (amhw_zlg_sdio_t *p_hw_sdio, am_bool_t cmd_stop)
{
	if (cmd_stop) {
	    p_hw_sdio->mmc_io |= (1 << 8);
	} else {
		p_hw_sdio->mmc_io &= ~(1 << 8);
	}
}

/**
 * \brief
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return AM_TRUE  当前指令后跟数据块
 *         AM_FALSE 当前指令后不跟数据块
 */
am_static_inline
am_bool_t amhw_zlg_sdio_cmdch_flag_get (amhw_zlg_sdio_t *p_hw_sdio)
{
    if ((p_hw_sdio->mmc_io) & (1 << 8)) {
    	return AM_TRUE;
    } else {
    	return AM_FALSE;
    }
}

/**
 * \brief 使能单个数据块
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_attoclkg_enable (amhw_zlg_sdio_t *p_hw_sdio)
{
	p_hw_sdio->mmc_io |= (1 << 7);
}

/**
 * \brief 禁能单个数据块
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_attoclkg_disable (amhw_zlg_sdio_t *p_hw_sdio)
{
	p_hw_sdio->mmc_io &= ~(1 << 7);
}

/**
 * \brief 启动命令后自动接收响应
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_enrresp_enable (amhw_zlg_sdio_t *p_hw_sdio)
{
	p_hw_sdio->mmc_io |= (1 << 6);
}

/**
 * \brief 禁能命令后自动接收响应
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_enrresp_disable (amhw_zlg_sdio_t *p_hw_sdio)
{
	p_hw_sdio->mmc_io &= ~(1 << 6);
}

/**
 * \brief 设置端口时钟线
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] mode   SDIO 控制寄存器端口时钟选择枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_pclkg_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_resive_or_send_mode_t mode)
{
	if (mode == AMHW_ZLG_SDIO_SEND_MODE) {
	    p_hw_sdio->mmc_io |= (1 << 3);
	} else {
		p_hw_sdio->mmc_io &= ~(1 << 3);
	}
}

/**
 * \brief 使能自动响应传输
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_autotr_enable (amhw_zlg_sdio_t *p_hw_sdio)
{
	    p_hw_sdio->mmc_io |= (1 << 2);
}

/**
 * \brief 禁能自动响应传输
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_autotr_disable (amhw_zlg_sdio_t *p_hw_sdio)
{
		p_hw_sdio->mmc_io &= ~(1 << 2);
}

/**
 * \brief 配置读写模式
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 * \param[in] mode   SDIO 控制寄存器读写枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_transfdir_set (amhw_zlg_sdio_t *p_hw_sdio, amhw_zlg_transfer_mode_t mode)
{
	if (mode == AMHW_ZLG_SDIO_READ_MODE) {
	    p_hw_sdio->mmc_io |= (1 << 1);
	} else {
		p_hw_sdio->mmc_io &= ~(1 << 1);
	}
}

/**
 * \brief
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_autodattr_enable (amhw_zlg_sdio_t *p_hw_sdio)
{
	    p_hw_sdio->mmc_io |= (1 << 0);
}

/**
 * \brief
 *
 * \param[in] p_hw_sdio 指向 amhw_zlg_sdio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_sdio_autodattr_disable (amhw_zlg_sdio_t *p_hw_sdio)
{
		p_hw_sdio->mmc_io &= ~(1 << 0);
}

/* 未完待续 。。。 */




/* end of file */
