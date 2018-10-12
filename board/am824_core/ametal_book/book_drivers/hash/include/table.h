#ifndef __TABLE_H
#define __TABLE_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _table_t;
typedef struct _table_t table_t;

typedef void* table_key_t;
typedef void* table_value_t;
typedef const void* table_const_key_t;
typedef const void* table_const_value_t;
typedef int (*table_visit_func_t)(void* ctx, table_const_key_t key, table_const_value_t value);

typedef int (*table_get_t)(table_t* table, table_const_key_t key, table_value_t value);
typedef int (*table_set_t)(table_t* table, table_const_key_t key, table_const_value_t value);
typedef int (*table_remove_t)(table_t* table, table_const_key_t key);
typedef int (*table_reset_t)(table_t* table);
typedef int (*table_close_t)(table_t* table);

/* KV 数据库的接口*/
struct _table_t {
    table_get_t     get;
    table_set_t     set;
    table_remove_t  remove;
    table_reset_t   reset;
};

/**
 * \brief 获取Key/Value。
 *
 * \param[in] table  ： 数据库对象
 * \param[in] key    ： 键
 * \param[out] value ： 值。由调用者分配不小于value_size（为0时，该值为NULL）的空间。
 *
 * \retval TRUE  成功
 * \retval FALSE 失败
 */
am_static_inline
int table_get (table_t* table, table_const_key_t key, table_value_t value)
{
	if ((table == NULL) || (table->get == NULL)) {
		return -AM_EINVAL;
	}
    return table->get(table, key, value);
}

/**
 * \brief 设置Key/Value。
 *
 * \param[in] table      ： 数据库对象
 * \param[in] key        ： 键
 * \param[in] value      ： 值
 *
 * \retval TRUE  成功
 * \retval FALSE 失败
 */
am_static_inline
int table_set (table_t* table, table_const_key_t key, table_const_key_t value)
{
	if ((table == NULL) || (table->set == NULL)) {
		return -AM_EINVAL;
	}
    return table->set(table, key, value);
}

/**
* \brief 删除指定的Key。
*
* \param[in] table      ： 数据库对象
* \param[in] key        ： 键
*
* \retval TRUE  成功
* \retval FALSE 失败
*/
am_static_inline
int table_remove(table_t* table, table_const_key_t key)
{
	if ((table == NULL) || (table->remove == NULL)) {
		return -AM_EINVAL;
	}
    return table->remove(table, key);
}

/**
* \brief 清除全部记录
*
* \param[in] table      ： 数据库对象
* \param[in] key        ： 键
*
* \retval TRUE  成功
* \retval FALSE 失败
*/
am_static_inline
int table_reset(table_t* table)
{
	if ((table == NULL) || (table->reset == NULL)) {
		return -AM_EINVAL;
	}
    return table->reset(table);
}

#ifdef __cplusplus
}
#endif

#endif  // __TABLE_H

