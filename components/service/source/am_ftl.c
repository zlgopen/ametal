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
 * \brief A Flash Translation Layer memory card driver
 *
 * \internal
 * \par Modification history
 * - 1.00 16-08-23  tee, first implementation.
 * \endinternal
 */
#undef AM_VDEBUG
#include "ametal.h"
#include "am_ftl.h"
#include "string.h"
#include "am_vdebug.h"

/*******************************************************************************
    Local Tag information type define
*******************************************************************************/

/**
 * Sector tag info
 */
#define __FTL_SECTOR_FREE     0xff
#define __FTL_SECTOR_USED     0x55
#define __FTL_SECTOR_IGNORE   0x11
#define __FTL_SECTOR_DELETED  0x00

#define __FTL_MAGIC_NUM    0x3c598761u

#define __FTL_BLOCK_TYPE_LOG    0x11
#define __FTL_BLOCK_TYPE_COPY   0x22
#define __FTL_BLOCK_TYPE_DATA   0x44

/* Block Control Information (16 bytes) */
struct __ftl_bci {
    uint32_t      magic_num;      /* magic num   */
    uint8_t       type_log;       /* log block   */
    uint8_t       type_copy;      /* copy block  */
    uint8_t       type_data;      /* data block, may be change from log block */
    uint8_t       type_reserved;  /* reserved for future */
    uint16_t      lbn1;           /* logic block */
    uint16_t      lbn2;           /* logic block */
    uint32_t      wear_info;      /* wear infomation */
};

#define __FTL_SECTOR_STAT_START    0x11
#define __FTL_SECTOR_STAT_DATA     0x22
#define __FTL_SECTOR_STAT_DATA_DEL 0x00    /* deleted by user */

/* Sector Control Information (4 bytes) */
struct __ftl_sci {
    uint8_t  stat_start;        /* start                */
    uint8_t  stat_data;         /* data saved complete  */
    uint8_t  locgic_sec0;       /* logic sector         */
    uint8_t  locgic_sec1;       /* logic sector         */
};

/*******************************************************************************
    Local defines
*******************************************************************************/

/*
 * these info are used in replunit_table
 */
#define __FTL_BLOCK_NIL          0xffff /* last block of a chain   */
#define __FTL_BLOCK_FREE         0xfffe /* free block              */
#define __FTL_BLOCK_NOTEXPLORED  0xfffd /* non explored block      */
#define __FTL_BLOCK_RESERVED     0xfffc /* bios block or bad block */


#define __FTL_ERASE_MARK      0x3c69

/**
 * Sector tag info
 */
#define __FTL_SECTOR_FREE     0xff
#define __FTL_SECTOR_USED     0x55
#define __FTL_SECTOR_IGNORE   0x11
#define __FTL_SECTOR_DELETED  0x00

/**
 * Fold MASK
 */
#define __FTL_FOLD_MARK_IN_PROGRESS 0x5555

#define __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn) \
    ((pbn + p_ftl->p_info->reserved_blocks) *      \
     (AM_MTD_ERASE_UNIT_SIZE_GET(p_ftl->mtd)))

/*******************************************************************************
    Local function declare
*******************************************************************************/
static int __ftl_log_buf_victim_switch (am_ftl_serv_t  *p_ftl,
                                        struct log_buf *p_log);

/*******************************************************************************
    The base utility fuctions
*******************************************************************************/

static int __ftl_memcmpb (void *a, int c, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        if (c != ((unsigned char *)a)[i])
            return 1;
    }
    return 0;
}

/*******************************************************************************
    The MTD base handle fuctions
*******************************************************************************/

/* already consider the reserved blocks */
static int __ftl_bci_read (am_ftl_serv_t    *p_ftl,
                           uint16_t          pbn,
                           struct __ftl_bci *p_bci)
{
    /* every block need with a some tag information */

    int       ret = 0;
    uint32_t  addr;

    addr = __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn);

    ret = am_mtd_read(p_ftl->mtd, addr, p_bci, sizeof(struct __ftl_bci));

    if (ret < sizeof(struct __ftl_bci)) {
        AM_DBG_INFO("ftl: __ftl_bci_read error, ret is: %d\n", ret);
        return ret;
    }

    return 0;
}

/******************************************************************************/

static int __ftl_bci_write (am_ftl_serv_t    *p_ftl,
                            uint16_t          pbn,
                            struct __ftl_bci *p_bci)
{
    /* every block need with a some tag information */

    int       ret = 0;
    uint32_t  addr;

    addr = __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn);

    ret = am_mtd_write(p_ftl->mtd, addr, p_bci, sizeof(struct __ftl_bci));

    if (ret < sizeof(struct __ftl_bci)) {
        AM_DBG_INFO("ftl: __ftl_bci_write error, ret is: %d\n", ret);
        return ret;
    }

    return 0;
}

/******************************************************************************/

/* already consider the reserved blocks */
static int __ftl_sci_read (am_ftl_serv_t    *p_ftl,
                           uint16_t          pbn,
                           uint16_t          sec,
                           struct __ftl_sci *p_sci)
{
    /* every block need with a some tag information */

    int       ret = 0;
    uint32_t  addr;

    addr = __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn) +
            sizeof(struct __ftl_bci) +
           (sizeof(struct __ftl_sci) * sec);

    ret = am_mtd_read(p_ftl->mtd, addr, p_sci, sizeof(struct __ftl_sci));

    if (ret < sizeof(struct __ftl_sci)) {
        AM_DBG_INFO("ftl: __ftl_sci_read error, ret is: %d\n", ret);
        return ret;
    }

    return 0;
}

/******************************************************************************/

static int __ftl_sci_write (am_ftl_serv_t    *p_ftl,
                            uint16_t          pbn,
                            uint16_t          sec,
                            struct __ftl_sci *p_sci)
{
    /* every block need with a some tag information */

    int       ret = 0;
    uint32_t  addr;

    addr =  __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn) +
            sizeof(struct __ftl_bci) +
           (sizeof(struct __ftl_sci) * sec);

    ret = am_mtd_write(p_ftl->mtd, addr, p_sci, sizeof(struct __ftl_sci));

    if (ret < sizeof(struct __ftl_sci)) {
        AM_DBG_INFO("ftl: __ftl_sci_write error, ret is: %d\n", ret);
        return ret;
    }

    return 0;
}
 
/******************************************************************************/

static int __ftl_data_read (am_ftl_serv_t     *p_ftl,
                            uint16_t           pbn,
                            uint16_t           sec,
                            void              *p_buf)
{
    size_t logic_blk_size = p_ftl->p_info->logic_blk_size;

    sec += p_ftl->sectors_hdr;     /* skip the header sectors for information */

    return am_mtd_read(p_ftl->mtd,
                       __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn) +
                       sec * logic_blk_size,
                       p_buf,
                       logic_blk_size);
}

/******************************************************************************/

static int __ftl_data_write (am_ftl_serv_t     *p_ftl,
                             uint16_t           pbn,
                             uint16_t           sec,
                             void              *p_buf)
{
    size_t logic_blk_size = p_ftl->p_info->logic_blk_size;

    sec += p_ftl->sectors_hdr;     /* skip the header sectors for information */

    return am_mtd_write(p_ftl->mtd,
                        __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn) +
                        sec * logic_blk_size,
                        p_buf,
                        logic_blk_size);
}

/******************************************************************************/

static int __ftl_block_erase (am_ftl_serv_t     *p_ftl,
                              uint16_t           pbn)
{
    int                 ret;
    struct __ftl_bci    bci;
    uint32_t            wear_info;

    if (__ftl_bci_read(p_ftl, pbn, &bci) >= 0) {

        if (bci.magic_num != __FTL_MAGIC_NUM) {  /* first use */
            bci.wear_info = 1;
        } else {
            bci.wear_info++;
            /* wrap (almost impossible with current flash) or free block */
            if (bci.wear_info == 0) {
                bci.wear_info = 1;
            }
        }
    }

    wear_info = bci.wear_info;

    memset(&bci, 0xFF, sizeof(bci));

    bci.magic_num = __FTL_MAGIC_NUM;
    bci.wear_info = wear_info;

    AM_DBG_INFO("Erase block : %d \n", pbn);

    ret = am_mtd_erase(p_ftl->mtd,
                       __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, pbn),
                       AM_MTD_ERASE_UNIT_SIZE_GET(p_ftl->mtd));

    if (ret < 0) {
        return ret;
    }

    return __ftl_bci_write(p_ftl, pbn, &bci);
}

/*******************************************************************************
    free block manage
*******************************************************************************/
static int __free_block_init (am_ftl_serv_t *p_ftl)
{
    /* all free at after initial */
    memset(p_ftl->p_free, 0xFF, p_ftl->free_size * sizeof(uint32_t));

    return 0;
}

/******************************************************************************/

/* mark a block is free */
static int __free_block_set (am_ftl_serv_t *p_ftl, uint16_t pbn, int val)
{
    if (val) {
        AM_DBG_INFO("set the %d is free!\n", pbn);
        p_ftl->p_free[pbn >> 5] |= (1 << (pbn & 0x1F));
    } else {
        p_ftl->p_free[pbn >> 5] &= ~(1 << (pbn & 0x1F));
    }

    return 0;
}

/******************************************************************************/

/* Get the next free block */
static int __free_block_next_get (am_ftl_serv_t *p_ftl, uint16_t pbn)
{
    unsigned int i;

    uint16_t   start_bit = pbn;               /* start at cur pos */
    uint16_t   end_bit   = p_ftl->nb_blocks;
    uint32_t  *p_free    = p_ftl->p_free;

    for (i = start_bit; i < end_bit; i++) {
        if ((p_free[i >> 5] & (1 << (i & 0x1F)))) {   /* It's free */
            return i;
        }
    }

    for (i = 0; i < start_bit; i++) {
        if ((p_free[i >> 5] & (1 << (i & 0x1F)))) {   /* It's free */
            return i;
        }
    }

    AM_DBG_INFO("after the start bit %d, Can't find a free block!\n", pbn);

    for (i = 0; i < p_ftl->free_size; i++) {
        AM_DBG_INFO("%x \n", p_free[i]);
    }

    return -1;
}

/******************************************************************************/
static int __ftl_mem_init (am_ftl_serv_t          *p_ftl,
                           uint8_t                *p_buf,
                           uint32_t                len)
{
    uintptr_t addr = (uintptr_t)p_buf;

    uint32_t nb_sectors;

    int      i;

    nb_sectors = p_ftl->sectors_per_blk + p_ftl->sectors_hdr;

    /* align with 4 bytes */
    addr = addr & ~0x03;

    /* The size should be max sectors per unit */
    p_ftl->free_size   =  (p_ftl->nb_blocks + 32 - 1) / 32;
    p_ftl->p_free      = (uint32_t *)addr;
    addr              += sizeof(uint32_t) * p_ftl->free_size;

    /* ram buf for write / read (logic_block_size bytes)*/
    p_ftl->p_wr_buf =  (uint8_t *)addr;
    addr            += sizeof(uint8_t) * p_ftl->p_info->logic_blk_size;

    /* log buffer, size is nb_log_blocks * sizeof(struct log_buf) */
    p_ftl->p_log_buf  = (struct log_buf *)addr;
    addr              += sizeof(struct log_buf) * p_ftl->p_info->nb_log_blocks;

    /* eun table, size is 2 * block */
    p_ftl->p_eun_table =  (uint16_t *)addr;
    addr               += sizeof(uint16_t) * p_ftl->nb_blocks;

    for (i = 0; i < p_ftl->p_info->nb_log_blocks; i++) {
        p_ftl->p_log_buf[i].p_map  = (uint8_t *)addr;
        addr                      += sizeof(uint8_t) * nb_sectors;
    }
    return 0;
}

/******************************************************************************/

static int __ftl_init (am_ftl_serv_t *p_ftl)
{
    const am_ftl_info_t *p_info  = p_ftl->p_info;
    uint32_t             ram_len = AM_FTL_RAM_SIZE_GET(
                                       AM_MTD_CHIP_SIZE_GET(p_ftl->mtd),
                                       AM_MTD_ERASE_UNIT_SIZE_GET(p_ftl->mtd),
                                       p_info->logic_blk_size,
                                       p_info->nb_log_blocks);

    AM_DBG_INFO("The expect size of ram is %d\n", ram_len);
    if (p_info->len < ram_len) {
        AM_DBG_INFO("The size (%d) of p_buf is not enough\n", p_info->len);
        return -1;
    }

    p_ftl->nb_blocks       =  AM_MTD_CHIP_SIZE_GET(p_ftl->mtd) /
                              AM_MTD_ERASE_UNIT_SIZE_GET(p_ftl->mtd);

    p_ftl->sectors_per_blk = AM_MTD_ERASE_UNIT_SIZE_GET(p_ftl->mtd) /
                             p_info->logic_blk_size;

    p_ftl->sectors_hdr     = ((sizeof(struct __ftl_sci) * p_ftl->sectors_per_blk
                               + sizeof(struct __ftl_bci) - 1)
                               / p_info->logic_blk_size) + 1;

    /* valid sectors per block in actually */
    p_ftl->sectors_per_blk -= p_ftl->sectors_hdr;

    /* reserved 1 block for victim */
    p_ftl->max_lbn         = ((p_ftl->nb_blocks
                               - 1
                               - p_info->reserved_blocks
                               - p_info->nb_log_blocks)
                              * p_ftl->sectors_per_blk) - 1;

    p_ftl->nb_blocks       -= p_info->reserved_blocks;


    AM_DBG_INFO("The sectors hdr is %d \n", p_ftl->sectors_hdr);
    AM_DBG_INFO("nb_blocks = %d \n", p_ftl->nb_blocks);
    AM_DBG_INFO("max_lbn = %d \n", p_ftl->max_lbn);
    AM_DBG_INFO("valid size / total_size = %d / %d \n",
                (p_ftl->max_lbn + 1) * p_info->logic_blk_size,
                AM_MTD_CHIP_SIZE_GET(p_ftl->mtd));

    if (__ftl_mem_init (p_ftl, p_info->p_buf, p_info->len) < 0) {
        return -1;
    }

    return 0;
}

/******************************************************************************/
static int __ftl_log_block_process (am_ftl_serv_t *p_ftl)
{
    int               i,j;
    size_t            log_blocks = p_ftl->p_info->nb_log_blocks;

    for (i = 0; i < log_blocks; i++) {

        if (p_ftl->p_log_buf[i].used == p_ftl->sectors_per_blk) { /* full */

            AM_DBG_INFO("The log buffer is full!\n");

            for (j = 0; j < p_ftl->sectors_per_blk; j++) {
                if (p_ftl->p_log_buf[i].p_map[j] != j) {
                    break;
                }
            }

            if (j == p_ftl->sectors_per_blk) {
                __ftl_log_buf_victim_switch(p_ftl, &p_ftl->p_log_buf[i]);
            }
        }
    }

    return 0;
}

/******************************************************************************/
static int __ftl_copy_block_process (am_ftl_serv_t *p_ftl, uint16_t copy_block)
{
    int               i;
    struct __ftl_bci  bci;
    size_t            log_blocks = p_ftl->p_info->nb_log_blocks;

    AM_DBG_INFO("there is a copy block!\n");
    if ((__ftl_bci_read(p_ftl, copy_block, &bci) < 0) ||
        (bci.lbn1 == 0xFFFF)) {
        if (__ftl_block_erase(p_ftl, copy_block) < 0) {
            __free_block_set(p_ftl, copy_block, 0);    /* mask it as not free */
        } else {
            __free_block_set(p_ftl, copy_block, 1);
        }
    }

    /* erase the directly buffer   */
    if (p_ftl->p_eun_table[bci.lbn1] != 0xFFFF) {
        if (__ftl_block_erase(p_ftl, p_ftl->p_eun_table[bci.lbn1]) >= 0) {
            __free_block_set(p_ftl, p_ftl->p_eun_table[bci.lbn1], 1);
        }
    }

    /* erase the log buffer   */
    for (i = 0; i < log_blocks; i++) {

        /* find the log buffer */
        if (p_ftl->p_log_buf[i].lbn == bci.lbn1) {

            if (__ftl_block_erase(p_ftl, p_ftl->p_log_buf[i].pbn ) >= 0) {
                __free_block_set(p_ftl, p_ftl->p_log_buf[i].pbn , 1);
            }

            p_ftl->p_log_buf[i].pbn      = 0xFFFF;
            p_ftl->p_log_buf[i].lbn      = 0xFFFF;
            p_ftl->p_log_buf[i].used     = 0;

            memset(p_ftl->p_log_buf[i].p_map, 0xFF, p_ftl->sectors_per_blk);
        }
    }

    /* set the copy block to data block */
    bci.type_data = __FTL_BLOCK_TYPE_DATA;
    __ftl_bci_write(p_ftl, copy_block, &bci);

    p_ftl->p_eun_table[bci.lbn1] = copy_block;  /* new relationship */

    return 0;
}

/******************************************************************************/

/* find a free block with minimum wear info as first free block to use */
static int __ftl_free_update (uint16_t *p_free_now,
                              uint32_t *p_wear_min,
                              uint16_t  new_pbn,
                              uint32_t  wear_new)
{
    if (*p_wear_min > wear_new) {   /* find a less erase block */
        *p_free_now = new_pbn;
        *p_wear_min = wear_new;
    }
    return 0;
}
/******************************************************************************/

static int __ftl_erase_at_mount (am_ftl_serv_t *p_ftl,
                                 uint16_t       pbn,
                                 uint32_t      *p_wear_min,
                                 uint32_t       wear_before)
{
    if (__ftl_block_erase(p_ftl, pbn) < 0) {
        __free_block_set(p_ftl, pbn, 0);    /* mask it as not free */
    } else {
       __ftl_free_update(&p_ftl->last_free, p_wear_min, pbn, wear_before + 1);
    }
    return 0;
}

/******************************************************************************/
static int __nfl_mount (am_ftl_serv_t *p_ftl)
{
    unsigned int i,j,k;
    unsigned int log_num  = 0;
    unsigned int sec_used = 0;

    size_t   log_blocks = p_ftl->p_info->nb_log_blocks;
    uint16_t max_lbn    = p_ftl->nb_blocks - log_blocks - 1;


    uint16_t copy_block = 0xFFFF;
    uint32_t wear_min   = 0xFFFFFFFF;

    struct __ftl_bci  bci;
    struct __ftl_sci  sci;

    /* after init, all block mask as free block */
    __free_block_init(p_ftl);

    p_ftl->last_free = 0;

    for (i = 0; i < log_blocks; i++) {
        p_ftl->p_log_buf[i].lbn  = 0xFFFF;
        p_ftl->p_log_buf[i].pbn  = 0xFFFF;
        p_ftl->p_log_buf[i].used = 0;
        memset(p_ftl->p_log_buf[i].p_map, 0xFF, p_ftl->sectors_per_blk);

    }

    /* init the logical to physical table */
    for (i = 0; i < p_ftl->nb_blocks; i++) {
        p_ftl->p_eun_table[i] = __FTL_BLOCK_NIL;
    }

    for (i = 0; i < p_ftl->nb_blocks; i++) {

        if (__ftl_bci_read(p_ftl, i, &bci) < 0) {
            __free_block_set(p_ftl, i, 0);    /* mask it as not free */
        }

        if (bci.magic_num != __FTL_MAGIC_NUM) {
            __ftl_erase_at_mount(p_ftl, i, &wear_min, 0);
            continue;
        }

        /* power failed, erase the block */
        if ((bci.lbn1  != bci.lbn2) ||
            ((bci.lbn1 != 0xFFFF) && (bci.lbn1 > max_lbn))) {
            __ftl_erase_at_mount(p_ftl, i, &wear_min, bci.wear_info);
            continue;
        }

        /* power failed while try to set it as a log buffer */
        if ((bci.type_log != __FTL_BLOCK_TYPE_LOG) &&
            (bci.type_log != 0xFF)) {
            __ftl_erase_at_mount(p_ftl, i, &wear_min, bci.wear_info);
            continue;
        }

        /* the block is a data buffer */
        if (bci.type_data != 0xFF) {

            if (bci.lbn1 > max_lbn) {
                __ftl_erase_at_mount(p_ftl, i, &wear_min, bci.wear_info);
                continue;
            } else {

                if (p_ftl->p_eun_table[bci.lbn1] != 0xFFFF) { 

                    AM_DBG_INFO("The lbn is already exist, just erase\n");
                    __ftl_erase_at_mount(p_ftl, i, &wear_min, bci.wear_info);
                    continue;
                }
                p_ftl->p_eun_table[bci.lbn1] = i;
                __free_block_set(p_ftl, i, 0);       /* mask it as not free */
            }

        } else if (bci.type_log  == __FTL_BLOCK_TYPE_LOG) {

            if (bci.lbn1 > max_lbn) {
                __ftl_erase_at_mount(p_ftl, i, &wear_min, bci.wear_info);
                continue;
            } else {                                 /* a log buffer */

                if (log_num >= log_blocks) {
                    AM_DBG_INFO("too much log buffer?? \n");
                    __ftl_erase_at_mount(p_ftl, i, &wear_min, bci.wear_info);
                    continue;
                }

                sec_used = 0;
                for (j = 0; j < p_ftl->sectors_per_blk; j++) {

                    if (__ftl_sci_read(p_ftl, i, j, &sci) >= 0) {

                        if (__ftl_memcmpb(&sci, 0xFF, sizeof(sci)) == 0) {
                            break;
                        }

                        /* The data didn't write */
                        if ((sci.stat_data   != __FTL_SECTOR_STAT_DATA) ||
                            (sci.locgic_sec0 != sci.locgic_sec1)) {

                            /* the sector data should be ignore */
                            p_ftl->p_log_buf[log_num].p_map[j] = 0xFF;

                        } else {                      /* valid data */

                            p_ftl->p_log_buf[log_num].p_map[j] = sci.locgic_sec0;
                        }
                        sec_used++;
                    }
                }

                /* not used */
                if (sec_used == 0) {

                    AM_DBG_INFO("The log buffer %d is not used! erase it \r\n",i);
                    __free_block_set(p_ftl, i, 1);        /* mask it as free */
                    __ftl_erase_at_mount(p_ftl, i, &wear_min, bci.wear_info);

                    continue;
                }

                __free_block_set(p_ftl, i, 0);         /* mask it as not free */

                p_ftl->p_log_buf[log_num].pbn  = i;
                p_ftl->p_log_buf[log_num].lbn  = bci.lbn1;
                p_ftl->p_log_buf[log_num].used = sec_used;

                AM_DBG_INFO("Find a log buffer! (%d, %d) : ", bci.lbn1, i);

                for (k = 0; k < sec_used; k++) {
                    AM_DBG_INFO("%d  ", p_ftl->p_log_buf[log_num].p_map[k]);
                }

                AM_DBG_INFO("\n");

                log_num++;
            }
        } else if (bci.type_copy != 0xFF) {  /* there is a copy block */

            copy_block = i;
            __free_block_set(p_ftl, copy_block, 0);

        } else {                             /* It's a free block    */
            __ftl_free_update(&p_ftl->last_free, &wear_min, i, bci.wear_info);
        }
    }

    AM_DBG_INFO("for each end!\n");

    /* process the copy block */
    if (copy_block != 0xFFFF) {
        __ftl_copy_block_process(p_ftl, copy_block);
    }

    /* process the log buffer */
    if (log_num != 0) {
        __ftl_log_block_process(p_ftl);
    }

    AM_DBG_INFO("last free is %d \n", p_ftl->last_free);

    return 0;
}

/******************************************************************************/

/*
 * NFTL_findfreeblock: Find a free Erase Unit on the NFTL partition.
 * This function is used when the give Virtual Unit Chain
 */
static uint16_t __ftl_freeblock_find (am_ftl_serv_t *p_ftl)
{
    int free = __free_block_next_get(p_ftl, p_ftl->last_free);

    if (free == -1) {
        AM_DBG_INFO("__ftl_freeblock_find: there are too few free EUNs\n");
        return __FTL_BLOCK_NIL;
    }

    /* todo: check the block is actually free, should check? */

    p_ftl->last_free = free;

    return free;
}

/******************************************************************************/

static int __ftl_log_buf_victim_switch (am_ftl_serv_t  *p_ftl,
                                        struct log_buf *p_log)
{
    struct __ftl_bci  bci;

    uint16_t direct_eun = p_ftl->p_eun_table[p_log->lbn];

    AM_DBG_INFO("only need switch!\n");

    /* may be erase last power up. because of power failed.*/
    if (direct_eun == 0xFFFF) {
         // do nothing
    } else {
        if (__ftl_block_erase(p_ftl, direct_eun) >= 0) {
            __free_block_set(p_ftl, direct_eun, 1);    /* mask it free */
        }
    }

    /* change to data block */
    __ftl_bci_read(p_ftl, p_log->pbn, &bci);
    bci.type_data =  __FTL_BLOCK_TYPE_DATA;
    __ftl_bci_write(p_ftl, p_log->pbn, &bci);

    p_ftl->p_eun_table[p_log->lbn] = p_log->pbn;    /* new relationship */

    p_log->pbn      = 0xFFFF;
    p_log->lbn      = 0xFFFF;
    p_log->used     = 0;

    memset(p_log->p_map, 0xFF, p_ftl->sectors_per_blk);

    return 0;
}
/******************************************************************************/

static int __ftl_log_buf_victim_normal (am_ftl_serv_t  *p_ftl,
                                        struct log_buf *p_log)
{
    int               i;
    struct __ftl_bci  bci;
    struct __ftl_sci  sci;
    uint16_t          new_eun;
    unsigned char     status;
    int               ret;
    uint16_t          direct_eun;

    /* Try to find an already-free block */
    new_eun  = __ftl_freeblock_find(p_ftl);

    if (new_eun == 0xFFFF) {
        return -1;
    }

    AM_DBG_INFO("The new victim log buffer is %d \n", new_eun);

    /* Save the this_vuc, the first node in the chain (8 ,8) */
    __ftl_bci_read(p_ftl, new_eun, &bci);
    bci.lbn1      = p_log->lbn;                 /* The logic block  */
    __ftl_bci_write(p_ftl, new_eun, &bci);

    memset(p_log->p_map, 0xFF, p_ftl->sectors_per_blk);

    /* copy the data into new block, log buffer first  */
    for (i = p_ftl->sectors_per_blk - 1; i >= 0; i--) {

        status = __FTL_SECTOR_IGNORE;

        /* 获取一个sector标签信息，以查看该sector的状态(bci)  */
        if (__ftl_sci_read(p_ftl, p_log->pbn, i, &sci) >= 0) {

             if ((sci.stat_start == __FTL_SECTOR_STAT_START) &&
                 (sci.stat_data  == __FTL_SECTOR_STAT_DATA)) {
                status = __FTL_SECTOR_USED;
             }
        }

        /* The data is valid and the data haven't be copied */
        if ((status                        == __FTL_SECTOR_USED) &&
            (p_log->p_map[sci.locgic_sec0] != 1)) {

            ret = __ftl_data_read(p_ftl, p_log->pbn, i, p_ftl->p_wr_buf);

            if (ret >= 0) {

                sci.stat_start = __FTL_SECTOR_STAT_START;
                sci.stat_data  = 0xFF;

                __ftl_sci_write(p_ftl, new_eun, sci.locgic_sec0, &sci);

                /* write the data */
                __ftl_data_write(p_ftl, 
                                 new_eun, 
                                 sci.locgic_sec0, 
                                 p_ftl->p_wr_buf);

                /* make the data is valid */
                sci.stat_data = __FTL_SECTOR_STAT_DATA;
                __ftl_sci_write(p_ftl, new_eun, sci.locgic_sec0, &sci);

                p_log->p_map[sci.locgic_sec0] = 1;  /* mark the sector be copied */
            }
        }
    }

    direct_eun = p_ftl->p_eun_table[p_log->lbn];

    /* copy the data into new block, directly buffer   */
    for (i = 0; (direct_eun != 0xFFFF) && (i < p_ftl->sectors_per_blk); i++) {

        /* The data havn't copy from log buffer */
        if (p_log->p_map[i] != 1) {

            status = __FTL_SECTOR_IGNORE;

            /* 获取一个sector标签信息，以查看该sector的状态(bci)  */
            if (__ftl_sci_read(p_ftl, direct_eun, i, &sci) >= 0) {

                 if ((sci.stat_start == __FTL_SECTOR_STAT_START) &&
                     (sci.stat_data  == __FTL_SECTOR_STAT_DATA)) {
                    status = __FTL_SECTOR_USED;
                 }
            }

            /* The data is valid and the data haven't be copied */
            if (status == __FTL_SECTOR_USED) {

                ret = __ftl_data_read(p_ftl, direct_eun, i, p_ftl->p_wr_buf);

                if (ret >= 0) {

                    sci.stat_start = __FTL_SECTOR_STAT_START;
                    sci.stat_data  = 0xFF;

                    __ftl_sci_write(p_ftl, new_eun, i, &sci);

                    /* write the data */
                    __ftl_data_write(p_ftl, new_eun, i, p_ftl->p_wr_buf);

                    /* mark the sector be copied */
                    sci.stat_data = __FTL_SECTOR_STAT_DATA;
                    __ftl_sci_write(p_ftl, new_eun, i, &sci);

                    p_log->p_map[sci.locgic_sec0] = 1;
                }
            }
        }
    }

    /* 1. copy done, set the type is copy  */
    bci.type_copy = __FTL_BLOCK_TYPE_COPY;
    __ftl_bci_write(p_ftl, new_eun, &bci);

    /* 2. set the lbn1 equal to lbn2, make it valid  */
    bci.lbn1 = bci.lbn2 = p_log->lbn;
    __ftl_bci_write(p_ftl, new_eun, &bci);

    /* 3.erase the directly buffer   */
    if (direct_eun != 0xFFFF) {
        if (__ftl_block_erase(p_ftl, direct_eun) >= 0) {
            __free_block_set(p_ftl, direct_eun, 1);    /* mask it free */
        }
    }

    /* 4. erase the log buffer   */
    if (p_log->pbn != 0xFFFF) {
        if (__ftl_block_erase(p_ftl, p_log->pbn) >= 0) {
            __free_block_set(p_ftl, p_log->pbn, 1);    /* mask it free */
        }
    }

    /* 5. set the new block to data block */
    bci.type_data = __FTL_BLOCK_TYPE_DATA;
    __ftl_bci_write(p_ftl, new_eun, &bci);

    p_ftl->p_eun_table[p_log->lbn] = new_eun;  /* new relationship */

    p_log->pbn      = 0xFFFF;
    p_log->lbn      = 0xFFFF;
    p_log->used     = 0;

    memset(p_log->p_map, 0xFF, p_ftl->sectors_per_blk);

    __free_block_set(p_ftl, new_eun, 0);     /* mask the block is not free */

    return 0;
}

/******************************************************************************/
static int __ftl_log_buf_victim (am_ftl_serv_t *p_ftl, struct log_buf *p_log)
{
    int i;

    AM_DBG_INFO("try to victim the block, lbn(%d)->pbn(%d)\n", p_log->lbn, 
                                                               p_log->pbn);

    if (p_log->used == p_ftl->sectors_per_blk) {

        AM_DBG_INFO("The log buffer is full!\n");

        for (i = 0; i < p_ftl->sectors_per_blk; i++) {
            if (p_log->p_map[i] != i) {
                break;
            }
        }

        if (i == p_ftl->sectors_per_blk) {
            return __ftl_log_buf_victim_switch(p_ftl, p_log);
        }
    }

    return __ftl_log_buf_victim_normal(p_ftl, p_log);
}

/******************************************************************************/
/*
 * Get the unit number into which we can write for this block.
 * Make it available if it isn't already
 */
static int __ftl_writeunit_find (am_ftl_serv_t *p_ftl,
                                 unsigned       lbn,
                                 uint16_t      *p_pbn,
                                 uint16_t      *p_sec)
{
    uint16_t this_vuc = lbn / p_ftl->sectors_per_blk;
    uint16_t idx      = lbn % p_ftl->sectors_per_blk;

    uint16_t write_eun;
    int      i;
    int      ret;
    struct __ftl_bci  bci;
    struct __ftl_sci  sci;

    struct log_buf   *p_log_victim = NULL;
    struct log_buf   *p_log_empty  = NULL;
    uint8_t           log_num      = 0;
    size_t            log_blocks   = p_ftl->p_info->nb_log_blocks;


    if (p_ftl->p_eun_table[this_vuc] == 0xFFFF) {   /* no related blocks */

        /* Try to find an already-free block */
        write_eun  = __ftl_freeblock_find(p_ftl);

        if (write_eun == 0xFFFF) {
            return -1;
        }

        AM_DBG_INFO("The new data block is %d .\n", write_eun);

        p_ftl->p_eun_table[this_vuc] = write_eun;

        /* tag the block is a data buffer */
        __ftl_bci_read(p_ftl, write_eun, &bci);
        bci.lbn1      = bci.lbn2 = this_vuc;
        bci.type_data = __FTL_BLOCK_TYPE_DATA;
        __ftl_bci_write(p_ftl, write_eun, &bci);

         *p_pbn = write_eun;
         *p_sec = idx;

         __free_block_set(p_ftl, write_eun, 0);   /* mask the block is not free */

        return 0;

    } else {                                     /* has related blocks */

        write_eun = p_ftl->p_eun_table[this_vuc];

        /* Read the sci TAG */
        __ftl_sci_read(p_ftl, write_eun, idx, &sci);

        if ((sci.stat_data == 0xFF) && (sci.stat_start == 0xFF)) {
            *p_pbn = write_eun;
            *p_sec = idx;

            /* Find the block in the chain */
            return 0;
        }
    }

    /*
     * OK. We didn't find one in the existing block, or exist in log buffer
     */
    p_log_victim = p_ftl->p_log_buf;
    for (i = 0; i < log_blocks; i++) {

        if (p_ftl->p_log_buf[i].lbn != 0xFFFF) {    /* find one exist log buf */
            log_num++;                                 
            if (p_ftl->p_log_buf[i].lbn == this_vuc) { /* matched */
                break;
            }
            if (p_log_victim->used < p_ftl->p_log_buf[i].used) {
                p_log_victim = &p_ftl->p_log_buf[i];
            }
        } else {
            p_log_empty = &p_ftl->p_log_buf[i];
        }
    }

    if (i == log_blocks) {                   /* Can't find log buf */

        if (log_num == log_blocks) {         /* log buf is full    */
            ret = __ftl_log_buf_victim(p_ftl, p_log_victim); /* victim use most */
            if (ret < 0) {
                return ret;
            }
        } else {                            /* not full */
            p_log_victim = p_log_empty;     /* such as victim the free log */
        }

    } else {                                /* find the matched  */

        if (p_ftl->p_log_buf[i].used == p_ftl->sectors_per_blk) {    /* full */
            ret = __ftl_log_buf_victim(p_ftl, &p_ftl->p_log_buf[i]); /* victim it  */
            if (ret < 0) {
                return ret;
            }
            p_log_victim = &p_ftl->p_log_buf[i];

        } else {

            p_ftl->p_log_buf[i].p_map[p_ftl->p_log_buf[i].used] = idx;

            *p_pbn = p_ftl->p_log_buf[i].pbn;
            *p_sec = p_ftl->p_log_buf[i].used++;

            return 0;
        }
    }

    /* check can directly write ? */
    write_eun = p_ftl->p_eun_table[this_vuc];

    /* Read the sci TAG */
    __ftl_sci_read(p_ftl, write_eun, idx, &sci);

    if ((sci.stat_data == 0xFF) && (sci.stat_start == 0xFF)) {
        *p_pbn = write_eun;
        *p_sec = idx;

        /* Find the block in the chain */
        return 0;
    }

    /* Ok, can't directly write, alloc a new free block as log buffer */
    write_eun = __ftl_freeblock_find(p_ftl);

    if (write_eun == 0xFFFF) {
        return -1;
    }

    AM_DBG_INFO("The new log block is %d .\n", write_eun);

    p_log_victim->pbn      = write_eun;
    p_log_victim->lbn      = this_vuc;
    p_log_victim->p_map[0] = idx;
    p_log_victim->used     = 1;


    *p_pbn = p_log_victim->pbn;
    *p_sec = 0;

    /* Save the this_vuc, the first node in the chain (8 ,8) */
    __ftl_bci_read(p_ftl, write_eun, &bci);
    bci.lbn1     = bci.lbn2 = this_vuc;
    bci.type_log = __FTL_BLOCK_TYPE_LOG;       /* The block is log buffer   */
    __ftl_bci_write(p_ftl, write_eun, &bci);

    __free_block_set(p_ftl, write_eun, 0);     /* mask the block is not free */

    return 0;
}

/******************************************************************************/
static int __ftl_readunit_check (am_ftl_serv_t *p_ftl,
                                 uint16_t       eun,
                                 uint16_t       sec)
{
    uint8_t           status = __FTL_SECTOR_IGNORE;   /* Initial as ignore */
    struct __ftl_sci  sci;

    if ((eun == 0xFFFF) || (sec == 0xFFFF)) {
        return -1;
    }

    /* 获取一个sector标签信息，以查看该sector的状态(sci)  */
    if (__ftl_sci_read(p_ftl, eun, sec, &sci) >= 0) {
         if ((sci.stat_start == __FTL_SECTOR_STAT_START) &&
             (sci.stat_data  == __FTL_SECTOR_STAT_DATA)) {

            status = __FTL_SECTOR_USED;
         }
    }

    /* find data in it */
    if (status == __FTL_SECTOR_USED) {
        return 0;
    }

    return -1;
}


/*
 * Get the unit number into which we can write for this block.
 * Make it available if it isn't already
 */
static int __ftl_readunit_find (am_ftl_serv_t *p_ftl,
                                unsigned       lbn,
                                uint16_t      *p_eun,
                                uint16_t      *p_sec)
{
    uint16_t logic_sec;
    uint16_t logic_block;

    size_t   log_blocks  = p_ftl->p_info->nb_log_blocks;

    int      i, j;

    logic_block = lbn / p_ftl->sectors_per_blk;
    logic_sec   = lbn % p_ftl->sectors_per_blk;

    /* check if in the log buffer */
    for (i = 0; i < log_blocks; i++) {
        if (p_ftl->p_log_buf[i].lbn == logic_block) {  /* find it */
            break;
        }
    }

    if (i < log_blocks) {   /* find it in log buffer */
         for (j = p_ftl->p_log_buf[i].used - 1; j >= 0; j--) {

             /* at start up, if the logic_sector error, the p_map[j] is 0xFF */
             if (p_ftl->p_log_buf[i].p_map[j] == logic_sec) {

                 /* It's valid */
                 if (__ftl_readunit_check(p_ftl,
                                          p_ftl->p_log_buf[i].pbn,
                                          j) == 0) {

                     *p_eun = p_ftl->p_log_buf[i].pbn;
                     *p_sec = j;

                     return 0;
                 }
             }
         }
    }

    /* can't find it in log buffer, read out from directly buffer */
    if (p_ftl->p_eun_table[logic_block] != 0xFFFF) {

        /* It's valid */
        if (__ftl_readunit_check(p_ftl,
                                 p_ftl->p_eun_table[logic_block],
                                 logic_sec) == 0) {        /* directly map */

            *p_eun = p_ftl->p_eun_table[logic_block];
            *p_sec = logic_sec;

            return 0;
        }
    }

    /* can't find it */
    return -1;
}

/******************************************************************************/

int __ftl_format_all (am_ftl_serv_t *p_ftl)
{
    unsigned int i;

    for (i = 0; i < p_ftl->nb_blocks; i++) {
        if (__ftl_block_erase(p_ftl, i) < 0) {
            __free_block_set(p_ftl, i, 0);    /* mask it as not free */
        }
    }

    return 0;
}

 
/*******************************************************************************
    Public fuctions
*******************************************************************************/

/* 读取一块数据，长度必须为逻辑块大小  */
int am_ftl_read (am_ftl_handle_t handle, unsigned int lbn, void *p_buf)
{
    am_ftl_serv_t *p_ftl = (am_ftl_serv_t *)handle;

    uint16_t read_eun = 0xFFFF;
    uint16_t read_sec = 0xFFFF;
 
    if (handle == NULL) {
        return -1;
    }

    if (lbn > p_ftl->max_lbn) {
        AM_DBG_INFO("FTL:The lbn (%d) is exceed the max val (%d)",
                lbn,
                p_ftl->max_lbn);

        return -1;
    }

    /* find a space to read */
    if (__ftl_readunit_find(p_ftl, lbn, &read_eun, &read_sec) >= 0) {

        return __ftl_data_read(p_ftl, read_eun, read_sec, p_buf);

    } else {
        AM_DBG_INFO("Can't find the lbn %d to read!\n", lbn);
    }

    /* the requested block is not on the media, return all 0x00 */
    memset(p_buf, 0, p_ftl->p_info->logic_blk_size);

    return 0;
}

/******************************************************************************/

int am_ftl_write (am_ftl_handle_t handle, unsigned int lbn, void *p_buf)
{
    am_ftl_serv_t *p_ftl = (am_ftl_serv_t *)handle;

    uint16_t         write_eun;
    uint16_t         write_sec;
    struct __ftl_sci sci;

    if (handle == NULL) {
        return -1;
    }

    if (lbn > p_ftl->max_lbn) {
        AM_DBG_INFO("FTL:The lbn (%d) is exceed the max val (%d)",
                lbn,
                p_ftl->max_lbn);

        return -1;
    }
 
    /* find a space to write */
    if (__ftl_writeunit_find(p_ftl, lbn, &write_eun, &write_sec) < 0) {

        AM_DBG_INFO("am_ftl_write(): Cannot find block to write to\n");

        /* If we _still_ haven't got a block to use, we're screwed */
        return -1;
    }

    AM_DBG_INFO("Write lbn %d to (%d, %d)!\n", lbn, write_eun, write_sec);

    /* Set the TAG to __FTL_SECTOR_USED */
    memset(&sci, 0xFF, sizeof(struct __ftl_sci));

    sci.stat_start = __FTL_SECTOR_STAT_START;

    /* the logic sector */
    sci.locgic_sec0 = sci.locgic_sec1 = lbn % p_ftl->sectors_per_blk;

    __ftl_sci_write(p_ftl, write_eun, write_sec, &sci);

#if 0
    /* check the data is 0xFF */
    __ftl_data_read(p_ftl, write_eun, write_sec, p_ftl->p_wr_buf);

    if (__ftl_memcmpb(p_ftl->p_wr_buf,
                      0xFF,
                      p_ftl->p_info->logic_blk_size) != 0) {
        AM_DBG_INFO("The block is error ! write failed! \n");
        while(1);
    }
#endif

    __ftl_data_write(p_ftl, write_eun, write_sec, p_buf);

    /* otherwise the data is invalid */
    sci.stat_data = __FTL_SECTOR_STAT_DATA;
    __ftl_sci_write(p_ftl, write_eun, write_sec, &sci);

    return 0;
}
 
/******************************************************************************/

am_ftl_handle_t am_ftl_init (am_ftl_serv_t          *p_ftl,
                             const am_ftl_info_t    *p_info,
                             am_mtd_handle_t         mtd_handle)
{
    if ((p_ftl                 == NULL)  ||
        (p_info                == NULL)  ||
        (p_info->p_buf         == NULL)  ||
        (p_info->nb_log_blocks < 2)      ||
        (mtd_handle == NULL)) {

        return NULL;
    }

    p_ftl->p_info = p_info;
    p_ftl->mtd    = mtd_handle;

    /* initial the memory allocation */
    if (__ftl_init(p_ftl) < 0) {
         AM_DBG_INFO("Initial Failed!\r\n");
         return NULL;
    }
 
    if (__nfl_mount(p_ftl) < 0) {
        AM_DBG_INFO("Mount Failed! Try to format!\r\n");

        if (__ftl_format_all(p_ftl) < 0) {

            AM_DBG_INFO("format failed!\r\n");
            return NULL;
        }

        if (__nfl_mount(p_ftl) < 0) {
            AM_DBG_INFO("Mount again Failed!\r\n");
            return NULL;
        }
    }

    return p_ftl;
}

/******************************************************************************/
size_t am_ftl_max_lbn_get (am_ftl_serv_t *p_ftl)
{
    if (p_ftl) {
        return (p_ftl->max_lbn + 1);
    }
    return 0;
}

/*******************************************************************************
    NVRAM Drivers
*******************************************************************************/

/* program one logic block */
am_local int __ftl_nvram_program_data (am_ftl_serv_t        *p_ftl,
                                       uint32_t              subaddr,
                                       uint8_t              *p_buf,
                                       uint32_t              len,
                                       am_bool_t             is_read)
{
    size_t lbn_size  = p_ftl->p_info->logic_blk_size;
    size_t lbn       = subaddr / lbn_size;
    size_t off       = subaddr % lbn_size;

    uint16_t read_eun  = 0xFFFF;
    uint16_t read_sec  = 0xFFFF;

    uint16_t write_eun = 0xFFFF;
    uint16_t write_sec = 0xFFFF;

    if (len == lbn_size) {

        /* just right a logic block */
        return (is_read) ? am_ftl_read(p_ftl, lbn, p_buf)
                         : am_ftl_write(p_ftl, lbn, p_buf);

    }

    if (is_read) {

        /* find the space to read */
        if (__ftl_readunit_find(p_ftl, lbn, &read_eun, &read_sec) >= 0) {

            AM_DBG_INFO("read %d nbytes from %d\n", len,
                        __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, read_eun)
                        + ((read_sec + p_ftl->sectors_hdr) * lbn_size)
                        + off);

            /* read the data */
            return am_mtd_read(p_ftl->mtd,
                               __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, read_eun)
                               + ((read_sec + p_ftl->sectors_hdr) * lbn_size)
                               + off,
                               p_buf,
                               len);
        } else {
            AM_DBG_INFO("Can't find the lbn %d to read!\n", lbn);
            memset(p_buf, 0x00, len);
            return 0;
        }
    } else {

        struct __ftl_sci sci;

        /* find a space to write */
        if (__ftl_writeunit_find(p_ftl, lbn, &write_eun, &write_sec) < 0) {

            AM_DBG_INFO("am_ftl_write(): Cannot find block to write to\n");

            /* If we _still_ haven't got a block to use, we're screwed */
            return -1;
        }

        /*
         * the address of read data may be changed while looking for write space
         * the sector of write unit haven't be written at here. so will not find
         * this unit while looking for read data.
         */
        if (__ftl_readunit_find(p_ftl, lbn, &read_eun, &read_sec) >= 0) {

            /* read the data before write */
            __ftl_data_read(p_ftl, read_eun, read_sec, p_ftl->p_wr_buf);

        } else {
            memset(p_ftl->p_wr_buf, 0x00, lbn_size);
        }

        AM_DBG_INFO("write %d nbytes to %d\n", len,
                    __FTL_PHY_BLOCK_START_ADDR_GET(p_ftl, write_eun)
                    + ((write_sec + p_ftl->sectors_hdr) * lbn_size)
                    + off);

        /* change the value to write */
        memcpy(p_ftl->p_wr_buf + off, p_buf, len);

        /* Set the TAG to __FTL_SECTOR_USED */
        memset(&sci, 0xFF, sizeof(struct __ftl_sci));

        sci.stat_start = __FTL_SECTOR_STAT_START;

        /* the logic sector */
        sci.locgic_sec0 = sci.locgic_sec1 = lbn % p_ftl->sectors_per_blk;

        __ftl_sci_write(p_ftl, write_eun, write_sec, &sci);

        __ftl_data_write(p_ftl, write_eun, write_sec, p_ftl->p_wr_buf);

        /* the data data save complete */
        sci.stat_data = __FTL_SECTOR_STAT_DATA;
        __ftl_sci_write(p_ftl, write_eun, write_sec, &sci);
    }

    return AM_OK;
}

/******************************************************************************/

/* ep24cxx read or write */
am_local int __ftl_nvram_rw (am_ftl_serv_t    *p_ftl,
                             int               start,
                             uint8_t          *p_buf,
                             size_t            len,
                             am_bool_t         is_read)
{
    uint32_t maxsize = (p_ftl->max_lbn + 1) * p_ftl->p_info->logic_blk_size;
    uint32_t maxaddr = maxsize - 1;

    uint32_t page    = p_ftl->p_info->logic_blk_size;
    uint32_t len_tmp;
    int      ret;

    if (p_ftl == NULL) {
        return -AM_EINVAL;
    }

    /* start address beyond this ftl's capacity */
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

    /* write the unaligned data of the start */
    len_tmp =  AM_ROUND_DOWN(start + page, page) - start;
    if (len < len_tmp) {
        len_tmp = len;
    }

    ret = AM_OK;
    ret = __ftl_nvram_program_data(p_ftl, start, p_buf, len_tmp, is_read);

    if (ret < AM_OK) {
        return -AM_EIO;
    }

    len   -= len_tmp;
    start += len_tmp;
    p_buf += len_tmp;

    /* write the rest data */

    while (len) {
        len_tmp = len > page ? page : len;
        ret = __ftl_nvram_program_data(p_ftl, start, p_buf, len_tmp, is_read);

        if (ret < AM_OK) {
            return -AM_EIO;
        }

        len   -= len_tmp;
        start += len_tmp;
        p_buf += len_tmp;
    }

    return ret;
}

/******************************************************************************/

/* pfn_nvram_get function driver */
am_local int __ftl_nvram_get (void            *p_drv,
                              int              offset,
                              uint8_t         *p_buf,
                              int              len)
{
    return __ftl_nvram_rw(p_drv, offset, (uint8_t *)p_buf, len, AM_TRUE);
}

/******************************************************************************/

/* pfn_nvram_set function driver */
am_local int __ftl_nvram_set (void            *p_drv,
                              int              offset,
                              uint8_t         *p_buf,
                              int              len)
{
    return __ftl_nvram_rw(p_drv, offset, (uint8_t *)p_buf, len, AM_FALSE);
}

/******************************************************************************/
am_local am_const struct am_nvram_drv_funcs  __g_ftl_nvram_drvfuncs = {
    __ftl_nvram_get,
    __ftl_nvram_set
};

/******************************************************************************/
int am_ftl_nvram_init (am_ftl_handle_t   handle,
                       am_nvram_dev_t   *p_dev,
                       const char       *p_name)
{
    if ((handle == NULL) || (p_dev == NULL) || (p_name == NULL)) {
        return -AM_EINVAL;
    }

    p_dev->p_next     = NULL;

    /* provide the standard nvram service */
    p_dev->p_funcs    = &__g_ftl_nvram_drvfuncs;
    p_dev->p_drv      = (void *)handle;
    p_dev->p_dev_name = p_name;

    return am_nvram_dev_register(p_dev);
}

/* end of file */
