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
 * \brief SC16IS7XX ����
 * \details ���������ж�ģʽ��ʹ������״̬�������ʽ����ȷ���Ƽ�ʹ�� Notepad++ �鿴
 *
 *                             ��������������������������������������������������                 ����������������������������������������������
 *                             ��         ����          ��  I2C �������   ��    ʹ�ܷ����ж�     ��
 * ������������������������������������������������������>��������������������������������������������������<������������������������������������������������������������������������������
 * ��                           ��I2C ����ʧ�� /         ��  [�����жϷ���] ��entry /              ��
 * ��                           ���� RS485 ��������Ϊ���թ���������������������������������>������ I2C ����(д IER)��
 * ��                           ��������������������������������������������������                 ��RS485 ��������Ϊ���� ��
 * ��                                       ��                             ����������������������������������������������
 * ��                                  IRQ Ϊ�͵�ƽ
 * ��                                       ��
 * ��                                       V
 * ��    I2C ����ʧ�ܻ�          ����������������������������������������������
 * ��    I2C ����ɹ�[û���ж�Դ]��        �� IIR       ��
 * ��<����������������������������������������������������������������������������������������������������<������������������������������������������������������������������������������������
 * ��                            ��entry /              ��                                          ��
 * ��                    ���������������������� I2C ����(�� IIR)������������������������������������������������                   ��
 * ��                    ��       ����������������������������������������������                      ��                   ��
 * ��             I2C ����ɹ�                                 I2C ����ɹ�[���� FIFO ���ж�]     ��
 * ��             [���ճ�ʱ��������ж�]                                      ��                   ��
 * ��                    ��                                                    V                   ��
 * ��                    V                                        ��������������������������������������������������       ��
 * ��            ��������������������������������������������������   I2C �������[FIFO ��]��       �� TXLVL        ��       ��
 * ��I2C ������ɩ�       �� RXLVL        ��   ��������������������������������������������������������������������������������������������       ��
 * ��[FIFO Ϊ 0] ��������������������������������������������������   ��                    ��entry /                ��       ��
 * ��<������������������������entry /                ��   ��                    ������ I2C ����(�� TXLVL)��       ��
 * ��            ������ I2C ����(�� RXLVL)��   ��                    ��������������������������������������������������       ��
 * ��            ��������������������������������������������������   ��                         ��            ^             ��
 * ��                  ��            ^        ��         I2C ����ɹ�[FIFO δ��] /    ��             ��
 * ��    I2C ����ɹ�[FIFO ��Ϊ 0]  ��        ��         pfn_txchar_get               ��             ��
 * ��                  ��            ��        ��                         ��  I2C ����ɹ�            ��
 * ��                  ��     I2C ����ɹ� /  ��                         ��  [���ַ���ȡ�� 485 ʹ��] ��
 * ��                  ��     pfn_rxchar_put  ��                         V            ��             ��
 * ��                  V            ��        ��                     ����������������������������������������������        ��
 * ��             ����������������������������������������������    ��                     ��        д THR       ��        ��
 * ��I2C ����ʧ�� ��       �� RHR        ��    ��                     ���������������������������������������������ȩ�����������������
 * ��<����������������������������������������������������������������������    ��                     ��entry �����ַ���ȡ / �� I2C ����ɹ�
 * ��             ��entry /              ��    ��                     ������ I2C ����(д THR)�� [���ַ���ȡ�� 485 ʧ��]
 * ��             ������ I2C ����(�� RHR)��    ��                     ����������������������������������������������
 * ��             ����������������������������������������������    ��                                ��
 * ��                                        ��                   I2C ����ʧ�ܻ� I2C ����ɹ���
 * ��<��������������������������������������������������������������������������������                   ���ַ���ȡ
 * ��                                                                         ��
 * ��                                                                         V
 * ��                                                              ����������������������������������������������
 * ��                        I2C ����ʧ�ܻ� I2C ����ɹ�[485 ʧ��] ��    ʧ�ܷ����ж�     ��
 * ��<������������������������������������������������������������������������������������������������������������������������������������������������������������������������
 * ��                                                              ��entry /              ��
 * ��                                                              ������ I2C ����(д IER)��
 * ��                                                              ����������������������������������������������
 * ��                                                                         ��     
 * ��                                                               I2C ����ɹ�[485 ʹ��]
 * ��                                                                         ��     
 * ��                                     I2C ����ʧ�ܻ�                      V     
 * ��                                     I2C ����ɹ�[�������] / ����������������������������������������������
 * ��                                     RS485 ��������Ϊ����     ��     �ȴ��������    ��
 * ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������<����
 *                                                                ��entry /              ��  ��
 *                                                                ������ I2C ����(�� LSR)��  ��
 *                                                                ����������������������������������������������  ��
 *                                                                           ��             ��
 *                                                               I2C ����ɹ�[����δ���]  ��
 *                                                                           ��             ��
 *                                                                           ������������������������������
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
  �궨��
*******************************************************************************/

#define  __ST_IDLE           0  /**< \brief ����״̬ */
#define  __ST_TX_INT_EN      1  /**< \brief ʹ�ܷ����ж� */
#define  __ST_IIR_RD         2  /**< \brief �� IIR �Ĵ��� */
#define  __ST_RXLVL_RD       3  /**< \brief �� RXLVL �Ĵ��� */
#define  __ST_RHR_RD         4  /**< \brief �� RHR �Ĵ��� */
#define  __ST_TXLVL_RD       5  /**< \brief �� TXLVL �Ĵ��� */
#define  __ST_THR_WE         6  /**< \brief д THR �Ĵ��� */
#define  __ST_TX_INT_DIS     7  /**< \brief ʧ�ܷ����ж� */
#define  __ST_TX_DONE_WAIT   8  /**< \brief �ȴ�������� */

#define  __EVT_NONE          0  /**< \brief ���¼� */
#define  __EVT_IRQ_LOW       1  /**< \brief IRQ Ϊ�͵�ƽ */
#define  __EVT_I2C_OK        2  /**< \brief I2C ����ɹ� */
#define  __EVT_I2C_ERR       3  /**< \brief I2C ����ʧ�� */
#define  __EVT_CHAR_NONE     4  /**< \brief ���ַ���ȡ */
#define  __EVT_CONTINUE      5  /**< \brief ���������¼� */

/** \brief ״̬���ڲ�״̬�л� */
#define __NEXT_STATE(s, e)        \
    do {                          \
        p_dev->state[chan] = (s); \
        new_event = (e);          \
    } while(0)

/** \brief �Ĵ�����ַ */
enum __SC16IS7XX_REG_ADDR {

    /** \brief ͨ�üĴ�����ַ��ֻ���� LCR[7] Ϊ 0 ʱ���� */
    SC16IS7XX_REG_RHR       = 0x00,    /**< \brief ���� FIFO �Ĵ��� */
    SC16IS7XX_REG_THR       = 0x00,    /**< \brief ���� FIFO �Ĵ��� */
    SC16IS7XX_REG_IER       = 0x01,    /**< \brief �ж�ʹ�ܼĴ��� */
    SC16IS7XX_REG_FCR       = 0x02,    /**< \brief FIFO ���ƼĴ��� */
    SC16IS7XX_REG_IIR       = 0x02,    /**< \brief �жϱ�־�Ĵ��� */
    SC16IS7XX_REG_LCR       = 0x03,    /**< \brief �߿��ƼĴ��� */
    SC16IS7XX_REG_MCR       = 0x04,    /**< \brief ���ƽ�������ƼĴ��� */
    SC16IS7XX_REG_LSR       = 0x05,    /**< \brief ��״̬�Ĵ��� */
    SC16IS7XX_REG_MSR       = 0x06,    /**< \brief ���ƽ����״̬�Ĵ��� */
    SC16IS7XX_REG_SPR       = 0x07,    /**< \brief �û����ݼĴ��� */
    SC16IS7XX_REG_TCR       = 0x06,    /**< \brief ������ƼĴ��� */
    SC16IS7XX_REG_TLR       = 0x07,    /**< \brief ����ˮƽ�Ĵ��� */
    SC16IS7XX_REG_TXLVL     = 0x08,    /**< \brief ���� FIFO ˮƽ�Ĵ��� */
    SC16IS7XX_REG_RXLVL     = 0x09,    /**< \brief ���� FIFO ˮƽ�Ĵ��� */
    SC16IS7XX_REG_IODIR     = 0x0A,    /**< \brief I/O ����Ĵ��� */
    SC16IS7XX_REG_IOSTATE   = 0x0B,    /**< \brief I/O ״̬�Ĵ��� */
    SC16IS7XX_REG_IOINTENA  = 0x0C,    /**< \brief I/O �ж�ʹ�ܼĴ��� */
    SC16IS7XX_REG_IOCTRL    = 0x0E,    /**< \brief I/O ���ƼĴ��� */
    SC16IS7XX_REG_EFCR      = 0x0F,    /**< \brief ��չ���Կ��ƼĴ��� */

    /** \brief ����Ĵ�����ַ��ֻ���� LCR[7] Ϊ 1���� LCR ��Ϊ 0xBF ʱ���� */
    SC16IS7XX_REG_DLL       = 0x00,    /**< \brief �����ʷ�Ƶ�����ֽڼĴ��� */
    SC16IS7XX_REG_DLH       = 0x01,    /**< \brief �����ʷ�Ƶ�����ֽڼĴ��� */

    /** \brief ��ǿ�Ĵ�����ַ��ֻ���� LCR Ϊ 0xBF ʱ���� */
    SC16IS7XX_REG_EFR       = 0x02,    /**< \brief ��ǿ���ԼĴ��� */
    SC16IS7XX_REG_XON1      = 0x04,    /**< \brief XON1 �Ĵ��� */
    SC16IS7XX_REG_XON2      = 0x05,    /**< \brief XON2 �Ĵ��� */
    SC16IS7XX_REG_XOFF1     = 0x06,    /**< \brief XOFF1 �Ĵ��� */
    SC16IS7XX_REG_XOFF2     = 0x07,    /**< \brief XOFF2 �Ĵ��� */
};

#define  SC16IS7XX_IER_RHR_EN_BIT    AM_BIT(0)  /**< \brief RHR �ж�ʹ��λ */
#define  SC16IS7XX_IER_THR_EN_BIT    AM_BIT(1)  /**< \brief THR �ж�ʹ��λ */
#define  SC16IS7XX_IER_RLS_EN_BIT    AM_BIT(2)  /**< \brief ������״̬�ж�ʹ��λ */
#define  SC16IS7XX_IER_MODEM_EN_BIT  AM_BIT(3)  /**< \brief ���ƽ����״̬�ж�ʹ��λ */
#define  SC16IS7XX_IER_SLEEP_EN_BIT  AM_BIT(4)  /**< \brief ˯��ģʽʹ��λ */
#define  SC16IS7XX_IER_XOFF_EN_BIT   AM_BIT(5)  /**< \brief XOFF �ж�ʹ��λ */
#define  SC16IS7XX_IER_RTS_EN_BIT    AM_BIT(6)  /**< \brief RTS �ж�ʹ��λ */
#define  SC16IS7XX_IER_CTS_EN_BIT    AM_BIT(7)  /**< \brief CTS �ж�ʹ��λ */

#define  SC16IS7XX_FCR_FIFO_EN_BIT   AM_BIT(0)     /**< \brief FIFO ʹ��λ */
#define  SC16IS7XX_FCR_RX_RESET_BIT  AM_BIT(1)     /**< \brief ���� FIFO ��λλ */
#define  SC16IS7XX_FCR_TX_RESET_BIT  AM_BIT(2)     /**< \brief ���� FIFO ��λλ */
#define  SC16IS7XX_FCR_TX_LEVEL_8    AM_SBF(0, 4)  /**< \brief ���� FIFO 8 �ֽڴ��� */
#define  SC16IS7XX_FCR_TX_LEVEL_16   AM_SBF(1, 4)  /**< \brief ���� FIFO 16 �ֽڴ��� */
#define  SC16IS7XX_FCR_TX_LEVEL_32   AM_SBF(2, 4)  /**< \brief ���� FIFO 32 �ֽڴ��� */
#define  SC16IS7XX_FCR_TX_LEVEL_56   AM_SBF(3, 4)  /**< \brief ���� FIFO 56 �ֽڴ��� */
#define  SC16IS7XX_FCR_RX_LEVEL_8    AM_SBF(0, 6)  /**< \brief ���� FIFO 8 �ֽڴ��� */
#define  SC16IS7XX_FCR_RX_LEVEL_16   AM_SBF(1, 6)  /**< \brief ���� FIFO 16 �ֽڴ��� */
#define  SC16IS7XX_FCR_RX_LEVEL_56   AM_SBF(2, 6)  /**< \brief ���� FIFO 56 �ֽڴ��� */
#define  SC16IS7XX_FCR_RX_LEVEL_60   AM_SBF(3, 6)  /**< \brief ���� FIFO 60 �ֽڴ��� */

#define  SC16IS7XX_IIR_SRC_NONE          AM_SBF(0x01, 0)  /**< \brief NONE */
#define  SC16IS7XX_IIR_SRC_RECV_ERR      AM_SBF(0x06, 0)  /**< \brief ����״̬���� */
#define  SC16IS7XX_IIR_SRC_RECV_TIMEOUT  AM_SBF(0x0C, 0)  /**< \brief ���ճ�ʱ */
#define  SC16IS7XX_IIR_SRC_RHR           AM_SBF(0x04, 0)  /**< \brief �������ж� */
#define  SC16IS7XX_IIR_SRC_THR           AM_SBF(0x02, 0)  /**< \brief ���Ϳ��ж� */
#define  SC16IS7XX_IIR_SRC_MODEM         AM_SBF(0x00, 0)  /**< \brief ���ƽ�����ж� */
#define  SC16IS7XX_IIR_SRC_IO            AM_SBF(0x30, 0)  /**< \brief �����ж� */
#define  SC16IS7XX_IIR_SRC_XOFF          AM_SBF(0x10, 0)  /**< \brief XOFF �ж� */
#define  SC16IS7XX_IIR_SRC_CRTS          AM_SBF(0x20, 0)  /**< \brief CTS/RTS ��Ч�ж� */
#define  SC16IS7XX_IIR_SRC_MASK          AM_SBF(0x3F, 0)  /**< \brief �ж�Դ���� */
#define  SC16IS7XX_IIR_FIFO_EN_FLAG      AM_BIT(6)        /**< \brief FIFO ʹ�ܱ�־ */

#define  SC16IS7XX_LCR_DATA_5BIT         AM_SBF(0, 0)  /**< \brief 5 λ����λ */
#define  SC16IS7XX_LCR_DATA_6BIT         AM_SBF(1, 0)  /**< \brief 6 λ����λ */
#define  SC16IS7XX_LCR_DATA_7BIT         AM_SBF(2, 0)  /**< \brief 7 λ����λ */
#define  SC16IS7XX_LCR_DATA_8BIT         AM_SBF(3, 0)  /**< \brief 8 λ����λ */
#define  SC16IS7XX_LCR_STOP_1BIT         AM_SBF(0, 2)  /**< \brief 1 λֹͣλ */
#define  SC16IS7XX_LCR_STOP_1_5BIT       AM_SBF(1, 2)  /**< \brief 1.5 λֹͣλ������λֻ��Ϊ 5 */
#define  SC16IS7XX_LCR_STOP_2BIT         AM_SBF(1, 2)  /**< \brief 2 λֹͣλ */
#define  SC16IS7XX_LCR_PARITY_NO         AM_SBF(0, 3)  /**< \brief ����żУ��λ */
#define  SC16IS7XX_LCR_PARITY_ODD        AM_SBF(1, 3)  /**< \brief ��У�� */
#define  SC16IS7XX_LCR_PARITY_EVEN       AM_SBF(3, 3)  /**< \brief żУ�� */
#define  SC16IS7XX_LCR_PARITY_FORCE_1    AM_SBF(5, 3)  /**< \brief У��λ����Ϊ 1 */
#define  SC16IS7XX_LCR_PARITY_FORCE_0    AM_SBF(7, 3)  /**< \brief У��λ����Ϊ 0 */
#define  SC16IS7XX_LCR_BREAK_BIT         AM_BIT(6)     /**< \brief ǿ������ TX ���� BREAK */
#define  SC16IS7XX_LCR_DIV_LATCH_EN_BIT  AM_BIT(7)     /**< \brief ��Ƶ��������ʹ��λ */
#define  SC16IS7XX_LCR_ACCESS_GENERAL    0x03          /**< \brief д���ֵ֮�����ͨ�üĴ��� */
#define  SC16IS7XX_LCR_ACCESS_SPECIAL    0x80          /**< \brief д���ֵ֮���������Ĵ��� */
#define  SC16IS7XX_LCR_ACCESS_ENHANCE    0xBF          /**< \brief д���ֵ֮�������ǿ�Ĵ��� */

#define  SC16IS7XX_LSR_DATA_IN_FLAG     AM_BIT(0)  /**< \brief RX FIFO �����ݴ��ڱ�־λ */
#define  SC16IS7XX_LSR_OVERRUN_FLAG     AM_BIT(1)  /**< \brief ���������־λ */
#define  SC16IS7XX_LSR_PARITY_FLAG      AM_BIT(2)  /**< \brief ��żУ������־λ */
#define  SC16IS7XX_LSR_FRAME_FLAG       AM_BIT(3)  /**< \brief ֡�����־λ */
#define  SC16IS7XX_LSR_BREAK_FLAG       AM_BIT(4)  /**< \brief ���յ� BREAK ��־λ */
#define  SC16IS7XX_LSR_THR_EMPTY_FLAG   AM_BIT(5)  /**< \brief ���� FIFO �ձ�־λ */
#define  SC16IS7XX_LSR_THSR_EMPTY_FLAG  AM_BIT(6)  /**< \brief ���� FIFO ����λ�Ĵ����ձ�־λ */
#define  SC16IS7XX_LSR_FIFO_ERR_FLAG    AM_BIT(7)  /**< \brief FIFO �����־λ */

#define  SC16IS7XX_TCR_HALT_LEVEL(num)    AM_SBF((num) & 0x0F, 0)  /**< \brief �ָ�����ˮƽ */
#define  SC16IS7XX_TCR_RESUME_LEVEL(num)  AM_SBF((num) & 0x0F, 4)  /**< \brief ֹͣ����ˮƽ */

#define  SC16IS7XX_TLR_TX_LEVEL(num)  AM_SBF((num) & 0x0F, 0)  /**< \brief ���� FIFO ����ˮƽ */
#define  SC16IS7XX_TLR_RX_LEVEL(num)  AM_SBF((num) & 0x0F, 4)  /**< \brief ���� FIFO ����ˮƽ */

#define  SC16IS7XX_IO_CTRL_RESET_BIT  AM_BIT(3)  /**< \brief �����λλ */

#define  SC16IS7XX_EFR_CTS_EN_BIT     AM_BIT(7)  /**< \brief CTS ����ʹ��λ */
#define  SC16IS7XX_EFR_RTS_EN_BIT     AM_BIT(6)  /**< \brief RTS ����ʹ��λ */
#define  SC16IS7XX_EFR_DETECT_EN_BIT  AM_BIT(5)  /**< \brief ָ���ַ����ʹ��λ */
#define  SC16IS7XX_EFR_ENHANCE_EN_BIT AM_BIT(4)  /**< \brief ��ǿ����ʹ��λ */

/*******************************************************************************
  ���غ�������
*******************************************************************************/

/**
 * \brief SPI ����״̬��
 */
am_local am_err_t __sm_event (am_sc16is7xx_dev_t *p_dev,
                              uint8_t             chan,
                              uint32_t            event);

/**
 * \brief UART ���ƺ���
 */
am_local int __uart_ioctl (void *p_drv, int request, void *p_arg);

/**
 * \brief ���� UART ���ͺ���
 */
am_local int __uart_tx_startup (void *p_drv);

/**
 * \brief ���ô��ڻص�����
 */
am_local int __uart_callback_set (void *p_drv,
                                  int   callback_type,
                                  void *pfn_callback,
                                  void *p_arg);

/**
 * \brief �Ӵ��ڻ�ȡһ���ַ�����ѯģʽ��
 */
am_local int __uart_poll_getchar (void *p_drv, char *p_inchar);

/**
 * \brief ���һ���ַ�����ѯģʽ��
 */
am_local int __uart_poll_putchar (void *p_drv, char outchar);

/*******************************************************************************
  ����ȫ�ֱ�������
*******************************************************************************/

/** \brief ��׼��ӿں���ʵ�� */
am_local am_const struct am_uart_drv_funcs __g_uart_drv_funcs = {
    __uart_ioctl,        /* UART ���ƺ��� */
    __uart_tx_startup,   /* ���� UART ���ͺ��� */
    __uart_callback_set, /* ���ô��ڻص����� */
    __uart_poll_getchar, /* �Ӵ��ڻ�ȡһ���ַ�����ѯģʽ�� */
    __uart_poll_putchar, /* ���һ���ַ�����ѯģʽ�� */
};

/*******************************************************************************
  ���غ�������
*******************************************************************************/

/**
 * \brief I2C ��Ϣ������ɻص�����
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

    /* ��¼״̬��������� */
    p_dev->sm_cnt[*p_chan]++;

    /* ��״̬�������¼� */
    if (AM_OK == p_dev->i2c_msg[*p_chan].status) {
        __sm_event(p_dev, *p_chan, __EVT_I2C_OK);
    } else {
        __sm_event(p_dev, *p_chan, __EVT_I2C_ERR);
    }

    p_dev->sm_cnt[*p_chan]--;
}

/**
 * \brief ACK �����жϷ�����
 */
am_local void __irq_pint_isr (void *p_arg)
{
    am_sc16is7xx_dev_t *p_dev = (am_sc16is7xx_dev_t *)p_arg;
    uint32_t            i;

    am_gpio_trigger_off(p_dev->p_devinfo->irq_pin);

    for (i = 0; i < p_dev->p_devinfo->chan_num; i++) {

        /* ֻ�ж�Ӧͨ�����ж�ʹ��֮�����״̬�������¼� */
        if (!p_dev->is_irq_en[i]) {
            continue;
        }

        if (0 != p_dev->sm_cnt[i]) {
            AM_DBG_INFO("isr %d sm_cnt: %d\r\n", i, p_dev->sm_cnt);
        }

        /* ��¼״̬��������� */
        p_dev->sm_cnt[i]++;

        /* ��״̬�������¼� */
        __sm_event(p_dev, i, __EVT_IRQ_LOW);

        p_dev->sm_cnt[i]--;
    }

    am_gpio_trigger_on(p_dev->p_devinfo->irq_pin);
}

/**
 * \brief ʹ�� IRQ �ж�
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
 * \brief �ر� IRQ �ж�
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
 * \brief д�Ĵ���
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

    /* ���õ�ַ�ֽ� */
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
 * \brief ���Ĵ���
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

    /* ���õ�ַ�ֽ� */
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
 * \brief ���� I2C �����д�Ĵ���
 *
 * \param[in]     p_dev     SC16IS7XX �豸
 * \param[in]     chan      ͨ��
 * \param[in]     is_read   �Ƿ��Ƕ��Ĵ���
 * \param[in]     reg_addr  �Ĵ�����ַ
 * \param[in,out] p_buf     ָ�򻺳�����ָ��
 * \param[in]     buf_lenth ��Ҫ��д���ֽ���
 *
 * \retval  AM_OK     I2C ���������ɹ����ȴ�����
 * \retval -AM_EINVAL ��������
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

    /* �ж� I2C �Ƿ�æ */
    key = am_int_cpu_lock();
    if (p_dev->is_i2c_busy[chan]) {
        am_int_cpu_unlock(key);
        return -AM_EBUSY;
    }
    p_dev->is_i2c_busy[chan] = AM_TRUE;
    am_int_cpu_unlock(key);

    /* ���õ�ַ�ֽ� */
    p_dev->subaddr[chan] = 0;
    AM_BITS_SET(p_dev->subaddr[chan], 1, 2, chan);
    AM_BITS_SET(p_dev->subaddr[chan], 3, 4, reg_addr);

    am_i2c_mktrans(&p_dev->i2c_trans[chan][0],   /* �� 0 �����䷢���ӵ�ַ */
                    p_dev->p_devinfo->i2c_addr,  /* Ŀ��������ַ */
                    AM_I2C_M_7BIT | AM_I2C_M_WR, /* 7 λ������ַ��д���� */
                   &p_dev->subaddr[chan],        /* �ӵ�ַ */
                    1);                          /* �ӵ�ַ���ȣ�1 �ֽ� */

    if (is_read) {
        am_i2c_mktrans(&p_dev->i2c_trans[chan][1],   /* �� 1 �������ȡ���� */
                        p_dev->p_devinfo->i2c_addr,  /* Ŀ��������ַ */
                        AM_I2C_M_RD,                 /* ������ */
                        p_buf,                       /* ���ݻ����� */
                        buf_lenth);                  /* ���ݸ��� */
    } else {
        am_i2c_mktrans(&p_dev->i2c_trans[chan][1],      /* �� 1 �����䷢������ */
                        p_dev->p_devinfo->i2c_addr,     /* Ŀ��������ַ (��������) */
                        AM_I2C_M_WR | AM_I2C_M_NOSTART, /* д����, ��������ʼ�źż��ӻ���ַ */
                        p_buf,                          /* ���ݻ����� */
                        buf_lenth);                     /* ���ݸ��� */
    }

    am_i2c_mkmsg(&p_dev->i2c_msg[chan],       /* I2C ��Ϣ */
                 &p_dev->i2c_trans[chan][0],  /* I2C ���� */
                  2,                          /* I2C �������� */
                  __i2c_complete,             /* ��Ϣ������ɻص����� */
                 &p_dev->uartinfo[chan]);     /* ���ݸ��ص������Ĳ��� */

    /* ���� I2C ���� */
    err = am_i2c_msg_start(p_dev->i2c_handle, &p_dev->i2c_msg[chan]);
    if (AM_OK != err) {
        key = am_int_cpu_lock();
        p_dev->is_i2c_busy[chan] = AM_FALSE;
        am_int_cpu_unlock(key);
    }

    return err;
}

/**
 * \brief SPI ����״̬��
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

        /* ��������ڲ������¼� */
        if (new_event != __EVT_NONE) {
            event = new_event;
            new_event = __EVT_NONE;
        }

        switch (p_dev->state[chan]) {

        /* ����״̬ */
        case __ST_IDLE:

            /* �������״̬ʱ���ȼ���Ƿ��������жϷ��͵��¼� */
            do {
                if ((p_dev->is_i2c_busy[chan]) ||
                    (!p_dev->int_en_flag[chan])) {
                    break;
                }

                key = am_int_cpu_lock();
                p_dev->int_en_flag[chan] = AM_FALSE;
                am_int_cpu_unlock(key);

                /* ��������ж��Ѿ�ʹ�ܣ�ֱ���˳� */
                if (SC16IS7XX_IER_THR_EN_BIT & p_dev->ier_reg[chan]) {
                    break;
                }

                /* ʹ�ܷ����ж� */
                p_dev->ier_reg[chan] |= SC16IS7XX_IER_THR_EN_BIT;

                __int_disable(p_dev, chan);

                /* ���� I2C ����д IER �Ĵ��� */
                err = __i2c_start(p_dev,                 /* SC16IS7X */
                                  chan,                  /* ͨ�� */
                                  AM_FALSE,              /* �Ƿ��Ƕ��Ĵ��� */
                                  SC16IS7XX_REG_IER,     /* �Ĵ�����ַ */
                                 &p_dev->ier_reg[chan],  /* ָ�򻺳�����ָ�� */
                                  1);                    /* ��Ҫ��д���ֽ��� */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    __int_enable(p_dev, chan);
                } else {
                    __NEXT_STATE(__ST_TX_INT_EN, __EVT_NONE);

                    /* �� RS485 ����Ϊ����ģʽ */
                    if ((p_dev->is_rs485_en[chan]) &&
                        (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
                        p_dev->p_devinfo->pfn_rs485_dir(chan, AM_TRUE);
                    }
                }
                break;
            } while (AM_FALSE);

            /* ���д�� IER �Ĵ�����ֱ�ӷ��� */
            if ((__ST_TX_INT_EN == p_dev->state[chan]) &&
                (__EVT_NONE == new_event)) {
                break;
            }

            /* �� I2C �����¼����м�����������ֵ֮���˳�״̬�� */
            if (__EVT_I2C_ERR == event) {

                /* �� RS485 ����Ϊ����ģʽ */
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

                /* �ж� IRQ �Ƿ�Ϊ�͵�ƽ */
                if (!am_gpio_get(p_dev->p_devinfo->irq_pin)) {

                    __int_disable(p_dev, chan);

                    /* ���� I2C ����� IIR �Ĵ��� */
                    err = __i2c_start(p_dev,                /* SC16IS7X */
                                      chan,                 /* ͨ�� */
                                      AM_TRUE,              /* �Ƿ��Ƕ��Ĵ��� */
                                      SC16IS7XX_REG_IIR,    /* �Ĵ�����ַ */
                                     &p_dev->iir_reg[chan], /* ָ�򻺳�����ָ�� */
                                      1);                   /* ��Ҫ��д���ֽ��� */
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

        /* ʹ�ܷ����ж� */
        case __ST_TX_INT_EN:

            /* I2C ����ʧ�� */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
            }

            /* I2C ����ɹ� */
            if (__EVT_I2C_OK == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
            }

            __int_enable(p_dev, chan);

            break;

        /* �� IIR �Ĵ��� */
        case __ST_IIR_RD:

            /* I2C ����ʧ�ܣ�ֱ�ӻص�����״̬ */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C ����ɹ��������ж�Դѡ����һ��״̬ */
            if (__EVT_I2C_OK == event) {

                /* ���û���ж�Դ�˳�״̬�� */
                if (SC16IS7XX_IIR_SRC_NONE ==
                    (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK)) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                    __int_enable(p_dev, chan);
                    break;
                } else if ((SC16IS7XX_IIR_SRC_RHR ==
                            (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK)) ||
                           (SC16IS7XX_IIR_SRC_RECV_TIMEOUT ==
                            (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK))) {

                    /* ���� FIFO �����߽��ճ�ʱ */

                    /* ���� I2C ����� RXLVL �Ĵ��� */
                    err = __i2c_start(p_dev,                  /* SC16IS7X */
                                      chan,                   /* ͨ�� */
                                      AM_TRUE,                /* �Ƿ��Ƕ��Ĵ��� */
                                      SC16IS7XX_REG_RXLVL,    /* �Ĵ�����ַ */
                                     &p_dev->rxlvl_reg[chan], /* ָ�򻺳�����ָ�� */
                                      1);                     /* ��Ҫ��д���ֽ��� */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_RXLVL_RD, __EVT_NONE);
                    }
                    break;
                } else if (SC16IS7XX_IIR_SRC_THR ==
                           (p_dev->iir_reg[chan] & SC16IS7XX_IIR_SRC_MASK)) {

                    /* ���� FIFO �� */

                    /* ���� I2C ����� TXLVL �Ĵ��� */
                    err = __i2c_start(p_dev,                  /* SC16IS7X */
                                      chan,                   /* ͨ�� */
                                      AM_TRUE,                /* �Ƿ��Ƕ��Ĵ��� */
                                      SC16IS7XX_REG_TXLVL,    /* �Ĵ�����ַ */
                                     &p_dev->txlvl_reg[chan], /* ָ�򻺳�����ָ�� */
                                      1);                     /* ��Ҫ��д���ֽ��� */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_TXLVL_RD, __EVT_NONE);
                    }
                    break;
                } else {

                    /* ���û��ƥ����ж�Դ��֤�� I2C ��ȡ���� */
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                }
                break;
            }
            break;

        /* �� RXLVL �Ĵ��� */
        case __ST_RXLVL_RD:

            /* I2C ����ʧ�� */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C ����ɹ� */
            if (__EVT_I2C_OK == event) {

                /* ������� FIFO �ֽڳ��� FIFO ��С��֤�� I2C ������󣬷��ؿ���״̬ */
                if ((0 == p_dev->rxlvl_reg[chan]) ||
                    (SC16IS7XX_FIFO_SIZE < p_dev->rxlvl_reg[chan])) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    break;
                }

                /* ���� I2C ����� RHR �Ĵ��� */
                err = __i2c_start(p_dev,                   /* SC16IS7X */
                                  chan,                    /* ͨ�� */
                                  AM_TRUE,                 /* �Ƿ��Ƕ��Ĵ��� */
                                  SC16IS7XX_REG_RHR,       /* �Ĵ�����ַ */
                                 &p_dev->rx_buf[chan][0],  /* ָ�򻺳�����ָ�� */
                                  p_dev->rxlvl_reg[chan]); /* ��Ҫ��д���ֽ��� */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_RHR_RD, __EVT_NONE);
                }
                break;
            }
            break;

        /* �� RHR �Ĵ��� */
        case __ST_RHR_RD:

            /* I2C ����ʧ�� */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C ����ɹ� */
            if (__EVT_I2C_OK == event) {

                /* ���ж� I2C �Ƿ����Դ��䶼��� */
                if (p_dev->i2c_msg[chan].trans_num != p_dev->i2c_msg[chan].done_num){
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                    break;
                }

                /* �����յ��������ύ�� UART ��ϵͳ */
                for (i = 0; i < p_dev->rxlvl_reg[chan]; i++) {
                    if (NULL == p_dev->pfn_rxchar_put[chan]) {
                        break;
                    }
                    p_dev->pfn_rxchar_put[chan](
                               p_dev->p_rxput_arg[chan],
                               p_dev->rx_buf[chan][i]);
                }

                /* ���� I2C ����� RXLVL �Ĵ��� */
                err = __i2c_start(p_dev,                  /* SC16IS7X */
                                  chan,                   /* ͨ�� */
                                  AM_TRUE,                /* �Ƿ��Ƕ��Ĵ��� */
                                  SC16IS7XX_REG_RXLVL,    /* �Ĵ�����ַ */
                                 &p_dev->rxlvl_reg[chan], /* ָ�򻺳�����ָ�� */
                                  1);                     /* ��Ҫ��д���ֽ��� */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_RXLVL_RD, __EVT_NONE);
                }
                break;
            }
            break;

        /* �� TXLVL �Ĵ��� */
        case __ST_TXLVL_RD:

            /* I2C ����ʧ�� */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C ����ɹ� */
            if (__EVT_I2C_OK == event) {

                /* ������� FIFO �������ؿ���״̬ */
                if (0 == p_dev->txlvl_reg[chan]) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                    break;
                }

                /* ������� FIFO �����ֽڳ��� FIFO ��С��֤�� I2C ������󣬷��ؿ���״̬ */
                if (SC16IS7XX_FIFO_SIZE < p_dev->txlvl_reg[chan]) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    break;
                }

                /* ��� RS485 δʹ�ܣ�һ�����д���� FIFO �����ݣ�����д��ʱ����� */
                if ((!p_dev->is_rs485_en[chan]) &&
                    (SC16IS7XX_FIFO_SIZE / 2 < p_dev->txlvl_reg[chan])) {
                    p_dev->txlvl_reg[chan] = SC16IS7XX_FIFO_SIZE / 2;
                }

                /* �� UART ��ϵͳ��ȡ��Ҫ���͵����� */
                for (i = 0; i < p_dev->txlvl_reg[chan]; i++) {
                    err = p_dev->pfn_txchar_get[chan](
                                     p_dev->p_txget_arg[chan],
                            (char *)&p_dev->tx_buf[chan][i]);
                    if (err != AM_OK) {
                        break;
                    }
                }

                /* �ж��Ƿ��ȡ������ */
                if (0 == i) {
                    __NEXT_STATE(__ST_THR_WE, __EVT_CHAR_NONE);
                    break;
                }

                /* ���� I2C ����д THR �Ĵ��� */
                err = __i2c_start(p_dev,                  /* SC16IS7X */
                                  chan,                   /* ͨ�� */
                                  AM_FALSE,               /* �Ƿ��Ƕ��Ĵ��� */
                                  SC16IS7XX_REG_THR,      /* �Ĵ�����ַ */
                                 &p_dev->tx_buf[chan][0], /* ָ�򻺳�����ָ�� */
                                  i);                     /* ��Ҫ��д���ֽ��� */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CHAR_NONE);
                } else {

                    /* �жϴ� UART ��ϵͳ��ȡ�����Ƿ�ɹ� */
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

        /* д THR �Ĵ��� */
        case __ST_THR_WE:

            /* ���ַ���ȡ�� I2C ����ʧ�� */
            if ((p_dev->char_none_flag[chan]) ||
                (__EVT_CHAR_NONE == event) ||
                (__EVT_I2C_ERR == event)) {

                key = am_int_cpu_lock();
                p_dev->char_none_flag[chan] = AM_FALSE;
                am_int_cpu_unlock(key);

                /* ʧ�ܷ����ж� */
                p_dev->ier_reg[chan] &= ~SC16IS7XX_IER_THR_EN_BIT;

                /* ���� I2C ����д IER �Ĵ��� */
                err = __i2c_start(p_dev,                /* SC16IS7X */
                                  chan,                 /* ͨ�� */
                                  AM_FALSE,             /* �Ƿ��Ƕ��Ĵ��� */
                                  SC16IS7XX_REG_IER,    /* �Ĵ�����ַ */
                                 &p_dev->ier_reg[chan], /* ָ�򻺳�����ָ�� */
                                  1);                   /* ��Ҫ��д���ֽ��� */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_TX_INT_DIS, __EVT_NONE);
                }
                break;
            }

            /* I2C ����ɹ� */
            if (__EVT_I2C_OK == event) {

                /*
                 * ���ʹ���� RS485���������ݣ�ֱ����ɣ���Ϊ��˫�� RS485 ����
                 * ����ʱ�����ܽ�������
                 */
                if ((p_dev->is_rs485_en[chan]) &&
                    (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {

                    /* ���� I2C ����� TXLVL �Ĵ��� */
                    err = __i2c_start(p_dev,                  /* SC16IS7X */
                                      chan,                   /* ͨ�� */
                                      AM_TRUE,                /* �Ƿ��Ƕ��Ĵ��� */
                                      SC16IS7XX_REG_TXLVL,    /* �Ĵ�����ַ */
                                     &p_dev->txlvl_reg[chan], /* ָ�򻺳�����ָ�� */
                                      1);                     /* ��Ҫ��д���ֽ��� */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);

                        /* �� RS485 ����Ϊ����ģʽ */
                        if ((p_dev->is_rs485_en[chan]) &&
                            (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
                            p_dev->p_devinfo->pfn_rs485_dir(chan, AM_FALSE);
                        }
                    } else {
                        __NEXT_STATE(__ST_TXLVL_RD, __EVT_NONE);
                    }
                } else { /* ��ÿ�η���֮���ȡ IIR �Ĵ������鿴�Ƿ���յ����ݣ����ⶪʧ */

                    /* ���� I2C ����� IIR �Ĵ��� */
                    err = __i2c_start(p_dev,                /* SC16IS7X */
                                      chan,                 /* ͨ�� */
                                      AM_TRUE,              /* �Ƿ��Ƕ��Ĵ��� */
                                      SC16IS7XX_REG_IIR,    /* �Ĵ�����ַ */
                                     &p_dev->iir_reg[chan], /* ָ�򻺳�����ָ�� */
                                      1);                   /* ��Ҫ��д���ֽ��� */
                    if (AM_OK != err) {
                        __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                    } else {
                        __NEXT_STATE(__ST_IIR_RD, __EVT_NONE);
                    }
                }
                break;
            }
            break;

        /* ʧ�ܷ����ж� */
        case __ST_TX_INT_DIS:

            /* I2C ����ʧ�� */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
            }

            /* RS485 δʹ�� */
            if (!p_dev->is_rs485_en[chan]) {
                __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);
                break;
            }

            /* ���ַ���ȡ�� I2C ����ɹ� */
            if ((__EVT_CHAR_NONE == event) ||
                (__EVT_I2C_OK == event)) {

                /* ���� I2C ����� LSR �Ĵ��� */
                err = __i2c_start(p_dev,                /* SC16IS7X */
                                  chan,                 /* ͨ�� */
                                  AM_TRUE,              /* �Ƿ��Ƕ��Ĵ��� */
                                  SC16IS7XX_REG_LSR,    /* �Ĵ�����ַ */
                                 &p_dev->lsr_reg[chan], /* ָ�򻺳�����ָ�� */
                                  1);                   /* ��Ҫ��д���ֽ��� */
                if (AM_OK != err) {
                    __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                } else {
                    __NEXT_STATE(__ST_TX_DONE_WAIT, __EVT_NONE);
                }
            }
            break;

        /* �ȴ�������� */
        case __ST_TX_DONE_WAIT:

            /* I2C ����ʧ�� */
            if (__EVT_I2C_ERR == event) {
                __NEXT_STATE(__ST_IDLE, __EVT_I2C_ERR);
                break;
            }

            /* I2C ����ɹ� */
            if (__EVT_I2C_OK == event) {

                /* ������� */
                if (SC16IS7XX_LSR_THSR_EMPTY_FLAG & p_dev->lsr_reg[chan]) {
                    __NEXT_STATE(__ST_IDLE, __EVT_CONTINUE);

                    /* �� RS485 ����Ϊ����ģʽ */
                    if ((p_dev->is_rs485_en[chan]) &&
                        (NULL != p_dev->p_devinfo->pfn_rs485_dir)) {
                        p_dev->p_devinfo->pfn_rs485_dir(chan, AM_FALSE);
                    }
                    break;
                } else {

                    /* ���� I2C ����� LSR �Ĵ��� */
                    err = __i2c_start(p_dev,                /* SC16IS7X */
                                      chan,                 /* ͨ�� */
                                      AM_TRUE,              /* �Ƿ��Ƕ��Ĵ��� */
                                      SC16IS7XX_REG_LSR,    /* �Ĵ�����ַ */
                                     &p_dev->lsr_reg[chan], /* ָ�򻺳�����ָ�� */
                                      1);                   /* ��Ҫ��д���ֽ��� */
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

        /* û�������ڲ����¼������� */
        if (new_event == __EVT_NONE) {
            break;
        }
    }

    return AM_OK;
}

/**
 * \brief ͨ�üĴ�������ʹ��
 */
am_local am_err_t __general_access_enable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    return __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, p_dev->lcr_reg[chan]);
}

/**
 * \brief ����Ĵ�������ʹ��
 */
am_local am_err_t __special_access_enable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    return __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, SC16IS7XX_LCR_ACCESS_SPECIAL);
}

/**
 * \brief ��ǿ�Ĵ�������ʹ��
 */
am_local am_err_t __enhance_access_enable (am_sc16is7xx_dev_t *p_dev, uint8_t chan)
{
    return __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, SC16IS7XX_LCR_ACCESS_ENHANCE);
}

/**
 * \brief ���ô��ڲ�����
 */
am_local am_err_t __serial_rate_set (am_sc16is7xx_dev_t *p_dev,
                                     uint8_t             chan,
                                     uint32_t            baudrate)
{
    am_sc16is7xx_devinfo_t *p_devinfo = (am_sc16is7xx_devinfo_t *)p_dev->p_devinfo;
    am_err_t                err;
    uint32_t                divisor;

    /* �жϲ������Ƿ�֧�� */
    if (((p_devinfo->clk_freq / 16) < baudrate) ||
        ((p_devinfo->clk_freq / 16 / ((2 << 16) - 1)) > baudrate)) {
        return -AM_ENOTSUP;
    }

    /* ����Ĵ�������ʹ�� */
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

    /* ͨ�üĴ�������ʹ�� */
    err = __general_access_enable(p_dev, chan);

    return err;
}

/**
 * \brief ���ô���ѡ��
 */
am_local am_err_t  __serial_opts_set (am_sc16is7xx_dev_t *p_dev,
                                      uint8_t             chan,
                                      uint32_t            opts)
{
    am_err_t err;
    uint8_t  reg = 0;

    /* �������ݳ��� */
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

    /* ������żУ��λ */
    if (opts & AM_UART_PARENB) {
        if (opts & AM_UART_PARODD) {
            reg |= SC16IS7XX_LCR_PARITY_ODD;
        } else {
            reg |= SC16IS7XX_LCR_PARITY_EVEN;
        }
    } else {
        reg |= SC16IS7XX_LCR_PARITY_NO;
    }

    /* ����ֹͣλ */
    if (opts & AM_UART_STOPB) {
        reg |= SC16IS7XX_LCR_STOP_2BIT;
    } else {
        reg |= SC16IS7XX_LCR_STOP_1BIT;
    }

    /* ������д�� LCR �Ĵ��� */
    err = __reg_write(p_dev, chan, SC16IS7XX_REG_LCR, reg);
    if (AM_OK == err) {
        p_dev->lcr_reg[chan] = reg;
        p_dev->serial_opts[chan] = opts;
    }

    return AM_OK;
}

/**
 * \brief ���ô���ģʽ
 */
am_local am_err_t __serial_mode_set (am_sc16is7xx_dev_t *p_dev,
                                     uint8_t             chan,
                                     uint32_t            mode)
{
    uint8_t  reg = 0;
    am_err_t err = AM_OK;

    /* �ж��Ƿ�֧�����ģʽ */
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

        /* ��ѯģʽֱ�ӹر������ж� */
        reg = 0;
        err = __reg_write(p_dev, chan, SC16IS7XX_REG_IER, reg);
    } else if (AM_UART_MODE_INT == mode) {

        __int_enable(p_dev, chan);

        /* �ж�ģʽ��ʹ�ܽ����ж� */
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
 * \brief UART ���ƺ���
 */
am_local int __uart_ioctl (void *p_drv, int request, void *p_arg)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];
    am_err_t               err        = AM_OK;

    switch (request) {

    /* ���������� */
    case AM_UART_BAUD_SET:
        err = __serial_rate_set(p_dev, *p_chan, (uint32_t)p_arg);
        break;

    /* �����ʻ�ȡ */
    case AM_UART_BAUD_GET:
        *(int *)p_arg = p_dev->serial_rate[*p_chan];
        break;

    /* ����ѡ������ */
    case AM_UART_OPTS_SET:
        err = __serial_opts_set(p_dev, *p_chan, (uint32_t)p_arg);
        break;

    /* ����ѡ���ȡ */
    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_dev->serial_opts[*p_chan];
        break;

    /* ģʽ���� */
    case AM_UART_MODE_SET:
        err = __serial_mode_set(p_dev, *p_chan, (uint32_t)p_arg);
        break;

    /* ģʽ��ȡ */
    case AM_UART_MODE_GET:
        *(int *)p_arg = p_dev->serial_mode[*p_chan];
        break;

    /* ��ȡ���ڿ����õ�ģʽ */
    case AM_UART_AVAIL_MODES_GET:
        *(int *)p_arg = p_dev->serial_mode_avail;
        break;

    /* ���� RS485 ģʽ */
    case AM_UART_RS485_SET:
        p_dev->is_rs485_en[*p_chan] = (am_bool_t)(int)p_arg;
        break;

    /* ��ȡ��ǰ�� RS485 ģʽ״̬ */
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
 * \brief ���� UART ���ͺ���
 */
am_local int __uart_tx_startup (void *p_drv)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];
    uint32_t               key;

    /* �жϵ�ǰ�Ƿ����ж�ģʽ */
    if (AM_UART_MODE_INT != p_dev->serial_mode[*p_chan]) {
        return -AM_EPERM;
    }

    /* �жϻص������Ƿ�ע�� */
    if (NULL == p_dev->pfn_txchar_get[*p_chan]) {
        return -AM_EPERM;
    }

    /* �жϷ����ж��Ƿ��Ѿ�ʹ�� */
    if (p_dev->ier_reg[*p_chan] & SC16IS7XX_IER_THR_EN_BIT) {
        return AM_OK;
    }

    key = am_int_cpu_lock();
    p_dev->int_en_flag[*p_chan] = AM_TRUE;
    am_int_cpu_unlock(key);

    /* ���״̬���Ѿ������У�ֱ�ӷ��� */
    if ((0 != p_dev->sm_cnt[*p_chan]) || (p_dev->is_i2c_busy[*p_chan])) {
        return AM_OK;
    }

    /* ��¼״̬��������� */
    p_dev->sm_cnt[*p_chan]++;

    /* ��״̬�������¼� */
    __sm_event(p_dev, *p_chan, __EVT_NONE);

    p_dev->sm_cnt[*p_chan]--;

    return AM_OK;
}

/**
 * \brief ���ô��ڻص�����
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

    /* ��ȡһ�������ַ� */
    case AM_UART_CALLBACK_TXCHAR_GET:
        p_dev->pfn_txchar_get[*p_chan] = (am_uart_txchar_get_t)pfn_callback;
        p_dev->p_txget_arg[*p_chan]    = p_arg;
        break;

    /* �ύһ�����յ����ַ� */
    case AM_UART_CALLBACK_RXCHAR_PUT:
        p_dev->pfn_rxchar_put[*p_chan] = (am_uart_rxchar_put_t)pfn_callback;
        p_dev->p_rxput_arg[*p_chan]    = p_arg;
        break;

    /* ����ص����� */
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
 * \brief �Ӵ��ڻ�ȡһ���ַ�����ѯģʽ��
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

    /* �жϵ�ǰ�Ƿ�����ѯģʽ */
    if (AM_UART_MODE_POLL != p_dev->serial_mode[*p_chan]) {
        return -AM_EPERM;
    }

    /* ��ȡ��״̬�Ĵ��� */
    err = __reg_read(p_dev, *p_chan, SC16IS7XX_REG_LSR, &reg);
    if (AM_OK != err) {
        return err;
    }
  
    /* �жϽ��ջ������Ƿ������� */
    if (!(SC16IS7XX_LSR_DATA_IN_FLAG & reg)) {
        return -AM_EAGAIN;
    }

    /* �ж��Ƿ��д��� */
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
 * \brief ���һ���ַ�����ѯģʽ��
 */
am_local int __uart_poll_putchar (void *p_drv, char outchar)
{
    am_sc16is7xx_cbinfo_t *p_uartinfo = (am_sc16is7xx_cbinfo_t *)p_drv;
    uint8_t               *p_chan     = (uint8_t *)p_uartinfo->p_arg[0];
    am_sc16is7xx_dev_t    *p_dev      = (am_sc16is7xx_dev_t *)p_uartinfo->p_arg[1];
    am_err_t               err;
    uint8_t                reg;

    /* �жϵ�ǰ�Ƿ�����ѯģʽ */
    if (AM_UART_MODE_POLL != p_dev->serial_mode[*p_chan]) {
        return -AM_EPERM;
    }

    /* ��ȡ���� FIFO ˮƽ�Ĵ��� */
    err = __reg_read(p_dev, *p_chan, SC16IS7XX_REG_TXLVL, &reg);
    if (AM_OK != err) {
        return err;
    }

    /* �жϷ��ͻ������Ƿ�Ϊ�� */
    if (0 == reg) {
        return -AM_EAGAIN;
    }

    /* �� RS485 ����Ϊ����ģʽ */
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

    /* �ȴ�������ɣ����� RS485 ����Ϊ����ģʽ */
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
  �ⲿ��������
*******************************************************************************/

/**
 * \brief SC16IS7XX ��ʼ��
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
        p_dev->pfn_txchar_get[i]    = __uart_poll_getchar;
        p_dev->pfn_rxchar_put[i]    = __uart_poll_putchar;
        p_dev->pfn_err[i]           = NULL;

        p_dev->serial_rate[i]       = p_devinfo->serial_rate[i];
        p_dev->serial_opts[i]       = p_devinfo->serial_data[i];  
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

        /* ���� I2C �豸 */
        am_i2c_mkdev(&p_dev->i2c_dev[i],
                      i2c_handle,
                      p_devinfo->i2c_addr,
                      AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    }

    /* ��ʼ�� RST ���� */
    if (-1 != p_devinfo->rst_pin) {
        am_gpio_pin_cfg(p_devinfo->rst_pin, AM_GPIO_OUTPUT_INIT_HIGH);
        am_gpio_set(p_devinfo->rst_pin, AM_GPIO_LEVEL_LOW);
        am_mdelay(1);
        am_gpio_set(p_devinfo->rst_pin, AM_GPIO_LEVEL_HIGH);
        am_mdelay(1);
    } else {
        am_mdelay(1);

        /* ���û������ RST ���ţ���ʹ�������λ */
        __reg_write(p_dev, 0, SC16IS7XX_REG_IOCTRL, SC16IS7XX_IO_CTRL_RESET_BIT);
        
        /* ʹ��ͨ�üĴ������� */
        __reg_write(p_dev, 0, SC16IS7XX_REG_LCR, SC16IS7XX_LCR_ACCESS_GENERAL);

        am_mdelay(1);
    }

    /* ��ʼ�� IRQ ���� */
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

        /* ʹ����ǿ���� */
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

        /* �������ݳ��� */
        if (5 == p_devinfo->serial_data[i]) {
            p_dev->serial_opts[i] = AM_UART_CS5;
        } else if (6 == p_devinfo->serial_data[i]) {
            p_dev->serial_opts[i] = AM_UART_CS6;
        } else if (7 == p_devinfo->serial_data[i]) {
            p_dev->serial_opts[i] = AM_UART_CS7;
        } else {
            p_dev->serial_opts[i] = AM_UART_CS8;
        }

        /* ������żУ��λ */
        if (1 == p_devinfo->serial_parity[i]) {
            p_dev->serial_opts[i] |= AM_UART_PARENB | AM_UART_PARODD;
        } else if (2 == p_devinfo->serial_parity[i]) {
            p_dev->serial_opts[i] |= AM_UART_PARENB;
        }

        /* ����ֹͣλ */
        if (2 == p_devinfo->serial_stop[i]) {
            p_dev->serial_opts[i] |= AM_UART_STOPB;
        }

        /* ���ô���ѡ�� */
        err = __serial_opts_set(p_dev, i, p_dev->serial_opts[i]);
        if (AM_OK != err) {
            AM_DBG_INFO("serial opts set failed %d!\r\n", err);
            return NULL;
        }

        /* ���ò����� */
        err = __serial_rate_set(p_dev, i, p_devinfo->serial_rate[i]);
        if (AM_OK != err) {
            AM_DBG_INFO("serial rate set failed %d!\r\n", err);
            return NULL;
        }
    }

    return (am_sc16is7xx_handle_t)p_dev;
}

/**
 * \brief ��ȡָ��ͨ���� UART ��׼����������
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
 * \brief ָ��ͨ���� UART ��ȡ����
 */
int am_sc16is7xx_uart_poll_receive (am_sc16is7xx_handle_t handle,
                                    uint8_t               chan,
                                    char                 *p_rxbuf, 
                                    uint32_t              nbytes)                                    
{
    int ret = AM_OK;

    /* �жϲ�����Ч�� */
    if ((NULL == handle) || (handle->p_devinfo->chan_num <= chan)) {
        return -AM_EINVAL;
    }

    while (nbytes-- && AM_OK == ret) {  
        ret = __uart_poll_getchar (&(handle->uartinfo[chan]), p_rxbuf);
        if (AM_OK != ret) {
            *p_rxbuf = '\0';
        }
        p_rxbuf++;
    } 

    return AM_OK; 
}

/**
 * \brief ָ��ͨ���� UART �����ַ���
 */
int am_sc16is7xx_uart_poll_send (am_sc16is7xx_handle_t handle,
                                 uint8_t               chan,
                                 char                 *p_txbuf, 
                                 uint32_t              nbytes)  
{
    int ret = 0;

    /* �жϲ�����Ч�� */
    if ((NULL == handle) || (handle->p_devinfo->chan_num <= chan)) {
        return -AM_EINVAL;
    }

    while (nbytes-- && *p_txbuf != '\0') {  
        ret = __uart_poll_putchar (&(handle->uartinfo[chan]), *p_txbuf++);

        if (AM_OK != ret) {
            return ret;
        }
    }

    return ret;
}

/**
 * \brief SC16IS7XX ���ʼ��
 */
am_err_t am_sc16is7xx_deinit (am_sc16is7xx_handle_t handle)
{
    am_sc16is7xx_dev_t *p_dev = handle;
    uint32_t            i;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* ���ʼ�� IRQ ���� */
    if (-1 != p_dev->p_devinfo->irq_pin) {
        am_gpio_trigger_off(p_dev->p_devinfo->irq_pin);
        am_gpio_trigger_disconnect(p_dev->p_devinfo->irq_pin,
                                   __irq_pint_isr,
                                   p_dev);
        am_gpio_pin_cfg(p_dev->p_devinfo->irq_pin,
                        AM_GPIO_INPUT | AM_GPIO_FLOAT);
    }

    /* ���ʼ�� RST ���� */
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
