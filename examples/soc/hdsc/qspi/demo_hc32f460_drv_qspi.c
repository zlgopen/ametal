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
 * \brief QSPI 读写QSPIflash例程，通过 drv 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口输出 "QSPI test ok !"；
 *
 * \par 源代码
 * \snippet demo_hc32f460_qspi.c src_hc32f460_drv_qspi
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-20  cds, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hc32f460_drv_qspi
 * \copydoc demo_hc32f460_drv_qspi.c
 */

/** [src_hc32f460_drv_qspi] */
#include "string.h"
#include "am_vdebug.h"
#include "am_hc32f460_qspi.h"
#include "hw/amhw_hc32f460_qspi.h"

/* QSPI memory bus address definition */
#define QSPI_BUS_ADDRESS                (0x98000000ul)
/* FLASH parameters definition */
#define FLASH_PAGE_SIZE                 (0x100u)
#define FLASH_SRCTOR_SIZE               (0x1000u)
#define FALSH_MAX_ADDR                  (0x800000ul)
#define FLASH_DUMMY_BYTE_VALUE          (0xffu)
#define FLASH_BUSY_BIT_MASK             (0x01u)

/* FLASH instruction definition */
#define FLASH_INSTR_WRITE_ENABLE        (0x06u)
#define FLASH_INSTR_PAGE_PROGRAM        (0x02u)
#define FLASH_INSTR_ERASE_4KB_SECTOR    (0x20u)
#define FLASH_INSTR_ERASE_CHIP          (0xC7u)
#define FLASH_INSTR_READ_SR1            (0x05u)
#define FLASH_INSTR_READ_SR2            (0x35u)
#define FLASH_INSTR_READ_SR3            (0x15u)

void __qspi_flash_write_enable(am_hc32f460_qspi_dev_t *p_dev)
{

    am_hc32f460_qspi_enter_directcom (p_dev);
    am_hc32f460_qspi_directcom_write(p_dev, FLASH_INSTR_WRITE_ENABLE);
    am_hc32f460_qspi_exit_directcom (p_dev);
}



int __qspi_wait_for_write_end(am_hc32f460_qspi_dev_t *p_dev)
{

    am_hc32f460_qspi_enter_directcom (p_dev);

    am_hc32f460_qspi_directcom_write(p_dev, FLASH_INSTR_READ_SR1);

    while((am_hc32f460_qspi_directcom_read(p_dev) & FLASH_BUSY_BIT_MASK)
          == FLASH_BUSY_BIT_MASK);

    am_hc32f460_qspi_exit_directcom (p_dev);

	return AM_OK;

}

int __qspi_erase_4kb_sector(am_hc32f460_qspi_dev_t *p_dev, uint32_t addr)
{
    if (addr > (QSPI_BUS_ADDRESS + FALSH_MAX_ADDR)) {
        return -AM_EINVAL;
    } else {

    __qspi_flash_write_enable(p_dev);

    am_hc32f460_qspi_enter_directcom (p_dev);
    am_hc32f460_qspi_directcom_write(p_dev, FLASH_INSTR_ERASE_4KB_SECTOR);
    am_hc32f460_qspi_directcom_write(p_dev, (uint8_t)((addr & 0xFF0000ul) >> 16));
    am_hc32f460_qspi_directcom_write(p_dev, (uint8_t)((addr & 0xFF00u) >> 8));
    am_hc32f460_qspi_directcom_write(p_dev, (uint8_t)((addr & 0xFFu)));
    am_hc32f460_qspi_exit_directcom (p_dev);

    /* Wait for flash idle */
    return __qspi_wait_for_write_end(p_dev);
    }

}


int __qspi_flash_write_page(am_hc32f460_qspi_dev_t *p_dev,
                            uint32_t addr,
                            const uint8_t p_data[],
                            uint16_t len)
{
    int enRet = AM_OK;
    uint16_t u16Index = 0u;

    if ((addr > FALSH_MAX_ADDR) || (NULL == p_data) || (len > FLASH_PAGE_SIZE))
    {
        enRet = -AM_EINVAL;
    }
    else
    {

        __qspi_flash_write_enable(p_dev);
        /* Send data to flash */
        am_hc32f460_qspi_enter_directcom (p_dev);
        am_hc32f460_qspi_directcom_write(p_dev, FLASH_INSTR_PAGE_PROGRAM);
        am_hc32f460_qspi_directcom_write(p_dev, (uint8_t)((addr & 0xFF0000ul) >> 16));
        am_hc32f460_qspi_directcom_write(p_dev, (uint8_t)((addr & 0xFF00u) >> 8));
        am_hc32f460_qspi_directcom_write(p_dev, (uint8_t)(addr & 0xFFu));
        while (len--)
        {
            am_hc32f460_qspi_directcom_write(p_dev, p_data[u16Index]);
            u16Index++;
        }

        am_hc32f460_qspi_exit_directcom (p_dev);
        /* Wait for flash idle */
        enRet = __qspi_wait_for_write_end(p_dev);
    }
    return enRet;
}


void demo_hc32f460_drv_qspi_entry(am_hc32f460_qspi_dev_t *p_dev)
{


    uint32_t flashaddr = 0u;
    uint16_t bufferlen = 0u;
    uint32_t pflashreadaddr;
    const char txbuffer[] = "QSPI read and write flash example: Welcome to use AMetal !";
    uint8_t rxbuffer[128];

    bufferlen = (uint16_t)sizeof(txbuffer);

    __qspi_erase_4kb_sector(p_dev, flashaddr);

    /* Write data to flash */
    __qspi_flash_write_page(p_dev,
                            flashaddr,
                            (uint8_t*)&txbuffer[0],
                            bufferlen);

    pflashreadaddr = QSPI_BUS_ADDRESS + flashaddr;

    am_hc32f460_qspi_read(p_dev, pflashreadaddr, rxbuffer, bufferlen);

    if (memcmp(txbuffer, rxbuffer, bufferlen) != 0)
    {
        am_kprintf("QSPI test not ok !");
    } else {
        am_kprintf("QSPI test ok !");
    }

}
