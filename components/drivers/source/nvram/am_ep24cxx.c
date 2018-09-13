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
 * \brief ep24cxx driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_ep24cxx.h"
#include "am_delay.h"

/*******************************************************************************
  forward declarations
*******************************************************************************/
am_local int __ep24cxx_nvram_get (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len);

am_local int __ep24cxx_nvram_set (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len);

/*******************************************************************************
  locals
*******************************************************************************/
                                  
am_local am_const struct am_nvram_drv_funcs  __g_ep24cxx_nvram_drvfuncs = {
    __ep24cxx_nvram_get,
    __ep24cxx_nvram_set
};    

/*******************************************************************************
  defines
*******************************************************************************/
 
/** \brief get type's max sizes (byte) = (2 ^ n) * 128 bytes */
#define __EP24CXX_TP_MAXSIZE_GET(type) \
    ((1 << AM_BITS_GET(type, 20, 4)) << 7)

/** \brief get type's page size (byte) */
#define __EP24CXX_TP_PGSIZE_GET(type) \
    AM_BITS_GET(type, 0, 15)

/** \brief get type's data address bits number in slave address (bit) */
#define __EP24CXX_TP_REG_BITLEN_OV_GET(type) \
    AM_BITS_GET(type, 16, 3)

/** \brief get type's register address length (byte) */
#define __EP24CXX_TP_REG_BYTELEN_GET(type) \
    (AM_BIT_ISSET(type, 19) ? 2 : 1)

/** \brief get type's write time */
#define __EP24CXX_TP_WRITE_TIME_GET(type) \
    AM_BITS_GET(type, 24, 8)

/*******************************************************************************
    local functions
*******************************************************************************/

/* program ep24cxx */
am_local int __ep24cxx_program_data (am_ep24cxx_dev_t        *p_dev,
                                     uint32_t                 subaddr,
                                     uint8_t                 *p_buf,
                                     uint32_t                 len,
                                     am_bool_t                is_read)
{
    const am_ep24cxx_devinfo_t *p_devinfo = p_dev->p_devinfo;
 
    uint32_t slave_addr    = p_devinfo->slv_addr;
    unsigned reg_bitlen_ov = __EP24CXX_TP_REG_BITLEN_OV_GET(p_devinfo->type);
    unsigned reg_bytelen   = __EP24CXX_TP_REG_BYTELEN_GET(p_devinfo->type);
    unsigned reg_bitlen    = reg_bytelen << 3;
    unsigned char twr      = __EP24CXX_TP_WRITE_TIME_GET(p_devinfo->type);

    int      ret;
    
    /*
     * calculate slave address and register address,
     * some register bits are embeded in slave address
     */
    if (reg_bitlen_ov) {
        unsigned ovbits = AM_BITS_GET(subaddr, reg_bitlen, reg_bitlen_ov);
        AM_BITS_SET(slave_addr,
                    0,             /* ov bits start bit0 in 7-bit slave addr */
                    reg_bitlen_ov,
                    ovbits);
    }

    /* re make device because of the slave address may be change */
    p_dev->i2c_dev.dev_addr = slave_addr;
 
    /* read/write eeprom */
    ret = (is_read == AM_TRUE) ?
          am_i2c_read(&(p_dev->i2c_dev),
                        subaddr,
                        p_buf,
                        len)
        : am_i2c_write(&(p_dev->i2c_dev),
                        subaddr,
                        p_buf,
                        len);
       
    if (ret != AM_OK) {
        return ret;
    }

    /* waiting for program done */
    /* todo: this delay should add a systick time ? */
    if ((is_read == AM_FALSE) && (twr)) {
        
        /* add one ms to make sure has enough time to program */
        am_mdelay(twr + 1);
    }

    return AM_OK;
}

/******************************************************************************/

/* ep24cxx read or write */
am_local int __ep24cxx_rw (am_ep24cxx_dev_t        *p_dev,
                           int                      start,
                           uint8_t                 *p_buf,
                           size_t                   len,
                           am_bool_t                is_read)
{
    const am_ep24cxx_devinfo_t *p_devinfo = p_dev->p_devinfo;

    uint32_t maxsize = __EP24CXX_TP_MAXSIZE_GET(p_devinfo->type);
    uint32_t maxaddr = maxsize - 1;
    
    uint32_t page     = __EP24CXX_TP_PGSIZE_GET(p_devinfo->type);
    uint32_t len_tmp;
    int      ret;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* start address beyond this eeprom's capacity */
    if (start > maxaddr) {
        return -AM_ENXIO;
    }

    /* no data will be read or written */
    if (len == 0) {
        return AM_OK;
    }

    /* adjust len that will not beyond eeprom's capacity */
    if ((start + len) > maxsize) {
        len = maxsize - start;
    }

    /* if page == 0 or read op. , this means there is no page limit */
    if ((is_read  == AM_TRUE) || (page == 0)) {
        page = maxsize;
    }

    /* write the unaligned data of the start */
    len_tmp =  AM_ROUND_DOWN(start + page, page) - start;
    if (len < len_tmp) {
        len_tmp = len;
    }

    /* lock the device */

    ret = AM_OK;
    ret = __ep24cxx_program_data(p_dev, start, p_buf, len_tmp, is_read);
    
    if (ret != AM_OK) {
        return -AM_EIO;
    }
    
    len   -= len_tmp;
    start += len_tmp;
    p_buf += len_tmp;

    /* write the rest data */

    while (len) {
        len_tmp = len > page ? page : len;
        ret = __ep24cxx_program_data(p_dev, start, p_buf, len_tmp, is_read);
        
        if (ret != AM_OK) {
            return -AM_EIO;
        }
        
        len   -= len_tmp;
        start += len_tmp;
        p_buf += len_tmp;
    }
 
    return ret;
}

/*******************************************************************************
    standard nvram driver functions
*******************************************************************************/

/* pfn_nvram_get function driver */
am_local int __ep24cxx_nvram_get (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len)
{
    return am_ep24cxx_read(p_drv, offset, (uint8_t *)p_buf, len);
}

/******************************************************************************/

/* pfn_nvram_set function driver */
am_local int __ep24cxx_nvram_set (void            *p_drv,
                                  int              offset,
                                  uint8_t         *p_buf,
                                  int              len)
{ 
    return am_ep24cxx_write(p_drv, offset, (uint8_t *)p_buf, len);
}

/*******************************************************************************
    public functions
*******************************************************************************/

/* ep24cxx init  */
am_ep24cxx_handle_t am_ep24cxx_init (am_ep24cxx_dev_t           *p_dev,
                                     const am_ep24cxx_devinfo_t *p_devinfo,
                                     am_i2c_handle_t             i2c_handle)
{
    unsigned reg_bytelen = 0;
    
    if ((p_dev == NULL) || (p_devinfo == NULL) || (i2c_handle == NULL)) {
        return NULL;
    }
    
    reg_bytelen = __EP24CXX_TP_REG_BYTELEN_GET(p_devinfo->type);
 
    am_i2c_mkdev(&(p_dev->i2c_dev), 
                   i2c_handle, 
                   p_devinfo->slv_addr, 
                   AM_I2C_ADDR_7BIT |  (reg_bytelen == 1 ? 
                       AM_I2C_SUBADDR_1BYTE : AM_I2C_SUBADDR_2BYTE));
 
    p_dev->p_devinfo = p_devinfo;
    p_dev->p_serv    = NULL;
 
    return p_dev;
}

/******************************************************************************/

/* ep24cxx deinit  */
int am_ep24cxx_deinit (am_ep24cxx_handle_t handle)
{
    if (handle->p_serv != NULL) {
        am_nvram_dev_unregister(handle->p_serv);
    }
    
    handle->p_serv = NULL;
    
    return AM_OK;
}

/******************************************************************************/
int am_ep24cxx_read (am_ep24cxx_handle_t  handle, 
                     int                  start_addr, 
                     uint8_t             *p_buf, 
                     int                  len)
{    
    return __ep24cxx_rw(handle,
                        start_addr,
                        p_buf,
                        len,
                        AM_TRUE);
}

/******************************************************************************/

int am_ep24cxx_write (am_ep24cxx_handle_t handle,
                      int                 start_addr, 
                      uint8_t            *p_buf, 
                      int                 len)
{
    return __ep24cxx_rw(handle,
                        start_addr,
                        p_buf,
                        len,
                        AM_FALSE);
}

/******************************************************************************/

/* provide standard nvram service for system */
int am_ep24cxx_nvram_init (am_ep24cxx_handle_t   handle,
                           am_nvram_dev_t       *p_dev,
                           const char           *p_dev_name)
{
    
    if ((handle == NULL) || (p_dev == NULL)) {
        return -AM_EINVAL;
    }
    
    p_dev->p_next       = NULL;
    
    /* provide the standard nvram service */
    p_dev->p_funcs    = &__g_ep24cxx_nvram_drvfuncs;
    p_dev->p_drv      = (void *)handle;
    p_dev->p_dev_name = p_dev_name;
    
    handle->p_serv       = p_dev;
    
    return am_nvram_dev_register(p_dev);
}

/* end of file */
