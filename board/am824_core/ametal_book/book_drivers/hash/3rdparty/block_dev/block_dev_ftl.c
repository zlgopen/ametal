#include "ametal.h"
#include "block_dev_ftl.h"
#include "am_ftl.h"

/*******************************************************************************
 本地函数
*******************************************************************************/

static block_num_t __block_dev_ftl_get_block_nr(block_dev_t *p_dev)
{
    block_dev_ftl_t *p_ftl_dev = (block_dev_ftl_t *)p_dev;
    return p_ftl_dev->num_lbn;
}

/******************************************************************************/

static am_bool_t __block_dev_ftl_read_block (block_dev_t *p_dev,
                                          block_num_t  block_num,
                                          void        *buff)
{
    block_dev_ftl_t *p_ftl_dev = (block_dev_ftl_t *)p_dev;

    am_ftl_read(p_ftl_dev->handle, p_ftl_dev->start_lbn + block_num, buff);

    return TRUE;
}

/******************************************************************************/

static am_bool_t __block_dev_ftl_write_block (block_dev_t  *p_dev,
                                           block_num_t   block_num,
                                           const void   *buff)
{
    block_dev_ftl_t *p_ftl_dev = (block_dev_ftl_t *)p_dev;

    am_ftl_write(p_ftl_dev->handle, p_ftl_dev->start_lbn + block_num, (void *)buff);

    return TRUE;
}

/******************************************************************************/

static void __block_dev_ftl_close (block_dev_t* dev)
{
    return;
}

/*******************************************************************************
 公共函数
*******************************************************************************/

block_dev_t* block_dev_ftl_init (block_dev_ftl_t *p_dev,
                                 am_ftl_handle_t  handle,
                                 uint32_t         start_lbn,
                                 block_num_t      num_lbn)
{
    return_value_if_fail(num_lbn > 0, NULL);

    p_dev->start_lbn = start_lbn;
    p_dev->num_lbn   = num_lbn;
    p_dev->handle    = handle;

    p_dev->base.close        = __block_dev_ftl_close;
    p_dev->base.read_block   = __block_dev_ftl_read_block;
    p_dev->base.write_block  = __block_dev_ftl_write_block;
    p_dev->base.get_block_nr = __block_dev_ftl_get_block_nr;;

    return &(p_dev->base);
}
