#ifndef TYPES_DEF_H
#define TYPES_DEF_H 
#include <stdlib.h>
#include <string.h>
#include "ametal.h"
#ifdef PC_TEST
#include <stdio.h>
#define trace printf
#else
#define DEVICE_BUILD 1
#define assert(p)
#define FILE void
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define trace(...)
#define NDEBUG
#endif//PC_TEST

#include <assert.h>

typedef float float32_t;
typedef double floa64_t;
typedef void* pointer_t;

#if defined(WIN32) || defined(DEVICE_BUILD)

#else
#ifndef _INT8_T_DECLARED
typedef __int8_t int8_t;
#define _INT8_T_DECLARED
#endif
#ifndef _UINT8_T_DECLARED
typedef __uint8_t uint8_t;
#define _UINT8_T_DECLARED
#endif

#ifndef _INT16_T_DECLARED
typedef __int16_t int16_t;
#define _INT16_T_DECLARED
#endif
#ifndef _UINT16_T_DECLARED
typedef __uint16_t uint16_t;
#define _UINT16_T_DECLARED
#endif

#ifndef _INT32_T_DECLARED
typedef __int32_t int32_t;
#define _INT32_T_DECLARED
#endif
#ifndef _UINT32_T_DECLARED
typedef __uint32_t uint32_t;
#define _UINT32_T_DECLARED
#endif
#endif//WIN32



#ifndef errno_t
typedef int errno_t;
#endif

#define ERRNO_OK 0
#define ERRNO_FAIL -1
#define ERRNO_NO_SPACE -2
#define ERRNO_NOT_FOUND -3
#define ERRNO_NO_PERMISSION -4
#define ERRNO_INVALID_PARAMS -5

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#ifndef WIN32
#define strncpy_s(a, al, b, bl) strncpy(a, b, bl);
#endif//WIN32

typedef void (*GObjectDestroyFunc)(void* obj);

typedef struct _GObject {
    GObjectDestroyFunc destroy;
    void* obj;
}GObject;

am_static_inline GObject gobject_init(GObjectDestroyFunc destroy, void* obj) {
    GObject gobject;
    gobject.destroy = destroy;
    gobject.obj = obj;
    
    return gobject;
}

#define ELM_NR(a) (sizeof(a)/sizeof(a[0]))
#ifdef NDEBUG
    #define ENSURE(p) (void)(p)
#else
    #define ENSURE(p) assert(p)
#endif

#ifdef NDEBUG
//#	define return_if_fail(p) if(!(p)) {return;}
//#	define return_value_if_fail(p, value) if(!(p)) {return (value);}
# define return_if_fail(p)  ENSURE(p)
# define return_value_if_fail(p, value)  ENSURE(p)
#else
#define return_if_fail(p) if(!(p)) \
    {trace("%s:%s:%d conditions not meet\n", __FILE__, __FUNCTION__, __LINE__); return;}
#define return_value_if_fail(p, value) if(!(p)) \
    {trace("%s:%s:%d conditions not meet\n", __FILE__, __FUNCTION__, __LINE__); return (value);}
#endif

#endif//TYPES_DEF_H 
