#include <stdarg.h>
#include <stdio.h>

#include "sh.h"

void log_mess(struct sh *sh, const char *format, ...)
{
    if (!sh->log)
        return;

    va_list ap;
    va_start(ap, format);

    vprintf(format, ap);

    va_end(ap);
}

void log_messp(bool print, const char *format, ...)
{
    if (!print)
        return;

    va_list ap;
    va_start(ap, format);

    vprintf(format, ap);

    va_end(ap);
}
