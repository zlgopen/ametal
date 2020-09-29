#include "am_boot_msg.h"
#include "am_boot_flash.h"
#include "am_boot_firmware.h"
#include "am_boot_firmware_recv.h"
#include "am_boot_enter_check.h"
#include "am_xmodem.h"

am_boot_flash_handle_t    am_zsl42x_boot_flash_inst_init(void);
am_boot_msg_handle_t      am_zsl42x_boot_msg_flag_inst_init(void);
am_boot_firmware_handle_t am_zsl42x_boot_firmware_flash_inst_init(void);
int                       am_zsl42x_boot_inst_init(void);
int am_zsl42x_clk_inst_deinit (void);

