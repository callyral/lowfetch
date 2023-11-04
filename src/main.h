#ifndef LOWFETCH_H_
#define LOWFETCH_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct SystemInfo;

char *get_ascii(bool use_file, const char *filename, size_t size);
char *get_xdg_desktop();

void info_print(char accent_color_char, bool accent_bold, bool use_order_file, char *order_filename, size_t order_file_size, struct SystemInfo system_info);

#endif
