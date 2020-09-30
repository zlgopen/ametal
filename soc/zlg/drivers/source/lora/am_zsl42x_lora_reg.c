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
 * \file zsl42x lora 读写寄存器相关操作
 *
 * \internal
 * \par Modification history
 * - 1.0.0 20-03-25  enf,first implementation.
 * - 1.0.1 20-09-23  yg, adaptation zsl42x.
 * \endinternal
 */

#include "am_zsl42x_lora_reg.h"
#include "am_zsl42x_lora.h"

/*******************************************************************************
    Function
*******************************************************************************/

int am_zsl42x_lora_setsleep (am_zsl42x_lora_dev_t                  *p_dev,
                             am_zsl42x_lora_sleep_mode_t            mode,
                             am_zsl42x_lora_sleep_mode_rtc_wakeup_t enable)
{
    uint8_t data   = 0;
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_SLEEP;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    data = ((uint8_t)mode << 2) | ((uint8_t)enable << 0);

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &data,
                                             1);
}

int am_zsl42x_lora_setstandby (am_zsl42x_lora_dev_t  *p_dev,
                               uint8_t                mode)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_STANDBY;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &mode,
                                             1);
}

int am_zsl42x_lora_setfs (am_zsl42x_lora_dev_t *p_dev)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_FS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             NULL,
                                             0);
}

int am_zsl42x_lora_settx (am_zsl42x_lora_dev_t *p_dev,
                          uint32_t              data)
{
    uint8_t tx_buff[3] = {0, 0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_TX;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (data >> 16) & 0xff;
    tx_buff[1] = (data >> 8) & 0xff;
    tx_buff[2] = (data >> 0) & 0xff;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             3);
}

int am_zsl42x_lora_setrx (am_zsl42x_lora_dev_t *p_dev,
                          uint32_t              data)
{
    uint8_t tx_buff[3] = {0, 0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_RX;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (data >> 16) & 0xff;
    tx_buff[1] = (data >> 8) & 0xff;
    tx_buff[2] = (data >> 0) & 0xff;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             3);
}

int am_zsl42x_lora_stoptimeronpreamble (am_zsl42x_lora_dev_t *p_dev,
                                        uint8_t               type)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_STOPTIMERONPREAMBLE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &type,
                                             1);
}

int am_zsl42x_lora_setrxdutycycle (am_zsl42x_lora_dev_t  *p_dev,
                                   uint32_t               rx_period,
                                   uint32_t               sleep_period)
{
    uint8_t tx_buff[6] = {0, 0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_RXDUTYCYCLE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (rx_period >> 16) & 0xff;
    tx_buff[1] = (rx_period >> 8) & 0xff;
    tx_buff[2] = (rx_period >> 0) & 0xff;

    tx_buff[3] = (sleep_period >> 16) & 0xff;
    tx_buff[4] = (sleep_period >> 8) & 0xff;
    tx_buff[5] = (sleep_period >> 0) & 0xff;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             6);
}

int am_zsl42x_lora_setcad (am_zsl42x_lora_dev_t *p_dev)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_CAD;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             NULL,
                                             0);
}

int am_zsl42x_lora_settxcontinuouswave (am_zsl42x_lora_dev_t *p_dev)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_TXCONTINUOUSWAVE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             NULL,
                                             0);
}

int am_zsl42x_lora_settxinfinitepreamble (am_zsl42x_lora_dev_t *p_dev)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_TXINFINITEPREAMBLE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             NULL,
                                             0);
}

int am_zsl42x_lora_setregulatormode (am_zsl42x_lora_dev_t *p_dev,
                                     uint8_t               type)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_REGULATORMODE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &type,
                                             1);
}

int am_zsl42x_lora_setcalibratefunction (am_zsl42x_lora_dev_t *p_dev,
                                         uint8_t               flag)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_CALIBRATE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &flag,
                                             1);
}

int am_zsl42x_lora_calibrateimage (am_zsl42x_lora_dev_t             *p_dev,
                                   am_zsl42x_lora_calibrate_image_t  type)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_CALIBRATEIMAGE;

    uint8_t tx_buff[2] = {0, 0};

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    switch(type) {
    case AM_ZSL42X_LORA_CALIBRATE_IMAGE_430MHz_440MHz :
        tx_buff[0] = 0x6B;
        tx_buff[1] = 0x6F;
        break;

    case AM_ZSL42X_LORA_CALIBRATE_IMAGE_470MHz_510MHz :
        tx_buff[0] = 0x75;
        tx_buff[1] = 0x81;
        break;

    case AM_ZSL42X_LORA_CALIBRATE_IMAGE_779MHz_787MHz :
        tx_buff[0] = 0xC1;
        tx_buff[1] = 0xC5;
        break;

    case AM_ZSL42X_LORA_CALIBRATE_IMAGE_863MHz_870MHz :
        tx_buff[0] = 0xD7;
        tx_buff[1] = 0xDB;
        break;

    default:
        tx_buff[0] = 0xE1;
        tx_buff[1] = 0xE9;
        break;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             2);
}

int am_zsl42x_lora_setpaconfig (am_zsl42x_lora_dev_t *p_dev,
                                uint8_t               paDutyCycle,
                                uint8_t               hpMax,
                                uint8_t               devieSel,
                                uint8_t               paLut)
{
    uint8_t tx_buff[4] = {0, 0, 0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_PACONFIG;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = paDutyCycle;
    tx_buff[1] = hpMax;
    tx_buff[2] = devieSel;
    tx_buff[3] = paLut;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             4);
}

int am_zsl42x_lora_setrxtxfallbackmode (am_zsl42x_lora_dev_t *p_dev,
                                        uint8_t               type)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_RXTXFALLBACKMODE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &type,
                                             1);
}

int am_zsl42x_lora_write_register (am_zsl42x_lora_dev_t     *p_dev,
                                   am_zsl42x_lora_regaddr_t  reg_addr,
                                   uint8_t                  *p_data,
                                   uint32_t                  len)
{
    uint8_t addr_buff[3] = {0};

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    addr_buff[0] = AM_ZSL42X_LORA_OPCODE_REGISTER_WRITE;
    addr_buff[1] = (reg_addr >> 8) & 0xff;
    addr_buff[2] = (reg_addr >> 0) & 0xff;

    return p_dev->p_drv_funcs->pfn_buf_write(p_dev,
                                             &addr_buff[0],
                                             3,
                                             p_data,
                                             len);
}

int am_zsl42x_lora_read_register (am_zsl42x_lora_dev_t     *p_dev,
                                  am_zsl42x_lora_regaddr_t  reg_addr,
                                  uint8_t                  *p_data,
                                  uint32_t                  len)
{
    uint8_t addr_buff[4] = {0};

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    addr_buff[0] = AM_ZSL42X_LORA_OPCODE_REGISTER_READ;
    addr_buff[1] = (reg_addr >> 8) & 0xff;
    addr_buff[2] = (reg_addr >> 0) & 0xff;
    addr_buff[3] = 0;

    return p_dev->p_drv_funcs->pfn_buf_read(p_dev,
                                            &addr_buff[0],
                                            4,
                                            p_data,
                                            len);
}

int am_zsl42x_lora_write_buffer (am_zsl42x_lora_dev_t  *p_dev,
                                 uint8_t                offset,
                                 uint8_t               *p_data,
                                 uint32_t               len)
{
    uint8_t addr_buff[2] = {0};

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    addr_buff[0] = AM_ZSL42X_LORA_OPCODE_BUFFER_WRITE;
    addr_buff[1] = offset;

    return p_dev->p_drv_funcs->pfn_buf_write(p_dev,
                                             &addr_buff[0],
                                             2,
                                             p_data,
                                             len);
}

int am_zsl42x_lora_setdioirqparams (am_zsl42x_lora_dev_t *p_dev,
                                    uint16_t              irq_mask,
                                    uint16_t              dio1_mask,
                                    uint16_t              dio2_mask,
                                    uint16_t              dio3_mask)
{
    uint8_t tx_buff[8] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_DIOIRQ_PARAMS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (irq_mask >> 8) & 0xff;
    tx_buff[1] = (irq_mask >> 0) & 0xff;

    tx_buff[2] = (dio1_mask >> 8) & 0xff;
    tx_buff[3] = (dio1_mask >> 0) & 0xff;

    tx_buff[4] = (dio2_mask >> 8) & 0xff;
    tx_buff[5] = (dio2_mask >> 0) & 0xff;

    tx_buff[6] = (dio3_mask >> 8) & 0xff;
    tx_buff[7] = (dio3_mask >> 0) & 0xff;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             8);
}

int am_zsl42x_lora_clearirqstatus (am_zsl42x_lora_dev_t *p_dev,
                                   uint16_t              clear_mask)
{
    uint8_t tx_buff[2] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_CLEAR_IRQSTATUS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (clear_mask >> 8) & 0xff;
    tx_buff[1] = (clear_mask >> 0) & 0xff;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             2);
}

int am_zsl42x_lora_setdio2rfswitch (am_zsl42x_lora_dev_t *p_dev,
                                    uint8_t               enable)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_DIO2_RFSWITCH;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &enable,
                                             1);
}

int am_zsl42x_lora_setdio3tcxoctrl (am_zsl42x_lora_dev_t          *p_dev,
                                    am_zsl42x_lora_dio3tcxoctrl_t  voltage,
                                    uint32_t                       data)
{
    uint8_t tx_buff[4] = {0, 0, 0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_DIO3_TCXOCTRL;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (uint8_t)voltage;
    tx_buff[1] = (data >> 16) & 0xff;
    tx_buff[2] = (data >> 8) & 0xff;
    tx_buff[3] = (data >> 0) & 0xff;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             4);
}

int am_zsl42x_lora_setrffrequency (am_zsl42x_lora_dev_t *p_dev,
                                   uint32_t              rffreq,
                                   uint32_t              fxtal)
{
    uint8_t  tx_buff[4] = {0, 0, 0, 0};
    uint32_t data = 0;
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_RF_FREQUENCY;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    data = ((float)rffreq / (float)fxtal) * pow(2, 25);

    tx_buff[0] = (data >> 24) & 0xFF;
    tx_buff[1] = (data >> 16) & 0xFF;
    tx_buff[2] = (data >> 8) & 0xFF;
    tx_buff[3] = (data >> 0) & 0xFF;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             4);
}

int am_zsl42x_lora_setpackettype (am_zsl42x_lora_dev_t *p_dev,
                                  uint8_t               type)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_PACKET_TYPE;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &type,
                                             1);
}

int am_zsl42x_lora_getpackettype (am_zsl42x_lora_dev_t *p_dev,
                                  uint8_t              *p_type)
{
    uint8_t addr_buff[2] = {0, 0};

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    addr_buff[0] = (uint8_t)AM_ZSL42X_LORA_OPCODE_GET_PACKET_TYPE;
    addr_buff[1] = 0;

    return p_dev->p_drv_funcs->pfn_buf_read((void *)p_dev,
                                            &addr_buff[0],
                                            2,
                                            p_type,
                                            1);
}

int am_zsl42x_lora_settxparam (am_zsl42x_lora_dev_t     *p_dev,
                               int8_t                    power,
                               am_zsl42x_lora_ramptime_t ramptime )
{
    uint8_t          data  = 0;
    int8_t           tx_buff[2] = {0, 0};

    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_TX_PARAMS;

    if (p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    if (p_dev->p_devinfo->core_type == AM_ZSL42X_LORA_CORE_TYPE_SX1261) {

        if (power == 15) {
            am_zsl42x_lora_setpaconfig(p_dev, 0x06, 0x00, 0x01, 0x01);
        } else {
            am_zsl42x_lora_setpaconfig(p_dev, 0x04, 0x00, 0x01, 0x01);
        }

        if( power >= 14 ) {
            power = 14;
        } else if( power < -17 ) {
            power = -17;
        }

        data = 0x18;

        am_zsl42x_lora_write_register(p_dev,
                                      AM_ZSL42X_LORA_REGADDR_OCP_CONFIG,
                                      &data,
                                      1);

    } else if ((p_dev->p_devinfo->core_type == AM_ZSL42X_LORA_CORE_TYPE_SX1262) ||
               (p_dev->p_devinfo->core_type == AM_ZSL42X_LORA_CORE_TYPE_SX1268)) {

        am_zsl42x_lora_read_register(p_dev,
                                     AM_ZSL42X_LORA_REGADDR_TX_CLAMPCONFIG,
                                     &data,
                                     1);
        data |= (0x0F << 1);
        am_zsl42x_lora_write_register(p_dev,
                                      AM_ZSL42X_LORA_REGADDR_TX_CLAMPCONFIG,
                                      &data,
                                      1);

        am_zsl42x_lora_setpaconfig(p_dev, 0x04, 0x07, 0x00, 0x01);

        if (power > 22) {
            power = 22;
        } else if(power < -9 ) {
            power = -9;
        }

        data = 0x38;

        am_zsl42x_lora_write_register(p_dev,
                                      AM_ZSL42X_LORA_REGADDR_OCP_CONFIG,
                                      &data,
                                      1);
    }

    tx_buff[0] = (int8_t)power;
    tx_buff[1] = (int8_t)ramptime;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             (uint8_t *)&tx_buff[0],
                                             2);
}

int am_zsl42x_lora_setmodulationparams_fsk (
                                   am_zsl42x_lora_dev_t       *p_dev,
                                   uint32_t                    bitrate,
                                   am_zsl42x_lora_fsk_filter_t filter_type,
                                   uint8_t                     bandwidth,
                                   uint32_t                    freq_deviation,
                                   uint32_t                    fxtal)
{
    uint8_t  tx_buff[8] = {0};
    uint32_t data       = 0;
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_MODULATION_PARAMS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    /* 根据目标比特率,计算获取BR */
    data = (uint32_t)((float)(32 * fxtal) / (float)bitrate);
    tx_buff[0] = (data >> 16) & 0xFF;
    tx_buff[1] = (data >> 8) & 0xFF;
    tx_buff[2] = (data >> 0) & 0xFF;

    tx_buff[3] = (uint8_t)filter_type;
    tx_buff[4] = bandwidth;

    /* 根据目标频率偏移，计算获取Fdev */
    data = ((float)(pow(2, 25) * freq_deviation) / (float)fxtal);
    tx_buff[5] = (data >> 16) & 0xFF;
    tx_buff[6] = (data >> 8) & 0xFF;
    tx_buff[7] = (data >> 0) & 0xFF;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             8);
}

int am_zsl42x_lora_setmodulationparams_lora (
                                    am_zsl42x_lora_dev_t *p_dev,
                                    am_zsl42x_lora_sf_t   sf_type,
                                    am_zsl42x_lora_bw_t   bw_type,
                                    am_zsl42x_lora_cr_t   cr_type,
                                    am_bool_t             lowdatarate_enable)
{
    uint8_t  tx_buff[8] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_MODULATION_PARAMS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (uint8_t)sf_type;
    tx_buff[1] = (uint8_t)bw_type;
    tx_buff[2] = (uint8_t)cr_type;
    tx_buff[3] = (uint8_t)lowdatarate_enable;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             4);
}

int am_zsl42x_lora_setpacketparams_fsk (
                       am_zsl42x_lora_dev_t                *p_dev,
                       uint16_t                             preamble_length,
                       am_zsl42x_lora_fsk_detector_length_t detector_length,
                       uint8_t                              syncword_number,
                       am_zsl42x_lora_fsk_addr_filter_t     addr_filter,
                       am_zsl42x_lora_fsk_packetsize_info_t sizeinfo_add,
                       uint8_t                              payloadlength,
                       am_zsl42x_lora_fsk_crc_type_t        crc_type,
                       am_bool_t                            whiteng_enable)
{
    uint8_t  tx_buff[9] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_PACKET_PARAMS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    if(syncword_number > 8) {
        syncword_number = 8;
    }

    tx_buff[0] = (preamble_length >> 8) & 0xFF;
    tx_buff[1] = (preamble_length >> 0) & 0xFF;
    tx_buff[2] = (uint8_t)detector_length;
    tx_buff[3] = syncword_number;
    tx_buff[4] = (uint8_t)addr_filter;
    tx_buff[5] = (uint8_t)sizeinfo_add;
    tx_buff[6] = payloadlength;
    tx_buff[7] = (uint8_t)crc_type;
    tx_buff[8] = (uint8_t)whiteng_enable;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             9);
}

int am_zsl42x_lora_setpacketparams_lora (
                           am_zsl42x_lora_dev_t         *p_dev,
                           uint16_t                      preamble_length,
                           am_zsl42x_lora_header_type_t  header_type,
                           uint8_t                       payloadlength,
                           am_zsl42x_lora_crc_type_t     crc_type,
                           am_zsl42x_lora_iq_type_t      iq_type)
{
    uint8_t  tx_buff[6] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_PACKET_PARAMS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (preamble_length >> 8) & 0xFF;
    tx_buff[1] = (preamble_length >> 0) & 0xFF;
    tx_buff[2] = (uint8_t)header_type;
    tx_buff[3] = payloadlength;
    tx_buff[4] = (uint8_t)crc_type;
    tx_buff[5] = (uint8_t)iq_type;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             6);
}

int am_zsl42x_lora_setcadparams_lora (
                             am_zsl42x_lora_dev_t          *p_dev,
                             am_zsl42x_lora_cad_symbolnum_t symbolnum,
                             uint8_t                        detpeak,
                             uint8_t                        detmin,
                             am_zsl42x_lora_cad_extimode_t  extimode,
                             uint32_t                       data)
{
    uint8_t  tx_buff[7] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_CAD_PARAMS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = (uint8_t)symbolnum;
    tx_buff[1] = detpeak;
    tx_buff[2] = detmin;
    tx_buff[3] = (uint8_t)extimode;
    tx_buff[4] = (data >> 16) & 0xFF;
    tx_buff[5] = (data >> 8) & 0xFF;
    tx_buff[6] = (data >> 0) & 0xFF;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             7);
}

int am_zsl42x_lora_setbufferbaseaddress (am_zsl42x_lora_dev_t     *p_dev,
                                         uint8_t          tx_baseaddress,
                                         uint8_t          rx_baseaddress)
{
    uint8_t  tx_buff[2] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_BUFFER_BASEADDRESS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    tx_buff[0] = tx_baseaddress;
    tx_buff[1] = rx_baseaddress;

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             2);
}

int am_zsl42x_lora_setsymbolnumtimeout_lora (am_zsl42x_lora_dev_t *p_dev,
                                             uint8_t               symbolnum)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_SET_LORA_SYMBNUM_TIMEOUT;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &symbolnum,
                                             1);
}

int am_zsl42x_lora_getstatus (am_zsl42x_lora_dev_t *p_dev,
                              uint8_t              *p_status)
{
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_GET_STATUS;

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_read((void *)p_dev,
                                            &opcode,
                                            1,
                                            p_status,
                                            1);
}

int am_zsl42x_lora_getrxbufferstatus (am_zsl42x_lora_dev_t          *p_dev,
                                      uint8_t      *p_rxstartbufferpointer,
                                      uint8_t             *p_payloadlength)
{
    uint8_t rx_buff[3] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_GET_RXBUFFER_STATUS;

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    if(p_dev->p_drv_funcs->pfn_buf_read(
       (void *)p_dev, &opcode, 1, &rx_buff[0], 3) == 0) {

        *p_payloadlength        = rx_buff[1];
        *p_rxstartbufferpointer = rx_buff[2];

        return 0;
    } else {
        return 1;
    }
}

int am_zsl42x_lora_getpacketstatus (am_zsl42x_lora_dev_t      *p_dev,
                                    am_radio_packet_status_t  *p_pktstate)
{
    uint8_t rx_buff[4] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_GET_PACKET_STATUS;

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    if(p_dev->p_drv_funcs->pfn_buf_read(
       (void *)p_dev, &opcode, 1, &rx_buff[0], 4) != 0) {
        return 1;
    }

    /* 减少读取次数 */
//    am_zsl42x_lora_getpackettype(p_dev, &p_pktstate->packettype);

    if ( p_dev->setconfig.packet_type == AM_ZSL42X_LORA_MODEM_LORA) {
        p_pktstate->packettype = AM_ZSL42X_LORA_MODEM_LORA;
        p_pktstate->params.lora.rssipkt = -rx_buff[1] >> 1;
        p_pktstate->params.lora.snrpkt = (((int8_t)rx_buff[2]) + 2) >> 2;
        p_pktstate->params.lora.signalrssipkt = -rx_buff[3] >> 1;
        p_pktstate->params.lora.freqerror = 0;

    } else if ( p_dev->setconfig.packet_type == AM_ZSL42X_LORA_MODEM_FSK) {
        p_pktstate->packettype = AM_ZSL42X_LORA_MODEM_FSK;
        p_pktstate->params.fsk.rxstatus = rx_buff[1];
        p_pktstate->params.fsk.rssisync = -rx_buff[2] >> 1;
        p_pktstate->params.fsk.rssiavg  = -rx_buff[3] >> 1;
        p_pktstate->params.fsk.freqerror = 0;
    } else {
        p_pktstate->packettype = 15; /* ERROR */
        am_zsl42x_lora_memset1((uint8_t *)p_pktstate,
                               0,
                               sizeof(am_radio_packet_status_t));
    }

    return 0;
}

int am_zsl42x_lora_getrssiinst (am_zsl42x_lora_dev_t *p_dev,
                                int8_t               *p_rssiinst)
{
    uint8_t rx_buff[2] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_GET_RSSI_INST;

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    if(p_dev->p_drv_funcs->pfn_buf_read(
       (void *)p_dev, &opcode, 1, &rx_buff[0], 4) == 0) {

        *p_rssiinst = rx_buff[1];
        return 0;
    } else {
        return 1;
    }
}

int am_zsl42x_lora_getstats (am_zsl42x_lora_dev_t *p_dev,
                             uint8_t              *nbpkt_received,
                             uint8_t              *nbpkt_crcerror,
                             uint8_t              *nbpkt_length_header_err)
{
    uint8_t rx_buff[4] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_GET_STATS;

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    if(p_dev->p_drv_funcs->pfn_buf_read(
       (void *)p_dev, &opcode, 1, &rx_buff[0], 4) == 0) {

        *nbpkt_received          = rx_buff[1] / 2;
        *nbpkt_crcerror          = rx_buff[2] / 4;
        *nbpkt_length_header_err = rx_buff[3] / 2;
        return 0;
    } else {
        return 1;
    }
}

int am_zsl42x_lora_resetstats (am_zsl42x_lora_dev_t *p_dev)
{
    uint8_t tx_buff[6] = {0, 0, 0, 0, 0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_RESET_STATS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             6);
}

int am_zsl42x_lora_getdeviceerrors (am_zsl42x_lora_dev_t *p_dev,
                                    uint16_t             *p_status)
{
    uint8_t rx_buff[3] = {0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_GET_DEVICE_ERRORS;

    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }

    if(p_dev->p_drv_funcs->pfn_buf_read(
       (void *)p_dev, &opcode, 1, &rx_buff[0], 3) == 0) {

        *p_status = (rx_buff[1] << 8) | rx_buff[2];
        return 0;
    } else {
        return 1;
    }
}

int am_zsl42x_lora_cleardeviceerrors (am_zsl42x_lora_dev_t *p_dev)
{
    uint8_t tx_buff[2] = {0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_CLEAR_DEVICE_ERRORS;

    if(p_dev->p_drv_funcs->pfn_buf_write == NULL) {
        return 1;
    }

    return p_dev->p_drv_funcs->pfn_buf_write((void *)p_dev,
                                             &opcode,
                                             1,
                                             &tx_buff[0],
                                             2);
}

int am_zsl42x_lora_getirqstatus (am_zsl42x_lora_dev_t *p_dev,
                                 uint16_t             *p_status)
{
    uint8_t rx_buff[3] = {0, 0, 0};
    uint8_t opcode = AM_ZSL42X_LORA_OPCODE_GET_IRQSTATUS;
    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }
    if(p_dev->p_drv_funcs->pfn_buf_read(
       (void *)p_dev, &opcode, 1, rx_buff, 3) == 0) {
        *p_status = (rx_buff[1] << 8) | (rx_buff[2] << 0);
        return 0;
    } else {
        return 1;
    }
}

int am_zsl42x_lora_read_buffer (am_zsl42x_lora_dev_t  *p_dev,
                                uint8_t                offset,
                                uint32_t               len,
                                uint8_t               *p_data)
{
    uint8_t addr_buff[3] = {0};
    if(p_dev->p_drv_funcs->pfn_buf_read == NULL) {
        return 1;
    }
    addr_buff[0] = AM_ZSL42X_LORA_OPCODE_BUFFER_READ;
    addr_buff[1] = offset;
    addr_buff[2] = 0;
    return p_dev->p_drv_funcs->pfn_buf_read(p_dev,
                                            &addr_buff[0],
                                            3,
                                            p_data,
                                            len);
}

/* end of file */
