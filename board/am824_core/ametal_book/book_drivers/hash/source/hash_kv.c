
#include "am_hwconf_microport.h"
#include "hash_kv.h"


#define NEXT_ADDR_OFFSET(kv)  0
#define KEY_OFFSET(kv)        sizeof(hash_kv_addr_t)
#define VALUE_OFFSET(kv)      (sizeof(hash_kv_addr_t) + (kv->key_size))
#define RECORD_SIZE(kv)       ((kv->key_size) + (kv->value_size) + sizeof(hash_kv_addr_t))

#define GET_KEY(kv, buff)               ((char*)buff+KEY_OFFSET(kv))
#define GET_VALUE(kv, buff)             ((char*)buff+VALUE_OFFSET(kv))
#define GET_NEXT_ADDR(kv, buff)         *(hash_kv_addr_t*)(buff)
#define SET_NEXT_ADDR(kv, buff, addr)   *(hash_kv_addr_t*)(buff) = addr;

#define DIRTY_THRESHOLD        50

typedef struct _hash_kv_header_t {
    uint32_t magic;                   /* 标识       */
    uint32_t version;                 /* 版本号  */
    uint32_t reserved;                /* 保留       */
    uint16_t entry_nr;                /* 哈希表大小 */
    uint16_t key_size;                /* 键的长度      */
    uint16_t value_size;              /* 值得长度            */
    hash_kv_addr_t free_record_head;  /* 空闲记录表的表头  */
} __hash_kv_header_t;

#define FHT_MAGIC 0x11223344
#define FHT_VERSION 0x00001000
#define INVALID_RECORD_ADDR 0

/*******************************************************************************
    本地函数
*******************************************************************************/

static void __hash_kv_header_init (__hash_kv_header_t *p_header,
                                   uint16_t            entry_nr,
                                   uint16_t            key_size,
                                   uint16_t            value_size)
{
    p_header->magic            = FHT_MAGIC;
    p_header->version          = FHT_VERSION;
    p_header->entry_nr         = entry_nr;
    p_header->key_size         = key_size;
    p_header->value_size       = value_size;
    p_header->free_record_head = INVALID_RECORD_ADDR;

    return;
}

/******************************************************************************/

static int __hash_kv_write_at (FILE           *fp,
                               char           *buff,
                               size_t          record_size,
                               hash_kv_addr_t  addr)
{
    if (fseek(fp, addr, SEEK_SET) != 0) {
        return -1;
    }

    if (fwrite(buff, 1, record_size, fp) != (int)record_size) {
        return -1;
    }

    return 0;
}

/******************************************************************************/
/* 读取一条hash记录 */
static int __hash_kv_read_at (FILE          *fp,
                              char          *buff,
                              size_t         record_size,
                              hash_kv_addr_t addr)
{
    if (fseek(fp, addr, SEEK_SET) != 0) {
        return -1;
    }

    if (fread(buff, 1, record_size, fp) != (int)record_size) {
        return -1;
    }

    return 0;
}

/******************************************************************************/
static int __hash_kv_do_flush (hash_kv_t *p_db)
{
    FILE               *fp = p_db->fp;
    __hash_kv_header_t  header;

    if (p_db->dirty == 0) {
        return 0;
    }

    if (__hash_kv_read_at(fp, (char*)&header, sizeof(header), 0) >= 0) {

        if (header.free_record_head != p_db->free_record_head) {

            header.free_record_head = p_db->free_record_head;
            if (__hash_kv_write_at(fp, (char*)&header, sizeof(header), 0) >= 0) {
                p_db->dirty = 0;
            }
        }
    }

    fflush(fp);

    return 0;
}

/******************************************************************************/
static int __hash_kv_do_close (hash_kv_t *p_db)
{
    __hash_kv_do_flush(p_db);
    fclose(p_db->fp);
    p_db->fp = NULL;

    return 0;
}

/******************************************************************************/

static void __hash_kv_dirty_inc (hash_kv_t *p_db)
{
    p_db->dirty++;
    if(p_db->dirty > DIRTY_THRESHOLD) {
        __hash_kv_do_flush(p_db);
    }
}

/******************************************************************************/

static int __hash_kv_do_create (hash_kv_t *p_db)
{
    return_value_if_fail(p_db != NULL && p_db->fp != NULL, -1);

    FILE               *fp = p_db->fp;
    __hash_kv_header_t  header;
    hash_kv_addr_t      buff[64];

    int ret;
    int n;
    int i;

    __hash_kv_header_init(&header,
                           p_db->size,
                           p_db->key_size,
                           p_db->value_size);

    if (fseek(fp, 0, SEEK_SET) != 0) {
        return -1;
    }

    ret = fwrite(&header, sizeof(header), 1, fp);
    p_db->free_record_head = header.free_record_head;

    return_value_if_fail(ret == 1, -1);

    memset(buff, 0x00, sizeof(buff));

    /* 将所有entry清零  */
    n = header.entry_nr/ AM_NELEMENTS(buff);
    for(i = 0; i < n; i++) {
        ret = fwrite(buff, sizeof(buff), 1, fp);
        return_value_if_fail(ret == 1, -1);
    }

    n = header.entry_nr % AM_NELEMENTS(buff);
    ret = fwrite(buff, sizeof(hash_kv_addr_t), n, fp);
    return_value_if_fail(ret == n, -1);

    assert(ftell(fp) == (header.entry_nr * sizeof(hash_kv_addr_t) + sizeof(header)));

    __hash_kv_dirty_inc(p_db);
    __hash_kv_do_flush(p_db);

    return 0;
}

/******************************************************************************/

static int __hash_kv_do_open (hash_kv_t  *p_db)
{
    return_value_if_fail(p_db != NULL && p_db->fp != NULL, -1);

    FILE               *fp = p_db->fp;
    __hash_kv_header_t  header;

    if (fseek(fp, 0, SEEK_SET) != 0) {
        return -1;
    }

    ENSURE(1 == fread(&header, sizeof(header), 1, fp));

    if(header.magic       !=  FHT_MAGIC        ||
       header.entry_nr    !=  p_db->size     ||
       header.key_size    !=  p_db->key_size ||
       header.value_size  !=  p_db->value_size) {

        return -1;
    }

    p_db->free_record_head = header.free_record_head;

    return 0;
}

/******************************************************************************/

/* 根据 Key 值计算 hash 表位置  */
uint16_t __hash_kv_hash_offset(hash_kv_t *p_db, const void *key)
{
    int loc = p_db->pfn_hash(key) % p_db->size;
    return sizeof(__hash_kv_header_t) + loc * sizeof(hash_kv_addr_t);
}

/******************************************************************************/

static hash_kv_addr_t __hash_kv_first_record_addr_get(
        hash_kv_t  *p_db,
        const void *key)
{
    hash_kv_addr_t addr   = INVALID_RECORD_ADDR;
    int            offset = __hash_kv_hash_offset(p_db, key);

    if (fseek(p_db->fp, offset, SEEK_SET) != 0) {
        return INVALID_RECORD_ADDR;
    }

    if (fread(&addr, sizeof(addr), 1, p_db->fp) != 1) {
        return INVALID_RECORD_ADDR;
    }

    return addr;
}

/******************************************************************************/

static int __hash_kv_first_record_addr_set (
        hash_kv_t *p_db,
        const void            *key,
        hash_kv_addr_t         addr) {

    int offset = __hash_kv_hash_offset(p_db, key);

    if (fseek(p_db->fp, offset, SEEK_SET) != 0) {
        return -1;
    }

    if (fwrite(&addr, sizeof(addr), 1, p_db->fp) != 1) {
        return -1;
    }

    return 0;
}

/******************************************************************************/
/* 初始化一条数据库记录 */
static char *__hash_kv_record_init (hash_kv_t *p_db,
                                    char                  *buff,
                                    const void            *p_key,
                                    const void            *p_value,
                                    hash_kv_addr_t         next)
{
    memcpy(buff+NEXT_ADDR_OFFSET(kv), &next, sizeof(next));
    memcpy(buff+KEY_OFFSET(kv), p_key, p_db->key_size);

    if ((p_value != NULL) && (p_db->value_size != 0)) {
        memcpy(buff + VALUE_OFFSET(p_db), p_value, p_db->value_size);
    }

    return buff;
}



/*******************************************************************************
    公共函数
*******************************************************************************/

int hash_kv_init (hash_kv_t      *p_db,
                  uint16_t        size,
                  uint16_t        key_size,
                  uint16_t        value_size,
                  hash_kv_func_t  pfn_hash,
                  const char      *file_name)
{
    return_value_if_fail(size > 10 && key_size > 0, -1);
    return_value_if_fail(p_db != NULL && pfn_hash != NULL, -1);
	
    p_db->fp          = NULL;
    p_db->pfn_hash    = pfn_hash;
    p_db->size        = size;
    p_db->key_size    = key_size;
    p_db->value_size  = value_size;

    p_db->dirty       = 0;
    p_db->p_file_name = file_name;

    /* 打开文件  */
    p_db->fp = fopen(file_name, "r+b");

    if (p_db->fp != NULL) {

        if (__hash_kv_do_open(p_db) != 0) {
            return __hash_kv_do_create(p_db);
        }

    } else {
        p_db->fp = fopen(file_name, "w+b");
        return __hash_kv_do_create(p_db);
    }

    return 0;
}

/******************************************************************************/
int hash_kv_add (hash_kv_t  *p_db,
                 const void *key,
                 const void *value)
{
    return_value_if_fail((p_db != NULL) && (p_db->fp != NULL) && (key != NULL), -1);

    int    ret = 0;
    FILE  *fp  = p_db->fp;

    char buff[MAX_RECORD_SIZE];

    size_t         record_size = RECORD_SIZE(p_db);
    hash_kv_addr_t first_addr  = __hash_kv_first_record_addr_get(p_db, key);
    hash_kv_addr_t addr;

#if 1
    addr = first_addr;
    while (addr != INVALID_RECORD_ADDR) {
        if (__hash_kv_read_at(fp, buff, record_size, addr) < 0) {
            return -1;
        }

        /* 找到该记录，*/
        if(memcmp(GET_KEY(kv, buff), key, p_db->key_size) == 0) {

            /* 更新值 */
            if ((value != NULL) && (p_db->value_size != 0)) {
                memcpy(buff + VALUE_OFFSET(p_db), value, p_db->value_size);
            }

            __hash_kv_dirty_inc(p_db);

            return __hash_kv_write_at(fp, buff, record_size, addr);

        } else {
            addr = GET_NEXT_ADDR(kv, buff);
        }
    }
#endif

    /* 该关键字的记录没有找到，寻找一个空闲位置存放，优先使用被删除的区域  */
    if (p_db->free_record_head) {
        addr = p_db->free_record_head;

        if (__hash_kv_read_at(fp, buff, record_size, addr) < 0) {
            return -1;
        }

        p_db->free_record_head = GET_NEXT_ADDR(kv, buff);
    } else {
        ret = fseek(fp, 0, SEEK_END);
        addr = ftell(fp);
    }

    /* 写入该条记录时， next addr区域写入的是 first_addr ，即插入单项链表的头部  */
    ret = __hash_kv_write_at(fp,
                             __hash_kv_record_init(p_db, buff, key, value, first_addr),
                             record_size,
                             addr);
		
    if (ret < 0) {
        return ret;
    }

    if (__hash_kv_first_record_addr_set(p_db, key, addr) < 0) {
        return -1;
    }

    __hash_kv_dirty_inc(p_db);

    return 0;
}

/******************************************************************************/

int hash_kv_search (hash_kv_t    *p_db,
                    const void   *p_key,
                    void         *p_value)
{
    return_value_if_fail((p_db != NULL) && (p_db->fp != NULL) && (p_key != NULL), -1);

    FILE   *fp = p_db->fp;
    char    buff[MAX_RECORD_SIZE];
    size_t  record_size = RECORD_SIZE(p_db);

    assert(record_size < MAX_RECORD_SIZE);

    hash_kv_addr_t addr = __hash_kv_first_record_addr_get(p_db, p_key);

    while (addr != INVALID_RECORD_ADDR) {

        int ret = __hash_kv_read_at(fp, buff, record_size, addr);

        if (ret < 0) {
            return ret;
        }

        if (memcmp(GET_KEY(p_db, buff), p_key, p_db->key_size) == 0) {
            if ((p_value != NULL) && (p_db->value_size != 0)) {
                memcpy(p_value, buff + VALUE_OFFSET(p_db), p_db->value_size);
            }
            return 0;
        }else{
            addr = GET_NEXT_ADDR(kv, buff);
        }
    }
    return -1;
}

/******************************************************************************/
int hash_kv_del (hash_kv_t   *p_db,
                             const void              *p_key)
{
    return_value_if_fail(p_db != NULL && p_db->fp != NULL && p_key != NULL, -1);

    FILE          *fp = p_db->fp;
    char           buff[MAX_RECORD_SIZE];

    size_t         record_size = RECORD_SIZE(p_db);;
    hash_kv_addr_t addr        = __hash_kv_first_record_addr_get(p_db, p_key);

    hash_kv_addr_t prev_addr = addr;
    hash_kv_addr_t first_addr = addr;

    while (addr != INVALID_RECORD_ADDR) {

        if (__hash_kv_read_at(fp, buff, record_size, addr) < 0) {
            return -1;
        }

        if (memcmp(GET_KEY(p_db, buff), p_key, p_db->key_size) == 0) {

            hash_kv_addr_t next_addr = GET_NEXT_ADDR(kv, buff);

            /* 更新空闲表 */
            SET_NEXT_ADDR(kv, buff, p_db->free_record_head);

            if (__hash_kv_write_at(fp, buff, record_size, addr) < 0) {
                return -1;
            }

            p_db->free_record_head = addr;

            /* 更新该条 hash 表 */
            if (addr == first_addr) {
                if (__hash_kv_first_record_addr_set(p_db, p_key, next_addr) < 0) {
                    return -1;
                }
            } else{
                if (__hash_kv_read_at(fp, buff, record_size, prev_addr) < 0) {
                    return -1;
                }

                SET_NEXT_ADDR(kv, buff, next_addr);
                if (__hash_kv_write_at(fp, buff, record_size, prev_addr) < 0) {
                    return -1;
                }
            }

            __hash_kv_dirty_inc(p_db);

            return 0;

        } else {
            prev_addr = addr;
            addr = GET_NEXT_ADDR(kv, buff);
        }
    }

    return -1;
}

/******************************************************************************/
int hash_kv_reset (hash_kv_t *p_db)
{
    return_value_if_fail(p_db != NULL && p_db->fp != NULL, -1);

    __hash_kv_do_close(p_db);

    funlink(p_db->p_file_name);

    p_db->fp = fopen(p_db->p_file_name, "w+b");
    if (p_db->fp == NULL) {
        return -1;
    }

    return __hash_kv_do_create(p_db);      /* 重新创建hash表  */
}

/******************************************************************************/
int hash_kv_flush (hash_kv_t *p_db)
{
    return_value_if_fail(p_db != NULL && p_db->fp != NULL, -1);

    return __hash_kv_do_flush(p_db);
}

/******************************************************************************/
int hash_kv_deinit (hash_kv_t *p_db)
{
    return_value_if_fail(p_db != NULL && p_db->fp != NULL, -1);

    __hash_kv_do_close(p_db);

    return 0;
}

/* end of file */
