#ifndef LOG_H
#define LOG_H

#include <stdbool.h>

#include "parser.h"
#include "struct.h"

// log.c

void log_mess(struct sh *sh, const char *format, ...);
void log_messp(bool print, const char *format, ...);

#endif /* !LOG_H */
