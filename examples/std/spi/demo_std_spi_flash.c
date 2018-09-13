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
 * \brief SPI 读写 FLASH(MX25L3206E) 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 接口和 FLASH 对应的接口连接。
 *
 * - 实验现象：
 *   1. 写数据到 FLASH；
 *   2. 读出刚才写入的数据；
 *   3. 调试串口打印测试结果。
 *
 * \note
 *   1. 可见，大多数情况下，直接使用 am_spi_write_then_read() 和
 *      am_spi_write_then_write() 函数即可。
 *
 * \par 源代码
 * \snippet demo_std_spi_flash.c src_std_spi_flash
 *
 * \internal
 * \par History
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_spi_flash
 * \copydoc demo_std_spi_flash.c
 */

/** [src_std_spi_flash] */
#include "ametal.h"
#include "am_spi.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**
 * \brief 定义 FLASH 相关命令
 */
#define WREN       0x06 /**< \brief SPI_FLASH 写使能命令 */
#define WRDI       0x04 /**< \brief SPI_FLASH 读取 ID 命令 */
#define RDSR       0x05 /**< \brief SPI_FLASH 读取状态命令 */
#define WRSR       0x01 /**< \brief SPI_FLASH 写状态命令 */
#define READ       0x03 /**< \brief SPI_FLASH 读操作命令 */
#define WRITE      0x02 /**< \brief SPI_FLASH 写操作命令 */
#define SE         0xD8 /**< \brief SPI_FLASH 擦除操作命令 */
#define CHIP_ERASE 0x60 /**< \brief SPI_FLASH 整片擦除命令 */

#define FLASH_PAGE_SIZE 256 /**< \brief SPI_FLASH 页大小定义 */

#define TEST_ADDR 0x0000          /**< \brief SPI_FLASH 测试页地址 */
#define TEST_LEN  FLASH_PAGE_SIZE /**< \brief 测试字节长度 */

static uint8_t g_tx_buf[TEST_LEN] = {0x9F}; /**< \brief SPI_FLASH 写数据缓存 */
static uint8_t g_rx_buf[TEST_LEN] = {0};    /**< \brief SPI_FLASH 读数据缓存 */

/**
 * \brief 判断 SPI_FLASH 是否处于忙状态
 */
static am_bool_t flash_isbusy_chk (am_spi_device_t *p_dev)
{
    uint8_t cmd_data = RDSR;
    uint8_t status_data;

    am_spi_write_then_read(p_dev,
                          &cmd_data,
                           1,
                          &status_data,
                           1);

    return (status_data & 0x01) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 写使能
 */
static void flash_wren (am_spi_device_t *p_dev)
{
    uint8_t tx_cmd = WREN; /* 发送的指令 */

    /* 设置传输参数，发送写使能命令 */
    am_spi_write_then_write(p_dev,
                           &tx_cmd,
                            1,
                            NULL,
                            0);
}

/**
 * \brief 擦除函数
 */
static void spi_flash_erase (am_spi_device_t *p_dev, uint32_t addr )
{
    uint8_t tx_cmd[4]; /* 发送的指令 */
    uint8_t cmd_len;   /* 指令长度 */

    flash_wren(p_dev);

    tx_cmd[0] = SE;
    tx_cmd[1] = (addr >> 16) & 0xff;
    tx_cmd[2] = (addr >>  8) & 0xff;
    tx_cmd[3] = addr & 0xff;

    cmd_len   = 4;

    am_spi_write_then_write(p_dev,
                            tx_cmd,
                            cmd_len,
                            NULL,
                            0);

    /* 等待 FLASH 处于空闲状态 */
    while (flash_isbusy_chk(p_dev) == AM_TRUE);
}

/**
 * \brief 写数据
 */
static void spi_flash_write (am_spi_device_t *p_dev,
                             uint32_t         addr,
                             uint32_t         length)
{
    uint8_t tx_cmd[4]; /* 发送的指令 */

    flash_wren(p_dev);

    /* 写入写指令和 24 位地址 */
    tx_cmd[0] = WRITE;
    tx_cmd[1] = addr >> 16;
    tx_cmd[2] = addr >>  8;
    tx_cmd[3] = addr;

    am_spi_write_then_write(p_dev,
                            tx_cmd,
                            4,
                            g_tx_buf,
                            length);

    /* 等待 FLASH 处于空闲状态 */
    while (flash_isbusy_chk(p_dev) == AM_TRUE);
}

/**
 * \brief 读数据
 */
 static void spi_flash_read (am_spi_device_t *p_dev,
                             uint32_t         addr,
                             uint32_t         length)
{
    uint8_t tx_cmd[4]; /* 发送的指令 */

    /* 写入读指令和 24 位地址 */
    tx_cmd[0] = READ;
    tx_cmd[1] = addr >> 16;
    tx_cmd[2] = addr >>  8;
    tx_cmd[3] = addr;

    am_spi_write_then_read(p_dev,
                           tx_cmd,
                           4,
                           g_rx_buf,
                           length);

    /* 等待 FLASH 处于空闲状态 */
    while (flash_isbusy_chk(p_dev) == AM_TRUE);
}

/**
 * \brief ID 检测
 */
static void flash_id_chk (am_spi_device_t *p_dev)
{
    uint8_t tx_cmd = 0x9F; /* 发送的指令 */
    uint8_t rd_id[3];

    am_spi_write_then_read(p_dev,
                          &tx_cmd,
                           1,
                           rd_id,
                           3);

    /* 等待 FLASH 处于空闲状态 */
    while (flash_isbusy_chk(p_dev) == AM_TRUE);

    AM_DBG_INFO("The ID is %x, %x ,%x\r\n", rd_id[0], rd_id[1], rd_id[2]);
}

/**
 * \brief FLASH 测试
 */
static void spi_flash_test_demo (am_spi_device_t *p_dev,
                                 uint32_t         addr,
                                 uint32_t         length)
{
    uint32_t i;

    /* 检查芯片 ID */
    flash_id_chk(p_dev);

    /* 擦除当前地址中数据 */
    spi_flash_erase(p_dev, addr);

    AM_DBG_INFO("FLASH erase ok\r\n");

    for (i = 0; i < length; i++) {
        g_tx_buf[i] = i + 1;
    }

    /* 写入数据到设定 FLASH 地址 */
    spi_flash_write(p_dev, addr, length);
    am_mdelay(10);

    AM_DBG_INFO("FLASH write ok\r\n");

    for (i = 0; i < length; i++) {
        g_rx_buf[i] = 0;
    }

    /* 从设定的 FLASH 地址中读取数据 */
    spi_flash_read(p_dev, addr, length);
    am_mdelay(10);

    /* 数据校验 */
    for (i = 0; i < length; i++) {

        /* 将读取到的数据通过串口打印出来 */
        AM_DBG_INFO(" read %2dst data is : 0x%2x \r\n", i, g_rx_buf[i]);
        if(g_rx_buf[i] != ((1 + i) & 0xFF)) {
            AM_DBG_INFO("verify failed!\r\n");
            while(1);
        }
    }
}

/**
 * \brief 例程入口
 */
void demo_std_spi_flash_entry (am_spi_handle_t spi_handle,
                               int             cs_pin,
                               uint32_t        test_addr,
                               uint32_t        test_length)
{
    am_spi_device_t g_spi_device;

    am_spi_mkdev(&g_spi_device,
                  spi_handle,
                  8,
                  AM_SPI_MODE_0,
                  1000000,
                  cs_pin,
                  NULL);

    am_spi_setup(&g_spi_device);

    AM_DBG_INFO("SPI flash test start!\r\n");

    spi_flash_test_demo(&g_spi_device, test_addr, test_length);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_spi_flash] */

/* end of file */
