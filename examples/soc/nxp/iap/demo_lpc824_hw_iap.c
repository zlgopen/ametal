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
 * \brief IAP 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 输出 UID, PID, Boot 代码版本号和签名；
 *   2. 拷贝 RAM 到 FLASH，输出相关页数据；
 *   3. 如果蜂鸣器响，说明 IAP 指令在执行产生错误。
 *
 *       在扇区和指定扇区号可进行 IAP 和 ISP 指令操作。另外，可进行页擦除。扇区大小
 *       为 1KB，页大小为 64 字节，一个扇区包含 16 页，扇区 0 和页 0 位于 0x0000 0000
 *       地址。16KB flash 仅包含扇区 0-15，32KB flash 包含扇区 0-31.
 *
 *       扇区编号       |    扇区大小[kB]      |    页编号      |         地址范围
 *    ------------- | ---------------- | ----------- | -------------------------
 *           0      |        1         |    0 -  15  | 0x0000 0000 - 0x0000 03ff
 *           1      |        1         |   16 -  31  | 0x0000 0400 - 0x0000 07ff
 *           2      |        1         |   32 -  47  | 0x0000 0800 - 0x0000 0bff
 *           3      |        1         |   48 -  63  | 0x0000 0c00 - 0x0000 0fff
 *           4      |        1         |   64 -  79  | 0x0000 1000 - 0x0000 13ff
 *           5      |        1         |   80 -  95  | 0x0000 1400 - 0x0000 17ff
 *           6      |        1         |   96 - 111  | 0x0000 1800 - 0x0000 1bff
 *           7      |        1         |  112 - 127  | 0x0000 1c00 - 0x0000 1fff
 *           8      |        1         |  128 - 143  | 0x0000 2000 - 0x0000 23ff
 *           9      |        1         |  144 - 159  | 0x0000 2400 - 0x0000 27ff
 *          10      |        1         |  160 - 175  | 0x0000 2800 - 0x0000 2bff
 *          11      |        1         |  176 - 191  | 0x0000 2c00 - 0x0000 2fff
 *          12      |        1         |  192 - 207  | 0x0000 3000 - 0x0000 33ff
 *          13      |        1         |  208 - 223  | 0x0000 3400 - 0x0000 37ff
 *          14      |        1         |  224 - 239  | 0x0000 3800 - 0x0000 3bff
 *          15      |        1         |  240 - 255  | 0x0000 3c00 - 0x0000 3fff
 *          16      |        1         |  256 - 271  | 0x0000 4000 - 0x0000 43ff
 *          17      |        1         |  272 - 287  | 0x0000 4400 - 0x0000 47ff
 *          18      |        1         |  288 - 303  | 0x0000 4800 - 0x0000 4bff
 *          19      |        1         |  304 - 319  | 0x0000 4c00 - 0x0000 4fff
 *          20      |        1         |  320 - 335  | 0x0000 5000 - 0x0000 53ff
 *          21      |        1         |  336 - 351  | 0x0000 5400 - 0x0000 57ff
 *          22      |        1         |  352 - 367  | 0x0000 5800 - 0x0000 5bff
 *          23      |        1         |  368 - 383  | 0x0000 5c00 - 0x0000 5fff
 *          24      |        1         |  384 - 399  | 0x0000 6000 - 0x0000 63ff
 *          25      |        1         |  400 - 415  | 0x0000 6400 - 0x0000 67ff
 *          26      |        1         |  416 - 431  | 0x0000 6800 - 0x0000 6bff
 *          27      |        1         |  432 - 447  | 0x0000 6c00 - 0x0000 6fff
 *          28      |        1         |  448 - 463  | 0x0000 7000 - 0x0000 73ff
 *          29      |        1         |  464 - 479  | 0x0000 7400 - 0x0000 77ff
 *          30      |        1         |  480 - 495  | 0x0000 7800 - 0x0000 7bff
 *          31      |        1         |  496 - 511  | 0x0000 7c00 - 0x0000 7fff
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 蜂鸣器需要短接 J7 跳线帽，才能被 PIO0_24 控制。而且必须在 am_prj_config.h
 *       内将 AM_CFG_BUZZER_ENABLE 定义为 1。但该宏已经默认配置为 1， 用户不必再次
 *       配置；
 *    3. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_iap.c src_lpc824_hw_iap
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-12  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_iap
 * \copydoc demo_lpc824_hw_iap.c
 */

/** [src_lpc824_hw_iap] */
#include "ametal.h"
#include "am_int.h"
#include "am_buzzer.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_iap.h"

/**
 * \brief 输出一页数据
 *
 * \param[in] sector_num     扇区编号，取值范围：0 ~ 31 (32KB flash)
 * \param[in] page_in_sector 页编号，取值范围：0 ~ 15
 *
 * \return 无
 */
am_local void __print_page_data (uint32_t sector_num, uint8_t page_in_sector)
{
    uint8_t *p_data   = NULL;
    uint32_t page_num = 0;
    uint32_t i        = 0;

    if (sector_num > 31 || page_in_sector > 15) {
        return;
    }

    page_num = sector_num * 16 + page_in_sector;

    AM_DBG_INFO("The page %d in sector %d data is : \r\n",
               page_in_sector, sector_num);

    p_data = (uint8_t *)(64 * page_num); /* 页的起始地址 */

    /* 一页的大小为 64 字节 */
    for (i = 0; i < 64; i++) {
        AM_DBG_INFO("%3d  ",*p_data++);
        if (((i+1) % 8) == 0) {
            AM_DBG_INFO("\r\n");
        }
    }
    AM_DBG_INFO("\r\n");
}

/**
 * \brief 例程入口
 */
void demo_lpc824_hw_iap_entry (void)
{
    uint32_t               i            = 0;
    uint8_t                boot_ver[2]  = {0};
    uint16_t               boot_ver_tmp = 0;
    uint32_t               uid[4]       = {0};
    uint32_t               pid          = 0;
    uint32_t               key          = 0;
    amhw_lpc82x_iap_stat_t stat;

    /* 将 RAM 中的数据拷贝到 FLASH 时，RAM 地址必须是字对齐 */
#if defined(__CC_ARM)
    __align(4) unsigned char data[256];
#elif defined(__GNUC__)
    __attribute__((aligned(4))) unsigned char data[256];
#endif

    /* FLASH 更新数据时，禁止被中断打断，需要关中断 */
    key = am_int_cpu_lock();

    /* 读取 UID */
    stat = amhw_lpc82x_iap_unique_id_read(uid);

    if (stat != AMHW_LPC82X_IAP_STAT_SUCCESS) {
        AM_DBG_INFO("Read UID failed : %d \r\n",stat);
        am_buzzer_beep(500);
    } else {
        AM_DBG_INFO("uid = 0x%04x%04x%04x%04x\r\n",
                    uid[3],
                    uid[2],
                    uid[1],
                    uid[0]);
    }

    /* 读取 PID */
    pid = amhw_lpc82x_iap_part_id_read();
    AM_DBG_INFO("PID = 0x%x\r\n", pid);

    /* 读取 boot 代码版本号 */
    boot_ver_tmp = amhw_lpc82x_iap_boot_ver_read();

    boot_ver[1] = boot_ver_tmp >> 8;
    boot_ver[0] = boot_ver_tmp;

    AM_DBG_INFO("Boot Code version: %d.%d \r\n", boot_ver[1], boot_ver[0]);

    /*
     * 将数据从 RAM 拷贝到 FLASH。
     * 扇区编号：31，地址范围：0x0000 7C00 - 0x0000 7FFF。
     */
    for (i = 0; i < 256; i++) {
        data[i] = i;
    }

    /* 准备扇区 31 */
    amhw_lpc82x_iap_prepare(31, 31);

    /* 擦除扇区 31 */
    amhw_lpc82x_iap_erase_sector(31, 31);

    /* 检查扇区是否空白 */
    stat = amhw_lpc82x_iap_blank_check(31, 31);

    if (stat != AMHW_LPC82X_IAP_STAT_SUCCESS) {
        AM_DBG_INFO("The Sector 31 is not blank\r\n");
        am_buzzer_beep(500);

    } else {
        AM_DBG_INFO("The Sector 31 is blank! \r\n");
    }

    amhw_lpc82x_iap_prepare(31, 31);

    /*
     * 将数据从 RAM 拷贝到 FLASH。
     * 扇区 31 的起始地址是 0x0000 7C00。
     */
    stat = amhw_lpc82x_iap_copy(0x00007C00, (uint32_t)&data[0], 256);

    if (stat != AMHW_LPC82X_IAP_STAT_SUCCESS) {
        AM_DBG_INFO("Copy to FLASH failed \r\n");
        am_buzzer_beep(500);
    } else {
        AM_DBG_INFO("Copy to FLASH success \r\n");
    }

    stat = amhw_lpc82x_iap_compare(0x00007C00, (uint32_t)&data[0], 256);

    if (stat != AMHW_LPC82X_IAP_STAT_SUCCESS) {
        AM_DBG_INFO("The data compare failed,%d \r\n", stat);
        am_buzzer_beep(500);
    } else {
        AM_DBG_INFO("The data compare success,the data in flash as follow :\r\n");
        __print_page_data(31, 0);
        __print_page_data(31, 1);
        __print_page_data(31, 2);
        __print_page_data(31, 3);
    }

    amhw_lpc82x_iap_prepare(31, 31);

    /* 擦除页 496 - 499
     * 扇区 31 的起始页编 496(31 * 16)
     * 扇区 31 前4页的数据都将改为：0xFF
     */
    amhw_lpc82x_iap_erase_page(496, 496 + 3);

    if (stat != AMHW_LPC82X_IAP_STAT_SUCCESS) {
        AM_DBG_INFO("The page erase failed,%d \r\n",stat);
        am_buzzer_beep(500);

    } else {
        AM_DBG_INFO("The page erase success,the data in flash as follow :\r\n");

        /* 所有数据应为 255 */
        __print_page_data(31, 0);
        __print_page_data(31, 1);
        __print_page_data(31, 2);
        __print_page_data(31, 3);
    }

    /* 重新使能中断 */
    am_int_cpu_unlock(key);

    AM_FOREVER {
        am_led_on(LED0);
        am_mdelay(500);
        am_led_off(LED0);
        am_mdelay(500);
    }
}
/** [src_lpc824_hw_iap] */

/* end of file */
