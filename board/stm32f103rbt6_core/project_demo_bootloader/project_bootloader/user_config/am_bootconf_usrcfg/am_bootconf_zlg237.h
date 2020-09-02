#include "am_boot_flash.h"
#include "am_boot_firmware_recv.h"
#include "am_boot_enter_check.h"
#include "am_boot_firmware.h"
#include "am_boot_msg.h"

/** \brief 单区BootLoader标准接口实例初始化*/
int am_zlg237_boot_single_inst_init(void);

/** \brief 单区固件存放实例初始化 */
am_boot_firmware_handle_t am_zlg237_boot_single_firmware_flash_inst_init(void);

/******************************************************************************/

/** \brief 双区固件存放实例初始化 */
am_boot_firmware_handle_t am_zlg237_boot_double_firmware_flash_inst_init(void);
/** \brief 消息（标志的方式）实例初始化 */
am_boot_msg_handle_t am_zlg237_boot_msg_flag_inst_init(void);
/** \brief 双区BootLoader标准接口实例初始化 */
int am_zlg237_boot_double_inst_init(void);


/******************************************************************************/

/** \brief flash实例初始化 */
am_boot_flash_handle_t am_zlg237_boot_kft_flash_inst_init(void);
/** \brief kft上位机实例初始化 */
int am_zlg237_boot_kft_inst_init(void);

/* end of file */
