#ifndef LOWFETCH_H_
#define LOWFETCH_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum ColorChars
{
    CHAR_WHITE = 'w',
    CHAR_RED = 'r',
    CHAR_GREEN = 'g',
    CHAR_YELLOW = 'y',
    CHAR_BLUE = 'b',
    CHAR_MAGENTA = 'm',
    CHAR_CYAN = 'c'
};

struct SystemInfo;

bool arg_parse(const char *argdef[], const char *string);
char *file_read(const char *filename, size_t size); /* read from a file, requires freeing the assigned variable later */

char *get_ansi_color_from(enum ColorChars color, bool bold);
char *get_ascii(bool file_mode, const char *filename, size_t size);
char *get_distro_id(size_t size);
char *get_uptime(size_t size);
char *get_kernel_version(size_t size);

int info_print(enum ColorChars accent_color_char, bool accent_bold, struct SystemInfo system_info);

#endif
