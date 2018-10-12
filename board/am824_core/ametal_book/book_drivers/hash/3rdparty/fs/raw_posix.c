#include "block_dev_ftl.h"
#include "raw_posix.h"

static block_dev_t *s_dev         = NULL;
static file_t      *__gp_files    = NULL;
static size_t       __g_files_num = 0;

void raw_posix_init(block_dev_t* dev, file_t *p_files, size_t num)
{
    s_dev         = dev;  // 块设备 信息
	__gp_files    = p_files; // 文件信息
    __g_files_num = num; // 文件数量
}

void raw_posix_deinit(void) {
	if (s_dev) {
		block_dev_close(s_dev);
		s_dev = NULL;
	}
}

static block_dev_t* dev_get() {
	assert(s_dev != NULL);
	return s_dev;
}

#define MAGIC 0xeaed1234
#define VERSION 0x00010000

typedef struct _file_header_t {
	uint32_t magic;
	uint32_t version;
	uint32_t reserved;
	uint32_t size;
}file_header_t;


// 获取块设备数量
static inline block_num_t block_num_of(size_t size) {
	return (block_num_t)(size/BLOCK_SIZE);
}

static unsigned char block_buff[BLOCK_SIZE];


static am_bool_t file_header_read(file_t* file, file_header_t* header) {
	block_dev_t* dev = dev_get();
	am_bool_t ret = block_dev_read_block(dev, file->start_block, block_buff);
	if(ret) {
		memcpy(header, block_buff, sizeof(file_header_t));
	}

	return ret;
}

static am_bool_t file_header_write(file_t*file, file_header_t* header) {
	block_dev_t* dev = dev_get();
	am_bool_t ret = block_dev_read_block(dev, file->start_block, block_buff);
	if(ret) {
		memcpy(block_buff, header, sizeof(file_header_t));
		ret = block_dev_write_block(dev, file->start_block, block_buff);
	}

	return ret;
}

// 清空块设备
static am_bool_t file_header_reset(file_t*file) {
	block_dev_t* dev = dev_get();
	memset(block_buff, 0x00, BLOCK_SIZE);

	return block_dev_write_block(dev, file->start_block, block_buff);
}

static size_t raw_transfer(file_t* file, am_bool_t read, size_t file_pos, void* buff, size_t len) {
	size_t ret = 0;
	size_t size = 0;
	size_t offset = 0;
	size_t pos = file_pos + sizeof(file_header_t);
	size_t end_pos = pos + len;
	block_dev_t* dev = dev_get();
	unsigned char* p = (unsigned char*)buff;
	block_num_t start = block_num_of(pos) + file->start_block;
	block_num_t end   = block_num_of(pos+len) + file->start_block;

	return_value_if_fail(end <= (file->start_block + file->block_nr), 0);

	for(block_num_t i = start; i <= end; i++) {
		if(i == start) {
			offset = pos%BLOCK_SIZE;
			size = BLOCK_SIZE - offset;
			
			// 
			if(len < size) {
				size = len;
			}
		}else if(i == end) {
			size = end_pos%BLOCK_SIZE;
			offset = 0;
		}else{
			offset = 0;
			size = BLOCK_SIZE;
		}
	
		if(read) {
			if(block_dev_read_block(dev, i, block_buff)) {
				memcpy(p, block_buff+offset, size);
				p += size;
				ret += size;
			}else{
				break;
			}
		}else{
			if(block_dev_read_block(dev, i, block_buff)) {
				memcpy(block_buff+offset, p, size);
				if(!block_dev_write_block(dev, i, block_buff)) {
					break;
				}
				p += size;
				ret += size;
			}else{
				break;
			}
		}
	}
	
	return ret;
}

static size_t raw_read_at(file_t* file, size_t pos, void* buff, size_t len) {
	return_value_if_fail(pos < file->size, 0);
	if((pos + len) > file->size) {
		len = file->size - pos;
	}
	
	return len > 0 ? raw_transfer(file, TRUE, pos, buff, len) : 0;
}

static size_t raw_write_at(file_t* file, size_t pos, const void* buff, size_t len) {
	if(pos > file->size) {
		pos = file->size;
	}
	size_t ret = raw_transfer(file, FALSE, pos, (void*)buff, len);
	if((pos + ret) > file->size) {
		file->size = pos + ret;
	}

	return ret;
}

file_t* raw_find(const char* path) {
	for(size_t i = 0; i < __g_files_num; i++) {
		file_t* iter = __gp_files + i;
		if(strcmp(iter->file_name, path) == 0) {
			return iter;	
		}
	}
	
	return NULL;
}

static void header_init(file_header_t* header, size_t size) {
	header->magic = MAGIC;
	header->version = VERSION;
	header->reserved = 0;
	header->size = size;
}

FILE* raw_fopen(const char* path, const char* mode) {
	return_value_if_fail(path != NULL && mode != NULL, NULL);
	file_t* file = raw_find(path);

	if(file != NULL) {
		file_header_t header;
		return_value_if_fail(file_header_read(file, &header), NULL);

		if(header.magic != MAGIC) {
			if(mode[0] == 'r') {
				return NULL;
			}

			header_init(&header, 0);
			return_value_if_fail(file_header_write(file, &header), NULL);
		}

		if(mode[0] == 'a') {
			file->size = header.size;
			file->cursor = header.size;
		}else{
		    file->size = header.size;    /* 文件内容保持  */
			file->cursor = 0;
		}
	}

	return (FILE*)file;
}

int raw_fclose(FILE *stream) {
	return_value_if_fail(stream != NULL, -1);
	raw_fflush(stream);

	return 0;
}

int raw_fseek(FILE *stream, long offset, int whence) {
	file_t* file = (file_t*)stream;
	return_value_if_fail(stream != NULL, -1);
	long cursor = file->cursor;
	int32_t size = raw_get_size(stream);

	switch(whence) {
		case SEEK_SET: {
			cursor = offset;
			break;
		}
		case SEEK_CUR: {
			cursor = cursor + offset;
			break;
		}
		case SEEK_END: {
			cursor = size + offset;
			break;
		}
	}
	file->cursor = cursor < 0 ? 0 : cursor;
	file->cursor = file->cursor > size ? (size) : file->cursor;

	return 0;
}

long raw_ftell(FILE *stream) {
	file_t* file = (file_t*)stream;
	return_value_if_fail(stream != NULL, -1);

	return file->cursor;
}

void raw_frewind(FILE *stream) {
	file_t* file = (file_t*)stream;
	return_if_fail(stream != NULL);
	file->cursor = 0;
}

size_t raw_fread(void *ptr, size_t size, size_t nitems, FILE *stream) {
	file_t* file = (file_t*)stream;
	return_value_if_fail(stream != NULL, 0);

	int ret = raw_read_at(file, file->cursor, ptr, size * nitems);
	if(ret > 0) {
		file->cursor += ret;
	}

	return ret > 0 ? ret/size : 0;
}

size_t raw_fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream) {
	file_t* file = (file_t*)stream;
	return_value_if_fail(stream != NULL, 0);

	int ret = raw_write_at(file, file->cursor, ptr, size * nitems);
	if(ret > 0) {
		file->cursor += ret;
	}

	return ret > 0 ? ret/size : 0;
}

int raw_fflush(FILE* stream) {
	file_t* file = (file_t*)stream;
	return_value_if_fail(stream != NULL, 0);
	
	file_header_t header;
	return_value_if_fail(file_header_read(file, &header), -1);
	header.size = file->size;
	return_value_if_fail(file_header_write(file, &header), -1);

	return 0;
}

int raw_feof(FILE *stream) {
	file_t* file = (file_t*)stream;
	return_value_if_fail(stream != NULL, 0);
	int32_t size = raw_get_size(stream);

	return size > file->cursor ? 0 : 1;
}

size_t raw_get_size(FILE *stream) { 
	file_t* file = (file_t*)stream;
	return_value_if_fail(stream != NULL, 0);
	
	return file->size;
}

am_bool_t raw_fexist(const char* path) {
	return raw_find(path) != NULL;
}

int raw_fremove(const char* path) {
	return_value_if_fail(path != NULL, -1);
	file_t* file = raw_find(path);
	if(file != NULL) {
		file->size = 0;
		file_header_reset(file);
	}

	return -1;
}

#ifdef PC_TEST
#include <stdio.h>

void raw_posix_debug() {
	printf("BLOCK_SIZE=%d\n", BLOCK_SIZE);
	for(size_t i = 0; i < ELM_NR(supported_files); i++) {
		file_t* iter = supported_files+i;
		printf("i=%lu name=%s cursor=%d size=%lu (%d, %d)\n", 
				i, iter->file_name, iter->cursor, iter->size, (int)iter->start_block, (int)iter->block_nr);
	}
}
#endif//PC_TEST
