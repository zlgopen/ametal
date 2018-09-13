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
 * \brief FM175XX寄存器定义
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-21  sky, first implementation.
 * - 1.01 17-10-31  sdq, make somd changes.
 * \endinternal
 */
 
#ifndef __FM175XX_REG_H
#define __FM175XX_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_fm175xx.h"
/** 
 * \addtogroup am_if_fm175xx_reg
 * \copydoc am_fm175xx_reg.h
 * @{
 */

/**
 * \name FM175XX_REG FM175XX寄存器定义
 * @{
 */
#define AM_FM175XX_COMMAND_REG           0x01   /**< \brief 启动和停止命令执行 */
#define AM_FM175XX_COMIEN_REG            0x02   /**< \brief 中断请求使能与禁止位 */
#define AM_FM175XX_DIVIEN_REG            0x03   /**< \brief 中断请求使能与禁止位 */
#define AM_FM175XX_COMIRQ_REG            0x04   /**< \brief 控制中断请求位 */
#define AM_FM175XX_DIVIRQ_REG            0x05   /**< \brief 控制中断请求位 */
#define AM_FM175XX_ERROR_REG             0x06   /**< \brief 显示上一条执行指令的错误状态 */
#define AM_FM175XX_STATUS1_REG           0x07   /**< \brief 通讯控制状态位 */
#define AM_FM175XX_STATUS2_REG           0x08   /**< \brief 接收机和发射器的控制状态位 */
#define AM_FM175XX_FIFODATA_REG          0x09   /**< \brief 64位fifo的输入输出 */
#define AM_FM175XX_FIFOLEVEL_REG         0x0a   /**< \brief fifo内有效数据深度 */
#define AM_FM175XX_WATERLEVEL_REG        0x0b   /**< \brief fifo上溢下溢警告的级别 */
#define AM_FM175XX_CONTROL_REG           0x0c   /**< \brief 控制寄存器 */
#define AM_FM175XX_BITFRAMING_REG        0x0d   /**< \brief 面向位的帧格式调整 */
#define AM_FM175XX_COLL_REG              0x0e   /**< \brief RF接口检测到第一个冲突位的位置 */
#define AM_FM175XX_EX_REG                0x0f   /**< \brief 扩展寄存器 */
#define AM_FM175XX_MODE_REG              0x11   /**< \brief 接收发射模式 */
#define AM_FM175XX_TXMODE_REG            0x12   /**< \brief 发射数据速率和帧格式*/
#define AM_FM175XX_RXMODE_REG            0x13   /**< \brief 接收数据速率和帧格式 */
#define AM_FM175XX_TXCONTROL_REG         0x14   /**< \brief 天线驱动引脚TX1和TX2控制 */
#define AM_FM175XX_TXASK_REG             0x15   /**< \brief 天线驱动设置 */
#define AM_FM175XX_TXSEL_REG             0x16   /**< \brief 天线驱动信号源选择 */
#define AM_FM175XX_RXSEL_REG             0x17   /**< \brief 内部接收器设置 */
#define AM_FM175XX_RXTHRESHOLD_REG       0x18   /**< \brief 接收译码器阈值选择 */
#define AM_FM175XX_DEMOD_REG             0x19   /**< \brief 解调器设置 */
#define AM_FM175XX_TX_REG                0x1c   /**< \brief 控制部分ISO/IEC 14443发射参数 */
#define AM_FM175XX_RX_REG                0x1d   /**< \brief 控制部分ISO/IEC 14443接收参数 */
#define AM_FM175XX_TYPEB_REG             0x1e   /**< \brief ISO/IEC 14443B 控制（FM17510该寄存器未使用） */
#define AM_FM175XX_SERIALSPEED_REG       0x1f   /**< \brief 选择串行UART接口速度（FM17510该寄存器未使用） */
#define AM_FM175XX_CRCMSB_REG            0x21   /**< \brief CRC计算结果高段 */
#define AM_FM175XX_CRCLSB_REG            0x22   /**< \brief CRC计算结果低段 */
#define AM_FM175XX_MODWIDTH_REG          0x24   /**< \brief 调制宽度控制 */
#define AM_FM175XX_RFCFG_REG             0x26   /**< \brief 配置接收器增益和RF电平 */
#define AM_FM175XX_GSN_REG               0x27   /**< \brief 选择天线驱动引脚TX1和TX2的电导率 */
#define AM_FM175XX_CWGSP_REG             0x28   /**< \brief 选择天线驱动引脚TX1和TX2的电导率 */
#define AM_FM175XX_MODGSP_REG            0x29   /**< \brief 选择天线驱动引脚TX1和TX2的电导率 */
#define AM_FM175XX_TMODE_REG             0x2a   /**< \brief 内部定时器模式设置 */
#define AM_FM175XX_TPRESCALER_REG        0x2b   /**< \brief 内部定时器预定标设置 */
#define AM_FM175XX_TRELOADMSB_REG        0x2c   /**< \brief 内部定时器16位重载值高位 */
#define AM_FM175XX_TRELOADLSB_REG        0x2d   /**< \brief 内部定时器16位重载值低位 */
#define AM_FM175XX_TCOUNTERVALMSB_REG    0x2e   /**< \brief 内部定时器16位计数值高位 */
#define AM_FM175XX_TCOUNTERVALLSB_REG    0x2f   /**< \brief 内部定时器16位计数值低位 */
#define AM_FM175XX_TESTSEL1_REG          0x31   /**< \brief 测试信号配置 */
#define AM_FM175XX_TESTSEL2_REG          0x32   /**< \brief 测试信号配置及PRBS控制 */
#define AM_FM175XX_TESTPINEN_REG         0x33   /**< \brief D1-D7引脚输出驱动使能（FM17510该寄存器未使用） */
#define AM_FM175XX_TESTPINVALUE_REG      0x34   /**< \brief 当被用于I/O总线时,定义D1-D7引脚的值（FM17510该寄存器未使用） */
#define AM_FM175XX_TESTBUS_REG           0x35   /**< \brief 显示内部测试总线的状态 */
#define AM_FM175XX_TESTCTRL_REG          0x36   /**< \brief 测试控制 */
#define AM_FM175XX_ANALOGTEST_REG        0x38   /**< \brief 控制引脚AUX1和AUX2 */
#define AM_FM175XX_TESTDAC1_REG          0x39   /**< \brief 为TestDAC1定义测试值 */
#define AM_FM175XX_TESTDAC2_REG          0x3a   /**< \brief 为TestDAC2定义测试值（FM17510该寄存器未使用） */
#define AM_FM175XX_TESTADC_REG           0x3b   /**< \brief 显示ADC I和Q的实际值 */

/** @} */


/**
 * \name FM175XX_REG FM175XX拓展寄存器定义
 * @{
 */
#define AM_FM175XX_LPCD_CTRL1            0x01
#define AM_FM175XX_LCPD_CTRL2            0X02
#define AM_FM175XX_LPCD_CTRL3            0x03
#define AM_FM175XX_LPCD_CTRL4            0x04
#define AM_FM175XX_LPCD_BIAS_CURRENT     0x05
#define AM_FM175XX_LPCD_ACD_REFERECE     0x06
#define AM_FM175XX_LPCD_SLEEP_CFG        0x07
#define AM_FM175XX_LPCD_READY_CFG        0x08
#define AM_FM175XX_LPCD_DETECT_CFG       0x09
#define AM_FM175XX_LPCD_VMIDBD_CFG       0x0A
#define AM_FM175XX_LPCD_AUTO_WUP_CFG     0x0B
#define AM_FM175XX_LPCD_ADC_RESULT_L     0x0C
#define AM_FM175XX_LPCD_ADC_RESULT_H     0x0D
#define AM_FM175XX_LPCD_THRESHOLD_MIN_L  0x0E
#define AM_FM175XX_LPCD_THRESHOLD_MIN_H  0x0F
#define AM_FM175XX_LPCD_THRESHOLD_MAX_L  0X10
#define AM_FM175XX_LPCD_THRESHOLD_MAX_H  0x11
#define AM_FM175XX_LPCD_IRQ              0x12
#define AM_FM175XX_LPCD_RFT1             0x13
#define AM_FM175XX_LPCD_RFT2             0x14
#define AM_FM175XX_LPCD_RFT3             0x15
#define AM_FM175XX_LPCD_RFT4             0x16
#define AM_FM175XX_LPCD_RFT5             0x1B
#define AM_FM175XX_LPCD_MISC             0x1C


/** @} */


/**
 * \name Command命令寄存器掩码 (0x01)
 * @{
 */

#define AM_FM175XX_COMMAND_RCV_OFF         0x20        /**< \brief 射频模拟接收器关闭 */
#define AM_FM175XX_COMMAND_POWER_DOWN      0x10        /**< \brief 进入软件掉电模式 */
#define AM_FM175XX_COMMAND_MASK            0x0F        /**< \brief 指令寄存器掩码 */

/** @} */


/**
 * \name CommIEN中断请求使能与禁止控制寄存器掩码 (0x02)
 * @{
 */

#define AM_FM175XX_COMMIEN_IRQ_INV         0x80    /**< \brief IRQ信号取反 缺省为1 */
#define AM_FM175XX_COMMIEN_TX_IEN          0x40    /**< \brief 允许TXIRQ发射中断请求传递到IRQ */
#define AM_FM175XX_COMMIEN_RX_IEN          0x20    /**< \brief 允许RXIRQ接收中断请求传递到IRQ */
#define AM_FM175XX_COMMIEN_IDLE_IEN        0x10    /**< \brief 允许IDLEIRQ空闲中断请求传递到IRQ */
#define AM_FM175XX_COMMIEN_HIALERT_IEN     0x08    /**< \brief 允许HIALERTIRQ渐满中断请求传递到IRQ */
#define AM_FM175XX_COMMIEN_LOALERT_IEN     0x04    /**< \brief 允许HIALERTIRQ渐空中断请求传递到IRQ */
#define AM_FM175XX_COMMIEN_ERR_IEN         0x02    /**< \brief 允许ERRIRQ错误中断请求传递到IRQ */
#define AM_FM175XX_COMMIEN_TIMER_IEN       0x01    /**< \brief 允许TIMERIRQ定时器中断请求传递到IRQ */

/** @} */


/**
 * \name DivIEN中断请求使能与禁止控制寄存器掩码 (0x03)
 * @{
 */
 
#define AM_FM175XX_DIVIEN_IRQ_PUSHPULL     0x80    /**< \brief IRQ引脚工作模式 1CMOS 0开漏输出 */
#define AM_FM175XX_DIVIEN_TINACT_IEN       0x10    /**< \brief 允许TIN中断请求传递到IRQ（FM17510不支持） */
#define AM_FM175XX_DIVIEN_CRC_IEN          0x04    /**< \brief 允许CRC中断请求传递到IRQ */

/** @} */


/**
 * \name CommIRq中断控制请求寄存器掩码 (0x04)
 * @{
 */
 
#define AM_FM175XX_COMMIRQ_SET1            0x80    /**< \brief COMMIRQ中断标记 写0清0寄存器内所有标记 */
#define AM_FM175XX_COMMIRQ_TX_IRQ          0x40    /**< \brief 发射数据的最后一位发出后立刻置1 */
#define AM_FM175XX_COMMIRQ_RX_IRQ          0x20    /**< \brief 接收到数据末尾时置1 */
#define AM_FM175XX_COMMIRQ_IDLE_IRQ        0x10    /**< \brief 执行到未知指令时置1 */
#define AM_FM175XX_COMMIRQ_HIALERT_IRQ     0x08    /**< \brief STATUS1寄存器的HIALERT置1时置1 */
#define AM_FM175XX_COMMIRQ_LOALERT_IRQ     0x04    /**< \brief STATUS1寄存器的LOALERT置1时置1 */
#define AM_FM175XX_COMMIRQ_ERR_IRQ         0x02    /**< \brief ERR寄存器中有任何err位时置1 */
#define AM_FM175XX_COMMIRQ_TIMER_IRQ       0x01    /**< \brief TimerValue计数归0时置1 */

/** @} */


/**
 * \name DivIRq中断控制请求寄存器掩码 (0x05)
 * @{
 */
 
#define AM_FM175XX_DIVIRQ_SET2             0x80    /**< \brief DIVIRQ中断标记 写1清0寄存器内所有标记 */
#define AM_FM175XX_DIVIRQ_TINACT_IRQ       0x10    /**< \brief TIN激活时 置1（FM17510不支持） */
#define AM_FM175XX_DIVIRQ_CRC_IRQ          0x04    /**< \brief CRC指令激活且所有数据处理完成后置1 */

/** @} */


/**
 * \name Error错误寄存器掩码 (0x06)
 * @{
 */
 
#define AM_FM175XX_ERROR_WR_ERR            0x80    /**< \brief 传输过程中数据被主控写入fifo时置位 */
#define AM_FM175XX_ERROR_TEMP_ERR          0x40    /**< \brief 内部温度传感器检测过热时置1 */
#define AM_FM175XX_ERROR_BUFFER_ERR        0x10    /**< \brief fifo已满时试图写fifo置1 */
#define AM_FM175XX_ERROR_COLL_ERR          0x08    /**< \brief 检测到冲突置1(仅A协议中106kbit时有效),接收器启动时自动清零 */
#define AM_FM175XX_ERROR_CRC_ERR           0x04    /**< \brief RxMode寄存器的RxCRCEN置1且CRC计算失败时置1,接收器启动时自动清零 */
#define AM_FM175XX_ERROR_PARITY_ERR        0x02    /**< \brief 奇偶校验失败时置1(仅A协议中106kbit时有效),接收器启动时自动清零 */
#define AM_FM175XX_ERROR_PROTOCOL_ERR      0x01    /**< \brief SOF错误或Authent数据流字节数不正确时置1 */

/** @} */


/**
 * \name Status1状态寄存器1掩码 (0x07)
 * @{
 */
 
#define AM_FM175XX_STATUS1_CRC_OK          0x40    /**< \brief CRC计算结果为0 */
#define AM_FM175XX_STATUS1_CRC_READY       0x20    /**< \brief CRC计算结束 */
#define AM_FM175XX_STATUS1_IRQ             0x10    /**< \brief 有中断 */
#define AM_FM175XX_STATUS1_TRUNING         0x08    /**< \brief 定时器正在运行 */
#define AM_FM175XX_STATUS1_HIALERT         0x02    /**< \brief fifo中的HIALERT 字节数小于WaterLevel */
#define AM_FM175XX_STATUS1_LOALERT         0x01    /**< \brief fifo中的LOALERT 字节数小于WaterLevel */

/** @} */


/**
 * \name Status2状态寄存器2掩码 (0x08)
 * @{
 */
 
#define AM_FM175XX_STATUS2_TEMP_SENS_CLEAR     0x80    /**< \brief 温度在125℃警报下时,置该位可以清除温度错误 */
#define AM_FM175XX_STATUS2_I2C_FORCE_HS        0x40    /**< \brief I2C输入滤波器设置.置1为高速模式,置0则根据应用的I2C协议调整. */
#define AM_FM175XX_STATUS2_CRYPTO1_ON          0x08    /**< \brief M1 Crypto1单元开启.M1卡有效,在成功执行Authent指令后置1,可软件清除 */
#define AM_FM175XX_STATUS2_MODEM_STATE_MASK    0x07    /**< \brief 发收状态机状态掩码 */
#define AM_FM175XX_STATUS2_MODEM_STATE_IDLE    0x00    /**< \brief 发收状态机空闲 */
#define AM_FM175XX_STATUS2_MODEM_STATE_SS      0x01    /**< \brief 发收状态机等待StartSend有效 */
#define AM_FM175XX_STATUS2_MODEM_STATE_TW      0x02    /**< \brief 发收状态机TxWait,直到RF场出现 */
#define AM_FM175XX_STATUS2_MODEM_STATE_SEND    0x03    /**< \brief 发收状态机发送中 */
#define AM_FM175XX_STATUS2_MODEM_STATE_RW      0x04    /**< \brief 发收状态机RxWait,直到RF场出现 */
#define AM_FM175XX_STATUS2_MODEM_STATE_DW      0x05    /**< \brief 发收状态机DataWait */
#define AM_FM175XX_STATUS2_MODEM_STATE_RECV    0x06    /**< \brief 发收状态机接收中 */

/** @} */


/**
 * \name FIFOLevel寄存器掩码 (0x0a)
 * @{
 */
 
#define AM_FM175XX_FIFOLEVEL_FLUSH_BUFFER      0x80    /**< \brief 置1清除内部fifo缓冲器的读写指针和ERR寄存器中的BufferOvfl位,读总为0 */
#define AM_FM175XX_FIFOLEVEL_MASK              0x7f    /**< \brief fifo缓冲区中的字节数,写入+1,读取-1 */

/** @} */


/**
 * \name WaterLevel寄存器掩码 (0x0b)
 * @{
 */
 
#define AM_FM175XX_WATERLEVEL_MASK         0x3F    /**< \brief fifo分水岭,上溢下溢分界 */

/** @} */


/**
 * \name Control其他控制寄存器掩码 (0x0c)
 * @{
 */
 
#define AM_FM175XX_CONTROL_TSTOPNOW        0x80    /**< \brief 定时器立刻停止 */
#define AM_FM175XX_CONTROL_TSTARTNOW       0x40    /**< \brief 定时器立刻启动 */
#define AM_FM175XX_CONTROL_RX_LAST_BITS    0x07    /**< \brief 最后接收字节的有效位数,为0则全部有效 */

/** @} */


/**
 * \name BitFraming寄存器掩码,面向位的帧格式调整 (0x0d)
 * @{
 */
 
#define AM_FM175XX_BITFRAMING_STARTSEND    0x80    /**< \brief 数据发射开始,该位只在Transceiver执行时有效 */
#define AM_FM175XX_BITFRAMING_RXALIGN_0    0x00    /**< \brief 最低位存在第0位,后续位顺延 */
#define AM_FM175XX_BITFRAMING_RXALIGN_1    0x10    /**< \brief 最低位存在第1位,后续位顺延 */
#define AM_FM175XX_BITFRAMING_RXALIGN_7    0x70    /**< \brief 最低位存在第7位,后续位顺延 */
#define AM_FM175XX_BITFRAMING_TXLASTBITS   0x07    /**< \brief 最后发送字节的需发送的位数,为0则全部需要发送 */

/** @} */


/**
 * \name CollIRq寄存器掩码,RF接口检测到的第一个冲突位 (0x0e)
 * @{
 */

/** \brief 置0则发生冲突位之后的所有接收位被清除(106kbit有效,否则置1) */
#define AM_FM175XX_COLLIRQ_VALUES_AFTERCOLL    0x80

/** \brief 没检测到冲突或冲突在CollPos范围外时置1(该位只在14443A读写器模式下被解读) */
#define AM_FM175XX_COLLIRQ_COLLPOS_NOTVALID    0x20

/** \brief 第一个冲突位位置,00表示32位,01表示1位(该位只在14443A读写器模式下,且COLLPOS_NOTVALID为0时被解读) */
#define AM_FM175XX_COLLIRQ_COLLPOS             0x1f

/** @} */


/**
 * \name EX扩展寄存器掩码 (0x0f)
 * @{
 */

#define AM_FM175XX_EX_MODE                 0xc0    /**< \brief 扩展寄存器访问模式 */
#define AM_FM175XX_EX_MODE_W_ADDR          0x40    /**< \brief 扩展寄存器写入二级地址 */
#define AM_FM175XX_EX_MODE_R_ADDR          0x80    /**< \brief 扩展寄存器读出二级地址 */
#define AM_FM175XX_EX_MODE_W_DATA          0xc0    /**< \brief 扩展寄存器写入扩展数据 */
#define AM_FM175XX_EX_MODE_R_DATA          0x00    /**< \brief 扩展寄存器读出扩展数据 */
#define AM_FM175XX_EX_MODE_ADDR_DATA       0x3f    /**< \brief 二级地址或数据 */

/** @} */

/**
 * \name Mode寄存器掩码,定义发射和接收模式 (0x11)
 * @{
 */

#define AM_FM175XX_MODE_MSB_FIRST          0x80    /**< \brief CRC协处理器以MSB位优先计算CRC,CRCResult寄存器中断MSB和LSB也按位翻转 */
#define AM_FM175XX_MODE_TXWAIT_RF          0x20    /**< \brief 读写器的模式时只有RF场建立后发射电路才启动 */
#define AM_FM175XX_MODE_POLTIN             0x08    /**< \brief TIN引脚的极性,1高电平有效,0低电平有效（FM17510不支持） */
#define AM_FM175XX_MODE_CRC_PRESET_MASK    0x03    /**< \brief CRC校验预设值 */
#define AM_FM175XX_MODE_CRC_PRESET_0000    0x00    /**< \brief CRC校验预设值0000 */
#define AM_FM175XX_MODE_CRC_PRESET_6363    0x01    /**< \brief CRC校验预设值6363 */
#define AM_FM175XX_MODE_CRC_PRESET_a671    0x02    /**< \brief CRC校验预设值a671 */
#define AM_FM175XX_MODE_CRC_PRESET_ffff    0x03    /**< \brief CRC校验预设值ffff */

/** @} */


/**
 * \name TxMode寄存器掩码,定义发射数据速率和帧格式 (0x12)
 * @{
 */
 
#define AM_FM175XX_TXMODE_TXCRC_EN         0x80    /**< \brief 置1数据发射时使能CRC校验生成(仅106kbit下能置0) */
#define AM_FM175XX_TXMODE_TXSPEED_MASK     0x70    /**< \brief 发送速率掩码 */
#define AM_FM175XX_TXMODE_TXSPEED_106KBIT  0x00    /**< \brief 发送速率106kbit */
#define AM_FM175XX_TXMODE_TXSPEED_212KBIT  0x10    /**< \brief 发送速率212kbit */
#define AM_FM175XX_TXMODE_TXSPEED_424KBIT  0x20    /**< \brief 发送速率424kbit */
#define AM_FM175XX_TXMODE_TXSPEED_848KBIT  0x30    /**< \brief 发送速率848kbit */
#define AM_FM175XX_TXMODE_INV_MOD          0x08    /**< \brief 置1,发送数据的调制反相 */

/** @} */


/**
 * \name RxMode寄存器掩码,定义接收数据速率和帧格式 (0x13)
 * @{
 */
 
#define AM_FM175XX_RXMODE_RXCRC_EN         0x80    /**< \brief 置1数据接收时使能CRC校验生成(仅106kbit下能置0) */
#define AM_FM175XX_RXMODE_RXSPEED_MASK     0x70    /**< \brief 发送速率掩码 */
#define AM_FM175XX_RXMODE_RXSPEED_106KBIT  0x00    /**< \brief 发送速率106kbit */
#define AM_FM175XX_RXMODE_RXSPEED_212KBIT  0x10    /**< \brief 发送速率212kbit */
#define AM_FM175XX_RXMODE_RXSPEED_424KBIT  0x20    /**< \brief 发送速率424kbit */
#define AM_FM175XX_RXMODE_RXSPEED_848KBIT  0x30    /**< \brief 发送速率848kbit */
#define AM_FM175XX_RXMODE_RXNOERR          0x08    /**< \brief 置1,忽略无效数据流,接收器保持激活.对于14443B,还需RxSOFReq设为1 */
#define AM_FM175XX_RXMODE_RXMULTIPLE       0x04    /**< \brief 置0,接收器接收到一个数据帧之后不再接收.置1,允许接收多个数据帧 */

/** @} */


/**
 * \name TxControl寄存器掩码,天线驱动引脚Tx1和Tx2控制 (0x14)
 *
 * \note FM17510没有TX2相关的定义
 * @{
 */
#define AM_FM175XX_TXCONTROL_INV_TX2RF_ON  0x80    /**< \brief 置1,若TX2驱动使能,则引脚TX2的输出信号取反 */
#define AM_FM175XX_TXCONTROL_INV_TX1RF_ON  0x40    /**< \brief 置1,若TX1驱动使能,则引脚TX2的输出信号取反 */
#define AM_FM175XX_TXCONTROL_INV_TX2RF_OFF 0x20    /**< \brief 置1,若TX2驱动关闭,则引脚TX2的输出信号取反 */
#define AM_FM175XX_TXCONTROL_INV_TX1RF_OFF 0x10    /**< \brief 置1,若TX1驱动关闭,则引脚TX2的输出信号取反 */
#define AM_FM175XX_TXCONTROL_TX2_CW        0x08    /**< \brief 置1,TX2将持续输出未调制的13.56MHz的能量载波;置0,则调制 */
#define AM_FM175XX_TXCONTROL_TX2RF_EN      0x02    /**< \brief 置1,TX2输出经由发送数据调制的13.56MHz能量载波 */
#define AM_FM175XX_TXCONTROL_TX1RF_EN      0x01    /**< \brief 置1,TX1输出经由发送数据调制的13.56MHz能量载波 */

/** @} */


/**
 * \name TxASK寄存器掩码,天线驱动设置 (0x15)
 * @{
 */

#define AM_FM175XX_TXASK_FORCE_100ASK      0x40    /**< \brief 置1,Force100ASK强制生成一个100%ASK调制 */

/** @} */


/**
 * \name TxSel寄存器掩码,天线驱动信号源选择 (0x16)
 * @{
 */
 
#define AM_FM175XX_TXSEL_DRIVER_SEL_MASK       0x30    /**< \brief 驱动Tx1和Tx2的输入掩码 */
#define AM_FM175XX_TXSEL_DRIVER_SEL_TRISTATE   0x00    /**< \brief 驱动Tx1和Tx2的输入选择三态 */
#define AM_FM175XX_TXSEL_DRIVER_SEL_IN         0x10    /**< \brief 驱动Tx1和Tx2的输入选择内部编码器的调制信号 */
#define AM_FM175XX_TXSEL_DRIVER_SEL_TIN        0x20    /**< \brief 驱动Tx1和Tx2的输入选择TIN的调制信号 */
#define AM_FM175XX_TXSEL_DRIVER_SEL_HIGH       0x30    /**< \brief 驱动Tx1和Tx2的输选择高电平 */
#define AM_FM175XX_TXSEL_TOUT_SEL_MASK         0x0f    /**< \brief 驱动Tx1和Tx2的输选择高电平 */

/**
 * \name TOUT引脚的输入选择（FM17510不支持）
 * @{
 */
#define AM_FM175XX_TXSEL_TOUT_SEL_TRISTATE     0x00    /**< \brief TOUT引脚的输入选择三态 */
#define AM_FM175XX_TXSEL_TOUT_SEL_LOW          0x01    /**< \brief TOUT引脚的输入选择低电平 */
#define AM_FM175XX_TXSEL_TOUT_SEL_HIGH         0x02    /**< \brief TOUT引脚的输入选择高电平 */
#define AM_FM175XX_TXSEL_TOUT_SEL_Testbus      0x03    /**< \brief TOUT引脚的输入选择TestBus */
#define AM_FM175XX_TXSEL_TOUT_SEL_in           0x04    /**< \brief TOUT引脚的输入选择内部编码器的调制信号 */
#define AM_FM175XX_TXSEL_TOUT_SEL_rcevs        0x05    /**< \brief TOUT引脚的输入选择待发送的串行数据流 */
#define AM_FM175XX_TXSEL_TOUT_SEL_sends        0x07    /**< \brief TOUT引脚的输入选择接收到的串行数据流(不要在M1模式下使用) */

/** @} */

/** @} */


/**
 * \name RxSel寄存器掩码,内部接收器设置 (0x17)
 * @{
 */
 
#define AM_FM175XX_RXSEL_UARTSEL_MASK          0xc0    /**< \brief 选择非接触UART的输入掩码 */
#define AM_FM175XX_RXSEL_UARTSEL_LOW           0x00    /**< \brief 非接触UART输入固定低电平 */
#define AM_FM175XX_RXSEL_UARTSEL_TIN           0x40    /**< \brief TIN的包络信号（FM17510不支持） */
#define AM_FM175XX_RXSEL_UARTSEL_IN            0x80    /**< \brief 来自内部模拟电路的调制信号 */
#define AM_FM175XX_RXSEL_UARTSEL_TIN_M         0xc0    /**< \brief 来自引脚TIN的调制信号（速度高于424kbit的时候有效）（FM17510不支持） */
#define AM_FM175XX_RXSEL_RXWAIT                0x3f    /**< \brief 数据完成发射后,接收器会在Rxwait定义的时钟延迟后被激活 */

/** @} */


/**
 * \name RxThreshold寄存器掩码,接收译码器阈值选择 (0x18)
 * @{
 */

/** \brief 译码器能接收的最小信号强度(低于这个水平不被处理) */
#define AM_FM175XX_RXTHRESHOLD_MINLEVEL   0xf0

/** \brief 输入到译码器的曼切斯特编码的弱半bit相对强半bit产生冲突位的最小信号强度 */
#define AM_FM175XX_RXTHRESHOLD_COLLLEVEL  0x07    

/** @} */


/**
 * \name Demod寄存器掩码,解调器设置 (0x19)
 * @{
 */
 
#define AM_FM175XX_DEMOD_ADDIQ_MASK        0xc0    /**< \brief 接收过程中I和Q通道的使用掩码 */
#define AM_FM175XX_DEMOD_ADDIQ_S           0x00    /**< \brief 选择较强通道 */
#define AM_FM175XX_DEMOD_ADDIQ_KEEP_S      0x40    /**< \brief 选择较强通道,并且在通讯过程中保持不变 */
#define AM_FM175XX_DEMOD_ADDIQ_IQ          0x80    /**< \brief 合并I和Q通道 */
#define AM_FM175XX_DEMOD_FIXIQ             0x20    /**< \brief 置1时ADDIQ为X0则接收固定I通道,为X1则接收固定Q通道  */
#define AM_FM175XX_DEMOD_TYPEB_EOF_MODE    0x10    /**< \brief 0:逻辑0+和1+无副载波调制 1:逻辑0+无副载波调制（FM17510不支持） */
#define AM_FM175XX_DEMOD_TAU_RCV           0x0c    /**< \brief 数据接收过程中改变内部时间常数(注:若设为00,PLL在数据接收中冻结) */
#define AM_FM175XX_DEMOD_TAU_SYNC          0x03    /**< \brief 在burst中改变内部PLL的时间常数 */

/** @} */


/**
 * \name Tx寄存器掩码,控制部分ISO/IEC 14443A发射参数 (0x1c)
 * @{
 */
 
#define AM_FM175XX_TX_WAIT         0x03    /**< \brief 该位定义了附加相应时间.缺省值为该位值加7bit */

/** @} */


/**
 * \name Rx寄存器掩码 (0x1d)
 * @{
 */
 
#define AM_FM175XX_RX_PARITY_DIS   0x10    /**< \brief 若该位置1,收发奇偶校验都关闭,校验位作为普通数据位处理 */

/** @} */


/**
 * \name TypeB寄存器掩码,ISO/IEC 14443B控制 (0x1e)（FM17510不支持）
 * @{
 */

#define AM_FM175XX_TYPEB_RXSOF_REQ         0x80    /**< \brief 该位置1,不以SOF起始的数据流会被忽略 */
#define AM_FM175XX_TYPEB_RXEOF_REQ         0x40    /**< \brief 该位置1,不以EOF结束的数据流会触发协议错误 */
#define AM_FM175XX_TYPEB_EOFSOF_WIDTH      0x10    /**< \brief 置1,SOF和EOF会有ISO/IEC 14443B中定义的最大长度;置0,会有最小长度 */
#define AM_FM175XX_TYPEB_NO_TXSOF          0x08    /**< \brief 置1,不产生SOF */
#define AM_FM175XX_TYPEB_NO_TXEOF          0x04    /**< \brief 置1,不产生EOF */
#define AM_FM175XX_TYPEB_TX_EGT            0x03    /**< \brief EGT长度,可以为0,1,2和3 */

/** @} */


/**
 * \name SerialSpeed寄存器掩码,选择串行UART接口的速度（FM17510不支持） (0x1f)
 * @{
 */

#define AM_FM175XX_SERIALSPEED_BR_T0       0xe0    /**< \brief BR_T0因子用于调节传输速度 */
#define AM_FM175XX_SERIALSPEED_BR_T1       0x0f    /**< \brief BR_T1因子用于调节传输速度 */

/** @} */


/**
 * \name RFCfg寄存器掩码,配置接收器增益 (0x26)
 * @{
 */
 
#define AM_FM175XX_RFCFG_RXGAIN_MASK       0x70    /**< \brief 接收器电压增益指数掩码 */
#define AM_FM175XX_RFCFG_RXGAIN_18DB       0x00    /**< \brief 接收器电压增益指数18dB */
#define AM_FM175XX_RFCFG_RXGAIN_23DB       0x10    /**< \brief 接收器电压增益指数23dB */
#define AM_FM175XX_RFCFG_RXGAIN_18DB_TOO   0x20    /**< \brief 接收器电压增益指数18dB */
#define AM_FM175XX_RFCFG_RXGAIN_23DB_TOO   0x30    /**< \brief 接收器电压增益指数23dB */
#define AM_FM175XX_RFCFG_RXGAIN_33DB       0x40    /**< \brief 接收器电压增益指数33dB */
#define AM_FM175XX_RFCFG_RXGAIN_38DB       0x50    /**< \brief 接收器电压增益指数38dB */
#define AM_FM175XX_RFCFG_RXGAIN_43DB       0x60    /**< \brief 接收器电压增益指数43dB */
#define AM_FM175XX_RFCFG_RXGAIN_48DB       0x70    /**< \brief 接收器电压增益指数48dB */

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
#define AM_FM175XX_CWGSN                   0xf0

/**
 * \brief 调制时段输出N驱动的电导,可控制调制深度:
 *        1.电导值位二进制加权
 *        2.soft Power-down模式下最高位强制置位 
 *        3.该寄存器的值只在TX1,TX2驱动开启时使用
 */
#define AM_FM175XX_MODGSN                  0x0f

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
#define AM_FM175XX_CWGSP                   0x3f

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
#define AM_FM175XX_MODGSP                  0x3f

/** @} */


/**
 * \name TMode寄存器,TPrescaler寄存器掩码,定义了定时器设置 (0x2A)
 * @{
 */
 
#define AM_FM175XX_T_AUTO                  0x80    /**< \brief 0定时器不受协议影响 1定时器自动计数 */
#define AM_FM175XX_T_GATE_MASK             0x60    /**< \brief 定时器门控掩码 */
#define AM_FM175XX_T_GATE_NONE             0x00    /**< \brief 定时器无门控 */
#define AM_FM175XX_T_GATE_TIN              0x20    /**< \brief 定时器由TIN做门控 */
#define AM_FM175XX_T_GATE_AUX1             0x40    /**< \brief 定时器由AUX1做门控 */
#define AM_FM175XX_T_AUTO_RESTART          0x10    /**< \brief 1定时器由重装载值自动重启倒计数,0计数器减到0时停止 */

/**
 * \brief TPrescaler的高四位
 *        定时器频率 f=13.56Mhz/(2*TPrescaler+1)
 *        TPrescaler=[TPrescaler_HI:TPrescaler_Lo]
 */
#define AM_FM175XX_T_PRESCALER_HI          0x0f    /**< \brief TPrescaler的高四位 */

/** @} */


/**
 * \name TestSel1寄存器掩码,测试信号配置 (0x31)
 * @{
 */
 
#define AM_FM175XX_TSTBUS_BITSEL           0x07    /**< \brief 从测试总线选择位传到TOUT */

/** @} */


/**
 * \name TestSel2寄存器掩码,测试信号配置和PRBS控制 (0x32)
 * @{
 */
 
#define AM_FM175XX_TSTBUS_FLIP             0x80    /**< \brief 置1 测试总线按D432651顺序映射到并行端口 */
#define AM_FM175XX_PRBS9                   0x40    /**< \brief 根据ITU-TO150开启并使能PRBS9序列 */
#define AM_FM175XX_PRBS15                  0x20    /**< \brief 根据ITU-TO150开启并使能PRBS15序列 */
#define AM_FM175XX_TSTBUS_SEL              0x1f    /**< \brief 选择测试总线 */

/** @} */


/**
 * \name TestPinEn寄存器掩码,D1~D6引脚输出驱动使能 （FM17510不支持）(0x33)
 * @{
 */
#define AM_FM175XX_RS232LINE_EN            0x80    /**< \brief 置0 串行UART的MX和DTRQ线无效 */

/**
 * \brief 使能D1~D6引脚输出驱动
 *        如bit1置1,使能D1引脚输出;bit5置1,使能D5引脚输出
 *        只在使能串行接口时有效.SPI=>D1~D4 UART且RS232LineEn置1时=>D1~D4
 */
#define AM_FM175XX_TESTPIN_EN              0x7e

/** @} */


/**
 * \name TestPinValue寄存器掩码,用作I/O时,定义了7位并行端口的值（FM17510不支持） (0x34)
 * @{
 */
 
#define AM_FM175XX_U_SEL_O                 0x80    /**< \brief 置1 通过该位控制7个并行端口的I/O功能 */

/**
 * \brief 用作I/O时,定义了7位并行端口的值.每个端口都必须由寄存器TestPinEn寄存器
 *        中的TestPinEn位来使能.
 *        注:若UselO置1,读寄存器表示了引脚D6~D1的实际状态
 *           若UselO置0,读寄存器得到的是TestPinValue寄存器的值
 */
#define AM_FM175XX_TEST_PIN_VALUE          0x7e

/** @} */


/**
 * \name TestCtrl寄存器掩码,测试控制 (0x36)
 * @{
 */
 
/** \brief 置1 接收链的内部信号处理被非线性的执行,可以增加106kbit通讯模式下的操作距离 */
#define AM_FM175XX_AMP_RCV                 0x40

/** @} */


/**
 * \name AnalogTest寄存器掩码,控制引脚AUX1和AUX2（FM17510不支持AUX2） (0x38)
 * @{
 */
 
#define AM_FM175XX_ANALOG_SEL_AUX1_MASK        0xf0    /**< \brief 控制AUX1掩码 */
#define AM_FM175XX_ANALOG_SEL_AUX1_TRISTATE    0x00    /**< \brief AUX1三态 */
#define AM_FM175XX_ANALOG_SEL_AUX1_DAC1        0x10    /**< \brief TestDAC1(AUX1)输出 */
#define AM_FM175XX_ANALOG_SEL_AUX1_CORR1       0x20    /**< \brief 测试信号Corr1 */
#define AM_FM175XX_ANALOG_SEL_AUX1_CORR2       0x30    /**< \brief 测试信号Corr2 */
#define AM_FM175XX_ANALOG_SEL_AUX1_MINLEVEL    0x40    /**< \brief 测试信号MINLEVEL */
#define AM_FM175XX_ANALOG_SEL_AUX1_ADC_I       0x50    /**< \brief 测试信号ADC通道I */
#define AM_FM175XX_ANALOG_SEL_AUX1_ADC_Q       0x60    /**< \brief 测试信号ADC通道Q */
#define AM_FM175XX_ANALOG_SEL_AUX1_ADC_IQ      0x70    /**< \brief 测试信号ADC通道IQ合并 */
#define AM_FM175XX_ANALOG_SEL_AUX1_HIGH        0xa0    /**< \brief HIGH */
#define AM_FM175XX_ANALOG_SEL_AUX1_LOW         0xb0    /**< \brief LOW */
#define AM_FM175XX_ANALOG_SEL_AUX1_T           0xc0    /**< \brief 发射有效状态 */
#define AM_FM175XX_ANALOG_SEL_AUX1_R           0xd0    /**< \brief 接收有效状态 */
#define AM_FM175XX_ANALOG_SEL_AUX1_SC          0xe0    /**< \brief 检测到副载波 */
#define AM_FM175XX_ANALOG_SEL_AUX1_TESTBUS_BIT 0xf0    /**< \brief TestSel1寄存器中TstBusBitSel位定义的 */

#define AM_FM175XX_ANALOG_SEL_AUX2_MASK        0x0f    /**< \brief 控制AUX2掩码 */
#define AM_FM175XX_ANALOG_SEL_AUX2_TRISTATE    0x00    /**< \brief AUX1三态 */
#define AM_FM175XX_ANALOG_SEL_AUX2_DAC1        0x01    /**< \brief TestDAC2(AUX2)输出 */
#define AM_FM175XX_ANALOG_SEL_AUX2_CORR1       0x02    /**< \brief 测试信号Corr1 */
#define AM_FM175XX_ANALOG_SEL_AUX2_CORR2       0x03    /**< \brief 测试信号Corr2 */
#define AM_FM175XX_ANALOG_SEL_AUX2_MINLEVEL    0x04    /**< \brief 测试信号MINLEVEL */
#define AM_FM175XX_ANALOG_SEL_AUX2_ADC_I       0x05    /**< \brief 测试信号ADC通道I */
#define AM_FM175XX_ANALOG_SEL_AUX2_ADC_Q       0x06    /**< \brief 测试信号ADC通道Q */
#define AM_FM175XX_ANALOG_SEL_AUX2_ADC_IQ      0x07    /**< \brief 测试信号ADC通道IQ合并 */
#define AM_FM175XX_ANALOG_SEL_AUX2_HIGH        0x0a    /**< \brief HIGH */
#define AM_FM175XX_ANALOG_SEL_AUX2_LOW         0x0b    /**< \brief LOW */
#define AM_FM175XX_ANALOG_SEL_AUX2_T           0x0c    /**< \brief 发射有效状态 */
#define AM_FM175XX_ANALOG_SEL_AUX2_R           0x0d    /**< \brief 接收有效状态 */
#define AM_FM175XX_ANALOG_SEL_AUX2_SC          0x0e    /**< \brief 检测到副载波 */
#define AM_FM175XX_ANALOG_SEL_AUX2_TESTBUS_BIT 0x0f    /**< \brief TestSel1寄存器中TstBusBitSel位定义的 */

/** @} */


/**
 * \name TestDAC1寄存器掩码,定义TestDAC1的测试值 (0x39)
 * @{
 */
 
/** \brief 定义TestDAC1的测试值.DAC1的输出可以通过设置寄存器AnalogTest中AUX1到0001来切换到AUX1 */
#define AM_FM175XX_TESTDAC1        0x3f

/** @} */


/**
 * \name TestDAC2寄存器掩码,定义TestDAC2的测试值 (0x3a)
 * @{
 */
 
/** \brief 定义TestDAC2的测试值.DAC2的输出可以通过设置寄存器AnalogTest中AUX2到0001来切换到AUX2 */
#define AM_FM175XX_TESTDAC2        0x3f

/** @} */


/**
 * \name TestADC寄存器掩码,显示了ADC I和Q通道的实际值 (0x3b)
 * @{
 */
 
#define AM_FM175XX_ADC_I           0xf0    /**< \brief 显示了ADC I通道的实际值 */
#define AM_FM175XX_ADC_Q           0x0f    /**< \brief 显示了ADC Q通道的实际值 */

/** @} */


/**
 * \name LpcdCtrl扩展寄存器掩码 (0x0f/0x01)
 * @{
 */
#define AM_
#define AM_FM175XX_LPCD_EN          0x01    /**< \brief 使能 LPCD模式  */
#define AM_FM175XX_LPCD_RSTN        0x02    /**< \brief LPCD复位   */
#define AM_FM175XX_LPCD_CALIBRA_EN  0x04    /**< \brief LPCD 计算模式使能   */
#define AM_FM175XX_LPCD_CMP_1       0x08    /**< \brief 比较次数 1次   */
#define AM_FM175XX_LPCD_CMP_3       0x13    /**< \brief 比较次数3次    */
#define AM_FM175XX_LPCD_IE          0x10    /**< \brief 使能LPCD中断  */
#define AM_FM175XX_CTRL_SET         0x20    /**< \brief LPCD 寄存器位控制设置   */
#define AM_FM175XX_CTRL_CLR         0x00    /**< \brief LPCD 寄存器位控制清除  */
/** @} */


/**
 * \name HpdCtrl扩展寄存器掩码 (0x0f/0x03)
 * @{
 */

#define AM_FM175XX_EXMODE          0xc0    /**< \brief 扩展寄存器模式字,读回始终0 */

/**
 * \brief 低功耗模式控制
 *        置0 当pin NPD=0且LPCDEn=0(缺省)时,芯片进入DPD模式
 *        置1 当pin NPD=0且LPCDEn=0(缺省)时,芯片进入HPD模式
 */
#define AM_FM175XX_HPDEN           0x20

/** @} */

/**
 * \name Lpcd Irq扩展寄存器掩码 (0x0f/0x12)
 * @{
 */
#define AM_FM175XX_LPCD_INT_IDLE          0x00  /**< \brief 无中断*/
#define AM_FM175XX_LPCD_CARD_IRQ          0x01  /**< \brief 卡进场中断*/
#define AM_FM175XX_LPCD_CALIB_IRQ         0x04  /**< \brief 调校完成中断 */
#define AM_FM175XX_LPCD_WUP_IRQ           0x10  /**< \brief 自动唤醒中断 */
/** @} */

/**
 * \name UseRet扩展寄存器掩码 (0x0f/0x1b)
 * @{
 */

/**
 * \brief 低功耗模式控制
 *        置0 不提供关键数据的保存功能
 *        置1 表示在HPD模式或LPCD模式下保存关键寄存器的设置,简化退出时的初始化工作
 */
#define AM_FM175XX_USERET          0x10

/** @} */

/**
 * \name LPCD MISC扩展寄存器掩码 (0x0f/0x1c)
 * @{
 */

#define AM_FM175XX_LCPD_CALIB_VMID_EN  0x01    /**< \brief LPCD 开启调教模式中的Vmind使能*/

/** @} */


/**
 * \name LVDctrl扩展寄存器掩码,低电压报警设置 (0x0f/0x1d)
 * @{
 */

#define AM_FM175XX_LVD_IRQ         0x10    /**< \brief 1表示发生低电压报警,写0清除此寄存器 */
#define AM_FM175XX_LVD_EFF         0x08    /**< \brief 发生低电压报警时1数字电路复位,0产生中断标志 */
#define AM_FM175XX_LVD_LE          0x04    /**< \brief 1低电压报警中断使能,缺省不使能 */
#define AM_FM175XX_LVD_CTRL_MASK   0x03    /**< \brief 低压报警控制 */
#define AM_FM175XX_LVD_CTRL_NONE   0x00    /**< \brief 不使能LVD功能 */
#define AM_FM175XX_LVD_CTRL_1_9V   0x01    /**< \brief 低于1.9V报警 */
#define AM_FM175XX_LVD_CTRL_2_1V   0x02    /**< \brief 低于2.1V报警(缺省) */
#define AM_FM175XX_LVD_CTRL_2_4V   0x03    /**< \brief 低于2.4V报警 */

/** @} */


/**
 * @} am_if_fm175xx_reg
 */

#ifdef __cplusplus
}
#endif
 
#endif /* __FM175XX_REG_H */

/* end of file */
