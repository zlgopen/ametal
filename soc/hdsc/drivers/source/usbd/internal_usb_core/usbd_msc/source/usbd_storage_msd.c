/******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file usbd_storage_msd.c
 **
 ** A detailed description is available at
 ** @link
        This file includes the user MSC application layer.
    @endlink
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
//#include "hc32_ddl.h"
#include "usbd_storage_msd.h"
#include "usbd_msc_mem.h"
#include "usb_conf.h"
//#include "w25qxx.h"
//#include "sdio_sdcard.h"


#include "am_usbd_msc.h"

//extern const uint8_t __g_standard_inquiry_data[];


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/* User callback functions */
//USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
//{
//    &STORAGE_Init,
//    &STORAGE_GetCapacity,
//    &STORAGE_IsReady,
//    &STORAGE_IsWriteProtected,
//    &STORAGE_Read,
//    &STORAGE_Write,
//    &STORAGE_GetMaxLun,
//#if 0    
//    (int8_t *)STORAGE_Inquirydata,
//#endif
//    (int8_t *)__g_standard_inquiry_data,
//};

USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{
    &STORAGE_Init,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &STORAGE_GetMaxLun,
#if 0    
    (int8_t *)STORAGE_Inquirydata,
#endif
    NULL,
};


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Max. supported device number */
#define STORAGE_LUN_NBR         2u

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* Variable for Storage operation status */
volatile uint8_t USB_STATUS_REG = 0u;

/* USB Mass storage querty data (36 bytes for each lun) */
const int8_t STORAGE_Inquirydata[] =
{
    /* LUN 0 */
    0x00,
    0x80,
    0x02,
    0x02,
    (USBD_STD_INQUIRY_LENGTH - 4u),
    0x00,
    0x00,
    0x00,
    /* Vendor Identification */
    'H', 'D', 'S', 'C', ' ', 'M', 'C', 'U', ' ',    //9 bytes
    /* Product Identification */
    'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h',    //15 bytes
    ' ', 'D', 'i', 's', 'k', ' ',
    /* Product Revision Level */
    '1', '.', '0', ' ',                             //4 bytes

    /* LUN 1 */
    0x00,
    0x80,
    0x02,
    0x02,
    (USBD_STD_INQUIRY_LENGTH - 4u),
    0x00,
    0x00,
    0x00,
    /* Vendor Identification */
    'H', 'D', 'S', 'C', ' ', 'M', 'C', 'U', ' ',    //9 bytes
    /* Product Identification */
    'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ',    //15 bytes
    'D', 'i', 's', 'k', ' ', ' ',
    /* Product Revision Level */
    '1', '.', '0', ' ',                             //4 bytes
};
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/* Pointer to USBD_MICRO_SDIO_fops */
USBD_STORAGE_cb_TypeDef *USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;


//extern am_usbd_msc_t  __g_usb_msc_dev;
extern const am_usbd_msc_diskinfo_t __g_usbd_msc_disk_info;

/**
 *******************************************************************************
 ** \brief Storage initialize
 **
 ** \param lun: logic number
 **   \@arg: 0, SD card
 **   \@arg: 1, SPI flash
 **
 ** \retval Ok
 **
 ******************************************************************************/ 
int8_t STORAGE_Init(uint8_t lun)
{
#if 0
//    SD_Init();
#if 0
    W25QXX_Init();
#endif    
    __usbd_disk_mal_init(&__g_usb_msc_dev, &__g_usbd_msc_disk_info);  /* 初始化文件系统. */
#endif    
    return AM_OK;
}

/**
 *******************************************************************************
 ** \brief Get Storage capacity
 **
 ** \param lun: logic number
 **   \@arg: 0, SD card
 **   \@arg: 1, SPI flash
 ** \param block_num: sector number
 ** \param block_size: sector size
 **
 ** \retval Ok
 **
 ******************************************************************************/
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
#if 0
    if (lun == 1u)
    {
        *block_size = 512u;
//        *block_num  = SDCardInfo.CardCapacity / 512;
    }else
    {
        *block_size = 512u;
        *block_num  = 1024u * 1024u * 8u / 512u;
    }
#endif   
    *block_size = __g_usbd_msc_disk_info.block_size;
    *block_num  = __g_usbd_msc_disk_info.block_count;    
    return AM_OK;
}

/**
 *******************************************************************************
 ** \brief Check storage if ready
 **
 ** \param lun: logic number
 **   \@arg: 0, SD card
 **   \@arg: 1, SPI flash
 **
 ** \retval Ok
 **
 ******************************************************************************/
int8_t  STORAGE_IsReady(uint8_t lun)
{
    USB_STATUS_REG |= (uint8_t)0X10;
    return AM_OK;
}

/**
 *******************************************************************************
 ** \brief Check storage if write protected
 **
 ** \param lun: logic number
 **   \@arg: 0, SD card
 **   \@arg: 1, SPI flash
 **
 ** \retval Ok
 **
 ******************************************************************************/
int8_t  STORAGE_IsWriteProtected(uint8_t lun)
{
    // todo
    return AM_OK;
}

/**
 *******************************************************************************
 ** \brief Read data from storage devices
 **
 ** \param [in] lun: logic number
 **   \@arg: 0, SD card
 **   \@arg: 1, SPI flash
 ** \param [in] blk_addr: sector address
 ** \param [in] blk_len: sector count
 ** \param [out] buf: data buffer be read
 **
 ** \retval Ok: read successful
 **         Other: read fail
 **
 ******************************************************************************/
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t res = (int8_t)0;
    USB_STATUS_REG |= (uint8_t)0X02;
#if 0    
    if (lun == 1u)
    {
//        res = SD_ReadDisk(buf, blk_addr, blk_len);
        if (res)
        {
            USB_STATUS_REG |= (uint8_t)0X08;
        }
    }else
    {
        W25QXX_Read(buf, blk_addr * 512u, blk_len * 512u);
    }
#endif
//    __mal_read(&__g_usb_msc_dev, 0, 64);
    return res;
}

/**
 *******************************************************************************
 ** \brief Write data to storage devices
 **
 ** \param [in] lun: logic number
 **   \@arg: 0, SD card
 **   \@arg: 1, SPI flash
 ** \param [in] blk_addr: sector address
 ** \param [in] blk_len: sector count
 ** \param [out] buf: data buffer be written
 **
 ** \retval Ok: write successful
 **         Other: write fail
 **
 ******************************************************************************/
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t res = (int8_t)0;
    USB_STATUS_REG |= (uint8_t)0X01;
#if 0    
    if (lun == 1u)
    {
//        res = SD_WriteDisk(buf, blk_addr, blk_len);
        if (res)
        {
            USB_STATUS_REG |= (uint8_t)0X04;
        }
    }else
    {
        W25QXX_Write(buf, blk_addr * 512u, blk_len * 512u);
    }
#endif
//    __mal_write(&__g_usb_msc_dev,
//                &__g_usb_msc_dev.w_offset - 64,
//                &__g_usb_msc_dev.p_info->p_cmd_buffer,
//                64);

    return res;
}

/**
 *******************************************************************************
 ** \brief Get supported logic number
 **
 ** \param None
 **
 ** \retval STORAGE_LUN_NBR-1
 **
 ******************************************************************************/
int8_t STORAGE_GetMaxLun(void)
{
#if 0
    return (int8_t)(STORAGE_LUN_NBR - 1u);
#endif    
    return 0;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
