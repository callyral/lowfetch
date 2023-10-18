#ifndef ASCII_H_
#define ASCII_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

enum ColorMode;
enum FileMode;

struct SystemInfo;

char *file_read(const char *filename, size_t size); /* read from a file, requires freeing the assigned variable later */

char *get_ascii(enum FileMode file_mode, const char *filename, size_t size);
char *get_distro_id(size_t size);
char *get_uptime(size_t size);
char *get_kernel_version(size_t size);

int info_print(enum ColorMode color_mode, struct SystemInfo system_info);

#endif
