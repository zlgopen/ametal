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
 * \brief I2C implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-19  vir, first implementation.
 * \endinternal
 */
#include "am_i2c_slv_subaddr.h"

/**
 * \brief 函数声明
 */
static int __i2c_slv_subaddr_addr_match(void *p_arg, am_bool_t is_rx);
static int __i2c_slv_subaddr_txbyte_get(void *p_arg, uint8_t *p_outbyte);
static int __i2c_slv_subaddr_rxbyte_put(void *p_arg, uint8_t inbyte);
static void __i2c_slv_subaddr_tran_stop(void *p_arg);
static int __i2c_slv_subaddr_gen_call(void *p_arg, uint8_t byte);

/**
 * \brief 回调函数变量
 */
static am_i2c_slv_cb_funcs_t __g_subaddr_cb_funs = {
        __i2c_slv_subaddr_addr_match,
        __i2c_slv_subaddr_txbyte_get,
        __i2c_slv_subaddr_rxbyte_put,
        __i2c_slv_subaddr_tran_stop,
        __i2c_slv_subaddr_gen_call,
};
/**
 * \brief 判断子地址的有效性
 */
static int __i2c_slv_subaddr_validator(void * p_arg)
{
    am_i2c_slv_subaddr_device_t  *p_dev = ( am_i2c_slv_subaddr_device_t *)p_arg;

    if ( p_dev->reg_mode &  AM_I2C_SLV_REG_IRREGULAR ) {
        if ( p_dev->subaddr >= p_dev->reg_num ) {
            return AM_ERROR;
        }
    } else {
        if ( p_dev->subaddr >= p_dev->reg_info->reg_len ) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

/**
 * \brief 获得子地址
 */
static int __i2c_slv_subaddr_get( void * p_arg,  uint8_t inbyte)
{
    am_i2c_slv_subaddr_device_t  *p_dev = ( am_i2c_slv_subaddr_device_t *)p_arg;

    /* 2字节的子地址 接受*/
    if ( p_dev->subaddr_dev_flags & AM_I2C_SLV_SUBADDR_2BYTE ){
        if( p_dev->subaddr_count_flag == 1) {
            /* 先将接受的两个字节放在 subaddr 中 */
            p_dev->subaddr  |=  inbyte << 8;
            p_dev->subaddr_count_flag = 0;  /* 下次接受从第0个开始接受 子地址字节数 */
            p_dev->is_subaddr = AM_FALSE;   /* 接收完子地址 */
            return AM_OK;
        } else {
            p_dev->subaddr_count_flag = 1;
            p_dev->subaddr    = inbyte;  /* 接受第一个字节子地址 */
        }
    } else {
        /* 否则默认 位一个字节的子地址 */
        p_dev->subaddr    = inbyte;
        p_dev->is_subaddr = AM_FALSE;  /* 表示已接受到子地址 */
        return AM_OK;
    }

    /* 表示没接受完 或接受错误 */
    return AM_ERROR;
}


/**
 * \brief 地址匹配时回调
 */
static int __i2c_slv_subaddr_addr_match(void *p_arg, am_bool_t is_rx)
{
    am_i2c_slv_subaddr_device_t  *p_dev = ( am_i2c_slv_subaddr_device_t *)p_arg;

    /** 执行用户设置的回调 */
    if (p_dev->p_cb_funs->pfn_addr_match ) {
        p_dev->p_cb_funs->pfn_addr_match( p_dev->p_arg, is_rx);
    }
    /* 准备这次接受或发送数据 */
    p_dev->is_subaddr = AM_TRUE;
    p_dev->data_count = 0;

    return AM_I2C_SLV_ACK;
}
/**
 * \brief 获取一个发送字节回调函数
 */
static int __i2c_slv_subaddr_txbyte_get(void *p_arg, uint8_t *p_outbyte)
{
    am_i2c_slv_subaddr_device_t  *p_dev = ( am_i2c_slv_subaddr_device_t *)p_arg;

    if ( NULL == p_dev->cur_reg_info) {
        return AM_I2C_SLV_NACK;
    }

    /*  判断是否支持自增 */
    if (p_dev->subaddr_dev_flags & AM_I2C_SLV_SUBADDR_NSELF_INCREASING ) {
        if ( p_dev->reg_mode &  AM_I2C_SLV_REG_IRREGULAR ) {
            if( p_dev->data_count >= p_dev->cur_reg_info->reg_len) {
                /* 从机不响应 ，发送数据0 */
                *p_outbyte = 0;
                return AM_I2C_SLV_NACK;
            }
        } else if (p_dev->data_count >= p_dev->reg_bit_width) {
            *p_outbyte = 0;
            return AM_I2C_SLV_NACK;
        }
    }

    /** 子地址自增处理  */
    if ( p_dev->reg_mode &  AM_I2C_SLV_REG_IRREGULAR ) {
        if ( p_dev->off_byte  >=  p_dev->cur_reg_info->reg_len ) {
            p_dev->off_byte = 0;
            p_dev->cur_reg_info = p_dev->cur_reg_info + 1;  /* 获取下一个子地址对应的寄存器信息 */

            /* 超过了寄存器的总个数 ，从0子地址开始  */
            if ( p_dev->cur_reg_info == ( p_dev->reg_info + p_dev->reg_num)) {
                p_dev->cur_reg_info =   p_dev->reg_info;
            }
        }
    } else {
        p_dev->off_byte %= p_dev->cur_reg_info->reg_len;   /* 超过从头开始 */
    }

    *p_outbyte = p_dev->cur_reg_info->p_reg_addr[ p_dev->off_byte];

    p_dev->off_byte++;         /* 字节偏移加1 */
    p_dev->is_rx = AM_FALSE;    /* 表示 发送状态 */
    p_dev->data_count++;       /* 接受的数据加1(不包括子地址数据 ) */

    return AM_I2C_SLV_ACK;
}

/**
 * \brief 接收到一个字节回调
 */
static int __i2c_slv_subaddr_rxbyte_put(void *p_arg, uint8_t inbyte)
{

    int ret ;
    am_i2c_slv_subaddr_device_t  *p_dev = ( am_i2c_slv_subaddr_device_t *)p_arg;
    uint8_t temp = 0;

    /* 获取子地址 */
    if ( p_dev->is_subaddr == AM_TRUE ) {
        ret =  __i2c_slv_subaddr_get( p_dev, inbyte);

        if ( AM_OK != ret ) {
            return ret;
        }
         /* 判断子地址的有效性 */
         ret = __i2c_slv_subaddr_validator(p_dev);
        if ( AM_OK != ret ) {
            p_dev->cur_reg_info = NULL;
            return ret;
        }
        if ( p_dev->reg_mode &  AM_I2C_SLV_REG_IRREGULAR ) {
            /* 寄存器不规则处理  */
             p_dev->cur_reg_info = p_dev->reg_info + p_dev->subaddr;
             p_dev->off_byte = 0;
        } else {
            /* 寄存器空间规则 处理 (8位，16位，32位)*/
            p_dev->cur_reg_info  = p_dev->reg_info;
            p_dev->off_byte    = inbyte * (p_dev->reg_bit_width);
        }
        return ret;
    }

    if ( NULL == p_dev->cur_reg_info) {
        return AM_I2C_SLV_NACK;
    }

    /*  判断是否支持自增 */
    if (p_dev->subaddr_dev_flags & AM_I2C_SLV_SUBADDR_NSELF_INCREASING ) {
        if ( p_dev->reg_mode &  AM_I2C_SLV_REG_IRREGULAR ) {
            if( p_dev->data_count >= p_dev->cur_reg_info->reg_len) {
                /* 从机不响应 */
                return AM_I2C_SLV_NACK;
            }
        } else if (p_dev->data_count >= p_dev->reg_bit_width) {
            return AM_I2C_SLV_NACK;
        }
    }

    /** 子地址自增处理  */
    if ( p_dev->reg_mode &  AM_I2C_SLV_REG_IRREGULAR ) {
        if ( p_dev->off_byte  >=  p_dev->cur_reg_info->reg_len ) {
            p_dev->off_byte = 0;
            p_dev->cur_reg_info = p_dev->cur_reg_info + 1;  /* 获取下一个子地址对应的寄存器信息 */

            /* 超过了寄存器的总个数 ，从0子地址开始  */
            if ( p_dev->cur_reg_info == ( p_dev->reg_info + p_dev->reg_num)) {
                p_dev->cur_reg_info =   p_dev->reg_info;
            }
        }
    } else {
        p_dev->off_byte %= p_dev->cur_reg_info->reg_len;   /* 超过从头开始 */
    }


    if ( p_dev->cur_reg_info->p_reg_wr) {
        /* 进行写保护处理 ,将接受到的数据存储起来*/
        if( p_dev->cur_reg_info->p_reg_wr [p_dev->off_byte] ) {
            temp =  p_dev->cur_reg_info->p_reg_addr[p_dev->off_byte];
            p_dev->cur_reg_info->p_reg_addr[p_dev->off_byte] = inbyte;
            p_dev->cur_reg_info->p_reg_addr[p_dev->off_byte] &= ~p_dev->cur_reg_info->p_reg_wr[p_dev->off_byte];
            p_dev->cur_reg_info->p_reg_addr[p_dev->off_byte] |= temp & p_dev->cur_reg_info->p_reg_wr[p_dev->off_byte];
        } else {
            p_dev->cur_reg_info->p_reg_addr[p_dev->off_byte] = inbyte;
        }
    } else {
        /* 若没设置读写标志，默认可读可写 */
        p_dev->cur_reg_info->p_reg_addr[p_dev->off_byte] = inbyte;
    }

    p_dev->off_byte++;          /* 字节偏移加1 */
    p_dev->is_rx = AM_TRUE;    /* 表示 处理接受状态 */
    p_dev->data_count++;       /* 接受的数据加1(不包括子地址数据 ) */

    return AM_I2C_SLV_ACK;
}


/**
 * \brief 传输停止回调
 */
static void __i2c_slv_subaddr_tran_stop(void *p_arg)
{
    am_i2c_slv_subaddr_device_t  *p_dev = ( am_i2c_slv_subaddr_device_t *)p_arg;

    if ( p_dev->p_cb_funs->pfn_tran_finish) {
        /* 传输结束回调 */
        p_dev->p_cb_funs->pfn_tran_finish( p_dev->p_arg,
                                           p_dev->is_rx,
                                           p_dev->subaddr,
                                           p_dev->data_count);
    }
    /* 接受完成 ,下次接受需要重新接受字地址 */
    p_dev->is_subaddr = AM_TRUE;
    p_dev->data_count = 0;
}

/**
 * \brief 广播回调函数
 */
static int __i2c_slv_subaddr_gen_call(void *p_arg, uint8_t byte)
{
    am_i2c_slv_subaddr_device_t  *p_dev = ( am_i2c_slv_subaddr_device_t *)p_arg;

    if ( p_dev->p_cb_funs->pfn_gen_call ) {
        p_dev->p_cb_funs->pfn_gen_call(p_dev->p_arg, byte);
    }

    return AM_I2C_SLV_ACK;
}

/**
 * \brief 生成一个含子地址的设备
 */
void am_i2c_slv_subaddr_mkdev( am_i2c_slv_subaddr_device_t   *p_dev,
                               am_i2c_slv_handle_t            handle,
                               am_i2c_slv_subaddr_cb_funcs_t *p_cb_funs,
                               void                          *p_arg,
                               uint16_t                       dev_addr,
                               uint16_t                       dev_flags,
                               am_i2c_slv_subaddr_reg_info_t *p_reg_info,
                               uint32_t                       reg_num,
                               uint8_t                        reg_mode)
{
    am_i2c_slv_device_t  *p_i2c_slv_dev = NULL;

    if ( NULL == p_dev     ||
         NULL == p_reg_info ) {
        return ;
    }

    p_i2c_slv_dev = & p_dev->i2c_slv_dev;

    /** 初始化带有子地址的设备结构体 */
    p_dev->is_subaddr         = AM_TRUE;
    p_dev->subaddr            = 0;
    p_dev->subaddr_count_flag = 0;
    p_dev->data_count         = 0;
    p_dev->off_byte           = 0;

    p_dev->subaddr_dev_flags = dev_flags;

    /** 回调函数 */
    p_dev->p_cb_funs         = p_cb_funs;
    p_dev->p_arg             = p_arg;

    /** 子地址中寄存器信息 */
    p_dev->reg_info          = p_reg_info;
    p_dev->cur_reg_info      = NULL;
    p_dev->reg_num           = reg_num;
    p_dev->reg_mode  = reg_mode;


    if ( reg_mode  & AM_I2C_SLV_REG_16_BIT) {
        /** 16位 ，两个字节 */
        p_dev->reg_bit_width = 2;
    } else if ( dev_flags & AM_I2C_SLV_REG_32_BIT ) {
        /* 32 位 ，四个字节 */
        p_dev->reg_bit_width = 4;
    } else {
        /* 8位 ，1字节 ，默认 */
        p_dev->reg_bit_width = 1;
    }

    /** 初始化 从设备结构体 */
    am_i2c_slv_mkdev (p_i2c_slv_dev,
                      handle,
                      &__g_subaddr_cb_funs ,
                      dev_addr,
                      dev_flags,
                      p_dev);

}

/* end of file */
