#ifndef BLOCK_DEV_FTL_H
#define BLOCK_DEV_FTL_H

#include "am_ftl.h"
#include "block_dev.h"

BEGIN_C_DECLS

typedef struct _ram_dev_t {
    block_dev_t     base;
    am_ftl_handle_t handle;
    uint32_t        start_lbn;
    block_num_t     num_lbn;
} block_dev_ftl_t;

/* 初始化，创建一个块设备  */
block_dev_t* block_dev_ftl_init (block_dev_ftl_t *p_dev,
                                 am_ftl_handle_t  handle,
                                 uint32_t         start_lbn,
                                 block_num_t      num_lbn);

END_C_DECLS

#endif//BEGIN_C_DECLS

