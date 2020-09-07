/*******************************************************************************
                             AMetal
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
 * \brief components 所有例程函数入口声明
 * \sa    demo_components_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-07-30  pea, first implementation
 * \endinternal
 */

#ifndef __DEMO_COMPONENTS_ENTRIES_H
#define __DEMO_COMPONENTS_ENTRIES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_ftl.h"
#include "am_mtd.h"
#include "am_mx25xx.h"
#include "am_ep24cxx.h"
#include "am_fm175xx.h"
#include "am_fm25clxx.h"
#include "am_zm516x.h"
#include "am_zlg9021.h"
#include "am_zlg52810.h"
#include "am_mk100.h"
#include "am_gs200.h"
#include "am_rs200.h"
#include "zsn603.h"
#include "am_xmodem.h"
#include "am_baudrate_detect.h"
#include "am_iso7816.h"
#include "am_fmse_a03.h"
/**
 * \brief EP24CXX 器件例程
 *
 * \param[in] ep24cxx_handle EP24CXX 标准服务句柄
 * \param[in] test_lenth     测试字节数
 *
 * \return 无
 */
void demo_ep24cxx_entry (am_ep24cxx_handle_t ep24cxx_handle, int32_t test_lenth);

/**
 * \brief  MX25XX器件 例程
 *
 * \param[in] mx25xx_handle  MX25XX 标准服务句柄
 * \param[in] test_lenth     测试字节数
 *
 * \return 无
 */
void demo_mx25xx_entry (am_mx25xx_handle_t mx25xx_handle, int32_t test_lenth);

/**
 * \brief FTL 例程
 *
 * \param[in] ftl_handle     FTL 标准服务句柄
 * \param[in] test_lenth     测试字节数
 *
 * \return 无
 */
void demo_ftl_entry (am_ftl_handle_t ftl_handle, int32_t test_lenth);

/**
 * \brief MTD 例程
 *
 * \param[in] mtd_handle  MTD 标准服务句柄
 * \param[in] test_lenth  测试字节数
 *
 * \return 无
 */
void demo_mtd_entry (am_mtd_handle_t mtd_handle, int32_t test_lenth);

/**
 * \brief FM175XX 读CPU卡例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_cpu_card (am_fm175xx_handle_t handle);

/**
 * \brief FM175XX 读A类型卡例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_read_id (am_fm175xx_handle_t handle);

/**
 * \brief A类卡读休眠卡例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_halt (am_fm175xx_handle_t handle);

/**
 * \brief A类卡读块例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_read_block (am_fm175xx_handle_t handle);

/**
 * \brief A类卡值操作例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_val_operate (am_fm175xx_handle_t handle);

/**
 * \brief A类写块例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_write_block (am_fm175xx_handle_t handle);

/**
 * \brief B类卡读卡号例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_piccb_read_id (am_fm175xx_handle_t handle);

/**
 * \brief FM175XX LPCD模式例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_lpcd_mode (am_fm175xx_handle_t handle);
/**
 * \brief FM175XX FM11RF005M卡操作例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_fm11rf005m_operate (am_fm175xx_handle_t handle);

/**
 * \brief zm516x 模块自动波特率例程
 * \param[in] handle  zm516x 服务句柄
 * \return 无
 */
void demo_zm516x_baudrate_entry (am_zm516x_handle_t handle);

/**
 * \brief zm516x 模块 GPIO 例程
 * \param[in] handle  zm516x 服务句柄
 * \return 无
 */
void demo_zm516x_gpio_entry (am_zm516x_handle_t handle);

/**
 * \brief zm516x 模块 KEY LED 例程
 * \param[in] handle  zm516x 服务句柄
 * \return 无
 */
void demo_zm516x_key_led_entry (am_zm516x_handle_t handle);

/**
 * \brief zm516x 模块组网例程
 * \param[in] handle  zm516x 服务句柄
 * \return 无
 */
void demo_zm516x_network_entry (am_zm516x_handle_t handle);

/**
 * \brief zm516x 模块搜索例程
 * \param[in] handle  zm516x 服务句柄
 * \return 无
 */
void demo_zm516x_search_entry (am_zm516x_handle_t handle);

/**
 * \brief zm516x 模块演示例程
 * \param[in] handle  zm516x 服务句柄
 * \return 无
 */
void demo_zm516x_entry (am_zm516x_handle_t handle);

/**
 * \brief zlg9021 蓝牙模块自动波特率例程
 * \param[in] handle  zlg9021 服务句柄
 * \return 无
 */
void demo_zlg9021_baudrate_entry (am_zlg9021_handle_t zlg9021_handle);

/**
 * \brief zlg9021 蓝牙模块 LED 例程
 * \param[in] handle  zlg9021 服务句柄
 * \return 无
 */
void demo_zlg9021_led_entry (am_zlg9021_handle_t zlg9021_handle);

/**
 * \brief zlg52810 蓝牙模块自动波特率例程
 * \param[in] handle  zlg52810 服务句柄
 * \return 无
 */
void demo_zlg52810_baudrate_entry (am_zlg52810_handle_t zlg52810_handle);
/**
 * \brief EEPROM例程
 * \param[in] 无
 * \return 无
 */
void demo_am845_core_eeprom_entry (void);

/**
 * \brief FM25CLxx例程
 * \param[in] handle  FM25CLxx 服务句柄
 * \return 无
 */
void demo_fm25clxx_entry(am_fm25clxx_handle_t fm25clxx_handle, int32_t test_lenth);

/**
 * \brief  ZSN603  LED测试实例程序
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \retval  no : 无返回值
 */
void demo_zsn603_led_test_entry (zsn603_handle_t handle);

/**
 * \brief  ZSN603  A类卡激活例程
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \retval  no : 无返回值
 */
void demo_zsn603_picca_active_test_entry (zsn603_handle_t handle);

/**
 * \brief  ZSN603  B类卡激活例程
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \retval  no : 无返回值
 */
void demo_zsn603_piccb_active_test_entry (zsn603_handle_t handle);

/**
 * \brief  ZSN603  自动检测模式例程
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \retval  no : 无返回值
 */
void demo_zsn603_auto_detect_test_entry(zsn603_handle_t handle);

/**
 * \brief xmodem接收例程
 * \param[in] handle  xmodem接收服务句柄
 * \return 无
 */
void demo_xmodem_rec_entry (am_xmodem_rec_handle_t  handle);

/**
 * \brief xmodem发送例程
 * \param[in] handle  xmodem发送服务句柄
 * \return 无
 */
void  demo_xmodem_tx_entry (am_xmodem_tx_handle_t  handle);

/**
 * \brief 串口波特率检测例程
 *
 * \param[in] baudrate_handle  波特率检测服务句柄
 * \param[in] uart_handle      串口服务句柄
 *
 * \return 无
 */
void demo_uart_auto_baudrate_entry (am_baudrate_detect_handle_t baudrate_handle,
                                    am_uart_handle_t            uart_handle);

/**
 * \brief MK100例程
 * \param[in] handle  MK100 服务句柄
 * \return 无
 */
void demo_mk100_entry(am_mk100_handle_t handle);

/**
 * \brief GS200 模块设置波特率例程
 * \param[in] handle  GS200设备服务句柄
 * \return 无
 */
void demo_gs200_baudrate_entry (am_gs200_handle_t gs200_handle,uint32_t gs200_baudrate);

/**
 * \brief RS200 模块例程
 * \param[in] handle  RS200设备服务句柄
 * \return 无
 */
void demo_rs200_entry (am_rs200_handle_t handle);

/**
 * \brief ISO7816 接触式卡例程
 * \param[in] iso7816_handle  ISO7816设备服务句柄
 * \return 无
 */
void demo_iso7816_entry(am_iso7816_handle_t iso7816_handle);

/**
 * \brief FMSE-A03 二进制读写测试例程
 * \param[in] handle： FMSE标准服务句柄
 * \return 无
 */
void demo_fmse_binary_rw_test_entry (am_fmse_handle_t handle);

/**
 * \brief FMSE-A03 命令测试例程
 * \param[in] handle： FMSE标准服务句柄
 * \return 无
 */
void demo_fmse_cmd_test_entry (am_fmse_handle_t handle);

/**
 * \brief FMSE-A03 数据压缩例程
 * \param[in] handle： FMSE标准服务句柄
 * \return 无
 */
void demo_fmse_data_compress_test_entry (am_fmse_handle_t handle);

/**
 * \brief FMSE-A03 加解密测试例程
 * \param[in] handle： FMSE标准服务句柄
 * \return 无
 */
void demo_fmse_data_encrypt_and_decrypt_entry (am_fmse_handle_t handle);

/**
 * \brief FMSE-A03 ECC计算例程
 * \param[in] handle： FMSE标准服务句柄
 * \return 无
 */
void demo_fmse_ecc_calc_test_entry (am_fmse_handle_t handle);

/**
 * \brief FMSE-A03 密钥对生成例程
 * \param[in] handle： FMSE标准服务句柄
 * \param[in] mode  ： 加密模式  0：1024RSA   1：2048RSA   2：SM2
 * \return 无
 */
void demo_fmse_gen_key_pair_entry (am_fmse_handle_t handle, uint8_t mode);

/**
 * \brief FMSE-A03 RSA载入例程
 * \param[in] handle： FMSE标准服务句柄
 * \return 无
 */
void demo_fmse_rsa_load_test_entry (am_fmse_handle_t  handle);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_COMPONENTS_ENTRIES_H */

/* end of file */
