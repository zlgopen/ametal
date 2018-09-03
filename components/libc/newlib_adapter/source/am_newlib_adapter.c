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
 * \brief NEW LIB 适配器
 *
 * \internal
 * \par Modification History
 * - 1.00 17-07-24  tee, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_newlib_adapter.h"
#include <reent.h>
#include <sys/errno.h>
#include <time.h>
#include "fcntl.h"
#include "unistd.h"
#include "sys/stat.h"
#include "am_mem.h"
#include "am_time.h"

/*******************************************************************************
   Global Variables
*******************************************************************************/
typedef struct am_newlib_adapter {
    const am_newlib_adapter_ops_t *p_ops;    /** < \brief 驱动函数*/
    void                          *p_cookie; /** < \brief 驱动函数参数  */
} am_newlib_adapter_t;

static am_newlib_adapter_t  __g_adapter;

/*******************************************************************************
   Reentrant versions of system calls
*******************************************************************************/
int _close_r(struct _reent *ptr, int fd)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
int _execve_r(struct _reent *ptr, const char * name, char *const *argv, char *const *env)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
int _fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
int _fork_r(struct _reent *ptr)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
    pstat->st_mode = S_IFCHR;
    return 0;
    /* return "not supported" */
   // ptr->_errno = ENOTSUP;
   // return -1;
}

/******************************************************************************/
int _getpid_r(struct _reent *ptr)
{
    return 1;
}

/******************************************************************************/
int _isatty_r(struct _reent *ptr, int fd)
{

    if (fd >=0 && fd < 3) return 1;

    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
int _kill_r(struct _reent *ptr, int pid, int sig)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
/* Establish a new name for an existing file */
int _link_r(struct _reent *ptr, const char *old, const char *new)
{
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
/* Set position in a file */
_off_t _lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
    return 0;
}

/******************************************************************************/
int _mkdir_r(struct _reent *ptr, const char *name, int mode)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
/* Open a file */
int _open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
/* Read from a file */
_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    if (fd < 3) {

        if (__g_adapter.p_ops && __g_adapter.p_ops->pfn_console_receive) {
            return __g_adapter.p_ops->pfn_console_receive(__g_adapter.p_cookie,
                                                          buf,
                                                          nbytes);
        }
    }

    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
int _rename_r(struct _reent *ptr, const char *old, const char *new)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
/* Increase program data space */
void * _sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
    /* no use this routine to get memory */
    return NULL;
}

/******************************************************************************/
/* Status of a file (by name) */
int _stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
    /* return "not supported" */
    pstat->st_mode = S_IFCHR;
    return 0;
}

/******************************************************************************/
/* Timing information for current process */
_CLOCK_T_ _times_r(struct _reent *ptr, struct tms *ptms)
{
    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

/******************************************************************************/
/* Remove a file’s directory entry */
int _unlink_r(struct _reent *ptr, const char *file)
{
    ptr->_errno = ENOENT;
    return -1;
}

/******************************************************************************/
int _wait_r(struct _reent *ptr, int *status)
{
    ptr->_errno = ECHILD;
    return -1;
}

/******************************************************************************/
_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    if (fd < 3) {

        if (__g_adapter.p_ops && __g_adapter.p_ops->pfn_console_send) {
            return __g_adapter.p_ops->pfn_console_send(__g_adapter.p_cookie,
                                                       buf,
                                                       nbytes);
        }
    }

    /* return "not supported" */
    ptr->_errno = ENOTSUP;
    return -1;
}

///******************************************************************************/
//int _gettimeofday_r(struct _reent *ptr, struct timeval *__tp, void *__tzp)
//{
//    am_timespec_t  tp;
//
//    if (am_timespec_get(&tp) == AM_OK) {
//
//        if (__tp != NULL) {
//            __tp->tv_sec  = (time_t)tp.tv_sec;
//            __tp->tv_usec = tp.tv_nsec * 1000UL;
//        }
//
//        return tp.tv_sec;
//    }
//
//    /* return "not supported" */
//    ptr->_errno = ENOTSUP;
//    return -1;
//}

/******************************************************************************/
void *_malloc_r (struct _reent *ptr, size_t size)
{
    void* result;
    result = (void*)am_mem_alloc (size);
    if (result == NULL) {
        ptr->_errno = ENOMEM;
    }
    return result;
}

/******************************************************************************/
void *_realloc_r (struct _reent *ptr, void *old, size_t newlen)
{
    void* result;
    result = am_mem_realloc (old, newlen);
    if (result == NULL)
    {
        ptr->_errno = AM_ENOMEM;
    }

    return result;
}

/******************************************************************************/
void *_calloc_r (struct _reent *ptr, size_t size, size_t len)
{
    void* result;
    result = (void*)am_mem_calloc (size, len);
    if (result == NULL)
    {
        ptr->_errno = AM_ENOMEM;
    }

    return result;
}

/******************************************************************************/
void _free_r (struct _reent *ptr, void *addr)
{
    am_mem_free (addr);
}

/******************************************************************************/
void _exit (int status)
{
    while (1);
}

/******************************************************************************/
void _system(const char *s)
{
    /* not support this call */
    return;
}

/*******************************************************************************
   Public Functions
*******************************************************************************/
int am_newlib_adapter_init (const am_newlib_adapter_ops_t *p_ops,
                            void                          *p_cookie)
{

    __g_adapter.p_ops    = p_ops;
    __g_adapter.p_cookie = p_cookie;

    return AM_OK;
}

/* end of file */
