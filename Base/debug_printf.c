#include "debug_printf.h"

#include <stdarg.h>

__attribute__((weak)) void print_to_serial(const char *str)
{
}

__attribute__((weak)) void log_to_serial(const char *str)
{
}
