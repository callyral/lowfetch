#ifndef LOWFETCH_BASE_H_
#define LOWFETCH_BASE_H_
#include <stdlib.h>
#include <stdbool.h>

char *file_read(const char *filename, size_t size);
char *get_ansi_color_from(char color_char, bool bold);

#endif
