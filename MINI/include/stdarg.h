/* File: stdarg.h */
#ifndef _STDARG_H_
#define _STDARG_H_

#ifndef __va_list__
#define __va_list__
typedef char *va_list;
#endif

#define va_start(ap, parmN) ap = (va_list) ((char *)&parmN + sizeof (parmN))
#define va_arg(ap, type) ((type *)(ap += sizeof (type)))[-1]
#define va_end(ap)

#endif
