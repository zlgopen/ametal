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
 * \brief 
 * 
 * 已知兼容芯片： 
 *   - MB85RS64
 * 
 * \internal
 * \par Modification history
 * - 1.00 19-12-09  RZ, first implementation.
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mb85rsxx.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/**
 * \name FRAM的各个命令
 * @{
 */

#define  __MB85RSXX_WREN  0x06  //设置写使能锁存器
#define  __MB85RSXX_WRDI  0x04  //写禁能
#define  __MB85RSXX_RDSR  0x05  //读状态寄存器
#define  __MB85RSXX_WRSR  0x01  //写状态寄存器
#define  __MB85RSXX_READ  0x03  //读取内存数据
#define  __MB85RSXX_WRITE 0x02  //写入内存数据
/** @} */

/**
 * \name 状态寄存器的值
 * @{
 * 寄存器说明：bit7:   WPEN  写保护使能位；
 *             bit3:2  BP1:0 块保护；
 *                           00--无
 *                           01--后1/4
 *                           10--后1/2
 *                           11--全部
 *             bit1    WEL   写使能；
 */
#define  __PROTECTED_ALL        0x8E //全部地址写保护:0000h-1FFFh
#define  __PROTECTED_HALF       0x8A //后半部分写保护:1000h-1FFFh
#define  __PROTECTED_HALF_HALF  0x86 //后1/4写保护   :1800h-1FFFh
#define  __PROTECTED_NO         0x82 //无写保护
/** @} */

/*******************************************************************************
  forward declarations
*******************************************************************************/
am_local int __mb85rsxx_nvram_get (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len);

am_local int __mb85rsxx_nvram_set (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len);
/*******************************************************************************
  locals
*******************************************************************************/
                                  
am_local am_const struct am_nvram_drv_funcs  __g_mb85rsxx_nvram_drvfuncs = {
    __mb85rsxx_nvram_get,
    __mb85rsxx_nvram_set
};  

/*******************************************************************************
  本地函数
*******************************************************************************/
static int __mb85rsxx_write_enb (am_mb85rsxx_dev_t *p_dev)
{
 
    uint8_t cmd = __MB85RSXX_WREN;

    return am_spi_write_then_write(&(p_dev->spi_dev),
                                   &cmd,
                                   1,
                                   NULL,
                                   0);
}

/******************************************************************************/
/*
static int __mb85rsxx_write_dis (am_mb85rsxx_dev_t *p_dev)
{
 
    uint8_t cmd = __MB85RSXX_WRDI;

    return am_spi_write_then_write(&(p_dev->spi_dev),
                                   &cmd,
                                   1,
                                   NULL,
                                   0);
}
*/
/******************************************************************************/
static int __mb85rsxx_reg_read (am_mb85rsxx_dev_t  *p_dev,
                                uint8_t            *p_val)
{
 
    uint8_t cmd = __MB85RSXX_RDSR;
 
    return am_spi_write_then_read(&(p_dev->spi_dev),
                                   &cmd,
                                   1,
                                   p_val,
                                   1);
}

/******************************************************************************/
static int __mb85rsxx_reg_write (am_mb85rsxx_dev_t *p_dev,
                                 uint8_t            val)
{
 
    uint8_t cmd = __MB85RSXX_WRSR;
 
    return am_spi_write_then_write(&(p_dev->spi_dev),
                                   &cmd,
                                   1,
                                   &val,
                                   1);
}

/******************************************************************************/
static int __mb85rsxx_read (am_mb85rsxx_dev_t   *p_dev,
                            uint32_t             addr,
                            uint8_t             *p_buf,
                            uint32_t             len)
{
    uint8_t  cmd_buf[3];

    cmd_buf[0] = __MB85RSXX_READ;
    cmd_buf[1] = (addr >> 8 ) & 0xFF;
    cmd_buf[2] = addr & 0xFF;

    return am_spi_write_then_read(&(p_dev->spi_dev),
                                  cmd_buf,
                                  3,
                                  p_buf,
                                  len);
}

/******************************************************************************/
static int __mb85rsxx_write (am_mb85rsxx_dev_t   *p_dev,
                             uint32_t             addr,
                             uint8_t             *p_buf,
                             uint32_t             len)
{
    uint8_t  cmd_buf[3];

    cmd_buf[0] = __MB85RSXX_WRITE;
    cmd_buf[1] = (addr >> 8 ) & 0xFF;
    cmd_buf[2] = addr & 0xFF;

    return am_spi_write_then_write(&(p_dev->spi_dev),
                                   cmd_buf,
                                   3,
                                   p_buf,
                                   len);
}
/*******************************************************************************
    standard nvram driver functions
*******************************************************************************/

/* pfn_nvram_get function driver */
am_local int __mb85rsxx_nvram_get (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len)
{
    return am_mb85rsxx_read(p_drv, offset, (uint8_t *)p_buf, len);
}

/******************************************************************************/

/* pfn_nvram_set function driver */
am_local int __mb85rsxx_nvram_set (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len)
{ 
    return am_mb85rsxx_write(p_drv, offset, (uint8_t *)p_buf, len);
}

/*******************************************************************************
  公共函数
*******************************************************************************/
am_mb85rsxx_handle_t am_mb85rsxx_init(am_mb85rsxx_dev_t            *p_dev,
                                      const am_mb85rsxx_devinfo_t  *p_devinfo,
                                      am_spi_handle_t               spi_handle)
{
    if ((p_dev == NULL) || (spi_handle == NULL)) {
        return NULL;
    }

    am_gpio_pin_cfg(p_devinfo->spi_cs_pin, AM_GPIO_OUTPUT_INIT_HIGH);
    
    p_dev->p_devinfo = p_devinfo;

    am_spi_mkdev(&(p_dev->spi_dev),
                 spi_handle,
                 8,
                 p_devinfo->spi_mode,
                 p_devinfo->spi_speed,
                 p_devinfo->spi_cs_pin,
                 NULL);
 
    if (am_spi_setup(&(p_dev->spi_dev)) != AM_OK) {
        return NULL;
    }

    /* 设备地址作为 handle 返回 */
    return p_dev;
}

/******************************************************************************/
int am_mb85rsxx_read (am_mb85rsxx_handle_t  handle,
                      uint32_t              addr,
                      uint8_t              *p_buf,
                      uint32_t              len)
{
    return __mb85rsxx_read(handle, addr, p_buf, len);
}

/******************************************************************************/
int am_mb85rsxx_write (am_mb85rsxx_handle_t  handle,
                       uint32_t              addr,
                       uint8_t              *p_buf,
                       uint32_t              len)
{
    __mb85rsxx_write_enb(handle);
    return __mb85rsxx_write(handle, addr, p_buf, len);
}

/*******************************************************************************
  其它一些器件相关的接口函数
*******************************************************************************/

/* 读取状态寄存器 */
int am_mb85rsxx_status_read (am_mb85rsxx_handle_t  handle,
                             uint8_t              *p_stat)
{
    return __mb85rsxx_reg_read(handle, p_stat);
}

/******************************************************************************/
/* 写状态寄存器 */
int am_mb85rsxx_status_write (am_mb85rsxx_handle_t handle,
                              uint8_t              val)
{
    return __mb85rsxx_reg_write(handle, val);
}

/******************************************************************************/

/* provide standard nvram service for system */
int am_mb85rsxx_nvram_init (am_mb85rsxx_handle_t   handle,
                            am_nvram_dev_t        *p_dev,
                            const char            *p_dev_name)
{
    
    if ((handle == NULL) || (p_dev == NULL)) {
        return -AM_EINVAL;
    }
    
    p_dev->p_next       = NULL;
    
    /* provide the standard nvram service */
    p_dev->p_funcs    = &__g_mb85rsxx_nvram_drvfuncs;
    p_dev->p_drv      = (void *)handle;
    p_dev->p_dev_name = p_dev_name;
    
    handle->p_serv       = p_dev;
    
    return am_nvram_dev_register(p_dev);
}

/* end of file */
