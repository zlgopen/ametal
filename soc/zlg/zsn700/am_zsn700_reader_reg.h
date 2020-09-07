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
 * \brief ZSN700_READER寄存器定义
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-01  htf, first implementation.
 * \endinternal
 */
 
#ifndef __ZSN700_READER_REG_H
#define __ZSN700_READER_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_zsn700_reader.h"
/** 
 * \addtogroup am_if_zsn700_reg
 * \copydoc am_zsn700_reg.h
 * @{
 */

/**
 * \name ZSN700_READER_REG ZSN700_READER寄存器定义
 * @{
 */
#define AM_ZSN700_READER_TXMODE_REG            0x12   /**< \brief 发射数据速率和帧格式*/
#define AM_ZSN700_READER_RXMODE_REG            0x13   /**< \brief 接收数据速率和帧格式 */
#define AM_ZSN700_READER_TXCONTROL_REG         0x14   /**< \brief 天线驱动引脚TX1和TX2控制 */
#define AM_ZSN700_READER_RXTHRESHOLD_REG       0x18   /**< \brief 接收译码器阈值选择 */
#define AM_ZSN700_READER_MODWIDTH_REG          0x24   /**< \brief 调制宽度控制 */
#define AM_ZSN700_READER_RFCFG_REG             0x26   /**< \brief 配置接收器增益和RF电平 */
#define AM_ZSN700_READER_GSN_REG               0x27   /**< \brief 选择天线驱动引脚TX1和TX2的电导率 */
#define AM_ZSN700_READER_CWGSP_REG             0x28   /**< \brief 选择天线驱动引脚TX1和TX2的电导率 */
#define AM_ZSN700_READER_MODGSP_REG            0x29   /**< \brief 选择天线驱动引脚TX1和TX2的电导率 */

/** @} */


/**
 * \name Command命令寄存器掩码 (0x01)
 * @{
 */

#define AM_ZSN700_READER_COMMAND_RCV_OFF         0x20        /**< \brief 射频模拟接收器关闭 */
#define AM_ZSN700_READER_COMMAND_POWER_DOWN      0x10        /**< \brief 进入软件掉电模式 */
#define AM_ZSN700_READER_COMMAND_MASK            0x0F        /**< \brief 指令寄存器掩码 */

/** @} */


/**
 * \name CommIEN中断请求使能与禁止控制寄存器掩码 (0x02)
 * @{
 */

#define AM_ZSN700_READER_COMMIEN_IRQ_INV         0x80    /**< \brief IRQ信号取反 缺省为1 */
#define AM_ZSN700_READER_COMMIEN_TX_IEN          0x40    /**< \brief 允许TXIRQ发射中断请求传递到IRQ */
#define AM_ZSN700_READER_COMMIEN_RX_IEN          0x20    /**< \brief 允许RXIRQ接收中断请求传递到IRQ */
#define AM_ZSN700_READER_COMMIEN_IDLE_IEN        0x10    /**< \brief 允许IDLEIRQ空闲中断请求传递到IRQ */
#define AM_ZSN700_READER_COMMIEN_HIALERT_IEN     0x08    /**< \brief 允许HIALERTIRQ渐满中断请求传递到IRQ */
#define AM_ZSN700_READER_COMMIEN_LOALERT_IEN     0x04    /**< \brief 允许HIALERTIRQ渐空中断请求传递到IRQ */
#define AM_ZSN700_READER_COMMIEN_ERR_IEN         0x02    /**< \brief 允许ERRIRQ错误中断请求传递到IRQ */
#define AM_ZSN700_READER_COMMIEN_TIMER_IEN       0x01    /**< \brief 允许TIMERIRQ定时器中断请求传递到IRQ */

/** @} */




/**
 * \name TxMode寄存器掩码,定义发射数据速率和帧格式 (0x12)
 * @{
 */
 
#define AM_ZSN700_READER_TXMODE_TXCRC_EN         0x80    /**< \brief 置1数据发射时使能CRC校验生成(仅106kbit下能置0) */
#define AM_ZSN700_READER_TXMODE_TXSPEED_MASK     0x70    /**< \brief 发送速率掩码 */
#define AM_ZSN700_READER_TXMODE_TXSPEED_106KBIT  0x00    /**< \brief 发送速率106kbit */
#define AM_ZSN700_READER_TXMODE_TXSPEED_212KBIT  0x10    /**< \brief 发送速率212kbit */
#define AM_ZSN700_READER_TXMODE_TXSPEED_424KBIT  0x20    /**< \brief 发送速率424kbit */
#define AM_ZSN700_READER_TXMODE_TXSPEED_848KBIT  0x30    /**< \brief 发送速率848kbit */
#define AM_ZSN700_READER_TXMODE_INV_MOD          0x08    /**< \brief 置1,发送数据的调制反相 */

/** @} */


/**
 * \name RxMode寄存器掩码,定义接收数据速率和帧格式 (0x13)
 * @{
 */
 
#define AM_ZSN700_READER_RXMODE_RXCRC_EN         0x80    /**< \brief 置1数据接收时使能CRC校验生成(仅106kbit下能置0) */
#define AM_ZSN700_READER_RXMODE_RXSPEED_MASK     0x70    /**< \brief 发送速率掩码 */
#define AM_ZSN700_READER_RXMODE_RXSPEED_106KBIT  0x00    /**< \brief 发送速率106kbit */
#define AM_ZSN700_READER_RXMODE_RXSPEED_212KBIT  0x10    /**< \brief 发送速率212kbit */
#define AM_ZSN700_READER_RXMODE_RXSPEED_424KBIT  0x20    /**< \brief 发送速率424kbit */
#define AM_ZSN700_READER_RXMODE_RXSPEED_848KBIT  0x30    /**< \brief 发送速率848kbit */
#define AM_ZSN700_READER_RXMODE_RXNOERR          0x08    /**< \brief 置1,忽略无效数据流,接收器保持激活.对于14443B,还需RxSOFReq设为1 */
#define AM_ZSN700_READER_RXMODE_RXMULTIPLE       0x04    /**< \brief 置0,接收器接收到一个数据帧之后不再接收.置1,允许接收多个数据帧 */

/** @} */


/**
 * \name TxControl寄存器掩码,天线驱动引脚Tx1和Tx2控制 (0x14)
 *
 * @{
 */
#define AM_ZSN700_READER_TXCONTROL_INV_TX2RF_ON  0x80    /**< \brief 置1,若TX2驱动使能,则引脚TX2的输出信号取反 */
#define AM_ZSN700_READER_TXCONTROL_INV_TX1RF_ON  0x40    /**< \brief 置1,若TX1驱动使能,则引脚TX2的输出信号取反 */
#define AM_ZSN700_READER_TXCONTROL_INV_TX2RF_OFF 0x20    /**< \brief 置1,若TX2驱动关闭,则引脚TX2的输出信号取反 */
#define AM_ZSN700_READER_TXCONTROL_INV_TX1RF_OFF 0x10    /**< \brief 置1,若TX1驱动关闭,则引脚TX2的输出信号取反 */
#define AM_ZSN700_READER_TXCONTROL_TX2_CW        0x08    /**< \brief 置1,TX2将持续输出未调制的13.56MHz的能量载波;置0,则调制 */
#define AM_ZSN700_READER_TXCONTROL_TX2RF_EN      0x02    /**< \brief 置1,TX2输出经由发送数据调制的13.56MHz能量载波 */
#define AM_ZSN700_READER_TXCONTROL_TX1RF_EN      0x01    /**< \brief 置1,TX1输出经由发送数据调制的13.56MHz能量载波 */

/** @} */

/**
 * \name RxThreshold寄存器掩码,接收译码器阈值选择 (0x18)
 * @{
 */

/** \brief 译码器能接收的最小信号强度(低于这个水平不被处理) */
#define AM_ZSN700_READER_RXTHRESHOLD_MINLEVEL   0xf0

/** \brief 输入到译码器的曼切斯特编码的弱半bit相对强半bit产生冲突位的最小信号强度 */
#define AM_ZSN700_READER_RXTHRESHOLD_COLLLEVEL  0x07

/** @} */


/**
 * \name RFCfg寄存器掩码,配置接收器增益 (0x26)
 * @{
 */
 
#define AM_ZSN700_READER_RFCFG_RXGAIN_MASK       0x70    /**< \brief 接收器电压增益指数掩码 */
#define AM_ZSN700_READER_RFCFG_RXGAIN_18DB       0x00    /**< \brief 接收器电压增益指数18dB */
#define AM_ZSN700_READER_RFCFG_RXGAIN_23DB       0x10    /**< \brief 接收器电压增益指数23dB */
#define AM_ZSN700_READER_RFCFG_RXGAIN_18DB_TOO   0x20    /**< \brief 接收器电压增益指数18dB */
#define AM_ZSN700_READER_RFCFG_RXGAIN_23DB_TOO   0x30    /**< \brief 接收器电压增益指数23dB */
#define AM_ZSN700_READER_RFCFG_RXGAIN_33DB       0x40    /**< \brief 接收器电压增益指数33dB */
#define AM_ZSN700_READER_RFCFG_RXGAIN_38DB       0x50    /**< \brief 接收器电压增益指数38dB */
#define AM_ZSN700_READER_RFCFG_RXGAIN_43DB       0x60    /**< \brief 接收器电压增益指数43dB */
#define AM_ZSN700_READER_RFCFG_RXGAIN_48DB       0x70    /**< \brief 接收器电压增益指数48dB */

/** @} */


/**
 * \name GsN寄存器掩码,驱动开启时,选择天线驱动引脚TX1和TX2的N驱动的电导 (0x27)
 * @{
 */

/**
 * \brief 无调制时段输出N驱动的电导,可设置输出功率,电流驱动及操作距离:
 *        1.电导值位二进制加权
 *        2.soft Power-down模式下最高位强制置位 
 *        3.该寄存器的值只在TX1,TX2驱动开启时使用
 */
#define AM_ZSN700_READER_CWGSN                   0xf0

/**
 * \brief 调制时段输出N驱动的电导,可控制调制深度:
 *        1.电导值位二进制加权
 *        2.soft Power-down模式下最高位强制置位 
 *        3.该寄存器的值只在TX1,TX2驱动开启时使用
 */
#define AM_ZSN700_READER_MODGSN                  0x0f

/** @} */


/**
 * \name CWGsP寄存器掩码,定义了无调制时段P驱动的电导 (0x28)
 * @{
 */

/**
 * \brief 无调制时段输出P驱动的电导,可设置输出功率,电流功耗及操作距离:
 *        1.电导值位二进制加权
 *        2.soft Power-down模式下最高位强制置位 
 */
#define AM_ZSN700_READER_CWGSP                   0x3f

/** @} */


/**
 * \name ModGsP寄存器掩码 (0x29)
 * @{
 */

/**
 * \brief 调制时段输出P驱动的电导,可设置输出功率,电流功耗及操作距离:
 *        1.电导值位二进制加权
 *        2.soft Power-down模式下最高位强制置位 
 */
#define AM_ZSN700_READER_MODGSP                  0x3f

/** @} */


/**
 * @} am_if_zsn700_reg
 */

#ifdef __cplusplus
}
#endif
 
#endif /* __ZSN700_READER_REG_H */

/* end of file */
