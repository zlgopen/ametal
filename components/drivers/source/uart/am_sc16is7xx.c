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
 * \brief SC16IS7XX 驱动
 * \details 本驱动在中断模式下使用如下状态机，如格式不正确，推荐使用 Notepad++ 查看
 *
 *                             ┌───────────────────────┐                 ┌─────────────────────┐
 *                             │         空闲          │  I2C 传输完成   │    使能发送中断     │
 * ┌──────────────────────────>├───────────────────────┤<────────────────├─────────────────────┤
 * │                           │I2C 传输失败 /         │  [启动中断发送] │entry /              │
 * │                           │将 RS485 方向设置为接收│────────────────>│启动 I2C 传输(写 IER)│
 * │                           └───────────────────────┘                 │RS485 方向设置为发送 │
 * │                                       │                             └─────────────────────┘
 * │                                  IRQ 为低电平
 * │                                       │
 * │                                       V
 * │    I2C 传输失败或          ┌─────────────────────┐
 * │    I2C 传输成功[没有中断源]│        读 IIR       │
 * │<───────────────────────────├─────────────────────┤<─────────────────────────────────────────┐
 * │                            │entry /              │                                          │
 * │                    ┌───────│启动 I2C 传输(读 IIR)│──────────────────────┐                   │
 * │                    │       └─────────────────────┘                      │                   │
 * │             I2C 传输成功                                 I2C 传输成功[发送 FIFO 空中断]     │
 * │             [接收超时或接收满中断]                                      │                   │
 * │                    │                                                    V                   │
 * │                    V                                        ┌───────────────────────┐       │
 * │            ┌───────────────────────┐   I2C 传输完成[FIFO 满]│       读 TXLVL        │       │
 * │I2C 传输完成│       读 RXLVL        │   ┌────────────────────├───────────────────────┤       │
 * │[FIFO 为 0] ├───────────────────────┤   │                    │entry /                │       │
 * │<───────────│entry /                │   │                    │启动 I2C 传输(读 TXLVL)│       │
 * │            │启动 I2C 传输(读 RXLVL)│   │                    └───────────────────────┘       │
 * │            └───────────────────────┘   │                         │            ^             │
 * │                  │            ^        │         I2C 传输成功[FIFO 未满] /    │             │
 * │    I2C 传输成功[FIFO 不为 0]  │        │         pfn_txchar_get               │             │
 * │                  │            │        │                         │  I2C 传输成功            │
 * │                  │     I2C 传输成功 /  │                         │  [有字符可取且 485 使能] │
 * │                  │     pfn_rxchar_put  │                         V            │             │
 * │                  V            │        │                     ┌─────────────────────┐        │
 * │             ┌─────────────────────┐    │                     │        写 THR       │        │
 * │I2C 传输失败 │       读 RHR        │    │                     ├─────────────────────┤────────┘
 * │<────────────├─────────────────────┤    │                     │entry 且有字符可取 / │ I2C 传输成功
 * │             │entry /              │    │                     │启动 I2C 传输(写 THR)│ [有字符可取且 485 失能]
 * │             │启动 I2C 传输(读 RHR)│    │                     └─────────────────────┘
 * │             └─────────────────────┘    │                                │
 * │                                        │                   I2C 传输失败或 I2C 传输成功或
 * │<───────────────────────────────────────┘                   无字符可取
 * │                                                                         │
 * │                                                                         V
 * │                                                              ┌─────────────────────┐
 * │                        I2C 传输失败或 I2C 传输成功[485 失能] │    失能发送中断     │
 * │<─────────────────────────────────────────────────────────────├─────────────────────┤
 * │                                                              │entry /              │
 * │                                                              │启动 I2C 传输(写 IER)│
 * │                                                              └─────────────────────┘
 * │                                                                         │     
 * │                                                               I2C 传输成功[485 使能]
 * │                                                                         │     
 * │                                     I2C 传输失败或                      V     
 * │                                     I2C 传输成功[发送完成] / ┌─────────────────────┐
 * │                                     RS485 方向设置为接收     │     等待发送完成    │
 * └──────────────────────────────────────────────────────────────├─────────────────────┤<─┐
 *                                                                │entry /              │  │
 *                                                                │启动 I2C 传输(读 LSR)│  │
 *                                                                └─────────────────────┘  │
 *                                                                           │             │
 *                                                               I2C 传输成功[发送未完成]  │
 *                                                                           │             │
 *                                                                           └─────────────┘
 *
 * \internal
 * \par modification history:
 * - 1.00 18-05-22  pea, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_sc16is7xx.h"
#include "am_int.h"
#include "am_uart.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include <string.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

#define  __ST_IDLE           0  /**< \brief 空闲状态 */
#define  __ST_TX_INT_EN      1  /**< \brief 使能发送中断 */
#define  __ST_IIR_RD         2  /**< \brief 读 IIR 寄存器 */
#define  __ST_RXLVL_RD       3  /**< \brief 读 RXLVL 寄存器 */
#define  __ST_RHR_RD         4  /**< \brief 读 RHR 寄存器 */
#define  __ST_TXLVL_RD       5  /**< \brief 读 TXLVL 寄存器 */
#define  __ST_THR_WE         6  /**< \brief 写 THR 寄存器 */
#define  __ST_TX_INT_DIS     7  /**< \brief 失能发送中断 */
#define  __ST_TX_DONE_WAIT   8  /**< \brief 等待发送完成 */

#define  __EVT_NONE          0  /**< \brief 无事件 */
#define  __EVT_IRQ_LOW       1  /**< \brief IRQ 为低电平 */
#define  __EVT_I2C_OK        2  /**< \brief I2C 传输成功 */
#define  __EVT_I2C_ERR       3  /**< \brief I2C 传输失败 */
#define  __EVT_CHAR_NONE     4  /**< \brief 无字符可取 */
#define  __EVT_CONTINUE      5  /**< \brief 继续操作事件 */

/** \brief 状态机内部状态切换 */
#define __NEXT_STATE(s, e)        \
    do {                          \
        p_dev->state[chan] = (s); \
        new_event = (e);          \
    } while(0)

/** \brief 寄存器地址 */
enum __SC16IS7XX_REG_ADDR {

    /** \brief 通用寄存器地址，只能在 LCR[7] 为 0 时访问 */
    SC16IS7XX_REG_RHR       = 0x00,    /**< \brief 接收 FIFO 寄存器 */
    SC16IS7XX_REG_THR       = 0x00,    /**< \brief 发送 FIFO 寄存器 */
    SC16IS7XX_REG_IER       = 0x01,    /**< \brief 中断使能寄存器 */
    SC16IS7XX_REG_FCR       = 0x02,    /**< \brief FIFO 控制寄存器 */
    SC16IS7XX_REG_IIR       = 0x02,    /**< \brief 中断标志寄存器 */
    SC16IS7XX_REG_LCR       = 0x03,    /**< \brief 线控制寄存器 */
    SC16IS7XX_REG_MCR       = 0x04,    /**< \brief 调制解调器控制寄存器 */
    SC16IS7XX_REG_LSR       = 0x05,    /**< \brief 线状态寄存器 */
    SC16IS7XX_REG_MSR       = 0x06,    /**< \brief 调制解调器状态寄存器 */
    SC16IS7XX_REG_SPR       = 0x07,    /**< \brief 用户数据寄存器 */
    SC16IS7XX_REG_TCR       = 0x06,    /**< \brief 传输控制寄存器 */
    SC16IS7XX_REG_TLR       = 0x07,    /**< \brief 触发水平寄存器 */
    SC16IS7XX_REG_TXLVL     = 0x08,    /**< \brief 发送 FIFO 水平寄存器 */
    SC16IS7XX_REG_RXLVL     = 0x09,    /**< \brief 接收 FIFO 水平寄存器 */
    SC16IS7XX_REG_IODIR     = 0x0A,    /**< \brief I/O 方向寄存器 */
    SC16IS7XX_REG_IOSTATE   = 0x0B,    /**< \brief I/O 状态寄存器 */
    SC16IS7XX_REG_IOINTENA  = 0x0C,    /**< \brief I/O 中断使能寄存器 */
    SC16IS7XX_REG_IOCTRL    = 0x0E,    /**< \brief I/O 控制寄存器 */
    SC16IS7XX_REG_EFCR      = 0x0F,    /**< \brief 扩展特性控制寄存器 */

    /** \brief 特殊寄存器地址，只能在 LCR[7] 为 1，且 LCR 不为 0xBF 时访问 */
    SC16IS7XX_REG_DLL       = 0x00,    /**< \brief 波特率分频器低字节寄存器 */
    SC16IS7XX_REG_DLH       = 0x01,    /**< \brief 波特率分频器高字节寄存器 */

    /** \brief 增强寄存器地址，只能在 LCR 为 0xBF 时访问 */
    SC16IS7XX_REG_EFR       = 0x02,    /**< \brief 增强特性寄存器 */
    SC16IS7XX_REG_XON1      = 0x04,    /**< \brief XON1 寄存器 */
    SC16IS7XX_REG_XON2      = 0x05,    /**< \brief XON2 寄存器 */
    SC16IS7XX_REG_XOFF1     = 0x06,    /**< \brief XOFF1 寄存器 */
    SC16IS7XX_REG_XOFF2     = 0x07,    /**< \brief XOFF2 寄存器 */
};

#define  SC16IS7XX_IER_RHR_EN_BIT    AM_BIT(0)  /**< \brief RHR 中断使能位 */
#define  SC16IS7XX_IER_THR_EN_BIT    AM_BIT(1)  /**< \brief THR 中断使能位 */
#define  SC16IS7XX_IER_RLS_EN_BIT    AM_BIT(2)  /**< \brief 接收线状态中断使能位 */
#define  SC16IS7XX_IER_MODEM_EN_BIT  AM_BIT(3)  /**< \brief 调制解调器状态中断使能位 */
#define  SC16IS7XX_IER_SLEEP_EN_BIT  AM_BIT(4)  /**< \brief 睡眠模式使能位 */
#define  SC16IS7XX_IER_XOFF_EN_BIT   AM_BIT(5)  /**< \brief XOFF 中断使能位 */
#define  SC16IS7XX_IER_RTS_EN_BIT    AM_BIT(6)  /**< \brief RTS 中断使能位 */
#define  SC16IS7XX_IER_CTS_EN_BIT    AM_BIT(7)  /**< \brief CTS 中断使能位 */

#define  SC16IS7XX_FCR_FIFO_EN_BIT   AM_BIT(0)     /**< \brief FIFO 使能位 */
#define  SC16IS7XX_FCR_RX_RESET_BIT  AM_BIT(1)     /**< \brief 接收 FIFO 复位位 */
#define  SC16IS7XX_FCR_TX_RESET_BIT  AM_BIT(2)     /**< \brief 发送 FIFO 复位位 */
#define  SC16IS7XX_FCR_TX_LEVEL_8    AM_SBF(0, 4)  /**< \brief 发送 FIFO 8 字节触发 */
#define  SC16IS7XX_FCR_TX_LEVEL_16   AM_SBF(1, 4)  /**< \brief 发送 FIFO 16 字节触发 */
#define  SC16IS7XX_FCR_TX_LEVEL_32   AM_SBF(2, 4)  /**< \brief 发送 FIFO 32 字节触发 */
#define  SC16IS7XX_FCR_TX_LEVEL_56   AM_SBF(3, 4)  /**< \brief 发送 FIFO 56 字节触发 */
#define  SC16IS7XX_FCR_RX_LEVEL_8    AM_SBF(0, 6)  /**< \brief 接收 FIFO 8 字节触发 */
#define  SC16IS7XX_FCR_RX_LEVEL_16   AM_SBF(1, 6)  /**< \brief 接收 FIFO 16 字节触发 */
#define  SC16IS7XX_FCR_RX_LEVEL_56   AM_SBF(2, 6)  /**< \brief 接收 FIFO 56 字节触发 */
#define  SC16IS7XX_FCR_RX_LEVEL_60   AM_SBF(3, 6)  /**< \brief 接收 FIFO 60 字节触发 */

#define  SC16IS7XX_IIR_SRC_NONE          AM_SBF(0x01, 0)  /**< \brief NONE */
#define  SC16IS7XX_IIR_SRC_RECV_ERR      AM_SBF(0x06, 0)  /**< \brief 接收状态错误 */
#define  SC16IS7XX_IIR_SRC_RECV_TIMEOUT  AM_SBF(0x0C, 0)  /**< \brief 接收超时 */
#define  SC16IS7XX_IIR_SRC_RHR           AM_SBF(0x04, 0)  /**< \brief 接收满中断 */
#define  SC16IS7XX_IIR_SRC_THR           AM_SBF(0x02, 0)  /**< \brief 发送空中断 */
#define  SC16IS7XX_IIR_SRC_MODEM         AM_SBF(0x00, 0)  /**< \brief 调制解调器中断 */
#define  SC16IS7XX_IIR_SRC_IO            AM_SBF(0x30, 0)  /**< \brief 引脚中断 */
#define  SC16IS7XX_IIR_SRC_XOFF          AM_SBF(0x10, 0)  /**< \brief XOFF 中断 */
#define  SC16IS7XX_IIR_SRC_CRTS          AM_SBF(0x20, 0)  /**< \brief CTS/RTS 无效中断 */
#define  SC16IS7XX_IIR_SRC_MASK          AM_SBF(0x3F, 0)  /**< \brief 中断源掩码 */
#define  SC16IS7XX_IIR_FIFO_EN_FLAG      AM_BIT(6)        /**< \brief FIFO 使能标志 */

#define  SC16IS7XX_LCR_DATA_5BIT         AM_SBF(0, 0)  /**< \brief 5 位数据位 */
#define  SC16IS7XX_LCR_DATA_6BIT         AM_SBF(1, 0)  /**< \brief 6 位数据位 */
#define  SC16IS7XX_LCR_DATA_7BIT         AM_SBF(2, 0)  /**< \brief 7 位数据位 */
#define  SC16IS7XX_LCR_DATA_8BIT         AM_SBF(3, 0)  /**< \brief 8 位数据位 */
#define  SC16IS7XX_LCR_STOP_1BIT         AM_SBF(0, 2)  /**< \brief 1 位停止位 */
#define  SC16IS7XX_LCR_STOP_1_5BIT       AM_SBF(1, 2)  /**< \brief 1.5 位停止位，数据位只能为 5 */
#define  SC16IS7XX_LCR_STOP_2BIT         AM_SBF(1, 2)  /**< \brief 2 位停止位 */
#define  SC16IS7XX_LCR_PARITY_NO         AM_SBF(0, 3)  /**< \brief 无奇偶校验位 */
#define  SC16IS7XX_LCR_PARITY_ODD        AM_SBF(1, 3)  /**< \brief 奇校验 */
#define  SC16IS7XX_LCR_PARITY_EVEN       AM_SBF(3, 3)  /**< \brief 偶校验 */
#define  SC16IS7XX_LCR_PARITY_FORCE_1    AM_SBF(5, 3)  /**< \brief 校验位保持为 1 */
#define  SC16IS7XX_LCR_PARITY_FORCE_0    AM_SBF(7, 3)  /**< \brief 校验位保持为 0 */
#define  SC16IS7XX_LCR_BREAK_BIT         AM_BIT(6)     /**< \brief 强制拉低 TX 产生 BREAK */
#define  SC16IS7XX_LCR_DIV_LATCH_EN_BIT  AM_BIT(7)     /**< \brief 分频器锁存器使能位 */
#define  SC16IS7XX_LCR_ACCESS_GENERAL    0x03          /**< \brief 写入此值之后操作通用寄存器 */
#define  SC16IS7XX_LCR_ACCESS_SPECIAL    0x80          /**< \brief 写入此值之后操作特殊寄存器 */
#define  SC16IS7XX_LCR_ACCESS_ENHANCE    0xBF          /**< \brief 写入此值之后操作增强寄存器 */

#define  SC16IS7XX_LSR_DATA_IN_FLAG     AM_BIT(0)  /**< \brief RX FIFO 中数据存在标志位 */
#define  SC16IS7XX_LSR_OVERRUN_FLAG     AM_BIT(1)  /**< \brief 数据溢出标志位 */
#define  SC16IS7XX_LSR_PARITY_FLAG      AM_BIT(2)  /**< \brief 奇偶校验错误标志位 */
#define  SC16IS7XX_LSR_FRAME_FLAG       AM_BIT(3)  /**< \brief 帧错误标志位 */
#define  SC16IS7XX_LSR_BREAK_FLAG       AM_BIT(4)  /**< \brief 接收到 BREAK 标志位 */
#define  SC16IS7XX_LSR_THR_EMPTY_FLAG   AM_BIT(5)  /**< \brief 发送 FIFO 空标志位 */
#define  SC16IS7XX_LSR_THSR_EMPTY_FLAG  AM_BIT(6)  /**< \brief 发送 FIFO 与移位寄存器空标志位 */
#define  SC16IS7XX_LSR_FIFO_ERR_FLAG    AM_BIT(7)  /**< \brief FIFO 错误标志位 */

#define  SC16IS7XX_TCR_HALT_LEVEL(num)    AM_SBF((num) & 0x0F, 0)  /**< \brief 恢复接收水平 */
#define  SC16IS7XX_TCR_RESUME_LEVEL(num)  AM_SBF((num) & 0x0F, 4)  /**< \brief 停止接收水平 */

#define  SC16IS7XX_TLR_TX_LEVEL(num)  AM_SBF((num) & 0x0F, 0)  /**< \brief 发送 FIFO 触发水平 */
#define  SC16IS7XX_TLR_RX_LEVEL(num)  AM_SBF((num) & 0x0F, 4)  /**< \brief 接收 FIFO 触发水平 */

#define  SC16IS7XX_IO_CTRL_RESET_BIT  AM_BIT(3)  /**< \brief 软件复位位 */

#define  SC16IS7XX_EFR_CTS_EN_BIT     AM_BIT(7)  /**< \brief CTS 流控使能位 */
#define  SC16IS7XX_EFR_RTS_EN_BIT     AM_BIT(6)  /**< \brief RTS 流控使能位 */
#define  SC16IS7XX_EFR_DETECT_EN_BIT  AM_BIT(5)  /**< \brief 指定字符检测使能位 */
#define  SC16IS7XX_EFR_ENHANCE_EN_BIT AM_BIT(4)  /**< \brief 增强特性使能位 */

/*******************************************************************************
  本地函数声明
*******************************************************************************/

/**
 * \brief SPI 传输状态机
 */
am_local am_err_t __sm_event (am_sc16is7xx_dev_t *p_dev,
                              uint8_t             chan,
                              uint32_t            event);

/**
 * \brief UART 控制函数
 */
am_local int __uart_ioctl (void *p_drv, int request, void *p_arg);

/**
 * \brief 启动 UART 发送函数
 */
am_local int __uart_tx_startup (void *p_drv);

/**
 * \brief 设置串口回调函数
 */
am_local int __uart_callback_set (void *p_drv,
                                  int   callback_type,
                                  void *pfn_callback,
                                  void *p_arg);

/**
 * \brief 从串口获取一个字符（查询模式）
 */
am_local int __uart_poll_getchar (void *p_drv, char *p_inchar);

/**
 * \brief 输出一个字符（查询模式）
 */
am_local int __uart_poll_putchar (void *p_drv, char outchar);

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief 标准层接口函数实现 */
am_local am_const struct am_uart_drv_funcs __g_uart_drv_funcs = {
    __uart_ioctl,        /* UART 控制函数 */
    __uart_tx_startup,   /* 启动 UART 发送函数 */
    __uart_callback_set, /* 设置串口回调函数 */
    __uart_poll_getchar, /* 从串口获取一个字符（查询模式） */
    __uart_poll_putchar, /* 输出一个字符（查询模式） */
};

/*******************************************************************************
  本地函数定义
*******************************************************************************/

/**
 * \brief I2C 消息处理完成回调函数
 */
am_local void __i2c_complete (void *p_arg)
{
    am_sc16is7xx_cbinfo_t *p_i2cinfo = (am_sc16is7xx_cbinfo_t *)p_arg;
    uint8_t               *p_chan    = (uint8_t *)p_i2cinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev     = (am_sc16is7xx_dev_t *)p_i2cinfo->p_arg[1];
    uint32_t               key;

    key = am_int_cpu_lock();
    p_dev->is_i2c_busy[*p_chan] = AM_FALSE;
    am_int_cpu_unlock(key);

    if (0 != p_dev->sm_cnt[*p_chan]) {
        AM_DBG_INFO("i2c %d sm_cnt: %d\r\n", *p_chan, p_dev->sm_cnt[*p_chan]);
    }

    /* 记录状态机重入次数 */
    p_dev->sm_cnt[*p_chan]++;

    /* 向状态机发送事件 */
    if (AM_OK == p_dev->i2c_msg[*p_chan].status) {
        __sm_event(p_dev, *p_chan, __EVT_I2C_OK);
    } else {
        __sm_event(p_dev, *p_chan, __EVT_I2C_ERR);
    }

    p_dev->sm_cnt[*p_chan]--;
}

/**
 * \brief ACK 引脚中断服务函数
 */
am_local void __irq_pint_isr (void *p_arg)
{
    am_sc16is7xx_dev_t *p_dev = (am_sc16is7xx_dev_t *)p_arg;
    uint32_t            i;

    am_gpio_trigger_off(p_dev->p_devinfo->irq_pin);

    for (i = 0; i < p_dev->p_devinfo->chan_num; i++) {

        /* 只有对应通道的中断使能之后才向状态机发送事件 */
        if (!p_dev->is_irq_en[i]) {
            continue;
        }

        if (0 != p_dev->sm_cnt[i]) {
            AM_DBG_INFO("isr %d sm_cnt: %d\r\n", i, p_dev->sm_cnt);
        }

        /* 记录状态机重入次数 */
        p_dev->sm_cnt[i]++;

        /* 向状态机发送事件 */
        __sm_event(p_dev, i, __EVT_IRQ_LOW);

        p_dev->sm_cnt[i]--;
    }

    am_gpio_trigger_on(p_dev->p_devinfo->irq_pin);
}

/**
 * \brief 使能 IRQ 中断
 */
am_local void __int_enable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    uint32_t key;

    key = am_int_cpu_lock();
    p_dev->is_irq_en[chan] = AM_TRUE;
    am_int_cpu_unlock(key);
    am_gpio_trigger_on(p_dev->p_devinfo->irq_pin);
}

/**
 * \brief 关闭 IRQ 中断
 */
am_local void __int_disable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    uint32_t i;
    uint32_t key;

    for (i = 0; i < p_dev->p_devinfo->chan_num; i++) {
        if (p_dev->is_irq_en[i]) {
            break;
        }
    }

    if (p_dev->p_devinfo->chan_num == i) {
        am_gpio_trigger_off(p_dev->p_devinfo->irq_pin);
    }

    key = am_int_cpu_lock();
    p_dev->is_irq_en[chan] = AM_FALSE;
    am_int_cpu_unlock(key);
}

/**
 * \brief 写寄存器
 */
am_local am_err_t __reg_write (am_sc16is7xx_dev_t *p_dev,
                               uint8_t             chan,
                               uint8_t             reg_addr,
                               uint8_t             data)
{
    am_err_t err;

    if ((NULL == p_dev) ||
        (p_dev->p_devinfo->chan_num <= chan) ||
        (0x0F < reg_addr)) {
        return -AM_EINVAL;
    }

    /* 设置地址字节 */
    p_dev->subaddr[chan] = 0;
    AM_BITS_SET(p_dev->subaddr[chan], 1, 2, chan);
    AM_BITS_SET(p_dev->subaddr[chan], 3, 4, reg_addr);

    err = am_i2c_write(&p_dev->i2c_dev[chan],
                        p_dev->subaddr[chan],
                       &data,
                        1);

    return err;
}

/**
 * \brief 读寄存器
 */
am_local am_err_t __reg_read (am_sc16is7xx_dev_t *p_dev,
                              uint8_t             chan,
                              uint8_t             reg_addr,
                              uint8_t            *p_data)
{
    am_err_t err;

    if ((NULL == p_dev) ||
        (p_dev->p_devinfo->chan_num <= chan) ||
        (0x0F < reg_addr) ||
        (NULL == p_data)) {
        return -AM_EINVAL;
    }

    /* 设置地址字节 */
    p_dev->subaddr[chan] = 0;
    AM_BITS_SET(p_dev->subaddr[chan], 1, 2, chan);
    AM_BITS_SET(p_dev->subaddr[chan], 3, 4, reg_addr);

    err = am_i2c_read(&p_dev->i2c_dev[chan],
                       p_dev->subaddr[chan],
                       p_data,
                       1);

    return err;
}

/**
 * \brief 启动 I2C 传输读写寄存器
 *
 * \param[in]     p_dev     SC16IS7XX 设备
 * \param[in]     chan      通道
 * \param[in]     is_read   是否是读寄存器
 * \param[in]     reg_addr  寄存器地址
 * \param[in,out] p_buf     指向缓冲区的指针
 * \param[in]     buf_lenth 需要读写的字节数
 *
 * \retval  AM_OK     I2C 传输启动成功，等待处理
 * \retval -AM_EINVAL 参数错误
 */
am_local am_err_t __i2c_start (am_sc16is7xx_dev_t *p_dev,
                               uint8_t             chan,
                               am_bool_t           is_read,
                               uint8_t             reg_addr,
                               uint8_t            *p_buf,
                               uint8_t             buf_lenth)
{
    uint32_t key;
    am_err_t err;

    if ((NULL == p_dev) ||
        (p_dev->p_devinfo->chan_num <= chan) ||
        (0x0F < reg_addr) ||
        (NULL == p_buf) ||
        (0 == buf_lenth)) {
        return -AM_EINVAL;
    }

    /* 判断 I2C 是否忙 */
    key = am_int_cpu_lock();
    if (p_dev->is_i2c_busy[chan]) {
        am_int_cpu_unlock(key);
        return -AM_EBUSY;
    }
    p_dev->is_i2c_busy[chan] = AM_TRUE;
    am_int_cpu_unlock(key);

    /* 设置地址字节 */
    p_dev->subaddr[chan] = 0;
    AM_BITS_SET(p_dev->subaddr[chan], 1, 2, chan);
    AM_BITS_SET(p_dev->subaddr[chan], 3, 4, reg_addr);

    am_i2c_mktrans(&p_dev->i2c_trans[chan][0],   /* 第 0 个传输发送子地址 */
                    p_dev->p_devinfo->i2c_addr,  /* 目标器件地址 */
                    AM_I2C_M_7BIT | AM_I2C_M_WR, /* 7 位器件地址，写操作 */
                   &p_dev->subaddr[chan],        /* 子地址 */
                    1);                          /* 子地址长度，1 字节 */

    if (is_read) {
        am_i2c_mktrans(&p_dev->i2c_trans[chan][1],   /* 第 1 个传输读取数据 */
                        p_dev->p_devinfo->i2c_addr,  /* 目标器件地址 */
                        AM_I2C_M_RD,                 /* 读操作 */
                        p_buf,                       /* 数据缓冲区 */
                        buf_lenth);                  /* 数据个数 */
    } else {
        am_i2c_mktrans(&p_dev->i2c_trans[chan][1],      /* 第 1 个传输发送数据 */
                        p_dev->p_devinfo->i2c_addr,     /* 目标器件地址 (将被忽略) */
                        AM_I2C_M_WR | AM_I2C_M_NOSTART, /* 写操作, 不发送起始信号及从机地址 */
                        p_buf,                          /* 数据缓冲区 */
                        buf_lenth);                     /* 数据个数 */
    }

    am_i2c_mkmsg(&p_dev->i2c_msg[chan],       /* I2C 消息 */
                 &p_dev->i2c_trans[chan][0],  /* I2C 传输 */
                  2,                          /* I2C 传输数量 */
                  __i2c_complete,             /* 消息处理完成回调函数 */
                 &p_dev->uartinfo[chan]);     /* 传递给回调函数的参数 */

    /* 启动 I2C 传输 */
    err = am_i2c_msg_start(p_dev->i2c_handle, &p_dev->i2c_msg[chan]);
    if (AM_OK != err) {
        key = am_int_cpu_lock();
        p_dev->is_i2c_busy[chan] = AM_FALSE;
        am_int_cpu_unlock(key);
    }

    return err;
}

/**
 * \brief SPI 传输状态机
 */
am_local am_err_t __sm_event (am_sc16is7xx_dev_t *p_dev,
                              uint8_t             chan,
                              uint32_t            event)
{
    uint32_t i;
    uint32_t new_event = __EVT_NONE;
    uint32_t key;
    am_err_t err;

    while (1) {

        /* 检查来自内部的新事件 */
        if (new_event != __EVT_NONE) {
            event = new_event;
            new_event = __EVT_NONE;
        }

        switch (p_dev->state[chan]) {

        /* 空闲状态 */
        case __ST_IDLE:

            /* 进入空闲状态时首先检查是否有启动中断发送的事件 */
            do {
                if ((p_dev->is_i2c_busy[chan]) ||
                    (!p_dev->int_en_flag[chan])) {
                    break;
                }

                key = am_int_cpu_lock();
                p_dev->int_en_flag[chan] = AM_FALSE;
                am_int_cpu_unlock(key);

                /* 如果发送中断已经使能，直接退出 */
                if (SC16IS7XX_IER_THR_EN_BIT & p_dev->ier_reg[chan]) {
                    break;
                }

                /* 使能发送中断 */
                p_dev->ier_reg[chan] |= SC16IS7XX_IER_THR_EN_BIT;

                __int_disable(p_dev, chan);

                /* 启动 I2C 传输写 IER 寄存器 */
                err = __i2c_start(p_dev,                 /* SC16IS7X */
                                  chan,                  /* 通道 */
                                  AM_FALSE,              /* 是否是读寄存器 */
                                  SC16IS7XX_REG_IER,     /* 寄存器地址 */
                                 &p_dev->ier_reg[chan],  /* 指向缓冲区的指针 */
                                  1);                    /* 需要读写的字节数 */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    __int_enable(p_dev, chan);
                } else {
                    __NEXT_STATE(__ST_TX_INT_EN, __EVT_NONE);

                    /* 将 RS485 设置为发送模式 */
                    if ((p_dev->is_rs485_en[chan]) &&
                        (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
                        p_dev->p_devinfo->pfn_rs485_dir(chan, AM_TRUE);
                    }
                }
                break;
            } while (AM_FALSE);

            /* 如果写了 IER 寄存器，直接返回 */
            if ((__ST_TX_INT_EN == p_dev->state[chan]) &&
                (__EVT_NONE == new_event)) {
                break;
            }

            /* 对 I2C 错误事件进行计数，超过阈值之后退出状态机 */
            if (__EVT_I2C_ERR == event) {

                /* 将 RS485 设置为接收模式 */
                if ((p_dev->is_rs485_en[chan]) &&
                    (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
                    p_dev->p_devinfo->pfn_rs485_dir(chan, AM_FALSE);
                }

                p_dev->i2c_err_cnt[chan]++;
                if (p_dev->i2c_err_cnt[chan] >= 5) {
                    p_dev->i2c_err_cnt[chan] = 0;
                    __NEXT_STATE(__ST_IDLE, __EVT_NONE);
                    __int_enable(p_dev, chan);
                    break;
                }
            } else {
                p_dev->i2c_err_cnt[chan] = 0;
            }

            if ((__EVT_CONTINUE == event) ||
                (__EVT_IRQ_LOW == event) ||
                (__EVT_I2C_ERR == event)) {

                /* 判断 IRQ 是否为低电平 */
                if (!am_gpio_get(p_dev->p_devinfo->irq_pin)) {

                    __int_disable(p_dev, chan);

                    /* 启动 I2C 传输读 IIR 寄存器 */
                    err = __i2c_start(p_dev,                /* SC16IS7X */
                                      chan,                 /* 通道 */
                                      AM_TRUE,              /* 是否是读寄存器 */
                                      SC16IS7XX_REG_IIR,    /* 寄存器地址 */
                                     &p_dev->iir_reg[chan], /* 指向缓冲区的指针 */
                                      1);                   /* 需要读写的字节数 */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_IIR_RD, __EVT_NONE);
                    }
                } else {
                    __int_enable(p_dev, chan);
                }
                break;
            }
            break;

        /* 使能发送中断 */
        case __ST_TX_INT_EN:

            /* I2C 传输失败 */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
            }

            /* I2C 传输成功 */
            if (__EVT_I2C_OK == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
            }

            __int_enable(p_dev, chan);

            break;

        /* 读 IIR 寄存器 */
        case __ST_IIR_RD:

            /* I2C 传输失败，直接回到空闲状态 */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C 传输成功，根据中断源选择下一个状态 */
            if (__EVT_I2C_OK == event) {

                /* 如果没有中断源退出状态机 */
                if (SC16IS7XX_IIR_SRC_NONE ==
                    (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK)) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                    __int_enable(p_dev, chan);
                    break;
                } else if ((SC16IS7XX_IIR_SRC_RHR ==
                            (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK)) ||
                           (SC16IS7XX_IIR_SRC_RECV_TIMEOUT ==
                            (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK))) {

                    /* 接收 FIFO 满或者接收超时 */

                    /* 启动 I2C 传输读 RXLVL 寄存器 */
                    err = __i2c_start(p_dev,                  /* SC16IS7X */
                                      chan,                   /* 通道 */
                                      AM_TRUE,                /* 是否是读寄存器 */
                                      SC16IS7XX_REG_RXLVL,    /* 寄存器地址 */
                                     &p_dev->rxlvl_reg[chan], /* 指向缓冲区的指针 */
                                      1);                     /* 需要读写的字节数 */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_RXLVL_RD, __EVT_NONE);
                    }
                    break;
                } else if (SC16IS7XX_IIR_SRC_THR ==
                           (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK)) {

                    /* 发送 FIFO 空 */

                    /* 启动 I2C 传输读 TXLVL 寄存器 */
                    err = __i2c_start(p_dev,                  /* SC16IS7X */
                                      chan,                   /* 通道 */
                                      AM_TRUE,                /* 是否是读寄存器 */
                                      SC16IS7XX_REG_TXLVL,    /* 寄存器地址 */
                                     &p_dev->txlvl_reg[chan], /* 指向缓冲区的指针 */
                                      1);                     /* 需要读写的字节数 */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_TXLVL_RD, __EVT_NONE);
                    }
                    break;
                } else {

                    /* 如果没有匹配的中断源，证明 I2C 读取错误 */
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                }
                break;
            }
            break;

        /* 读 RXLVL 寄存器 */
        case __ST_RXLVL_RD:

            /* I2C 传输失败 */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C 传输成功 */
            if (__EVT_I2C_OK == event) {

                /* 如果接收 FIFO 字节超过 FIFO 大小，证明 I2C 传输错误，返回空闲状态 */
                if ((0 == p_dev->rxlvl_reg[chan]) ||
                    (SC16IS7XX_FIFO_SIZE < p_dev->rxlvl_reg[chan])) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    break;
                }

                /* 启动 I2C 传输读 RHR 寄存器 */
                err = __i2c_start(p_dev,                   /* SC16IS7X */
                                  chan,                    /* 通道 */
                                  AM_TRUE,                 /* 是否是读寄存器 */
                                  SC16IS7XX_REG_RHR,       /* 寄存器地址 */
                                 &p_dev->rx_buf[chan][0],  /* 指向缓冲区的指针 */
                                  p_dev->rxlvl_reg[chan]); /* 需要读写的字节数 */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_RHR_RD, __EVT_NONE);
                }
                break;
            }
            break;

        /* 读 RHR 寄存器 */
        case __ST_RHR_RD:

            /* I2C 传输失败 */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C 传输成功 */
            if (__EVT_I2C_OK == event) {

                /* 先判断 I2C 是否所以传输都完成 */
                if (p_dev->i2c_msg[chan].trans_num != p_dev->i2c_msg[chan].done_num){
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                    break;
                }

                /* 将接收到的数据提交到 UART 子系统 */
                for (i = 0; i < p_dev->rxlvl_reg[chan]; i++) {
                    if (NULL == p_dev->pfn_rxchar_put[chan]) {
                        break;
                    }
                    p_dev->pfn_rxchar_put[chan](
                               p_dev->p_rxput_arg[chan],
                               p_dev->rx_buf[chan][i]);
                }

                /* 启动 I2C 传输读 RXLVL 寄存器 */
                err = __i2c_start(p_dev,                  /* SC16IS7X */
                                  chan,                   /* 通道 */
                                  AM_TRUE,                /* 是否是读寄存器 */
                                  SC16IS7XX_REG_RXLVL,    /* 寄存器地址 */
                                 &p_dev->rxlvl_reg[chan], /* 指向缓冲区的指针 */
                                  1);                     /* 需要读写的字节数 */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_RXLVL_RD, __EVT_NONE);
                }
                break;
            }
            break;

        /* 读 TXLVL 寄存器 */
        case __ST_TXLVL_RD:

            /* I2C 传输失败 */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C 传输成功 */
            if (__EVT_I2C_OK == event) {

                /* 如果发送 FIFO 满，返回空闲状态 */
                if (0 == p_dev->txlvl_reg[chan]) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                    break;
                }

                /* 如果发送 FIFO 空闲字节超过 FIFO 大小，证明 I2C 传输错误，返回空闲状态 */
                if (SC16IS7XX_FIFO_SIZE < p_dev->txlvl_reg[chan]) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    break;
                }

                /* 如果 RS485 未使能，一次最多写入半个 FIFO 的数据，避免写入时间过长 */
                if ((!p_dev->is_rs485_en[chan]) &&
                    (SC16IS7XX_FIFO_SIZE / 2 < p_dev->txlvl_reg[chan])) {
                    p_dev->txlvl_reg[chan] = SC16IS7XX_FIFO_SIZE / 2;
                }

                /* 从 UART 子系统获取需要发送的数据 */
                for (i = 0; i < p_dev->txlvl_reg[chan]; i++) {
                    err = p_dev->pfn_txchar_get[chan](
                                     p_dev->p_txget_arg[chan],
                            (char *)&p_dev->tx_buf[chan][i]);
                    if (err != AM_OK) {
                        break;
                    }
                }

                /* 判断是否获取到数据 */
                if (0 == i) {
                    __NEXT_STATE(__ST_THR_WE, __EVT_CHAR_NONE);
                    break;
                }

                /* 启动 I2C 传输写 THR 寄存器 */
                err = __i2c_start(p_dev,                  /* SC16IS7X */
                                  chan,                   /* 通道 */
                                  AM_FALSE,               /* 是否是读寄存器 */
                                  SC16IS7XX_REG_THR,      /* 寄存器地址 */
                                 &p_dev->tx_buf[chan][0], /* 指向缓冲区的指针 */
                                  i);                     /* 需要读写的字节数 */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CHAR_NONE);
                } else {

                    /* 判断从 UART 子系统获取数据是否成功 */
                    if (i == p_dev->txlvl_reg[chan]) {
                        __NEXT_STATE(__ST_THR_WE, __EVT_NONE);
                    } else {
                        __NEXT_STATE(__ST_THR_WE, __EVT_NONE);
                        key = am_int_cpu_lock();
                        p_dev->char_none_flag[chan] = AM_TRUE;
                        am_int_cpu_unlock(key);
                    }
                }
                break;
            }
            break;

        /* 写 THR 寄存器 */
        case __ST_THR_WE:

            /* 无字符可取或 I2C 传输失败 */
            if ((p_dev->char_none_flag[chan]) ||
                (__EVT_CHAR_NONE == event) ||
                (__EVT_I2C_ERR == event)) {

                key = am_int_cpu_lock();
                p_dev->char_none_flag[chan] = AM_FALSE;
                am_int_cpu_unlock(key);

                /* 失能发送中断 */
                p_dev->ier_reg[chan] &= ~SC16IS7XX_IER_THR_EN_BIT;

                /* 启动 I2C 传输写 IER 寄存器 */
                err = __i2c_start(p_dev,                /* SC16IS7X */
                                  chan,                 /* 通道 */
                                  AM_FALSE,             /* 是否是读寄存器 */
                                  SC16IS7XX_REG_IER,    /* 寄存器地址 */
                                 &p_dev->ier_reg[chan], /* 指向缓冲区的指针 */
                                  1);                   /* 需要读写的字节数 */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_TX_INT_DIS, __EVT_NONE);
                }
                break;
            }

            /* I2C 传输成功 */
            if (__EVT_I2C_OK == event) {

                /*
                 * 如果使能了 RS485，发送数据，直到完成，因为半双工 RS485 发送
                 * 数据时不可能接收数据
                 */
                if ((p_dev->is_rs485_en[chan]) &&
                    (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {

                    /* 启动 I2C 传输读 TXLVL 寄存器 */
                    err = __i2c_start(p_dev,                  /* SC16IS7X */
                                      chan,                   /* 通道 */
                                      AM_TRUE,                /* 是否是读寄存器 */
                                      SC16IS7XX_REG_TXLVL,    /* 寄存器地址 */
                                     &p_dev->txlvl_reg[chan], /* 指向缓冲区的指针 */
                                      1);                     /* 需要读写的字节数 */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);

                        /* 将 RS485 设置为接收模式 */
                        if ((p_dev->is_rs485_en[chan]) &&
                            (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
                            p_dev->p_devinfo->pfn_rs485_dir(chan, AM_FALSE);
                        }
                    } else {
                        __NEXT_STATE(__ST_TXLVL_RD, __EVT_NONE);
                    }
                } else { /* 在每次发送之后读取 IIR 寄存器，查看是否接收到数据，避免丢失 */

                    /* 启动 I2C 传输读 IIR 寄存器 */
                    err = __i2c_start(p_dev,                /* SC16IS7X */
                                      chan,                 /* 通道 */
                                      AM_TRUE,              /* 是否是读寄存器 */
                                      SC16IS7XX_REG_IIR,    /* 寄存器地址 */
                                     &p_dev->iir_reg[chan], /* 指向缓冲区的指针 */
                                      1);                   /* 需要读写的字节数 */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_IIR_RD, __EVT_NONE);
                    }
                }
                break;
            }
            break;

        /* 失能发送中断 */
        case __ST_TX_INT_DIS:

            /* I2C 传输失败 */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
            }

            /* RS485 未使能 */
            if (!p_dev->is_rs485_en[chan]) {
                __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                break;
            }

            /* 无字符可取或 I2C 传输成功 */
            if ((__EVT_CHAR_NONE == event) ||
                (__EVT_I2C_OK == event)) {

                /* 启动 I2C 传输读 LSR 寄存器 */
                err = __i2c_start(p_dev,                /* SC16IS7X */
                                  chan,                 /* 通道 */
                                  AM_TRUE,              /* 是否是读寄存器 */
                                  SC16IS7XX_REG_LSR,    /* 寄存器地址 */
                                 &p_dev->lsr_reg[chan], /* 指向缓冲区的指针 */
                                  1);                   /* 需要读写的字节数 */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_TX_DONE_WAIT, __EVT_NONE);
                }
            }
            break;

        /* 等待发送完成 */
        case __ST_TX_DONE_WAIT:

            /* I2C 传输失败 */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C 传输成功 */
            if (__EVT_I2C_OK == event) {

                /* 发送完成 */
                if (SC16IS7XX_LSR_THSR_EMPTY_FLAG & p_dev->lsr_reg[chan]) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);

                    /* 将 RS485 设置为接收模式 */
                    if ((p_dev->is_rs485_en[chan]) &&
                        (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
                        p_dev->p_devinfo->pfn_rs485_dir(chan, AM_FALSE);
                    }
                    break;
                } else {

                    /* 启动 I2C 传输读 LSR 寄存器 */
                    err = __i2c_start(p_dev,                /* SC16IS7X */
                                      chan,                 /* 通道 */
                                      AM_TRUE,              /* 是否是读寄存器 */
                                      SC16IS7XX_REG_LSR,    /* 寄存器地址 */
                                     &p_dev->lsr_reg[chan], /* 指向缓冲区的指针 */
                                      1);                   /* 需要读写的字节数 */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_TX_DONE_WAIT, __EVT_NONE);
                    }
                    break;
                }
            }
            break;

        default:
            break;
        }

        /* 没有来自内部的事件，跳出 */
        if (new_event == __EVT_NONE) {
            break;
        }
    }

    return AM_OK;
}

/**
 * \brief 通用寄存器操作使能
 */
am_local am_err_t __general_access_enable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    return __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, p_dev->lcr_reg[chan]);
}

/**
 * \brief 特殊寄存器操作使能
 */
am_local am_err_t __special_access_enable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    return __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, SC16IS7XX_LCR_ACCESS_SPECIAL);
}

/**
 * \brief 增强寄存器操作使能
 */
am_local am_err_t __enhance_access_enable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    return __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, SC16IS7XX_LCR_ACCESS_ENHANCE);
}

/**
 * \brief 配置串口波特率
 */
am_local am_err_t __serial_rate_set (am_sc16is7xx_dev_t *p_dev,
                                     uint8_t             chan,
                                     uint32_t            baudrate)
{
    am_sc16is7xx_devinfo_t *p_devinfo = (am_sc16is7xx_devinfo_t *)p_dev->p_devinfo;
    am_err_t                err;
    uint32_t                divisor;

    /* 判断波特率是否支持 */
    if (((p_devinfo->clk_freq / 16) < baudrate) ||
        ((p_devinfo->clk_freq / 16 / ((2 << 16) - 1)) > baudrate)) {
        return -AM_ENOTSUP;
    }

    /* 特殊寄存器操作使能 */
    err = __special_access_enable(p_dev, chan);
    if (AM_OK != err) {
        goto exit;
    }

    divisor = p_devinfo->clk_freq / 16 / baudrate;
    err = __reg_write(p_dev, chan, SC16IS7XX_REG_DLL, AM_BITS_GET(divisor, 0, 8));
    if (AM_OK != err) {
        goto exit;
    }
    err = __reg_write(p_dev, chan, SC16IS7XX_REG_DLH, AM_BITS_GET(divisor, 8, 8));
    if (AM_OK != err) {
        goto exit;
    }

exit:

    /* 通用寄存器操作使能 */
    err = __general_access_enable(p_dev, chan);

    return err;
}

/**
 * \brief 配置串口选项
 */
am_local am_err_t  __serial_opts_set (am_sc16is7xx_dev_t *p_dev,
                                      uint8_t             chan,
                                      uint32_t            opts)
{
    am_err_t err;
    uint8_t  reg = 0;

    /* 配置数据长度 */
    switch (opts & AM_UART_CSIZE) {

    case AM_UART_CS5:
        reg |= SC16IS7XX_LCR_DATA_5BIT;
        break;

    case AM_UART_CS6:
        reg |= SC16IS7XX_LCR_DATA_6BIT;
        break;

    case AM_UART_CS7:
        reg |= SC16IS7XX_LCR_DATA_7BIT;
        break;

    case AM_UART_CS8:
        reg |= SC16IS7XX_LCR_DATA_8BIT;
        break;

    default:
        reg |= SC16IS7XX_LCR_DATA_8BIT;
        break;
    }

    /* 配置奇偶校验位 */
    if (opts & AM_UART_PARENB) {
        if (opts & AM_UART_PARODD) {
            reg |= SC16IS7XX_LCR_PARITY_ODD;
        } else {
            reg |= SC16IS7XX_LCR_PARITY_EVEN;
        }
    } else {
        reg |= SC16IS7XX_LCR_PARITY_NO;
    }

    /* 配置停止位 */
    if (opts & AM_UART_STOPB) {
        reg |= SC16IS7XX_LCR_STOP_2BIT;
    } else {
        reg |= SC16IS7XX_LCR_STOP_1BIT;
    }

    /* 将配置写入 LCR 寄存器 */
    err = __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, reg);
    if (AM_OK == err) {
        p_dev->lcr_reg[chan] = reg;
        p_dev->serial_opts[chan] = opts;
    }

    return AM_OK;
}

/**
 * \brief 配置串口模式
 */
am_local am_err_t __serial_mode_set (am_sc16is7xx_dev_t *p_dev,
                                     uint8_t             chan,
                                     uint32_t            mode)
{
    uint8_t  reg = 0;
    am_err_t err = AM_OK;

    /* 判断是否支持相关模式 */
    if ((mode == AM_UART_MODE_POLL) &&
        (!(p_dev->serial_mode_avail | AM_UART_MODE_POLL))) {
        return -AM_ENOTSUP;
    }
    if ((mode == AM_UART_MODE_INT) &&
        (!(p_dev->serial_mode_avail | AM_UART_MODE_INT))) {
        return -AM_ENOTSUP;
    }

    if (AM_UART_MODE_POLL == mode) {

        __int_disable(p_dev, chan);

        /* 轮询模式直接关闭所有中断 */
        reg = 0;
        err = __reg_write(p_dev, chan, SC16IS7XX_REG_IER, reg);
    } else if (AM_UART_MODE_INT == mode) {

        __int_enable(p_dev, chan);

        /* 中断模式下使能接收中断 */
        reg = SC16IS7XX_IER_RHR_EN_BIT;
        err = __reg_write(p_dev, chan, SC16IS7XX_REG_IER, reg);
    } else {
        err = -AM_ENOTSUP;
    }

    if (AM_OK == err) {
        p_dev->ier_reg[chan] = reg;
        p_dev->serial_mode[chan] = mode;
    }

    return err;
}
/******************************************************************************/

/**
 * \brief UART 控制函数
 */
am_local int __uart_ioctl (void *p_drv, int request, void *p_arg)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];
    am_err_t               err        = AM_OK;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:
        err = __serial_rate_set(p_dev, *p_chan, (uint32_t)p_arg);
        break;

    /* 波特率获取 */
    case AM_UART_BAUD_GET:
        *(int *)p_arg = p_dev->serial_rate[*p_chan];
        break;

    /* 串口选项设置 */
    case AM_UART_OPTS_SET:
        err = __serial_opts_set(p_dev, *p_chan, (uint32_t)p_arg);
        break;

    /* 串口选项获取 */
    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_dev->serial_opts[*p_chan];
        break;

    /* 模式设置 */
    case AM_UART_MODE_SET:
        err = __serial_mode_set(p_dev, *p_chan, (uint32_t)p_arg);
        break;

    /* 模式获取 */
    case AM_UART_MODE_GET:
        *(int *)p_arg = p_dev->serial_mode[*p_chan];
        break;

    /* 获取串口可设置的模式 */
    case AM_UART_AVAIL_MODES_GET:
        *(int *)p_arg = p_dev->serial_mode_avail;
        break;

    /* 设置 RS485 模式 */
    case AM_UART_RS485_SET:
        p_dev->is_rs485_en[*p_chan] = (am_bool_t)(int)p_arg;
        break;

    /* 获取当前的 RS485 模式状态 */
    case AM_UART_RS485_GET:
        *(int *)p_arg = p_dev->is_rs485_en[*p_chan];
        break;

    default:
        err = -AM_ENOTSUP;
        break;
    }

    return err;
}

/**
 * \brief 启动 UART 发送函数
 */
am_local int __uart_tx_startup (void *p_drv)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];
    uint32_t               key;

    /* 判断当前是否是中断模式 */
    if (AM_UART_MODE_INT != p_dev->serial_mode[*p_chan]) {
        return -AM_EPERM;
    }

    /* 判断回调函数是否注册 */
    if (NULL == p_dev->pfn_txchar_get[*p_chan]) {
        return -AM_EPERM;
    }

    /* 判断发送中断是否已经使能 */
    if (p_dev->ier_reg[*p_chan] & SC16IS7XX_IER_THR_EN_BIT) {
        return AM_OK;
    }

    key = am_int_cpu_lock();
    p_dev->int_en_flag[*p_chan] = AM_TRUE;
    am_int_cpu_unlock(key);

    /* 如果状态机已经在运行，直接返回 */
    if ((0 != p_dev->sm_cnt[*p_chan]) || (p_dev->is_i2c_busy[*p_chan])) {
        return AM_OK;
    }

    /* 记录状态机重入次数 */
    p_dev->sm_cnt[*p_chan]++;

    /* 向状态机发送事件 */
    __sm_event(p_dev, *p_chan, __EVT_NONE);

    p_dev->sm_cnt[*p_chan]--;

    return AM_OK;
}

/**
 * \brief 设置串口回调函数
 */
am_local int __uart_callback_set (void *p_drv,
                                  int   callback_type,
                                  void *pfn_callback,
                                  void *p_arg)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];

    switch (callback_type) {

    /* 获取一个发送字符 */
    case AM_UART_CALLBACK_TXCHAR_GET:
        p_dev->pfn_txchar_get[*p_chan] = (am_uart_txchar_get_t)pfn_callback;
        p_dev->p_txget_arg[*p_chan]    = p_arg;
        break;

    /* 提交一个接收到的字符 */
    case AM_UART_CALLBACK_RXCHAR_PUT:
        p_dev->pfn_rxchar_put[*p_chan] = (am_uart_rxchar_put_t)pfn_callback;
        p_dev->p_rxput_arg[*p_chan]    = p_arg;
        break;

    /* 错误回调函数 */
    case AM_UART_CALLBACK_ERROR:
        p_dev->pfn_err[*p_chan]   = (am_uart_err_t)pfn_callback;
        p_dev->p_err_arg[*p_chan] = p_arg;
        break;

    default:
        return -AM_ENOTSUP;
    }

    return AM_OK;
}

/**
 * \brief 从串口获取一个字符（查询模式）
 */
am_local int __uart_poll_getchar (void *p_drv, char *p_inchar)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];
    am_err_t               err;
    uint8_t                reg;

    if (NULL == p_inchar) {
        return -AM_EINVAL;
    }

    /* 判断当前是否是轮询模式 */
    if (AM_UART_MODE_POLL != p_dev->serial_mode[*p_chan]) {
        return -AM_EPERM;
    }

    /* 读取线状态寄存器 */
    err = __reg_read(p_dev, *p_chan, SC16IS7XX_REG_LSR, &reg);
    if (AM_OK != err) {
        return err;
    }

    /* 判断接收缓冲区是否有数据 */
    if (!(SC16IS7XX_LSR_DATA_IN_FLAG & reg)) {
        return -AM_EAGAIN;
    }

    /* 判断是否有错误 */
    if (SC16IS7XX_LSR_FIFO_ERR_FLAG & reg) {
        __reg_read(p_dev, *p_chan, SC16IS7XX_REG_RHR, &reg);
        return -AM_EIO;
    }

    err = __reg_read(p_dev, *p_chan, SC16IS7XX_REG_RHR, &reg);
    if (AM_OK == err) {
        *p_inchar = reg;
    }

    return err;
}

/**
 * \brief 输出一个字符（查询模式）
 */
am_local int __uart_poll_putchar (void *p_drv, char outchar)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];
    am_err_t               err;
    uint8_t                reg;

    /* 判断当前是否是轮询模式 */
    if (AM_UART_MODE_POLL != p_dev->serial_mode[*p_chan]) {
        return -AM_EPERM;
    }

    /* 读取发送 FIFO 水平寄存器 */
    err = __reg_read(p_dev, *p_chan, SC16IS7XX_REG_TXLVL, &reg);
    if (AM_OK != err) {
        return err;
    }

    /* 判断发送缓冲区是否为空 */
    if (0 == reg) {
        return -AM_EAGAIN;
    }

    /* 将 RS485 设置为发送模式 */
    if ((p_dev->is_rs485_en[*p_chan]) &&
        (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
        p_dev->p_devinfo->pfn_rs485_dir(*p_chan, AM_TRUE);
    }

    err = __reg_write(p_dev, *p_chan, SC16IS7XX_REG_THR, outchar);
    if (AM_OK != err) {
        if ((p_dev->is_rs485_en[*p_chan]) &&
            (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
            p_dev->p_devinfo->pfn_rs485_dir(*p_chan, AM_FALSE);
        }
        return err;
    }

    /* 等待发送完成，并将 RS485 设置为接收模式 */
    if ((p_dev->is_rs485_en[*p_chan]) &&
        (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
        do {
            err = __reg_read(p_dev, *p_chan, SC16IS7XX_REG_LSR, &reg);
            if (AM_OK != err) {
                p_dev->p_devinfo->pfn_rs485_dir(*p_chan, AM_FALSE);
                return err;
            }
        } while (!(SC16IS7XX_LSR_THSR_EMPTY_FLAG & reg));

        p_dev->p_devinfo->pfn_rs485_dir(*p_chan, AM_FALSE);
    }

    return AM_OK;
}

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief SC16IS7XX 初始化
 */
am_sc16is7xx_handle_t am_sc16is7xx_init (am_sc16is7xx_dev_t           *p_dev,
                                         const am_sc16is7xx_devinfo_t *p_devinfo,
                                         am_i2c_handle_t               i2c_handle)
{
    uint32_t         i;
    am_err_t         err;
    am_local uint8_t chan_list[SC16IS7XX_CHAN_MAX];

    if ((NULL == p_dev) ||
        (NULL == p_devinfo) ||
        (NULL == i2c_handle) ||
        (SC16IS7XX_CHAN_MAX < p_devinfo->chan_num)) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->i2c_handle = i2c_handle;
    p_dev->p_devinfo  = (am_sc16is7xx_devinfo_t *)p_devinfo;

    for (i = 0; i < p_dev->p_devinfo->chan_num; i++) {
        chan_list[i] = i;

        p_dev->uart_serv[i].p_funcs = (struct am_uart_drv_funcs *)&__g_uart_drv_funcs;
        p_dev->uart_serv[i].p_drv   = &p_dev->uartinfo[i];
        p_dev->pfn_txchar_get[i]    = NULL;
        p_dev->pfn_rxchar_put[i]    = NULL;
        p_dev->pfn_err[i]           = NULL;

        p_dev->serial_rate[i]       = 9600;
        p_dev->serial_opts[i]       = AM_UART_CS8;
        p_dev->serial_mode[i]       = AM_UART_MODE_POLL;

        p_dev->is_rs485_en[i]       = AM_FALSE;

        p_dev->iir_reg[i]           = 0;
        p_dev->ier_reg[i]           = 0;
        p_dev->lcr_reg[i]           = 0;
        p_dev->rxlvl_reg[i]         = 0;
        p_dev->txlvl_reg[i]         = 0;

        p_dev->state[i]             = __ST_IDLE;
        p_dev->int_en_flag[i]       = AM_FALSE;
        p_dev->char_none_flag[i]    = AM_FALSE;

        p_dev->sm_cnt[i]            = 0;
        p_dev->i2c_err_cnt[i]       = 0;
        p_dev->is_i2c_busy[i]       = AM_FALSE;
        p_dev->is_irq_en[i]         = AM_FALSE;

        p_dev->uartinfo[i].p_arg[0] = &chan_list[i];
        p_dev->uartinfo[i].p_arg[1] = p_dev;

        p_dev->i2cinfo[i].p_arg[0] = &chan_list[i];
        p_dev->i2cinfo[i].p_arg[1] = p_dev;

        /* 构造 I2C 设备 */
        am_i2c_mkdev(&p_dev->i2c_dev[i],
                      i2c_handle,
                      p_devinfo->i2c_addr,
                      AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    }

    /* 初始化 RST 引脚 */
    if (-1 != p_devinfo->rst_pin) {
        am_gpio_pin_cfg(p_devinfo->rst_pin, AM_GPIO_OUTPUT_INIT_HIGH);
        am_gpio_set(p_devinfo->rst_pin, AM_GPIO_LEVEL_LOW);
        am_mdelay(1);
        am_gpio_set(p_devinfo->rst_pin, AM_GPIO_LEVEL_HIGH);
        am_mdelay(1);
    } else {
        am_mdelay(1);

        /* 如果没有配置 RST 引脚，则使用软件复位 */
        __reg_write(p_dev, 0, SC16IS7XX_REG_LCR, SC16IS7XX_LCR_ACCESS_GENERAL);
        __reg_write(p_dev, 0, SC16IS7XX_REG_IOCTRL, SC16IS7XX_IO_CTRL_RESET_BIT);

        am_mdelay(1);
    }

    /* 初始化 IRQ 引脚 */
    if (-1 != p_devinfo->irq_pin) {
        p_dev->serial_mode_avail = AM_UART_MODE_INT | AM_UART_MODE_POLL;

        err = am_gpio_pin_cfg(p_devinfo->irq_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        err |= am_gpio_trigger_connect(p_devinfo->irq_pin,
                                       __irq_pint_isr,
                                       (void *)p_dev);
        err |= am_gpio_trigger_cfg(p_devinfo->irq_pin, AM_GPIO_TRIGGER_FALL);
        err |= am_gpio_trigger_off(p_dev->p_devinfo->irq_pin);
        if (AM_OK != err) {
            AM_DBG_INFO("gpio trigger init failed %d!\r\n", err);
            return NULL;
        }
    } else {
        p_dev->serial_mode_avail = AM_UART_MODE_POLL;
    }

    for (i = 0; i < p_dev->p_devinfo->chan_num; i++) {

        /* 使能增强特性 */
        err = __enhance_access_enable(p_dev, i);
        err |= __reg_write(p_dev, i, SC16IS7XX_REG_EFR, SC16IS7XX_EFR_ENHANCE_EN_BIT);
        err |= __general_access_enable(p_dev, i);
        if (AM_OK != err) {
            AM_DBG_INFO("enhance enable failed %d!\r\n", err);
            return NULL;
        }

        err |= __reg_write(p_dev, i, SC16IS7XX_REG_FCR, SC16IS7XX_FCR_FIFO_EN_BIT);
        if (AM_OK != err) {
            AM_DBG_INFO("fifo enable failed %d!\r\n", err);
            return NULL;
        }

        /* 配置数据长度 */
        if (5 == p_devinfo->serial_data[i]) {
            p_dev->serial_opts[i] = AM_UART_CS5;
        } else if (6 == p_devinfo->serial_data[i]) {
            p_dev->serial_opts[i] = AM_UART_CS6;
        } else if (7 == p_devinfo->serial_data[i]) {
            p_dev->serial_opts[i] = AM_UART_CS7;
        } else {
            p_dev->serial_opts[i] = AM_UART_CS8;
        }

        /* 配置奇偶校验位 */
        if (1 == p_devinfo->serial_parity[i]) {
            p_dev->serial_opts[i] |= AM_UART_PARENB | AM_UART_PARODD;
        } else if (2 == p_devinfo->serial_parity[i]) {
            p_dev->serial_opts[i] |= AM_UART_PARENB;
        }

        /* 配置停止位 */
        if (2 == p_devinfo->serial_stop[i]) {
            p_dev->serial_opts[i] |= AM_UART_STOPB;
        }

        /* 配置串口选项 */
        err = __serial_opts_set(p_dev, i, p_dev->serial_opts[i]);
        if (AM_OK != err) {
            AM_DBG_INFO("serial opts set failed %d!\r\n", err);
            return NULL;
        }

        /* 配置波特率 */
        err = __serial_rate_set(p_dev, i, p_devinfo->serial_rate[i]);
        if (AM_OK != err) {
            AM_DBG_INFO("serial rate set failed %d!\r\n", err);
            return NULL;
        }
    }

    return (am_sc16is7xx_handle_t)p_dev;
}

/**
 * \brief 获取指定通道的 UART 标准服务操作句柄
 */
am_uart_handle_t am_sc16is7xx_uart_handle_get (am_sc16is7xx_handle_t handle,
                                               uint8_t               chan)
{
    if ((NULL == handle) ||
        (handle->p_devinfo->chan_num <= chan)) {
        return NULL;
    }

    return &handle->uart_serv[chan];
}

/**
 * \brief SC16IS7XX 解初始化
 */
am_err_t am_sc16is7xx_deinit (am_sc16is7xx_handle_t handle)
{
    am_sc16is7xx_dev_t *p_dev = handle;
    uint32_t            i;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 解初始化 IRQ 引脚 */
    if (-1 != p_dev->p_devinfo->irq_pin) {
        am_gpio_trigger_off(p_dev->p_devinfo->irq_pin);
        am_gpio_trigger_disconnect(p_dev->p_devinfo->irq_pin,
                                   __irq_pint_isr,
                                   p_dev);
        am_gpio_pin_cfg(p_dev->p_devinfo->irq_pin,
                        AM_GPIO_INPUT | AM_GPIO_FLOAT);
    }

    /* 解初始化 RST 引脚 */
    if (-1 != p_dev->p_devinfo->rst_pin) {
        am_gpio_pin_cfg(p_dev->p_devinfo->rst_pin,
                        AM_GPIO_INPUT | AM_GPIO_FLOAT);
    }

    for (i = 0; i < p_dev->p_devinfo->chan_num; i++) {
        p_dev->uart_serv[i].p_funcs = NULL;
        p_dev->pfn_txchar_get[i]    = NULL;
        p_dev->pfn_rxchar_put[i]    = NULL;
        p_dev->pfn_err[i]           = NULL;
    }
    p_dev->i2c_handle = NULL;
    p_dev->p_devinfo  = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    return AM_OK;
}

/* end of file */
