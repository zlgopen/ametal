#ifndef BLOCK_DEV_H
#define BLOCK_DEV_H

#include "types_def.h"

BEGIN_C_DECLS

//块设备单块的大小，一般选择256的整数倍。
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 256
#endif//BLOCK_SIZE

typedef unsigned short block_num_t;

struct _block_dev_t;
typedef struct _block_dev_t block_dev_t;

typedef block_num_t (*block_dev_get_block_nr_t)(block_dev_t* dev);
typedef am_bool_t (*block_dev_read_block_t)(block_dev_t* dev, block_num_t block_num, void* buff);
typedef am_bool_t (*block_dev_write_block_t)(block_dev_t* dev, block_num_t block_num, const void* buff);
typedef void   (*block_dev_close_t)(block_dev_t* dev);


/*
* 块设备的接口。
*/

struct _block_dev_t {
	block_dev_get_block_nr_t   get_block_nr;
	block_dev_write_block_t    write_block;
	block_dev_read_block_t     read_block;
	block_dev_close_t          close;
};

/**
* \brief 获取块设备中总共的块数。
*
* \param[in] dev      ： 块设备对象
*
* \retval 块设备中总共的块数
*/
am_static_inline block_num_t block_dev_get_block_nr(block_dev_t* dev) {
	return_value_if_fail(dev != NULL && dev->get_block_nr != NULL, 0);

	return dev->get_block_nr(dev);
}

/**
* \brief 从块设备中读取指定块的数据。
*
* \param[in] dev        ： 块设备对象
* \param[in] block_num  ： 块号
* \param[out] buff      ： 返回数据的buff，由外面提供不小于BLOCK_SIZE大小的空间。
*
* \retval TRUE  成功
* \retval FALSE 失败
*/
am_static_inline am_bool_t block_dev_read_block(block_dev_t* dev, block_num_t block_num, void* buff) {
	return_value_if_fail(dev != NULL && dev->read_block != NULL 
			&& block_num < block_dev_get_block_nr(dev), FALSE);

	return dev->read_block(dev, block_num, buff);
}

/**
* \brief 向块设备中写入指定块的数据。
*
* \param[in] dev        ： 块设备对象
* \param[in] block_num  ： 块号
* \param[in] buff       ： 待写入数据的buff，大小不小于BLOCK_SIZE。
*
* \retval TRUE  成功
* \retval FALSE 失败
*/
am_static_inline am_bool_t block_dev_write_block(block_dev_t* dev, block_num_t block_num, const void* buff) {
	return_value_if_fail(dev != NULL && dev->write_block != NULL 
			&& block_num < block_dev_get_block_nr(dev), FALSE);

	return dev->write_block(dev, block_num, buff);
}

/**
* \brief 关闭块设备，释放相关资源。
*
* \param[in] dev        ： 块设备对象
*
* \retval 无
*/
am_static_inline void block_dev_close(block_dev_t* dev) {
	return_if_fail(dev != NULL && dev->close != NULL);

	dev->close(dev);
}

END_C_DECLS

#endif//BLOCK_DEV_H

