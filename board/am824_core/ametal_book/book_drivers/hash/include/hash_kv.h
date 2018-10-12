/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 基于哈希表的微型数据库
 *
 *
 * \internal
 * \par modification history:
 * - 1.00 18-05-02  tee, first implementation
 * \endinternal
 */

#ifndef __HASH_KV_H
#define __HASH_KV_H

#include "types_def.h"
#include "stdint.h"
#include "stdio.h"
#include "raw_posix.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 记录最大长度：key_size + value_size + sizeof(hash_kv_addr_t)*/
#define MAX_RECORD_SIZE  128

 

typedef uint32_t hash_kv_addr_t;
typedef int (*hash_kv_func_t)(const void *p_key);
typedef int (*hash_func_t)(const void* data, size_t len);	
typedef int (*compare_func_t)(const void* a, const void* b, size_t len);
typedef am_bool_t (*visit_func_t)(void* ctx, const void* data);

/* 基于文件哈希表的数据库*/
typedef struct _db_micro_hash_kv_t {
    FILE                       *fp;                  // 文件句柄
    const char                 *p_file_name;         // 文件名
    hash_kv_func_t              pfn_hash;            // 哈希函数
    uint32_t                    dirty;               // 数据修改计数器
    uint16_t                    size;                // 哈希表的大小，直接影响性能，一般设置为预计最大记录数的1/4到1倍。
    uint16_t                    key_size;            // 键的长度
    uint16_t                    value_size;          // 值的长度
    hash_kv_addr_t              free_record_head;    // 回收站链表的表头
} hash_kv_t;

/**
 * \brief 初始化文件哈希表，只有初始化之后，才能调用其它函数。
 *
 * \param[in] kv         ： 数据库对象
 * \param[in] size       ： 哈希表的大小，直接影响性能，一般设置为预计最大记录数的1/4到1倍。
 * \param[in] key_size   ： 键的长度 
 * \param[in] value_size ： 值的长度 
 * \param[in] hash       ： 哈希函数
 * \param[in] file_name  ：用以存储 哈希数据库相关信息的文件名
 *
 * \retval AW_OK  成功
 */
int hash_kv_init (hash_kv_t      *p_db,
                  uint16_t        size,
                  uint16_t        key_size,
                  uint16_t        value_size,
                  hash_kv_func_t  pfn_hash,
                  const char     *file_name);

/**
 * \brief 添加一条记录（包含 Key 和  Value）
 *
 * \param[in] p_db     ： 数据库对象
 * \param[in] key        ： 键
 * \param[in] value      ： 值
 *
 * \retval AW_OK  成功
 *
 * \note 设置时，应该保证当前关键字的记录不存在
 */
int hash_kv_add (hash_kv_t  *p_db,
                 const void *key,
                 const void *value);

/**
 * \brief 获取Key/Value。
 *
 * \param[in]  p_db    ： 数据库对象
 * \param[in]  key       ： 键
 * \param[out] value     ： 值。由调用者分配不小于value_size的空间。
 *
 * \retval AW_OK  成功
 * \retval 其它值   失败
 */
int hash_kv_search (hash_kv_t     *p_db,
                    const void    *p_key,
                    void          *p_value);
/**
 * \brief 删除指定的  Key 的记录
 *
 * \param[in] kv         ： 数据库对象
 * \param[in] key        ： 键
 *
 * \retval AW_OK  成功
 * \retval 其它值   失败
 */
int hash_kv_del (hash_kv_t   *p_db,
                             const void              *p_key);

/**
 * \brief 复位数据库，将删除所有数据
 *
 * \param[in] p_db ： 数据库对象
 *
 * \retval AW_OK  成功
 * \retval 其它值   失败
 */
int hash_kv_reset (hash_kv_t *p_db);

/**
 * \brief 如果数据库有修改，同步到块设备。
 *
 * \param[in] p_db  ： 数据库对象
 *
 * \retval AW_OK  成功
 * \retval 其它值   失败
 */
int hash_kv_flush (hash_kv_t *p_db);

/**
 * \brief 解初始化，关闭初始化时打开的文件，释放相关资源
 *
 * \param[in] p_db ： 数据库对象
 *
 * \retval AW_OK  成功
 * \retval 其它值   失败
 */
int hash_kv_deinit (hash_kv_t *p_db);


#ifdef __cplusplus
}
#endif

#endif /* HASH_KV_H */

/* end of file */

