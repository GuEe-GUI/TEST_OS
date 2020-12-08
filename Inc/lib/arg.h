/*
    File:       arg.h

    Contains:   Provide indefinite arguments interface

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __LIB_ARG__
#define __LIB_ARG__

#ifndef __va_list__
#define __va_list__
typedef char *va_list;
#endif

#define va_start(ap, parmN) ap = (va_list) ((char *)&parmN + sizeof (parmN))
#define va_arg(ap, type) ((type *)(ap += sizeof (type)))[-1]
#define va_end(ap)

#endif
