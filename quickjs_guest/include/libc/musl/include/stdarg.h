#ifndef _STDARG_H
#define _STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER

typedef char* va_list;
#define __crt_va_start(ap, x) __crt_va_start_a(ap, x)

void __cdecl __va_start(va_list*, ...);

#define __crt_va_start_a(ap, x) ((void)(__va_start(&ap, x)))
#define __crt_va_arg(ap, t)                                               \
        ((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
            ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))             \
            :  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))

#define __crt_va_end(ap) ((void)(ap = (va_list)0))
    
#define va_start __crt_va_start
#define va_arg   __crt_va_arg
#define va_end   __crt_va_end
#define va_copy(destination, source) ((destination) = (source))
    
#else

#define __NEED_va_list

#include <bits/alltypes.h>

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)
#endif

#ifdef __cplusplus
}
#endif

#endif