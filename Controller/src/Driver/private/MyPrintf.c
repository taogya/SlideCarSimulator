#include "MyPrintf.h"
#include <stdio.h>
#include <stdarg.h>


void MyPrintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}