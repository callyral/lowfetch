#ifndef LOWFETCH_H_
#define LOWFETCH_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct SystemInfo;

bool arg_parse(const char *argdef[], const char *string);
void help_menu_print(const char **argdefs_list[]);

char *file_read(const char *filename, size_t size); // read from a file, requires freeing the assigned variable later

char *get_ansi_color_from(char color_char, bool bold);
char *get_ascii(bool use_file, const char *filename, size_t size);
char *get_distro_id(size_t size);
char *get_uptime(size_t size);
char *get_kernel_version(size_t size);

int info_print(char accent_color_char, bool accent_bold, bool use_order_file, char *order_filename, size_t order_file_size, struct SystemInfo system_info);

#endif
