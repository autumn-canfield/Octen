#ifndef _ERROR_H
#define _ERROR_H

#include<stdlib.h>
#include<stdarg.h>
#include<stdio.h>

static inline void error(const char *fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   vprintf(fmt, args);
   exit(1);
}

#endif // _ERROR_H

