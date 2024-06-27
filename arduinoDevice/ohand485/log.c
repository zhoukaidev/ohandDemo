

#include "log.h"

#include <stdio.h>
#include <stdarg.h>


const char LOG_LEVEL_ID[] = {'D', 'I', 'W', 'E', 'F'};

static LOG_LEVEL _log_level = LOG_LVL_INFO;


LOG_LEVEL log_get_level(void)
{
  return _log_level;
}


void log_set_level(LOG_LEVEL level)
{
  _log_level = level;
}


void log_msg(LOG_LEVEL level, const char* format, ...)
{
  va_list arglist;

  if (_log_level <= level)
  {
    printf("[%c] ", LOG_LEVEL_ID[level]);
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
  }
}


// void log_msg(LOG_LEVEL level, ...)
// {
//  if (_log_level <= level)
//  {
//    fprintf(stderr, "[%c] ", LOG_LEVEL_ID[_log_level]);
//    fprintf(stderr, __VA_ARGS__);
//  }
// }
