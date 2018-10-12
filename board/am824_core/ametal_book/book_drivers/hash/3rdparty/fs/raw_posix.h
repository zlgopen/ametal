
#ifndef RAW_POSIX_H
#define RAW_POSIX_H

#include "block_dev_ftl.h"

BEGIN_C_DECLS


typedef struct _raw_file_t {
    const char* file_name;
    uint16_t start_block;
    uint16_t block_nr;
    int32_t cursor;
    size_t size;
}file_t;

/*除了raw_posix_init之外，一般不要直接调用raw_开头的函数，而是调用标准文件API。*/
FILE*  raw_fopen(const char* path, const char* mode);
size_t raw_fread(void *ptr, size_t size, size_t nitems, FILE *stream);
size_t raw_fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream);
int    raw_fclose(FILE *fp);

int  raw_fseek(FILE *stream, long offset, int whence);
long raw_ftell(FILE *stream);
void raw_frewind(FILE *stream);
int  raw_feof(FILE *stream);
int  raw_fflush(FILE *stream);
int  raw_fremove(const char* path);
am_bool_t raw_fexist(const char* path);
size_t raw_get_size(FILE *stream);

/**
 * \brief 为了模拟POSIX API，我们需要一个全局的块设备对象，本函数用来初始化这个全局的块设备对象。
 * 在调用本函数之后，才能调用其它文件API。
 *
 * \param[in] dev       ： 块设备对象
 */
void raw_posix_init(block_dev_t* dev, file_t *p_files, size_t num);

/**
 * \brief ~初始化，关闭块设备。
 */
void raw_posix_deinit(void);

#define RAW_POSIX_API 1


#ifdef RAW_POSIX_API
#	define fopen(x, y)        raw_fopen(x, y)
#	define fclose(x)          raw_fclose(x)
#	define fread(x, y, z, w)  raw_fread(x, y, z, w)
#	define fwrite(x, y, z, w) raw_fwrite(x, y, z, w)
#	define fseek(x, y, z)     raw_fseek(x, y, z)
#	define ftell(x)           raw_ftell(x)
#	define rewind(x)          raw_frewind(x)
#	define feof(x)            raw_feof(x)
#	define fflush(x)          raw_fflush(x)
#	define funlink(x)         raw_fremove(x)
#endif//RAW_POSIX_API

#ifdef PC_TEST
#include "block_dev_ram.h"
static inline int init_debug_fs(block_num_t block_nr) {
	raw_posix_init(block_dev_create_mem(block_nr));
	
	return 0;
}

static inline int deinit_debug_fs() {
	raw_posix_deinit();

	return 0;
}
void raw_posix_debug();
#else
am_static_inline void raw_posix_debug() {
}
#endif//PC_TEST

END_C_DECLS

#endif//RAW_POSIX_H

