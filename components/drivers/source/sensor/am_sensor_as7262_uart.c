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
 * \brief 传感器 AS7262 (UART模式) 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as7262_uart.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_int.h"

/**< 调试信息输出 */
#define __AS7262_DEBUG                              (0)

/* \brief   命令接收解析状态机  */
#define __AS7262_CMD_PROC_STAT_NONE                 0   /* 无命令流程       */
#define __AS7262_CMD_PROC_STAT_START                1   /* 命令流程启动     */
#define __AS7262_CMD_PROC_STAT_SEND                 2   /* 命令发送         */
#define __AS7262_CMD_PROC_STAT_DATA                 3   /* 接收到数据       */
#define __AS7262_CMD_PROC_STAT_RECV_O               4   /* 接收应答 "O"     */
#define __AS7262_CMD_PROC_STAT_RECV_E               5   /* 接收应答 "E"     */
#define __AS7262_CMD_PROC_STAT_RECV_ER              6   /* 接收应答 "ER"    */
#define __AS7262_CMD_PROC_STAT_RECV_ERR_1           7   /* 接收应答 "ERR"   */
#define __AS7262_CMD_PROC_STAT_RECV_ERRO            8   /* 接收应答 "ERRO"  */
#define __AS7262_CMD_PROC_STAT_RECV_END             9   /* 命令接收完成     */
#define __AS7262_CMD_PROC_STAT_RECV_OVERFLOW        10  /* 接收命令溢出     */
#define __AS7262_CMD_PROC_STAT_RECV_ERR             11  /* 接收命令错误     */

/* \brief 参数类型标识 */
#define __AS7262_ARG_TYPE_UINT32                    1   /* 传入参数为uint32_t 类型  */
#define __AS7262_ARG_TYPE_STRING                    2   /* 传入参数为char * 类型    */
#define __AS7262_ARG_TYPE_FLOAT                     3   /* 传入参数为float 类型     */
#define __AS7262_ARG_TYPE_RAW_DATA                  4   /* 传入参数为uint16_t * 类型 */
#define __AS7262_ARG_TYPE_CAL_DATA                  5   /* 传入参数为float * 类型 */

/** \brief 将两个uint8转换为一个uint16_t类型 */
#define __AS7262_UINT8_TO_UINT16(buff)              ((uint16_t)((*buff) << 8 | (*(buff + 1))))

/* The max data value with a given length */
am_local const int __g_max_val[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000
};

/*******************************************************************************
 * 本地函数声明
 ******************************************************************************/
/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id);

/** \brief 获取传感器通道采样数据 */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf);

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result);

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result);

/** \brief 配置传感器通道属性 */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val);

/** \brief 获取传感器通道属性 */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val);

/** \brief 设置触发，一个通道仅能设置一个触发回调函数 */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg);

/** \brief 打开触发 */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id);

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id);

/** \brief 传感器标准服务 */
am_local am_const struct am_sensor_drv_funcs __g_sensor_as7262_uart_funcs = {
        __pfn_type_get,
        __pfn_data_get,
        __pfn_enable,
        __pfn_disable,
        __pfn_attr_set,
        __pfn_attr_get,
        __pfn_trigger_cfg,
        __pfn_trigger_on,
        __pfn_trigger_off
};

/*******************************************************************************
  发送接收相关函数声明
*******************************************************************************/

/** 
 * \brief AS7262 命令发送
 */
am_local int __as7262_cmd_send (am_sensor_as7262_uart_dev_t *p_this,
                                const char                  *p_hdstr,
                                int                          arg_type,
                                int                          max_len,
                                void                        *p_arg);

/** 
 * \brief AS7262 发送字符串
 */
am_local int __as7262_cmd_str_send (am_sensor_as7262_uart_dev_t *p_this,
                                    const char                  *p_str);

/** 
 * \brief AS7262 发送整数
 */
am_local int __as7262_cmd_uint_send (am_sensor_as7262_uart_dev_t   *p_this,
                                     unsigned int                   data,
                                     int                            max_len);

/** 
 * \brief AS7262 发送字节
 */
am_local int __as7262_cmd_char_send (am_sensor_as7262_uart_dev_t    *p_this,
                                     char                            ch);

/** 
 * \brief AS7262 数据发送
 */
am_local int __as7262_data_send (am_sensor_as7262_uart_dev_t    *p_this,
                                 const uint8_t                  *p_buf,
                                 int                             len);

/** 
 * \brief AS7262 发送回调函数
 */
am_local int __as7262_uart_txchar_get (void *p_arg, char *p_outchar);

/** 
 * \brief AS7262 发送结果 (等待发送完成)
 */
am_local int __as7262_cmd_result (am_sensor_as7262_uart_dev_t  *p_this,
                                  int                           arg_type,
                                  void                         *p_arg,
                                  am_bool_t                     need_check);

/** 
 * \brief AS7262 接收回调函数
 */
am_local int __as7262_uart_rxchar_put (void *p_arg, char inchar);

/** 
 * \brief AS7262 主机发送完成后,从机回复
 */
am_local int __as7262_cmd_ack_recv_proc (am_sensor_as7262_uart_dev_t *p_this, char inchar);

/** 
 * \brief AS7262 发送结果信息获取 (包含校验)
 */
am_local int __as7262_cmd_result_info_get (am_sensor_as7262_uart_dev_t  *p_this,
                                           int                           arg_type,
                                           void                         *p_arg,
                                           am_bool_t                     need_check);

/*******************************************************************************/

am_local am_err_t __as7262_uart_ioctl (void                      *p_drv,
                                       int                        cmd,
                                       void                      *p_arg)
{
    am_err_t ret = -AM_EIO;

    am_sensor_as7262_uart_dev_t *p_dev = (am_sensor_as7262_uart_dev_t  *)p_drv;

    switch (cmd) {
    /* 设置ITIME */
    case AM_SENSOR_AS7262_ITIME_SET:
        ret = __as7262_cmd_send(p_dev,
                                "INTTIME=",
                                __AS7262_ARG_TYPE_STRING,
                                0,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 获取ITIME */
    case AM_SENSOR_AS7262_ITIME_GET:
        ret = __as7262_cmd_send(p_dev,
                                "INTTIME",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 设置GAIN */
    case AM_SENSOR_AS7262_GAIN_SET:
        ret = __as7262_cmd_send(p_dev,
                                "GAIN=",
                                __AS7262_ARG_TYPE_UINT32,
                                1,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 获取GAIN */
    case AM_SENSOR_AS7262_GAIN_GET:
        ret = __as7262_cmd_send(p_dev,
                                "GAIN",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 获取DEVICE TEMP */
    case AM_SENSOR_AS7262_DEVICE_TEMP_GET:
        ret = __as7262_cmd_send(p_dev,
                                "TEMP",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 设置BANK */
    case AM_SENSOR_AS7262_BANK_SET:
        ret = __as7262_cmd_send(p_dev,
                                "TCSMD=",
                                __AS7262_ARG_TYPE_UINT32,
                                1,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 获取BANK */
    case AM_SENSOR_AS7262_BANK_GET:
        ret = __as7262_cmd_send(p_dev,
                                "TCSMD",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 设置LED_IND */
    case AM_SENSOR_AS7262_LED_IND_SET:
        ret = __as7262_cmd_send(p_dev,
                                "LED0=",
                                __AS7262_ARG_TYPE_UINT32,
                                1,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 获取LED_IND */
    case AM_SENSOR_AS7262_LED_IND_GET:
        ret = __as7262_cmd_send(p_dev,
                                "LED0",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 设置LED_DRV */
    case AM_SENSOR_AS7262_LED_DRV_SET:
        ret = __as7262_cmd_send(p_dev,
                                "LED1=",
                                __AS7262_ARG_TYPE_UINT32,
                                1,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 获取LED_DRV */
    case AM_SENSOR_AS7262_LED_DRV_GET:
        ret = __as7262_cmd_send(p_dev,
                                "LED1",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 设置ICL_IND */
    case AM_SENSOR_AS7262_ICL_IND_SET:
        ret = __as7262_cmd_send(p_dev,
                                "LEDC=",
                                __AS7262_ARG_TYPE_STRING,
                                0,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 设置ICL_DRV */
    case AM_SENSOR_AS7262_ICL_DRV_SET:
        ret = __as7262_cmd_send(p_dev,
                                "LEDC=",
                                __AS7262_ARG_TYPE_STRING,
                                0,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 获取ICL_IND */
    case AM_SENSOR_AS7262_ICL_IND_GET:
        ret = __as7262_cmd_send(p_dev,
                                "LEDC",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 获取ICL_DRV */
    case AM_SENSOR_AS7262_ICL_DRV_GET:
        ret = __as7262_cmd_send(p_dev,
                                "LEDC",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_UINT32,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* 软件复位 */
    case AM_SENSOR_AS7262_SW_RST:
        ret = __as7262_cmd_send(p_dev,
                                "RST",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        break;

    /* 获取HW VERSION */
    case AM_SENSOR_AS7262_HW_VERSION_GET:
        ret = __as7262_cmd_send(p_dev,
                                "VERHW",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_STRING,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* ATDATA */
    case AM_SENSOR_AS7262_ATDATA:
        ret = __as7262_cmd_send(p_dev,
                                "DATA",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_RAW_DATA,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* ATCDATA */
    case AM_SENSOR_AS7262_ATCDATA:
        ret = __as7262_cmd_send(p_dev,
                                "CDATA",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_CAL_DATA,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* ATBURST */
    case AM_SENSOR_AS7262_ATBURST:
        ret = __as7262_cmd_send(p_dev,
                                "BURST=",
                                __AS7262_ARG_TYPE_UINT32,
                                1,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* NOP */
    case AM_SENSOR_AS7262_AT:
        ret = __as7262_cmd_send(p_dev,
                                NULL,
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* 返回系统软件版本号 */
    case AM_SENSOR_AS7262_ATVERSW:
        ret = __as7262_cmd_send(p_dev,
                                "VERSW",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  __AS7262_ARG_TYPE_STRING,
                                  p_arg,
                                  AM_FALSE);
        break;

    /* ATFWU */
    case AM_SENSOR_AS7262_ATFWU:
        ret = __as7262_cmd_send(p_dev,
                                "FWU=",
                                __AS7262_ARG_TYPE_STRING,
                                0,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* ATFW */
    case AM_SENSOR_AS7262_ATFW:
        ret = __as7262_cmd_send(p_dev,
                                "FW=",
                                __AS7262_ARG_TYPE_STRING,
                                0,
                                p_arg);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* ATFWA */
    case AM_SENSOR_AS7262_ATFWA:
        ret = __as7262_cmd_send(p_dev,
                                "FWA",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    /* ATFWS */
    case AM_SENSOR_AS7262_ATFWS:
        ret = __as7262_cmd_send(p_dev,
                                "FWS",
                                0,
                                0,
                                NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __as7262_cmd_result(p_dev,
                                  0,
                                  NULL,
                                  AM_FALSE);
        break;

    case AM_SENSOR_AS7262_NREAD:
        *(int *)p_arg = am_rngbuf_nbytes(&(p_dev->rx_rngbuf));
        ret = AM_OK;
        break;

    case AM_SENSOR_AS7262_NWRITE:
        *(int *)p_arg = am_rngbuf_nbytes(&(p_dev->tx_rngbuf));
        ret = AM_OK;
        break;

    case AM_SENSOR_AS7262_FLUSH :
    {
        int key = am_int_cpu_lock();

        am_rngbuf_flush(&(p_dev->rx_rngbuf));
        am_rngbuf_flush(&(p_dev->tx_rngbuf));
        am_int_cpu_unlock(key);
        ret = AM_OK;
        break;
    }

    case AM_SENSOR_AS7262_WFLUSH :
    {
        int key = am_int_cpu_lock();

        am_rngbuf_flush(&(p_dev->tx_rngbuf));
        am_int_cpu_unlock(key);
        ret = AM_OK;
        break;
    }

    case AM_SENSOR_AS7262_RFLUSH :
    {
        int key = am_int_cpu_lock();

        am_rngbuf_flush(&(p_dev->rx_rngbuf));
        am_int_cpu_unlock(key);
        ret = AM_OK;
        break;
    }

    case AM_SENSOR_AS7262_TIMEOUT:
        p_dev->timeout_ms = (uint32_t)p_arg;
        ret = AM_OK;
        break;

    default:
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/*******************************************************************************/

/** \brief AS7262 复位 */
am_local am_err_t __as7262_rst(am_sensor_as7262_uart_dev_t  *p_this)
{
    am_err_t ret = AM_OK;

    /* AS7262硬件复位 */
    if (p_this->sensor_uart_dev_info->p_devinfo->rst_pin != -1) {
        am_gpio_set(p_this->sensor_uart_dev_info->p_devinfo->rst_pin, AM_GPIO_LEVEL_LOW);
        am_mdelay(150);
        am_gpio_set(p_this->sensor_uart_dev_info->p_devinfo->rst_pin, AM_GPIO_LEVEL_HIGH);
    }

    /* AS7262软件复位 */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_SW_RST, NULL);
    am_mdelay(10);

    return ret;
}

/** \brief AS7262 参数初始化 */
am_local am_err_t __as7262_param_init(am_sensor_as7262_uart_dev_t   *p_this)
{
    as7262_param_config_t *p_param_config = (as7262_param_config_t *)(p_this->isa.dev_info->p_param_default);

    am_err_t    ret = AM_OK;

    uint8_t     regval = 0;
    char        temp[10] = {0};

    /* GAIN */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_GAIN_SET, (void *)(uint32_t)(p_param_config->gain_val));
    if (ret != AM_OK) {
        return ret;
    }

    /* INTEGRATION_TIME */
    sprintf((char *)temp, "%d", p_param_config->itime_val);
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_ITIME_SET, (void *)temp);
    if (ret != AM_OK) {
        return ret;
    }

    /* LED_IND */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_LED_IND_SET, (void *)(uint32_t)(p_param_config->led_ind));
    if (ret != AM_OK) {
        return ret;
    }

    /* LED_DRV */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_LED_DRV_SET, (void *)(uint32_t)(p_param_config->led_drv));
    if (ret != AM_OK) {
        return ret;
    }

    /* ICL_IND、ICL_DRV (LED_IND和LED_DRV的电流设置发相同的命令，一个字节中不同的位域) */
    regval = ((p_param_config->icl_drv) << 4 ) | (p_param_config->icl_ind);
    sprintf((char *)temp, "%d", regval);
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_ICL_IND_SET, (void *)temp);
    if (ret != AM_OK) {
        return ret;
    }

    return ret;
}

/** \brief AS7262 校准 */
am_local am_err_t __as7262_calibration(am_sensor_as7262_uart_dev_t  *p_this)
{
    am_err_t ret    = AM_OK;

    

    return ret;
}

/**
 * \brief AS7262 中断使能
 */
am_local void __as7262_int_enable(am_sensor_as7262_uart_dev_t   *p_this)
{

}

/**
 * \brief AS7262 中断禁能
 */
am_local void __as7262_int_disable(am_sensor_as7262_uart_dev_t  *p_this)
{

}

/**
 * \brief AS7262 获取结果
 */
am_local am_err_t __as7262_result_get(am_sensor_as7262_uart_dev_t   *p_this,
                                      as7262_result_info_t          *p_result)
{
    am_err_t ret = AM_OK;

    uint32_t temperature = 0;

    uint16_t i = 0;
    uint16_t rawdata[6] = {0};
    float    caldata[6];

    /* 原始数据 */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_ATDATA, (void *)rawdata);
    if (ret != AM_OK) {
        return ret;
    }

    /* 校准数据 */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_ATCDATA, (void *)caldata);
    if (ret != AM_OK) {
        return ret;
    }

    for (i = 0; i < 6; i++) {
        p_result->raw_data[i] = rawdata[i];
        p_result->cal_data[i] = caldata[i];
        p_result->cal_data[i] = p_this->isa.cal_val[0].cal[i];  /* 此处将校准值清零，不使用校准值 */
    }

    /* 校准后的数据值 */
    p_result->channel_v = p_result->raw_data[0] - p_result->cal_data[0];
    p_result->channel_b = p_result->raw_data[1] - p_result->cal_data[1];
    p_result->channel_g = p_result->raw_data[2] - p_result->cal_data[2];
    p_result->channel_y = p_result->raw_data[3] - p_result->cal_data[3];
    p_result->channel_o = p_result->raw_data[4] - p_result->cal_data[4];
    p_result->channel_r = p_result->raw_data[5] - p_result->cal_data[5];

    /* 温度 */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_DEVICE_TEMP_GET, (void *)&temperature);
    if (ret != AM_OK) {
        return ret;
    }
    p_result->device_temp = (int8_t)temperature;
//    #if __AS7262_DEBUG
        am_kprintf("\r\nDevice temperature = %d ℃ \r\n", p_result->device_temp);
//    #endif

    return ret;
}

/*******************************************************************************/

/** \breif 中断回调函数 */
am_local void __as7262_alarm_callback (void *p_arg)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_arg;

    am_isr_defer_job_add(&(p_this->isa.g_myjob));   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_arg;

    uint8_t i = 0;

    uint8_t bank_mode = 0;

    as7262_result_info_t result;

    /* 获取数据 */
    __as7262_result_get(p_this, &result);

    p_this->isa.data[0].val = result.channel_v;
    p_this->isa.data[0].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[1].val = result.channel_b;
    p_this->isa.data[1].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[2].val = result.channel_g;
    p_this->isa.data[2].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[3].val = result.channel_y;
    p_this->isa.data[3].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[4].val = result.channel_o;
    p_this->isa.data[4].unit = AM_SENSOR_UNIT_BASE;

    p_this->isa.data[5].val = result.channel_r;
    p_this->isa.data[5].unit = AM_SENSOR_UNIT_BASE;

    for (i = 0; i < 6; i++) {
        if (p_this->isa.pfn_trigger_cb[i] &&
                (p_this->isa.flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->isa.pfn_trigger_cb[i](p_this->isa.p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);
        }
    }

    /* 获取 BANK_MODE */
    __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_BANK_GET, &bank_mode);
    /* 若 BANK_MODE 为单次触发模式 */
    if (bank_mode == AM_AS7262_BANK_MODE_3) {
        __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_BANK_SET, (void *)AM_AS7262_BANK_MODE_3);
    }
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_AS7262_CHAN_1 || id == AM_AS7262_CHAN_2 || 
        id == AM_AS7262_CHAN_3 || id == AM_AS7262_CHAN_4 || 
        id == AM_AS7262_CHAN_5 || id == AM_AS7262_CHAN_6 ) {
        return AM_SENSOR_TYPE_SPECTRAL;
    } else {
        return -AM_ENODEV;
    }
}

/** \brief 获取传感器通道采样数据 */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    as7262_result_info_t result;

    int cur_id = 0;
    int i = 0;

    uint8_t bank_mode = 0;

    uint16_t times = 0xff;       /* 读取数据时间 */

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (AM_BIT_GET(p_this->isa.trigger, 15) != 1) {
        return -AM_EPERM;
    }

    for (i = 0; i < num; i++) {
        p_buf[i].unit = AM_SENSOR_UNIT_INVALID;
    }

    /* 若为1，则可能在数据准备就绪触发回调函数中使用 */
    if (num == 1) {
        cur_id = p_ids[0];
        /* 若打开数据准备就绪触发方式，则直接赋值 */
        if ((AM_BIT_GET(p_this->isa.trigger, 6)) &&
                ((p_this->isa.flags[0] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[1] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[2] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[3] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[4] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->isa.flags[5] & AM_SENSOR_TRIGGER_DATA_READY))) {
            p_buf[0].val = p_this->isa.data[cur_id].val;
            p_buf[0].unit= p_this->isa.data[cur_id].unit;
            return AM_OK;
        }
    }

    /** \brief 读取结果数据 */
    do {
        ret = __as7262_result_get(p_this, &result);
        if (times != 0) {
            times--;
        }
    } while ((ret != AM_OK) && (times != 0));

    if (ret != AM_OK) {
        return ret;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            p_buf[i].val  = result.channel_v;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 1) {

            p_buf[i].val  = result.channel_b;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 2) {

            p_buf[i].val  = result.channel_g;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 3) {

            p_buf[i].val  = result.channel_y;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 4) {

            p_buf[i].val  = result.channel_o;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else if (cur_id == 5) {

            p_buf[i].val  = result.channel_r;
            p_buf[i].unit = AM_SENSOR_UNIT_BASE;

        } else {
            return -AM_ENODEV;  /*< \brief 若此次通道不属于该传感器，直接返回 */
        }
    }

    /* 获取 BANK_MODE */
    __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_BANK_GET, &bank_mode);
    /* 若 BANK_MODE 为单次触发模式 */
    if (bank_mode == AM_AS7262_BANK_MODE_3) {
        __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_BANK_SET, (void *)AM_AS7262_BANK_MODE_3);
    }

    return ret;
}

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_drv;
    as7262_param_config_t *p_param_config = (as7262_param_config_t *)(p_this->isa.dev_info->p_param_default);

    int i = 0;
    int cur_id = 0;

    am_err_t ret = -AM_ENODEV;
    am_err_t curent_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || 
            cur_id == 3 || cur_id == 4 || cur_id == 5) {
            if (p_result != NULL) {
                p_result[i].val = AM_OK;
            }

            AM_BIT_SET(p_this->isa.trigger, 14 - cur_id);

            ret = AM_OK;
        } else {
            if (p_result != NULL) {
                p_result[i].val = -AM_ENODEV;
            }
            curent_ret = -AM_ENODEV;
        }
    }

    if (ret != AM_OK) {     /*< \breif 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->isa.trigger, 15);
    }    

    /* AS7262复位 */
    ret = __as7262_rst(p_this);
    if (ret != AM_OK) {
        am_kprintf("\r\nSensor AS7262 RESET ERROR! \r\n");
        return ret;
    }

    /* 寄存器参数初始化 */
    ret = __as7262_param_init(p_this);
    if (ret != AM_OK) {
        return ret;
    }

    /* 获取校准数据 */
    ret = __as7262_calibration(p_this);     //未添加校准功能xxxxxxxxxxxx
    if (ret != AM_OK) {
        return ret;
    }

    /* 使能模式 */
    ret = __as7262_uart_ioctl(p_this, AM_SENSOR_AS7262_BANK_SET,
                                (void *)(uint32_t)(p_param_config->bank_mode));
    if (ret != AM_OK) {
        return ret;
    }

    return curent_ret;
}

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || 
            cur_id == 3 || cur_id == 4 || cur_id == 5) {

            AM_BIT_CLR(p_this->isa.trigger, 14 - cur_id);
            ret = AM_OK;

        } else {
            ret = -AM_ENODEV;
        }

        if (p_result != NULL) {
            p_result[i].val = ret;
        }
    }

    if ((AM_BIT_GET(p_this->isa.trigger, 14) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 13) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 12) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 11) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 10) == 0)
            && (AM_BIT_GET(p_this->isa.trigger, 9) == 0)) {

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->isa.trigger, 15);
        }
    }

    return cur_ret;
}

/** \brief 配置传感器通道属性 */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val)
{
    return -AM_ENOTSUP;
}

/** \brief 获取传感器通道属性 */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val)
{
    return -AM_ENOTSUP;
}

/** \brief 设置触发，一个通道仅能设置一个触发回调函数 */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_drv;

    if (id != 0 && id != 1 && id != 2 && 
        id != 3 && id != 4 && id != 5) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (flags) {
    case AM_SENSOR_TRIGGER_DATA_READY:       /* 数据准备就绪触发 */
        break;
    default:
        return -AM_ENOTSUP;
    }

    p_this->isa.pfn_trigger_cb[id] = pfn_cb;
    p_this->isa.p_arg[id]          = p_arg;
    p_this->isa.flags[id]          = flags;

    return AM_OK;
}

/** \brief 打开触发 */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0 && id != 1 && id != 2 && 
        id != 3 && id != 4 && id != 5) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->isa.trigger & 0x40) {
        return AM_OK;
    }

    if (p_this->isa.dev_info->trigger_pin != -1) {
        ret = am_gpio_trigger_on(p_this->isa.dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }

    /* 使能中断 */
    __as7262_int_enable(p_this);

    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->isa.trigger, 0);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 1) {
        AM_BIT_SET(p_this->isa.trigger, 1);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 2) {
        AM_BIT_SET(p_this->isa.trigger, 2);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 3) {
        AM_BIT_SET(p_this->isa.trigger, 3);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 4) {
        AM_BIT_SET(p_this->isa.trigger, 4);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }
    if (cur_ret == AM_OK && id == 5) {
        AM_BIT_SET(p_this->isa.trigger, 5);
        AM_BIT_SET(p_this->isa.trigger, 6);
    }

    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_as7262_uart_dev_t* p_this = (am_sensor_as7262_uart_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (id != 0 && id != 1 && id != 2 && 
        id != 3 && id != 4 && id != 5) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_this->isa.pfn_trigger_cb[id] = NULL;
    p_this->isa.p_arg[id]          = NULL;
    p_this->isa.flags[id]          = 0;

    AM_BIT_CLR(p_this->isa.trigger, id);

    if ((p_this->isa.trigger & 0x3F) == 0) {

        if ((p_this->isa.dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->isa.dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        if (cur_ret == AM_OK) {

            /* 禁能中断 */
            __as7262_int_disable(p_this);

            AM_BIT_CLR(p_this->isa.trigger, 6);
        }
    }

    return cur_ret;
}

/*******************************************************************************
  发送接收相关函数声明
*******************************************************************************/

/** 
 * \brief AS7262 命令发送
 */
am_local int __as7262_cmd_send (am_sensor_as7262_uart_dev_t *p_this,
                                const char                  *p_hdstr,
                                int                          arg_type,
                                int                          max_len,
                                void                        *p_arg)
{
    p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_START;

    /* The tx buffer temp for receive cmd ack, only used after send complete */
    p_this->p_cmd_rxbuf   = p_this->sensor_uart_dev_info->p_uart_txbuf;
    p_this->cmd_rxbuf_len = p_this->sensor_uart_dev_info->txbuf_size;
    p_this->cmd_rx_len    = 0;

    /* command always start with "AT"*/
    __as7262_cmd_str_send(p_this, "AT");

    if (p_hdstr) {
        __as7262_cmd_str_send(p_this, p_hdstr);
    }

    if (arg_type == __AS7262_ARG_TYPE_UINT32) {

        __as7262_cmd_uint_send(p_this, (uint32_t)p_arg, max_len);

    } else if (arg_type == __AS7262_ARG_TYPE_STRING) {

        if (p_arg) {
            __as7262_cmd_str_send(p_this, (char *)p_arg);
        }

    }

    /* command always end with '\n' */
    __as7262_cmd_str_send(p_this, "\n");

    return AM_OK;
}


/** 
 * \brief AS7262 发送字符串
 */
am_local int __as7262_cmd_str_send (am_sensor_as7262_uart_dev_t *p_this,
                                    const char                  *p_str)
{
    return __as7262_data_send(p_this, (const uint8_t *)(p_str), strlen(p_str));
}

/** 
 * \brief AS7262 发送整数
 */
am_local int __as7262_cmd_uint_send (am_sensor_as7262_uart_dev_t   *p_this,
                                      unsigned int                  data,
                                      int                           max_len)
{
    char c;
    int  first_none_zero = 0;  /* whether appear first none zero data */

    data = data % __g_max_val[max_len];

    while (max_len != 0) {

        max_len--;

        c = data / __g_max_val[max_len] + '0';

        if (c != '0') {

            first_none_zero = 1;
            __as7262_cmd_char_send(p_this, c);

        } else if (first_none_zero || (max_len == 0)){

            /* first none zero data is appear */
            __as7262_cmd_char_send(p_this, c);
        }

        data = data % __g_max_val[max_len];
    };

    return 0;
}

/** 
 * \brief AS7262 发送字节
 */
am_local int __as7262_cmd_char_send (am_sensor_as7262_uart_dev_t    *p_this,
                                     char                            ch)
{
    return __as7262_data_send(p_this, (const uint8_t *)(&ch), 1);
}

/** 
 * \brief AS7262 数据发送
 */
am_local int __as7262_data_send (am_sensor_as7262_uart_dev_t    *p_this,
                                 const uint8_t                  *p_buf,
                                 int                             len)
{
    am_rngbuf_t rb     = &(p_this->tx_rngbuf);
    uint32_t    nbytes = len;

    uint32_t rb_ct, write_ct;

    if (am_rngbuf_isfull(rb) == AM_TRUE) {      //环形缓冲区是否填满
        return -AM_ENOSPC;                      //设备剩余空间不足
    }

    rb_ct = am_rngbuf_freebytes(rb);            //获取环形缓冲区空闲空间大小
    write_ct = (rb_ct > len) ? len : rb_ct;

    am_rngbuf_put(rb, (const char *)p_buf, write_ct);//存放若干字节到环形缓冲区

    p_buf += write_ct;
    len   -= write_ct;

    am_uart_tx_startup(p_this->uart_handle);    //启动UART中断模式数据传输

    return nbytes - len;
}

/** 
 * \brief AS7262 发送回调函数
 */
am_local int __as7262_uart_txchar_get (void *p_arg, char *p_outchar)
{
    am_sensor_as7262_uart_dev_t  *p_dev =   (am_sensor_as7262_uart_dev_t *)p_arg;
    am_rngbuf_t     rb          =   &(p_dev->tx_rngbuf);
    uint32_t        key         =   am_int_cpu_lock();

    /* Success pop one data from ring buffer */
    if (am_rngbuf_getchar(rb, p_outchar) == 1) {    //从环形缓冲区取出一个字节数据
        am_int_cpu_unlock(key);     //1:数据成功取出    0:数据取出失败，缓冲区为空
        return AM_OK;
    }

    if (p_dev->cmd_proc_state == __AS7262_CMD_PROC_STAT_START) {

        p_dev->cmd_proc_state = __AS7262_CMD_PROC_STAT_SEND;

        /* send complete  */
        am_wait_done(&p_dev->ack_wait);
    }

    am_int_cpu_unlock(key);

    return -AM_EEMPTY;     /* No data to transmit,return -AM_EEMPTY */
}

/** 
 * \brief AS7262 发送结果 (等待发送完成)
 */
am_local int __as7262_cmd_result (am_sensor_as7262_uart_dev_t  *p_this,
                                  int                           arg_type,
                                  void                         *p_arg,
                                  am_bool_t                     need_check)
{
    int          key;

    /* 等待发送完成 */
    am_wait_on(&p_this->ack_wait);

    /* 一帧数据要在100ms内接收完毕，否则判断为帧数据接收超时 */
    am_softimer_start(&p_this->timer, 100);

    am_rngbuf_init(&(p_this->tx_rngbuf),
                    (char *)p_this->sensor_uart_dev_info->p_uart_txbuf,
                    p_this->sensor_uart_dev_info->txbuf_size);

    while (1) {

        /* wait 200ms */
        if (am_wait_on_timeout(&p_this->ack_wait, 200) == AM_OK) {

            key = am_int_cpu_lock();

            /* success process */
            if (p_this->cmd_proc_state == __AS7262_CMD_PROC_STAT_RECV_END) {

                p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_NONE;
                am_int_cpu_unlock(key);

                return __as7262_cmd_result_info_get(p_this,
                                                    arg_type,
                                                    p_arg,
                                                    need_check);
            }

            if (p_this->cmd_proc_state != __AS7262_CMD_PROC_STAT_RECV_ERR) {

                am_int_cpu_unlock(key);
                continue;
            }

            am_int_cpu_unlock(key);
        }

        /* run to here, some error happen */
        key = am_int_cpu_lock();

        /* move the data to rx rngbuf */
        if (p_this->cmd_rx_len != 0) {
            am_rngbuf_put(&(p_this->rx_rngbuf),
                          (const char *)(p_this->p_cmd_rxbuf),
                           p_this->cmd_rx_len);

            am_wait_done(&(p_this->rx_wait));
        }

        p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_NONE;

        am_int_cpu_unlock(key);

        return AM_ERROR;
    }
}

/** 
 * \brief AS7262 接收回调函数
 */
am_local int __as7262_uart_rxchar_put (void *p_arg, char inchar)
{
    am_sensor_as7262_uart_dev_t *p_dev  = (am_sensor_as7262_uart_dev_t *)p_arg;
    am_rngbuf_t     rx_rb  = &(p_dev->rx_rngbuf);

    if (p_dev->cmd_proc_state >= __AS7262_CMD_PROC_STAT_SEND) {
        /* success process by cmd ack */
        if (__as7262_cmd_ack_recv_proc(p_dev, inchar) == AM_OK) {
             return AM_OK;
        }
    }

    /* wait done */
    am_wait_done(&p_dev->rx_wait);      //结束之前开启的接收数据的等待

    if (am_rngbuf_putchar(rx_rb, inchar) == 1) {        /* 存放一个字节到环形缓冲区 */
        return AM_OK;                                   //1:数据成功存放    0:数据存放失败，缓冲区满
    }

    return -AM_EFULL;          /* No data to receive,return -AM_EFULL */
}

/** 
 * \brief AS7262 主机发送完成后,从机回复
 */
am_local int __as7262_cmd_ack_recv_proc (am_sensor_as7262_uart_dev_t *p_this, char inchar)
{
    int ret = AM_ERROR;

    switch (p_this->cmd_proc_state) {

    case __AS7262_CMD_PROC_STAT_SEND:  /* Cmd send complete       */
        memset(p_this->p_cmd_rxbuf, 0 , p_this->cmd_rxbuf_len);
        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        if (inchar == 'O') {
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_O;     //接收到 'O'
        } else if (inchar == 'E') {
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_E;     //接收到 'E'
        } else {
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_DATA;       //接收到第一个字节(既不是 'O' 也不是 'E')
        }
        break;

    case __AS7262_CMD_PROC_STAT_DATA:
        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        if (inchar == 'O') {
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_O;     //接收到 'O'
        } else if (inchar == 'E') {
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_E;     //接收到 'E'
        }
        break;

    case __AS7262_CMD_PROC_STAT_RECV_O:
        if (inchar == 'K') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_END;
            am_softimer_stop(&p_this->timer);       //关闭接收超时检测定时器
        }
        break;

    case __AS7262_CMD_PROC_STAT_RECV_E:
        if (inchar == 'R') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_ER;
        }
        break;

    case __AS7262_CMD_PROC_STAT_RECV_ER:
        if (inchar == 'R') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_ERR_1;
        }
        break;

    case __AS7262_CMD_PROC_STAT_RECV_ERR_1:
        if (inchar == 'O') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_ERRO;
        }
        break;

    case __AS7262_CMD_PROC_STAT_RECV_ERRO:
        if (inchar == 'R') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_END;
            am_softimer_stop(&p_this->timer);       //关闭接收超时检测定时器
        }
        break;

    default:
        ret = -AM_ESRCH;
        p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_ERR;       //接收命令错误
        break;
    }

    if (ret != AM_OK) {

        if (p_this->cmd_rx_len != 0) {  /* not empty, error happen */
            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_ERR;   //接收命令错误
        }
    } else {

        /* full */
        if ((p_this->cmd_rx_len     == p_this->cmd_rxbuf_len) &&
            (p_this->cmd_proc_state != __AS7262_CMD_PROC_STAT_RECV_END)) {

            p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_ERR;   //接收命令错误
        }
    }

    /* receive one ack data */
    am_wait_done(&p_this->ack_wait);

    return ret;
}

/** 
 * \brief AS7262 发送结果信息获取 (包含校验)
 */
/*
 *  回复帧结尾为  : 0x20 0x4F 0x4B
 *  即ASCII表中的 : ' '  'O'  'K' 
**/
am_local int __as7262_cmd_result_info_get (am_sensor_as7262_uart_dev_t  *p_this,
                                           int                           arg_type,
                                           void                         *p_arg,
                                           am_bool_t                     need_check)
{
    char *p_ack_str = (char *)(p_this->p_cmd_rxbuf);
    char *p_start   = NULL;
    char *p_end     = NULL;
    char *p         = NULL;

    char *delim = ", ";     /* delim是分隔符的集合，使用 ',' 和 ' ' 作为分隔符 */ 
    char  data_str[50] = {0};

    uint8_t i = 0;

    p_this->p_cmd_rxbuf[p_this->cmd_rx_len] = '\0';

    AM_DBG_INFO("\r\nack is %d bytes : \r\n%s",
                p_this->cmd_rx_len,
                p_this->p_cmd_rxbuf);

    /* error happen */
    if (strstr(p_ack_str,"ERROR") != NULL) {
        return AM_AS7262_PARAMETER_INVALID;
    } else if (strstr(p_ack_str, "OK") == NULL) {
        return AM_AS7262_RECEIVE_INVALID;
    } else {

        if (p_arg == NULL) {    /* 设置类指令直接返回 "OK" */
            if (strstr(p_ack_str, "OK") != NULL) {
                return AM_OK;
            } else {
                return AM_ERROR;
            }
        }

        /* Get the data for p_arg */
        p_end = strstr(p_ack_str, " OK");    /* 返回参数后接上 " OK" */

        if (p_end == NULL) {
            return AM_ERROR;
        } else {
            *p_end = '\0';      /* 去掉最后 " OK" */
        }

        p_start = p_ack_str;

        if (arg_type == __AS7262_ARG_TYPE_UINT32) {

            *(uint32_t *)p_arg = strtoul(p_start, &p_end, 10);

        } else if (arg_type == __AS7262_ARG_TYPE_STRING) {

            strcpy(p_arg, p_start);

        } else if (arg_type == __AS7262_ARG_TYPE_RAW_DATA) {

            uint16_t *p_raw_data = (uint16_t *)p_arg;

            /* 复制接收到的字符串 */
            strcpy(data_str, p_start);

            /* 第一次调用 strtok 时，第一个参数传入要分割的字符串 */
            p = strtok(data_str, delim);

            while (p != NULL) {

                if (i >= 6) {
                    break;
                }

                p_raw_data[i] = strtoul(p, NULL, 10);

                i++;

                /* 后面再次调用 strtok 时，第一个参数传入 NULL */
                p = strtok(NULL, delim);
            }

        } else if (arg_type == __AS7262_ARG_TYPE_CAL_DATA) {

            float *p_cal_data = (float *)p_arg;

            /* 复制接收到的字符串 */
            strcpy(data_str, p_start);

            /* 第一次调用 strtok 时，第一个参数传入要分割的字符串 */
            p = strtok(data_str, delim);

            while (p != NULL) {

                if (i >= 6) {
                    break;
                }

                p_cal_data[i] = (float)atof(p);

                i++;

                /* 后面再次调用 strtok 时，第一个参数传入 NULL */
                p = strtok(NULL, delim);
            }

        }
    }

    return AM_OK;
}

/** 
 * \brief AS7262 接收超时回调函数
 */
am_local void __as7262_timeout_cb (void *p_arg)
{
    am_sensor_as7262_uart_dev_t  *p_this = (am_sensor_as7262_uart_dev_t *)p_arg;
    int              key    = am_int_cpu_lock();

    am_softimer_stop(&(p_this->timer));         //关闭接收超时检测定时器
    p_this->cmd_proc_state = __AS7262_CMD_PROC_STAT_RECV_END;
    am_wait_done(&(p_this->ack_wait));
    am_int_cpu_unlock(key);
}

/*******************************************************************************
  Public functions
*******************************************************************************/

/**
 * \brief 传感器 AS7262 (UART模式) 初始化
 */
am_sensor_handle_t am_sensor_as7262_uart_init (
        am_sensor_as7262_uart_dev_t             *p_sensor_uart_dev,
        const am_sensor_as7262_uart_devinfo_t   *p_sensor_uart_devinfo)
{
    uint8_t i = 0;

    if ((p_sensor_uart_dev                   == NULL) ||
        (p_sensor_uart_devinfo               == NULL) ||
        (p_sensor_uart_devinfo->p_uart_rxbuf == NULL) ||
        (p_sensor_uart_devinfo->p_uart_txbuf == NULL)) {
        return NULL;
    }

    p_sensor_uart_dev->uart_handle = p_sensor_uart_devinfo->pfn_uart_handle_get();
    p_sensor_uart_dev->isa.pfn_ioctl = __as7262_uart_ioctl;

    p_sensor_uart_dev->isa.as7262_dev.p_drv     = p_sensor_uart_dev;
    p_sensor_uart_dev->isa.as7262_dev.p_funcs   = &__g_sensor_as7262_uart_funcs;
    p_sensor_uart_dev->sensor_uart_dev_info     = p_sensor_uart_devinfo;
    p_sensor_uart_dev->isa.dev_info             = p_sensor_uart_devinfo->p_devinfo;
    p_sensor_uart_dev->isa.trigger              = 0;
    for (i = 0; i < 6; i++) {
        p_sensor_uart_dev->isa.pfn_trigger_cb[i]  = NULL;
        p_sensor_uart_dev->isa.p_arg[i]           = NULL;
        p_sensor_uart_dev->isa.flags[i]           = 0;
        p_sensor_uart_dev->isa.data[i].val        = 0;
        p_sensor_uart_dev->isa.data[i].unit       = AM_SENSOR_UNIT_INVALID;
    }

    p_sensor_uart_dev->cmd_proc_state = __AS7262_CMD_PROC_STAT_NONE;
    p_sensor_uart_dev->timeout_ms = (uint32_t)AM_WAIT_FOREVER;
    am_wait_init(&(p_sensor_uart_dev->ack_wait));
    am_wait_init(&(p_sensor_uart_dev->rx_wait));

    if (p_sensor_uart_devinfo->p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_sensor_uart_devinfo->p_devinfo->trigger_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        am_gpio_trigger_connect(p_sensor_uart_devinfo->p_devinfo->trigger_pin,
                                __as7262_alarm_callback,
                                (void*)p_sensor_uart_dev);
        am_gpio_trigger_cfg(p_sensor_uart_devinfo->p_devinfo->trigger_pin, AM_GPIO_TRIGGER_FALL);  /* 下降沿触发 */
    }

    am_isr_defer_job_init(&(p_sensor_uart_dev->isa.g_myjob), __am_pfnvoid_t, p_sensor_uart_dev, 1);

    am_uart_ioctl(p_sensor_uart_dev->uart_handle, AM_UART_BAUD_SET, (void *)(p_sensor_uart_devinfo->baudrate));

    /* Initialize the ring-buffer */
    am_rngbuf_init(&(p_sensor_uart_dev->rx_rngbuf),
                   (char *)p_sensor_uart_devinfo->p_uart_rxbuf,
                   p_sensor_uart_devinfo->rxbuf_size);

    am_rngbuf_init(&(p_sensor_uart_dev->tx_rngbuf),
                   (char *)p_sensor_uart_devinfo->p_uart_txbuf,
                   p_sensor_uart_devinfo->txbuf_size);

    am_softimer_init(&(p_sensor_uart_dev->timer), __as7262_timeout_cb , (void *)p_sensor_uart_dev);

    am_uart_ioctl(p_sensor_uart_dev->uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);

    am_uart_callback_set(p_sensor_uart_dev->uart_handle,
                         AM_UART_CALLBACK_TXCHAR_GET,
        (int (*)(void *))__as7262_uart_txchar_get,
                         (void *)(p_sensor_uart_dev));

    am_uart_callback_set(p_sensor_uart_dev->uart_handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
        (int (*)(void *))__as7262_uart_rxchar_put,
                         (void *)(p_sensor_uart_dev));

    return &(p_sensor_uart_dev->isa.as7262_dev);
}

/**
 * \brief 传感器 AS7262 (UART模式) 去初始化
 */
am_err_t am_sensor_as7262_uart_deinit (am_sensor_handle_t handle)
{
    am_sensor_as7262_uart_dev_t *p_dev = (am_sensor_as7262_uart_dev_t *)(handle->p_drv);

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->sensor_uart_dev_info->pfn_uart_deinit(p_dev->uart_handle);
    p_dev->uart_handle = NULL;

    p_dev->isa.as7262_dev.p_drv     = NULL;
    p_dev->isa.as7262_dev.p_funcs   = NULL;
    p_dev->isa.dev_info             = NULL;
    p_dev->sensor_uart_dev_info     = NULL;

    return AM_OK;
}

/* end of file */

